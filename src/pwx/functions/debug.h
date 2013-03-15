#pragma once
#ifndef PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED
#define PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED 1

/** @file
  *
  * @brief debug.h
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

#ifdef LIBPWX_DEBUG
# include <cstdio>
# include <cstring>
# ifdef PWX_THREADS
#   include <mutex>
#   define PWX_THREADDEBUG 1
# endif // PWX_THREADSS
#endif // LIBPWX_DEBUG

namespace pwx {

// If any debugging mode is activated, a central logging functions is needed:
#ifdef LIBPWX_DEBUG

// The central log needs a log mutex if multi threading is used:
# ifdef PWX_THREADDEBUG
	static std::recursive_mutex _pwx_internal_LOG_mutex;
#   define _LOCK_LOG_MUTEX   { pwx::_pwx_internal_LOG_mutex.lock();   }
#   define _UNLOCK_LOG_MUTEX { pwx::_pwx_internal_LOG_mutex.unlock(); }
# else
#   define _LOCK_LOG_MUTEX   { }
#   define _UNLOCK_LOG_MUTEX { }
#endif // PWX_THREADDEBUG

// The main logging function:
void debug_log(const char* fmt, ...);

// And the main wrapper:
# define DEBUG_LOG(part, fmt, ...) { \
	char trace_info[1024]; \
	snprintf(trace_info, 256, "[%s] %s:%d - %s : %s\n", part, basename(__FILE__), __LINE__, __FUNCTION__, fmt); \
	_LOCK_LOG_MUTEX \
	pwx::debug_log(trace_info, __VA_ARGS__); \
	_UNLOCK_LOG_MUTEX \
}
#else
# define DEBUG_LOG(...) {}
# define debug_log(...) {} // Just in case someone uses it directly...
#endif // LIBPWX_DEBUG

// Specialized logging macros for mutex locking/unlocking
#ifdef PWX_THREADDEBUG
# define LOG_LOCK(obj)       DEBUG_LOG("LOCK", "%s has %d locks", #obj, obj->count())
# define LOG_UNLOCK(obj)     DEBUG_LOG("ÚNLOCK", "%s has %d locks", #obj, obj->count())
# define LOG_LOCK_GUARD(obj) DEBUG_LOG("GUARD", "%s has %d locks", #obj, obj->count())
#else
# define DEBUG_LOG(...) {}
# define LOG_LOCK(...) {}
# define LOG_UNLOCK(...) {}
# define LOG_LOCK_GUARD(...) {}
#endif // PWX_THREADDEBUG

// Specialized templates to get/set next/prev pointers with checks and logging:
#ifdef LIBPWX_DEBUG
template<typename T> T* _debug_get_next(T* obj)
{
#ifdef PWX_THREADS
	if (obj) return (obj)->getNext();
#else
	if (obj) return (obj)->next;
#endif // PWX_THREADS
	DEBUG_LOG("GET NEXT", "%s is nullptr!", "obj")
	return nullptr;
}

template<typename T> T* _debug_get_prev(T* obj)
{
#ifdef PWX_THREADS
	if (obj) return (obj)->getPrev();
#else
	if (obj) return (obj)->prev;
#endif // PWX_THREADS
	DEBUG_LOG("GET PREV", "%s is nullptr!", "obj")
	return nullptr;
}

template<typename T> void _debug_set_next(T* obj, void* new_next)
{
#ifdef PWX_THREADS
	if (obj) (obj)->setNext(static_cast<T*>(new_next));
#else
	if (obj) (obj)->next = static_cast<T*>(new_next);
#endif // PWX_THREADS
	else DEBUG_LOG("SET NEXT", "%s is nullptr!", "obj")
}

template<typename T> void _debug_set_prev(T* obj, void* new_prev)
{
#ifdef PWX_THREADS
	if (obj) (obj)->setPrev(static_cast<T*>(new_prev));
#else
	if (obj) (obj)->prev = static_cast<T*>(new_prev);
#endif // PWX_THREADS
	else DEBUG_LOG("SET PREV", "%s is nullptr!", "obj")
}
#endif // LIBPWX_DEBUG



} // namespace pwx
#endif // PWX_LIBPWX_SRC_FUNCTIONS_DEBUG_H_INCLUDED

