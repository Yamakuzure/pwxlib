#ifndef PWX_LIBPWX_PWX_GENERAL_TYPES_H_INCLUDED
#define PWX_LIBPWX_PWX_GENERAL_TYPES_H_INCLUDED 1
#pragma once

/** @file pwx_types.h
  *
  * @brief Includes and typedefs for types used throughout libpwx
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


#include <atomic>


/** @brief true if @a a is of the same type as @a b
  *
  * @param a left type
  * @param b right type
  * @return true if @a and @a b are of the same type
**/
#define isSameType(a, b) (std::is_same<a, b>::value)


/** @brief true if @a a is a pointer
  *
  * @param a type to check
  * @return true if @a is a pointer
**/
#define isPointer(a) (std::is_pointer<a>::value)


/** @brief true if @a a is an integral type
  *
  * @param a type to check
  * @return true if @a is an integral type
**/
#define isIntType(a) (std::is_integral<a>::value)


/** @brief true if @a a is a floating point type
  *
  * @param a type to check
  * @return true if @a is a floating point type
**/
#define isFloatType(a) (std::is_floating_point<a>::value)


/** @brief true if @a is either int or float type
  *
  * @param a type to check
  * @return true if @a is either an integer or a floating point type
**/
#define isNumericType(a) (isIntType(a) || isFloatType(a))


/** @brief true if @a a is an array
  *
  * @param a type to check
  * @return true if @a is an array
**/
#define isArrayType(a) (std::is_array<a>::value)


/// @namespace pwx
namespace pwx {


typedef std::atomic_bool          abool_t; //!< Shortcut to std::atomic_bool
typedef std::atomic_flag          aflag_t; //!< Shortcut to std::atomic_flag
typedef std::atomic_uint_fast32_t aui32_t; //!< Shortcut to std::atomic_uint_fast32_t
typedef std::atomic_size_t        asize_t; //!< Shortcut to std::atomic_size_t
typedef std::memory_order         mord_t;  //!< Shortcut to std::memory_order


} // namespace pwx


#endif // PWX_LIBPWX_PWX_GENERAL_TYPES_H_INCLUDED

