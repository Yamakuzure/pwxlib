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
  TDoubleList(void (*destroy_)(data_t* data)) noexcept
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

  /** @brief delete the element holding the specified data
    *
    * This method deletes the element in the list that holds @a data.
    *
    * If you intent to work with the element, use remData instead.
    *
    * @param[in] data the data the element that is to be deleted holds
    * @return the number of elements remaining in the list after the deletion.
  **/
  virtual uint32_t delData(data_t* data)
    {
      PWX_LOCK_GUARD(base_t, this)
      try
        {
          elem_t* removed = remData(data);
          if (removed)
            delete removed;
          return eCount;
        }
      PWX_THROW_STD_FURTHER("delete", "Deleting an element in TDoubleList::delData() failed.")
    }

  /** @brief delete the specified element
    *
    * This method deletes the element @a elem.
    *
    * If you intent to work with the element, use remElem instead.
    *
    * If @a elem is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * @param[in] elem the element that is to be deleted
    * @return the number of elements remaining in the list after the deletion.
  **/
  virtual uint32_t delElem(elem_t* elem)
    {
      PWX_LOCK_GUARD(base_t, this)
      try
        {
          elem_t* removed = remElem(elem);
          if (removed)
            delete removed;
          return eCount;
        }
      PWX_THROW_STD_FURTHER("delete", "Deleting an element in TDoubleList::delElem() failed.")
    }

  using base_t::delNext;
  using base_t::delNextElem;

  /** @brief delete the element before the element holding the specified data
    *
    * This method deletes the element in the list before the element
    * that holds @a next.
    *
    * If @a next is set to nullptr, the last element (aka tail) is
    * deleted.
    *
    * If you intent to work with the element, use remPrev instead.
    *
    * If there is no item before the element holding @a next, a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] next the data the element that succeeds the element to delete holds
    * @return the number of elements remaining in the list after the deletion.
  **/
  virtual uint32_t delPrev(data_t* next)
    {
      PWX_LOCK_GUARD(base_t, this)
      try
        {
          elem_t* removed = remPrev(next);
          if (removed)
            delete removed;
          return eCount;
        }
      PWX_THROW_FURTHER
      PWX_THROW_STD_FURTHER("delete", "Deleting an element in TDoubleList::delPrev() failed.")
    }

  /** @brief delete the element before the specified element
    *
    * This method deletes the element in the list before the element
    * @a next.
    *
    * If @a next is set to nullptr, the last element (aka tail) is
    * deleted.
    *
    * If you intent to work with the element, use remPrevElem instead.
    *
    * If @a next is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * If there is no item before the element @a next a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] next the element that succeeds the element to delete
    * @return the number of elements remaining in the list after the deletion.
  **/
  virtual uint32_t delPrevElem(elem_t* next)
    {
      PWX_LOCK_GUARD(base_t, this)
      try
        {
          elem_t* removed = remPrevElem(next);
          if (removed)
            delete removed;
          return eCount;
        }
      PWX_THROW_FURTHER
      PWX_THROW_STD_FURTHER("delete", "Deleting an element in TDoubleList::delPrevElem() failed.")
    }

  /** @brief find the item with the given @a data
    *
    * This method searches through the list and returns the item
    * with the given @a data or nullptr if @a data is not stored in this
    * list.
    *
    * @param data pointer to the data to find
    * @return return a pointer to the element storing @a data
  **/
  elem_t* find(data_t* data) noexcept
    {
      return const_cast<elem_t* >(find(static_cast<const data_t* >(data)));
    }

  /** @brief find the item with the given @a data
    *
    * This method searches through the list and returns a const pointer
    * to the item with the given @a data or nullptr if @a data is not stored
    * in this list.
    *
    * @param data pointer to the data to find
    * @return return a const pointer to the element storing @a data
  **/
  const elem_t* find(const data_t* data) const noexcept
    {
      if (nullptr == curr)
        return nullptr;

      // From here on, curr is valid and the list needs to be guarded:
      PWX_LOCK_GUARD(list_t, const_cast<list_t* >(this))

      // Quick exit if curr is already what we want:
      if (curr->data == data)
        return curr;

      // The next does only make sense if we have more than one element
      if (eCount > 1)
        {
          // Exit if head is wanted...
          if (head->data == data)
            {
              curr = head;
              eNr  = 0;
              return head;
            }

          // ...or tail
          if (tail->data == data)
            {
              curr = tail;
              eNr  = eCount - 1;
              return tail;
            }

          // Safe the current position so we can restore curr if we can't find data
          elem_t*  oldCurr = curr;
          uint32_t oldNr   = eNr;
          while (curr != tail)
            {
              ++eNr;
              curr = curr->next;
              if (curr->data == data)
                return curr;
            }

          // As there is no positive result, yet, wander backwards from the old position:
          eNr  = oldNr;
          curr = oldCurr;
          while (curr != head)
            {
              --eNr;
              curr = curr->prev;
              if (curr->data == data)
                return curr;
            }
        } // End of handling a search with more than one element

      return nullptr;
    }

  using base_t::insNext;
  using base_t::insNextElem;

  /** @brief insert a new data pointer before the specified data
    *
    * This method inserts a new element in the list before the element
    * holding @a next.
    *
    * If @a next is set to nullptr, the new element will become the new
    * tail of the list.
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ItemCreationFailed" is thrown.
    *
    * @param[in] next the data the element that should succeed the new element
    * @param[in] data the pointer that is to be added.
    * @return the number of elements in this list after the insertion
  **/
  uint32_t insPrev(data_t* next, data_t* data)
    {
      PWX_LOCK_GUARD(list_t, this)

      if (next && (nullptr == find(next)) )
        // find sets curr to the correct value.
        PWX_THROW("ItemNotFound", "Item not found", "The searched item can not be found in this doubly linked list")

      // First create a new element for data
      elem_t* newElement = nullptr;
      PWX_TRY(newElement = new elem_t(data, destroy))
      PWX_THROW_STD_FURTHER("ItemCreationFailed", "The Creation of a new list item failed.")

      privInsert(next ? curr->prev : nullptr, newElement);

      // Count the new element and give the number back
      return ++eCount;
    }

  /** @brief insert a new data pointer before the specified element
    *
    * This method inserts a new element in the list before the element
    * @a next.
    *
    * If @a next is set to nullptr, the new element will become the new
    * tail of the list.
    *
    * If @a next is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ItemCreationFailed" is thrown.
    *
    * @param[in] next the element that should succeed the new element
    * @param[in] data the pointer that is to be added.
    * @return the number of elements in this list after the insertion
  **/
  uint32_t insPrevElem(elem_t* next, data_t* data)
    {
      PWX_LOCK_GUARD(list_t, this)

      // First create a new element for data
      elem_t* newElement = nullptr;
      PWX_TRY(newElement = new elem_t(data, destroy))
      PWX_THROW_STD_FURTHER("ItemCreationFailed", "The Creation of a new list item failed.")

      privInsert(next ? next->prev : nullptr, newElement);

      // Count the new element and give the number back
      return ++eCount;
    }

  /** @brief remove the element holding the specified data
    *
    * This method removes the element in the list that holds @a data
    * and returns a pointer to the removed element.
    *
    * You have to delete the removed element by yourself. If you do not intent
    * to work with the removed element, use delData instead.
    *
    * @param[in] data the data the element is to be removed holds
    * @return a pointer to the removed element
  **/
  elem_t* remData(data_t* data) noexcept
    {
      elem_t* toRemove = nullptr;

      if (data)
        toRemove = find(data);

      // If we were successful, we have to detach toRemove first:
      if (toRemove)
        privRemove(toRemove->prev, toRemove);

      return toRemove;
    }

  /** @brief remove the specified element
    *
    * This method removes the element @a elem and returns a pointer
    * to the removed element.
    *
    * You have to delete the removed element by yourself. If you do not intent
    * to work with the removed element, use delElem instead.
    *
    * If @a elem is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * @param[in] elem the element that is to be removed
    * @return a pointer to the removed element
  **/
  elem_t* remElem(elem_t* elem) noexcept
    {
      if (elem)
        privRemove(elem->prev, elem);
      return elem;
    }

  using base_t::remNext;
  using base_t::remNextElem;

  /** @brief remove the element before the element holding the specified data
    *
    * This method removes the element in the list before the element
    * that holds @a next and returns a pointer to the removed element.
    *
    * If @a next is set to nullptr, the last element (aka tail) is
    * removed.
    *
    * You have to delete the removed element by yourself. If you do not intent
    * to work with the removed element, use delPrev instead.
    *
    * If there is no item before the element @a next holds, a
    * pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] next the data the element that succeeds the element to remove holds
    * @return a pointer to the removed element
  **/
  elem_t* remPrev(data_t* next)
    {
      PWX_LOCK_GUARD(list_t, this)

      if (next && (nullptr == find(next)) )
        // find sets curr to the correct value.
        PWX_THROW("ItemNotFound", "Item not found", "The searched item can not be found in this doubly linked list")

      if (next && (nullptr == curr->prev) )
        PWX_THROW("OutOfRange", "Item out of range", "There is no element before the element holding the given next pointer")

      elem_t* toRemove = next ? curr->prev : tail;

      // Lock both curr and the item to remove:
      PWX_DOUBLE_LOCK(elem_t, curr, elem_t, toRemove);

      // Now detach toRemove
      privRemove(toRemove->prev, toRemove);

      --eCount;

      return toRemove;
    }

  /** @brief remove the element before the specified element
    *
    * This method removes the element in the list before the element
    * @a next and returns a pointer to the removed element.
    *
    * If @a next is set to nullptr, the last element (aka tail) is
    * removed.
    *
    * You have to delete the removed element by yourself. If you do not intent
    * to work with the removed element, use delPrevElem instead.
    *
    * If @a next is no element of this list, the wrong list is updated
    * and both element counts will be wrong then. So please make sure to
    * use the correct element on the correct list!
    *
    * If there is no item before @a next or if the list is empty,
    * a pwx::CException with the name "OutOfRange" is thrown.
    *
    * @param[in] next the element that succeeds the element to remove
    * @return a pointer to the removed element
  **/
  elem_t* remPrevElem(elem_t* next)
    {
      PWX_LOCK_GUARD(list_t, this)

      if (next && (nullptr == next->prev) )
        PWX_THROW("OutOfRange", "Item out of range", "There is no element before the given prev element")

      if (0 == eCount)
        PWX_THROW("OutOfRange", "Item out of range", "The list is empty")

      elem_t* toRemove = next ? next->prev : tail;

      // Lock the item to remove and then detach it out of the list
      toRemove->lock();
      privRemove(toRemove->prev, toRemove);

      --eCount;
      toRemove->unlock();

      return toRemove;
    }

  /* ===============================================
   * === Public operators                        ===
   * ===============================================
  */
  list_t &operator=(const list_t &rhs) PWX_DELETE; // No assignment

  /** @brief return a read-only pointer to the element with the given @a index
    *
    * This operator retrieves an element by index like an array. The pointer given
    * back is read-only.
    *
    * There will be no exception if the index is out of range, it will be wrapped
    * to press it into the valid range. This means that an index of -1 can be used
    * to retrieve the last element (tail) for instance.
    *
    * If the list is empty, the operator returns nullptr.
    *
    * If you use this operator to quickly access head or tail, neither the currently
    * used internal pointer nor number are changed. Head and tail are given back
    * directly.
    *
    * @param[in] index the index of the element to find.
    * @return read-only pointer to the element, or nullptr if the list is empty.
  **/
  const elem_t* operator[](const int32_t index) const noexcept
    {
      return privGetElementByIndex(index);
    }

  /** @brief return a read/write pointer to the element with the given @a index
    *
    * This operator retrieves an element by index like an array. The pointer given
    * back is write enabled, so use with care.
    *
    * There will be no exception if the index is out of range, it will be wrapped
    * to press it into the valid range. This means that an index of -1 can be used
    * to retrieve the last element (tail) for instance.
    *
    * If the list is empty, the operator returns nullptr.
    *
    * If you use this operator to quickly access head or tail, neither the currently
    * used internal pointer nor number are changed. Head and tail are given back
    * directly.
    *
    * @param[in] index the index of the element to find.
    * @return read/write pointer to the element, or nullptr if the list is empty.
  **/
  elem_t* operator[](int32_t index) noexcept
    {
      return const_cast<elem_t* >(privGetElementByIndex(static_cast<const int32_t>(index)));
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

  /// @brief wrapping method to retrieve an element by any index or nullptr if the list is empty
  virtual const elem_t* privGetElementByIndex(int32_t index) const noexcept
    {
      if (eCount)
        {
          // Mod index into range
          uint32_t xIdx = static_cast<uint32_t>(index < 0
                                                ? eCount - (::std::abs(index) % eCount)
                                                : index % eCount);
          // Unfortunately this results in xIdx equaling eCount
          // (which is wrong) if index is a negative multiple of
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

  /// @brief simple method to insert an element into the list
  virtual void privInsert(elem_t* prev, elem_t* elem)
    {
      if (elem)
        {
          if (prev)
            {
              if (tail == prev)
                tail = elem;
              elem->next = prev->next;
              if (elem->next)
                elem->next->prev = elem;
              prev->next = elem;
              elem->prev = prev;
              // curr is only maintainable if it is prev
              if (curr != prev)
                {
                  // In which case it wouldn't have needed any change.
                  curr = head;
                  eNr  = 0;
                }
            }
          else if (eCount)
            {
              elem->next = head;
              if (head)
                head->prev = elem;
              head = elem;
              ++eNr; // No matter what happened, curr has another element before it now.
            }
          else
            {
              // If we had no elements yet, head and tail need to be set:
              head = elem;
              tail = elem;
              curr = head;
            }
        } // End of having an element to insert
    }

  /// @brief simple method to remove an element from the list
  virtual void privRemove(elem_t* prev, elem_t* elem)
    {
      if (elem)
        {
          // maintain tail and head first
          if (tail == elem)
            tail = elem->prev;

          if (elem == head)
            head = elem->next;

          // now maintain the neighbors
          if (elem->next)
            elem->next->prev = elem->prev;
          if (elem->prev)
            elem->prev->next = elem->next;

          // curr needs to be valid
          if (curr == elem)
            {
              if (elem->next)
                curr = elem->next;
              else if (prev)
                {
                  curr = prev;
                  --eNr;
                }
            }
          else
            {
              curr = head;
              eNr  = 0;
            } // End of maintaining curr

          // if this was the last item, sanitize the list:
          if (1 == eCount)
            {
              head = nullptr;
              curr = nullptr;
              tail = nullptr;
              eNr  = 0;
            }

          // Finally elem does not need pointers to its neighbors any more
          elem->next = nullptr;
          elem->prev = nullptr;
        } // end of having an element to remove
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
