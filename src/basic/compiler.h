#ifndef PWX_LIBPWX_SRC_BASIC_PWX_COMPILER_H_INCLUDED
#define PWX_LIBPWX_SRC_BASIC_PWX_COMPILER_H_INCLUDED 1
#pragma once

/** @file pwx_compiler.h
  *
  * @brief General preprocessor macros and defines that are compiler dependent.
  *
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
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


#include "pwx_config.h"


/* -----------------------------------------------------------------------
 * --- pwxLib needs C++17 features. Sorry, no way around this!         ---
 * -------------------------------------------------------------------- */
#if ( !defined(__cplusplus) ) || ( __cplusplus < 201703L )
  #pragma error "pwxLib needs at least C++17!"
#endif // __cplusplus defined and large enough?
#if ( defined(__cplusplus) ) && ( __cplusplus >= 202002L )
  #define PWX_HAS_CXX20 1
#else
  #define PWX_HAS_CXX20 0
#endif


/* -----------------------------------------------------------------------
 * --- Already prepare Windows specific settings if needed.            ---
 * -------------------------------------------------------------------- */
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#  define PWX_IS_LINUX   0
#  define PWX_IS_WINDOWS 1
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <WinSDKVer.h>
#  define WINVER       _WIN32_WINNT_WIN7
#  define _WIN32_WINNT _WIN32_WINNT_WIN7
#  include <SDKDDKVer.h>
#  include <windows.h>
#else
#  define PWX_IS_LINUX   1
#  define PWX_IS_WINDOWS 0
#endif // Windows or Linux
/** @def PWX_IS_LINUX
  * @brief Set to 1 if this is a Linux system, 0 otherwise.
  *
  * @def PWX_IS_WINDOWS
  * @brief Set to 1 if this is a Windows system, 0 otherwise.
**/


/* -----------------------------------------------------------------------
 * --- Some macros/functions work differently on different compilers!  ---
 * -------------------------------------------------------------------- */
#if defined(__clang__)
#  if (__clang_major__ < 5)
#    pragma error "You need at least clang-5 to build pwxLib!"
#  endif
#  define PWX_IS_CLANG 1
#  define PWX_IS_GCC   0
#  define PWX_IS_MSVC  0
#elif defined(_MSC_VER)
#  if (_MSC_VER < 1914)
#    pragma error "You need at least Visual Studio version 15.7.1 / VC++ 14.14 to build pwxLib!"
#  endif // version check - We assume something compatible (MinGW/CLang) otherwise
#  define PWX_IS_CLANG 0
#  define PWX_IS_GCC   0
#  define PWX_IS_MSVC  1
#elif defined(__GNUC__)
#  if ( __GNUC__ < 7 )
#    pragma error "You need at least gcc-7.1 to build pwxLib!"
#  endif
#  define PWX_IS_CLANG 0
#  define PWX_IS_GCC   1
#  define PWX_IS_MSVC  0
#else
#  define PWX_IS_CLANG 0
#  define PWX_IS_GCC   0
#  define PWX_IS_MSVC   0
#  pragma error "Currently only Microsoft Visual C++, GNU gcc and Clang are supported"
#endif // Visual C++ versus GNU gcc versus Clang
/** @def PWX_IS_MSVC
  * @brief 1 if Visual C++ is used, 0 otherwise
  *  Microsoft Visual C++ is C++17 feature complete enough since:
  *  * Product Name : Visual Studio 2017 version 15.7.1
  *  * VC++ version : 14.14
  *  * _MSC_VER     : 1914
  *  * _MSC_FULL_VER: 191426428
  *
  * @def PWX_IS_GCC
  * @brief 1 if GNU gcc is used, 0 otherwise
  *  GNU gcc is C++17 feature complete enough since gcc-8.1.0
  *
  * @def PWX_IS_CLANG
  * @brief 1 if LLVM clang is used, 0 otherwise
  *  LLVM clang is C++17 feature complete enough since clang-5
**/


/** @def PWX_API
  * @brief defines to set the right modifier for library export/import
*/
#if PWX_IS_MSVC || defined __CYGWIN__
  #ifdef PWX_EXPORTS
	#if PWX_IS_GCC || PWX_IS_CLANG
	  #define PWX_API __attribute__ ((dllexport))
	#else
	  #define PWX_API __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
	#endif
  #else
	#ifdef PWX_IS_GCC || PWX_IS_CLANG
	  #define PWX_API __attribute__ ((dllimport))
	#else
	  #define PWX_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
	#endif
  #endif
  #define PWX_LOCAL
#else // not on windows
  #define PWX_API    __attribute__ ((visibility ("default")))
  #define PWX_LOCAL  __attribute__ ((visibility ("hidden")))
#endif // windows versus unix
/** @def PWX_API
  * @brief Set the right modifier for library export/import
*/
/** @def PWX_LOCAL
  * @brief Modifier to hide internal structures and members
*/


#if LIBPWX_DEBUG
  #define PWX_IS_DEBUG_MODE   1
  #define PWX_IS_RELEASE_MODE 0
  #define PWX_PRIVATE_INLINE
#else
  #define PWX_IS_DEBUG_MODE   0
  #define PWX_IS_RELEASE_MODE 1
  #if defined(PWX_EXPORTS)
	#define PWX_PRIVATE_INLINE inline
  #else
	#define PWX_PRIVATE_INLINE
  #endif
#endif
/** @def PWX_PRIVATE_INLINE
  * @brief Private methods of the worker classes can be inlined, unless
  *        this is a debugging build
**/
/** @def PWX_IS_DEBUG_MODE
  * @brief Set to 1 in debug mode, and to 0 otherwise
**/
/** @def PWX_IS_RELEASE_MODE
  * @brief Set to 1 in release mode, and to 0 otherwise
**/


/* ---------------------------------------------------------------------
 * --- Some modifiers to methods, structs, classes and functions are ---
 * --- defined centrally to be able to apply changes quickly on a    ---
 * --- global scope.                                                 ---
 * ------------------------------------------------------------------ */
#ifndef PWX_NODOX
#define PWX_DEFAULT      =default
#define PWX_DELETE       =delete
#define PWX_VIRTUAL_PURE =0
#endif // ignored by doxygen


/* ---------------------------------------------------------
 * --- Every header has to make pwx::finish() available. ---
 * --- It does not include anything itself, so including ---
 * --- it globally here is pretty safe.                  ---
 * ------------------------------------------------------ */
#include "libpwx/finish.h"


/* ---------------------------------------------------------
 * --- Every header has to make pwx::init() available. ---
 * --- It does not include anything itself, so including ---
 * --- it globally here is pretty safe.                  ---
 * ------------------------------------------------------ */
#include "libpwx/init.h"


#endif // PWX_LIBPWX_SRC_BASIC_PWX_COMPILER_H_INCLUDED
