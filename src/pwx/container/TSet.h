#pragma once
#ifndef PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED
#define PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED 1

/** @file TSet.h
  *
  * @brief Declaration of the TSet template to store unique elements
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

#include "pwx/types/CLockable.h"
#include "pwx/general/macros.h"
#include "pwx/container/TDoubleList.h"
#include "pwx/functions/set_fwd.h"

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
  * The set is derived from TDoubleList to manage its element. Every element is
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
  * @see pwx::TDoubleList for further information.
**/
template<typename data_t>
class PWX_API TSet : public TDoubleList<data_t>
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef TDoubleElement<data_t>      elem_t; //!< Type of the stored elements
	typedef TDoubleList<data_t, elem_t> base_t; //!< Base type of the set
	typedef TSet<data_t>                list_t; //!< Type of this set


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
	using base_t::disable_thread_safety;
	using base_t::empty;
	using base_t::enable_thread_safety;
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
		bool     result     = true;

		// The empty set is always a subset of everything.
		if ( eCount.load(std::memory_order_acquire)
		  && (this != &src)) {
			if (src.eCount.load(std::memory_order_acquire)) {
				PWX_DOUBLE_LOCK(list_t, const_cast<list_t*>(this), list_t, const_cast<list_t*>(&src))
				elem_t* xCurr  = head;
				bool    isDone = false;

				// A simple loop will do, because we can use privFindData directly.
				while (result && xCurr && !isDone) {
					if (src.privFindData(**xCurr)) {
						if (xCurr == tail)
							isDone = true;
						else
							xCurr = this->beThreadSafe.load(std::memory_order_relaxed)
								  ? xCurr->next.load(std::memory_order_acquire)
								  : xCurr->next.load(std::memory_order_relaxed);
					} else
						result = false;
				}
			} else
				result = false;
		}

		return result;
	}


	using base_t::pop;
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
		// A big lock on both is needed to ensure that
		// a) Nothing is added by another thread and
		// b) src can not go away before the reset is done.
		if (this->beThreadSafe) {
			PWX_LOCK(this)
			if (this != &src) {
				while (!const_cast<list_t*>(&src)->try_lock()) {
					PWX_UNLOCK(this)
					std::this_thread::yield();
					PWX_LOCK(this)
				}
			}
		}

		// Now do the work
		clear();
		if (&src != this) {
			destroy  = src.destroy;
			isSorted = src.isSorted;
		}

		// Unlock if needed
		if (this->beThreadSafe) {
			PWX_UNLOCK(const_cast<list_t*>(&src))
			PWX_UNLOCK(this)
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
		PWX_TRY_PWX_FURTHER(return base_t::pop_back())
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
		PWX_TRY_PWX_FURTHER(return base_t::push_front(data))
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
		PWX_TRY_PWX_FURTHER(return base_t::push_front(src))
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
		uint32_t localCount = eCount.load(std::memory_order_acquire);

		/* Note:
		 * When the set is sorted, we can make some quick exit assumptions:
		 * 1: If head is larger, data can not be in the set
		 * 2: If tail is smaller, data can not be in the set
		*/
		if (localCount) {
			// Quick exit if sorted set assumption 1 is correct:
			if (isSorted && (**head > data)) {
				curr = nullptr;
				return nullptr;
			}

			// Reset curr if a previous search has invalidated it:
			if (nullptr == curr)
				curr = head;

			// Quick exit if curr is correct:
			if (**curr == data)
				return curr;

			// Quick exit if head is wanted:
			if (**head == data) {
				curr = head;
				return curr;
			}
			// End of having at least one element
		} else
			// return immediately if there are no elements
			return nullptr;

		// Checking tail does only make sense if we have at least 2 element
		if (localCount > 1) {
			// Quick exit if sorted set assumption 2 is correct:
			if (isSorted && (data > **tail)) {
				curr = tail;
				return nullptr;
			}

			// Quick exit if tail is wanted:
			if (**tail == data) {
				curr = tail;
				return curr;
			}
		} // End of having at least two elements

		// The complete search is useless unless there are at least three elements:
		if (localCount > 2) {
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
				while (data > **curr)
					curr = curr->getNext();

				// Step 2: Move down until curr is smaller
				while (**curr > data)
					curr = curr->getPrev();

				/* Due to this order curr is now either pointing to an element
				 * holding data, or the next smaller element. The latter detail
				 * is important for the sorted insertion. If data is not found,
				 * all inserting methods can now insert a new element holding
				 * the searched data using insNextElem() on curr.
				*/
				return (**curr == data ? curr : nullptr);
			} else {
				curr = head->getNext();

				// Note: head and tail are already checked.
				while ((curr != tail) && (**curr != data))
					curr = curr->getNext();

				// Because tail is already checked, a pointer comparison will do:
				return (curr != tail ? curr : nullptr);
			}
		} // End of having at least 3 elements


		return nullptr;
	}


	/// @brief preparation method to insert data behind data
	virtual uint32_t privInsDataBehindData(data_t* prev, data_t* data)
	{
		// All preparation methods use big locks to ensure data consistency!
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*data))
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? curr : prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (!isSorted && prev && (nullptr == prevElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this singly linked list")

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
		if (!this->beThreadSafe.load(std::memory_order_relaxed))
			newElement->do_locking(false);

		// 3: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert data behind an element
	virtual uint32_t privInsDataBehindElem(elem_t* prev, data_t* data)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*data))
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? curr : prev;
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
		if (!this->beThreadSafe.load(std::memory_order_relaxed))
			newElement->do_locking(false);

		// 3: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind data
	virtual uint32_t privInsElemBehindData(data_t* prev, const elem_t &src)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*src))
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? curr : prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (!isSorted && prev && (nullptr == prevElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this singly linked list")
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
		if (!this->beThreadSafe.load(std::memory_order_relaxed))
			newElement->do_locking(false);

		// 4: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind an element
	virtual uint32_t privInsElemBehindElem(elem_t* prev, const elem_t &src)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*src))
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? curr : prev;
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
		if (!this->beThreadSafe.load(std::memory_order_relaxed))
			newElement->do_locking(false);

		// 4: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert data before data
	virtual uint32_t privInsDataBeforeData(data_t* next, data_t* data)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*data))
			return size();

		// 1: Prepare the next element
		elem_t* nextElement = !isSorted && next ? const_cast<elem_t*>(find(next)) : nullptr;
		if (!isSorted && next && (nullptr == nextElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this doubly linked list")
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
		if (!this->beThreadSafe.load(std::memory_order_relaxed))
			newElement->do_locking(false);

		// 3: Do the real insert
		elem_t* prev = isSorted
					 ? curr
					 : nextElement
						? this->beThreadSafe.load(std::memory_order_relaxed)
							? nextElement->getPrev()
							: nextElement->prev.load(std::memory_order_relaxed)
						: tail;
		if (nextElement)
			PWX_UNLOCK(nextElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
	}


	/// @brief preparation method to insert data before an element
	virtual uint32_t privInsDataBeforeElem(elem_t* next, data_t* data)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*data))
			return size();

		// 1: Prepare the next element
		elem_t* nextElement = !isSorted && next ? next : nullptr;
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
		if (!this->beThreadSafe.load(std::memory_order_relaxed))
			newElement->do_locking(false);

		// 3: Do the real insert
		elem_t* prev = isSorted
					 ? curr
					 : nextElement
						? this->beThreadSafe.load(std::memory_order_relaxed)
							? nextElement->getPrev()
							: nextElement->prev.load(std::memory_order_relaxed)
						: tail;
		if (nextElement)
			PWX_UNLOCK(nextElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
	}


	/// @brief preparation method to insert an element copy before data
	virtual uint32_t privInsElemBeforeData(data_t* next, const elem_t &src)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*src))
			return size();

		// 1: Prepare the next element
		elem_t* nextElement = !isSorted && next ? const_cast<elem_t*>(find(next)) : nullptr;
		if (!isSorted && next && (nullptr == nextElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this doubly linked list")
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
		if (!this->beThreadSafe.load(std::memory_order_relaxed))
			newElement->do_locking(false);

		// 4: Do the real insert
		elem_t* prev = isSorted
					 ? curr
					 : nextElement
						? this->beThreadSafe.load(std::memory_order_relaxed)
							? nextElement->getPrev()
							: nextElement->prev.load(std::memory_order_relaxed)
						: tail;
		if (nextElement)
			PWX_UNLOCK(nextElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
	}


	/// @brief preparation method to insert an element copy before an element
	virtual uint32_t privInsElemBeforeElem(elem_t* next, const elem_t &src)
	{
		PWX_LOCK_GUARD(list_t, this)
		if (privFindData(*src))
			return size();

		// 1: Prepare the next element
		elem_t* nextElement = !isSorted && next ? next : nullptr;
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
		if (!this->beThreadSafe.load(std::memory_order_relaxed))
			newElement->do_locking(false);

		// 4: Do the real insert
		elem_t* prev = isSorted
					 ? curr
					 : nextElement
						? this->beThreadSafe.load(std::memory_order_relaxed)
							? nextElement->getPrev()
							: nextElement->prev.load(std::memory_order_relaxed)
						: tail;
		if (nextElement)
			PWX_UNLOCK(nextElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
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

#include "pwx/functions/set_func.h"

#endif // PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED

