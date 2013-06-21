#ifndef PWX_LIBPWX_PWX_CONTAINER_TOPENHASH_H_INCLUDED
#define PWX_LIBPWX_PWX_CONTAINER_TOPENHASH_H_INCLUDED

/** @file TOpenHash.h
  *
  * @brief @todo add brief description
  *
  * @todo add extensive description
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

#include <pwx/base/VTHashBase.h>

namespace pwx {

/** @class TOpenHash
  *
  * @brief Open hash container for variable types
  *
  * @todo describe properly
**/
template<typename key_t, typename data_t, typename elem_t = THashElement<key_t, data_t> >
class TOpenHash : public VTHashBase<key_t, data_t, elem_t>
{
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
	  * @param[in] initSize Initial size of the hash table.
	  * @param[in] keyLen_ Length of the key to limit hash generation.
	**/
	TOpenHash (size_t initSize, size_t keyLen_) noexcept:
		base_t(initSize, keyLen_)
	{ }


	/** @brief full constructor with key length
	  *
	  * The full constructor initializes an empty hash with user defined delete
	  * method, hashing method and key length. The initial size is the @a initSize
	  *
	  * @param[in] initSize The initial size of the table.
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash_ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
	  * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
	**/
	TOpenHash (size_t initSize,
				void (*destroy_) (data_t* data),
				uint32_t (*hash_) (const key_t* key, size_t keyLen),
				size_t keyLen_) noexcept :
		base_t(initSize, destroy_, hash_, keyLen_)
	{ }


	/** @brief full constructor without key length
	  *
	  * The full constructor initializes an empty hash with user defined delete
	  * method and hashing method withour key length. The initial size is the
	  * @a initSize
	  *
	  * @param[in] initSize The initial size of the table.
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash_ A pointer to a function that can hash the keys that are stored
	**/
	TOpenHash (size_t initSize,
				void (*destroy_) (data_t* data),
				uint32_t (*hash_) (const key_t* key)) noexcept :
		base_t(initSize, destroy_, hash_)
	{ }


	/** @brief limiting user method constructor
	  *
	  * This constructor only takes a destroy method and a hash
	  * method with explicit key length.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash__ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
	  * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
	**/
	TOpenHash( void (*destroy_) (data_t* data),
				uint32_t (*hash_) (const key_t* key, size_t keyLen),
				size_t keyLen_) noexcept :
		base_t(destroy_, hash_, keyLen_)
	{ }


	/** @brief user method constructor
	  *
	  * This constructor only takes a destroy method and a hash
	  * method without explicit key length.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] hash__ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
	**/
	TOpenHash( void (*destroy_) (data_t* data),
				uint32_t (*hash_) (const key_t* key)) noexcept :
		base_t(destroy_, hash_)
	{ }


	/** @brief destroy method constructor
	  *
	  * This constructor only takes a destroy method.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TOpenHash(uint32_t (*destroy_) (data_t* data)) noexcept :
		base_t(destroy_)
	{ }


	/** @brief key length constructor
	  *
	  * This constructor only takes a key length and sets the destroy
	  * and hash methods to nullptr.
	  *
	  * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
	**/
	TOpenHash(size_t keyLen_) noexcept :
		base_t (keyLen_)
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor uses the default constructor to set the data
	  * destroy method and the hash method to the null pointer with
	  * full key usage
	**/
	TOpenHash() noexcept :
		base_t ()
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
	TOpenHash (const hash_t &src) :
		base_t (src)
	{  }


	virtual ~TOpenHash() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	using base_t::add;
	using base_t::clear;
	using base_t::disable_thread_safety;
	using base_t::empty;
	using base_t::enable_thread_safety;
	using base_t::get;
	using base_t::getData;
	using base_t::grow;
	using base_t::size;
	using base_t::sizeMax;


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

	using base_t::protGetHash;
	using base_t::protIsVacated;


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	using base_t::eCount;
	using base_t::hashTable;
	using base_t::vacated;

private:

	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	/** @brief real index calculation
	  *
	  * This is the real index calculation that can be told where to
	  * stop. VTHashBase::get() needs a method to get the index to a
	  * key that does not end in vacated. But privInsert() is quite
	  * happy populating vacated positions.
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
	  * @param[in] key const reference of the key to evaluate
	  * @param[in] allowVacated if set to true, the method will return indexes from vacated positions and jump those otherwise.
	  * @return the index an element with this key would have in the table
	**/
	virtual size_t privGetIndex(const key_t &key, bool allowVacated) const noexcept
	{
		uint32_t xHash = this->protGetHash(&key);

		// Use multiplication method for the base index
		double dHash   = static_cast<double>(xHash) * 0.618;
		size_t idxBase = static_cast<size_t>(std::floor( (dHash - std::floor(dHash) * this->sizeMax()) ));

		// Use division probing for the stepping
		size_t tabSize = this->sizeMax();
		size_t idxStep = xHash % (tabSize - (tabSize % 2 ? 1 : 3));

		// idxStep must not be even
		if (!(idxStep % 2))
			idxStep += idxStep > 20 ? -1 : 1;

		// Move down until an appropriate value is found
		size_t oriStep = idxStep; // to revert if necessary
		while ((idxStep > 1) && !(tabSize % idxStep))
			idxStep -= 2;

		// Move up if this was not sucessful
		if (!(tabSize % idxStep)) {
			idxStep = oriStep + 2;
			while (!(tabSize % idxStep))
				idxStep += 2;
		}

		// Now probe the table until we are done or have found the key
		bool   isFound   = false;
		bool   isVacated = false;
		size_t pos       = 0;
		for (size_t i = 0; !isFound && (i < tabSize); ++i) {
			pos       = (idxBase + (idxStep * i)) % tabSize;
			isVacated = protIsVacated(pos);

			// we are done
			if (	// a) the hashTable has a nullptr at pos or
					(nullptr == hashTable[pos])
					// b) the hashTable has an element with the same key
				||	(!isVacated && (*hashTable[pos] == key))
					// c) allowVacated is true and the position is vacated
				||	(isVacated && allowVacated) )
				isFound = true;

		}

		// Now "pos" points at the first nullptr place found
		// or it points to a found element with the same key.

		return pos;

		// WARNING: If the hash table reaches load level 1.0 (all
		// places used) and the key is not in the table, the
		// whole thing would be screwed. That is one of the
		// strongest reason to automatically grow on reaching
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
	virtual size_t privGetIndex(const key_t &key) const noexcept
	{
		return privGetIndex(key, false);
	}


	/// @brief private insertion relying on privGetIndex() to resolve collisions
	virtual size_t privInsert(elem_t* elem)
	{
		PWX_LOCK_GUARD(hash_t, this)

		size_t  idx  = privGetIndex(elem->key, true); // Happy with vacated positions

		assert( ((nullptr == hashTable[idx]) || !protIsVacated(idx))
			&& "ERROR: TOpenHash::privGetIndex(key, true) returned an occupied position!");

		this->hashTable[idx] = elem;
		eCount.fetch_add(1, this->beThreadSafe.load(PWX_MEMORDER_RELAXED)
							? PWX_MEMORDER_RELEASE : PWX_MEMORDER_RELAXED);
		return this->size();
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
	virtual elem_t* privRemove (size_t index) noexcept
	{
		elem_t* result = nullptr;
		if ((index < this->sizeMax()) && hashTable[index] && !protIsVacated(index)) {

			PWX_LOCK_GUARD(hash_t, this)

			// Note: Open Hashes mark empty positions with the "vacated" sentry
			if ((index < this->sizeMax()) && hashTable[index] && !protIsVacated(index)) {
				result = hashTable[index];
				hashTable[index] = &vacated;
				result->remove();
				eCount.fetch_sub(1, this->beThreadSafe.load(PWX_MEMORDER_RELAXED)
									? PWX_MEMORDER_RELEASE : PWX_MEMORDER_RELAXED);
			} // End of outer check
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
	virtual elem_t* privRemove (const key_t &key) noexcept
	{
		return privRemove(privGetIndex(key));
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
TOpenHash<data_t, elem_t> operator+ (const TOpenHash<data_t, elem_t> &lhs, const TOpenHash<data_t, elem_t> &rhs)
{
	TOpenHash<data_t, elem_t> result(lhs);

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
TOpenHash<data_t, elem_t> operator- (const TOpenHash<data_t, elem_t> &lhs, const TOpenHash<data_t, elem_t> &rhs)
{
	 TOpenHash<data_t, elem_t> result(lhs);

	if (&lhs != &rhs) {
		PWX_TRY_PWX_FURTHER(result -= rhs)
	} else
		result.clear();

	return result;
}


} // namespace pwx


#endif // PWX_LIBPWX_PWX_CONTAINER_TOPENHASH_H_INCLUDED

