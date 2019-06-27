#ifndef PWX_LIBPWX_PWX_CONTAINER_TSTACK_H_INCLUDED
#define PWX_LIBPWX_PWX_CONTAINER_TSTACK_H_INCLUDED 1
#pragma once

/** @file TStack.h
  *
  * @brief Declaration of a basic template for stacks based on singly linked lists
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


#include "basic/pwx_macros.h"
#include "container/TSingleElement.h"
#include "container/TSingleList.h"


/// @namespace pwx
namespace pwx {


/** @class TStack PStack <PStack>
  *
  * @brief Template to build stacks of variable types
  *
  * The stack is a basic container derived from `pwx::TSingleList` to manage
  * its elements. The stack is deriving from (*isA*) instead of using (*hasA*)
  * a `pwx::TSingleList` to enable it to be used like a list if necessary
  * without having to copy a lot of code.
  *
  * The constructor takes an optional `destroy(T*)` function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * Being a stack, the meanings of head, tail, front and back are reversed compared
  * to the singly linked list. The first added element will always be "head", and
  * meant by "back". "front" is where the next new item is pushed and where "tail"
  * resides. Please keep that in mind. However, push() and pop() will always do
  * the right thing for you.
  *
  * It is recommended that you use the much more advanced `std::stack` or
  * `std::deque`unless you need to store a very large number of elements and can not
  * live with the downside of every element having to be copied into the std
  * container.
  *
  * @see `pwx::TSingleList` for further information.
**/
template<typename data_t>
class PWX_API TStack : public TSingleList<data_t> {
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef TSingleElement<data_t>      elem_t; //!< Type of the stored elements
	typedef TSingleList<data_t, elem_t> base_t; //!< Base type of the stack
	typedef TStack<data_t>              list_t; //!< Type of this stack


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
	TStack ( void ( *destroy_ ) ( data_t* data ) ) noexcept :
		base_t( destroy_ )
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor sets the data destroy method to the null pointer.
	**/
	TStack() noexcept :
		base_t ( nullptr )
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * @param[in] src reference of the list to copy.
	**/
	TStack ( list_t const& src ) noexcept :
		base_t ( src ) {
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
	using base_t::disable_thread_safety;
	using base_t::empty;
	using base_t::enable_thread_safety;
	using base_t::find;
	using base_t::get;
	using base_t::getData;
	using base_t::insNext;
	using base_t::insNextElem;


	/** @brief pop the top element from the stack
	  *
	  * This is the regular stack operation to get the top element.
	  *
	  * To get an element from the bottom, use `pop_back()` or `shift()`.
	  *
	  * The element is removed from the stack so you have to take
	  * care of its deletion once you are finished with it.
	  *
	  * If there is no element in the stack a `pwx::CException` with the
	  * name "OutOfRange" is thrown.
	  *
	  * @return the top element on the stack.
	**/
	virtual elem_t* pop() {
		PWX_TRY_PWX_FURTHER ( return base_t::pop_back() )
	}


	/** @brief alias to remove the bottom element (head)
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use `delNext()` instead.
	  *
	  * If the list is empty, `nullptr` is returned.
	  *
	  * @return a pointer to the removed element or `nullptr` if the list is empty
	**/
	virtual elem_t* pop_back() noexcept {
		return base_t::pop_front();
	}


	/** @brief alias to remove the top element (tail)
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use `delNext()` instead.
	  *
	  * If the list is empty, `nullptr` is returned.
	  *
	  * @return a pointer to the removed element or `nullptr` if the list is empty
	**/
	virtual elem_t* pop_front() noexcept {
		return base_t::pop_back();
	}


	/** @brief short alias for `push_back(data_t *data)`
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push ( data_t* data ) {
		PWX_TRY_PWX_FURTHER ( return base_t::push_back ( data ) )
	}


	/** @brief short alias for `push_back(elem_t const &src)`
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push ( elem_t const& src ) {
		PWX_TRY_PWX_FURTHER ( return base_t::push_back ( src ) )
	}


	/** @brief alias to add a data pointer to the bottom of the stack.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_back ( data_t* data ) {
		PWX_TRY_PWX_FURTHER ( return base_t::push_front ( data ) )
	}


	/** @brief alias to add an element copy to the bottom of the stack.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_back ( elem_t const& src ) {
		PWX_TRY_PWX_FURTHER ( return base_t::push_front ( src ) )
	}


	/** @brief alias to add a data pointer to the top of the stack.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_front ( data_t* data ) {
		PWX_TRY_PWX_FURTHER ( return base_t::push_back ( data ) )
	}


	/** @brief alias to add an element copy to the top of the stack.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_front ( elem_t const& src ) {
		PWX_TRY_PWX_FURTHER ( return base_t::push_back ( src ) )
	}


	using base_t::remNext;
	using base_t::remNextElem;


	/** @brief shift an element from the stack
	  *
	  * This is the irregular stack operation shifting an element
	  * from the bottom.
	  *
	  * To get an element from the top, use `pop()` or `pop_front()`.
	  *
	  * The element is removed from the stack so you have to take
	  * care of its deletion once you are finished with it.
	  *
	  * If there is no element in the stack a `pwx::CException` with the
	  * name "OutOfRange" is thrown.
	  *
	  * @return the top element on the stack.
	**/
	virtual elem_t* shift() {
		PWX_TRY_PWX_FURTHER( return base_t::pop_front() )
	}


	using base_t::size;


	/** @brief unshift an element under the stack
	  *
	  * This is the irregular stack operation unshifting an element
	  * under the bottom.
	  *
	  * To add an element to the top, use `push()` or `push_front()`.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data data pointer to store.
	  * @return number of elements stored after the operation.
	**/
	virtual uint32_t unshift( data_t* data ) {
		PWX_TRY_PWX_FURTHER( return base_t::push_front( data ) )
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

