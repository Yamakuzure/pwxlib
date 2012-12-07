#pragma once
#ifndef PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED
#define PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED 1

/** @file TSingleList.h
  *
  * @brief Declaration of a basic template for singly linked lists
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

#include "pwx/base/VContainer.h"
#include "pwx/types/TSingleElement.h"

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
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * If PWX_THREADS is defined, changes to the element are done in a locked state.
**/
template<typename data_t, typename elem_t = TSingleElement<data_t> >
class PWX_API TSingleList : public VContainer
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef VContainer                  base_t;
	typedef TSingleList<data_t, elem_t> list_t;


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
		PWX_LOCK_NOEXCEPT(this)
		while (head) {
			try {
#ifdef PWX_THREADS
				elem_t* toDelete = head;
				if (toDelete && !toDelete->destroyed()) {
					privRemove(nullptr, toDelete);
					PWX_UNLOCK_NOEXCEPT(this)
					privDelete(toDelete);
				} else {
					PWX_UNLOCK_NOEXCEPT(this)
					std::this_thread::yield();
				}
#else
				privDelete(remNext(nullptr));
#endif // PWX_THREADS
			}
			PWX_CATCH_AND_FORGET(CException)
			PWX_LOCK_NOEXCEPT(this)
		}
		PWX_UNLOCK_NOEXCEPT(this)
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
		PWX_TRY_PWX_FURTHER(return privDelete(remNext(prev)))
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
		PWX_TRY_PWX_FURTHER(return privDelete(remNextElem(prev)))
	}


	/// @brief return true if this container is empty
	virtual bool empty() const noexcept
	{
		PWX_LOCK_GUARD(list_t, const_cast<list_t*>(this))
		return (nullptr == head);
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
		uint32_t localCount = size();
		if (localCount > 1) {
			PWX_TRY (return remNextElem (const_cast<elem_t* > (privGetElementByIndex (-2))))
			PWX_CATCH_AND_FORGET (CException)
		}
		else if (localCount) {
			PWX_TRY (return remNext (nullptr))
			PWX_CATCH_AND_FORGET (CException)
		}
		return nullptr;
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
		uint32_t localCount = size();
		if (localCount) {
			PWX_TRY (return remNext (nullptr))
			PWX_CATCH_AND_FORGET (CException)
		}
		return nullptr;
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
		PWX_LOCK(this)
		elem_t* xTail = tail;
		PWX_UNLOCK(this)
		PWX_TRY_PWX_FURTHER (return privInsDataBehindElem (xTail, data))
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
		PWX_LOCK(this)
		elem_t* xTail = tail;
		PWX_UNLOCK(this)
		PWX_TRY_PWX_FURTHER (return privInsElemBehindElem (xTail, src))
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
	  * If there is no element behind the element @a prev a
	  * pwx::CException with the name "OutOfRange" is thrown.
	  *
	  * @param[in] prev the data the element that precedes the element to remove holds
	  * @return a pointer to the removed element
	**/
	virtual elem_t* remNext (data_t* prev)
	{
		PWX_TRY_PWX_FURTHER(return privRemoveAfterData(prev))
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
	  * empty, a pwx::CException with the name "OutOfRange" is thrown.
	  *
	  * @param[in] prev the element that precedes the element to remove
	  * @return a pointer to the removed element
	**/
	virtual elem_t* remNextElem (elem_t* prev)
	{
		PWX_TRY_PWX_FURTHER(return privRemoveAfterElement(prev))
	}


	/// @brief return the number of stored elements
	uint32_t size() const noexcept
	{
		PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this));
		uint32_t currSize = eCount;
		PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this));
		return currSize;
	}


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief assignment operator
	  *
	  * Clears this list and copies all elements from @a rhs
	  * into this list.
	  *
	  * If PWX_THREADS is defined, an exclusive lock for [b]both[/b]
	  * lists is required!
	  *
	  * @param[in] rhs reference of the list to copy.
	  * @return reference to this.
	**/
	virtual list_t &operator= (const list_t & rhs)
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
	  * Add all elements from @a rhs to this list.
	  *
	  * If PWX_THREADS is defined, an exclusive lock for [b]both[/b]
	  * lists is required!
	  *
	  * @param[in] rhs reference of the list to add.
	  * @return reference to this.
	**/
	virtual list_t &operator+= (const list_t & rhs)
	{
		if (&rhs != this) {
			PWX_DOUBLE_LOCK (list_t, this, list_t, const_cast<list_t*> (&rhs))
			elem_t* rhsCurr = rhs.head;
			bool    isDone  = false;

			while (rhsCurr && !isDone) {
				PWX_TRY_PWX_FURTHER (insNextElem (tail, *rhsCurr))
				if (rhsCurr == rhs.tail)
					isDone = true;
				else
					rhsCurr = rhsCurr->next;
			}
		}
		return *this;
	}


	/** @brief substraction assignment operator
	  *
	  * Remove all elements from @a rhs from this list.
	  *
	  * If PWX_THREADS is defined, an exclusive lock for [b]both[/b]
	  * lists is required!
	  *
	  * @param[in] rhs reference of the list to substract.
	  * @return reference to this.
	**/
	virtual list_t &operator-= (const list_t & rhs)
	{
		if (&rhs != this) {
			PWX_DOUBLE_LOCK (list_t, this, list_t, const_cast<list_t*> (&rhs))
			elem_t* rhsCurr = rhs.head;
			elem_t* lhsPrev = nullptr;
			data_t* rhsData = nullptr;
			bool    isDone  = false;

			while (rhsCurr && !isDone) {
				rhsData = rhsCurr->data.get();

				// Head must be treated first, privFindPrev won't help.
				if (rhsData == head->data.get())
					PWX_TRY_PWX_FURTHER(delNextElem(nullptr))
				else {
					lhsPrev = privFindPrev(rhsData);
					if (lhsPrev)
						PWX_TRY_PWX_FURTHER(delNextElem(lhsPrev))
				}

				if (rhsCurr == rhs.tail)
					isDone = true;
				else
					rhsCurr = rhsCurr->next;
			}

		} else
			clear();
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


	/// @brief Search until the current element contains the searched data
	virtual const elem_t* protFind (const data_t* data) const noexcept
	{
		uint32_t localCount = size();

		// Return at once if the list is empty
		if (!localCount)
			return nullptr;

		PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
		// Use local values of *now*, no big lock needed, then.
		elem_t*  xCurr      = curr;
		PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))

		if (nullptr == xCurr)
			return nullptr;

		// Quick exit if xCurr is already what we want:
		if (xCurr->data.get() == data)
			return xCurr;

		// The next does only make sense if we have more than one element
		if (localCount > 1) {
			// Exit if head is wanted...
			PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
			if (head->data.get() == data) {
				curr = head;
				return head;
			}

			// ...or tail
			if (tail->data.get() == data) {
				curr = tail;
				return tail;
			}

			// Otherwise search for the previous item, it's the next, then
			PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))
			elem_t* prev = privFindPrev (data);
			if (prev)
				return prev->next;
		} // End of handling a search with more than one element

		return nullptr;
	}


	/// @brief simple method to insert an element into the list
	virtual uint32_t protInsert (elem_t* insPrev, elem_t* insElem) noexcept
	{
		uint32_t localCount   = size();
		bool     needRenumber = true;

		// Now the real insertion can be done:
		PWX_LOCK_NOEXCEPT(this)
		if (insPrev) {
			if (tail == insPrev) {
				// In this case we do not need a full renumbering
				needRenumber = false;
				insElem->eNr = tail->eNr + 1;
				tail = insElem;
			}
			insElem->next = insPrev->next;
			insPrev->next = insElem;
		} else if (localCount) {
			insElem->next = head;
			head = insElem;
		} else {
			// If we had no elements yet, head and tail need to be set:
			head = insElem;
			tail = insElem;
		}

		// Set curr and renumber the list
		curr       = insElem;
		localCount = ++eCount;
		if (needRenumber)
			doRenumber = true;
		PWX_UNLOCK_NOEXCEPT(this)

		return localCount;
	}


	/// @brief renumber all elements
	virtual void protRenumber() const noexcept
	{
		if (doRenumber) {
			PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
			elem_t*  xCurr  = head;
			uint32_t xNr    = 0;
			bool     isDone = false;

			while (xCurr && !isDone) {
				xCurr->eNr = xNr++;
				if (xCurr == tail)
					isDone = true;
				else
					xCurr = xCurr->next;
			}

			doRenumber = false;
			PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))
		}
	}

	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	mutable
	elem_t*  curr   = nullptr;   //!< pointer to the currently handled element
	elem_t*  head   = nullptr;   //!< pointer to the first element
	elem_t*  tail   = nullptr;   //!< pointer to the last element


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

	/// @brief Delete the element @a removed
	virtual uint32_t privDelete(elem_t* removed)
	{
		try {
			if (removed) {
				PWX_LOCK(removed)
				if (!removed->destroyed())
					// Note: The dtor of CLockable will remove all
					// locks imposed by this thread.
					delete removed;
				else
					PWX_UNLOCK(removed)
			}
			return size();
		}
		PWX_THROW_PWXSTD_FURTHER ("delete", "Deleting an element in TSingleList::privDelete() failed.")
	}


	/// @brief Search until the next element contains the searched data
	virtual elem_t* privFindPrev (const data_t* data) const noexcept
	{
		PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
		elem_t*  xPrev = head;
		elem_t*  xTail = tail;
		PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))

		elem_t*  xCurr = xPrev->next;

		while (xPrev && (xPrev != xTail)) {
			if (xCurr->data.get() == data) {
				PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
				curr = xCurr;
				PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))
				return xPrev;
			}
			xPrev = xCurr;
			xCurr = xCurr->next;
		}

		// If we are here, prev points to tail. No match found.
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
				xIdx %= localCount;

			PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
			elem_t*  xHead = head;
			elem_t*  xTail = tail;
			elem_t*  xCurr = curr;
			/* Note: It is important to use a local number. If the current
			 * number of xCurr is used, wandering the list might _jump_ the
			 * asked for index if another thread deletes an element that
			 * resided before the currently investigated element.
			 * Further the question is which element has the given index at
			 * the moment this method begins, not when it ends.
			*/
			uint32_t xNr   = xCurr->eNr;
			PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))

			// Is xCurr already correct?
			if (xIdx == xNr)
				return xCurr;

			// Is xIdx the next member, like in a for loop?
			if (xIdx == (xNr + 1)) {
				xCurr = xCurr->next;
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

			// Ok, let's go. But only start from head if we currently are beyond.
			if (xIdx < xNr) {
				xCurr = xHead->next;
				xNr  = 1;
			}

			// Otherwise the next of xCurr is already checked, so skip it
			else {
				xCurr = xCurr->next;
				++xNr;
			}

			// Now look into the rest
			while ( xCurr && (xNr < xIdx) && (xNr < (localCount - 1))) {
				xCurr = xCurr->next;
				++xNr;

				// if Another thread deletes any element during this search,
				// and if this element was in our path, we reach the already
				// checked xHead and xTail.
				// To solve this bad situation a big lock is imposed and a
				// recursive call will eventually find the correct location.
				if (xTail == xCurr) {
					try {
						PWX_LOCK_GUARD(list_t, const_cast<list_t*>(this))
						// Note: We do not set curr to tail, because xTail might
						// be somewhere else than the current tail, now.
						curr = xCurr;
						return privGetElementByIndex(index);
					}
					PWX_CATCH_AND_FORGET(std::exception)
				}
			}

			// xCurr is sure to be pointing where it should now.
			PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
			curr = xCurr;
			PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))
			return xCurr;
		}

		return nullptr;
	}


	/// @brief preparation method to insert data behind data
	virtual uint32_t privInsDataBehindData(data_t* prev, data_t* data)
	{
		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (prev && (nullptr == prevElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this singly linked list")

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY_STD_FURTHER (newElement = new elem_t (data, destroy),
							 "ElementCreationFailed",
							 "The Creation of a new list element failed.")

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert data behind an element
	virtual uint32_t privInsDataBehindElem(elem_t* prev, data_t* data)
	{
		// 1: Prepare the previous element
		elem_t* prevElement = prev;

#ifdef PWX_THREADDEBUG
		if (prevElement) {
			PWX_LOCK(prevElement)
			while (prevElement->destroyed()) {
				// This is bad. It means that someone manually deleted the element.
				// If the element still has a next, or if it is the last element,
				// we can, however, continue.
				uint32_t localCount = size();
				if ((localCount > 1) && prevElement->next) {
					PWX_LOCK(prevElement->next)
					PWX_UNLOCK(prevElement)
					prevElement = prevElement->next;
				} else if (localCount < 2) {
					PWX_UNLOCK(prevElement)
					prevElement = nullptr; // New head about
				} else {
					PWX_UNLOCK(prevElement)
					// my bad...
					PWX_THROW("Illegal Condition", "Previous element destroyed",
							  "An element used as prev for insertion is destroyed.")
				}
			} // End of ensuring a valid prevElement
		}
		if (prevElement) PWX_UNLOCK(prevElement)
#endif // PWX_THREADDEBUG

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY_STD_FURTHER (newElement = new elem_t (data, destroy),
							 "ElementCreationFailed",
							 "The Creation of a new list element failed.")

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind data
	virtual uint32_t privInsElemBehindData(data_t* prev, const elem_t &src)
	{
		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (prev && (nullptr == prevElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this singly linked list")

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
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind an element
	virtual uint32_t privInsElemBehindElem(elem_t* prev, const elem_t &src)
	{
		// 1: Prepare the previous element
		elem_t* prevElement = prev;

#ifdef PWX_THREADDEBUG
		if (prevElement) {
			PWX_LOCK(prevElement)
			while (prevElement->destroyed()) {
				uint32_t localCount = size();
				if ((localCount > 1) && prev->next) {
					PWX_LOCK(prevElement->next)
					PWX_UNLOCK(prevElement)
					prevElement = prevElement->next;
				}
				else if (localCount < 2) {
					PWX_UNLOCK(prevElement)
					prevElement = nullptr; // New head about
				}
				else {
					PWX_UNLOCK(prevElement)
					PWX_THROW("Illegal Condition", "Previous element destroyed",
							  "An element used as prev for insertion is destroyed.")
				}
			} // End of ensuring a valid prevElement
		}
		if (prevElement) PWX_UNLOCK(prevElement)
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
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief simple method to remove an element from the list, all locks must be in place!
	virtual void privRemove (elem_t* prev, elem_t* elem) noexcept
	{
		if (elem) {
			bool needRenumber = true;
			PWX_LOCK_NOEXCEPT(this)

			// maintain tail and head first
			if (tail == elem) {
				// In this case no full renumbering is
				// required, only the last number off.
				needRenumber = false;
				if (head == elem)
					// The last item is going bye bye, now!
					tail = nullptr;
				else
					tail = prev;
			}

			if (elem == head) {
				if (tail)
					head = elem->next;
				else
					head = nullptr;
			}

			// now maintain the neighbors
			if (prev && (prev != elem)) {
				prev->next = elem->next;
				curr = prev;
			} else
				curr = head;

			// Finally elem does not need pointers to its neighbors any more
			// and the list needs to be renumbered
			elem->next = nullptr;
			--eCount;
			if (needRenumber)
				doRenumber = true;
			PWX_UNLOCK_NOEXCEPT(this)
		} // end of having an element to remove
	}


	/// @brief remove the element after the specified data
	virtual elem_t* privRemoveAfterData(data_t* prev)
	{
		elem_t* xPrev = nullptr;

		if (prev && (nullptr == (xPrev = const_cast<elem_t*>(protFind (prev))) ) )
			PWX_THROW ("ElementNotFound", "Element not found", "The searched element can not be found in this singly linked list")

		if (xPrev && (nullptr == xPrev->next))
			PWX_THROW ("OutOfRange", "Element out of range", "There is no element behind element holding the given prev pointer")

		elem_t* toRemove = xPrev ? xPrev->next : head;

		privRemove (xPrev, toRemove);

		return toRemove;
	}


	/// @brief remove the element after the specified element
	virtual elem_t* privRemoveAfterElement(elem_t* prev)
	{
		PWX_LOCK_GUARD(list_t, this)

#ifdef PWX_THREADDEBUG
		if (prev && prev->destroyed()) {
			// If it is deleted, there is no "next" to get on with
			PWX_THROW("Illegal Condition", "prev element destroyed",
					  "The previous element for a removal is already destroyed.")
		}
#endif // PWX_THREADDEBUG

		if (prev && (nullptr == prev->next))
			PWX_THROW ("OutOfRange", "Element out of range", "There is no element behind the given prev element")

		if (empty())
			PWX_THROW ("OutOfRange", "Element out of range", "The list is empty")

		elem_t* toRemove = prev ? prev->next : head;
		privRemove (prev, toRemove);

		return toRemove;
	}
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

