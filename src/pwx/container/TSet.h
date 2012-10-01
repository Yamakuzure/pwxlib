#pragma once
#ifndef PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED
#define PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED 1

/** @file TSet.h
  *
  * @brief Declaration of the TSet template to store unique elements
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
  * @brief Template to build sets of variable types
  *
  * A set is a group of elements, where each element exists exactly once. Two
  * sets are equal, if their members are equal. Therefore the sets {1, 2, 3} and
  * {3, 2, 1} are equal. Although sets are unordered, the default constructor
  * will build an ordered set to speed up the access.
  *
  * If the set is needed to be unordered, it must be constructed with "false" as
  * an argument.
  *
  * Unsorted sets will generally be much closer to O(n) on any insertion and
  * random access than sorted sets.
  *
  * The set uses a doubly linked list to manage its data pointers. Every element is
  * checked for uniqueness before storing it in a set.
  *
  * This is done on the data level, not pointer level. This makes it necessary for
  * any data to support operator==(T). Alternatively an operator==(T,T) must be
  * available.
  *
  * The constructor takes an optional destroy(T*) function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * It is recommended that you use the much more advanced std::set unless you need
  * to store a very large number of elements and can not live with the downside of
  * every element having to be copied into the std container.
  *
  * If PWX_THREADS is defined, changes to the element are done in a locked state.
**/
template<typename data_t>
class TSet : public CLockable
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
  TSet(void (*destroy_)(data_t* data)) noexcept
  : destroy(destroy_),
    sList(destroy_)
    { /* nothing to be done here */ }

  /** @brief empty constructor
    *
    * The empty constructor sets the data destroy method to the null pointer.
  **/
  TSet() noexcept
  : destroy(nullptr), sList(nullptr)
    { /* nothing to be done here */ }

  /** @brief bool constructor
    *
    * The bool constructor sets the data destroy method to the null pointer.
    *
    * @param[in] sorted_ whether the set should be sorted or not
  **/
  TSet(bool sorted_) noexcept
  : destroy(nullptr),
    isSorted(sorted_),
    sList(nullptr)
    { /* nothing to be done here */ }

  /** @brief default constructor
    *
    * The default constructor initializes an empty set.
    *
    * @param[in] sorted_ whether the set should be sorted or not
    * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
  **/
  TSet(bool sorted_, void (*destroy_)(data_t* data)) noexcept
  : destroy(destroy_),
    isSorted(sorted_),
    sList(destroy_)
    { /* nothing to be done here */ }

  virtual ~TSet() noexcept;

  TSet(const list_t &rhs) PWX_DELETE; // No copy ctor

  /* ===============================================
   * === Public methods                          ===
   * ===============================================
  */

  /** @brief add a new data pointer to the set
    *
    * If the set is sorted, the element will be moved to the correct
    * location. If the set is not sorted, it will be added to the end.
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ElementCreationFailed" is thrown.
    *
    * If there is an element with the same data stored in the set,
    * the new pointer is not stored but silently ignored.
    *
    * @param[in] data data pointer to store.
    * @return number of elements stored after the operation.
  **/
  uint32_t add(data_t* data)
    {
      uint32_t listSize = sList.size();

      if (privFind(*data))
        return listSize;


      if (isSorted && listSize)
        {
          /// Case A: The new element is the new smallest element:
          elem_t* head = sList[0];
          if (**head > *data)
            {
              PWX_TRY(return sList.insNext(nullptr, data))
              PWX_THROW_FURTHER
            }

          /// Case B: The new element is the new largest element:
          elem_t* tail = sList[-1];
          if (*data > **tail)
            {
              PWX_TRY(return sList.insPrev(nullptr, data))
              PWX_THROW_FURTHER
            }

          /// Case C: We have to search for the right location:
          if (nullptr == prev)
            prev = head->next;

          /* downwards: */
          if (**prev > *data)
            {
              while (**prev > *data)
                prev = prev->prev;
              // Now prev is the first smaller element
              PWX_TRY(return sList.insNextElem(prev, data))
              PWX_THROW_FURTHER
            }

          /* upwards : */
          while (*data > **prev)
            prev = prev->next;
          // Now prev is the first larger element
          PWX_TRY(return sList.insPrevElem(prev, data))
          PWX_THROW_FURTHER
        }
      else
        {
          PWX_TRY(return sList.insPrev(nullptr, data))
          PWX_THROW_FURTHER
        }
    }


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


  /** @brief build the difference of this set with another
    *
    * The difference of two sets consists of all elements that
    * are present in this set but not the other.
    *
    * This method returns a newly build set consisting of copies
    * of the elements that are present in this but not the other set.
    *
    * If both sets are equal, an empty set is returned.
    *
    * If the new set can not be created, a pwx::CException with
    * the name "SetCreationFailed" is thrown.
    *
    * If an element can not be created, a pwx::CException with the
    * name "ElementCreationFailed" is thrown.
    *
    * If the data of an element could not be copied, a pwx::CException
    * with the name "CopyingDataFailed" is thrown.
    *
    * @param rhs pointer to the set to compare with
    * @return pointer to the difference set. You have to delete it yourself!
  **/
  list_t* differenceFrom(list_t* rhs)
    {
      list_t* diff = nullptr;
      PWX_TRY(diff = new list_t(isSorted, destroy))
      PWX_THROW_STD_FURTHER("SetCreationFailed", "TSet::differenceFrom() could not create a difference set.")

      // Return an empty set if a difference with this is asked for:
      if (this == rhs)
        return diff;

      // Otherwise build a difference
      size_t lSize = sList.size();
      for (size_t i = 0; i < lSize; ++i)
        {
          elem_t* curr = sList[i];
          if (!rhs->privFind(**curr))
            {
              // This element exists only in this set, so add a new element to the difference:
              data_t* newData = nullptr;
              PWX_TRY(newData = new data_t(**curr))
              PWX_THROW_STD_FURTHER("CopyingDataFailed", "TSet::differenceFrom() could not copy different data.")
              PWX_TRY(diff->add(newData))
              PWX_THROW_FURTHER
            }
        } // End of walking through the elements

      return diff;
    }


  /// @brief return true if the set is empty, false otherwise
  bool empty() const noexcept { return !sList.size(); }


  /** @brief find the element with the given @a data
    *
    * This method searches through the set and returns the element
    * with the given @a data or nullptr if @a data is not stored in this
    * set.
    *
    * <B>Important</B>: Sets work on data level searching for the data
    * and <I>not</I> pointers. This is very different from the behavior
    * of TSingleList and TDoubleList.
    *
    * @param data reference to find
    * @return return a pointer to the element storing @a data
  **/
  elem_t* find(data_t &data) noexcept
    {
      return const_cast<elem_t* >(privFind(static_cast<const data_t>(data)));
    }


  /** @brief find the element with the given @a data
    *
    * This method searches through the set and returns a const pointer
    * to the element with the given @a data or nullptr if @a data is not stored
    * in this set.
    *
    * <B>Important</B>: Sets work on data level searching for the data
    * and <I>not</I> pointers. This is very different from the behavior
    * of TSingleList and TDoubleList.
    *
    * @param data reference to find
    * @return return a const pointer to the element storing @a data
  **/
  const elem_t* find(const data_t &data) const noexcept
    {
      return privFind(data);
    }


  /** @brief build the intersection of this set with another
    *
    * The intersection of two sets consists of all elements that
    * are present in both sets.
    *
    * This method returns a newly build set consisting of copies
    * of the elements that are present in both sets.
    *
    * If both sets do not intersect, an empty set is returned.
    *
    * The intersection of the set with itself is the set. This means
    * that you will get a pointer to this set and not a copy if you
    * write something like "foo = bar->intersects(bar);"
    *
    * If the new set can not be created, a pwx::CException with
    * the name "SetCreationFailed" is thrown.
    *
    * If an intersecting element can not be created, a pwx::CException
    * with the name "ElementCreationFailed" is thrown.
    *
    * If the data of an intersecting element could not be copied, a
    * pwx::CException with the name "CopyingDataFailed" is thrown.
    *
    * @param rhs pointer to the set to compare with
    * @return pointer to the intersecting set. You have to delete it yourself!
  **/
  list_t* intersectionWith(list_t* rhs)
    {
      list_t* inters = nullptr;
      PWX_TRY(inters = new list_t(isSorted, destroy))
      PWX_THROW_STD_FURTHER("SetCreationFailed", "TSet::intersectionWith() could not create an intersection set.")

      // Return this if an intersection with this is asked for:
      if (this == rhs)
        return this;

      // Otherwise build an intersection
      size_t lSize = sList.size();
      for (size_t i = 0; i < lSize; ++i)
        {
          elem_t* curr = sList[i];
          if (rhs->privFind(**curr))
            {
              // This element exists in both, so add a new element to the intersection:
              data_t* newData = nullptr;
              PWX_TRY(newData = new data_t(**curr))
              PWX_THROW_STD_FURTHER("CopyingDataFailed", "TSet::intersectionWith() could not copy intersecting data.")
              PWX_TRY(inters->add(newData))
              PWX_THROW_FURTHER
            }
        } // End of walking through the elements

      return inters;
    }


  /** @brief alias to remove the last element (tail)
    * @return the last element or nullptr if the set is empty
  **/
  elem_t* pop() noexcept
    {
      return remove(-1);
    }


  /** @brief add an element to the end of the set
    *
    * If the set is sorted, the element will be moved to the correct
    * location. If the set is not sorted, it will be added to the end.
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ElementCreationFailed" is thrown.
    *
    * If there is an element with the same data stored in the set,
    * the new pointer is not stored but silently ignored.
    *
    * @param[in] data data pointer to store.
    * @return number of elements stored after the operation.
  **/
  uint32_t push(data_t* data)
    {
      PWX_TRY(return add(data))
      PWX_THROW_FURTHER
    }


  /** @brief remove an element from the set
    *
    * This method removes and returns the element with
    * the index @a index. If the set is empty, a nullptr
    * is returned.
    *
    * @param[in] idx index of the element, wrapped if negative or out of bounds.
    * @return a pointer to the removed element or nullptr if the set is empty.
  **/
  elem_t* remove(int32_t index) noexcept
    {
      elem_t* toRemove = sList[index];

      if (toRemove)
        sList.remElem(toRemove);

      return toRemove;
    }


  /** @brief alias to remove the first element (head)
    * @return the first element or nullptr if the set is empty
  **/
  elem_t* shift() noexcept
    {
      return remove(0);
    }


  /// @brief return the number of stored elements
  uint32_t size() const noexcept { return sList.size(); }


  /** @brief return true if this set is a subset of @a rhs
    *
    * The set is a subset of @a rhs if @a rhs contains all elements
    * of this set.
    *
    * @param[in] rhs pointer to the set to compare with
    * @return true if this is a subset of @a rhs, false otherwise
  **/
  bool subsetOf(list_t* rhs)
    {
      if (this == rhs)
        return true;

      size_t lSize  = sList.size();
      bool   result = true;
      for (size_t i = 0; result && (i < lSize); ++i)
        {
          if (!rhs->privFind(**sList[i]))
            result = false;
        } // End of walking through the elements
      return result;
    }


  /** @brief build the union of this set with another
    *
    * The union of two sets consists of all elements that
    * are present in either set.
    *
    * This method returns a newly build set consisting of copies
    * of the elements that are present in either set.
    *
    * If the new set can not be created, a pwx::CException with
    * the name "SetCreationFailed" is thrown.
    *
    * If an element can not be created, a pwx::CException with the
    * name "ElementCreationFailed" is thrown.
    *
    * If the data of an element could not be copied, a pwx::CException
    * with the name "CopyingDataFailed" is thrown.
    *
    * @param rhs pointer to the set to compare with
    * @return pointer to the union set. You have to delete it yourself!
  **/
  list_t* unionWith(list_t* rhs)
    {
      list_t* unionSet = nullptr;
      PWX_TRY(unionSet = new list_t(isSorted, destroy))
      PWX_THROW_STD_FURTHER("SetCreationFailed", "TSet::unionWith() could not create a union set.")

      // Otherwise add all from this...
      size_t lSize = sList.size();
      for (size_t i = 0; i < lSize; ++i)
        {
          data_t* newData = nullptr;
          PWX_TRY(newData = new data_t(**sList[i]))
          PWX_THROW_STD_FURTHER("CopyingDataFailed", "TSet::unionWith() could not copy uinion data.")
          PWX_TRY(unionSet->add(newData))
          PWX_THROW_FURTHER
        } // End of walking through the elements

      // Then add all from the other
      lSize = rhs->size();
      for (size_t i = 0; i < lSize; ++i)
        {
          elem_t* curr = *rhs[i];
          if (!unionSet->privFind(**curr))
            {
              data_t* newData = nullptr;
              PWX_TRY(newData = new data_t(**curr))
              PWX_THROW_STD_FURTHER("CopyingDataFailed", "TSet::unionWith() could not copy uinion data.")
              PWX_TRY(unionSet->add(newData))
              PWX_THROW_FURTHER
            }
        } // End of walking through the elements

      return unionSet;
    }


  /** @brief add an element to the end of the set
    *
    * If the set is sorted, the element will be moved to the correct
    * location. If the set is not sorted, it will be added to the started.
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ElementCreationFailed" is thrown.
    *
    * If there is an element with the same data stored in the set,
    * the new pointer is not stored but silently ignored.
    *
    * @param[in] data data pointer to store.
    * @return number of elements stored after the operation.
  **/
  uint32_t unshift(data_t* data)
    {
      if (isSorted)
        {
          PWX_TRY(return add(data))
          PWX_THROW_FURTHER
        }
      else
        {
          PWX_TRY(return sList.insNext(nullptr, data))
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


  /** @brief return true if this set equals another
    *
    * @param[in] rhs right hand side operand
    * @return true if both sets are equal
  **/
  bool operator==(const list_t &rhs) const noexcept
    {
      size_t lSize  = sList.size();
      size_t rSize  = rhs.size();

      // Check A: Both sets must have equal size:
      bool result = (lSize == rSize);

      // Check B: The this must be a subset of rhs
      if (result)
        result = subsetOf(&rhs);

      return result;
    }


  /** @brief return true if this set is different from another
    *
    * @param[in] rhs right hand side operand
    * @return true if both sets are not equal
  **/
  bool operator!=(const list_t &rhs) const noexcept
    {
      return (!operator==(rhs));
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
  void (*destroy)(data_t* data_);

  elem_t* privFind(data_t &data) noexcept
    {
      PWX_LOCK_GUARD(list_t, const_cast<list_t*>(this))

      uint32_t listSize = sList.size();
      if (0 == listSize)
        return nullptr;

      // Allow a quick out if prev has been set and a neighbor (or prev) is wanted:
      if (prev && prev->data)
        {
          if (**prev == data)
            return prev;
          else if (prev->next && (**prev->next == data))
            {
              prev = prev->next;
              return prev;
            }
          else if (prev->prev && (**prev->prev == data))
            {
              prev = prev->prev;
              return prev;
            }
        }

      /* another quick exit if head or tail are wanted: */
      elem_t* head = sList[0];
      elem_t* tail = sList[-1];
      if (**head == data)
        return head;
      if (listSize > 1)
        {
          if (**tail == data)
            return tail;
        }
      else
        return nullptr; // head is alone and not the searched one.

      /* There are two completely different situations here:
       * A) The set is sorted.
       *    In this case we do not only search for the data but maintain this->prev
       *    to point to the prev of the next larger element if data can not be found.
       *    push()/unshift() then use this->prev for an sList.insNextElem()
       * B) The set is unsorted.
       *    Here we have to walk all the way through the whole sList until the
       *    element is found or tail is reached.
       *    However, prev stores the last accessed element for a speedier access to
       *    its neighbors
      */

      /// Situation A)
      if (isSorted)
        {
          if (nullptr == prev)
            // Oh no, start from scratch
            prev = head;

          /* Go downwards: */
          while ((head != prev) && (**prev > data))
            prev = prev->prev;
          /* Go upwards: */
          while ((tail != prev) && (data > **prev))
            prev = prev->next;

          /* Return either prev or nullptr now */
          return (**prev == data ? prev : nullptr);
        } // End of find in a sorted set

      /// Situation B)
      prev = head->next;
      while ((tail != prev) && !(data == **prev))
        prev = prev->next;

      /* Return either prev or nullptr now */
      return (**prev == data ? prev : nullptr);
    }

  /* ===============================================
   * === Private operators                       ===
   * ===============================================
  */

  /* ===============================================
   * === Private members                         ===
   * ===============================================
  */
  const bool          isSorted = true;  //!< Whether to sort elements on insertion or not.
  TDoubleList<data_t> sList;            //!< Data is held by a doubly linked list.
  mutable
  elem_t*             prev     = nullptr; //!< Set by privFind and utilized by push() and unshift() if isSorted is true.

}; // class TSet

/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the set.
**/
template<typename data_t>
TSet<data_t>::~TSet() noexcept
  {
    sList.clear();
  }

} // namespace pwx
#endif // PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED

