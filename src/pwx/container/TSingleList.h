#pragma once
#ifndef PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED
#define PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED 1

/** @file TSingleList.h
  *
  * @brief Declaration of a basic template for singly linked lists
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
#include <pwx/general/macros.h>
#include <pwx/types/TSingleElement.h>

namespace pwx {

/** @class TSingleList
  *
  * @brief Template to build singly linked lists of variable types
  *
  * The singly linked list is the most basic container. Elements of type
  * pwx::TSingleElement<T> are chained together using their next pointers.
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
template<typename data_t, typename elem_t = TSingleElement<data_t> >
class TSingleList : public CLockable
{
public:
  /* ===============================================
   * === Public types                            ===
   * ===============================================
  */
  typedef TSingleList<data_t, elem_t> list_t;

  /* ===============================================
   * === Public Constructors and destructors     ===
   * ===============================================
  */

  /** @brief default constructor
    *
    * The default constructor initializes an empty list.
    *
    * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
  **/
  TSingleList(void (*destroy_)(data_t* data_)) noexcept
  : destroy(destroy_)
    { /* nothing to be done here */ }

  /** @brief empty constructor
    *
    * The empty constructor uses the default constructor to set the data
    * destroy method to the null pointer.
  **/
  TSingleList() noexcept
  : list_t(nullptr)
    { /* nothing to be done here */ }

  virtual ~TSingleList() noexcept;

  TSingleList(const list_t &rhs) PWX_DELETE; // No copy ctor

  /* ===============================================
   * === Public methods                          ===
   * ===============================================
  */

  /** @brief delete all elements
    *
    * This is a quick way to get rid of all elements
    * at once. If a destroy() function was set, it is
    * used for the data deletion. Otherwise it is
    * assumed that data_t responds to the delete
    * operator.
  **/
  virtual void clear() noexcept
    {
      while (eCount)
        delNext(nullptr);
    }

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
    * If there is no item behind the element holding @a prev_ a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] prev_ the data the element that precedes the element to delete holds
    * @return the number of elements remaining in the list after the deletion.
  **/
  virtual uint32_t delNext(data_t* prev_)
    {
      PWX_LOCK_GUARD(list_t, this)
      try
        {
          elem_t* removed = remNext(prev_);
          if (removed)
            delete removed;
          return eCount;
        }
      PWX_THROW_FURTHER
      PWX_THROW_STD_FURTHER("delete", "Deleting an element in TListBase::delNext() failed.")
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
    * If there is no item behind the element @a prev_ a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] prev_ the element that precedes the element to delete
    * @return the number of elements remaining in the list after the deletion.
  **/
  virtual uint32_t delNextElem(elem_t* prev_)
    {
      PWX_LOCK_GUARD(list_t, this)
      try
        {
          elem_t* removed = remNextElem(prev_);
          if (removed)
            delete removed;
          return eCount;
        }
      PWX_THROW_FURTHER
      PWX_THROW_STD_FURTHER("delete", "Deleting an element in TListBase::delNextElem() failed.")
    }

  /** @brief find the item with the given @a data_
    *
    * This method searches through the list and returns the item
    * with the given @a data_ or nullptr if @a data_ is not stored in this
    * list.
    *
    * @param data_ pointer to the data to find
    * @return return a pointer to the element storing @a data_
  **/
  virtual elem_t* find(data_t* data_) noexcept
    {
      return const_cast<elem_t* >(find(static_cast<const data_t* >(data_)));
    }

  /** @brief find the item with the given @a data_
    *
    * This method searches through the list and returns a const pointer
    * to the item with the given @a data_ or nullptr if @a data_ is not stored
    * in this list.
    *
    * @param data_ pointer to the data to find
    * @return return a const pointer to the element storing @a data_
  **/
  virtual const elem_t* find(const data_t* data_) const noexcept
    {
      if (nullptr == curr)
        return nullptr;

      // From here on, curr is valid and the list needs to be guarded:
      PWX_LOCK_GUARD(list_t, const_cast<list_t* >(this))

      // Quick exit if curr is already what we want:
      if (curr->data == data_)
        return curr;

      // Safe the current position so we can restore curr if we can't find data_
      elem_t*  oldCurr = curr;
      uint32_t oldNr   = eNr;
      curr = head;
      eNr  = 0;
      while (curr)
        {
          if (curr->data == data_)
            return curr;
          ++eNr;
          curr = curr->next;
        }

      // If we are here, data_ can not be found. Restore curr and give nullptr back.
      eNr  = oldNr;
      curr = oldCurr;
      return nullptr;
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
    * the name "ItemCreationFailed" is thrown.
    *
    * @param[in] prev_ the data the element that should precede the new element holds
    * @param[in] data_ the pointer that is to be added.
    * @return the number of elements in this list after the insertion
  **/
  virtual uint32_t insNext(data_t* prev_, data_t* data_)
    {
      PWX_LOCK_GUARD(list_t, this)

      if (prev_ && (nullptr == find(prev_)) )
        // find sets curr to the correct value.
        PWX_THROW("ItemNotFound", "Item not found", "The searched item can not be found in this singly linked list")

      // First create a new element for data_
      elem_t* newElement = nullptr;
      PWX_TRY(newElement = new elem_t(data_, destroy))
      PWX_THROW_STD_FURTHER("ItemCreationFailed", "The Creation of a new list item failed.")

      if (nullptr == prev_)
        {
          if (head)
            newElement->next = head;
          head = newElement;
          // In this case we need to raise eNr because an element
          // is inserted before curr in the list
          ++eNr;
        }
      else
        {
          PWX_LOCK_GUARD(elem_t, curr)
          // Note: newElement is not accessible from anywhere else, thus it does not need to be locked.
          if (curr->next)
            newElement->next = curr->next;
          else
            // If we have a new tail, it needs to be noted:
            tail = newElement;
          curr->next       = newElement;
        }

      // If we had no elements yet, head and tail need to be set:
      if (0 == eCount)
        {
          head = newElement;
          tail = newElement;
          curr = newElement;
          eNr  = 0;
        }

      // Count the new element and give the number back
      return ++eCount;
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
    * the name "ItemCreationFailed" is thrown.
    *
    * @param[in] prev_ the element that should precede the new element
    * @param[in] data_ the pointer that is to be added.
    * @return the number of elements in this list after the insertion
  **/
  virtual uint32_t insNextElem(elem_t* prev_, data_t* data_)
    {
      PWX_LOCK_GUARD(list_t, this)

      // First create a new element for data_
      elem_t* newElement = nullptr;
      PWX_TRY(newElement = new elem_t(data_, destroy))
      PWX_THROW_STD_FURTHER("ItemCreationFailed", "The Creation of a new list item failed.")

      if (prev_)
        {
          PWX_LOCK_GUARD(elem_t, curr)
          // Note: newElement is not accessible from anywhere else, thus it does not need to be locked.
          if (prev_->next)
            newElement->next = prev_->next;
          else
            // If we have a new tail, it needs to be noted:
            tail = newElement;
          prev_->next      = newElement;

        }
      else
        {
          if (head)
            newElement->next = head;
          head = newElement;
        }

      // If we had no elements yet, head and tail need to be set:
      if (0 == eCount)
        {
          head = newElement;
          tail = newElement;
        }

      // curr needs to be reseted, otherwise eNr is not maintainable
      curr = head;
      eNr  = 0;

      // Count the new element and give the number back
      return ++eCount;
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
    * If there is no item behind the element @a prev_ a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] prev_ the data the element that precedes the element to remove holds
    * @return a pointer to the removed element
  **/
  virtual elem_t* remNext(data_t* prev_)
    {
      PWX_LOCK_GUARD(list_t, this)

      if (prev_ && (nullptr == find(prev_)) )
        // find sets curr to the correct value.
        PWX_THROW("ItemNotFound", "Item not found", "The searched item can not be found in this singly linked list")

      if (prev_ && (nullptr == curr->next) )
        PWX_THROW("OutOfRange", "Item out of range", "There is no element behind element holding the given prev_ pointer")

      elem_t* toRemove = prev_ ? curr->next : head;

      // Lock both curr and the item to remove:
      PWX_DOUBLE_LOCK(elem_t, curr, elem_t, toRemove);

      if (nullptr == prev_)
        {
          if (head == curr)
            // curr needs to stay valid on the current head, eNr stays being zero then
            curr = toRemove->next;
          else
            // Otherwise eNr needs to be lowered because an item before curr in the list is removed
            --eNr;

          // if this was the last item, sanitize the list:
          if (nullptr == toRemove->next)
            {
              head = nullptr;
              curr = nullptr;
              tail = nullptr;
              eNr  = 0;
            }
          else
            head = toRemove->next;
        }
      else
        curr->next = toRemove->next;

      // Maintain tail if the tail element is to be removed:
      if (tail == toRemove)
        tail = curr;

      toRemove->next = nullptr;
      --eCount;

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
    * If there is no item behind the element @a prev_ or if the list is
    * empty, a pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] prev_ the element that precedes the element to remove
    * @return a pointer to the removed element
  **/
  virtual elem_t* remNextElem(elem_t* prev_)
    {
      PWX_LOCK_GUARD(list_t, this)

      if (prev_ && (nullptr == prev_->next) )
        PWX_THROW("OutOfRange", "Item out of range", "There is no element behind the given prev_ element")

      if (0 == eCount)
        PWX_THROW("OutOfRange", "Item out of range", "The list is empty")

      elem_t* toRemove = prev_ ? prev_->next : head;

      // Lock the item to remove
      toRemove->lock();

      if (prev_)
        prev_->next = toRemove->next;
      else
        {
          // if this was the last item, sanitize the list:
          if (nullptr == toRemove->next)
            {
              head = nullptr;
              curr = nullptr;
              tail = nullptr;
              eNr  = 0;
            }
          else
            head = toRemove->next;
        }

      // Maintain tail if the tail element is to be removed:
      if (tail == toRemove)
        tail = prev_;

      toRemove->next = nullptr;

      // curr is reseted to head, because we can't maintain eNr otherwise.
      curr = head;
      eNr  = 0;
      --eCount;
      toRemove->unlock();

      return toRemove;
    }

  /// @brief return the number of stored elements
  uint32_t size() const noexcept { return eCount; }

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
      return privGetElementByIndex(index_);
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
      return const_cast<elem_t* >(privGetElementByIndex(static_cast<const int32_t>(index_)));
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
  void (*destroy)(data_t* data_);

  /* ===============================================
   * === Protected operators                     ===
   * ===============================================
  */

  /* ===============================================
   * === Protected members                       ===
   * ===============================================
  */
  uint32_t eCount = 0;         //!< Element count
  mutable
  uint32_t eNr    = 0;         //!< Number of the element curr points to
  mutable
  elem_t*  curr   = nullptr;   //!< pointer to the currently handled element
  elem_t*  head   = nullptr;   //!< pointer to the first element
  elem_t*  tail   = nullptr;   //!< pointer to the last element

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

  /// @brief wrapping method to retrieve an element by any index or nullptr if the list is empty
  virtual const elem_t* privGetElementByIndex(int32_t index_) const noexcept
    {
      if (eCount)
        {
          // Mod index_ into range
          uint32_t xIdx = static_cast<uint32_t>(index_ < 0
                                                ? eCount - (::std::abs(index_) % eCount)
                                                : index_ % eCount);
          // Unfortunately this results in xIdx equaling eCount
          // (which is wrong) if index_ is a negative multiple of
          // eCount:
          if (xIdx >= eCount)
            xIdx = xIdx % eCount;

          // Now protect the list, we need curr to be changeable without fear:
          PWX_LOCK_GUARD(list_t, const_cast<list_t* >(this))

          // Is curr already correct?
          if (xIdx == eNr)
            return curr;

          // Is xIdx the next member, like in a for loop?
          if (xIdx == (eNr + 1))
            {
              curr = curr->next;
              ++eNr;
              return curr;
            }

          // Is it the head we want?
          if (0 == xIdx)
            return head;

          // Or tail ?
          if ((eCount - 1) == xIdx)
            return tail;

          // Ok, let's go. But only start from head if we currently are beyond.
          if (xIdx < eNr)
            {
              curr = head->next;
              eNr  = 1;
            }
          // And the next of curr is already checked, so skip it
          else
            {
              curr = curr->next;
              ++eNr;
            }
          // Now look into the rest
          while ((eNr < xIdx) && (eNr < (eCount - 1)))
            {
              curr = curr->next;
              ++eNr;
            }
          // curr is sure to be pointing where it should now.
          return curr;
        }

      return nullptr;
    }

  /* ===============================================
   * === Private operators                       ===
   * ===============================================
  */

  /* ===============================================
   * === Private members                         ===
   * ===============================================
  */

}; // class TSingleList

/** @brief default destructor
  *
  * This destructor will delete all items currently stored. There is no
  * need to clean up manually before deleting the list.
**/
template<typename data_t, typename elem_t>
TSingleList<data_t, elem_t>::~TSingleList() noexcept
  {
    clear();
  }

} // namespace pwx

#endif // PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED

