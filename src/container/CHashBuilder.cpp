/***
  This file is part of the PrydeWorX Library (pwxLib).

  Copyright 2007 - 2017 Sven Eden

  The PrydeWorX Library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  The PrydeWorX Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with pwxLib; If not, see <http://www.gnu.org/licenses/>.

  History and Changelog are maintained in pwx.h
***/


#include "CHashBuilder.h"
#include "CRandom.h"


namespace pwx {


/// From pwx.cpp
extern CRandom RNG; // [R]andom [N]-Value [G]enerator


/** @brief get the current key length
  *
  * @return the currently set key length
  */
uint32_t CHashBuilder::getKeyLen() const noexcept
{
	return keyLen;
}

/** @brief seta new key length
  *
  * @param[in] keyLen_ the key length to use in the future
  */
void CHashBuilder::setKeyLen(uint32_t keyLen_) noexcept
{
	keyLen = keyLen_;
}


uint32_t CHashBuilder::hash_rng(const int16_t* key)      const noexcept { return RNG.hash(*key); }
uint32_t CHashBuilder::hash_rng(const uint16_t* key)     const noexcept { return RNG.hash(*key); }
uint32_t CHashBuilder::hash_rng(const int32_t* key)      const noexcept { return RNG.hash(*key); }
uint32_t CHashBuilder::hash_rng(const uint32_t* key)     const noexcept { return RNG.hash(*key); }
uint32_t CHashBuilder::hash_rng(const int64_t* key)      const noexcept { return RNG.hash(*key); }
uint32_t CHashBuilder::hash_rng(const uint64_t* key)     const noexcept { return RNG.hash(*key); }
uint32_t CHashBuilder::hash_rng(const float* key)        const noexcept { return RNG.hash(*key); }
uint32_t CHashBuilder::hash_rng(const double* key)       const noexcept { return RNG.hash(*key); }
uint32_t CHashBuilder::hash_rng(const long double* key)  const noexcept { return RNG.hash(*key); }

uint32_t CHashBuilder::hash_rng(const char* key)  const noexcept
{
	if (keyLen)
		return RNG.hash(key, keyLen);
	else
		return RNG.hash(key, strlen(key));
}

uint32_t CHashBuilder::hash_rng(const std::string* key) const noexcept
{
	if (keyLen && (keyLen > key->size()) ) {
		std::string subKey = key->substr(0, keyLen);
		return RNG.hash(subKey);
	} else
		return RNG.hash( *(const_cast<std::string*>(key)) );
	return 0;
}

} // namespace pwx
