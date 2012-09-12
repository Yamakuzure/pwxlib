#pragma once
#ifndef PWX_CCONFIGGROUP_H_INCLUDED
#define PWX_CCONFIGGROUP_H_INCLUDED

/** @internal
  * @file CConfigGroup.h
  *
  * @brief This file defines the CConfigGroup class used by pwxLib/CCnfFileHandler.h
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
  * History and Changelog maintained in CCnfFileHandler.h.
**/

#ifndef PWX_CCNFFILEHANDLER_H_INCLUDED
# error "Do not include CConfigGroup.h, include CCnfFileHandler.h!"
#endif // Check for main file

namespace pwx {
namespace cfh {
// note: The namespaces are documented in CCnfFileHandler.h!

/** @class CConfigGroup
  * @brief Representation of a group of config items.
  *
  * This class is to be used in a pwxMemRingContainer. There it has a unique
  * name, which is the group name. This works, because groups have to be unique in
  * their file. The ID in the container is used to maintain the ordering of the
  * groups in the file.
  *
  * The keys in a group are saved in a tDataRing, that has to be handed over
  * via the ctor.
**/
class PWX_INTERNAL CConfigGroup: private ::pwx::Uncopyable
{
  /* --- private members --- */
  cfflags         dataFlags;
  ::std::string   internalName;
  tDataRing      *items;
  tDataInterface  contInterface;
  ::std::string   lastKey;

public:
  /** @brief default ctor
    *
    * This class does not produce its own containers. They have to be created beforehand
    * and handed over via the ctor for the instance to use. The creation is therefore
    * the responsibility of CCnfFileHandler.
    *
    * @param[in] intName Internal name of the config group
    * @param[in] container Pointer to a container to hold the keys and comments in this group
    * @param[in] DataFlags The flags describing this data group set. @see cfflags
  **/
  explicit CConfigGroup(const char *intName, tDataRing *container, cfflags DataFlags):
    dataFlags(DataFlags), internalName(intName), items(container), contInterface(container), lastKey("")
    {
      assert (items && "ERROR: CConfigGroup must not be created without a valid container!");
      assert (internalName.size() && "ERROR: CConfigGroup must not be created without a valid internal name!");
    }

  /** @brief default dtor
    *
    * Although this is completely superfluous, I like it clean and set items to NULL here.
  **/
  ~CConfigGroup() { items = NULL; }

  /* --- direct inline methods --- */
  /** @brief return true if a specific key exists in this group **/
  bool         exists  (const char *key) const PWX_WARNUNUSED { return contInterface.exists(key); }
  /** @brief get the data string of a specific key by number **/
  CConfigData *getData (int32_t nr)            PWX_WARNUNUSED { return nr < contInterface.size() ? contInterface.getData(nr) : NULL; }
  /** @brief get a specific key by number **/
  const char  *getKey  (int32_t nr)      const PWX_WARNUNUSED { return contInterface.getItemName(nr); }
  /** @brief return the key that was last modified **/
  const char * getLastKey      () const { return lastKey.c_str();  }
  /** @brief get the number of saved keys in this group, including comment and blank lines **/
  int32_t      size    ()                const PWX_WARNUNUSED { return contInterface.size(); }
  /** @brief return true if this file sorts its data **/
  bool         sorted  ()                const                { return dataFlags & cfSort ? true : false; }

  /* --- indirect inline methods --- */
  PWX_INLINE int32_t      addData       (const char *key, const char *data, const char *comment);
  PWX_INLINE int32_t      addKey        (const char *key, const char *data, const char *comment);
  PWX_INLINE const char * getComment    (const char *key) PWX_WARNUNUSED;
  PWX_INLINE int32_t      getCommentSize(const char *key) PWX_WARNUNUSED;
  PWX_INLINE const char * getData       (const char *key) PWX_WARNUNUSED;
  PWX_INLINE int32_t      getDataCount  (const char *key) PWX_WARNUNUSED;
  PWX_INLINE const char * getDataItem   (const char *key, int32_t nr) PWX_WARNUNUSED;
  PWX_INLINE void         save          (::std::ostream &os, const char *groupName, int32_t maxLineLength);
  PWX_INLINE int32_t      setData       (const char *key, const char *data, const char *comment);
  PWX_INLINE void         setFlags      (cfflags DataFlags);
  PWX_INLINE int32_t      sort          (const char *key) PWX_WARNUNUSED;
  PWX_INLINE void         sort          (bool sortKeys, bool sortData);
  PWX_INLINE int32_t      unique        (const char *key) PWX_WARNUNUSED;

  /* --- operators --- */
  /// @brief return true if the internal name of this group (case insensitive) is "larger" than the name of @a rhs
  bool operator>(CConfigGroup &rhs)
    {
      return STRCGT(internalName.c_str(), rhs.internalName.c_str());
    }

private:
  /* --- copying is not allowed at all: --- */
  CConfigGroup(CConfigGroup&);
  CConfigGroup &operator=(CConfigGroup&);
};

typedef ::pwx::mrf::TMemRing<CConfigGroup>    tGroupRing;
typedef ::pwx::mrf::MRInterface<CConfigGroup> tGroupInterface;

/** @brief operator to shift a config group upon a stream.
  *
  * This operator does nothing, config groups have to be saved via their CConfigFile instance.
  *
  * @param[in] os The ostream to write into
  * @param[in] 1 The pushed CConfigGroup, which is ignored as this method does nothing
  * @return the ostream reference
**/
::std::ostream &operator<<(::std::ostream &os, CConfigGroup& PWX_UNUSED)
  {
    return os;
  }

} // End of namespace cfh
} // End of namespace pwx

#endif // PWX_CCONFIGGROUP_H_INCLUDED


