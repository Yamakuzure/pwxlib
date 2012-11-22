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

#include <pwx/container/TSingleList.h>
#include <pwx/types/TDoubleElement.h>

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
		eNr  = 0;
		while (eNr < (eCount - 1)) {
			if (curr && curr->next)
				curr->next->prev = curr;
			curr = curr->next;
			++eNr;
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
		while (eCount) {
			try {
#ifdef PWX_THREADS
				PWX_LOCK_NOEXCEPT(this)
				elem_t* toDelete = tail;
				if (eCount && toDelete && !toDelete->destroyed()) {
					privRemove(toDelete->prev, toDelete);
					PWX_UNLOCK_NOEXCEPT(this)
					privDelete(toDelete);
				} else
					PWX_UNLOCK_NOEXCEPT(this)
#else
				privDelete(remElem(tail));
#endif // PWX_THREADS
			}
			PWX_CATCH_AND_FORGET(CException)
		}
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


	// empty() provided by base_t


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
	using base_t::pop_back;
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
	virtual uint32_t protInsert (elem_t* prev, elem_t* elem) noexcept
	{
		PWX_LOCK(this)
		if (prev) {
			if (tail == prev)
				tail = elem;
			elem->next = prev->next;
			if (elem->next)
				elem->next->prev = elem;
			prev->next = elem;
			elem->prev = prev;
			// curr is only maintainable if it is prev
			if (curr != prev) {
				// In which case it wouldn't have needed any change.
				curr = head;
				eNr  = 0;
			}
		} else if (eCount) {
			elem->next = head;
			if (head)
				head->prev = elem;
			head = elem;
			++eNr; // No matter what happened, curr has another element before it now.
		} else {
			// If we had no elements yet, head and tail need to be set:
			head = elem;
			tail = elem;
			curr = head;
		}

		uint32_t xCount = ++eCount;

		PWX_UNLOCK(this)

		return xCount;
	}


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	using base_t::eCount;
	using base_t::eNr;
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
			if (removed) {
				PWX_LOCK(removed)
				if (!removed->destroyed())
					delete removed;
				else
					PWX_UNLOCK(removed)
			}
			return eCount;
		}
		PWX_THROW_PWXSTD_FURTHER ("delete", "Deleting an element in TDoubleList::privDelete() failed.")
	}


	/// @brief Search until the next element contains the searched data
	virtual elem_t* protFindPrev (const data_t* data) const noexcept
	{
		PWX_LOCK(const_cast<list_t*>(this))
		elem_t*  oldCurr = curr;
		elem_t*  xCurr   = oldCurr->next; // Go upwards first
		uint32_t oldNr   = eNr;
		uint32_t xNr     = oldNr + 1;
		PWX_UNLOCK(const_cast<list_t*>(this))

		while (xCurr && (xCurr != tail) ) {
			if (xCurr->data.get() == data) {
				PWX_LOCK(const_cast<list_t*>(this))
				curr = xCurr;
				eNr  = xNr;
				PWX_UNLOCK(const_cast<list_t*>(this))
				return xCurr->prev;
			}
			++xNr;
			xCurr = xCurr->next;
		} // End of moving upwards

		// If no result has been found, move downwards oldCurr is not head
		if (oldNr) {
			xCurr = oldCurr->prev;
			xNr   = oldNr - 1;

			while (xCurr) {
				if (xCurr->data.get() == data) {
					PWX_LOCK(const_cast<list_t*>(this))
					curr = xCurr;
					eNr  = xNr;
					PWX_UNLOCK(const_cast<list_t*>(this))
					return xCurr->prev;
				}
				if (xCurr == head)
					xCurr = nullptr; // done, not found
				else {
					--xNr;
					xCurr = xCurr->prev;
				}
			} // End of moving downwards
		} // End of having started beyond head

		// If we are here, xCurr is nullptr. no match found
		return nullptr;
	}


	/// @brief wrapping method to retrieve an element by any index or nullptr if the list is empty
	virtual const elem_t* privGetElementByIndex (int32_t index) const noexcept
	{
		if (eCount) {
			PWX_LOCK(const_cast<list_t*>(this))
			elem_t*  xCurr  = curr;
			uint32_t xNr    = eNr;
			uint32_t xCount = eCount;
			PWX_UNLOCK(const_cast<list_t*>(this))

			// Mod index into range
			uint32_t xIdx = static_cast<uint32_t> (index < 0
												   ? xCount - (::std::abs (index) % xCount)
												   : index % xCount);
			// Unfortunately this results in xIdx equaling xCount
			// (which is wrong) if index is a negative multiple of
			// xCount:
			if (xIdx >= xCount)
				xIdx = xIdx % xCount;

			// Is xCurr already correct?
			if (xIdx == xNr)
				return xCurr;

			// Is xIdx the next member, like in an upward for loop?
			if (xIdx == (eNr + 1)) {
				xCurr = xCurr->next;
				PWX_LOCK(const_cast<list_t*>(this))
				curr  = xCurr;
				eNr   = xNr + 1;
				PWX_UNLOCK(const_cast<list_t*>(this))
				return xCurr;
			}

			// Is xIdx the prev member, like in a downward for loop?
			if (xIdx == (eNr - 1)) {
				xCurr = xCurr->prev;
				PWX_LOCK(const_cast<list_t*>(this))
				curr  = xCurr;
				eNr   = xNr - 1;
				PWX_UNLOCK(const_cast<list_t*>(this))
				return xCurr;
			}

			// Is it the head we want?
			if (0 == xIdx)
				return head;

			// Or tail ?
			if ( (eCount - 1) == xIdx)
				return tail;

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
				// A) xIdx is between xNr and eCount
				if ( (xIdx - xNr) <= ( (eCount - xNr) / 2)) {
					// 1: xIdx is nearer to eNr -> move upwards from curr
					xCurr = xCurr->next;
					++eNr;
				} else {
					// 2: xIdx is nearer to eCount -> move downwards from tail
					goUp  = false;
					PWX_LOCK(const_cast<list_t*>(this))
					xCurr = tail->prev;
					xNr   = eCount - 2;
					PWX_UNLOCK(const_cast<list_t*>(this))
				}
			} // end of group A
			else {
				// B) xIdx is between 0 and xNr
				if (xIdx <= (xNr / 2)) {
					// 1: xIdx is nearer to 0 -> move upwards from head
					PWX_LOCK(const_cast<list_t*>(this))
					xCurr = head->next;
					xNr  = 1;
					PWX_UNLOCK(const_cast<list_t*>(this))
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

				// if Another thread deletes any element during this search,
				// another "flip" is necessary to stay valid:
				PWX_LOCK(const_cast<list_t*>(this))
				if ( (tail == xCurr) || (head == xCurr) ) {
					xCount = eCount;
					if (xCount) {
						// We need to re-mod xIdx:
						xIdx = static_cast<uint32_t> (index < 0
													? eCount - (::std::abs (index) % eCount)
													: index % eCount);
						if (xIdx >= eCount)
							xIdx = xIdx % eCount;
						xCurr = goUp ? head : tail;
						xNr   = goUp ? 0 : (eCount - 1);
					} else
						xCurr = nullptr; // List has been emptied?
				}
				PWX_UNLOCK(const_cast<list_t*>(this))
			}

			// If xCurr is still set, it points to where it should now:
			if (xCurr) {
				PWX_LOCK(const_cast<list_t*>(this))
				curr = xCurr;
				eNr  = xNr;
				PWX_UNLOCK(const_cast<list_t*>(this))
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
				if ((eCount > 1) && nextElement->prev) {
					PWX_LOCK(nextElement->prev)
					PWX_UNLOCK(nextElement)
					nextElement = nextElement->prev;
				}
				else if (eCount < 2) {
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

#ifdef PWX_THREADDEBUG
		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}
#endif // PWX_THREADDEBUG

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
				if ((eCount > 1) && nextElement->prev) {
					PWX_LOCK(nextElement->prev)
					PWX_UNLOCK(nextElement)
					nextElement = nextElement->prev;
				}
				else if (eCount < 2) {
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

#ifdef PWX_THREADDEBUG
		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}
#endif // PWX_THREADDEBUG

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
			PWX_LOCK(this)

			// maintain tail and head first
			if (tail == elem)
				tail = elem->prev;

			if (elem == head)
				head = elem->next;

			// now maintain the neighbors
			if (elem->next)
				elem->next->prev = elem->prev;
			if (elem->prev)
				elem->prev->next = elem->next;

			// curr needs to be valid
			if (curr == elem) {
				if (elem->next)
					curr = elem->next;
				else if (prev) {
					curr = prev;
					--eNr;
				}
			} else {
				curr = head;
				eNr  = 0;
			} // End of maintaining curr

			// if this was the last element, sanitize the list:
			if (1 == eCount) {
				head = nullptr;
				curr = nullptr;
				tail = nullptr;
				eNr  = 0;
			}

			// Finally elem does not need pointers to its neighbors any more
			elem->next = nullptr;
			elem->prev = nullptr;
			--eCount;
			PWX_UNLOCK(this)
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
		PWX_LOCK(next)

#ifdef PWX_THREADDEBUG
		if (next->destroyed()) {
			// If it is deleted, there is no "prev" to get on with
			PWX_THROW("Illegal Condition", "next element destroyed",
					  "The next element for a removal is already destroyed.")
		}
#endif // PWX_THREADDEBUG

		if (next && (nullptr == next->prev))
			PWX_THROW ("OutOfRange", "Element out of range", "There is no element before the given next element")

		if (0 == eCount)
			PWX_THROW ("OutOfRange", "Element out of range", "The list is empty")

		elem_t* toRemove = next ? next->prev : tail;

		privRemove (toRemove->prev, toRemove);

		PWX_UNLOCK(next)

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
  *
  * @param[in] lhs left hand side reference
  * @param[in] rhs right hand side reference
  * @return the new list.
**/
template<typename data_t, typename elem_t>
TDoubleList<data_t, elem_t> operator+ (const TDoubleList<data_t, elem_t> &lhs, const TDoubleList<data_t, elem_t> &rhs)
{
	typedef TDoubleList<data_t, elem_t> list_t;
	PWX_LOCK(&lhs)
	list_t result (lhs);
	PWX_UNLOCK(&lhs)
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
	typedef TDoubleList<data_t, elem_t> list_t;
	PWX_LOCK(&lhs)
	list_t result (lhs);
	PWX_UNLOCK(&lhs)

	if (&lhs != &rhs) {
		PWX_TRY_PWX_FURTHER (result -= rhs)
	} else
		result.clear();

	return result;
}

} // namespace pwx

#endif // PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED
