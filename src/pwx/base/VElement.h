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

#include <pwx/general/macros.h>
#include <pwx/types/CLockable.h>
#include <pwx/types/TVarDeleter.h>
#include <atomic>
#include <memory>

namespace pwx
{

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

	/** @brief returns true if the data was destroyed
	  *
	  * The destructor of TSingleElement and TDoubleElement
	  * will try to get a final lock on the element when it
	  * is destroyed. If another thread acquires a lock
	  * between the data destruction and this final dtor lock,
	  * destroyed() will return "true".
	  *
	  * @return true if the element is within its destruction process.
	**/
	bool destroyed() const noexcept
	{
		return isDestroyed.load(PWX_MEMORDER_ACQUIRE);
	}


	/** @brief disable thread safety
	  *
	  * This method disables all thread safety measures.
	  *
	  * <B>Warning</B>: It is completely unchecked whether the
	  * element is used by more than one thread. If concurrent
	  * threads work with this element while this method is
	  * called, the outcome is unpredictable.
	  */
	virtual void disable_thread_safety() noexcept
	{
		this->do_locking(false);
		this->beThreadSafe.store(false, PWX_MEMORDER_RELEASE);
	}


	/** @brief enable thread safety
	  *
	  * This method enables all thread safety measures.
	  */
	virtual void enable_thread_safety() noexcept
	{
		this->do_locking(true);
		this->beThreadSafe.store(true, PWX_MEMORDER_RELEASE);
	}


	/** @brief return true if the element is a member of a container
	  *
	  * For this to work derived elements and containers using these
	  * have to maintain isRemoved accordingly.
	  *
	  * @return true if the element is a member of any container
	**/
	bool inserted() const noexcept
	{
		return !isRemoved.load(PWX_MEMORDER_ACQUIRE);
	}


	/** @brief return the current number of the element in a thread safe way
	  * @return the current number of the element
	**/
	uint32_t nr()
	{
		return beThreadSafe.load(PWX_MEMORDER_RELAXED)
				? eNr.load(PWX_MEMORDER_ACQUIRE)
				: eNr.load(PWX_MEMORDER_RELAXED);
	}


	/* ===============================================
	 * === Public members                          ===
	 * ===============================================
	*/

	mutable
	std::atomic_uint_fast32_t
	eNr = ATOMIC_VAR_INIT(0); //!< Number of the element


protected:

	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	 */

	mutable
	std::atomic_bool
	beThreadSafe = ATOMIC_VAR_INIT(true);  //!< Use next/prev pointers directly if set to false.
	mutable
	std::atomic_bool
	isDestroyed  = ATOMIC_VAR_INIT(false); //!< Should be set to true by the destructors of deriving classes.
	mutable
	std::atomic_bool
	isRemoved    = ATOMIC_VAR_INIT(true);  //!< Set to true by ctor and remove*(), set to false by insert*() methods.

}; // class VContainer

#if defined(PWX_EXPORTS)
/// @brief ~VElement default destructor.
VElement::~VElement() noexcept
{ }
#endif

} // namespace pwx
#endif // PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED

