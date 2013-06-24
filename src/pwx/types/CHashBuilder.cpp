#include <pwx/worker/CRandom.h>

#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include <pwx/types/CHashBuilder.h>

namespace pwx {

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
