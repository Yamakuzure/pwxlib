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
  * (c) 2007 - 2020 PrydeWorX
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


#include "basic/pwx_compiler.h"


#if LIBPWX_DEBUG
# include "basic/pwx_trace_info.h"
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

/// @namespace pwx
namespace pwx {


// If any debugging mode is activated, a central logging functions is needed:
#if LIBPWX_DEBUG

// The main logging function:
void debug_log( char const* fmt, ... ) PWX_API; //!< internal debug logging function to stdout
void debug_err( char const* fmt, ... ) PWX_API; //!< internal debug logging function to stderr

// And the main wrappers:
# define DEBUG_LOG(part, fmt, ...)                   \
	::pwx::debug_log( ">> [%8s] %s : " fmt "\n", \
	                  part,                      \
	                  ::pwx::get_trace_info(     \
	                          __FILE__,          \
	                          __LINE__,          \
	                          PWX_FUNC), __VA_ARGS__ )

# define DEBUG_LOG_THERE(location, part, fmt, ...)    \
	::pwx::debug_log( ">> [%8s] %s : " fmt "\n",  \
	                  part, location, __VA_ARGS__ )

# define DEBUG_LOG_CAUGHT_STD(part)                  \
	catch (::std::exception &e) {                \
		DEBUG_LOG(part,                      \
		        "Caught std::exception: %s", \
		        e.what());                   \
	}

# define DEBUG_ERR(part, fmt, ...)                   \
	::pwx::debug_err( ">> [%8s] %s : " fmt "\n", \
	                  part,                      \
	                  ::pwx::get_trace_info(     \
	                          __FILE__,          \
	                          __LINE__,          \
	                          PWX_FUNC), __VA_ARGS__ )

# define DEBUG_ERR_THERE(location, part, fmt, ...)    \
	::pwx::debug_err( ">> [%8s] %s : " fmt "\n",  \
	                  part, location, __VA_ARGS__ )
#else
# define DEBUG_LOG(...)            do{}while(0)
# define DEBUG_LOG_THERE(...)      do{}while(0)
# define DEBUG_LOG_CAUGHT_STD(...)
# define DEBUG_ERR(...)            do{}while(0)
# define DEBUG_ERR_THERE(...)      do{}while(0)
/* debug_log() and debug_err() are not defined here, because they would cause
 *   pwx::debug_log(...);
 * to become
 *   pwx::{};
 ' which no compiler is happy with.
*/
#endif // LIBPWX_DEBUG

/** @def DEBUG_LOG
  * @brief Print a debugging information message with automatic location information.
  * This is a macro that gets removed in release builds.
  *
  * @def DEBUG_LOG_THERE
  * @brief Print a debugging information message with manual location information.
  * This is a macro that gets removed in release builds.
  *
  * @def DEBUG_LOG_CAUGHT_STD
  * @brief catch an `std::exception` and log its `what()`.
  * This macro can be used to log an exception that is normally ignored, but might
  * be somewhat interesting for debugging purposes.
  *
  * @def DEBUG_ERR
  * @brief Print a debugging error message with automatic location information.
  * This is a macro that gets removed in release builds.

  * @def DEBUG_ERR_THERE
  * @brief Print a debugging error message with manual location information.
  * This is a macro that gets removed in release builds.
**/


} // namespace pwx


#endif // PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED

