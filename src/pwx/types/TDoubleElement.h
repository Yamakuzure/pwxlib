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
  * completely safe to copy TsingleElement instances.
  *
  * The data pointer itself is public. You can use foo->data.get() to access it.
  * Further the operator* is overloaded and **foo will result in a reference to the
  * data.
  *
  * The next element in the list can be retrieved using the public foo->getNext() pointer.
  * The previous element in the list can be retrieved using the public foo->getPrev() pointer.
  *
  * <B>Important</B>: If you plan to use this type in a multi-threaded environment,
  * it is strongly recommended to use the getNext(), setNext(), getPrev() and setPrev()
  * functions to manipulate the next and prev pointers. These methods will lock the
  * element prior any read/write access.
  *
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * === FIXME : ===
  * original: "If PWX_THREADS is defined, changes to the element are done in a locked state."
  * -> This must be changed. No automatic locking all the time, but run time
  *     variable handling of thread safety.
  *    - How ? Maybe telling elements via VElement whether they are used concurrently or not?
  * === : EMXIF ===
**/
template<typename data_t>
struct PWX_API TDoubleElement : public VElement
{

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/
	typedef VElement                base_t;
	typedef TDoubleElement<data_t>  elem_t;
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


	/** @brief returns a pointer to the next element or nullptr if there is none.
	  *
	  * This method will lock this element and is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * @return the next pointer or nullptr if there is none.
	**/
	elem_t* getNext() const noexcept
	{
		{} /// FIXME: PWX_LOCK_GUARD(elem_t, const_cast<elem_t*>(this))
		return next;
	}


	/** @brief returns a pointer to the prev element or nullptr if there is none.
	  *
	  * This method will lock this element and is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * @return the prev pointer or nullptr if there is none.
	**/
	elem_t* getPrev() const noexcept
	{
		{} /// FIXME: PWX_LOCK_GUARD(elem_t, const_cast<elem_t*>(this))
		return prev;
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
		{} /// FIXME: PWX_LOCK_GUARD(elem_t, this)
		next = new_next;
	}


	/** @brief set the prev pointer to another element.
	  *
	  * This method will lock this element and is therefore safe to use
	  * in a multi-threaded environment.
	  *
	  * @param[in] prev target where the prev pointer should point at.
	**/
	void setPrev(elem_t* new_prev) noexcept
	{
		{} /// FIXME: PWX_LOCK_GUARD(elem_t, this)
		prev = new_prev;
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
			{} /// FIXME: PWX_LOCK_GUARD (elem_t, this)
			data	= src.data;
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

	share_t data;           //!< The data this list element points to, wrapped in a shared_ptr.
	elem_t* next = nullptr; //!< The next element in the list or nullptr if this is the tail
	elem_t* prev = nullptr; //!< The previous element in the list or nullptr if this is the head


private:

	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	/// @brief internal method to directly set the number
	void setNr(const uint32_t newNr) const noexcept
	{
		eNr = newNr;
	}


	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	bool isDestroyed = false; //!< Set to false by the dtor to let other threads react if they ask destroyed().
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
	isDestroyed = true;

	if (1 == data.use_count()) {
		{} /// FIXME: PWX_LOCK_GUARD (elem_t, this)
		PWX_TRY(data.reset()) // the shared_ptr will delete the data now
		catch(...) { }
	}
	// Do another locking, so that threads having had to wait while the data
	// was destroyed have a chance now to react before the object is gone.
	{} /// FIXME: PWX_LOCK_GUARD (elem_t, this)
}


} // namespace pwx

#endif // PWX_LIBPWX_TYPES_TDOUBLEELEMENT_H_INCLUDED
