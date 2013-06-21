#ifndef PWX_LIBPWX_PWX_CONTAINER_TCHAINHASH_H_INCLUDED
#define PWX_LIBPWX_PWX_CONTAINER_TCHAINHASH_H_INCLUDED

/** @file TChainHash.h
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

/// @brief two-type enum to allow the chained hash to use dynamic hashing types
enum eChainHashMethod {
	CHM_Division = 1,  //!< Use the division method
	CHM_Multiplication //!< Use the multiplication method
};

/** @class TChainHash
  *
  * @brief Chained hash container for variable types
  *
  * @todo describe properly
**/
template<typename key_t, typename data_t, typename elem_t = THashElement<key_t, data_t> >
class TChainHash : public VTHashBase<key_t, data_t, elem_t>
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef VTHashBase<key_t, data_t, elem_t> base_t;     //!< Base type of the hash
	typedef TChainHash<key_t, data_t, elem_t> hash_t;     //!< Type of this hash


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
	TChainHash (size_t initSize, size_t keyLen_) noexcept:
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
	TChainHash (size_t initSize,
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
	TChainHash (size_t initSize,
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
	TChainHash( void (*destroy_) (data_t* data),
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
	TChainHash( void (*destroy_) (data_t* data),
				uint32_t (*hash_) (const key_t* key)) noexcept :
		base_t(destroy_, hash_)
	{ }


	/** @brief destroy method constructor
	  *
	  * This constructor only takes a destroy method.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TChainHash(uint32_t (*destroy_) (data_t* data)) noexcept :
		base_t(destroy_)
	{ }


	/** @brief key length constructor
	  *
	  * This constructor only takes a key length and sets the destroy
	  * and hash methods to nullptr.
	  *
	  * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
	**/
	TChainHash(size_t keyLen_) noexcept :
		base_t (keyLen_)
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor uses the default constructor to set the data
	  * destroy method and the hash method to the null pointer with
	  * full key usage
	**/
	TChainHash() noexcept :
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
	TChainHash (const hash_t &src) :
		base_t (src)
	{  }


	virtual ~TChainHash() noexcept;


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
		PWX_LOCK_GUARD(hash_t, this)
		size_t oldSize = this->sizeMax();
		if (targetSize > oldSize) {
			setHashMethod(targetSize);
			PWX_TRY_PWX_FURTHER(return base_t::grow(targetSize));
		}
		return oldSize;
	}


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


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	using base_t::eCount;
	using base_t::hashTable;


private:

	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	/** @brief get the index of a key
	  *
	  * In a chained hash the index is calculated directly using either
	  * the division method or the multiplication method.
	  * Which method to use is determined when setting the size of the
	  * hash table upon creation and whenever the table is enlarged.
	  *
	  * This method just returns the index and does not check whether
	  * the wanted key actually exists in the hash table.
	  *
	  * @param[in] key const reference of the key to evaluate
	  * @return the index an element with this key would have in the table
	**/
	virtual size_t privGetIndex(const key_t &key) const noexcept
	{
		uint32_t xHash = this->protGetHash(&key);

		if (CHM_Division == CHMethod)
			return xHash % this->sizeMax();
		else {
			double dHash = static_cast<double>(xHash) * 0.618;
			return static_cast<size_t>(std::floor( (dHash - std::floor(dHash) * this->sizeMax()) ));
		}
	}


	/// @brief private insertion doing bucket filling to resolve collisions
	virtual size_t privInsert(elem_t* elem)
	{
		PWX_LOCK_GUARD(hash_t, this)

		size_t  idx  = privGetIndex(elem->key);
		elem_t* root = this->hashTable[idx];
		if (root) {
			elem_t* next = root->getNext();
			while (next) {
				root = next;
				next = root->getNext();
			}
			root->setNext(elem);
		} else
			this->hashTable[idx] = elem;

		eCount.fetch_add(1, this->beThreadSafe.load(PWX_MEMORDER_RELAXED)
							? PWX_MEMORDER_RELEASE : PWX_MEMORDER_RELAXED);
		return this->size();
	}


	/** @brief removes the element with the specified index and returns it.
	  *
	  * Always the first element is removed, the next will become "root"
	  * or the place is set to nullptr if this is the last.
	  *
	  * If the index is out of bounds, nullptr is returned.
	  *
	  * @param[in] index the index to remove
	  * @return a pointer to the removed element or nullptr if no such element exists
	**/
	virtual elem_t* privRemove (size_t index) noexcept
	{
		elem_t* result = nullptr;
		if ((index < this->sizeMax()) && hashTable[index]) {

			PWX_LOCK_GUARD(hash_t, this)

			// Note: Chained Hashes do not use "vacated" sentries, no check needed.
			if ((index < this->sizeMax()) && hashTable[index]) {
				result = hashTable[index];
				hashTable[index] = result->getNext();
				result->remove();
				eCount.fetch_sub(1, this->beThreadSafe.load(PWX_MEMORDER_RELAXED)
									? PWX_MEMORDER_RELEASE : PWX_MEMORDER_RELAXED);
			} // End of outer check
		} // End of outer check

		return result;
	}


	/** @brief removes the element with the specified key and returns it.
	  *
	  * @param[in] key the key of the element to remove has
	  * @return a pointer to the removed element or nullptr if no such element exists
	**/
	virtual elem_t* privRemove (const key_t &key) noexcept
	{
		PWX_LOCK_GUARD(hash_t, this)

		size_t index   = privGetIndex(key);
		elem_t* result = hashTable[index];
		elem_t* prev   = nullptr;

		while (result && (*result != key)) {
			prev   = result;
			result = result->getNext();
		}

		// Now if result is set, there is either a prev or
		// result is the root of the bucket:
		if (result) {
			if (prev)
				prev->removeNext();
			else {
				hashTable[index] = result->getNext();
				result->remove();
			}
			eCount.fetch_sub(1, this->beThreadSafe.load(PWX_MEMORDER_RELAXED)
								? PWX_MEMORDER_RELEASE : PWX_MEMORDER_RELAXED);
		}

		return result;
	}


	/// @brief internal method to set the hashing method according to @a targetSize
	void setHashMethod(size_t targetSize) noexcept
	{
		CHMethod = CHM_Multiplication; // default the safe one

		// Test 1: Even sizes can't use the division method
		if (targetSize % 2) {
			// Test 2: For the division method to safely work, the size
			// should be a prime number with a good distance to the
			// next smaller and larger 2^x values:
			size_t lowerBound = 64;
			size_t upperBound = 128;

			// find bounds:
			while (lowerBound > targetSize) {
				upperBound = lowerBound;
				lowerBound /= 2;
			}

			while (upperBound < targetSize) {
				lowerBound = upperBound;
				upperBound *= 2;
			}

			size_t middle  = (lowerBound + upperBound) / 2;
			size_t midDist = middle > targetSize ? middle - targetSize : targetSize - middle;

			if (midDist < std::min((targetSize - lowerBound) / 2, (upperBound - targetSize) / 2) ) {
				// Test 3: (almost) a prime number
				// For this to test the size is simply divided by the first
				// 8 odd numbers (but 15) and must not be devidable by more
				// than one
				int divided = 0;
				for (size_t divisor = 3; (divided < 2) && (divisor < 20); divisor += 2) {
					// 15 is already covered by 3 and 5
					if (15 != divisor) {
						if (!(targetSize % divisor))
							++divided;
					}

					// Now if divided is lower than 2, the division method can be used
					if (divided < 2)
						CHMethod = CHM_Division;
				} // End of checking divisors
			} // End of valid Test 2
		} // End of checking for an odd number
	}


	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	eChainHashMethod CHMethod = CHM_Division; //!< Which Hashing method is used

}; // class TChainHash

/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the hash.
**/
template<typename key_t, typename data_t, typename elem_t>
TChainHash<key_t, data_t, elem_t>::~TChainHash() noexcept
{ /* all done in base_t dtor */ }


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
TChainHash<data_t, elem_t> operator+ (const TChainHash<data_t, elem_t> &lhs, const TChainHash<data_t, elem_t> &rhs)
{
	TChainHash<data_t, elem_t> result(lhs);

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
TChainHash<data_t, elem_t> operator- (const TChainHash<data_t, elem_t> &lhs, const TChainHash<data_t, elem_t> &rhs)
{
	 TChainHash<data_t, elem_t> result(lhs);

	if (&lhs != &rhs) {
		PWX_TRY_PWX_FURTHER(result -= rhs)
	} else
		result.clear();

	return result;
}

} // namespace pwx


#endif // PWX_LIBPWX_PWX_CONTAINER_TCHAINHASH_H_INCLUDED

