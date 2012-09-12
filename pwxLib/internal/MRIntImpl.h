#pragma once
#ifndef PWX_TMRINTIMPL_H_INCLUDED
#define PWX_TMRINTIMPL_H_INCLUDED 1

/** @internal
  * @file TMRTMAImpl.h
  *
  * @brief Implementations for MRInterface
  *
  * @see CMemRingFactory.h for documentation and history
  *
**/

#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
# error "Do not include TMRIntImpl.h, include CMemRingFactory.h!!"
#endif // Check for main file


namespace pwx {
namespace mrf {
// Note: The namespaces are documented in CMemRingFactory.h

/* --- public methods from CMemRing --- */

/// @brief return true if the container is empty
template<typename T>
bool MRInterface<T>::empty () const
{
  reattach();
  return (dataCount == 0);
}

/// @brief get the current name of the container
template<typename T>
const char * MRInterface<T>::getName () const
{
  return cont ? cont->getName() : "";
}

/// @brief tell the interface that its currently handled item is no longer save
template<typename T>
void MRInterface<T>::reset ()
{
  if (cont)
    {
      current = cont->current;
      reattach();
    }
}

/// @brief get the number of items.
template<typename T>
int32_t MRInterface<T>::size () const
{
  reattach();
  return (dataCount);
}

/* --- public methods from TMemRing --- */

/** @brief Add data to the end of the ring
  *
  * This method adds an item holding @a data to the end of the memory ring. The
  * provided @a aId and @a aName will be changed if the id or name map shall be
  * used and the given id or name is not unique.
  *
  * If it is not possible to create a new item a pwx::mrf::cantCreateItem exception
  * is thrown.
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * @param[in] data A pointer to the data to add
  * @param[in] aId The wanted (optional) id for the data
  * @param[in] aName The wanted (optional) name for the data
  *
  * @return  The number of the new data in the ring.
**/
template<typename T>
int32_t MRInterface<T>::add (T *data, uint32_t aId, const char *aName)
{
  assert (data);
  int32_t result = -1;

  if (cont)
    {
      reattach();
      // to speed up things we force current to point to the last item
      // before calling insert
      if (root && root->getPrev())
        current = root->getPrev();
      try
        {
          ::pwx::Lock contLock(cont);
          // Note: Being a friend class surely helps...
          TItem<T> *oldCurr = cont->current;
          cont->current = current;
          result = cont->insert(data, -1, aId, aName, true);
          cont->current = oldCurr;
          reattach();
        }
      PWX_THROW_FURTHER
    }
  return result;
}


/** @brief Add a value to the end of the ring
  *
  * This method is a convenience wrapper to add data by value.
  * See add(T *data,...) for more information
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * @param[in]  value The value an item should be created from
  * @param[in] aId The wanted (optional) id for the data
  * @param[in] aName The wanted (optional) name for the data
  *
  * @return  The number of the new data in the ring.
**/
template<typename T>
int32_t MRInterface<T>::add (T value, uint32_t aId, const char *aName)
{
  int32_t result = -1;

  if (cont)
    {
      reattach();
      // Same scheme as with *data. But this time we move it into the try block
      try
        {
          T * xData = new T(value); // If this fails it's not our fault
          if (root && root->getPrev())
            current = root->getPrev();
          ::pwx::Lock contLock(cont);
          TItem<T> *oldCurr = cont->current;
          cont->current = current;
          result = cont->insert(xData, -1, aId, aName, false);
          cont->current = oldCurr;
          reattach();
        }
      catch (::std::bad_alloc &e)
        {
          PWX_THROW(cantCreateItem, e.what(), "Add() tried to turn a value into *data")
        }
      PWX_THROW_FURTHER
    }

  return result;
}


/** @brief Add data to a sorted position in the ring
  *
  * This method adds an item holding @a data into the memory ring. The
  * provided @a aId and @a aName will be changed if the id or name map shall be
  * used and the given id or name is not unique. The position is determined by
  * the data, meaning it is sorted into the ring. For this to work your data has to
  * support "bool operator>(T&)".
  *
  * If it is not possible to create a new item a pwx::mrf::cantCreateItem exception
  * is thrown.
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * @param[in] data A pointer to the data to add
  * @param[in] aId The wanted (optional) id for the data
  * @param[in] aName The wanted (optional) name for the data
  * @param[in] asc if set to false, the sorting will be done in descending order
  *
  * @return The number of the new data in the ring.
**/
template<typename T>
int32_t MRInterface<T>::add_sorted (T *data, uint32_t aId, const char *aName, bool asc)
{
  assert (data);
  int32_t newNum = root ? -1 : 0;
  int32_t result = -1;

  reattach();
  if (NULL == current)
    current = root;

  if (asc)
    newNum = setCurrToOrder(base::OT_LargerData, data, 0, NULL);
  else
    newNum = setCurrToOrder(base::OT_SmallerData, data, 0, NULL);

  try
    {
      // Now we need the lock:
      ::pwx::Lock contLock(cont);

      // Now that we have the lock, there might have been changes.
      reattach();

      // Note: Changes would end up being small, so this would be very quick:
      if (asc)
        newNum = setCurrToOrder(base::OT_LargerData, data, 0, NULL);
      else
        newNum = setCurrToOrder(base::OT_SmallerData, data, 0, NULL);

      // Save containers "current" for reversal
      TItem<T> *oldCurr = cont->current;
      cont->current = current;
      result = cont->insert(data, newNum, aId, aName, true);

      // And said reversal:
      cont->current = oldCurr;
      reattach();
    }
  PWX_THROW_FURTHER

  return result;
}


/** @brief Add a value to a sorted position in the ring
  *
  * This method is a convenience wrapper to add data by value.
  * See add_sorted(T *data,...) for more information
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * @param[in] value The value an item should be created from
  * @param[in] aId The wanted (optional) id for the data
  * @param[in] aName The wanted (optional) name for the data
  * @param[in] asc if set to false, the sorting will be done in descending order
  *
  * @return  The number of the new data in the ring.
**/
template<typename T>
int32_t MRInterface<T>::add_sorted (T value, uint32_t aId, const char *aName, bool asc)
{
  int32_t newNum = root ? -1 : 0;
  int32_t result = -1;

  try
    {
      // This is the same scheme as with pointers:
      T * xData = new T(value); // If this fails it's not our fault

      if (NULL == current)
        current = root;

      // even without a single item setCurrToOrder won't fail
      if (asc)
        newNum = setCurrToOrder(base::OT_LargerData, xData, 0, NULL);
      else
        newNum = setCurrToOrder(base::OT_SmallerData, xData, 0, NULL);

      // Now we need the lock:
      ::pwx::Lock contLock(cont);

      // Now that we have the lock, there might have been changes.
      reattach();

      // Note: Changes would end up being small, so this would be very quick:
      if (asc)
        newNum = setCurrToOrder(base::OT_LargerData, xData, 0, NULL);
      else
        newNum = setCurrToOrder(base::OT_SmallerData, xData, 0, NULL);

      TItem<T> *oldCurr = cont->current;
      cont->current = current;
      // Note: no PWX_TRY needed, all is in one try-catch-block
      result = cont->insert(xData, newNum, aId, aName, false);
      cont->current = oldCurr;
      reattach();
    }
  catch (::std::bad_alloc &e)
    {
      PWX_THROW(cantCreateItem, e.what(), "Interface Add_sorted() tried to turn a value into *data")
    }
  PWX_THROW_FURTHER

  return result;
}


/** @brief return true if an item with the id @a aId exists
  * @param[in] aId The Id to search for
  * @return true if an item with the id @a aId exists, false otherwise.
**/
template<typename T>
bool MRInterface<T>::exists (uint32_t aId) const
  {
    reattach();
    // We have to use our own version of setCurrToId to use the real id map if enabled!
    TItem<T> *oldCurr = current;
    bool result = setCurrToId(aId);
    current = oldCurr;
    return result;
  }


/** @brief return true if an item with the name @a aName exists
  * @param[in] aName The name to search for
  * @return true if an item with the name @a aName exists, false otherwise.
**/
template<typename T>
bool MRInterface<T>::exists (const char *aName) const
  {
    reattach();
    // We have to use our own version of setCurrToName to use the real name map if enabled!
    TItem<T> *oldCurr = current;
    bool result = setCurrToName(aName);
    current = oldCurr;
    return result;
  }


/** @brief return true if an item with the number @a aNr exists
  * @param[in] aNr The number to search for
  * @return true if an item with the number @a aNr exists, false otherwise.
**/
template<typename T>
bool MRInterface<T>::exists (int32_t aNr) const
  {
    reattach();
    return TMemRing<T>::exists(aNr);
  }


/** @brief Get a pointer to the data that has the given Nr.
  *
  * Note: This is a wrapper for operator[], which you can use instead.
  *
  * This method throws a pwx::mrf::nrOutOfRange exception if this container
  * is empty.
  *
  * @param[in] aNr The Nr of the wanted data
  * @return a pointer to the data
**/
template<typename T>
T* MRInterface<T>::getData (int32_t aNr) const
{
  PWX_TRY(return(operator[](aNr)))
  PWX_THROW_FURTHER
}


/** @brief Get a pointer to the data that has the given Id.
  *
  * Note : To get data by its number, you can use the [] operator, too.
  *
  * Please note that this method is only reliable, if the id map is used, or if you
  * ensure that the ids are unique.
  *
  * This method throws a pwx::mrf::idNotFound exception if no data with this id can
  * be found.
  * If more than one data has this id, the first one found will be retrieved.
  *
  * @param[in] aId The ID of the wanted data
  * @return a pointer to the retrieved data
**/
template<typename T>
T* MRInterface<T>::getData (uint32_t aId) const
{
  reattach();
  // We have to use our own version of setCurrToId to use the real id map if enabled!
  if (!setCurrToId(aId))
    {
      // No.
      char msg[91] = "";
      ::std::sprintf(msg, "The id %u can not be found in \"%s\" !", aId, cont->name.substr(0,32).c_str());
      PWX_THROW(idNotFound, msg, "Searched by interface getData()")
    }
  return current->getDataPointer();
}


/** @brief Get a pointer of the data that has the given name.
  *
  * Note : To get data by its number, you can use the [] operator, too.
  *
  * Please note that this method is only reliable, if the name map is used, or if you
  * ensure that the names are unique.
  *
  * This method throws a pwx::mrf::nameNotFound exception if no data with this name
  * can be found.
  * If more than one item has this name, the first one found will be retrieved.
  *
  * @param[in] aName The name of the wanted data
  * @return the retrieved data
**/
template<typename T>
T* MRInterface<T>::getData (const char *aName) const
{
  reattach();
  // We have to use our own version of setCurrToName to use the real name map if enabled!
  if (!setCurrToName(aName))
    {
      char msg[91] = "";
      ::std::string xName = aName;
      pwx_snprintf(msg, 90, "The name \"%s%s\" can not be found in \"%s%s\" !",
                   xName.substr(0,23).c_str(),      xName.size() > 23 ? "..." : "",
                   cont->name.substr(0,23).c_str(), cont->name.size() > 23 ? "..." : "");
      // Now get it out:
      PWX_THROW(nameNotFound, msg, "Searched by interface getData()")
    }
  return (current->getDataPointer());
}


/** @brief get the id of a specific item by data
  *
  * This method returns the id of the item holding the given data.
  * If no item holds this data, a pwx::mrf::dataNotFound exception
  *  is thrown.
  *
  * @param[in] data the pointer to the data searched
  * @return The id of the item
 */
template<typename T>
uint32_t MRInterface<T>::getDataId (const T *data) const
{
  reattach();
  PWX_TRY(return TMemRing<T>::getDataId(data))
  PWX_THROW_FURTHER
}


/** @brief get the name of a specific item by data
  *
  * This method returns the name of the item holding the given data.
  * If no item holds this data, a pwx::mrf::dataNotFound exception
  * is thrown.
  *
  * @param[in] data the pointer to the data searched
  * @return The name of the item
 */
template<typename T>
const char * MRInterface<T>::getDataName (const T *data) const
{
  reattach();
  PWX_TRY(return TMemRing<T>::getDataName(data))
  PWX_THROW_FURTHER
}


/** @brief get the number of a specific item by data
  *
  * This method returns the number of the item holding the given data.
  * If no item holds this data, a pwx::mrf::dataNotFound exception is
  * thrown.
  *
  * @param[in] data the pointer to the data searched
  * @return The nr of the item
 */
template<typename T>
int32_t MRInterface<T>::getDataNr (const T *data) const
{
  reattach();
  PWX_TRY(return TMemRing<T>::getDataNr(data))
  PWX_THROW_FURTHER
}


/** @brief get item reference count
  *
  * This method returns the reference count of the given @a data.
  * If no item holds this data, a pwx::mrf::dataNotFound exception is
  * thrown.
  *
  * @param[in] data the pointer to the data searched
  * @return The reference count of the item
**/
template<typename T>
int32_t MRInterface<T>::getDataRefCount (const T *data) const
{
  reattach();
  PWX_TRY(return TMemRing<T>::getDataRefCount(data))
  PWX_THROW_FURTHER
}


/** @brief get a pointer of the surrounding TItem that holds the given data
  *
  * This method returns the surrounding TItem that holds @a aData. If no such item
  * exists, it returns NULL.
  *
  * @param[in] aData the data the wanted TItem holds.
  * @return a pointer to the TItem or NULL if we have no item holding this data
**/
template<typename T>
TItem<T>* MRInterface<T>::getItem(T * aData) const
{
  reattach();
  return TMemRing<T>::getItem(aData);
}



/** @brief get a pointer of the surrounding TItem with the given id
  *
  * This method returns the surrounding TItem that has the id @a aId. If no
  * such item exists, it returns NULL.
  *
  * @param[in] aId the id of the wanted TItem
  * @return a pointer to the TItem or NULL if we have no such item
**/
template<typename T>
TItem<T>* MRInterface<T>::getItem(uint32_t aId) const
{
  reattach();
  return (setCurrToId(aId) ? current : NULL);
}


/** @brief get a pointer of the surrounding TItem with the given name
  *
  * This method returns the surrounding TItem that has the name @a aName. If
  * no such item exists, it returns NULL.
  *
  * @param[in] aName the name of the wanted TItem
  * @return a pointer to the TItem or NULL if we have no such item
**/
template<typename T>
TItem<T>* MRInterface<T>::getItem(const char *aName) const
{
  reattach();
  return (setCurrToName(aName) ? current : NULL);
}

/** @brief get a pointer of the surrounding TItem with the given number
  *
  * This small method is meant to allow containers to copy TItems, which gives
  * them the opportunity to enlist their copies internally as reference counts
  * in return.
  *
  * This method never throws, but returns NULL if the container is empty.
  *
  * @param[in] aNr the number of the wanted TItem, wrapped and normalized if necessary
  * @return a pointer to the TItem or NULL if we have no items
**/
template<typename T>
TItem<T>* MRInterface<T>::getItem(int32_t aNr) const
{
  reattach();
  return TMemRing<T>::getItem(aNr);
}


/** @brief get the id of an item by its number
  *
  * This method returns the Id of the item with the given number, counted from the
  * beginning if aNr is positive, counted from the end if negative, and wrapped if
  * too large.
  *
  * Due to this handling of the number, this method does not throw unless the container
  * is empty, which would result in a pwx::mrf::nrOutofRange exception to be thrown.
  *
  * @param[in] aNr the number of the item. Negative numbers count from the end, too large numbers will be wrapped
  * @return The id of the item
**/
template<typename T>
uint32_t MRInterface<T>::getItemId (int32_t aNr) const
{
  reattach();
  PWX_TRY(return TMemRing<T>::getItemId(aNr))
  PWX_THROW_FURTHER
}


/** @brief get the id of an item by its name
  *
  * This method returns the Id of the item with the given name.
  * If such an item does not exists, a pwx::mrf::nameNotFound
  * exception is thrown.
  *
  * @param[in] aName the name of the wanted item
  * @return the id of the corresponding item
**/
template<typename T>
uint32_t MRInterface<T>::getItemId (const char * aName) const
{
  reattach();
  if (!setCurrToName(aName))
    {
      char msg[91] = "";
      ::std::string xName = aName;
      ::std::sprintf(msg, "The name \"%s%s\" can not be found in \"%s%s\" !",
                     xName.substr(0,23).c_str(),      xName.size() > 23 ? "..." : "",
                     cont->name.substr(0,23).c_str(), cont->name.size() > 23 ? "..." : "");
      // Now get it out:
      PWX_THROW(nameNotFound, msg, "Searched by interface getItemId()")
    }
  return (current->getId());
}


/** @brief get the name of an item by its id
  *
  * This method returns the name of the item with the given id. If
  * there is no item with this id, then a pwx::mrf::idNotFound
  * exception is thrown.
  * @param[in] aId the id of the item.
  * @return The name of the item
**/
template<typename T>
const char * MRInterface<T>::getItemName (uint32_t aId) const
{
  reattach();
  if (!setCurrToId(aId))
    {
      // No.
      char msg[91] = "";
      pwx_snprintf(msg, 91, "The id %u can not be found in \"%s\" !", aId, cont->name.substr(0,32).c_str());
      PWX_THROW(idNotFound, msg, "Searched by interface getItemName()")
    }
  return (current->getName());
}


/** @brief get the name of an item by its number
  *
  * This method returns the name of the item with the given number, counted from the
  * beginning if aNr is positive, counted from the end if negative, and wrapped if
  * too large.
  *
  * Due to this handling of the number, this method does not throw unless the container
  * is empty, which would result in a pwx::mrf::nrOutofRange exception to be thrown.
  *
  * @param[in] aNr the number of the item. Negative numbers count from the end, too large numbers will be wrapped
  * @return The name of the item
 */
template<typename T>
const char * MRInterface<T>::getItemName (int32_t aNr) const
{
  reattach();
  PWX_TRY(return TMemRing<T>::getItemName(aNr))
  PWX_THROW_FURTHER
}


/** @brief get the nr of an item by its id
  *
  * This method returns the nr of the item with the given id. If there is no
  * item with this id, a pwx::mrf::idNotFound exeption is thrown.
  *
  * @param[in] aId the id of the item.
  * @return The nr of the item
**/
template<typename T>
int32_t MRInterface<T>::getItemNr (uint32_t aId) const
{
  reattach();
  if (!setCurrToId(aId))
    {
      // No.
      char msg[91] = "";
      ::std::sprintf(msg, "The id %u can not be found in \"%s\" !", aId, cont->name.substr(0,32).c_str());
      PWX_THROW(idNotFound, msg, "Searched by interface getItemNr()")
    }
  return (current->getNr());
}


/** @brief get the nr of an item by its name
  *
  * This method returns the nr of the item with the given name. If there is no
  * item with this name, a pwx::mrf::nameNotFound exeption is thrown.

  * @param[in] aName the name of the data.
  * @return The nr of the data
**/
template<typename T>
int32_t MRInterface<T>::getItemNr (const char *aName) const
{
  reattach();
  if (!setCurrToName(aName))
    {
      char msg[91] = "";
      ::std::string xName = aName;
      ::std::sprintf(msg, "The name \"%s%s\" can not be found in \"%s%s\" !",
                     xName.substr(0,23).c_str(),      xName.size() > 23 ? "..." : "",
                     cont->name.substr(0,23).c_str(), cont->name.size() > 23 ? "..." : "");
      // Now get it out:
      PWX_THROW(nameNotFound, msg, "Searched by interface getItemNr()")
    }
  return (current->getNr());
}


/** @brief get item reference count
  *
  * This method returns the reference count of the item with the given number,
  * counted from the beginning if aNr is positive, counted from the end if
  * negative, and wrapped if too large.
  *
  * Due to this handling of the number, this method does not throw unless the container
  * is empty, which would result in a pwx::mrf::nrOutofRange exception to be thrown.
  *
  * @param[in] aNr the number of the item. Negative numbers count from the end, too large numbers will be wrapped
  * @return The reference count of the item
**/
template<typename T>
int32_t MRInterface<T>::getItemRefCount (int32_t aNr) const
{
  reattach();
  PWX_TRY(return TMemRing<T>::getItemRefCount(aNr))
  PWX_THROW_FURTHER
}


/** @brief insert new data
  *
  * This method inserts an item at the position indicated by aNr or at the first
  * position if omitted. If the id map is used, the given Id is changed if not unique,
  * the same happens with the name if the name map is used.
  *
  * After insertion the final position is given back.
  *
  * If the @a aNr is positive, the item will replace the item with the given
  * number, pushing it aside. If @a aNr is negative, the item will be added
  * _after_ the item with the resulting number. With this behaviour the result
  * of @a aNr = 0 is an unshift, @a aNr = -1 is a push operation.
  *
  * If is is not possible to create a new item, a pwx::mrf::cantCreateItem exception
  * is thrown.
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * @param[in]  data the data that shall be inserted
  * @param[in] aNr the wanted position by number, counted backwards if negative and wrapped if too large
  * @param[in] aId the wanted id. It will be changed if it has to be unique but isn't.
  * @param[in] aName the wanted name. It will be changed if it has to be unique but isn't.
  * @return the final number representing the position of the inserted item.
**/
template<typename T>
int32_t MRInterface<T>::insert (T *data, int32_t aNr, uint32_t aId, const char *aName)
{
  assert(data);
  int32_t result = -1;

  // The extended insert nr is available in TMemRing only.
  // Therefore we simply ensure current is correct, lock
  // the container with a lock object for exception safety
  // and let TMemRing insert it:
  if (cont)
    {
      reattach();
      if ((aNr != current->getNr()) && !setCurrToNr(aNr))
        current = root;
      try
        {
          ::pwx::Lock contLock(cont);
          TItem<T> *oldCurr = cont->current;
          cont->current = current;
          result = cont->insert(data, aNr, aId, aName, true);
          cont->current = oldCurr;
          reattach();
        }
      PWX_THROW_FURTHER
    }

  return result;
}


/** @brief insert a new value
  *
  * This method is a convenience wrapper for insert(T *data,...) to be used on
  * values. See there for further information.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * @param[in]  value the value that shall be inserted
  * @param[in] aNr the wanted position by number, counted backwards if negative and wrapped if too large
  * @param[in] aId the wanted id. It will be changed if it has to be unique but isn't.
  * @param[in] aName the wanted name. It will be changed if it has to be unique but isn't.
  * @return the final number representing the position of the inserted item.
**/
template<typename T>
int32_t MRInterface<T>::insert (T value, int32_t aNr, uint32_t aId, const char *aName)
{
  int32_t result = -1;
  // Same scheme as with pointers.

  if (cont)
    {
      reattach();
      if ((aNr != current->getNr()) && !setCurrToNr(aNr))
        current = root;
      try
        {
          T * xData = new T(value); // If this fails it's not our fault
          ::pwx::Lock contLock(cont);
          TItem<T> *oldCurr = cont->current;
          cont->current = current;
          result = cont->insert(xData, aNr, aId, aName, false);
          cont->current = oldCurr;
          reattach();
        }
      catch (::std::bad_alloc &e)
        {
          PWX_THROW(cantCreateItem, e.what(), "Insert tried to turn a value into *data")
        }
      PWX_THROW_FURTHER
    }
  return result;
}


/** @brief Returns true if the id map is used, false otherwise.
**/
template <typename T>
bool MRInterface<T>::isIdMapUsed () const
{
  return cont ? cont->isIdMapUsed() : false;
}


/** @brief Returns true if the name map is used, false otherwise.
**/
template <typename T>
bool MRInterface<T>::isNameMapUsed () const
{
  return cont ? cont->isNameMapUsed() : false;
}


/** @brief move an item around
  *
  * This method can be used to move an item around the memory ring.
  * The numbers are counted from the beginning if they are positive,
  * and from the end if negative. If they are too large, they are
  * wrapped.
  *
  * If the container has no items, a pwx::mrf::nrOutOfRange
  * exception is thrown.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * @param[in] oldNr the position of the item that should be moved
  * @param[in] newNr the position of the item it should be moved to
  * @return the final position
**/
template <typename T>
int32_t MRInterface<T>::move (int32_t oldNr, int32_t newNr)
{
  int32_t result = -1;

  if (cont)
    {
      reattach();
      // Moving is not trivial, thus we have to
      // Lock the container first!
      ::pwx::Lock contLock(cont);
      PWX_TRY(result = cont->move(oldNr, newNr))
      PWX_THROW_FURTHER
      reattach();
    }

  return result;
}



/** @brief set the Id of an item
  *
  * Change the Id of the item with the given number. If the container is
  * empty, a pwx::mrf::nrOutOfRange exception is thrown.
  * if the id map shall be used, but the insertion fails, a
  * pwx::mrf::noIdMapAvailable exception is thrown.
  *
  * If the id map is used, the id of an item must be unique. This method
  * takes care of the uniqueness of the id in such a case and will change
  * the given value if necessary. For this reason the finally set id is
  * given back for you to check.
  *
  * @param[in] aNr The number of the item, wrapped and normalized if necessary.
  * @param[in] aId The wanted new id of the item, changed if necessary to ensure a unique id if needed.
  * @return the final id of the item
**/
template <typename T>
uint32_t MRInterface<T>::setItemId (int32_t aNr, uint32_t aId)
{
  uint32_t result = aId;
  reattach();

  if (cont)
    {
      int32_t xNr = TMemRing<T>::normalizeNumber(aNr);
      if (!setCurrToNr(xNr))
        {
          char msg[91] = "";
          pwx_snprintf(msg, 90, "The nr %d (arg %d), is out of range in \"%s\".",
                         xNr, aNr, cont->name.substr(0,26).c_str());
          if (dataCount)
            PWX_THROW(nrOutOfRange, "Numbering broken! [interface setItemId()]", msg)
          else
            PWX_THROW(nrOutOfRange, "Container empty! [interface setItemId()]", msg)
        }
      if (aId != current->getId())
        {
          // The rest is done by the target container:
          try
            {
              ::pwx::Lock contLock(cont);
              TItem<T> *oldCurr = cont->current;
              cont->current = current;
              result = cont->setItemId(xNr, aId);
              cont->current = oldCurr;
            }
          PWX_THROW_FURTHER
        }
    } // end of having cont

  return result;
}


/** @brief set the name of an item
  *
  * Change the name of the item with the given number. If the container is
  * empty, a pwx::mrf::nrOutOfRange exception is thrown.
  * If the name map shall be used but the insertion fails, a
  * pwx::mrf::noNameMapAvailable exception is thrown;
  *
  * If the name map is used, the name of an item must be unique. This method
  * takes care of the uniqueness of the name in such a case and will change
  * the given value if necessary. For this reason the finally set name is
  * given back for you to check.
  *
  * @param[in] aNr The number of the item, wrapped and normalized if necessary.
  * @param[in] aName The wanted new name of the item, changed if necessary to ensure a unique name if needed.
  * @return the final name of the item
**/
template <typename T>
const char * MRInterface<T>::setItemName (int32_t aNr, const char *aName)
{
  reattach();
  if (cont)
    {
      int32_t xNr = TMemRing<T>::normalizeNumber(aNr);
      if (!setCurrToNr(xNr))
        {
          char msg[91] = "";
          pwx_snprintf(msg, 90, "The nr %d (arg %d), is out of range in \"%s\".",
                         xNr, aNr, cont->name.substr(0,26).c_str());
          if (dataCount)
            PWX_THROW(nrOutOfRange, "Numbering broken! [interface setItemName()]", msg)
          else
            PWX_THROW(nrOutOfRange, "Container empty! [interface setItemName()]", msg)
        }
      if (!aName || STRNE(aName, current->getName()))
        {
          // Here we let the container do it as well to keep the map in sync:
          try
            {
              ::pwx::Lock contLock(cont);
              TItem<T> *oldCurr = cont->current;
              cont->current = current;
              const char *result = cont->setItemName(xNr, aName);
              cont->current = oldCurr;
              return result;
            }
          PWX_THROW_FURTHER
        }
    }
  return current ? current->getName() : "";
}


/** @brief sort items by data
  *
  * This method sorts all items by their content. If @a asc is omitted or
  * set to true, they will be sorted in ascending order. For a descending
  * order @a asc has to be set to false.
  *
  * The data you store has to support "bool operator>(T&)" for this to work!
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * Note: Sorting can be interrupted with interruptSorting();
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
**/
template<typename T>
void MRInterface<T>::sort (bool asc)
{
  doSort = true;
  PWX_TRY(for ( ; doSort && sort_once(asc) ; ) )
  PWX_THROW_FURTHER
}


/** @brief sort items by Id
  *
  * This method sorts all items by their Id. If @a asc is omitted or
  * set to true, they will be sorted in ascending order. For a descending
  * order @a asc has to be set to false.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * Note: Sorting can be interrupted with interruptSorting();
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
**/
template <typename T>
void MRInterface<T>::sortById (bool asc)
{
  doSort = true;
  PWX_TRY(for ( ; doSort && sortById_once(asc) ; ) )
  PWX_THROW_FURTHER
}


/** @brief sort items by Name
  *
  * This method sorts all items by their Name. If @a asc is omitted or
  * set to true, they will be sorted in ascending order. For a descending
  * order @a asc has to be set to false.
  *
  * <B>Important</B>: This method sorts case insensitive!
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * Note: Sorting can be interrupted with interruptSorting();
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
**/
template <typename T>
void MRInterface<T>::sortByName (bool asc)
{
  doSort = true;
  PWX_TRY(for ( ; doSort && sortByName_once(asc) ; ) )
  PWX_THROW_FURTHER
}

/** @brief sort items once by data
  *
  * This method sorts all items once by their content. If @a asc is omitted or
  * set to true, they will be sorted in ascending order. For a descending
  * order @a asc has to be set to false. The method returns the number of items
  * that have have been moved. This way you can sort a large number of items
  * once, check for user interruption, and sort again until the method returns 0.
  *
  * The data you store has to support "bool operator>(T&)" for this to work!
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
  * @return The number of items that have been moved
**/
template <typename T>
int32_t MRInterface<T>::sort_once (bool asc)
{
  doSort = true;
  PWX_TRY(return sort_p(asc ? base::OT_LargerData : base::OT_SmallerData))
  PWX_THROW_FURTHER
}


/** @brief sort items once by Id
  *
  * This method sorts all items by their Id. If @a asc is omitted or
  * set to true, they will be sorted in ascending order. For a descending
  * order @a asc has to be set to false. The method returns the number of items
  * that have have been moved. This way you can sort a large number of items
  * once, check for user interruption, and sort again until the method returns 0.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
  * @return The number of items that have been moved
**/
template <typename T>
int32_t MRInterface<T>::sortById_once (bool asc)
{
  doSort = true;
  PWX_TRY(return sort_p(asc ? base::OT_LargerId : base::OT_SmallerId))
  PWX_THROW_FURTHER
}


/** @brief sort items once by Name
  *
  * This method sorts all items by their Name. If @a asc is omitted or
  * set to true, they will be sorted in ascending order. For a descending
  * order @a asc has to be set to false. The method returns the number of items
  * that have have been moved. This way you can sort a large number of items
  * once, check for user interruption, and sort again until the method returns 0.
  *
  * <B>Important</B>: This method sorts case insensitive!
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
  * @return The number of items that have been moved
**/
template <typename T>
int32_t MRInterface<T>::sortByName_once (bool asc)
{
  doSort = true;
  PWX_TRY(return sort_p(asc ? base::OT_LargerName : base::OT_SmallerName))
  PWX_THROW_FURTHER
}


// --- templated methods ---

/** @brief This method returns true, if the given data is stored in this container.
  *
  * Note: It has to walk through the memory ring structure, so you might
  *       not want to use it unless you really really need to.
  *
  * @param[in]  data the data you search for
**/
template <typename T> template<typename Td>
bool MRInterface<T>::isIn (Td *data) const
{
  bool result = false;
  reattach();

  // Don't do anything at all if it is the wrong type:
  if (isSameType(T, Td))
    result = setCurrToItem(data);

  return (result);
}


/** @brief This method returns true, if the given value is stored in this container.
  *
  * Note: It has to walk through the memory ring structure, and compare with
  *       every single item content, so you might not want to use it unless you
  *       really really need to.
  *
  * @param[in]  value the value you search for
**/
template <typename T> template<typename Td>
bool MRInterface<T>::isValueIn (Td value) const
{
  bool result = false;
  reattach();

  // Don't do anything at all if it is the wrong type:
  if (isSameType(T, Td))
    result = setCurrToValue(value);

  return (result);
}


// --- operators ---

/** @brief get a pointer to data by index
  *
  * Returns a pointer to the data with the given number, counted
  * from the beginning if @a aNr is positive, counted from the end if
  * negative, and wrapped if too large.
  *
  * If the container is empty, a pwx::mrf::nrOutOfRange exception is thrown.
  *
  * @param[in] aNr the number of the wanted item
  * @return a pointer to the held data
**/
template<typename T>
T* MRInterface<T>::operator[] (int32_t aNr) const
{
  reattach();
  int32_t xNr = TMemRing<T>::normalizeNumber(aNr);
  if (!setCurrToNr(xNr))
    {
      char msg[91] = "";
      pwx_snprintf(msg, 90, "The nr %d (arg %d), is out of range in \"%s\".",
                     xNr, aNr, cont->name.substr(0,26).c_str());
      if (dataCount)
        PWX_THROW(nrOutOfRange, "Numbering broken! (interface operator[])", msg)
      else
        PWX_THROW(nrOutOfRange, "Container empty! (interface operator[])", msg)
    }
  return current->getDataPointer();
}


/* --- private helper methods --- */

/// @brief sync root and datacount with interfaced container
template<typename T>
void MRInterface<T>::reattach() const
  {
    if (cont)
      {
        root      = cont->root;
        if (dataCount != cont->dataCount)
          {
            if (dataCount > cont->dataCount)
              // This means an item has been deleted, it might have been our current
              current   = cont->current;
            dataCount = cont->dataCount;
          }
      }
    else
      {
        root      = NULL;
        current   = NULL;
        dataCount = 0;
      }
  }


/** @brief internal method to set current to the item with the given ID
  *
  * If the Id could not be found, the method returns false, true on success
**/
template<typename T>
bool MRInterface<T>::setCurrToId  (uint32_t aId) const
{
  bool result = false;
  reattach();
  if (cont && (!current || (current->getId() != aId)))
    {
      // What a pity, we need to act:
      current = root;
      if (root && (current->getId() != aId))
        {
          // we need to search, try id map first if available
          if (isIdMapUsed())
            {
              // here we use our local pointers to use the containers Map
              tIdIter_c iter = lIdMap->find(aId);
              if (iter != lIdMap->end())
                current = iter->second;
            } // End of search by id map
          else
            {
              // without the map, we have to look "by hand"
              TItem<T> *xPrev = current;
              TItem<T> *xNext = current ? current->getNext() : NULL;
              while (current && (current->getId() != aId) && (xNext != root))
                {
                  if (!xNext)
                    {
                      // It might be that current is moved away by a different thread.
                      if (xPrev && (current != xPrev->getNext()))
                        xNext = xPrev->getNext();
                      assert ((xNext || (dataCount < 2)) && "ERROR: Interface setCurrId() hit a dead end when it shouldn't!");
                    }
                  else
                    xPrev = current;
                  current = xNext;
                  xNext = current ? current->getNext() : NULL;
                } // End of no target found and ring not finished yet
            } // End of looking manually
        } // End of looking at all with available root
    }

  // Were we successful?
  if (current && (current->getId() == aId))
    result = true;

  return (result);
}


/** @brief internal method to set current to the item with the given ID
  *
  * If the name could not be found the result is false, true otherwise
**/
template<typename T>
bool MRInterface<T>::setCurrToName  (const char * aName) const
{
  bool result = false;
  reattach();
  if (cont && (!current || STRNE(current->getName(), aName)))
    {
      // We don't have it already, so do something
      current = root;
      if (root && STRNE(current->getName(), aName))
        {
          // we need to search, try name map first if available
          if (isNameMapUsed())
            {
              // Again we use our local pointers to use the containers Map
              tNameIter_c iter = lNameMap->find(aName);
              if (iter != lNameMap->end())
                current = iter->second;
            } // End of search by name map
          else
            {
              // without the map, we have to look "by hand"
              TItem<T> *xPrev = current;
              TItem<T> *xNext = current ? current->getNext() : NULL;
              while (current && STRNE(current->getName(), aName) && (xNext != root))
                {
                  if (!xNext)
                    {
                      // It might be that current is moved away by a different thread.
                      if (xPrev && (current != xPrev->getNext()))
                        xNext = xPrev->getNext();
                      assert ((xNext || (dataCount < 2)) && "ERROR: Interface setCurrName() hit a dead end when it shouldn't!");
                    }
                  else
                    xPrev = current;
                  current = xNext;
                  xNext = current ? current->getNext() : NULL;
                } // End of no target found and ring not finished yet
            } // End of looking manually
        } // End of looking at all aka root available
    }

  // Were we successful?
  if (current && STREQ(current->getName(), aName))
    result = true;

  return (result);
}



/** @brief internal method that does the sorting.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to define PWX_THREADS
  * before including the first pwxLib header. Otherwise the locking mechanisms do nothing.
  *
  * Note: Sorting can be interrupted with interruptSorting();
  *
  * @param[in] type -1/1 by data desc/asc, -2/2 by id, -3/3 by name
  * @return number of sorting operations done
**/
template<typename T>
int32_t MRInterface<T>::sort_p (base::eOrderType type)
{
  unsorted = 0;

  if (doSort && cont && (dataCount > 2))
    {
      assert (root);

      TItem<T> * xNext = NULL;
      TItem<T> * source= NULL;
      int32_t    oldNr = 0;
      int32_t    newNr = 0;
      cont->lock();
      current          = root;
      xNext            = root->getNext();
      cont->unlock();

      while (doSort && current && (xNext != root))
        {
          // Sorting is a nasty business. Everything has to be locked properly or bad
          // juju is the result. Therefore we skip if we can not lock!
          if (current->tryLock())
            {
              oldNr = current->getNr();
              source= current;
              // setCurrToOrder() needs to lock items, thus current has to be unlocked first
              source->unlock();
              newNr = setCurrToOrder(type, source->getDataPointer(), source->getId(), source->getName());
              if (doSort && (newNr != oldNr) && (source != current) && current->tryLock())
                {
                  // The trick is, that if current has the right position, setCurrToOrder will
                  // do nothing and give the items number back.
                  unsorted++;
                  // Now, as dest is locked, we can carry on. But dest has to be unlocked first
                  current->unlock();
                  ::pwx::Lock contLock(cont);
                  // Now this thread has the lock, all other threads might have changed the container
                  // like renumbering (harmless) or changing the root item (serious). We therefore have
                  // to reattach first.
                  reattach();

                  // Between the lock waits either or both could have been moved. We have to check again:
                  oldNr = source->getNr();
                  newNr = setCurrToOrder(type, source->getDataPointer(), source->getId(), source->getName());
                  // If neither got moved around, setCurrToOrder will return at once
                  if (doSort && (newNr != oldNr) && (source != current))
                    {
                      // As we have a lock on the container itself we can do the move directly.
                      TItem<T> *newRoot = NULL;
                      try
                        {
                          if (newNr < 0)
                            newRoot = source->move(current, NULL, NULL);
                          else
                            newRoot = source->move(NULL, current, NULL);
                        }
                      PWX_THROW_FURTHER

                      if (newRoot && (0 == newRoot->getNr()) && (root != newRoot))
                        {
                          // A new root must be noted and told the container
                          root = newRoot;
                          cont->root = root;
                        }
                      else
                        // So this is no longer unsorted, the position is corrected
                        --unsorted;
                    } // End of action still required
                  else
                    // So this is no longer counted as being unsorted,
                    // another thread is (probably) going to move it right now
                    --unsorted;
                } // End of needing a number change and dest is lackable
            } // End of current (source) is lockable
          reattach();
          assert (xNext && "ERROR: xNext is NULL here! Why?");

          // Move on...
          cont->lock(); // We need to be damn sure neither xNext nor current are manipulated now!
          if (xNext != current->getPrev())
          // They can be equal if two items switch their place
            current = xNext;
          // ... and get new next
          xNext = current ? current->getNext() : NULL;
          cont->unlock();

          assert (current && "ERROR: How on earth did current become NULL???");
        } // End of loop
    } // End of sorting
  else if (doSort && cont && (2 == dataCount))
    {
      // Special case with two items:
      ::pwx::Lock contLock(cont);
      reattach(); // Just in case another thread already corrected the order
      current = root->getNext();
      if ( doSort && (
           (( 3 == type) && STRCGT(root->getName(),   current->getName()) )
        || ((-3 == type) && STRCGT(current->getName(),root->getName()   ) )
        || (( 2 == type) && (root->getId()      >     current->getId()  ) )
        || ((-2 == type) && (current->getId()   >     root->getId()     ) )
        || (( 1 == type) && (root->getData()    >     current->getData()) )
        || ((-1 == type) && (current->getData() >     root->getData()   ) ) ) )
        {
          ++unsorted;
          // We do this directly here:
          TItem<T> *newRoot = root->swap(current);
          if (newRoot && (0 == newRoot->getNr()) && (root != newRoot))
            {
              // A new root must be noted and told the container
              root = newRoot;
              cont->root = root;
            }
        }
      // Note: It does not matter if type is invalid here, the items simply won't get swapped then.
    }
  // No else, as sorting an empty container simply does nothing

  return unsorted;
}


/** @brief small internal methods to swap two items and take care of root
**/
template<typename T>
void MRInterface<T>::swapItems (TItem<T> * first, TItem<T> * second)
{
  assert(first && second && (first != second));
  reattach();

  if (cont && first)
    {
      // The contLock is needed, so that only one swap is taking place at one time:
      ::pwx::Lock contLock(cont);
      // "swap" locks all neighbors, but neither first nor second.
      // This is the policy that no object locks itself. As first is bound
      // to become second and vice versa, locking second would break this rule
      first->lock();
      if (second && (second != first))
        second->lock();
      TItem<T> *newRoot = first->swap(second);
      if (newRoot && (0 == newRoot->getNr()))
        root = newRoot;
      assert (root && (0 == root->getNr()) && "\nERROR: interface swapItems invalidated root!\n");
      if (second && (second != first))
        second->unlock();
      first->unlock();
    }
}


} // End of namespace mrf

} // End of namespace pwx

#endif // PWX_TMRINTIMPL_H_INCLUDED

