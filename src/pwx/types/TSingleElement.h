#pragma once
#ifndef PWX_LIBPWX_TYPES_TSINGLEELEMENT_H_INCLUDED
#define PWX_LIBPWX_TYPES_TSINGLEELEMENT_H_INCLUDED 1

/** @file TSingleElement.h
  *
  * @brief Declaration of a basic template for singly linked list elements
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

/** @struct TSingleElement
  *
  * @brief Template for an element of a singly linked list or ring of variable types
  *
  * This is a very simple and basic type to wrap a pointer of variable type into
  * an object that is used with pwx::TSingleList and pwx::TSingleRing.
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
  *
  * If you plan to use this type in a multi-threaded environment, you can use the
  * getNext() and setNext() functions to manipulate the next pointer. See below for
  * more on multi threaded usage.
  *
  * To insert any element into a list you can use insertNext() to have it inserted
  * after the called element safely. If you just added an element to a container
  * you can use insert() to tell the element that it has been inserted into a
  * container.
  *
  * To remove an element from a list, you can use removeNext() to have the element
  * after this to be removed safely. If there is no other element to call removeNext()
  * from, you can use an elements remove() function to tell it that it has been
  * removed from its container. This will set the next pointer to nullptr as well.
  *
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * <B>Notes on multi threaded environments</B>
  *
  * If you plan to use an element in a strictly single-threaded way, you can use
  * the function do_locking(bool) inherited from CLockable to disable the locking
  * mechanism. You can then use insertNext() / removeNext() without the locking
  * overhead. However, as the locking is enabled by default, it might be more
  * convenient to simply use the next pointers directly.
  *
  * <I>Critical work flows</I>
  *
  * The following work flows can be troublesome if multiple threads perform
  * concurrent tasks on an element:
  * <TABLE border="1">
  * <TR>
  *   <TH>Task</TH><TH>Problematic action</TH><TH>Solution</TH>
  * </TR><TR>
  *   <TD>Retrieve next element</TD><TD>Remove this element</TD>
  *   <TD>Elements know when they are removed. getNext() then delivers the
  * previously stored pointer, if any.</TD>
  * </TR><TR>
  *   <TD>Retrieve next element</TD><TD>Move element to different container</TD>
  *   <TD>This is not detectable, so never move an element. Remove and copy insert it!</TD>
  * </TR><TR>
  *   <TD>Insert an element after this</TD><TD>Either element destroyed by another thread</TD>
  *   <TD>insertNext() will lock both this and the new next element. Further it checks
  * whether any is destroyed and only inserts the element if both are not marked as destroyed.<BR />
  * If either is marked as destroyed, a pwx::CException is thrown, as those conditions imply
  * serious bugs.</TD>
  * </TR><TR>
  *   <TD>Remove the next element</TD><TD>The next element gets removed or another element is
  * inserted between the two elements by another thread</TD>
  *   <TD>removeNext() will try to lock both elements after one another in a Release->Yield->Lock
  * cycle until both are locked or the next element changes. In the latter event the method does
  * not remove the element, as it is gone.<BR />
  * If the next element goes away before it can be removed, a pwx::CException is thrown.</TD>
  * </TR>
  * </TABLE>
**/
template<typename data_t>
struct PWX_API TSingleElement : public VElement
{

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef VElement                base_t;     //!< Base type of this element
	typedef TSingleElement<data_t>  elem_t;     //!< Type of this element
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
	TSingleElement (data_t* data_, void (*destroy_) (data_t* data_)) noexcept
	: data (data_, TVarDeleter<data_t> (destroy_))
	{ }


	/** @brief explicit constructor
	  *
	  * Delegating ctor that calls the default ctor with destroy_ being the nullptr
	  *
	  * @param[in] data_ A pointer to the data this list element is to hold.
	**/
	explicit
	TSingleElement (data_t* data_) noexcept
	: elem_t (data_, nullptr)
	{ }


	TSingleElement() PWX_DELETE; // nullptr data is not allowed


	/** @brief copy ctor
	  *
	  * The copy ctor creates a stand-alone element without neighbors
	  * copying the data pointer and destroy method from @a src.
	  * As the data is wrapped in a shared_ptr, data will not get
	  * deleted unless the last reference is gone.
	  *
	  * <B>Important</B>: Whether the element does locking or not
	  * is *not* copied. It will silently turned on by default!
	  *
	  * @param[in] src reference to the element to copy.
	**/
	TSingleElement (const elem_t &src) noexcept
	: base_t (src),
	  data (src.data)
	{ }


	virtual ~TSingleElement() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	/** @brief returns a pointer to the next element or nullptr if there is none.
	  *
	  * This method uses atomic::load() and is therefore safe to use
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


	/** @brief tell the element that it has been inserted.
	  *
	  * If an element becomes the last or only element of a container,
	  * it has no next element. This method can be used to tell the
	  * element that it is stored in a container even without a next
	  * pointer set.
	**/
	void insert() noexcept
	{
		isRemoved.store(false, std::memory_order_release);
	}


	/** @brief insert an element after this element.
	  *
	  * This is an extra method to not only set the next pointer
	  * of this element, but the next pointer of the inserted element
	  * safely, too, in a multi-threaded environment.
	  *
	  * If either this or the new element is marked as destroyed,
	  * a pwx::CException is thrown. Such a condition implies that
	  * there is something seriously wrong.
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

			// Set the neighborhood of the new next
			new_next->next.store(xOldNext, std::memory_order_relaxed);

			// Store new next neighbor and tell it that it has been inserted
			setNext(new_next);
			new_next->insert();
		} else if (destroyed())
			PWX_THROW("Illegal_Insert", "Destroyed elements can't insert",
					"Tried to insert an element after an already destroyed element!")
		else
			PWX_THROW("Illegal_Insert", "Can't insert a destroyed element",
					"Tried to insert an element that has already been destroyed!")
	}


	/** @brief tell the element that it has been removed.
	  *
	  * Whenever you remove an element from a container you should
	  * call this method to tell it that it has been removed.
	  * The next pointer of the element will be set to nullptr
	  * by this method.
	**/
	void remove() noexcept
	{
		PWX_LOCK_GUARD(elem_t, this)
		setNext(nullptr);
		isRemoved.store(true, std::memory_order_release);
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
			elem_t* xOldNext = toRemove->getNext();

			/* See notes in TDoubleElement::remove() */

			// Do a release->yield->lock cycle until this is locked
			while ((this->getNext() == toRemove) && !this->try_lock()) {
				// Can't lock this, so yield until possible
				PWX_UNLOCK(toRemove)
				std::this_thread::yield();
				PWX_LOCK(toRemove)
			}

			// Now if next still points to toRemove, this is locked:
			if (this->getNext() == toRemove) {
				this->setNext(xOldNext);
				this->unlock();
			} else
				// Otherwise toRemove went away, which is bad
				PWX_THROW("Illegal_Remove", "Next element to remove went away",
						"A next element to remove went away while waiting for the lock!")

			// 2: Remove neighborhood:
			toRemove->remove();
		} // End of having a neighbor to handle
	}


	/** @brief set the next pointer to another element.
	  *
	  * This method uses atomic::store() and is therefore safe to use
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
		return *this;
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
						"nullptr TSingleElement<T>->data",
						"The pointer lhs->data to dereference is nullptr.")
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
						"nullptr TSingleElement<T>->data",
						"The pointer lhs->data to dereference is nullptr.")
		return *data;
	}


	/* ===============================================
	 * === Public members                          ===
	 * ===============================================
	*/

	share_t    data; 							//!< The data this list element points to, wrapped in a shared_ptr.
	neighbor_t next = ATOMIC_VAR_INIT(nullptr); //!< The next element in the list or nullptr if this is the tail.


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

}; // struct TSingleElement


/** @brief destructor
  *
  * The destructor invokes a lock on the instance to allow
  * other threads to react before the object itself is gone.
  *
  * Because of the usage of shared_ptr wrapping the data this
  * is only deleted if, and only if, this is the very last
  * element referencing this data.
**/
template<typename data_t>
TSingleElement<data_t>::~TSingleElement() noexcept
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

#endif // PWX_LIBPWX_TYPES_TSINGLEELEMENT_H_INCLUDED


