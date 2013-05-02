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
		return const_cast<elem_t* > (privFindData(static_cast<const data_t> (data), nullptr));
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
		return privFindData(data, nullptr);
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
		if ( eCount.load(PWX_MEMORDER_ACQUIRE)
		  && (this != &src)) {
			if (src.eCount.load(PWX_MEMORDER_ACQUIRE)) {
				PWX_DOUBLE_LOCK(list_t, const_cast<list_t*>(this), list_t, const_cast<list_t*>(&src))
				elem_t* xCurr  = head();
				bool    isDone = false;

				// A simple loop will do, because we can use privFindData directly.
				while (result && xCurr && !isDone) {
					if (src.privFindData(**xCurr, nullptr)) {
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
		if (this->beThreadSafe) {
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


	/** @brief simple method to insert an element into the list
	  *
	  * If either @a insPrev or @a insElem is marked as destroyed,
	  * a pwx::CException is thrown. Such a condition implies that
	  * there is something seriously wrong.
	  *
	  * This is the protInsert() method from TDoubleList without locks. All
	  * private insertion methods in TSet have to lock the set anyway.
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
		uint32_t locCnt = eCount.load(PWX_MEMORDER_ACQUIRE);

		curr(insElem);

		// If this is a sorted set, check consistency.
		// Note  : If we reach this point, the set is locked!
		// Note2 : Otherwise we have a problem!
#if defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)
#  if defined(PWX_THREADDEBUG)
#    define LOCAL_DEBUG_LOG THREAD_LOG
#  else
#    define LOCAL_DEBUG_LOG DEBUG_LOG
# endif
		if (this->beThreadSafe.load(PWX_MEMORDER_RELAXED) && (0 == this->lock_count()) )
			PWX_THROW("MissingLock", "TSet::protInsert() called without a lock in place!",
						"This is evil and must be fixed NOW!")
		if (isSorted && insPrev && insElem && (**insPrev >= **insElem)) {
			LOCAL_DEBUG_LOG("TSet", "insPrev: % 5d - \"%s\"",
						insPrev->eNr.load(),
						to_string(**insPrev).c_str())
			LOCAL_DEBUG_LOG("TSet", "insElem: % 5d - \"%s\"",
						insElem->eNr.load(),
						to_string(**insElem).c_str())
			PWX_THROW("BustedOrder", "insPrev is LARGER than insElem ? What the hell?",
						"This is evil ans must be fixed NOW!")
		}
		elem_t* xNext = insPrev ? insPrev->getNext() : nullptr;
		if (isSorted && xNext && insElem && (**insElem >= **xNext)) {
			LOCAL_DEBUG_LOG("TSet", "insElem: % 5d - \"%s\"",
						insElem->eNr.load(),
						to_string(**insElem).c_str())
			LOCAL_DEBUG_LOG("TSet", "xNext  : % 5d - \"%s\"",
						insPrev->eNr.load(),
						to_string(**insPrev).c_str())
			PWX_THROW("BustedOrder", "insElem is LARGER than insP ? What the hell?",
						"This is evil ans must be fixed NOW!")
		}
#  undef LOCAL_DEBUG_LOG
#endif // defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)

		if (locCnt && insPrev && (tail() != insPrev)) {
			// Case 4: A normal insert
			doRenumber.store(true, PWX_MEMORDER_RELEASE);
			PWX_TRY_PWX_FURTHER(insPrev->insertNext(insElem))
		} else {
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

		// Raise eCount and set renumbering mode
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
	  * If the set is sorted and @a data can not be found, then @a start
	  * points to the element which would precede an element holding @a data
	  * if it where present. With this special outcome the inserting methods
	  * can simply use insNextElem() with @a start to add data in a sorted way.
	  * The only detail to look at is the situation when the new element
	  * must become the new head. In this special case, <B>@a start is set to
	  * nullptr</B>.
	  *
	  * If @a start is set to nullptr, it simply isn't used and the search
	  * starts with <I>curr</I>.
	  *
	  * Important: Although this method uses <I>curr</I> if @a start points to
	  * nullptr, <I>curr</I> is not set here.
	  *
	  * @param[in] data reference of the data to search.
	  * @param[in,out] start pointer pointer from where to start, stores where to end.
	  * @return pointer to the element or nullptr if @a data is not present in the set.
	**/
	const elem_t* privFindData (const data_t &data, elem_t** start,
								bool reentered = false) const noexcept
	{
#define PFD_SET_START(new_start) { \
			if (start) { \
				*start = new_start; \
			} \
		}
		// Note: Methods that call privFindData() to prepare an insertion should
		//       search again after start is adjusted and a lock is acquired.
		uint32_t localCount = eCount.load(PWX_MEMORDER_ACQUIRE);
		elem_t*  xCurr      = (start && *start) ? *start : curr() ? curr() : head();
		elem_t*  xOld       = nullptr; // To fix thread concurrency

		/* Note:
		 * When the set is sorted, we can make some quick exit assumptions:
		 * 1: If head is larger, data can not be in the set
		 * 2: If tail is smaller, data can not be in the set
		*/
		if (localCount) {
			// Quick exit if sorted set assumption 1 is correct:
			if (isSorted && (**head() > data)) {
				PFD_SET_START(nullptr)
				return nullptr;
			}

			// Quick exit if xCurr is correct:
			if (**xCurr == data)
				return xCurr;

			// Quick exit if head is wanted:
			if (**head() == data)
				return head();
			// End of having at least one element
		} else
			// return immediately if there are no elements
			return nullptr;

		// Checking tail does only make sense if we have at least 2 element
		if (localCount > 1) {
			// Quick exit if sorted set assumption 2 is correct:
			if (isSorted && (data > **tail())) {
				PFD_SET_START(tail())
				return nullptr;
			}

			// Quick exit if tail is wanted:
			if (**tail() == data)
				return tail();
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
				 * about this is the absence of any need to check xCurr against
				 * head or tail.
				 * Pitfalls: If this is a multi-threaded environment there are some
				 * nasty issues that can arise:
				 * A) If data is larger than head but smaller than heads next
				 *    element, and another thread removes head just now, xCurr
				 *    will end up being nullptr when going down.
				 * B) The same happens with tail vice versa.
				*/

				// Step 1: Move up until data is no longer larger than **xCurr
				bool doStop = false;
				while (!doStop && xCurr && (data > **xCurr)) {
					if (tail() == xCurr)
						doStop = true;
					else {
						xOld  = xCurr;
						xCurr = xCurr->getNext();
						if (nullptr == xCurr)
							// Should not happen unless someone does something stupid
							// with different threads. (like torture does ;))
							xCurr = xOld ? xOld : head();
					}
				} // End of wandering up

				// Step 2: check whether we reached tail and assumption 2 is suddenly valid
				if (doStop && (data > **tail())) {
					PFD_SET_START(tail())
					return nullptr;
				}

				// Step 3: Check whether xCurr was busted
				if (nullptr == xCurr) xCurr = tail(); // Might happen due to thread concurrency

				// Step 4: Go down until data is no longer smaller than **xCurr
				doStop = false;
				while (!doStop && xCurr && (**xCurr > data)) {
					if (head() == xCurr)
						doStop = true;
					else {
						xOld  = xCurr;
						xCurr = xCurr->getPrev();
						if (nullptr == xCurr)
							xCurr = xOld ? xOld : tail(); // dito
					}
				} // End of wandering down

				// Step 5: check whether we reached head and assumption 1 is suddenly valid
				if (doStop && (**head() > data)) {
					PFD_SET_START(nullptr)
					return nullptr;
				}

				// Step 6: Check that we really really reached a valid end, or re-enter with a lock:
				elem_t* xNext = xCurr ? xCurr->getNext() : nullptr;
				elem_t* xPrev = xCurr ? xCurr->getPrev() : nullptr;
				if (xCurr && ( (xPrev && (**xPrev > data)) || (xNext && (data > **xNext)) ) ) {
					// This is bad. Something busted the set!
					if (reentered) {
						// This is FUBAR! No exception can fix this!

#ifndef LIBPWX_DEBUG
						// I know that it says "noexcept". This is intentional
						// to have at least some message and a possible std::terminate()
						PWX_THROW("Broken_TSet",
								"TSet is FUBAR! Please enable DEBUG in the Makefile",
								"TSet is FUBAR! Please enable DEBUG in the Makefile")
#else
#  define GET_OBJ_ENR(obj) obj ? obj->eNr.load() : -1
#  define GET_OBJ_DAT(obj) obj ? to_string(**(obj)).c_str() : "nullptr"
#  define GET_OBJ_ALL(obj) GET_OBJ_ENR(obj), GET_OBJ_DAT(obj)

						protRenumber();

						DEBUG_LOG("TSet", "Double recursion detected with %d locks",
										this->lock_count())
						DEBUG_LOG("TSet", "Searching for data \"%s\"",
										to_string(data).c_str())
						DEBUG_LOG("TSet", "head : nr % 5d, data \"%s\"", GET_OBJ_ALL(head()))
						DEBUG_LOG("TSet", "start: nr % 5d, data \"%s\"",
									(start && *start) ? (*start)->eNr.load() : -1,
									(start && *start) ? to_string(**(*start)).c_str() : "nullptr")
						elem_t* xShowPrev = xPrev;
						for (int32_t i = 1; i < 5 ; ++i)
							xShowPrev = (xPrev && xPrev->getPrev()) ? xPrev->getPrev() : xPrev;
						while (xShowPrev && (xShowPrev != xPrev)) {
							DEBUG_LOG("TSet", "xPrev: nr % 5d, data \"%s\"", GET_OBJ_ALL(xShowPrev))
							xShowPrev = xShowPrev->getNext();
						}
						DEBUG_LOG("TSet", "xPrev: nr % 5d, data \"%s\"", GET_OBJ_ALL(xPrev))
						DEBUG_LOG("TSet", "xCurr: nr % 5d, data \"%s\"", GET_OBJ_ALL(xCurr))
						DEBUG_LOG("TSet", "xNext: nr % 5d, data \"%s\"", GET_OBJ_ALL(xNext))
						elem_t* xShowNext = xNext;
						for (int32_t i = 1; i < 5; ++i) {
							if (xShowNext->getNext()) {
								xShowNext = xShowNext->getNext();
								DEBUG_LOG("TSet", "xNext: nr % 5d, data \"%s\"", GET_OBJ_ALL(xShowNext))
							}
						}
						DEBUG_LOG("TSet", "tail : nr % 5d, data \"%s\"", GET_OBJ_ALL(tail()))

#  undef GET_OBJ_ALL
#  undef GET_OBJ_DAT
#  undef GET_OBJ_ENR

						PWX_THROW("Broken_TSet",
								"The order is busted",
								"Please investigate which of the above elements is wrong")
#endif // LIBPWX_DEBUG
						// The system *should* have terminated now, but make it sure to happen:
						std::terminate();
					} // is busted? Otherwise another thread simply interfered

					PWX_LOCK_GUARD(list_t, const_cast<list_t*>(this))

					if (start && (*start == xCurr) )
						*start = head();
					else if (!start)
						curr(head()); // Used if no start pointer given

					return this->privFindData(data, start, true);
				}
			} else {
				xCurr = head()->getNext();

				// Note: head and tail are already checked.
				while (xCurr && (xCurr != tail() ) && (**xCurr != data)) {
					xOld  = xCurr;
					xCurr = xCurr->getNext();
					if (nullptr == xCurr)
						xCurr = xOld ? xOld : head(); // dito
				}
			}
			/* Due to this order xCurr is now either pointing to an element
			 * holding data, or the next smaller element. The latter detail
			 * is important for the sorted insertion. If data is not found,
			 * all inserting methods can now insert a new element holding
			 * the searched data using insNextElem() on curr.
			*/
			if (xCurr && (**xCurr == data))
				return xCurr;
			else {
				PFD_SET_START(xCurr)
				return nullptr;
			}
		} // End of having at least 3 elements

#undef PFD_SET_START

		return nullptr;
	}


	/// @brief preparation method to insert data behind data
	virtual uint32_t privInsDataBehindData(data_t* prev, data_t* data)
	{
		elem_t* xCurr = curr();

		if (privFindData(*data, &xCurr))
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? xCurr : prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (!isSorted && prev && (nullptr == prevElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this set")


		// 2: All preparation methods check twice and use big locks to ensure data consistency!
		PWX_LOCK_GUARD(list_t, this)
		// Now prevElement must not change any more
		if (prevElement)
			PWX_LOCK(prevElement)
		// And search again
		if (privFindData(*data, &xCurr)) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			return size();
		} else if (isSorted && (prevElement != xCurr)) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			prevElement = xCurr;
			if (prevElement)
				PWX_LOCK(prevElement)
		}


		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 4: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert data behind an element
	virtual uint32_t privInsDataBehindElem(elem_t* prev, data_t* data)
	{
		elem_t* xCurr = curr();

		if (privFindData(*data, &xCurr))
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? xCurr : prev;

		// 2: All preparation methods check twice and use big locks to ensure data consistency!
		PWX_LOCK_GUARD(list_t, this)
		// Now prevElement must not change any more
		if (prevElement)
			PWX_LOCK(prevElement)
		// And search again
		if (privFindData(*data, &xCurr)) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			return size();
		} else if (isSorted && (prevElement != xCurr)) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			prevElement = xCurr;
			if (prevElement)
				PWX_LOCK(prevElement)
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 4: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind data
	virtual uint32_t privInsElemBehindData(data_t* prev, const elem_t &src)
	{
		elem_t* xCurr = curr();

		if (privFindData(*src, &xCurr))
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? xCurr : prev ? const_cast<elem_t*>(protFind(prev)) : nullptr;
		if (!isSorted && prev && (nullptr == prevElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this singly linked list")

		// 2: Check source:
		PWX_LOCK_GUARD(list_t, this)
		// Now prevElement must not change any more
		if (prevElement)
			PWX_LOCK(prevElement)
		PWX_LOCK(const_cast<elem_t*>(&src))

		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: All preparation methods check twice and use big locks to ensure data consistency!
		if (privFindData(*src, &xCurr)) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			return size();
		} else if (isSorted && (prevElement != xCurr)) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			prevElement = xCurr;
			if (prevElement)
				PWX_LOCK(prevElement)
		}

		// 4: Create a new element
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

		// 5: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert an element copy behind an element
	virtual uint32_t privInsElemBehindElem(elem_t* prev, const elem_t &src)
	{
		elem_t* xCurr = curr();

		if (privFindData(*src, &xCurr))
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = isSorted ? xCurr : prev;
		if (prevElement)
			PWX_LOCK(prevElement)

		// 2: Check source:
		PWX_LOCK_GUARD(list_t, this)
		// Now prevElement must not change any more
		if (prevElement)
			PWX_LOCK(prevElement)
		PWX_LOCK(const_cast<elem_t*>(&src))

		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: All preparation methods check twice and use big locks to ensure data consistency!
		if (privFindData(*src, &xCurr)) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			return size();
		} else if (isSorted && (prevElement != xCurr)) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			prevElement = xCurr;
			if (prevElement)
				PWX_LOCK(prevElement)
		}

		// 4: Create a new element
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

		// 5: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement, newElement))
	}


	/// @brief preparation method to insert data before data
	virtual uint32_t privInsDataBeforeData(data_t* next, data_t* data)
	{
		elem_t* xCurr = curr();

		if (privFindData(*data, &xCurr))
			return size();


		// 1: Prepare the next element
		elem_t* prevElement = xCurr; // Will become the pointer for protInsert() if sorted
		elem_t* nextElement = !isSorted && next ? const_cast<elem_t*>(find(next)) : nullptr;
		if (!isSorted && next && (nullptr == nextElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this doubly linked list")

		// 2: All preparation methods check twice and use big locks to ensure data consistency!
		PWX_LOCK_GUARD(list_t, this)

		if (!isSorted && nextElement)
			PWX_LOCK(nextElement)
		if (isSorted && prevElement)
			PWX_LOCK(prevElement)

		if (privFindData(*data, &xCurr)) {
			if (!isSorted && nextElement)
				PWX_UNLOCK(nextElement)
			if (isSorted && prevElement)
				PWX_UNLOCK(prevElement)
			return size();
		} else if (isSorted) {
			if (xCurr != prevElement) {
				if (prevElement)
					PWX_UNLOCK(prevElement)
				prevElement = xCurr;
				if (prevElement)
					PWX_LOCK(prevElement)
			}
		} else if (nextElement) {
			PWX_UNLOCK(nextElement)
			prevElement = nextElement->getPrev();
			if (prevElement)
				PWX_LOCK(prevElement)
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 4: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement ? prevElement : tail(), newElement))
	}


	/// @brief preparation method to insert data before an element
	virtual uint32_t privInsDataBeforeElem(elem_t* next, data_t* data)
	{
		elem_t* xCurr = curr();

		if (privFindData(*data, &xCurr))
			return size();

		// 1: Prepare the next element
		elem_t* prevElement = xCurr; // Will become the pointer for protInsert() if sorted
		elem_t* nextElement = !isSorted && next ? next : nullptr;

		// 2: All preparation methods check twice and use big locks to ensure data consistency!
		PWX_LOCK_GUARD(list_t, this)

		if (!isSorted && nextElement)
			PWX_LOCK(nextElement)
		if (isSorted && prevElement)
			PWX_LOCK(prevElement)

		if (privFindData(*data, &xCurr)) {
			if (!isSorted && nextElement)
				PWX_UNLOCK(nextElement)
			if (isSorted && prevElement)
				PWX_UNLOCK(prevElement)
			return size();
		} else if (isSorted) {
			if (xCurr != prevElement) {
				if (prevElement)
					PWX_UNLOCK(prevElement)
				prevElement = xCurr;
				if (prevElement)
					PWX_LOCK(prevElement)
			}
		} else if (nextElement) {
			PWX_UNLOCK(nextElement)
			prevElement = nextElement->getPrev();
			if (prevElement)
				PWX_LOCK(prevElement)
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (data, destroy))
		catch(std::exception &e) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 4: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement ? prevElement : tail(), newElement))
	}


	/// @brief preparation method to insert an element copy before data
	virtual uint32_t privInsElemBeforeData(data_t* next, const elem_t &src)
	{
		elem_t* xCurr = curr();

		if (privFindData(*src, &xCurr))
			return size();

		// 1: Prepare the next element
		elem_t* prevElement = xCurr; // Will become the pointer for protInsert() if sorted
		elem_t* nextElement = !isSorted && next ? const_cast<elem_t*>(find(next)) : nullptr;
		if (!isSorted && next && (nullptr == nextElement))
			PWX_THROW ("ElementNotFound",
					   "Element not found",
					   "The searched element can not be found in this doubly linked list")

		// 2: Check source:
		PWX_LOCK_GUARD(list_t, this)
		if (!isSorted && nextElement)
			PWX_LOCK(nextElement)
		if (isSorted && prevElement)
			PWX_LOCK(prevElement)
		PWX_LOCK(const_cast<elem_t*>(&src))

		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (!isSorted && nextElement)
				PWX_UNLOCK(nextElement)
			if (isSorted && prevElement)
				PWX_UNLOCK(prevElement)
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: All preparation methods check twice and use big locks to ensure data consistency!
		if (privFindData(*src, &xCurr)) {
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (!isSorted && nextElement)
				PWX_UNLOCK(nextElement)
			if (isSorted && prevElement)
				PWX_UNLOCK(prevElement)
			return size();
		} else if (isSorted) {
			if (xCurr != prevElement) {
				if (prevElement)
					PWX_UNLOCK(prevElement)
				prevElement = xCurr;
				if (prevElement)
					PWX_LOCK(prevElement)
			}
		} else if (nextElement) {
			PWX_UNLOCK(nextElement)
			prevElement = nextElement->getPrev();
			if (prevElement)
				PWX_LOCK(prevElement)
		}

		// 4: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (src))
		catch(std::exception &e) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 5: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement ? prevElement : tail(), newElement))
	}


	/// @brief preparation method to insert an element copy before an element
	virtual uint32_t privInsElemBeforeElem(elem_t* next, const elem_t &src)
	{
		elem_t* xCurr = curr();

		if (privFindData(*src, &xCurr))
			return size();

		// 1: Prepare the next element
		elem_t* prevElement = xCurr; // Will become the pointer for protInsert() if sorted
		elem_t* nextElement = !isSorted && next ? next : nullptr;

		// 2: Check source:
		PWX_LOCK_GUARD(list_t, this)
		if (!isSorted && nextElement)
			PWX_LOCK(nextElement)
		if (isSorted && prevElement)
			PWX_LOCK(prevElement)
		PWX_LOCK(const_cast<elem_t*>(&src))

		if (src.destroyed()) {
			// What on earth did the caller think?
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (!isSorted && nextElement)
				PWX_UNLOCK(nextElement)
			if (isSorted && prevElement)
				PWX_UNLOCK(prevElement)
			PWX_THROW("Illegal Condition", "Source element destroyed",
					  "An element used as source for insertion is destroyed.")
		}

		// 3: All preparation methods check twice and use big locks to ensure data consistency!
		if (privFindData(*src, &xCurr)) {
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			if (!isSorted && nextElement)
				PWX_UNLOCK(nextElement)
			if (isSorted && prevElement)
				PWX_UNLOCK(prevElement)
			return size();
		} else if (isSorted) {
			if (xCurr != prevElement) {
				if (prevElement)
					PWX_UNLOCK(prevElement)
				prevElement = xCurr;
				if (prevElement)
					PWX_LOCK(prevElement)
			}
		} else if (nextElement) {
			PWX_UNLOCK(nextElement)
			prevElement = nextElement->getPrev();
			if (prevElement)
				PWX_LOCK(prevElement)
		}

		// 4: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY(newElement = new elem_t (src))
		catch(std::exception &e) {
			if (prevElement)
				PWX_UNLOCK(prevElement)
			PWX_UNLOCK(const_cast<elem_t*>(&src))
			PWX_THROW("ElementCreationFailed", e.what(), "The Creation of a new list element failed.")
		}
		if (!this->beThreadSafe.load(PWX_MEMORDER_RELAXED))
			newElement->disable_thread_safety();

		// 5: Do the real insert
		if (prevElement)
			PWX_UNLOCK(prevElement)
		PWX_TRY_PWX_FURTHER(return protInsert(prevElement ? prevElement : tail(), newElement))
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

