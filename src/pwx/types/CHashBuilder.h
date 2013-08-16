#ifndef PWX_LIBPWX_PWX_TYPES_THASHBUILDER_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_THASHBUILDER_H_INCLUDED

/** @file CHashBuilder.h
  *
  * @brief Class to get the hash of a value of variable type
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

#include <pwx/types/CException.h>
#include <pwx/general/macros.h>
#include <pwx/general/templates.h>

namespace pwx {


/** @class CHashBuilder
  *
  * @brief Simple class to generate hashes out of keys
  *
  * The class CHashBuilder can be used to conveniently get the
  * hash value of a value of a variable type.
  * This can be obtained by either providing a hashing function
  * or, if the type is supported by RNG, using RNG.hash() on
  * the value to hash.
  *
  * Currently RNG supports 8, 16, 32 and 64 bit signed and unsigned
  * integers, float, double, C-Strings (char*) and std::string.
  * Further a key length can be set when using either a user defined
  * hashing function that supports a key length, or when using char*
  * or std::string with RNG.hash().
  *
**/
class CHashBuilder
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/


	/** @brief key length constructor
	  *
	  * If your key is a C-String or std::string, you can set
	  * an optional key length. The hashing done by RNG, or
	  * a provided hash method, will then be limited to this
	  * maximum key length; or whatever a provided hash method
	  * does with the key length.
	  *
	  * @param[in] keyLen_ optional key length
	**/
	CHashBuilder(uint32_t keyLen_) noexcept :
		keyLen(keyLen_)
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor uses the default constructor to
	  * set the key length to zero.
	**/
	CHashBuilder() noexcept :
		CHashBuilder(0)
	{ }


	/** @brief copy constructor copying the key length
	  * @param[in] src source reference
	**/
	CHashBuilder(const CHashBuilder &src) :
		keyLen(src.keyLen)
	{ }


	virtual ~CHashBuilder() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	uint32_t getKeyLen()           const noexcept;
	void     setKeyLen(uint32_t keyLen_) noexcept;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief operator() to build a hash from a key
	  *
	  * This operator serves as a dispatcher. The default is
	  * to use RNG.hash() on the provided key, but a user defined
	  * hashing method can be used instead.
	  *
	  * Please note that RNG.hash can only handle [u]int16_t,
	  * [u]int32_t, [u]int64_t, float, double, char* and std::string
	  * keys. You must provide your own hashing method on any other
	  * types or the apllication will be forced to terminate.
	  *
	  * The operator has two optional parameters. The first is a pointer
	  * to a general hash method, the second to a hash method taking
	  * a second uint32_t argument called keyLen; although it is up to
	  * the method what it uses the key length for.
	  *
	  * @param[in] key pointer to the key to hash
	  * @param[in] hash_user a user defined method to hash a single key (default: nullptr)
	  * @param[in] hash_limited a user defined method to hash a single key with keyLen argument (default: nullptr)
	  * @return the hash result as an unsigned 32 bit integer.
	**/
	template<typename key_t>
	uint32_t operator()(const key_t* key,
			uint32_t (*hash_user)    (const key_t* key)                  = nullptr,
			uint32_t (*hash_limited) (const key_t* key, uint32_t keyLen) = nullptr
		) const noexcept
	{
		if (hash_user)
			return hash_user(key);
		else if (hash_limited)
			return hash_limited(key, keyLen);
		else
			return this->hash_pwx(key);
	}



protected:

	/* ===============================================
	 * === Protected methods                       ===
	 * ===============================================
	*/

	/** @brief internal hash method for type RNG can handle
	  *
	  * If a type not supported is used, an exception is thrown
	  * triggering std::terminate(). This is done because the
	  * condition implies a serious bug. Either because the user
	  * has not provided a hashing function or because operator()
	  * is faulty.
	  *
	  * @param[in] pointer to the key to hash
	  * @return the hash modded to the hash size
	**/
	template<typename key_t>
	uint32_t hash_pwx(const key_t* key) const noexcept
	{
		compAssert( (
				isIntType(key_t)
			||	isFloatType(key_t)
			||	isSameType(key_t, char)
			||	isSameType(key_t, std::string) )
		  && "ERROR: unsupported type used in CHashBuilder::hash_pwx(key* key)");
        if ( isIntType(key_t)
		  || isFloatType(key_t)
		  || isSameType(key_t, char)
		  || isSameType(key_t, std::string) )
		  return this->hash_rng(key);
		else
			PWX_THROW("abort", "Illegal type in hash_pwx()", "hash_pwx() was called with a type unsupported by RNG!")
	}


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/


private:

	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	uint32_t hash_rng(const int16_t*     key) const noexcept;
	uint32_t hash_rng(const uint16_t*    key) const noexcept;
	uint32_t hash_rng(const int32_t*     key) const noexcept;
	uint32_t hash_rng(const uint32_t*    key) const noexcept;
	uint32_t hash_rng(const int64_t*     key) const noexcept;
	uint32_t hash_rng(const uint64_t*    key) const noexcept;
	uint32_t hash_rng(const float*       key) const noexcept;
	uint32_t hash_rng(const double*      key) const noexcept;
	uint32_t hash_rng(const long double* key) const noexcept;
	uint32_t hash_rng(const char*        key) const noexcept;
	uint32_t hash_rng(const std::string* key) const noexcept;
	template<typename key_t>
	uint32_t hash_rng(key_t* key);


	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	uint32_t keyLen = 0; //!< optional key length for C-String and std::string keys (0 = use strlen() on C-Strings)


}; // class CHashBuilder

#if defined(PWX_EXPORTS)
/// @brief ~CHashBuilder default destructor.
CHashBuilder::~CHashBuilder() noexcept
{ }
#endif

} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_THASHBUILDER_H_INCLUDED

