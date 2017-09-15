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


#include "macros.h"
#include "CArgCallback.h"
#include "CException.h"


namespace pwx {


/** brief default ctor
  *
  * No parameter check, the caller must ensure consistent
  * values that make the instance usable.
  *
  * @see pwx::CArgHandler::addArg()
  *
  * @param[in] arg_short Short argument like "-a" or "x".
  * @param[in] arg_long Long argument like "--foo" or "-bar".
  * @param[out] arg_cb Pointer to the callback function to use.
  * @param[in] arg_desc Help text for this argument.
  * @param[in] param_name Name shown in <> int the help text.
**/
CArgCallback::CArgCallback(const char* arg_short, const char* arg_long,
			void (*arg_cb)(const char*, const char*),
			const char* arg_desc, const char* param_name)
		noexcept :
	VArgTargetBase(arg_short, arg_long, ATT_CB, arg_desc, param_name),
	cb    (arg_cb)
{
	/* nothing to do here */
}


/** @brief destructor
  * has nothing to do.
**/
CArgCallback::~CArgCallback() noexcept
{
	/* nothing to do here */
}


/** @brief process an argument parameter
  *
  * Simple method that calls the stored callback function with @a param.
  *
  * If no callback function was installed using the constructor,
  * this method does silently nothing.
  *
  * @return AEN_OK if the callback function was set.
**/
eArgErrorNumber CArgCallback::process(const char *param)
{
	if (cb) {
		try {
			cb(aLong.size() ? aLong.c_str() : aShort.c_str(), param);
			this->gotParameter = true;
		}
		PWX_THROW_STD_FURTHER("ArgCbException","")
		catch (...) {
			PWX_THROW("ArgCbException", "Unknown exception", "")
		}
	}

	return AEN_OK;
}

} // namespace pwx
