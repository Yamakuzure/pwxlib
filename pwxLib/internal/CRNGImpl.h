#pragma once
#ifndef PWX_CRNGIMPL_H_INCLUDED
#define PWX_CRNGIMPL_H_INCLUDED 1

/** @internal
  * @file CRNGImpl.h
  *
  * @brief Implementations for CRandom
  *
  * @see CRandom.h for documentsation and history
  *
**/

#ifndef PWX_RANDOM_H_INCLUDED
# error "Do not include CRNGImpl.h, include CRandom.h!"
#endif // Check for main file

/* --- Static values --- */
#include "pwxLib/internal/CRandomStatics.h"

// --- Specialized methods are grouped together:

/* --- Hash Implementation */
#include "pwxLib/internal/CRandomHash.h"

/* --- Random Name Implementation */
#include "pwxLib/internal/CRandomWordConstants.h"
#include "pwxLib/internal/CRandomName.h"

/* --- Simple Noise Implementation */
#include "pwxLib/internal/CRandomNoise.h"

/* --- Random Number Implementation */
#include "pwxLib/internal/CRandomRandom.h"

/* --- Simplex Noise Implementation */
#include "pwxLib/internal/CRandomSimplex.h"


namespace pwx {
namespace rng {
// Note: The namespaces are documented in CRandom.h


// --- general member methods:

/** @brief return current seed
  *
  * This method simply returns the current Seed used to manipulate values
  * to calculate Simplex Noise and random names.
  *
  * @return Current Seed
**/
int32_t CRandom::getSeed() const
{
  return (seed);
}


/** @brief set Simplex Seed
  *
  * Set the seed to @a newSeed which will cause the simplex table to be
  * reinitialized.
**/
void CRandom::setSeed(int32_t newSeed)
{
  newSeed &= constants::fourthMaxInt;
  if (newSeed != seed)
    {
      seed = newSeed;
      for (int32_t i = 0; i < 256; i++)
        {
          spxTab[i]       = hash(seed + i) % 256;
          spxTab[i + 256] = spxTab[i];
        }
    }
}

} // End of namespace rng

} // End of namespace pwx

#endif // PWX_CRNGIMPL_H_INCLUDED
