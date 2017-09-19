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


#include "MathHelpers.h"


namespace pwx {


// Internal functions to do the checking
namespace private_ {


/// @internal check floating point values using Ulps and abs diff
template<typename Tf, typename Ti>
bool private_areAlmostEqualUlpsAndAbs(
		const Tf &lhs, const Tf &rhs,
		Tf maxDiff, Ti maxUlpsDiff) noexcept
{
	// Check if the numbers are really close -- needed
	// when comparing numbers near zero.
	Tf absDiff = std::fabs(lhs - rhs);
	if (absDiff <= maxDiff)
		return true;

	sFloatPoint<Tf> uLhs(lhs);
	sFloatPoint<Tf> uRhs(rhs);

	// Different signs means they do not match.
	if (uLhs.Negative() != uRhs.Negative())
		return false;

	// Find the difference in ULPs.
	Ti ulpsDiff = abs(uLhs.i - uRhs.i);
	if (ulpsDiff <= maxUlpsDiff)
		return true;

	return false;
}

/// @internal check floating point values using relative and diff
template<typename Tf>
bool private_areAlmostEqualRelativeAndAbs(
		const Tf &lhs, const Tf &rhs,
		Tf maxDiff, Tf maxRelDiff) noexcept
{
	// Check if the numbers are really close -- needed
	// when comparing numbers near zero.
	Tf diff = std::fabs(lhs - rhs);
	if (diff <= maxDiff)
		return true;

	Tf flhs = fabs(lhs);
	Tf frhs = fabs(rhs);
	Tf largest = (frhs > flhs) ? frhs : flhs;

	if (diff <= largest * maxRelDiff)
		return true;
	return false;
}


/* Ignore -Wfloat-equal for this method. The warning is useless here,
 * because the operator== is only used to catch signed zeros.
 * All other cases, meaning all cases where this warning is indeed very
 * valuable, are already excluded by the sign test.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"


/// @internal template to dispatch to the correct AlmostEqual function
template<typename Tf>
bool private_dispatchAlmostEqual(const Tf &lhs, const Tf &rhs) noexcept
{
	typedef typename pwx::sFloatPoint<Tf>::Ti Ti;

	// return at once if the sign differs, but use
	// operator== to catch -0 versus +0
	if (SIGN(lhs) != SIGN(rhs))
		return lhs == rhs;

	Tf fLhs = std::fabs(lhs);
	Tf fRhs = std::fabs(rhs);

	// With very small numbers, the relative check is used
	if ((fLhs <= 1.0) && (fRhs <= 1.0))
		return private_areAlmostEqualRelativeAndAbs<Tf>(
				lhs, rhs, 0.f, pwx::sFloatPoint<Tf>::epsilon());

	// If both numbers are equal or larger than 4, we need to find the magnitude
	if ((fLhs >= 4.0) && (fRhs >= 4.0)) {
		Tf mag = std::fmin(std::log2(fLhs), std::log2(fLhs));
		return private_areAlmostEqualUlpsAndAbs<Tf, Ti>(
				lhs, rhs, pwx::sFloatPoint<Tf>::epsilon() * mag,
				static_cast<Ti>(mag * 2));
	}

	// With numbers between 1.0 and 4.0 a normal ULPS check is in order
	if ((fLhs >= 1.0) && (fRhs >= 1.0) && (fLhs <= 4.0) && (fRhs <= 4.0))
		return private_areAlmostEqualUlpsAndAbs<Tf, Ti>(
				lhs, rhs, pwx::sFloatPoint<Tf>::epsilon(), 2);

	// If none of these valid, the numbers are simply different.
	return false;
}


#pragma GCC diagnostic pop


} // namespace private_


/** @brief test two floats whether they are near enough to be considered equal
  *
  * This functions takes two floats and tests whether they can be considered
  * equal. For this the tolerated difference and the tolerated ULPS
  * difference are determined. Then an internal function can check these values.
  *
  * @param[in] lhs left hand side value
  * @param[in] rhs right hand side value
  * @return true if bot values can be considered equal although their representation might differ
**/
bool areAlmostEqual(const float lhs, const float rhs) noexcept
{
	return private_::private_dispatchAlmostEqual<float>(lhs, rhs);
}


/** @brief test two doubles whether they are near enough to be considered equal
  *
  * This functions takes two doubles and tests whether they can be considered
  * equal. For this the tolerated difference and the tolerated ULPS
  * difference are determined. Then an internal function can check these values.
  *
  * @param[in] lhs left hand side value
  * @param[in] rhs right hand side value
  * @return true if bot values can be considered equal although their representation might differ
**/
bool areAlmostEqual(const double lhs, const double rhs) noexcept
{
	return private_::private_dispatchAlmostEqual<double>(lhs, rhs);
}


/** @brief test two long doubles whether they are near enough to be considered equal
  *
  * This functions takes two long doubles and tests whether they can be considered
  * equal. For this the tolerated difference and the tolerated ULPS
  * difference are determined. Then an internal function can check these values.
  *
  * @param[in] lhs left hand side value
  * @param[in] rhs right hand side value
  * @return true if bot values can be considered equal although their representation might differ
**/
bool areAlmostEqual(const long double lhs, const long double rhs) noexcept
{
	return private_::private_dispatchAlmostEqual<long double>(lhs, rhs);
}

} // namespace pwx
