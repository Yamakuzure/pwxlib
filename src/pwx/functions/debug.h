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


namespace pwx {

// If any debugging mode is activated, a central logging functions is needed:
#if defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)

// The main logging function:
void debug_log(const char* fmt, ...);
void debug_err(const char* fmt, ...);

// And the main wrapper:
# define DEBUG_LOG(part, fmt, ...) { \
	char trace_info[1024]; \
	snprintf(trace_info, 256, ">> [%8s] %s:%d - %s : %s\n", \
			part, basename(__FILE__), __LINE__, __FUNCTION__, fmt); \
	pwx::debug_log(trace_info, __VA_ARGS__); \
}
# define DEBUG_ERR(part, fmt, ...) { \
	char trace_info[1024]; \
	snprintf(trace_info, 256, ">> [%8s] %s:%d - %s : %s\n", \
			part, basename(__FILE__), __LINE__, __FUNCTION__, fmt); \
	pwx::debug_err(trace_info, __VA_ARGS__); \
}
#else
# define DEBUG_LOG(...) {}
# define DEBUG_ERR(...) {}
# define debug_log(...) {} // Just in case someone uses it directly...
# define debug_err(...) {}
#endif // LIBPWX_DEBUG || PWX_THREADDEBUG

// Specialized logging macros for mutex locking/unlocking
#ifdef PWX_THREADDEBUG
/** @internal
  * Almost same as DEBUG_LOG, just adds current thread id
  * and is deactivated unless PWX_THREADDEBUG is defined.
**/
# define THREAD_LOG(part, fmt, ...) { \
	char trace_info[1024]; \
	snprintf(trace_info, 256, ">> tid 0x%lx;[%8s] %s:%d - %s : %s\n", \
		CURRENT_THREAD_ID, part, \
		basename(__FILE__), __LINE__, __FUNCTION__, fmt); \
	pwx::debug_log(trace_info, __VA_ARGS__); \
}
# define THREAD_ERR(part, fmt, ...) { \
	char trace_info[1024]; \
	snprintf(trace_info, 256, ">> tid 0x%lx;[%8s] %s:%d - %s : %s\n", \
		CURRENT_THREAD_ID, part, \
		basename(__FILE__), __LINE__, __FUNCTION__, fmt); \
	pwx::debug_err(trace_info, __VA_ARGS__); \
}
/** @internal
  * @brief Special macro to log locking states.
  * IMPORTANT: If @a to_lock are anything else but an object derived from pwx::CLockable,
  * this macro will possible cause crashes or won't even compile.
  * @param action C-String with the name of the function that will be called without parentheses.
  * @param locker object that is calling @a action()
  * @param to_lock pointer to a CLockable derived object that is going to be locked/used
 **/
# define DEBUG_LOCK_STATE(action, locker, to_lock) { \
	if (to_lock && to_lock->is_locking()) \
		THREAD_LOG("DLS", "%s->%s(%s) %s has %u locks (state \"%s\") owned by tid 0x%lx", \
					#locker, action, #to_lock, #to_lock, \
					to_lock->CL_Lock_Count.load(), \
					to_lock->is_locked() ? "locked" : "unlocked", \
					to_lock->CL_Thread_ID.load()) \
}
# define LOG_LOCK(obj)       if (obj && obj->is_locking()) \
	THREAD_LOG("LOCK", "Locked %s (has %d locks now)", #obj, obj->lock_count())
# define LOG_UNLOCK(obj)     if (obj && obj->is_locking()) \
	THREAD_LOG("UNLOCK", "Unlocked %s (has %d locks now)", #obj, obj->lock_count())
# define LOG_LOCK_GUARD(obj) if (obj && obj->is_locking()) \
	THREAD_LOG("GUARD", "Guarded %s (has %d locks now)", #obj, obj->lock_count())
#else
# define THREAD_LOG(...) {}
# define THREAD_ERR(...) {}
# define DEBUG_LOCK_STATE(action, locker, to_lock) {}
# define LOG_LOCK(...) {}
# define LOG_UNLOCK(...) {}
# define LOG_LOCK_GUARD(...) {}
#endif // PWX_THREADDEBUG




} // namespace pwx
#endif // PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED

