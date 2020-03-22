#ifndef PWX_LIBPWX_PWX_CONTAINER_TQUEUE_H_INCLUDED
#define PWX_LIBPWX_PWX_CONTAINER_TQUEUE_H_INCLUDED 1
#pragma once

/** @file TQueue.h
  *
  * @brief Declaration of a basic template for queues based on doubly linked lists
  *
  * (c) 2007 - 2020 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
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


#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"

#include "basic/CLockable.h"
#include "container/TDoubleList.h"


namespace pwx {


/** @class TQueue PQueue <PQueue>
  *
  * @brief Template to build queues of variable types
  *
  * The queue is a basic container derived from `std::TDoubleList` to manage its
  * elements. The queue is deriving from (isA) instead of using (hasA) a TDoubleList
  * to enable it to be used like a list if necessary without having to copy a lot of
  * code.
  *
  * The constructor takes an optional `destroy(T*)` function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * It is recommended that you use the much more advanced `std::queue` or `std::deque`
  * unless you need to store a very large number of elements and can not live with
  * the downside of every element having to be copied into the std container.
  *
  * @see `pwx::TDoubleList` for further information.
**/
template<typename data_t>
class PWX_API TQueue : public TDoubleList<data_t> {
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef TDoubleElement<data_t>      elem_t; //!< Type of the stored elements
	typedef TDoubleList<data_t, elem_t> base_t; //!< Base type of the queue
	typedef TQueue<data_t>              list_t; //!< Type of this queue


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor initializes an empty queue.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TQueue ( void ( *destroy_ ) ( data_t* data ) ) noexcept :
		base_t( destroy_ )
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor sets the data destroy method to the null pointer.
	**/
	TQueue() noexcept :
		base_t ( nullptr )
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * @param[in] src reference of the list to copy.
	**/
	TQueue ( const list_t& src ) noexcept :
		base_t ( src ) {
		// The copy ctor of base_t has already copied all elements.
	}


	virtual ~TQueue() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	using base_t::clear;
	using base_t::delData;
	using base_t::delElem;
	using base_t::delNext;
	using base_t::delNextElem;
	using base_t::delPrev;
	using base_t::delPrevElem;
	using base_t::disable_thread_safety;
	using base_t::empty;
	using base_t::enable_thread_safety;
	using base_t::find;
	using base_t::get;
	using base_t::getData;
	using base_t::insNext;
	using base_t::insNextElem;
	using base_t::insPrev;
	using base_t::insPrevElem;


	/** @brief pop the first element from the queue
	  *
	  * This is the regular queue operation to get the first element.
	  * Being a queue this element comes from the start.
	  *
	  * To get an element from the back, use pop_back() or shift().
	  *
	  * The element is removed from the queue so you have to take
	  * care of its deletion once you are finished with it.
	  *
	  * If there is no element in the queue a pwx::CException with the
	  * name "OutOfRange" is thrown.
	  *
	  * @return the first element on the queue.
	**/
	virtual elem_t* pop() {
		PWX_TRY_PWX_FURTHER ( return base_t::pop_front() )
	}


	using base_t::pop_back;
	using base_t::pop_front;
	using base_t::push;
	using base_t::push_back;
	using base_t::push_front;
	using base_t::remData;
	using base_t::remElem;
	using base_t::remNext;
	using base_t::remNextElem;
	using base_t::remPrev;
	using base_t::remPrevElem;


	/** @brief shift an element from the queue
	  *
	  * This is the irregular queue operation shifting an element
	  * from the end.
	  *
	  * To get an element from the start, use pop() or pop_front().
	  *
	  * The element is removed from the queue so you have to take
	  * care of its deletion once you are finished with it.
	  *
	  * If there is no element in the queue a pwx::CException with the
	  * name "OutOfRange" is thrown.
	  *
	  * @return the last element on the queue.
	**/
	virtual elem_t* shift() {
		PWX_TRY_PWX_FURTHER( return pop_back() )
	}


	using base_t::size;


	/** @brief unshift an element to the end of the queue
	  *
	  * This is the irregular queue operation unshifting an element
	  * to the end of the queue.
	  *
	  * To add an element to the front, use push() or push_front().
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data data pointer to store.
	  * @return number of elements stored after the operation.
	**/
	virtual uint32_t unshift( data_t* data ) {
		PWX_TRY_PWX_FURTHER( return push_back( data ) )
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


/* --- Override new/delete, so we can do memory allocation recording if wanted --- */
#ifndef PWX_NODOX
public:
	void* operator new     ( decltype( sizeof( 0 ) ) s )                   { return ( void* )pwx_calloc( uint8_t, s ); }
	void* operator new[]   ( decltype( sizeof( 0 ) ) s )                   { return ( void* )pwx_calloc( uint8_t, s ); }
	void  operator delete  ( void* ptr )                          noexcept { pwx_free( ptr ); }
	void  operator delete  ( void* ptr, decltype( sizeof( 0 ) ) ) noexcept { pwx_free( ptr ); }
	void  operator delete[]( void* ptr )                          noexcept { pwx_free( ptr ); }
	void  operator delete[]( void* ptr, decltype( sizeof( 0 ) ) ) noexcept { pwx_free( ptr ); }
#endif // NODOX
}; // class TQueue


/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the queue.
**/
template<typename data_t>
TQueue<data_t>::~TQueue() noexcept
{ /* done in base_t dtor */ }


} // namespace pwx

#endif // PWX_LIBPWX_PWX_CONTAINER_TQUEUE_H_INCLUDED

