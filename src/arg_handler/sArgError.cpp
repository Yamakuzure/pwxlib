/**
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2018 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://prydeworx.com/pwxlib/
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
  * History and change log are maintained in pwx.h
**/


#include <cstdlib>
#include <cstring>

#include "sArgError.h"


namespace pwx {


/** @brief default ctor
  *
  * @param[in] errno_ error number of the error
  * @param[in] error_ text describing the error
  */
sArgError::sArgError(eArgErrorNumber errno_, const char* error_) noexcept :
	arg_errno(static_cast<int32_t>(errno_)),
	arg_error(error_ ? strdup(error_) : nullptr)
{
	/* nothing to do here */
}


/** @brief default dtor
  */
sArgError::~sArgError() noexcept
{
	if (arg_error) free(const_cast<char*>(arg_error));
}


/// @brief return true if both errors have the same errno
bool operator==(const sArgError &lhs, const sArgError &rhs) noexcept
{
	return lhs.arg_errno == rhs.arg_errno;
}

/// @brief return true if lhs.errno is greater than rhs.errno
bool operator>(const sArgError &lhs, const sArgError &rhs) noexcept
{
	return lhs.arg_errno > rhs.arg_errno;
}



} // namespace pwx