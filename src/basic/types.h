#ifndef PWX_LIBPWX_PWX_GENERAL_TYPES_H_INCLUDED
#define PWX_LIBPWX_PWX_GENERAL_TYPES_H_INCLUDED

/** @file types.h
  *
  * @brief Includes and typedefs for types used throughout libpwx
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

#include <cstdint>
#include <atomic>
#include <memory>

namespace pwx {

typedef std::atomic_bool          abool_t;
typedef std::atomic_flag          aflag_t;
typedef std::atomic_uint_fast32_t aui32_t;
typedef std::atomic_size_t        asize_t;
typedef std::memory_order         mord_t;

} // namespace pwx
#endif // PWX_LIBPWX_PWX_GENERAL_TYPES_H_INCLUDED

