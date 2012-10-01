#pragma once
#ifndef PWX_PWXLIB_PWX_CONTAINER_TDOUBLERING_H_INCLUDED
#define PWX_PWXLIB_PWX_CONTAINER_TDOUBLERING_H_INCLUDED 1

/** @file <filename>
  *
  * @brief <description>
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
  *
  * History and Changelog are maintained in pwx.h
**/

#include <pwx/container/TDoubleList.h>

namespace pwx {

/** @class TDoubleRing
  *
  * @brief Template to build doubly linked rings of variable types
  *
  * The doubly linked ring is an extension to the singly linked ring.
  * The head element has a prev pointer pointing to tail rather than nullptr.
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
template<typename data_t, typename elem_t = TDoubleElement<data_t> >
class TDoubleRing : public TDoubleList<data_t, elem_t>
{
public:
  /* ===============================================
   * === Public types                            ===
   * ===============================================
  */
  typedef TDoubleList<data_t, elem_t> base_t;
  typedef TDoubleRing<data_t, elem_t> list_t;

  /* ===============================================
   * === Public Constructors and destructors     ===
   * ===============================================
  */

  /** @brief default constructor
    *
    * The default constructor initializes an empty ring.
    *
    * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
  **/
  TDoubleRing(void (*destroy_)(data_t* data_)) noexcept
  : base_t(destroy_)
    { /* nothing to be done here */ }

  /** @brief empty constructor
    *
    * The empty constructor uses the base constructor to set the data
    * destroy method to the null pointer.
  **/
  TDoubleRing() noexcept
  : base_t(nullptr)
    { /* nothing to be done here */ }

  virtual ~TDoubleRing() noexcept;

  TDoubleRing(const list_t &rhs) PWX_DELETE; // No copy ctor

  /* ===============================================
   * === Public methods                          ===
   * ===============================================
  */

  using base_t::clear;

  /** @brief delete the element after the element holding the specified data
    *
    * This method deletes the element in the list after the element
    * that holds @a prev_.
    *
    * If @a prev_ is set to nullptr, the root element (aka head) is
    * deleted.
    *
    * If you intent to work with the element, use remNext instead.
    *
    * If there is no element behind the element holding @a prev_ a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] prev_ the data the element that precedes the element to delete holds
    * @return the number of elements remaining in the list after the deletion.
  **/
  virtual uint32_t delNext(data_t* prev_)
    {
      PWX_LOCK_GUARD(list_t, this)
      PWX_TRY(base_t::delNext(prev_))
      PWX_THROW_FURTHER
      PWX_THROW_STD_FURTHER("delete", "Deleting an element in TDoubleRing::delNext() failed.")
      privConnectEnds();
      return eCount;
    }

  /** @brief delete the element after the specified element
    *
    * This method deletes the element in the list after the element
    * @a prev_.
    *
    * If @a prev_ is set to nullptr, the root element (aka head) is
    * deleted.
    *
    * If you intent to work with the element, use remNextElem instead.
    *
    * If @a prev_ is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * If there is no element behind the element @a prev_ a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] prev_ the element that precedes the element to delete
    * @return the number of elements remaining in the list after the deletion.
  **/
  virtual uint32_t delNextElem(elem_t* prev_)
    {
      PWX_LOCK_GUARD(list_t, this)
      PWX_TRY(base_t::delNextElem(prev_))
      PWX_THROW_FURTHER
      PWX_THROW_STD_FURTHER("delete", "Deleting an element in TDoubleRing::delNextElem() failed.")
      privConnectEnds();
      return eCount;
    }

  /** @brief find the element with the given @a data_
    *
    * This method searches through the list and returns the element
    * with the given @a data_ or nullptr if @a data_ is not stored in this
    * list.
    *
    * @param data_ pointer to the data to find
    * @return return a pointer to the element storing @a data_
  **/
  virtual elem_t* find(data_t* data_) noexcept
    {
      return const_cast<elem_t* >(base_t::find(static_cast<const data_t* >(data_)));
    }

  /** @brief find the element with the given @a data_
    *
    * This method searches through the list and returns a const pointer
    * to the element with the given @a data_ or nullptr if @a data_ is not stored
    * in this list.
    *
    * @param data_ pointer to the data to find
    * @return return a const pointer to the element storing @a data_
  **/
  virtual const elem_t* find(const data_t* data_) const noexcept
    {
      return base_t::find(data_);
    }

  /** @brief insert a new data pointer after the specified data
    *
    * This method inserts a new element in the list after the element
    * holding @a prev_.
    *
    * If @a prev_ is set to nullptr, the new element will become the new
    * head of the list.
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ElementCreationFailed" is thrown.
    *
    * @param[in] prev_ the data the element that should precede the new element holds
    * @param[in] data_ the pointer that is to be added.
    * @return the number of elements in this list after the insertion
  **/
  virtual uint32_t insNext(data_t* prev_, data_t* data_)
    {
      PWX_LOCK_GUARD(list_t, this)
      PWX_TRY(base_t::insNext(prev_, data_))
      PWX_THROW_FURTHER
      PWX_THROW_STD_FURTHER("ElementCreationFailed", "The Creation of a new list element failed.")
      privConnectEnds();
      return eCount;
    }

  /** @brief insert a new data pointer after the specified element
    *
    * This method inserts a new element in the list after the element
    * @a prev_.
    *
    * If @a prev_ is set to nullptr, the new element will become the new
    * head of the list.
    *
    * If @a prev_ is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ElementCreationFailed" is thrown.
    *
    * @param[in] prev_ the element that should precede the new element
    * @param[in] data_ the pointer that is to be added.
    * @return the number of elements in this list after the insertion
  **/
  virtual uint32_t insNextElem(elem_t* prev_, data_t* data_)
    {
      PWX_LOCK_GUARD(list_t, this)
      PWX_TRY(base_t::insNextElem(prev_, data_))
      PWX_THROW_FURTHER
      PWX_THROW_STD_FURTHER("ElementCreationFailed", "The Creation of a new list element failed.")
      privConnectEnds();
      return eCount;
    }

  /** @brief remove the element after the element holding the specified data
    *
    * This method removes the element in the list after the element
    * that holds @a prev_ and returns a pointer to the removed element.
    *
    *
    * If @a prev_ is set to nullptr, the root element (aka head) is
    * removed.
    *
    * You have to delete the removed element by yourself. If you do not intent
    * to work with the removed element, use delNext instead.
    *
    * If there is no element behind the element @a prev_ a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] prev_ the data the element that precedes the element to remove holds
    * @return a pointer to the removed element
  **/
  virtual elem_t* remNext(data_t* prev_)
    {
      elem_t* toRemove = nullptr;
      PWX_LOCK_GUARD(list_t, this)
      PWX_TRY(toRemove = base_t::remNext(prev_))
      PWX_THROW_FURTHER
      privConnectEnds();
      return toRemove;
    }

  /** @brief remove the element after the specified element
    *
    * This method removes the element in the list after the element
    * @a prev_.
    *
    * If @a prev_ is set to nullptr, the root element (aka head) is
    * removed.
    *
    * If @a prev_ is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * If there is no element behind the element @a prev_ or if the list is
    * empty, a pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] prev_ the element that precedes the element to remove
    * @return a pointer to the removed element
  **/
  virtual elem_t* remNextElem(elem_t* prev_)
    {
      elem_t* toRemove = nullptr;
      PWX_LOCK_GUARD(list_t, this)
      PWX_TRY(toRemove = base_t::remNextElem(prev_))
      PWX_THROW_FURTHER
      privConnectEnds();
      return toRemove;
    }

  using base_t::size;

  /* ===============================================
   * === Public operators                        ===
   * ===============================================
  */
  list_t &operator=(const list_t &rhs) PWX_DELETE; // No assignment

  /** @brief return a read-only pointer to the element with the given @a index_
    *
    * This operator retrieves an element by index like an array. The pointer given
    * back is read-only.
    *
    * There will be no exception if the index_ is out of range, it will be wrapped
    * to press it into the valid range. This means that an index of -1 can be used
    * to retrieve the last element (tail) for instance.
    *
    * If the list is empty, the operator returns nullptr.
    *
    * If you use this operator to quickly access head or tail, neither the currently
    * used internal pointer nor number are changed. Head and tail are given back
    * directly.
    *
    * @param[in] index_ the index of the element to find.
    * @return read-only pointer to the element, or nullptr if the list is empty.
  **/
  virtual const elem_t* operator[](const int32_t index_) const noexcept
    {
      return base_t::operator[](index_);
    }

  /** @brief return a read/write pointer to the element with the given @a index_
    *
    * This operator retrieves an element by index like an array. The pointer given
    * back is write enabled, so use with care.
    *
    * There will be no exception if the index_ is out of range, it will be wrapped
    * to press it into the valid range. This means that an index of -1 can be used
    * to retrieve the last element (tail) for instance.
    *
    * If the list is empty, the operator returns nullptr.
    *
    * If you use this operator to quickly access head or tail, neither the currently
    * used internal pointer nor number are changed. Head and tail are given back
    * directly.
    *
    * @param[in] index_ the index of the element to find.
    * @return read/write pointer to the element, or nullptr if the list is empty.
  **/
  virtual elem_t* operator[](int32_t index_) noexcept
    {
      return base_t::operator[](index_);
    }

  /* ===============================================
   * === Public members                          ===
   * ===============================================
  */

protected:
  /* ===============================================
   * === Protected Constructors and destructors  ===
   * ===============================================
  */

  /* ===============================================
   * === Protected methods                       ===
   * ===============================================
  */

  /* ===============================================
   * === Protected operators                     ===
   * ===============================================
  */

  /* ===============================================
   * === Protected members                       ===
   * ===============================================
  */
  using base_t::eCount;
  using base_t::head;
  using base_t::tail;

private:
  /* ===============================================
   * === Private Constructors and destructors    ===
   * ===============================================
  */

  /* ===============================================
   * === Private methods                         ===
   * ===============================================
  */

  /// IMPORTANT: private methods do not lock, callers must have locked!

  /// @brief simple private method to make sure the ring is closed
  void privConnectEnds() noexcept
    {
      if (head && (head->prev != tail))
        head->prev = tail;
      if (tail && (tail->next != head))
        tail->next = head;
    }

  /* ===============================================
   * === Private operators                       ===
   * ===============================================
  */

  /* ===============================================
   * === Private members                         ===
   * ===============================================
  */

}; // class TDoubleRing

/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the list.
**/
template<typename data_t, typename elem_t>
TDoubleRing<data_t, elem_t>::~TDoubleRing() noexcept
  {
    /* --- deletion is done in the TDoubleList dtor --- */
  }


} // namespace pwx
#endif // PWX_PWXLIB_PWX_CONTAINER_TDOUBLERING_H_INCLUDED

