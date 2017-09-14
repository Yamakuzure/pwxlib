#pragma once
#ifndef PWX_PWXLIB_PWX_FUNCTIONS_SET_FWD_H_INCLUDED
#define PWX_PWXLIB_PWX_FUNCTIONS_SET_FWD_H_INCLUDED 1

/** @file set_fwd.h
  *
  * @brief Forward declarations and function prototypes for pwx/container/TSet.h
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

#ifndef PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED
# error "Do not include set_fwd.h, include pwx/container/TSet.h!"
#endif // Check for main file

#include "TSet.h" // Make IDE-Parsers happy

namespace pwx {

// Forward TSet for prototype definitions
template<typename data_t> class TSet;

// --- Function Prototypes ---
template<typename data_t>
TSet<data_t> PWX_API *set_difference  (const TSet<data_t>* const lhs, const TSet<data_t>* const rhs);
template<typename data_t>
TSet<data_t> PWX_API  set_difference  (const TSet<data_t> &lhs, const TSet<data_t> &rhs);
template<typename data_t>
TSet<data_t> PWX_API *set_intersection(const TSet<data_t>* const lhs, const TSet<data_t>* const rhs);
template<typename data_t>
TSet<data_t> PWX_API  set_intersection(const TSet<data_t> &lhs, const TSet<data_t> &rhs);
template<typename data_t>
TSet<data_t> PWX_API *set_union       (const TSet<data_t>* const lhs, const TSet<data_t>* const rhs);
template<typename data_t>
TSet<data_t> PWX_API  set_union       (const TSet<data_t> &lhs, const TSet<data_t> &rhs);

// --- operator prototypes ---
template<typename data_t>
bool         PWX_API  operator==(const TSet<data_t> &lhs, const TSet<data_t> &rhs) noexcept;
template<typename data_t>
bool         PWX_API  operator!=(const TSet<data_t> &lhs, const TSet<data_t> &rhs) noexcept;
template<typename data_t>
TSet<data_t> PWX_API  operator+ (const TSet<data_t> &lhs, const TSet<data_t> &rhs);
template<typename data_t>
TSet<data_t> PWX_API  operator- (const TSet<data_t> &lhs, const TSet<data_t> &rhs);

} // namespace pwx
#endif // PWX_PWXLIB_PWX_FUNCTIONS_SET_FWD_H_INCLUDED

