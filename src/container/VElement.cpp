/***
  This file is part of the PrydeWorX Library (pwxLib).

  Copyright 2007 - 2017 Sven Eden

  The PrydeWorX Library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  The PrydeWorX Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with pwxLib; If not, see <http://www.gnu.org/licenses/>.

  History and Changelog are maintained in pwx.h
***/


#include "CThreadElementStore.h"
#include "VElement.h"


namespace pwx {


/// @brief VElement default constructor.
VElement::VElement() noexcept
{ }

/// @brief VElement copy constructor.
VElement::VElement(const VElement& src) noexcept
 :	base_t(src)
{ }


/// @brief ~VElement default destructor.
VElement::~VElement() noexcept
{
	this->remove();
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
void VElement::disable_thread_safety() noexcept
{
	this->do_locking(false);
}


/** @brief enable thread safety
  *
  * This method enables all thread safety measures.
  */
void VElement::enable_thread_safety() noexcept
{
	this->do_locking(true);
}


/** @brief mark this element as being inserted
  *
  * This method should be called by all deriving element classes
  * upon insertion to mark this element as being inserted.
  *
  * Additionally this method will store the given pointer to
  * the handling CThreadElementStore and invalidate itself there
  * upon removal.
  *
  * @param[in] new_store pointer to the CThreadElementStore that might handle this element from now on.
**/
void VElement::insert(store_t* new_store) noexcept
{
	isRemoved.store(false, memOrdStore);
	if (currStore)
		currStore->invalidate(this);
	currStore = new_store;
}


/** @brief return true if the element is a member of a container
  *
  * For this to work derived elements and containers using these
  * have to use insert()/remove() accordingly.
  *
  * @return true if the element is a member of any container
**/
bool VElement::inserted() const noexcept
{
	return !isRemoved.load(memOrdLoad);
}


/** @brief return the current number of the element in a thread safe way
  * @return the current number of the element
**/
uint32_t VElement::nr() const noexcept
{
	return eNr.load(memOrdLoad);
}


/** @brief Mark this element as being removed
  *
  * This method should be called by all deriving element classes
  * upon removal to mark this element as being removed.
  *
  * Additionally this method will call invalidate(this) on that
  * a set CThreadElementStore store if it has been set upon insertion.
**/
void VElement::remove() noexcept
{
	isRemoved.store(true, memOrdStore);
	if (currStore)
		currStore->invalidate(this);
}


/** @brief return true if the element is not a member of a container
  *
  * For this to work derived elements and containers using these
  * have to use insert()/remove() accordingly.
  *
  * @return true if the element is not a member of any container
**/
bool VElement::removed() const noexcept
{
	return isRemoved.load(memOrdLoad);
}


/// Copy nothing but CLockable values, number and CThreadElementStore must be set by containers.
VElement &VElement::operator=(const VElement &src) noexcept
{
	beThreadSafe(src.beThreadSafe());
	return *this;
}


} // namespace pwx
