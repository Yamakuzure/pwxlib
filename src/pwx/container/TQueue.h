#pragma once
#ifndef PWX_LIBPWX_PWX_CONTAINER_TQUEUE_H_INCLUDED
#define PWX_LIBPWX_PWX_CONTAINER_TQUEUE_H_INCLUDED 1

#include <pwx/container/TDoubleList.h>

namespace pwx {

/** @class TQueue
  *
  * @brief
  *
  * @todo : Describe properly
**/
template<typename data_t>
class TQueue
{
public:
  /* ===============================================
   * === Public types                            ===
   * ===============================================
  */

  typedef TQueue<data_t> list_t;
  typedef TDoubleElement<data_t> elem_t;

  /* ===============================================
   * === Public Constructors and destructors     ===
   * ===============================================
  */

  /** @brief default constructor
    *
    * The default constructor initializes an empty stack.
    *
    * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
  **/
  TQueue(void (*destroy_)(data_t* data_)) noexcept
  : sList(destroy_)
    { /* nothing to be done here */ }

  /** @brief empty constructor
    *
    * The empty constructor sets the data destroy method to the null pointer.
  **/
  TQueue() noexcept
  : sList(nullptr)
    { /* nothing to be done here */ }

  virtual ~TQueue() noexcept;

  TQueue(const list_t &rhs) PWX_DELETE; // No copy ctor

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

  /** @brief push a new data pointer onto the queue
    *
    * This is the regular queue operation to add a new element.
    * Being a queue this new element is put on top of it.
    *
    * To add a new data pointer to the bottom, use unshift().
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ItemCreationFailed" is thrown.
    *
    * @param[in] data_ data pointer to store.
    * @return number of elements stored after the operation.
  **/
  uint32_t push(data_t* data_)
    {
      PWX_TRY(return sList.insNext(nullptr, data_))
      PWX_THROW_FURTHER
    }

  /** @brief pop the last element from the queue
    *
    * This is the regular queue operation to get the oldest element.
    * Being a queue this element comes from the bottom.
    *
    * To get an element from the top, use shift().
    *
    * The element is removed from the queue so you have to take
    * care of its deletion once you are finished with it.
    *
    * If there is no element in the queue a pwx::CException with the
    * name "OutOfRange" is thrown.
    *
    * @return the bottom element on the queue.
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

  /** @brief shift the newest element from the top of the queue
    *
    * This is the irregular queue operation to get the newest
    * element.
    *
    * The regular queue operation to get the top element is
    * pop ().
    *
    * The element is removed from the queue so you have to take
    * care of its deletion once you are finished with it.
    *
    * If there is no element in the queue a pwx::CException with the
    * name "OutOfRange" is thrown.
    *
    * @return the top element from the queue.
  **/
  elem_t* shift()
    {
      PWX_TRY(return sList.remNext(nullptr))
      PWX_THROW_FURTHER
    }

  /// @brief return the number of stored elements
  uint32_t size() const noexcept { return sList.size(); }

  /** @brief unshift a new data pointer under the bottom of the queue
    *
    * This is the irregular queue operation to add a new data
    * pointer under the queue.
    *
    * The regular queue operation to add a new data pointer
    * onto the top is push().
    *
    * If the new element can not be created, a pwx::CException with
    * the name "ItemCreationFailed" is thrown.
    *
    * @param[in] data_ data pointer to store.
    * @return number of elements stored after the operation.
  **/
  uint32_t unshift(data_t* data_)
    {
      if (sList.size())
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
  TDoubleList<data_t> sList; //!< Data is held by a singly linked list.

}; // class TQueue

/** @brief default destructor
  *
  * This destructor will delete all items currently stored. There is no
  * need to clean up manually before deleting the list.
**/
template<typename data_t>
TQueue<data_t>::~TQueue() noexcept
  {
    sList.clear();
  }


} // namespace pwx

#endif // PWX_LIBPWX_PWX_CONTAINER_TQUEUE_H_INCLUDED

