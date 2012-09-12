#pragma once
#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
#define PWX_CMEMRINGFACTORY_H_INCLUDED 1

/** @file CMemRingFactory.h
  * ------------------------
  *
  * @brief Declaration of CMemRingFactory
  *
  * <B>Important</B>: If you plan to use MRF in a multi-threaded environment, please make sure
  * the macro "PWX_THREADS" is defined <I>before</I> including this file!
  *
  * (c) 2007-2012 Sven Eden, PrydeWorX
  * @author Sven Eden, PrydeWorX - Hamburg, Germany
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
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.0.1     2010-09-27  sed, PrydeWorX  Initial Release
  * 0.0.2     2010-11-04  sed, PrydeWorX  First version with all methods tested and working
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.0.1   2011-03-06  sed, PrydeWorX  Welcome to 2011 !
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Some restructuring for the documentation, some corrections and version bump to
  *                                       the new pwxLib release.
  * 0.7.2.1   2011-04-20  sed, PrydeWorX  Changed to use the new simplified PWX_THROW_FURTHER and added naming to mrf
  *                                       exceptions.
  * 0.7.2.2   2011-04-29  sed, PrydeWorX  added delItem() methods to TMemRing to get rid of data easily. Needed by CFH.
  * 0.7.2.3   2011-05-04  sed, PrydeWorX  added exists() methods to easily find out whether a specific container exists.
  * 0.7.2.4   2011-05-23  sed, PrydeWorX  added exists() methods to TMemRing, to find out whether a specific item exists.
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0.1   2011-08-02  sed, PrydeWorX  Added missing PWX_INLINE to the MRF exceptions, CMemRing and CItem dtors
  * 0.8.0.2   2011-08-03  sed, PrydeWorX  Optimized insertion of items to gain some speed.
  *                                       Added TMemRing<T>::disableTracking() to optionally disable reference tracking
  *                                       per Container to gain (lots of) speed and added big fat warnings about it.
  * 0.8.0.3   2011-08-06  sed, PrydeWorX  Added the special cases of the previous and next items to the retrieval of
  *                                       items and data to speed up the performance of loops.
  * 0.8.0.4   2011-08-07  sed, PrydeWorX  Added TMemRing<T>::add_sorted() and TMemRing<T>::sort*_once() to allow for
  *                                       a sorted state after adding tons of items and to make sorting interruptible.
  * 0.8.0.5   2011-08-08  sed, PrydeWorX  Added TMemRingInterface<T> to allow multi-threaded read-only access, and
  *                                       TMemRingInserter<T> for multi-threaded adding of items
  * 0.8.0.6   2011-08-11  sed, PrydeWorX  Unified Interfaces to MRInterface<T> and finished thread safety measures
  * 0.8.0.7   2011-08-12  sed, PrydeWorX  Changed the renumbering of a memory ring form a recursive to a direct method.
  *                                       The recursive approach busted maximum recursion depth (mcount) on large containers.
  *                                       Optimized searching items by index number a bit.
  * 0.8.0.8   2011-08-15  sed, PrydeWorX  Rewrote interface getters to use the real id and name map instead of the shadowed
  *                                       (and empty!) ones.
  * 0.8.0.9   2011-08-19  sed, PrydeWorX  Fixed the workflow of root item change detection.
  * 0.8.0.10  2011-08-19  sed, PrydeWorX  Applied new merge sort to the sorting by id and name.
  * 0.8.0.11  2011-08-23  sed, PrydeWorX  Added some thread safety measures to the setCurrTo*() methods.
  * 0.8.0.12  2011-08-26  sed, PrydeWorX  Unified setCurrTo*() methods into setCurrToOrder().
  * 0.8.0.13  2011-09-01  sed, PrydeWorX  optimized and fixed setCurrToOrder(). Much smaller and more speedy now.
  * 0.8.0.14  2011-09-02  sed, PrydeWorX  fixed add_sorted and added a safety check and "strayItem" exception to
  *                                       TItem<T>::insert().
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.1.1   2011-10-06  sed, PrydeWorX  The interfaces now reassigns the currently handled item if the container size
  *                                       changed before the interface is reattached.
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3.1   2012-01-05  sed, PrydeWorX  Eventually cleaned up some old code that made cppcheck to croak loudly and wasn't
  *                                       needed anyway.
  * 0.8.5     2012-03-01  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5.1   2012-03-20  sed, PrydeWorX  fixed a memory leak that could show up when the name map is used
  * 0.8.5.2   2012-03-22  sed, PrydeWorX  fixed some Effective C++ issues
  * 0.8.5.3   2012-04-03  sed, PrydeWorX  fixed a doxygen issue with the public and private sort() variants of MRInterface
  * 0.8.5.4   2012-04-13  sed, PrydeWorX  substituted all (unsafe) sprintf calls with the new pwx_snprintf wrapper
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

// Initial customization point is the definition of whether MRF starts with
// or without an idMap or nameMap. .

#if !defined(PWX_MRF_USEIDMAP)
//! Define that we use an ID Map
#  define PWX_MRF_USEIDMAP true
#endif // PWX_MRF_USEIDMAP

#if !defined(PWX_MRF_USENAMEMAP)
//! Define that we use a name Map
#  define PWX_MRF_USENAMEMAP true
#endif // PWX_MRF_USENAMEMAP

#include "pwxLib/tools/DefaultDefines.h"
#include "pwxLib/tools/StreamHelpers.h"

// We need to know our stuff:
#ifndef PWX_TMRINTERFACE_H_INCLUDED
#  include "pwxLib/tools/MRInterface.h"
#else
#  include "pwxLib/internal/TMemRing.h"
#endif


/** @namespace pwx
  *
  * @brief Namespace for everything PrydeWorX related
  *
  * All PrydeWorX projects reside in this namespace and it's sub-namespaces.
**/
namespace pwx
{

/** @namespace pwx::mrf
  *
  * @brief Namespace for CMemRingFactory.
  *
  * Everything concerning MRF is implemented here or in one of the subnamespaces.
  * @see pwx::mrf::base for base classes
**/
namespace mrf
{

/** @class CMemRingFactory
  *
  * @brief factory class to generate pwxMemRing Containers and maintain them
  *
  * <B>Important</B>: If you plan to use MRF in a multi-threaded environment, please make sure
  * the macro "PWX_THREADS" is defined <I>before</I> including CMemRingFactory.h!
  *
  * This is a factory class which is responsible to create and maintain containers.
  * As long as containers are handled by this object, they are automatically cleaned
  * up on destruction of this object, which is normally the end of the program
  * utilizing it.
  *
  * There are four ways of getting a container by their held data:
  *  -# create(data, ...) will explicitedly create a container with the given
  * data saved as the first item.
  *  -# add(data, ...) will add a new item with the given data to the first
  * container available that holds this type of data. If no such container exists,
  * a new one will be created.
  *  -# get(data) will return the first container in which the given data is
  * currently stored. If no such container exists, a new container will be created.
  *  -# find(data) will return the first container in which the given data is
  * currently stored. If no such container exists, the method returns NULL.
  *
  * All four methods return a pointer to the container for further usage. The type
  * of the container is pwx::mrf::TMemRing<T>, with T being the type of your data.
  *
  * A note on multi-threaded access:
  * If you plan to use threads to work with containers and their data, you should
  * take a look at pwx::mrf::MRInterface<T> (pwxLib/tools/MRInterface.h), which is a
  * thread safe interface to pwx::mrf::TMemRing<T>. To enable thread safety your program
  * ought to be compiled with -DPWX_THREADS, or you have to define PWX_THREADS prior
  * including the first header from pwxLib.
  *
  * A note on the stored data:
  * Although there are wrapper methods to be used with values, you really should insert
  * data as pointers only, created with their respective new operator. The reason is,
  * that stored data is deleted with their delete operator. This means that the system
  * is not aware of values.
  * If you add values, they are used as arguments to the types new operator. So while
  * pwxMemRing is not meant to store simple types like int or float, it can be used to
  * do so.
  * Important: Only create(), add(), get() and find() have wrappers for values.
  *
  * Internally the containers are handled like items in a container themselves, so
  * you can get containers by their number, Id or name. Unlike the generated containers,
  * the Id and name are not optional and handled by this object. But you can change the
  * name of a container if you like. But you can get a pointer, the number, id or name
  * of a container with the help of various getX() methods.
  *
  * All containers can utilize an id map and/or a name map. This makes it possible
  * to identify a stored item by either its id and/or name. As a convenience MRF can
  * be told how to set the usage of those maps, they are turned on by default, on
  * creation. You can do this by defining PWX_MRF_USEIDMAP and/or PWX_MRF_USENAMEMAP
  * as being "false" prior including this file.
  *
  * Although the number of an item unsigned, the methods using the number handle signed
  * numbers, which then are normalized and wrapped. Those methods will only fail, by
  * throwing an exception, if the container holds no items.
  *
  * <B>Important note about speed</B>:<BR />
  * Whenever a pointer is added, all containers of the same type are searched for this
  * pointer to track the number of references. With <I>small</I> containers this is
  * no problem and multiple insertions of the same pointer are safe.<BR />
  * However if you want to add larger amounts of items to a container, let's say
  * 5,000+, adding items becomes more and more slower. For this reason you can disable
  * the reference tracking with the containers TMemRing<T>::disableTracking() method. It can
  * <B>only be disabled</B> and never re-enabled again! Further data then is deleted
  * no matter how many other items point to it, too, when its item is removed! So please
  * make sure to store each pointer only once.<BR />
  * Speed tests with 20000 Integers on an Intel Core i7, 2.8GHz, single threaded show
  * the following:
  *
  *  -# Maps enabled,  tracking enabled  : 20000 done in 2500 ms
  *  -# Maps disabled, tracking enabled  : 20000 done in 2400 ms
  *  -# Maps enabled,  tracking disabled : 20000 done in 50 ms
  *  -# Maps disabled, tracking disabled : 20000 done in 20 ms
  *
  * Thus the maps of ID and Names are no problem, but the tracking really snails it down!
  *
  * If you do not want a static instance but create your own, you can define
  * PWX_NO_MRF_INSTANCE prior including this file and build you own. Besides that
  * you can use TMemRing directly, but have to maintain your containers yourself.
**/
class PWX_VISIBLE CMemRingFactory
{
public:
  //  ---  methods  ---
  PWX_INLINE void                   clear     ();
  PWX_INLINE bool                   empty     ()                     const;
  PWX_INLINE bool                   exists    (uint32_t aId)         const PWX_WARNUNUSED;
  PWX_INLINE bool                   exists    (const char *aName)    const PWX_WARNUNUSED;
  PWX_INLINE bool                   exists    (int32_t aNr)          const PWX_WARNUNUSED;
  PWX_INLINE uint32_t               getId     (const char *aName)    const PWX_WARNUNUSED;
  PWX_INLINE uint32_t               getId     (int32_t aNr)          const PWX_WARNUNUSED;
  PWX_INLINE const char           * getName   (uint32_t aId)         const PWX_WARNUNUSED;
  PWX_INLINE const char           * getName   (int32_t aNr)          const PWX_WARNUNUSED;
  PWX_INLINE int32_t                getNr     (uint32_t aId)         const PWX_WARNUNUSED;
  PWX_INLINE int32_t                getNr     (const char *aName)    const PWX_WARNUNUSED;
  PWX_INLINE uint32_t               setId     (int32_t aNr, uint32_t aId);
  PWX_INLINE const char           * setName   (int32_t aNr, const char *aName);
  PWX_INLINE int32_t                size      ()                     const PWX_WARNUNUSED;

  // --- templated methods ---
  template<typename T> PWX_INLINE TMemRing<T> * add        (T *data, uint32_t aId=0, const char *aName=NULL) PWX_WARNUNUSED;
  template<typename T> PWX_INLINE TMemRing<T> * add        (T value, uint32_t aId=0, const char *aName=NULL) PWX_WARNUNUSED;
  template<typename T> PWX_INLINE TMemRing<T> * create     (T *data, uint32_t aId=0, const char *aName=NULL) PWX_WARNUNUSED;
  template<typename T> PWX_INLINE TMemRing<T> * create     (T value, uint32_t aId=0, const char *aName=NULL) PWX_WARNUNUSED;
  template<typename T> PWX_INLINE int32_t       erase      (TMemRing<T> *container)             PWX_WARNUNUSED;
  template<typename T> PWX_INLINE int32_t       eraseByData(T *data)                            PWX_WARNUNUSED;
  template<typename T> PWX_INLINE TMemRing<T> * find       (T *data)                            PWX_WARNUNUSED;
  template<typename T> PWX_INLINE TMemRing<T> * find       (T value)                            PWX_WARNUNUSED;
  template<typename T> PWX_INLINE TMemRing<T> * findEmpty  ()                                   PWX_WARNUNUSED;
  template<typename T> PWX_INLINE TMemRing<T> * get        (T *data)                            PWX_WARNUNUSED;
  template<typename T> PWX_INLINE TMemRing<T> * get        (T value)                            PWX_WARNUNUSED;
  template<typename T> PWX_INLINE uint32_t      getId      (const TMemRing<T> *container) const PWX_WARNUNUSED;
  template<typename T> PWX_INLINE const char  * getName    (const TMemRing<T> *container) const PWX_WARNUNUSED;
  template<typename T> PWX_INLINE int32_t       getNr      (const TMemRing<T> *container) const PWX_WARNUNUSED;
  template<typename T> PWX_INLINE int32_t       getRefCount(T *data)                            PWX_WARNUNUSED;
  template<typename T> PWX_INLINE void          load       (::std::ifstream &is, bool search = false);
  template<typename T> PWX_INLINE int32_t       merge      (TMemRing<T> *aDest, TMemRing<T> *aSource, bool autodestruct=true);
  template<typename T> PWX_INLINE void          registCont (TMemRing<T> *container, bool itemsOnly);
  template<typename T> PWX_INLINE void          registItem (TItem<T>    *item);

private:
  // --- private members ---
  TMemRing<base::CMemRing> containers;
  bool newUsesIdMap;
  bool newUsesNameMap;

  // --- private templated methods and dispatchers ---
  template<typename Ti, typename Tc> PWX_INLINE TMemRing<Ti> * dispatch (Tc             * cont) PWX_WARNUNUSED;
  template<typename T>               PWX_INLINE TMemRing<T>  * newCont  (T *data, uint32_t aId, const char *aName)
                                                                        PWX_WARNUNUSED;

public:
  // ctor/dtor need to be inlined directly for the linker to not wreak havoc on multiple inclusion
  /** @brief Default constructor.
   *
   * @param[in] useIdMap if set to true (default), new containers will utilize their id map.
   * @param[in] useNameMap if set to true (default), new containers will utilize their name map.
   */
  CMemRingFactory (bool useIdMap, bool useNameMap):containers(NULL, "MRF Main Container"),
    newUsesIdMap(useIdMap),newUsesNameMap(useNameMap)
  { /* odd but true... nothing to be done here! */ }

  /** @brief default dtor. It will delete all containers.
   */
  ~CMemRingFactory ()
  {
    /* nothing to be done here */
  }
};

} // End namespace mrf

#ifndef PWX_NO_MRF_INSTANCE
static mrf::CMemRingFactory MRF(PWX_MRF_USEIDMAP, PWX_MRF_USENAMEMAP); //!< Static instance of CMemRingFactory to be used
#endif // PWX_NO_MRF_INSTANCE

} // End of pwx namespace

/* --- Implementation: --- */
#include "pwxLib/internal/CMRFImpl.h"

/* --- remaining method from TMemRing: --- */
#include "pwxLib/internal/TMRT_insert.h"

#endif // PWX_CMEMRINGFACTORY_H_INCLUDED
