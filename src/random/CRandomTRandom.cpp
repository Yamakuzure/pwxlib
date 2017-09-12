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

