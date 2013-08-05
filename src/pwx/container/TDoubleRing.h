#pragma once
#ifndef PWX_PWXLIB_PWX_CONTAINER_TDOUBLERING_H_INCLUDED
#define PWX_PWXLIB_PWX_CONTAINER_TDOUBLERING_H_INCLUDED 1

/** @file TDoubleRing.h
  *
  * @brief Declaration of a basic template for doubly linked rings
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
  *
  * History and Changelog are maintained in pwx.h
**/

#include <pwx/container/TDoubleList.h>

namespace pwx
{

/** @class TDoubleRing
  *
  * @brief Template to build doubly linked rings of variable types
  *
  * The doubly linked ring is a doubly linked list with a tail having a next
  * pointer to head which has a prev pointer to tail.
  *
  * The constructor takes an optional destroy(T*) function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * @see pwx::TDoubleList for further information.
**/
template<typename data_t, typename elem_t = TDoubleElement<data_t> >
class PWX_API TDoubleRing : public TDoubleList<data_t, elem_t>
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef TDoubleList<data_t, elem_t> base_t; //!< Base type of the ring
	typedef TDoubleRing<data_t, elem_t> list_t; //!< Type of this ring


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor initializes an empty ring.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TDoubleRing (void (*destroy_) (data_t* data)) noexcept :
		base_t (destroy_)
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor uses the base constructor to set the data
	  * destroy method to the null pointer.
	**/
	TDoubleRing() noexcept :
		base_t (nullptr)
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * @param[in] src reference of the ring to copy.
	**/
	TDoubleRing (const list_t &src) noexcept :
		base_t (src)
	{
		// TDoubleList copies the elements
		privConnectEnds(); // All we have to do here!
	}


	virtual ~TDoubleRing() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	using base_t::clear;


	/** @brief delete the element holding the specified data
	  *
	  * This method deletes the element in the ring that holds @a data.
	  *
	  * If you intent to work with the element, use remData instead.
	  *
	  * @param[in] data the data the element that is to be deleted holds
	  * @return the number of elements remaining in the ring after the deletion.
	**/
	virtual uint32_t delData (data_t* data)
	{
		PWX_TRY_PWX_FURTHER (base_t::delData (data))
		return privConnectEnds();
	}


	/** @brief delete the specified element
	  *
	  * This method deletes the element @a elem.
	  *
	  * If you intent to work with the element, use remElem instead.
	  *
	  * If @a elem is no element of this ring, the wrong ring is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct ring!
	  *
	  * @param[in] elem the element that is to be deleted
	  * @return the number of elements remaining in the ring after the deletion.
	**/
	virtual uint32_t delElem (elem_t* elem)
	{
		PWX_TRY_PWX_FURTHER (base_t::delElem (elem))
		return privConnectEnds();
	}


	/** @brief delete the element after the element holding the specified data
	  *
	  * This method deletes the element in the list after the element
	  * that holds @a prev.
	  *
	  * If @a prev is set to nullptr, the root element (aka head) is
	  * deleted.
	  *
	  * If you intent to work with the element, use remNext instead.
	  *
	  * If there is no element behind the element holding @a prev a
	  * pwx::CException with the name "OutOfRange" is thrown.
	  *
	  * @param[in] prev the data the element that precedes the element to delete holds
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delNext (data_t* prev)
	{
		PWX_TRY_PWX_FURTHER (base_t::delNext (prev))
		return privConnectEnds();
	}


	/** @brief delete the element after the specified element
	  *
	  * This method deletes the element in the list after the element
	  * @a prev.
	  *
	  * If @a prev is set to nullptr, the root element (aka head) is
	  * deleted.
	  *
	  * If you intent to work with the element, use remNextElem instead.
	  *
	  * If @a prev is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If there is no element behind the element @a prev a
	  * pwx::CException with the name "OutOfRange" is thrown.
	  *
	  * @param[in] prev the element that precedes the element to delete
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delNextElem (elem_t* prev)
	{
		PWX_TRY_PWX_FURTHER (base_t::delNextElem (prev))
		return privConnectEnds();
	}


	/** @brief delete the element before the element holding the specified data
	  *
	  * This method deletes the element in the ring before the element
	  * that holds @a next.
	  *
	  * If @a next is set to nullptr, the last element (aka tail) is
	  * deleted.
	  *
	  * If you intent to work with the element, use remPrev instead.
	  *
	  * If there is no element before the element holding @a next, a
	  * pwx::CException with the name "OutOfRange" is thrown.
	  *
	  * @param[in] next the data the element that succeeds the element to delete holds
	  * @return the number of elements remaining in the ring after the deletion.
	**/
	virtual uint32_t delPrev (data_t* next)
	{
		PWX_TRY_PWX_FURTHER (base_t::delPrev (next))
		return privConnectEnds();
	}


	/** @brief delete the element before the specified element
	  *
	  * This method deletes the element in the ring before the element
	  * @a next.
	  *
	  * If @a next is set to nullptr, the last element (aka tail) is
	  * deleted.
	  *
	  * If you intent to work with the element, use remPrevElem instead.
	  *
	  * If @a next is no element of this ring, the wrong ring is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct ring!
	  *
	  * If there is no element before the element @a next a
	  * pwx::CException with the name "OutOfRange" is thrown.
	  *
	  * @param[in] next the element that succeeds the element to delete
	  * @return the number of elements remaining in the ring after the deletion.
	**/
	virtual uint32_t delPrevElem (elem_t* next)
	{
		PWX_TRY_PWX_FURTHER (base_t::delPrevElem (next))
		return privConnectEnds();
	}


	using base_t::empty;
	using base_t::find;
	using base_t::get;
	using base_t::getData;


	/** @brief insert a new data pointer after the specified data
	  *
	  * This method inserts a new element in the list after the element
	  * holding @a prev.
	  *
	  * If @a prev is set to nullptr, the new element will become the new
	  * head of the list.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] prev the data the element that should precede the new element holds
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t insNext (data_t* prev, data_t* data)
	{
		PWX_TRY_PWX_FURTHER (base_t::insNext (prev, data))
		return privConnectEnds();
	}


	/** @brief insert an element copy after the specified data
	  *
	  * This method inserts a new element in the list after the element
	  * holding @a prev that is a copy of @a src.
	  *
	  * If @a prev is set to nullptr, the new element will become the new
	  * head of the list.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] prev the data the element that should precede the new element holds.
	  * @param[in] src reference to the element to copy.
	  * @return the number of elements in this list after the insertion.
	**/
	virtual uint32_t insNext (data_t* prev, const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (base_t::insNext (prev, src))
		return privConnectEnds();
	}


	/** @brief insert a new data pointer after the specified element
	  *
	  * This method inserts a new element in the list after the element
	  * @a prev.
	  *
	  * If @a prev is set to nullptr, the new element will become the new
	  * head of the list.
	  *
	  * If @a prev is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] prev the element that should precede the new element
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t insNextElem (elem_t* prev, data_t* data)
	{
		PWX_TRY_PWX_FURTHER (base_t::insNextElem (prev, data))
		return privConnectEnds();
	}


	/** @brief insert an element copy after the specified element
	  *
	  * This method inserts a new element in the list after the element
	  * @a prev that is a copy of @a src.
	  *
	  * If @a prev is set to nullptr, the new element will become the new
	  * head of the list.
	  *
	  * If @a prev is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] prev the element that should precede the new element.
	  * @param[in] src reference to the element to copy.
	  * @return the number of elements in this list after the insertion.
	**/
	virtual uint32_t insNextElem (elem_t* prev, const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (base_t::insNextElem (prev, src))
		return privConnectEnds();
	}


	/** @brief insert a new data pointer before the specified data
	  *
	  * This method inserts a new element in the list before the element
	  * holding @a next.
	  *
	  * If @a next is set to nullptr, the new element will become the new
	  * tail of the list.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] next the data the element that should succeed the new element
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	uint32_t insPrev (data_t* next, data_t* data)
	{
		PWX_TRY_PWX_FURTHER (base_t::insPrev (next, data))
		return privConnectEnds();
	}


	/** @brief insert an element copy before the specified data
	  *
	  * This method inserts a new element in the list before the element
	  * holding @a next as a copy of @a src.
	  *
	  * If @a next is set to nullptr, the new element will become the new
	  * tail of the list.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] next the data the element that should succeed the new element.
	  * @param[in] src reference to the element to copy.
	  * @return the number of elements in this list after the insertion.
	**/
	uint32_t insPrev (data_t* next, const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (base_t::insPrev (next, src))
		return privConnectEnds();
	}


	/** @brief insert a new data pointer before the specified element
	  *
	  * This method inserts a new element in the list before the element
	  * @a next.
	  *
	  * If @a next is set to nullptr, the new element will become the new
	  * tail of the list.
	  *
	  * If @a next is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] next the element that should succeed the new element
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	uint32_t insPrevElem (elem_t* next, data_t* data)
	{
		PWX_TRY_PWX_FURTHER (base_t::insPrevElem (next, data))
		return privConnectEnds();
	}


	/** @brief insert an element copy before the specified element
	  *
	  * This method inserts a new element in the list before the element
	  * @a next as a copy of @a src.
	  *
	  * If @a next is set to nullptr, the new element will become the new
	  * tail of the list.
	  *
	  * If @a next is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] next the element that should succeed the new element.
	  * @param[in] src reference to the element to copy.
	  * @return the number of elements in this list after the insertion.
	**/
	uint32_t insPrevElem (elem_t* next, const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (base_t::insPrevElem (next, src))
		return privConnectEnds();
	}


	/** @brief short alias for pop_front()
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delNext instead.
	  *
	  * If the ring is empty, nullptr is returned.
	  *
	  * @return a pointer to the removed element or nullptr if the ring is empty
	**/
	virtual elem_t* pop() noexcept
	{
		elem_t* toRemove = base_t::pop_front();
		if (toRemove)
			privConnectEnds();
		return toRemove;
	}


	/** @brief alias to remove the last element (tail)
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delNext instead.
	  *
	  * If the ring is empty, nullptr is returned.
	  *
	  * @return a pointer to the removed element or nullptr if the ring is empty
	**/
	virtual elem_t* pop_back() noexcept
	{
		elem_t*  toRemove   = base_t::pop_back();
		if (toRemove)
			privConnectEnds();
		return toRemove;
	}


	/** @brief alias to remove the first element (head)
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delNext instead.
	  *
	  * If the ring is empty, nullptr is returned.
	  *
	  * @return a pointer to the removed element or nullptr if the ring is empty
	**/
	virtual elem_t* pop_front() noexcept
	{
		elem_t* toRemove = base_t::pop_front();
		if (toRemove)
			privConnectEnds();
		return toRemove;
	}


	/** @brief short alias for push_back(data_t *data)
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this ring after the insertion
	**/
	virtual uint32_t push (data_t *data)
	{
		PWX_TRY_PWX_FURTHER (base_t::push_back (data))
		return privConnectEnds();
	}


	/** @brief short alias for push_back(const elem_t &src)
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this ring after the insertion
	**/
	virtual uint32_t push (const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (base_t::push_back (src))
		return privConnectEnds();
	}


	/** @brief alias to add a data pointer to the end of the ring.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this ring after the insertion
	**/
	virtual uint32_t push_back (data_t *data)
	{
		PWX_TRY_PWX_FURTHER (base_t::push_back(data))
		return privConnectEnds();
	}


	/** @brief alias to add an element copy to the end of the ring.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this ring after the insertion
	**/
	virtual uint32_t push_back (const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (base_t::push_back(src))
		return privConnectEnds();
	}


	/** @brief alias to add a data pointer to the head of the ring.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this ring after the insertion
	**/
	virtual uint32_t push_front (data_t *data)
	{
		PWX_TRY_PWX_FURTHER (base_t::insNext (nullptr, data))
		return privConnectEnds();
	}


	/** @brief alias to add an element copy to the head of the ring.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this ring after the insertion
	**/
	virtual uint32_t push_front (const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (base_t::insNext (nullptr, src))
		return privConnectEnds();
	}


	/** @brief remove the element holding the specified data
	  *
	  * This method removes the element in the list that holds @a data
	  * and returns a pointer to the removed element.
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delData instead.
	  *
	  * @param[in] data the data the element is to be removed holds
	  * @return a pointer to the removed element
	**/
	elem_t* remData (data_t* data) noexcept
	{
		elem_t* toRemove = base_t::remData (data);
		if (toRemove)
			privConnectEnds();
		return toRemove;
	}


	/** @brief remove the specified element
	  *
	  * This method removes the element @a elem and returns a pointer
	  * to the removed element.
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delElem instead.
	  *
	  * If @a elem is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * @param[in] elem the element that is to be removed
	  * @return a pointer to the removed element
	**/
	elem_t* remElem (elem_t* elem) noexcept
	{
		elem_t* toRemove = base_t::remElem (elem);
		if (toRemove)
			privConnectEnds();
		return toRemove;
	}


	/** @brief remove the element after the element holding the specified data
	  *
	  * This method removes the element in the list after the element
	  * that holds @a prev and returns a pointer to the removed element.
	  *
	  *
	  * If @a prev is set to nullptr, the root element (aka head) is
	  * removed.
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delNext instead.
	  *
	  * If there is no element behind the element @a prev a nullptr is returned.
	  *
	  * @param[in] prev the data the element that precedes the element to remove holds
	  * @return a pointer to the removed element or nullptr if there is none
	**/
	virtual elem_t* remNext (data_t* prev) noexcept
	{
		elem_t* toRemove = base_t::remNext (prev);
		if (toRemove)
			privConnectEnds();
		return toRemove;
	}


	/** @brief remove the element after the specified element
	  *
	  * This method removes the element in the list after the element
	  * @a prev.
	  *
	  * If @a prev is set to nullptr, the root element (aka head) is
	  * removed.
	  *
	  * If @a prev is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If there is no element behind the element @a prev or if the list is
	  * empty, a nullptr is returned.
	  *
	  * @param[in] prev the element that precedes the element to remove
	  * @return a pointer to the removed element or nullptr if there is none
	**/
	virtual elem_t* remNextElem (elem_t* prev) noexcept
	{
		elem_t* toRemove = base_t::remNextElem (prev);
		if (toRemove)
			privConnectEnds();
		return toRemove;
	}


	/** @brief remove the element before the element holding the specified data
	  *
	  * This method removes the element in the list before the element
	  * that holds @a next and returns a pointer to the removed element.
	  *
	  * If @a next is set to nullptr, the last element (aka tail) is
	  * removed.
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delPrev instead.
	  *
	  * If there is no element before the element @a next holds, a nullptr is returned.
	  *
	  * @param[in] next the data the element that succeeds the element to remove holds
	  * @return a pointer to the removed element or nullptr if there is none
	**/
	elem_t* remPrev (data_t* next) noexcept
	{
		elem_t* toRemove = base_t::remPrev (next);
		if (toRemove)
			privConnectEnds();
		return toRemove;
	}


	/** @brief remove the element before the specified element
	  *
	  * This method removes the element in the list before the element
	  * @a next and returns a pointer to the removed element.
	  *
	  * If @a next is set to nullptr, the last element (aka tail) is
	  * removed.
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delPrevElem instead.
	  *
	  * If @a next is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If there is no element before @a next or if the list is empty,
	  * a nullptr is returned.
	  *
	  * @param[in] next the element that succeeds the element to remove
	  * @return a pointer to the removed element or nullptr if there is none
	**/
	elem_t* remPrevElem (elem_t* next) noexcept
	{
		elem_t* toRemove = base_t::remPrevElem (next);
		if (toRemove)
			privConnectEnds();
		return toRemove;
	}


	using base_t::size;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief assignment operator
	  *
	  * Clears this list and copies all elements from @a rhs
	  * into this list.
	  *
	  * @param[in] rhs reference of the list to copy.
	  * @return reference to this.
	**/
	virtual list_t &operator= (const list_t &rhs)
	{
		if (&rhs != this) {
			PWX_TRY_PWX_FURTHER (base_t::operator= (rhs))
			privConnectEnds();
		}
		return *this;
	}


	/** @brief addition assignment operator
	  *
	  * Add all elements from @a rhs to this list.
	  *
	  * @param[in] rhs reference of the list to add.
	  * @return reference to this.
	**/
	virtual list_t &operator+= (const list_t &rhs)
	{
		if (&rhs != this) {
			PWX_TRY_PWX_FURTHER (base_t::operator+= (rhs))
			privConnectEnds();
		}
		return *this;
	}


	/** @brief substraction assignment operator
	  *
	  * Remove all elements from @a rhs from this list.
	  *
	  * @param[in] rhs reference of the list to substract.
	  * @return reference to this.
	**/
	virtual list_t &operator-= (const list_t &rhs)
	{
		if (&rhs != this) {
			PWX_TRY_PWX_FURTHER (base_t::operator-= (rhs))
			privConnectEnds();
		} else
			clear();
		return *this;
	}


	using base_t::operator[];


protected:
	/* ===============================================
	 * === Protected methods                       ===
	 * ===============================================
	*/

	using base_t::head;
	using base_t::tail;


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	using base_t::eCount;


private:
	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	/// @brief simple private method to make sure the ring is closed
	virtual uint32_t privConnectEnds() noexcept
	{
		// Return early if nothing is to be done:
		if (!head() || !tail() || ((head() == tail()->getNext()) && (tail() == head()->getPrev())) )
			return eCount.load(PWX_MEMORDER_ACQUIRE);

		if (this->beThreadSafe.load(PWX_MEMORDER_RELAXED)) {
			// In this case we do a lock cycle until a valid tail is
			// found or the ring is empty
			PWX_LOCK(this)
			while (tail() && tail()->destroyed()) {
				PWX_UNLOCK(this)
				PWX_LOCK(this)
			}
			// Now tail is either nullptr (ring is empty) or valid and locked.
			if (tail() && (head() != tail()->getNext()))
				tail()->setNext(head());

			// The same has to be done with head()->prev
			while (head() && head()->destroyed()) {
				PWX_UNLOCK(this)
				PWX_LOCK(this)
			}
			// Now head is either nullptr (ring is empty) or valid and locked.
			if (head() && (tail() != head()->getPrev()))
				head()->setPrev(tail());
			PWX_UNLOCK(this)
		} // End of thread safe connection
		else {
			head()->prev.store(tail(), PWX_MEMORDER_RELAXED);
			tail()->next.store(head(), PWX_MEMORDER_RELAXED);
		}

		return eCount.load(PWX_MEMORDER_ACQUIRE);
	}
}; // class TDoubleRing


/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the list.
**/
template<typename data_t, typename elem_t>
TDoubleRing<data_t, elem_t>::~TDoubleRing() noexcept
{ }


/** @brief addition operator
  *
  * Create and return a new ring consisting of all elements of @a lhs and @a rhs.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local ring is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: All elements
  * will be copied twice!
  *
  * @param[in] lhs left hand side reference
  * @param[in] rhs right hand side reference
  * @return the new ring.
**/
template<typename data_t, typename elem_t>
TDoubleRing<data_t, elem_t> operator+ (const TDoubleRing<data_t, elem_t> &lhs, const TDoubleRing<data_t, elem_t> &rhs)
{
	TDoubleRing<data_t, elem_t> result (lhs);

	if (&lhs != &rhs) {
		PWX_TRY_PWX_FURTHER (result += rhs)
	}

	return result;
}


/** @brief substraction operator
  *
  * Remove all elements from @a rhs from this ring.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local ring is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: The difference
  * elements are copied twice!
  *
  * @param[in] lhs reference of the ring from which to substract.
  * @param[in] rhs reference of the ring to substract.
  * @return reference to this.
**/
template<typename data_t, typename elem_t>
TDoubleRing<data_t, elem_t> operator- (const TDoubleRing<data_t, elem_t> &lhs, const TDoubleRing<data_t, elem_t> &rhs)
{
	TDoubleRing<data_t, elem_t> result (lhs);

	if (&lhs != &rhs) {
		PWX_TRY_PWX_FURTHER (result -= rhs)
	} else
		result.clear();

	return result;
}

} // namespace pwx
#endif // PWX_PWXLIB_PWX_CONTAINER_TDOUBLERING_H_INCLUDED

