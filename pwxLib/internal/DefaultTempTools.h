#pragma once
#ifndef PWX_DDTEMPTOOLS_H_INCLUDED
#define PWX_DDTEMPTOOLS_H_INCLUDED 1

#ifndef PWX_DEFINES_H_INCLUDED
# error "Do not include DefaultTempTools.h directly, include pwxDefaultDefines.h instead!"
#endif // Check for default defines

/** @internal
  * @file DefaultTempTools.h
  *
  * @brief location of helping templates and classes for pwx namespace.
  *
  * @see pwxDefaultDefines.h for details and history
  *
**/

// We need those for some helper macros
#include "pwxLib/internal/MSVCwarnPush.h"
#if defined(_GLIBCXX_INCLUDE_AS_CXX0X) || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(_MSC_VER)
#  include <type_traits>
#else
#  include <tr1/type_traits>
#endif
#include "pwxLib/internal/MSVCwarnPop.h"



/** @namespace pwx
  *
  * @brief Namespace for everything PrydeWorX related
  *
  * All PrydeWorX projects reside in this namespace and it's sub-namespaces.
**/
namespace pwx
{


/** @brief compile time assertion
  *
  * This little trick is taken from Andrei Alexandrescu: 'Modern C++ Design'.
  * It can be used to check an expression at compile time.
  * Of course this does not substitute cassert completely, only expressions
  * that are ready while the compiler does its work can be checked with this.
**/
template <bool> struct pwxCompAssert;
template <>     struct pwxCompAssert<true> {};
#define compAssert(expression) (::pwx::pwxCompAssert<(expression) != 0>())


/** @brief type trait to check for same type
**/
#if defined(_GLIBCXX_TR1_TYPE_TRAITS) || defined(_TYPE_TRAITS_)
// TR1 type traits have been included:
#define isSameType(a, b) (::std::tr1::is_same<a, b>::value)
#elif defined(_GLIBCXX_TYPE_TRAITS)
// C++0x type traits have been included:
#define isSameType(a, b) (::std::is_same<a, b>::value)
#endif //isSameType


/** @brief type trait to check whether a pointer is given or not
**/
#if defined(_GLIBCXX_TR1_TYPE_TRAITS) || defined(_TYPE_TRAITS_)
// TR1 type traits have been included:
#define isPointer(a) (::std::tr1::is_pointer<a>::value)
#elif defined(_GLIBCXX_TYPE_TRAITS)
// C++0x type traits have been included:
#define isPointer(a) (::std::is_pointer<a>::value)
#endif // _GLIBCXX_TR1_TYPE_TRAITS

/** @brief type trait to check whether a type is an integral type (integers)
**/
#if defined(_GLIBCXX_TR1_TYPE_TRAITS) || defined(_TYPE_TRAITS_)
// TR1 type traits have been included:
#define isIntType(a) (::std::tr1::is_integral<a>::value)
#elif defined(_GLIBCXX_TYPE_TRAITS)
// C++0x type traits have been included:
#define isIntType(a) (::std::is_integral<a>::value)
#endif // _GLIBCXX_TR1_TYPE_TRAITS

/** @brief type trait to check whether a type is a floating point type
**/
#if defined(_GLIBCXX_TR1_TYPE_TRAITS) || defined(_TYPE_TRAITS_)
// TR1 type traits have been included:
#define isFloatType(a) (::std::tr1::is_floating_point<a>::value)
#elif defined(_GLIBCXX_TYPE_TRAITS)
// C++0x type traits have been included:
#define isFloatType(a) (::std::is_floating_point<a>::value)
#endif // _GLIBCXX_TR1_TYPE_TRAITS

/** @brief type trait to check whether a type is an array type
**/
#if defined(_GLIBCXX_TR1_TYPE_TRAITS) || defined(_TYPE_TRAITS_)
// TR1 type traits have been included:
#define isArrayType(a) (::std::tr1::is_array<a>::value)
#elif defined(_GLIBCXX_TYPE_TRAITS)
// C++0x type traits have been included:
#define isArrayType(a) (::std::is_array<a>::value)
#endif // _GLIBCXX_TR1_TYPE_TRAITS

// The following methods need cmath, which is for once included by default in DefaultDefines.h

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
#   if defined(_GLIBCXX_CMATH) || defined(_CMATH_)
    return (static_cast<T>(::std::sqrt(pwx_pow(x2 - x1, 2) + pwx_pow(y2 - y1, 2))));
#   else
    return (static_cast<T>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2))));
#   endif // use cmath
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
#   if defined(_GLIBCXX_CMATH) || defined(_CMATH_)
    return (static_cast<T>(::std::sqrt(pwx_pow(x2 - x1, 2) + pwx_pow(y2 - y1, 2) + pwx_pow(z2 - z1, 2))));
#   else
    return (static_cast<T>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2))));
#   endif // use cmath
  else
    return ((T)0);
}


/**@brief check whether @a value is between @a low and @a high
 *
 * @param[in]  value the value to test
 * @param[in]  low the lower border, tested with >=
 * @param[in]  high the higher border, tested with <
 * @return   true if value >= low and < high, else false
 *
 * This method is only meant to be a shortcut. it does the very same as
 * if ((@a value >= @a low) && (@a value < @a high))
 * so while @a low is included in the interval, @a high is excluded
**/
template<typename T1, typename T2, typename T3>
inline bool isBetween(const T1 value, const T2 low, const T3 high)
{
  return (((value >= low) && (value < high)) ? true : false);
}


/** @brief rotating left shift
  *
  * This little function shifts a value @a shift bits to the left,
  * shifting bits that "fall out" back in on the right side.
  *
  * @param[in] val The value to shift. Operators <<, >> and | have to be defined for its type.
  * @param[in] shift The numbers of bits to shift.
  * @return The shifted result.
**/
template<typename T>
inline T rshl(T val, uint32_t shift)
  {
    const size_t bits = sizeof(T) * 8;
    return (val << shift) | (val >> (bits - shift));
  }


/** @brief rotating right shift
  *
  * This little function shifts a value @a shift bits to the right,
  * shifting bits that "fall out" back in on the left side.
  *
  * @param[in] val The value to shift. Operators <<, >> and | have to be defined for its type.
  * @param[in] shift The numbers of bits to shift.
  * @return The shifted result.
**/
template<typename T>
inline T rshr(T val, uint32_t shift)
  {
    const size_t bits = sizeof(T) * 8;
    return (val >> shift) | (val << (bits - shift));
  }


/** @class Uncopyable
  * @brief base class to make classes inheriting uncopyable
  *
  *  To make a class uncopyable, just inherit private from this one,
  *  meaning to add a ": private pwx::Uncopyable" to the class definition
**/
class Uncopyable
{
  Uncopyable(const Uncopyable&); // Copy constructor
  Uncopyable& operator=(const Uncopyable&); // Copy assign operator
protected:
  Uncopyable() {}
  virtual ~Uncopyable() {}
};

} // End of pwx namespace

#endif // PWX_DDTEMPTOOLS_H_INCLUDED
