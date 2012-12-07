#pragma once
#ifndef PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED
#define PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED 1

/** @file TDoubleList.h
  *
  * @brief Declaration of a basic template for doubly linked lists
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

#include "pwx/container/TSingleList.h"
#include "pwx/types/TDoubleElement.h"

namespace pwx
{

/** @class TDoubleList
  *
  * @brief Template to build doubly linked lists of variable types
  *
  * The doubly linked list is an extension to the singly linked list.
  * Elements of type pwx::TDoubleElement<T> are chained together using their next
  * and prev pointers. Being able to look into what lies before an element
  * speeds up certain operations at the cost of one additional pointer per element.
  *
  * The constructor takes an optional destroy(T*) function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * If PWX_THREADS is defined, changes to the element are done in a locked state.
**/
template<typename data_t, typename elem_t = TDoubleElement<data_t> >
class PWX_API TDoubleList : public TSingleList<data_t, elem_t>
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef TSingleList<data_t, elem_t> base_t;
	typedef TDoubleList<data_t, elem_t> list_t;


	/* ===============================================
	 * === Public constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor initializes an empty list.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TDoubleList (void (*destroy_) (data_t* data)) noexcept :
		base_t (destroy_)
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor uses the default constructor to set the data
	  * destroy method to the null pointer.
	**/
	TDoubleList() noexcept :
		base_t (nullptr)
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * @param[in] src reference of the list to copy.
	**/
	TDoubleList (const list_t &src) noexcept :
		base_t (src)
	{
		// The copy ctor of base_t has already copied all elements.
		// But they do not have a valid prev pointer, yet.
		curr = head;
		while (curr != tail) {
			if (curr && curr->next)
				curr->next->prev = curr;
			curr = curr->next;
		}
	}


	virtual ~TDoubleList() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	/** @brief delete all elements
	  *
	  * This is a quick way to get rid of all elements
	  * at once. If a destroy() function was set, it is
	  * used for the data deletion. Otherwise it is
	  * assumed that data_t responds to the delete
	  * operator.
	**/
	virtual void clear() noexcept
	{
		PWX_LOCK_NOEXCEPT(this)
		while (tail) {
			try {
#ifdef PWX_THREADS
				elem_t* toDelete = tail;
				if (toDelete && !toDelete->destroyed()) {
					privRemove(toDelete->prev, toDelete);
					PWX_UNLOCK_NOEXCEPT(this)
					privDelete(toDelete);
				} else {
					PWX_UNLOCK_NOEXCEPT(this)
					std::this_thread::yield();
				}
#else
				privDelete(remElem(tail));
#endif // PWX_THREADS
			}
			PWX_CATCH_AND_FORGET(CException)
			PWX_LOCK_NOEXCEPT(this)
		}
		PWX_UNLOCK_NOEXCEPT(this)
	}


	/** @brief delete the element holding the specified data
	  *
	  * This method deletes the element in the list that holds @a data.
	  *
	  * If you intent to work with the element, use remData instead.
	  *
	  * @param[in] data the data the element that is to be deleted holds
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delData (data_t* data)
	{
		PWX_TRY_PWX_FURTHER(return privDelete(remData(data)))
	}


	/** @brief delete the specified element
	  *
	  * This method deletes the element @a elem.
	  *
	  * If you intent to work with the element, use remElem instead.
	  *
	  * If @a elem is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * @param[in] elem the element that is to be deleted
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delElem (elem_t* elem)
	{
		PWX_TRY_PWX_FURTHER(return privDelete(remElem(elem)))
	}


	using base_t::delNext;
	using base_t::delNextElem;


	/** @brief delete the element before the element holding the specified data
	  *
	  * This method deletes the element in the list before the element
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
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delPrev (data_t* next)
	{
		PWX_TRY_PWX_FURTHER(return privDelete(remPrev(next)))
	}


	/** @brief delete the element before the specified element
	  *
	  * This method deletes the element in the list before the element
	  * @a next.
	  *
	  * If @a next is set to nullptr, the last element (aka tail) is
	  * deleted.
	  *
	  * If you intent to work with the element, use remPrevElem instead.
	  *
	  * If @a next is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If there is no element before the element @a next a
	  * pwx::CException with the name "OutOfRange" is thrown.
	  *
	  * @param[in] next the element that succeeds the element to delete
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delPrevElem (elem_t* next)
	{
		PWX_TRY_PWX_FURTHER(return privDelete(remPrevElem(next)))
	}


	using base_t::empty;
	using base_t::find;
	using base_t::get;
	using base_t::getData;
	using base_t::insNext;
	using base_t::insNextElem;


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
	virtual uint32_t insPrev (data_t* next, data_t* data)
	{
		PWX_TRY_PWX_FURTHER(return privInsDataBeforeData(next, data))
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
	virtual uint32_t insPrev (data_t* next, const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER(return privInsElemBeforeData(next, src))
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
	virtual uint32_t insPrevElem (elem_t* next, data_t* data)
	{
		PWX_TRY_PWX_FURTHER(return privInsDataBeforeElem(next, data))
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
	virtual uint32_t insPrevElem (elem_t* next, const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER(return privInsElemBeforeElem(next, src))
	}


	using base_t::pop;


	/** @brief alias to remove the last element (tail)
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
		uint32_t localCount = size();
		try
			if (localCount > 1) {
				PWX_LOCK(this)
				elem_t* xTail = tail;
				PWX_UNLOCK(this)
				PWX_TRY (return remNextElem (xTail->prev))
				PWX_CATCH_AND_FORGET (CException)
			}
		}
		else if (localCount) {
			PWX_TRY (return remNext (nullptr))
			PWX_CATCH_AND_FORGET (CException)
		}
		return nullptr;
	}


	using base_t::pop_front;
	using base_t::push;
	using base_t::push_back;
	using base_t::push_front;


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
	virtual elem_t* remData (data_t* data) noexcept
	{
		return privRemoveData(data);
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
	virtual elem_t* remElem (elem_t* elem) noexcept
	{
		return privRemoveElem(elem);
	}


	using base_t::remNext;
	using base_t::remNextElem;


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
	  * If there is no element before the element @a next holds, a
	  * pwx::CException with the name "OutOfRange" is thrown.
	  *
	  * @param[in] next the data the element that succeeds the element to remove holds
	  * @return a pointer to the removed element
	**/
	virtual elem_t* remPrev (data_t* next)
	{
		PWX_TRY_PWX_FURTHER(return privRemoveBeforeData(next))
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
	  * a pwx::CException with the name "OutOfRange" is thrown.
	  *
	  * @param[in] next the element that succeeds the element to remove
	  * @return a pointer to the removed element
	**/
	virtual elem_t* remPrevElem (elem_t* next)
	{
		PWX_TRY_PWX_FURTHER(return privRemoveBeforeElem(next))
	}


	using base_t::size;


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
	using base_t::protFind;

	/// @brief simple method to insert an element into the list
	virtual uint32_t protInsert (elem_t* insPrev, elem_t* insElem) noexcept
	{
		uint32_t localCount = size();

		PWX_LOCK_NOEXCEPT(this)
		if (insPrev) {
			// Maintain tail first
			if (tail == insPrev)
				tail = insElem;
			// Then take care of a possible next neighbor
			insElem->next = insPrev->next;
			if (insElem->next)
				insElem->next->prev = insElem;
			// And the previous, of course
			insPrev->next = insElem;
			insElem->prev = insPrev;
		} else if (localCount) {
			insElem->next = head;
			if (head)
				head->prev = insElem;
			head = insElem;
		} else {
			// If we had no insElements yet, head and tail need to be set:
			head = insElem;
			tail = insElem;
		}

		// Set curr and renumber the list
		curr       = insElem;
		doRenumber = true;
		localCount = ++eCount;
		PWX_UNLOCK_NOEXCEPT(this)

		return localCount;
	}


	using base_t::protRenumber;


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	using base_t::eCount;
	using base_t::doRenumber;
	using base_t::curr;
	using base_t::head;
	using base_t::tail;


private:
	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	/* Important: Public methods should not lock unless it is crucial.
	 *            All necessary checks are done by the public methods,
	 *            work is done by the private methods which will lock
	 *            where appropriate.
	*/

	/// @brief Delete the element behind @a prev
	virtual uint32_t privDelete(elem_t* removed)
	{
		try {
			PWX_LOCK(this)
			uint32_t localCount = size();
			PWX_UNLOCK(this)
			if (removed) {
				PWX_LOCK(removed)
				if (!removed->destroyed())
					delete removed;
				else
					PWX_UNLOCK(removed)
			}
			return localCount;
		}
		PWX_THROW_PWXSTD_FURTHER ("delete", "Deleting an element in TDoubleList::privDelete() failed.")
	}


	/// @brief Search until the next element contains the searched data
	virtual elem_t* privFindPrev (const data_t* data) const noexcept
	{
		PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
		elem_t*  oldCurr = curr;
		elem_t*  xHead   = head;
		elem_t*  xTail   = tail;
		PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))

		elem_t*  xCurr   = oldCurr->next; // Go upwards first

		while (xCurr && (xCurr != xTail) ) {
			if (xCurr->data.get() == data) {
				PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
				curr = xCurr;
				PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))
				return xCurr->prev;
			}
			xCurr = xCurr->next;
		} // End of moving upwards

		// If no result has been found, move downwards if oldCurr is not head
		if (oldCurr != xHead) {
			xCurr = oldCurr->prev;

			while (xCurr) {
				if (xCurr->data.get() == data) {
					PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
					curr = xCurr;
					PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))
					return xCurr->prev;
				}
				if (xCurr == xHead)
					xCurr = nullptr; // done, not found
				else
					xCurr = xCurr->prev;
			} // End of moving downwards
		} // End of having started beyond head

		// If we are here, xCurr is nullptr. no match found
		return nullptr;
	}


	/// @brief wrapping method to retrieve an element by any index or nullptr if the list is empty
	virtual const elem_t* privGetElementByIndex (int32_t index) const noexcept
	{
		protRenumber();
		uint32_t localCount = size();

		if (localCount) {
			// Mod index into range
			uint32_t xIdx = static_cast<uint32_t> (index < 0
												   ? localCount - (std::abs (index) % localCount)
												   : index % localCount);
			// Unfortunately this results in xIdx equaling localCount
			// (which is wrong) if index is a negative multiple of
			// localCount:
			if (xIdx >= localCount)
				xIdx = xIdx % localCount;

			PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
			elem_t*  xCurr = curr;
			elem_t*  xHead = head;
			elem_t*  xTail = tail;
			// See note in TSingleList about why a local number is used.
			uint32_t xNr   = xCurr->eNr;
			PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))

			// Is xCurr already correct?
			if (xIdx == xNr)
				return xCurr;

			// Is xIdx the next member, like in an upward for loop?
			if (xIdx == (xNr + 1)) {
				xCurr = xCurr->next;
				PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
				curr  = xCurr;
				PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))
				return xCurr;
			}

			// Is xIdx the prev member, like in a downward for loop?
			if (xNr && (xIdx == (xNr - 1)) ) {
				xCurr = xCurr->prev;
				PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
				curr  = xCurr;
				PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))
				return xCurr;
			}

			// Is it the head we want?
			if (0 == xIdx)
				return xHead;

			// Or tail ?
			if ( (localCount - 1) == xIdx)
				return xTail;

			/* Manual search with four possibilities:
			 * A) xIdx is between xNr and eCount
			 *   1: xIdx is nearer to xNr    -> move upwards from xCurr
			 *   2: xIdx is nearer to eCount -> move downwards from tail
			 * B) xIdx is between 0 and xNr
			 *   1: xIdx is nearer to 0      -> move upwards from head
			 *   2: xIdx is nearer to xNr    -> move downwards from xCurr
			*/
			bool goUp = true;
			if (xIdx > xNr) {
				// A) xIdx is between xNr and localCount
				if ( (xIdx - xNr) <= ( (localCount - xNr) / 2)) {
					// 1: xIdx is nearer to xNr -> move upwards from curr
					xCurr = xCurr->next;
					++xNr;
				} else {
					// 2: xIdx is nearer to localCount -> move downwards from tail
					goUp  = false;
					xCurr = xTail->prev;
					xNr   = localCount - 2;
				}
			} // end of group A
			else {
				// B) xIdx is between 0 and xNr
				if (xIdx <= (xNr / 2)) {
					// 1: xIdx is nearer to 0 -> move upwards from head
					xCurr = xHead->next;
					xNr  = 1;
				} else {
					// 2: xIdx is nearer to eNr -> move downwards from curr
					goUp = false;
					xCurr = xCurr->prev;
					--xNr;
				}
			} // end of group B

			// Now solve the move by a simple while loop:
			while (xCurr && (xIdx != xNr)) {
				xCurr = goUp ? xCurr->next : xCurr->prev;
				xNr += goUp ? 1 : -1;

				// Same solution to a "dead-end" like in the single list
				if ( (xTail == xCurr) || (xHead == xCurr) ) {
					try {
						PWX_LOCK_GUARD(list_t, const_cast<list_t*>(this))
						curr = xCurr;
						return privGetElementByIndex(index);
					}
					PWX_CATCH_AND_FORGET(std::exception)
				}
			}

			// If xCurr is still set, it points to where it should now:
			if (xCurr) {
				PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
				curr = xCurr;
				PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))
			}

			return xCurr;
		}

		return nullptr;
	}


	/// @brief preparation method to insert data before data
	virtual uint32_t privInsDataBeforeData(data_t* next, data_t* data)
	{
		// 1: Prepare the next element
		elem_t* nextElement = next ? const_cast<elem_t*>(find(next)) : nullptr;
		if (next && (nullptr == nextElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this doubly linked list")

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY_STD_FURTHER (newElement = new elem_t (data, destroy),
							 "ElementCreationFailed",
							 "The Creation of a new list element failed.")

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(nextElement ? nextElement->prev : nullptr, newElement))
	}


	/// @brief preparation method to insert data before an element
	virtual uint32_t privInsDataBeforeElem(elem_t* next, data_t* data)
	{
		// 1: Prepare the next element
		elem_t* nextElement = next;

#ifdef PWX_THREADDEBUG
		if (nextElement) {
			PWX_LOCK(nextElement)
			while (nextElement->destroyed()) {
				// This is bad. It means that someone manually deleted the element.
				// If the element still has a prev, or if it is the last element,
				// we can, however, continue.
				uint32_t localCount = size();
				if ((localCount > 1) && nextElement->prev) {
					PWX_LOCK(nextElement->prev)
					PWX_UNLOCK(nextElement)
					nextElement = nextElement->prev;
				}
				else if (localCount < 2) {
					PWX_UNLOCK(nextElement)
					nextElement = nullptr; // New head about
				}
				else {
					PWX_UNLOCK(nextElement)
					// my bad...
					PWX_THROW("Illegal Condition", "Next element destroyed",
							  "An element used as next for insertion is destroyed.")
				}
			} // End of ensuring a valid nextElement
		}
		if (nextElement) PWX_UNLOCK(nextElement)
#endif // PWX_THREADDEBUG

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY_STD_FURTHER (newElement = new elem_t (data, destroy),
							 "ElementCreationFailed",
							 "The Creation of a new list element failed.")

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(nextElement ? nextElement->prev : nullptr, newElement))
	}


	/// @brief preparation method to insert an element copy before data
	virtual uint32_t privInsElemBeforeData(data_t* next, const elem_t &src)
	{
		// 1: Prepare the next element
		elem_t* nextElement = next ? const_cast<elem_t*>(find(next)) : nullptr;
		if (next && (nullptr == nextElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this doubly linked list")

		// 2: Check source:
		PWX_LOCK(const_cast<elem_t*>(&src))

		if (src.destroyed()) {
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			// What on earth did the caller think?
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY_STD_FURTHER (newElement = new elem_t (src),
							 "ElementCreationFailed",
							 "The Creation of a new list element failed.")
		PWX_UNLOCK(const_cast<elem_t*>(&src))

		// 4: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(nextElement ? nextElement->prev : nullptr, newElement))
	}


	/// @brief preparation method to insert an element copy before an element
	virtual uint32_t privInsElemBeforeElem(elem_t* next, const elem_t &src)
	{
		// 1: Prepare the next element
		elem_t* nextElement = next;

#ifdef PWX_THREADDEBUG
		if (nextElement) {
			PWX_LOCK(nextElement)
			while (nextElement->destroyed()) {
				// This is bad. It means that someone manually deleted the element.
				// If the element still has a prev, or if it is the last element,
				// we can, however, continue.
				uint32_t localCount = size();
				if ((localCount > 1) && nextElement->prev) {
					PWX_LOCK(nextElement->prev)
					PWX_UNLOCK(nextElement)
					nextElement = nextElement->prev;
				}
				else if (localCount < 2) {
					PWX_UNLOCK(nextElement)
					nextElement = nullptr; // New head about
				}
				else {
					PWX_UNLOCK(nextElement)
					// my bad...
					PWX_THROW("Illegal Condition", "Next element destroyed",
							  "An element used as next for insertion is destroyed.")
				}
			} // End of ensuring a valid nextElement
		}
		if (nextElement) PWX_UNLOCK(nextElement)
#endif // PWX_THREADDEBUG

		// 2: Check source:
		PWX_LOCK(const_cast<elem_t*>(&src))

		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY_STD_FURTHER (newElement = new elem_t (src),
							 "ElementCreationFailed",
							 "The Creation of a new list element failed.")
		PWX_UNLOCK(const_cast<elem_t*>(&src))

		// 4: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(nextElement ? nextElement->prev : nullptr, newElement))
	}


	/// @brief simple method to remove an element from the list
	virtual void privRemove (elem_t* prev, elem_t* elem) noexcept
	{
		if (elem) {
			PWX_LOCK_NOEXCEPT(this)

			// maintain tail and head first
			if (tail == elem)
				tail = prev;

			if (elem == head)
				head = elem->next;

			// now maintain the neighbors
			if (prev) {
				prev->next = elem->next;
				curr = prev;
			} else {
				curr = head;
				// If this was the last element, tail is nullptr now.
				if (nullptr == tail)
					head = nullptr;
			}

			if (curr && curr->next)
				curr->next->prev = curr;

			// Finally elem does not need pointers to its neighbors any more
			// and the list needs to be renumbered
			elem->next = nullptr;
			elem->prev = nullptr;
			doRenumber = true;
			--eCount;
			PWX_UNLOCK_NOEXCEPT(this)
		} // end of having an element to remove
	}


	/// @brief simple wrapper to prepare the direct removal of data
	virtual elem_t* privRemoveData(data_t* data) noexcept
	{
		elem_t* toRemove = nullptr;

		if ( (nullptr == data) || (nullptr == (toRemove = find (data))) )
			return nullptr;

		privRemove (toRemove->prev, toRemove);

		return toRemove;
	}


	/// @brief simple wrapper to prepare the direct removal of an element
	virtual elem_t* privRemoveElem(elem_t* elem) noexcept
	{
		PWX_LOCK_GUARD(elem_t, elem)

		if (elem->destroyed())
			// If it is deleted, it can not be removed as it is already detached.
			// And more important: It will go away after the unlock!
			return nullptr;

		privRemove (elem->prev, elem);

		return elem;
	}


	/// @brief simple wrapper to prepare the removal of an element before data
	virtual elem_t* privRemoveBeforeData(data_t* next)
	{
		elem_t* xNext = nullptr;

		if (next && (nullptr == (xNext = find (next))) )
			PWX_THROW ("ElementNotFound", "Element not found",
						"The searched element can not be found in this doubly linked list")

		if (xNext && (nullptr == xNext->prev))
			PWX_THROW ("OutOfRange", "Element out of range",
						"There is no element before the element holding the given next pointer")

		elem_t* toRemove = xNext ? xNext->prev : tail;

		privRemove (toRemove->prev, toRemove);

		return toRemove;
	}


	/// @brief simple wrapper to prepare the removal of an element before another element
	virtual elem_t* privRemoveBeforeElem(elem_t* next)
	{
		PWX_LOCK_GUARD(list_t, this)

#ifdef PWX_THREADDEBUG
		if (next && next->destroyed()) {
			// If it is deleted, there is no "prev" to get on with
			PWX_THROW("Illegal Condition", "next element destroyed",
					  "The next element for a removal is already destroyed.")
		}
#endif // PWX_THREADDEBUG

		if (next && (nullptr == next->prev))
			PWX_THROW ("OutOfRange", "Element out of range", "There is no element before the given next element")

		if (empty())
			PWX_THROW ("OutOfRange", "Element out of range", "The list is empty")

		elem_t* toRemove = next ? next->prev : tail;

		privRemove (toRemove->prev, toRemove);

		return toRemove;
	}
}; // class TDoubleList

/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the list.
**/
template<typename data_t, typename elem_t>
TDoubleList<data_t, elem_t>::~TDoubleList() noexcept
{ }


/** @brief addition operator
  *
  *
  * Create and return a new list consisting of all elements of @a lhs and @a rhs.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local list is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: All elements
  * will be copied twice!
  * Only use this operator if you really think you need a construct like
  * 'listC = listA + listB'.
  *
  * @param[in] lhs left hand side reference
  * @param[in] rhs right hand side reference
  * @return the new list.
**/
template<typename data_t, typename elem_t>
TDoubleList<data_t, elem_t> operator+ (const TDoubleList<data_t, elem_t> &lhs, const TDoubleList<data_t, elem_t> &rhs)
{
	TDoubleList<data_t, elem_t> result (lhs);

	if (&lhs != &rhs) {
		PWX_TRY_PWX_FURTHER (result += rhs)
	}

	return result;
}


/** @brief substraction operator
  *
  * Remove all elements from @a rhs from this list.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local list is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: The difference
  * elements are copied twice!
  *
  * @param[in] rhs reference of the list to substract.
  * @return reference to this.
**/
template<typename data_t, typename elem_t>
TDoubleList<data_t, elem_t> operator- (const TDoubleList<data_t, elem_t> &lhs, const TDoubleList<data_t, elem_t> &rhs)
{
	TDoubleList<data_t, elem_t> result (lhs);

	if (&lhs != &rhs) {
		PWX_TRY_PWX_FURTHER (result -= rhs)
	} else
		result.clear();

	return result;
}

} // namespace pwx

#endif // PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED
