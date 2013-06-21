#pragma once
#ifndef PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED
#define PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED 1

/** @file TSingleList.h
  *
  * @brief Declaration of a basic template for singly linked lists
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

#include <pwx/base/VContainer.h>
#include <pwx/types/TSingleElement.h>

namespace pwx
{

/** @class TSingleList
  *
  * @brief Template to build singly linked lists of variable types
  *
  * The singly linked list is the most basic container. Elements of type
  * pwx::TSingleElement<T> are chained together using their next pointers.
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
template<typename data_t, typename elem_t = TSingleElement<data_t> >
class PWX_API TSingleList : public VContainer
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef VContainer                  base_t; //!< Base type of the list
	typedef TSingleList<data_t, elem_t> list_t; //!< Type of this list
	typedef typename elem_t::neighbor_t neighbor_t; //!< Type of elements neighbors, used for curr, head and tail.


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor initializes an empty list.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TSingleList (void (*destroy_) (data_t* data_)) noexcept :
		destroy (destroy_)
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor uses the default constructor to set the data
	  * destroy method to the null pointer.
	**/
	TSingleList() noexcept :
		list_t (nullptr)
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * If a new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference of the list to copy.
	**/
	TSingleList (const list_t &src) :
		base_t (src)
	{
		operator+=(src);
	}


	virtual ~TSingleList() noexcept;


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
		while (head()) {
			PWX_LOCK(this)
			if (head()) {
				toDelete = privRemoveAfterElement(nullptr);
				// Now that the element is removed, we do not
				// need to have a full lock any more
				PWX_UNLOCK(this)
				if (toDelete) {
					PWX_TRY(protDelete(toDelete))
					catch(...) { } // We can't do anything about that
				}
			} // end of having head after lock is acquired.
			else
				PWX_UNLOCK(this)
		} // end of while head
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
	  * Nothing happens if there is no element behind the element holding @a prev.
	  * If the deletion of the data throws an exception, it will be turned
	  * into a pwx CException and passed on.
	  *
	  * @param[in] prev the data the element that precedes the element to delete holds
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delNext (data_t* prev)
	{
		PWX_TRY_PWX_FURTHER(return protDelete(remNext(prev)))
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
	  * Nothing happens if there is no element behind the element holding @a prev.
	  * If the deletion of the data throws an exception, it will be turned
	  * into a pwx CException and passed on.
	  *
	  * @param[in] prev the element that precedes the element to delete
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delNextElem (elem_t* prev)
	{
		PWX_TRY_PWX_FURTHER(return protDelete(remNextElem(prev)))
	}


	/** @brief disable thread safety
	  *
	  * This method disables all thread safety measures.
	  *
	  * <B>Warning</B>: It is completely unchecked whether the
	  * container is used by more than one thread. If concurrent
	  * threads work with this container while this method is
	  * called, the outcome is unpredictable.<BR/>
	  * Further this method disables all locking mechanisms in
	  * all elements stored and all elements that are added
	  * after calling this method. Calling this method with a
	  * lot of elements stored is therefore rather costly!
	  */
	void disable_thread_safety() noexcept
	{
		PWX_LOCK(this)
		this->do_locking(false);
		elem_t* xCurr = head();
		do {
			if (xCurr) {
				xCurr->disable_thread_safety();
				xCurr = xCurr->getNext();
			}
		} while (xCurr && xCurr != tail());
		this->beThreadSafe.store(false, PWX_MEMORDER_RELEASE);
		PWX_UNLOCK(this) // Just for the record
	}


	/// @brief return true if this container is empty
	virtual bool empty() const noexcept
	{
		return !eCount.load(PWX_MEMORDER_ACQUIRE);
	}


	/** @brief enable thread safety
	  *
	  * This method enables all thread safety measures.
	  *
	  * <B>Warning</B>: This method enables all locking
	  * mechanisms in all elements stored and all elements
	  * that are added after calling this method. Calling
	  * this method with a lot of elements stored is
	  * therefore rather costly!
	  */
	void enable_thread_safety() noexcept
	{
		this->do_locking(true);
		elem_t* xCurr = head();
		do {
			if (xCurr) {
				xCurr->enable_thread_safety();
				xCurr = xCurr->getNext();
			}
		} while (xCurr && xCurr != tail());
		this->beThreadSafe.store(true, PWX_MEMORDER_RELEASE);
	}


	/** @brief find the element with the given @a data
	  *
	  * This method searches through the list and returns the element
	  * with the given @a data or nullptr if @a data is not stored in this
	  * list.
	  *
	  * @param data pointer to the data to find
	  * @return return a pointer to the element storing @a data
	**/
	virtual elem_t* find (data_t* data) noexcept
	{
		return const_cast<elem_t* > (protFind (static_cast<const data_t* > (data)));
	}


	/** @brief find the element with the given @a data
	  *
	  * This method searches through the list and returns a const pointer
	  * to the element with the given @a data or nullptr if @a data is not stored
	  * in this list.
	  *
	  * @param data pointer to the data to find
	  * @return return a const pointer to the element storing @a data
	**/
	virtual const elem_t* find (const data_t* data) const noexcept
	{
		return protFind(data);
	}


	/** @brief return a read-only pointer to the element with the given @a index
	  *
	  * This method retrieves an element by index like an array. The pointer given
	  * back is read-only.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * If the list is empty, the method returns nullptr.
	  *
	  * If you use this method to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read-only pointer to the element, or nullptr if the list is empty.
	**/
	virtual const elem_t* get (const int32_t index) const noexcept
	{
		return privGetElementByIndex (index);
	}


	/** @brief return a read/write pointer to the element with the given @a index
	  *
	  * This method retrieves an element by index like an array. The pointer given
	  * back is write enabled, so use with care.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * If the list is empty, the method returns nullptr.
	  *
	  * If you use this method to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read/write pointer to the element, or nullptr if the list is empty.
	**/
	virtual elem_t* get (int32_t index) noexcept
	{
		return const_cast<elem_t* > (privGetElementByIndex (static_cast<const int32_t> (index)));
	}


	/** @brief return a read-only reference to the data of the element with the given @a index
	  *
	  * This method retrieves an element by index like an array and returns a reference
	  * to the stored data. The reference given back is read-only.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * If the list is empty, the function throws a pwx::CException with the name
	  * "OutOfRange".
	  *
	  * If you use this method to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read-only reference to the elements data.
	**/
	virtual const data_t &getData (const int32_t index) const
	{
		PWX_LOCK_GUARD(list_t, const_cast<list_t*>(this))
		// Note: The guard is needed or another thread can
		// delete the retrieved element between the retrieval
		// and the delivery. A segfault would be the result.
		const elem_t *elem = privGetElementByIndex (index);
		if (nullptr == elem) {
			PWX_THROW("OutOfRange", "The list is empty", "getData() used on an empty list.")
		}
		return **elem;
	}


	/** @brief return a read/write reference to the data of the element with the given @a index
	  *
	  * This method retrieves an element by index like an array and returns a reference
	  * to the stored data. The reference given back is write enabled, so use with care.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * If the list is empty, the function throws a pwx::CException with the name
	  * "OutOfRange".
	  *
	  * If you use this method to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read/write reference to the elements data.
	**/
	virtual data_t &getData (int32_t index)
	{
		PWX_TRY_PWX_FURTHER(return const_cast<data_t&>(getData(static_cast<const int32_t>(index))))
	}


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
		PWX_TRY_PWX_FURTHER(return privInsDataBehindData(prev, data))
	}


	/** @brief insert an element copy after the specified data
	  *
	  * This method inserts a new element in the list after the element
	  * holding @a prev that is a copy from element @a src.
	  *
	  * If @a prev is set to nullptr, the new element will become the new
	  * head of the list.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] prev the data the element that should precede the new element holds.
	  * @param[in] src a reference of the element to copy.
	  * @return the number of elements in this list after the insertion.
	**/
	virtual uint32_t insNext (data_t* prev, const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER(return privInsElemBehindData(prev, src))
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
		PWX_TRY_PWX_FURTHER(return privInsDataBehindElem(prev, data))
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
	  * @param[in] src reference of the lement to copy.
	  * @return the number of elements in this list after the insertion.
	**/
	virtual uint32_t insNextElem (elem_t* prev, const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER(return privInsElemBehindElem(prev, src))
	}


	/** @brief short alias for pop_front()
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delNext instead.
	  *
	  * If the list is empty, nullptr is returned.
	  *
	  * @return a pointer to the removed element or nullptr if the list is empty
	**/
	virtual elem_t* pop() noexcept
	{
		return pop_front();
	}


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
		PWX_LOCK_GUARD(list_t, this)
		// Note: The guard is needed to ensure that no thread changes the
		// number of elements beyond the border of eCount > 1
		return (eCount.load(PWX_MEMORDER_ACQUIRE) > 1
				? privRemoveAfterElement(const_cast<elem_t* > (privGetElementByIndex (-2)))
				: privRemoveAfterData(nullptr));
	}


	/** @brief alias to remove the first element (head)
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
		return privRemoveAfterData(nullptr);
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
		PWX_TRY_PWX_FURTHER (return push_back (data))
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
		PWX_TRY_PWX_FURTHER (return push_back (src))
	}


	/** @brief alias to add a data pointer to the end of the list.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_back (data_t *data)
	{
		PWX_TRY_PWX_FURTHER (return privInsDataBehindElem (tail(), data))
	}


	/** @brief alias to add an element copy to the end of the list.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_back (const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (return privInsElemBehindElem (tail(), src))
	}


	/** @brief alias to add a data pointer to the head of the list.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_front (data_t *data)
	{
		PWX_TRY_PWX_FURTHER (return privInsDataBehindElem (nullptr, data))
	}


	/** @brief alias to add an element copy to the head of the list.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_front (const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER (return privInsElemBehindElem (nullptr, src))
	}


	/** @brief remove the element after the element holding the specified data
	  *
	  * This method removes the element in the list after the element
	  * that holds @a prev and returns a pointer to the removed element.
	  *
	  * If @a prev is set to nullptr, the root element (aka head) is
	  * removed.
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use delNext instead.
	  *
	  * If there is no element behind the element @a prev or if the list is
	  * empty, nullptr is returned.
	  *
	  * @param[in] prev the data the element that precedes the element to remove holds
	  * @return a pointer to the removed element or nullptr if @a prev is held by the last element or the list is empty
	**/
	virtual elem_t* remNext (data_t* prev) noexcept
	{
		return privRemoveAfterData(prev);
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
	  * empty, nullptr is returned.
	  *
	  * @param[in] prev the element that precedes the element to remove
	  * @return a pointer to the removed element or nullptr if the list is empty or @a prev is the last element.
	**/
	virtual elem_t* remNextElem (elem_t* prev) noexcept
	{
		return privRemoveAfterElement(prev);
	}


	/// @brief return the number of stored elements
	uint32_t size() const noexcept
	{
		return this->beThreadSafe.load(PWX_MEMORDER_RELAXED)
			? eCount.load(PWX_MEMORDER_ACQUIRE)
			: eCount.load(PWX_MEMORDER_RELAXED);
	}


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief assignment operator
	  *
	  * Clears this list and copies all elements from @a rhs
	  * into this list. Both the destroy method (if any) and
	  * the setting whether the container is using its thread
	  * safety measures is copied as well.
	  *
	  * FIXME: An exclusive lock for [b]both[/b] lists is required?
	  *
	  * @param[in] rhs reference of the list to copy.
	  * @return reference to this.
	**/
	virtual list_t &operator= (const list_t & rhs)
	{
		if (&rhs != this) {
			PWX_DOUBLE_LOCK (list_t, this, list_t, const_cast<list_t*> (&rhs))
			clear();
			destroy      = rhs.destroy;
			beThreadSafe = rhs.beThreadSafe.load(PWX_MEMORDER_RELAXED);
			PWX_TRY_PWX_FURTHER (return operator+=(rhs))
		}
		return *this;
	}


	/** @brief addition assignment operator
	  *
	  * Add all elements from @a rhs to this list.
	  *
	  * FIXME: An exclusive lock for [b]both[/b] lists is required?
	  *
	  * @param[in] rhs reference of the list to add.
	  * @return reference to this.
	**/
	virtual list_t &operator+= (const list_t & rhs)
	{
		if (&rhs != this) {
			PWX_DOUBLE_LOCK (list_t, this, list_t, const_cast<list_t*> (&rhs))
			elem_t* rhsCurr = rhs.head();
			bool    isDone  = false;
			bool    isTS    = this->beThreadSafe.load(PWX_MEMORDER_ACQUIRE);

			while (rhsCurr && !isDone) {
				PWX_TRY_PWX_FURTHER (privInsElemBehindElem (tail(), *rhsCurr))
				if (!isTS)
					tail()->disable_thread_safety();
				if (rhsCurr == rhs.tail())
					isDone = true;
				else
					rhsCurr = rhsCurr->getNext();
			}
		}
		return *this;
	}


	/** @brief substraction assignment operator
	  *
	  * Remove all elements from @a rhs from this list.
	  *
	  * FIXME: An exclusive lock for [b]both[/b] lists is required?
	  *
	  * @param[in] rhs reference of the list to substract.
	  * @return reference to this.
	**/
	virtual list_t &operator-= (const list_t & rhs)
	{
		if (&rhs != this) {
			PWX_DOUBLE_LOCK (list_t, this, list_t, const_cast<list_t*> (&rhs))
			elem_t* rhsCurr = rhs.head();
			elem_t* lhsPrev = nullptr;
			data_t* rhsData = nullptr;
			bool    isDone  = false;

			while (rhsCurr && !isDone) {
				rhsData = rhsCurr->data.get();

				// Head must be treated first, privFindPrev won't help.
				if (rhsData == head()->data.get())
					PWX_TRY_PWX_FURTHER(protDelete(remNextElem(nullptr)))
				else {
					lhsPrev = privFindPrev(rhsData);
					if (lhsPrev)
						PWX_TRY_PWX_FURTHER(protDelete(remNextElem(lhsPrev)))
				}

				if (rhsCurr == rhs.tail())
					isDone = true;
				else
					rhsCurr = rhsCurr->getNext();
			}

		} else {
			PWX_LOCK_GUARD(list_t, this)
			// Here we lock before calling clear, or the operator might
			// end up removing elements other threads just inserted.
			clear();
		}

		return *this;
	}


	/** @brief return a read-only pointer to the element with the given @a index
	  *
	  * This operator retrieves an element by index like an array. The pointer given
	  * back is read-only.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * If the list is empty, the operator returns nullptr.
	  *
	  * If you use this operator to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read-only pointer to the element, or nullptr if the list is empty.
	**/
	virtual const elem_t* operator[] (const int32_t index) const noexcept
	{
		return privGetElementByIndex (index);
	}


	/** @brief return a read/write pointer to the element with the given @a index
	  *
	  * This operator retrieves an element by index like an array. The pointer given
	  * back is write enabled, so use with care.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * If the list is empty, the operator returns nullptr.
	  *
	  * If you use this operator to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read/write pointer to the element, or nullptr if the list is empty.
	**/
	virtual elem_t* operator[] (int32_t index) noexcept
	{
		return const_cast<elem_t* > (privGetElementByIndex (static_cast<const int32_t> (index)));
	}


protected:
	/* ===============================================
	 * === Protected methods                       ===
	 * ===============================================
	*/

	void (*destroy) (data_t* data_);

	// accessors for curr/head/tail

	/// @brief return curr according to thread safety setting
	elem_t* curr() const
	{
		if (this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			return curr_.load(PWX_MEMORDER_ACQUIRE);
		else
			return curr_.load(PWX_MEMORDER_RELAXED);
	}


	/// @brief set curr to @a new_curr according to thread safety settings
	void curr(elem_t* new_curr) const
	{
		if (this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			curr_.store(new_curr, PWX_MEMORDER_RELEASE);
		else
			curr_.store(new_curr, PWX_MEMORDER_RELAXED);
	}


	/// @brief set curr to @a new_curr according to thread safety settings
	void curr(elem_t* new_curr)
	{
		if (this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			curr_.store(new_curr, PWX_MEMORDER_RELEASE);
		else
			curr_.store(new_curr, PWX_MEMORDER_RELAXED);
	}


	/// @brief return head according to thread safety setting
	elem_t* head() const
	{
		if (this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			return head_.load(PWX_MEMORDER_ACQUIRE);
		else
			return head_.load(PWX_MEMORDER_RELAXED);
	}


	/// @brief set head to @a new_head according to thread safety settings
	void head(elem_t* new_head)
	{
		if (this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			head_.store(new_head, PWX_MEMORDER_RELEASE);
		else
			head_.store(new_head, PWX_MEMORDER_RELAXED);
	}


	/// @brief return tail according to thread safety setting
	elem_t* tail() const
	{
		if (this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			return tail_.load(PWX_MEMORDER_ACQUIRE);
		else
			return tail_.load(PWX_MEMORDER_RELAXED);
	}


	/// @brief set tail to @a new_tail according to thread safety settings
	void tail(elem_t* new_tail)
	{
		if (this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			tail_.store(new_tail, PWX_MEMORDER_RELEASE);
		else
			tail_.store(new_tail, PWX_MEMORDER_RELAXED);
	}


	// Overridable protected methods

	/** @brief Delete the element @a removed
	  *
	  * <B>Important</B>: this method will throw "illegal_delete" if
	  * @a removed is actually not removed from the list. Making sure
	  * this method is only called with a removed element allows to
	  * use it without a lock on the list itself.
	**/
	virtual uint32_t protDelete(elem_t* removed)
	{
		if (removed && removed->inserted())
			PWX_THROW("illegal_delete", "delete non-removed element", "Deleting an element that is not removed is illegal!")

		if (removed) {
			// Use double check technique to avoid illegal double deletion
			if (!removed->destroyed()) {
				PWX_LOCK(removed)
				if (!removed->destroyed()) {
					PWX_UNLOCK(removed)
					PWX_TRY(delete removed)
					PWX_THROW_PWXSTD_FURTHER ("delete",
						"Deleting an element failed.")
				} else
					PWX_UNLOCK(removed)
			} // end of if not destroyed yet
		} // end of having an element to delete
		return eCount.load(PWX_MEMORDER_ACQUIRE);
	}


	/// @brief Search until the current element contains the searched data
	virtual const elem_t* protFind (const data_t* data) const noexcept
	{
		elem_t* result = nullptr;

		// Return at once if the list is empty
		if (empty())
			return nullptr;

		PWX_LOCK(const_cast<list_t*>(this))

		// Exit if curr is nullptr (list emptied while we waited for the lock)
		if (nullptr == curr()) {
			PWX_UNLOCK(const_cast<list_t*>(this))
			return nullptr;
		}

		// Quick exit if curr is already what we want:
		if (curr()->data.get() == data) {
			// Safe curr first, so it isn't changed before being returned
			result = curr();
			PWX_UNLOCK(const_cast<list_t*>(this))
			return result;
		}

		// The next does only make sense if we have more than one element
		if (eCount.load(PWX_MEMORDER_ACQUIRE) > 1) {

			// Exit if head is wanted...
			if (head()->data.get() == data) {
				curr(head());
				result = curr();
				PWX_UNLOCK(const_cast<list_t*>(this))
				return result;
			}

			// ...or tail
			if (tail()->data.get() == data) {
				curr(tail());
				result = curr();
				PWX_UNLOCK(const_cast<list_t*>(this))
				return result;
			}

			// Otherwise we have to search for it.
			elem_t* xCurr  = head()->getNext(); // head is already checked.
			bool    isDone = false;
			if (this->beThreadSafe.load(PWX_MEMORDER_ACQUIRE)) {
				PWX_UNLOCK(const_cast<list_t*>(this))

				while (!result && !isDone && xCurr) {
					if (xCurr->data.get() == data)
						result = xCurr;
					else if (xCurr == tail())
						isDone = true;
					else
						xCurr = xCurr->getNext();
				}
			} else {
				// No need to unlock, PWX_LOCK hasn't done anything anyway.
				// Further tail() is already checked and we don't assume
				// it to change while the search is running.
				while (!result && xCurr && (xCurr != tail() )) {
					if (xCurr->data.get() == data)
						result = xCurr;
					else
						xCurr = xCurr->next.load(PWX_MEMORDER_RELAXED);
				}
			} // End of manual traversing the container

		} // End of handling a search with more than one element
		else
			PWX_UNLOCK(const_cast<list_t*>(this))

		return result;
	}


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
			DEBUG_LOCK_STATE("insertNext", insPrev, insElem)
			PWX_UNLOCK(this)
			PWX_TRY_PWX_FURTHER(insPrev->insertNext(insElem))
		} else {
			PWX_LOCK_GUARD(list_t, this)
			PWX_UNLOCK(this)
			if (!locCnt) {
				// Case 1: The list is empty
				head(insElem);
				tail(insElem);
				DEBUG_LOCK_STATE("insertBefore", insElem, insElem)
				PWX_TRY_PWX_FURTHER(insElem->insertBefore(nullptr))
			} else if (nullptr == insPrev) {
				// Case 2: A new head is to be set
				DEBUG_LOCK_STATE("insertBefore", insElem, head())
				PWX_TRY_PWX_FURTHER(insElem->insertBefore(head() ))
				head(insElem);
				doRenumber.store(true, PWX_MEMORDER_RELEASE);
			} else if (insPrev == tail() ) {
				// Case 3: A new tail is to be set
				insElem->eNr.store(
					tail()->eNr.load(PWX_MEMORDER_ACQUIRE) + 1,
					PWX_MEMORDER_RELEASE);
				DEBUG_LOCK_STATE("insertNext", tail(), insElem)
				PWX_TRY_PWX_FURTHER(tail()->insertNext(insElem))
				tail(insElem);
			}
			DEBUG_LOCK_STATE("lock_guard_dtor", this, this)
		}

		eCount.fetch_add(1, PWX_MEMORDER_RELEASE);
		return eCount.load(PWX_MEMORDER_ACQUIRE);
	}


	/// @brief renumber all elements
	virtual void protRenumber() const noexcept
	{
		// Now do the renumbering if it is necessary:
		if (doRenumber.load(PWX_MEMORDER_ACQUIRE)) {

			// Do a big lock, so multiple threads calling this function
			// won't renumber multiple times when once is enough.
			PWX_LOCK_GUARD(list_t, const_cast<list_t*>(this))

			// Check again, maybe this does not need any renumbering any more:
			if (!doRenumber.load(PWX_MEMORDER_ACQUIRE))
				return;

			elem_t*  xCurr  = head();
			uint32_t xNr    = 0;
			bool     isDone = false;

			while (xCurr && !isDone) {
				xCurr->eNr.store(xNr++, PWX_MEMORDER_RELEASE);
				if (xCurr == tail() )
					isDone = true;
				else
					xCurr = xCurr->next;
			}

			doRenumber.store(false, PWX_MEMORDER_RELEASE);
		}
	}


private:
	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	/* Important: Public methods should not lock unless it is critical.
	 *            All necessary checks are done by the public methods,
	 *            work is done by the private methods which will lock
	 *            where appropriate.
	*/

	/// @brief Search until the next element contains the searched data
	// Note: This method must be invoked with a lock in place! It does *NOT* lock!
	virtual elem_t* privFindPrev (const data_t* data) const noexcept
	{
		elem_t*  xPrev = head();
		elem_t*  xCurr = xPrev->getNext();

		while (xPrev && (xPrev != tail() )) {
			if (xCurr->data.get() == data) {
				curr(xCurr);
				return xPrev;
			}
			xPrev = xCurr;
			xCurr = xCurr->getNext();
		}

		// If we are here, prev points to tail. No match found.
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
				curr(xCurr);
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

			// Ok, let's go. But only start from head if we currently are beyond.
			if (xIdx < xNr) {
				xCurr = head()->getNext();
				xNr  = 1;
			}

			// Otherwise the next of xCurr is already checked, so skip it
			else {
				xCurr = xCurr->getNext();
				++xNr;
			}

			// Now look into the rest
			while ( xCurr && (xNr < xIdx)) {

				/* There is one critical possibility here:
				 * If more than one element is removed while we traverse
				 * the list, it might happen that xIdx is suddenly beyond
				 * eCount. If this happens xCurr and xIdx have to be "warped"
				 * around tail -> head if xCurr reaches tail
				 */
				if (tail() == xCurr) {
					PWX_LOCK(const_cast<list_t*>(this))
					// Do a double check, maybe the warp is not needed any more
					if (tail() == xCurr) {
						xIdx -= eCount.load(PWX_MEMORDER_ACQUIRE);
						xCurr = head();
						xNr   = 0;
					}
					PWX_UNLOCK(const_cast<list_t*>(this))
				} // End of consistency check
				else {
					xCurr = xCurr->getNext();
					++xNr;
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


	/// @brief preparation method to insert data behind data
	virtual uint32_t privInsDataBehindData(data_t* prev, data_t* data)
	{
		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (prev && (nullptr == prevElement)) {
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this singly linked list")
		}

		// Now prevElement must not change any more
		if (prevElement)
			PWX_LOCK(prevElement)

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 3: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert data behind an element
	virtual uint32_t privInsDataBehindElem(elem_t* prev, data_t* data)
	{
		// 1: prev, if set, must not change any more
		if (prev)
			PWX_LOCK(prev)

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			if (prev)
				PWX_UNLOCK(prev)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 3: Do the real insert
		if (prev)
			PWX_UNLOCK(prev)
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
	}


	/// @brief preparation method to insert an element copy behind data
	virtual uint32_t privInsElemBehindData(data_t* prev, const elem_t &src)
	{
		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (prev && (nullptr == prevElement)) {
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this singly linked list")
		}

		// Now prevElement must not change any more
		if (prevElement)
			PWX_LOCK(prevElement)

		// 2: Check source:
		PWX_LOCK(const_cast<elem_t*>(&src))

		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (src))
		catch(std::exception &e) {
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		PWX_UNLOCK(const_cast<elem_t*>(&src))
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 4: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind an element
	virtual uint32_t privInsElemBehindElem(elem_t* prev, const elem_t &src)
	{
		// 1: prev, if set, must not change any more
		if (prev)
			PWX_LOCK(prev)

		// 2: Check source:
		PWX_LOCK(const_cast<elem_t*>(&src))

		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (prev)
				PWX_UNLOCK(prev)
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (src))
		catch(std::exception &e) {
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (prev)
				PWX_UNLOCK(prev)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		PWX_UNLOCK(const_cast<elem_t*>(&src))
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 4: Do the real insert
		if (prev)
			PWX_UNLOCK(prev)
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
	}


	/// @brief simple method to remove an element from the list.
	/// This method does *NOT* lock, a lock must be in place!
	virtual void privRemove (elem_t* prev, elem_t* elem) noexcept
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
			elem->remove();
			doRenumber.store(true, PWX_MEMORDER_RELEASE);
		} else {
			if (tail() == elem)
				// Case 2:
				tail(prev);
			PWX_TRY_PWX_FURTHER(prev->removeNext())
		}

		if (1 == eCount.fetch_sub(1)) {
			// The list is empty!
			curr(nullptr);
			head(nullptr);
			tail(nullptr);
		}
	}


	/** @brief remove the element after the specified data
	  * If @a prev data can not be found, nothing happens and nullptr is returned.
	  * @return nullptr if the element holding @a prev is the last element or the list is empty.
	**/
	virtual elem_t* privRemoveAfterData(data_t* prev) noexcept
	{
		// Need a big lock, only one removal at a time!
		PWX_LOCK_GUARD(list_t, this)

		elem_t* xPrev    = prev ? const_cast<elem_t*>(protFind (prev)) : nullptr;
		elem_t* toRemove = xPrev ? xPrev->getNext() : prev ? nullptr : head();

		if (toRemove)
			privRemove (xPrev, toRemove);

		return toRemove;
	}


	/** @brief remove the element after the specified element
	  * @return nullptr if prev->next is nullptr or the list is empty
	**/
	virtual elem_t* privRemoveAfterElement(elem_t* prev) noexcept
	{
		// Need a big lock, only one removal at a time!
		PWX_LOCK_GUARD(list_t, this)

		elem_t* toRemove = prev ? prev->getNext() : head();
		if (toRemove)
			privRemove (prev, toRemove);

		return toRemove;
	}


	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	mutable
	neighbor_t curr_ = ATOMIC_VAR_INIT(nullptr); //!< pointer to the currently handled element
	neighbor_t head_ = ATOMIC_VAR_INIT(nullptr); //!< pointer to the first element
	neighbor_t tail_ = ATOMIC_VAR_INIT(nullptr); //!< pointer to the last element
}; // class TSingleList


/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the list.
**/
template<typename data_t, typename elem_t>
TSingleList<data_t, elem_t>::~TSingleList() noexcept
{
	clear();
}


/** @brief addition operator
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
TSingleList<data_t, elem_t> operator+ (const TSingleList<data_t, elem_t> &lhs, const TSingleList<data_t, elem_t> &rhs)
{
	TSingleList<data_t, elem_t> result(lhs);

	if (&lhs != &rhs)
		PWX_TRY_PWX_FURTHER(result += rhs)

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
TSingleList<data_t, elem_t> operator- (const TSingleList<data_t, elem_t> &lhs, const TSingleList<data_t, elem_t> &rhs)
{
	 TSingleList<data_t, elem_t> result(lhs);

	if (&lhs != &rhs) {
		PWX_TRY_PWX_FURTHER(result -= rhs)
	} else
		result.clear();

	return result;
}

} // namespace pwx

#endif // PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED

