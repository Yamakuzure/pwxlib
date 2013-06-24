#ifndef PWX_LIBPWX_PWX_BASE_VHASH_H_INCLUDED
#define PWX_LIBPWX_PWX_BASE_VHASH_H_INCLUDED

/** @file VHash.h
  *
  * @brief Virtual base class for hash containers
  *
  * This file consists of a base class template for the hash containers.
  * It is not intended to be used singularily and is therefore virtual
  * The common hash template does not include any collision
  * resolving, this is implemented in the respective hash container.
  *
  * (c) 2007 - 2013 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
  *
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
**/

#if defined(PWX_EXPORTS)
#  undef PWX_EXPORTS
#  define PWX_REDEF_EXPORTS 1
# endif

#include <pwx/base/VContainer.h>
#include <pwx/types/THashElement.h>
#include <pwx/types/CHashBuilder.h>

#if defined(PWX_REDEF_EXPORTS)
#  define PWX_EXPORTS 1
#  undef PWX_REDEF_EXPORTS
# endif

#include <cmath>

namespace pwx {

/** @class VTHashBase
  *
  * @brief Virtual base class for hash containers
  *
  * @todo describe properly
**/
template<typename key_t, typename data_t, typename elem_t = THashElement<key_t, data_t> >
class VTHashBase : public VContainer
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef VContainer                          base_t;        //!< Base type of the hash
	typedef VTHashBase<key_t, data_t, elem_t>   hash_t;        //!< Type of this hash


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
	VTHashBase(	size_t initSize, size_t keyLen_,
				double maxLoad_, double dynGrow_) noexcept:
		hashBuilder (keyLen_),
		hashSize (initSize),
		hashTable (nullptr),
		maxLoadFactor(maxLoad_),
		dynGrowFactor(dynGrow_)
	{
		// Generate the hash table
		try {
			hashTable = new elem_t*[hashSize];
			for (size_t i = 0; i < hashSize; ++i)
				hashTable[i] = nullptr;
		}
		PWX_THROW_STD_FURTHER("VTHashBase failure", "HashTable could not be created")

		// Generate the vacated pointer:
		try {
			vacChar = new char;
			vacated = reinterpret_cast<elem_t*>(vacChar);
		}
		PWX_THROW_STD_FURTHER("VTHashBase failure", "Vacated pointer could not be created")
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
	VTHashBase(	size_t initSize,
				void (*destroy_) (data_t* data),
				uint32_t (*hash_) (const key_t* key, size_t keyLen),
				size_t keyLen_,
				double maxLoad_, double dynGrow_) noexcept :
		hash_t(initSize, keyLen_, maxLoad_, dynGrow_)
	{
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
	VTHashBase(	size_t initSize,
				void (*destroy_) (data_t* data),
				uint32_t (*hash_) (const key_t* key),
				double maxLoad_, double dynGrow_) noexcept :
		hash_t(initSize, (size_t)0, maxLoad_, dynGrow_)
	{
		destroy = destroy_;
		hash_user = hash_;
	}


	/** @brief limiting user method constructor
	  *
	  * This constructor only takes a destroy method and a hash
	  * method with explicit key length.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash__ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
	  * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	VTHashBase(	void (*destroy_) (data_t* data),
				uint32_t (*hash_) (const key_t* key, size_t keyLen),
				size_t keyLen_,
				double maxLoad_, double dynGrow_) noexcept :
		hash_t((size_t)100, keyLen_, maxLoad_, dynGrow_)
	{
		destroy = destroy_;
		hash_limited = hash_;
	}


	/** @brief user method constructor
	  *
	  * This constructor only takes a destroy method and a hash
	  * method without explicit key length.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash__ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
	  * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
	  * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
	**/
	VTHashBase(	void (*destroy_) (data_t* data),
				uint32_t (*hash_) (const key_t* key),
				double maxLoad_, double dynGrow_) noexcept :
		hash_t((size_t)100, (size_t)0, maxLoad_, dynGrow_)
	{
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
	VTHashBase(	uint32_t (*destroy_) (data_t* data),
				double maxLoad_, double dynGrow_) noexcept :
		hash_t((size_t)100, (size_t)0, maxLoad_, dynGrow_)
	{
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
	VTHashBase(	size_t keyLen_,
				double maxLoad_, double dynGrow_) noexcept :
		hash_t ((size_t)100, keyLen_, maxLoad_, dynGrow_)
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
	VTHashBase(	double maxLoad_, double dynGrow_) noexcept :
		hash_t ((size_t)100, (size_t)0, maxLoad_, dynGrow_)
	{ }


	/// Empty ctor is not available
	VTHashBase() PWX_DELETE;


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * If a new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference of the hash to copy.
	**/
	VTHashBase(	const hash_t &src) :
		base_t (src),
		destroy(src.destroy),
		hash_user(src.hash_user),
		hash_limited(src.hash_limited),
		hashBuilder (src.hashBuilder.getKeyLen()),
		hashSize (src.hashSize),
		hashTable (nullptr),
		maxLoadFactor(src.maxLoadFactor),
		dynGrowFactor(src.dynGrowFactor)
	{
		// Generate the hash table
		try {
			hashTable = new elem_t*[hashSize];
			for (size_t i = 0; i < hashSize; ++i)
				hashTable[i] = nullptr;
		}
		PWX_THROW_STD_FURTHER("VTHashBase copy failure", "HashTable could not be created")

		// Generate the vacated pointer:
		try {
			vacChar = new char;
			vacated = reinterpret_cast<elem_t*>(vacChar);
		}
		PWX_THROW_STD_FURTHER("VTHashBase copy failure", "Vacated pointer could not be created")

		operator+=(src);
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
	virtual size_t add(const elem_t &src)
	{
		// Use double search to only lock if the key is
		// not found in the first run
		if (nullptr == this->get(src.key)) {
			PWX_LOCK_GUARD(hash_t, this)
			if (nullptr == this->get(src.key)) {

				// 1: Check source:
				PWX_LOCK(const_cast<elem_t*>(&src))

				if (src.destroyed()) {
					// What on earth did the caller think?
					PWX_UNLOCK(const_cast<elem_t*>(&src))
					PWX_THROW("Illegal Condition", "Source element destroyed",
							  "An element used as source for insertion is destroyed.")
				}

				// 2: Create a new element
				elem_t* newElement = nullptr;
				PWX_TRY(newElement = new elem_t (src))
				catch(std::exception &e) {
					PWX_UNLOCK(const_cast<elem_t*>(&src))
					PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new hash element failed.")
				}
				PWX_UNLOCK(const_cast<elem_t*>(&src))
				if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
					newElement->disable_thread_safety();

				// 3: Do the real insert
				return privInsert(newElement);
			} // End of inner check
		} // End of outer check

		return this->size();
	}


	virtual size_t add(const key_t &key, data_t* data)
	{
		// Use double search to only lock if the key is
		// not found in the first run
		if (nullptr == this->get(key)) {
			PWX_LOCK_GUARD(hash_t, this)
			if (nullptr == this->get(key)) {

				// 1: Create a new element
				elem_t* newElement = nullptr;
				PWX_TRY(newElement = new elem_t (key, data, destroy))
				catch(std::exception &e) {
					PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new hash element failed.")
				}
				if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
					newElement->disable_thread_safety();

				// 2: Do the real insert
				return privInsert(newElement);
			} // End of inner check
		} // End of outer check

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
	virtual void clear() noexcept
	{
		elem_t* toDelete  = nullptr;
		size_t  pos       = 0;
		size_t  tabSize   = sizeMax();

		if (!hashTable) return; // Should never happen

		while (pos < tabSize) {
			PWX_LOCK(this)
			while (hashTable[pos] && (hashTable[pos] != vacated)) {
				toDelete = privRemove(pos);
				PWX_UNLOCK(this)
				if (toDelete) {
					PWX_TRY(protDelete(toDelete))
					catch(...) { } // We can't do anything about that
				}
			}
			++pos;
		} // end of while pos < tabSize
	}


	/** @brief delete the element @a elem
	  *
	  * If the hash table does not contain this element, nothing happens.
	  *
	  * To only remove the element from the hash for further
	  * usage, use remElem(elem) instead.
	  *
	  * If the deletion of the element throws an exception, it will be turned
	  * into a pwx CException and passed on.
	  *
	  * <B>Warning</B>: The element @a elem is invalid after this operation!
	  *
	  * @param elem reference to the element to delete
	  * @return The number of elements after the operation.
	**/
	virtual uint32_t delElem(elem_t &elem)
	{
		elem_t* toDelete = remElem(elem);
		PWX_TRY_PWX_FURTHER(return protDelete(toDelete));
	}


	/** @brief delete the element with the key @a key
	  *
	  * If the hash table does not contain an element with the key
	  * @a key, nothing happens.
	  *
	  * To only remove the element from the hash for further
	  * usage, use remKey(key) instead.
	  *
	  * If the deletion of the element throws an exception, it will be turned
	  * into a pwx CException and passed on.
	  *
	  * @param key reference to the key to search for
	  * @return The number of elements after the operation.
	**/
	virtual uint32_t delKey(const key_t &key)
	{
		elem_t* toDelete = remKey(key);
		PWX_TRY_PWX_FURTHER(return protDelete(toDelete));
	}


	/** @brief disable thread safety
	  *
	  * This method disables all thread safety measures.
	  *
	  * <B>Warning</B>: It is completely unchecked whether the
	  * container is used by more than one thread. If concurrent
	  * threads work with this container while this method is
	  * called, the outcome is unpredictable.<BR/>
	  * Further this method disables all locking mechanisms in
	  * all elements stored and all elements that are added
	  * after calling this method. Calling this method with a
	  * lot of elements stored is therefore rather costly!
	  */
	virtual void disable_thread_safety() noexcept
	{
		PWX_LOCK(this)
		this->do_locking(false);
		size_t  pos       = 0;
		size_t  tabSize   = this->sizeMax();
		elem_t* xCurr     = nullptr;
		while (pos < tabSize) {
			xCurr = hashTable[pos];
			if (!protIsVacated(pos)) {
				while (xCurr) {
					xCurr->disable_thread_safety();
					xCurr = xCurr->getNext();
				}
			}
			++pos;
		}
		this->beThreadSafe.store(false, PWX_MEMORDER_RELEASE);
		PWX_UNLOCK(this) // Just for the record
	}


	/// @brief return true if this container is empty
	virtual bool empty() const noexcept
	{
		return !eCount.load(PWX_MEMORDER_ACQUIRE);
	}


	/** @brief enable thread safety
	  *
	  * This method enables all thread safety measures.
	  *
	  * <B>Warning</B>: This method enables all locking
	  * mechanisms in all elements stored and all elements
	  * that are added after calling this method. Calling
	  * this method with a lot of elements stored is
	  * therefore rather costly!
	  */
	virtual void enable_thread_safety() noexcept
	{
		this->do_locking(true);
		size_t  pos       = 0;
		size_t  tabSize   = this->sizeMax();
		elem_t* xCurr     = nullptr;
		while (pos < tabSize) {
			xCurr = hashTable[pos];
			if (!protIsVacated(pos)) {
				while (xCurr) {
					xCurr->enable_thread_safety();
					xCurr = xCurr->getNext();
				}
			}
			++pos;
		}
		this->beThreadSafe.store(true, PWX_MEMORDER_RELEASE);
	}


	/** @brief returns a read-only pointer to the element with the key @a key
	  *
	  * @param[in] key the key to search for
	  * @return a const pointer to the element or nullptr if the key could not be found.
	**/
	virtual elem_t* get(const key_t &key) const noexcept
	{
		return privGet(key);
	}


	/** @brief returns a read/write pointer to the element with the key @a key
	  *
	  * @param[in] key the key to search for
	  * @return a pointer to the element or nullptr if the key could not be found.
	**/
	virtual elem_t* get(const key_t &key) noexcept
	{
		return const_cast<elem_t*>(privGet(static_cast<const key_t>(key)));
	}


	/** @brief returns a read-only reference to the stored data with key @a key
	  *
	  * If the data pointer is nullptr, a pwx::CException with the name
	  * "NullDataException" is thrown.
	  *
	  * @param[in] key the key to search for
	  * @return a read/write reference to the stored data.
	**/
	virtual data_t &getData(const key_t &key) const
	{
		PWX_TRY_PWX_FURTHER(return **(get(key)));
	}


	/** @brief returns a read/write reference to the stored data with key @a key
	  *
	  * If the data pointer is nullptr, a pwx::CException with the name
	  * "NullDataException" is thrown.
	  *
	  * @param[in] key the key to search for
	  * @return a read/write reference to the stored data.
	**/
	virtual data_t &getData(const key_t &key)
	{
		PWX_TRY_PWX_FURTHER(return **(get(key)));
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
	virtual size_t grow(size_t targetSize)
	{
		if (targetSize > sizeMax()) {
            PWX_LOCK_GUARD(hash_t, this)
			if (targetSize > sizeMax()) {
				// --- Create a new larger table ---
				elem_t** oldTab = hashTable;
				try {
					hashTable = new elem_t*[targetSize];
					for (size_t i = 0; i < targetSize; ++i)
						hashTable[i] = nullptr;
				}
				PWX_THROW_STD_FURTHER("grow failure", "Larger HashTable could not be created")

				// --- Copy all elements ---
				elem_t* toMove  = nullptr;
				size_t  pos       = 0;
				size_t  tabSize   = sizeMax();

				while (pos < tabSize) {
					while (oldTab[pos] && (oldTab[pos] != vacated)) {
						if ( ( toMove = privRemove(pos) ) )
							PWX_TRY_PWX_FURTHER(this->add(*toMove))
						// Note: eCount is not affected. privRemove() will reduce
						// it while add() increases it.
					}
					++pos;
				}

				// --- Set new size ---
				hashSize.store(targetSize,
								this->beThreadSafe.load(PWX_MEMORDER_RELAXED)
								? PWX_MEMORDER_RELEASE
								: PWX_MEMORDER_RELAXED);

				// --- Delete old table ---
				PWX_TRY_STD_FURTHER(delete [] oldTab, "Delete failed",
						"Deleting the old table after growing into a new table failed.")
			} // End of inner size check
		} // End of outer size check

		return sizeMax();
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
	virtual elem_t* remElem(elem_t &elem) noexcept
	{
		return privRemove(elem.key);
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
	virtual elem_t* remKey(const key_t &key) noexcept
	{
		return privRemove(key);
	}


	/// @brief return the number of stored elements
	uint32_t size() const noexcept
	{
		return this->beThreadSafe.load(PWX_MEMORDER_RELAXED)
			? eCount.load(PWX_MEMORDER_ACQUIRE)
			: eCount.load(PWX_MEMORDER_RELAXED);
	}


	/// @brief return the maximum number of places (elements for open, buckets for chained hashes)
	size_t sizeMax() const noexcept
	{
		return this->beThreadSafe.load(PWX_MEMORDER_RELAXED)
			? hashSize.load(PWX_MEMORDER_ACQUIRE)
			: hashSize.load(PWX_MEMORDER_RELAXED);
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
	virtual hash_t &operator= (const hash_t & rhs)
	{
		if (&rhs != this) {
			PWX_DOUBLE_LOCK (hash_t, this, hash_t, const_cast<hash_t*> (&rhs))
			clear();
			destroy      = rhs.destroy;
			hash_user    = rhs.hash_user;
			hash_limited = rhs.hash_limited;
			hashBuilder.setKeyLen(rhs.hashBuilder.getKeyLen());

			size_t tgtSize = rhs.sizeMax();
			if (sizeMax() < tgtSize)
				PWX_TRY_PWX_FURTHER(this->grow(tgtSize));

			beThreadSafe = rhs.beThreadSafe.load(PWX_MEMORDER_RELAXED);
			PWX_TRY_PWX_FURTHER (return operator+=(rhs))
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
	virtual hash_t &operator+= (const hash_t & rhs)
	{
		if (&rhs != this) {
			PWX_DOUBLE_LOCK (hash_t, this, hash_t, const_cast<hash_t*> (&rhs))

			// --- grow this table if needed ---
			size_t rhsSize = rhs.sizeMax();
			size_t lhsSize = this->sizeMax();

			if (rhsSize > lhsSize)
				PWX_TRY_PWX_FURTHER(this->grow(rhsSize));

			// --- copy all elements ---
			elem_t* rhsCurr = nullptr;
			size_t  rhsPos  = 0;
			bool    isTS    = this->beThreadSafe.load(PWX_MEMORDER_ACQUIRE);

			while (rhsPos < rhsSize) {
				if (!rhs.protIsVacated(rhsPos)) {
					rhsCurr = rhs.hashTable[rhsPos];
					if (rhsCurr) {
						PWX_TRY_PWX_FURTHER(this->add(*rhsCurr))
						if (!isTS)
							this->get(rhsCurr->key)->disable_thread_safety();
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
	virtual hash_t &operator-= (const hash_t & rhs)
	{
		if (&rhs != this) {
			PWX_DOUBLE_LOCK (hash_t, this, hash_t, const_cast<hash_t*> (&rhs))

			size_t rhsSize = rhs.sizeMax();
			elem_t* lhsCurr = nullptr;
			elem_t* rhsCurr = nullptr;
			size_t  rhsPos  = 0;

			while (rhsPos < rhsSize) {
				if (!rhs.protIsVacated(rhsPos)) {
					rhsCurr = rhs.hashTable[rhsPos];
					if (rhsCurr) {
						lhsCurr = privRemove(rhsCurr->key);
						if (lhsCurr)
							PWX_TRY_PWX_FURTHER(protDelete(lhsCurr))
					}
				}
				++rhsPos;
			}
		} else {
			PWX_LOCK_GUARD(hash_t, this)
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
	virtual const elem_t* operator[] (const int64_t index) const noexcept
	{
		return privGetByIndex(index);
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
	virtual elem_t* operator[] (int64_t index) noexcept
	{
		return const_cast<elem_t* > (privGetByIndex(static_cast<const int64_t> (index)));
	}


protected:

	/* ===============================================
	 * === Protected methods                       ===
	 * ===============================================
	*/

	void     (*destroy)      (data_t* data)                     = nullptr;
	uint32_t (*hash_user)    (const key_t*  key)                = nullptr;
	uint32_t (*hash_limited) (const key_t*  key, size_t keyLen) = nullptr;


	/** @brief Delete the element @a removed
	  *
	  * <B>Important</B>: this method will throw "illegal_delete" if
	  * @a removed is actually not removed from the hash. Making sure
	  * this method is only called with a removed element allows to
	  * use it without a lock on the hash itself.
	**/
	virtual uint32_t protDelete(elem_t* removed)
	{
		if (removed && removed->inserted())
			PWX_THROW("illegal_delete", "delete non-removed element", "Deleting an element that is not removed is illegal!")

		if (removed) {
			// Use double check technique to avoid illegal double deletion
			if (!removed->destroyed()) {
				PWX_LOCK(removed)
				if (!removed->destroyed()) {
					PWX_UNLOCK(removed)
					PWX_TRY(delete removed)
					PWX_THROW_PWXSTD_FURTHER ("delete",
						"Deleting an element failed.")
				} else
					PWX_UNLOCK(removed)
			} // end of if not destroyed yet
		} // end of having an element to delete
		return eCount.load(PWX_MEMORDER_ACQUIRE);
	}


	/** @brief use hashBuilder to generate a hash out of a key
	  * @param key pointer to the key to hash
	  * @return uint32_t hash of @a key
	**/
	virtual uint32_t protGetHash(const key_t* key) const noexcept
	{
		return hashBuilder(key, hash_user, hash_limited);
	}


	/** @brief return true if the specified position is vacated
	  *
	  * If @a index is out of range, false is returned.
	  *
	  * @param[in] index the index to check
	  * @return true if the position is marked as vacated, false otherwise.
	**/
	bool protIsVacated(const size_t index) const noexcept
	{
		if (index < this->sizeMax()) {
			PWX_LOCK_GUARD(hash_t, const_cast<hash_t*>(this))
			return hashTable[index] == vacated;
		}
		return false;
	}


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	CHashBuilder       hashBuilder; //!< instance that will handle the key generation
	std::atomic_size_t hashSize;    //!< number of places to maintain
	elem_t**           hashTable;   //!< the central array that is our hash
	char*              vacChar;     //!< alias pointer to get around the empty elem_t ctor restriction
	elem_t*            vacated;     //!< The Open Hash sets empty places to point at this.
	// Note: vacated is placed here, so clear(), disable_thread_safety() and
	//       enable_thread_safety() can be unified here as well. Otherwise
	//       the hashes would need individual functions that only differ
	//       in one compare.


private:

	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/


	/** @brief returns a read-only pointer to the element with the key @a key
	  *
	  * @param[in] key the key to search for
	  * @return a const pointer to the element or nullptr if the key could not be found.
	**/
	virtual elem_t* privGet(const key_t &key) const noexcept
	{
		size_t  keyIdx = privGetIndex(key);
		elem_t* xCurr  = hashTable[keyIdx];

		while (xCurr && (*xCurr != key))
			xCurr = xCurr->getNext();

		return xCurr;
	}


	/** @brief get an element by index
	  * @param[in] index the index of the element to find.
	  * @return read-only pointer to the element, or nullptr if there is no element with the specific index.
	**/
	virtual const elem_t* privGetByIndex(const int64_t index) const noexcept
	{
		// Mod index into range
		size_t xSize = sizeMax();
		size_t xIdx  = static_cast<size_t> (index < 0
											? xSize - (std::abs (index) % xSize)
											: index % xSize);

		// Unfortunately this results in xIdx equaling xSize
		// (which is wrong) if index is a negative multiple of
		// xSize:
		if (xIdx >= xSize)
			xIdx %= xSize;

		return protIsVacated(xIdx) ? nullptr : hashTable[xIdx];
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
	virtual size_t privGetIndex(const key_t &key) const noexcept PWX_VIRTUAL_PURE;


	// How collisions are resolved is a hash type specific matter
	virtual size_t privInsert(elem_t* elem) PWX_VIRTUAL_PURE;


	// This method must be implemented by the hash templates themselves, because
	// the outcome of a remove is different whether it is a chained (nullptr)
	// or open (vacated) hash.
	virtual elem_t* privRemove (size_t index) noexcept PWX_VIRTUAL_PURE;


	// The same applies to removal by key:
	virtual elem_t* privRemove (const key_t &key) noexcept PWX_VIRTUAL_PURE;


	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	const double maxLoadFactor; //!< When the load factor reaches this, the table is grown.
	const double dynGrowFactor; //!< When the table is automatically grown, it is grown by this factor.
}; // class VTHashBase

/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the hash.
**/
template<typename key_t, typename data_t, typename elem_t>
VTHashBase<key_t, data_t, elem_t>::~VTHashBase() noexcept
{
	PWX_LOCK_GUARD(hash_t, this)

	// Wipe hash table
	size_t  tabSize = sizeMax();
	size_t  pos     = 0;
	elem_t* xCurr   = nullptr;

	while (pos < tabSize) {
		if (!protIsVacated(pos)) {
			while (hashTable[pos]) {
				xCurr = hashTable[pos];
				hashTable[pos] = xCurr->getNext();
				delete xCurr;
			}
		}
		++pos;
	} // End of wiping

	// Delete hash table
	if (hashTable) {
		PWX_TRY(delete [] hashTable)
		PWX_CATCH_AND_FORGET(std::exception) // Can't do anything about that!
	}

	// Delete "vacated" sentry
	vacated = nullptr;
	if (vacChar) {
		PWX_TRY(delete vacChar)
		PWX_CATCH_AND_FORGET(std::exception) // dito
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
VTHashBase<data_t, elem_t> operator+ (const VTHashBase<data_t, elem_t> &lhs, const VTHashBase<data_t, elem_t> &rhs)
{
	VTHashBase<data_t, elem_t> result(lhs);

	if (&lhs != &rhs)
		PWX_TRY_PWX_FURTHER(result += rhs)

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
VTHashBase<data_t, elem_t> operator- (const VTHashBase<data_t, elem_t> &lhs, const VTHashBase<data_t, elem_t> &rhs)
{
	 VTHashBase<data_t, elem_t> result(lhs);

	if (&lhs != &rhs) {
		PWX_TRY_PWX_FURTHER(result -= rhs)
	} else
		result.clear();

	return result;
}


} // namespace pwx

#endif // PWX_LIBPWX_PWX_BASE_VHASH_H_INCLUDED
