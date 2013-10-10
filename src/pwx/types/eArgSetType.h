#ifndef PWX_LIBPWX_PWX_TYPES_EARGSETTYPE_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_EARGSETTYPE_H_INCLUDED

/** @file eArgSetType.h
  *
  * @brief enum to describe what PAH should do when an ATT_SET target receives more than one parameter
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


/** @enum eArgSetType
  * @brief determine what to do when multiple parameters are submitted to an ATT_SET target.
  *
  * This enum only applies to ATT_SET targets to be able to change the
  * default behaviour of the ATT_SET target, which is to overwrite the
  * set value on each call to process().
**/
enum eArgSetType {
	STT_ERROR     = 0, //!< Receiving more than one parameter is an error.
	STT_OVERWRITE = 1, //!< Overwrite the set value (default)
	STT_IGNORE    = 2  //!< Only set the value once, silently ignore all further parameters.
};


#endif // PWX_LIBPWX_PWX_TYPES_EARGSETTYPE_H_INCLUDED

