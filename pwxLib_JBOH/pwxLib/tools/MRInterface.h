#pragma once
#ifndef PWX_TMRINTERFACE_H_INCLUDED
#define PWX_TMRINTERFACE_H_INCLUDED 1

/** @file MRInterface.h
  * ---------------------------
  *
  * @brief Interface Class to a TMemRing for multiple parallel access
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

// As MRInterface.h is a "tool", it must be allowed to include it alone.
// Therefore it is not an error if CMemRingFactory.h isn't included, yet,
// Just do it now then.
#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
#  include "pwxLib/CMemRingFactory.h"
#endif // Check for main file
#ifndef PWX_TMEMRING_H_INCLUDED
#  include "pwxLib/internal/TMemRing.h"
#endif

namespace pwx {

namespace mrf {

/** @class MRInterface
  *
  * @brief Interface class to handle Containers in a thread safe way.
  *
  * This is an interface class to allow simultaneous access to a container
  * from different parallel places like threads. When searching for an item
  * the container saves which item has been retrieved last to speed up access
  * to nearby items like in loops. Doing this from different threads invalidates
  * this position on every access. This interface uses its own retrieval on
  * a container. Therefore access is sped up greatly if every thread uses its own
  * interface.
  *
  * Manipulating a container from different threads is possible by simply locking
  * the container before every manipulation. But since many modifications involve
  * a search for the right position the usage of this interface can help, too.
  * When manipulating a container through the interface, container and item locking
  * is done automatically and more fine grained.
  *
  * For data retrieval you can use the same method set as can be found in TMemRing.
  * For data manipulation, however, only a small subset is provided to keep the
  * complexity low. The methods for stack operation, loading, saving, merging,
  * deleting and clearing a container have been removed. Furthermore there is no
  * manipulating operator available via this interface. (operator[] is provided,
  * though) You can neither set the usage of the maps via the interface nor disable
  * the reference tracking.
  *
  * Additionally to the inherited (and sometimes overwritten) methods from TMemRing<T>,
  * MRInterface<T> has four more methods. getCurrentNr() returns the number of the
  * currently handled item, getUnsortedCount() reports the number of unsorted items
  * found during running a sort, interruptSorting() stops the current sorting process
  * and reset() tells the interface, that its currently used item is no longer save.
  * These four methods can be used to control what is happening in a multi threaded
  * environment. See the gravMat tool for how this can be done.
**/
template<typename T>
class PWX_VISIBLE MRInterface : private TMemRing<T>, private ::pwx::Uncopyable
{
public:

  // --- types ---
  typedef typename TMemRing<T>::tIdMap      tIdMap;
  typedef typename TMemRing<T>::tIdIter_c   tIdIter_c;
  typedef typename TMemRing<T>::tNameMap    tNameMap;
  typedef typename TMemRing<T>::tNameIter_c tNameIter_c;

  /* --- methods from CMemRing --- */
  PWX_INLINE bool         empty   () const;
  PWX_INLINE const char * getName () const;
  PWX_INLINE int32_t      size    () const;

  /* --- methods from TMemRing --- */
  PWX_INLINE int32_t          add             (T *data, uint32_t aId=0, const char *aName=NULL);
  PWX_INLINE int32_t          add             (T value, uint32_t aId=0, const char *aName=NULL);
  PWX_INLINE int32_t          add_sorted      (T *data, uint32_t aId=0, const char *aName=NULL, bool asc=true);
  PWX_INLINE int32_t          add_sorted      (T value, uint32_t aId=0, const char *aName=NULL, bool asc=true);
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
  PWX_INLINE bool             isIdMapUsed     ()                   const;
  PWX_INLINE bool             isNameMapUsed   ()                   const;
  PWX_INLINE int32_t          move            (int32_t aOldNr, int32_t aNewNr);
  PWX_INLINE void             reset           ();
  PWX_INLINE uint32_t         setItemId       (int32_t aNr, uint32_t aId);
  PWX_INLINE const char     * setItemName     (int32_t aNr, const char *aName);
  PWX_INLINE void             sort            (bool asc=true);
  PWX_INLINE void             sortById        (bool asc=true);
  PWX_INLINE void             sortByName      (bool asc=true);
  PWX_INLINE int32_t          sort_once       (bool asc=true);
  PWX_INLINE int32_t          sortById_once   (bool asc=true);
  PWX_INLINE int32_t          sortByName_once (bool asc=true);

  // --- direct inline methods ---
  /// @brief returns the number of the currently handled item
  int32_t getCurrentNr    () const { return current ? current->getNr() : 0; }
  /// @brief returns the currently reported number of unsorted items
  int32_t getUnsortedCount() const { return unsorted; }
  /// @brief notify the interface that the current sorting is to be interrupted
  void    interruptSorting()       { doSort = false; }

  // --- templated methods ---
  template<typename Td> PWX_INLINE bool isIn        (Td *data) const;
  template<typename Td> PWX_INLINE bool isValueIn   (Td  data) const;

  // --- operators ---
  PWX_INLINE T           *operator[](int32_t aNr) const;

private:
  // --- attributes ---
  TMemRing<T> * cont;     //!< Pointer to the interfaced container
  bool          doSort;   //!< Set to true by the sorting methods, they stop if doSort is set to false by interruptSorting()
  tIdMap      * lIdMap;   //!< Pointer to the idMap of the interfaced container
  tNameMap    * lNameMap; //!< Pointer to the nameMap of the interfaced container
  int32_t       unsorted; //!< Used by the interfaces sort method to report the state to the outside

  // We need to tell the compiler that we wish to reach through:
  using TMemRing<T>::current;
  using base::CMemRing::dataCount;
  using TMemRing<T>::root;

  // --- private helper methods ---
  PWX_INLINE void reattach() const; // Note: This has to be const or all retrieval methods would have to become non-const
  // The following from TMemRing are needed:
  using TMemRing<T>::setCurrToItem;
  using TMemRing<T>::setCurrToOrder;
  using TMemRing<T>::setCurrToNr;
  using TMemRing<T>::setCurrToValue;
  // The following two have to be overwritten to use the
  // local map pointers if the maps are used:
  PWX_INLINE bool    setCurrToId   (uint32_t aId)       const PWX_WARNUNUSED;
  PWX_INLINE bool    setCurrToName (const char * aName) const PWX_WARNUNUSED;
  // These are overwritten to put locks in at a fine grained level:
  PWX_INLINE int32_t sort_p        (base::eOrderType type)    PWX_WARNUNUSED;
  PWX_INLINE void    swapItems     (TItem<T> * first, TItem<T> * second);

public:
  // dtor needs to be inlined directly for the linker to not wreak havoc on multiple inclusion

  /** @brief default dtor
  **/
  virtual ~MRInterface ()
    {
      // we do not delete anything. To make sure of that it is sufficient to set current and root
      // to be NULL and dataCount be 0. ~TMemRing() won't do anything then.
      cont = NULL;
      reattach(); // This'll invalidate the values
    }

  /** @brief Default constructor.
    *
    * This is a read-only interface. Therefore the only parameter needed is the container
    * that is to be interfaced.
    *
    * @param[in] container Pointer to the Container that is to be interfaced
  **/
  explicit MRInterface (TMemRing<T> * container):
    TMemRing<T>(NULL), cont(container), doSort(false), lIdMap(NULL), lNameMap(NULL), unsorted(0)
  {
    assert (cont && "ERROR: MRInterface ctor called with NULL container!");
    if (cont)
      {
        lIdMap = &(cont->idMap);
        lNameMap = &(cont->nameMap);
      }
    reattach();
  }

private:
  /* --- neither empty ctor, copy ctor nor operator= are allowed --- */
  MRInterface<T>();
  MRInterface<T>(MRInterface<T>&);
  MRInterface<T> &operator=(MRInterface<T>&);
};

} // End namespace mrf

} // End of pwx namespace

/* --- Implementation: --- */
#include "pwxLib/internal/MRIntImpl.h"

#endif // PWX_TMRINTERFACE_H_INCLUDED

