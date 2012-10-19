#pragma once
#ifndef PWX_LIBPWX_PWX_GENERAL_TEMPLATES_H_INCLUDED
#define PWX_LIBPWX_PWX_GENERAL_TEMPLATES_H_INCLUDED 1

/** @file templates.h
  *
  * @brief Helper templates and macros for various little issues
  *
  * (c) 2007 - 2012 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         yamakuzure@users.sourceforge.net
  *         http://pwxlib.sourceforge.net
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

#include <type_traits>

namespace pwx
{

/** @brief compile time assertion
  *
  * This little trick is taken from Andrei Alexandrescu: 'Modern C++ Design'.
  * It can be used to check an expression at compile time.
  * Of course this does not substitute cassert completely, only expressions
  * that are known while the compiler does its work can be checked with this.
**/
template <bool> struct PWX_API pwxCompAssert;
template <>     struct PWX_API pwxCompAssert<true> {};
#define compAssert(expression) (::pwx::pwxCompAssert<(expression) != 0>())

} // namespace pwx
#endif // PWX_LIBPWX_PWX_GENERAL_TEMPLATES_H_INCLUDED
