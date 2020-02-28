#ifndef PWX_LIBPWX_PWX_BASE_VHASH_H_INCLUDED
#define PWX_LIBPWX_PWX_BASE_VHASH_H_INCLUDED 1
#pragma once

/** @file VTHashBase.h
  *
  * @brief Virtual base class for hash containers
  *
  * This file consists of a base class template for the hash containers.
  * It is not intended to be used singularly and is therefore virtual.
  * The common hash template does not include any collision resolving,
  * this is implemented in the respective hash table container.
  *
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  * History and change log are maintained in pwxlib.h
**/


#include <cassert>
#include <cmath>
#include <thread>


#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"

#include "container/CHashBuilder.h"
#include "container/THashElement.h"
#include "container/VContainer.h"


/// @namespace pwx
namespace pwx {


/// @brief Support macro to start growing a hashtable only when it is save to do so
#define HASH_START_GROW \
	growing.fetch_add(1, memOrdStore); \
	PWX_LOCK_GUARD(this); \
	while ( removing.load(memOrdLoad) || inserting.load(memOrdLoad) || clearing.load(memOrdLoad) ) { \
		PWX_LOCK_GUARD_RESET(this); \
	}


/// @brief Support macro to stop growing a hastable
#define HASH_STOP_GROW \
	growing.fetch_sub(1, memOrdStore); \
	PWX_LOCK_GUARD_CLEAR()


/// @brief Support macro to start inserting an element only when it is save to do so
#define HASH_START_INSERT \
	PWX_LOCK_GUARD(this); \
	while ( growing.load(memOrdLoad) || clearing.load(memOrdLoad) ) { \
		PWX_LOCK_GUARD_RESET(this); \
	} \
	inserting.fetch_add(1, memOrdStore)


/// @brief Support macro to stop inserting an element
#define HASH_STOP_INSERT \
	inserting.fetch_sub(1, memOrdStore); \
	PWX_LOCK_GUARD_CLEAR()


/// @brief Support macro to start removing an element only when it is save to do so
#define HASH_START_REMOVE \
	PWX_LOCK_GUARD(this); \
	if (this->isDestroyed.load()) return 0; \
	while ( growing.load(memOrdLoad) || clearing.load(memOrdLoad) ) { \
		PWX_LOCK_GUARD_RESET(this); \
	}  \
	removing.fetch_add(1, memOrdStore)


/// @brief Support macro to stop removing an element
#define HASH_STOP_REMOVE \
	removing.fetch_sub(1, memOrdStore); \
	PWX_LOCK_GUARD_CLEAR()


/// @brief Support macro to wait for possible growing actions to finish
#define HASH_WAIT_FOR_CLEAR_AND_GROW \
	PWX_LOCK_GUARD(this); \
	while ( growing.load(memOrdLoad) || clearing.load(memOrdLoad) ) { \
		PWX_LOCK_GUARD_RESET(this); \
	} \
	PWX_LOCK_GUARD_CLEAR()


/// @brief Support macro to have the clear method wait until all actions ceased
#define HASH_START_CLEAR \
	PWX_LOCK_GUARD(this); \
	while ( removing.load(memOrdLoad) ||  growing.load(memOrdLoad) ||  inserting.load(memOrdLoad) ) { \
		PWX_LOCK_GUARD_RESET(this); \
	} \
	clearing.fetch_add(1, memOrdStore); \
	PWX_LOCK_GUARD_CLEAR()


/// @brief Support macro to notifiy other threads that the container is cleared
#define HASH_STOP_CLEAR \
	clearing.fetch_sub(1, memOrdStore)


/** @brief two-type enum determining the hashing type
  * This allows the chained hash to use dynamic hashing types for its
  * basic hash->idx function, while the open addressed hash uses
  * this to determine what to use for the secondary hashing.
**/
enum eChainHashMethod {
	CHM_Division = 1,  //!< Use the division method
	CHM_Multiplication //!< Use the multiplication method
};


/** @class VTHashBase
  *
  * @brief Virtual base class for hash containers
  *
  * There are two basic hash table containers, TChainHash
  * and TOpenHash. The difference is the way the hash tables
  * order their data and resolve collisions. While the chained
  * hash table TChainHash uses buckets, the open hash table
  * TOpenHash uses double hash probing.
  *
  * However, most operations are the same once the place for
  * an element evaluated out of its key is known. These
  * common operations are done in this base class, which
  * call pure virtual private methods, that are then defined
  * in TChainHash and TOpenHash to provide the proper
  * collision resolving.
**/
template<typename key_t, typename data_t, typename elem_t = THashElement<key_t, data_t> >
class VTHashBase : public VContainer {
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef VContainer                          base_t; //!< Base type of the hash
	typedef VTHashBase<key_t, data_t, elem_t>   hash_t; //!< Type of this hash


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default destructor takes a length for the initial size and
	  * a length for the key. Further it initializes the hash table.
	  *
	  * The key length is only needed if you use C-String keys without 0-byte delimiter
	  * or if you are using C-Strings or std::string keys that can be so long that you
	  * want to limit the length of the key itself and ignore further characters.
	  *
	  * To set any of the user methods, one of the specialized constructors
	  * can be used.
	  *
	  * @param[in] initSize Initial size of the hash table.
	  * @param[in] keyLen_ Length of the key to limit hash generation.
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	VTHashBase( uint32_t initSize, uint32_t keyLen_,
	            double maxLoad_, double dynGrow_ )
		: hashBuilder  ( keyLen_ )
		, hashSize     ( initSize )
		, dynGrowFactor( dynGrow_ )
		, maxLoadFactor( maxLoad_ )
		, hashTable    ( nullptr )
		, hashTableLock()
		, vacChar      ( nullptr )
		, vacated      ( nullptr ) {

		// Generate the hash table
		try {
			hashTable = new elem_t* [hashSize];
			for ( size_t i = 0; i < hashSize; ++i )
				// We can do this directly in a ctor. No other thread here, yet
				hashTable[i] = nullptr;
			/* Why do we not set this to 'vacated'?
			 * If an open hash element needs more than one hop, and an element that caused
			 * one of the hops gets removed, it's bucket is marked as being 'vacated'. This is
			 * done so the search for an element can jump 'vacated' buckets, but stops if it
			 * hits a `nullptr` bucket. If we initialized every bucket with 'vacated', the
			 * search for an element would endlessly circle the table as it never knows where to
			 * stop.
			 */
		}
		PWX_THROW_STD_FURTHER( "VTHashBase failure", "HashTable could not be created" );

		// Generate the vacated pointer:
		try {
			vacChar = new char;
			vacated = reinterpret_cast<elem_t*>( vacChar );
		}
		PWX_THROW_STD_FURTHER( "VTHashBase failure", "Vacated pointer could not be created" );
	}


	/** @brief full constructor with key length
	  *
	  * The full constructor initializes an empty hash with user defined delete
	  * method, hashing method and key length. The initial size is the @a initSize
	  *
	  * @param[in] initSize The initial size of the table.
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash_ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
	  * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	VTHashBase( uint32_t initSize,
	            void ( *destroy_ ) ( data_t* data ),
	            uint32_t ( *hash_ ) ( const key_t* key, uint32_t keyLen ),
	            uint32_t keyLen_,
	            double maxLoad_, double dynGrow_ ) noexcept
		: hash_t( initSize, keyLen_, maxLoad_, dynGrow_ ) {

		destroy = destroy_;
		hash_limited = hash_;
	}


	/** @brief full constructor without key length
	  *
	  * The full constructor initializes an empty hash with user defined delete
	  * method and hashing method withour key length. The initial size is the
	  * @a initSize
	  *
	  * @param[in] initSize The initial size of the table.
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash_ A pointer to a function that can hash the keys that are stored
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	VTHashBase( uint32_t initSize,
	            void ( *destroy_ ) ( data_t* data ),
	            uint32_t ( *hash_ ) ( const key_t* key ),
	            double maxLoad_, double dynGrow_ )
		: hash_t( initSize, ( uint32_t )0, maxLoad_, dynGrow_ ) {

		destroy = destroy_;
		hash_user = hash_;
	}


	/** @brief limiting user method constructor
	  *
	  * This constructor only takes a destroy method and a hash
	  * method with explicit key length.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash_ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
	  * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	VTHashBase( void ( *destroy_ ) ( data_t* data ),
	            uint32_t ( *hash_ ) ( const key_t* key, uint32_t keyLen ),
	            uint32_t keyLen_,
	            double maxLoad_, double dynGrow_ ) noexcept
		: hash_t( ( uint32_t )100, keyLen_, maxLoad_, dynGrow_ ) {

		destroy = destroy_;
		hash_limited = hash_;
	}


	/** @brief user method constructor
	  *
	  * This constructor only takes a destroy method and a hash
	  * method without explicit key length.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash_ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	VTHashBase( void ( *destroy_ ) ( data_t* data ),
	            uint32_t ( *hash_ ) ( const key_t* key ),
	            double maxLoad_, double dynGrow_ ) noexcept
		: hash_t( ( uint32_t )100, ( uint32_t )0, maxLoad_, dynGrow_ ) {

		destroy = destroy_;
		hash_user = hash_;
	}


	/** @brief destroy method constructor
	  *
	  * This constructor only takes a destroy method.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	VTHashBase( void ( *destroy_ ) ( data_t* data ),
	            double maxLoad_, double dynGrow_ ) noexcept
		: hash_t( ( uint32_t )100, ( uint32_t )0, maxLoad_, dynGrow_ ) {

		destroy = destroy_;
	}


	/** @brief key length constructor
	  *
	  * This constructor only takes a key length and sets the destroy
	  * and hash methods to nullptr.
	  *
	  * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	VTHashBase( uint32_t keyLen_,
	            double maxLoad_, double dynGrow_ ) noexcept
		: hash_t ( ( uint32_t )100, keyLen_, maxLoad_, dynGrow_ )
	{ }


	/** @brief pseudo empty constructor
	  *
	  * The pseudo empty constructor uses the default constructor to
	  * set the data destroy method and the hash method to the null
	  * pointer with full key usage.
	  *
	  * However, because of the very different needs of chained
	  * versus open hash tables, both the maximum load factor
	  * and the dynamic growth rate must be set. A true empty
	  * constructor is not possible.
	  *
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	VTHashBase( double maxLoad_, double dynGrow_ ) noexcept
		: hash_t ( ( uint32_t )100, ( uint32_t )0, maxLoad_, dynGrow_ )
	{ }


	/// Empty ctor is not available
	VTHashBase() PWX_DELETE;


	/** @brief copy constructor
	  *
	  * Prepares a copy of the elements of @a src.
	  *
	  * If the memory setup fails, `std::exception` is caught and thrown further
	  * as a `pwx::CException` with the name `VTHashBaseCopyFailure`.
	  *
	  * Note: This is only a base class preparation, the actual copying of the
	  * elements must be done in the derived hash containers.
	  *
	  * @param[in] src reference of the hash to copy.
	**/
	VTHashBase( const hash_t& src )
		: base_t ( src )
		, destroy( src.destroy )
		, hash_user( src.hash_user )
		, hash_limited( src.hash_limited )
		, hashBuilder ( src.hashBuilder.getKeyLen() )
		, hashSize ( 0 ) // Set after making sure src isn't growing
		, hashTable ( nullptr )
		, maxLoadFactor( src.maxLoadFactor )
		, dynGrowFactor( src.dynGrowFactor ) {

		PWX_LOCK_GUARD( &src );
		while ( src.growing.load( memOrdLoad ) > 0 ) {
			PWX_LOCK_GUARD_RESET( &src );
		}

		// Could have changed after a grow:
		hashSize = src.hashSize;

		// Generate the hash table
		try {
			hashTable = new elem_t* [hashSize];
			for ( uint32_t i = 0; i < hashSize; ++i )
				// We can do this directly in a copy ctor. No other thread here, yet
				hashTable[i] = nullptr;
			/* Why do we not set this to 'vacated'? See main ctor why. */
		}
		PWX_THROW_STD_FURTHER( "VTHashBaseCopyFailure", "HashTable could not be created" );

		// Generate the vacated pointer:
		try {
			vacChar = new char;
			vacated = reinterpret_cast<elem_t*>( vacChar );
		}
		PWX_THROW_STD_FURTHER( "VTHashBaseCopyFailure", "Vacated pointer could not be created" );
	}


	virtual ~VTHashBase() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/


	/** @brief add an element to the hash
	  *
	  * this method copies the element @a src into the hash
	  * table if the key of the element can not be found, yet.
	  *
	  * @param[in] src reference of the element to copy
	  * @return the resulting number of stored elements
	**/
	virtual uint32_t add( const elem_t& src ) {
		// Use double search to only lock if the key is
		// not found in the first run
		if ( !this->privGet( src.key ) ) {

			HASH_START_INSERT;

			if ( !this->privGet( src.key ) ) {
				double newSize = 0.;
				PWX_TRY_PWX_FURTHER( newSize = privAdd( src ) )
				HASH_STOP_INSERT;

				// Grow if needed
				double maxSize = sizeMax();
				if ( ( newSize / maxSize ) > maxLoadFactor ) {
					PWX_TRY_PWX_FURTHER(
					        grow( static_cast<uint32_t>( maxSize * dynGrowFactor ) ) )
				}

			} else
				HASH_STOP_INSERT;
		}

		return this->size();
	}


	/** @brief add a key-data-pair to the hash
	  *
	  * this method adds a new element to the hash if @a key
	  * is not present, yet.
	  *
	  * @param[in] key the key of the new element
	  * @param[in] data pointer to the data of the new element
	  * @return the resulting number of stored elements
	**/
	virtual uint32_t add( const key_t& key, data_t* data ) {
		// Use double search to only lock if the key is
		// not found in the first run
		if ( !this->privGet( key ) ) {

			HASH_START_INSERT;

			if ( !this->privGet( key ) ) {
				double newSize = 0.;
				PWX_TRY_PWX_FURTHER( newSize = privAdd( key, data ) )
				HASH_STOP_INSERT;

				// Grow if needed
				double maxSize = sizeMax();
				if ( ( newSize / maxSize ) > maxLoadFactor ) {
					PWX_TRY_PWX_FURTHER(
					        grow( static_cast<uint32_t>( maxSize * dynGrowFactor ) ) )
				}

			} else
				HASH_STOP_INSERT;
		}

		return this->size();
	}


	/** @brief delete all elements
	  *
	  * This is a quick way to get rid of all elements
	  * at once. If a destroy() function was set, it is
	  * used for the data deletion. Otherwise it is
	  * assumed that data_t responds to the delete
	  * operator.
	**/
	virtual void clear() noexcept {
		/* IMPORTANT: this method MUST NOT call other public
		 * methods, or it will freeze waiting for itself to stop!
		 */
		elem_t*   toDel   = nullptr;
		elem_t*   delNext = nullptr;
		uint32_t  pos     = 0;
		uint32_t  tabSize = sizeMax();

		if ( !hashTable ) return; // Should never happen

		HASH_START_CLEAR;

		while ( ( eCount.load( PWX_MEMORDER_RELAXED ) > 0 ) && hashTable ) {
			// We have to use a temporary "grand lock" for cases where multiple threads
			// call this method. Otherwise, no matter what we do, the destruction of elements
			// will cause data races.
			PWX_DOUBLE_LOCK_GUARD( this, &hashTableLock );
			toDel = hashTable[ pos ];
			if ( toDel && (toDel != vacated) && !toDel->destroyed() && toDel->try_lock() ) {
				// This thread has exclusive access, but it
				// must be ensured that no other thread is
				// in the destructor unlock cycle:
				if ( toDel->destroyed() ) {
					toDel->unlock();
					continue;
				}

				hashTable[ pos ] = nullptr;
				PWX_DOUBLE_LOCK_GUARD_CLEAR(); // exclusive enough, now.

				// remove a possible chain:
				while ( ( delNext = toDel->removeNext() )
				                && !delNext->destroyed()
				                && delNext != toDel ) {
					--eCount;
					delete delNext;
					// Note: The destroy() method is used in the delete operator, not here!
				}

				// delete this element
				if ( !toDel->destroyed() ) {
					--eCount;
					delete toDel;
				}
			};

			// Advance and wrap:
			if ( ++pos >= tabSize )
				pos = 0;
		} // end of looping the hash table

		HASH_STOP_CLEAR;
	}


	/** @brief delete the element @a elem
	  *
	  * If the hash table does not contain this element, nothing happens.
	  *
	  * To only remove the element from the hash for further
	  * usage, use `remElem(elem)` instead.
	  *
	  * If the deletion of the element throws an exception, it will be turned
	  * into a `pwx::CException` and passed on.
	  *
	  * **Warning**: The element @a elem is invalid after this operation!
	  *
	  * @param elem reference to the element to delete
	  * @return The number of elements after the operation.
	**/
	virtual uint32_t delElem( elem_t& elem ) {
		HASH_START_REMOVE;

		uint32_t remaining = 0;
		elem_t*  toDelete  = privRemoveKey( elem.key );

		PWX_TRY_PWX_FURTHER( remaining = protDelete( toDelete ) );

		HASH_STOP_REMOVE;

		return remaining;
	}


	/** @brief delete the element with the key @a key
	  *
	  * If the hash table does not contain an element with the key
	  * @a key, nothing happens.
	  *
	  * To only remove the element from the hash for further
	  * usage, use `remKey(key)` instead.
	  *
	  * If the deletion of the element throws an exception, it will be turned
	  * into a `pwx::CException` and passed on.
	  *
	  * @param key reference to the key to search for
	  * @return The number of elements after the operation.
	**/
	virtual uint32_t delKey( const key_t& key ) {
		HASH_START_REMOVE;

		uint32_t remaining = 0;
		elem_t*  toDelete  = privRemoveKey( key );

		PWX_TRY_PWX_FURTHER( remaining = protDelete( toDelete ) );

		HASH_STOP_REMOVE;

		return remaining;
	}


	/** @brief disable thread safety
	  *
	  * This method disables all thread safety measures.
	  *
	  * **Warning**: It is completely unchecked whether the
	  * container is used by more than one thread. If concurrent
	  * threads work with this container while this method is
	  * called, the outcome is unpredictable.<BR/>
	  * Further this method disables all locking mechanisms in
	  * all elements stored and all elements that are added
	  * after calling this method. Calling this method with a
	  * lot of elements stored is therefore rather costly!
	  */
	virtual void disable_thread_safety() noexcept {
		HASH_WAIT_FOR_CLEAR_AND_GROW;

		// Turn off first
		this->beThreadSafe( false );

		// Now reset the guard, so others waiting for
		// the lock can finish their business first
		PWX_LOCK_GUARD_RESET( this );

		uint32_t  pos       = 0;
		uint32_t  tabSize   = this->sizeMax();
		elem_t*   xCurr     = nullptr;
		while ( pos < tabSize ) {
			xCurr = table_get( pos );
			if ( xCurr ) {
				while ( xCurr ) {
					xCurr->disable_thread_safety();
					xCurr = xCurr->getNext();
				}
			}
			++pos;
		}
	}


	/// @brief return true if this container is empty
	virtual bool empty() const noexcept {
		HASH_WAIT_FOR_CLEAR_AND_GROW;
		return !eCount.load( memOrdLoad );
	}


	/** @brief enable thread safety
	  *
	  * This method enables all thread safety measures.
	  *
	  * **Warning**: This method enables all locking
	  * mechanisms in all elements stored and all elements
	  * that are added after calling this method. Calling
	  * this method with a lot of elements stored is
	  * therefore rather costly!
	  */
	virtual void enable_thread_safety() noexcept {
		this->beThreadSafe( true );

		HASH_WAIT_FOR_CLEAR_AND_GROW;

		uint32_t  pos       = 0;
		uint32_t  tabSize   = this->sizeMax();
		elem_t*   xCurr     = nullptr;
		while ( pos < tabSize ) {
			xCurr = table_get( pos );
			if ( xCurr ) {
				while ( xCurr ) {
					xCurr->enable_thread_safety();
					xCurr = xCurr->getNext();
				}
			}
			++pos;
		}
	}


	/// @brief return true if an element with @a key exists
	virtual bool exists( const key_t& key ) const noexcept {
		HASH_WAIT_FOR_CLEAR_AND_GROW;
		return this->privGet( key ) ? true : false;
	}


	/** @brief returns a read-only pointer to the element with the key @a key
	  *
	  * @param[in] key the key to search for
	  * @return a const pointer to the element or nullptr if the key could not be found.
	**/
	virtual elem_t* get( const key_t& key ) const noexcept {
		HASH_WAIT_FOR_CLEAR_AND_GROW;
		return this->privGet( key );
	}


	/** @brief returns a read/write pointer to the element with the key @a key
	  *
	  * @param[in] key the key to search for
	  * @return a pointer to the element or nullptr if the key could not be found.
	**/
	virtual elem_t* get( const key_t& key ) noexcept {
		HASH_WAIT_FOR_CLEAR_AND_GROW;
		return const_cast<elem_t*>( this->privGet( key ) );
	}


	/** @brief returns a read-only reference to the stored data with key @a key
	  *
	  * If the data pointer is nullptr, a pwx::CException with the name
	  * "NullDataException" is thrown.
	  *
	  * @param[in] key the key to search for
	  * @return a read/write reference to the stored data.
	**/
	virtual data_t& getData( const key_t& key ) const {
		PWX_TRY_PWX_FURTHER( return **( get( key ) ) );
	}


	/** @brief returns a read/write reference to the stored data with key @a key
	  *
	  * If the data pointer is nullptr, a pwx::CException with the name
	  * "NullDataException" is thrown.
	  *
	  * @param[in] key the key to search for
	  * @return a read/write reference to the stored data.
	**/
	virtual data_t& getData( const key_t& key ) {
		PWX_TRY_PWX_FURTHER( return **( get( key ) ) );
	}


	/** @brief return the number of "hops" needed when the element was inserted
	  *
	  * This method returns the number of hops (or collisions) that
	  * were needed when inserting the element.
	  *
	  * If the element is nowhere inserted, it returns zero.
	  *
	  * @param[in] key The key to search for
	  * @return number of hops needed.
	**/
	virtual uint32_t getHops( const key_t& key ) const noexcept {
		HASH_WAIT_FOR_CLEAR_AND_GROW;
		elem_t* elem = privGet( key );
		if ( elem )
			return elem->hops;
		return 0;
	}

	/** @brief grow the size of a hash table
	  *
	  * This method increases the hash table by creating a new
	  * table and moving all elements into the new one.
	  *
	  * This method does not shrink a table. It does nothing if
	  * @a targetSize is not larger than the current size.
	  * Therefore the resulting size is returned for you to check.
	  *
	  * @param[in] targetSize the new size of the hash.
	  * @return the resulting size
	**/
	virtual uint32_t grow( uint32_t targetSize ) {
		/* IMPORTANT: this method MUST NOT call other public
		 * methods, or it will freeze waiting for itself to stop!
		 */
		if ( targetSize > sizeMax() ) {

			HASH_START_GROW;

			// --- store old size ---
			uint32_t oldSize = hashSize.load( memOrdLoad );

			if ( targetSize > oldSize ) {
				DEBUG_LOG( "Hash Grow", "Growing hash table from %u top %u", oldSize, targetSize );

				// --- Create a new larger table ---
				PWX_NAMED_LOCK_GUARD( table_lock, &hashTableLock );
				elem_t** oldTab = hashTable;
				try {
					hashTable = new elem_t* [targetSize];

					// --- reset eCount, it will be restored by the move ---
					eCount.store( 0, memOrdStore );

					// --- "nullify" the new table ---
					for ( uint32_t i = 0; i < targetSize; ++i )
						table_set( i, nullptr );
				}
				PWX_THROW_STD_FURTHER( "GrowFailure", "Larger HashTable could not be created" );

				// --- Determine new hashing method ---
				privSetHashMethod( targetSize );

				// --- Set new size ---
				hashSize.store( targetSize, memOrdStore );

				// --- Copy all elements ---
				elem_t*   toMove    = nullptr;
				elem_t*   xNext     = nullptr;
				uint32_t  pos       = 0;

				while ( pos < oldSize ) {
					// Note: hashTableLock is still locked, so we can directly edit the tables
					while ( oldTab[pos] && ( oldTab[pos] != vacated ) ) {
						toMove = oldTab[pos];
						xNext  = toMove->getNext();

						// Add the old element to the new table and delete the old one:
						PWX_TRY_PWX_FURTHER( privAdd( toMove->key, toMove->data.get() ) );

						// Now maintain the old table:
						oldTab[pos] = xNext != toMove ? xNext : nullptr;

						// And delete the old element
						if ( toMove && !toMove->destroyed() ) {
							PWX_TRY_STD_FURTHER( delete toMove, "delete_error", "deleting an element failed" );
						}
					}
					++pos;
				}

				// --- Delete old table ---
				PWX_TRY_STD_FURTHER( delete [] oldTab, "Delete failed",
				                     "Deleting the old table after growing into a new table failed." )
			} // End of inner size check

			// --- Growing is finished: ---
			HASH_STOP_GROW;

		} // End of outer size check

		return sizeMax();
	}


	/** @brief short alias for pop_back()
	  *
	  * This is a convenience function that removes the last element
	  * found in the table. As it has to traverse the table, this
	  * operation can be costly for large tables that only have elements
	  * somewhere at the beginning.
	  *
	  * You have to delete the removed element by yourself.
	  *
	  * If the hash table is empty, nullptr is returned.
	  *
	  * @return a pointer to the removed element or nullptr if the hash is empty
	**/
	virtual elem_t* pop() noexcept {
		return this->pop_back();
	}


	/** @brief alias to remove the last element (tail)
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delNext instead.
	  *
	  * If the list is empty, nullptr is returned.
	  *
	  * @return a pointer to the removed element or nullptr if the list is empty
	**/
	virtual elem_t* pop_back() noexcept {
		if ( eCount.load( memOrdLoad ) > 0 ) {

			HASH_START_REMOVE;

			uint32_t maxPos = sizeMax();
			uint32_t pos    = maxPos;
			elem_t*  elem   = nullptr;

			while ( pos && ( eCount.load( PWX_MEMORDER_RELAXED ) > 0 ) ) {
				elem = table_get( --pos );
				if ( elem && elem->inserted() && !elem->destroyed() ) {
					PWX_NAMED_LOCK_GUARD( ElemRemover, elem );
					if ( elem->inserted() && !elem->destroyed() )
						return privRemoveIdx( pos );
				}
			}

			HASH_STOP_REMOVE;
		}

		return nullptr;
	}


	/** @brief alias to remove the first element (head)
	  *
	  * This is a convenience function that removes the first element
	  * found in the table. As it has to traverse the table, this
	  * operation can be costly for large tables that only have elements
	  * somewhere at the end.
	  *
	  * You have to delete the removed element by yourself.
	  *
	  * If the hash table is empty, nullptr is returned.
	  *
	  * @return a pointer to the removed element or nullptr if the hash is empty
	**/
	virtual elem_t* pop_front() noexcept {
		if ( eCount.load( memOrdLoad ) > 0 ) {

			HASH_START_REMOVE;

			uint32_t maxPos = sizeMax();
			uint32_t pos    = 0;
			elem_t*  elem   = nullptr;

			while ( ( pos < maxPos ) && ( eCount.load( PWX_MEMORDER_RELAXED ) > 0 ) ) {
				elem = table_get( pos++ );
				if ( elem && elem->inserted() && !elem->destroyed() ) {
					PWX_NAMED_LOCK_GUARD( ElemRemover, elem );
					if ( elem->inserted() && !elem->destroyed() )
						return privRemoveIdx( pos );
				}
			}

			HASH_STOP_REMOVE;
		}

		return nullptr;
	}


	/// @brief simple wrapper to add() to be conformant with the other container types
	virtual uint32_t push ( const key_t& key, data_t* data ) {
		PWX_TRY_PWX_FURTHER( return this->add( key, data ) )
	}


	/// @brief simple wrapper to add() to be conformant with the other container types
	virtual uint32_t push ( const elem_t& src ) {
		PWX_TRY_PWX_FURTHER( return this->add( src ) )
	}


	/// @brief simple wrapper to add() to be conformant with the other container types
	virtual uint32_t push_back ( const key_t& key, data_t* data ) {
		PWX_TRY_PWX_FURTHER( return this->add( key, data ) )
	}


	/// @brief simple wrapper to add() to be conformant with the other container types
	virtual uint32_t push_back ( const elem_t& src ) {
		PWX_TRY_PWX_FURTHER( return this->add( src ) )
	}


	/// @brief simple wrapper to add() to be conformant with the other container types
	virtual uint32_t push_front ( const key_t& key, data_t* data ) {
		PWX_TRY_PWX_FURTHER( return this->add( key, data ) )
	}


	/// @brief simple wrapper to add() to be conformant with the other container types
	virtual uint32_t push_front ( const elem_t& src ) {
		PWX_TRY_PWX_FURTHER( return this->add( src ) )
	}


	/** @brief remove the element with the key @a key
	  *
	  * If the hash table does not contain an element with the key
	  * @a key, nullptr is returned.
	  *
	  * If you remove an element from the hash, you are responsible
	  * to delete the element yourself. If you do not need to use
	  * the element, use delElem(key) instead.
	  *
	  * @param elem reference of the element to search for
	  * @return the given pointer or nullptr if the element does not exist
	**/
	virtual elem_t* remElem( elem_t& elem ) noexcept {
		HASH_START_REMOVE;
		elem_t* result = privRemoveKey( elem.key );
		HASH_STOP_REMOVE;
		return result;
	}


	/** @brief remove the element with the key @a key
	  *
	  * If the hash table does not contain an element with the key
	  * @a key, nullptr is returned.
	  *
	  * If you remove an element from the hash, you are responsible
	  * to delete the element yourself. If you do not need to use
	  * the element, use delKey(key) instead.
	  *
	  * @param key reference to the key to search for
	  * @return a pointer to the element with the key @a key or nullptr
	**/
	virtual elem_t* remKey( const key_t& key ) noexcept {
		HASH_START_REMOVE;
		elem_t* result = privRemoveKey( key );
		HASH_STOP_REMOVE;
		return result;
	}


	/// @brief simple wrapper to pop_front() to be conformant with other containers
	virtual elem_t* shift() noexcept {
		return this->pop_front();
	}


	/// @brief return the number of stored elements
	uint32_t size() const noexcept {
		HASH_WAIT_FOR_CLEAR_AND_GROW;
		return eCount.load( memOrdLoad );
	}


	/// @brief return the maximum number of places (elements for open, buckets for chained hashes)
	uint32_t sizeMax() const noexcept {
		HASH_WAIT_FOR_CLEAR_AND_GROW;
		return this->hashSize.load( memOrdLoad );
	}


	/// @brief simple wrapper to add() to be conformant with other containers
	virtual uint32_t unshift( const key_t& key, data_t* data ) {
		PWX_TRY_PWX_FURTHER( return this->add( key, data ) )
	}


	/// @brief simple wrapper to add() to be conformant with other containers
	virtual uint32_t unshift( const elem_t& src ) {
		PWX_TRY_PWX_FURTHER( return this->add( src ) )
	}


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief assignment operator
	  *
	  * Clears this hash and copies all elements from @a rhs
	  * into this hash. The destroy and hash methods are copied
	  * as well as the thread safety state
	  *
	  * @param[in] rhs reference of the hash to copy.
	  * @return reference to this.
	**/
	virtual hash_t& operator= ( const hash_t& rhs ) {
		if ( &rhs != this ) {
			HASH_WAIT_FOR_CLEAR_AND_GROW;
			PWX_DOUBLE_LOCK_GUARD( this, &rhs );
			clear();
			destroy      = rhs.destroy;
			hash_user    = rhs.hash_user;
			hash_limited = rhs.hash_limited;
			hashBuilder.setKeyLen( rhs.hashBuilder.getKeyLen() );

			uint32_t tgtSize = rhs.sizeMax();
			if ( sizeMax() < tgtSize ) {
				PWX_TRY_PWX_FURTHER( grow( this->grow( tgtSize ) ) )
			}

			beThreadSafe( rhs.beThreadSafe() );
			PWX_TRY_PWX_FURTHER ( operator+=( rhs ) )
		}
		return *this;
	}


	/** @brief addition assignment operator
	  *
	  * Add all elements from @a rhs to this hash.
	  *
	  * Warning: The table size is increased if @a rhs has a
	  * larger size, no matter whether the element count makes
	  * this neccessary or not!
	  *
	  * @param[in] rhs reference of the hash to add.
	  * @return reference to this.
	**/
	virtual hash_t& operator+= ( const hash_t& rhs ) {
		if ( &rhs != this ) {
			HASH_WAIT_FOR_CLEAR_AND_GROW;
			PWX_DOUBLE_LOCK_GUARD( this, &rhs );

			// --- grow this table if needed ---
			uint32_t rhsSize = rhs.sizeMax();
			uint32_t lhsSize = this->sizeMax();

			if ( rhsSize > lhsSize )
				PWX_TRY_PWX_FURTHER( this->grow( rhsSize ) );

			// --- copy all elements ---
			elem_t*  rhsCurr = nullptr;
			uint32_t rhsPos  = 0;
			bool     isTS    = this->beThreadSafe();

			while ( rhsPos < rhsSize ) {
				if ( !rhs.protIsUnused( rhsPos ) ) {
					rhsCurr = rhs.table_get( rhsPos );
					while ( rhsCurr ) {
						PWX_TRY_PWX_FURTHER( this->add( *rhsCurr ) )
						if ( !isTS )
							this->get( rhsCurr->key )->disable_thread_safety();
						rhsCurr = rhsCurr->getNext();
					}
				}
				++rhsPos;
			}
		}
		return *this;
	}


	/** @brief substraction assignment operator
	  *
	  * Remove all elements from @a rhs from this hash.
	  *
	  * @param[in] rhs reference of the hash to substract.
	  * @return reference to this.
	**/
	virtual hash_t& operator-= ( const hash_t& rhs ) {
		if ( &rhs != this ) {
			HASH_WAIT_FOR_CLEAR_AND_GROW;
			PWX_DOUBLE_LOCK_GUARD( this, &rhs );

			uint32_t rhsSize = rhs.sizeMax();
			elem_t* lhsCurr = nullptr;
			elem_t* rhsCurr = nullptr;
			uint32_t  rhsPos  = 0;

			while ( rhsPos < rhsSize ) {
				if ( !rhs.table_elem_equals( rhsPos, rhs.vacated ) ) {
					rhsCurr = rhs.hashTable[rhsPos];
					while ( rhsCurr ) {
						lhsCurr = remKey( rhsCurr->key );
						if ( lhsCurr ) {
							PWX_TRY_PWX_FURTHER( protDelete( lhsCurr ) )
						}
						rhsCurr = rhsCurr->getNext();
					}
				}
				++rhsPos;
			}
		} else {
			PWX_LOCK_GUARD( this );
			// Here we lock before calling clear, or the operator might
			// end up removing elements other threads just inserted.
			clear();
		}

		return *this;
	}


	/** @brief return a read-only pointer to the element with the given @a index
	  *
	  * This operator retrieves an element by index like an array. The pointer given
	  * back is read-only.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * If there is no element at the specific position of the hash table, the
	  * operator returns nullptr.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read-only pointer to the element, or nullptr if there is no element with the specific index.
	**/
	virtual const elem_t* operator[] ( const int64_t index ) const noexcept {
		HASH_WAIT_FOR_CLEAR_AND_GROW;
		return privGetByIndex( index );
	}


	/** @brief return a read/write pointer to the element with the given @a index
	  *
	  * This operator retrieves an element by index like an array. The pointer given
	  * back is write enabled, so use with care.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * If the hash is empty, the operator returns nullptr.
	  *
	  * If you use this operator to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read/write pointer to the element, or nullptr if the hash is empty.
	**/
	virtual elem_t* operator[] ( int64_t index ) noexcept {
		HASH_WAIT_FOR_CLEAR_AND_GROW;
		return const_cast<elem_t* > ( privGetByIndex( index ) );
	}


protected:

	/* ===============================================
	 * === Protected methods                       ===
	 * ===============================================
	*/

	/* Important:
	 * All protected and private methods must not lock the container!
	 * Container locking is done by the public methods!
	 * Further all protected and private
	 * methods must not use public methods, or they might end up
	 * freezing while waiting for themselves to end!
	 */

	/// @brief function pointer to an alternative destroy function to delete @a data
	void     ( *destroy )      ( data_t* data )                       = nullptr;

	/// @brief function pointer to an alternative function to hash a single @a key
	uint32_t ( *hash_user )    ( const key_t*  key )                  = nullptr;

	/// @brief function pointer to an alternative function to hash a single @a key with given @a keyLen
	uint32_t ( *hash_limited ) ( const key_t*  key, uint32_t keyLen ) = nullptr;


	/** @brief Delete the element @a removed
	  *
	  * <B>Important</B>: this method will throw "illegal_delete" if
	  * @a removed is actually not removed from the hash. Making sure
	  * this method is only called with a removed element allows to
	  * use it without a lock on the hash itself.
	**/
	virtual uint32_t protDelete( elem_t* removed ) {
		if ( removed && removed->inserted() )
			PWX_THROW( "illegal_delete", "delete non-removed element", "Deleting an element that is not removed is illegal!" );

		if ( removed ) {
			// Use double check technique to avoid illegal double deletion
			if ( !removed->destroyed() ) {
				PWX_LOCK( removed );
				if ( !removed->destroyed() ) {
					PWX_UNLOCK( removed );
					PWX_TRY( delete removed )
					PWX_THROW_PWXSTD_FURTHER ( "delete", "Deleting an element failed." );
				} else {
					PWX_UNLOCK( removed );
				}
			} // end of if not destroyed yet
		} // end of having an element to delete

		return eCount.load( memOrdLoad );
	}


	/** @brief use hashBuilder to generate a hash out of a key
	  * @param key pointer to the key to hash
	  * @return uint32_t hash of @a key
	**/
	virtual uint32_t protGetHash( const key_t* key ) const {
		return hashBuilder( key, hash_user, hash_limited );
	}


	/** @brief return true if the specified position is empty (aka `nullptr`)
	  *
	  * WARNING: This method does **NOT** check @a idx! Check it beforehand!
	  *
	  * @param[in] idx the index to check
	  * @return true if the position is empty, false otherwise.
	**/
	bool protIsEmpty( const uint32_t idx ) const noexcept {
		return table_elem_equals( idx, nullptr );
	}


	/** @brief return true if the specified position is empty or vacated
	  *
	  * WARNING: This method does **NOT** check @a idx! Check it beforehand!
	  *
	  * @param[in] idx the index to check
	  * @return true if the position is unused, false otherwise.
	**/
	bool protIsUnused( const uint32_t idx ) const noexcept {
		// Early exit if this was destroyed (Can be through some dtor like in CThreadElementStore)
		if ( this->isDestroyed.load() || ( nullptr == hashTable ) )
			return false;

		bool doLocking = beThreadSafe();

		if ( doLocking ) {
			// No need to debug log this, so we do not use PWX_LOCK_OBJ() here.
			hashTableLock.lock();
			if ( this->isDestroyed.load() || ( nullptr == hashTable ) ) {
				hashTableLock.unlock();
				return false;
			}
		}

		bool result = ( hashTable[idx] == nullptr ) || ( hashTable[idx] == vacated );

		if ( doLocking )
			hashTableLock.unlock();

		return result;
	}


	/** @brief return true if the specified position is vacated
	  *
	  * WARNING: This method does **NOT** check @a idx! Check it beforehand!
	  *
	  * @param[in] idx the index to check
	  * @return true if the position is marked as vacated, false otherwise.
	**/
	bool protIsVacated( const uint32_t idx ) const noexcept {
		return table_elem_equals( idx, vacated );
	}


	/** @brief return true if the data at index @a idx is @a data
	  *
	  * WARNING: The protected hashTable methods do **NOT** check @a idx! Check it beforehand!
	  *
	  * @param[in] idx  Index at which the data to compare
	  * @param[in] data The data to compare.
	  * @return return true if the data at index @a idx equals @a data
	**/
	bool table_data_equals( uint32_t idx, data_t const& data ) const noexcept {
		// Early exit if this was destroyed (Can be through some dtor like in CThreadElementStore)
		if ( this->isDestroyed.load() || ( nullptr == hashTable ) )
			return false;

		bool doLocking = beThreadSafe();

		if ( doLocking ) {
			// No need to debug log this, so we do not use PWX_LOCK_OBJ() here.
			hashTableLock.lock();
			if ( this->isDestroyed.load() || ( nullptr == hashTable ) ) {
				hashTableLock.unlock();
				return false;
			}
		}

		bool result = hashTable[idx] ? ( hashTable[idx] != vacated ? *( hashTable[idx] ) == data : false ) : false;

		if ( doLocking )
			hashTableLock.unlock();

		return result;
	}


	/** @brief return true if the element at index @a idx is @a elem
	  *
	  * WARNING: The protected hashTable methods do **NOT** check @a idx! Check it beforehand!
	  *
	  * @param[in] idx  Index at which the element to compare
	  * @param[in] elem Pointer to the element to compare. `vacated` is compared if this is `nullptr`.
	  * @return return true if the element at index @a idx is @a elem
	**/
	bool table_elem_equals( uint32_t idx, elem_t const* elem ) const noexcept {
		// Early exit if this was destroyed (Can be through some dtor like in CThreadElementStore)
		if ( this->isDestroyed.load() || ( nullptr == hashTable ) )
			return false;

		bool doLocking = beThreadSafe();

		if ( doLocking ) {
			// No need to debug log this, so we do not use PWX_LOCK_OBJ() here.
			hashTableLock.lock();
			if ( this->isDestroyed.load() || ( nullptr == hashTable ) ) {
				hashTableLock.unlock();
				return false;
			}
		}

		bool result = hashTable[idx] ? hashTable[idx] == elem : (elem ? false : true);

		if ( doLocking )
			hashTableLock.unlock();

		return result;
	}


	/** @brief return true if the data at index @a idx has the key @a key
	  *
	  * WARNING: The protected hashTable methods do **NOT** check @a idx! Check it beforehand!
	  *
	  * @param[in] idx  Index at which the key to compare
	  * @param[in] key The key to compare.
	  * @return return true if the data at index @a idx has the key @a key
	**/
	bool table_key_equals( uint32_t idx, key_t const& key ) const noexcept {
		// Early exit if this was destroyed (Can be through some dtor like in CThreadElementStore)
		if ( this->isDestroyed.load() || ( nullptr == hashTable ) )
			return false;

		bool doLocking = beThreadSafe();

		if ( doLocking ) {
			// No need to debug log this, so we do not use PWX_LOCK_OBJ() here.
			hashTableLock.lock();
			if ( this->isDestroyed.load() || ( nullptr == hashTable ) ) {
				hashTableLock.unlock();
				return false;
			}
		}

		bool result = hashTable[idx] ? ( hashTable[idx] != vacated ? hashTable[idx]->key == key : false ) : false;

		if ( doLocking )
			hashTableLock.unlock();

		return result;
	}


	/** @brief return the element at index @a idx
	  *
	  * WARNING: The protected hashTable methods do **NOT** check @a idx! Check it beforehand!
	  *
	  * Note: Vacated buckets are always returned as `nullptr`. Use `protIsVacated() if you
	  * need to know whether this bucket is vacated or not.
	  *
	  * @param[in] idx  Index from which the element to get
	  * @return return the element at index @a idx
	**/
	elem_t* table_get( uint32_t idx ) noexcept {
		// Early exit if this was destroyed (Can be through some dtor like in CThreadElementStore)
		if ( this->isDestroyed.load() || ( nullptr == hashTable ) )
			return nullptr;

		bool doLocking = beThreadSafe();

		if ( doLocking ) {
			// No need to debug log this, so we do not use PWX_LOCK_OBJ() here.
			hashTableLock.lock();
			if ( this->isDestroyed.load() || ( nullptr == hashTable ) ) {
				hashTableLock.unlock();
				return nullptr;
			}
		}

		elem_t* result = hashTable[idx];

		if ( doLocking )
			hashTableLock.unlock();

		return result == vacated ? nullptr : result;
	}


	/** @brief return the element at index @a idx const variant
	  *
	  * WARNING: The protected hashTable methods do **NOT** check @a idx! Check it beforehand!
	  *
	  * Note: Vacated buckets are always returned as `nullptr`. Use `protIsVacated()` if you
	  * need to know whether this bucket is vacated or not.
	  *
	  * @param[in] idx  Index from which the element to get
	  * @return return the element at index @a idx
	**/
	elem_t* table_get( uint32_t idx ) const noexcept {
		// Early exit if this was destroyed (Can be through some dtor like in CThreadElementStore)
		if ( this->isDestroyed.load() || ( nullptr == hashTable ) )
			return nullptr;

		bool doLocking = beThreadSafe();

		if ( doLocking ) {
			// No need to debug log this, so we do not use PWX_LOCK_OBJ() here.
			hashTableLock.lock();
			if ( this->isDestroyed.load() || ( nullptr == hashTable ) ) {
				hashTableLock.unlock();
				return nullptr;
			}
		}

		elem_t* result = hashTable[idx];

		if ( doLocking )
			hashTableLock.unlock();

		return result == vacated ? nullptr : result;
	}


	/** @brief Set hashTable element to @a elem
	  *
	  * WARNING: The protected hashTable methods do **NOT** check @a idx! Check it beforehand!
	  *
	  * @param[in] idx  Index at which the element to set
	  * @param[in] elem Pointer to the element to set.
	  * @return Pointer of the previous element
	**/
	elem_t* table_set( uint32_t idx, elem_t* elem ) noexcept {
		// Early exit if this was destroyed (Can be through some dtor like in CThreadElementStore)
		if ( this->isDestroyed.load() || ( nullptr == hashTable ) )
			return nullptr;

		bool doLocking = beThreadSafe();

		if ( doLocking ) {
			// No need to debug log this, so we do not use PWX_LOCK_OBJ() here.
			hashTableLock.lock();
			if ( this->isDestroyed.load() || ( nullptr == hashTable ) ) {
				hashTableLock.unlock();
				return nullptr;
			}
		}

		elem_t* oldElem = hashTable[idx];
		hashTable[idx]  = elem;

		if ( doLocking )
			hashTableLock.unlock();

		return oldElem;
	}


	/** @brief Set hashTable element to `vacated`
	  *
	  * WARNING: The protected hashTable methods do **NOT** check @a idx! Check it beforehand!
	  *
	  * @param[in] idx  Index at which to set to `vacated`
	  * @return Pointer of the previous element
	**/
	elem_t* table_vacate( uint32_t idx ) noexcept {
		// Early exit if this was destroyed (Can be through some dtor like in CThreadElementStore)
		if ( this->isDestroyed.load() || ( nullptr == hashTable ) )
			return nullptr;

		bool doLocking = beThreadSafe();

		if ( doLocking ) {
			// No need to debug log this, so we do not use PWX_LOCK_OBJ() here.
			hashTableLock.lock();
			if ( this->isDestroyed.load() || ( nullptr == hashTable ) ) {
				hashTableLock.unlock();
				return nullptr;
			}
		}

		elem_t* oldElem = hashTable[idx];
		hashTable[idx]  = vacated;

		if ( doLocking )
			hashTableLock.unlock();

		return oldElem;
	}


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	using base_t::isDestroyed;

	aui32_t          clearing  = ATOMIC_VAR_INIT( 0 ); //!< Needed by the control macros
	eChainHashMethod CHMethod  = CHM_Division;         //!< Which Hashing method is used
	aui32_t          growing   = ATOMIC_VAR_INIT( 0 ); //!< Needed by the control macros
	CHashBuilder     hashBuilder;                      //!< instance that will handle the key generation
	aui32_t          hashSize;                         //!< number of places to maintain
	aui32_t          inserting = ATOMIC_VAR_INIT( 0 ); //!< Needed by the control macros
	aui32_t          removing  = ATOMIC_VAR_INIT( 0 ); //!< Needed by the control macros


private:

	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	/* Important:
	 * All protected and private methods must not lock the container!
	 * Container locking is done by the public methods!
	 * Further all protected and private
	 * methods must not use public methods, or they might end up
	 * freezing while waiting for themselves to end!
	 */

	/** @brief private part of adding an element
	  * This is done here, instead of in add() for grow()
	  * to call without stopping itself.
	  * Inserting must be advertised and existence must be tested!
	**/
	virtual uint32_t privAdd( const elem_t& src ) {
		// 1: Check source:
		PWX_LOCK_OBJ( const_cast<elem_t*>( &src ) )

		if ( src.destroyed() ) {
			// What on earth did the caller think?
			PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) )
			PWX_THROW( "Illegal Condition", "Source element destroyed",
			           "An element used as source for insertion is destroyed." );
		}

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( src ) )
		catch( std::exception& e ) {
			PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) )
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new hash element failed." );
		}
		PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) );
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 3: Do the real insert
		return privInsert( newElement );
	}


	/** @brief private part of adding a key/data pair
	  * This is done here, instead of in add() for grow()
	  * to call without stopping itself.
	  * Inserting must be advertised and existence must be tested!
	**/
	virtual uint32_t privAdd( const key_t& key, data_t* data ) {
		// 1: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( key, data, destroy ) )
		catch( std::exception& e )
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new hash element failed." );

		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 2: Do the real insert
		return privInsert( newElement );
	}


	/** @brief returns a read-only pointer to the element with the key @a key
	  *
	  * @param[in] key the key to search for
	  * @return a const pointer to the element or nullptr if the key could not be found.
	**/
	virtual elem_t* privGet( const key_t& key ) const noexcept {
		uint32_t  keyIdx = privGetIndex( key );
		elem_t* xCurr   = table_get( keyIdx );

		while ( xCurr && ( *xCurr != key ) )
			xCurr = xCurr->getNext();

		return xCurr;
	}


	/** @brief get an element by index
	  * @param[in] index the index of the element to find.
	  * @return read-only pointer to the element, or nullptr if there is no element with the specific index.
	**/
	virtual const elem_t* privGetByIndex( const int64_t index ) const noexcept {
		// Mod index into range
		uint32_t xSize = hashSize.load( memOrdLoad );
		uint32_t xIdx  = static_cast<uint32_t> ( index < 0
		                 ? xSize - ( std::abs ( index ) % xSize )
		                 : index % xSize );

		// Unfortunately this results in xIdx equaling xSize
		// (which is wrong) if index is a negative multiple of
		// xSize:
		if ( xIdx >= xSize )
			xIdx %= xSize;

		return table_get( xIdx );
	}


	/* Getting the index of a key depends on what kind of hash it
	 * is and what the hashSize is.
	 * In an open hash table a double hash method using the multiplication
	 * method on the first and the division method on the second should
	 * always do fine.
	 * But in a chained hash table the hashSize is important. The division
	 * method is slightly faster but needs an odd hashSize that is a prime
	 * number not too near to any 2^x.
	 * Therefore the inherited specialized hash tables need to determine
	 * how to get the index of a key.
	*/
	virtual uint32_t privGetIndex( const key_t& key ) const noexcept PWX_VIRTUAL_PURE;


	// How collisions are resolved is a hash type specific matter
	virtual uint32_t privInsert( elem_t* elem ) PWX_VIRTUAL_PURE;


	// This method must be implemented by the hash templates themselves, because
	// the outcome of a remove is different whether it is a chained (nullptr)
	// or open (vacated) hash.
	virtual elem_t* privRemoveIdx ( uint32_t index ) noexcept PWX_VIRTUAL_PURE;


	// The same applies to removal by key:
	virtual elem_t* privRemoveKey ( const key_t& key ) noexcept PWX_VIRTUAL_PURE;

	/// @brief internal method to set the hashing method according to @a targetSize
	void privSetHashMethod( uint32_t targetSize ) noexcept {
		CHMethod = CHM_Multiplication; // default the safe one

		// Test 1: Even sizes can't use the division method
		if ( targetSize % 2 ) {
			// Test 2: For the division method to safely work, the size
			// should be a prime number with a good distance to the
			// next smaller and larger 2^x values:
			uint32_t lowerBound = 64;
			uint32_t upperBound = 128;

			// find bounds:
			while ( lowerBound > targetSize ) {
				upperBound = lowerBound;
				lowerBound /= 2;
			}

			while ( upperBound < targetSize ) {
				lowerBound = upperBound;
				upperBound *= 2;
			}

			uint32_t middle  = ( lowerBound + upperBound ) / 2;
			uint32_t midDist = middle > targetSize ? middle - targetSize : targetSize - middle;

			if ( midDist < std::min( ( targetSize - lowerBound ) / 2, ( upperBound - targetSize ) / 2 ) ) {
				// Test 3: (almost) a prime number
				// For this to test the size is simply divided by the first
				// 8 odd numbers (but 15) and must not be dividable by more
				// than one
				int divided = 0;
				for ( uint32_t divisor = 3; ( divided < 2 ) && ( divisor < 20 ); divisor += 2 ) {
					// 15 is already covered by 3 and 5
					if ( 15 != divisor ) {
						if ( !( targetSize % divisor ) )
							++divided;
					}
				} // End of checking divisors

				// Now if divided is lower than 2, the division method can be used
				if ( divided < 2 )
					CHMethod = CHM_Division;
			} // End of valid Test 2
		} // End of checking for an odd number
	}


	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	double const dynGrowFactor; //!< When the table is automatically grown, it is grown by this factor.
	double const maxLoadFactor; //!< When the load factor reaches this, the table is grown.
	elem_t**     hashTable;     //!< the central array that is our hash
	mutable
	CLockable    hashTableLock; //!< the table_*() methods use this to secure access to the table
	char*        vacChar;       //!< alias pointer to get around the empty elem_t ctor restriction
	elem_t*      vacated;       //!< The Open Hash sets empty places to point at this.
	// Note: vacated is placed here, so clear(), disable_thread_safety() and
	//       enable_thread_safety() can be unified here as well. Otherwise
	//       the hashes would need individual functions that only differ
	//       in one compare.
}; // class VTHashBase


/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the hash.
**/
template<typename key_t, typename data_t, typename elem_t>
VTHashBase<key_t, data_t, elem_t>::~VTHashBase() noexcept {
	PWX_DOUBLE_LOCK_GUARD( this, &hashTableLock );

	// Mark as destroyed
	isDestroyed.store( true, memOrdStore );

	// Be sure everybody waiting for a lock can react on isDestroyed:
	while ( waiting() ) {
		PWX_DOUBLE_LOCK_GUARD_RESET( this, &hashTableLock );
	}

	// Wipe hash table
	this->clear();

	// Delete hash table
	if ( hashTable ) {
		// clear() does **not** eliminate vacated buckets, so we have to nullify them, too
		memset( hashTable, 0, sizeof( elem_t* ) * hashSize.load() );

		// now delete it.
		PWX_TRY( delete [] hashTable )
		DEBUG_LOG_CAUGHT_STD( "delete hashTable" )
		catch( ... ) { /* Can't do anything about that! */ }
		hashTable = nullptr;
		hashSize.store( 0 );
	}

	// Delete "vacated" sentry
	vacated = nullptr;
	if ( vacChar ) {
		PWX_TRY( delete vacChar )
		DEBUG_LOG_CAUGHT_STD( "delete vacChar" )
		catch( ... ) { /* Can't do anything about that! */ }
		vacChar = nullptr;
	}

	// Be sure everybody waiting for a lock can react on this being destroyed:
	while ( waiting() ) {
		PWX_DOUBLE_LOCK_GUARD_RESET( this, &hashTableLock );
	}
}


/** @brief addition operator
  *
  * Create and return a new hash consisting of all elements of @a lhs and @a rhs.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local hash is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: All elements
  * will be copied twice!
  * Only use this operator if you really think you need a construct like
  * 'hashC = hashA + hashB'.
  *
  * @param[in] lhs left hand side reference
  * @param[in] rhs right hand side reference
  * @return the new hash.
**/
template<typename data_t, typename elem_t>
VTHashBase<data_t, elem_t> operator+ ( const VTHashBase<data_t, elem_t>& lhs, const VTHashBase<data_t, elem_t>& rhs ) {
	VTHashBase<data_t, elem_t> result( lhs );

	if ( &lhs != &rhs ) {
		PWX_TRY_PWX_FURTHER( result += rhs );
	}

	return result;
}


/** @brief substraction operator
  *
  * Remove all elements from @a rhs from this hash.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local hash is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: The difference
  * elements are copied twice!
  *
  * @param[in] lhs reference of the hash from which to substract.
  * @param[in] rhs reference of the hash to substract.
  * @return reference to this.
**/
template<typename data_t, typename elem_t>
VTHashBase<data_t, elem_t> operator- ( const VTHashBase<data_t, elem_t>& lhs, const VTHashBase<data_t, elem_t>& rhs ) {
	VTHashBase<data_t, elem_t> result( lhs );

	if ( &lhs != &rhs ) {
		PWX_TRY_PWX_FURTHER( result -= rhs );
	} else
		result.clear();

	return result;
}


} // namespace pwx

#endif // PWX_LIBPWX_PWX_BASE_VHASH_H_INCLUDED
