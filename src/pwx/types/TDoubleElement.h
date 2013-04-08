#pragma once
#ifndef PWX_LIBPWX_TYPES_TDOUBLEELEMENT_H_INCLUDED
#define PWX_LIBPWX_TYPES_TDOUBLEELEMENT_H_INCLUDED 1

/** @file TDoubleElement.h
  *
  * @brief Declaration of a basic template for doubly linked list elements
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

#include "pwx/base/VElement.h"

namespace pwx
{

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
  * If the element already got removed, the method simply does nothing.</TD>
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
	  * is *not* copied. It will silently turned on by default!
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

	/** @brief returns a pointer to the next element or nullptr if there is none.
	  *
	  * This method will use atomic::load() and is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * @return the next pointer or nullptr if there is none.
	**/
	elem_t* getNext() const noexcept
	{
		if ( isRemoved.load(std::memory_order_acquire) )
			return oldNext.load(std::memory_order_acquire);
		return next.load(std::memory_order_acquire);
	}


	/** @brief returns a pointer to the prev element or nullptr if there is none.
	  *
	  * This method will use atomic::load() and is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * @return the prev pointer or nullptr if there is none.
	**/
	elem_t* getPrev() const noexcept
	{
		if ( isRemoved.load(std::memory_order_acquire) )
			return oldPrev.load(std::memory_order_acquire);
		return prev.load(std::memory_order_acquire);
	}


	/** @brief tell the element that it has been inserted.
	  *
	  * If an element becomes the last or only element of a container,
	  * it has no next element. This method can be used to tell the
	  * element that it is stored in a container even without a next
	  * pointer set.
	**/
	void insert() const noexcept
	{
		isRemoved.store(false, std::memory_order_release);
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
	**/
	void insertNext(elem_t* new_next)
	{
		if (!new_next || (new_next == this))
			return;

		if (!destroyed() && !new_next->destroyed()) {
			PWX_DOUBLE_LOCK(elem_t, this, elem_t, new_next)

			/* Now that we have the double lock, it is crucial to
			 * check again. Otherwise we might just insert a destroyed element.
			*/
			if (destroyed())
				PWX_THROW("Illegal_Insert", "Destroyed elements can't insert",
						"The inserting element has been destroyed while waiting for the lock!")

			if (new_next->destroyed())
				PWX_THROW("Illegal_Insert", "Can't insert a destroyed element",
						"The element to insert has been destroyed while waiting for the lock!")

			// Safe old next pointer.
			elem_t* xOldNext = this->getNext();

			// Before we can go ahead it is important to check and lock the
			// old next, as it has to get its prev pointer manipulated
			if (xOldNext) {
				PWX_LOCK(xOldNext)
				// Check its status, it must not be destroyed either
				if (xOldNext->destroyed()) {
					PWX_UNLOCK(xOldNext)
					PWX_THROW("Illegal_Insert", "The next element is destroyed",
							"The next element has been destroyed while waiting for the lock!")
				}
			}

			// Set the neighborhood of the new next
			new_next->next.store(xOldNext, std::memory_order_relaxed);
			new_next->prev.store(this, std::memory_order_relaxed);

			// Store new next and prev neighbor.
			setNext(new_next);
			if (xOldNext) {
				xOldNext->setPrev(new_next);
				PWX_UNLOCK(xOldNext)
			}
		} else if (destroyed())
			PWX_THROW("Illegal_Insert", "Destroyed elements can't insert",
					"Tried to insert an element after an already destroyed element!")
		else
			PWX_THROW("Illegal_Insert", "Can't insert a destroyed element",
					"Tried to insert an element that has already been destroyed!")
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
	**/
	void insertPrev(elem_t* new_prev)
	{
		if (!new_prev || (new_prev == this))
			return;

		if (!destroyed() && !new_prev->destroyed()) {
			PWX_DOUBLE_LOCK(elem_t, this, elem_t, new_prev)

			/* Now that we have the double lock, it is crucial to
			 * check again. Otherwise we might just insert a destroyed element.
			*/
			if (destroyed())
				PWX_THROW("Illegal_Insert", "Destroyed elements can't insert",
						"The inserting element has been destroyed while waiting for the lock!")

			if (new_prev->destroyed())
				PWX_THROW("Illegal_Insert", "Can't insert a destroyed element",
						"The element to insert has been destroyed while waiting for the lock!")

			// Safe old prev pointer.
			elem_t* xOldPrev = this->getPrev();

			// Before we can go ahead it is important to check and lock the
			// old prev , as it has to get its next pointer manipulated
			if (xOldPrev) {
				PWX_LOCK(xOldPrev)
				// Check its status, it must not be destroyed either
				if (xOldPrev->destroyed()) {
					PWX_UNLOCK(xOldPrev)
					PWX_THROW("Illegal_Insert", "The previous element is destroyed",
							"The previous element has been destroyed while waiting for the lock!")
				}
			}

			// Set the neighborhood of the new prev
			new_prev->next.store(this, std::memory_order_relaxed);
			new_prev->prev.store(xOldPrev, std::memory_order_relaxed);

			// Store new next and prev neighbor.
			setPrev(new_prev);
			if (xOldPrev) {
				xOldPrev->setNext(new_prev);
				PWX_UNLOCK(xOldPrev)
			}
		} else if (destroyed())
			PWX_THROW("Illegal_Insert", "Destroyed elements can't insert",
					"Tried to insert an element before an already destroyed element!")
		else
			PWX_THROW("Illegal_Insert", "Can't insert a destroyed element",
					"Tried to insert an element that has already been destroyed!")
	}


	/** @brief Remove this element from a list.
	  *
	  * Whenever you remove an element from a container you should
	  * call this method to tell it that it has been removed.
	  * Both the next and previous elements will be notified and
	  * the pointers to them set to nullptr.
	**/
	void remove() const noexcept
	{
		// Do an acquiring test before the element is actually locked
		if (next.load(std::memory_order_acquire) || prev.load(std::memory_order_acquire)) {
			PWX_LOCK(this)
			elem_t* xOldPrev = nullptr;
			elem_t* xOldNext = nullptr;

			/* The challenge here is to do this without deadlocks.
			 * Basically the neighbors need to be locked in turn.
			 * But if another thread has a lock on one of those
			 * waiting to get a lock on this, a deadlock will happen.
			 * The solution is to go some try_lock() lengths until
			 * everybody is happy.
			 * Further more while we yield lock-free, another thread
			 * might have just removed our next or previous neighbor.
			 * It is therefore necessary to always use getNext() and
			 * getPrev() to have the real current neighbor.
			 */

			// 1: Handle previous neighbor
			while ((xOldPrev = this->getPrev()) && !xOldPrev->try_lock()) {
				// xOldPrev is valid, but we can not lock.
				PWX_UNLOCK(this)
				std::this_thread::yield();
				PWX_LOCK(this)
			}

			// If xOldPrev is valid now, it is also locked:
			if (xOldPrev) {
				if (xOldPrev->getNext() == this)
					// Still points to this, so make it point to next instead
					xOldPrev->setNext(this->getNext());
				xOldPrev->unlock();
				xOldPrev = nullptr;
			}

			// 2: Handle next neighbor
			while ((xOldNext = this->getNext()) && !xOldNext->try_lock()) {
				// xOldNext is valid, but we can not lock.
				PWX_UNLOCK(this)
				std::this_thread::yield();
				PWX_LOCK(this)
			}

			// If xOldNext is valid now, it is also locked:
			if (xOldNext) {
				if (xOldNext->getPrev() == this)
					// Still points to this, so make it point to prev instead
					xOldNext->setPrev(this->getPrev());
				xOldNext->unlock();
				xOldNext = nullptr;
			}

			// 3: Remove neighborhood:
			this->setPrev(nullptr);
			this->setNext(nullptr);
			this->isRemoved.store(true, std::memory_order_release);
			PWX_UNLOCK(this)
		} // End of having at least one neighbor to handle
	}


	/** @brief remove the next element from a list.
	  *
	  * This method removes the successor of this element
	  * from a list in a thread safe way.
	  *
	  * If the next element gets moved or removed while this
	  * thread waits for the lock, a pwx::CException is thrown.
	**/
	void removeNext()
	{
		elem_t* toRemove = nullptr;

		// Do an acquiring test before the element is actually locked
		if ( (toRemove = this->getNext()) ) {
			PWX_LOCK_GUARD(elem_t, toRemove)

			/* See notes in TDoubleElement::remove() */

			// Lock this, the former previous neighbor
			while ((this->getNext() == toRemove) && !this->try_lock()) {
				// Can't lock this, so yield until possible
				PWX_UNLOCK(toRemove)
				std::this_thread::yield();
				PWX_LOCK(toRemove)
			}

			// Now if next still points to toRemove, this is locked:
			if (this->getNext() == toRemove)
				toRemove->remove();
			else {
				// Otherwise toRemove went away, which is bad
				PWX_UNLOCK(toRemove)
				PWX_THROW("Illegal_Remove", "Next element to remove went away",
						"A next element to remove went away while waiting for the lock!")
			}
		} // End of having a next element to remove
	}


	/** @brief remove the previous element from a list.
	  *
	  * This method removes the predecessor of this element
	  * from a list in a thread safe way.
	  *
	  * If the previous element gets moved or removed while this
	  * thread waits for the lock, a pwx::CException is thrown.
	**/
	void removePrev()
	{
		elem_t* toRemove = nullptr;

		// Do an acquiring test before the element is actually locked
		if ( (toRemove = this->getPrev()) ) {
			PWX_LOCK_GUARD(elem_t, toRemove)

			/* See notes in TDoubleElement::remove() */

			// Lock this, the former next neighbor
			while ((this->getPrev() == toRemove) && !this->try_lock()) {
				// Can't lock this, so yield until possible
				PWX_UNLOCK(toRemove)
				std::this_thread::yield();
				PWX_LOCK(toRemove)
			}

			// Now if next still points to toRemove, this is locked:
			if (this->getPrev() == toRemove)
				toRemove->remove();
			else {
				// Otherwise toRemove went away, which is bad
				PWX_UNLOCK(toRemove)
				PWX_THROW("Illegal_Remove", "Previous element to remove went away",
						"A previous element to remove went away while waiting for the lock!")
			}
		} // End of having a next element to remove
	}


	/** @brief set the next pointer to another element.
	  *
	  * This method will use atomic::store() and is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * @param[in] new_next target where the next pointer should point at.
	**/
	void setNext(elem_t* new_next) noexcept
	{
		elem_t* currNext = next.load(std::memory_order_acquire);
		next.store(new_next, std::memory_order_release);
		if (currNext)
			oldNext.store(currNext, std::memory_order_release);
	}


	/** @brief set the prev pointer to another element.
	  *
	  * This method will use atomic::store() is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * @param[in] new_prev target where the prev pointer should point at.
	**/
	void setPrev(elem_t* new_prev) noexcept
	{
		elem_t* currPrev = prev.load(std::memory_order_acquire);
		prev.store(new_prev, std::memory_order_release);
		if (currPrev)
			oldPrev.store(currPrev, std::memory_order_release);
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
			PWX_DOUBLE_LOCK(elem_t, this, elem_t, const_cast<elem_t*>(&src))
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
	data_t &operator*()
	{
		PWX_LOCK_GUARD(elem_t, this)
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
	const data_t &operator*() const
	{
		PWX_LOCK_GUARD(elem_t, const_cast<elem_t*>(this))
		if (nullptr == data.get())
			PWX_THROW ( "NullDataException",
						"nullptr TDoubleElement<T>->data",
						"The data pointer to dereference is nullptr.")
		return *data;
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
	using base_t::isRemoved;

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
	isDestroyed.store(true);

	if (1 == data.use_count()) {
		// Produce a lock guard before checking again.
		PWX_NAMED_LOCK_GUARD(Make_Exclusive, elem_t, this)
		// So the lock is only generated if there is a possibility
		// that we have to delete data, but another thread might
		// have made a copy in the mean time before "isDestroyed"
		// was finished setting to true.
		if (1 == data.use_count()) {
			PWX_TRY(data.reset()) // the shared_ptr will delete the data now
			catch(...) { }

			// Do another locking, so that threads having had to wait while the data
			// was destroyed have a chance now to react before the object is gone.
			PWX_NAMED_LOCK_GUARD (Lock_After_Delete, elem_t, this)
		}
	}
}


} // namespace pwx

#endif // PWX_LIBPWX_TYPES_TDOUBLEELEMENT_H_INCLUDED
