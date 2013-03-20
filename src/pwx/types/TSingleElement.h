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
  * getNext() and setNext() functions to manipulate the next pointer. These methods
  * will lock the element prior any read/write access.
  *

FIXME: Test whether atomic load/store aren't enough.

  * However, as the next pointer is wrapped into std::atomic, you can use its
  * load/store functions to avoid locking instead.
  *
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * If you plan to use an element in a strictly single-threaded way, you can use
  * the function do_locking(bool) inherited from CLockable to disable the locking
  * mechanism.
**/
template<typename data_t>
struct PWX_API TSingleElement : public VElement
{

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef VElement                base_t;
	typedef TSingleElement<data_t>  elem_t;
	typedef std::shared_ptr<data_t> share_t;
	typedef std::atomic<elem_t*>    neighbor_t;


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
	: data (data_, TVarDeleter<data_t> (destroy_)),
	  next(ATOMIC_VAR_INIT(nullptr))
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
		return next.load(std::memory_order_acquire);
	}


	/** @brief set the next pointer to another element.
	  *
	  * This method will lock this element and is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * @param[in] next target where the next pointer should point at.
	**/
	void setNext(elem_t* new_next) noexcept
	{
		PWX_LOCK(this)
		next.store(new_next, std::memory_order_release);
		PWX_UNLOCK(this)
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
		if (this != &src) {
			PWX_LOCK(this)
			data = src.data;
			PWX_UNLOCK(this)
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

	share_t    data; //!< The data this list element points to, wrapped in a shared_ptr.
	neighbor_t next; //!< The next element in the list or nullptr if this is the tail.


protected:

	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	 */

	using base_t::isDestroyed;

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
		PWX_LOCK_GUARD (elem_t, this)
		PWX_TRY(data.reset()) // the shared_ptr will delete the data now
		catch(...) { }
	}
	// Do another locking, so that threads having had to wait while the data
	// was destroyed have a chance now to react before the object is gone.
	PWX_LOCK_GUARD (elem_t, this)
}

} // namespace pwx

#endif // PWX_LIBPWX_TYPES_TSINGLEELEMENT_H_INCLUDED


