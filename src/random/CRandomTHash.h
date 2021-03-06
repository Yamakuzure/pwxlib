#pragma once
#ifndef PWX_LIBPWX_PWX_INTERNAL_CRANDOMTHASH_H_INCLUDED
#define PWX_LIBPWX_PWX_INTERNAL_CRANDOMTHASH_H_INCLUDED 1

/** @internal
  * @file CRandomTHash.h
  *
  * @brief definition of three little templates that do all the hash() handling
  *
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
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


#include <type_traits>

#include "basic/compiler.h"
#include "basic/macros.h"

#include "math_helpers/MathHelpers.h"
#include "random/CRandomConstants.h"


/// @namespace pwx
namespace pwx {


/** @verbatim
===================================================================================================
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
   ------------+------------+----------+------------+----------+--------------------------------------
  Result: The floating point hashing has improved greatly. But the random result for float suggests,
          that a higher integer range is needed. And the hash result for double and long double is
          far from being really good. Maybe it would be better to mix the floating point bytes directly
          into an uint32_t hash.
          And for (u)int16_t a specialized hash loop is needed that simply goes from lowest to max.

Statistics with 65535 hashes for (u)int16_t after the following changes:
	1) Added a specialized builder that does no longer use random values but simply loops through the
	   range of int16_t.
   ------------+------------+----------+------------+----------+--------------------------------------
   Type        | Unique rand|    Quota | Unique Hash|    Quota | Result
   ------------+------------+----------+------------+----------+--------------------------------------
   int16_t     |        n/a |      n/a |     65,535 | 100.00 % | Hash is perfect!
   uint16_t    |        n/a |      n/a |     65,535 | 100.00 % | Hash is perfect!
   ------------+------------+----------+------------+----------+--------------------------------------
  Result: Every single value for (u)int16_t results in a unique hash value. fine!

Statistics with 10M hashes for float, double, long double after changing to a direct mix:
   ------------+------------+----------+------------+----------+--------------------------------------
   Type        | Unique rand|    Quota | Unique Hash|    Quota | Result
   ------------+------------+----------+------------+----------+--------------------------------------
   Long Double |  9,986,139 |  99.86 % |  1,472,965 |  14.75 % | Random is great, Hash is a disaster.
   Double      |  9,986,162 |  99.86 % |  9,216,337 |  92.29 % | Random is great, Hash is very good!
   Float       |  9,001,882 |  90.02 % |  9,001,882 | 100.00 % | Random is very good, Hash is perfect!
   ------------+------------+----------+------------+----------+--------------------------------------
  Result: The range for float should be enlarged again, I doubt it is the random algorithm. Further
          the new direct mixing seems to be sufficient on double and float, but does not work well
		  with long double. XORing 8/16 bytes into four with 4/2 bit shift seems to destroy certain
		  hash values. The solution tried next will be to do some more inter-mixing of the bytes.

Statistics with 100M (!) hashes for everything after rewriting the float hash function:
   ------------+-------------+----------+-------------+----------+--------------------------------------
   Type        | Unique rand |    Quota | Unique Hash |    Quota | Result
   ------------+-------------+----------+-------------+----------+--------------------------------------
   Long Double |  98,841,301 |  98.84 % |  96,714,170 |  97.85 % | Random is great, Hash is great!
   Double      |  98,845,336 |  98.85 % |  97,611,543 |  98.75 % | Random is great, Hash is great!
   Float       |  48,479,805 |  48.48 % |  45,315,059 |  93.47 % | Random is bad, Hash is very good!
   int32_t     |  98,845,792 |  98.85 % |  98,845,792 | 100.00 % | Random is great, Hash is is perfect!
   uint32_t    |  98,843,940 |  98.84 % |  98,843,940 | 100.00 % | Random is great, Hash is is perfect!
   int64_t     |  98,843,912 |  98.84 % |  97,715,073 |  98.86 % | Random is great, Hash is is great!
   uint64_t    |  98,845,554 |  98.85 % |  97,719,178 |  98.86 % | Random is great, Hash is is great!
   C-String    | 100,000,000 | 100.00 % |  98,723,200 |  99.87 % | Random is perfect, Hash is is great!
   ------------+-------------+----------+-------------+----------+--------------------------------------
  Result: I think this is the final version for now. The low number of unique random floats is mainly
          the result of not finding a good range for testing. But everything else looks splendid now.

Statistics with 10M hashes for float and (long) double after a slight change (phase shift) to the
hash algorithm, and a better production of random values:
   ------------+-------------+----------+-------------+----------+--------------------------------------
   Type        | Unique rand |    Quota | Unique Hash |    Quota | Result
   ------------+-------------+----------+-------------+----------+--------------------------------------
   Long Double |   9,988,379 |  98.88 % |   9,947,530 |  99.59 % | Random is great, Hash is great!
   Double      |   9,988,475 |  98.88 % |   9,952,956 |  99.64 % | Random is great, Hash is great!
   Float       |   9,067,506 |  90.68 % |   9,011,867 |  99.39 % | Random is very good, Hash is great!
   ------------+-------------+----------+-------------+----------+--------------------------------------
  Result: I think we can leave it now as it is.

Statistics with 10M hashes for float and (long) double after adding the sFloatPoint helper and using
that to get integer representations.
   ------------+-------------+----------+-------------+----------+--------------------------------------
   Type        | Unique rand |    Quota | Unique Hash |    Quota | Result
   ------------+-------------+----------+-------------+----------+--------------------------------------
   Long Double |   9,988,312 |  98.88 % |   9,976,647 |  99.88 % | Random is great, Hash is great!
   Double      |   9,988,392 |  98.88 % |   9,964,963 |  99.77 % | Random is great, Hash is great!
   Float       |   9,068,003 |  90.68 % |   9,068,003 | 100.00 % | Random is very good, Hash is perfect!
   ------------+-------------+----------+-------------+----------+--------------------------------------
  Result: There wasn't much space above the previous results anyway. However, the new algorithm
          is slightly faster. And hopefully it will solve the massive secondary clustering  of
          the open addressed hash container when using floating point keys.
          (Although, why would anyone use floating point types for hash keys anyway?)

===================================================================================================
  @endverbatim
**/


#ifndef PWX_NODOX

uint32_t private_hash_str( char const* key, size_t keyLen ) noexcept;

using constants::fullMaxInt;
using constants::fullMaxLong;

#endif // Do not document with doxygen


/// @internal hash handler for integers.
template< typename Tval >
uint32_t private_hash_int( Tval key ) noexcept {
	uint32_t xHash = 0;

	// --- int16_t and int32_t can use the same algorithm
	if ( isSameType( int16_t, Tval ) || isSameType( int32_t, Tval ) ) {
		/* ====================================================================
		 * === This is hash32shift() like described by Thomas Wang, 01/2007 ===
		 * ==================================================================== */
		auto xKey = static_cast<int32_t>( key );
		if ( isSameType( int16_t, Tval ) ) {
			xKey ^= ( xKey << 16 ) ^ ( xKey << 8 );
		}
		xKey         = ( ~xKey ) + ( xKey << 15 );
		xKey ^= ( xKey & fullMaxInt ) >> 12; // xKey >>> 12
		xKey += xKey << 2;
		xKey ^= ( xKey & fullMaxInt ) >> 4; // xKey >>> 4
		xKey *= 2057;
		xKey ^= ( xKey & fullMaxInt ) >> 16; // xKey >>> 16
		xHash        = static_cast<uint32_t> ( xKey );
	}
		// --- uint16_t and uint32_t can use the same algorithm, too
	else if ( isSameType( uint16_t, Tval ) || isSameType( uint32_t, Tval ) ) {
		/* ========================================================================
		 * === This is hash() like described by Robert Jenkins, 6-shift version ===
		 * ======================================================================== */
		auto xKey = static_cast<uint32_t>( key );
		if ( isSameType( uint16_t, Tval ) ) {
			xKey ^= ( xKey << 16 ) ^ ( xKey << 8 );
		}
		xKey          = ( xKey + 0x7ed55d16 ) + ( xKey << 12 );
		xKey          = ( xKey ^ 0xc761c23c ) ^ ( xKey >> 19 );
		xKey          = ( xKey + 0x165667b1 ) + ( xKey << 5 );
		xKey          = ( xKey + 0xd3a2646c ) ^ ( xKey << 9 );
		xKey          = ( xKey + 0xfd7046c5 ) + ( xKey << 3 );
		xHash         = ( xKey ^ 0xb55a4f09 ) ^ ( xKey >> 16 );
	}
		// --- int64_t needs its own algorithm
	else if ( isSameType( int64_t, Tval ) ) {
		/* =====================================================================
		 * === This is hash64shift() like described by Thomas Wang, 01/2007. ===
		 * === Modified to result in a unified uint32_t hash.                ===
		 * ===================================================================== */
		key   = ( ~key ) + ( key << 21 );
		key ^= ( key & fullMaxLong ) >> 24; // key >>> 24
		key += ( key << 3 ) + ( key << 8 );
		key ^= ( key & fullMaxLong ) >> 14; // key >>> 14
		key += ( key << 2 ) + ( key << 4 );
		key ^= ( key & fullMaxLong ) >> 28; // key >>> 28
		xHash = static_cast<uint32_t> ( 0x00000000ffffffff & ( key + ( key >> 31 ) ) );
	}
		// --- int64_t needs its own algorithm
	else if ( isSameType( uint64_t, Tval ) ) {
		/* ==============================================================
		 * === This is a 64 (or 2 x 32) to 32 bit shift hash function ===
		 * === like described by Thomas Wang, 01/2007                 ===
		 * ============================================================== */
		key   = ( ~key ) + ( key << 18 );
		key ^= ( key & fullMaxLong ) >> 31; // key >>> 31
		key *= 21;
		key ^= ( key & fullMaxLong ) >> 11; // key >>> 11
		key += key << 6;
		key ^= ( key & fullMaxLong ) >> 22; // key >>> 22
		xHash = static_cast<uint32_t> ( key );
	}
		// --- __int128_t must be split and combined
	else if ( isSameType( __int128_t, Tval ) ) {
		static const size_t halfSize = sizeof( Tval ) * 4;

		auto     left  = static_cast<int64_t>( ( key >> halfSize ) & 0xffffffffffffffff );
		auto     right = static_cast<uint64_t>( key & 0xffffffffffffffff );
		uint32_t lres  = private_hash_int< int64_t >( left );
		uint32_t rres  = private_hash_int< int64_t >( right );
		/* Now combine the hashes the following way:
		 * lres | left 16 bit  | right 16 bit
		 *   op |     xor      |      +
		 * rres | right 16 bit | left 16 bit
		 *  =>  | new left 16b | new right 16b
		*/
		uint32_t lnew = ( ( ( lres >> 16 ) & 0x0000ffff ) ^ ( rres & 0x0000ffff ) ) << 16;
		uint32_t rnew = ( ( rres >> 16 ) & 0x0000ffff ) + ( lres & 0x0000ffff );
		// Finally xor both together
		xHash = lnew ^ rnew;
	}

	return xHash;
}


/// @internal hash handler for float and (long) double. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
template< typename Tval >
uint32_t private_hash_flt( const Tval* key ) noexcept {
	typedef typename pwx::sFloatPoint< Tval >::Ti Ti;
	pwx::sFloatPoint< Tval >                      f( *key );
	return private_hash_int< Ti >( f.i );
}


} // namespace pwx


#endif // PWX_LIBPWX_PWX_INTERNAL_CRANDOMTHASH_H_INCLUDED
