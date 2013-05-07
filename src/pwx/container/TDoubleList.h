#pragma once
#ifndef PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED
#define PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED 1

/** @file TDoubleList.h
  *
  * @brief Declaration of a basic template for doubly linked lists
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
  * This container is designed to work safely in a multi-threaded environment.
  * See below for more on single and multi threaded usage.
  *
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * <B>Notes on multi threaded environments</B>
  *
  * If you plan to use this container in a strictly single-threaded way, you can
  * turn off most of the thread safety measures with disable_thread_safety().
**/
template<typename data_t, typename elem_t = TDoubleElement<data_t> >
class PWX_API TDoubleList : public TSingleList<data_t, elem_t>
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef TSingleList<data_t, elem_t> base_t; //!< Base type of this list
	typedef TDoubleList<data_t, elem_t> list_t; //!< Type of this list


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
		// No need to do anything. The base ctor copies all elements using
		// protInsert() which is actually in this class.
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
		elem_t* toDelete = nullptr;
		while (tail()) {
			PWX_LOCK(this)
			if (tail()) {
				toDelete = tail();
				privRemove(toDelete);

				// Now that the element is removed, we do not
				// need to have a full lock any more
				PWX_UNLOCK(this)
				if (toDelete) {
					PWX_TRY(protDelete(toDelete))
					catch(...) { } // We can't do anything about that
				}
			} // end of having tail after lock is acquired.
			else
				PWX_UNLOCK(this)
		} // end of while head
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
		PWX_TRY_PWX_FURTHER(return protDelete(remData(data)))
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
		PWX_TRY_PWX_FURTHER(return protDelete(remElem(elem)))
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
		PWX_TRY_PWX_FURTHER(return protDelete(remPrev(next)))
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
		PWX_TRY_PWX_FURTHER(return protDelete(remPrevElem(next)))
	}


	using base_t::disable_thread_safety;
	using base_t::empty;
	using base_t::enable_thread_safety;
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
		return privRemoveElem(tail());
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
	  * If there is no element before the element @a next holds, or if the
	  * list is empty, nullptr is returned.
	  *
	  * @param[in] next the data the element that succeeds the element to remove holds
	  * @return a pointer to the removed element or nullptr if there is no previous element or the list is empty
	**/
	virtual elem_t* remPrev (data_t* next) noexcept
	{
		return privRemoveBeforeData(next);
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
	  * @return a pointer to the removed element or nullptr if @a next is the first element or the list is empty.
	**/
	virtual elem_t* remPrevElem (elem_t* next) noexcept
	{
		return privRemoveBeforeElem(next);
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
	using base_t::protDelete;
	using base_t::protFind;


	/** @brief simple method to insert an element into the list
	  *
	  * If either @a insPrev or @a insElem is marked as destroyed,
	  * a pwx::CException is thrown. Such a condition implies that
	  * there is something seriously wrong.
	  *
	  * @param[in] insPrev Element after which the new element is to be inserted
	  * @param[in] insElem The element to insert.
	  * @return The number of elements in the list after the insertion
	**/
	virtual uint32_t protInsert (elem_t* insPrev, elem_t* insElem)
	{
		/* There are four possibilities:
		 * 1: The list is empty
		 *    head, tail and curr have to be set to the new
		 *    element, no full renumbering is needed then.
		 * 2: insPrev is nullptr
		 *    head has to be changed to be the new element
		 * 3: insPrev is tail
		 *    tail has to be set to the new element, no full
		 *    renumbering is needed then.
		 * 4: Otherwise insPrev->insertNext() can do the insertion
		*/
		PWX_LOCK(this)

		uint32_t locCnt = eCount.load(PWX_MEMORDER_ACQUIRE);

		curr(insElem);

		if (locCnt && insPrev && (tail() != insPrev)) {
			// Case 4: A normal insert
			doRenumber.store(true, PWX_MEMORDER_RELEASE);
			PWX_UNLOCK(this)
			PWX_TRY_PWX_FURTHER(insPrev->insertNext(insElem))
		} else {
			PWX_LOCK_GUARD(list_t, this)
			PWX_UNLOCK(this)
			if (!locCnt) {
				// Case 1: The list is empty
				PWX_TRY_PWX_FURTHER(insElem->insertBefore(nullptr))
				head(insElem);
				tail(insElem);
			} else if (nullptr == insPrev) {
				// Case 2: A new head is to be set
				PWX_TRY_PWX_FURTHER(head()->insertPrev(insElem))
				head(insElem);
				doRenumber.store(true, PWX_MEMORDER_RELEASE);
			} else if (insPrev == tail() ) {
				// Case 3: A new tail is to be set
				insElem->eNr.store(
					tail()->eNr.load(PWX_MEMORDER_ACQUIRE) + 1,
					PWX_MEMORDER_RELEASE);
				PWX_TRY_PWX_FURTHER(tail()->insertNext(insElem))
				tail(insElem);
			}
		}

		eCount.fetch_add(1, PWX_MEMORDER_RELEASE);
		return eCount.load(PWX_MEMORDER_ACQUIRE);
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

	/// @brief Search until the next element contains the searched data
	// Note: This method must be invoked with a lock in place! It does *NOT* lock!
	virtual elem_t* privFindPrev (const data_t* data) const noexcept
	{
		elem_t*  xPrev   = curr();
		elem_t*  xCurr   = xPrev->getNext();

		while (xPrev && (xPrev != tail() )) {
			if (xCurr->data.get() == data) {
				curr(xCurr);
				return xPrev;
			}
			xPrev = xCurr;
			xCurr = xCurr->getNext();
		}

		// If we are here, prev points to tail. No match found.
		xCurr = curr();
		xPrev = xCurr->getPrev();

		while (xCurr && (xCurr != head() )) {
			if (xCurr->data.get() == data) {
				curr(xCurr);
				return xPrev;
			}
			xPrev = xCurr;
			xCurr = xCurr->getPrev();
		}

		// Here xCurr points to head, no match found.
		return nullptr;
	}


	/// @brief wrapping method to retrieve an element by any index or nullptr if the list is empty
	virtual const elem_t* privGetElementByIndex (int32_t index) const noexcept
	{
		protRenumber();

		// It is necessary to lock briefly to ensure a consistent
		// start of the search with a minimum of checks
		PWX_LOCK(const_cast<list_t*>(this))

		uint32_t locCnt = eCount.load(PWX_MEMORDER_ACQUIRE);

		if (locCnt) {
			elem_t*  xCurr = curr() ? curr() : head();
			uint32_t xNr   = xCurr->eNr.load(PWX_MEMORDER_ACQUIRE);

			PWX_UNLOCK(const_cast<list_t*>(this))

			// Mod index into range
			uint32_t xIdx = static_cast<uint32_t> (index < 0
												   ? locCnt - (std::abs (index) % locCnt)
												   : index % locCnt);

			// Unfortunately this results in xIdx equaling locCnt
			// (which is wrong) if index is a negative multiple of
			// locCnt:
			if (xIdx >= locCnt)
				xIdx %= locCnt;

			// Is xCurr already correct?
			if (xIdx == xNr)
				return xCurr;

			// Is xIdx the next member, like in a for loop?
			if (xIdx == (xNr + 1)) {
				xCurr = xCurr->getNext();
				PWX_LOCK(const_cast<list_t*>(this))
				curr(xCurr);
				PWX_UNLOCK(const_cast<list_t*>(this))
				return xCurr;
			}

			// Or the previous
			if (xIdx == (xNr - 1)) {
				xCurr = xCurr->getPrev();
				PWX_LOCK(const_cast<list_t*>(this))
				curr(xCurr);
				PWX_UNLOCK(const_cast<list_t*>(this))
				return xCurr;
			}

			// Is it the head we want?
			if (0 == xIdx)
				return head();

			// Or tail ?
			if ( (locCnt - 1) == xIdx)
				/* Note: Although, after unlocking, tail might be pointing
				 * somewhere else, the check is against locCnt to ensure
				 * that any call to index -1 retrieves the current tail.
				 */
				return tail();

			// Ok, let's go. But xCurr is already checked
			bool upwards = true;
			if (xIdx < xNr) {
				upwards = false;
				xCurr = xCurr->getPrev();
				--xNr;
			} else {
				xCurr = xCurr->getNext();
				++xNr;
			}

			// Now look into the rest
			while ( xCurr && (xNr != xIdx)) {

				/* There is one critical possibility here:
				 * If more than one element is removed while we traverse
				 * the list, it might happen that xIdx is suddenly beyond
				 * eCount or we reach head, first.
				 * If this happens xCurr and xIdx have to be "warped"
				 * around head and tail if xCurr reaches head or tail
				 */
				if (upwards && (tail() == xCurr)) {
					PWX_LOCK(const_cast<list_t*>(this))
					// Do a double check, maybe the warp is not needed any more
					if (tail() == xCurr) {
						xIdx -= eCount.load(PWX_MEMORDER_ACQUIRE);
						xCurr = head();
						xNr   = 0;
					}
					PWX_UNLOCK(const_cast<list_t*>(this))
				} else if (!upwards && (head() == xCurr)) {
					PWX_LOCK(const_cast<list_t*>(this))
					if (head() == xCurr) {
						xIdx += eCount.load(PWX_MEMORDER_ACQUIRE);
						xCurr = tail();
						xNr   = xCurr->eNr.load(PWX_MEMORDER_ACQUIRE);
					}
					PWX_UNLOCK(const_cast<list_t*>(this))
				} // End of consistency check
				else {
					if (upwards)
						xCurr = xCurr->getNext();
					else
						xCurr = xCurr->getPrev();
					xNr += upwards ? 1 : -1;
				}

			} // end of searching loop

			// xCurr is sure to be pointing where it should now.
			PWX_LOCK(const_cast<list_t*>(this))
			curr(xCurr);
			PWX_UNLOCK(const_cast<list_t*>(this))
			return xCurr;
		}

		return nullptr;
	}


	/// @brief preparation method to insert data before data
	virtual uint32_t privInsDataBeforeData(data_t* next, data_t* data)
	{
		// 1: Prepare the next element
		elem_t* nextElement = next ? const_cast<elem_t*>(protFind(next)) : nullptr;
		if (next && (nullptr == nextElement)) {
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this doubly linked list")
		}

		// Now nextElement must not change any more
		if (nextElement)
			PWX_LOCK(nextElement)

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			if (nextElement)
				PWX_UNLOCK(nextElement)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 3: Do the real insert
		elem_t* prev = nextElement ? nextElement->getPrev() : tail();
		if (nextElement)
			PWX_UNLOCK(nextElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
	}


	/// @brief preparation method to insert data before an element
	virtual uint32_t privInsDataBeforeElem(elem_t* next, data_t* data)
	{
		// 1: next, if set, must not change any more
		if (next)
			PWX_LOCK(next)

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			if (next)
				PWX_UNLOCK(next)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 3: Do the real insert
		elem_t* prev = next ? next->getPrev() : tail();
		if (next)
			PWX_UNLOCK(next)
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
	}


	/// @brief preparation method to insert an element copy before data
	virtual uint32_t privInsElemBeforeData(data_t* next, const elem_t &src)
	{
		// 1: Prepare the next element
		elem_t* nextElement = next ? const_cast<elem_t*>(protFind(next)) : nullptr;
		if (next && (nullptr == nextElement)) {
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this doubly linked list")
		}

		// Now nextElement must not change any more
		if (nextElement)
			PWX_LOCK(nextElement)

		// 2: Check source:
		PWX_LOCK(const_cast<elem_t*>(&src))

		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (nextElement)
				PWX_UNLOCK(nextElement)
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (src))
		catch(std::exception &e) {
			if (nextElement)
				PWX_UNLOCK(nextElement)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 4: Do the real insert
		elem_t* prev = nextElement ? nextElement->getPrev() : tail();
		if (nextElement)
			PWX_UNLOCK(nextElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
	}


	/// @brief preparation method to insert an element copy before an element
	virtual uint32_t privInsElemBeforeElem(elem_t* next, const elem_t &src)
	{
		// 1: next, if set must not change any more
		if (next)
			PWX_LOCK(next)

		// 2: Check source:
		PWX_LOCK(const_cast<elem_t*>(&src))

		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (next)
				PWX_UNLOCK(next)
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (src))
		catch(std::exception &e) {
			if (next)
				PWX_UNLOCK(next)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 4: Do the real insert
		elem_t* prev = next ? next->getPrev() : tail();
		if (next)
			PWX_UNLOCK(next)
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
	}


	/// @brief simple method to remove an element from the list.
	/// This method does *NOT* lock, a lock must be in place!
	virtual void privRemove (elem_t* elem) noexcept
	{
		// return at once if there is no element to remove
		if (!elem)
			return;

		/* The following scenarios are possible:
		 * 1: elem is head
		 * 2: elem is tail
		 * 3: elem is something else.
		*/
		if (head() == elem) {
			// Case 1
			head(head()->getNext());
			doRenumber.store(true, PWX_MEMORDER_RELEASE);
		} else if (tail() == elem)
			// Case 2:
			tail(tail()->getPrev());
		else
			doRenumber.store(true, PWX_MEMORDER_RELEASE);
		elem->remove();

		if (1 == eCount.fetch_sub(1)) {
			// The list is empty!
			curr(nullptr);
			head(nullptr);
			tail(nullptr);
		}
	}


	/// @brief simple wrapper to prepare the direct removal of data
	virtual elem_t* privRemoveData(data_t* data) noexcept
	{
		// Need a big lock, only one removal at a time!
		PWX_LOCK_GUARD(list_t, this)

		elem_t* toRemove = nullptr;

		if ( (nullptr == data) || (nullptr == (toRemove = find (data))) )
			return nullptr;

		privRemove (toRemove);

		return toRemove;
	}


	/// @brief simple wrapper to prepare the direct removal of an element
	virtual elem_t* privRemoveElem(elem_t* elem) noexcept
	{
		if (nullptr == elem)
			return nullptr;

		// Need a big lock, only one removal at a time!
		PWX_DOUBLE_LOCK(list_t, this, elem_t, elem)

		if (elem->destroyed())
			// If it is deleted, it can not be removed as it is already detached.
			// And more important: It will go away after the unlock!
			return nullptr;

		privRemove (elem);

		return elem;
	}


	/** @brief simple wrapper to prepare the removal of an element before data
	  * If @a next data can not be found, nothing happens and nullptr is returned.
	  * @return nullptr if @a next is held by the first element or the list is empty.
	**/
	virtual elem_t* privRemoveBeforeData(data_t* next) noexcept
	{
		// Need a big lock, only one removal at a time!
		PWX_LOCK_GUARD(list_t, this)

		elem_t* xNext = next ? find (next) : nullptr;
		elem_t* toRemove = xNext ? xNext->getPrev() : next ? nullptr : head();

		if (toRemove)
			privRemove (toRemove);

		return toRemove;
	}


	/** @brief simple wrapper to prepare the removal of an element before another element
	  * @return nullptr if @a next is the first element or the list is empty.
	**/
	virtual elem_t* privRemoveBeforeElem(elem_t* next) noexcept
	{
		// Need a big lock, only one removal at a time!
		PWX_LOCK_GUARD(list_t, this)

		elem_t* toRemove = next ? next->getPrev() : tail();
		if (toRemove)
			privRemove (toRemove);

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
  * @param[in] lhs reference of the list from which to substract.
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
