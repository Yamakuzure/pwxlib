#pragma once
#ifndef PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED
#define PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED 1

/** @file VElement.h
  *
  * @brief Virtual base class for all container elements.
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

#include <memory>
#include "pwx/types/CLockable.h"
#include "pwx/types/TVarDeleter.h"
#include "pwx/general/macros.h"

namespace pwx
{

/** @class VElement
  *
  * @brief Virtual base class for all container elements.
  *
  * This class is strictly virtual. ALl element templates have
  * to inherit public from this base class.
**/
class VElement : public CLockable
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef CLockable  base_t;
	typedef VElement   elem_t;


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	explicit VElement() noexcept;
	VElement (const VElement &src) noexcept;
	virtual ~VElement() noexcept;


	/* ===============================================
	 * === Public members                          ===
	 * ===============================================
	*/

	mutable uint32_t eNr = 0; //!< Number of the element
}; // class VContainer

#if defined(PWX_EXPORTS)
/// @brief ~VElement default destructor.
VElement::~VElement() noexcept
{ }
#endif

} // namespace pwx
#endif // PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED

