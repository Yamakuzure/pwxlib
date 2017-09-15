#ifndef PWX_LIBPWX_PWX_TYPES_EARGTARGETTYPE_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_EARGTARGETTYPE_H_INCLUDED

/** @file eArgTargetType.h
  *
  * @brief Enum to describe what to do with a PAH target
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


namespace pwx {


/** @enum eArgTargetType
  * @brief determine what to do with a program argument handler target
  *
  * The value taken when adding an argument to PAH (or any other
  * instance of CArgHAndler) decides upon what to do with a
  * target when parsing command line arguments.
**/
enum eArgTargetType {
	ATT_FALSE = 0, //!< Set target to false (or (T)0 if not bool)
	ATT_TRUE  = 1, //!< Set target to true  (or (T)1 if not bool)
	ATT_ADD   = 2, //!< Add parameter to target
	ATT_INC   = 3, //!< Increase target by 1
	ATT_SUB   = 4, //!< Substract parameter from target
	ATT_DEC   = 5, //!< Decrease target by 1
	ATT_SET   = 6, //!< Set target to the value of the parameter
	ATT_CB    = 7  //!< The target is a callback function of the type void (*cb)(const char*, T*)
};


} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_EARGTARGETTYPE_H_INCLUDED

