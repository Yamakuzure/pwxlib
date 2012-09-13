#pragma once
#ifndef PWX_CCONFIGDATA_H_INCLUDED
#define PWX_CCONFIGDATA_H_INCLUDED

/** @internal
  * @file CConfigData.h
  *
  * @brief This file defines the CConfigData class used by pwxLib/CCnfFileHandler.h
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
# error "Do not include CConfigData.h, include CCnfFileHandler.h!"
#endif // Check for main file

namespace pwx {
namespace cfh {
// note: The namespaces are documented in CCnfFileHandler.h!

/** @class CConfigData
  * @brief Simple class to hold the data and/or comments of one key
  *
  * This class is to be used in a pwxMemRingContainer. There it is stored
  * as an item having a unique name, which is the key. This ensures that
  * keys are unique in their group. The ID in the container is used
  * to maintain the ordering of the keys in a group.
**/
class PWX_INTERNAL CConfigData
{
  /* --- private members --- */
  ::std::string                comment;
  ::std::list< ::std::string > data;
  cfflags                      dataFlags;
  // The folowing two are representatives and thus mutable
  mutable ::std::string        dataStr;
  mutable bool                 dataStrBuilt;
  ::std::string                internalName;

  /* --- types --- */
  typedef ::std::list< ::std::string >::iterator dataIter;

public:
  /** @brief default ctor
    *
    * This constructor saves the (optional) comment and splits the (optional)
    * data into items. If @a sortData is set to true, the data items are sorted
    * alphabetically afterwards.
    *
    * @param[in] key The key this data set is identified by in its group and file
    * @param[in] aData C-String with the data items
    * @param[in] aComment C-String with the comments to this data
    * @param[in] DataFlags The flags describing this data set. @see cfflags
  **/
  explicit CConfigData(const char *key, const char *aData, const char *aComment, cfflags DataFlags):
    comment(""),
#if defined(PWX_HAS_CXX11_INIT)
    data({}),
#endif
    dataFlags(DataFlags), dataStr(""), dataStrBuilt(false), internalName(key)
    {
      assert (internalName.size() && "ERROR: CConfigData must not be created without a valid key!");
      add(aData, aComment);
    } // End of ctor

  /// @brief the dtor does not need to do anything.
  ~CConfigData() {}

  /* --- direct inline methods --- */
  /// @brief return the number of characters of the comment
  int32_t     commentSize()         const { return comment.size();  }
  /// @brief return the comment as a const char*
  const char *getComment()          const { return comment.c_str(); }
  /// @brief change the current configuring flags to @a DataFlags
  void        setFlags(cfflags DataFlags) { dataFlags = DataFlags; dataStrBuilt = false; }
  /// @brief return the number of data items currently saved
  int32_t     size()                const { return data.size();     }

  /* --- indirect inline methods --- */
  PWX_INLINE int32_t     add       (const char *aData, const char *aComment);
  PWX_INLINE const char *getData   ()           const PWX_WARNUNUSED;
  PWX_INLINE const char *getEntry  (int32_t nr) const PWX_WARNUNUSED;
  PWX_INLINE void        save      (::std::ostream &os, const char *key, uint32_t maxLineLength);
  PWX_INLINE int32_t     set       (const char *aData, const char *aComment);
  PWX_INLINE void        sort      ();
  PWX_INLINE int32_t     unique    () PWX_WARNUNUSED;

  /* --- operators --- */
  /// @brief return true if the key of this item (case insensitive) is "larger" than the key of @a rhs
  bool operator>(CConfigData &rhs)
    {
      return STRCGT(internalName.c_str(), rhs.internalName.c_str());
    }

private:
  // Should be called whenever dataStrBuilt is false prior getting full data
  void buildDataStr() const
    {
      int32_t dataCnt = data.size();
      dataStr = "";

      if (dataCnt)
        {
          const char dataSep = getDataSep();
          int32_t    nr      = 0;

          // Now build the string
          while (nr < dataCnt)
            {
              dataStr += getEntry(nr++);
              // we need a separator if the is another item left
              if (nr < dataCnt)
                dataStr += dataSep;
            }
        } // End of having data to write

      // It is built now:
      dataStrBuilt = true;
    }

  // Convenience function to get the current comment separator
  char getCommentSep() const
    {
      char commSep = '#';

      if ( dataFlags & cfCSemicolon) commSep = ';';
      else if (dataFlags & cfCSpace) commSep = ' ';

      return commSep;
    }

  // Convenience function to get the current data separator
  char getDataSep() const
    {
      char dataSep = ',';

      if (     dataFlags & cfDColon)     dataSep = ':';
      else if (dataFlags & cfDPipe)      dataSep = '|';
      else if (dataFlags & cfDSemicolon) dataSep = ';';
      else if (dataFlags & cfDSpace)     dataSep = ' ';

      return dataSep;
    }

  // Convenience function to get the current key separator
  char getKeySep() const
    {
      char keySep = '=';

      if (     dataFlags & cfKColon)     keySep = ':';
      else if (dataFlags & cfKSemicolon) keySep = ';';
      else if (dataFlags & cfKSpace)     keySep = ' ';

      return keySep;
    }
};

typedef ::pwx::mrf::TMemRing<CConfigData>    tDataRing;
typedef ::pwx::mrf::MRInterface<CConfigData> tDataInterface;

/** @brief operator to shift config data upon a stream.
  *
  * This operator does nothing, config data items have to be saved via their CConfigFile instance.
  *
  * @param[in] os The ostream to write into
  * @param[in] 1 The pushed CConfigData, which is ignored as this method does nothing
  * @return the ostream reference
**/
::std::ostream &operator<<(::std::ostream &os, CConfigData& PWX_UNUSED)
  {
    return os;
  }

} // End of namespace cfh
} // End of namespace pwx

#endif // PWX_CCONFIGDATA_H_INCLUDED


