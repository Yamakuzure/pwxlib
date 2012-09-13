#pragma once
#ifndef PWX_LIBPWX_PWX_CONTAINER_TSTACK_H_INCLUDED
#define PWX_LIBPWX_PWX_CONTAINER_TSTACK_H_INCLUDED 1

#include <pwx/container/TSingleList.h>

namespace pwx {

/** @class TStack
  *
  * @brief
  *
  * @todo : Describe properly
**/
template<typename data_t>
class TStack
{
public:
  /* ===============================================
   * === Public types                            ===
   * ===============================================
  */

  typedef TStack<data_t>         list_t;
  typedef TSingleElement<data_t> elem_t;

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
  TStack(void (*destroy_)(data_t* data_)) noexcept
  : sList(destroy_)
    { /* nothing to be done here */ }

  /** @brief empty constructor
    *
    * The empty constructor sets the data destroy method to the null pointer.
  **/
  TStack() noexcept
  : sList(nullptr)
    { /* nothing to be done here */ }

  virtual ~TStack() noexcept;

  TStack(const list_t &rhs) PWX_DELETE; // No copy ctor

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

  /** @brief push a new data pointer onto the stack
    *
    * This is the regular stack operation to add a new element.
    * Being a stack this new element is put on top of it.
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

  /** @brief pop the first element from the stack
    *
    * This is the regular stack operation to get the newest element.
    * Being a stack this element comes from the top.
    *
    * To get an element from the bottom, use shift().
    *
    * The element is removed from the stack so you have to take
    * care of its deletion once you are finished with it.
    *
    * If there is no element in the stack a pwx::CException with the
    * name "OutOfRange" is thrown.
    *
    * @return the top element on the stack.
  **/
  elem_t* pop()
    {
      PWX_TRY(return sList.remNext(nullptr))
      PWX_THROW_FURTHER
    }

  /** @brief shift the oldest element from the bottom of the stack
    *
    * This is the irregular stack operation to get the oldest
    * element.
    *
    * The regular stack operation to get the top element is
    * pop ().
    *
    * The element is removed from the stack so you have to take
    * care of its deletion once you are finished with it.
    *
    * If there is no element in the stack a pwx::CException with the
    * name "OutOfRange" is thrown.
    *
    * @return the bottom element on the stack.
  **/
  elem_t* shift()
    {
      if (sList.size())
        {
          PWX_TRY(return sList.remNextElem(sList[-2]))
          PWX_THROW_FURTHER
        }
      else
        {
          PWX_TRY(return pop())
          PWX_THROW_FURTHER
        }
    }

  /// @brief return the number of stored elements
  uint32_t size() const noexcept { return sList.size(); }

  /** @brief unshift a new data pointer under the bottom of the stack
    *
    * This is the irregular stack operation to add a new data
    * pointer under the stack.
    *
    * The regular stack operation to add a new data pointer
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
      if (sList.size() > 1)
        {
          PWX_TRY(return sList.insNextElem(sList[-1], data_))
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
  TSingleList<data_t> sList; //!< Data is held by a singly linked list.

}; // class TStack

/** @brief default destructor
  *
  * This destructor will delete all items currently stored. There is no
  * need to clean up manually before deleting the list.
**/
template<typename data_t>
TStack<data_t>::~TStack() noexcept
  {
    sList.clear();
  }

} // namespace pwx

#endif // PWX_LIBPWX_PWX_CONTAINER_TSTACK_H_INCLUDED

