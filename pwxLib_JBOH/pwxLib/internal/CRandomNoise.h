#pragma once
#ifndef PWX_RANDOM_NOISE_H
#define PWX_RANDOM_NOISE_H

#ifndef PWX_RANDOM_H_INCLUDED
# error "Don't include CRandomNoise.h directly! Include pwxLib/CRandom.h instead!"
#endif

/** @internal
  * @file CRandomNoise.h
  *
  * @brief Implementations for CRandom - Simple Noise Generation
  *
  * @see CRandom.h for documentsation and history
  *
**/

namespace pwx {
namespace rng {
// Note: The namespaces are documented in CRandom.h

/* -------------------------------------- *
 * --- Public Methods Implementations --- *
 * -------------------------------------- */

/** @brief noise with one dimension
  *
  * This method calculates a noise value between -1.0 and 1.0 out of one integer
  *
  * @param[in] x paramter to transform
  * @return double noise value between -1.0 and +1.0
**/
double CRandom::noise(int32_t x) const
{
  return (1.0 - (static_cast<double>(hash(x) & constants::fullMaxInt) / constants::noiseMod));
}


/** @brief noise with two dimensions
  *
  * This method calculates a noise value between -1.0 and 1.0 out of two integers
  *
  * @param[in] x paramter to transform
  * @param[in] y paramter to transform
  * @return double noise value between -1.0 and +1.0
**/
double CRandom::noise(int32_t x, int32_t y) const
{
  return (1.0 - (static_cast<double>(
                      (hash(x) & constants::halfMaxInt)
                    + (hash(y) & constants::halfMaxInt)
                  ) / constants::noiseMod));
}


/** @brief noise with three dimensions
  *
  * This method calculates a noise value between -1.0 and 1.0 out of three integers
  *
  * @param[in] x paramter to transform
  * @param[in] y paramter to transform
  * @param[in] z paramter to transform
  * @return double noise value between -1.0 and +1.0
**/
double CRandom::noise(int32_t x, int32_t y, int32_t z) const
{
  return (1.0 - (static_cast<double>(
                      (hash(x) & constants::halfMaxInt)
                    + (hash(y) & constants::fourthMaxInt)
                    + (hash(z) & constants::fourthMaxInt)
                  ) / constants::noiseMod));
}


/** @brief noise with four dimensions
  *
  * This method calculates a noise value between -1.0 and 1.0 out of four integers
  *
  * @param[in] x paramter to transform
  * @param[in] y paramter to transform
  * @param[in] z paramter to transform
  * @param[in] w paramter to transform
  * @return double noise value between -1.0 and +1.0
**/
double CRandom::noise(int32_t x, int32_t y, int32_t z, int32_t w) const
{
  return (1.0 - (static_cast<double>(
                      (hash(x) & constants::fourthMaxInt)
                    + (hash(y) & constants::fourthMaxInt)
                    + (hash(z) & constants::fourthMaxInt)
                    + (hash(w) & constants::fourthMaxInt)
                  ) / constants::noiseMod));
}


/* --------------------------------------- *
 * --- Private Methods Implementations --- *
 * --------------------------------------- */
double CRandom::noiseD(double x) const
{
  return noise(doubToInt(x));
}

double CRandom::noiseD(double x, double y) const
{
  return noise(doubToInt(x), doubToInt(y));
}

double CRandom::noiseD(double x, double y, double z) const
{
  return noise(doubToInt(x), doubToInt(y), doubToInt(z));
}

double CRandom::noiseD(double x, double y, double z, double w) const
{
  return noise(doubToInt(x), doubToInt(y), doubToInt(z), doubToInt(w));
}

int32_t CRandom::doubToInt(double val) const
  {
    // Bring val in range if it is between -1000 and +1000
    while (  ((val < 0.) && (val > -1000.))   // negative range
           ||((val > 0.) && (val <  1000.)) ) // positive range
      val *= 1000.;

    // Bring val in range if it is smaller than min int or larger than maval int
    while (  ( val < static_cast<double>(constants::fullMinInt))   // negative range
           ||( val > static_cast<double>(constants::fullMaxInt)) ) // positive range
      val /= 10.;

    return static_cast<int32_t>(pwx_round(val));
  }

} // End of namespace rng

} // End of namespace pwx

#endif // PWX_RANDOM_Noise_H
