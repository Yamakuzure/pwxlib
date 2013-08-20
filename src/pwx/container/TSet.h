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

#include <pwx/types/CLockable.h>
#include <pwx/general/macros.h>
#include <pwx/container/TDoubleList.h>
#include <pwx/container/TChainHash.h>
#include <pwx/functions/set_fwd.h>

namespace pwx
{

/** @class TSet
  *
  * @brief Template to build sets of variable types
  *
  * A set is a group of elements, where each element exists exactly once. Two
  * sets are equal, if their members are equal. Therefore the sets {1, 2, 3} and
  * {3, 2, 1} are equal. Although sets are explicitly unordered. To speed up
  * access TSet uses a chained hash table to search and verify data.
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
  * To maintain the order of elements as they are added the standard operations
  * to add are push(), add an element to the end of the set, and pop(), take an
  * element from the beginning of the set.
  * The two reversal methods unshift() and shift() to add to the beginning and to
  * take from the end are available for convenience.
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
	typedef TChainHash<data_t, elem_t>  hash_t; //!< Chained hash for data lookup


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor initializes an empty set.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TSet (void (*destroy_) (data_t* data)) noexcept :
		base_t(destroy_),
		lookup(list_t::do_not_destroy)
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor sets the data destroy method to the null pointer.
	**/
	TSet() noexcept :
		base_t (nullptr),
		lookup(list_t::do_not_destroy)
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * @param[in] src reference of the list to copy.
	**/
	TSet (const list_t &src) noexcept :
		base_t (src),
		lookup(list_t::do_not_destroy)
	{
		// The copy ctor of base_t has already copied all elements.
		// However, lookup is empty and needs to be filled:
		elem_t* xCurr = head();
		elem_t* xTail = tail();
		bool    done  = false;
		while (xCurr && !done) {
			lookup.add(**xCurr, xCurr);
			if (xCurr == xTail)
				done = true;
			else
				xCurr = xCurr->getNext();
		}
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
	using base_t::get;
	using base_t::getData;


	/** @brief return true if @a elem is an element of this set
	  *
	  * @param[in] elem reference to the element to test
	  * @return true if the element is a member of this set, false otherwise
	**/
	virtual bool hasMember(const elem_t &elem) const noexcept
	{
		return (protFindData(*elem) ? true : false);
	}


	/** @brief return true if @a data is an element of this set
	  *
	  * @param[in] data reference to the data to test
	  * @return true if the data is a member of this set, false otherwise
	**/
	virtual bool hasMember(const data_t &data) const noexcept
	{
		return (protFindData(data) ? true : false);
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
		if ( eCount.load(memOrdLoad)
		  && (this != &src)) {
			if (src.eCount.load(memOrdLoad)) {
				PWX_DOUBLE_LOCK_GUARD(list_t, this, list_t, &src)
				elem_t* xCurr  = head();
				bool    isDone = false;

				// A simple loop will do, because we can use protFindData directly.
				while (result && xCurr && xCurr->data.get() && !isDone) {
					if (src.protFindData(**xCurr)) {
						if (xCurr == tail())
							isDone = true;
						else
							xCurr = xCurr->getNext();
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
		if (beThreadSafe()) {
			PWX_LOCK(this)
			if (this != &src) {
				while (!const_cast<list_t*>(&src)->try_lock()) {
					PWX_UNLOCK(this)
					PWX_LOCK(this)
				}
			}
		}

		// Now do the work
		clear();
		if (&src != this)
			destroy  = src.destroy;

		// Unlock if needed
		if (beThreadSafe()) {
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
	  * If there is no element to shift, nullptr is returned.
	  *
	  * @return the top element on the set.
	**/
	virtual elem_t* shift() noexcept
	{
		return privRemove(tail());
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
		PWX_TRY_PWX_FURTHER(return privInsDataBeforeElem (head(), data))
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
		PWX_TRY_PWX_FURTHER(return privInsElemBeforeElem (head(), src))
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
			PWX_DOUBLE_LOCK_GUARD (list_t, this, list_t, const_cast<list_t*> (&rhs))
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

	using base_t::beThreadSafe;
	using base_t::head;
	using base_t::tail;
	using base_t::destroy;


	/// @brief Search until the current element contains the searched data
	virtual const elem_t* protFind (const data_t* data) const noexcept
	{
		// Having a lookup hash this is simple,
		if (!data) return nullptr;
		elem_t* result = const_cast<elem_t*>(protFindData(*data));
		// But the *pointer* must match and not only the data
		return (result && result->data.get() == data) ? result : nullptr;
	}


	/// @brief Search until the current element contains the searched data content
	virtual const elem_t* protFindData (const data_t &data) const noexcept
	{
		// Very simple with a lookup hash:
		auto hashElem = lookup.get(data);
		return (hashElem ? hashElem->data.get() : nullptr);
	}


	/** @brief simple method to insert an element into the list
	  *
	  * If either @a insPrev or @a insElem is marked as destroyed,
	  * a pwx::CException is thrown. Such a condition implies that
	  * there is something seriously wrong.
	  *
	  * Note: As the private preparation methods that prepare the insert
	  *       have to lock the set to ensure consistency, this variant
	  *       is different from the one in TDoubleList as it does not
	  *       need to lock by itself.
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

		if (size()) {
			if (insPrev) {
				if (tail() == insPrev) {
					// Case 3: A new tail is to be set
					insElem->eNr.store(
						tail()->eNr.load(memOrdLoad) + 1,
						memOrdStore);
					PWX_TRY_PWX_FURTHER(tail()->insertNext(insElem, nullptr))
					tail(insElem);
				} else {
					// Case 4: A normal insert
					this->doRenumber.store(true, memOrdStore);
					PWX_TRY_PWX_FURTHER(insPrev->insertNext(insElem, nullptr))
				}
			} else {
				// Case 2: A new head is to be set
				PWX_TRY_PWX_FURTHER(head()->insertPrev(insElem, nullptr))
				head(insElem);
				this->doRenumber.store(true, memOrdStore);
			}
		} else {
			// Case 1: The list is empty
			PWX_TRY_PWX_FURTHER(insElem->insertBefore(nullptr, nullptr))
			head(insElem);
			tail(insElem);
		}

		// Do this last or non-locking methods would
		// find the element before it is really inserted.
		lookup.add(**insElem, insElem);

		eCount.fetch_add(1, memOrdStore);
		return eCount.load(memOrdLoad);
	}


	using base_t::protRenumber;


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	using base_t::eCount;
	using base_t::memOrdLoad;
	using base_t::memOrdStore;

private:
	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	/// @internal do nothing with the element. (lookup must not delete!)
	static void do_not_destroy(elem_t*) { }


	/// @brief clear this list
	/// Difference to TSingleList::privClear(): Here the lookup table
	/// must be maintained instead of the currStroe. It is therefore
	/// not possible to simply "declare" the Set as being empty.
	virtual void privClear() noexcept {
		elem_t* xTail = nullptr;
		while (tail()) {
			PWX_LOCK(this)
			xTail = privRemove(tail());
			PWX_UNLOCK(this)
			if (xTail && !xTail->destroyed()) {
				lookup.delKey(**xTail);
				delete xTail;
			}
		}
	}


	/// @brief Search until the next element contains the searched data
	virtual elem_t* privFindPrev (const data_t* data) const noexcept
	{
		elem_t* xCurr = const_cast<elem_t*>(protFind(data));
		return xCurr ? xCurr->getPrev() : nullptr;
	}


	/// @brief preparation method to insert data behind data
	virtual uint32_t privInsDataBehindData(data_t* prev, data_t* data)
	{
		// Return at once if the data is already known:
		if (!data || protFindData(*data))
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_NAMED_LOCK_GUARD(the_set, list_t, this)

		// Return at once if the data is now known the lock is in place:
		if (protFindData(*data))
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (prev && (nullptr == prevElement)) {
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this set")
		}

		// Now prevElement must not change any more
		PWX_NAMED_LOCK_GUARD(the_prev_element, elem_t, prevElement)

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe())
			newElement->disable_thread_safety();

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert data behind an element
	virtual uint32_t privInsDataBehindElem(elem_t* prev, data_t* data)
	{
		// Return at once if the data is already known:
		if (!data || protFindData(*data))
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_DOUBLE_LOCK_GUARD(list_t, this, elem_t, prev)

		// Return at once if the data is now known the lock is in place:
		if (protFindData(*data))
			return size();

		// 1: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe())
			newElement->disable_thread_safety();

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
	}


	/// @brief preparation method to insert an element copy behind data
	virtual uint32_t privInsElemBehindData(data_t* prev, const elem_t &src)
	{
		// Return at once if the source data is already known:
		if (protFindData(*src))
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_NAMED_DOUBLE_LOCK_GUARD(set_and_source, list_t, this, elem_t, &src)

		// Return at once if the source data is now known the lock is in place:
		if (protFindData(*src))
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (prev && (nullptr == prevElement)) {
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this singly linked list")
		}

		// Now prevElement must not change any more
		PWX_NAMED_LOCK_GUARD(the_prev_element, elem_t, prevElement)

		// 2: Check source:
		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (src))
		catch(std::exception &e) {
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}

		if (!this->beThreadSafe())
			newElement->disable_thread_safety();

		// 4: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind an element
	virtual uint32_t privInsElemBehindElem(elem_t* prev, const elem_t &src)
	{
		// Return at once if the source data is already known:
		if (protFindData(*src))
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_TRIPLE_LOCK_GUARD(list_t, this, elem_t, prev, elem_t, &src)

		// Return at once if the source data is now known the lock is in place:
		if (protFindData(*src))
			return size();

		// 1: Check source:
		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (src))
		catch(std::exception &e) {
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}

		if (!this->beThreadSafe())
			newElement->disable_thread_safety();

		// 4: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(prev, newElement))
	}


	/// @brief preparation method to insert data behind data
	virtual uint32_t privInsDataBeforeData(data_t* next, data_t* data)
	{
		// Return at once if the data is already known:
		if (!data || protFindData(*data))
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_NAMED_LOCK_GUARD(the_set, list_t, this)

		// Return at once if the data is now known the lock is in place:
		if (protFindData(*data))
			return size();

		// 1: Prepare the nextious element
		elem_t* nextElement = next ? const_cast<elem_t*>(protFind(next)) : nullptr;
		if (next && (nullptr == nextElement)) {
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this set")
		}
		elem_t* prevElement = nextElement ? nextElement->getPrev() : nullptr;


		// Now prevElement must not change any more
		PWX_NAMED_LOCK_GUARD(the_prev_element, elem_t, prevElement)

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe())
			newElement->disable_thread_safety();

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert data behind an element
	virtual uint32_t privInsDataBeforeElem(elem_t* next, data_t* data)
	{
		// Return at once if the data is already known:
		if (!data || protFindData(*data))
			return size();

		// It is important to have the previous element, not the next:
		elem_t* prevElement = next ? next->getPrev() : nullptr;

		// To ensure set consistency, a big lock is required:
		PWX_DOUBLE_LOCK_GUARD(list_t, this, elem_t, prevElement)

		// Return at once if the data is now known the lock is in place:
		if (protFindData(*data))
			return size();

		// 1: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe())
			newElement->disable_thread_safety();

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind data
	virtual uint32_t privInsElemBeforeData(data_t* next, const elem_t &src)
	{
		// Return at once if the source data is already known:
		if (protFindData(*src))
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_NAMED_DOUBLE_LOCK_GUARD(set_and_source, list_t, this, elem_t, &src)

		// Return at once if the source data is now known the lock is in place:
		if (protFindData(*src))
			return size();

		// 1: Prepare the nextious element
		elem_t* nextElement = next ? const_cast<elem_t*>(protFind(next)) : nullptr;
		if (next && (nullptr == nextElement)) {
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this singly linked list")
		}
		elem_t* prevElement = nextElement ? nextElement->getPrev() : nullptr;

		// Now prevElement must not change any more
		PWX_NAMED_LOCK_GUARD(the_next_element, elem_t, prevElement)

		// 2: Check source:
		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (src))
		catch(std::exception &e) {
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}

		if (!this->beThreadSafe())
			newElement->disable_thread_safety();

		// 4: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind an element
	virtual uint32_t privInsElemBeforeElem(elem_t* next, const elem_t &src)
	{
		// Return at once if the source data is already known:
		if (protFindData(*src))
			return size();

		// Use prev, not next:
		elem_t* prevElement = next ? next->getPrev() : nullptr;

		// To ensure set consistency, a big lock is required:
		PWX_TRIPLE_LOCK_GUARD(list_t, this, elem_t, prevElement, elem_t, &src)

		// Return at once if the source data is now known the lock is in place:
		if (protFindData(*src))
			return size();

		// 1: Check source:
		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (src))
		catch(std::exception &e) {
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}

		if (!this->beThreadSafe())
			newElement->disable_thread_safety();

		// 4: Do the real insert
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief simple method to remove an element from the list.
	virtual elem_t* privRemove (elem_t* elem) noexcept
	{
		// return at once if there is no element to remove
		if (!elem || elem->removed() || elem->destroyed())
			return nullptr;

		// Take out of lookup, so it isn't found any more:
		lookup.delKey(**elem);

		/* The following scenarios are possible:
		 * 1: elem is head
		 * 2: elem is tail
		 * 3: elem is something else.
		*/
		if (head() == elem) {
			// Case 1
			PWX_LOCK(this)
			/* The reasons for the double check are the same as
			 * with TSingleList::privRemoveNextElem()
			 */
			if (head() == elem)
				head(head()->getNext());
			this->doRenumber.store(true, memOrdStore);
			PWX_UNLOCK(this)
		} else if (tail() == elem) {
			// Case 2:
			PWX_LOCK(this)
			if (tail() == elem)
				tail(tail()->getPrev());
			PWX_UNLOCK(this)
		}
		else
			this->doRenumber.store(true, memOrdStore);
		elem->remove();

		if (1 == eCount.fetch_sub(1)) {
			// The list is empty!
			PWX_LOCK(this)
			// Is it really?
			if (0 == eCount.load(memOrdLoad)) {
				head(nullptr);
				tail(nullptr);
			}
			PWX_UNLOCK(this)
		}

		return elem;
	}


	/// @brief simple wrapper to prepare the direct removal of data
	virtual elem_t* privRemoveData(data_t* data) noexcept
	{
		return privRemove (data ? const_cast<elem_t*>(protFindData(*data)) : nullptr);
	}


	/** @brief remove the element after the specified data
	  *
	  * If @a prev data can not be found, nothing happens and nullptr is returned.
	  *
	  * Being a set this method searches for the data content and <U>not</U> the pointer!
	  *
	  * @return nullptr if the element holding @a prev is the last element or the list is empty.
	**/
	virtual elem_t* privRemoveAfterData(data_t* prev) noexcept
	{
		elem_t* xPrev = prev ? const_cast<elem_t*>(protFindData (*prev)) : nullptr;
		elem_t* toRemove = xPrev ? xPrev->getNext() : prev ? nullptr : head();

		if (toRemove)
			return privRemove (toRemove);

		return nullptr;
	}


	/** @brief simple wrapper to prepare the removal of an element before data
	  *
	  * If @a next data can not be found, nothing happens and nullptr is returned.
	  *
	  * Being a set this method searches for the data content and <U>not</U> the pointer!
	  *
	  * @return nullptr if @a next is held by the first element or the list is empty.
	**/
	virtual elem_t* privRemoveBeforeData(data_t* next) noexcept
	{
		elem_t* xNext = next ? const_cast<elem_t*>(protFindData(*next)) : nullptr;
		elem_t* toRemove = xNext ? xNext->getPrev() : next ? nullptr : tail();

		if (toRemove)
			return privRemove (toRemove);

		return nullptr;
	}


	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	hash_t lookup; //!< Quick search for data without having to traverse (and lock) the whole set.

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

