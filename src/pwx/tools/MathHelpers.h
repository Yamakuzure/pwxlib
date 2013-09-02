#ifndef PWX_LIBPWX_PWX_TOOLS_MATHHELPERS_H_INCLUDED
#define PWX_LIBPWX_PWX_TOOLS_MATHHELPERS_H_INCLUDED

/** @file MathHelpers.h
  *
  * @brief This file defines some general mathematical helper functions
  *
  * The code handling floating point comparison is taken from
  * Bruce Dawson's article "Comparing Floating Point Numbers, 2012 Edition"
  * that can be read here:
  * http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
  *
  * (c) 2007 - 2013 PrydeWorX
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

#include <pwx/general/compiler.h>
#include <pwx/general/macros.h>

#include <cmath>
#include <cstdlib>
#include <limits>
#include <cassert>

namespace pwx
{


/* floating types unions
 * ---------------------
 * These unions are helpers that can be used to get data from
 * float, double and long double values. They are based on the
 * article "Comparing Floating Point Numbers, 2012 Edition",
 * written by Bruce Dawson, that can be read here:
 * http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
 *
 * Note: The bit sizes(*) are taken from the standard definition of the types,
 *       (see http://wikipedia.org/en/floating_point) and have been confirmed using
 *       std::numeric_limits. If they do not fit for you, please send me a note.
 * (*) The bit size defines which integer representation to use.
*/

/** @brief basic template for floating point helpers
  *
  * The basic template does nothing whatsoever, the
  * real functionality is added in the templates
  * specialization.
**/
template<typename Tf>
struct sFloatPoint
{
	typedef size_t Ti;

	explicit sFloatPoint(Tf num) : f(num) {}
	sFloatPoint() : f(static_cast<Tf>(0)) {}

	bool Negative()    const { return false; }
	Ti   RawMantissa() const { return static_cast<Ti>(0); }
	Ti   RawExponent() const { return static_cast<Ti>(0); }

	union {
		Ti i;
		Tf f;
	};

	static const int digits = 0;
	static       Tf  epsilon() noexcept { return static_cast<Ti>(0); }
};


/// @brief Specialized helper to handle floats
template<>
struct sFloatPoint<float>
{
	typedef int32_t Ti;

	explicit sFloatPoint(float num) : f(num) {}
	sFloatPoint() : f(static_cast<float>(0)) {}

	bool Negative()    const { return (i >> 31) != 0; }
	Ti   RawMantissa() const { return i & ((static_cast<Ti>(1) << 23) - 1); }
	Ti   RawExponent() const { return (i >> 23) & 0xff; }

	union {
		Ti    i;
		float f;
	};

	static const int   digits  = std::numeric_limits<float>::digits;
	static       float epsilon() noexcept {
		return std::numeric_limits<float>::epsilon();
	}
};


/// @brief Specialized helper to handle doubles
template<>
struct sFloatPoint<double>
{
	typedef int64_t Ti;

	explicit sFloatPoint(double num) : f(num) {}
	sFloatPoint() : f(static_cast<double>(0)) {}

	bool Negative()    const { return (i >> 63) != 0; }
	Ti   RawMantissa() const { return i & ((static_cast<Ti>(1) << 52) - 1); }
	Ti   RawExponent() const { return (i >> 52) & 0xffff; }

	union {
		Ti     i;
		double f;
	};

	static const int    digits  = std::numeric_limits<double>::digits;
	static       double epsilon() noexcept {
		return std::numeric_limits<double>::epsilon();
	}
};


/// @brief Specialized helper to handle long doubles
template<>
struct sFloatPoint<long double>
{
	typedef __int128_t Ti;

	explicit sFloatPoint(long double num) : f(num) {}
	sFloatPoint() : f(static_cast<long double>(0)) {}

	bool Negative()    const { return (i >> 127) != 0; }
	Ti   RawMantissa() const { return i & ((static_cast<Ti>(1) << 63) - 1); }
	Ti   RawExponent() const { return (i >> 63) & 0xffffffffffffffff; }

	union {
		Ti          i;
		long double f;
	};

	static const int         digits  = std::numeric_limits<long double>::digits;
	static       long double epsilon() noexcept {
		return std::numeric_limits<long double>::epsilon();
	}
};


/* ===================================================
 * === Functions that compare floating type values ===
 * ===================================================
*/
bool areAlmostEqual(float lhs, float rhs) noexcept;
bool areAlmostEqual(double lhs, double rhs) noexcept;
bool areAlmostEqual(long double lhs, long double rhs) noexcept;
// template dummy to enable areAlmostEqual() to be used with type_traits conditions
template<typename T>
bool areAlmostEqual(T lhs, T rhs) noexcept
{
	return lhs == rhs;
}


/* ==============================================
 * === General functions to help with degrees ===
 * ==============================================
*/

/**@brief return the normalized distance of two points in 2D space
 *
 * @param[in]  x1  X-Coordinate of the first point
 * @param[in]  y1  Y-Coordinate of the first point
 * @param[in]  x2  X-Coordinate of the second point
 * @param[in]  y2  Y-Coordinate of the second point
 * @return   The distance between both points, type is the same as arguments.
 *
 * This method returns the distance between two points.
**/
template<typename T>
inline T absDistance(const T x1, const T y1, const T x2, const T y2)
{
  if (isIntType(T) || isFloatType(T))
    // Do nothing if this isn't a type we can calculate with
    return (static_cast<T>(std::sqrt(std::pow(x2 - x1, 2.0) + std::pow(y2 - y1, 2.0))));
  else
    return ((T)0);
}


/**@brief return the normalized distance of two points in 3D space
 *
 * @param[in]  x1  X-Coordinate of the first point
 * @param[in]  y1  Y-Coordinate of the first point
 * @param[in]  z1  Z-Coordinate of the first point
 * @param[in]  x2  X-Coordinate of the second point
 * @param[in]  y2  Y-Coordinate of the second point
 * @param[in]  z2  Z-Coordinate of the second point
 * @return   The distance between both points, type is the same as the arguments.
 *
 * This method returns the distance between two points.
**/
template<typename T>
inline T absDistance(const T x1, const T y1, const T z1,
                     const T x2, const T y2, const T z2)
{
  if (isIntType(T) || isFloatType(T))
    // Do nothing if this isn't a type we can calculate with
    return (static_cast<T>(std::sqrt(std::pow(x2 - x1, 2.0) + std::pow(y2 - y1, 2.0) + std::pow(z2 - z1, 2.0))));
  else
    return ((T)0);
}


/** @brief degToRad
  *
  * Recalculate a @a degree into a radiant.
  *
  * Warning: Although this is a template, it is your responsibility to use
  * it with a sensible type! Type checking would have too much impact, so
  * if you receive weird/wrong results because of the type you use, then
  * you have been warned. ;-) (But of course the transformation to double is
  * done here, too. @see double getNormalizedDegree(const T degree))
  *
  * @param[in]  degree the value to re-calculate
  * @return the resulting radiant as a double
  *
**/
template<typename T> double degToRad(T degree)
{
	return (static_cast<double>(degree) * M_PIl / 180.);
}


/** @brief getNormalizedDegree (neutral version)
  *
  * This helper method makes sure, that the provided @a degree is in a range of
  * 0 <= result < 360
  *
  * Warning: Although this is a template, it is your responsibility to use
  * it with a sensible type! Type checking would have too much impact, so
  * if you receive weird/wrong results because of the type you use, then
  * you have been warned. ;-)
  * But I guess you won't get a problem, as your @a degree will be transformed
  * into a double anyway. So as long as your type can be converted, everything
  * should be in order.
  *
  * @param[in]  degree the degree you want to have re-calculated into a normalized range
  * @return the equivalent degree in a range between 0 and 360 as a double
  *
**/
template<typename T> double getNormalizedDegree(const T degree)
{
	double result = static_cast<double>(degree);

	if ((result >= 360.) || (result < 0.)) {
		// We have to mod here, but in a split
		if (result <  0.  ) {
			int32_t modVal = static_cast<int32_t>(result / 360.);
			double  reduct = SIGN(modVal) * 360. * modVal;
			result  = 360. - (-result - reduct);
		}
		// The reason for the split is, that the result can be
		// exactly 360.0 now while reduct is higher than 360
		if (result >= 360.) {
			int32_t modVal = static_cast<int32_t>(result / 360.);
			double  reduct = SIGN(modVal) * 360. * modVal;
			result -= reduct;
		}
	}

	assert ((result >= 0.) && (result < 360.) && "getNormalizedDegree() double mod failed!");

	return (result);
}


} // namespace pwx


#endif // PWX_LIBPWX_PWX_TOOLS_MATHHELPERS_H_INCLUDED

