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
  * History and changelog are maintained in pwx.h
**/

#include <pwx/types/CLockable.h>

namespace pwx {

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
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * If PWX_THREADS is defined, changes to the element are done in a locked state.
**/
template<typename T>
struct PWX_API TSingleElement : public CLockable
{
  /* ===============================================
   * === Public types                            ===
   * ===============================================
  */
  typedef TSingleElement<T> elem_t;

  /* ===============================================
   * === Public Constructors and destructors     ===
   * ===============================================
  */

  /** @brief default constructor
    *
    * The explicit constructor only sets the data to point at. It does nothing special
    * otherwise and is the most minimal constructor available.
    *
    * @param data_[in] A pointer to the data this list item is to hold.
    * @param destroy_[in] A pointer to a function that is to be used to destroy the data
  **/
  TSingleElement(T* data_, void (*destroy_)(T* data_)) noexcept
  : data(data_),
    destroy(destroy_)
    { }

  /** @brief explicit constructor
    *
    * Delegating ctor that calls the default ctor with destroy_ beeing the nullptr
    *
    * @param data_[in] A pointer to the data this list item is to hold.
  **/
  explicit
  TSingleElement(T* data_) noexcept
  : elem_t(data_, nullptr)
    { }

  TSingleElement() PWX_DELETE;

  virtual ~TSingleElement() noexcept;

  TSingleElement(const elem_t &src) PWX_DELETE;

  /* ===============================================
   * === Public methods                          ===
   * ===============================================
  */

  /* ===============================================
   * === Public operators                        ===
   * ===============================================
  */
  elem_t& operator=(const elem_t &rhs) PWX_DELETE;

  /** @brief dereferencing an element returns a reference to the stored data
    *
    * If the data pointer is nullptr, a pwx::CException with the name
    * "NullDataException" is thrown.
    *
    * @return a read/write reference to the stored data.
  **/
  T &operator*()
    {
      if (nullptr == data)
        PWX_THROW("NullDataException", "nullptr TSingleElement<T>->data", "The pointer lhs->data to dereference is nullptr.")
      return *data;
    }

  /** @brief dereferencing a constant element returns a constant reference to the stored data
    *
    * If the data pointer is nullptr, a pwx::CException with the name
    * "NullDataException" is thrown.
    *
    * @return a read only reference to the stored data.
  **/
  const T &operator*() const
    {
      if (nullptr == data)
        PWX_THROW("NullDataException", "nullptr TSingleElement<T>->data", "The pointer lhs->data to dereference is nullptr.")
      return *data;
    }

  /* ===============================================
   * === Public members                          ===
   * ===============================================
  */
  T*      data = nullptr; //!< The data this list item points to
  elem_t* next = nullptr; //!< The next item in the list or nullptr if this is the tail

protected:
  /* ===============================================
   * === Protected members                       ===
   * ===============================================
  */

private:
  /* ===============================================
   * === Private methods                         ===
   * ===============================================
  */
  void (*destroy)(T* data_);
}; // struct TSingleElement


/** @brief destructor
  *
  * The destructor deletes the data pointed, too. If a destroy method had been set
  * by the constructor, it will be used for the data destruction. Otherwise it
  * is assumed that the data pointer responds to the delete operator.
**/
template<typename T>
TSingleElement<T>::~TSingleElement() noexcept
  {
    if (data)
      {
        PWX_LOCK_GUARD(elem_t, this)
        if (destroy)
          {
            PWX_TRY(destroy(data))
            catch(...) { }
          }
        else
          {
            PWX_TRY(delete data)
            catch(...) { }
          }
        data = nullptr;
      }
    // Do another locking, so that threads having had to wait while the data
    // was destroyed have a chance now to react before the object is gone
    PWX_LOCK_GUARD(elem_t, this)
  }

} // namespace pwx

#endif // PWX_LIBPWX_TYPES_TSINGLEELEMENT_H_INCLUDED


