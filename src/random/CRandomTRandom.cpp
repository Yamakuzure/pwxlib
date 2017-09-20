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


#include "CRandomTRandom.h"


/// @internal random number generator. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
pwx::private_::rand_t pwx::private_::private_get_random() noexcept
{
	rand_t randVal = privRandDev_();
	while (randVal == lastRndValue.load(PWX_MEMORDER_ACQUIRE))
		randVal = privRandDev_();
	lastRndValue.store(randVal, PWX_MEMORDER_RELEASE);
	return randVal;
}


/// @internal random character handler. NEVER EXPOSE OR USE OUTSIDE CRandom.cpp !
size_t pwx::private_::private_random_str(char* dest, size_t min_, size_t max_) noexcept
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

