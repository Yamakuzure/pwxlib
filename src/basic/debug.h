#ifndef PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED
#define PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED 1
#pragma once

/** @file pwx_debug.h
  *
  * @brief Debugging utilities that do nothing unless LIBPWX_DEBUG is defined.
  *
  * Macros and functions to react on the various debugging modes.
  * They are designed to be used everywhere appropriate, getting
  * out of the way when the needed mode is not set.
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


#include "basic/compiler.h"


#if LIBPWX_DEBUG
# include "basic/trace_info.h"
# include "log/log.h"
#endif // LIB or THREAD debug mode


#if PWX_ANNOTATIONS
// In short: If LIBPWX_DEBUG is defined, the annotations are re-defined so the
// compiler gives information about the include chain that is wrong. Otherwise
// only notify and undef.
# ifdef _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE
#   pragma message "_GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE already defined!"
#   pragma message "Annotations might not work correctly, please check your include order."
#   if !LIBPWX_DEBUG
#     undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE
#   endif
# endif
# define _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(a) ANNOTATE_HAPPENS_BEFORE(a)
# ifdef _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER
#   pragma message "_GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER already defined!"
#   pragma message "Annotations might not work correctly, please check your include order."
#   if !LIBPWX_DEBUG
#     undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER
#   endif
# endif
# define _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(a)  ANNOTATE_HAPPENS_AFTER(a)
# include <valgrind/helgrind.h>
#endif // ANNOTATIONS


/** @def log_debug
  * @brief Debug log macro that does nothing in release mode
**/
#if PWX_IS_DEBUG_MODE
#  define log_debug( title_, message_, ... ) PWX_log_wrapper(::pwx::LOG_DEBUG, title_, message_, __VA_ARGS__)
#else
#  define log_debug( ... ) do {} while(0)
#endif // debug enabled


/** @def log_debug_there
  * @brief Special log_debug variant, which a location from elsewhere is given.
**/
#if PWX_IS_DEBUG_MODE
#  define log_debug_there( loc_, title_, message_, ... ) \
	::pwx::log( loc_, ::pwx::LOG_DEBUG, title_, message_, __VA_ARGS__)
#else
#  define log_debug_there( ... ) do {} while(0)
#endif // debugging enabled


/** @def log_debug_error
  * @brief Special log_debug variant, which marks the debug message as being an error
**/
#if PWX_IS_DEBUG_MODE
#  define log_debug_error( title_, message_, ... ) PWX_log_wrapper(::pwx::LOG_DEBUG, title_, "ERROR:" message_, __VA_ARGS__)
#else
#  define log_debug_error( ... ) do {} while(0)
#endif // debug enabled


/** @def log_debug_caught_std
  * @brief Catch an std exception and use log_debug to log it, but ignore it otherwise
**/
# define log_debug_caught_std( part )               \
       catch (::std::exception &e) {                \
               log_debug(part,                      \
                       "Caught std::exception: %s", \
                       e.what());                   \
       }


#endif // PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED

