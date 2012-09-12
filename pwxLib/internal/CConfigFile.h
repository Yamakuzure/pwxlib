#pragma once
#ifndef PWX_CCONFIGFILE_H_INCLUDED
#define PWX_CCONFIGFILE_H_INCLUDED

/** @internal
  * @file CConfigFile.h
  *
  * @brief This file defines the CConfigFile class used by pwxLib/CCnfFileHandler.h
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
# error "Do not include CConfigFile.h, include CCnfFileHandler.h!"
#endif // Check for main file

#include "pwxLib/tools/DefaultDefines.h"

namespace pwx {
namespace cfh {
// note: The namespaces are documented in CCnfFileHandler.h!

/** @class CConfigFile
  * @brief Simple class to represent config files.
  *
  * This class manages config files. It does so by using an instance of tGroupRing
  * which then manages the groups contained in the config file.
  *
  * Every config file has at least one group name by the constant
  * pwx::cfh::cfConstants::CFC_NoGroupName which means that the keys in there are not
  * preceded by a [GROUPNAME] tag in the file. For regular ini files this is illegal, but
  * it allows the system to handle files that are no ini files but sourceable shell scripts
  * that set shell variables. Those scripts are looking remarkably like inifiles but simply
  * have no groups.
**/
class PWX_INTERNAL CConfigFile: private ::pwx::Uncopyable
{
  /* --- private members --- */
  cfflags          dataFlags;
  ::std::string    fileName;
  tGroupRing      *groups;
  ::std::string    internalName;
  tGroupInterface  contInterface;
  bool             isChanged;
  ::std::string    lastKey;
  size_t           maxLineLength;

public:
  /** @brief default ctor
    *
    * This class does not produce its own containers. They have to be created beforehand
    * and handed over via the ctor for the instance to use. The creation is therefore
    * the responsibility of CCnfFileHandler.
    *
    * The file name isn't only mandatory, it is important to know that this class does
    * not care whether the file exists or not. It _will_ be overwritten on every save.
    * Therefore CCnfFileHandler, that creates instances of this class, has the
    * responsibility to ensure that CConfigFile can write to the given filename, and to
    * do backups when needed.
    *
    * @param[in] intName Internal name of the config file
    * @param[in] container Pointer to a container to hold the groups of this file
    * @param[in] aFileName the path and name of the file.
    * @param[in] aMaxLineLength Key + Separator + Data must not exceed this or the line will be wrapped
    * @param[in] DataFlags The flags describing this config file. @see cfflags
  **/
  explicit CConfigFile(const char *intName, tGroupRing *container, const char *aFileName, size_t aMaxLineLength,
                       cfflags DataFlags):
      dataFlags(DataFlags), fileName(aFileName && strlen(aFileName) ? aFileName : ""),
      groups(container), internalName(intName), contInterface(container), isChanged(false), lastKey(""),
      maxLineLength(aMaxLineLength)
    {
      assert (groups && "ERROR: CConfigFile must not be created without a valid container!");
      assert (fileName.size() && "ERROR: CConfigFile needs a file name to work with!");
      assert (internalName.size() && "ERROR: CConfigFile must not be created without a valid internal name!");
    }

  /** @brief default dtor
    *
    * Although this is completely superfluous, I like it clean and set groups to NULL here.
  **/
  ~CConfigFile()
    {
      groups = NULL;
    }

  /* --- direct inline methods --- */
  /** @brief return true if any changes where made after this config file has been loaded or saved **/
  bool         changed         () const { return isChanged;        }
  /** @brief delete all keys in all groups **/
  void         clear           ()
    {
      groups->lock();
      groups->clear();
      groups->unlock();
      isChanged = true;
    }
  /** @brief return the set char identifying a comment **/
  char getCommentSep () const
    {
      char commSep = '#';

      if ( dataFlags & cfCSemicolon) commSep = ';';
      else if (dataFlags & cfCSpace) commSep = ' ';

      return commSep;
    }
  /** @brief return the set char separating data entries from each other **/
  char getDataSep () const
    {
      char dataSep = ',';

      if (     dataFlags & cfDColon)     dataSep = ':';
      else if (dataFlags & cfDPipe)      dataSep = '|';
      else if (dataFlags & cfDSemicolon) dataSep = ';';
      else if (dataFlags & cfDSpace)     dataSep = ' ';

      return dataSep;
    }
  /** @brief return the file name used by this config file **/
  const char  *getFileName     () const { return fileName.c_str(); }
  /** @brief return the currently set data flags **/
  cfflags      getFlags        () const { return dataFlags;        }
  /** @brief return the set char separating keys from data **/
  char getKeySep() const
    {
      char keySep = '=';

      if (     dataFlags & cfKColon)     keySep = ':';
      else if (dataFlags & cfKSemicolon) keySep = ';';
      else if (dataFlags & cfKSpace)     keySep = ' ';

      return keySep;
    }
  /** @brief return the set max length of a data line in this config file **/
  size_t       getMaxLineLength() const { return maxLineLength;    }
  /** @brief return the key that was last modified **/
  const char * getLastKey      () const { return lastKey.c_str();  }
  /** @brief return true if a specific group is known **/
  bool         hasGroup        (const char *group) const { return contInterface.exists(group); }
  /** @brief return true if this file sorts its data **/
  bool         sorted          () const { return dataFlags & cfSort ? true : false; }
  /** @brief return true if this file allows source entries **/
  bool         useSource       () const { return dataFlags & cfUseSource ? true : false; }

  /* --- indirect inline methods --- */
  PWX_INLINE int32_t      addData       (const char *group, const char *key, const char *data, const char *comment) PWX_WARNUNUSED;
  PWX_INLINE void         addFlags      (cfflags DataFlags);
  PWX_INLINE void         addGroup      (CConfigGroup *newGroup, const char *groupName);
  PWX_INLINE const char * getComment    (const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE int32_t      getCommentSize(const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE const char * getData       (const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE int32_t      getDataCount  (const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE const char * getDataItem   (const char *group, const char *key, int32_t nr) PWX_WARNUNUSED;
  PWX_INLINE void         removeFlags   (cfflags aDataFlags);
  PWX_INLINE void         save          ();
  PWX_INLINE void         save          (::std::ostream &os);
  PWX_INLINE void         setFlags      (cfflags DataFlags);
  PWX_INLINE int32_t      sort          (const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE void         sort          (bool sortGroups, bool sortKeys, bool sortData);
  PWX_INLINE void         sort          ();
  PWX_INLINE int32_t      setData       (const char *group, const char *key, const char *data, const char *comment) PWX_WARNUNUSED;
  PWX_INLINE int32_t unique             (const char *group, const char *key) PWX_WARNUNUSED;

  /* --- operators --- */
  /// @brief return true if the internal name of this file (case insensitive) is "larger" than the name of @a rhs
  bool operator>(CConfigFile &rhs)
    {
      return STRCGT(internalName.c_str(), rhs.internalName.c_str());
    }

private:
  /* --- copying is not allowed at all: --- */
  CConfigFile(CConfigFile&);
  CConfigFile &operator=(CConfigFile&);
};

typedef ::pwx::mrf::TMemRing<CConfigFile>    tFileRing;
typedef ::pwx::mrf::MRInterface<CConfigFile> tFileInterface;

/** @brief operator to shift a config file upon a stream.
  *
  * If the writing failed, a pwx::cfh::writeFailed exception is thrown.
  *
  * @param[in] os The ostream to write into
  * @param[in] cf The CConfigFile instance to save.
  * @return the ostream reference
**/
::std::ostream &operator<<(::std::ostream &os, CConfigFile &cf)
  {
    try
      {
        if (os.good())
          cf.save(os);
      }
    PWX_THROW_FURTHER
    return os;
  }

} // End of namespace cfh
} // End of namespace pwx

#endif // PWX_CCONFIGFILE_H_INCLUDED


