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


/// @namespace pwx
namespace pwx {


/** @class CException PException <PException>
  * @brief Basic exception class with tracing functionality
  *
  * This class is meant to provide a tracing exception to get as much information as possible.
  *
  * To make the most out of this system, the file `basic/pwx_macros.h` provides many macros
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
class PWX_API CException : public std::exception {

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
	  * You can use the macro `PWX_THROW()` from `basic/pwx_macros.h` to throw this
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
	CException ( char const* const name_, char const* const what_, char const* const where_,
	             char const* const func_, char const* const desc_ ) noexcept;


	/** @brief copy constructor
	  *
	  * @param[in] src the exception to copy
	  */
	CException ( const CException& src ) noexcept;


	CException () PWX_DELETE; // No empty ctor!


	/** @brief destructor
	  *
	  * As all C-Strings are malloc'd they have to be freed.
	  */
	virtual ~CException() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	/// @brief Return the name of the exception
	char const* name () const noexcept;


	/// @brief Return the exception reason
	char const* what () const noexcept;


	/// @brief Return the original throwing position
	char const* where () const noexcept;


	/// @brief Return the exception description
	char const* desc () const noexcept;


	/// @brief Return the pretty function where the original throw occurred.
	char const* pfunc () const noexcept;


	/// @brief Return the trace of the exceptions path
	char const* trace () const noexcept;


	/** @brief addToTrace
	  *
	  * This function adds a line break and then @a trace_ to
	  * the exception trace. Exceptions from string operations
	  * are ignored. It is better to have a broken trace than
	  * a broken unwinding.
	  *
	  * The `PWX_THROW()` macro family in `basic/pwx_macros.h`
	  * makes extensive use of this.
	  *
	  * @param[in] trace_ Trace information to append
	  */
	void addToTrace ( char const* trace_ ) noexcept;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	CException& operator= ( const CException& e ) PWX_DELETE; // No assignment


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


