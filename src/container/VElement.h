#pragma once
#ifndef PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED
#define PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED 1

/** @file VElement.h
  *
  * @brief Virtual base class for all container elements.
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


#include "macros.h"
#include "CLockable.h"
#include "TVarDeleter.h"
#include "CLockGuard.h"

namespace pwx
{

// Forward CThreadElementStore, there is a circular dependency.
namespace private_ {
class CThreadElementStore;
}


/** @class VElement
  *
  * @brief Virtual base class for all container elements.
  *
  * This class is strictly virtual. ALl element templates have
  * to inherit public from this base class.
  *
  * Notes: <UL>
  * <LI>eNr does not need a lock, it is atomic.</LI>
  * <LI>eNr needs no const_cast, it is mutable.</LI>
  * </UL>
**/
class PWX_API VElement : public CLockable
{
public:

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef CLockable                     base_t;
	typedef VElement                      elem_t;
	typedef private_::CThreadElementStore store_t;


	/* ===============================================
	 * === Public constructors and destructors     ===
	 * ===============================================
	*/

	explicit VElement() noexcept;
	VElement (const VElement &src) noexcept;
	virtual ~VElement() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	virtual void     disable_thread_safety()    noexcept;                //!< turn off locking
	virtual void     enable_thread_safety()     noexcept;                //!< turn on locking
	virtual void     insert(store_t* new_store) noexcept;                //!< mark as inserted
	        bool     inserted()           const noexcept PWX_WARNUNUSED; //!< return true if marked as inserted
	        uint32_t nr()                 const noexcept PWX_WARNUNUSED; //!< return current number
	virtual void     remove()                   noexcept;                //!< mark as removed
	        bool     removed()            const noexcept PWX_WARNUNUSED; //!< return true if marked as removed


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	virtual VElement &operator=(const VElement &src) noexcept;


	/* ===============================================
	 * === Public members                          ===
	 * ===============================================
	*/

	mutable
	aui32_t eNr = ATOMIC_VAR_INIT(0); //!< Number of the element


protected:

	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	 */

	using base_t::isDestroyed;
	using base_t::memOrdLoad;
	using base_t::memOrdStore;


private:

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	 */

	store_t* currStore = nullptr;                //!< If set by a container the element will invalidate itself upon removal.
	mutable
	abool_t  isRemoved = ATOMIC_VAR_INIT(true);  //!< Set to true by ctor and remove*(), set to false by insert*() methods.

}; // class VContainer


} // namespace pwx


#endif // PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED
