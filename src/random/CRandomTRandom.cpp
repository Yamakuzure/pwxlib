/**
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2020 PrydeWorX
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


#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "random/CRandomTRandom.h"


/// @internal random number generator. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
pwx::private_::rand_t pwx::private_::private_get_random() noexcept {
	rand_t randVal = privRandDev_();
	while ( randVal == lastRndValue.load( std::memory_order_acquire ) )
		randVal = privRandDev_();
	lastRndValue.store( randVal, std::memory_order_release );
	return randVal;
}


/// @internal random character handler. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
size_t pwx::private_::private_random_str( char* dest, size_t min_, size_t max_ ) noexcept {
	static const uint8_t lowA = 'a';
	static const uint8_t uppA = 'A';

	size_t pos = 0;

	if ( min_ || max_ ) {
		size_t xMin   = std::min( min_, max_ );
		size_t xMax   = std::max( min_, max_ );
		size_t finishRange = xMax - xMin;
		size_t finishDone  = finishRange;

		while ( ( pos < ( xMax - 1 ) )
		                && ( ( pos < xMin )
		                     || ( private_random( static_cast<size_t>( 0 ), finishRange ) <= finishDone ) )
		      ) {
			// Set up next character
			dest[pos] =  static_cast<uint8_t>( 0x000000ff & ( ( private_::private_get_random() ) % 26 ) )
			             + ( static_cast<uint8_t>( 0x000000ff & ( ( private_::private_get_random() ) %  2 ) ) ? lowA : uppA );

			// Advance pos and reduce finishDone if xMin is already met
			if ( ++pos >= xMin )
				--finishDone;
		} // end of building string

		// Set the final zero byte as well:
		dest[pos++] = 0x0;
	}

	return pos;
}

