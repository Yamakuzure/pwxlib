#pragma once
#ifndef PWX_CMEMRING_H_INCLUDED
#define PWX_CMEMRING_H_INCLUDED 1

/** @internal
  * @file CMemRing.h
  * ------------------------
  *
  * @brief Declaration of CMemRing, the base class for TMemRing
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
# error "Do not include CMemRing.h, include CMemRingFactory.h!!"
#endif // Check for main file


namespace pwx {
namespace mrf {

// As CMemRingFactory is not declared at this point, it needs to be forwarded:
class CMemRingFactory;
// Note: The Methods using CMemRingFactory are defined in pexLib/internal/TMRT_insert.h
// and will be included by CMemRingFactory.h

namespace base {
// Note: pwx::mrf::base is documented in CItem.h, which is always included

/** @class CMemRing
  *
  * @brief abstract base class for the TMemRing template
  *
  * This is the abstract base class for all TMemRing templates, holding
  * what all memory ring containers have in common and is not depending
  * on the held datatype.
  *
  * All containers have a count of the held data, which is used not only
  * for counting, but for checking the numbering consistency as well.
**/
class PWX_INTERNAL CMemRing: public ::pwx::Lockable
{
  typedef ::pwx::mrf::CMemRingFactory ownerType;

public:
  /// @brief default dtor that does nothing but being the first virtual non-inline method
  PWX_INLINE virtual ~CMemRing ();

  /// @brief return true if the container is empty
  bool empty () const
  {
    return (dataCount == 0);
  }

  /// @brief get the current name of the container
  const char * getName () const
  {
    return (name.c_str());
  }

  /// @brief Placeholder for CMemRing Items, used by MRF
  virtual void save (::std::ostream&)  const { }

  /** @brief set the current name of the container via C-String
  *
  * @param[in] aName the desired name. If set to NULL, the current name is erased.
  **/
  void setName (const char * aName)
  {
    name = aName ? aName : "";
  }

  /** @brief set the current name of the container
  *
  * @param[in] aName the desired name. If set to NULL, the current name is erased.
  **/
  void setName (const ::std::string &aName)
  {
    name = aName.size() ? aName : "";
  }

  /// @brief get the number of items.
  int32_t size () const
  {
    return (dataCount);
  }

  // --- operators ---

protected:
  mutable // dataCount has to be calculated on the fly from the interface and is therefore mutable
  int32_t       dataCount; //!< This value is raised by one with each data added, and lowered by one with each data substracted.
  ::std::string name;      //!< containers can have names, too!
  ownerType   * owner;     //!< needed to register item copies

public:
  // --- direct inline ctor to make linkers happy ---

  /// @brief default ctor, which sets the datatype and an optional name
  explicit CMemRing (ownerType * aOwner, const char * aName = NULL):
    dataCount(0),name("Container"),owner(aOwner)
    {
      if (aName && strlen(aName))
        name = aName;
    }

  /** @brief copy constructor
    *
    * The copy ctor copies the owner and the name. The datacount is set
    * by the copying of the items.
  **/
  CMemRing (CMemRing &source):Lockable(source), dataCount(0), name(source.name), owner(source.owner)
  { }

private:
  CMemRing(): dataCount(0), name(""), owner(NULL) {} // See TMemRing<T>() why this is needed

  // There is no assignment of raw MemRings without item content
  CMemRing &operator= (CMemRing&);
};

// Note: This base class is small enough to be able to
//       live without an additional implementation file

// Place the virtual dtor here as an out-line method, so the vtable is
// forced to be placed here.
CMemRing::~CMemRing() { }

// --- stream operators ---
PWX_INLINE ::std::ostream& operator<<(::std::ostream &os, CMemRing &cont)
{
  cont.save(os);
  return(os);
}

} // End of namespace base

} // End of namespace mrf

} // End of namespace pwx

#endif //PWX_CMEMRING_H_INCLUDED


