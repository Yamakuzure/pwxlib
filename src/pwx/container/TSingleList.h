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
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * If PWX_THREADS is defined, changes to the element are done in a locked state.
**/
template<typename data_t, typename elem_t = TSingleElement<data_t> >
class TSingleList : public VContainer
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
		// lock the source list, it must not be changed now.
		PWX_LOCK_GUARD (list_t, const_cast<list_t> (src))
		uint32_t rSize = src.size();
		for (int32_t i = 0; i < rSize; ++i) {
			PWX_TRY_PWX_FURTHER (insNextElem (tail, *src[i]))
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
		PWX_LOCK_GUARD(list_t, this)
		while (eCount)
			delNext (nullptr);
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
		PWX_LOCK_GUARD (list_t, this)
		try {
			elem_t* removed = remNext (prev);
			if (removed)
				delete removed;
			return eCount;
		}
		PWX_THROW_PWXSTD_FURTHER ("delete", "Deleting an element in TSingleList::delNext() failed.")
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
		PWX_LOCK_GUARD (list_t, this)
		try {
			elem_t* removed = remNextElem (prev);
			if (removed)
				delete removed;
			return eCount;
		}
		PWX_THROW_PWXSTD_FURTHER ("delete", "Deleting an element in TSingleList::delNextElem() failed.")
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
		return const_cast<elem_t* > (find (static_cast<const data_t* > (data)));
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
		if (nullptr == curr)
			return nullptr;

		// From here on, curr is valid and the list needs to be guarded:
		PWX_LOCK_GUARD (list_t, const_cast<list_t* > (this))

		// Quick exit if curr is already what we want:
		if (curr->data.get() == data)
			return curr;

		// The next does only make sense if we have more than one element
		if (eCount > 1) {
			// Exit if head is wanted...
			if (head->data.get() == data) {
				curr = head;
				eNr  = 0;
				return head;
			}

			// ...or tail
			if (tail->data.get() == data) {
				curr = tail;
				eNr  = eCount - 1;
				return tail;
			}

			// Otherwise search for the previous item, it's the next, then
			elem_t* prev = privFindPrev (data);
			// Note: privFindPrev returns the wanted element and sets curr to the next element.
			if (prev)
				return curr;
		} // End of handling a search with more than one element

		return nullptr;
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
		PWX_LOCK_GUARD (list_t, this)

		if (prev && (nullptr == find (prev)))
			// find sets curr to the correct value.
			PWX_THROW ("ElementNotFound", "Element not found", "The searched element can not be found in this singly linked list")

		// First create a new element for data
		elem_t* newElement = nullptr;
		PWX_TRY_STD_FURTHER (newElement = new elem_t (data, destroy),
							 "ElementCreationFailed",
							 "The Creation of a new list element failed.")

		return privInsert (prev ? curr : nullptr, newElement);
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
		PWX_LOCK_GUARD (list_t, this)

		if (prev && (nullptr == find (prev)))
			// find sets curr to the correct value.
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this singly linked list")

		// First create a new element for data
		elem_t* newElement = nullptr;
		PWX_TRY_STD_FURTHER (newElement = new elem_t (src),
							 "ElementCreationFailed",
							 "The Creation of a new list element failed.")

		return privInsert (prev ? curr : nullptr, newElement);
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
		PWX_LOCK_GUARD (list_t, this)

		// First create a new element for data
		elem_t* newElement = nullptr;
		PWX_TRY_STD_FURTHER (newElement = new elem_t (data, destroy),
							 "ElementCreationFailed",
							 "The Creation of a new list element failed.")

		return privInsert (prev, newElement);
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
		PWX_LOCK_GUARD (list_t, this)

		// First create a new element for data
		elem_t* newElement = nullptr;
		PWX_TRY_STD_FURTHER (newElement = new elem_t (src),
							 "ElementCreationFailed",
							 "The Creation of a new list element failed.")

		return privInsert (prev, newElement);
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
		PWX_LOCK_GUARD (list_t, this)
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
		PWX_LOCK_GUARD (list_t, this)
		if (eCount) {
			PWX_TRY (return remNext (nullptr))
			PWX_CATCH_AND_FORGET (CException)
		}
		return nullptr;
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
		PWX_LOCK_GUARD (list_t, this)
		PWX_TRY_PWX_FURTHER (return insNextElem (tail, data))
	}


	/** @brief alias to add an element copy to the end of the list.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_back (elem_t &src)
	{
		PWX_LOCK_GUARD (list_t, this)
		PWX_TRY_PWX_FURTHER (return insNextElem (tail, src))
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
		PWX_LOCK_GUARD (list_t, this)
		PWX_TRY_PWX_FURTHER (return insNext (nullptr, data))
	}


	/** @brief alias to add an element copy to the head of the list.
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_front (elem_t &src)
	{
		PWX_LOCK_GUARD (list_t, this)
		PWX_TRY_PWX_FURTHER (return insNext (nullptr, src))
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
		PWX_LOCK_GUARD (list_t, this)

		if (prev && (nullptr == find (prev)))
			// find sets curr to the correct value.
			PWX_THROW ("ElementNotFound", "Element not found", "The searched element can not be found in this singly linked list")

		if (prev && (nullptr == curr->next))
			PWX_THROW ("OutOfRange", "Element out of range", "There is no element behind element holding the given prev pointer")

		elem_t* toRemove = prev ? curr->next : head;

		// Lock both curr and the element to remove, then get it out
		PWX_DOUBLE_LOCK (elem_t, curr, elem_t, toRemove);
		privRemove (prev ? curr : nullptr, toRemove);

		return toRemove;
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
		PWX_LOCK_GUARD (list_t, this)

		if (prev && (nullptr == prev->next))
			PWX_THROW ("OutOfRange", "Element out of range", "There is no element behind the given prev element")

		if (0 == eCount)
			PWX_THROW ("OutOfRange", "Element out of range", "The list is empty")

		elem_t* toRemove = prev ? prev->next : head;

		// Lock and remove the element
		toRemove->lock();
		privRemove (prev, toRemove);
		toRemove->unlock();

		return toRemove;
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

	void (*destroy) (data_t* data_);


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

	/// IMPORTANT: private methods do not lock, callers must have locked!

	/// @brief Search until the next element contains the searched data
	virtual elem_t* privFindPrev (const data_t* data) const noexcept
	{
		elem_t* prev = head;
		curr = head->next;
		eNr  = 1;
		while (prev != tail) {
			if (curr->data.get() == data)
				return prev;
			++eNr;
			prev = curr;
			curr = curr->next;
		}
		// If we are here, prev points to tail and curr is invalid:
		curr = prev;
		--eNr;
		return nullptr;
	}

	/// @brief wrapping method to retrieve an element by any index or nullptr if the list is empty
	virtual const elem_t* privGetElementByIndex (int32_t index) const noexcept
	{
		if (eCount) {
			// Mod index into range
			uint32_t xIdx = static_cast<uint32_t> (index < 0
												   ? eCount - (::std::abs (index) % eCount)
												   : index % eCount);
			// Unfortunately this results in xIdx equaling eCount
			// (which is wrong) if index is a negative multiple of
			// eCount:
			if (xIdx >= eCount)
				xIdx = xIdx % eCount;

			// Now protect the list, we need curr to be changeable without fear:
			PWX_LOCK_GUARD (list_t, const_cast<list_t* > (this))

			// Is curr already correct?
			if (xIdx == eNr)
				return curr;

			// Is xIdx the next member, like in a for loop?
			if (xIdx == (eNr + 1)) {
				curr = curr->next;
				++eNr;
				return curr;
			}

			// Is it the head we want?
			if (0 == xIdx)
				return head;

			// Or tail ?
			if ( (eCount - 1) == xIdx)
				return tail;

			// Ok, let's go. But only start from head if we currently are beyond.
			if (xIdx < eNr) {
				curr = head->next;
				eNr  = 1;
			}
			// Otherwise the next of curr is already checked, so skip it
			else {
				curr = curr->next;
				++eNr;
			}
			// Now look into the rest
			while ( (eNr < xIdx) && (eNr < (eCount - 1))) {
				curr = curr->next;
				++eNr;
			}
			// curr is sure to be pointing where it should now.
			return curr;
		}

		return nullptr;
	}


	/// @brief simple method to insert an element into the list
	virtual uint32_t privInsert (elem_t* prev, elem_t* elem)
	{
		if (elem) {
			if (prev) {
				if (tail == prev)
					tail = elem;
				elem->next = prev->next;
				prev->next = elem;
				// curr is only maintainable if it is prev
				if (curr != prev) {
					// In which case it wouldn't have needed any change.
					curr = head;
					eNr  = 0;
				}
			} else if (eCount) {
				elem->next = head;
				head = elem;
				++eNr; // No matter what happened, curr has another element before it now.
			} else {
				// If we had no elements yet, head and tail need to be set:
				head = elem;
				tail = elem;
				curr = head;
			}
			++eCount;
		} // End of having an element to insert
		return eCount;
	}


	/// @brief simple method to remove an element from the list
	virtual uint32_t privRemove (elem_t* prev, elem_t* elem)
	{
		if (elem) {
			// maintain tail and head first
			if (tail == elem)
				tail = prev;

			if (elem == head)
				head = elem->next;

			// now maintain the neighbors
			if (prev)
				prev->next = elem->next;

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
			--eCount;
		} // end of having an element to remove
		return eCount;
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
	lhs.lock();
	list_t result(lhs);
	lhs.unlock();
	if (&lhs != &rhs) {
		PWX_LOCK_GUARD (list_t, const_cast<list_t*> (&rhs))
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
	lhs.lock();
	list_t result(lhs);
	lhs.unlock();

	if (&lhs != &rhs) {
		PWX_LOCK_GUARD (list_t, const_cast<list_t*> (&rhs))
		PWX_TRY_PWX_FURTHER(result -= rhs)
	} else
		result.clear();

	return result;
}

} // namespace pwx

#endif // PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED

