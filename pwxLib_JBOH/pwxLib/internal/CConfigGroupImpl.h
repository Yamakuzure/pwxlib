#pragma once
#ifndef PWX_CCONFIGGROUPIMPL_H_INCLUDED
#define PWX_CCONFIGGROUPIMPL_H_INCLUDED

/** @internal
  * @file CConfigGroupImpl.h
  *
  * @brief This file defines the methods of CConfigGroup used by CCnfFileHandler
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
# error "Do not include CConfigGroupImpl.h, include CCnfFileHandler.h!"
#endif // Check for main file

namespace pwx {
namespace cfh {
// note: The namespaces are documented in CCnfFileHandler.h!

/* =========================================== */
/* === Implementations for CConfigGroup    === */
/* =========================================== */

/** @brief Add content to an existing key
  *
  * Add @a data and @a comment to the existing key @a key
  *
  * If the @a key does not exist, it will be created.
  *
  * @param[in] key The key to add additional data and comment to
  * @param[in] data The data to add
  * @param[in] comment The comment string to add
  * @return The number of data items present for this key if @a data is set, otherwise the length of the comment.
**/
int32_t CConfigGroup::addData (const char *key, const char *data, const char *comment)
  {
    CConfigData *currData = NULL;
    PWX_TRY(currData = contInterface.getData(key))
    PWX_CATCH_AND_FORGET(::pwx::mrf::nameNotFound)

    int32_t result = 0;

    if (currData)
      {
        // The key is present and can be modified
        result  = currData->add(data, comment);
        lastKey = key;
      }
    else
      // It isn't there? Create it then!
      result = addKey(key, data, comment);

    return result;
  }


/** @brief add a new key with data and comment
  *
  * Add a new @a key with @a data and @a comment
  *
  * If the @a key already exists, the key will be changed. This is wanted
  * behavior, because pure comment lines and blank lines do not have
  * keys to be identified by.
  *
  * @param[in] key The key to add
  * @param[in] data The data to add
  * @param[in] comment The comment string to add
  * @return The number of data items present for this key if @a data is set, otherwise the length of the comment.
**/
int32_t CConfigGroup::addKey (const char *key, const char *data, const char *comment)
  {
    CConfigData *newData = new CConfigData(key, data, comment, dataFlags);
    // Note: TMemRing::add will manipulate "key" until it is unique.
    int32_t newNr = contInterface.add(newData, 0, key);
    lastKey = contInterface.getItemName(newNr);

    return data ? newData->size() : newData->commentSize();
  }


/** @brief get the size of the comment of key @a key
  *
  * If the key does not exist, it will be added with an empty data string
  * and empty comment.
  *
  * @param[in] key the key to retrieve the comment size from
  * @return the size of the comment, which might be 0.
**/
int32_t CConfigGroup::getCommentSize (const char *key)
{
  if (key && strlen(key))
    {
      CConfigData *currData = NULL;

      PWX_TRY(currData = contInterface.getData(key))
      PWX_CATCH_AND_FORGET(::pwx::mrf::nameNotFound)

      if (currData)
        return currData->commentSize();
      else
        addKey(key, NULL, NULL);
    }
  return 0;
}


/** @brief get the comment string of the key @a key
  *
  * If the key does not exist, it will be added with an empty data string
  * and empty comment.
  *
  * @param[in] key the key to retrieve the comment from
  * @return the comment or an empty string if the key had to be added.
**/
const char * CConfigGroup::getComment (const char *key)
{
  if (key && strlen(key))
    {
      CConfigData *currData = NULL;

      PWX_TRY(currData = contInterface.getData(key))
      PWX_CATCH_AND_FORGET(::pwx::mrf::nameNotFound)

      if (currData)
        return currData->getComment();
      else
        addKey(key, NULL, NULL);
    }
  return "";
}


/** @brief get the full data string of the key @a key
  *
  * If the key does not exist, it will be added with an empty data string
  * and empty comment.
  *
  * @param[in] key the key to retrieve the data from
  * @return the data or an empty string if the key had to be added.
**/
const char * CConfigGroup::getData (const char *key)
{
  if (key && strlen(key))
    {
      CConfigData *currData = NULL;

      PWX_TRY(currData = contInterface.getData(key))
      PWX_CATCH_AND_FORGET(::pwx::mrf::nameNotFound)

      if (currData)
        return currData->getData();
      else
        addKey(key, NULL, NULL);
    }
  return "";
}


/** @brief get the count of data itmes of key @a key
  *
  * If the key does not exist, it will be added with an empty data string
  * and empty comment.
  *
  * @param[in] key the key to retrieve the data count from
  * @return the data count, which might be 0.
**/
int32_t CConfigGroup::getDataCount (const char *key)
{
  if (key && strlen(key))
    {
      CConfigData *currData = NULL;

      PWX_TRY(currData = contInterface.getData(key))
      PWX_CATCH_AND_FORGET(::pwx::mrf::nameNotFound)

      if (currData)
        return currData->size();
      else
        addKey(key, NULL, NULL);
    }
  return 0;
}


/** @brief get the data item of the key @a key with the number 0
  *
  * If the key does not exist, it will be added with an empty data string
  * and empty comment.
  * If @a nr is out of range, an empty string is returned.
  *
  * The first data item has the number 0.
  * To count data items backwards from the end, a negative nr @a nr can
  * be used. The last item then has the number -1.
  *
  * @param[in] key the key to retrieve the data item from
  * @param[in] nr the number of the data item counted from the beginning if positive, and from the end if negative.
  * @return the data or an empty string if the key had to be added.
**/
const char * CConfigGroup::getDataItem (const char *key, int32_t nr)
{
  if (key && strlen(key))
    {
      CConfigData *currData = NULL;

      PWX_TRY(currData = contInterface.getData(key))
      PWX_CATCH_AND_FORGET(::pwx::mrf::nameNotFound)

      if (currData)
        return currData->getEntry(nr);
      else
        addKey(key, NULL, NULL);
    }
  return "";
}


/** @brief write this group onto an outstream
  *
  * This method writes a group tag if needed and writes all known items
  * to the given ostream @a os.
  *
  * If the writing failed, a pwx::cfh::writeFailed exception is thrown.
  *
  * @param[in,out] os An ostream to write into.
  * @param[in] groupName The name of this group, groups do not know their "item name" otherwise.
  * @param[in] maxLineLength The maximum length of each line before a line break occurs.
**/
void CConfigGroup::save (::std::ostream &os, const char *groupName, int32_t maxLineLength)
  {
    // If this isn't CFC_NoGroupName, we have to write a group tag first if it is allowed:
    if (STRNE(groupName, CFC_NoGroupName) && !(dataFlags & cfNoGroups))
      os << "[" << groupName << "]" << ::std::endl;

    int32_t itemCnt = contInterface.size();

    // Now we loop through all the groups items and save them according to their ID order
    for (int32_t i = 0; (i < itemCnt) && os.good(); ++i)
      {
        ::std::string key  = getKey(i);
        CConfigData  *data = getData(i);

        PWX_TRY(data->save(os, key.c_str(), maxLineLength))
        PWX_THROW_FURTHER
      } // End of walking through the groups content
  }


/** @brief Change the content represented by a key
  *
  * Change the data and comment of the @a key to @a data and @a comment
  *
  * If the @a key does not exist, it will be created.
  *
  * @param[in] key The key to set data in
  * @param[in] data The data to set unless it is NULL, then only the comment is replaced
  * @param[in] comment The comment string to set
  * @return the number of data items held by this key or the length of the comment if @a data is NULL.
**/
int32_t CConfigGroup::setData (const char *key, const char *data, const char *comment)
  {
    CConfigData *currData = NULL;
    PWX_TRY(currData = contInterface.getData(key))
    PWX_CATCH_AND_FORGET(::pwx::mrf::nameNotFound)

    int32_t result = 0;

    if (currData)
      {
        // The key is present and can be changed
        result  = currData->set(data, comment);
        lastKey = key;
      }
    else
      // It isn't there? Create it then!
      result = addKey(key, data, comment);

    return result;
  }


/** @brief set the dataFlags to @a DataFlags **/
void CConfigGroup::setFlags(cfflags DataFlags)
  {
    if (dataFlags != DataFlags)
      {
        dataFlags = DataFlags;

        // Now tell all data:
        size_t dSize = contInterface.size();
        for (size_t i = 0; i < dSize; ++i)
          contInterface[i]->setFlags(dataFlags);
      }
  }

/** @brief Sort the data of a specific key
  *
  * @param[in] key The key to sort the data items in
  * @return the number of data items held by this key.
**/
int32_t CConfigGroup::sort (const char *key)
  {
    CConfigData *currData = NULL;
    PWX_TRY(currData = contInterface.getData(key))
    PWX_CATCH_AND_FORGET(::pwx::mrf::nameNotFound)

    int32_t result = 0;

    if (currData)
      {
        // The key is present and can be sorted
        currData->sort();
        result = currData->size();
      }
    else
      // It isn't there? Create it then!
      result = addKey(key, NULL, NULL);

    return result;
  }


/** @brief Sort keys and / or data
  *
  * @param[in] sortKeys whether to sort keys by their ID or not
  * @param[in] sortData whether to sort the keys data alphabetically or not.
**/
void CConfigGroup::sort (bool sortKeys, bool sortData)
  {
    if (sortKeys)
      contInterface.sortById();

    if (sortData)
      {
        // Loop through all items and let them sort their data
        CConfigData * currData = NULL;
        int32_t cnt = contInterface.size();
        for (int32_t nr = 0; nr < cnt; ++nr)
          {
            currData = contInterface.getData(nr);
            currData->sort();
          }
      }
  }


/** @brief make all data items of a specific key unique
  *
  * This method first sorts the data items of @a key and then erases
  * doublettes.
  *
  * If the @a key does not exist, it will be created.
  *
  * Please note that this method sorts the data whether it is created
  * as being a sorting config file or not.
  *
  * @param[in] key The key to modify
  * @return The number of data items this key holds after sorting and filtering.
**/
int32_t CConfigGroup::unique(const char *key)
  {
    CConfigData *currData = NULL;
    PWX_TRY(currData = contInterface.getData(key))
    PWX_CATCH_AND_FORGET(::pwx::mrf::nameNotFound)

    int32_t result = 0;

    if (currData)
      // The key is present and can be changed
      result = currData->unique();
    else
      // It isn't there? Create it then!
      result = addKey(key, "", "");

    return result;
  }

} // End of namespace cfh
} // End of namespace pwx

#endif // PWX_CCONFIGGROUPIMPL_H_INCLUDED


