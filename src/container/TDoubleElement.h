#pragma once
#ifndef PWX_LIBPWX_TYPES_TDOUBLEELEMENT_H_INCLUDED
#define PWX_LIBPWX_TYPES_TDOUBLEELEMENT_H_INCLUDED 1

/** @file TDoubleElement.h
  *
  * @brief Declaration of a basic template for doubly linked list elements
  *
  * (c) 2007 - 2017 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
  *
  * The PrydeWorX Library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public License as
  * published by the Free Software Foundation; either version 2.1 of the
  * License, or (at your option) any later version.
  *
  * The PrydeWorX Library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public License
  * along with pwxLib; If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
**/


#include "VElement.h"
#include "MathHelpers.h"


namespace pwx {


/** @struct TDoubleElement
  *
  * @brief Template for an element of a doubly linked list or ring of variable types
  *
  * This is a very simple and basic type to wrap a pointer of variable type into
  * an object that is used with pwx::TDoubleList and pwx::TDoubleRing.
  *
  * The constructor takes an optional destroy(T*) function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * The data pointer itself is wrapped into an std::shared_ptr. It is therefore
  * completely safe to copy TSingleElement instances.
  *
  * The data pointer itself is public. You can use foo->data.get() to access it.
  * Further the operator* is overloaded and **foo will result in a reference to the
  * data.
  *
  * The next element in the list can be retrieved using the public foo->next pointer.
  * The previous element in the list can be retrieved using the public foo->prev pointer.
  *
  * If you plan to use this type in a multi-threaded environment, you can use the
  * getNext(), getPrev(), setNext() and setPrev() functions to manipulate the next and
  * prev pointers. See below for more on multi threaded usage.
  *
  * To insert any element into a list you can use insertNext() to have it inserted
  * after the called element or insertPrev() to have it inserted before the called
  * element safely.
  *
  * To remove an element from a list, you can use remove(), removeNext() and
  * removePrev() to have it removed safely.
  *
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * <B>Notes on multi threaded environments</B>
  *
  * See TSingleElement for the general discussion on multi threaded environments.
  *
  * <I>Critical work flows</I>
  *
  * The following work flows can be troublesome if multiple threads perform
  * concurrent tasks on an element. Additionally to the tasks below, the tasks
  * described in TSingleElement apply as well.
  * <TABLE border="1">
  * <TR>
  *   <TH>Task</TH><TH>Problematic action</TH><TH>Solution</TH>
  * </TR><TR>
  *   <TD>Retrieve previous element</TD><TD>Remove this element</TD>
  *   <TD>Elements know when they are removed. getPrev() then delivers the
  * previously stored pointer, if any.</TD>
  * </TR><TR>
  *   <TD>Retrieve previous element</TD><TD>Move element to different container</TD>
  *   <TD>This is not detectable, so never move an element. Remove and copy insert it!</TD>
  * </TR><TR>
  *   <TD>Insert an element before this</TD><TD>Either element destroyed by another thread</TD>
  *   <TD>insertPrev() will lock both this and the new previous element. Further it checks
  * whether any is destroyed and only inserts the element if both are not marked as destroyed.<BR />
  * If either is marked as destroyed, a pwx::CException is thrown, as those conditions imply
  * serious bugs.</TD>
  * </TR><TR>
  *   <TD>Remove the previous element</TD><TD>The previous element gets removed or another element is
  * inserted between the two elements by another thread</TD>
  *   <TD>removePrev() will try to lock both elements after one another in a Release->Yield->Lock
  * cycle until both are locked or the previous element changes. In the latter event the method does
  * not remove the element, as it is gone.<BR />
  * If the previous element goes away before it can be removed, a pwx::CException is thrown.</TD>
  * </TR><TR>
  *   <TD>Remove this element</TD><TD>Another thread removes this element, too</TD>
  *   <TD>remove() will check whether the element is still valid once all locks are acquired.
  * If the element got removed already, the method simply does nothing.</TD>
  * </TR>
  * </TABLE>
**/
template<typename data_t>
struct PWX_API TDoubleElement : public VElement
{

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/
	typedef VElement                base_t;     //!< Base type of this element
	typedef TDoubleElement<data_t>  elem_t;     //!< Type of this element
	typedef std::shared_ptr<data_t> share_t;    //!< data_t wrapped in std::shared_ptr
	typedef std::atomic<elem_t*>    neighbor_t; //!< elem_t* wrapped in std::atomic
	typedef base_t::store_t         store_t;    //!< The element store type to register this element with


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor sets both the data pointer and the destroy method.
	  *
	  * @param[in] data_ A pointer to the data this list element is to hold.
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TDoubleElement (data_t* data_, void (*destroy_) (data_t* data_)) noexcept
	: data (data_, TVarDeleter<data_t> (destroy_))
	{ }


	/** @brief explicit constructor
	  *
	  * Delegating ctor that calls the default ctor with destroy_ being the nullptr
	  *
	  * @param[in] data_ A pointer to the data this list element is to hold.
	**/
	explicit
	TDoubleElement (data_t* data_) noexcept
	: elem_t (data_, nullptr)
	{ }


	TDoubleElement() PWX_DELETE; // nullptr data is not allowed!


	/** @brief copy ctor
	  *
	  * The copy ctor creates a stand-alone element without neighbors
	  * copying the data pointer and destroy method from @a src.
	  * Further it will notice all elements holding the same pointer
	  * of its presence.
	  * As the data is wrapped in a shared_ptr, data will not get
	  * deleted unless the last reference is gone.
	  *
	  * <B>Important</B>: Whether the element does locking or not
	  * is *not* copied. It will silently be turned on by default!
	  *
	  * @param[in] src reference to the element to copy.
	**/
	TDoubleElement (const elem_t &src) noexcept
	: base_t (src),
	  data (src.data)
	{ }


	virtual ~TDoubleElement() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	/** @brief compare this element with some data and return -1, 0, +1
	  *
	  * This is a convenient method that safely compares this element to
	  * some data. If this elements data is larger than the other, the
	  * method returns 1. If both are equal it returns 0 and 1 if the
	  * other data is larger.
	  *
	  * This element get locked and checked against destruction and
	  * nullptr data.
	  *
	  * @param[in] other reference to the data to compare with
	  * @return +1 one if this data is larger, -1 if the other is larger, 0 if both are equal.
	**/
	int32_t compare(const data_t &other) const noexcept PWX_WARNUNUSED
	{
		if (&other != this->data.get()) {
			PWX_LOCK_GUARD(this)

			// A: Check destruction status
			if (this->destroyed()) return -1;

			// B: check Data status
			data_t* thisData = this->data.get();

			if (thisData) {
				if (isFloatType(data_t) && areAlmostEqual(*thisData, other))
					return 0;
				return *thisData > other ?  1
					 : other > *thisData ? -1 : 0;
			} else
				return -1;
		} // No else. compare(this->data.get()) always returns 0

		return 0;
	}


	/** @brief compare this element with another and return -1, 0, +1
	  *
	  * This is a convenient method that safely compares this element to
	  * another. If this elements data is larger than the others data,
	  * the method returns 1. If both are equal it returns 0 and 1 if
	  * the other elements data is larger.
	  *
	  * Both elements get locked and checked against destruction and
	  * nullptr data.
	  *
	  * @param[in] other pointer to the element to compare with
	  * @return +1 one if this data is larger, -1 if the other is larger, 0 if both are equal.
	**/
	int32_t compare(const elem_t* const other) const noexcept PWX_WARNUNUSED
	{
		if (other) {
			if (other != this) {
				PWX_DOUBLE_LOCK_GUARD(this, other)

				// A: Check destruction status
				bool thisDest = this->destroyed();
				bool otheDest = other->destroyed();

				if (thisDest && otheDest)	return  0;
				if (thisDest)				return -1;
				if (otheDest)				return  1;

				// B: check Data status
				data_t* thisData = this->data.get();
				data_t* otheData = other->data.get();

				if (thisData && otheData) {
					if (isFloatType(data_t) && areAlmostEqual(*thisData, *otheData))
						return 0;
					return *thisData > *otheData ?  1
						 : *otheData > *thisData ? -1 : 0;
				}
				if (thisData)	return  1;
				if (otheData)	return -1;
			} // No else. compare(this) always returns 0
		} else
			return 1; // The other is nullptr, this is always larger

		return 0;
	}


	/** @brief returns a pointer to the next element or nullptr if there is none.
	  *
	  * This method will use atomic::load() and is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * @return the next pointer or nullptr if there is none.
	**/
	elem_t* getNext() const noexcept PWX_WARNUNUSED
	{
		if (beThreadSafe()) {
			elem_t* curNext = next.load(memOrdLoad);
			if ( !curNext && removed() )
				return oldNext.load(memOrdLoad);
			return curNext;
		}
		return next.load(memOrdLoad);
	}


	/** @brief returns a pointer to the prev element or nullptr if there is none.
	  *
	  * This method will use atomic::load() and is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * @return the prev pointer or nullptr if there is none.
	**/
	elem_t* getPrev() const noexcept PWX_WARNUNUSED
	{
		if (beThreadSafe()) {
			elem_t* curPrev = prev.load(memOrdLoad);
			if ( !curPrev && removed() )
				return oldPrev.load(memOrdLoad);
			return curPrev;
		}
		return prev.load(memOrdLoad);
	}


	/** @brief insert an element before another
	  *
	  * This is a special insertion method that is to be used if
	  * this element is the first element of a container.
	  *
	  * if @a new_next is *not* nullptr or this element, the
	  * insertion will be done by its insertPrev() method.
	  *
	  * If either this or the @a new_next element is marked as destroyed,
	  * a pwx::CException is thrown. Such a condition implies that
	  * there is something seriously wrong.
	  *
	  * @param[in] new_next target where the next pointer should point at.
	  * @param[in] new_store optional pointer to the CThreadElementStore that will handle this element
	**/
	void insertBefore(elem_t* new_next, store_t* store)
	{
		if (!new_next || (new_next == this)) {
			insert(store);
			return;
		}

		PWX_TRY_PWX_FURTHER(new_next->insertPrev(this, store))
	}


	/** @brief insert an element after this element.
	  *
	  * This is an extra method to not only set the next pointer
	  * of this element, but the next and prev pointer of the
	  * inserted element, and the prev pointer of the old next
	  * element safely, too, in a multi-threaded environment.
	  *
	  * If any of this, the current next or the new element are marked
	  * as destroyed, a pwx::CException is thrown. Such a condition
	  * implies that there is something seriously wrong.
	  *
	  * If this element is marked as removed, it is simply assumed to
	  * be a single element in a container and will be marked as not
	  * removed by the insert.
	  *
	  * On the other hand, if @a new_next is either this element or
	  * nullptr, the method simply does nothing.
	  *
	  * @param[in] new_next target where the next pointer should point at.
	  * @param[in] new_store optional pointer to the CThreadElementStore that will handle this element
	**/
	void insertNext(elem_t* new_next, store_t* new_store)
	{
		if (!new_next || (new_next == this))
			return;

		if (beThreadSafe()) {
			// Do locking and double checks if this has to be thread safe
			if (!destroyed() && !new_next->destroyed()) {
				// Although it should not be necessary, a lock guard on
				// the new element is in order.
				PWX_LOCK_GUARD(new_next)

				elem_t* xOldNext = next.load(memOrdLoad);
				PWX_DOUBLE_LOCK_GUARD(this, xOldNext)

				/* The main challenge here is, that another thread might have
				 * changed the relationship between xOldNext and this element.
				 * We have to reset the lock guard as often as it takes to have
				 * both this and its true next to be locked.
				 */
				bool nextIsNext = xOldNext == next.load(memOrdLoad);
				while (!nextIsNext) {
					xOldNext = next.load(memOrdLoad);
					PWX_DOUBLE_LOCK_GUARD_RESET(this, xOldNext)
					nextIsNext = xOldNext == next.load(memOrdLoad);
				}

				// Check again to be sure not to handle destroyed elements
				if (destroyed())
					PWX_THROW("Illegal_Insert", "Destroyed elements can't insert",
							"The inserting element has been destroyed while waiting for the lock!")

				if (new_next->destroyed())
					PWX_THROW("Illegal_Insert", "Can't insert a destroyed element",
							"The element to insert has been destroyed while waiting for the lock!")

				// Before we can go ahead it is important to check the old
				// next , as it has to get its prev pointer manipulated
				if (xOldNext && xOldNext->destroyed())
					PWX_THROW("Illegal_Insert", "The next element is destroyed",
							"The next element has been destroyed while waiting for the lock!")

				// Insert the new element
				new_next->setNext(xOldNext);
				new_next->setPrev(this);
				new_next->insert(new_store);

				// Store new next and prev neighbor
				setNext(new_next);
				if (xOldNext)
					xOldNext->setPrev(new_next);
			}
			else if (destroyed())
				PWX_THROW("Illegal_Insert", "Destroyed elements can't insert",
						"Tried to insert an element after an already destroyed element!")
			else
				PWX_THROW("Illegal_Insert", "Can't insert a destroyed element",
						"Tried to insert an element that has already been destroyed!")
		} else {
			// Otherwise do it directly
			elem_t* xOldNext = next.load(memOrdLoad);
			new_next->next.store(xOldNext, memOrdStore);
			new_next->prev.store(this, memOrdStore);
			new_next->insert(new_store);
			next.store(new_next, memOrdStore);
			if (xOldNext)
				xOldNext->prev.store(new_next, memOrdStore);
		}
	}


	/** @brief insert an element before this element.
	  *
	  * This is an extra method to not only set the prev pointer
	  * of this element, but the next and prev pointer of the
	  * inserted element, and the next pointer of the old previous
	  * element safely, too, in a multi-threaded environment.
	  *
	  * If any of this, the current previous or the new element are
	  * marked as destroyed, a pwx::CException is thrown. Such a
	  * condition implies that there is something seriously wrong.
	  *
	  * If this element is marked as removed, it is simply assumed to
	  * be a single element in a container and will be marked as not
	  * removed by the insert.
	  *
	  * On the other hand, if @a new_prev is either this element or
	  * nullptr, the method simply does nothing.
	  *
	  * @param[in] new_prev target where the prev pointer should point at.
	  * @param[in] new_store optional pointer to the CThreadElementStore that will handle this element
	**/
	void insertPrev(elem_t* new_prev, store_t* new_store)
	{
		if (!new_prev || (new_prev == this))
			return;

		if (beThreadSafe()) {
			// Do locking and double checks if this has to be thread safe
			if (!destroyed() && !new_prev->destroyed()) {
				// The notes in insertNext() apply here as well
				PWX_LOCK_GUARD(new_prev)

				elem_t* xOldPrev = prev.load(memOrdLoad);
				PWX_DOUBLE_LOCK_GUARD(this, xOldPrev)

				bool prevIsPrev = xOldPrev == prev.load(memOrdLoad);
				while (!prevIsPrev) {
					xOldPrev = prev.load(memOrdLoad);
					PWX_DOUBLE_LOCK_GUARD_RESET(this, xOldPrev)
					prevIsPrev = xOldPrev == prev.load(memOrdLoad);
				}

				// Check again to be sure not to handle destroyed elements
				if (destroyed())
					PWX_THROW("Illegal_Insert", "Destroyed elements can't insert",
							"The inserting element has been destroyed while waiting for the lock!")

				if (new_prev->destroyed())
					PWX_THROW("Illegal_Insert", "Can't insert a destroyed element",
							"The element to insert has been destroyed while waiting for the lock!")

				// Before we can go ahead it is important to check the old
				// prev , as it has to get its prev pointer manipulated
				if (xOldPrev && xOldPrev->destroyed())
					PWX_THROW("Illegal_Insert", "The prev element is destroyed",
							"The prev element has been destroyed while waiting for the lock!")

				// Set the neighborhood of the new prev
				new_prev->setNext(this);
				new_prev->setPrev(xOldPrev);
				new_prev->insert(new_store);

				// Store new next and prev neighbor.
				setPrev(new_prev);
				if (xOldPrev)
					xOldPrev->setNext(new_prev);
			} else if (destroyed())
				PWX_THROW("Illegal_Insert", "Destroyed elements can't insert",
						"Tried to insert an element before an already destroyed element!")
			else
				PWX_THROW("Illegal_Insert", "Can't insert a destroyed element",
						"Tried to insert an element that has already been destroyed!")
		} else {
			// Otherwise do it directly
			elem_t* xOldPrev = prev.load(memOrdLoad);
			new_prev->prev.store(xOldPrev, memOrdStore);
			new_prev->next.store(this, memOrdStore);
			new_prev->insert(new_store);
			prev.store(new_prev, memOrdStore);
			if (xOldPrev)
				xOldPrev->next.store(new_prev, memOrdStore);
		}
	}


	/** @brief Remove this element from a list.
	  *
	  * Whenever you remove an element from a container you should
	  * call this method to tell it that it has been removed.
	  * Both the next and previous elements will be notified and
	  * the pointers to them set to nullptr.
	**/
	virtual void remove() noexcept
	{
		if (beThreadSafe()) {
			base_t::remove();

			// Do an acquiring test before the element is actually locked
			elem_t* xOldPrev = prev.load(memOrdLoad);
			elem_t* xOldNext = next.load(memOrdLoad);
			if (xOldPrev || xOldNext) {
				PWX_TRIPLE_LOCK_GUARD(this, xOldPrev, xOldNext)

				/* Here both the next and previous neighbor must be ensured
				 * to be consistent, or the container order would be utterly
				 * destroyed. A memory leak would be the result.
				 */
				bool prevIsPrev = xOldPrev == prev.load(memOrdLoad);
				bool nextIsNext = xOldNext == next.load(memOrdLoad);
				while (!prevIsPrev || !nextIsNext) {
					xOldPrev = prev.load(memOrdLoad);
					xOldNext = next.load(memOrdLoad);
					PWX_TRIPLE_LOCK_GUARD_RESET(this, xOldPrev, xOldNext)
					prevIsPrev = xOldPrev == prev.load(memOrdLoad);
					nextIsNext = xOldNext == next.load(memOrdLoad);
				}


				// 1: Handle previous neigbor
				if (xOldPrev && (xOldPrev != this))
					xOldPrev->setNext(xOldNext);

				// 2: Handle next neighbor
				if (xOldNext && (xOldNext != this))
					xOldNext->setPrev(xOldPrev);


			} // End of having at least one neighbor to handle

			// 3: Remove neighborhood:
			this->setPrev(nullptr);
			this->setNext(nullptr);
		} else {
			// No thread safety? Then just kick it out:
			elem_t* xOldNext = next.load(memOrdLoad);
			elem_t* xOldPrev = prev.load(memOrdLoad);

			if (xOldNext && (xOldNext != this))
				xOldNext->prev.store(xOldPrev, memOrdStore);

			if (xOldPrev && (xOldPrev != this))
				xOldPrev->next.store(xOldNext, memOrdStore);

			prev.store(nullptr, memOrdStore);
			next.store(nullptr, memOrdStore);

			base_t::remove();
		}
	}


	/** @brief remove the next element from a list.
	  *
	  * This method removes the successor of this element
	  * from a list in a thread safe way.
	  *
	  * @return the removed element
	**/
	elem_t* removeNext() noexcept
	{
		elem_t* toRemove = getNext();
		if (toRemove && (toRemove != this)) {
			toRemove->remove();
			return toRemove;
		}
		return nullptr;
	}


	/** @brief remove the previous element from a list.
	  *
	  * This method removes the predecessor of this element
	  * from a list in a thread safe way.
	  *
	  * @return the removed element
	**/
	elem_t* removePrev() noexcept
	{
		elem_t* toRemove = getPrev();
		if (toRemove && (toRemove != this)) {
			toRemove->remove();
			return toRemove;
		}
		return nullptr;
	}


	/** @brief set the next pointer to another element.
	  *
	  * This method will use atomic::store() and is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * <B>Important</B>: This method will *<B>NOT</B>* change the prev
	  * pointer of @a new_next!
	  *
	  * @param[in] new_next target where the next pointer should point at.
	**/
	void setNext(elem_t* new_next) noexcept
	{
		if (beThreadSafe()) {
			elem_t* currNext = next.load(memOrdLoad);
			next.store(new_next, memOrdStore);
			if (currNext)
				oldNext.store(currNext, memOrdStore);
		} else
			next.store(new_next, memOrdStore);
	}


	/** @brief set the prev pointer to another element.
	  *
	  * This method will use atomic::store() is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * <B>Important</B>: This method will *<B>NOT</B>* change the next
	  * pointer of @a new_prev!
	  *
	  * @param[in] new_prev target where the prev pointer should point at.
	**/
	void setPrev(elem_t* new_prev) noexcept
	{
		if (beThreadSafe()) {
			elem_t* currPrev = prev.load(memOrdLoad);
			prev.store(new_prev, memOrdStore);
			if (currPrev)
				oldPrev.store(currPrev, memOrdStore);
		} else
			prev.store(new_prev, memOrdStore);
	}


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief assignment operator
	  *
	  * The assignment operator copies over the element and
	  * the destroy method. This element will stay where it
	  * is, and not change its position.
	  *
	  * @param[in] src const reference of the element to copy
	  * @return reference to this element
	**/
	elem_t& operator= (const elem_t &src) noexcept
	{
		if ((this != &src) && !destroyed() && !src.destroyed()) {
			PWX_DOUBLE_LOCK_GUARD(this, &src)
			if (!destroyed() && !src.destroyed())
				data = src.data;
				// note: destroy method wrapped in data!
		}
	}


	/** @brief dereferencing an element returns a reference to the stored data
	  *
	  * If the data pointer is nullptr, a pwx::CException with the name
	  * "NullDataException" is thrown.
	  *
	  * @return a read/write reference to the stored data.
	**/
	data_t &operator*() PWX_WARNUNUSED
	{
		PWX_LOCK_GUARD(this)
		if (nullptr == data.get())
			PWX_THROW ( "NullDataException",
						"nullptr TDoubleElement<T>->data",
						"The data pointer to dereference is nullptr.")
		return *data;
	}


	/** @brief dereferencing a constant element returns a constant reference to the stored data
	  *
	  * If the data pointer is nullptr, a pwx::CException with the name
	  * "NullDataException" is thrown.
	  *
	  * @return a read only reference to the stored data.
	**/
	const data_t &operator*() const PWX_WARNUNUSED
	{
		PWX_LOCK_GUARD(this)
		if (nullptr == data.get())
			PWX_THROW ( "NullDataException",
						"nullptr TDoubleElement<T>->data",
						"The data pointer to dereference is nullptr.")
		return *data;
	}


	/** @brief return true if this element has the data @a data
	  * @param[in] data_ const reference of the data to check
	  * @return true if this element has the same data
	**/
	bool operator==(const data_t &data_) const noexcept PWX_WARNUNUSED
	{
		if (isFloatType(data_t))
			return areAlmostEqual(*data, data_);
		return *data == data_;
	}


	/** @brief return true if this element has differne data than @a data
	  * @param[in] data_ const reference of the data to check
	  * @return true if this element has different data
	**/
	bool operator!=(const data_t &data_) const noexcept PWX_WARNUNUSED
	{
		if (isFloatType(data_t))
			return !areAlmostEqual(*data, data_);
		return !(*data == data_);
	}


	/* ===============================================
	 * === Public members                          ===
	 * ===============================================
	*/

	share_t    data; 							//!< The data this list element points to, wrapped in a shared_ptr.
	neighbor_t next = ATOMIC_VAR_INIT(nullptr); //!< The next element in the list or nullptr if this is the tail
	neighbor_t prev = ATOMIC_VAR_INIT(nullptr); //!< The previous element in the list or nullptr if this is the head


protected:

	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	 */

	using base_t::isDestroyed;


private:

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	 */

	neighbor_t oldNext = ATOMIC_VAR_INIT(nullptr);
	neighbor_t oldPrev = ATOMIC_VAR_INIT(nullptr);

}; // struct TDoubleElement


/** @brief destructor
  *
  * The destructor invokes a lock on the instance to allow
  * other threads to react before the object itself is gone.
  *
  * Because of the usage of shared_ptr wrapping the data this
  * is only done if, and only if, this is the very last element
  * referencing this data.
**/
template<typename data_t>
TDoubleElement<data_t>::~TDoubleElement() noexcept
{
	if (beThreadSafe())
		isDestroyed.store(true);

	if (1 == data.use_count()) {
		if (beThreadSafe()) {
			// Lock the element before checking again.
			DEBUG_LOCK_STATE("PWX_LOCK", this, this)
			PWX_LOCK(this)
			// So the lock is only generated if there is a possibility
			// that we have to delete data, but another thread might
			// have made a copy in the mean time before "isDestroyed"
			// was finished setting to true.
			if (1 == data.use_count()) {
				PWX_TRY(data.reset()) // the shared_ptr will delete the data now
				catch(...) { }

				// Do a lock cycle, so that threads having had to wait while the data
				// was destroyed have a chance now to react before the object is gone.
				DEBUG_LOCK_STATE("PWX_UNLOCK", this, this)
				PWX_UNLOCK(this)
				DEBUG_LOCK_STATE("PWX_LOCK_GUARD", this, this)
				PWX_LOCK_GUARD(this)
			} else {
				DEBUG_LOCK_STATE("PWX_UNLOCK", this, this)
				PWX_UNLOCK(this)
			}
		} else {
			// No thread safety? Then just do it!
			PWX_TRY(data.reset())
			catch(...) { }
		}
	}
}


} // namespace pwx

#endif // PWX_LIBPWX_TYPES_TDOUBLEELEMENT_H_INCLUDED
