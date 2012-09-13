#pragma once
#ifndef PWX_CITEM_H_INCLUDED
#define PWX_CITEM_H_INCLUDED 1

/** @internal
  * @file CItem.h
  * ------------------------
  *
  * @brief Declaration of CItem, the base class for TItem, and class Identifiers
  *
  * This file is always included, so it is the place to define the base identifiers
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
# error "Do not include CItem.h, include CMemRingFactory.h!!"
#endif // Check for main file

namespace pwx {
namespace mrf {


/** @namespace pwx::mrf::base
  *
  * @brief Sub-namespace under namespace mrf for base classes.
  *
  * All base classes for MRF are defined in this namespace.
  * @see pwx::mrf for classes using these base classes
**/
namespace base
{

// --- Class Identifiers, which shall be used for loading and saving of data ---
const char ID_CITEM_START = 'C'; // The following is Core Data of CItem
const char ID_TITEM_START = 'T'; // The following is shell data of TItem
const char ID_TRING_START = 'R'; // The following is shell data of TMemRing
const char ID_TRING_DATA  = 'D'; // The following are the items from a TMemRing
const char ID_MRF_START   = 'M'; // Start of saving all of MRF

// --- Enum for sorting and searching ---
enum eOrderType
{
    OT_SmallerData = 1L << 0,
    OT_LargerData  = 1L << 1,
    OT_SmallerId   = 1L << 2,
    OT_LargerId    = 1L << 3,
    OT_SmallerName = 1L << 4,
    OT_LargerName  = 1L << 5
};

/** @class CItem
  *
  * @brief base class with general internal data and data handling used by the TItem template
  *
  * This is the abstract base class for all TItem templates, holding what all items have in
  * common and is not depending on the held data type.
  *
  * All items have a number, which identifies them by position in the memory ring they are
  * stored within. They have an id, which, if used, identifies them within an id map, and a
  * name to identify items within a name map if used.
  *
  * Ids and names are optional for the container. The container then is responsible to disallow
  * unusable values if Id and/or name maps shall be used.
  *
  * Although it is possible to use items without making use of the id or name, they are the only
  * reason why one might use pwxMemRing at all. On the other hand, if you do not use the id and
  * the name for identification, you could use those as free tags, adding data to your items
  * that do not hold such data on their own.
**/
class PWX_INTERNAL CItem: public ::pwx::Lockable
{
public:
  // --- methods ---
  PWX_INLINE uint32_t     getId         () const PWX_WARNUNUSED;
  PWX_INLINE const char * getName       () const PWX_WARNUNUSED;
  PWX_INLINE int32_t      getNr         () const PWX_WARNUNUSED;
  PWX_INLINE void         setCoreValues (int32_t aNr, uint32_t aId, const char * aName);
  PWX_INLINE void         setId         (uint32_t aId);
  PWX_INLINE void         setName       (::std::string &aName);
  PWX_INLINE void         setName       (const char * aName);

  // --- virtual methods ---
  virtual PWX_INLINE void    save       (::std::ostream & os) const;

  // --- abstract methods ---
  /// @brief setNr is not trivial and therefore abstract and to be defined in TItem
  virtual PWX_INLINE CItem * setNr      (int32_t) PWX_VIRTUAL_PURE;

  // --- operators ---
  PWX_INLINE CItem& operator=(const CItem &source);

protected:
  // --- attributes ---
  uint32_t      id;      //!< The id identifies an item with its containers id map.
  ::std::string name;    //!< The name identifies the item within its containers name map.
  int32_t       nr;      //!< The number identifies an item within its memory ring.


public:
  // The default ctor needs to be in-lined directly for the linker to not wreak havoc on multiple inclusion

  /** @brief default constructor, setting all three core data values.
    *
    * The constructing container is responsible to set usable values, as Items
    * have no possibility to check for themselves.
  **/
  explicit CItem (int32_t aNr, uint32_t aId, const char *aName):
    id(aId), name("an item"), nr(aNr)
  { if (aName) name = aName; }

  /** @brief copy constructor to copy items.
    *
    * The copying container is responsible to set the number, id and name to
    * usable values afterwards. The destination item can't check for itself,
    * because it doesn't even need to be within the same container as the source.
  **/
  CItem (CItem &source):Lockable(source), id(source.id), name(source.name), nr(source.nr)
  { }

private:
  /** @brief empty ctor, that does nothing besides being private.
  **/
  CItem ():id(0), name(""), nr(0) { }

protected:
  /** @brief Default dtor. It does nothing besides being non-public.
   */
  PWX_INLINE virtual ~CItem ();

};

// The dtor, although protected, is out-line to have the vtable here
CItem::~CItem() { }

} // End of namespace base

} // End of namespace mrf

} // End of namespace pwx

/* --- Implementation: --- */
#include "pwxLib/internal/CMRIImpl.h"

#endif // PWX_CITEM_H_INCLUDED
