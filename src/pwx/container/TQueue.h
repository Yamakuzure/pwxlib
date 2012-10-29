#pragma once
#ifndef PWX_LIBPWX_PWX_CONTAINER_TQUEUE_H_INCLUDED
#define PWX_LIBPWX_PWX_CONTAINER_TQUEUE_H_INCLUDED 1

#include <pwx/container/TDoubleList.h>

/** @file TQueue.h
  *
  * @brief Declaration of a basic template for queues based on doubly linked lists
  *
  * (c) 2007 - 2012 PrydeWorX
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
#include <pwx/container/TDoubleList.h>

namespace pwx
{

/** @class TQueue
  *
  * @brief Template to build queues of variable types
  *
  * The queue is a basic container deriving TDoubleList to manage its data
  * pointers. The queue is deriving from (isA) instead of using (hasA) a
  * a TDoubleList to enable it to be used like a list if necessary without
  * having to copy a lot of code.
  *
  * The constructor takes an optional destroy(T*) function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * It is recommended that you use the much more advanced std::queue or std::deque
  * unless you need to store a very large number of elements and can not live with
  * the downside of every element having to be copied into the std container.
  *
  * If PWX_THREADS is defined, changes to the element are done in a locked state.
**/
template<typename data_t>
class PWX_API TQueue : public TDoubleList<data_t>
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef TDoubleElement<data_t>      elem_t;
	typedef TDoubleList<data_t, elem_t> base_t;
	typedef TQueue<data_t>              list_t;


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
	TQueue (void (*destroy_) (data_t* data)) noexcept :
		base_t(destroy_)
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor sets the data destroy method to the null pointer.
	**/
	TQueue() noexcept :
		base_t (nullptr)
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * @param[in] src reference of the list to copy.
	**/
	TQueue (const list_t &src) noexcept :
		base_t (src)
	{
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
	using base_t::empty;
	using base_t::find;
	using base_t::get;
	using base_t::getData;
	using base_t::insNext;
	using base_t::insNextElem;
	using base_t::insPrev;
	using base_t::insPrevElem;


	/** @brief pop the last element from the queue
	  *
	  * This is the regular queue operation to get the last element.
	  * Being a queue this element comes from the end.
	  *
	  * To get an element from the front, use pop_front() or shift().
	  *
	  * The element is removed from the queue so you have to take
	  * care of its deletion once you are finished with it.
	  *
	  * If there is no element in the queue a pwx::CException with the
	  * name "OutOfRange" is thrown.
	  *
	  * @return the last element on the queue.
	**/
	virtual elem_t* pop() noexcept
	{
		PWX_TRY_PWX_FURTHER (return pop_back())
	}


	using base_t::pop_back;
	using base_t::pop_front;


	/** @brief push a new data pointer onto the queue
	  *
	  * This is the regular queue operation to add a new element.
	  * Being a queue this new element is put on top of it.
	  *
	  * To add a new data pointer to the bottom, use push_back() or
	  * unshift().
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data data pointer to store.
	  * @return number of elements stored after the operation.
	**/
	virtual uint32_t push (data_t* data)
	{
		PWX_TRY_PWX_FURTHER (return push_front (data))
	}


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
	  * from the head.
	  *
	  * To get an element from the tail, use pop() or pop_back().
	  *
	  * The element is removed from the queue so you have to take
	  * care of its deletion once you are finished with it.
	  *
	  * If there is no element in the queue a pwx::CException with the
	  * name "OutOfRange" is thrown.
	  *
	  * @return the top element on the queue.
	**/
	virtual elem_t* shift()
	{
		PWX_TRY_PWX_FURTHER(return pop_front())
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
	virtual uint32_t unshift(data_t* data)
	{
		PWX_TRY_PWX_FURTHER(return push_back(data))
	}


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief assignment operator
	  *
	  * Clears this queue and copies all elements from @a rhs
	  * onto this queue.
	  *
	  * @param[in] rhs reference of the queue to copy.
	  * @return reference to this.
	**/
	virtual list_t &operator= (const list_t &rhs)
	{
		if (&rhs != this) {
			PWX_DOUBLE_LOCK (list_t, this, list_t, const_cast<list_t*> (&rhs))
			clear();
			destroy = rhs.destroy;
			PWX_TRY_PWX_FURTHER (*this += rhs)
		}
		return *this;
	}


	/** @brief addition assignment operator
	  *
	  * Add all elements from @a rhs to this list. Being a
	  * queue the elements must be retrieved in reverse order
	  * and pushed on the queue. Otherwise the ordering is
	  * reversed and the FiFo structure damaged.
	  *
	  * @param[in] rhs reference of the list to add.
	  * @return reference to this.
	**/
	virtual list_t &operator+= (const list_t & rhs)
	{
		if (&rhs != this) {
			PWX_DOUBLE_LOCK (list_t, this, list_t, const_cast<list_t*> (&rhs))
			int32_t rSize = rhs.size();
			for (int32_t i = 1; i <= rSize; ++i) {
				PWX_TRY_PWX_FURTHER (insNext (nullptr, *rhs[0 - i]))
			}
		}
		return *this;
	}


	using base_t::operator-=;
	using base_t::operator[];


protected:
	/* ===============================================
	 * === Protected methods                       ===
	 * ===============================================
	*/
	using base_t::destroy;
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

