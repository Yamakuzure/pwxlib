/***
  This file is part of the PrydeWorX Library (pwxLib).

  Copyright 2007 - 2017 Sven Eden

  The PrydeWorX Library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  The PrydeWorX Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with pwxLib; If not, see <http://www.gnu.org/licenses/>.

  History and Changelog are maintained in pwx.h
***/


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
