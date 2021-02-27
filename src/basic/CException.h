#ifndef PWX_LIBPWX_PWX_TYPES_CEXCEPTION_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_CEXCEPTION_H_INCLUDED 1
#pragma once

/** @file CException.h
  *
  * @brief Declaration of CException, a tracing exception
  *
  * CException serves the purpose of a base class that allows to trace the
  * path an exception takes until it is caught. To enable the tracing the
  * macros (see pwx/general/pwx_macros.h) PWX_THROW and PWX_THROW_FURTHER have
  * to be used.
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


#include <exception>
#include <string>

#include "basic/compiler.h"
#include "basic/macros.h"
#include "basic/string_utils.h"


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
#define PWX_THROW(name, msg, desc) {                         \
	::pwx::CException _pwx_exception(                        \
	                        ::pwx::strnull(name),            \
	                        ::pwx::strnull(msg),             \
	                        ::pwx::get_trace_info(__FILE__,  \
	                                              __LINE__,  \
	                                              PWX_FUNC), \
	                        __PRETTY_FUNCTION__,             \
	                        ::pwx::strnull(desc));           \
	throw(_pwx_exception);                                   \
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
	PWX_THROW_PWX_FURTHER                    \
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


/// @namespace pwx
namespace pwx {


/** @class CException PException <PException>
  * @brief Basic exception class with tracing functionality
  *
  * This class is meant to provide a tracing exception to get as much information as possible.
  *
  * To make the most out of this system, the file `basic/macros.h` provides many macros
  * to try, catch and throw further exceptions with tracing information.
  *
  * | Getter    | Returns...                                                         |
  * |:--------- |:------------------------------------------------------------------ |
  * | `name()`  | The name of the thrown exception                                   |
  * | `what()`  | Information about what went wrong                                  |
  * | `where()` | The origin of the exception in the style "file:line - method name" |
  * | `desc()`  | A description that possibly contains data about what happened      |
  * | `pfunc()` | Full function name (and template parameters if applicable)         |
  * | `trace()` | A trace of the exception path if it was filled on its way          |
**/
class PWX_API CException final : public std::exception {

public:

	/* ===============================================
	 * === Public Constructors and destructor      ===
	 * ===============================================
	*/

	/** @brief explicit constructor
	  *
	  * This constructor is to be used to create a pwx::CException. All relevant data
	  * is set exactly once with this constructor and must not be changed apart from
	  * the trace.
	  *
	  * You can use the macro `PWX_THROW()` from `basic/macros.h` to throw this
	  * kind of exceptions in a very convenient way. It'll fill in the positional
	  * information for you. The macro `PWX_THROW_STD_FURTHER()` from the same file
	  * can be used to convert an `std::exception` into a `pwx::CException` with
	  * tracing information.
	  *
	  * @param[in] name_ The name of the exception.
	  * @param[in] what_ The text that is to be returned by the what() method.
	  * @param[in] where_ The positional information where it was thrown.
	  * @param[in] func_ Name of the function where this was thrown.
	  * @param[in] desc_ Description of what wnet wrong.
	  */
	explicit
	CException( char const* name_, char const* what_, char const* where_,
	            char const* func_, char const* desc_ ) noexcept;


	/** @brief copy constructor
	  *
	  * @param[in] src the exception to copy
	  */
	CException( const CException &src ) noexcept;


	CException() PWX_DELETE; // No empty ctor!


	/** @brief destructor
	  *
	  * As all C-Strings are malloc'd they have to be freed.
	  */
	~CException() noexcept final;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	/// @brief Return the name of the exception
	[[nodiscard]] char const* name() const noexcept;


	/// @brief Return the exception reason
	[[nodiscard]] char const* what() const noexcept final;


	/// @brief Return the original throwing position
	[[nodiscard]] char const* where() const noexcept;


	/// @brief Return the exception description
	[[nodiscard]] char const* desc() const noexcept;


	/// @brief Return the pretty function where the original throw occurred.
	[[nodiscard]] char const* pfunc() const noexcept;


	/// @brief Return the trace of the exceptions path
	[[nodiscard]] char const* trace() const noexcept;


	/** @brief addToTrace
	  *
	  * This function adds a line break and then @a trace_ to
	  * the exception trace. Exceptions from string operations
	  * are ignored. It is better to have a broken trace than
	  * a broken unwinding.
	  *
	  * The `PWX_THROW()` macro family in `basic/macros.h`
	  * makes extensive use of this.
	  *
	  * @param[in] trace_ Trace information to append
	  */
	void addToTrace( char const* trace_ ) noexcept;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	CException &operator=( const CException &e ) PWX_DELETE; // No assignment


private:

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	char const* const txtName;  //!< the name of the exception
	char const* const txtWhat;  //!< the classic what() text
	char const* const txtWhere; //!< A malloc'd C-String with "file:line - method"
	char const* const txtFunc;  //!< The result of __PRETTY_FUNC__
	char const* const txtDesc;  //!< Optional description
	std::string txtTrace;       //!< A trace, that can hopefully be added together
};


} // namespace pwx
#endif // PWX_LIBPWX_PWX_TYPES_CEXCEPTION_H_INCLUDED


