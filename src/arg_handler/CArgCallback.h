#ifndef PWX_LIBPWX_PWX_TYPES_CARGCALLBACK_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_CARGCALLBACK_H_INCLUDED

/** @file CArgCallback.h
  *
  * @brief Declaration of the CArgCallback class
  *
  * (c) 2007 - 2013 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
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

#include "VArgTargetBase.h"
namespace pwx {


/** @struct CArgCallback
  * @brief Definition of one command line argument using a callback function
  *
  * This is meant to be used with a callback function. If a target pointer
  * is to be used, use TArgTarget instead.
**/
class CArgCallback : public VArgTargetBase
{
public:
	explicit CArgCallback(const char* arg_short, const char* arg_long,
				void (*arg_cb)(const char*, const char*),
				const char* arg_desc, const char* param_name) noexcept;

	virtual ~CArgCallback() noexcept;

	virtual eArgErrorNumber process(const char* param);

private:

	// callback
	void (*cb)(const char*, const char*);

};


} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_CARGCALLBACK_H_INCLUDED

