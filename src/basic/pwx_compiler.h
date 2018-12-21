#pragma once
#ifndef PWX_LIBPWX_CONFIG_COMPILER_H_INCLUDED
#define PWX_LIBPWX_CONFIG_COMPILER_H_INCLUDED 1

/** @file pwx_compiler.h
  *
  * @brief General preprocessor macros and defines that are compiler dependent.
  *
  * (c) 2007 - 2018 PrydeWorX
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
  * History and change log are maintained in pwx.h
**/

/* -----------------------------------------------------------------------
 * --- Microsoft Visual C++ is no longer supported until they include  ---
 * --- enough C++11 support to be usable again. The extended           ---
 * --- initialization lists are still missing, while gcc supports them ---
 * --- for ages now.                                                   ---
 * -----------------------------------------------------------------------
*/
#if defined(_MSC_VER)
#  pragma error "Visual C++ is not supported! Use gcc-4.5+ via cygwin or mingw instead."
#elif !defined(__GNUC__)
#  error "You need gcc 4.7.3 / gcc-4.8.1 or newer to compile libpwx."
#endif


/* ---------------------------------------------------------------
 * --- gcc prior 4.7.3 / 4.8.1 have problems with std::atomic. ---
 * ---------------------------------------------------------------
*/
#if (__GNUC__ < 4) \
 || ( (__GNUC__ == 4) \
   && ( (__GNUC_MINOR__ < 7) \
     || ((__GNUC_MINOR__ == 7) && (__GNUC_PATCHLEVEL__ < 3)) \
     || ((__GNUC_MINOR__ == 8) && (__GNUC_PATCHLEVEL__ < 1)) ) )
#  error "gcc versions before gcc-4.7.3 / gcc-4.8.1 are not supported. Please upgrade to a newer version."
#elif (!defined(__cplusplus) || (__cplusplus < 201103L)) && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#  error "C++11 features must be enabled to compile and use libpwx."
#else
# define PWX_MEMORDER_RELAXED std::memory_order_relaxed
# define PWX_MEMORDER_CONSUME std::memory_order_consume
# define PWX_MEMORDER_ACQUIRE std::memory_order_acquire
# define PWX_MEMORDER_RELEASE std::memory_order_release
# define PWX_MEMORDER_ACQ_REL std::memory_order_acq_rel
# define PWX_MEMORDER_ACQ_CST std::memory_order_seq_cst
#endif // Problematic gcc versions


/* -------------------------------------------------------------------
 * --- defines to set the right modifier for library export/import ---
 * -------------------------------------------------------------------
*/
#ifdef PWX_EXPORTS
#  define PWX_API __attribute__((visibility("default")))
#else
#  define PWX_API
#endif


/* ---------------------------------------------------------------------
 * --- Private methods of the worker classes can be inlined, unless  ---
 * --- this is a debugging build                                     ---
 * ---------------------------------------------------------------------
*/
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
#define PWX_DEFAULT      =default
#define PWX_DELETE       =delete
#define PWX_PURE         __attribute__ ((pure))
#define PWX_UNUSED       __attribute__ ((unused))
#define PWX_USED         __attribute__ ((used))
#define PWX_VIRTUAL_PURE =0
#define PWX_WARNUNUSED   __attribute__ ((warn_unused_result))

#endif // PWX_LIBPWX_CONFIG_COMPILER_H_INCLUDED
