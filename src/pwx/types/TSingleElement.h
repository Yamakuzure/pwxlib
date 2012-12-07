#pragma once
#ifndef PWX_LIBPWX_TYPES_TSINGLEELEMENT_H_INCLUDED
#define PWX_LIBPWX_TYPES_TSINGLEELEMENT_H_INCLUDED 1

/** @file TSingleElement.h
  *
  * @brief Declaration of a basic template for singly linked list elements
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
  * completely safe to copy TsingleElement instances.
  *
  * The data pointer itself is public. You can use foo->data.get() to access it.
  * Further the operator* is overloaded and **foo will result in a reference to the
  * data.
  *
  * The next element in the list can be retrieved using the public foo->next pointer.
  *
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * If PWX_THREADS is defined, changes to the element are done in a locked state.
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

	/** @brief returns true if the data  was destroyed
	  *
	  * The destructor will try to get a final lock on the
	  * element when it is destroyed. If another thread
	  * acquires a lock between the data destruction and
	  * this final dtor lock, destroy() will return "true".
	  *
	  * @return true if the element is within its destruction process.
	**/
	bool destroyed() const noexcept { return isDestroyed; }


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
			PWX_LOCK_GUARD (elem_t, this)
			data    = src.data;
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

	share_t data;           //!< The data this list element points to, wrapped in a shared_ptr.
	elem_t* next = nullptr; //!< The next element in the list or nullptr if this is the tail.


private:

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	bool isDestroyed = false; //!< Set to true by the dtor to let other threads react if they ask destroyed().
}; // struct TSingleElement


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
TSingleElement<data_t>::~TSingleElement() noexcept
{
	isDestroyed = true;

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


