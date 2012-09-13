#pragma once
#ifndef PWX_TMEMRING_H_INCLUDED
#define PWX_TMEMRING_H_INCLUDED 1

/** @internal
  * @file TMemRing.h
  * ------------------------
  *
  * @brief Declaration of TMemRing
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
  * History and ChangeLog are maintained in @see CMemRingFactory.h !
**/

#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
# error "Do not include TMemRing.h, include CMemRingFactory.h!"
#endif // Check for main file

#include "pwxLib/internal/MSVCwarnPush.h"
#include <list>
#include <map>
#include <ostream>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <cmath> // for absDistance to appear.
#include "pwxLib/internal/MSVCwarnPop.h"

#include "pwxLib/tools/DefaultDefines.h"
#include "pwxLib/internal/MRFexceptions.h"
#include "pwxLib/internal/TItem.h"
#include "pwxLib/internal/CMemRing.h"

namespace pwx {

/** @namespace pwx::sh
  *
  * @brief importing namespace for pwx::StreamHelpers
**/
namespace sh
{
  using namespace ::pwx::StreamHelpers;
} // End of namespace sh

namespace mrf {

// Forward for the friend declaration:
template<typename T> class MRInterface;

/** @class TMemRing
  *
  * @brief Container class to handle typed items
  *
  * This is the template that is responsible to handle typed
  * items. Unlike TItem, this template adds most of its
  * functionality here, only the counting of items is derived
  * from base::CMemRing.
  *
  * The numbering of the items begins with 0, which is the
  * root item. All methods that retrieve an item by its number
  * can be used backwards by using a negative number, and wrap
  * the number around the valid range.
  * So if you want item 15, but there are only 10 items, You'll
  * get item 5.
**/
template<typename T>
class PWX_VISIBLE TMemRing : public base::CMemRing
{
public:

  // --- types ---
  typedef typename ::std::map< uint32_t , TItem<T> *>      tIdMap;
  typedef typename ::std::pair< uint32_t, TItem<T> *>      tIdPair;
  typedef typename tIdMap::const_iterator                  tIdIter_c;
  typedef typename ::std::map< ::std::string, TItem<T> *>  tNameMap;
  typedef typename ::std::pair< ::std::string, TItem<T> *> tNamePair;
  typedef typename tNameMap::const_iterator                tNameIter_c;

  // --- methods ---
  PWX_INLINE int32_t          add             (T *data, uint32_t aId=0, const char *aName=NULL);
  PWX_INLINE int32_t          add             (T value, uint32_t aId=0, const char *aName=NULL);
  PWX_INLINE int32_t          add_sorted      (T *data, uint32_t aId=0, const char *aName=NULL, bool asc=true);
  PWX_INLINE int32_t          add_sorted      (T value, uint32_t aId=0, const char *aName=NULL, bool asc=true);
  PWX_INLINE void             clear           ();
  PWX_INLINE void             delItem         (uint32_t aId);
  PWX_INLINE void             delItem         (const char *aName);
  PWX_INLINE void             delItem         (int32_t aNr);
  PWX_INLINE void             disableTracking ();
  PWX_INLINE int32_t          erase           (T *data);
  PWX_INLINE bool             exists          (uint32_t aId)       const PWX_WARNUNUSED;
  PWX_INLINE bool             exists          (const char *aName)  const PWX_WARNUNUSED;
  PWX_INLINE bool             exists          (int32_t aNr)        const PWX_WARNUNUSED;
  PWX_INLINE T              * getData         (int32_t aNr)        const PWX_WARNUNUSED;
  PWX_INLINE T              * getData         (uint32_t aId)       const PWX_WARNUNUSED;
  PWX_INLINE T              * getData         (const char *aName)  const PWX_WARNUNUSED;
  PWX_INLINE uint32_t         getDataId       (const T *data)      const PWX_WARNUNUSED;
  PWX_INLINE const char     * getDataName     (const T *data)      const PWX_WARNUNUSED;
  PWX_INLINE int32_t          getDataNr       (const T *data)      const PWX_WARNUNUSED;
  PWX_INLINE int32_t          getDataRefCount (const T *data)      const PWX_WARNUNUSED;
  PWX_INLINE TItem<T>       * getItem         (T * aData)          const PWX_WARNUNUSED;
  PWX_INLINE TItem<T>       * getItem         (uint32_t aId)       const PWX_WARNUNUSED;
  PWX_INLINE TItem<T>       * getItem         (const char *aName)  const PWX_WARNUNUSED;
  PWX_INLINE TItem<T>       * getItem         (int32_t aNr)        const PWX_WARNUNUSED;
  PWX_INLINE uint32_t         getItemId       (int32_t aNr)        const PWX_WARNUNUSED;
  PWX_INLINE uint32_t         getItemId       (const char *aName)  const PWX_WARNUNUSED;
  PWX_INLINE const char     * getItemName     (uint32_t aId)       const PWX_WARNUNUSED;
  PWX_INLINE const char     * getItemName     (int32_t aNr)        const PWX_WARNUNUSED;
  PWX_INLINE int32_t          getItemNr       (uint32_t aId)       const PWX_WARNUNUSED;
  PWX_INLINE int32_t          getItemNr       (const char *aName)  const PWX_WARNUNUSED;
  PWX_INLINE int32_t          getItemRefCount (int32_t aNr)        const PWX_WARNUNUSED;
  PWX_INLINE int32_t          insert          (T *data, int32_t aNr=-1, uint32_t aID=0, const char *aName=NULL);
  PWX_INLINE int32_t          insert          (T value, int32_t aNr=-1, uint32_t aID=0, const char *aName=NULL);
  PWX_INLINE void             load            (::std::ifstream &is, bool search = false);
  PWX_INLINE T              * pop             ()                         PWX_WARNUNUSED;
  PWX_INLINE void             push            (T *data, uint32_t aId=0, const char *aName=NULL);
  PWX_INLINE void             push            (T value, uint32_t aId=0, const char *aName=NULL);
  PWX_INLINE bool             isIdMapUsed     ()                   const;
  PWX_INLINE bool             isNameMapUsed   ()                   const;
  PWX_INLINE int32_t          mergeWith       (TMemRing<T> *source, bool autodestruct=true);
  PWX_INLINE int32_t          move            (int32_t aOldNr, int32_t aNewNr);
  PWX_INLINE T              * remove          (int32_t aNr);
  PWX_INLINE void             save            (::std::ostream &os) const;
  PWX_INLINE uint32_t         setItemId       (int32_t aNr, uint32_t aId);
  PWX_INLINE const char     * setItemName     (int32_t aNr, const char *aName);
  PWX_INLINE void             sort            (bool asc=true);
  PWX_INLINE void             sortById        (bool asc=true);
  PWX_INLINE void             sortByName      (bool asc=true);
  PWX_INLINE int32_t          sort_once       (bool asc=true);
  PWX_INLINE int32_t          sortById_once   (bool asc=true);
  PWX_INLINE int32_t          sortByName_once (bool asc=true);
  PWX_INLINE T              * shift           ()                           PWX_WARNUNUSED;
  PWX_INLINE void             unshift         (T *data, uint32_t aId=0, const char *aName=NULL);
  PWX_INLINE void             unshift         (T value, uint32_t aId=0, const char *aName=NULL);
  PWX_INLINE void             useIdMap        (bool state);
  PWX_INLINE void             useNameMap      (bool state);

  // --- templated methods ---
  template<typename Td> PWX_INLINE bool isIn        (Td *data) const;
  template<typename Td> PWX_INLINE bool isValueIn   (Td  data) const;

  // --- operators ---
  PWX_INLINE T           *operator[](int32_t aNr) const;
  PWX_INLINE TMemRing<T> &operator= (TMemRing<T> &source);
  PWX_INLINE TMemRing<T> &operator+=(TMemRing<T> &source);
  PWX_INLINE TMemRing<T> &operator+=(T *data);
  PWX_INLINE TMemRing<T> &operator+=(T value);
  PWX_INLINE TMemRing<T> &operator-=(TMemRing<T> &source);
  PWX_INLINE TMemRing<T> &operator-=(T *data);
  PWX_INLINE TMemRing<T> &operator-=(T value);

private:
  // --- attributes ---
  mutable // current is an internal helper and thus is allowed to be changed in const methods
  TItem<T>    *current;    //!< The data the Container "looks at" right now or NULL if none is selected.
  mutable // root is an internal helper as well and thus is allowed to be changed in const methods
  TItem<T>    *root;       //!< The root data, meaning the data with the number 0.
  tIdMap       idMap;      //!< A mapping of ids to items, usage is optional.
  tNameMap     nameMap;    //!< A mapping of names to items, usage is optional.
  uint32_t     maxId;      //!< Local note of the max id used, a free unique can then easily be generated if needed.
  bool         trackRef;   //!< If disabled the container does no longer track multiple references to added items
  bool         idMapUsed;  //!< All items are saved in idMap, and all IDs are ensured to be unique if set to true.
  bool         nameMapUsed;//!< All items are saved in nameMap, and all names are ensured to be unique if set to true.

  // The interface needs private access
  friend class MRInterface<T>;

  // --- private helper methods ---
  PWX_INLINE void     addAllFromCont      (TMemRing<T> *source, bool emptySource);
  PWX_INLINE void     deleteCurr          (bool withData);
  PWX_INLINE char   * getUniqueName       (const char * src)   const PWX_WARNUNUSED;
  PWX_INLINE uint32_t getUniqueId         (uint32_t src)             PWX_WARNUNUSED;
  PWX_INLINE int32_t  insert              (T *data, int32_t aNr, uint32_t aID, const char *aName, bool doReg);
  PWX_INLINE int32_t  movInsAfter         (TItem<T> *newItem, TItem<T> *newPrev, TItem<T> *oldCurr) PWX_WARNUNUSED;
  PWX_INLINE int32_t  movInsBefore        (TItem<T> *newItem, TItem<T> *newNext, TItem<T> *oldCurr) PWX_WARNUNUSED;
  PWX_INLINE int32_t  normalizeNumber     (int32_t aNr)        const PWX_WARNUNUSED;
  PWX_INLINE void     rebuildIdMap        ();
  PWX_INLINE void     rebuildNameMap      ();
  PWX_INLINE void     registerWithOwner   ();
  PWX_INLINE bool     setCurrToId         (uint32_t aId)       const PWX_WARNUNUSED;
  PWX_INLINE bool     setCurrToItem       (T *data)            const PWX_WARNUNUSED;
  PWX_INLINE bool     setCurrToItem       (const T *data)      const PWX_WARNUNUSED;
  PWX_INLINE bool     setCurrToName       (const char * aName) const PWX_WARNUNUSED;
  PWX_INLINE bool     setCurrToNr         (int32_t aNr)        const PWX_WARNUNUSED;
  PWX_INLINE int32_t  setCurrToOrder      (base::eOrderType type, T *data, uint32_t aId,
                                           const char *aName)  const PWX_WARNUNUSED;
  PWX_INLINE int32_t  setCurrToOrder      (base::eOrderType type, const T *data, uint32_t aId,
                                           const char *aName)  const PWX_WARNUNUSED;
  PWX_INLINE bool     setCurrToValue      (T data)             const PWX_WARNUNUSED;
  PWX_INLINE int32_t  sort                (base::eOrderType type)    PWX_WARNUNUSED;
  PWX_INLINE void     swapItems           (TItem<T> *first, TItem<T> *second);

public:
  // dtor needs to be inlined directly for the linker to not wreak havoc on multiple inclusion

  /** @brief default dtor
  **/
  virtual ~TMemRing ()
  {
    clear();
  }

  /** @brief Default constructor.
    *
    * Note: As the container has to register itself with its owner, it _will_ throw a
    *       pwx::mrf::cantCreateItem exception if the registration fails with this
    *       exception!
    *       Furthermore the registration can throw pwx::mrf::noIdMapAvailable or
    *       pwx::mrf::noNameMapAvailable. Both are fatal!
    *
    * @param[in] aOwner Pointer to the Factory owning this container or NULL if free
    * @param[in] withIdMap Set to false to not use the id map, true by default
    * @param[in] withNameMap Set to false to not use the name map, true by default
    * @param[in] aContName Optional name for the container
  **/
  explicit TMemRing (CMemRingFactory * aOwner, bool withIdMap=true, bool withNameMap=true, const char *aContName=NULL):
    CMemRing(aOwner, aContName),current(NULL),root(NULL),
#if defined(PWX_HAS_CXX11_INIT)
    idMap({}), nameMap({}),
#endif
    maxId(0),trackRef(true),
    idMapUsed(withIdMap),nameMapUsed(withNameMap)
  {
    // If we have an owner, it is time to register ourselves:
    if (owner)
      {
        PWX_TRY(registerWithOwner())
        PWX_THROW_FURTHER
      }
  }

  /** @brief copy ctor
    *
    * The copy constructor copies all items from source to this new instance.
    *
    * Note: if an id map shall be used, but the map can't be initialized with the first data,
    *       an exception of the type pwx::mrf::noIdMapAvailable will be thrown. The same applies
    *       to the name map, the exceptions is of the type pwx::mrf::noNameMapAvailable then.
    *
    * Note 2 : Whether an id/name map is used depends on the source container, as this detail
    *          is, of course, copied, too. Normally this should be completely save, but if an
    *          exception is thrown, the only way to copy the container is to do it "by hand"
    *          and without the revolting map. This is what MRF does.
    *
    * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
    * is thrown. This indicates that an operation is done in a multi threaded environment where two
    * threads move an item without proper locking. Please make sure to use MRInterface<T> with
    * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
  **/
  TMemRing (TMemRing<T> &source) : base::CMemRing(source)
  {
    TItem<T> *tgtItem = NULL;

    // mirror map settings:
    idMapUsed   = source.isIdMapUsed();
    nameMapUsed = source.isNameMapUsed();

    // mirror tracking
    trackRef = source.trackRef;

    // Mirror owner
    owner = source.owner;

    // If we have an owner, it is time to register ourselves:
    if (owner)
      {
        PWX_TRY(registerWithOwner())
        PWX_THROW_FURTHER
      }

    // now push all source items upon this:
    for (int32_t i = 0; i < source.size(); ++i)
      {
        // Use the TItem<T> copy ctor to make the items know their copies
        PWX_TRY(tgtItem = new TItem<T>(source.getItem(i)))
        catch (::std::bad_alloc &e)
          {
            char txt[128] = "";
            sprintf(txt, "copy ctor tried to create a copy TItem from nr %u, [%s], container [%s]",
                    i, ::std::string(source.getName()).substr(0,16).c_str(),
                    ::std::string(source.getItem(i).getName()).substr(0,16).c_str());
            PWX_THROW(cantCreateItem, e.what(), txt)
          }
        ++dataCount;

        // Set id and name:
        tgtItem->setId  (source.getItemId(i));
        tgtItem->setName(source.getItemName(i));

        // Now push it:
        PWX_TRY(movInsBefore(tgtItem, root, current))
        PWX_THROW_FURTHER

      } // End of copying cycle
  }

private:
  /* Although the non-empty ctor above is marked 'explicit', this isn't sufficient.
   * Without this private empty ctor the compiler will generate a myriad of mystical
   * error messages about illegal usage of functions, whenever someone tries to use
   * an 'instance' of TMemRing<T>. With this private empty ctor, the compiler does
   * point into the right direction if someone forgets to set the owner, either by
   * using "TMemRing<foo> cont;" or "TMemRing<foo> * cont = new TMemRing<foo>()".
   */
  TMemRing<T>() {}
};

} // End namespace mrf

} // End of pwx namespace

/* --- Implementation: --- */
#include "pwxLib/internal/TMRTImpl.h"

#endif // PWX_TMEMRING_H_INCLUDED
