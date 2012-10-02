#pragma once
#ifndef PWX_LIBPWX_TYPES_TDOUBLEELEMENT_H_INCLUDED
#define PWX_LIBPWX_TYPES_TDOUBLEELEMENT_H_INCLUDED 1

/** @file TDoubleElement.h
  *
  * @brief Declaration of a basic template for doubly linked list elements
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

#include <memory>
#include <pwx/types/CLockable.h>
#include <pwx/types/TVarDeleter.h>

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
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * If PWX_THREADS is defined, changes to the element are done in a locked state.
**/
template<typename data_t>
struct PWX_API TDoubleElement : public CLockable
{
  /* ===============================================
   * === Public types                            ===
   * ===============================================
  */
  typedef CLockable                 base_t;
  typedef TDoubleElement<data_t>    elem_t;
  typedef ::std::shared_ptr<data_t> share_t;

  /* ===============================================
   * === Public Constructors and destructors     ===
   * ===============================================
  */

  /** @brief default constructor
    *
    * The default constructor sets both the data pointer and the destroy method.
    *
    * @param data_[in] A pointer to the data this list element is to hold.
    * @param destroy_[in] A pointer to a function that is to be used to destroy the data
  **/
  TDoubleElement(data_t* data_, void (*destroy_)(data_t* data_)) noexcept
  : data(data_, TVarDeleter<data_t>(destroy_))
  { }

  /** @brief explicit constructor
    *
    * Delegating ctor that calls the default ctor with destroy_ being the nullptr
    *
    * @param data_[in] A pointer to the data this list element is to hold.
  **/
  explicit
  TDoubleElement(data_t* data_) noexcept
  : elem_t(data_, nullptr)
    { }

  TDoubleElement() PWX_DELETE;

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
  TDoubleElement(const elem_t &src) noexcept
  : base_t(src),
    data(src.data)
    { }

  virtual ~TDoubleElement() noexcept;

  /* ===============================================
   * === Public methods                          ===
   * ===============================================
  */

  /* ===============================================
   * === Public operators                        ===
   * ===============================================
  */
  /** @brief assignment operator
    *
    * The assignment operator copies over the element and
    * the destroy method. This element will stay where it
    * is, and not change its position.
  **/
  elem_t& operator=(const elem_t &src) noexcept
    {
      PWX_LOCK_GUARD(elem_t, this)
      data = src.data;
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
        PWX_THROW("NullDataException", "nullptr TDoubleElement<T>->data", "The data pointer to dereference is nullptr.")
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
        PWX_THROW("NullDataException", "nullptr TDoubleElement<T>->data", "The data pointer to dereference is nullptr.")
      return *data;
    }

  /* ===============================================
   * === Public members                          ===
   * ===============================================
  */
  share_t data;           //!< The data this list element points to, wrapped in a shared_ptr.
  elem_t* next = nullptr; //!< The next element in the list or nullptr if this is the tail
  elem_t* prev = nullptr; //!< The previous element in the list or nullptr if this is the head
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
    if (1 == data.use_count())
      {
        this->lock();
        data.reset(); // the shared_ptr will delete the data now
        this->unlock();

        // Do another locking, so that threads having had to wait while the data
        // was destroyed have a chance now to react before the object is gone
        PWX_LOCK_GUARD(elem_t, this)
      }
  }


} // namespace pwx

#endif // PWX_LIBPWX_TYPES_TDOUBLEELEMENT_H_INCLUDED
