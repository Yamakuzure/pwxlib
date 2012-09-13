#pragma once
#ifndef PWX_DEFINES_H_INCLUDED
#define PWX_DEFINES_H_INCLUDED 1

/** @file DefaultDefines.h
  *
  * @brief This file adds some defines and tools, that are used everywhere in pwxLib.
  *
  * The following is a list of tools, templates and classes defined in the namespace pwx:
  *  -# Definitions
  *    - standard integers are used everywhere, thus [u]int*_t are made available.
  *    - The existence of M_PIl is ensured.
  *  -# Macros
  *    - compAssert(expression) produces a compilation error if "expression" is false.
  *    - isSameType(a, b) returns true if a and b are of the same type.
  *    - isPointer(a) returns true if a is a pointer.
  *    - isIntType(a) returns true if a is an integer.
  *    - isFloatType(a) returns true if a is a float or double.
  *    - isArrayType(a) returns true if a is an array.
  *    - PWX_TRY(func) is a wrapper for "try { func; }"
  *    - PWX_THROW(exception, message, description) wraps adding positional information to an exception
  *      derived from pwx::Exception.
  *    - PWX_THROW_FURTHER catches an exception derived from pwx::Exception, adds positional data and re-throws it.
  *    - PWX_CATCH_AND_FORGET(exception) can be used to ignore a <B>specific</B> exception.
  *    - SIGN(x) - returns 1 if x is positive or zero, -1 if x is explicitly negative.
  *    - STR[C]EQ(a, b) - returns true if the C-Strings a and b are equal. [C] = case insensitive.
  *    - STR[C]GT(a, b) - returns true if the C-String a is "greater" than b. [C] = case insensitive.
  *    - STR[C]LT(a, b) - returns true if the C-String a is "less" than b. [C] = case insensitive.
  *    - STR[C]NE(a, b) - returns true if the C-Strings a and b are not equal. [C] = case insensitive.
  *  -# Templated Functions
  *    - absDistance(x1, y1, x2, y2) returns the absolute distance between two 2D points.
  *    - absDistance(x1, y1, z1, x2, y2, z2) returns the absolute distance between two 3D points.
  *    - isBetween(value, low, high) returns true if the value is >=low and <high.
  *    - rshl(value, shift) - shifts value "shift" steps left, rotating bits back on the right side.
  *    - rshr(value, shift) - shifts value "shift" steps right, rotating bits back on the left side.
  *  -# Classes
  *    - Uncopyable - inherit private from this class to make all descendants uncopyable.
  *    - Lockable - inherit public from this class to add MutEx functionality to all descendants. (*)
  *    - Lock - Use this to lock an instance of a class derived from "Lockable" via ctor, unlock via dtor. (*)
  *    .
  *    (*) To enable multi threading the macro "PWX_THREADS" has to be defined <I>before</I> including this file!
  *
  * (c) 2007-2012 Sven Eden, PrydeWorX
  * @author Sven Eden, PrydeWorX - Hamburg, Germany
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
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.0.1     2007-12-21  sed, PrydeWorX  First Design
  * 0.1.0     2007-12-22  sed, PrydeWorX  initial release
  * 0.1.1     2008-02-01  sed, PrydeWorX  added some method and function helpers
  * 0.1.2     2008-07-14  sed, PrydeWorX  added NULL and PI
  * 0.2.0     2009-09-21  sed, PrydeWorX  added mathematical and string helpers
  * 0.3.0     2009-09-25  sed, PrydeWorX  added ::std::floor(), CEIL(), STRCEQ() and STRCNE()
  * 0.3.1     2009-10-09  sed, PrydeWorX  replaced old C-stuff with C++ versions (eventually!)
  * 0.4.0     2009-11-06  sed, PrydeWorX  added between()
  * 0.4.1     2009-12-10  sed, PrydeWorX  killed ABSDISTANCE macro and added absdistance for 3 dimensions
  * 0.5.0     2010-01-28  sed, PrydeWorX  Added PWX_INLINE and changed PWX_FORCEINLINE to be only used
  *                                       if NDEBUG is set, resulting in the same behavior as assert() utilizes
  * 0.5.1     2010-04-14  sed, PrydeWorX  Some rework to have pwxLib comply as much as possible with Items 57
  *                                       to 59 in "C++ Coding Standards" (Herb Sutter, Andrei Alexandrescu, 2005)
  * 0.5.2     2010-04-28  sed, PrydeWorX  removed pwxDDVersion and PWX_FORCEINLINE. Both have no real use, the latter
  *                                       might be even dangerous nowadays.
  * 0.5.3     2010-05-17  sed, PrydeWorX  Changed PWX_INLINE to not inline if PWXLIB_DEBUG is defined.
  * 0.5.4     2010-05-21  sed, PrydeWorX  absdistance does no longer provide empty default arguments.
  * 0.5.5     2010-08-06  sed, PrydeWorX  added compile time assertions (Alexandrescu 2001) via compAssert(expr)
  * 0.5.6     2010-09-18  sed, PrydeWorX  added type comparison macro isSameType(a, b)
  * 0.5.7     2010-09-30  sed, PrydeWorX  added PWX_TRY(), PWX_THROW() and PWX_THROWFURTHER() to make
  *                                       exception handling easier and better readable
  * 0.5.8     2010-10-05  sed, PrydeWorX  NULL and M_PIl removed, added PWX_CATCH_AND_FORGET()
  * 0.5.9     2010-10-07  sed, PrydeWorX  Mathematical helpers now depend on the inclusion of cmath/math.h
  * 0.6.0     2010-10-19  sed, PrydeWorX  type traits fixed, they now listen to both tr1 and C++0x type_traits
  * 0.6.1     2010-10-20  sed, PrydeWorX  removed isFundamental(), it wasn't really useful.
  *                                       added string helpers STRLT(), STRGT(), STRCLT() and STRCGT()
  * 0.6.2     2010-10-29  sed, PrydeWorX  removed the namespace defines, as they produce more problems than advantages
  * 0.6.3     2010-11-02  sed, PrydeWorX  added type_traits as I need more than is worth to copy
  * 0.6.4     2010-11-03  sed, PrydeWorX  eventually removed all (bad) underscore prefixes from _PWX_* defines and macros
  * 0.6.5     2010-11-08  sed, PrydeWorX  renamed between() to isBetween(), and absdistance() to absDistance(). Further
  *                                       both absDistance() overloads now check for a valid datatype (integer and floats).
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.0.1   2010-11-12  sed, PrydeWorX  (re-)added includes, that are used everywhere
  * 0.7.0.2   2010-12-30  sed, PrydeWorX  added stdint.h from Alexander Chemeris, 2006, msinttypes.googlecode.com to
  *                                       provide [u]int*_t types for Microsoft Visual C++ compilers.
  * 0.7.0.3   2011-01-05  sed, PrydeWorX  Some updates to make pwxLib compile with Visual C++ 2008.
  * 0.7.0.4   2011-01-10  sed, PrydeWorX  Added MSVC++ replacements for strcasecmp.
  * 0.7.0.5   2011-03-06  sed, PrydeWorX  Welcome to 2011 !
  * 0.7.0.6   2011-03-15  sed, PrydeWorX  added usage of new static char buffers in pwx::Exception to the macros
  *                                       PWX_THROW and PWX_THROW_FURTHER.
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2.1   2011-04-20  sed, PrydeWorX  Unified PWX_THROW_FURTHER thanks to new exception naming method
  * 0.7.2.2   2011-05-09  sed, PrydeWorX  Added rshl() and rshr() for rotating left and right shift
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0.1   2011-08-10  sed, PrydeWorX  Added pwx::Lockable, a class to inherit from adding MutEx functionality and
  *                                       pwx::Lock, a class to use Lockable objects locking them exception safe. (RAII)
  * 0.8.0.2   2011-08-12  sed, PrydeWorX  Fixed a typo, its "__GNUC__" not "_GNUC_"!
  * 0.8.0.3   2011-08-18  sed, PrydeWorX  made cmath being included by default again. Not having it caused to many headaches!
  * 0.8.0.4   2011-08-23  sed, PrydeWorX  Added an object check to the Lock dtor and a tryLock() method to Lockable.
  * 0.8.0.5   2011-09-02  sed, PrydeWorX  Made Multi-Threading optional via PWX_THREADS macro and internal/DefaultMutex.h
  * 0.8.0.6   2011-09-11  sed, PrydeWorX  Removed the default arguments from the 2D version of absDistance. They were too
  *                                       much of a bug source!
  * 0.8.0.7   2011-09-18  sed, PrydeWorX  Reverted pwx::Exception and its helper macros to local helper C-Strings, the static
  *                                       buffer is problematic pwx::Exception using objects shows up in more than one
  *                                       compilation unit.
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.1.1   2011-10-05  sed, PrydeWorX  Added undefs plus warnings if min/max macros are found
  * 0.8.1.2   2011-10-06  sed, PrydeWorX  Added M_PI_2l and M_PI_4l ifndef, MSVc++ doesn't know'em
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3.1   2011-11-30  sed, PrydeWorX  DefaultMutex.h: Some documentation corrections
  * 0.8.3.2   2012-01-05  sed, PrydeWorX  Privatized Lockable::operator=, it is a serious error to try to assign an instance
  *                                       anyway.
  * 0.8.5     2012-03-01  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5.1   2012-04-12  sed, PrydeWorX  Eventually removed the argument from PWX_CATCH_AND_FORGET(), MSVC++10 nags too much.
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

// Load the function wrapper as early as possible:
#include "pwxLib/internal/MSVCwrap.h"

// Disable nuisance warnings from MS compiler:
#include "pwxLib/internal/MSVCwarnPush.h"

// It is too much a hassle to include this in the correct order when absDistance or other
// helper functions are needed. Include it here by default and have less headaches!
#include <cmath>

// We need the int*_t typedefs for better compatibility to 32bit, and must know whether
// C++11 extended initialization lists are available:
#ifdef PWX_HAS_CXX11_INIT
# undef PWX_HAS_CXX11_INIT
#endif
#if defined(_MSC_VER)
#  if (_MSC_VER < 1600)
/** Visual C++ prior 2010 needs external inttypes.
  * These are provided by Alexander Chemeris, 2006, http://msinttypes.googlecode.com
**/
#    include <cstdlib>
#    include <pwxLib/external/stdint.h>
#  else // MSVC++10 with service pack has cstdint in its root include folder
#    include <cstdint>
#  endif // MSVC++
#else // Others are either GNU compatible or have a tr1 subfolder with cstdint in them
#  if defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(_GLIBCXX_INCLUDE_AS_CXX0X)
#    include <cstdint>
     // Unfortunately only GCC (since version 4.4) supports the new initialization lists
#    define PWX_HAS_CXX11_INIT 1
#  else
#    include <tr1/cstdint>
#  endif // GNU-GCC
#endif // MSC or not MSC

// Used almost anywhere (but StreamHelpers.h)
#include <cassert>
// Used almost anywhere (but MathHelpers.h)
#include <string>
// We need this for the string helpers to work with MSVC++
#if defined(_MSC_VER)
#  include <cstring>
#endif
// reenable MSVC warnings
#include "pwxLib/internal/MSVCwarnPop.h"

// Undefine all:
#undef PWX_INTERNAL
#undef PWX_VISIBLE
#undef PWX_EXTERNVISIBLE
#undef PWX_VIRTUAL_PURE
#undef PWX_PURE
#undef PWX_UNUSED
#undef PWX_USED
#undef PWX_WARNUNUSED
#undef PWX_INLINE
#undef SIGN
#undef STRCEQ
#undef STRCNE
#undef STREQ
#undef STRNE
#undef PWX_TRY
#undef PWX_THROW
#undef PWX_THROWFURTHER
#undef PWX_CATCH_AND_FORGET
#undef isSameType
#undef isPointer
#undef isFundamental


// Add M_PIl[24] if not known (Like in VC++ 2008)
#ifndef M_PIl
#  define M_PIl   3.1415926535897932384626433832795029L
#  define M_PI_2l 1.5707963267948966192313216916397514L
#  define M_PI_4l 0.7853981633974483096156608458198757L
#endif

// Visibilities
#if defined(__GNUC__)
#  define PWX_INTERNAL      __attribute__ ((visibility ("internal")))
#  define PWX_VISIBLE       __attribute__ ((visibility ("default")))
#  define PWX_EXTERNVISIBLE __attribute__ ((externally_visible))
#else
#  define PWX_INTERNAL
#  define PWX_VISIBLE
#  define PWX_EXTERNVISIBLE
#endif

// vars && functions
#define PWX_VIRTUAL_PURE =0
#if defined(__GNUC__)
#  define PWX_PURE       __attribute__ ((pure))
#  define PWX_UNUSED     __attribute__ ((unused))
#  define PWX_USED       __attribute__ ((used))
#  define PWX_WARNUNUSED __attribute__ ((warn_unused_result))
#else
#  define PWX_PURE
#  define PWX_UNUSED
#  define PWX_USED
#  define PWX_WARNUNUSED
#endif

// Inlining is supported if not in debugging mode:
#ifdef PWXLIB_DEBUG
#define PWX_INLINE
#else // do not debug:
#define PWX_INLINE inline
#endif // ask for debugging


// Exception helpers
//---------------------
// These are meant to make throwing and catch->throw easier and better readable
/// This macro can be used for single try { ONE_FUNC_CALL } - entries. (simply saves writing curly brackets)
#define PWX_TRY(func) \
  try \
    { \
      func; \
    }
// Note: Of course one can use PWX_TRY(funcA; funcB; funcC; ... ; funcZ), but how readable is that?
// Answer: With just two it works splendid. ;-)

/// This one fills in positional information
#if defined(_MSC_VER)
#  define PWX_THROW(except, msg, desc) { \
     char _pwx_trace_info[256]; \
     char _pwx_fname_info[64]; \
     char _pwx_extension[8]; \
     _splitpath_s(__FILE__, NULL, 0, NULL, 0, _pwx_fname_info, 63, _pwx_extension, 7); \
     sprintf_s(_pwx_trace_info, "%s%s:%d %s", _pwx_fname_info, \
               _pwx_extension, __LINE__, __FUNCTION__); \
     throw(except(strlen(msg) ? _strdup(msg) : _strdup("no message"), \
                  _strdup(_pwx_trace_info), __FUNCSIG__, \
                  strlen(desc) ? _strdup(desc) : _strdup("no description"))); \
   }
#else
#  define PWX_THROW(except, msg, desc) { \
     char _pwx_trace_info[256]; \
     sprintf(_pwx_trace_info, "%s:%d %s", basename(__FILE__), __LINE__, __FUNCTION__); \
     throw(except(strlen(msg) ? strdup(msg) : strdup("no message"), \
                  strdup(_pwx_trace_info), __PRETTY_FUNCTION__, \
                  strlen(desc) ? strdup(desc) : strdup("no description"))); \
   }
#endif
/// This one simply catches any ::pwx::Exception exception and its descendants, adds it's positional data to the trace, and re-throws
#if defined(_MSC_VER)
#  define PWX_THROW_FURTHER \
     catch(::pwx::Exception &e) { \
       char _pwx_trace_info[256]; \
       char _pwx_fname_info[64]; \
       char _pwx_extension[8]; \
       _splitpath_s(__FILE__, NULL, 0, NULL, 0, _pwx_fname_info, 63, _pwx_extension, 7); \
       sprintf_s(_pwx_trace_info, "--> Called by %s%s:%d %s", _pwx_fname_info, \
                 _pwx_extension, __LINE__, __FUNCTION__); \
       e.addToTrace(_pwx_trace_info); \
       throw; \
     }
#else
#  define PWX_THROW_FURTHER \
     catch(::pwx::Exception &e) { \
       char _pwx_trace_info[256]; \
       sprintf(_pwx_trace_info, "--> Called by %s:%d %s", basename(__FILE__), __LINE__, __FUNCTION__); \
       e.addToTrace(_pwx_trace_info); \
       throw; \
     }
#endif

/// This one is meant for situations, when a _specific_ exception can (or should) be ignored
#define PWX_CATCH_AND_FORGET(except) catch(except&) { }

// string helpers
//---------------------
// These are only defined to have _shorter_ versions to write. (Me lazy git... ;))
#if defined(_MSC_VER)
#  define STRCEQ(a,b) (_stricmp(a,b) == 0)
#  define STRCNE(a,b) (_stricmp(a,b) != 0)
#  define STRCLT(a,b) (_stricmp(a,b) < 0)
#  define STRCGT(a,b) (_stricmp(a,b) > 0)

#  define STREQ(a,b) (strcmp(a,b) == 0)
#  define STRNE(a,b) (strcmp(a,b) != 0)
#  define STRLT(a,b) (strcmp(a,b) < 0)
#  define STRGT(a,b) (strcmp(a,b) > 0)
#else
#  define STRCEQ(a,b) (strcasecmp(a,b) == 0)
#  define STRCNE(a,b) (strcasecmp(a,b) != 0)
#  define STRCLT(a,b) (strcasecmp(a,b) < 0)
#  define STRCGT(a,b) (strcasecmp(a,b) > 0)

#  define STREQ(a,b) (::std::strcmp(a,b) == 0)
#  define STRNE(a,b) (::std::strcmp(a,b) != 0)
#  define STRLT(a,b) (::std::strcmp(a,b) < 0)
#  define STRGT(a,b) (::std::strcmp(a,b) > 0)
#endif // GNU or MSC

// mathematical helpers
//---------------------
// I haven't found a good replacement for this, so it stays being a macro:
#define SIGN(x)      (((x) < 0) ? -1 : 1)

// Include template tools:
#include "pwxLib/internal/DefaultTempTools.h"
// Include mutex tools:
#include "pwxLib/internal/DefaultMutex.h"
// note: Order is relevant, pwx::Lock depends on pwx::Uncopyable

// Unfortunately we might run into a problem with min/max macros if someone defined them.
#if defined(min)
#  if defined(_MSC_VER)
#    pragma message ("WARNING: Someone was _stupid_ enough to define a min() macro! UNDEFINING!")
#  else
#    warning "WARNING: Someone was _stupid_ enough to define a min() macro! UNDEFINING!"
#  endif
#  undef min
#endif // min macro defined
#if defined(max)
#  if defined(_MSC_VER)
#    pragma message ("WARNING: Someone was _stupid_ enough to define a max() macro! UNDEFINING!")
#  else
#    warning "WARNING: Someone was _stupid_ enough to define a max() macro! UNDEFINING!"
#  endif
#  undef max
#endif // max macro defined

#endif // PWX_DEFINES_H_INCLUDED
