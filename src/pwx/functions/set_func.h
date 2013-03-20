#pragma once
#ifndef PWX_PWXLIB_PWX_FUNCTIONS_SET_FUNC_H_INCLUDED
#define PWX_PWXLIB_PWX_FUNCTIONS_SET_FUNC_H_INCLUDED 1

/** @file set_func.h
  *
  * @brief Definitions of functions for pwx/container/TSet.h
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

#include <pwx/container/TSet.h> // Make IDE-Parsers happy

namespace pwx {

// --- function implementations ---

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
  * @param[in] lhs pointer to the left hand side set
  * @param[in] rhs pointer to the right hand side set
  * @return A new set being the difference of @a lhs and @a rhs
**/
template<typename data_t>
TSet<data_t>* set_difference(const TSet<data_t>* const lhs, const TSet<data_t>* const rhs)
{
	typedef TSet<data_t> list_t;

	list_t *newSet = nullptr;

	// Build an empty set and copy sorted switch and destroy method from lhs:
	PWX_TRY(newSet = new list_t())
	PWX_THROW_STD_FURTHER("SetCreationFailed", "set_difference() could not create the difference set!")
	newSet->reset(*lhs);

	// Actions is only needed if lhs is not the empty
	// set and lhs and rhs are not the same set
	if (lhs->size() && (lhs != rhs)) {
		{} /// FIXME: PWX_DOUBLE_LOCK(list_t, const_cast<list_t*>(lhs), list_t, const_cast<list_t*>(rhs))

		/* Two possibilities:
		 * 1.: rhs is the empty set. Then newSet is a simple copy of lhs
		 * 2.: rhs has elements. Then we have to traverse lhs and add
		 *     all elements that are no member of rhs
		*/
		if (rhs->empty())
			// This is possibility 1
			*newSet = *lhs;
		else {
			// This is possibility 2
			uint32_t lSize = lhs->size();
			for (uint32_t idx = 0; idx < lSize; ++idx) {
				auto* elem = lhs->get(idx);
				if (!rhs->hasMember(*elem)) {
					PWX_TRY_PWX_FURTHER(newSet->push(*elem))
				}
			} // End of traversing lhs
		} // End of building possibility 2
	} // End of having a combination requiring action

	return newSet;
}


/** @brief build the difference of two sets
  *
  * This is a wrapper of the pointer using version of set_difference()
  * to be able to use references.
  *
  * Please be aware that using references is rather costly, because the
  * resulting set has to be copied twice. You should use the pointer
  * versions wherever possible.
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
TSet<data_t> set_difference(const TSet<data_t> &lhs, const TSet<data_t> &rhs)
{
	TSet<data_t> *ptrSet = nullptr;
	PWX_TRY_PWX_FURTHER(ptrSet = set_difference(&lhs, &rhs))
	TSet<data_t> newSet(*ptrSet);
	delete ptrSet;
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
  * @param[in] lhs pointer to the left hand side set
  * @param[in] rhs pointer to the right hand side set
  * @return A new set being the intersection of @a lhs and @a rhs
**/
template<typename data_t>
TSet<data_t>* set_intersection(const TSet<data_t>* const lhs, const TSet<data_t>* const rhs)
{
	typedef TSet<data_t> list_t;

	list_t *newSet = nullptr;

	// Build an empty set and copy sorted switch and destroy method from lhs:
	PWX_TRY(newSet = new list_t())
	PWX_THROW_STD_FURTHER("SetCreationFailed", "set_difference() could not create the difference set!")
	newSet->reset(*lhs);

	// Action is only needed if neither is the empty set
	if (lhs->size() && rhs->size()) {
		{} /// FIXME: PWX_DOUBLE_LOCK(list_t, const_cast<list_t*>(lhs), list_t, const_cast<list_t*>(rhs))

		/* Two possibilities:
		 * 1.: rhs and lhs are equal, then we simply copy lhs
		 * 2.: Otherwise we have to traverse lhs and add
		 *     all elements that are a member of rhs.
		*/
		if (lhs == rhs)
			// This is possibility 1
			*newSet = *lhs;
		else {
			// This is possibility 2
			uint32_t lSize = lhs->size();
			for (uint32_t idx = 0; idx < lSize; ++idx) {
				auto* elem = lhs->get(idx);
				if (rhs->hasMember(*elem)) {
					PWX_TRY_PWX_FURTHER(newSet->push(*elem))
				}
			} // End of traversing lhs
		} // End of building possibility 2
	} // End of having a combination requiring action

	return newSet;
}


/** @brief build the intersection of two sets
  *
  * This is a wrapper of the pointer using version of set_intersection()
  * to be able to use references.
  *
  * Please be aware that using references is rather costly, because the
  * resulting set has to be copied twice. You should use the pointer
  * versions wherever possible.
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
TSet<data_t> set_intersection(const TSet<data_t> &lhs, const TSet<data_t> &rhs)
{
	TSet<data_t> *ptrSet = nullptr;
	PWX_TRY_PWX_FURTHER(ptrSet = set_intersection(&lhs, &rhs))
	TSet<data_t> newSet(*ptrSet);
	delete ptrSet;
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
  * @param[in] lhs pointer to the left hand side set
  * @param[in] rhs pointer to the right hand side set
  * @return A new set being the union of @a lhs and @a rhs
**/
template<typename data_t>
TSet<data_t>* set_union(const TSet<data_t>* const lhs, const TSet<data_t>* const rhs)
{
	typedef TSet<data_t> list_t;

	list_t *newSet = nullptr;

	// Build an empty set:
	PWX_TRY(newSet = new list_t())
	PWX_THROW_STD_FURTHER("SetCreationFailed", "set_difference() could not create the difference set!")

	// If lhs has elements, we can begin with adding them.
	if (lhs->size()) {
		{} /// FIXME: PWX_LOCK_GUARD(list_t, const_cast<list_t*>(lhs))
		newSet->reset(*lhs);
		PWX_TRY_PWX_FURTHER(*newSet += *lhs)
	}

	// If rhs has elements, we can add them (, too)
	if (rhs->size()) {
		{} /// FIXME: PWX_LOCK_GUARD(list_t, const_cast<list_t*>(rhs))
		if (newSet->empty())
			// We still need isSorted and destroy!
			newSet->reset(*rhs);
		PWX_TRY_PWX_FURTHER(*newSet += *rhs)
	}

	return newSet;
}


/** @brief build the union of two sets
  *
  * This is a wrapper of the pointer using version of set_union() to be
  * able to use references.
  *
  * Please be aware that using references is rather costly, because the
  * resulting set has to be copied twice. You should use the pointer
  * versions wherever possible.
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
  * @return A new set being the union of @a lhs and @a rhs
**/
template<typename data_t>
TSet<data_t> set_union(const TSet<data_t> &lhs, const TSet<data_t> &rhs)
{
	TSet<data_t> *ptrSet = nullptr;
	PWX_TRY_PWX_FURTHER(ptrSet = set_union(&lhs, &rhs))
	TSet<data_t> newSet(*ptrSet);
	delete ptrSet;
	return newSet;
}


// --- operator implementations ---

/** @brief return true if two sets are equal
  *
  * Two sets are equal if one set is a subset of the other
  * and both have the same number of elements.
  *
  * @param[in] lhs left hand side set
  * @param[in] rhs right hand side set
  * @return true if both sets are equal
**/
template<typename data_t>
bool operator==(const TSet<data_t> &lhs, const TSet<data_t> &rhs) noexcept
{
	bool result = true;

	if (&lhs != &rhs) {
		if (lhs.size() != rhs.size())
			result = false;
		else
			result = lhs.isSubsetOf(rhs);
	}

	return result;
}


/** @brief return true if two sets are different
  *
  * Two sets are different they differ in number of elements
  * or if one set consists of elements the other has not.
  *
  * @param[in] lhs left hand side set
  * @param[in] rhs right hand side set
  * @return true if both sets are different
**/
template<typename data_t>
bool operator!=(const TSet<data_t> &lhs, const TSet<data_t> &rhs) noexcept
{
	return !operator==(lhs, rhs);
}


/** @brief return the union of two sets by operation.
  *
  * This is a wrapper of the pointer using version of set_union() to be
  * able to use references with the + operator.
  *
  * Please be aware that using references is rather costly, because the
  * resulting set has to be copied thrice. You should use the pointer
  * versions of set_union() wherever possible.
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
  * @return A new set being the union of @a lhs and @a rhs
**/
template<typename data_t>
TSet<data_t> operator+ (const TSet<data_t> &lhs, const TSet<data_t> &rhs)
{
	PWX_TRY_PWX_FURTHER(return set_union(lhs, rhs))
}


/** @brief build the difference of two sets by operation.
  *
  * This is a wrapper of the pointer using version of set_difference()
  * to be able to use references with the - operator.
  *
  * Please be aware that using references is rather costly, because the
  * resulting set has to be copied thrice. You should use the pointer
  * versions of set_difference() wherever possible.
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
TSet<data_t> operator- (const TSet<data_t> &lhs, const TSet<data_t> &rhs)
{
	PWX_TRY_PWX_FURTHER(return set_difference(lhs, rhs))
}


} // namespace pwx
#endif // PWX_PWXLIB_PWX_FUNCTIONS_SET_FUNC_H_INCLUDED

