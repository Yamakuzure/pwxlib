#pragma once
#ifndef PWX_CCONFIGFILEIMPL_H_INCLUDED
#define PWX_CCONFIGFILEIMPL_H_INCLUDED

/** @internal
  * @file CConfigFileImpl.h
  *
  * @brief This file defines the methods of CConfigFile used by CCnfFileHandler
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
  * History and Changelog maintained in CCfhFileHandler.h.
**/

#ifndef PWX_CCNFFILEHANDLER_H_INCLUDED
# error "Do not include CConfigFileImpl.h, include CCnfFileHandler.h!"
#endif // Check for main file

namespace pwx {
namespace cfh {
// note: The namespaces are documented in CCnfFileHandler.h!

/* =========================================== */
/* === Implementations for CConfigFile     === */
/* =========================================== */

/** @brief add a key-value-pair and / or a comment
  *
  * This method manipulates an entry of this config file identified
  * by @a group and @a key. For blank lines and pure comment lines
  * there are two special key names. If either
  * pwx::cfh::cfConstants::CFC_BlankLine or CFC_PureComment are set
  * as @a key, this method will create a new entry, and not
  * manipulate existing ones.
  *
  * If the Entry identified by @a key is found, the data set by @a data
  * will be added to the existing set.
  *
  * Comments are handled the same, according to the given data.
  *
  * If @a group can not be found, something is really wrong and a
  * pwx::mrf::nameNotFound exception is thrown. The group has to
  * exist prior calling this method, please use your CCnfFileHandler
  * instance (or the global CFH) instead then.
  *
  * @param[in] group The name of the group to manipulate
  * @param[in] key Key identifying the entry
  * @param[in] data Data to store in this entry
  * @param[in] comment Optional information in a comment
  * @return The number of data items present for this key if @a data is set, otherwise the length of the comment.
**/
int32_t CConfigFile::addData(const char *group, const char *key, const char *data, const char *comment)
  {
    CConfigGroup *currGroup = NULL;
    PWX_TRY(currGroup = contInterface.getData(group))
    PWX_THROW_FURTHER

    int32_t result = 0;

    if ( STREQ(key, CFC_BlankLine) || STREQ(key, CFC_PureComment) // Those are always added!
      || !currGroup->exists(key) )
      result = currGroup->addKey(key, data, comment);
    else if (data)
      {
        ::std::string newData = data;

        // If we have a shell instance in our data set, it has to be erased, and the data added:
        ::pwx::StreamHelpers::cropShell(key, newData);
        result = currGroup->addData(key, newData.c_str(), comment);
      } // End of key exists and data is set
    else
      // If data is NULL, only a comment is added
      result = currGroup->addData(key, NULL, comment);

    isChanged = true;
    lastKey   = currGroup->getLastKey();

    return result;
  }


/** @brief add flags to the data flags
  * This method sadds @a CfFlags to the current set of data flags.
  *
  * If the given flags interfere with existing flags, the offending flags are
  * removed without further notice. Otherwise you had to first remove the old
  * data separator flag before setting a new one.
  *
  * @param[in] DataFlags Flags to describe the config file. @see cfflags
*/
void CConfigFile::addFlags (cfflags DataFlags)
  {
    // === Issue 1: Key Separators ===
    //---------------------------------
    cfflags testFlags = cfKAssign | cfKColon | cfKSemicolon | cfKSpace;
    if ((dataFlags & testFlags) && (DataFlags & testFlags))
      removeFlags(testFlags);

    // === Issue 2: Data Separators ===
    //----------------------------------
    testFlags = cfDColon | cfDComma | cfDPipe | cfDSemicolon | cfDSpace;
    if ((dataFlags & testFlags) && (DataFlags & testFlags))
      removeFlags(testFlags);

    // === Issue 3: Comment Separators ===
    //-------------------------------------
    testFlags = cfCSemicolon | cfCMesh | cfCSpace;
    if ((dataFlags & testFlags) && (DataFlags & testFlags))
      removeFlags(testFlags);

    // Now add the given flags
    cfflags newFlags = dataFlags | DataFlags;
    setFlags(newFlags);
  }


/** @brief add a new group to the file
  *
  * This method adds a new group to the file. The CConfigGroup object,
  * and therefore it's data container as well, have to be created
  * beforehand.
  *
  * @param[in] newGroup The CConfigGroup object holding the data items
  * @param[in] groupName The name of the group
  *
  * The ID is set automatically!
**/
void CConfigFile::addGroup (CConfigGroup *newGroup, const char *groupName)
  {
    contInterface.add(newGroup, 0, groupName);
    isChanged = true;
  }


/** @brief get the comment string held by @a group -> @a key
  *
  * If @a group can not be found, something is really wrong and a
  * pwx::mrf::nameNotFound exception is thrown. The group has to
  * exist prior calling this method. Please use your CCnfFileHandler
  * instance (or the global CFH) instead then.
  *
  * @param[in] group The name of the group to look into
  * @param[in] key Key identifying the entry
  * @return The comment string of the given @a group -> @a key.
**/
const char * CConfigFile::getComment(const char *group, const char *key)
  {
    CConfigGroup *currGroup = NULL;
    PWX_TRY(currGroup = contInterface.getData(group))
    PWX_THROW_FURTHER

    return currGroup->getComment(key);
  }


/** @brief get the size of the comment held by @a group -> @a key
  *
  * This method returns the size of the comment currently held by
  * the key @a key in the group @a group.
  *
  * If @a group can not be found, something is really wrong and a
  * pwx::mrf::nameNotFound exception is thrown. The group has to
  * exist prior calling this method. Please use your CCnfFileHandler
  * instance (or the global CFH) instead then.
  *
  * @param[in] group The name of the group to look into
  * @param[in] key Key identifying the entry
  * @return The size of the comment of this key
**/
int32_t CConfigFile::getCommentSize(const char *group, const char *key)
  {
    CConfigGroup *currGroup = NULL;
    PWX_TRY(currGroup = contInterface.getData(group))
    PWX_THROW_FURTHER

    return currGroup->getCommentSize(key);
  }


/** @brief get the data string held by @a group -> @a key
  *
  * If @a group can not be found, something is really wrong and a
  * pwx::mrf::nameNotFound exception is thrown. The group has to
  * exist prior calling this method. Please use your CCnfFileHandler
  * instance (or the global CFH) instead then.
  *
  * @param[in] group The name of the group to look into
  * @param[in] key Key identifying the entry
  * @return The data string of the given @a group -> @a key.
**/
const char * CConfigFile::getData(const char *group, const char *key)
  {
    CConfigGroup *currGroup = NULL;
    PWX_TRY(currGroup = contInterface.getData(group))
    PWX_THROW_FURTHER

    return currGroup->getData(key);
  }


/** @brief get the number of data items held by @a group -> @a key
  *
  * This method returns the number of data items currently held by
  * the key @a key in the group @a group.
  *
  * If @a group can not be found, something is really wrong and a
  * pwx::mrf::nameNotFound exception is thrown. The group has to
  * exist prior calling this method. Please use your CCnfFileHandler
  * instance (or the global CFH) instead then.
  *
  * @param[in] group The name of the group to look into
  * @param[in] key Key identifying the entry
  * @return The number of data items present for this key
**/
int32_t CConfigFile::getDataCount(const char *group, const char *key)
  {
    CConfigGroup *currGroup = NULL;
    PWX_TRY(currGroup = contInterface.getData(group))
    PWX_THROW_FURTHER

    return currGroup->getDataCount(key);
  }


/** @brief get the data item with the number @a nr held by @a group -> @a key
  *
  * This method returns the data item with the number @a nr currently
  * held by the key @a key in the group @a group.
  *
  * The first data item has the number 0.
  * To count data items backwards from the end, a negative nr @a nr can
  * be used. The last item then has the number -1.
  *
  * If @a group can not be found, something is really wrong and a
  * pwx::mrf::nameNotFound exception is thrown. The group has to
  * exist prior calling this method. Please use your CCnfFileHandler
  * instance (or the global CFH) instead then.
  *
  * @param[in] group The name of the group to look into
  * @param[in] key Key identifying the entry
  * @param[in] nr the number of the item, negative numbers count backwards from the end.
  * @return The data item with the number @a nr or an empty string if the number is out of range.
**/
const char * CConfigFile::getDataItem(const char *group, const char *key, int32_t nr)
  {
    CConfigGroup *currGroup = NULL;
    PWX_TRY(currGroup = contInterface.getData(group))
    PWX_THROW_FURTHER

    return currGroup->getDataItem(key, nr);
  }


/** @brief remove flags from this file **/
void CConfigFile::removeFlags(cfflags aDataFlags)
  {
    cfflags newFlags = dataFlags;
    if ((newFlags & cfCSemicolon)  && (aDataFlags & cfCSemicolon))  newFlags ^= cfCSemicolon;
    if ((newFlags & cfCMesh)       && (aDataFlags & cfCMesh))       newFlags ^= cfCMesh;
    if ((newFlags & cfCSpace)      && (aDataFlags & cfCSpace))      newFlags ^= cfCSpace;
    if ((newFlags & cfDColon)      && (aDataFlags & cfDColon))      newFlags ^= cfDColon;
    if ((newFlags & cfDComma)      && (aDataFlags & cfDComma))      newFlags ^= cfDComma;
    if ((newFlags & cfDPipe)       && (aDataFlags & cfDPipe))       newFlags ^= cfDPipe;
    if ((newFlags & cfDSemicolon)  && (aDataFlags & cfDSemicolon))  newFlags ^= cfDSemicolon;
    if ((newFlags & cfDSpace)      && (aDataFlags & cfDSpace))      newFlags ^= cfDSpace;
    if ((newFlags & cfKAssign)     && (aDataFlags & cfKAssign))     newFlags ^= cfKAssign;
    if ((newFlags & cfKColon)      && (aDataFlags & cfKColon))      newFlags ^= cfKColon;
    if ((newFlags & cfKSemicolon)  && (aDataFlags & cfKSemicolon))  newFlags ^= cfKSemicolon;
    if ((newFlags & cfKSpace)      && (aDataFlags & cfKSpace))      newFlags ^= cfKSpace;
    if ((newFlags & cfNoEncase)    && (aDataFlags & cfNoEncase))    newFlags ^= cfNoEncase;
    if ((newFlags & cfNoGroups)    && (aDataFlags & cfNoGroups))    newFlags ^= cfNoGroups;
    if ((newFlags & cfNoEmptyName) && (aDataFlags & cfNoEmptyName)) newFlags ^= cfNoEmptyName;
    if ((newFlags & cfNoWrap)      && (aDataFlags & cfNoWrap))      newFlags ^= cfNoWrap;
    if ((newFlags & cfSort)        && (aDataFlags & cfSort))        newFlags ^= cfSort;
    if ((newFlags & cfUseSource)   && (aDataFlags & cfUseSource))   newFlags ^= cfUseSource;
    if ((newFlags & cfWriteEmpty)  && (aDataFlags & cfWriteEmpty))  newFlags ^= cfWriteEmpty;

    if (newFlags != dataFlags)
      setFlags(newFlags);
  }


/** @brief save the contents of this file to disk
  *
  * If the writing failed, a pwx::cfh::writeFailed exception is thrown.
**/
void CConfigFile::save ()
  {
    ::std::ofstream outFile(fileName.c_str());

    if (outFile.is_open())
      {
        PWX_TRY(save(outFile))
        PWX_THROW_FURTHER

        // Throw if writing failed anywhere
        if (outFile.bad())
          {
            ::std::string msg = "Write error in file ";
            msg += fileName;
            PWX_THROW(writeFailed, "save failed!", msg.c_str())
          }

        if (outFile.is_open())
          outFile.close();

        isChanged = false;
      } // end of outFile is open
    else
      {
        // The file could not be opened
        ::std::string msg = "The file ";
        msg += fileName;
        msg += " could is not opened or ready.";
        PWX_THROW(writeFailed, "save failed!", msg.c_str())
      }
  }


/** @brief save the contents of this file to an ostream
  *
  * This is the worker method that does save data to an ostream.
  *
  * If the writing failed, a pwx::cfh::writeFailed exception is thrown.
  *
  * @param[in] os and open ostream to write data into
**/
void CConfigFile::save (::std::ostream &os)
  {
    int32_t groupCnt = contInterface.size();
    // To ensure, that all groups are in the correct order, they are re-ordered
    // now. Although it looks like this is not necessary, it ensures safety
    // if things change in the future, or the sources are tampered with.
    // Further more the data has to be sorted at least now if wanted.
    sort(true, true, sorted());

    if (os.good())
      {
        ::std::string groupName = "";
        for (int32_t nr = 0; (nr < groupCnt) && os.good(); ++nr)
          {
            CConfigGroup * currGroup = contInterface.getData(nr);
            int32_t        itemCnt   = currGroup->size();

            // Only groups that have anything to save are processed:
            if (itemCnt)
              {
                groupName = contInterface.getItemName(nr);
                PWX_TRY(currGroup->save(os, groupName.c_str(), maxLineLength))
                PWX_THROW_FURTHER
              } // End of group having items
          } // End of going through the groups
        isChanged = false;
      } // end of os is good
  }


/** @brief add / set a key-value-pair and / or a comment
  *
  * This method manipulates an entry of this config file identified
  * by @a group and @a key. For blank lines and pure comment lines
  * there are two special key names. If either
  * pwx::cfh::cfConstants::CFC_BlankLine or CFC_PureComment are set
  * as @a key, this method will create a new entry, and not manipulate
  * existing ones.
  *
  * If the Entry identified by @a key is found, there are two
  * possibilities:
  *
  *  -# The data has an entry resembling the @a key in a shell like
  *     fashion, this means either "$key" or "${key}". The data will
  *     then be added to the already stored data.
  *  -# The data has no entry like that. The data will then be
  *     changed to @a data.
  *
  * Comments are handled the same, according to the given data.
  *
  * If @a group can not be found, something is really wrong and a
  * pwx::mrf::nameNotFound exception is thrown. The group has to
  * exist prior calling this method, please use your CCnfFileHandler
  * instance (or the global CFH) instead then.
  *
  * @param[in] group The name of the group to manipulate
  * @param[in] key Key identifying the entry
  * @param[in] data Data to store in this entry
  * @param[in] comment Optional information in a comment
  * @return The number of data items present for this key
**/
int32_t CConfigFile::setData(const char *group, const char *key, const char *data, const char *comment)
  {
    CConfigGroup *currGroup = NULL;
    PWX_TRY(currGroup = contInterface.getData(group))
    PWX_THROW_FURTHER

    int32_t result = 0;

    if ( STREQ(key, CFC_BlankLine) || STREQ(key, CFC_PureComment) // Those are always added!
      || !currGroup->exists(key) )
      result = currGroup->addKey(key, data, comment);
    else if (data)
      {
        ::std::string newData = data;

        // If we have a shell instance in our data set, it has to be erased, and the data added:
        if (::pwx::StreamHelpers::cropShell(key, newData))
          // Yes, so add the remaining data:
          result = currGroup->addData(key, newData.c_str(), comment);
        else
          // Otherwise simply replace the known data
          result = currGroup->setData(key, data, comment);
      } // End of key exists and data is set
    else
      // If data is NULL, only the comment is set
      result = currGroup->setData(key, NULL, comment);

    isChanged = true;
    lastKey   = currGroup->getLastKey();

    return result;
  }


/** @brief set the current data flags to a new value **/
void CConfigFile::setFlags(cfflags DataFlags)
  {
    if (dataFlags != DataFlags)
      {
        dataFlags = DataFlags;
        isChanged = true;

        // Now tell all data:
        size_t dSize = contInterface.size();
        for (size_t i = 0; i < dSize; ++i)
          contInterface[i]->setFlags(dataFlags);
      }
  }


/** @brief Sort a specific groups keys data
  *
  * This method sorts the data of the key @a key in the group @a group
  * and returns the number of data items held by that key.
  *
  * If @a group can not be found, something is really wrong and a
  * pwx::mrf::nameNotFound exception is thrown. The group has to
  * exist prior calling this method. Please use your CCnfFileHandler
  * instance (or the global CFH) instead then.
  *
  * @param[in] group The name of the group to look into
  * @param[in] key Key identifying the entry to sort
  * @return The number of data items held by this @a group -> @a key.
**/
int32_t CConfigFile::sort(const char *group, const char *key)
  {
    CConfigGroup *currGroup = NULL;
    PWX_TRY(currGroup = contInterface.getData(group))
    PWX_THROW_FURTHER

    isChanged = true;

    return currGroup->sort(key);
  }


/** @brief Sort groups keys and data
  *
  * This is a quick way to sort groups, keys and their data.
  * Groups and keys are sorted by their ID, which is the order in which
  * they are loaded and/or added. Data is sorted alphabetically.
  *
  * @param sortGroups whether to sort groups or not
  * @param sortKeys whether to sort keys or not
  * @param sortData whether to sort the keys data or not
**/
void CConfigFile::sort(bool sortGroups, bool sortKeys, bool sortData)
  {
    if (sortGroups)
      contInterface.sortById();

    if (sortKeys || sortData)
      {
        CConfigGroup * currGroup = NULL;
        int32_t cnt = contInterface.size();
        for (int32_t nr = 0; nr < cnt; ++nr)
          {
            currGroup = contInterface.getData(nr);
            currGroup->sort(sortKeys, sortData);
          }
      }

    if (sortGroups || sortKeys || sortData)
      isChanged = true;
  }


/** @brief Sort All
  *
  * This simply does sort groups, their keys and the keys data.
  * To have a better control use
  * @see sort(bool sortGroups, bool sortKeys, bool sortData)
**/
void CConfigFile::sort()
  {
    sort(true, true, true);
  }


/** @brief make all data items of a specific key unique
  *
  * This method first sorts the data items of @a key in the @a group
  * and then erases doublettes.
  *
  * If @a group can not be found, something is really wrong and a
  * pwx::mrf::nameNotFound exception is thrown. The group has to
  * exist prior calling this method, please use your CCnfFileHandler
  * instance (or the global CFH) instead then.
  *
  * If the @a key does not exist, it will be created.
  *
  * Please note that this method sorts the data whether it is created
  * as being a sorting config file or not.
  *
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @return The number of data items this group->key holds after sorting and filtering.
**/
int32_t CConfigFile::unique(const char *group, const char *key)
  {
    CConfigGroup *currGroup = NULL;
    PWX_TRY(currGroup = contInterface.getData(group))
    PWX_THROW_FURTHER

    isChanged = true;

    return currGroup->unique(key);
  }

} // End of namespace cfh
} // End of namespace pwx

#endif // PWX_CCONFIGFILEIMPL_H_INCLUDED

