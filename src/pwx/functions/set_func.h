#pragma once
#ifndef PWX_PWXLIB_PWX_FUNCTIONS_SET_FUNC_H_INCLUDED
#define PWX_PWXLIB_PWX_FUNCTIONS_SET_FUNC_H_INCLUDED 1

/** @file set_func.h
  *
  * @brief Definitions of functions for pwx/container/TSet.h
  *
  * (c) 2007 - 2012 PrydeWorX
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

#include <pwx/container/TSet.h> // Make IDE-Parsers happy

namespace pwx {

/** @brief build the difference of two sets
  *
  * This function builds a new set, created using the new operator,
  * so you have to delete it with the delete operator, that consists
  * of the difference of the sets @a lhs and @a rhs.
  *
  * If @a lhs is the empty set or @a lhs equals @a rhs an empty set is
  * returned. If @a rhs is the empty set a copy of @a lhs is returned.
  *
  * If the new set can not be created, a pwx::CException with
  * the name "SetCreationFailed" is thrown.
  *
  * If a new element can not be created, a pwx::CException with
  * the name "ElementCreationFailed" is thrown.
  *
  * @param[in] lhs reference of the left hand side set
  * @param[in] rhs reference of the right hand side set
  * @return A new set being the difference of @a lhs and @a rhs
**/
template<typename data_t>
TSet<data_t>* set_difference(const TSet<data_t> &lhs, const TSet<data_t> &rhs)
{
	typedef TSet<data_t> list_t;

	list_t *newSet = nullptr;

	// Build an empty set and copy sorted switch and destroy method from lhs:
	PWX_TRY(newSet = new list_t())
	PWX_THROW_STD_FURTHER("SetCreationFailed", "set_difference() could not create the difference set!")
	newSet->reset(lhs);

	// Actions is only needed if lhs is not the empty
	// set and lhs and rhs are not the same set
	if (lhs.size() && (&lhs != &rhs)) {
		PWX_DOUBLE_LOCK(list_t, const_cast<list_t*>(&lhs), list_t, const_cast<list_t*>(&rhs))

		/* Two possibilities:
		 * 1.: rhs is the empty set. Then newSet is a simple copy of lhs
		 * 2.: rhs has elements. Then we have to traverse lhs and add
		 *     all elements that are no member of rhs
		*/
		if (rhs.empty())
			// This is possibility 1
			*newSet = lhs;
		else {
			// This is possibility 2
			uint32_t lSize = lhs.size();
			for (uint32_t idx = 0; idx < lSize; ++idx) {
				if (!rhs.hasMember(*lhs[idx])) {
					PWX_TRY_PWX_FURTHER(newSet->push(*lhs[idx]))
				}
			} // End of traversing lhs
		} // End of building possibility 2
	} // End of having a combination requiring action

	return newSet;
}


/** @brief build the intersection of two sets
  *
  * This function builds a new set, created using the new operator,
  * so you have to delete it with the delete operator, that consists
  * of the intersection of the sets @a lhs and @a rhs.
  *
  * If either set is the empty set, the intersection is the empty set.
  *
  * If @a lhs equals @a rhs, the intersection is a copy of @a lhs.
  *
  * If the new set can not be created, a pwx::CException with
  * the name "SetCreationFailed" is thrown.
  *
  * If a new element can not be created, a pwx::CException with
  * the name "ElementCreationFailed" is thrown.
  *
  * @param[in] lhs reference of the left hand side set
  * @param[in] rhs reference of the right hand side set
  * @return A new set being the intersection of @a lhs and @a rhs
**/
template<typename data_t>
TSet<data_t>* set_intersection(const TSet<data_t> &lhs, const TSet<data_t> &rhs)
{
	typedef TSet<data_t> list_t;

	list_t *newSet = nullptr;

	// Build an empty set and copy sorted switch and destroy method from lhs:
	PWX_TRY(newSet = new list_t())
	PWX_THROW_STD_FURTHER("SetCreationFailed", "set_difference() could not create the difference set!")
	newSet->reset(lhs);

	// Actions is only needed if neither is the empty
	if (lhs.size() && rhs.size()) {
		PWX_DOUBLE_LOCK(list_t, const_cast<list_t*>(&lhs), list_t, const_cast<list_t*>(&rhs))

		/* Two possibilities:
		 * 1.: rhs and lhs are equal, then we simply copy lhs
		 * 2.: Otherwise we have to traverse lhs and add
		 *     all elements that are a member of rhs, too.
		*/
		if (&lhs == &rhs)
			// This is possibility 1
			*newSet = lhs;
		else {
			// This is possibility 2
			uint32_t lSize = lhs.size();
			for (uint32_t idx = 0; idx < lSize; ++idx) {
				if (rhs.hasMember(*lhs[idx])) {
					PWX_TRY_PWX_FURTHER(newSet->push(*lhs[idx]))
				}
			} // End of traversing lhs
		} // End of building possibility 2
	} // End of having a combination requiring action

	return newSet;
}


/** @brief build the union of two sets
  *
  * This function builds a new set, created using the new operator,
  * so you have to delete it with the delete operator, that consists
  * of the union of the sets @a lhs and @a rhs.
  *
  * If either set is the empty set, the union is a copy of the other.
  *
  * If the new set can not be created, a pwx::CException with
  * the name "SetCreationFailed" is thrown.
  *
  * If a new element can not be created, a pwx::CException with
  * the name "ElementCreationFailed" is thrown.
  *
  * @param[in] lhs reference of the left hand side set
  * @param[in] rhs reference of the right hand side set
  * @return A new set being the intersection of @a lhs and @a rhs
**/
template<typename data_t>
TSet<data_t>* set_union(const TSet<data_t> &lhs, const TSet<data_t> &rhs)
{
	typedef TSet<data_t> list_t;

	list_t *newSet = nullptr;

	// Build an empty set:
	PWX_TRY(newSet = new list_t())
	PWX_THROW_STD_FURTHER("SetCreationFailed", "set_difference() could not create the difference set!")

	// If lhs has elements, we can begin with addingits elements
	if (lhs.size()) {
		PWX_LOCK_GUARD(list_t, const_cast<list_t*>(&lhs))
		newSet->reset(lhs);
		PWX_TRY_PWX_FURTHER(*newSet += lhs)
	}

	// If rhs has elements, we can add them (, too)
	if (rhs.size()) {
		PWX_LOCK_GUARD(list_t, const_cast<list_t*>(&rhs))
		if (newSet->empty())
			// We still need isSorted and destroy!
			newSet->reset(rhs);
		PWX_TRY_PWX_FURTHER(*newSet += rhs)
	}

	return newSet;
}


} // namespace pwx
#endif // PWX_PWXLIB_PWX_FUNCTIONS_SET_FUNC_H_INCLUDED

