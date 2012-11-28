#pragma once
#ifndef PWX_LIBPWX_PWX_INTERNAL_CRANDOMTHASH_H_INCLUDED
#define PWX_LIBPWX_PWX_INTERNAL_CRANDOMTHASH_H_INCLUDED 1

/** @internal
  * @file CRandomTHash.h
  *
  * @brief definition of three little templates that do all the hash() handling
  *
  * (c) 2007 - 2012 PrydeWorX
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

#include "pwx/general/macros.h"
#include "pwx/internal/CRandomConstants.h"
#include <type_traits>
#include <cmath>

namespace pwx {

namespace private_ {

/* ================================================================================================
Statistics with 10M Hashes (65535 for (u)int16_t) :
   ------------+------------+----------+------------+----------+--------------------------------------
   Type        | Unique rand|    Quota | Unique Hash|    Quota | Result
   ------------+------------+----------+------------+----------+--------------------------------------
   Long Double |  8,309,222 |  83.09 % |    397,556 |   4.78 % | Random is good, Hash is abysmal!
   Double      |  3,615,883 |  36.16 % |    115,769 |   3.20 % | Random is bad, Hash is a nightmare!
   Float       |  7,012,670 |  70.13 % |  1,366,255 |  19.48 % | Random is OK, Hash is a disaster!
   int16_t     |     41,296 |  63.01 % |     41,296 | 100.00 % | Random is OK, Hash is perfect!
   uint16_t    |     41,537 |  63.38 % |     41,537 | 100.00 % | Random is OK, Hash is perfect!
   int32_t     |  9,988,334 |  99.88 % |  9,988,334 | 100.00 % | Random is great, Hash is is perfect!
   uint32_t    |  9,988,339 |  99.88 % |  9,988,339 | 100.00 % | Random is great, Hash is is perfect!
   int64_t     |  9,988,393 |  99.88 % |  9,976,858 |  99.88 % | Random is great, Hash is is great!
   uint64_t    |  9,988,332 |  99.88 % |  9,976,620 |  99.88 % | Random is great, Hash is is great!
   C-String    | 10,000,000 | 100.00 % |  9,987,160 |  99.87 % | Random is perfect, Hash is is great!
   ------------+------------+----------+------------+----------+--------------------------------------
  Result: The floating point random number generation must be improved, and the hash building has
          some serious problem or is a serious problem in itself.

Statistics with 10M Hashes (65535 for (uint16_t)) after the following changes:
	1) Changed the random range for the floats to -5000.0 to +5000.0
	2) private_hash_str now converts the char* string into an uint8_t* buffer
   ------------+------------+----------+------------+----------+--------------------------------------
   Type        | Unique rand|    Quota | Unique Hash|    Quota | Result
   ------------+------------+----------+------------+----------+--------------------------------------
   Long Double |  9,961,939 |  99.62 % |  5,988,634 |  60.12 % | Random is great, Hash is OK!
   Double      |  9,981,779 |  99.81 % |  6,569,158 |  65.81 % | Random is great, Hash is OK!
   Float       |  7,017,313 |  70.17 % |  6,920,215 |  98.62 % | Random is OK, Hash is great!
   int16_t     |     41,379 |  63.14 % |     41,379 | 100.00 % | Random is OK, Hash is perfect!
   uint16_t    |     41,505 |  63.33 % |     41,505 | 100.00 % | Random is OK, Hash is perfect!
   int32_t     |  9,988,413 |  99.88 % |  9,988,413 | 100.00 % | Random is great, Hash is is perfect!
   uint32_t    |  9,988,459 |  99.88 % |  9,988,459 | 100.00 % | Random is great, Hash is is perfect!
   int64_t     |  9,988,454 |  99.88 % |  9,976,745 |  99.88 % | Random is great, Hash is is great!
   uint64_t    |  9,988,427 |  99.88 % |  9,976,682 |  99.88 % | Random is great, Hash is is great!
   C-String    | 10,000,000 | 100.00 % |  9,987,110 |  99.87 % | Random is perfect, Hash is is great!
  Result: The floating point hashing has improved greatly. But the random result for float suggests,
          that a higher integer range is needed. And the hash result for double and long double is
          far from being really good. Maybe it would be better tp mix the floating point bytes directly
          into an uint32_t hash.
          And for (u)int16_t a specialized hash loop is needed that simply goes from lowest to max.
   ================================================================================================ */
using constants::fullMaxInt;
using constants::fullMaxLong;

// These are needed for the float hashes conversion using modf/trunc
const long double maxInt64inLD = static_cast<long double>(std::numeric_limits<int64_t>::max());
const long double minInt64inLD = static_cast<long double>(std::numeric_limits<int64_t>::lowest());

/// @internal hash handler for integers. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
template<typename Tval>
uint32_t private_hash_int(Tval key) noexcept
{
	uint32_t xHash = 0;

	// --- int16_t and int32_t can use the same algorithm
	if (isSameType(int16_t, Tval) || isSameType(int32_t, Tval)) {
		/* ====================================================================
		 * === This is hash32shift() like described by Thomas Wang, 01/2007 ===
		 * ==================================================================== */
		int32_t xKey = static_cast<int32_t>(key);
		if (isSameType(int16_t, Tval))
			xKey ^= (xKey << 16) ^ (xKey << 8);
		xKey = (~xKey) + (xKey << 15);
		xKey ^= (xKey & fullMaxInt) >> 12; // xKey >>> 12
		xKey += xKey << 2;
		xKey ^= (xKey & fullMaxInt) >> 4; // xKey >>> 4
		xKey *= 2057;
		xKey ^= (xKey & fullMaxInt) >> 16; // xKey >>> 16
		xHash = static_cast<uint32_t> (xKey);
	}
	// --- uint16_t and uint32_t can use the same algorithm, too
	else if (isSameType(uint16_t, Tval) || isSameType(uint32_t, Tval)) {
		/* ========================================================================
		 * === This is hash() like described by Robert Jenkins, 6-shift version ===
		 * ======================================================================== */
		uint32_t xKey = static_cast<uint32_t>(key);
		if (isSameType(uint16_t, Tval))
			xKey ^= (xKey << 16) ^ (xKey << 8);
		xKey = (xKey  + 0x7ed55d16) + (xKey << 12);
		xKey = (xKey  ^ 0xc761c23c) ^ (xKey >> 19);
		xKey = (xKey  + 0x165667b1) + (xKey << 5);
		xKey = (xKey  + 0xd3a2646c) ^ (xKey << 9);
		xKey = (xKey  + 0xfd7046c5) + (xKey << 3);
		xHash = (xKey  ^ 0xb55a4f09) ^ (xKey >> 16);
	}
	// --- int64_t needs its own algorithm
	else if (isSameType(int64_t, Tval)) {
		/* =====================================================================
		 * === This is hash64shift() like described by Thomas Wang, 01/2007. ===
		 * === Modified to result in a unified uint32_t hash.                ===
		 * ===================================================================== */
		key  = (~key) + (key << 21);
		key ^= (key & fullMaxLong) >> 24; // key >>> 24
		key += (key << 3) + (key << 8);
		key ^= (key & fullMaxLong) >> 14; // key >>> 14
		key += (key << 2) + (key << 4);
		key ^= (key & fullMaxLong) >> 28; // key >>> 28
		xHash = static_cast<uint32_t> (0x00000000ffffffff & (key + (key >> 31)));
	}
	// --- int64_t needs its own algorithm
	else if (isSameType(uint64_t, Tval)) {
		/* ==============================================================
		 * === This is a 64 (or 2 x 32) to 32 bit shift hash function ===
		 * === like described by Thomas Wang, 01/2007                 ===
		 * ============================================================== */
		key  = (~key) + (key << 18);
		key ^= (key & fullMaxLong) >> 31; // key >>> 31
		key *= 21;
		key ^= (key & fullMaxLong) >> 11; // key >>> 11
		key += key << 6;
		key ^= (key & fullMaxLong) >> 22; // key >>> 22
		xHash = static_cast<uint32_t> (key);
	}

	return xHash;
}


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

	// The sum is then a nice uint32_t key:
	return private_hash_int<uint32_t>(sum);
}


/// @internal hash handler for floats. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
template<typename Tval>
uint32_t private_hash_flt(Tval key) noexcept
{
	static const size_t vSize = sizeof(Tval);
	static       char   buf[vSize + 1];
	memset(buf, 0, vSize + 1);
	memcpy(buf, reinterpret_cast<char*>(&key), vSize);
	return private_hash_str(buf, vSize);
}

} // namespace private_

} // namespace pwx
#endif // PWX_LIBPWX_PWX_INTERNAL_CRANDOMTHASH_H_INCLUDED


