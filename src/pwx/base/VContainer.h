#pragma once
#ifndef PWX_LIBPWX_BASE_VCONTAINER_H_INCLUDED
#define PWX_LIBPWX_BASE_VCONTAINER_H_INCLUDED 1

/** @file VContainer.h
  *
  * @brief Virtual base class for all containers.
  *
  * (c) 2007 - 2013 PrydeWorX
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

#include <pwx/general/macros.h>
#include <pwx/types/CLockable.h>

namespace pwx
{

/** @class VContainer
  *
  * @brief Virtual base class for all containers.
  *
  * This class is strictly virtual. ALl container templates have
  * to inherit public from this base class.
  *
  * Notes: <UL>
  * <LI>Both doRenumber and eCount are atomic and need no lock</LI>
  * <LI>Both doRenumber and eCount are mutable and need no const_cast</LI>
  * </UL>
**/
class VContainer : public CLockable
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef CLockable                 base_t;
	typedef VContainer                list_t;
	typedef std::atomic_bool          abool_t;
	typedef std::atomic_uint_fast32_t aui32_t;


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	explicit VContainer() noexcept;
	VContainer (const VContainer &src) noexcept;
	virtual ~VContainer() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	virtual void     clear()                 noexcept PWX_VIRTUAL_PURE;
	virtual void     disable_thread_safety() noexcept PWX_VIRTUAL_PURE;
	virtual bool     empty()           const noexcept PWX_VIRTUAL_PURE;
	virtual void     enable_thread_safety()  noexcept PWX_VIRTUAL_PURE;
	virtual uint32_t size()            const noexcept PWX_VIRTUAL_PURE;


protected:

	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	mutable
	abool_t doRenumber = ATOMIC_VAR_INIT(false); //!< If set to true, a renumbering is done before retrieving elements by index
	mutable
	aui32_t eCount     = ATOMIC_VAR_INIT(0);     //!< Current number of elements

	using base_t::memOrdLoad;
	using base_t::memOrdStore;
}; // class VContainer

#if defined(PWX_EXPORTS)
/// @brief ~VContainer default destructor.
VContainer::~VContainer() noexcept
{ }
#endif

} // namespace pwx
#endif // PWX_LIBPWX_BASE_VCONTAINER_H_INCLUDED

