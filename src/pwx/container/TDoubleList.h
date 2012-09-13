#pragma once
#ifndef PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED
#define PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED 1

/** @file TDoubleList.h
  *
  * @brief Declaration of a basic template for doubly linked lists
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

#include <pwx/container/TSingleList.h>
#include <pwx/types/TDoubleElement.h>

namespace pwx {

/** @class TDoubleList
  *
  * @brief Template to build doubly linked lists of variable types
  *
  * The doubly linked list is an extension to the singly linked list.
  * Elements of type pwx::TDoubleElement<T> are chained together using their next
  * and prev pointers. Being able to look into what lies before an element
  * speeds up certain operations at the cost of one additional pointer per element.
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
class TDoubleList : public TSingleList<data_t, elem_t>
{
public:
  /* ===============================================
   * === Public types                            ===
   * ===============================================
  */
  typedef TSingleList<data_t, elem_t> base_t;
  typedef TDoubleList<data_t, elem_t> list_t;

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
  TDoubleList(void (*destroy_)(data_t* data_)) noexcept
  : base_t(destroy_)
    { /* nothing to be done here */ }

  /** @brief empty constructor
    *
    * The empty constructor uses the default constructor to set the data
    * destroy method to the null pointer.
  **/
  TDoubleList() noexcept
  : base_t(nullptr)
    { /* nothing to be done here */ }

  virtual ~TDoubleList() noexcept;

  TDoubleList(const list_t &rhs) PWX_DELETE; // No copy ctor

  /* ===============================================
   * === Public methods                          ===
   * ===============================================
  */

  using base_t::clear;
  using base_t::delNext;
  using base_t::delNextElem;

  /** @brief delete the element before the element holding the specified data
    *
    * This method deletes the element in the list before the element
    * that holds @a next_.
    *
    * If @a next_ is set to nullptr, the last element (aka tail) is
    * deleted.
    *
    * If you intent to work with the element, use remPrev instead.
    *
    * If there is no item before the element holding @a next_, a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] next_ the data the element that succeeds the element to delete holds
    * @return the number of elements remaining in the list after the deletion.
  **/
  virtual uint32_t delPrev(data_t* next_)
    {
      PWX_LOCK_GUARD(base_t, this)
      try
        {
          elem_t* removed = remPrev(next_);
          if (removed)
            delete removed;
          return eCount;
        }
      PWX_THROW_FURTHER
      PWX_THROW_STD_FURTHER("delete", "Deleting an element in TListBase::delNext() failed.")
    }

  /** @brief delete the element before the specified element
    *
    * This method deletes the element in the list before the element
    * @a next_.
    *
    * If @a next_ is set to nullptr, the last element (aka tail) is
    * deleted.
    *
    * If you intent to work with the element, use remPrevElem instead.
    *
    * If @a next_ is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * If there is no item before the element @a next_ a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] next_ the element that succeeds the element to delete
    * @return the number of elements remaining in the list after the deletion.
  **/
  virtual uint32_t delPrevElem(elem_t* next_)
    {
      PWX_LOCK_GUARD(base_t, this)
      try
        {
          elem_t* removed = remPrevElem(next_);
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
  elem_t* find(data_t* data_) noexcept
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
  const elem_t* find(const data_t* data_) const noexcept
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
      while (curr->next)
        {
          ++eNr;
          curr = curr->next;
          if (curr->data == data_)
            return curr;
        }

      // As there is no positive result, yet, wander backwards from the old position:
      eNr  = oldNr;
      curr = oldCurr;
      while (curr->prev)
        {
          --eNr;
          curr = curr->prev;
          if (curr->data == data_)
            return curr;
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
  uint32_t insNext(data_t* prev_, data_t* data_)
    {
      PWX_LOCK_GUARD(list_t, this)
      PWX_TRY(base_t::insNext(prev_, data_))
      PWX_THROW_FURTHER

      // We need to maintain prev pointer(s) before returning eCount:
      if (prev_)
        // curr->next is now the new element:
        return privAddPrev(curr);
      else
        // head is the new element
        return privAddPrev(nullptr);
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
  uint32_t insNextElem(elem_t* prev_, data_t* data_)
    {
      PWX_LOCK_GUARD(list_t, this)

      PWX_TRY(base_t::insNextElem(prev_, data_))
      PWX_THROW_FURTHER

      // We need to maintain prev pointer(s) before returning eCount:
      if (prev_)
        // prev_->next is now the new element:
        return privAddPrev(prev_);
      else
        // head is the new element
        return privAddPrev(nullptr);
    }

  /** @brief insert a new data pointer before the specified data
    *
    * This method inserts a new element in the list before the element
    * holding @a next_.
    *
    * If @a next_ is set to nullptr, the new element will become the new
    * tail of the list.
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ItemCreationFailed" is thrown.
    *
    * @param[in] next_ the data the element that should succeed the new element
    * @param[in] data_ the pointer that is to be added.
    * @return the number of elements in this list after the insertion
  **/
  uint32_t insPrev(data_t* next_, data_t* data_)
    {
      PWX_LOCK_GUARD(list_t, this)

      if (next_ && (nullptr == find(next_)) )
        // find sets curr to the correct value.
        PWX_THROW("ItemNotFound", "Item not found", "The searched item can not be found in this doubly linked list")

      // First create a new element for data_
      elem_t* newElement = nullptr;
      PWX_TRY(newElement = new elem_t(data_, destroy))
      PWX_THROW_STD_FURTHER("ItemCreationFailed", "The Creation of a new list item failed.")

      if (nullptr == next_)
        {
          if (tail)
            {
              newElement->prev = tail;
              tail->next = newElement;
            }
          tail = newElement;
        }
      else
        {
          PWX_LOCK_GUARD(elem_t, curr)
          // Note: newElement is not accessible from anywhere else, thus it does not need to be locked.
          if (curr->prev)
            {
              curr->prev->next = newElement;
              newElement->prev = curr->prev;
            }
          else
            // If we have a new tail, it needs to be noted:
            tail = newElement;

          // In this case a new element is inserted before curr,
          // so eNr must be raised:
          ++eNr;

          newElement->next = curr;
          curr->prev       = newElement;
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

  /** @brief insert a new data pointer before the specified element
    *
    * This method inserts a new element in the list before the element
    * @a next_.
    *
    * If @a next_ is set to nullptr, the new element will become the new
    * tail of the list.
    *
    * If @a next_ is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ItemCreationFailed" is thrown.
    *
    * @param[in] next_ the element that should succeed the new element
    * @param[in] data_ the pointer that is to be added.
    * @return the number of elements in this list after the insertion
  **/
  uint32_t insPrevElem(elem_t* next_, data_t* data_)
    {
      PWX_LOCK_GUARD(list_t, this)

      // First create a new element for data_
      elem_t* newElement = nullptr;
      PWX_TRY(newElement = new elem_t(data_, destroy))
      PWX_THROW_STD_FURTHER("ItemCreationFailed", "The Creation of a new list item failed.")

      if (next_)
        {
          PWX_LOCK_GUARD(elem_t, curr)
          // Note: newElement is not accessible from anywhere else, thus it does not need to be locked.
          if (next_->prev)
            {
              next_->prev->next = newElement;
              newElement->prev  = next_->prev;
            }
          else
            // If we have a new head, it needs to be noted:
            head = newElement;
          newElement->next  = next_;
          next_->prev       = newElement;

        }
      else
        {
          if (tail)
            {
              tail->next = newElement;
              newElement->prev = tail;
            }
          tail = newElement;
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
    * If there is no item behind the element @a prev_ holds, a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] prev_ the data the element that precedes the element to remove holds
    * @return a pointer to the removed element
  **/
  elem_t* remNext(data_t* prev_)
    {
      elem_t* toRemove = nullptr;
      PWX_LOCK_GUARD(list_t, this)
      PWX_TRY(toRemove = base_t::remNext(prev_))
      PWX_THROW_FURTHER

      // We need to maintain prev pointer(s) before returning toRemove:
      if (prev_)
        {
          // curr->next is the former next of the element to remove:
          if (curr->next)
            curr->next->prev = curr;
        }
      else if (head)
        // the old head is removed
        head->prev = nullptr;
      return toRemove;
    }

  /** @brief remove the element after the specified element
    *
    * This method removes the element in the list after the element
    * @a prev_ and returns a pointer to the removed element.
    *
    * If @a prev_ is set to nullptr, the root element (aka head) is
    * removed.
    *
    * You have to delete the removed element by yourself. If you do not intent
    * to work with the removed element, use delNextElem instead.
    *
    * If @a prev_ is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * If there is no item behind the  @a prev_ or if the list is empty,
    * a pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] prev_ the element that precedes the element to remove
    * @return a pointer to the removed element
  **/
  elem_t* remNextElem(elem_t* prev_)
    {
      elem_t* toRemove = nullptr;
      PWX_LOCK_GUARD(list_t, this)
      PWX_TRY(toRemove = base_t::remNextElem(prev_))
      PWX_THROW_FURTHER

      // We need to maintain prev pointer(s) before returning toRemove:
      if (prev_)
        {
          // prev_->next is the former next of the element to remove:
          if (prev_->next)
            prev_->next->prev = prev_;
        }
      else if (head)
        // the old head is removed
        head->prev = nullptr;
      return toRemove;
    }

  /** @brief remove the element before the element holding the specified data
    *
    * This method removes the element in the list before the element
    * that holds @a next_ and returns a pointer to the removed element.
    *
    * If @a next_ is set to nullptr, the last element (aka tail) is
    * removed.
    *
    * You have to delete the removed element by yourself. If you do not intent
    * to work with the removed element, use delPrev instead.
    *
    * If there is no item before the element @a next_ holds, a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] next_ the data the element that succeeds the element to remove holds
    * @return a pointer to the removed element
  **/
  elem_t* remPrev(data_t* next_)
    {
      PWX_LOCK_GUARD(list_t, this)

      if (next_ && (nullptr == find(next_)) )
        // find sets curr to the correct value.
        PWX_THROW("ItemNotFound", "Item not found", "The searched item can not be found in this doubly linked list")

      if (next_ && (nullptr == curr->prev) )
        PWX_THROW("OutOfRange", "Item out of range", "There is no element before the element holding the given next_ pointer")

      elem_t* toRemove = next_ ? curr->prev : tail;

      // Lock both curr and the item to remove:
      PWX_DOUBLE_LOCK(elem_t, curr, elem_t, toRemove);

      if (nullptr == next_)
        {
          if (tail == curr)
            // curr needs to stay valid on the current tail
            curr = toRemove->prev;

          // if this was the last item, sanitize the list:
          if (nullptr == toRemove->prev)
            {
              head = nullptr;
              curr = nullptr;
              tail = nullptr;
              eNr  = 0;
            }
          else
            {
              tail = toRemove->prev;
              tail->next = nullptr;
            }
        }
      else
        {
          curr->prev = toRemove->prev;
          if (curr->prev)
            curr->prev->next = curr;
        }

      // head has to be checked now
      if (head == toRemove)
        head = curr;

      // if eNr is greater than zero it needs to be lowered.
      // - If tail is removed, curr has been set to the new tail
      // - Otherwise an element before curr was removed
      --eNr;

      toRemove->next = nullptr;
      toRemove->prev = nullptr;
      --eCount;

      return toRemove;
    }

  /** @brief remove the element before the specified element
    *
    * This method removes the element in the list before the element
    * @a next_ and returns a pointer to the removed element.
    *
    * If @a next_ is set to nullptr, the last element (aka tail) is
    * removed.
    *
    * You have to delete the removed element by yourself. If you do not intent
    * to work with the removed element, use delPrevElem instead.
    *
    * If @a next_ is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * If there is no item before @a next_ or if the list is empty,
    * a pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] next_ the element that succeeds the element to remove
    * @return a pointer to the removed element
  **/
  elem_t* remPrevElem(elem_t* next_)
    {
      PWX_LOCK_GUARD(list_t, this)

      if (next_ && (nullptr == next_->prev) )
        PWX_THROW("OutOfRange", "Item out of range", "There is no element before the given prev_ element")

      if (0 == eCount)
        PWX_THROW("OutOfRange", "Item out of range", "The list is empty")

      elem_t* toRemove = next_ ? next_->prev : tail;

      // Lock the item to remove
      toRemove->lock();

      if (next_)
        next_->prev = toRemove->prev;
      else
        {
          // if this was the last item, sanitize the list:
          if (nullptr == toRemove->prev)
            {
              head = nullptr;
              curr = nullptr;
              tail = nullptr;
              eNr  = 0;
            }
          else
            {
              tail = toRemove->prev;
              tail->next = nullptr;
            }
        }

      if (toRemove->prev)
        toRemove->prev->next = toRemove->next;
      else if (head == toRemove)
        head = next_;

      toRemove->next = nullptr;
      toRemove->prev = nullptr;

      // curr is reseted to head, because we can't maintain eNr otherwise.
      curr = head;
      eNr  = 0;
      --eCount;
      toRemove->unlock();

      return toRemove;
    }

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
  const elem_t* operator[](const int32_t index_) const noexcept
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
  elem_t* operator[](int32_t index_) noexcept
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
  using base_t::destroy;

  /* ===============================================
   * === Protected operators                     ===
   * ===============================================
  */

  /* ===============================================
   * === Protected members                       ===
   * ===============================================
  */
  using base_t::eCount;
  using base_t::eNr;
  using base_t::curr;
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

  /// @brief little method to maintain the prev pointers of @a prev_ or head and its next, returns eCount
  virtual uint32_t privAddPrev(elem_t* prev_)
    {
      if (prev_)
        {
          // prev_ is the new element
          if (prev_->next)
            {
              elem_t* newElement = prev_->next;
              if (newElement->next)
                newElement->next->prev = newElement;
              newElement->prev = prev_;
            }
        }
      else if (head && head->next)
        head->next->prev = head;

      return eCount;
    }

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

          // Is xIdx the next member, like in an upward for loop?
          if (xIdx == (eNr + 1))
            {
              curr = curr->next;
              ++eNr;
              return curr;
            }

          // Is xIdx the prev member, like in a downward for loop?
          if (xIdx == (eNr - 1))
            {
              curr = curr->prev;
              --eNr;
              return curr;
            }

          // Is it the head we want?
          if (0 == xIdx)
            return head;

          // Or tail ?
          if ((eCount - 1) == xIdx)
            return tail;

          /* Manual search with four possibilities:
           * A) xIdx is between eNr and eCount
           *   1: xIdx is nearer to eNr    -> move upwards from curr
           *   2: xIdx is nearer to eCount -> move downwards from tail
           * B) xIdx is between 0 and eNr
           *   1: xIdx is nearer to 0      -> move upwards from head
           *   2: xIdx is nearer to eNr    -> move downwards from curr
          */
          bool goUp = true;
          if (xIdx > eNr)
            {
              // A) xIdx is between eNr and eCount
              if ( (xIdx - eNr) <= ((eCount - eNr) / 2))
                {
                   // 1: xIdx is nearer to eNr -> move upwards from curr
                   curr = curr->next;
                   ++eNr;
                }
              else
                {
                   // 2: xIdx is nearer to eCount -> move downwards from tail
                   goUp = false;
                   curr = tail->prev;
                   eNr  = eCount - 2;
                }
            } // end of group A
          else
            {
              // B) xIdx is between 0 and eNr
              if (xIdx <= (eNr / 2))
                {
                   // 1: xIdx is nearer to 0 -> move upwards from head
                   curr = head->next;
                   eNr  = 1;
                }
              else
                {
                   // 2: xIdx is nearer to eNr -> move downwards from curr
                   goUp = false;
                   curr = curr->prev;
                   --eNr;
                }
            } // end of group B

          /* Note:
           * This simple loop without checks works for two reasons:
           * - head, tail and curr are already checked and can be skipped
           * - xIdx is sure to be anywhere between 0 and (eCount - 1)
          */

          // Now solve the move by a simple while loop:
          while (xIdx != eNr)
            {
              curr = goUp ? curr->next : curr->prev;
              eNr += goUp ? 1 : -1;
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
}; // class TDoubleList

/** @brief default destructor
  *
  * This destructor will delete all items currently stored. There is no
  * need to clean up manually before deleting the list.
**/
template<typename data_t, typename elem_t>
TDoubleList<data_t, elem_t>::~TDoubleList() noexcept
  {
    /* --- clearing is done in the TSingleList destructor --- */
  }

} // namespace pwx

#endif // PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED
