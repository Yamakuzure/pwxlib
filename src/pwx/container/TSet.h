#pragma once
#ifndef PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED
#define PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED 1

/** @file TSet.h
  *
  * @brief Declaration of the TSet template to store unique items
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

#include <pwx/types/CLockable.h>
#include <pwx/general/macros.h>
#include <pwx/container/TDoubleList.h>

namespace pwx {

/** @class TSet
  *
  * @brief Template to build sorted and unsorted sets of variable types
  *
  * The set is a more advanced container using a doubly linked list to manage its
  * data pointers. Every item is checked for uniqueness before storing it in a set.
  * This means that every single item is unique within a set.
  *
  * This is done on the data level, not pointer level. This makes it necessary for
  * any data to support operator==(T). Alternatively an operator==(T,T) must be
  * available.
  *
  * On construction it can be decided to have the set being sorted, which is done
  * on insertion. The push() and pop() operations mirror a set, thus it has to
  * be decided whether to keep the set character of the set or gain speed on
  * insertions and random access by ordering the items.
  *
  * Unsorted sets will generally be much closer to O(n) on any insertion and and
  * random access than sorted sets.
  *
  * Apart from this the major difference to a set is the support for the random
  * access using operator[].
  *
  * The constructor takes an optional destroy(T*) function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * It is recommended that you use the much more advanced std::set or std::deque
  * unless you need to store a very large number of elements and can not live with
  * the downside of every element having to be copied into the std container.
  *
  * If PWX_THREADS is defined, changes to the element are done in a locked state.
**/
template<typename data_t>
class TSet
{
public:
  /* ===============================================
   * === Public types                            ===
   * ===============================================
  */

  typedef TSet<data_t>           list_t;
  typedef TDoubleElement<data_t> elem_t;

  /* ===============================================
   * === Public Constructors and destructors     ===
   * ===============================================
  */

  /** @brief destroy function constructor
    *
    * The destroy function constructor initializes an empty unsorted set.
    *
    * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
  **/
  TSet(void (*destroy_)(data_t* data_)) noexcept
  : sList(destroy_)
    { /* nothing to be done here */ }

  /** @brief empty constructor
    *
    * The empty constructor sets the data destroy method to the null pointer.
  **/
  TSet() noexcept
  : sList(nullptr)
    { /* nothing to be done here */ }

  /** @brief bool constructor
    *
    * The bool constructor sets the data destroy method to the null pointer.
    *
    * @param[in] sorted_ whether the set should be sorted or not
  **/
  TSet(bool sorted_) noexcept
  : isSorted(sorted_),
    sList(nullptr)
    { /* nothing to be done here */ }

  /** @brief default constructor
    *
    * The default constructor initializes an empty set.
    *
    * @param[in] sorted_ whether the set should be sorted or not
    * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
  **/
  TSet(bool sorted_, void (*destroy_)(data_t* data_)) noexcept
  : isSorted(sorted_),
    sList(destroy_)
    { /* nothing to be done here */ }

  virtual ~TSet() noexcept;

  TSet(const list_t &rhs) PWX_DELETE; // No copy ctor

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
  void clear() noexcept
    {
      sList.clear();
    }

  /** @brief push a new data pointer onto the set
    *
    * This is the regular set operation to add a new element.
    * Being a set this new element is put on top of it.
    *
    * If the set is sorted, the element will be moved to the correct
    * location and therefore not explicitly appear on top.
    *
    * To add a new data pointer to the bottom, use unshift().
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ItemCreationFailed" is thrown.
    *
    * If there is an item with the same data stored in the set,
    * a pwx::CException with the name "DataUniquenessFailed" is
    * thrown.
    *
    * @param[in] data_ data pointer to store.
    * @return number of elements stored after the operation.
  **/
  uint32_t push(data_t* data_)
    {
      /// @todo Add check
      if (isSorted)
        {
          /// @todo implementation to be done
        }
      else
        {
          PWX_TRY(return sList.insNext(nullptr, data_))
          PWX_THROW_FURTHER
        }
    }

  /** @brief pop the last element from the set
    *
    * This is the regular set operation to get the oldest element.
    * Being a set this element comes from the bottom. If the
    * set is ordered, this is not explicitly the oldest, but the
    * largest element from its data point of view.
    *
    * To get an element from the top, use shift(). For random access
    * use operator[index].
    *
    * The element is removed from the set so you have to take
    * care of its deletion once you are finished with it.
    *
    * If there is no element in the set a pwx::CException with the
    * name "OutOfRange" is thrown.
    *
    * @return the bottom element on the set.
  **/
  elem_t* pop()
    {
      if (sList.size() > 1)
        {
          PWX_TRY(return sList.remPrev(nullptr))
          PWX_THROW_FURTHER
        }
      else
        {
          PWX_TRY(return shift())
          PWX_THROW_FURTHER
        }
    }

  /** @brief shift the newest element from the top of the set
    *
    * This is the irregular set operation to get the newest
    * element. If the set is ordered, this is not explicitly
    * the newest, but the smallest element from its data point
    * of view.
    *
    * The regular set operation to get the top element is
    * pop (). For random access use operator[index].
    *
    * The element is removed from the set so you have to take
    * care of its deletion once you are finished with it.
    *
    * If there is no element in the set a pwx::CException with the
    * name "OutOfRange" is thrown.
    *
    * @return the top element from the set.
  **/
  elem_t* shift()
    {
      PWX_TRY(return sList.remNext(nullptr))
      PWX_THROW_FURTHER
    }

  /// @brief return the number of stored elements
  uint32_t size() const noexcept { return sList.size(); }

  /** @brief unshift a new data pointer under the bottom of the set
    *
    * This is the irregular set operation to add a new data
    * pointer under the set.
    *
    * If the set is sorted, the element will be moved to the correct
    * location and therefore not explicitly appear at the bottom.
    *
    * The regular set operation to add a new data pointer
    * onto the top is push().
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ItemCreationFailed" is thrown.
    *
    * If there is an item with the same data stored in the set,
    * a pwx::CException with the name "DataUniquenessFailed" is
    * thrown.
    *
    * @param[in] data_ data pointer to store.
    * @return number of elements stored after the operation.
  **/
  uint32_t unshift(data_t* data_)
    {
      /// @todo : add check
      if (isSorted)
        {
          PWX_TRY(return push(data_)) // Its the same if the set is sorted
          PWX_THROW_FURTHER
        }
      else if (sList.size())
        {
          PWX_TRY(return sList.insPrev(nullptr, data_))
          PWX_THROW_FURTHER
        }
      else
        {
          PWX_TRY(return push(data_))
          PWX_THROW_FURTHER
        }
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
    * If the set is empty, the operator returns nullptr.
    *
    * @param[in] index the index of the element to find.
    * @return read-only pointer to the element, or nullptr if the set is empty.
  **/
  const elem_t* operator[](const int32_t index) const noexcept
    {
      return sList[index];
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
    * If the set is empty, the operator returns nullptr.
    *
    * @param[in] index the index of the element to find.
    * @return read/write pointer to the element, or nullptr if the set is empty.
  **/
  elem_t* operator[](int32_t index) noexcept
    {
      return sList[index];
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

private:
  /* ===============================================
   * === Private Constructors and destructors    ===
   * ===============================================
  */

  /* ===============================================
   * === Private methods                         ===
   * ===============================================
  */

  /* ===============================================
   * === Private operators                       ===
   * ===============================================
  */

  /* ===============================================
   * === Private members                         ===
   * ===============================================
  */
  const bool          isSorted = false; //!< Whether to sort items on insertion or not.
  TDoubleList<data_t> sList;            //!< Data is held by a doubly linked list.

}; // class TSet

/** @brief default destructor
  *
  * This destructor will delete all items currently stored. There is no
  * need to clean up manually before deleting the set.
**/
template<typename data_t>
TSet<data_t>::~TSet() noexcept
  {
    sList.clear();
  }

} // namespace pwx
#endif // PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED

