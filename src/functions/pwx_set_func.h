#ifndef PWX_PWXLIB_PWX_FUNCTIONS_SET_FUNC_H_INCLUDED
#define PWX_PWXLIB_PWX_FUNCTIONS_SET_FUNC_H_INCLUDED 1
#pragma once

/** @file pwx_set_func.h
  *
  * @brief Definitions of functions for pwx/container/TSet.h
  *
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  * History and change log are maintained in pwxlib.h
**/

#ifndef PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED
# error "Do not include pwx_set_fwd.h, include pwx/container/TSet.h!"
#endif // Check for main file

#include "basic/pwx_compiler.h"

#include "container/TSet.h" // Make IDE-Parsers happy

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
TSet<data_t>* set_difference( const TSet<data_t>* const lhs, const TSet<data_t>* const rhs ) {
	typedef TSet<data_t> list_t;

	list_t* newSet = nullptr;

	// Build an empty set and copy destroy method from lhs:
	PWX_TRY( newSet = new list_t() )
	PWX_THROW_STD_FURTHER( "SetCreationFailed", "set_difference(); could not create the difference set!" );
	newSet->reset( *lhs );

	// Actions is only needed if lhs is not the empty
	// set and lhs and rhs are not the same set
	if ( lhs->size() && ( lhs != rhs ) ) {
		PWX_DOUBLE_LOCK_GUARD( lhs, rhs );

		/* Two possibilities:
		 * 1.: rhs is the empty set. Then newSet is a simple copy of lhs
		 * 2.: rhs has elements. Then we have to traverse lhs and add
		 *     all elements that are no member of rhs
		*/
		if ( rhs->empty() )
			// This is possibility 1
			*newSet = *lhs;
		else {
			// This is possibility 2
			auto xHead  = lhs->get( 0 );
			auto xTail  = lhs->get( -1 );
			auto xCurr  = xHead;
			bool isDone = false;
			while ( xCurr && !isDone ) {
				if ( !rhs->hasMember( *xCurr ) )
					newSet->push( *xCurr );
				if ( xCurr == xTail )
					isDone = true;
				else
					xCurr = xCurr->getNext();
			} // end of traversing lhs
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
TSet<data_t> set_difference( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) {
	TSet<data_t>* ptrSet = nullptr;
	PWX_TRY_PWX_FURTHER( ptrSet = set_difference( &lhs, &rhs ) )
	TSet<data_t> newSet( *ptrSet );
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
TSet<data_t>* set_intersection( const TSet<data_t>* const lhs, const TSet<data_t>* const rhs ) {
	typedef TSet<data_t> list_t;

	list_t* newSet = nullptr;

	// Build an empty set and copy sorted switch and destroy method from lhs:
	PWX_TRY( newSet = new list_t() )
	PWX_THROW_STD_FURTHER( "SetCreationFailed", "set_intersection(); could not create the intersection set!" );
	newSet->reset( *lhs );

	// Action is only needed if neither is the empty set
	if ( lhs->size() && rhs->size() ) {
		PWX_DOUBLE_LOCK_GUARD( lhs, rhs );

		/* Two possibilities:
		 * 1.: rhs and lhs are equal, then we simply copy lhs
		 * 2.: Otherwise we have to traverse lhs and add
		 *     all elements that are a member of rhs.
		*/
		if ( lhs == rhs )
			// This is possibility 1
			*newSet = *lhs;
		else {
			// This is possibility 2
			auto xHead  = lhs->get( 0 );
			auto xTail  = lhs->get( -1 );
			auto xCurr  = xHead;
			bool isDone = false;
			while ( xCurr && !isDone ) {
				if ( rhs->hasMember( *xCurr ) )
					newSet->push( *xCurr );
				if ( xCurr == xTail )
					isDone = true;
				else
					xCurr = xCurr->getNext();
			} // end of traversing lhs
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
TSet<data_t> set_intersection( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) {
	TSet<data_t>* ptrSet = nullptr;
	PWX_TRY_PWX_FURTHER( ptrSet = set_intersection( &lhs, &rhs ) )
	TSet<data_t> newSet( *ptrSet );
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
TSet<data_t>* set_union( const TSet<data_t>* const lhs, const TSet<data_t>* const rhs ) {
	typedef TSet<data_t> list_t;

	list_t* newSet = nullptr;

	// Build an empty set:
	PWX_TRY( newSet = new list_t() )
	PWX_THROW_STD_FURTHER( "SetCreationFailed", "set_union(); could not create the union set!" );

	// If lhs has elements, we can begin with adding them.
	if ( lhs->size() ) {
		PWX_LOCK_GUARD( lhs );
		newSet->reset( *lhs );
		PWX_TRY_PWX_FURTHER( *newSet += *lhs );
	}

	// If rhs has elements, we can add them (, too)
	if ( rhs->size() ) {
		PWX_LOCK_GUARD( rhs );
		if ( newSet->empty() )
			// We still need isSorted and destroy!
			newSet->reset( *rhs );
		PWX_TRY_PWX_FURTHER( *newSet += *rhs );
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
TSet<data_t> set_union( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) {
	TSet<data_t>* ptrSet = nullptr;
	PWX_TRY_PWX_FURTHER( ptrSet = set_union( &lhs, &rhs ) )
	TSet<data_t> newSet( *ptrSet );
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
bool operator==( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) noexcept {
	bool result = true;

	if ( &lhs != &rhs ) {
		if ( lhs.size() != rhs.size() )
			result = false;
		else
			result = lhs.isSubsetOf( rhs );
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
bool operator!=( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) noexcept {
	return !operator==( lhs, rhs );
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
TSet<data_t> operator+ ( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) {
	PWX_TRY_PWX_FURTHER( return set_union( lhs, rhs ) )
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
TSet<data_t> operator- ( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) {
	PWX_TRY_PWX_FURTHER( return set_difference( lhs, rhs ) )
}


} // namespace pwx
#endif // PWX_PWXLIB_PWX_FUNCTIONS_SET_FUNC_H_INCLUDED

