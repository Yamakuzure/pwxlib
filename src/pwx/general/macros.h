#pragma once
#ifndef PWX_PWXLIB_BASE_MACROS_H_INCLUDED
#define PWX_PWXLIB_BASE_MACROS_H_INCLUDED 1

/** @file macros.h
  *
  * @brief preprocessor macros for general usage.
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

// Debug? (Ensure it is loaded if compiler.h isn't needed)
#include <pwx/functions/debug.h>

/** @brief Return the sign as -1 or +1 of an expression
  *
  * <I>Prerequisites</I>: none
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
  * <I>Prerequisites</I>: none
  *
  * @param[in] func the function body within the try {} statement without final semicolon.
**/
#define PWX_TRY(func) \
	try { \
		func; \
	}


/** @brief throw wrapper to throw a pwx::CException with trace information
  *
  * This macro fills in positional information before throwing pwx::CException.
  *
  * <I>Prerequisites</I>: pwx/types/CException.h
  *
  * @param[in] name const char name of the exception
  * @param[in] msg const char message to be returned by the exceptions what() method
  * @param[in] desc const char message to be returned by the exceptions desc() method
**/
#define PWX_THROW(name, msg, desc) { \
		char _pwx_trace_info[256]; \
		sprintf(_pwx_trace_info, "%s:%d %s", basename(__FILE__), __LINE__, __FUNCTION__); \
		throw(pwx::CException(strlen(name) ? name : "no name", strlen(msg) ? msg : "no message", \
							  _pwx_trace_info, __PRETTY_FUNCTION__, \
							  strlen(desc) ? desc : "no description")); \
	}


/** @brief catch wrapper to add positional information and re-throw the caught exception
  *
  * This macro catches any pwx::CException exception derivate, adds positional
  * data to the trace, and re-throws the exception.
  *
  * <I>Prerequisites</I>: pwx/types/CException.h
**/
#define PWX_THROW_PWX_FURTHER catch(::pwx::CException &e) { \
		char _pwx_trace_info[256]; \
		sprintf(_pwx_trace_info, "--> Called by %s:%d %s", basename(__FILE__), __LINE__, __FUNCTION__); \
		e.addToTrace(_pwx_trace_info); \
		throw; \
	}


/** @brief catch wrapper for std::exception to add positional information and throw a pwx::CException
  *
  * This macro can be used where an std::exception is to be caught to transform it into
  * a tracking pwx::CException. The message will always be the return value of the
  * caught exceptions what() method.
  *
  * <I>Prerequisites</I>: pwx/types/CException.h
  *
  * @param[in] name const char name of the exception
  * @param[in] desc const char message to be returned by the exceptions desc() method
**/
#define PWX_THROW_STD_FURTHER(name, desc) catch(std::exception &e) { \
		PWX_THROW(name, e.what(), desc) \
	}


/** @brief catch wrapper for pwx::CException and std::exception
  *
  * This macro can be used where both a pwx::CException or an std::exception can
  * be caught. The latter is transformed it into a tracking pwx::CException.
  * If an std::exception is caught, the message will always be the return value
  * of the caught exceptions what() method.
  *
  * <I>Prerequisites</I>: pwx/types/CException.h
  *
  * @param[in] name const char name of the exception for std::exception
  * @param[in] desc const char message to be returned by the exceptions desc() method if an std::exception is caught.
**/
#define PWX_THROW_PWXSTD_FURTHER(name, desc) \
		PWX_THROW_PWX_FURTHER \
		PWX_THROW_STD_FURTHER(name, desc)


/** @brief try and throw pwx::CExceptions further
  *
  * This macro is a convenience wrapper to have a try and
  * a delegation of a possibly thrown pwx::CException
  * in one call.
  *
  * <I>Prerequisites</I>: pwx/types/CException.h
  *
  * @param[in] func the function body within the try {} statement without final semicolon.
**/
#define PWX_TRY_PWX_FURTHER(func) { \
		PWX_TRY(func) \
		PWX_THROW_PWX_FURTHER \
	}

/** @brief try and throw std::exception as pwx::CExceptions further
  *
  * This macro is a convenience wrapper to have a try and
  * a delegation of a possibly thrown std::exception, that is
  * transformed into a pwx::CException, further in one call
  *
  * <I>Prerequisites</I>: pwx/types/CException.h
  *
  * @param[in] func the function body within the try {} statement without final semicolon.
  * @param[in] name const char name of the exception.
  * @param[in] desc const char message to be returned by the exceptions desc() method.
**/
#define PWX_TRY_STD_FURTHER(func, name, desc) { \
		PWX_TRY(func) \
		PWX_THROW_STD_FURTHER(name, desc) \
	}

/** @brief try and throw both std::exception or pwx::CExceptions further
  *
  * This macro is a convenience wrapper to have a try and
  * a delegation of a possibly thrown pwx::CException, that
  * can be a transformation of an std::exception, in one call
  *
  * <I>Prerequisites</I>: pwx/types/CException.h
  *
  * @param[in] func the function body within the try {} statement without final semicolon.
  * @param[in] name const char name of the exception for std::exception
  * @param[in] desc const char message to be returned by the exceptions desc() method if an std::exception is caught.
**/
#define PWX_TRY_PWXSTD_FURTHER(func, name, desc) { \
		PWX_TRY(func) \
		PWX_THROW_PWX_FURTHER \
		PWX_THROW_STD_FURTHER(name, desc) \
	}


/** @brief This catches and ignores an exception.
  *
  * When a specific exception can occur but does not need any actions,
  * it can be ignored with this macro.
  *
  * <I>Prerequisites</I>: none
  *
  * @param[in] except anything that can be "caught".
**/
#define PWX_CATCH_AND_FORGET(except) catch(except&) { }


/** @brief Alias for the current threads get_id()
  *
  * <I>Prerequisites</I>: thread
  *
  * @return The std::thread::id of the current thread.
**/
#define CURRENT_THREAD_ID static_cast<size_t>(__gthread_self())


/** @brief Use object->lock if @a object is defined
  *
  * <I>Prerequisites</I>: pwx/types/CLockable.h
  *
  * @param object pointer to the object to lock.
**/
#define PWX_LOCK(object) { \
	if (nullptr != object) {\
		(object)->lock(); \
		LOG_LOCK(object) \
	} \
}


/** @brief Use object->try_lock if @a object is defined
  *
  * <I>Prerequisites</I>: pwx/types/CLockable.h
  *
  * @param object pointer to the object to try_lock.
  * @return true if the lock could be acquired, false otherwise
**/
#define PWX_TRY_LOCK(object) ((object) ? (object)->try_lock() : false)


/** @brief Use object->unlock if @a object is defined.
  *
  * <I>Prerequisites</I>: pwx/types/CLockable.h
  *
  * @param object pointer to the object to unlock.
**/
#define PWX_UNLOCK(object) { \
	if (nullptr != object) { \
		(object)->unlock(); \
		LOG_UNLOCK(object) \
	} \
}


/** @brief Create a lock guard on the given object, that is unlocked when leaving the current scope
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param object pointer to the object to lock
**/
#define PWX_NAMED_LOCK_GUARD(Name, object) \
	DEBUG_LOCK_STATE("TLockGuard", this, object) \
	pwx::CLockGuard pwx_libpwx_lock_guard_##Name(object); \
	LOG_LOCK_GUARD(object)


/** @brief Create a lock guard on the given object, that is unlocked when leaving the current scope
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param object pointer to the object to lock
**/
#define PWX_LOCK_GUARD(object) \
	PWX_NAMED_LOCK_GUARD(__FUNCTION__, object)

/** @brief Reset a lock guard to a new value
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param object pointer to the object to reset the lock guard to
**/
#define PWX_NAMED_LOCK_GUARD_RESET(Name, object) \
	LOG_UNLOCK_GUARD(object) \
	pwx_libpwx_lock_guard_##Name.reset(object); \
	LOG_LOCK_GUARD(object)


/** @brief Reset a lock guard to a new value
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param object pointer to the object to reset the lock guard to
**/
#define PWX_LOCK_GUARD_RESET(object) \
	PWX_NAMED_LOCK_GUARD_RESET(__FUNCTION__, object)


/** @brief Create a lock guard on two given objects, which are unlocked when leaving the current scope
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
**/
#define PWX_NAMED_DOUBLE_LOCK_GUARD(Name, objA, objB) \
	DEBUG_LOCK_STATE("CLockGuard A", this, objA) \
	DEBUG_LOCK_STATE("CLockGuard B", this, objB) \
	pwx::CLockGuard pwx_libpwx_double_lock_guard_##Name(objA, objB); \
	LOG_DOUBLE_LOCK_GUARD(objA, objB)


/** @brief Create a lock guard on two given objects, which are unlocked when leaving the current scope
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
**/
#define PWX_DOUBLE_LOCK_GUARD(objA, objB) \
	PWX_NAMED_DOUBLE_LOCK_GUARD(__FUNCTION__, objA, objB)


/** @brief Reset a double lock guard to two new values
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
**/
#define PWX_NAMED_DOUBLE_LOCK_GUARD_RESET(Name, objA, objB) \
	LOG_DOUBLE_UNLOCK_GUARD(objA, objB) \
	pwx_libpwx_double_lock_guard_##Name.reset(objA, objB); \
	LOG_DOUBLE_LOCK_GUARD(objA, objB)


/** @brief Reset a double lock guard to two new values
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
**/
#define PWX_DOUBLE_LOCK_GUARD_RESET(objA, objB) \
	PWX_NAMED_DOUBLE_LOCK_GUARD_RESET(__FUNCTION__, objA, objB)


/** @brief Create a lock guard on three given objects, which are unlocked when leaving the current scope
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
  * @param objC pointer to the second object to lock
**/
#define PWX_NAMED_TRIPLE_LOCK_GUARD(Name, objA, objB, objC) \
	DEBUG_LOCK_STATE("CLockGuard A", this, objA) \
	DEBUG_LOCK_STATE("CLockGuard B", this, objB) \
	DEBUG_LOCK_STATE("CLockGuard C", this, objC) \
	pwx::CLockGuard pwx_libpwx_triple_lock_guard_##Name(objA, objB, objC); \
	LOG_TRIPLE_LOCK_GUARD(objA, objB, objC)


/** @brief Create a lock guard on three given objects, which are unlocked when leaving the current scope
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
  * @param objC pointer to the second object to lock
**/
#define PWX_TRIPLE_LOCK_GUARD(objA, objB, objC) \
	PWX_NAMED_TRIPLE_LOCK_GUARD(__FUNCTION__, objA, objB, objC)


/** @brief Reset a triple lock guard to two new values
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
  * @param objC pointer to the third object to reset the lock guard to
**/
#define PWX_NAMED_TRIPLE_LOCK_GUARD_RESET(Name, objA, objB, objC) \
	LOG_TRIPLE_UNLOCK_GUARD(objA, objB, objC) \
	pwx_libpwx_triple_lock_guard_##Name.reset(objA, objB, objC); \
	LOG_TRIPLE_LOCK_GUARD(objA, objB, objC)


/** @brief Reset a triple lock guard to two new values
  *
  * <I>Prerequisites</I>: pwx/types/CLockGuard.h
  *
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
  * @param objC pointer to the third object to reset the lock guard to
**/
#define PWX_TRIPLE_LOCK_GUARD_RESET(objA, objB, objC) \
	PWX_NAMED_TRIPLE_LOCK_GUARD_RESET(__FUNCTION__, objA, objB, objC)


/** @brief return true if two C-Strings are equal ignoring case
  *
  * <I>Prerequisites</I>: cstring
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if both C-Strings are equal ignoring case
**/
#define STRCEQ(a,b) (strcasecmp(a,b) == 0)


/** @brief return true if two C-Strings are not equal ignoring case
  *
  * <I>Prerequisites</I>: cstring
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if both C-Strings are not equal ignoring case
**/
#define STRCNE(a,b) (strcasecmp(a,b) != 0)


/** @brief true if @a a is "lower" than @a b ignoring case
  *
  * <I>Prerequisites</I>: cstring
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if @a a is "lower" than @a b ignoring case
**/
#define STRCLT(a,b) (strcasecmp(a,b) < 0)


/** @brief true if @a a is "greater" than @a b ignoring case
  *
  * <I>Prerequisites</I>: cstring
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if @a a is "greater" than @a b ignoring case
**/
#define STRCGT(a,b) (strcasecmp(a,b) > 0)


/** @brief return true if two C-Strings are equal
  *
  * <I>Prerequisites</I>: cstring
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if both C-Strings are equal
**/
#define STREQ(a,b) (std::strcmp(a,b) == 0)


/** @brief return true if two C-Strings are not equal
  *
  * <I>Prerequisites</I>: cstring
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if both C-Strings are not equal
**/
#define STRNE(a,b) (std::strcmp(a,b) != 0)


/** @brief true if @a a is "lower" than @a b
  *
  * <I>Prerequisites</I>: cstring
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if @a a is "lower" than @a b
**/
#define STRLT(a,b) (std::strcmp(a,b) < 0)


/** @brief true if @a a is "greater" than @a b
  *
  * <I>Prerequisites</I>: cstring
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if @a a is "greater" than @a b
**/
#define STRGT(a,b) (std::strcmp(a,b) > 0)


/** @brief true if @a a is of the same type as @a b
  *
  * <I>Prerequisites</I>: type_traits
  *
  * @param a left type
  * @param b right type
  * @return true if @a and @a b are of the same type
**/
#define isSameType(a, b) (std::is_same<a, b>::value)


/** @brief true if @a a is a pointer
  *
  * <I>Prerequisites</I>: type_traits
  *
  * @param a type to check
  * @return true if @a is a pointer
**/
#define isPointer(a) (std::is_pointer<a>::value)


/** @brief true if @a a is an integral type
  *
  * <I>Prerequisites</I>: type_traits
  *
  * @param a type to check
  * @return true if @a is an integral type
**/
#define isIntType(a) (std::is_integral<a>::value)


/** @brief true if @a a is a floating point type
  *
  * <I>Prerequisites</I>: type_traits
  *
  * @param a type to check
  * @return true if @a is a floating point type
**/
#define isFloatType(a) (std::is_floating_point<a>::value)


/** @brief true if @a is either int or float type
  *
  * <I>Prerequisites</I>: type_traits
  *
  * @param a type to check
  * @return true if @a is either an integer or a floating point type
**/
#define isNumericType(a) (isIntType(a) || isFloatType(a))


/** @brief true if @a a is an array
  *
  * <I>Prerequisites</I>: type_traits
  *
  * @param a type to check
  * @return true if @a is an array
**/
#define isArrayType(a) (std::is_array<a>::value)


/** @brief Check whether file @a f exists
  *
  * <I>Prerequisites</I>: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a exists
**/
#define pwx_file_exists(f) (0 == access(f, F_OK))


/** @brief Check whether file @a f is executable
  *
  * <I>Prerequisites</I>: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is executable
**/
#define pwx_file_isX(f) (0 == access(f, X_OK))


/** @brief Check whether file @a f is writable
  *
  * <I>Prerequisites</I>: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is writable
**/
#define pwx_file_isW(f) (0 == access(f, W_OK))


/** @brief Check whether file @a f is readable
  *
  * <I>Prerequisites</I>: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is readable
**/
#define pwx_file_isR(f) (0 == access(f, R_OK))


/** @brief Check whether file @a f is readable and writable
  *
  * <I>Prerequisites</I>: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is readable and writable
**/
#define pwx_file_isRW(f) (0 == access(f, W_OK | W_OK))

#endif // PWX_PWXLIB_BASE_MACROS_H_INCLUDED
