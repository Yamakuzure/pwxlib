#pragma once
#ifndef PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED
#define PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED 1

/** @file debug.h
  *
  * @brief Debugging utilities that do nothing unless LIBPWX_DEBUG is defined.
  *
  * Macros and functions to react on the various debugging modes.
  * They are designed to be used everywhere appropriate, getting
  * out of the way when the needed mode is not set.
  *
  * (c) 2007 - 2013 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
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

// Handle includes and defines
#if defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)
# include <cstdio>
# include <cstring>
# include <atomic>
#endif // LIBPWX_DEBUG || PWX_THREADDEBUG

#if defined(PWX_ANNOTATIONS)
/* Add Race Detector annotations.
 * See http://gcc.gnu.org/onlinedocs/libstdc++/manual/debug.html#debug.races why
 */
#include <valgrind/helgrind.h>
# define _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(A) ANNOTATE_HAPPENS_BEFORE(A)
# define _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(A)  ANNOTATE_HAPPENS_AFTER(A)
#endif // ANNOTATIONS


namespace pwx {

// If any debugging mode is activated, a central logging functions is needed:
#if defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)

// The central log needs a log lock:
static std::atomic_flag _pwx_internal_LOG_output_lock = ATOMIC_FLAG_INIT;

// The main logging function:
void debug_log(const char* fmt, ...);

// And the main wrapper:
# define DEBUG_LOG(part, fmt, ...) { \
	char trace_info[1024]; \
	snprintf(trace_info, 256, "[%s] %s:%d - %s : %s\n", part, basename(__FILE__), __LINE__, __FUNCTION__, fmt); \
	while (_pwx_internal_LOG_output_lock.test_and_set(std::memory_order_acquire)) \
		std::this_thread::yield(); \
	pwx::debug_log(trace_info, __VA_ARGS__); \
	_pwx_internal_LOG_output_lock.clear(std::memory_order_release); \
}
#else
# define DEBUG_LOG(...) {}
# define debug_log(...) {} // Just in case someone uses it directly...
#endif // LIBPWX_DEBUG || PWX_THREADDEBUG

// Specialized logging macros for mutex locking/unlocking
#ifdef PWX_THREADDEBUG
# define THREAD_LOG          DEBUG_LOG
# define LOG_LOCK(obj)       THREAD_LOG("LOCK", "%s has %d locks", #obj, obj->lock_count())
# define LOG_UNLOCK(obj)     THREAD_LOG("UNLOCK", "%s has %d locks", #obj, obj->lock_count())
# define LOG_LOCK_GUARD(obj) THREAD_LOG("GUARD", "%s has %d locks", #obj, obj->lock_count())
#else
# define THREAD_LOG(...) {}
# define LOG_LOCK(...) {}
# define LOG_UNLOCK(...) {}
# define LOG_LOCK_GUARD(...) {}
#endif // PWX_THREADDEBUG

} // namespace pwx
#endif // PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED

