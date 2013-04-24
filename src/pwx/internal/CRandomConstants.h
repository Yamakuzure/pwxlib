#pragma once
#ifndef PWX_LIBPWX_PWX_INTERNAL_CRANDOMCONSTANTS_H_INCLUDED
#define PWX_LIBPWX_PWX_INTERNAL_CRANDOMCONSTANTS_H_INCLUDED 1

/** @internal
  * @file CRandomConstants.h
  *
  * @brief Internal constants used by CRandom.cpp
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

#include <cstdint>
#include <limits>

namespace pwx {

/** @namespace pwx::constants
  *
  * @brief internal constants
  *
  * @see pwx for the namespace surrounding this
  * @see pwx::NameConstants for special constants used by the name generator
**/
namespace constants
{

/** The following are shortcuts for various limits used by CRandom: **/
const int32_t fullMaxInt    = std::numeric_limits<int32_t>::max();
const int32_t fullMinInt    = std::numeric_limits<int32_t>::lowest();
const int32_t halfMaxInt    = fullMaxInt >> 1;
const int32_t fourthMaxInt  = halfMaxInt >> 1;
const int64_t fullMaxLong   = std::numeric_limits<int64_t>::max();
const double  noiseMod      = static_cast<double>(fullMaxInt);


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

