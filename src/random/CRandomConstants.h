#ifndef PWX_LIBPWX_PWX_INTERNAL_CRANDOMCONSTANTS_H_INCLUDED
#define PWX_LIBPWX_PWX_INTERNAL_CRANDOMCONSTANTS_H_INCLUDED 1
#pragma once

/** @file CRandomConstants.h
  *
  * @brief Internal constants used by CRandom.cpp
  *
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
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


#include <limits>

#include "basic/pwx_types.h"


namespace pwx {


/** @namespace pwx::constants
  *
  * @brief internal constants
  *
  * @see pwx for the namespace surrounding this
  * @see pwx::NameConstants for special constants used by the name generator
**/
namespace constants {

/* The following are shortcuts for various limits used by CRandom: */
const int32_t fullMaxInt    = std::numeric_limits<int32_t>::max();    //!< Shortcut to the int32_t maximum value
const int32_t fullMinInt    = std::numeric_limits<int32_t>::lowest(); //!< Shortcut to the int32_t minimum value
const int32_t halfMaxInt    = fullMaxInt >> 1;            //!< Shortcut to the half of the int32_t maximum value
const int32_t fourthMaxInt  = halfMaxInt >> 1;            //!< Shortcut to the half of the int32_t minimum value
const int64_t fullMaxLong   = std::numeric_limits<int64_t>::max();    //!< Shortcut to the int64_t maximum value 
const double  noiseMod      = static_cast<double>( fullMaxInt );      //!< fullMaxInt as double for noise modulation


/** Simplex manipulation grid **/
const int32_t spxGrTab[32][4] = {
    { 1,  1,  0,  1}, { 1, -1,  0,  1}, {-1,  1,  0,  1}, {-1, -1,  0,  1}, // Last Index for 1D:  3 (Mod  4)
    { 0,  1,  1,  1}, { 0, -1,  1,  1}, { 1,  0,  1,  1}, {-1,  0,  1,  1}, // Last Index for 2D:  7 (Mod  8)
    { 0,  1, -1,  1}, { 0, -1, -1,  1}, { 1,  0, -1,  1}, {-1,  0, -1,  1}, // Last Index for 3D: 11 (Mod 12)
    { 1,  1,  0, -1}, {-1,  1,  0, -1}, { 1, -1,  0, -1}, {-1, -1,  0, -1}, { 0,  1,  1, -1}, { 0, -1,  1, -1},
    { 1,  0,  1, -1}, {-1,  0,  1, -1}, { 0,  1, -1, -1}, { 0, -1, -1, -1}, { 1,  0, -1, -1}, {-1,  0, -1, -1},
    { 1,  1,  1,  0}, { 1,  1, -1,  0}, { 1, -1,  1,  0}, { 1, -1, -1,  0}, {-1,  1,  1,  0}, {-1,  1, -1,  0},
    {-1, -1,  1,  0}, {-1, -1, -1,  0}                                      // Last Index for 4D: 31 (Mod 32)
};


/** Simplex Transformation grid **/
const int32_t spxTrTab[64][4] = {
    {0, 1, 2, 3}, {0, 1, 3, 2}, {0, 0, 0, 0}, {0, 2, 3, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 2, 3, 0},
    {0, 2, 1, 3}, {0, 0, 0, 0}, {0, 3, 1, 2}, {0, 3, 2, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 3, 2, 0},
    {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
    {1, 2, 0, 3}, {0, 0, 0, 0}, {1, 3, 0, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 3, 0, 1}, {2, 3, 1, 0},
    {1, 0, 2, 3}, {1, 0, 3, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 0, 3, 1}, {0, 0, 0, 0}, {2, 1, 3, 0},
    {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
    {2, 0, 1, 3}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {3, 0, 1, 2}, {3, 0, 2, 1}, {0, 0, 0, 0}, {3, 1, 2, 0},
    {2, 1, 0, 3}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {3, 1, 0, 2}, {0, 0, 0, 0}, {3, 2, 0, 1}, {3, 2, 1, 0}
};


/** Simplex Skewing/Unskewing Values **/
const double spxSkew[3][2] = {
    {0.36602540378443859658830206171842L, 0.21132486540518713447056597942719L}, // 2D
    {0.32258064516129031362723367237777L, 0.16393442622950821219163231035054L}, // 3D
    {0.30901699437494745126286943559535L, 0.13819660112501050419631098975515L}  // 4D
};

} // End of namespace constants

} // namespace pwx

#endif // PWX_LIBPWX_PWX_INTERNAL_CRANDOMCONSTANTS_H_INCLUDED

