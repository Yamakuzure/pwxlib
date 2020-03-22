#ifndef PWX_PWXLIB_BASE_MACROS_H_INCLUDED
#define PWX_PWXLIB_BASE_MACROS_H_INCLUDED 1
#pragma once

/** @file pwx_macros.h
  *
  * @brief Preprocessor macros for general usage.
  *
  * Note: You have to include the appropriate (system) headers yourself
  *       when using any of these macros.
  *
  * (c) 2007 - 2020 PrydeWorX
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


#include "basic/pwx_compiler.h"


/* -----------------------------------------------------------------------
 * --- Various attributes and functions that are not universal         ---
 * -------------------------------------------------------------------- */

#ifndef PWX_NODOX
#if PWX_IS_MSVC
// Attributes
#  define PWX_PRINTF(...)
#  define PWX_ALLOC(...)
#  define PWX_UNUSED          [[maybe_unused]]
#  define PWX_DEPRECATED      [[deprecated]]
#  define PWX_PACKED
#  define PWX_MALLOC
#  define PWX_WEAK
#  define PWX_LIKELY(x)       (x)
#  define PWX_UNLIKELY(x)     (x)
#  define PWX_WARNUNUSED      _Check_return_
// Functions
#  define strcasecmp          _stricmp
#  define strdup              _strdup
#  define strerror_r(n, b, l) ( strerror_s(b, l, n) ? b : "strerror_s failed" )
#  define strncasecmp         _strnicmp
#else
#  define PWX_PRINTF(a,b) __attribute__ ((format (printf, a, b)))
#  if PWX_IS_CLANG
#    define PWX_ALLOC(...)
#  else
#    define PWX_ALLOC(...) __attribute__ ((alloc_size(__VA_ARGS__)))
#  endif // CLang does not know this attribute, yet.
#  define PWX_UNUSED       __attribute__ ((unused))
#  define PWX_DEPRECATED   __attribute__ ((deprecated))
#  define PWX_PACKED       __attribute__ ((packed))
#  define PWX_MALLOC       __attribute__ ((malloc))
#  define PWX_WEAK         __attribute__ ((weak))
#  define PWX_LIKELY(x)   (__builtin_expect(!!(x),1))
#  define PWX_UNLIKELY(x) (__builtin_expect(!!(x),0))
#  define PWX_WARNUNUSED  __attribute__ ((warn_unused_result))
#endif // Macros only for gcc/clang


#if PWX_IS_GCC && __GNUC__ >= 7
#  define PWX_FALLTHROUGH   __attribute__((fallthrough))
#else
#  define PWX_FALLTHROUGH
#endif


#if PWX_IS_MSVC || (PWX_IS_GCC && __GNUC__ >= 9)
#  define _maybe_unused  [[maybe_unused]]
#else
#  define _maybe_unused_
#endif


#define VOID_0 ((void)0)
#define ELEMENTSOF(x) ({                              \
	static_assert( std::rank<typeof(x)>::value ); \
	size_t _res_ = sizeof(x)/sizeof((x)[0]);      \
	_res_;                                        \
})


#define PWX_FUNC  __func__


#endif // ignored by doxygen


/** @brief Alias for the current threads get_id()
  *
  * *Prerequisites*: thread
  *
  * @return The std::thread::id of the current thread.
**/
#if PWX_IS_MSVC
#  define CURRENT_THREAD_ID _Thrd_id()
#else
#  define CURRENT_THREAD_ID static_cast<size_t>(__gthread_self())
#endif // MSVC vs gcc/clang


/* -----------------------------------------------------------------------
 * --- General purpose macros with documentation                       ---
 * -------------------------------------------------------------------- */

/** @brief Return the sign as -1 or +1 of an expression
  *
  * *Prerequisites*: none
  * @param[in] expr any expression that can be compared against 0 (zero)
  * @return -1 if @a expr < 0, 1 otherwise
**/
#define SIGN(expr) (((expr) < 0) ? -1 : 1)


/** @brief Simple try{} wrapper
  *
  * This macro can be used for single try { ONE_FUNC_CALL } - entries.
  * It is possible to use more than one call by simply adding them
  * separated by semicolons, but it won't improve readability though.
  *
  * *Prerequisites*: none
  *
  * @param[in] func the function body within the try {} statement without final semicolon.
**/
#define PWX_TRY(func) \
	try {         \
		func; \
	}


/** @brief throw wrapper to throw a `pwx::CException` with trace information
  *
  * This macro fills in positional information before throwing `pwx::CException`.
  *
  * *Prerequisites*: pwx/types/CException.h
  *
  * @param[in] name char const name of the exception
  * @param[in] msg char const message to be returned by the exceptions what() method
  * @param[in] desc char const message to be returned by the exceptions desc() method
**/
#define PWX_THROW(name, msg, desc) {                                      \
	::pwx::CException _pwx_exception(                                 \
	                        strlen(name) ? name : "no name",          \
	                        strlen(msg)  ? msg  : "no message",       \
	                        ::pwx::get_trace_info(__FILE__, __LINE__, \
	                                              PWX_FUNC),          \
	                        __PRETTY_FUNCTION__,                      \
	                        strlen(desc) ? desc : "no description");  \
	throw(_pwx_exception);                                            \
}


/** @brief catch wrapper to add positional information and re-throw the caught exception
  *
  * This macro catches any `pwx::CException exception`, adds positional
  * data to the trace, and re-throws the exception.
  *
  * *Prerequisites*: pwx/types/CException.h
**/
#define PWX_THROW_PWX_FURTHER catch(::pwx::CException &e) {  \
	e.addToTrace(::pwx::get_trace_msg("--> Called by",   \
	             __FILE__, __LINE__, PWX_FUNC));         \
	throw e;                                             \
}


/** @brief catch wrapper for std::exception to add positional information and throw a `pwx::CException`
  *
  * This macro can be used where an `std::exception` is to be caught to transform it into
  * a tracking `pwx::CException`. The message will always be the return value of the
  * caught exceptions what() method.
  *
  * *Prerequisites*: pwx/types/CException.h
  *
  * @param[in] name char const name of the exception
  * @param[in] desc char const message to be returned by the exceptions `desc()` method
**/
#define PWX_THROW_STD_FURTHER(name, desc)       \
	catch(std::exception &e) {              \
		PWX_THROW(name, e.what(), desc) \
	}


/** @brief catch wrapper for pwx::CException and std::exception
  *
  * This macro can be used where both a `pwx::CExceptioǹ` or an `std::exception` can
  * be caught. The latter is transformed it into a tracking `pwx::CException`.
  * If an `std::exception` is caught, the message will always be the return value
  * of the caught exceptions `what()` method.
  *
  * *Prerequisites*: pwx/types/CException.h
  *
  * @param[in] name char const name of the exception for `std::exception`
  * @param[in] desc char const message to be returned by the exceptions `desc()` method if an `std::exception` is caught.
**/
#define PWX_THROW_PWXSTD_FURTHER(name, desc) \
	PWX_THROW_PWX_FURTHER                \
	PWX_THROW_STD_FURTHER(name, desc)


/** @brief try and throw `pwx::CExceptions` further
  *
  * This macro is a convenience wrapper to have a try and a delegation
  * of a possibly thrown `pwx::CException` in one call.
  *
  * *Prerequisites*: pwx/types/CException.h
  *
  * @param[in] func the function body within the `try{}` statement without final semicolon.
**/
#define PWX_TRY_PWX_FURTHER(func)     \
	PWX_TRY(func)         \
	PWX_THROW_PWX_FURTHER


/** @brief try and throw `std::exception` as `pwx::CExceptions` further
  *
  * This macro is a convenience wrapper to have a try and a delegation
  * of a possibly thrown `std::exception`, that is transformed into a
  * `pwx::CException`, further in one call
  *
  * *Prerequisites*: pwx/types/CException.h
  *
  * @param[in] func the function body within the `try{}` statement without final semicolon.
  * @param[in] name char const name of the exception.
  * @param[in] desc char const message to be returned by the exceptions `desc()` method.
**/
#define PWX_TRY_STD_FURTHER(func, name, desc)     \
	PWX_TRY(func)                     \
	PWX_THROW_STD_FURTHER(name, desc)


/** @brief try and throw both `std::exception` and `pwx::CExceptions` further
  *
  * This macro is a convenience wrapper to have a try and a delegation
  * of a possibly thrown `pwx::CException`, that can be a transformation
  * of an `std::exception`, in one call
  *
  * *Prerequisites*: pwx/types/CException.h
  *
  * @param[in] func the function body within the `try{}` statement without final semicolon.
  * @param[in] name char const name of the exception for `std::exception`.
  * @param[in] desc char const message to be returned by the exceptions `desc()` method if an `std::exception` is caught.
**/
#define PWX_TRY_PWXSTD_FURTHER(func, name, desc)   \
	PWX_TRY(func)                      \
	PWX_THROW_PWX_FURTHER              \
	PWX_THROW_STD_FURTHER(name, desc)


/** @brief This catches and ignores an exception.
  *
  * When a specific exception can occur but does not need any actions,
  * it can be ignored with this macro.
  *
  * *Prerequisites*: none
  *
  * @param[in] except anything that can be "caught".
**/
#define PWX_CATCH_AND_FORGET(except) catch(except&) { }


/** @brief Use `object->lock()` if @a object is not `nullptr`
  *
  * *Prerequisites*: pwx/types/CLockable.h
  *
  * @param object pointer to the object to lock.
**/
#define PWX_LOCK(object)    \
if (nullptr != (object) ) { \
	(object)->lock();   \
}


/** @brief Use `object->lock()`, @a object will be asserted
  *
  * The background for this slightly changed macro is gcc-7
  * throwing out warnings if the address of a local value or
  * the 'this'-pointer is used in a non-null-comparison.
  *
  * If your compiler freaks out with `PWX_LOCK(foo);`, and you
  * are very certain that 'foo' can never be `nullptr`, then
  * just use `PWX_LOCK_OBJ(foo);` instead.
  *
  * *Prerequisites*: pwx/types/CLockable.h
  *
  * @param object pointer to the object to lock.
**/
#define PWX_LOCK_OBJ(object) \
	assert (object);     \
	(object)->lock()


/** @brief Use `object->try_lock89` if @a object is defined
  *
  * *Prerequisites*: pwx/types/CLockable.h
  *
  * @param object pointer to the object to try_lock.
  * @return true if the lock could be acquired, false otherwise
**/
#define PWX_TRY_LOCK(object) ((object) ? (object)->try_lock() : false)


/** @brief Use `object->unlock()` if @a object is defined.
  *
  * *Prerequisites*: pwx/types/CLockable.h
  *
  * @param object pointer to the object to unlock.
**/
#define PWX_UNLOCK(object)  \
if (object) {               \
	(object)->unlock(); \
}


/** @brief Use `object->unlock()`, @a object is asserted.
  *
  * The background for this slightly changed macro is gcc-7
  * throwing out warnings if the address of a local value or
  * the 'this'-pointer is used in a non-null-comparison.
  *
  * If your compiler freaks out with `PWX_UNLOCK(foo);`, and you
  * are very certain that 'foo' can never be `nullptr`, then
  * just use `PWX_UNLOCK_OBJ(foo);` instead.
  *
  * *Prerequisites*: pwx/types/CLockable.h
  *
  * @param object pointer to the object to unlock.
**/
#define PWX_UNLOCK_OBJ(object) { \
	assert(object);          \
	(object)->unlock();      \
}


/** @brief Do an `object->unlock()`, `object->lock()` cycle if @a object is defined.
  *
  * *Prerequisites*: pwx/types/CLockable.h
  *
  * @param object pointer to the object to cycle the lock.
**/
#define PWX_RELOCK(object)  \
if (object) {               \
	(object)->unlock(); \
	(object)->lock();   \
}


/** @brief Do an `object->unlock()`, `object->lock()` cycle, @a object is asserted.
  *
  * The background for this slightly changed macro is gcc-7
  * throwing out warnings if the address of a local value or
  * the 'this'-pointer is used in a non-null-comparison.
  *
  * If your compiler freaks out with `PWX_RELOCK(foo);`, and you
  * are very certain that 'foo' can never be `nullptr`, then
  * just use `PWX_RELOCK_OBJ(foo);` instead.
  *
  * *Prerequisites*: pwx/types/CLockable.h
  *
  * @param object pointer to the object to cycle the lock.
**/
#define PWX_RELOCK_OBJ(object) { \
	assert(object);          \
	(object)->unlock();      \
	(object)->lock();        \
}


/** @brief Create a lock guard on the given object, that is unlocked when leaving the current scope
  *
  * If you only need exactly one lock guard in your function/method, you can
  * use `PWX_LOCK_GUARD()`. It will use the function name.
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param object pointer to the object to lock
**/
#define PWX_NAMED_LOCK_GUARD(Name, object) ::pwx::CLockGuard pwx_libpwx_lock_guard_##Name(object)


/** @brief Create a lock guard on the given object, that is unlocked when leaving the current scope
  *
  * If you need more than one lock guard, you have to use
  * `PWX_NAMED_LOCK_GUARD()` for any additional ones.
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param object pointer to the object to lock
**/
#define PWX_LOCK_GUARD(object) PWX_NAMED_LOCK_GUARD(PWX_FUNC, object)


/// @brief Helper for a nullptr CLockable
#define NULL_LOCK static_cast<CLockable*>(nullptr)


/** @brief Clear a named lock guard, unlocking all currently locked objects
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
**/
#define PWX_NAMED_LOCK_GUARD_CLEAR(Name) pwx_libpwx_lock_guard_##Name.reset(NULL_LOCK, NULL_LOCK, NULL_LOCK)


/** @brief Clear the lock guard named after the enclosing function, unlocking all currently held objects
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
**/
#define PWX_LOCK_GUARD_CLEAR() PWX_NAMED_LOCK_GUARD_CLEAR(PWX_FUNC)


/** @brief Reset a lock guard to a new value
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * **Important**: Do not use nullptr or NULL for @a object! Use `PWX_NAMED_LOCK_GUARD_CLEAR()` instead!
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param object pointer to the object to reset the lock guard to
**/
#define PWX_NAMED_LOCK_GUARD_RESET(Name, object) pwx_libpwx_lock_guard_##Name.reset(object)


/** @brief Reset a lock guard to a new value
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * **Important**: Do not use nullptr or NULL for @a object! Use `PWX_LOCK_GUARD_CLEAR()` instead!
  *
  * @param object pointer to the object to reset the lock guard to
**/
#define PWX_LOCK_GUARD_RESET(object) PWX_NAMED_LOCK_GUARD_RESET(PWX_FUNC, object)


/** @brief Create a lock guard on two given objects, which are unlocked when leaving the current scope
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
**/
#define PWX_NAMED_DOUBLE_LOCK_GUARD(Name, objA, objB) \
	::pwx::CLockGuard pwx_libpwx_double_lock_guard_##Name(objA, objB)


/** @brief Create a lock guard on two given objects, which are unlocked when leaving the current scope
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
**/
#define PWX_DOUBLE_LOCK_GUARD(objA, objB) PWX_NAMED_DOUBLE_LOCK_GUARD(PWX_FUNC, objA, objB)


/** @brief Clear a named double lock guard, unlocking all currently locked objects
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
**/
#define PWX_NAMED_DOUBLE_LOCK_GUARD_CLEAR(Name) \
	pwx_libpwx_double_lock_guard_##Name.reset(NULL_LOCK, NULL_LOCK, NULL_LOCK)



/** @brief Clear the double lock guard named after the enclosing function, unlocking all currently held objects
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
**/
#define PWX_DOUBLE_LOCK_GUARD_CLEAR() PWX_NAMED_DOUBLE_LOCK_GUARD_CLEAR(PWX_FUNC)


/** @brief Reset a double lock guard to two new values
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
**/
#define PWX_NAMED_DOUBLE_LOCK_GUARD_RESET(Name, objA, objB) \
	pwx_libpwx_double_lock_guard_##Name.reset(objA, objB)


/** @brief Reset a double lock guard to two new values
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
**/
#define PWX_DOUBLE_LOCK_GUARD_RESET(objA, objB) \
	PWX_NAMED_DOUBLE_LOCK_GUARD_RESET(PWX_FUNC, objA, objB)


/** @brief Create a lock guard on three given objects, which are unlocked when leaving the current scope
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
  * @param objC pointer to the second object to lock
**/
#define PWX_NAMED_TRIPLE_LOCK_GUARD(Name, objA, objB, objC)    \
	::pwx::CLockGuard pwx_libpwx_triple_lock_guard_##Name(objA, objB, objC)


/** @brief Create a lock guard on three given objects, which are unlocked when leaving the current scope
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
  * @param objC pointer to the second object to lock
**/
#define PWX_TRIPLE_LOCK_GUARD(objA, objB, objC) \
	PWX_NAMED_TRIPLE_LOCK_GUARD(PWX_FUNC, objA, objB, objC)


/** @brief Clear a named triple lock guard, unlocking all currently locked objects
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
**/
#define PWX_NAMED_TRIPLE_LOCK_GUARD_CLEAR(Name) \
	pwx_libpwx_triple_lock_guard_##Name.reset(NULL_LOCK, NULL_LOCK, NULL_LOCK)


/** @brief Clear the triple lock guard named after the enclosing function, unlocking all currently held objects
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
**/
#define PWX_TRIPLE_LOCK_GUARD_CLEAR() PWX_NAMED_DOUBLE_LOCK_GUARD_CLEAR(PWX_FUNC)


/** @brief Reset a triple lock guard to two new values
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
  * @param objC pointer to the third object to reset the lock guard to
**/
#define PWX_NAMED_TRIPLE_LOCK_GUARD_RESET(Name, objA, objB, objC) \
	pwx_libpwx_triple_lock_guard_##Name.reset(objA, objB, objC)


/** @brief Reset a triple lock guard to two new values
  *
  * *Prerequisites*: pwx/types/CLockGuard.h
  *
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
  * @param objC pointer to the third object to reset the lock guard to
**/
#define PWX_TRIPLE_LOCK_GUARD_RESET(objA, objB, objC) \
	PWX_NAMED_TRIPLE_LOCK_GUARD_RESET(PWX_FUNC, objA, objB, objC)


/** @brief Check whether file @a f exists
  *
  * *Prerequisites*: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a exists
**/
#define pwx_file_exists(f) (0 == access(f, F_OK))


/** @brief Check whether file @a f is executable
  *
  * *Prerequisites*: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is executable
**/
#define pwx_file_isX(f) (0 == access(f, X_OK))


/** @brief Check whether file @a f is writable
  *
  * *Prerequisites*: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is writable
**/
#define pwx_file_isW(f) (0 == access(f, W_OK))


/** @brief Check whether file @a f is readable
  *
  * *Prerequisites*: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is readable
**/
#define pwx_file_isR(f) (0 == access(f, R_OK))


/** @brief Check whether file @a f is readable and writable
  *
  * *Prerequisites*: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is readable and writable
**/
#define pwx_file_isRW(f) (0 == access(f, W_OK | W_OK))


// Many of them are useless without our trace info getter
#include "basic/pwx_trace_info.h"


#endif // PWX_PWXLIB_BASE_MACROS_H_INCLUDED
