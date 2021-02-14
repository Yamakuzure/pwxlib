/**
  * This file is part of the PrydeWorX Library (pwxLib).
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


#include <cstring>

#include "basic/pwx_macros.h"

#include "random/CRandomTHash.h"


/// @namespace pwx
namespace pwx {

/// @internal hash handler for strings.
uint32_t private_hash_str( char const* key, size_t keyLen ) noexcept {
	uint32_t part = 0, sum = 0;
	size_t   len  = keyLen ? keyLen : strlen( key );
	size_t   phase, level;

	// Quick exit if there is no string:
	if ( 0 == len ) {
		return 0;
	}

	// We need unsigned values to not overflow anything:
	auto keyBuf = reinterpret_cast<const uint8_t*>( key );

	// loop through the string and combine
	// groups of four to the sum
	for ( size_t pos = 0 ; pos < len ; ++pos ) {
		phase = pos % 4;
		part |= static_cast<uint32_t>( keyBuf[pos] << ( phase * 8 ) );
		if ( 3 == phase ) {
			level = pos % 3;
			if ( 1 == level ) {
				sum |= part >> 1;
			} else if ( 2 == level ) {
				sum ^= part << 4;
			} else {
				sum += part >> 2;
			}
			part  = 0;
		} // End of adding result
	} // end of going through the string

	// If there is something in part left, it has to be added:
	if ( part ) {
		sum += part >> 4;
	}

	// The sum is then a nice uint32_t hash:
	return sum;
}


} // namespace pwx
