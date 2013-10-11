#ifndef PWX_LIBPWX_PWX_TYPES_EARGERRORNUMBER_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_EARGERRORNUMBER_H_INCLUDED

/** @file eArgErrorNumber.h
  *
  * @brief enum describing all valid error numbers for argument parsing
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

#include <pwx/general/compiler.h>

namespace pwx {


/** @enum eArgErrorNumber
  * @brief defines valid error numbers for argument parsing
**/
enum eArgErrorNumber
{
	AEN_OK                  = 0,          //!< Everything in order
	AEN_ARGUMENT_UNKNOWN    = 0x00000001, //!< The found argument is not known
	AEN_PARAM_TYPE_MISMATCH = 0x00000002, //!< The type of the parameter doesn't match the target
	AEN_PARAMETER_MISSING   = 0x00000004, //!< An argument that needs a parameter got none to process
	AEN_PROCESSING_ERROR    = 0x00000008  //!< Set when target->process() threw an exception.
};


} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_EARGERRORNUMBER_H_INCLUDED

