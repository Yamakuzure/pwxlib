#ifndef PWX_LIBPWX_PWX_TYPES_SARGERROR_H_INCLDUED
#define PWX_LIBPWX_PWX_TYPES_SARGERROR_H_INCLDUED

/** @file sArgError.h
  *
  * @brief Declaration of struct sArgError for CArgHandler error handling
  *
  * (c) 2007 - 2017 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
  *
  * The PrydeWorX Library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public License as
  * published by the Free Software Foundation; either version 2.1 of the
  * License, or (at your option) any later version.
  *
  * The PrydeWorX Library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public License
  * along with pwxLib; If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
**/


#include "compiler.h"
#include "eArgErrorNumber.h"


namespace pwx {


/** @struct sArgError
  * @brief tiny struct to hold an error number and text together.
**/
struct sArgError
{
	int32_t     arg_errno; //!< number of the error, taken from eArgErrorNumber
	const char* arg_error; //!< String with the error text

	// Note: The prefix arg_ is needed, or the preprocessor will
	//       substitute "errno" with "(*__errno_location ())"

	explicit sArgError(eArgErrorNumber errno_, const char* error_) noexcept;
	~sArgError() noexcept;

	// No empty ctor, no copying
	sArgError() PWX_DELETE;
	sArgError(const sArgError&) PWX_DELETE;
	sArgError &operator=(const sArgError&) PWX_DELETE;

};

// Operators needed for pwx container storage:
bool operator==(const sArgError &lhs, const sArgError &rhs) noexcept;
bool operator>(const sArgError &lhs, const sArgError &rhs) noexcept;

} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_SARGERROR_H_INCLDUED

