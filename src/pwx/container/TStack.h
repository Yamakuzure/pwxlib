#pragma once
#ifndef PWX_LIBPWX_PWX_CONTAINER_TSTACK_H_INCLUDED
#define PWX_LIBPWX_PWX_CONTAINER_TSTACK_H_INCLUDED 1

#include <pwx/container/TSingleList.h>

/** @file TStack.h
  *
  * @brief Declaration of a basic template for stacks based on singly linked lists
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

#include <pwx/types/CLockable.h>
#include <pwx/general/macros.h>
#include <pwx/container/TSingleList.h>

namespace pwx
{

/** @class TStack
  *
  * @brief Template to build stacks of variable types
  *
  * The stack is a basic container deriving TSingleList to manage its data
  * pointers. The stack is deriving from (isA) instead of using (hasA) a
  * TSingleList to enable it to be used like a list if necessary without
  * having to copy a lot of code.
  *
  * The constructor takes an optional destroy(T*) function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * Being a stack, the meanings of head, tail, front and back are reversed compared
  * to the singly linked list. The first added element will always be "head", and
  * meant by "back". "front" is where the next new item is pushed and where "tail"
  * resides. Please keep that in mind. However, push() and pop() will always do
  * the right thing for you.
  *
  * It is recommended that you use the much more advanced std::stack unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::stack.
  *
  * === FIXME : ===
  * original: "If PWX_THREADS is defined, changes to the container are done in a locked state."
  * -> This must be changed. No automatic locking all the time, but run time
  *     variable handling of thread safety.
  *    - How ? Maybe telling Containers via VContainer whether they are used concurrently or not?
  * === : EMXIF ===
**/
template<typename data_t>
class PWX_API TStack : public TSingleList<data_t>
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef TSingleElement<data_t>      elem_t;
	typedef TSingleList<data_t, elem_t> base_t;
	typedef TStack<data_t>              list_t;


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor initializes an empty stack.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TStack (void (*destroy_) (data_t* data)) noexcept :
		base_t(destroy_)
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor sets the data destroy method to the null pointer.
	**/
	TStack() noexcept :
		base_t (nullptr)
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * @param[in] src reference of the list to copy.
	**/
	TStack (const list_t &src) noexcept :
		base_t (src)
	{
		// The copy ctor of base_t has already copied all elements.
	}


	virtual ~TStack() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	using base_t::clear;
	using base_t::delNext;
	using base_t::delNextElem;
	using base_t::empty;
	using base_t::find;
	using base_t::get;
	using base_t::getData;
	using base_t::insNext;
	using base_t::insNextElem;


	/** @brief pop the top element from the stack
	  *
	  * This is the regular stack operation to get the top element.
	  *
	  * To get an element from the bottom, use pop_back() or shift().
	  *
	  * The element is removed from the stack so you have to take
	  * care of its deletion once you are finished with it.
	  *
	  * If there is no element in the stack a pwx::CException with the
	  * name "OutOfRange" is thrown.
	  *
	  * @return the top element on the stack.
	**/
	virtual elem_t* pop() noexcept
	{
		PWX_TRY_PWX_FURTHER (return base_t::pop_back())
	}


	/** @brief alias to remove the bottom element (head)
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delNext instead.
	  *
	  * If the list is empty, nullptr is returned.
	  *
	  * @return a pointer to the removed element or nullptr if the list is empty
	**/
	virtual elem_t* pop_back() noexcept
	{
		return base_t::pop_front();
	}


	/** @brief alias to remove the top element (tail)
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delNext instead.
	  *
	  * If the list is empty, nullptr is returned.
	  *
	  * @return a pointer to the removed element or nullptr if the list is empty
	**/
	virtual elem_t* pop_front() noexcept
	{
		return base_t::pop_back();
	}


	/** @brief short alias for push_back(data_t *data)
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push (data_t *data)
	{
		PWX_TRY_PWX_FURTHER (return base_t::push_back (data))
	}


	/** @brief short alias for push_back(const elem_t &src)
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push (const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (return base_t::push_back (src))
	}


	/** @brief alias to add a data pointer to the bottom of the stack.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_back (data_t *data)
	{
		PWX_TRY_PWX_FURTHER (return base_t::push_front (data))
	}


	/** @brief alias to add an element copy to the bottom of the stack.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_back (const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (return base_t::push_front (src))
	}


	/** @brief alias to add a data pointer to the top of the stack.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_front (data_t *data)
	{
		PWX_TRY_PWX_FURTHER (return base_t::push_back (data))
	}


	/** @brief alias to add an element copy to the top of the stack.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_front (const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (return base_t::push_back (src))
	}


	using base_t::remNext;
	using base_t::remNextElem;


	/** @brief shift an element from the stack
	  *
	  * This is the irregular stack operation shifting an element
	  * from the bottom.
	  *
	  * To get an element from the top, use pop() or pop_front().
	  *
	  * The element is removed from the stack so you have to take
	  * care of its deletion once you are finished with it.
	  *
	  * If there is no element in the stack a pwx::CException with the
	  * name "OutOfRange" is thrown.
	  *
	  * @return the top element on the stack.
	**/
	virtual elem_t* shift()
	{
		PWX_TRY_PWX_FURTHER(return base_t::pop_front())
	}


	using base_t::size;


	/** @brief unshift an element under the stack
	  *
	  * This is the irregular stack operation unshifting an element
	  * under the bottom.
	  *
	  * To add an element to the top, use push() or push_front().
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data data pointer to store.
	  * @return number of elements stored after the operation.
	**/
	virtual uint32_t unshift(data_t* data)
	{
		PWX_TRY_PWX_FURTHER(return base_t::push_front(data))
	}


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	using base_t::operator=;
	using base_t::operator+=;
	using base_t::operator-=;
	using base_t::operator[];


protected:
	/* ===============================================
	 * === Protected methods                       ===
	 * ===============================================
	*/
	using base_t::destroy;
}; // class TStack


/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the stack.
**/
template<typename data_t>
TStack<data_t>::~TStack() noexcept
{ /* done in base_t dtor */ }

} // namespace pwx

#endif // PWX_LIBPWX_PWX_CONTAINER_TSTACK_H_INCLUDED

