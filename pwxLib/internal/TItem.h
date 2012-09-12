#pragma once
#ifndef PWX_TITEM_H_INCLUDED
#define PWX_TITEM_H_INCLUDED 1

/** @internal
  * @file TItem.h
  * ------------------------
  *
  * @brief Declaration of TItem, the data wrapper class for TMemRing
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
# error "Do not include TItem.h, include CMemRingFactory.h !"
#endif // Check for main file

#include "pwxLib/internal/CItem.h"

namespace pwx {
namespace mrf {

// forwarding TMemRing<T> to allow TItems to know their owning container
template<typename T> class TMemRing;

/** @class TItem
  *
  * @brief data class to encapsulate stored data, adding a reference count, number, id and name.
  *
  * This is the template that is used as a shell around the held data.
  * Besides handling of the data it does not add much functionality.
  * The core functionality, like handling reference counts, the number,
  * id and name, is done by base::CItem.
**/
template <typename T>
class PWX_VISIBLE TItem : public base::CItem
{
public:
  // --- types ---
  typedef typename ::std::list<TItem<T> *> tList;
  typedef typename tList::iterator         tIter;
  typedef          TItem<T>                tItem;

  // --- methods ---
  PWX_INLINE void       addToCopies   (TItem<T> *item, bool cascade);
  PWX_INLINE void       addToCopies   (TItem<T> &item, bool cascade);
  PWX_INLINE void       delFromCopies (TItem<T> *data);
  PWX_INLINE TItem<T> * destroy       (bool withData = true);
  PWX_INLINE const T  & getData       ()                   const PWX_WARNUNUSED;
  PWX_INLINE T        & getData       ()                         PWX_WARNUNUSED;
  PWX_INLINE T        * getDataPointer()                         PWX_WARNUNUSED;
  PWX_INLINE TItem<T> * getNext       ()                   const PWX_WARNUNUSED;
  PWX_INLINE TItem<T> * getPrev       ()                   const PWX_WARNUNUSED;
  PWX_INLINE int32_t    getRefCount   ()                   const PWX_WARNUNUSED;
  PWX_INLINE TItem<T> * move          (TItem<T> *aPrev, TItem<T> *aNext, int32_t *newNum) PWX_WARNUNUSED;
  PWX_INLINE void       save          (::std::ostream &os) const;
  PWX_INLINE TItem<T> * setNr         (int32_t aNr)              PWX_WARNUNUSED;
  PWX_INLINE TItem<T> * swap          (TItem<T> *other)          PWX_WARNUNUSED;
  PWX_INLINE TItem<T>  &operator=     (TItem<T> &source);

private:
  // --- attributes ---
  tList        copies; //!< list of items that are known to hold the same data pointer
  T           *data;   //!< the data this is all about
  TItem<T>    *next;   //!< the next item in the memory ring
  TMemRing<T> *owner;  //!< Owner of the item
  TItem<T>    *prev;   //!< the previous item in the memory ring


  // --- methods ---
  PWX_INLINE void       clearCopies();
  PWX_INLINE TItem<T> * detach(bool doRenumbering) PWX_WARNUNUSED;
  PWX_INLINE TItem<T> * insert(bool doRenumbering) PWX_WARNUNUSED;

public:
  // ctor/dtor need to be inlined directly for the linker to not wreak havoc on multiple inclusion

  /** @brief default constructor
    *
    * default constructor, adding data. The constructing container is
    * responsible for chosing usable values for nr, id and name.
  **/
  explicit TItem (T *aData, int32_t aNr, uint32_t aId, const char *aName, TMemRing<T> *aOwner):
    CItem(aNr, aId, aName),
#if defined(PWX_HAS_CXX11_INIT)
    copies({}),
#endif
    data(aData),next(NULL),owner(aOwner),prev(NULL)
  { /* nothing to be done in here */ }

  /** @brief Copy constructor.
    *
    * Source is saved in copies. Number, id and name are copied, but the
    * neighborhood is the callers responsibility.
    *
    * @param[in] source the source data to copy from
  **/
  TItem (TItem<T> &source):CItem(source),
#if defined(PWX_HAS_CXX11_INIT)
    copies({}),
#endif
    data(source.data),next(NULL),owner(source.owner),prev(NULL)
  {
    source.addToCopies(this, true);
  }

private:
  /** @brief empty ctor that must not be used
  **/
  TItem() {}

  /** @brief default dtor that does nothing but being private. Use destroy()!
  **/
  ~TItem () { }
};

} // End namespace mrf

} // End of pwx namespace

/* --- Implementation: --- */
#include "pwxLib/internal/TMRIImpl.h"

#endif // PWX_TITEM_H_INCLUDED
