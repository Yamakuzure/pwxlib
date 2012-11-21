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

#include <pwx/base/VContainer.h>
#include <pwx/container/TContStateList.h>


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
	typedef TContState<elem_t>          state_t;
	typedef TContStateList<elem_t>      state_list_t;


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
		// lock the source list, it must not be changed now.
		PWX_LOCK_GUARD (list_t, const_cast<list_t*> (&src))
		uint32_t rSize = src.size();
		for (uint32_t i = 0; i < rSize; ++i) {
			PWX_TRY_PWX_FURTHER (privInsElemBehindElem(tail, *src[i]))
		}
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
		while (eCount) {
			try {
#ifdef PWX_THREADS
				PWX_LOCK_NOEXCEPT(this)
				elem_t* toDelete = head;
				if (eCount && toDelete && !toDelete->destroyed()) {
					privRemove(nullptr, toDelete);
					PWX_UNLOCK_NOEXCEPT(this)
					privDelete(toDelete);
				} else
					PWX_UNLOCK_NOEXCEPT(this)
#else
				privDelete(remNext(nullptr));
#endif // PWX_THREADS
			}
			PWX_CATCH_AND_FORGET(CException)
		}
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


	// empty() provided by base_t


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
		return const_cast<elem_t* > (privFind (static_cast<const data_t* > (data)));
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
		return privFind(data);
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


	/** @briefshort alias for pop_front()
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
		if (eCount > 1) {
			PWX_TRY (return remNextElem (const_cast<elem_t* > (privGetElementByIndex (-2))))
			PWX_CATCH_AND_FORGET (CException)
		}
		else if (eCount) {
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
		if (eCount) {
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
		PWX_TRY_PWX_FURTHER (return privInsDataBehindElem (tail, data))
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
		PWX_TRY_PWX_FURTHER (return privInsElemBehindElem (tail, src))
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
			int32_t rSize = rhs.size();
			for (int32_t i = 0; i < rSize; ++i) {
				PWX_TRY_PWX_FURTHER (insNextElem (tail, *rhs[i]))
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
			int32_t rSize = rhs.size();
			for (int32_t i = 0; i < rSize; ++i) {
				elem_t* prev = privFindPrev (rhs[i]->data.get());
				if (prev) {
					PWX_TRY_PWX_FURTHER (delNextElem (prev))
				}
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

	/// @brief internal wrapper that takes care of state data if it is reseted
	state_t* getState() noexcept
	{
		state_t* state = state_list.getState();

		if (state->reseted) {
			PWX_LOCK_NOEXCEPT(this)
			if (eCount) {
				state->curr = head;
				state->eNr  = 0;
			} else {
				state->curr = nullptr;
				state->eNr  = -1;
			}
			PWX_UNLOCK_NOEXCEPT(this)
			state->reseted = false;
		}

		return state;
	}


	void (*destroy) (data_t* data_);


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	elem_t* head = nullptr;   //!< pointer to the first element
	elem_t* tail = nullptr;   //!< pointer to the last element
	mutable
	state_list_t state_list;

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
					// Note: The dtor of CLockable will remove all
					// locks imposed by this thread.
					delete removed;
				else
					PWX_UNLOCK(removed)
			}
			return eCount;
		}
		PWX_THROW_PWXSTD_FURTHER ("delete", "Deleting an element in TSingleList::privDelete() failed.")
	}


	/// @brief Search until the current element contains the searched data
	virtual const elem_t* privFind (const data_t* data) const noexcept
	{
		state_t* state = getState();

		if (nullptr == state->curr)
			return nullptr;

		// Quick exit if curr is already what we want:
		if (state->curr->data.get() == data)
			return state->curr;

		// For subsequent searches curr->next is to be checked as well:
		if (state->curr->next && (state->curr->next->data.get() == data)) {
			state->curr = state->curr->next;
			++state->eNr;
			return state->curr;
		}

		// The next does only make sense if we have more than one element
		if (eCount > 1) {
			// Exit if head is wanted...
			PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
			if (head->data.get() == data) {
				state->curr = head;
				state->eNr  = 0;
				return head;
			}

			// ...or tail
			if (tail->data.get() == data) {
				state->curr = tail;
				state->eNr  = eCount - 1;
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


	/// @brief Search until the next element contains the searched data
	virtual elem_t* privFindPrev (const data_t* data) const noexcept
	{
		state_t* state = getState();
		PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
		elem_t*  prev  = head;

		if (prev && prev != tail) {
			state->curr    = prev->next;
			state->eNr     = 1;
			if (state->reseted)
				state->reseted = false;
		}
		PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))

		while (prev && (prev != tail) && state->curr) {
			if (state->curr->data.get() == data)
				return prev;
			++state->eNr;
			prev  = state->curr;
			state->curr = state->curr->next;
		}

		// If we are here, prev points to tail. No match found.
		if (nullptr == state->curr) {
			state->curr    = head;
			state->eNr     = 0;
			state->reseted = false;
		}

		return nullptr;
	}


	/// @brief wrapping method to retrieve an element by any index or nullptr if the list is empty
	virtual const elem_t* privGetElementByIndex (int32_t index) const noexcept
	{
		if (eCount) {
			state_t* state = state_list.getState();

			// Mod index into range
			PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
			uint32_t xIdx = static_cast<uint32_t> (index < 0
												   ? eCount - (::std::abs (index) % eCount)
												   : index % eCount);
			// Unfortunately this results in xIdx equaling eCount
			// (which is wrong) if index is a negative multiple of
			// eCount:
			if (xIdx >= eCount)
				xIdx = xIdx % eCount;

			if (nullptr == state->curr) {
				state->curr = head;
				state->eNr  = 0;
			}
			PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))

			// Is curr already correct?
			if (xIdx == state->eNr)
				return state->curr;

			// Is xIdx the next member, like in a for loop?
			if (xIdx == (state->eNr + 1)) {
				PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))

				// It is important to make sure that the last item is
				// still around and not deleted, yet.
				if (state->curr->next && !state->curr->next->destroyed()) {
					state->curr = state->curr->next;
					++state->eNr;
				} else
					// Otherwise state is suspicious.
					// Now that we hold a lock, do a (one-time) recursive call
					return privGetElementByIndex(index);
				PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))
				return state->curr;
			}

			// Is it the head we want?
			if (0 == xIdx)
				return head;

			// Or tail ?
			if ( (eCount - 1) == xIdx)
				return tail;

			// Ok, let's go. But only start from head if we currently are beyond.
			if (xIdx < state->eNr) {
				state->curr = head->next;
				state->eNr  = 1;
			}
			// Otherwise the next of curr is already checked, so skip it
			else {
				state->curr = state->curr->next;
				++state->eNr;
			}
			// Now look into the rest
			while ( (state->eNr < xIdx) && (state->eNr < (eCount - 1)) && state->curr) {
				state->curr = state->curr->next;
				++state->eNr;
			}

			// Now curr is either the wanted element ...
			if (state->curr)
				return state->curr;
			// ... or someone freaked the list
			else {
				PWX_LOCK_NOEXCEPT(const_cast<list_t*>(this))
				return privGetElementByIndex(index);
				PWX_UNLOCK_NOEXCEPT(const_cast<list_t*>(this))
			}
		}

		return nullptr;
	}


	/// @brief preparation method to insert data behind data
	/// Note: The list must be locked beforehand!
	virtual uint32_t privInsDataBehindData(data_t* prev, data_t* data)
	{
		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>(privFind(prev)) : nullptr;
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
		PWX_TRY_PWX_FURTHER(return privInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert data behind an element
	/// Note: The list must be locked beforehand!
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
				if ((eCount > 1) && prevElement->next) {
					PWX_LOCK(prevElement->next)
					PWX_UNLOCK(prevElement)
					prevElement = prevElement->next;
				}
				else if (eCount < 2) {
					PWX_UNLOCK(prevElement)
					prevElement = nullptr; // New head about
				}
				else {
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
		PWX_TRY_PWX_FURTHER(return privInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind data
	/// Note: The list must be locked beforehand!
	virtual uint32_t privInsElemBehindData(data_t* prev, const elem_t &src)
	{
		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>(privFind(prev)) : nullptr;
		if (prev && (nullptr == prevElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this singly linked list")

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
		PWX_TRY_PWX_FURTHER(return privInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind an element
	/// Note: The list must be locked beforehand!
	virtual uint32_t privInsElemBehindElem(elem_t* prev, const elem_t &src)
	{
		// 1: Prepare the previous element
		elem_t* prevElement = prev;

#ifdef PWX_THREADDEBUG
		if (prevElement) {
			PWX_LOCK(prevElement)
			while (prevElement->destroyed()) {
				if ((eCount > 1) && prev->next) {
					PWX_LOCK(prevElement->next)
					PWX_UNLOCK(prevElement)
					prevElement = prevElement->next;
				}
				else if (eCount < 2) {
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
		PWX_TRY_PWX_FURTHER(return privInsert(prevElement, newElement))
	}


	/// @brief Simple inserter, all locks must be in place!
	virtual uint32_t privInsert (elem_t* insPrev, elem_t* insElem) noexcept
	{
		// Now the real insertion can be done:
		PWX_LOCK(this)

		// Reset all states, the list will change
		state_list.resetStates();

		if (insPrev) {
			if (tail == insPrev)
				tail = insElem;
			insElem->next = insPrev->next;
			insPrev->next = insElem;
		} else if (eCount) {
			insElem->next = head;
			head = insElem;
		} else {
			// If we had no elements yet, head and tail need to be set:
			head = insElem;
			tail = insElem;
		}

		uint32_t xCount = ++eCount;

		PWX_UNLOCK(this)

		return xCount;
	}


	/// @brief simple method to remove an element from the list, all locks must be in place!
	virtual void privRemove (elem_t* prev, elem_t* elem) noexcept
	{
		if (elem) {
			PWX_LOCK(this)

			// Reset all states, the list will change
			state_list.resetStates();

			// maintain tail and head first
			if (tail == elem)
				tail = prev;

			if (elem == head)
				head = elem->next;

			// now maintain the neighbors
			if (prev)
				prev->next = elem->next;

			// if this was the last element, sanitize the list:
			if (1 == eCount) {
				head = nullptr;
				tail = nullptr;
			}

			// Finally elem does not need pointers to its neighbors any more
			elem->next = nullptr;
			--eCount;
			PWX_UNLOCK(this)
		} // end of having an element to remove
	}


	/// @brief remove the element after the specified data
	virtual elem_t* privRemoveAfterData(data_t* prev)
	{
		elem_t* xPrev = nullptr;

		if (prev && (nullptr == (xPrev = const_cast<elem_t*>(privFind (prev))) ) )
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
		PWX_LOCK(prev)

#ifdef PWX_THREADDEBUG
		if (prev->destroyed()) {
			// If it is deleted, there is no "next" to get on with
			PWX_THROW("Illegal Condition", "prev element destroyed",
					  "The previous element for a removal is already destroyed.")
		}
#endif // PWX_THREADDEBUG

		if (prev && (nullptr == prev->next))
			PWX_THROW ("OutOfRange", "Element out of range", "There is no element behind the given prev element")

		if (0 == eCount)
			PWX_THROW ("OutOfRange", "Element out of range", "The list is empty")

		elem_t* toRemove = prev ? prev->next : head;
		privRemove (prev, toRemove);

		PWX_UNLOCK(prev)

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
  *
  * @param[in] lhs left hand side reference
  * @param[in] rhs right hand side reference
  * @return the new list.
**/
template<typename data_t, typename elem_t>
TSingleList<data_t, elem_t> operator+ (const TSingleList<data_t, elem_t> &lhs, const TSingleList<data_t, elem_t> &rhs)
{
	typedef TSingleList<data_t, elem_t> list_t;
	PWX_LOCK(&lhs)
	list_t result(lhs);
	PWX_UNLOCK(&lhs)
	if (&lhs != &rhs) {
		PWX_TRY_PWX_FURTHER(result += rhs)
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
TSingleList<data_t, elem_t> operator- (const TSingleList<data_t, elem_t> &lhs, const TSingleList<data_t, elem_t> &rhs)
{
	typedef TSingleList<data_t, elem_t> list_t;
	PWX_LOCK(&lhs)
	list_t result(lhs);
	PWX_UNLOCK(&lhs)

	if (&lhs != &rhs) {
		PWX_TRY_PWX_FURTHER(result -= rhs)
	} else
		result.clear();

	return result;
}

} // namespace pwx

#endif // PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED

