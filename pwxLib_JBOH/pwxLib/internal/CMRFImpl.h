#pragma once
#ifndef PWX_CMRFIMPL_H_INCLUDED
#define PWX_CMRFIMPL_H_INCLUDED 1

/** @internal
  * @file CMRFImpl.h
  *
  * @brief Implementations for CMemRingFactory
  *
  * @see CMemRingFactory.h for documentation and history
  *
**/

#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
# error "Do not include CMRFImpl.h, include CMemRingFactory.h!"
#endif // Check for main file

namespace pwx {
namespace mrf {
// Note: The namespaces are documented in CMemRingFactory.h

//  ---  public methods  ---

/** @brief clear MRF of _all_ containers.
  *
  * Note: Everything _will_ be deleted!
**/
void CMemRingFactory::clear()
{
  containers.clear();
}


/** @brief return true if MRF does not manage any containers
**/
bool CMemRingFactory::empty() const
{
  return (containers.empty());
}


/** @brief check whether a container with a specific id exists
  * @param[in] aId The id to be searched for
  * @return true if such a container exists, false otherwise
**/
bool CMemRingFactory::exists (uint32_t aId) const
  {
    return (containers.getItem(aId) ? true : false);
  }


/** @brief check whether a container with a specific name exists
  * @param[in] aName The name to be searched for
  * @return true if such a container exists, false otherwise
**/
bool CMemRingFactory::exists (const char *aName) const
  {
    return (containers.getItem(aName) ? true : false);
  }


/** @brief check whether a container with a specific nr exists
  * @param[in] aNr The nr to be searched for
  * @return true if such a container exists, false otherwise
**/
bool CMemRingFactory::exists (int32_t aNr) const
  {
    return (containers.getItem(aNr) ? true : false);
  }


/** @brief get the id of a container with the given name
  *
  * This method returns the Id of the container with the given name.
  * If such a container does not exist, a pwx::mrf::nameNotFound
  * exception is thrown.
  *
  * @param[in] aName the name of the wanted container
  * @return the id of the corresponding container
**/
uint32_t CMemRingFactory::getId (const char *aName) const
{
  PWX_TRY(return (containers.getItemId(aName)))
  PWX_THROW_FURTHER
}


/** @brief get the id of a container with the given number
  *
  * This method returns the Id of the container with the given number.
  * If there are no containers, a pwx::mrf::nrOutOfRange
  * exception is thrown.
  *
  * @param[in] aNr the nr of the wanted container, normalized and wrapped if necessary
  * @return the id of the corresponding container
**/
uint32_t CMemRingFactory::getId (int32_t aNr) const
{
  PWX_TRY(return (containers.getItemId(aNr)))
  PWX_THROW_FURTHER
}


/** @brief get the name of a container with the given id
  *
  * This method returns the name of the container with the given id.
  * If there is no container with such an id, a pwx::mrf::idNotFound
  * exception is thrown.
  *
  * @param[in] aId the id of the wanted container
  * @return the name of the corresponding container
**/
const char * CMemRingFactory::getName (uint32_t aId) const
{
  PWX_TRY(return (containers.getItemName(aId)))
  PWX_THROW_FURTHER
}


/** @brief get the name of a container with the given nr
  *
  * This method returns the name of the container with the given number, counted from the
  * beginning if aNr is positive, counted from the end if negative, and wrapped if
  * too large.
  *
  * Due to this handling of the number, this method does not throw unless there are
  * no containers, which would result in a pwx::mrf::nrOutofRange exception to be thrown.
  *
  * @param[in] aNr the nr of the wanted container, normalized and wrapped if necessary
  * @return the name of the corresponding container
**/
const char * CMemRingFactory::getName (int32_t aNr) const
{
  PWX_TRY(return (containers.getItemName(aNr)))
  PWX_THROW_FURTHER
}


/** @brief get the nr of a container with the given id
  *
  * This method returns the number of the container with the given id.
  * If there is no container with such an id, a pwx::mrf::idNotFound
  * exception is thrown.
  *
  * @param[in] aId the id of the wanted container
  * @return the number of the corresponding container
**/
int32_t CMemRingFactory::getNr (uint32_t aId) const
{
  PWX_TRY(return (containers.getItemNr(aId)))
  PWX_THROW_FURTHER
}


/** @brief get the nr of a container with the given name
  *
  * This method returns the number of the container with the given name.
  * If there is no container with such a name, a pwx::mrf::nameNotFound
  * exception is thrown.
  *
  * @param[in] aName the name of the wanted container
  * @return the number of the corresponding container
**/
int32_t CMemRingFactory::getNr (const char *aName) const
{
  PWX_TRY(return (containers.getItemNr(aName)))
  PWX_THROW_FURTHER
}


/** @brief set the Id of a container
  *
  * Change the Id of the container with the given number. If there are no
  * containers, a pwx::mrf::nrOutOfRange exception is thrown.
  * If the insertion into the id map fails, a pwx::mrf::noIdMapAvailable
  * exceptin is thrown.
  *
  * The ids of the containers must be unique. This method takes care of the
  * uniqueness of the ids and will change the given value if necessary.
  * For this reason the finally set id is given back for you to check.
  *
  * @param[in] aNr The number of the container, wrapped and normalized if necessary.
  * @param[in] aId The wanted new id of the container, changed if necessary.
  * @return the final id of the container
**/
uint32_t CMemRingFactory::setId (int32_t aNr, uint32_t aId)
{
  PWX_TRY(return (containers.setItemId(aNr, aId)))
  PWX_THROW_FURTHER
}


/** @brief set the name of a container
  *
  * Change the name of the container with the given number. If there are no
  * containers, a pwx::mrf::nrOutOfRange exception is thrown.
  * If the insertion into the name map fails, a pwx::mrf::noNameMapAvaiable
  * exceptin is thrown.
  *
  * The names of the containers must be unique. This method takes care of the
  * uniqueness of the names and will change the given value if necessary.
  * For this reason the finally set name is given back for you to check.
  *
  * @param[in] aNr The number of the container, wrapped and normalized if necessary.
  * @param[in] aName The wanted new name of the container, changed if necessary.
  * @return the final name of the container
**/
const char * CMemRingFactory::setName (int32_t aNr, const char *aName)
{
  PWX_TRY(return (containers.setItemName(aNr, aName)))
  PWX_THROW_FURTHER
}


/** @brief return the number of handled containers
  *
  * @return the number of handled containers
**/
int32_t CMemRingFactory::size () const
{
  return (containers.size());
}



// --- public templated members ---

/** @brief add data to a container
  *
  * This method adds the given data with optional id and name to
  * the first container found that can handle the container. If such
  * a container does not exist, a new one will be created.
  *
  * If the container uses its id or name map, the given id and/or name
  * are ensured to be unique and thus might get changed.
  *
  * If you do not provide a valid pointer to data (aka NULL), the first
  * container fit for saving the type, or the newly created container
  * are given back nevertheless.
  *
  * If it is not possible to create a new container, a pwx::mrf::cantCreateContainer
  * exception is thrown.
  * If it is not possible to create a new item, a pwx::mrf::cantCreateItem exception
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
  * @param[in]  data pointer to the data to be added
  * @param[in] aId an optional id for the new data item.
  * @param[in] aName an optional name for the new data item.
  * @return A pointer to the container @a data got stored in or would be stored if @a data is NULL
**/
template<typename T>
TMemRing<T> * CMemRingFactory::add (T *data, uint32_t aId, const char *aName)
{
  TMemRing<T> * result = find<T>(static_cast<T *>(NULL));
  /* Note: The NULL has to be casted, or 32bit compilation will take it
   * as a value, using the wrong find() method. It is strange and I have
   * absolutely no idea why this is so. But it is... */

  if (data && result)
    {
      // insert the data into the found container
      PWX_TRY(result->insert(data, -1, aId, aName))
      PWX_THROW_FURTHER
    }
  else if (!result)
    {
      // We need to create a new container:
      PWX_TRY(result = newCont(data, aId, aName))
      PWX_THROW_FURTHER
    }

  return (result);
}


/** @brief add data by value to a container
  *
  * This method is a convenience wrapper for add(T *data, ...), see there for
  * further information.
  *
  * @param[in] value the value to be added
  * @param[in] aId an optional id for the new data item.
  * @param[in] aName an optional name for the new data item.
  * @return A pointer to the container @a value got stored in
**/
template<typename T>
TMemRing<T> * CMemRingFactory::add (T value, uint32_t aId, const char *aName)
{
  try
    {
      T * xData = new T(value); // If this fails it's not our fault
      PWX_TRY(return(add (xData, aId, aName)))
      PWX_THROW_FURTHER
    }
  catch (::std::bad_alloc &e)
    {
      PWX_THROW(cantCreateItem, e.what(), "Add() tried to turn a value into *data")
    }
}


/** @brief Create a new container
  *
  * This method creates a new container with the given @a data as the first stored item
  * with optional id and name.
  *
  * If the container uses its id or name map, the given id and/or name
  * are ensured to be unique and thus might get changed.
  *
  * if @a data is NULL, an empty container is created.
  *
  * If it is not possible to create a new container, a pwx::mrf::cantCreateContainer
  * exception is thrown.
  * If it is not possible to create a new item, a pwx::mrf::cantCreateItem exception
  * is thrown.
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * @param[in]  data pointer to the data to be added
  * @param[in] aId an optional id for the new data item.
  * @param[in] aName an optional name for the new data item.
  * @return A pointer to the container @a data got stored in.
**/
template<typename T>
TMemRing<T> * CMemRingFactory::create (T *data, uint32_t aId, const char *aName)
{
  // create the new container
  TMemRing<T> * result = NULL;
  PWX_TRY(result = newCont(data, aId, aName))
  PWX_THROW_FURTHER

  return (result);
}


/** @brief Create a new container by value
  *
  * This method is a convenience wrapper for create (T *data, ...). See there for
  * further information.
  *
  * @param[in]  value value for which an item shall be added
  * @param[in] aId an optional id for the new data item.
  * @param[in] aName an optional name for the new data item.
  * @return A pointer to the container @a value got stored in
**/
template<typename T>
TMemRing<T> * CMemRingFactory::create (T value, uint32_t aId, const char *aName)
{
  try
    {
      T * xData = new T(value); // If this fails it's not our fault
      PWX_TRY(return(create (xData, aId, aName)))
      PWX_THROW_FURTHER
    }
  catch (::std::bad_alloc &e)
    {
      PWX_THROW(cantCreateItem, e.what(), "Create tried to turn a value into *data")
    }
}


/** @brief delete a container
  *
  * Deletes a container handled by MRF and returns the number of containers
  * erased. Please be aware that the container and all its containing
  * items _will_ be deleted! But if any item holds data that is known to
  * be saved in another container, too, this data will not be deleted. This
  * means that you won't invalidate any of your items accidently as long as
  * you use MRF to organize your containers.
  *
  * If the given @a container is not stored in MRF, the method will return
  * 0 and will _not_ delete @a container!
  *
  * @param[in]  container the container to be erased
  * @return the number of containers erased
 */
template<typename T>
int32_t CMemRingFactory::erase (TMemRing<T> * container)
{
  return (containers.erase(container));
}


/** @brief delete containers by held data
  *
  * This method deletes all containers that are handled by MRF and hold at
  * least one item pointing to @a data. The number of erased containers
  * is given back. Please be aware that the containers and all its containing
  * items _will_ be deleted! But if any item holds data that is known to
  * be saved in another container, too, this data will not be deleted. This
  * means that you won't invalidate any of you items accidently as long as
  * you use MRF to organize your containers, but have a known copy outside
  * of MRF.
  *
  * If the given @a data is not stored in any container in MRF, the method will
  * return 0.
  *
  * @param[in]  data the data to search for
  * @return the number of containers erased
 */
template<typename T>
int32_t CMemRingFactory::eraseByData(T *data)
{
  int32_t erased = 0;

  if (data)
    {
      while (TMemRing<T> * candidate = find(data))
        erased += erase(candidate);
      // Note to self: This is why I _love_ OOP! ;)
    }

  return (erased);
}


/** @brief find the first container that stores @a data
  *
  * This method will return a pointer to the first container that stores
  * @a data. if @a data is NULL, it will return the first container that
  * stores the givene datatype. If no such container exists, the method
  * will return NULL.
  *
  * Note: If you want to use this method with a call like find<T>(NULL),
  * please make sure to actually *cast* the NULL to a pointer to T like
  * this: find<T>(static_cast<T *>(NULL)), or the method actually called
  * will be find<T>(T value) instead of this. This would happen for
  * instance, on a gcc-4.4 compiler when compiling 32bit. The same gcc
  * does things right when compiling 64bit with a simple NULL, but for
  * 32bit the cast is mandatory!
  *
  * @param[in]  data The data to search for
  * @return A pointer to the first container storing @a data or NULL if no container holds @a data
**/
template<typename T>
TMemRing<T> * CMemRingFactory::find (T *data)
{
  TMemRing<T> * result   = NULL;
  bool          isFound  = false;
  int32_t       currCont = 0;
  int32_t       maxCont  = containers.size();

  // Try to find available container:
  while (!isFound && (currCont < maxCont))
    {
      result = dispatch<T>(containers[currCont]);
      if ( result && (!data || result->isIn(data)) )
        isFound = true;
      else
        ++currCont;
    }

  return (isFound ? result : NULL);
}


/** @brief find the first container that stores @a value
  *
  * This method will return a pointer to the first container that stores
  * @a value. If no such container exists, it will return NULL.
  *
  * @param[in]  value The value to search for
  * @return A pointer to the first container storing @a value or NULL if no container holds this value
**/
template<typename T>
TMemRing<T> * CMemRingFactory::find (T value)
{
  TMemRing<T> * result   = NULL;
  bool          isFound  = false;
  int32_t       currCont = 0;
  int32_t       maxCont  = containers.size();

  // Try to find available container:
  while (!isFound && (currCont < maxCont))
    {
      if ( (result = dispatch<T>(containers[currCont]))
           && result->isValueIn(value) )
        isFound = true;
      else
        ++currCont;
    }

  return (isFound ? result : NULL);
}


/** @brief find the first container that stores the given type and is empty
  *
  * This method will return a pointer to the first container that stores
  * the given type and does not contain any items. If no such container
  * exists, the method will return NULL.
  *
  * @return A pointer to the first empty container storing the given type or NULL if no such contaienr could be found
**/
template<typename T>
TMemRing<T> * CMemRingFactory::findEmpty ()
{
  TMemRing<T> * result  = NULL;
  bool         isFound  = false;
  int32_t      currCont = 0;
  int32_t      maxCont  = containers.size();

  // Try to find available container:
  while (!isFound && (currCont < maxCont))
    {
      result = dispatch<T>(containers[currCont]);
      if ( result && result->empty() )
        isFound = true;
      else
        ++currCont;
    }

  return (isFound ? result : NULL);
}


/** @brief get the first container that stores @a data
  *
  * This method will return a pointer to the first container that stores
  * @a data. If @a data is NULL, it will return the first container that
  * stores the same datatype. If no such container exists, it will be
  * created.
  *
  * If it is not possible to create a new container, a pwx::mrf::cantCreateContainer
  * exception is thrown.
  * If it is not possible to create a new item, a pwx::mrf::cantCreateItem exception
  * is thrown.
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * @param[in]  data The data to search for
  * @return A pointer to the first container storing @a data
**/
template<typename T>
TMemRing<T> * CMemRingFactory::get (T *data)
{
  TMemRing<T> * result = find(data);
  if (NULL == result)
    {
      // Alright, we do it ourselves...
      PWX_TRY(result = newCont(data, 0, NULL))
      PWX_THROW_FURTHER
    }
  return (result);
}


/** @brief get the first container that stores @a value
  *
  * This method will return a pointer to the first container that stores
  * @a value. If no such container exists, it will be created.
  *
  * This is a convenience wrapper for get(T * data), see there for more
  * information.
  *
  * @param[in] value The value to search for
  * @return A pointer to the first container storing @a value
**/
template<typename T>
TMemRing<T> * CMemRingFactory::get (T value)
{
  TMemRing<T> * result = find(value);
  if (NULL == result)
    {
      // Alright, we do it ourselves...
      try
        {
          T * xData = new T(value); // If this fails it's not our fault
          PWX_TRY(result = newCont(xData, 0, NULL))
          PWX_THROW_FURTHER
        }
      catch (::std::bad_alloc &e)
        {
          PWX_THROW(cantCreateItem, e.what(), "get tried to turn a value into T*")
        }
    }
  return (result);
}


/** @brief get the id of a specific container
  *
  * This method returns the current id of a specific container. If the
  * given @a container is not stored by MRF, a pwx::mrf::dataNotFound
  * exception is thrown.
  *
  * @param[in]  container pointer to the container whose id shall be retrieved
  * @return the current id of the @a container
**/
template<typename T>
uint32_t CMemRingFactory::getId (const TMemRing<T> *container) const
{
  PWX_TRY(return (containers.getDataId(container)))
  PWX_THROW_FURTHER
}


/** @brief get the name of a specific container
  *
  * This method returns the current name of a specific container. If the
  * given @a container is not stored by MRF, a pwx::mrf::dataNotFound
  * exception is thrown.
  *
  * @param[in]  container pointer to the container whose name shall be retrieved
  * @return the current name of the @a container
**/
template<typename T>
const char * CMemRingFactory::getName (const TMemRing<T> *container) const
{
  PWX_TRY(return (containers.getDataName(container)))
  PWX_THROW_FURTHER
}


/** @brief get the number of a specific container
  *
  * This method returns the current number of a specific container. If the
  * given @a container is not stored by MRF, a pwx::mrf::dataNotFound exception
  * is thrown.
  *
  * @param[in]  container pointer to the container whose number shall be retrieved
  * @return the current number of the @a container
**/
template<typename T>
int32_t CMemRingFactory::getNr (const TMemRing<T> *container) const
{
  PWX_TRY(return (containers.getDataNr(container)))
  PWX_THROW_FURTHER
}


/** @brief get the refcount of a specific item
  *
  * This method tries to find an item that points to @a data in all managed
  * containers that stores the same type. If such an item can be found, the
  * method returns its reference count, otherwise the method returns 0.
  *
  * @param[in]  data pointer to be searched for
  * @return the reference count upon this data or 0, if it is nowhere saved in any container
**/
template<typename T>
int32_t CMemRingFactory::getRefCount(T *data)
{
  int32_t result = 0;

  if (data)
    {
      TMemRing<T> * cont = find(data);
      if (cont)
        result = cont->getDataRefCount(data);
    }

  return (result);
}


/** @brief load a specific Container
  *
  * This method searches for the first empty container that saves
  * the specified type. If no such container exists, it will create
  * one. Then the next container data from @a is will be loaded
  * into the container. The required data must be found at the
  * current position, or will be searched for from the current
  * position on if @a search is set to true.
  *
  * If the allocation of a name string fails, a
  * pwx::mrf::allocFailed exception is thrown.
  * If loading fails on a value, a pwx::mrf::loadFailed
  * exception is thrown.
  * If is is not possible to create a new container, a
  * pwx::mrf::cantCreateContainer exception is thrown.
  * If is is not possible to create a new item, a
  * pwx::mrf::cantCreateItem exception is thrown.
  * If an idMap shall be used but the insertion fails,
  * a pwx::mrf::noIdMapAvailable exception is thrown.
  * If a nameMap shall be used but the insertion fails,
  * a pwx::mrf::noNameMapAvailable exception is thrown.
  *
  * @param[in] is an open ifstream
  * @param[in] search the next starting container will be searched if set to true
**/
template<typename T>
void CMemRingFactory::load(::std::ifstream &is, bool search)
{
  if (is.good())
    {
      TMemRing<T> * cont = findEmpty<T>();
      if (NULL == cont)
        {
          // We have to create a container!
          PWX_TRY(cont = newCont<T>(NULL, 0u, NULL))
          PWX_THROW_FURTHER
        }
      // We do not need to load anything, the container can do for itself.
      PWX_TRY(cont->load(is, search))
      PWX_THROW_FURTHER
    }
}


/** @brief This method merges all items from @a aSource into @a aDest
  *
  * The source container will be empty afterwards, and destroyed unless
  * autodestruct is set to false.
  * The items will be appended, thus receiving new numbers.
  * Ids that would be non-unique but have to will be changed. The same
  * applies to names.
  *
  * If it is not possible to create a new item, a pwx::mrf::cantCreateItem exception
  * is thrown.
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * @param[in]  aDest the container receiving the sources items
  * @param[in]  aSource the container that shall be merged with @a aDest
  * @param[in] autodestruct if set to true, the source container will be deleted after the merge
  * @return the total item count after the merge of @a aDest
**/
template<typename T>
int32_t CMemRingFactory::merge (TMemRing<T> *aDest, TMemRing<T> *aSource, bool autodestruct)
{
  int32_t result = 0;
  if (aDest)
    {
      // Now go on, but we do the destruction ourselves if necessary:
      PWX_TRY(result = aDest->mergeWith(aSource, false))
      PWX_THROW_FURTHER
      if (autodestruct)
        containers.erase(aSource);
    }

  return (result);
}


/** @brief register a container with MRF
  *
  * This method adds a container, that has been built outside of MRF.
  * MRF will take over the handling, and will take care of its
  * destruction when MRF is destroyed, normaly at the end of the
  * program. If @a itemsOnly is set to true, the container will not be
  * taken over, but all of its items will be registered with MRF.
  * The latter means, that the method looks for copies of the items,
  * and ensures that they know each other.
  *
  * Please do not add a pointer to a container instance that resides
  * on a local stack with this method, or the whole thing will blow
  * up! You have been warned.
  *
  * If it is not possible to create a new item, a pwx::mrf::cantCreateItem exception
  * is thrown.
  * If an idMap shall be used but the insertion fails, a pwx::mrf::noIdMapAvailable
  * exception is thrown.
  * If a nameMap shall be used but the insertion fails, a pwx::mrf::noNameMapAvailable
  * exception is thrown.
  *
  * @param[in]  container a pointer to the container that is to be added.
  * @param[in] itemsOnly if set to true, the container itself will not be taken over by MRF
**/
template<typename T>
void CMemRingFactory::registCont (TMemRing<T> *container, bool itemsOnly)
{
  if (container && !containers.isIn(container))
    {
      for (int32_t i = 0; i < container->size(); ++i)
        registItem(container->getItem(i));
      if (!itemsOnly)
        {
          PWX_TRY(containers.push(container, 0, container->getName()))
          PWX_THROW_FURTHER
        }
    }
}


/** @brief register an item with MRF
  *
  * This method is normally called by TMemRing<> containers only to ensure
  * that all items know their copies. But if you copy items from an
  * MRF-handled container to the outside, and want to keep it even if
  * the container you copied from goes away, you can tell all copies in
  * MRF-handled containers about its present with this method.
  *
  * @param[in]  item a pointer to the item that is to be registered with MRF-containers.
**/
template<typename T>
void CMemRingFactory::registItem (TItem<T> *item)
{
  if (item)
    {
      T * xData = item->getDataPointer();
      TMemRing<T> * cont = find(xData);
      if (cont)
        cont->getItem(xData)->addToCopies(item, true);
      // This is sufficient, because addToCopies will cascade through all other copies
    }
}

// --- private templated methods and dispatchers ---

/** @brief a dispatcher, which will most probably have to dynamic_cast.
**/
template<typename Ti, typename Tc>
TMemRing<Ti> * CMemRingFactory::dispatch (Tc * cont)
{
  TMemRing<Ti> * result = NULL;

  if (isSameType(Tc, TMemRing<Ti>))
    // Although this will never be true right now, it does no harm as this will be
    // optimized out, but should stay here for future development.
    result = static_cast<TMemRing<Ti>*>(cont);
  else
    result = dynamic_cast<TMemRing<Ti>*>(cont); // If this fails, result stays NULL

  return (result);
}


/** @brief internal method to create a new container
**/
template<typename T>
TMemRing<T> * CMemRingFactory::newCont (T *data, uint32_t aId, const char *aName)
{
  TMemRing<T> * result = NULL;
  PWX_TRY(result = new TMemRing<T>(this, newUsesIdMap, newUsesNameMap, NULL))
  PWX_THROW_FURTHER
  catch (::std::bad_alloc &e)
    {
      PWX_THROW(cantCreateContainer, e.what(), "newCont() tried to create a new Container")
    }

  // Now add our first item if needed:
  if (data)
    {
      PWX_TRY(result->insert(data, 0, aId, aName))
      PWX_THROW_FURTHER
    }
  result->setName(containers.getItemName(-1)); // Clone container name

  return (result);
}

} // End of namespace mrf

} // End of namespace pwx

#endif // PWX_CMRFIMPL_H_INCLUDED
