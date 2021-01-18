#ifndef PWX_LIBPWX_PWX_TYPES_SARGERROR_H_INCLDUED
#define PWX_LIBPWX_PWX_TYPES_SARGERROR_H_INCLDUED 1
#pragma once

/** @file sArgError.h
  *
  * @brief Declaration of struct sArgError for CArgHandler error handling
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


#include "basic/pwx_compiler.h"

#include "arg_handler/eArgErrorNumber.h"


/// @namespace pwx
namespace pwx {


/** @struct sArgError
  * @brief tiny struct to hold an error number and text together.
**/
struct sArgError {
	int32_t     arg_errno; //!< number of the error, taken from eArgErrorNumber
	char const* arg_error; //!< String with the error text

	// Note: The prefix arg_ is needed, or the preprocessor might
	//       substitute "errno" with "(*__errno_location ())"

	/** @brief default ctor
	  *
	  * @param[in] errno_ error number of the error
	  * @param[in] error_ text describing the error
	  */
	explicit sArgError( eArgErrorNumber errno_, char const* error_ ) noexcept;


	/// @brief default dtor
	~sArgError() noexcept;

	// No empty ctor, no copying
	sArgError()                               PWX_DELETE;
	sArgError( sArgError const& )             PWX_DELETE;
	sArgError( sArgError const&& )            PWX_DELETE;
	sArgError& operator=( sArgError const&  ) PWX_DELETE;
	sArgError& operator=( sArgError const&& ) PWX_DELETE;

};


// Operators needed for pwx container storage:

/// @brief return true if both errors have the same errno
bool operator==( const sArgError& lhs, const sArgError& rhs ) noexcept;


/// @brief return true if lhs.errno is greater than rhs.errno
bool operator>( const sArgError& lhs, const sArgError& rhs ) noexcept;


} // namespace pwx


#endif // PWX_LIBPWX_PWX_TYPES_SARGERROR_H_INCLDUED

