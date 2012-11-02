#pragma once
#ifndef PWX_PWXLIB_BASE_MACROS_H_INCLUDED
#define PWX_PWXLIB_BASE_MACROS_H_INCLUDED 1

/** @file macros.h
  *
  * @brief preprocessor macros for general usage.
  *
  * (c) 2007 - 2012 PrydeWorX
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
#define PWX_THROW_STD_FURTHER(name, desc) catch(::std::exception &e) { \
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
#define PWX_TRY_PWX_FURTHER(func) \
	PWX_TRY(func) \
	PWX_THROW_PWX_FURTHER


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
#define PWX_TRY_STD_FURTHER(func, name, desc) \
	PWX_TRY(func) \
	PWX_THROW_STD_FURTHER(name, desc)


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
#define PWX_TRY_STDPWX_FURTHER(func, name, desc) \
	PWX_TRY(func) \
	PWX_THROW_PWX_FURTHER \
	PWX_THROW_STD_FURTHER(name, desc)


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


/** @brief Use object->lock if PWX_THREADS is defined, and trace possible exceptions
  *
  * <I>Prerequisites</I>: pwx/types/CLockable.h
  *
  * @param object pointer to the object to lock.
**/
#if defined(PWX_THREADS)
#  define PWX_LOCK(object) { \
		PWX_TRY(object->lock()) \
		PWX_THROW_FURTHER \
	}
#else
#  define PWX_LOCK(object)
#endif


/** @brief Use object->try_lock if PWX_THREADS is defined
  *
  * <I>Prerequisites</I>: pwx/types/CLockable.h
  *
  * @param object pointer to the object to try_lock.
  * @return true if the lock could be acquired, false otherwise
**/
#if defined(PWX_THREADS)
#  define PWX_TRY_LOCK(object) (object->try_lock())
#else
#  define PWX_TRY_LOCK(object)
#endif


/** @brief Use object->unlock if PWX_THREADS is defined, and trace possible exceptions
  *
  * <I>Prerequisites</I>: pwx/types/CLockable.h
  *
  * @param object pointer to the object to unlock.
**/
#if defined(PWX_THREADS)
#  define PWX_UNLOCK(object) { \
		PWX_TRY(object->unlock()) \
		PWX_THROW_FURTHER \
	}
#else
#  define PWX_UNLOCK(object)
#endif


/** @brief Impose a lock guard on the given object, that is unlocked when leaving the current scope
  *
  * <I>Prerequisites</I>: pwx/types/CLockable.h
  *
  * @param name a string to add to the local variable name to be able to use more than one guard
  * @param T the type of the object to lock
  * @param object pointer to the object to lock
**/
#if defined(PWX_THREADS)
#  define PWX_NAMED_LOCK_GUARD(Name, T, object) \
	::std::lock_guard<T> pwx_libpwx_lock_guard_##Name(*object);
#else
#  define PWX_LOCK_GUARD(Name, T, object)
#endif


/** @brief Impose a lock guard on the given object, that is unlocked when leaving the current scope
  *
  * <I>Prerequisites</I>: pwx/types/CLockable.h
  *
  * @param T the type of the object to lock
  * @param object pointer to the object to lock
**/
#if defined(PWX_THREADS)
#  define PWX_LOCK_GUARD(T, object) \
	PWX_NAMED_LOCK_GUARD(Default, T, object)
#else
#  define PWX_LOCK_GUARD(T, object)
#endif


/** @brief Lock two elements simultaneously
  *
  * <I>Prerequisites</I>: pwx/types/CLockable.h
  *
  * @param Ta the type of the first object to lock
  * @param objA pointer to the first object to lock
  * @param Tb the type of the second object to lock
  * @param objB pointer to the second object to lock
**/
#if defined(PWX_THREADS)
#  define PWX_DOUBLE_LOCK(Ta, objA, Tb, objB) \
	::std::unique_lock<Ta> pwx_libpwx_double_lock_A(*objA, ::std::defer_lock); \
	::std::unique_lock<Tb> pwx_libpwx_double_lock_B(*objB, ::std::defer_lock); \
	::std::lock(pwx_libpwx_double_lock_A, pwx_libpwx_double_lock_B);
#else
#  define PWX_LOCK_DEFER(Ta, objA, Tb, objB)
#endif


/** @brief return true if two C-Strings are equal ignoring case
  *
  * <I>Prerequisites</I>: <cstring>
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if both C-Strings are equal ignoring case
**/
#define STRCEQ(a,b) (strcasecmp(a,b) == 0)


/** @brief return true if two C-Strings are not equal ignoring case
  *
  * <I>Prerequisites</I>: <cstring>
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if both C-Strings are not equal ignoring case
**/
#define STRCNE(a,b) (strcasecmp(a,b) != 0)


/** @brief true if @a a is "lower" than @a b ignoring case
  *
  * <I>Prerequisites</I>: <cstring>
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if @a a is "lower" than @a b ignoring case
**/
#define STRCLT(a,b) (strcasecmp(a,b) < 0)


/** @brief true if @a a is "greater" than @a b ignoring case
  *
  * <I>Prerequisites</I>: <cstring>
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if @a a is "greater" than @a b ignoring case
**/
#define STRCGT(a,b) (strcasecmp(a,b) > 0)


/** @brief return true if two C-Strings are equal
  *
  * <I>Prerequisites</I>: <cstring>
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if both C-Strings are equal
**/
#define STREQ(a,b) (::std::strcmp(a,b) == 0)


/** @brief return true if two C-Strings are not equal
  *
  * <I>Prerequisites</I>: <cstring>
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if both C-Strings are not equal
**/
#define STRNE(a,b) (::std::strcmp(a,b) != 0)


/** @brief true if @a a is "lower" than @a b
  *
  * <I>Prerequisites</I>: <cstring>
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if @a a is "lower" than @a b
**/
#define STRLT(a,b) (::std::strcmp(a,b) < 0)


/** @brief true if @a a is "greater" than @a b
  *
  * <I>Prerequisites</I>: <cstring>
  *
  * @param a left hand C-String
  * @param b right hand C-String
  * @return true if @a a is "greater" than @a b
**/
#define STRGT(a,b) (::std::strcmp(a,b) > 0)


/** @brief true if @a a is of the same type as @a b
  *
  * <I>Prerequisites</I>: <type_traits>
  *
  * @param a left type
  * @param b right type
  * @return true if @a and @a b are of the same type
**/
#define isSameType(a, b) (::std::is_same<a, b>::value)


/** @brief true if @a a is a pointer
  *
  * <I>Prerequisites</I>: <type_traits>
  *
  * @param a type to check
  * @return true if @a is a pointer
**/
#define isPointer(a) (::std::is_pointer<a>::value)


/** @brief true if @a a is an integral type
  *
  * <I>Prerequisites</I>: <type_traits>
  *
  * @param a type to check
  * @return true if @a is an integral type
**/
#define isIntType(a) (::std::is_integral<a>::value)


/** @brief true if @a a is a floating point type
  *
  * <I>Prerequisites</I>: <type_traits>
  *
  * @param a type to check
  * @return true if @a is a floating point type
**/
#define isFloatType(a) (::std::is_floating_point<a>::value)


/** @brief true if @a a is an array
  *
  * <I>Prerequisites</I>: <type_traits>
  *
  * @param a type to check
  * @return true if @a is an array
**/
#define isArrayType(a) (::std::is_array<a>::value)


#endif // PWX_PWXLIB_BASE_MACROS_H_INCLUDED
