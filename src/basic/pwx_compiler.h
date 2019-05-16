#ifndef PWX_LIBPWX_SRC_BASIC_PWX_COMPILER_H_INCLUDED
#define PWX_LIBPWX_SRC_BASIC_PWX_COMPILER_H_INCLUDED
#pragma once

/** @file pwx_compiler.h
  *
  * @brief General preprocessor macros and defines that are compiler dependent.
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


/* -----------------------------------------------------------------------
 * --- pwxLib needs C++17 features. Sorry, no way around this!         ---
*/
#if (!defined(__cplusplus)) || (__cplusplus < 201703L)
   #pragma error "pwxLib needs at least C++17!" 
#endif // __cplusplus defined and large enough?


/** @def PWX_IS_MSC
  * @brief 1 if Visual C++ is used, 0 otherwise
  *  Microsoft Visual C++ is C++17 feature complete enough since:
  *  * Product Name : Visual Studio 2017 version 15.7.1
  *  * VC++ version : 14.14
  *  * _MSC_VER     : 1914
  *  * _MSC_FULL_VER: 191426428
**/
#if defined(_MSC_VER)
#  if (_MSC_VER < 1914)
#    pragma error "You need at least Visual Studio version 15.7.1 / VC++ 14.14 to build pwxLib!"
#  endif // version check
#  define PWX_IS_MSC 1
#else
#  define PWX_IS_MSC 0
#endif // _MSC_VER is defined


/** @def PWX_IS_GCC
  * @brief 1 if GNU gcc is used, 0 otherwise
  *  GNU gcc is C++17 feature complete enough since gcc-7.1.0
**/
#if defined(__GNUC__)
#  define PWX_IS_GCC 1
#  if (__GNUC__ < 7)
#    pragma error "You need at least gcc-7.1 to build pwxLib!"
#  endif
#else
#  define PWX_IS_GCC 0
#endif // __GNUC__


/** @def PWX_IS_CLANG
  * @brief 1 if LLVM clang is used, 0 otherwise
  *  LLVM clang is C++17 feature complete enough since clang-5
**/
#if defined(__clang__)
#  define PWX_IS_CLANG 1
#  if (__clang_major__ < 5)
#    pragma error "You need at least clang-5 to build pwxLib!"
#  endif
#else
#  define PWX_IS_CLANG 0
#endif // __clang__


/* --- some shortcuts --- */
/// @brief More distinct shortcut to std::memory_order_relaxed
#define PWX_MEMORDER_RELAXED std::memory_order_relaxed
/// @brief More distinct shortcut to std::memory_order_consume
#define PWX_MEMORDER_CONSUME std::memory_order_consume
/// @brief More distinct shortcut to std::memory_order_acquire
#define PWX_MEMORDER_ACQUIRE std::memory_order_acquire
/// @brief More distinct shortcut to std::memory_order_release
#define PWX_MEMORDER_RELEASE std::memory_order_release
/// @brief More distinct shortcut to std::memory_order_acq_rel
#define PWX_MEMORDER_ACQ_REL std::memory_order_acq_rel
/// @brief More distinct shortcut to std::memory_order_seq_cst
#define PWX_MEMORDER_ACQ_CST std::memory_order_seq_cst


/** @def PWX_API
  * @brief defines to set the right modifier for library export/import
*/
#ifdef PWX_EXPORTS
#  define PWX_API __attribute__((visibility("default")))
#else
#  define PWX_API
#endif


/** @def PWX_PRIVATE_INLINE
  * @brief Private methods of the worker classes can be inlined, unless
  *        this is a debugging build
**/
#if defined(LIBPWX_DEBUG) || !defined(PWX_EXPORTS)
# define PWX_PRIVATE_INLINE
#else
# define PWX_PRIVATE_INLINE inline
#endif


/* ---------------------------------------------------------------------
 * --- Some modifiers to methods, structs, classes and functions are ---
 * --- defined centrally to be able to apply changes quickly on a    ---
 * --- global scope.                                                 ---
 * ---------------------------------------------------------------------
*/
#ifndef PWX_NODOX
#define PWX_DEFAULT      =default
#define PWX_DELETE       =delete
#define PWX_PURE         __attribute__ ((pure))
#define PWX_UNUSED       __attribute__ ((unused))
#define PWX_USED         __attribute__ ((used))
#define PWX_VIRTUAL_PURE =0
#define PWX_WARNUNUSED   __attribute__ ((warn_unused_result))
#endif // ignored by doxygen

#endif // PWX_LIBPWX_SRC_BASIC_PWX_COMPILER_H_INCLUDED
