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


#include "basic/CLockGuard.h"
#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"


#if defined(PWX_ANNOTATIONS)
// In short: If LIBPWX_DEBUG is defined, the annotations are re-defined so the
// compiler gives information about the include chain that is wrong. Otherwise
// only notify and undef.
# ifdef _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE
#   pragma message "_GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE already defined!"
#   pragma message "Annotations might not work correctly, please check your include order."
#   ifndef LIBPWX_DEBUG
#     undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE
#   endif
# endif
# define _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(a) ANNOTATE_HAPPENS_BEFORE(a)
# ifdef _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER
#   pragma message "_GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER already defined!"
#   pragma message "Annotations might not work correctly, please check your include order."
#   ifndef LIBPWX_DEBUG
#     undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER
#   endif
# endif
# define _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(a)  ANNOTATE_HAPPENS_AFTER(a)
# include <valgrind/helgrind.h>
#endif // ANNOTATIONS

// Handle includes and defines
#if defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)
# include <cstdio>
# include <cstring>
# include <atomic>
#endif // LIBPWX_DEBUG || PWX_THREADDEBUG


/// @namespace pwx
namespace pwx {


// If any debugging mode is activated, a central logging functions is needed:
#if defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)

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
# define DEBUG_LOG_CAUGHT_STD(...) do{}while(0)
# define DEBUG_ERR(...)            do{}while(0)
# define DEBUG_ERR_THERE(...)      do{}while(0)
/* debug_log() and debug_err() are not defined here, because they would cause
 *   pwx::debug_log(...);
 * to become
 *   pwx::{};
 ' which no compiler is happy with.
*/
#endif // LIBPWX_DEBUG || PWX_THREADDEBUG

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

// Specialized logging macros for mutex locking/unlocking
#ifdef PWX_THREADDEBUG
# define THREAD_LOG(part, fmt, ...)                       \
	::pwx::debug_log( ">> tid 0x%lx;[%8s] %s : " fmt, \
	                  CURRENT_THREAD_ID, part,        \
	                  ::pwx::get_trace_info(          \
	                          __FILE__,               \
	                          __LINE__,               \
	                          PWX_FUNC), __VA_ARGS__ )

# define THREAD_ERR(part, fmt, ...) {                     \
	::pwx::debug_err( ">> tid 0x%lx;[%8s] %s : " fmt, \
	                  CURRENT_THREAD_ID, part,        \
	                  ::pwx::get_trace_info(          \
	                          __FILE__,               \
	                          __LINE__,               \
	                          PWX_FUNC), __VA_ARGS__ )

# define DEBUG_LOCK_STATE(action, locker, to_lock)      \
	THREAD_LOG("DLS", "%s->%s(%s) %s has %u locks"  \
		   "(state \"%s\") owned by tid 0x%lx", \
		   #locker, action, #to_lock, #to_lock, \
		   (to_lock)->lock_count(),             \
		   (to_lock)->is_locked()               \
			? "locked" : "unlocked",        \
		   (to_lock)->owner_thread_id() )

# define LOG_LOCK(obj) if ( (obj)->is_locking() )  \
	THREAD_LOG("LOCK",                         \
		   "Locked %s (has %d locks now)", \
		   #obj, (obj)->lock_count() )

# define LOG_UNLOCK(obj) if ((obj)->is_locking())    \
	THREAD_LOG("UNLOCK",                         \
		   "Unlocked %s (has %d locks now)", \
		   #obj, (obj)->lock_count() )


// Lock guard one object
# define LOG_LOCK_GUARD(obj) if ((obj)->is_locking()) \
	THREAD_LOG("GUARD", "Guarded %s (has %d locks now)", #obj, (obj)->lock_count())


# define LOG_UNLOCK_GUARD(obj) if ((obj)->is_locking()) \
	THREAD_LOG("GUARD", "Un-guarding %s (has %d locks now)", #obj, (obj)->lock_count())


// Lock guard two objects
# define LOG_DOUBLE_LOCK_GUARD(objA, objB)   \
	LOG_LOCK_GUARD(objA);                \
	LOG_LOCK_GUARD(objB);
# define LOG_DOUBLE_UNLOCK_GUARD(objA, objB) \
	LOG_UNLOCK_GUARD(objA);              \
	LOG_UNLOCK_GUARD(objB);

// Lock guard three objects
# define LOG_TRIPLE_LOCK_GUARD(objA, objB, objC) \
	LOG_LOCK_GUARD(objA);                    \
	LOG_LOCK_GUARD(objB);                    \
	LOG_LOCK_GUARD(objC);
# define LOG_TRIPLE_UNLOCK_GUARD(objA, objB, objC) \
	LOG_UNLOCK_GUARD(objA);                    \
	LOG_UNLOCK_GUARD(objB);                    \
	LOG_UNLOCK_GUARD(objC);
#else
# define THREAD_LOG(...)
# define THREAD_ERR(...)
# define DEBUG_LOCK_STATE(action, locker, to_lock)
# define LOG_LOCK(...)
# define LOG_UNLOCK(...)
# define LOG_LOCK_GUARD(...)
# define LOG_UNLOCK_GUARD(...)
# define LOG_DOUBLE_LOCK_GUARD(...)
# define LOG_DOUBLE_UNLOCK_GUARD(...)
# define LOG_TRIPLE_LOCK_GUARD(...)
# define LOG_TRIPLE_UNLOCK_GUARD(...)
#endif // PWX_THREADDEBUG

/** @def  THREAD_LOG
  * @brief Almost the same as DEBUG_LOG, the current thread id is added.
  * This macro is only active in debug mode when PWX_THREADDEBUG is defined.
  *
  * @def  THREAD_ERR
  * @brief Almost the same as DEBUG_ERR, the current thread id is added.
  * This macro is only active in debug mode when PWX_THREADDEBUG is defined.
  *
  * @def  DEBUG_LOCK_STATE
  * @brief Special macro to log locking states.
  * **IMPORTANT**: If @a to_lock is anything else but an object derived from pwx::CLockable,
  * this macro will probably cause crashes or won't even compile.
  * @param action C-String with the name of the function that will be called without parentheses.
  * @param locker object that is calling @a action()
  * @param to_lock pointer to a CLockable derived object that is going to be locked/used
  *
  * @def  LOG_LOCK
  * @brief Use THREAD_LOG to log when @a obj is locked.
  * This macro is only active in debug mode when PWX_THREADDEBUG is defined.
  *
  * @def  LOG_UNLOCK
  * @brief Use THREAD_LOG to log when @a obj is unlocked.
  * This macro is only active in debug mode when PWX_THREADDEBUG is defined.
  *
  * @def  LOG_LOCK_GUARD
  * @brief Use THREAD_LOG to log when @a obj is guarded using CLockGuard.
  * This macro is only active in debug mode when PWX_THREADDEBUG is defined.
  *
  * @def  LOG_UNLOCK_GUARD
  * @brief Use THREAD_LOG to log when @a obj is no longer guarded by CLockGuard.
  * This macro is only active in debug mode when PWX_THREADDEBUG is defined.
  *
  * @def  LOG_DOUBLE_LOCK_GUARD
  * @brief Use THREAD_LOG to log when @a objA and @a objB are guarded using CLockGuard.
  * This macro is only active in debug mode when PWX_THREADDEBUG is defined.
  *
  * @def  LOG_DOUBLE_UNLOCK_GUARD
  * @brief Use THREAD_LOG to log when @a objA and objB are no longer guarded by CLockGuard.
  * This macro is only active in debug mode when PWX_THREADDEBUG is defined.
  *
  * @def  LOG_TRIPLE_LOCK_GUARD
  * @brief Use THREAD_LOG to log when @a objA, @a objB and @a objC are guarded using CLockGuard.
  * This macro is only active in debug mode when PWX_THREADDEBUG is defined.
  *
  * @def  LOG_TRIPLE_UNLOCK_GUARD
  * @brief Use THREAD_LOG to log when @a objA, @a objB and objC are no longer guarded by CLockGuard.
  * This macro is only active in debug mode when PWX_THREADDEBUG is defined.
**/


} // namespace pwx
#endif // PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED

