#pragma once
#ifndef PWX_TMRTIMPL_H_INCLUDED
#define PWX_TMRTIMPL_H_INCLUDED 1

/** @internal
  * @file TMRTImpl.h
  *
  * @brief Implementations for TMemRing
  *
  * @see CMemRingFactory.h for documentation and history
  *
**/

#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
# error "Do not include TMRTImpl.h, include CMemRingFactory.h!!"
#endif // Check for main file

namespace pwx {
namespace mrf {
// Note: The namespaces are documented in CMemRingFactory.h

// --- Public methods ---

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
  * @param[in] data A pointer to the data to add
  * @param[in] aId The wanted (optional) id for the data
  * @param[in] aName The wanted (optional) name for the data
  *
  * @return  The number of the new data in the ring.
**/
template<typename T>
int32_t TMemRing<T>::add (T *data, uint32_t aId, const char *aName)
{
  assert (data);

  PWX_TRY(insert(data, -1, aId, aName, true))
  PWX_THROW_FURTHER
  return (dataCount - 1);
}


/** @brief Add a value to the end of the ring
  *
  * This method is a convenience wrapper to add data by value.
  * See add(T *data,...) for more information
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in]  value The value an item should be created from
  * @param[in] aId The wanted (optional) id for the data
  * @param[in] aName The wanted (optional) name for the data
  *
  * @return  The number of the new data in the ring.
**/
template<typename T>
int32_t TMemRing<T>::add (T value, uint32_t aId, const char *aName)
{
  try
    {
      T * xData = new T(value); // If this fails it's not our fault
      insert(xData, -1, aId, aName, false);
    }
  catch (::std::bad_alloc &e)
    {
      PWX_THROW(cantCreateItem, e.what(), "Add() tried to turn a value into *data")
    }
  PWX_THROW_FURTHER

  return (dataCount - 1);
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
  * @param[in] data A pointer to the data to add
  * @param[in] aId The wanted (optional) id for the data
  * @param[in] aName The wanted (optional) name for the data
  * @param[in] asc if set to false, the sorting will be done in descending order
  *
  * @return The number of the new data in the ring.
**/
template<typename T>
int32_t TMemRing<T>::add_sorted (T *data, uint32_t aId, const char *aName, bool asc)
{
  assert (data);
  int32_t newNum = root ? -1 : 0;

  if (NULL == current)
    current = root;

  if (asc)
    newNum = setCurrToOrder(base::OT_LargerData, data, 0, NULL);
  else
    newNum = setCurrToOrder(base::OT_SmallerData, data, 0, NULL);

  PWX_TRY(insert(data, newNum, aId, aName, true))
  PWX_THROW_FURTHER
  return (newNum >= 0 ? newNum : dataCount - 1);
}


/** @brief Add a value to to a sorted position in the ring
  *
  * This method is a convenience wrapper to add data by value.
  * See add_sorted(T *data,...) for more information
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] value The value an item should be created from
  * @param[in] aId The wanted (optional) id for the data
  * @param[in] aName The wanted (optional) name for the data
  * @param[in] asc if set to false, the sorting will be done in descending order
  *
  * @return  The number of the new data in the ring.
**/
template<typename T>
int32_t TMemRing<T>::add_sorted (T value, uint32_t aId, const char *aName, bool asc)
{
  int32_t newNum = root ? -1 : 0;

  try
    {
      T * xData = new T(value); // If this fails it's not our fault

      if (NULL == current)
        current = root;

      if (asc)
        newNum = setCurrToOrder(base::OT_LargerData, xData, 0, NULL);
      else
        newNum = setCurrToOrder(base::OT_SmallerData, xData, 0, NULL);
      newNum = insert(xData, newNum, aId, aName, false);
    }
  catch (::std::bad_alloc &e)
    {
      PWX_THROW(cantCreateItem, e.what(), "Add_sorted() tried to turn a value into *data")
    }
  PWX_THROW_FURTHER

  return (newNum > 0 ? newNum : dataCount - 1);
}


/** @brief Clear the container of all items
**/
template<typename T>
void TMemRing<T>::clear ()
{
  // Are we correctly set?
  assert (dataCount == (root ? root->getPrev() ? root->getPrev()->getNr() + 1 : 1 : 0));

  if (root)
    {
      // Save state and "shut down" maps
      bool oldIdState   = idMapUsed;
      bool oldNameState = nameMapUsed;
      if (idMapUsed)   useIdMap(false);
      if (nameMapUsed) useNameMap(false);

      current = root->getPrev() ? root->getPrev() : root;

      while (current)
        deleteCurr(true);

      // revert maps to old state:
      if (oldIdState)   useIdMap(true);
      if (oldNameState) useNameMap(true);
    }

  // maxId has to be reset now:
  maxId = 0;

  // This should have done it::
  assert(!current && !root && !dataCount); // Both NULL and no data left
}


/** @brief Delete an item by ID
  *
  * This method deletes one item identified by the given @a aId.
  * There is no answer whether any item was deleted or not. If
  * there is no item with the given @a aId, this method simply
  * does nothing.
  *
  * Note: If no uniqueness of ids is enforced by using the id map,
  * several items might get deleted if they have the same id.
  *
  * @param[in] aId the ID of the item that is to be deleted.
**/
template<typename T>
void TMemRing<T>::delItem (uint32_t aId)
  {
    while (setCurrToId(aId))
      deleteCurr(true);
  }


/** @brief Delete an item by name
  *
  * This method deletes one item identified by the given @a aName.
  * There is no answer whether any item was deleted or not. If
  * there is no item with the given @a aName, this method simply
  * does nothing.
  *
  * Note: If no uniqueness of names is enforced by using the name map,
  * several items might get deleted if they have the same name.
  *
  * @param[in] aName the name of the item that is to be deleted.
**/
template<typename T>
void TMemRing<T>::delItem (const char *aName)
  {
    while (setCurrToName(aName))
      deleteCurr(true);
  }


/** @brief Delete an item by nr
  *
  * This method deletes one item identified by the given @a aNr.
  * There is no answer whether any item was deleted or not. If
  * the container is empty, this method simply does nothing.
  *
  * @param[in] aNr the nr of the item that is to be deleted.
**/
template<typename T>
void TMemRing<T>::delItem (int32_t aNr)
  {
    if (setCurrToNr(aNr))
      deleteCurr(true);
  }


/** @brief disable reference tracking of items
  *
  * Once called this container will no longer track references to
  * items added. This greatly increases the speed of inserting and
  * adding items, but there no longer is a check whether data
  * can be deleted or not when an item is removed.
  *
  * <B>Warning</B>: Data will be deleted as all items are considered to
  * have no other references. Please <I>make sure</I> you add data only
  * once, or bad things <B>will</B> happen!
**/
template<typename T>
void TMemRing<T>::disableTracking ()
  {
    trackRef = false;
  }


/** @brief delete data by content
  *
  * Deletes an item from this container and returns the number of items
  * actually deleted. Please be aware that the data itself will _not_
  * be deleted as long as it has existing copies the item(s) know of.
  *
  * This method never throws. If the given @a data can not be found in
  * this container, it will simply return 0.
  *
  * @param[in] data the data to be searched for
  * @return the number of items deleted
 */
template<typename T>
int32_t TMemRing<T>::erase (T *data)
{
  int32_t nrDeleted = 0;
  assert (data);

  // No data? no action
  if (data)
    {
      while (setCurrToItem(data))
        {
          deleteCurr(true);
          ++nrDeleted;
        }
    }

  return (nrDeleted);
}


/** @brief return true if an item with the id @a aId exists
  * @param[in] aId The Id to search for
  * @return true if an item with the id @a aId exists, false otherwise.
**/
template<typename T>
bool TMemRing<T>::exists (uint32_t aId) const
  {
    return getItem(aId) ? true : false;
  }


/** @brief return true if an item with the name @a aName exists
  * @param[in] aName The name to search for
  * @return true if an item with the name @a aName exists, false otherwise.
**/
template<typename T>
bool TMemRing<T>::exists (const char *aName) const
  {
    return getItem(aName) ? true : false;
  }


/** @brief return true if an item with the number @a aNr exists
  * @param[in] aNr The number to search for
  * @return true if an item with the number @a aNr exists, false otherwise.
**/
template<typename T>
bool TMemRing<T>::exists (int32_t aNr) const
  {
    return ((aNr >= 0) && (aNr < dataCount)) ? true : false;
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
T* TMemRing<T>::getData (int32_t aNr) const
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
T* TMemRing<T>::getData (uint32_t aId) const
{
  if (!setCurrToId(aId))
    {
      // No.
      char msg[91] = "";
      pwx_snprintf(msg, 90, "The id %u can not be found in \"%s\" !", aId, name.substr(0,32).c_str());
      PWX_THROW(idNotFound, msg, "Searched by getData()")
    }
  return (current->getDataPointer());
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
T* TMemRing<T>::getData (const char *aName) const
{
  if (!setCurrToName(aName))
    {
      char msg[91] = "";
      ::std::string xName = aName;
      pwx_snprintf(msg, 90, "The name \"%s%s\" can not be found in \"%s%s\" !",
                     xName.substr(0,23).c_str(), xName.size() > 23 ? "..." : "",
                     name.substr(0,23).c_str(),  name.size() > 23 ? "..." : "");
      // Now get it out:
      PWX_THROW(nameNotFound, msg, "Searched by getData()")
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
uint32_t TMemRing<T>::getDataId (const T *data) const
{
  if (!setCurrToItem(data))
    {
      // No.
      char msg[66] = "";
      pwx_snprintf(msg, 65, "The data can not be found in \"%s\" !", name.substr(0,32).c_str());
      PWX_THROW(dataNotFound, msg, "Searched by getItemId()")
    }
  return (current->getId());
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
const char * TMemRing<T>::getDataName (const T *data) const
{
  if (!setCurrToItem(data))
    {
      // No.
      char msg[66] = "";
      pwx_snprintf(msg, 65, "The data can not be found in \"%s\" !", name.substr(0,32).c_str());
      PWX_THROW(dataNotFound, msg, "Searched by getItemName()")
    }
  return (current->getName());
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
int32_t TMemRing<T>::getDataNr (const T *data) const
{
  if (!setCurrToItem(data))
    {
      // No.
      char msg[66] = "";
      pwx_snprintf(msg, 65, "The data can not be found in \"%s\" !", name.substr(0,32).c_str());
      PWX_THROW(dataNotFound, msg, "Searched by getItemNr()")
    }
  return (current->getNr());
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
int32_t TMemRing<T>::getDataRefCount (const T *data) const
{
  if (!setCurrToItem(data))
    {
      char msg[66] = "";
      pwx_snprintf(msg, 65, "The data can not be found in \"%s\" !", name.substr(0,32).c_str());
      PWX_THROW(dataNotFound, msg, "Searched by getDataRefCount()")
    }
  return (current->getRefCount());
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
TItem<T>* TMemRing<T>::getItem(T * aData) const
{
  return (setCurrToItem(aData) ? current : NULL);
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
TItem<T>* TMemRing<T>::getItem(uint32_t aId) const
{
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
TItem<T>* TMemRing<T>::getItem(const char *aName) const
{
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
TItem<T>* TMemRing<T>::getItem(int32_t aNr) const
{
  return (root && setCurrToNr(normalizeNumber(aNr)) ? current : NULL);
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
uint32_t TMemRing<T>::getItemId (int32_t aNr) const
{
  int32_t xNr = normalizeNumber(aNr);
  if (!setCurrToNr(xNr))
    {
      char msg[91] = "";
      pwx_snprintf(msg, 90, "The nr %d (arg %d), is out of range in \"%s\".",
                     xNr, aNr, name.substr(0,26).c_str());
      if (dataCount)
        PWX_THROW(nrOutOfRange, "Numbering broken! [getItemId()]", msg)
      else
        PWX_THROW(nrOutOfRange, "Container empty! [getItemId()]", msg)
    }
  return (current->getId());
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
uint32_t TMemRing<T>::getItemId (const char * aName) const
{
  if (!setCurrToName(aName))
    {
      char msg[91] = "";
      ::std::string xName = aName;
      pwx_snprintf(msg, 90, "The name \"%s%s\" can not be found in \"%s%s\" !",
                     xName.substr(0,23).c_str(), xName.size() > 23 ? "..." : "",
                     name.substr(0,23).c_str(),  name.size() > 23 ? "..." : "");
      // Now get it out:
      PWX_THROW(nameNotFound, msg, "Searched by getItemId()")
    }
  return (current->getId());
}


/** @brief get the name of an item by its id
  *
  * This method returns the name of the item with the given id. If
  * there is no item with this id, then a pwx::mrf::idNotFound
  * exeption is thrown.
  * @param[in] aId the id of the item.
  * @return The name of the item
**/
template<typename T>
const char * TMemRing<T>::getItemName (uint32_t aId) const
{
  if (!setCurrToId(aId))
    {
      // No.
      char msg[91] = "";
      pwx_snprintf(msg, 90, "The id %u can not be found in \"%s\" !", aId, name.substr(0,32).c_str());
      PWX_THROW(idNotFound, msg, "Searched by getItemName()")
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
const char * TMemRing<T>::getItemName (int32_t aNr) const
{
  int32_t xNr = normalizeNumber(aNr);
  if (!setCurrToNr(xNr))
    {
      char msg[91] = "";
      pwx_snprintf(msg, 90, "The nr %d (arg %d), is out of range in \"%s\".",
                     xNr, aNr, name.substr(0,26).c_str());
      if (dataCount)
        PWX_THROW(nrOutOfRange, "Numbering broken! [getItemName()]", msg)
      else
        PWX_THROW(nrOutOfRange, "Container empty! [getItemName()]", msg)
    }
  return (current->getName());
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
int32_t TMemRing<T>::getItemNr (uint32_t aId) const
{
  if (!setCurrToId(aId))
    {
      // No.
      char msg[91] = "";
      pwx_snprintf(msg, 90, "The id %u can not be found in \"%s\" !", aId, name.substr(0,32).c_str());
      PWX_THROW(idNotFound, msg, "Searched by getItemNr()")
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
int32_t TMemRing<T>::getItemNr (const char *aName) const
{
  if (!setCurrToName(aName))
    {
      char msg[91] = "";
      ::std::string xName = aName;
      pwx_snprintf(msg, 90, "The name \"%s%s\" can not be found in \"%s%s\" !",
                     xName.substr(0,23).c_str(), xName.size() > 23 ? "..." : "",
                     name.substr(0,23).c_str(),  name.size() > 23 ? "..." : "");
      // Now get it out:
      PWX_THROW(nameNotFound, msg, "Searched by getItemNr()")
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
int32_t TMemRing<T>::getItemRefCount (int32_t aNr) const
{
  int32_t xNr = normalizeNumber(aNr);
  if (!setCurrToNr(xNr))
    {
      char msg[91] = "";
      pwx_snprintf(msg, 90, "The nr %d (arg %d), is out of range in \"%s\".",
                     xNr, aNr, name.substr(0,26).c_str());
      if (dataCount)
        PWX_THROW(nrOutOfRange, "Numbering broken! [getRefCount()]", msg)
      else
        PWX_THROW(nrOutOfRange, "Container empty! [getRefCount()]", msg)
    }
  return (current->getRefCount());
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
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in]  data the data that shall be inserted
  * @param[in] aNr the wanted position by number, counted backwards if negative and wrapped if too large
  * @param[in] aId the wanted id. It will be changed if it has to be unique but isn't.
  * @param[in] aName the wanted name. It will be changed if it has to be unique but isn't.
  * @return the final number representing the position of the inserted item.
**/
template<typename T>
int32_t TMemRing<T>::insert (T *data, int32_t aNr, uint32_t aId, const char *aName)
{
  PWX_TRY(return(insert(data, aNr, aId, aName, true)))
  PWX_THROW_FURTHER
}


/** @brief insert a new value
  *
  * This method is a convenience wrapper for insert(T *data,...) to be used on
  * values. See there for further information.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in]  value the value that shall be inserted
  * @param[in] aNr the wanted position by number, counted backwards if negative and wrapped if too large
  * @param[in] aId the wanted id. It will be changed if it has to be unique but isn't.
  * @param[in] aName the wanted name. It will be changed if it has to be unique but isn't.
  * @return the final number representing the position of the inserted item.
**/
template<typename T>
int32_t TMemRing<T>::insert (T value, int32_t aNr, uint32_t aId, const char *aName)
{
  try
    {
      T * xData = new T(value); // If this fails it's not our fault
      PWX_TRY(return(insert(xData, aNr, aId, aName, false)))
      PWX_THROW_FURTHER
    }
  catch (::std::bad_alloc &e)
    {
      PWX_THROW(cantCreateItem, e.what(), "Insert tried to turn a value into *data")
    }
}

// The following are little helper macros to allow TMemRing<T>::load() to use
// single line "instructions" for reading values and throw on each failure with the
// proper result. I guess nobody would want to have it cluttered like if writing
// those checks and throws directly...
// Note: isOK must be an available bool!
#define mrtLoadVal(val, ifs, sep, valName) { \
if (isOK && !ifs.eof()) \
  { \
    isOK = sh::readNextValue(val, ifs, sep, false, false); \
    if (!isOK) \
      { \
        if (xName) free (xName); \
        PWX_THROW(loadFailed, valName, "The value could not be loaded from stream") \
      } \
  } }
#define mrtSkipCRLF(ifs) { \
if (isOK && !ifs.eof()) \
  { \
    isOK = sh::skipLineBreak(ifs); \
    if (!isOK) \
      { \
        if (xName) free (xName); \
        PWX_THROW(loadFailed, "stream broken", "The stream broke when skipping line breaks") \
      } \
  } }


/** @brief load container from a file stream
  *
  * This method loads the container name, id and name map usage, and
  * all items from an open ifstream.
  * If the container is not empty, it will be cleared.
  *
  * Note: If the container already holds items, they won't be cleared if no
  * container data can be found in the stream.
  *
  * If the allocation of a name string fails, a pwx::mrf::allocFailed
  * exception is thrown.
  * If loading fails on a value, a pwx::mrf::loadFailed exception
  * is thrown.
  * If is is not possible to create a new item, a pwx::mrf::cantCreateItem exception
  * is thrown.
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] is open ifstream to read from
  * @param[in] search if set to true, the stream is searched for the next container start
**/
template<typename T>
void TMemRing<T>::load (::std::ifstream &is, bool search)
{
  if (is.good() && is.is_open())
    {
      char blockIdent = 0;

      if (search)
        {
          // Forward to the next identifier
          sh::forwardTo(is, base::ID_TRING_START);
          if (is.good())
            blockIdent = base::ID_TRING_START;
        }
      else if (!is.eof())
        // No search, so we depend that the right position is already found.
        is >> blockIdent;
      if (is.good() && (base::ID_TRING_START == blockIdent))
        {
          // We need some help for the name strings:
          char * xName = NULL;
          int32_t    nLen  = 32; // Allocated size of xName
          int32_t    fLen  = 0;  // Read length of the next name
          bool   isOK  = true;
          xName = static_cast<char *>(calloc(nLen + 1, sizeof(char)));
          if (!xName)
            {
              char msg[91] = "";
              pwx_snprintf(msg, 90, "Allocation of %d bytes for the container name failed.", nLen);
              PWX_THROW(allocFailed, "load() failed on container name allocation", msg)
            }

          // Before we can load anything, we have to make sure this container is empty:
          if (dataCount)
            clear();

          // Here we go:
          mrtLoadVal(maxId,       is, ';', "maximum known ID")
          mrtLoadVal(idMapUsed,   is, ';', "is ID Map used")
          mrtLoadVal(nameMapUsed, is, ';', "is Name Map used")
          mrtLoadVal(fLen,        is, ';', "Container Name Length")
          // is xName long enough?
          if (fLen > nLen)
            {
              // Our xName is too short
              char * newName = static_cast<char *>(realloc(xName, ((fLen + 1) * sizeof(char))));
              if (newName)
                {
                  xName   = newName;
                  newName = NULL;
                  nLen    = fLen;
                }
              else
                {
                  char msg[91] = "";
                  pwx_snprintf(msg, 90, "reallocation of %d to %d bytes for the container name failed.", nLen + 1, fLen + 1);
                  PWX_THROW(allocFailed, "load() failed on container name reallocation", msg)
                }
            }
          // Now the name can be read.
          if (is.peek() == ';') is.ignore(1);
          is.getline(xName, fLen);
          name = xName;
          mrtSkipCRLF(is)
          mrtLoadVal(blockIdent, is, 0, "Data Block Identifier")

          // Do we have data to load, now?
          if (base::ID_TRING_DATA == blockIdent)
            {
              // Yes we have!
              T           * newData  = NULL;
              uint32_t  xId          = 0;
              mrtLoadVal(blockIdent, is, ';', "Item Block Identifier")
              while (is.good() && (base::ID_CITEM_START == blockIdent))
                {
                  xId = 0;
                  PWX_TRY(newData = new T())
                  catch (::std::bad_alloc &e)
                    {
                      PWX_THROW(cantCreateItem, e.what(), "load() tried to create a new Data placeholder.")
                    }
                  mrtLoadVal(xId , is, ';', "Item ID")
                  mrtLoadVal(fLen, is, ';', "Item Name length")
                  // is xName long enough?
                  if (fLen > nLen)
                    {
                      // Our xName is too short
                      char * newName = static_cast<char *>(realloc(xName, ((fLen + 1) * sizeof(char))));
                      if (newName)
                        {
                          xName   = newName;
                          newName = NULL;
                          nLen    = fLen;
                        }
                      else
                        {
                          char msg[91] = "";
                          pwx_snprintf(msg, 90, "reallocation of %d to %d bytes for an item name failed.", nLen + 1, fLen + 1);
                          PWX_THROW(allocFailed, "load() failed on item name reallocation", msg)
                        }
                    }
                  // Now the name can be read.
                  if (is.peek() == ';')
                    is.ignore(1);
                  is.getline(xName, fLen, ';'); // getline eats the seperator
                  mrtLoadVal(blockIdent, is, 0, "Item Data Identifier")
                  if (base::ID_TITEM_START == blockIdent)
                    mrtLoadVal(*newData, is, ';', "Item Data")
                  else
                    is.putback(blockIdent); // If the data was empty, there is something else here
                  // Now we have to insert the item with its loaded values:
                  PWX_TRY(insert(newData, -1, xId, xName, true))
                  PWX_THROW_FURTHER
                  // Item data is ended by endline for safety, so skip again:
                  mrtSkipCRLF(is)
                  mrtLoadVal(blockIdent, is, 0, "Identifier after Item Data")
                }
            } // End of having a data block
          else
            // If it was something else, we put it back:
            is.putback(blockIdent);
          // Before we exit, we need to free our xName:
          if (xName) free (xName);
        } // End of is TRing
      else
        PWX_THROW(loadFailed, "Container load failed", "No container found in stream")
    } // End of having a good ifstream
}

// leave a green planet!
#undef mrtloadVal
#undef mrtSkipCRLF

/** @brief retrieve a pointer to the last items data from the container
  *
  * This method returns the last items data from the container and deletes
  * the item. The stored data is kept.
  * Due to this behavior you are responsible to free the data yourself
  * unless another item points to this data. In this case it would crash
  * your program to delete the data by hand.
  * To be save you can use getItemRefCount(-1) before using pop(). If the
  * result is larger than one, you must not delete the returned data. If
  * the result equals one, you have to delete the data by hand.
  *
  * If there are no items in this container, the method returns NULL.
  *
  * @return a pointer to the data stored by the last item in the ring, or NULL if the container is empty
**/
template<typename T>
T * TMemRing<T>::pop ()
{
  T * result = NULL;

  if (root)
    {
      result = operator[](-1); // This also sets current
      deleteCurr(false);
    }

  return (result);
}


/** @brief push_back data
  *
  * This method pushes new data into the container, meaning it will become the last item.
  * If the id map is used, the given Id is changed if not unique, the same happens with the
  * name if the name map is used.
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
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in]  data the data that shall be inserted
  * @param[in] aId the wanted id. It will be changed if it has to be unique but isn't.
  * @param[in] aName the wanted name. It will be changed if it has to be unique but isn't.
**/
template<typename T>
void TMemRing<T>::push (T *data, uint32_t aId, const char *aName)
{
  assert(data); // runtime check to not call with NULL data

  // With the new behaviour of insert(), it can do the job:
  PWX_TRY(insert(data, -1, aId, aName, true))
  PWX_THROW_FURTHER
}


/** @brief push_back a value
  *
  * This method is a convenience wrapper to push back a value that is turned into an item.
  *
  * See push(T *data,...) for more information.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in]  value the value that shall be inserted
  * @param[in] aId the wanted id. It will be changed if it has to be unique but isn't.
  * @param[in] aName the wanted name. It will be changed if it has to be unique but isn't.
**/
template<typename T>
void TMemRing<T>::push (T value, uint32_t aId, const char *aName)
{
  try
    {
      T * xData = new T(value); // If this fails it's not our fault
      insert(xData, -1, aId, aName, false);
    }
  catch (::std::bad_alloc &e)
    {
      PWX_THROW(cantCreateItem, e.what(), "Add tried to turn a value into *data")
    }
  PWX_THROW_FURTHER
}


/** @brief Returns true if the id map is used, false otherwise.
**/
template <typename T>
bool TMemRing<T>::isIdMapUsed () const
{
  return (idMapUsed);
}


/** @brief Returns true if the name map is used, false otherwise.
**/
template <typename T>
bool TMemRing<T>::isNameMapUsed () const
{
  return (nameMapUsed);
}


/** @brief This method merges all items from source into this container.
  *
  * The source container will be empty afterwards, and destroyed unless
  * autodestruct is set to false.
  * The items will be appended, thus receiving new numbers.
  * Ids that would be non-unique but have to for the id map will be
  * changed. The same applies to names if the name map is used.
  *
  * If is is not possible to create a new item, a pwx::mrf::cantCreateItem exception
  * is thrown.
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * @param[in] source the container that shall be merged with this
  * @param[in] autodestruct if set to true, the source container will be deleted
  * @return the total item count after the merge
**/
template <typename T>
int32_t TMemRing<T>::mergeWith (TMemRing<T> *source, bool autodestruct)
{
  assert (source != this); // Run time assertion

  PWX_TRY(addAllFromCont(source, true))
  PWX_THROW_FURTHER

  if (source && (source != this) && autodestruct)
    delete(source);

  return (dataCount);
}


/** @brief move an item around
  *
  * This method can be used to move an item around the memory ring.
  * The numbers are counted from the beginning if they are positive,
  * and from the end if negative. If they are too large, they are
  * wrapped.
  *
  * Positioning: If @a newNr is positive, the item having this
  * number will be pushed aside. If @a newNr is negative, the
  * item having @a oldNr will be placed <I>after</I> the item with
  * the number @a newNr.
  *
  * If the container has no items, a pwx::mrf::nrOutOfRange
  * exception is thrown.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] oldNr the position of the item that should be moved
  * @param[in] newNr the position of the item it should be moved to
  * @return the final position
**/
template <typename T>
int32_t TMemRing<T>::move (int32_t oldNr, int32_t newNr)
{
  int32_t xOld = normalizeNumber(oldNr);
  int32_t xNew = normalizeNumber(newNr);

  if (xOld != xNew)
    {
      TItem<T> *newNeighbor = NULL;

      // Get the new neighbor:
      if (setCurrToNr(xNew))
        newNeighbor = current;
      else
        {
          char msg[91] = "";
          pwx_snprintf(msg, 90, "The nr %d (arg %d), is out of range in \"%s\".",
                         xNew, newNr, name.substr(0,26).c_str());
          if (dataCount)
            PWX_THROW(nrOutOfRange, "Numbering broken! [move() new]", msg)
         else
           PWX_THROW(nrOutOfRange, "Container empty! [move() new]", msg)
        }

      // set the item to move:
      if (!setCurrToNr(xOld))
        {
          char msg[91] = "";
          pwx_snprintf(msg, 90, "The nr %d (arg %d), is out of range in \"%s\".",
                         xOld, oldNr, name.substr(0,26).c_str());
          if (dataCount)
            PWX_THROW(nrOutOfRange, "Numbering broken! [move() old]", msg)
          else
            PWX_THROW(nrOutOfRange, "Container empty! [move() old]", msg)
        }

      TItem<T> *newRoot = NULL;
      try
        {
          if (newNr < 0)
            newRoot = current->move(newNeighbor, NULL, NULL); // Move after new neighbor
          else
            newRoot = current->move(NULL, newNeighbor, NULL); // Move in front of new neighbor
          if (newRoot && (0 == newRoot->getNr()))
            root = newRoot;
        }
      PWX_THROW_FURTHER

      assert (root && (0 == root->getNr()) && "\nERROR: move invalidated root!\n");
    }

  return (xOld);
}


/** @brief remove an item
  *
  * This method removes the item with the given number and returns the held
  * data. If the container is empty, this method returns NULL.
  *
  * Like unshift() and pop() the responsibility for the returned data is
  * sent over to you. So unless getItemRefCount(@a aNr) did not return
  * something larger than 1, you have to delete the data yourself. If it
  * returns a higher reference count, you must not delete the returned data.
  *
  * @param[in] aNr the number of the item to remove
  * @return a pointer to the held data
**/
template <typename T>
T * TMemRing<T>::remove (int32_t aNr)
{
  T * result = NULL;
  if (dataCount)
    {
      result = operator[](aNr); // This also sets current
      deleteCurr(false);
    }
  return (result);
}



/** @brief save all items in an ostream
  *
  * Important: The stored data must have an operator<<(stream, data)
  *            to be used with!
  *
  * @param[in] os an open ostream
**/
template <typename T>
void TMemRing<T>::save (::std::ostream &os) const
{
  if (os.good())
    {
      os << base::ID_TRING_START << ";";
      os << maxId << ";";
      os << idMapUsed << ";";
      os << nameMapUsed << ";";
      os << (name.size() + 1) << ";" << name << ::std::endl;

      // Now walk through the ring and push all items onto the stream:
      if (root)
        {
          os  << base::ID_TRING_DATA << ";";
          TItem<T> * oldCurr = current;
          current = root;
          do
            {
              if (current)
                {
                  current->save(os);
                  current = current->getNext();
                }
            }
          while (os.good() && current && (current != root));
          // Go back:
          current = oldCurr;
        }
      else
        os << ::std::endl;
    }
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
uint32_t TMemRing<T>::setItemId (int32_t aNr, uint32_t aId)
{
  int32_t xNr = normalizeNumber(aNr);
  if (!setCurrToNr(xNr))
    {
      char msg[91] = "";
      pwx_snprintf(msg, 90, "The nr %d (arg %d), is out of range in \"%s\".",
                     xNr, aNr, name.substr(0,26).c_str());
      if (dataCount)
        PWX_THROW(nrOutOfRange, "Numbering broken! [setItemId()]", msg)
      else
        PWX_THROW(nrOutOfRange, "Container empty! [setItemId()]", msg)
    }
  uint32_t oldId = current->getId();
  if (aId != oldId)
    {
      if (idMapUsed)
        {
          uint32_t xId = getUniqueId(aId);
          idMap.erase(oldId);
          current->setId(xId);
          if (!idMap.insert(tIdPair(current->getId(), current)).second)
            PWX_THROW(noIdMapAvailable, "setItemId failed", "setItemId tried to insert")
        }
      else
        current->setId(aId);
    }
  return (current->getId());
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
const char * TMemRing<T>::setItemName (int32_t aNr, const char *aName)
{
  int32_t xNr = normalizeNumber(aNr);
  if (!setCurrToNr(xNr))
    {
      char msg[91] = "";
      pwx_snprintf(msg, 90, "The nr %d (arg %d), is out of range in \"%s\".",
                     xNr, aNr, name.substr(0,26).c_str());
      if (dataCount)
        PWX_THROW(nrOutOfRange, "Numbering broken! [setItemName()]", msg)
      else
        PWX_THROW(nrOutOfRange, "Container empty! [setItemName()]", msg)
    }
  const char * oldName = current->getName();
  if (!aName || STRNE(aName, oldName))
    {
      if (nameMapUsed)
        {
          char * xName = getUniqueName(aName);
          nameMap.erase(oldName);
          current->setName(xName);
          if (xName) free (xName);
          if (!nameMap.insert(tNamePair(current->getName(), current)).second )
            PWX_THROW(noNameMapAvailable, "setItemName failed", "setItemName tried to insert")
        }
      else
        current->setName(aName ? aName : "");
    }
  return (current->getName());
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
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
**/
template <typename T>
void TMemRing<T>::sort (bool asc)
{
  PWX_TRY(for ( ; sort_once(asc) ; ) )
  PWX_THROW_FURTHER
}


/** @brief sort items by Id
  *
  * This method sorts all items by their Id. If @a asc is omitted or
  * set to true, they will be sorted in ascending order. For a descending
  * order @a asc has to be set to false.
  *
  * The data you store has to support "bool operator>(T&)" for this to work!
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
**/
template <typename T>
void TMemRing<T>::sortById (bool asc)
{
  PWX_TRY(for ( ; sortById_once(asc) ; ) )
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
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
**/
template <typename T>
void TMemRing<T>::sortByName (bool asc)
{
  PWX_TRY(for ( ; sortByName_once(asc) ; ) )
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
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
  * @return The number of items that have been moved
**/
template <typename T>
int32_t TMemRing<T>::sort_once (bool asc)
{
  PWX_TRY(return sort(asc ? base::OT_LargerData : base::OT_SmallerData))
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
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
  * @return The number of items that have been moved
**/
template <typename T>
int32_t TMemRing<T>::sortById_once (bool asc)
{
  PWX_TRY(return sort(asc ? base::OT_LargerId : base::OT_SmallerId))
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
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] asc if true (default), order ascending, descending otherwise
  * @return The number of items that have been moved
**/
template <typename T>
int32_t TMemRing<T>::sortByName_once (bool asc)
{
  PWX_TRY(return sort(asc ? base::OT_LargerName : base::OT_SmallerName))
  PWX_THROW_FURTHER
}


/** @brief retrieve a pointer to the first data from the container
  *
  * This method returns the first items data from the container and deletes
  * the item. The stored data is kept.
  * Due to this behavior you are responsible to free the data yourself unless
  * another container has an item pointing to this data. In this case it would
  * crash your program to delete the data by hand.
  * To be save you can use getItemRefCount(0) before using shift(). If the
  * result is larger than one, you must not delete the returned data. If the
  * result equals one, you have to delete the data by hand.
  *
  * If there are no items in this container, the method returns NULL.
  *
  * @return a pointer to the data stored by the first item in the ring, or NULL if the container is empty
**/
template<typename T>
T * TMemRing<T>::shift ()
{
  T * result = NULL;

  if (root)
    {
      result = operator[](0); // This sets current to root
      deleteCurr(false);
    }

  return (result);
}



/** @brief push_front data
  *
  * This method unshifts new data into the container, meaning it will become the first data.
  * If the id map is used, the given Id is changed if not unique, the same happens with the
  * name if the name map is used.
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
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in]  data the data that shall be inserted
  * @param[in] aId the wanted id. It will be changed if it has to be unique but isn't.
  * @param[in] aName the wanted name. It will be changed if it has to be unique but isn't.
**/
template<typename T>
void TMemRing<T>::unshift (T *data, uint32_t aId, const char *aName)
{
  // Luckily this can be done by insert()
  PWX_TRY(insert(data, 0, aId, aName, true))
  PWX_THROW_FURTHER
}


/** @brief push_front a value
  *
  * This method is a convenience wrapper to push front a value that is turned into an item.
  *
  * See unshift(T *data,...) for more information.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in]  value the value that shall be inserted
  * @param[in] aId the wanted id. It will be changed if it has to be unique but isn't.
  * @param[in] aName the wanted name. It will be changed if it has to be unique but isn't.
**/
template<typename T>
void TMemRing<T>::unshift (T value, uint32_t aId, const char *aName)
{
  try
    {
      T * xData = new T(value); // If this fails it's not our fault
      PWX_TRY(insert(xData, 0, aId, aName, false))
      PWX_THROW_FURTHER
    }
  catch (::std::bad_alloc &e)
    {
      PWX_THROW(cantCreateItem, e.what(), "Unshift tried to turn a value into *data")
    }
}


/** @brief enable or disables the usage of the id map.
  *
  * If you change the state, the id map is either cleared or filled.
  * It will never keep a state that leads to an inconsistent state.
  * For this reason you should not change the state of the id map
  * usage on containers that already hold a lot of items.
  *
  * Note: if you enable the id map usage on a container that holds
  * items, then the ids currently set might get changed to ensure
  * that every id is unique!
  *
  * If the insertion of an item fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  *
  * @param[in] state Enable (true) or disable (false) the id map usage
**/
template<typename T>
void TMemRing<T>::useIdMap (bool state)
{
  if (state != idMapUsed)
    {
      idMapUsed = state;
      if (idMapUsed && root)
        {
          // Now we (might) have to fill idMap:
          current = root;
          do
            {
              // 1 - ensure that the id is unique
              current->setId(getUniqueId(current->getId()));

              // 2 - insert into map:
              if (!idMap.insert(tIdPair(current->getId(), current)).second )
                PWX_THROW(noIdMapAvailable, "useIdMap failed", "useIdMap tried to insert")

              // 3 - go to the next item:
              current = current->getNext();
            }
          while (current && (current != root));
        }
      else if (root)
        // Simple matter, just clear the map:
        idMap.clear();
    } // end of action to be taken
}


/** @brief enable or disables the usage of the name map.
  *
  * If you change the state, the name map is either cleared or filled.
  * It will never keep a state that leads to an inconsistent state.
  * For this reason you should not change the state of the name map
  * usage on containers that already hold a lot of items.
  *
  * Note: if you enable the name map usage on a container that holds
  * items, then the names currently set might get changed to ensure
  * that every name is unique!
  *
  * If the insertion of an item fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * @param[in] state Enable (true) or disable (false) the name map usage
**/
template<typename T>
void TMemRing<T>::useNameMap (bool state)
{
  if (state != nameMapUsed)
    {
      nameMapUsed = state;
      if (nameMapUsed && root)
        {
          // Now we (might) have to fill idMap:
          char * xName = NULL;
          current      = root;
          do
            {
              // 1 - ensure that the name is unique
              xName = getUniqueName(current->getName());
              current->setName(xName);
              if (xName) free (xName);

              // 2 - insert into map:
              if (!nameMap.insert(tNamePair(current->getName(), current)).second )
                PWX_THROW(noNameMapAvailable, "useNameMap failed", "useNameMap tried to insert")

              // 3 - go to the next item:
              current = current->getNext();
            }
          while (current && (current != root));
        }
      else if (root)
        // Simple matter, just clear the map:
        nameMap.clear();
    } // end of action to be taken
}


// --- templated methods ---

/** @brief This method returns true, if the givin data is stored in this container.
  *
  * Note: It has to walk through the memory ring structure, so you might
  *       not want to use it unless you really really need to.
  *
  * @param[in]  data the data you search for
**/
template <typename T> template<typename Td>
bool TMemRing<T>::isIn (Td *data) const
{
  bool result = false;

  // Don't do anything at all if it is the wrong type:
  if (isSameType(T, Td))
    result = setCurrToItem(data);

  return (result);
}


/** @brief This method returns true, if the givin value is stored in this container.
  *
  * Note: It has to walk through the memory ring structure, and compare with
  *       every single item content, so you might not want to use it unless you
  *       really really need to.
  *
  * @param[in]  value the value you search for
**/
template <typename T> template<typename Td>
bool TMemRing<T>::isValueIn (Td value) const
{
  bool result = false;

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
T* TMemRing<T>::operator[] (int32_t aNr) const
{
  int32_t xNr = normalizeNumber(aNr);
  if (!setCurrToNr(xNr))
    {
      char msg[91] = "";
      pwx_snprintf(msg, 90, "The nr %d (arg %d), is out of range in \"%s\".",
                     xNr, aNr, name.substr(0,26).c_str());
      if (dataCount)
        PWX_THROW(nrOutOfRange, "Numbering broken! (operator[])", msg)
      else
        PWX_THROW(nrOutOfRange, "Container empty! (operator[])", msg)
    }
  return (current->getDataPointer());
}


/** @brief clone a container
  *
  * This operator will empty the container and then copy the contents of the
  * source container into it. The source container will remain untouched.
  *
  * To move all items from source, use mergeWith().
  *
  * If is is not possible to create a new item, a pwx::mrf::cantCreateItem exception
  * is thrown.
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * @param[in] source the source container
  * @return a reference to this
**/
template<typename T>
TMemRing<T> &TMemRing<T>::operator= (TMemRing<T> &source)
{
  TMemRing<T> * xSource = &source;
  assert (xSource != this); // Run time assertion
  if (xSource != this)
    {
      clear();

      // mirror map settings:
      idMapUsed   = xSource->isIdMapUsed();
      nameMapUsed = xSource->isNameMapUsed();

      // Tracking is not mirrored, as items are regularly added.

      PWX_TRY(addAllFromCont(xSource, false))
      PWX_THROW_FURTHER
    }
  return (*this);
}

/** @brief add all items from one container to this
  *
  * This operator adds all items from the source container to this one.
  * These are copies, they are not removed from the source container!
  *
  * To actually move the items, use the mergeWith() method.
  *
  * If is is not possible to create a new item, a pwx::mrf::cantCreateItem exception
  * is thrown.
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * @param[in] source the source container
  * @return a reference to this
**/
template<typename T>
TMemRing<T> &TMemRing<T>::operator+= (TMemRing<T> &source)
{
  TMemRing<T> * xSource = &source;

  PWX_TRY(addAllFromCont(xSource, false))
  PWX_THROW_FURTHER

  return (*this);
}


/** @brief add data to this container
  *
  * Operator to add data at the end of the memory ring. Id and name are
  * set automatically, and ensured to be unique if idMap and/or nameMap
  * is used.
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
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] data to be added
  * @return a reference to this
**/
template<typename T>
TMemRing<T> &TMemRing<T>::operator+= (T *data)
{
  PWX_TRY(insert(data, -1, 0, "", true))
  PWX_THROW_FURTHER
  return (*this);
}


/** @brief add a value to this container
  *
  * This operator is a convenient wrapper to add values.
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
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] value to be added
  * @return a reference to this
**/
template<typename T>
TMemRing<T> &TMemRing<T>::operator+= (T value)
{
  try
    {
      T * xData = new T(value); // If this fails it's not our fault
      PWX_TRY(insert(xData, -1, 0, "", false))
      PWX_THROW_FURTHER
    }
  catch (::std::bad_alloc &e)
    {
      PWX_THROW(cantCreateItem, e.what(), "Add tried to turn a value into *data")
    }
  return (*this);
}


/** @brief remove all items stored in another container
  *
  * This operator removes all items from this container, that are stored
  * in the source container as well.
  *
  * Using this operator with itself (aka foo -= foo;) is the same as
  * calling clear() and will empty the container.
  *
  * If you substract a different container, no data will be deleted, as
  * the source container holds a copy of each. If you substract itself,
  * all data that is not stored elsewhere, too, is deleted.
  *
  * @param[in] source the source container
  * @return a reference to this
**/
template<typename T>
TMemRing<T> &TMemRing<T>::operator-= (TMemRing<T> &source)
{
  if (this == &source)
    clear();
  else
    {
      for (int32_t i = 0; i < source.size(); ++i)
        erase(source[i]);
    } // end of action part
  return (*this);
}


/** @brief remove an item
  *
  * This operator removes the given data from this container. Please be aware
  * that no data that has a copy pointing to it will be deleted. Only the
  * item will be removed from this container.
  *
  * This operator never throws, it simply does nothing if @a data is not
  * stored here.
  *
  * @param[in] data to be erased
  * @return a reference to this
 */
template<typename T>
TMemRing<T> &TMemRing<T>::operator-= (T *data)
{
  if (data)
    {
      while (setCurrToItem(data))
        deleteCurr(true);
    }

  return (*this);
}


/** @brief remove a value
  *
  * This operator removes all items whose data equals the given value from
  * this container. Please be aware that no data that has a copy pointing
  * to it will be deleted. Only the item will be removed from this container.
  *
  * @param[in] value to be erased
  * @return a reference to this
 */
template<typename T>
TMemRing<T> &TMemRing<T>::operator-= (T value)
{
  while (setCurrToValue(value))
    deleteCurr(true);
  return (*this);
}


// --- private helper methods ---

/** @brief add all items from source to this, delete from source if emptySource is true
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
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
**/
template<typename T>
void TMemRing<T>::addAllFromCont (TMemRing<T> * source, bool emptySource)
{
  if (source && (source != this))
    {
      // push all source items upon this, removing them from source if wanted:
      int32_t i   = source->size();
      int32_t xNr;
      TItem<T> * endItem = root ? (root->getPrev() ? root->getPrev() : root) : NULL;
      TItem<T> * newItem = NULL;

      while (i)
        {
          --i;
          // Use the TItem<T> copy ctor to make the items know their copies
          PWX_TRY(newItem = new TItem<T>(*(source->getItem(i))))
          catch (::std::bad_alloc &e)
            {
              char txt[128] = "";
              pwx_snprintf(txt, 127, "mergeWith() tried to create a copy TItem from nr %u, [%s], container [%s]",
                      i, ::std::string(source->getName()).substr(0,16).c_str(),
                      ::std::string(source->getItem(i)->getName()).substr(0,16).c_str());
              PWX_THROW(cantCreateItem, e.what(), txt)
            }

          // Set unique id and name:
          if (idMapUsed || nameMapUsed)
            {
              uint32_t xId   = getUniqueId(newItem->getId());
              char   * xName = getUniqueName(newItem->getName());
              newItem->setCoreValues(0, xId, xName);
              if (xName) free (xName);
            }

          // Now add the item:
          PWX_TRY(xNr = endItem ? movInsAfter(newItem, endItem, current) : movInsBefore(newItem, NULL, current))
          PWX_THROW_FURTHER

          if (root->getPrev())
            xNr = root->getPrev()->getNr();

          // Now dataCount must be xNr + 1:
          assert (dataCount == (xNr + 1));
          if (dataCount == (xNr + 1))
            PWX_THROW(strayItem, "Insert failed!", "dataCount is wrong after inserting an item into the target container!")

          if (emptySource)
            // remove item from source:
            source->remove(i);
        } // End of moving cycle
    } // End of action part
  // Finally dataCount must always be the last number plus one:
  assert( (!root && !dataCount) // No items and data count is zero
       || ( root && !root->getPrev() && (1 == dataCount)) // root is alone
       || ( root &&  root->getPrev()
         &&(root->getPrev()->getNr() == (dataCount - 1))) ); // correct numbering on more than one item
}


/** @brief delete current and set current to the former predecessor
**/
template<typename T>
void TMemRing<T>::deleteCurr (bool withData)
{
  if (current)
    {
      // save old prev
      TItem<T> * prev = (current != root) ? current->getPrev() : current->getNext();

      // Save id and name for later use
      uint32_t      oldId   = current->getId();
      ::std::string oldName = current->getName();

      TItem<T> *newRoot = current->destroy(withData);
      if (newRoot && (0 == newRoot->getNr()))
        root = newRoot;
      current = prev;
      --dataCount;

      assert (((0 == dataCount) || (root && (0 == root->getNr()))) && "\nERROR: deleteCurr invalidated root!\n");

      // Drop from id map:
      if (idMapUsed && (idMap.erase(oldId) < 1))
        rebuildIdMap();
      // Drop from name map:
      if (nameMapUsed && oldName.size() && (nameMap.erase(oldName.c_str()) < 1))
        rebuildNameMap();

      if (0 == dataCount)
        {
          // This was the last data:
          current = NULL;
          root    = NULL;
        }

    } // End of having current
}


/** @brief internal method to get a unique name
  *
  * This method must not be used unless nameMapUsed is true!
  *
  * @return a malloc'd string, so you are responsible to free it after usage!
**/
template<typename T>
char * TMemRing<T>::getUniqueName (const char * src) const
{
  int32_t       number = 0;
  ::std::string xName  = src ? src : "";
  ::std::string result = xName; // We need two for incremental optional appendices

  // If we have no name (yet), we create one:
  if (xName.empty())
    {
      char newName[16] = "";
      pwx_snprintf(newName, 15, "data_%010u", maxId);
      xName  = newName; // Now stays this name
      result = xName;
    }

  // Now, does it have to be changed?
  if (nameMap.find(xName) != nameMap.end())
    {
      // We already have such an element
      char appendix[11] = "";
      do
        {
          result = xName;
          pwx_snprintf(appendix, 10, "_%08u", ++number);
          result += appendix;
        }
      // still not unique ?
      while (nameMap.find(result) != nameMap.end());
    }

  return pwx_strdup(result.c_str());
}


/** @brief internal method to get a unique id
  *
  * This method must always be called when an ID has to be set and
  * *before* getUniqueName is called!
**/
template<typename T>
uint32_t TMemRing<T>::getUniqueId (uint32_t src)
{
  uint32_t result = src ? src : 1;

  // maxId is raised for every call to this method:
  ++maxId; // (otherwise getUniqueName() would be inefficient)

  if (idMapUsed && (idMap.find(result) != idMap.end()))
    // We already have such an element
    result = maxId;
  else if (result > maxId)
    maxId = result;

  return result;
}


/** @brief internal method to move a new item after another item
  *
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * @param[in]  newItem the new item to be moved
  * @param[in]  newPrev the item the new one should precede
  * @param[in]  oldCurr the previous current to revert to if things go wrong
**/
template<typename T>
int32_t TMemRing<T>::movInsAfter (TItem<T> * newItem, TItem<T> * newPrev, TItem<T> * oldCurr)
{
  assert(newItem);     // runtime check against new NULL data

  if (root && !newPrev)
    newPrev = root->getPrev() ? root->getPrev() : root; // None given, will become last of the items

  try
    {
      TItem<T> *newRoot = newItem->move(newPrev, NULL, NULL); // This'll take care of the right position and numbering
      if (newRoot && (0 == newRoot->getNr()))
        root = newRoot;
      assert(root && (0 == root->getNr()) && "\nERROR: move did not report root correctly in movInsAfter!\n");
    }
  PWX_THROW_FURTHER

  current = newItem;
  int32_t result = current->getNr();
  ++dataCount;

  // Now fill the maps if they are wanted:
  if (idMapUsed && !idMap.insert(tIdPair(current->getId(), current)).second )
    {
      current = oldCurr;
      TItem<T> *newRoot = newItem->destroy(true);
      if (newRoot && (0 == newRoot->getNr()))
        root = newRoot;
      assert (root && (0 == root->getNr()) && "\nERROR: movInsAfter invalidated root!\n");
      --dataCount;
      PWX_THROW(noIdMapAvailable, "movInsAfter failed", "movInsAfter tried to insert an id into the map")
    }
  else if (nameMapUsed && !nameMap.insert(tNamePair(current->getName(), current)).second )
    {
      current = oldCurr;
      TItem<T> *newRoot = newItem->destroy(true);
      if (newRoot && (0 == newRoot->getNr()))
        root = newRoot;
      assert (root && (0 == root->getNr()) && "\nERROR: movInsAfter invalidated root!\n");
      --dataCount;
      PWX_THROW(noNameMapAvailable, "movInsAfter failed", "movInsAfter tried to insert a name into the map")
    }

  return (result);
}


/** @brief internal method to move a new item before another item
  *
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in]  newItem the new item to be moved
  * @param[in]  newNext the item the new one should precede
  * @param[in]  oldCurr the previous current to revert to if things go wrong
**/
template<typename T>
int32_t TMemRing<T>::movInsBefore (TItem<T> * newItem, TItem<T> * newNext, TItem<T> * oldCurr)
{
  assert(newItem);     // runtime check against new NULL data

  if (root && !newNext)
    newNext = root; // None given, will become last of the items

  try
    {
      TItem<T> *newRoot = newItem->move(NULL, newNext, NULL); // This'll take care of the right position and numbering
      if (newRoot && (0 == newRoot->getNr()))
        root = newRoot;
      assert(root && (0 == root->getNr()) && "\nERROR: move did not report root correctly in movInsBefore!\n");
    }
  PWX_THROW_FURTHER

  current = newItem;
  int32_t result = current->getNr();
  ++dataCount;

  // Now fill the maps if they are wanted:
  if (idMapUsed && !idMap.insert(tIdPair(current->getId(), current)).second )
    {
      current = oldCurr;
      TItem<T> *newRoot = newItem->destroy(true);
      if (newRoot && (0 == newRoot->getNr()))
        root = newRoot;
      assert (root && (0 == root->getNr()) && "\nERROR: movInsBefore invalidated root!\n");
      PWX_THROW(noIdMapAvailable, "movInsBefore failed", "movInsBefore tried to insert")
    }
  if (nameMapUsed && !nameMap.insert(tNamePair(current->getName(), current)).second )
    {
      current = oldCurr;
      TItem<T> *newRoot = newItem->destroy(true);
      if (newRoot && (0 == newRoot->getNr()))
        root = newRoot;
      assert (root && (0 == root->getNr()) && "\nERROR: movInsBefore invalidated root!\n");
      PWX_THROW(noNameMapAvailable, "movInsBefore failed", "movInsBefore tried to insert")
    }

  return (result);
}


/** @brief internal method to normalize a signed number to unsigned index
**/
template<typename T>
int32_t TMemRing<T>::normalizeNumber (int32_t aNr) const
{
  int32_t result = 0;

  if (dataCount > 2)
    {
      // We normalize here
      result = aNr;
      if      (result > dataCount) result %= dataCount;
      else if (result < 0        ) result  = dataCount - (::std::abs(result) % dataCount);
    }
  else if ((dataCount == 2) && (::std::abs(aNr) % 2))
    result = 1; // with two items only odd numbers are item 2, aka nr 1

  return (result);
}


/** @brief internal method to build the name map from scratch
**/
template<typename T>
void TMemRing<T>::rebuildIdMap  ()
{
  idMap.clear();
  bool finished = false;
  TItem<T> * oldCurr = current;
  current = root;
  while (current && !finished && idMapUsed)
    {
      // Try to insert:
      if (!idMap.insert(tIdPair(current->getId(), current)).second )
        {
          // As we must not throw, clear what we have and shut the usage off
          idMap.clear();
          idMapUsed = false;
        }
      else
        // We succeeded, so advance:
        current = current->getNext();
      if (current == root)
        // If we are back on root, we are finished:
        finished = true;
    }
  current = oldCurr;
}


/** @brief internal method to build the name map from scratch
**/
template<typename T>
void TMemRing<T>::rebuildNameMap()
{
  nameMap.clear();
  bool finished = false;
  TItem<T> * oldCurr = current;
  current = root;
  while (current && !finished && nameMapUsed)
    {
      // Try to insert:
      if (!nameMap.insert(tNamePair(current->getName(), current)).second )
        {
          // As we must not throw, clear what we have and shut the usage off
          nameMap.clear();
          nameMapUsed = false;
        }
      else
        // We succeeded, so advance:
        current = current->getNext();
      if (current == root)
        // If we are back on root, we are finished:
        finished = true;
    }
  current = oldCurr;
}


/** @brief internal method to set current to the item with the given ID
  *
  * If the Id could not be found, the method returns false, true on success
**/
template<typename T>
bool TMemRing<T>::setCurrToId  (uint32_t aId) const
{
  bool result = false;
  if (!current || (current->getId() != aId))
    {
      // What a pity, we need to act:
      current = root;
      if (root && (current->getId() != aId))
        {
          // we need to search, try id map first if available
          if (idMapUsed)
            {
              // We'll try ...
              tIdIter_c iter = idMap.find(aId);
              if (iter != idMap.end())
                current = iter->second;
            } // End of search by id map
          else if (current)
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
                      assert ((xNext || (dataCount < 2)) && "ERROR: setCurrId() hit a dead end when it shouldn't!");
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


/** @brief internal method to set current to the item holding the given @a data
  *
  * If the data could not be found, false is returned, true on success
**/
template<typename T>
bool TMemRing<T>::setCurrToItem (T *data) const
{
  bool result = false;
  if (!current || (current->getDataPointer() != data))
    {
      // What a pity, we need to act:
      current = root;
      if (root && (current->getDataPointer() != data))
        {
          TItem<T> *xPrev = current;
          TItem<T> *xNext = current ? current->getNext() : NULL;
          while (current && (current->getDataPointer() != data) && (xNext != root))
            {
              if (!xNext)
                {
                  // It might be that current is moved away by a different thread.
                  if (xPrev && (current != xPrev->getNext()))
                    xNext = xPrev->getNext();
                  assert ((xNext || (dataCount < 2)) && "ERROR: setCurrName() hit a dead end when it shouldn't!");
                }
              else
                xPrev = current;
              current = xNext;
              xNext = current ? current->getNext() : NULL;
            } // End of no target found and ring not finished yet
        }
    }

  // Were we successful?
  if (current && (current->getDataPointer() == data))
    result = true;

  return (result);
}


/** @brief internal method to set current to the item holding the given @a data
  *
  * If the data could not be found, false is returned, true on success
  *
  * Note: this is just a const wrapper!
**/
template<typename T>
bool TMemRing<T>::setCurrToItem (const T *data) const
{
  return (setCurrToItem(const_cast<T *>(data)));
}


/** @brief internal method to set current to the item with the given ID
  *
  * If the name could not be found the result is false, true otherwise
**/
template<typename T>
bool TMemRing<T>::setCurrToName  (const char * aName) const
{
  bool result = false;
  if (!current || STRNE(current->getName(), aName))
    {
      // We don't have it already, so do something
      current = root;
      if (root && STRNE(current->getName(), aName))
        {
          // we need to search, try name map first if available
          if (nameMapUsed)
            {
              // We'll try ...
              tNameIter_c iter = nameMap.find(aName);
              if (iter != nameMap.end())
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
                      assert ((xNext || (dataCount < 2)) && "ERROR: setCurrName() hit a dead end when it shouldn't!");
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


/** @brief internal method to set current to the item with the given Number
  *
  * If the number is out of range the result is false, true otherwise
**/
template<typename T>
bool TMemRing<T>::setCurrToNr  (int32_t aNr) const
{
  bool result = false;
  assert ((aNr >= -1) && (aNr < dataCount) && "\nERROR: setCurrToNr called with aNr out of range!\n");

  if (!current || (current->getNr() != aNr))
    {
      // To make loops more effective, we check the next ...
      if (current && current->getNext() && ( aNr == current->getNext()->getNr() ) )
        current = current->getNext();
      // ... and the prev item first
      else if (current && current->getPrev() && ( aNr == current->getPrev()->getNr() ) )
        current = current->getPrev();
      // Special handling if it is the last item
      else if (-1 == aNr)
        current = root ? root->getPrev() ? root->getPrev() : root : NULL;
      // If we have exactly two items, the while loop below would not work:
      else if ((2 == dataCount) && (aNr < 2) && (aNr > -1))
        current = (0 == aNr) ? root : root->getNext();
      else
        {
          // current is somewhere else, go walking:
          if (!current)
            current = root;

          if (current && (current->getNr() != aNr))
            {
              /* There are four possible movements:
               * 1.: Upwards from root
               * 2.: Downwards from root
               * 3.: Upwards from current
               * 4.: Downwards from current.
               * So lets see what's the smallest distance first
               */
              bool bAsc = true; // per default we think its 1 or 3.
              int32_t currNum  = current->getNr();
              int32_t rootDist = dataCount - aNr; // Upwards direction is aNr itself
              int32_t currDist = ::pwx::absDistance(currNum, 0, aNr, 0); // We do not know which direction yet
              if (aNr < currDist)
                // Case 1.: Upwards from root
                current = root;
              else if (rootDist < currDist)
                {
                  // Case 2.: Downwards from root
                  current = root->getPrev();
                  bAsc = false;
                }
              else if (currNum > aNr)
                // Case 4.: Downwards from current
                bAsc = false;
              // Case 3.: Upwards from current is the default.

              while (current && (current->getNr() != aNr)
                   && ( ( bAsc && (current->getNext() != root))
                     || (!bAsc && (current != root)) ) )
                current = bAsc ? current->getNext() : current->getPrev();
            } // End of everything fit for the search
        } // End of action
    } // end of check

  // Were we successful?
  if (current &&
      ( ((aNr >= 0) && (current->getNr() == aNr))
      ||((aNr <  0) && (current->getNr() == (dataCount + aNr)))
      ) )
    result = true;

  return (result);
}


/** @brief internal method to set current to the first item in order according to @a type
  *
  * This does only work if T supports operator>(T&)!
  *
  * The return value is the number of current unless current is the last item
  * but still larger, in which case -1 is returned
  *
  * <B>IMPORTANT</B>: <I>Never</I> lock an item prior calling this method! Dead Lock ahead!
  *
  * Notes: searching for a larger/smaller name is case insensitive
**/
template<typename T>
int32_t TMemRing<T>::setCurrToOrder (base::eOrderType type, T *data, uint32_t aId, const char *aName) const
{
  bool searchData  = ((type == base::OT_LargerData) || (type == base::OT_SmallerData))  ? true : false;
  bool searchId    = ((type == base::OT_LargerId)   || (type == base::OT_SmallerId))    ? true : false;
  bool searchName  = ((type == base::OT_LargerName) || (type == base::OT_SmallerName))  ? true : false;
  bool asc         = ((type == base::OT_LargerData) || (type == base::OT_LargerId)
                   || (type == base::OT_LargerName)) ? true : false;
  bool smallerPrev = false;

  // Debugging asserts next
  assert ( ( !searchData || data )
         && "ERROR: setCurrToOrder needs data with OT_*Data order type!");
  assert ( ( !searchName || (aName && strlen(aName)) )
         && "ERROR: setCurrToOrder needs a valid name with OT_*Name order type!");
  assert ( (searchData || searchId || searchName)
         && "ERROR: setCurrToOrder needs a clear order type!");

  int32_t result = -2; // everything >-2 is a legal result

  if (root && root->getNext())
    {
      bool up  = true; // The normal direction

      // These values are needed to ensure that a result is found
      TItem<T> *xPrev = NULL, *xNext = NULL, *oldCurr = NULL;

      // Make sure current points to something
      if (!current)
        current = root;

      while (current && (result == -2) && (1 < dataCount))
        {
          // Lock current to get legal values
          ::pwx::Lock currLock(current);
          xPrev = current->getPrev();
          xNext = current->getNext();

          if (xPrev && xNext)
            {
              /* Work flow:
               *------------
               * Note: When searching in descending order the words "larger" and "smaller"
               *       have to be swapped.
               *
               * Case 1: current is not smaller
               *        a. xPrev is smaller or current is root
               *           --> result is currents number
               *        b. "up" and current is not root and xPrev is not smaller
               *           --> search direction is "down"
               * Case 2: current is smaller
               *        a. xPrev is smaller and xNext is root
               *           --> result is -1
               *        b. ("down" and xPrev is smaller) or current is root
               *           --> search direction is "up"
               */

              // First determine whether xPrev is smaller or not:
              // Note: We count equal size to be "smaller", or sorting would
              // swap equal but correctly ordered items until hell freezes...
              if ( /* --- smaller xPrev check for data search --- */
                   ( searchData && ( ( asc && (xPrev->getData() > *data) )
                                   ||(!asc && (*data > xPrev->getData()) ) ) )
                   /* --- smaller xPrev check for id search --- */
                 ||( searchId   && ( ( asc && (xPrev->getId() > aId) )
                                   ||(!asc && (aId > xPrev->getId()) ) ) )
                   /* --- smaller xPrev check for name search --- */
                 ||( searchName && ( ( asc && STRCGT(xPrev->getName(), aName) )
                                   ||(!asc && STRCGT(aName, xPrev->getName()) ) ) ) )
                smallerPrev = false;
              else
                smallerPrev = true;

              /* === Check Case 1: current is not smaller === */
              if ( /* ---Prefix condition for data search ---*/
                    ( searchData && ( ( asc && !(*data > current->getData()) )
                                    ||(!asc && !(current->getData() > *data) ) ) )
                    /* ---Prefix condition for id search ---*/
                  ||( searchId   && ( ( asc && !(aId > current->getId()) )
                                    ||(!asc && !(current->getId() > aId) ) ) )
                    /* ---Prefix condition for name search ---*/
                  ||( searchName && ( ( asc && !STRCGT(aName, current->getName()) )
                                    ||(!asc && !STRCGT(current->getName(), aName) ) ) ) )
                // Subcases a and be are checked independently
                {
                  // Check for Subcase a.
                  if ((current == root) || smallerPrev)
                    // current is the result!
                    result = current->getNr();
                  // Check for Subcase b.
                  else if (up && (current != root) && !smallerPrev)
                    // Search direction has to be changed
                    up = false;
                } // End of handling Case 1
              /* === Check Case 2: current _is_ smaller === */
              else
                {
                  // Check for Subcase a.
                  if ((xNext == root) && smallerPrev)
                    // result is -1, a new last item
                    result = -1;
                  // Check for Subcase b.
                  else if ((current == root) || (!up && smallerPrev))
                    // Search direction has to be changed
                    up = true;
                } // End of handling Case 1

              // Now advance if we do not have a result, yet
              if (-1 > result)
                {
                  oldCurr = current;
                  if (up)
                    current = xNext;
                  else
                    current = xPrev;
                }

            } // End of having both prev and next
          else
            {
              // This might mean that someone removed current while we waited for
              // the lock. Although this is a very stupid thing to do, this method
              // must not fail, so we revert to a sane state
              if (oldCurr && oldCurr->getNext())
                // I don't see how oldCurr could possibly become NULL, but better be safe here.
                current = oldCurr->getNext();
              else
                // Now this is plain stupid. What are they doing? *sigh* start from scratch then
                current = root;
            }

        } // End of while search not finished but possible

    } // End of having at least two items

  // Now if we still have no result, we just check root
  if (-2 == result )
    {
      current = root;
      // Here it does only matter if root is larger or not
      // The caller to this method has to handle a false result
      // which means that there is only one root that is smaller
      // or no items at all.
      if ( !current || (current && (
            /* --- Data search ---*/
            ( searchData && ( ( asc && !(*data > current->getData()) )
                            ||(!asc && !(current->getData() > *data) ) ) )
            /* --- Id search ---*/
          ||( searchId   && ( ( asc && !(aId > current->getId()) )
                            ||(!asc && !(current->getId() > aId) ) ) )
            /* --- Name search ---*/
          ||( searchName && ( ( asc && !STRCGT(aName, current->getName()) )
                            ||(!asc && !STRCGT(current->getName(), aName) ) ) ) ) ) )
        result = 0;
      else
        result = -1;
    }

  return result;
}


/** @brief internal method to set current to the first item in order according to @a type
  *
  * This does only work if T supports operator>(T&)!
  *
  * Note: this is just a const wrapper!
**/
template<typename T>
int32_t TMemRing<T>::setCurrToOrder (base::eOrderType type, const T *data, uint32_t aId, const char *aName) const
{
  return setCurrToOrder(type, const_cast<T *>(data), aId, aName);
}


/** @brief internal method to set current to the item which data has the value of @a data
  *
  * If the data could not be found the result is false, true otherwise
**/
template<typename T>
bool TMemRing<T>::setCurrToValue (T data) const
{
  bool result = false;
  if (!current || (current->getData() != data))
    {
      // What a pity, we need to act:
      if (root && (!current || (current->getData() != data)) )
        {
          current = root;
          while (current && (current->getData() != data) && (current->getNext() != root))
            current = current->getNext();
        }
    }

  // Were we successful?
  if (current && (current->getData() == data))
    result = true;

  return (result);
}


/** @brief internal method that does the sorting.
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  *
  * @param[in] type -1/1 by data desc/asc, -2/2 by id, -3/3 by name
  * @return number of sorting operations done
**/
template<typename T>
int32_t TMemRing<T>::sort (base::eOrderType type)
{
  int32_t unsorted = 0;

  if (dataCount > 2)
    {
      assert (root);

      TItem<T> * xNext = NULL;
      TItem<T> * source= NULL;
      int32_t    oldNr = 0;
      int32_t    newNr = 0;
      current          = root;
      xNext            = root->getNext();

      while (xNext != root)
        {
          xNext = current ? current->getNext() : NULL;
          oldNr = current->getNr();
          source= current;
          newNr = setCurrToOrder(type, current->getDataPointer(), current->getId(), current->getName());
          /* Important: There are two impossibilities:
           * a) An item should become the new predecessor of it's own next item and
           * b) An item should become the new successor of it's own previous item.
           * Both would mean that newNr != oldNr but no move can be done as it _is_
           * correctly positioned. */
          assert (((newNr >= 0) || (current->getNr() != (source->getNr() - 1)) )
                && "ERROR: setCurrToOrder has decided to place source after current while it already is! (sort)");
          assert (((newNr < 0) || (current->getNr() != (source->getNr() + 1)) )
                && "ERROR: setCurrToOrder has decided to place source in front of current while it already is! (sort)");
          if (newNr != oldNr)
            {
              // The trick is, that if current has the right position, setCurrToOrder will
              // do nothing and give the items number back.
              unsorted++;
              TItem<T> *newRoot = NULL;
              try
                {
                  if (newNr < 0)
                    newRoot = source->move(current, NULL, NULL);
                  else
                    newRoot = source->move(NULL, current, NULL);
                }
              PWX_THROW_FURTHER
              if (newRoot && (0 == newRoot->getNr()))
                root = newRoot;
            } // End of needing a number change
          // Move on...
          if (xNext != current->getPrev())
          // They can be equal if two items switch their place
            current = xNext;
        } // End of loop
    } // End of sorting
  else if (2 == dataCount)
    {
      // Special case with two items:
      current = root->getNext();
      if ( ( (base::OT_LargerName  == type) && STRCGT(root->getName(),   current->getName()) )
        || ( (base::OT_SmallerName == type) && STRCGT(current->getName(),root->getName()   ) )
        || ( (base::OT_LargerId    == type) && (root->getId()      >     current->getId()  ) )
        || ( (base::OT_SmallerId   == type) && (current->getId()   >     root->getId()     ) )
        || ( (base::OT_LargerData  == type) && (root->getData()    >     current->getData()) )
        || ( (base::OT_SmallerData == type) && (current->getData() >     root->getData()   ) ) )
        {
          swapItems(root, current);
          ++unsorted;
        }
      // Note: It does not matter if type is invalid here, the items simply won't get swapped then.
    }
  // No else, as sorting an empty container simply does nothing

  return unsorted;
}


/** @brief small internal methods to swap two items and take care of root
**/
template<typename T>
void TMemRing<T>::swapItems (TItem<T> * first, TItem<T> * second)
{
  assert(first && second && (first != second));

  TItem<T> *newRoot = first->swap(second);
  if (newRoot && (0 == newRoot->getNr()))
    root = newRoot;
  assert (root && (0 == root->getNr()) && "\nERROR: swapItems invalidated root!\n");
}

// --- stream operators ---
template<typename T>
::std::ifstream& operator>>(::std::istream &is, TMemRing<T> &cont)
{
  PWX_TRY(cont.load(is))
  PWX_THROW_FURTHER
  return(is);
}

template<typename T>
::std::ostream& operator<<(::std::ostream &os, TMemRing<T> &cont)
{
  PWX_TRY(cont.save(os))
  PWX_THROW_FURTHER
  return(os);
}

} // End of namespace mrf

} // End of namespace pwx

#endif // PWX_TMRTIMPL_H_INCLUDED
