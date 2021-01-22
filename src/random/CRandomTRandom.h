#pragma once
#ifndef PWX_LIBPWX_PWX_INTERNAL_CRANDOMTRANDOM_H_INCLUDED
#define PWX_LIBPWX_PWX_INTERNAL_CRANDOMTRANDOM_H_INCLUDED 1

/** @internal
  * @file CRandomTRandom.h
  *
  * @brief definition of a little template that does all the random() handling
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


#include <atomic>
#include <random>

#include "basic/pwx_compiler.h"

#include "math_helpers/MathHelpers.h"


/// @namespace pwx
namespace pwx {

/** @namespace private_
  * @internal
  * @brief all private functions, that are to be used in library compilation units only, reside here
**/
namespace private_ {

typedef std::random_device::result_type rand_t;

rand_t private_get_random32() noexcept;
rand_t private_get_random64() noexcept;
size_t private_random_str( char* dest, size_t min_, size_t max_ ) noexcept;


// random number selectors
template< class T, std::size_t SIZE > struct type_size_less_than : std::integral_constant< bool, ( sizeof( T ) < SIZE ) > {};
template< class T, std::size_t SIZE > struct type_size_at_least : std::integral_constant< bool, ( sizeof( T ) >= SIZE ) > {};
template< class T, typename std::enable_if< type_size_less_than< T, 64 >::value >::type* = nullptr >
	rand_t private_get_random() noexcept { return private_get_random32(); }
template< class T, typename std::enable_if< type_size_at_least< T, 64 >::value >::type* = nullptr >
	rand_t private_get_random() noexcept { return private_get_random64(); }


/// @internal random number handler. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
template< typename Tval > Tval private_random( Tval min_, Tval max_ ) noexcept {
	// Type borders:
	static const auto maxRandomValue   = static_cast<long double>( std::random_device::max() );
	static const auto minRandomValue   = static_cast<long double>( std::random_device::min() );
	static const auto randomValueRange = maxRandomValue - minRandomValue;
	static const Tval realMaxVal       = std::numeric_limits< Tval >::max();
	static const Tval realMinVal       = std::numeric_limits< Tval >::lowest();
	static const auto xMaxVal          = static_cast<long double>( realMaxVal );
	static const auto xMinVal          = static_cast<long double>( realMinVal );


	// Quick exit when no calculation can be done
	if ( areAlmostEqual( max_, min_ ) ) {
		return ( max_ );
	} else {
		// Step 1: Get a new random value
		rand_t randVal = private_get_random< Tval >();

		// Step 2: Reorder min_ and max_ and bring everything to long double and in range
		long double xMin = static_cast<long double>( std::min( min_, max_ ) );
		long double xMax = static_cast<long double>( std::max( min_, max_ ) );
		long double xVal = xMin // Start with this offset
		                   - minRandomValue // Shift so the range fits
		                   + ( static_cast<long double>( randVal ) * ( ( xMax - xMin ) / randomValueRange ) );

		// Step 3: Check Type borders:
		if ( xVal > xMaxVal ) xVal = xMaxVal;
		if ( xVal < xMinVal ) xVal = xMinVal;

		return static_cast<Tval>( xVal );
	}

}


} // namespace private_
} // namespace pwx
#endif // PWX_LIBPWX_PWX_INTERNAL_CRANDOMTRANDOM_H_INCLUDED

