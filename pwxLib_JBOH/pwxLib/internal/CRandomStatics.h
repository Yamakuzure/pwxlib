#pragma once
#ifndef PWX_RANDOM_STATICS_H
#define PWX_RANDOM_STATICS_H

#ifndef PWX_RANDOM_H_INCLUDED
# error "Don't include CRandomStatic.h directly! Include pwxLib/CRandom.h instead!"
#endif

/** @internal
  * @file CRandomStatics.h
  *
  * @brief Implementations for CRandom - Static Value Initialization
  *
  * @see CRandom.h for documentsation and history
  *
**/

namespace pwx {
namespace rng {
// Note: The namespaces are documented in CRandom.h

/** @namespace pwx::rng::constants
  *
  * @brief constants for usage by CRandom
  *
  * @see pwx::rng for the namespace surrounding this
  * @see pwx::rng::NameConstants for special constants used by the name generator
**/
namespace constants
{

/** The following are shortcuts for various limits used by CRandom: **/
const int32_t fullMaxInt    = ::std::numeric_limits<int32_t>::max();
const int32_t fullMinInt    = ::std::numeric_limits<int32_t>::min();
const int32_t halfMaxInt    = fullMaxInt >> 1;
const int32_t fourthMaxInt  = halfMaxInt >> 1;
const int64_t fullMaxLong   = ::std::numeric_limits<int64_t>::max();
const double  noiseMod      = static_cast<double>(halfMaxInt);


/** Simplex manipulation grid **/
const int32_t spxGrTab[32][4] =
{
  { 1,  1,  0,  1}, { 1, -1,  0,  1}, {-1,  1,  0,  1}, {-1, -1,  0,  1}, // Last Index for 1D:  3 (Mod  4)
  { 0,  1,  1,  1}, { 0, -1,  1,  1}, { 1,  0,  1,  1}, {-1,  0,  1,  1}, // Last Index for 2D:  7 (Mod  8)
  { 0,  1, -1,  1}, { 0, -1, -1,  1}, { 1,  0, -1,  1}, {-1,  0, -1,  1}, // Last Index for 3D: 11 (Mod 12)
  { 1,  1,  0, -1}, {-1,  1,  0, -1}, { 1, -1,  0, -1}, {-1, -1,  0, -1}, { 0,  1,  1, -1}, { 0, -1,  1, -1},
  { 1,  0,  1, -1}, {-1,  0,  1, -1}, { 0,  1, -1, -1}, { 0, -1, -1, -1}, { 1,  0, -1, -1}, {-1,  0, -1, -1},
  { 1,  1,  1,  0}, { 1,  1, -1,  0}, { 1, -1,  1,  0}, { 1, -1, -1,  0}, {-1,  1,  1,  0}, {-1,  1, -1,  0},
  {-1, -1,  1,  0}, {-1, -1, -1,  0}                                      // Last Index for 4D: 31 (Mod 32)
};


/** Simplex Transformation grid **/
const int32_t spxTrTab[64][4] =
{
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
const double spxSkew[3][2] =
{
  {0.36602540378443859658830206171842L, 0.21132486540518713447056597942719L}, // 2D
  {0.32258064516129031362723367237777L, 0.16393442622950821219163231035054L}, // 3D
  {0.30901699437494745126286943559535L, 0.13819660112501050419631098975515L}  // 4D
};

} // End of namespace constants

} // End of namespace rng

} // End of namespace pwx

#endif // PWX_RANDOM_STATICS_H
