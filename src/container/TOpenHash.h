#ifndef PWX_LIBPWX_PWX_CONTAINER_TOPENHASH_H_INCLUDED
#define PWX_LIBPWX_PWX_CONTAINER_TOPENHASH_H_INCLUDED
#pragma once

/** @file TOpenHash.h
  *
  * @brief Declaration of a basic template for open hash containers
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


#if defined(LIBPWX_DEBUG)
#  include "stream_helpers/StreamHelpers.h"
#endif // LIBPWX_DEBUG
#include "container/VTHashBase.h"


namespace pwx {


/** @class TOpenHash POpenHash <POpenHash>
  *
  * @brief Open hash container for variable types
  *
  * This is a standard open hash container.
  *
  * In a open hash container each bucket is exactly one singular place. This means,
  * that upon reaching the correct bucket, the correct item still is right there.
  *
  * If a bucket is already taken when a new item is to be inserted, a secondary hash
  * algorithm is used to generate a different offset to move through the bucket
  * array. This movement is called "hops".
  *
  * In an optimal situation each item can be reached by the primary hash algorithm
  * and the retrieval is simple and fast.
  * But if either a bad hash algorithm or a too low hash size is chosen, items can
  * scatter and need many hops.
  *
  * To avoid such a situation, you should always chose a size that is the number of
  * items you need at most, plus 20%, and then raised to the next prime number.
  *
  * Further the PrydeWorX Library Open Hash container uses "Robin Hood Hashing". This
  * means, that if an item is to be inserted and reaches a bucket that is taken, item
  * in there is moved further if it has fewer hops than the new item.
  *
  * The hash algorithms used within the PrydeWorX library already offer a very high
  * level of distribution. For keys not handled by these, or if you think you have
  * a better hash algorithm, you can set your own hash function with the constructor.
**/
template<typename key_t, typename data_t, typename elem_t = THashElement<key_t, data_t> >
class PWX_API TOpenHash : public VTHashBase<key_t, data_t, elem_t> {
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef VTHashBase<key_t, data_t, elem_t> base_t;     //!< Base type of the hash
	typedef TOpenHash<key_t, data_t, elem_t> hash_t;     //!< Type of this hash


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
	  * The @a maxLoad_ and @a dynGrow_ parameters can be used to tell the hash table
	  * how much it should grow when the the specified load factor is reached.
	  * The default for open hash tables is to grow by a factor of 1.50 (50%) when
	  * a load factor of 0.8 is reached.
	  *
	  * @param[in] initSize Initial size of the hash table.
	  * @param[in] keyLen_ Length of the key to limit hash generation.
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	TOpenHash(  uint32_t initSize, uint32_t keyLen_,
	            double maxLoad_, double dynGrow_ ) noexcept:
		base_t( initSize, keyLen_, maxLoad_, dynGrow_ )
	{ }


	/** @brief full constructor with key length
	  *
	  * The full constructor initializes an empty hash with user defined delete
	  * method, hashing method and key length. The initial size is the @a initSize
	  *
	  * The @a maxLoad_ and @a dynGrow_ parameters can be used to tell the hash table
	  * how much it should grow when the the specified load factor is reached.
	  * The default for open hash tables is to grow by a factor of 1.50 (50%) when
	  * a load factor of 0.8 is reached.
	  *
	  * @param[in] initSize The initial size of the table.
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash_ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
	  * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	TOpenHash(  uint32_t initSize,
	            void ( *destroy_ ) ( data_t* data ),
	            uint32_t ( *hash_ ) ( const key_t* key, uint32_t keyLen ),
	            uint32_t keyLen_,
	            double maxLoad_, double dynGrow_ ) noexcept :
		base_t( initSize, destroy_, hash_, keyLen_, maxLoad_, dynGrow_ )
	{ }


	/** @brief full constructor without key length
	  *
	  * The full constructor initializes an empty hash with user defined delete
	  * method and hashing method withour key length. The initial size is the
	  * @a initSize
	  *
	  * The @a maxLoad_ and @a dynGrow_ parameters can be used to tell the hash table
	  * how much it should grow when the the specified load factor is reached.
	  * The default for open hash tables is to grow by a factor of 1.50 (50%) when
	  * a load factor of 0.8 is reached.
	  *
	  * @param[in] initSize The initial size of the table.
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash_ A pointer to a function that can hash the keys that are stored
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	TOpenHash(  uint32_t initSize,
	            void ( *destroy_ ) ( data_t* data ),
	            uint32_t ( *hash_ ) ( const key_t* key ),
	            double maxLoad_, double dynGrow_ ) :
		base_t( initSize, destroy_, hash_, maxLoad_, dynGrow_ )
	{ }


	/** @brief limiting user method constructor
	  *
	  * This constructor only takes a destroy method and a hash
	  * method with explicit key length.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash_ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
	  * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
	**/
	TOpenHash(  void ( *destroy_ ) ( data_t* data ),
	            uint32_t ( *hash_ ) ( const key_t* key, uint32_t keyLen ),
	            uint32_t keyLen_ ) noexcept :
		base_t( destroy_, hash_, keyLen_, 0.8, 1.5 )
	{ }


	/** @brief user method constructor
	  *
	  * This constructor only takes a destroy method and a hash
	  * method without explicit key length.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash_ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
	**/
	TOpenHash(  void ( *destroy_ ) ( data_t* data ),
	            uint32_t ( *hash_ ) ( const key_t* key ) ) noexcept :
		base_t( destroy_, hash_, 0.8, 1.5 )
	{ }


	/** @brief destroy method constructor
	  *
	  * This constructor only takes a destroy method.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TOpenHash(  void ( *destroy_ ) ( data_t* data ) ) noexcept :
		base_t( destroy_, 0.8, 1.5 )
	{ }


	/** @brief key length constructor
	  *
	  * This constructor only takes a key length and sets the destroy
	  * and hash methods to nullptr.
	  *
	  * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
	**/
	TOpenHash(  uint32_t keyLen_ ) noexcept :
		base_t ( keyLen_, 0.8, 1.5 )
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor uses the default constructor to set the data
	  * destroy method and the hash method to the null pointer with
	  * full key usage
	**/
	TOpenHash() noexcept :
		base_t ( 0.8, 1.5 )
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * If a new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference of the hash to copy.
	**/
	TOpenHash(  const hash_t& src ) :
		base_t ( src ) {
		operator+=( src );
	}


	virtual ~TOpenHash() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	using base_t::add;
	using base_t::clear;
	using base_t::disable_thread_safety;
	using base_t::empty;
	using base_t::exists;
	using base_t::enable_thread_safety;
	using base_t::get;
	using base_t::getData;
	using base_t::getHops;
	using base_t::grow;
	using base_t::pop;
	using base_t::pop_back;
	using base_t::pop_front;
	using base_t::push;
	using base_t::push_back;
	using base_t::push_front;
	using base_t::shift;
	using base_t::size;
	using base_t::sizeMax;
	using base_t::unshift;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	using base_t::operator=;
	using base_t::operator+=;
	using base_t::operator-=;
	using base_t::operator[];


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

	using base_t::protGetHash;


	/** @brief use hashBuilder to generate a secondary hash out of primary hash
	  * @param priHash pointer to the primary hash to hash again
	  * @return uint32_t hash of @a priHash
	**/
	virtual uint32_t protGetSecHash( const uint32_t* priHash ) const {
		return hashBuilder( priHash );
	}


	using base_t::protIsVacated;


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	using base_t::CHMethod;
	using base_t::eCount;
	using base_t::hashBuilder;
	using base_t::hashSize;
	using base_t::hashTable;
	using base_t::memOrdLoad;
	using base_t::memOrdStore;
	using base_t::vacated;

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

	/** @brief real index calculation
	  *
	  * This is the real index calculation that can be told where to
	  * stop. VTHashBase::get() needs a method to get the index to a
	  * key that does not end in vacated. But privInsert() is quite
	  * happy populating vacated (*) positions.
	  *
	  * In an open hash table the index is calculated using a double
	  * hash method. Basically this means to use one base index and
	  * a value as a stepping to probe the table.
	  *
	  * Because of TOpenHash being dynamic in its size, it is not
	  * possible to craft both. The second value, the stepping, must
	  * be chosen to allow the probing of all places before the first
	  * is probed twice.
	  *
	  * "vacated" positions might not only be those marked with the
	  * vacated sentry, but those occupied by elements with fewer hops,
	  * too. These are given back if @a hops is not nullptr.
	  * The method to replace elements with fewer hops is called
	  * "Robin Hood Hashing", and reduces secondary clustering.
	  *
	  * @param[in] key const reference of the key to evaluate
	  * @param[in] allowVacated if set to true, the method will return indexes from vacated positions and jump those otherwise.
	  * @param[out] hops if not nullptr, the number of hops done to find the final position is noted in here.
	  * @return the index an element with this key would have in the table
	**/
	virtual uint32_t privGetIndex( const key_t& key, bool allowVacated, uint32_t* hops ) const noexcept {
		uint32_t priHash = this->protGetHash( &key );

		// Use multiplication method for the base index
		uint32_t tabSize = this->hashSize.load( memOrdLoad );
		double   dHash   = static_cast<double>( priHash ) * 0.618;
		uint32_t idxBase = static_cast<uint32_t>( std::floor( ( dHash - std::floor( dHash ) ) * tabSize ) );

		// Use the current hashing method for the stepping (secondary hash)
		uint32_t idxStep = this->privGetStepping( priHash );

		// Now probe the table until we are done or have found the key
		bool     isFound     = false;
		bool     isVacated   = false;
		bool     beRobinHood = allowVacated && hops;
		uint32_t pos         = idxBase;
		for ( uint32_t i = 0; !isFound && ( i < tabSize ); ++i ) {
			isVacated = hashTable[pos] == vacated;

			// we are done if ...
			if (        // a) the hashTable has a nullptr at pos or
			        ( nullptr == hashTable[pos] )
			        // b) the hashTable has an element with the same key
			        ||      ( !isVacated && ( *hashTable[pos] == key ) )
			        // c) allowVacated is true and the position is vacated
			        ||      ( isVacated && allowVacated )
			        // d) beRobinHood is true and the position is taken, but the element has fewer hops
			        //    with hops being greater than 1 and the resident has at least two less.
			        ||  ( beRobinHood && !isVacated && ( *hops > 1 ) && ( hashTable[pos]->hops < ( *hops - 1 ) ) ) )
				isFound = true;
			else {
				pos = ( pos + idxStep ) % tabSize;
				if ( hops ) ++( *hops );

				// check whether the stepping goes round
				if ( ( ( i + 1 ) < tabSize ) && ( pos == idxBase ) ) {
					// This means the idx stepping is screwed
					DEBUG_ERR( "open hash",
					           "Unfull probing at hop %s: pos %u == base %u, step %u in size %u",
					           hops ? pwx::to_string( *hops ).c_str() : "n/a",
					           pos, idxBase, idxStep, tabSize );
					idxStep += idxStep % 2 ? 2 : 3;
					// Be sure idxStep does not end up being tabSize or we'll be here again next round.
					if ( idxStep >= tabSize )
						idxStep = 3;
					pos = ( pos + idxStep ) % tabSize;
				}
			}
		} // end of traversing the table

		#if defined(LIBPWX_DEBUG)
		if ( hops && ( *hops == tabSize ) ) {
			// This is really bad!
			DEBUG_ERR( "open hash", "\n---\nHash table seems to be full or %s is screwed:", PWX_CURRENT_FUNC );
			DEBUG_ERR( "open hash", "  Table size : %u", tabSize );
			DEBUG_ERR( "open hash", "  Elements   : %u", this->eCount.load( memOrdLoad ) );
			DEBUG_ERR( "open hash", "  Hops done  : %u", *hops ); // Trivial, but the if() is not part of the output
			DEBUG_ERR( "open hash", "  Initial Idx: %u", idxBase % tabSize );
			DEBUG_ERR( "open hash", "  Stepping   : %u", idxStep );
			DEBUG_ERR( "open hash", "  1st Hash   : %u\n---", priHash );
		}
		#endif // defined(LIBPWX_DEBUG)

		// Now "pos" points at the first nullptr place found,
		// the first vacated position if accepted or to an
		// element with the same key.

		return pos;

		// WARNING: If the hash table reaches load level 1.0 (all
		// places used) and the key is not in the table, the
		// whole thing would be screwed. That is one of the
		// strongest reasons to automatically grow on reaching
		// load level 0.8.
	}


	/** @brief get the index of a key jumping vacated places
	  *
	  * This is just a wrapper to satisfy the needs of VTHashBase::get()
	  * to return a non-vacated position.
	  *
	  * @param[in] key const reference of the key to evaluate
	  * @return the index an element with this key would have in the table
	**/
	virtual uint32_t privGetIndex( const key_t& key ) const noexcept {
		return privGetIndex( key, false, nullptr );
	}


	/// @brief small method to get the second hash stepping calculation out of privGetIndex()
	virtual uint32_t privGetStepping( uint32_t primary_hash ) const noexcept {
		uint32_t stepping = 0;
		uint32_t secHash  = this->protGetSecHash( &primary_hash );
		uint32_t tabSize  = hashSize.load( memOrdLoad );
		uint32_t secSize  = tabSize - ( tabSize % 2 ? 2 : 1 );

		if ( CHM_Division == CHMethod )
			stepping = secHash % secSize;
		else {
			double dHash = static_cast<double>( secHash ) * 0.618;
			stepping     = static_cast<uint32_t>( std::floor( ( dHash - std::floor( dHash ) ) * secSize ) );
		}

		// Be sure stepping is sane:
		if ( stepping < 3 )
			// On odd sizes a step of 2 always traverses all positions.
			stepping = tabSize % 2 ? 2 : 3;

		// Both must not be the same, even or odd
		if ( ( tabSize % 2 ) == ( stepping % 2 ) )
			++stepping;

		/* An important part is to make sure that the stepping
		* must not be n/(n+1) of the table size, or the stepping
		* will lead to unfull probing.
		* However, experiments show that the range 2<n<9 is enough
		* to test.
		*/
		bool isAdapted = true;
		while ( isAdapted ) {
			isAdapted = false;
			for ( uint32_t n = 3; !isAdapted && ( n < 9 ); ++n ) {
				if ( stepping == ( tabSize * n / ( n + 1 ) ) ) {
					isAdapted = true;
					stepping += 2;
				}
			}

			// tabSize must not be devidable by stepping either
			if ( !( tabSize % stepping ) ) {
				isAdapted = true;
				stepping += 2;
			}

			// And the stepping must not grow larger than tabSize
			if ( stepping > tabSize ) {
				isAdapted = true;
				stepping = stepping % tabSize;
			}
		} // end of detecting dangerous fractions of tabSize.

		return stepping;
	}


	/// @brief private insertion relying on privGetIndex() to resolve collisions
	virtual uint32_t privInsert( elem_t* elem ) {
		uint32_t  idx  = privGetIndex( elem->key, true, &( elem->hops ) );

		#if defined(LIBPWX_DEBUG)
		// The position must not be occupied unless the hop count is smaller
		// Note: This used to be an assert, but the checks are too flat then.
		uint32_t tabSize  = this->hashSize.load( memOrdLoad );
		if ( idx >= tabSize ) {
			DEBUG_ERR( "open hash",
			           "privGetIndex returned %u on table size %u (insert key: \"%s\")",
			           idx, tabSize, pwx::to_string( elem->key ).c_str() );
			PWX_THROW( "out of bounds", "Index too large",
			           "The index returned by privGetIndex() is larger than the table size" );
		}
		if ( ( nullptr != hashTable[idx] )
		                && ( hashTable[idx] != vacated )
		                && ( ( hashTable[idx]->hops >= elem->hops )
		                     || ( *hashTable[idx] == elem->key ) ) ) {
			if ( *hashTable[idx] == elem->key ) {
				DEBUG_ERR( "open hash",
				           "privInsert called with key \"%s\", already found at index %u",
				           pwx::to_string( elem->key ).c_str(), idx );
				DEBUG_ERR( "open hash", " -> table size %u, hops performed %u", tabSize, elem->hops );
				PWX_THROW( "illegal index", "key already exists",
				           "privInsert called with an already stored key!" );
			} else if ( hashTable[idx]->hops >= elem->hops ) {
				DEBUG_ERR( "open hash",
				           "Robin Hood failed and delivered %u hops when we have %u",
				           hashTable[idx]->hops, elem->hops );
				DEBUG_ERR( "open hash", " -> table size %u", tabSize );
				PWX_THROW( "illegal index", "not enough hops",
				           "An element to replace has more or equal hops" );
			}
		}
		#endif // defined(LIBPWX_DEBUG)

		/* There are two possible situations now:
		 * a) the position is occupied, but the element
		 *    found has fewer hops.
		 *    -> insert new element here, then find a new
		 *       slot for the replaced.
		 * b) the position is empty or marked as vacated
		 *    -> insert new element here
		*/
		elem_t* oldElem = nullptr;
		if ( hashTable[idx] && ( hashTable[idx] != vacated ) )
			// This is situation a)
			oldElem = this->privRemoveIdx( idx );
		hashTable[idx] = elem; // Fulfills both situations
		elem->insertAsFirst();
		eCount.fetch_add( 1, memOrdStore );

		// Now solve situation a)
		while ( oldElem ) {
			elem       = oldElem;
			oldElem    = nullptr;
			elem->hops = 0;
			idx = privGetIndex( elem->key, true, &( elem->hops ) );
			if ( hashTable[idx] && ( hashTable[idx] != vacated ) )
				// This is situation a) again
				oldElem = this->privRemoveIdx( idx );
			hashTable[idx] = elem; // Item is moved
			elem->insertAsFirst();
			eCount.fetch_add( 1, memOrdStore );
		}

		return this->eCount.load( memOrdLoad );
	}


	/** @brief removes the element with the specified index and returns it.
	  *
	  * The position in the table is marked as "vacated".
	  *
	  * If the index is out of bounds, nullptr is returned.
	  *
	  * @param[in] index the index to remove
	  * @return a pointer to the removed element or nullptr if no such element exists
	**/
	virtual elem_t* privRemoveIdx ( uint32_t index ) noexcept {
		elem_t* result = nullptr;
		if ( ( index < this->hashSize.load( memOrdLoad ) ) && hashTable[index] && !protIsVacated( index ) ) {

			// Note: Open Hashes mark empty positions with the "vacated" sentry
			result = hashTable[index];
			hashTable[index] = vacated;
			result->remove();
			--eCount;
		} // End of outer check

		return result;
	}


	/** @brief removes the element with the specified key and returns it.
	  *
	  * The position in the table is marked as "vacated".
	  *
	  * @param[in] key the key of the element to remove has
	  * @return a pointer to the removed element or nullptr if no such element exists
	**/
	virtual elem_t* privRemoveKey ( const key_t& key ) noexcept {
		return privRemoveIdx( privGetIndex( key ) );
	}
}; // class TOpenHash

/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the hash.
**/
template<typename key_t, typename data_t, typename elem_t>
TOpenHash<key_t, data_t, elem_t>::~TOpenHash() noexcept
{ /* all done in base_t dtor */ }


/** @brief addition operator
  *
  * Create and return a new hash consisting of all elements of @a lhs and @a rhs.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local hash table is created on the stack. Giving
  * it back means that it will be copied onto your result. In other words: All
  * elements will be copied twice!
  * Only use this operator if you really think you need a construct like
  * 'hashC = hashA + hashB'.
  *
  * @param[in] lhs left hand side reference
  * @param[in] rhs right hand side reference
  * @return the new hash.
**/
template<typename data_t, typename elem_t>
TOpenHash<data_t, elem_t> operator+ ( const TOpenHash<data_t, elem_t>& lhs, const TOpenHash<data_t, elem_t>& rhs ) {
	TOpenHash<data_t, elem_t> result( lhs );

	if ( &lhs != &rhs )
		PWX_TRY_PWX_FURTHER( result += rhs );

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
TOpenHash<data_t, elem_t> operator- ( const TOpenHash<data_t, elem_t>& lhs, const TOpenHash<data_t, elem_t>& rhs ) {
	TOpenHash<data_t, elem_t> result( lhs );

	if ( &lhs != &rhs ) {
		PWX_TRY_PWX_FURTHER( result -= rhs );
	} else
		result.clear();

	return result;
}


} // namespace pwx


#endif // PWX_LIBPWX_PWX_CONTAINER_TOPENHASH_H_INCLUDED

