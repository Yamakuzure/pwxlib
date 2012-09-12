#pragma once
#ifndef PWX_RANDOM_RANDOM_H
#define PWX_RANDOM_RANDOM_H

#ifndef PWX_RANDOM_H_INCLUDED
# error "Don't include CRandomRandom.h directly! Include pwxLib/CRandom.h instead!"
#endif

/** @internal
  * @file CRandomRandom.h
  *
  * @brief Implementations for CRandom - Random Number Generation
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

/** @brief Generate a random value of int32_t between 0 and @a max.
  *
  * This is the simplest method, which, when used without an argument,
  * behaves exactly like rand() would, but ensures, that the return
  * value is different than the one returned the last time any of
  * the random methods was used.
  *
  * if a negative @a max is submitted, the result will be @a max <= result <= 0.
  *
  * @param[in] max Specifies the desired maximum value (default is RAND_MAX).
  * @return A random value between 0 and @a max.
**/
int32_t CRandom::random(int32_t max)
{
  if (max == 0)
    return (0);
  else
    {
      int32_t result = rand();
      int32_t xMax   = abs(max);  // To ensure that xMax is positive

      while (result == lastRndValue)
        result = rand();
      lastRndValue  =  result;  // Last rand() result saved!

      if (xMax < RAND_MAX)
        // Now the result has to be recalculated into 0<=result<=max
        result  %= xMax + 1; // + 1 to include xMax

      return (result * SIGN(max));
    }
}


/** @brief Generate a random value of int32_t between @a min and @a max.
  *
  * This method returns a value between @a min and @a max if either
  * @a min < @a max, or @a max is negative and @a max < @a min.
  *
  * if a negative @a max is submitted, the result will be @a max <= result <= @a min.
  * This might be looking odd, but @a min <= result <= @a max with negative @a min and
  * @a max is possible, too. It is just a convenience.
  *
  * @param[in] min Specifies the desired minimum value.
  * @param[in] max Specifies the desired maximum value.
  * @return A random value between 0 and max.
**/
int32_t CRandom::random(int32_t min, int32_t max)
{
  if (max == min)
    return (max);
  else
    {
      int32_t xMin = ::std::min(min, max);

      // As the result is an int32_t, we can use random()
      return (random(::std::max(min, max) - xMin) + xMin);
    }
}


/** @brief Generate a random value of int64_t between 0 and @a max.
  *
  * @see CRandom::random(int32_t max) for documentation
  *
  * This one should be used with care! As rand() returns values between
  * 0 and the maximum number an int32_t can hold, recalculating the result
  * into a int64_t with a maximum higher than INT_MAX *will* eliminate
  * various numbers which can not be returned
  *
  * Example:<BR>
  * If you get a random number between 0 and 10, and then recalculate it
  * to be between 0 and 20, all odd numbers will be eliminated. So your
  * result *will* be even, no matter what you try!
  *
  * @param[in] max Specifies the desired maximum value.
  * @return A random value between 0 and max.
**/
int64_t CRandom::random(int64_t max)
{
  if (max == 0)
    return (0);
  else
    {
      /* There are two possibilities:
       * 1.: max <=  RAND_MAX
       *     Then just return the result of the integer version.
       * 2.: max >  RAND_MAX
       *     Return the result of the integer version, multiplied by
       *     the difference factor between max and RAND_MAX
       */
      if (::std::abs(max) <= RAND_MAX)
        // First Situation:
        return (static_cast<int64_t>(random(static_cast<int32_t>(max))));
      else
        {
          // Second Situation:
          int64_t     result  =  static_cast<int64_t>(random());
          long double factor  =  abs(static_cast<long double>(max) / static_cast<long double>(RAND_MAX));
          return (static_cast<int64_t>(pwx_round(result * factor * SIGN(max))));
        }
    }
}


/** @brief Generate a random value of int64_t between @a min and @a max.
  *
  * @see CRandom::random(int32_t min, int32_t max) for documentation
  *
  * Use this method if you need a random value between @a min and @a max,
  * but have a @a max that is larger than INT_MAX.
  *
  * @param[in] min Specifies the desired minimum value.
  * @param[in] max Specifies the desired maximum value.
  * @return A random value between min and max.
**/
int64_t CRandom::random(int64_t min, int64_t max)
{
  if (max == min)
    return (max);
  else
    {
      int64_t xMin = ::std::min(min, max);
      int64_t xMax = ::std::max(min, max);

      // Can we use random(max) ?
      if (xMin == 0L)
        return (random(xMax));
      // Can we use random(min) ?
      else if (xMax == 0L)
        return (random(xMin));
      else
        return (random(xMax - xMin) + xMin);
    }
}


/** @brief Generate a random value of float between 0 and @a max.
  *
  * @see CRandom::random(int32_t max) for documentation
  *
  * @param[in] max Specifies the desired maximum value.
  * @return A random value between 0 and max.
**/
float CRandom::random(float max)
{
  if (max == 0.0)
    return (0.0);
  else
    {
      int32_t iRand = random();
      return (max * (static_cast<float>(iRand) / static_cast<float>(RAND_MAX)));
    }
}


/** @brief Generate a random value of float between @a min and @a max.
  *
  * @see CRandom::random(int32_t min, int32_t max) for documentation
  *
  * @param[in] min Specifies the desired minimum value.
  * @param[in] max Specifies the desired maximum value.
  * @return A random value between min and max.
**/
float CRandom::random(float min, float max)
{
  if (max == min)
    return (max);
  else
    {
      float xMin = ::std::min(min, max);
      float xMax = ::std::max(min, max);

      // Can we use random(float max) ?
      if (xMin == 0.0)
        return (random(xMax));
      // Can we use random(float min ?
      else if (xMax == 0.0)
        return (random(xMin));
      else
        return (random(xMax - xMin) + xMin);
    }
}


/** @brief Generate a random value of double between 0 and @a max.
  *
  * @see CRandom::random(int32_t max) for documentation
  *
  * @param[in] max Specifies the desired maximum value.
  * @return A random value between 0 and max.
**/
double CRandom::random(double max)
{
  if (max == 0.0)
    return (0.0);
  else
    {
      int32_t iRand = random();
      return (max * (static_cast<double>(iRand) / static_cast<double>(RAND_MAX)));
    }
}


/** @brief Generate a random value of double between @a min and @a max.
  *
  * @see CRandom::random(int32_t min, int32_t max) for documentation
  *
  * @param[in] min Specifies the desired minimum value.
  * @param[in] max Specifies the desired maximum value.
  * @return A random value between min and max.
**/
double CRandom::random(double min, double max)
{
  if (max == min)
    return (max);
  else
    {
      double xMin = ::std::min(min, max);
      double xMax = ::std::max(min, max);

      // Can we use random(double max) ?
      if (xMin == 0.0)
        return (random(xMax));
      // Can we use random(double min ?
      else if (xMax == 0.0)
        return (random(xMin));
      else
        return (random(xMax - xMin) + xMin);
    }
}


/** @brief Generate a random value of long double between 0 and @a max.
  *
  * @see CRandom::random(int32_t max) for documentation
  *
  * @param[in] max Specifies the desired maximum value.
  * @return A random value between 0 and max.
**/
long double CRandom::random(long double max)
{
  if (max == 0.0)
    return (max);
  else
    {
      int32_t iRand = random();
      return (max * (static_cast<long double>(iRand) / static_cast<long double>(RAND_MAX)));
    }
}


/** @brief Generate a random value of long double between @a min and @a max.
  *
  * @see CRandom::random(int32_t min, int32_t max) for documentation.
  *
  * @param[in] min Specifies the desired minimum value.
  * @param[in] max Specifies the desired maximum value.
  * @return A randomvalue between min and max.
**/
long double CRandom::random(long double min, long double max)
{
  if (max == min)
    return (max);
  else
    {
      long double xMin = ::std::min(min, max);
      long double xMax = ::std::max(min, max);

      // Can we use random(long double max) ?
      if (xMin == 0.0)
        return (random(xMax));
      // Can we use random(long double min ?
      else if (xMax == 0.0)
        return (random(xMin));
      else
        return (random(xMax - xMin) + xMin);
    }
}

/* --------------------------------------- *
 * --- Private Methods Implementations --- *
 * --------------------------------------- */


} // End of namespace rng

} // End of namespace pwx

#endif // PWX_RANDOM_RANDOM_H
