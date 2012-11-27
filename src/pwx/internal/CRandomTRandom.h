#pragma once
#ifndef PWX_LIBPWX_PWX_INTERNAL_CRANDOMTRANDOM_H_INCLUDED
#define PWX_LIBPWX_PWX_INTERNAL_CRANDOMTRANDOM_H_INCLUDED 1

/** @internal
  * @file CRandomTRandom.h
  *
  * @brief definition of a little template that does all the random() handling
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

#include <random>

namespace pwx {

/** @internal
  * @namespace private_
  * @brief all private functions that are to be used in library compilation units only reside here
**/
namespace private_ {

typedef std::random_device::result_type rand_t;

const long double maxRandomValue   = static_cast<long double>(std::random_device::max());
const long double minRandomValue   = static_cast<long double>(std::random_device::min());
const long double randomValueRange = maxRandomValue - minRandomValue;

static std::random_device privRandDev_;
static rand_t lastRndValue = 0;  //!< The last generated random value


/// @internal random number generator. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
/// Important: RNG has to be _LOCKED_ before using this function!
rand_t private_get_random() noexcept
{
	rand_t randVal = privRandDev_();
	while (randVal == lastRndValue)
		randVal = privRandDev_();
	lastRndValue = randVal;
	return randVal;
}


/// @internal random number handler. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
/// Important: RNG has to be _LOCKED_ before using this function!
template<typename Tval>
Tval private_random(Tval min_, Tval max_) noexcept
{
	// Type borders:
	static const Tval realMaxVal = std::numeric_limits<Tval>::max();
	static const Tval realMinVal = std::numeric_limits<Tval>::lowest();
	static const long double xMaxVal = static_cast<long double>(realMaxVal);
	static const long double xMinVal = static_cast<long double>(realMinVal);

	// Quick exit when no calculation can be done
	if (max_ == min_)
		return (max_);
	else {
		// Step 1: Get a new random value
		rand_t randVal = private_get_random();

		// Step 2: Reorder min_ and max_ and bring everything to long double and in range
		long double xMin = static_cast<long double>(std::min(min_, max_));
		long double xMax = static_cast<long double>(std::max(min_, max_));
		long double xVal = xMin // Start with this offset
		                 - minRandomValue // Shift so the range fits
						 + (static_cast<long double>(randVal) * ((xMax - xMin) / randomValueRange));

		// Step 3: Check Type borders:
		if (xVal > xMaxVal) xVal = xMaxVal;
		if (xVal < xMinVal) xVal = xMinVal;

		return static_cast<Tval>(xVal);
	}

}

/// @internal random character handler. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
/// Important: RNG has to be _LOCKED_ before using this function!
size_t private_random_str(char* dest, size_t min_, size_t max_) noexcept
{
	static const uint8_t lowA = 'a';
	static const uint8_t uppA = 'A';

	size_t pos = 0;

	if (min_ || max_ ) {
		size_t xMin   = std::min(min_, max_);
		size_t xMax   = std::max(min_, max_);
		size_t finishRange = xMax - xMin;
		size_t finishDone  = finishRange;

		while ( (pos < (xMax - 1))
			 && ( (pos < xMin)
			   || (private_random(static_cast<size_t>(0), finishRange) <= finishDone))
			  ) {
			// Set up next character
			dest[pos] =  static_cast<uint8_t>(0x000000ff & ((private_::private_get_random()) % 26))
			          + (static_cast<uint8_t>(0x000000ff & ((private_::private_get_random()) %  2)) ? lowA : uppA);

			// Advance pos and reduce finishDone if xMin is already met
			if (++pos >= xMin)
				--finishDone;
		} // end of building string

		// Set the final zero byte as well:
		dest[pos++] = 0x0;
	}

	return pos;
}

} // namespace private_
} // namespace pwx
#endif // PWX_LIBPWX_PWX_INTERNAL_CRANDOMTRANDOM_H_INCLUDED

