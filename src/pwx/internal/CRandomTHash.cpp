#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include <pwx/internal/CRandomTHash.h>

#include <cstring>

namespace pwx {

namespace private_ {

/// @internal hash handler for strings. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
uint32_t private_hash_str(const char* key, size_t keyLen) noexcept
{
	uint32_t part  = 0, sum = 0;
	size_t   len   = keyLen ? keyLen : strlen(key);
	size_t   phase = 0, level = 0;

	// Quick exit if there is no string:
	if (0 == len)
		return 0;

	// We need unsigned values to not overflow anything:
	const uint8_t* keyBuf = reinterpret_cast<const uint8_t*>(key);

	// loop through the string and combine
	// groups of four to the sum
	for (size_t pos = 0; pos < len; ++pos) {
		phase = pos % 4;
		part |= static_cast<uint32_t>(keyBuf[pos] << (phase * 8));
		if (3 == phase) {
			level = pos % 3;
			if (1 == level)
				sum |= part >> 1;
			else if (2 == level)
				sum ^= part << 4;
			else
				sum += part >> 2;
			part = 0;
		} // End of adding result
	} // end of going through the string

	// If there is something in part left, it has to be added:
	if (part)
		sum += part >> 4;

	// The sum is then a nice uint32_t hash:
	return sum;
}


/// @internal generate a hash out of an uint8_t buffer. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
/// IMPORTANT: max keyLen is 32!
uint32_t private_hash_buf(const uint8_t* key, size_t keyLen) noexcept
{
	uint32_t xHash = 0, part = 0;
	size_t   phase = 0, tgtPos = 24;
	bool     isMixed = false;

	// Now mix the bytes into our hash key
	for (size_t pos = 0; pos < keyLen; ++pos) {
		if (key[pos]) {
			part |= static_cast<uint32_t>(key[pos]) << tgtPos;

			// If all positions are filled, add the part to xHash
			if (0 == tgtPos) {
				tgtPos = 24;

				// If there is a value in xHash, it becomes mixed, now:
				if (xHash)
					isMixed = true;

				// Now add our part to xHash
				phase = pos % 4;
				if (2 == phase)
					xHash |= part >> 1;
				else if (1 == phase)
					xHash ^= part << 4;
				else
					xHash += part >> 2;
				part = 0;
			} else
				tgtPos -= 8;
		} // End of having non-zero byte
	} // End of looping through the source

	// If there is something in part left, it has to be added:
	if (part) {
		if (xHash) {
			isMixed = true;
			xHash += part >> 4;
		}
		else
			xHash = part;
	}

	// Now return our mixed hash, or use the value, that came out
	// of less than at least five filled bytes, as a key:
	if (isMixed)
		return xHash;
	else
		return private_hash_int<uint32_t>(xHash);
}


} // namespace private_

} // namespace pwx