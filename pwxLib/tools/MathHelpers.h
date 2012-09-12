#pragma once
#ifndef PWX_MATHHELPERS_H_INCLUDED
#define PWX_MATHHELPERS_H_INCLUDED 1

/** @file MathHelpers.h
  *
  * @brief Some general mathematical helper functions
  *
  *
  * (c) 2007-2012 Sven Eden, PrydeWorX
  * @author Sven Eden, PrydeWorX - Hamburg, Germany
  *         yamakuzure@users.sourceforge.net
  *         http://pwxlib.sourceforge.net
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
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.0.1     2010-11-06  sed, PrydeWorX  First Design
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.0.1   2011-01-05  sed, PrydeWorX  Some updates to compile with MS Visual C++ 2008
  * 0.7.0.2   2011-01-06  sed, PrydeWorX  Fixed two serious bugs:
  *                                       The first bug was an order issue in the single value version of
  *                                       pwx::MathHelpers::getNormalizedDegree, that caused the assertion to
  *                                       fail when called with an angle of exactly -360.0 degrees. The second
  *                                       bug was a comparision issue in the two value version, that caused
  *                                       undefined behaviour when called with a value of exactly 360.0
  *                                       degrees.
  * 0.7.0.3   2011-01-23  sed, PrydeWorX  pow and round are now wrapped for MSVC++ compatibility.
  * 0.7.0.4   2011-03-06  sed, PrydeWorX  Welcome to 2011 !!
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version.
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0.1   2011-09-14  sed, PrydeWorX  Fixed a bug that could let getNormalizedDegree fail if an argument is an exact
  *                                       product of -360.0 and 2+.
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3.1   2012-01-31  sed, PrydeWorX  Added wavelengthToRGB() and rgbToWavelength()
  * 0.8.3.2   2012-02-01  sed, PrydeWorX  Added frequencyToRGB() and rgbToFrequency()
  * 0.8.3.3   2012-02-03  sed, PrydeWorX  Added applyGamma(), unapplyGamma() and prototypes as there are enough functions now
  * 0.8.3.4   2012-02-04  sed, PrydeWorX  Moved color related methods to ColorHelpers.h
  * 0.8.5     2012-03-01  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

#include "pwxLib/internal/MSVCwarnPush.h"
#include <cmath>
#include <cstdlib>
#include "pwxLib/internal/MSVCwarnPop.h"
#include "pwxLib/tools/DefaultDefines.h"

/** @namespace pwx
  *
  * @brief Namespace for everything PrydeWorX related
  *
  * All PrydeWorX projects reside in this namespace and it's sub-namespaces.
**/
namespace pwx
{
/** @namespace pwx::MathHelpers
  *
  * @brief Namespace for global math helper functions
  *
  * This namespace consists of funtions helping out with certain mathematical problems.
**/
namespace MathHelpers
{

/* --- Prototypes ---
 *====================
*/
template<typename T> PWX_INLINE double  degToRad(T degree);
template<typename T> PWX_INLINE double  getNormalizedDegree(const T degree);
template<typename T> PWX_INLINE int32_t getNormalizedDegree(const T degree, int32_t precision);


/* --- Implementations ---
 *=========================
*/

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
  double result = static_cast<double>(degree); // -1080
  if ((result >= 360.0L) || (result < 0.0L))
    {
      // We have to mod here, but in a split
      if (result <  0.0L  )
        {
          int32_t modVal = static_cast<int32_t>(result / 360.0L);
          double  reduct = SIGN(modVal) * 360.0L * modVal;
          result  = 360.0L - (-result - reduct);
        }
      // The reason for the split is, that result can be exactly 360.0 now while reduct is higher than 360
      if (result >= 360.0L)
        {
          int32_t modVal = static_cast<int32_t>(result / 360.0L);
          double  reduct = SIGN(modVal) * 360.0L * modVal;
          result -= reduct;
        }
    }

  assert ((result >= 0) && (result < 360.0L) && "getNormalizedDegree() double mod failed!");

  return (result);
}


/** @brief getNormalizedDegree (precision version)
  *
  * This helper method makes sure, that the provided @a degree is in a range of
  * 0 <= result < (360 * 10^@a precision)
  * The purpose is to make sure that the result is a valid index of the sine and
  * cosine tables of SCT. So I daresay it won't be usefull for outside use.
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
  * @param[in] precision precision for the degree, should be lastPrec for the wanted purpose
  * @return the equivalent degree in a range between 0 and 360 as an _integer_
  *
**/
template<typename T> int32_t getNormalizedDegree(const T degree, int32_t precision)
{
  assert(precision >= 0);
  double  multipl = static_cast<double>(pwx_pow(10., precision));
  int32_t normDeg = static_cast<int32_t>(pwx_round(degree * multipl));
  int32_t tabSize = static_cast<int32_t>(pwx_round(360. * multipl));

  if      (normDeg >= tabSize) normDeg %= tabSize;
  else if (normDeg <  0      ) normDeg  = tabSize - (-normDeg % tabSize);

  return (normDeg);
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
  return (static_cast<double>(degree) * M_PIl / 180.0L);
}


} // End of namespace MathHelpers

} // End of namespace pwx

#endif // PWX_MATHHELPERS_H_INCLUDED
