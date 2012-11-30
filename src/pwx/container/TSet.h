#pragma once
#ifndef PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED
#define PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED 1

/** @file TSet.h
  *
  * @brief Declaration of the TSet template to store unique elements
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
#include <pwx/functions/set_fwd.h>

namespace pwx
{

/** @class TSet
  *
  * @brief Template to build sets of variable types
  *
  * A set is a group of elements, where each element exists exactly once. Two
  * sets are equal, if their members are equal. Therefore the sets {1, 2, 3} and
  * {3, 2, 1} are equal. Although sets are unordered, the default constructor
  * will build an ordered set to speed up the access and several operations.
  *
  * If the set is needed to be unordered, it must be constructed with "false" as
  * an argument.
  *
  * Unsorted sets will generally be much closer to O(n) on any insertion and
  * random access than sorted sets.
  *
  * The set inherits from TDoubleList to manage its data pointers. Every element is
  * checked for uniqueness before storing it in a set.
  *
  * This is done on the data level, not pointer level. This makes it necessary for
  * any data to support operator== and operator>.
  *
  * The constructor takes an optional destroy(data_t*) function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * Set operations to build unions, differences and intersections are implemented
  * outside the class with functions prefixed with set_.
  *
  * It is recommended that you use the much more advanced std::set unless you need
  * to store a very large number of elements and can not live with the downside of
  * every element having to be copied into the std container.
  *
  * If PWX_THREADS is defined, changes to the element are done in a locked state.
**/
template<typename data_t>
class PWX_API TSet : public TDoubleList<data_t>
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef TDoubleElement<data_t>      elem_t;
	typedef TDoubleList<data_t, elem_t> base_t;
	typedef TSet<data_t>                list_t;


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor initializes an empty set.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	  * @param[in] sorted defaults to true. Set to false to create an unordered set.
	**/
	TSet (void (*destroy_) (data_t* data), bool sorted = true) noexcept :
		base_t(destroy_),
		isSorted(sorted)
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor sets the data destroy method to the null pointer.
	  * @param[in] sorted defaults to true. Set to false to create an unordered set.
	**/
	TSet(bool sorted = true) noexcept :
		base_t (nullptr),
		isSorted(sorted)
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * @param[in] src reference of the list to copy.
	**/
	TSet (const list_t &src) noexcept :
		base_t (src)
	{
		// The copy ctor of base_t has already copied all elements.
	}


	virtual ~TSet() noexcept;


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


	/** @brief find the element with the given @a data content
	  *
	  * This method searches through the set and returns the element
	  * with the given @a data or nullptr if @a data is not stored in this
	  * set.
	  *
	  * This is a search for the data and not a pointer. Stored objects must
	  * therefore support operator== and operator> in a suitable way.
	  *
	  * @param data reference of the data to find
	  * @return return a pointer to the element storing @a data
	**/
	virtual elem_t* find (data_t &data) noexcept
	{
		return const_cast<elem_t* > (privFindData(static_cast<const data_t> (data)));
	}


	/** @brief find the element with the given @a data content
	  *
	  * This method searches through the set and returns a const pointer
	  * to the element with the given @a data or nullptr if @a data is not
	  * stored in this set.
	  *
	  * This is a search for the data and not a pointer. Stored objects must
	  * therefore support operator== and operator> in a suitable way.
	  *
	  * @param data reference of the data to find
	  * @return return a const pointer to the element storing @a data
	**/
	virtual const elem_t* find (const data_t &data) const noexcept
	{
		return privFindData(data);
	}


	using base_t::get;
	using base_t::getData;


	/** @brief return true if @a elem is an element of this set
	  *
	  * @param[in] elem reference to the element to test
	  * @return true if the element is a member of this set, false otherwise
	**/
	virtual bool hasMember(const elem_t &elem) const noexcept
	{
		return (find(*elem) ? true : false);
	}


	/** @brief return true if @a data is an element of this set
	  *
	  * @param[in] data reference to the data to test
	  * @return true if the data is a member of this set, false otherwise
	**/
	virtual bool hasMember(const data_t &data) const noexcept
	{
		return (find(data) ? true : false);
	}


	using base_t::insNext;
	using base_t::insNextElem;
	using base_t::insPrev;
	using base_t::insPrevElem;


	/** @brief return true if this set is a subset of @a src
	  *
	  * A set A is a subset of set B, if set B has all elements
	  * of set A.
	  *
	  * @param[in] src reference of the set to test against
	  * return true if this set is a subset of @a src
	**/
	virtual bool isSubsetOf(const list_t &src) const noexcept
	{
		bool result = true;

		// The empty set is always a subset of everything.
		if (eCount && (this != &src)) {
			if (src.size()) {
				PWX_DOUBLE_LOCK(list_t, const_cast<list_t*>(this), list_t, const_cast<list_t*>(&src))
				elem_t* xCurr = head;

				// A simple loop will do, because we can use privFindData directly.
				do {
					if (src.privFindData(**xCurr))
						xCurr = xCurr->next;
					else
						result = false;
				} while (result && (xCurr != tail));
			} else
				result = false;
		}

		return result;
	}


	using base_t::pop;
	using base_t::pop_back;
	using base_t::pop_front;


	/** @brief push an element onto the set
	  *
	  * This is the regular set operation pushing an element
	  * onto the end of the set.
	  *
	  * To add an element to the front, use unshift() or push_front().
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data data pointer to store.
	  * @return number of elements stored after the operation.
	**/
	virtual uint32_t push(data_t* data)
	{
		PWX_TRY_PWX_FURTHER(return push_back(data))
	}


	/** @brief push an element copy onto the set
	  *
	  * This is the regular set operation pushing an element
	  * copy onto the end of the set.
	  *
	  * To add an element to the front, use unshift() or push_front().
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src element to copy.
	  * @return number of elements stored after the operation.
	**/
	virtual uint32_t push(const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER(return push_back(src))
	}


	using base_t::push_back;
	using base_t::push_front;
	using base_t::remData;
	using base_t::remElem;
	using base_t::remNext;
	using base_t::remNextElem;
	using base_t::remPrev;
	using base_t::remPrevElem;


	/** @brief reset a set to a predefined state of a different set
	  *
	  * This method can be used to clear a set and copy both the
	  * sorted switch and the destroy method from another set.
	  * Basically this is meant to build a working set without
	  * having to use the copy constructor, which would copy all
	  * members as well.
	  *
	  * If a set is reseted using itself, it is cleared of all elements
	  * nevertheless.
	  *
	  * @param[in] src reference of the set to take the base values from
	**/
	virtual void reset(const list_t &src) noexcept
	{
		PWX_LOCK_GUARD(list_t, this)
		clear();
		if (&src != this) {
			destroy  = src.destroy;
			isSorted = src.isSorted;
		}
	}


	/** @brief shift an element from the set
	  *
	  * This is the irregular set operation shifting an element
	  * from the end of the set.
	  *
	  * To get an element from the start, use pop() or pop_front().
	  *
	  * The element is removed from the set so you have to take
	  * care of its deletion once you are finished with it.
	  *
	  * If there is no element in the set a pwx::CException with the
	  * name "OutOfRange" is thrown.
	  *
	  * @return the top element on the set.
	**/
	virtual elem_t* shift()
	{
		PWX_TRY_PWX_FURTHER(return pop_back())
	}


	using base_t::size;


	/** @brief unshift an element onto the set
	  *
	  * This is the irregular set operation unshifting an element
	  * onto the start of the set.
	  *
	  * To add an element to the end, use push() or push_back().
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data data pointer to store.
	  * @return number of elements stored after the operation.
	**/
	virtual uint32_t unshift(data_t* data)
	{
		PWX_TRY_PWX_FURTHER(return push_front(data))
	}


	/** @brief unshift an element copy onto the set
	  *
	  * This is the irregular set operation unshifting an element
	  * copy onto the start of the set.
	  *
	  * To add an element to the end, use push() or push_back().
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src element to copy.
	  * @return number of elements stored after the operation.
	**/
	virtual uint32_t unshift(const elem_t &src)
	{
		PWX_TRY_PWX_FURTHER(return push_front(src))
	}


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief assignment operator
	  *
	  * Clears this set and copies all elements from @a rhs
	  * onto this set.
	  *
	  * @param[in] rhs reference of the set to copy.
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
	using base_t::protInsert;


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	using base_t::eCount;
	using base_t::eNr;
	using base_t::curr;
	using base_t::head;
	using base_t::tail;

	bool isSorted; //!< determines whether the set is sorted or not.

private:
	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	/** @brief find an element holding the specified @a data
	  *
	  * The protFind() method of the containers search for pointers, while
	  * this special method searches for the data behind the pointers.
	  *
	  * If the set is sorted and @a data can not be found, then curr points
	  * to the element which would precede an element holding @a data if
	  * it where present. With this special outcome the inserting methods
	  * can simply use insNextElem() with curr to add data in a sorted way.
	  * The only detail to look at is the situation when the new element
	  * must become the new head. In this special case, <B>curr is set to
	  * nullptr</B> and <B>eNr is set to -1</B>.
	  *
	  * @param[in] data reference of the data to search.
	  * @return pointer to the element or nullptr if @a data is not present in the set.
	**/
	const elem_t* privFindData (const data_t &data) const noexcept
	{
		// Note: As the consistency of the content of a set is more important
		//       than anything, a big lock is a must-have here!
		PWX_LOCK_GUARD(list_t, const_cast<list_t*>(this))

		/* Note:
		 * When the set is sorted, we can make some quick exit assumptions:
		 * 1: If head is larger, data can not be in the set
		 * 2: If tail is smaller, data can not be in the set
		*/
		if (eCount) {
			// Quick exit if sorted set assumption 1 is correct:
			if (isSorted && (**head > data)) {
				eNr  = -1;
				curr = nullptr;
				return nullptr;
			}

			// Reset curr if a previous search has invalidated it:
			if (nullptr == curr) {
				curr = head;
				eNr  = 0;
			}

			// Quick exit if curr is correct:
			if (**curr == data)
				return curr;

			// Quick exit if head is wanted:
			if (**head == data) {
				eNr = 0;
				curr = head;
				return curr;
			}
			// End of having at least one element
		} else
			// return immediately if there are no elements
			return nullptr;

		// Checking tail does only make sense if we have at least 2 element
		if (eCount > 1) {
			// Quick exit if sorted set assumption 2 is correct:
			if (isSorted && (data > **tail)) {
				eNr  = eCount -1;
				curr = tail;
				return nullptr;
			}

			// Quick exit if tail is wanted:
			if (**head == data) {
				eNr = eCount - 1;
				curr = tail;
				return curr;
			}
		} // End of having at least two elements

		// The complete search is useless unless there are at least three elements:
		if (eCount > 2) {
			/* The searching differs between sorted and unsorted sets:
			 * A)   sorted : We can determine a direction to go to make
			 *               the search as quick as possible
			 * B) unsorted : We have to wander through the whole set
			*/
			if (isSorted) {
				/* Here we know that data is definitely in the range of the set.
				 * It is larger than head an smaller than tail. The good thing
				 * about this is the absence of any need to check curr against
				 * head or tail.
				*/
				// Step 1: Move up until curr is larger
				while (data > **curr) {
					curr = curr->next;
					++eNr;
				}
				// Step 2: Move down until curr is smaller
				while (**curr > data) {
					curr = curr->prev;
					--eNr;
				}
				/* Due to this order curr is now either pointing to an element
				 * holding data, or the next smaller element. The latter detail
				 * is important for the sorted insertion. If data is not found,
				 * all inserting methods can now insert a new element holding
				 * the searched data using insNextElem() on curr.
				*/
				return (**curr == data ? curr : nullptr);
			} else {
				curr = head->next;
				eNr  = 1;
				// Note: head and tail are already checked.
				while ((curr != tail) && (**curr != data)) {
					curr = curr->next;
					++eNr;
				}
				// Because tail is already checked, a pointer comparison will do:
				return (curr != tail ? curr : nullptr);
			}
		} // End of having at least 3 elements


		return nullptr;
	}


	/// @brief preparation method to insert data behind data
	virtual uint32_t privInsDataBehindData(data_t* prev, data_t* data)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*data))
			return eCount;

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? curr : prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (!isSorted && prev && (nullptr == prevElement))
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
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*data))
			return eCount;

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? curr : prev;

#ifdef PWX_THREADDEBUG
		if (prevElement) {
			while (prevElement->destroyed()) {
				// This is bad. It means that someone manually deleted the element.
				// If the element still has a next, or if it is the last element,
				// we can, however, continue.
				if ((eCount > 1) && prevElement->next)
					prevElement = prevElement->next;
				else if (eCount < 2)
					prevElement = nullptr; // New head about
				else
					// my bad...
					PWX_THROW("Illegal Condition", "Previous element destroyed",
							  "An element used as prev for insertion is destroyed.")
			} // End of ensuring a valid prevElement
		}
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
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*src))
			return eCount;

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? curr : prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (!isSorted && prev && (nullptr == prevElement))
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
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind an element
	virtual uint32_t privInsElemBehindElem(elem_t* prev, const elem_t &src)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*src))
			return eCount;

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? curr : prev;

#ifdef PWX_THREADDEBUG
		if (prevElement) {
			while (prevElement->destroyed()) {
				if ((eCount > 1) && prev->next)
					prevElement = prevElement->next;
				else if (eCount < 2)
					prevElement = nullptr; // New head about
				else
					PWX_THROW("Illegal Condition", "Previous element destroyed",
							  "An element used as prev for insertion is destroyed.")
			} // End of ensuring a valid prevElement
		}
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
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert data before data
	virtual uint32_t privInsDataBeforeData(data_t* next, data_t* data)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*data))
			return eCount;

		// 1: Prepare the next element
		elem_t* nextElement = isSorted ? curr : next ? const_cast<elem_t*>(find(next)) : nullptr;
		if (!isSorted && next && (nullptr == nextElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this doubly linked list")

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY_STD_FURTHER (newElement = new elem_t (data, destroy),
							 "ElementCreationFailed",
							 "The Creation of a new list element failed.")

		// 3: Do the real insert
		if (isSorted)
			PWX_TRY_PWX_FURTHER(return protInsert(curr, newElement))
		else
			PWX_TRY_PWX_FURTHER(return protInsert(nextElement ? nextElement->prev : nullptr, newElement))
	}


	/// @brief preparation method to insert data before an element
	virtual uint32_t privInsDataBeforeElem(elem_t* next, data_t* data)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*data))
			return eCount;

		// 1: Prepare the next element
		elem_t* nextElement = isSorted ? curr : next;

#ifdef PWX_THREADDEBUG
		if (nextElement) {
			while (nextElement->destroyed()) {
				// This is bad. It means that someone manually deleted the element.
				// If the element still has a prev, or if it is the last element,
				// we can, however, continue.
				if ((eCount > 1) && nextElement->prev)
					nextElement = nextElement->prev;
				else if (eCount < 2)
					nextElement = nullptr; // New head about
				else
					// my bad...
					PWX_THROW("Illegal Condition", "Next element destroyed",
							  "An element used as next for insertion is destroyed.")
			} // End of ensuring a valid nextElement
		}
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
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*src))
			return eCount;

		// 1: Prepare the next element
		elem_t* nextElement = isSorted ? curr : next ? const_cast<elem_t*>(find(next)) : nullptr;
		if (!isSorted && next && (nullptr == nextElement))
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
		if (isSorted)
			PWX_TRY_PWX_FURTHER(return protInsert(curr, newElement))
		else
			PWX_TRY_PWX_FURTHER(return protInsert(nextElement ? nextElement->prev : nullptr, newElement))
	}


	/// @brief preparation method to insert an element copy before an element
	virtual uint32_t privInsElemBeforeElem(elem_t* next, const elem_t &src)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*src))
			return eCount;

		// 1: Prepare the previous element
		elem_t* nextElement = isSorted ? curr : next;

#ifdef PWX_THREADDEBUG
		if (nextElement) {
			while (nextElement->destroyed()) {
				// This is bad. It means that someone manually deleted the element.
				// If the element still has a prev, or if it is the last element,
				// we can, however, continue.
				if ((eCount > 1) && nextElement->prev)
					nextElement = nextElement->prev;
				else if (eCount < 2)
					nextElement = nullptr; // New head about
				else
					// my bad...
					PWX_THROW("Illegal Condition", "Next element destroyed",
							  "An element used as next for insertion is destroyed.")
			} // End of ensuring a valid nextElement
		}
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
		if (isSorted)
			PWX_TRY_PWX_FURTHER(return protInsert(curr, newElement))
		else
			PWX_TRY_PWX_FURTHER(return protInsert(nextElement ? nextElement->prev : nullptr, newElement))
	}

}; // class TSet


/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the set.
**/
template<typename data_t>
TSet<data_t>::~TSet() noexcept
{ /* deletion is done in base_t ctor */ }


} // namespace pwx

#include <pwx/functions/set_func.h>

#endif // PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED

