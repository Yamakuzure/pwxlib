#ifndef PWX_LIBPWX_SRC_BASIC_PWX_COMPILER_H_INCLUDED
#define PWX_LIBPWX_SRC_BASIC_PWX_COMPILER_H_INCLUDED 1
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


#if defined(_MSC_VER)
#  if (_MSC_VER < 1914)
#    pragma error "You need at least Visual Studio version 15.7.1 / VC++ 14.14 to build pwxLib!"
#  endif // version check
#  define PWX_IS_CLANG 0
#  define PWX_IS_GCC   0
#  define PWX_IS_MSC   1
#elif defined(__GNUC__)
#  define PWX_IS_CLANG 0
#  define PWX_IS_GCC   1
#  define PWX_IS_MSC   0
#  if (__GNUC__ < 7)
#    pragma error "You need at least gcc-7.1 to build pwxLib!"
#  endif
#elif defined(__clang__)
#  define PWX_IS_CLANG 1
#  define PWX_IS_GCC   0
#  define PWX_IS_MSC   0
#  if (__clang_major__ < 5)
#    pragma error "You need at least clang-5 to build pwxLib!"
#  endif
#else
#  define PWX_IS_CLANG 0
#  define PWX_IS_GCC   0
#  define PWX_IS_MSC   0
#  pragma error "Currently only Microsoft Visual C++, GNU gcc and Clang are supported"
#endif // Visual C++ versus GNU gcc versus Clang
/** @def PWX_IS_MSC
  * @brief 1 if Visual C++ is used, 0 otherwise
  *  Microsoft Visual C++ is C++17 feature complete enough since:
  *  * Product Name : Visual Studio 2017 version 15.7.1
  *  * VC++ version : 14.14
  *  * _MSC_VER     : 1914
  *  * _MSC_FULL_VER: 191426428
  *
  * @def PWX_IS_GCC
  * @brief 1 if GNU gcc is used, 0 otherwise
  *  GNU gcc is C++17 feature complete enough since gcc-7.1.0
  *
  * @def PWX_IS_CLANG
  * @brief 1 if LLVM clang is used, 0 otherwise
  *  LLVM clang is C++17 feature complete enough since clang-5
**/


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
#  if PWX_IS_MSC
#    define PWX_API __declspec(dllexport)
#  else
#    define PWX_API __attribute__((visibility("default")))
#  endif
#else
#  if PWX_IS_MSC
#    define PWX_API __declspec(dllimport)
#  else
#    define PWX_API
#  endif
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
#define PWX_VIRTUAL_PURE =0
#if PWX_IS_MSC
#  define PWX_WARNUNUSED _Check_return_
#else
#  define PWX_WARNUNUSED __attribute__ ((warn_unused_result))
#endif // Difference for MSVC
#endif // ignored by doxygen


/** @def PWX_CURRENT_FUNC
  * @brief macro/identifier for the current function name
  *
  * To be sure to have a valid macro/identifier for the current function,
  * `PWX_CURRENT_FUNC` is set to what hopefully is the correct macro on
  * the used compiler and standard.
**/
#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)) || (defined(__cplusplus) && (__cplusplus >= 201103))
#  define PWX_CURRENT_FUNC  __func__
#elif (defined(__IBMCPP__) && (__IBMCPP__ >= 500)) || (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600))
#  define PWX_CURRENT_FUNC __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#  define PWX_CURRENT_FUNC __FUNC__
#else
#  define PWX_CURRENT_FUNC "(unknown)"
#endif


#endif // PWX_LIBPWX_SRC_BASIC_PWX_COMPILER_H_INCLUDED
