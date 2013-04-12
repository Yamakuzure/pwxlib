#pragma once
#ifndef PWX_LIBPWX_CONFIG_COMPILER_H_INCLUDED
#define PWX_LIBPWX_CONFIG_COMPILER_H_INCLUDED 1

/** @file compiler.h
  *
  * @brief General preprocessor macros and defines that are compiler dependent.
  *
  * (c) 2007 - 2013 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         yamakuzure@users.sourceforge.net
  *         http://pwxlib.sourceforge.net
  *
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
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
#elif defined(__GNUC__)
#  if (__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ < 5))
#    error "gcc versions before gcc-4.5 are not supported. Please upgrade to a newer version."
#  elif (!defined(__cplusplus) || (__cplusplus < 201103L)) && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#    error "C++11 features must be enabled to compile and use libpwx."
#  endif
#else
#  error "You need gcc 4.5 or newer to compile libpwx."
#endif

/* -------------------------------------------------------------------
 * --- defines to set the right modifier for library export/import ---
 * -------------------------------------------------------------------
*/
#ifdef PWX_EXPORTS
#  define PWX_API __attribute__ ((visibility("default")))
#else
#  define PWX_API
#endif


/* ---------------------------------------------------------------------
 * --- Private methods of the worker classes can be inlined, unless  ---
 * --- this is a debugging build                                     ---
 * ---------------------------------------------------------------------
*/
#if defined(LIBPWX_DEBUG)
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


/* ------------------------------------------------------------------
 * --- To be damn sure how many bits which integral type uses the ---
 * -- stdint types are needed.                                    ---
 * ------------------------------------------------------------------
*/
#include <cstdint>

// Debug? (Ensure it is loaded if macros.h isn't needed)
#if defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)
# include "pwx/functions/debug.h"
#endif // LIBPWX_DEBUG || PWX_THREADDEBUG

#endif // PWX_LIBPWX_CONFIG_COMPILER_H_INCLUDED
