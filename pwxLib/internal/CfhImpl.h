#pragma once
#ifndef PWX_CFHIMPL_H_INCLUDED
#define PWX_CFHIMPL_H_INCLUDED

/** @internal
  * @file CfhImpl.h
  *
  * @brief In this file are the definitions of the methods declared in pwxLib/CCnfFileHandler.h
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
# error "Do not include CfhImpl.h, include CCnfFileHandler.h!"
#endif // Check for main file

namespace pwx {
namespace cfh {
// note: The namespaces are documented in CCnfFileHandler.h!

/* =========================================== */
/* === Implementations for CCnfFileHandler === */
/* =========================================== */


/** @brief add a comment to a groups key in a specific file
  *
  * This method sets the current file to @a file and the current group
  * to @a group. To this @a group in @a file @a comment is added to the
  * existing comment of the key @a key.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled file
  * to @a file and the currently handled group to @a group.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @param[in] comment The comment to add
  * @return The total length of the comment of this file->group->key.
**/
int32_t CCnfFileHandler::addComment(const char* file, const char* group, const char* key, const char* comment)
{
  if ((currFileName != file) || (currGroup != group))
    setGroup(file, group);

  return addComment(key, comment);
}


/** @brief add a comment to a groups key in the current file
  *
  * This method sets the current group to @a group. To this @a group
  * @a comment is added to the existing comment of the key @a key.
  *
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled group to @a group.
  *
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @param[in] comment The comment to add
  * @return The total length of the comment of this file->group->key.
**/
int32_t CCnfFileHandler::addComment(const char* group, const char* key, const char* comment)
{
  if (currGroup != group)
    setGroup(group);

  return addComment(key, comment);
}


/** @brief add a comment to a key of the current group in the current file
  *
  * This method adds @a comment to the existing comment of the key @a key
  * in the currently set group of the currently set file.
  *
  * If the key does not exist, it will be created.
  *
  * @param[in] key The key to modify
  * @param[in] comment The comment to add
  * @return The total length of the comment of this file->group->key.
**/
int32_t CCnfFileHandler::addComment(const char* key, const char* comment)
{
  return currFile->addData(currGroup.c_str(), key, NULL, comment);
}


/** @brief add data to a groups key in a specific file
  *
  * This method sets the current file to @a file and the current group
  * to @a group. To this @a group in @a file @a data is added to the
  * existing data of the key @a key.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled file
  * to @a file and the currently handled group to @a group.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @param[in] data The data to add
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::addData(const char* file, const char* group, const char* key, const char* data)
{
  if ((currFileName != file) || (currGroup != group))
    setGroup(file, group);

  return addData(key, data);
}


/** @brief add data to a groups key in the current file
  *
  * This method sets the current group to @a group. To this @a group @a data
  * is added to the existing data of the key @a key.
  *
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled group to @a group.
  *
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @param[in] data The data to add
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::addData(const char* group, const char* key, const char* data)
{
  if (currGroup != group)
    setGroup(group);

  return addData(key, data);
}


/** @brief add data to a key of the current group in the current file
  *
  * This method adds @a data to the existing data of the key @a key in the
  * currently set group of the currently set file.
  *
  * If the key does not exist, it will be created.
  *
  * @param[in] key The key to modify
  * @param[in] data The data to add
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::addData(const char* key, const char* data)
{
  return currFile->addData(currGroup.c_str(), key, data, NULL);
}


/** @brief add flags to the specified file
  * This method sets the current file to @a file and adds @a CfFlags to the current
  * file flags.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the given flags interfere with existing flags, the offending flags are
  * removed without further notice. Otherwise you had to first remove the old
  * data separator flag before setting a new one.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] CfFlags Flags to describe the config file. @see cfflags
*/
void CCnfFileHandler::addFlags (const char *file, cfflags CfFlags)
  {
    if (currFileName != file)
      setGroup(file, cfConstants::CFC_NoGroupName);

    currFile->addFlags(CfFlags);
  }


/** @brief close a config file
  *
  * This method closes a config file specified by the internal name @a name.
  * The second optional parameter can be used to first save the file if
  * autosave is turned off.
  *
  * If no such file exists, or no @a name is provided, a
  * pwx::cfh::illegalFileName exception is thrown.
  *
  * @param[in] name The internal name of the config file
  * @param[in] forceSave Set to true to save the file first if autosave is is turned off
  * @return The number of remaining open config files.
**/
int32_t CCnfFileHandler::close(const char *name, bool forceSave)
  {
    if (name && strlen(name))
      {
        // Check whether the name is unknown, which must not occur:
        CConfigFile * file = NULL;
        PWX_TRY(file = contInterface->getData(name))
        PWX_CATCH_AND_FORGET( ::pwx::mrf::nameNotFound )
        if (NULL == file)
          PWX_THROW(illegalFileName, "The given file name is not known", name)

        // Save the file if autosave is true and the file is changed, or if forceSave is true
        if ((autoSave && file->changed()) || forceSave)
          file->save();

        // Now it can be "closed".
        files->lock();
        files->erase(file);
        contInterface->reset();
        files->unlock();
        file = NULL;
        return contInterface->size();
      }
    else
      PWX_THROW(illegalFileName, "You have to provide a config name to save.",
                "CCnfFileHandler::save(const char *name) called without a name.")
  }


/** @brief create a new config file
  *
  * This method creates a new config file according to the arguments. If the file,
  * specified by @a path, already exists, it will be either be overwritten if
  * @a overwrite is set to true, or a pwx::cfh.:fileExists exeption is thrown.
  *
  * If no file @a name is specified, or already exists, the method throws a pwx::cfh::illegalFileName exeption.
  * If no file @a path is specified, or is not writable, the method throws a pwx::cfh::illegalPath exeption.
  * If the creation fails, the method throws a pwx::cfh::cantCreateObjects exception
  *
  * @param[in] name The internal name of the file. pwx::cfh::illegalFileName is thrown if this is wrongly set.
  * @param[in] path Absolute or relative file path. pwx::cfh::illegalPath is thrown if this is wrongly set.
  * @param[in] CFFlags Flags to describe the config file. @see cfflags
  * @param[in] maxLineLength If key + separator + data exceeds this limit, the data will be line wrapped
  * @param[in] doOverwrite Overwrite existing file (true) or throw pwx::cfh::fileExists (false)
  * @return The number of files now known to the system
**/
int32_t CCnfFileHandler::create(const char *name, const char *path, cfflags CFFlags, uint32_t maxLineLength, bool doOverwrite)
  {
    int32_t result = 0;
    assert(path && strlen(path) && "ERROR: CCnfFileHandler::create() called without file path!");
    assert(name && strlen(name) && "ERROR: CCnfFileHandler::create() called without a file name!");

    if (name && strlen(name) && path && strlen(path))
      {
        // Check whether the name is already known, which must not occur:
        int32_t xNr = -1;
        PWX_TRY(xNr = localMRF.getNr(name))
        PWX_CATCH_AND_FORGET( ::pwx::mrf::nameNotFound )
        if (xNr != -1)
          PWX_THROW(illegalFileName, "The given file name already exists", name)
        // Check against doOverwrite flag and file existence.
        if (!doOverwrite && pwx_file_exists(path))
          PWX_THROW(fileExists, "(create) The file already exists.", path)
        // Next we need to know whether we actually can doOverwrite the file if it exists
        if (pwx_file_exists(path) && !pwx_file_isW(path))
          PWX_THROW(illegalPath, "(create) The file is not writable.", path)

        // Now we can get the two standard containers and set up the file:
        try
          {
            // The data container has no initial item
            tDataRing    * dataCont  = localMRF.create(static_cast<CConfigData *>(NULL), CFFlags);
            // The first group gets this empty data container
            CConfigGroup * initGroup = new CConfigGroup(CFC_NoGroupName, dataCont, CFFlags);
            // The group container has a first item named CFC_NoGroupName. the ID is set automatically
            tGroupRing   * groupCont = localMRF.create(initGroup, 0, CFC_NoGroupName);
            // Now the file can be created with this group container
            CConfigFile  * file      = new CConfigFile(name, groupCont, path, maxLineLength, CFFlags);
            // And finally add the file to the files container with its internal name
            result = contInterface->add(file, 0, name);
            // This is set to be the default now of course:
            setGroup(contInterface->getItemName(result++), CFC_NoGroupName);
            // The result has to be increased, as TMemRing::add() returns the number of the new file, not the count.
          }
        catch (::std::bad_alloc &e)
          {
            PWX_THROW(cantCreateObjects, e.what(), "Create() could not create necessary objects")
          }
        PWX_THROW_FURTHER
        // That's it.
      } // End of having a name and path
    else if (name && strlen(name))
      PWX_THROW(illegalPath, "(create) Proper path needed.", "path must neither be empty nor NULL!")
    else
      PWX_THROW(illegalFileName, "(create) Proper file name needed.", "name must neither be empty nor NULL!")

    return result;
  }


/** @brief internal method to create a new group in an existing file.
**/
void CCnfFileHandler::createGroup(const char *file, const char *group)
  {
    CConfigFile * cnfFile = NULL;
    try
      {
        cnfFile = contInterface->getData(file);
        // The new group needs an empty data container
        tDataRing    * dataCont = localMRF.create(static_cast<CConfigData *>(NULL));
        // Now create the group...
        CConfigGroup * newGroup = new CConfigGroup(group, dataCont, cnfFile->getFlags());
        // ... and add it to the file with the proper group name
        cnfFile->addGroup(newGroup, group);
      }
    catch (::std::bad_alloc &e)
      {
        PWX_THROW(cantCreateObjects, e.what(), "createGroup() could not create necessary objects")
      }
    PWX_THROW_FURTHER
  }


/** @brief load config file
  *
  * This method loads config file according to the arguments. If the file,
  * specified by @a path, does not exist, it will be either be created if
  * @a doCreate is set to true, or a pwx::cfh.:fileNotFound exeption is thrown.
  *
  * If no file @a name is specified, or already exists, the method throws a pwx::cfh::illegalFileName exeption.
  * If no file @a path is specified, or is not writable, the method throws a pwx::cfh::illegalPath exeption.
  *
  * @param[in] name The internal name of the file. pwx::cfh::illegalFileName is thrown if this is wrongly set.
  * @param[in] path Absolute or relative file path. pwx::cfh::illegalPath is thrown if this is wrongly set.
  * @param[in] CFFlags Flags to describe the config file. @see cfflags
  * @param[in] maxLineLength If key + separator + data exceeds this limit, the data will be line wrapped
  * @param[in] doCreate Create a new file if it does not exist (true) or throw pwx::cfh::fileNotFound (false)
  * @return The number of files now known to the system
**/
int32_t CCnfFileHandler::load(const char *name, const char *path, cfflags CFFlags, uint32_t maxLineLength, bool doCreate)
  {
    int32_t result = 0;
    assert(path && strlen(path) && "ERROR: CCnfFileHandler::load() called without file path!");
    assert(name && strlen(name) && "ERROR: CCnfFileHandler::load() called without a file name!");

    if (name && strlen(name) && path && strlen(path))
      {
        // Look whether we already know a file with that name, it will be re-loaded.
        CConfigFile * newFile = NULL;
        PWX_TRY(newFile = contInterface->getData(name))
        PWX_CATCH_AND_FORGET(::pwx::mrf::nameNotFound)

        // Set up the file:
        if (newFile)
          newFile->clear();
        else
          {
            // Check against doCreate flag and file existence.
            if (!doCreate && !pwx_file_exists(path))
              PWX_THROW(fileNotFound, "(load) The file could not be found.", path)
            // Next we need to know whether we actually can (over)write the file
            if (!pwx_file_isW(path))
              PWX_THROW(illegalPath, "(load) The file is not writable.", path)
            // The trick here is to let create() do the set up and handle the file stuff only
            PWX_TRY(result = create(name, path, CFFlags, maxLineLength, true))
            PWX_THROW_FURTHER
            PWX_TRY(newFile = contInterface->getData(name))
            PWX_THROW_FURTHER
          }

        // That's it, now open and load the file:
        loadFile();

      } // End of having a name and path
    else if (name && strlen(name))
      PWX_THROW(illegalPath, "(load) Proper path needed.", "path must neither be empty nor NULL!")
    else
      PWX_THROW(illegalFileName, "(load) Proper file name needed.", "name must neither be empty nor NULL!")

    return result;
  }


/** @brief get the comment string from @a key in the group @a group of the file @a file
  *
  * This method sets the currently handled file to @a file and the currently handled
  * group to @a group. It then retrieves the comment string currently held by @a key in
  * that file and group.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  * If the number does not exist, an empty string is returned.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to look into
  * @param[in] key The key from which to retrieve the comment string.
  * @return The comment string of that @a file -> @a group -> @a key.
**/
const char * CCnfFileHandler::getComment(const char *file, const char *group, const char *key)
{
  if ((currFileName != file) || (currGroup != group))
    {
      PWX_TRY(setGroup(file, group))
      PWX_THROW_FURTHER
    }

  return getComment(key);
}


/** @brief get the comment string from @a key in the group @a group of the currently handled file
  *
  * This method sets the currently handled group to @a group. It then retrieves the
  * comment string currently held by @a key in that file and group.
  *
  * If the group and/or the key does not exist, it will be created.
  *
  * @param[in] group The name of the group to look into
  * @param[in] key The key from which to retrieve the comment string.
  * @return The comment string of that @a group -> @a key.
**/
const char * CCnfFileHandler::getComment(const char *group, const char *key)
{
  if (currGroup != group)
    {
      PWX_TRY(setGroup(group))
      PWX_THROW_FURTHER
    }

  return getComment(key);
}


/** @brief get the comment string from @a key in the currently handled group of the currently handled file
  *
  * This method retrieves the comment string currently held by @a key in that file and group.
  *
  * If the key does not exist, it will be created.
  *
  * @param[in] key The key from which to retrieve the comment string.
  * @return The data string of the @a key.
**/
const char * CCnfFileHandler::getComment(const char *key)
{
  return currFile->getComment(currGroup.c_str(), key);
}


/** @brief get the size of the comment from @a key in the group @a group of the file @a file
  *
  * This method sets the currently handled file to @a file and the currently handled
  * group to @a group. It then retrieves the size of the comment currently held
  * by @a key in that file and group.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to look into
  * @param[in] key The key from which to retrieve the comment size.
  * @return The size of the comment this @a file -> @a group-> @a key currently holds.
**/
int32_t CCnfFileHandler::getCommentSize(const char *file, const char *group, const char *key)
{
  if ((currFileName != file) || (currGroup != group))
    {
      PWX_TRY(setGroup(file, group))
      PWX_THROW_FURTHER
    }

  return getCommentSize(key);
}


/** @brief get the size of the comment from @a key in the group @a group of the current file
  *
  * This method sets the currently handled group to @a group. It then retrieves the size
  * of the comment currently held by @a key in that group of the currently set file.
  *
  * If the group and/or the key does not exist, it will be created.
  *
  * @param[in] group The name of the group to look into
  * @param[in] key The key from which to retrieve the comment size.
  * @return The size of the comment this @a group -> @a key currently holds.
**/
int32_t CCnfFileHandler::getCommentSize(const char *group, const char *key)
{
  if (currGroup != group)
    {
      PWX_TRY(setGroup(group))
      PWX_THROW_FURTHER
    }

  return getCommentSize(key);
}


/** @brief get the size of the comment from @a key in the current group of the current file
  *
  * This method retrieves the size of the comment currently held by @a key in the currently
  * set group of the currently set file.
  *
  * If the key does not exist, it will be created.
  *
  * @param[in] key The key from which to retrieve the comment size.
  * @return The size of the comment this @a key currently holds.
**/
int32_t CCnfFileHandler::getCommentSize(const char *key)
{
  return currFile->getCommentSize(currGroup.c_str(), key);
}


/** @brief get the data string from @a key in the group @a group of the file @a file
  *
  * This method sets the currently handled file to @a file and the currently handled
  * group to @a group. It then retrieves the data string currently held by @a key in
  * that file and group.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  * If the number does not exist, an empty string is returned.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to look into
  * @param[in] key The key from which to retrieve the data string.
  * @return The data string of that @a file -> @a group -> @a key.
**/
const char * CCnfFileHandler::getData(const char *file, const char *group, const char *key)
{
  if ((currFileName != file) || (currGroup != group))
    {
      PWX_TRY(setGroup(file, group))
      PWX_THROW_FURTHER
    }

  return getData(key);
}


/** @brief get the data string from @a key in the group @a group of the currently handled file
  *
  * This method sets the currently handled group to @a group. It then retrieves the
  * data string currently held by @a key in that file and group.
  *
  * If the group and/or the key does not exist, it will be created.
  *
  * @param[in] group The name of the group to look into
  * @param[in] key The key from which to retrieve the data string.
  * @return The data string of that @a group -> @a key.
**/
const char * CCnfFileHandler::getData(const char *group, const char *key)
{
  if (currGroup != group)
    {
      PWX_TRY(setGroup(group))
      PWX_THROW_FURTHER
    }

  return getData(key);
}


/** @brief get the data string from @a key in the currently handled group of the currently handled file
  *
  * This method retrieves the data string currently held by @a key in that file and group.
  *
  * If the key does not exist, it will be created.
  *
  * @param[in] key The key from which to retrieve the data string.
  * @return The data string of the @a key.
**/
const char * CCnfFileHandler::getData(const char *key)
{
  return currFile->getData(currGroup.c_str(), key);
}


/** @brief get the number of data items from @a key in the group @a group of the file @a file
  *
  * This method sets the currently handled file to @a file and the currently handled
  * group to @a group. It then retrieves the number of data items currently held
  * by @a key in that file and group.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to look into
  * @param[in] key The key from which to retrieve the data count.
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::getDataCount(const char *file, const char *group, const char *key)
{
  if ((currFileName != file) || (currGroup != group))
    {
      PWX_TRY(setGroup(file, group))
      PWX_THROW_FURTHER
    }

  return getDataCount(key);
}


/** @brief get the number of data items from @a key in the group @a group of the current file
  *
  * This method sets the currently handled group to @a group. It then retrieves the number
  * of data items currently held by @a key in that group of the currently set file.
  *
  * If the group and/or the key does not exist, it will be created.
  *
  * @param[in] group The name of the group to look into
  * @param[in] key The key from which to retrieve the data count.
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::getDataCount(const char *group, const char *key)
{
  if (currGroup != group)
    {
      PWX_TRY(setGroup(group))
      PWX_THROW_FURTHER
    }

  return getDataCount(key);
}


/** @brief get the number of data items from @a key in the current group of the current file
  *
  * This method retrieves the number of data items currently held by @a key in the currently
  * set group of the currently set file.
  *
  * If the key does not exist, it will be created.
  *
  * @param[in] key The key from which to retrieve the data count.
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::getDataCount(const char *key)
{
  return currFile->getDataCount(currGroup.c_str(), key);
}


/** @brief get the data item with the nr @a nr from @a key in the group @a group of the file @a file
  *
  * This method sets the currently handled file to @a file and the currently handled
  * group to @a group. It then retrieves the data item with the nr @a nr currently held
  * by @a key in that file and group.
  *
  * The first data item has the number 0.
  * To count data items from the end, you can use a negative number. A number of -1
  * then would return the last data item.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  * If the number does not exist, an empty string is returned.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to look into
  * @param[in] key The key from which to retrieve the data item.
  * @param[in] nr the number of the item, negative numbers count backwards from the end.
  * @return The data item with the given number or an empty string if @a nr is out of range.
**/
const char * CCnfFileHandler::getDataItem(const char *file, const char *group, const char *key, int32_t nr)
{
  if ((currFileName != file) || (currGroup != group))
    {
      PWX_TRY(setGroup(file, group))
      PWX_THROW_FURTHER
    }

  return getDataItem(key, nr);
}


/** @brief get the data item with the nr @a nr from @a key in the group @a group of the currently handled file
  *
  * This method sets the currently handled group to @a group. It then retrieves the
  * data item with the nr @a nr currently held by @a key in that file and group.
  *
  * The first data item has the number 0.
  * To count data items from the end, you can use a negative number. A number of -1
  * then would return the last data item.
  *
  * If the group and/or the key does not exist, it will be created.
  * If the number does not exist, an empty string is returned.
  *
  * @param[in] group The name of the group to look into
  * @param[in] key The key from which to retrieve the data item.
  * @param[in] nr the number of the item, negative numbers count backwards from the end.
  * @return The data item with the given number or an empty string if @a nr is out of range.
**/
const char * CCnfFileHandler::getDataItem(const char *group, const char *key, int32_t nr)
{
  if (currGroup != group)
    {
      PWX_TRY(setGroup(group))
      PWX_THROW_FURTHER
    }

  return getDataItem(key, nr);
}


/** @brief get the data item with the nr @a nr from @a key in the currently handled group of the currently handled file
  *
  * This method retrieves the data item with the nr @a nr currently held by @a key in that file and group.
  *
  * The first data item has the number 0.
  * To count data items from the end, you can use a negative number. A number of -1
  * then would return the last data item.
  *
  * If the key does not exist, it will be created.
  * If the number does not exist, an empty string is returned.
  *
  * @param[in] key The key from which to retrieve the data item.
  * @param[in] nr the number of the item, negative numbers count backwards from the end.
  * @return The data item with the given number or an empty string if @a nr is out of range.
**/
const char * CCnfFileHandler::getDataItem(const char *key, int32_t nr)
{
  return currFile->getDataItem(currGroup.c_str(), key, nr);
}


/** @brief return the set flags of the specified file
  * This method sets the current file to @a file and returns this files flags.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @return CFFlags Flags that describe the config file. @see cfflags
*/
cfflags CCnfFileHandler::getFlags (const char *file)
  {
    if (currFileName != file)
      setGroup(file, cfConstants::CFC_NoGroupName);

    return currFile->getFlags();
  }


/** @brief return true if this file has been changed after the last save or load
  *
  * This method returns true, if the file has been changed after it has been loaded
  * or saved.
  *
  * Please note that this method sets the currently handled file to @a file and the
  * currently handled group to CFC_NoGroupName.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @return True, if changes have been made, false otherwise.
**/
bool CCnfFileHandler::isChanged(const char *file)
{
  if (currFileName != file)
    {
      PWX_TRY(setGroup(file, CFC_NoGroupName))
      PWX_THROW_FURTHER
    }

  return currFile->changed();
}


/** @brief load the content of the current set file
  *
  * This method (re-)loads the config of the file currently set with
  * setGroup. Already loaded data will not be deleted but overwritten.
  * Therefore, if a file shall be completely re-loaded, the data in memory
  * has to be cleared first.
  *
  * If the file consists of an errornous group name, like with a missing
  * closing square bracket, the method throws pwx::cfh::illegalGroupName.
  *
  * If a bareword is found that can not be identified, a
  * pwx::cfh::illegalContent exceptions is thrown.
  *
  * If reading the input file fails, a pwx::cfh::readFailed exception is
  * thrown.
**/
void CCnfFileHandler::loadFile()
  {
    using ::pwx::StreamHelpers::trim;
    using ::pwx::StreamHelpers::to_string;

    // Note: As this is a private method, we do not need to check
    // currFile, it is set right prior calling this method.
    ::std::ifstream file(currFile->getFileName());

    if (file.is_open())
      {
        ::std::string line      = "";
        ::std::string key       = "";
        ::std::string prevKey   = ""; // Used to attach single comment lines to the previous key
        ::std::string data      = "";
        ::std::string comment   = "";
        size_t        posL      = 0;
        size_t        posR      = 0;
        size_t        lineNum   = 0;
        char          encase    = 0;
        const char    commSep   = currFile->getCommentSep();
        const char    dataSep   = currFile->getDataSep();
        const char    keySep    = currFile->getKeySep();
        bool          isWrapped; // This one is set to true if the last char of a line is a slash.
        bool          isLegal   = true;

        while (file.good())
          {
            eFileStages stage    = FS_Begin;
            bool        isGroup ;
            bool        isSingle; // Set to true for pure comments and blank lines
            ::std::getline(file, line);
            trim(line);
            ++lineNum;

            while (file.good() && (FS_Finish != stage))
              {
                // First switch through preparatory stages.
                switch (stage)
                  {
                    case FS_Begin:
                      /** === Stage FS_Begin ===
                        * ======================
                      **/
                      // In this stage we have to find out which stage follows.
                      if (line.size())
                        {
                          if ('[' == line.at(0))
                            // Simple, a group is started.
                            stage = FS_Group;
                          else if (commSep == line.at(0))
                            // It is a comment line.
                            stage = FS_Comment;
                          else if (encase || isWrapped)
                            // We have data left to read
                            stage = FS_Data;
                          else
                            // If those cases fail, it must be a new key.
                            stage = FS_Key;
                        }
                      else
                        // Just finish on a blank line
                        stage = FS_Finish;
                      break;
                    case FS_Group:
                      /** === Stage FS_Group ===
                        * ======================
                      **/
                      // A group name is everything that is enclosed between square brackets.
                      // First delete the opening bracket:
                      line.erase(0, 1);
                      // Now erase everything from the last closing square bracket on
                      posR = line.find_last_of(']');
                      if (posR != line.npos)
                        line.erase(posR);
                      else
                        isLegal = false;
                      if (line.size())
                        // setGroup(group) will create the container, set up the group and set currGroup.
                        setGroup(line.c_str());
                      else
                        isLegal = false;
                      // Now throw if this isn't going anywhere
                      if (!isLegal)
                        {
                          // ouch... a line like "[]" or "[foo" is a no-no.
                          ::std::string msg = "Illegal group name in line ";
                          msg += to_string(lineNum);
                          msg += " in ";
                          msg += currFile->getFileName();
                          PWX_THROW(illegalGroupName, "load failed!", msg.c_str())
                        }
                      // After this we are finished:
                      stage   = FS_Finish;
                      isGroup = true; // Don't add a blank line, a group was added.
                      break;
                    case FS_Key:
                      /** === Stage FS_Key   ===
                        * ======================
                      **/
                      // What we need here is everything in front of the keySep:
                      posL = line.find_first_of(keySep);
                      if (posL != line.npos)
                        {
                          // We have one, so copy to key and trim it
                          key = line.substr(0, posL);
                          trim(key);
                          prevKey.clear(); // We have a new one...

                          // Now shorten the line and set the next stage:
                          line.erase(0, posL + 1);
                          trim(line);
                          // If we have anything left, it has to be parsed, jumped otherwise
                          if (line.empty())
                            stage = FS_Finish;
                          else
                            stage = FS_Data;
                        }
                      else if (currFile->useSource() && ( (line.substr(0, 2) == ". ") || (line.substr(0, 7) == "source ")) )
                        {
                          posL = line.find_first_of(' ');
                          key = CFC_SourceEntry;
                          line.erase(0, posL + 1);
                          trim(line);
                          stage = FS_Data;
                        }
                      else
                        {
                          // A simple bareword that is no comment does not work if it is no source
                          ::std::string msg = "Illegal bareword found in line ";
                          msg += to_string(lineNum);
                          msg += " in ";
                          msg += currFile->getFileName();
                          msg += "\n >>";
                          msg += line;
                          msg += "<<";
                          PWX_THROW(illegalContent, "load failed!", msg.c_str())
                        }
                      break;
                    case FS_Data:
                      /** === Stage FS_Data  ===
                        * ======================
                      **/
                      // Two possibilities: a comment starts, no comment starts anywhere on this line.
                      // so first we need to know whether there is a commSep:
                      posL = line.find_first_of(commSep);
                      if (posL != line.npos)
                        {
                          // If there is a commSep, the comment has to be cut off of the line:
                          comment += line.substr(posL + 1);
                          // Note: We use operator+=, because comments can be spread by multi line wrapping as well.
                          trim(comment);
                          line.erase(posL); // Cut everything starting with the first commSep
                          trim(line); // Important to neither miss slashes nor encases.
                        }
                      // Now simply add line to data, and check the last char for wrapped lines and encases
                      // if we still have a line to add:
                      if (line.size())
                        {
                          // First: check the first char for an encase:
                          if (!encase && (('\'' == line.at(0)) || ('"' == line.at(0))) )
                            {
                              encase = line.at(0);
                              line.erase(0, 1);
                            }
                          // Second: Check for the last sign to check if this is wrapped.
                          if ('\\' == line.at(line.size() - 1))
                            {
                              isWrapped = true;
                              line.erase(line.size() - 1, 1);
                            }
                          else
                            isWrapped = false;
                          // Third: Check for the last char if this is encased
                          if (encase && (encase == line.at(line.size() - 1)))
                            {
                              encase = 0;
                              line.erase(line.size() - 1, 1);
                            }
                          // add to data and set next stage if neither still encased nor wrapped:
                          if (data.size())
                            data += dataSep;
                          data += line;
                        } // end of having a line for data

                        // finish this!
                        stage = FS_Finish;
                      break;
                    case FS_Comment:
                      /** === Stage FS_Comment ===
                        * ========================
                      **/
                      // As comments are "this line only", it is really easy:
                      posL = line.find_first_not_of(commSep);
                      if (posL != line.npos)
                        {
                          comment = line.substr(posL);
                          trim(comment);
                        }
                      stage = FS_Finish;
                      break;
                    case FS_Finish:
                      /** === Stage FS_Finish ===
                        * =======================
                      **/
                      // Nothing to be done in the last stage, it is not a preparation and done later
                    default:
                      // There is no default.
                      break;
                  } // End of switching preparatory stages
                // performing necessary actions in the finished stage is excluded from the switch.
                if (FS_Finish == stage)
                  {
                    // If this is a group indicator, however, there is nothing to be done.
                    // The same applies to data that is either encased or wrapped by backslash
                    if (!isGroup && !encase && !isWrapped)
                      {
                        // Three possibilities: Key-Value-Pair, pure comment, blank line.
                        if (key.empty())
                          {
                            if (comment.empty())
                              {
                                isSingle = true;
                                key = CFC_BlankLine;
                              }
                            else
                              {
                                if (prevKey.empty())
                                  isSingle = true;
                                key = CFC_PureComment;
                              }
                          }
                        // Now key, data, comment can be added:
                        if (isSingle || prevKey.empty())
                          currFile->setData(currGroup.c_str(), key.c_str(), data.c_str(), comment.c_str());
                        else
                          currFile->addData(currGroup.c_str(), prevKey.c_str(), data.c_str(), comment.c_str());

                        // Update prevKey according to the situation
                        if (isSingle)
                          {
                            prevKey.clear(); // Blank lines are not noted.
                            isSingle = false;
                          }
                        else
                          prevKey = currFile->getLastKey();

                        // Clear old Key:
                        if (key.size())
                          key.clear();

                        // Clear old Data:
                        if (data.size())
                          data.clear();

                        // Clear old Comment:
                        if (comment.size())
                          comment.clear();
                      } // end of not a group
                  } // End of finishing item
              } // End of while not finished

          } // End of while having lines to read
        // Is everything in order?
        if (!file.eof())
          {
            // This means, that something else is wrong appart from reaching EOF.
            ::std::string msg = "Read error in line in line ";
            msg += to_string(lineNum);
            msg += " in ";
            msg += currFile->getFileName();
            PWX_THROW(readFailed, "load failed!", msg.c_str())
          }
        file.close();
      } // end of if file is opened
    else
      {
        // The file could not be opened
        ::std::string msg = "The file ";
        msg += currFile->getFileName();
        msg += " could not be opened ";
        PWX_THROW(readFailed, "load failed!", msg.c_str())
      }
  }


/** @brief remove flags from the specified file
  * This method sets the current file to @a file and removes @a CfFlags from
  * the current file flags.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] CFFlags Flags to describe the config file. @see cfflags
*/
void CCnfFileHandler::removeFlags (const char *file, cfflags CfFlags)
  {
    if (currFileName != file)
      setGroup(file, cfConstants::CFC_NoGroupName);

    currFile->removeFlags(CfFlags);
  }


/** @brief save all files that are marked as being changed.
**/
void CCnfFileHandler::save ()
  {
    uint32_t cnt = contInterface->size();
    if (cnt)
      {
        for (uint32_t i = 0; i < cnt; ++i)
          save(contInterface->getItemName(i));
      }
  }


/** @brief save all files to the given ostream @a os
  *
  * This method can be usefull if you wish to save everything to one specific
  * ostream. Normally this should be used to either merge config files or
  * to print them out to a stream like cout or cerr.
  *
  * Please note that this method does not return a reference to @a os as it
  * is not meant to be a helper for an operator<< implementation.
  *
  * Furthermore the files are saved whether they are marked as being changed
  * or not.
  *
  * If the ostream goes bad, the method simply stops to work.
  *
  * @param[in] os The ostream to save into
**/
void CCnfFileHandler::save (::std::ostream &os)
  {
    uint32_t cnt = contInterface->size();
    if (cnt && os.good())
      {
        for (uint32_t i = 0; os.good() && (i < cnt); ++i)
          save(contInterface->getItemName(i), os);
      }
  }


/** @brief save a specific file
  *
  * This method saves the config file identified by @a name.
  *
  * The file is only saved, if the file is marked as being changed since the
  * last loading or saving.
  *
  * If no such file exists, or no @a name is provided, a
  * pwx::cfh::illegalFileName exception is thrown.
  *
  * @param[in] name The name the config file is identified by internally.
**/
void CCnfFileHandler::save (const char *name)
  {
    if (name && strlen(name))
      {
        // Check whether the name is unknown, which must not occur:
        CConfigFile * file = NULL;
        PWX_TRY(file = contInterface->getData(name))
        PWX_CATCH_AND_FORGET( ::pwx::mrf::nameNotFound )
        if (NULL == file)
          PWX_THROW(illegalFileName, "The given file name is not known", name)
        if (file->changed())
          file->save();
      }
    else
      PWX_THROW(illegalFileName, "You have to provide a config name to save.",
                "CCnfFileHandler::save(const char *name) called without a name.")
  }


/** @brief save a specific file to a given ostream @a os
  *
  * This method saves the config file identified by @a name
  * to the ostream @a os.
  *
  * If no such file exists, or no @a name is provided, a
  * pwx::cfh::illegalFileName exception is thrown.
  *
  * Please note that this method does not return a reference to @a os as it
  * is not meant to be a helper for an operator<< implementation.
  *
  * The file is saved with this method, no matter whether it has been
  * changed or not.
  *
  * If the ostream goes bad, the method simply stops to work.
  *
  * @param[in] name The name the config file is identified by internally.
  * @param[in] os The ostream to save into
**/
void CCnfFileHandler::save (const char *name, ::std::ostream &os)
  {
    if (name && strlen(name) && os.good())
      {
        // Check whether the name is unknown, which must not occur:
        CConfigFile * file = NULL;
        PWX_TRY(file = contInterface->getData(name))
        PWX_CATCH_AND_FORGET( ::pwx::mrf::nameNotFound )
        if (NULL != file)
          file->save(os);
        else
          PWX_THROW(illegalFileName, "The given file name is not known", name)
      }
    else
      PWX_THROW(illegalFileName, "You have to provide a config name to save.",
                "CCnfFileHandler::save(const char *name) called without a name.")
  }


/** @brief set the comment of a groups key in a specific file
  *
  * This method sets the current file to @a file and the current group
  * to @a group. In this @a group in @a file the comment of @a key is
  * set to @a comment.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled file
  * to @a file and the currently handled group to @a group.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @param[in] comment The data to set
  * @return The total length of the comment of this file->group->key.
**/
int32_t CCnfFileHandler::setComment(const char* file, const char* group, const char* key, const char* comment)
{
  if ((currFileName != file) || (currGroup != group))
    {
      PWX_TRY(setGroup(file, group))
      PWX_THROW_FURTHER
    }

  return setComment(key, comment);
}


/** @brief set the comment of a groups key in the current file
  *
  * This method sets the current group to @a group. In this @a group the
  * comment of @a key is set to @a comment.
  *
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled group to @a group.
  *
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @param[in] comment The data to set
  * @return The total length of the comment of this file->group->key.
**/
int32_t CCnfFileHandler::setComment(const char* group, const char* key, const char* comment)
{
  if (currGroup != group)
    setGroup(group);

  return setComment(key, comment);
}


/** @brief set the comment of a key of the current group in the current file
  *
  * This method sets @a comment of the key @a key in the currently set group of
  * the currently set file.
  *
  * If the key does not exist, it will be created.
  *
  * @param[in] key The key to modify
  * @param[in] comment The data to set
  * @return The total length of the comment of this file->group->key.
**/
int32_t CCnfFileHandler::setComment(const char* key, const char* comment)
{
  return currFile->setData(currGroup.c_str(), key, NULL, comment);
}


/** @brief set data to a groups key in a specific file
  *
  * This method sets the current file to @a file and the current group
  * to @a group. To this @a group in @a file the data of @a key is set
  * to @a data.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled file
  * to @a file and the currently handled group to @a group.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @param[in] data The data to set
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::setData(const char* file, const char* group, const char* key, const char* data)
{
  if ((currFileName != file) || (currGroup != group))
    {
      PWX_TRY(setGroup(file, group))
      PWX_THROW_FURTHER
    }

  return setData(key, data);
}


/** @brief set data to a groups key in the current file
  *
  * This method sets the current group to @a group. In this @a group
  * @a key is set to @a date.
  *
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled group to @a group.
  *
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @param[in] data The data to set
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::setData(const char* group, const char* key, const char* data)
{
  if (currGroup != group)
    setGroup(group);

  return setData(key, data);
}


/** @brief set data to a key of the current group in the current file
  *
  * This method sets @a data of the key @a key in the currently set group of
  * the currently set file.
  *
  * If the key does not exist, it will be created.
  *
  * @param[in] key The key to modify
  * @param[in] data The data to set
  * @param[in] sortData Directly sort the data after its addition, defaults to false
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::setData(const char* key, const char* data)
{
  return currFile->setData(currGroup.c_str(), key, data, NULL);
}


/** @brief set the flags of the specified file
  * This method sets the current file to @a file and sets its flags to
  * @a CfFlags.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  *
  * Warning: This can change everything describing a file! But it is possible
  * with this to load an INI file, change the flags and save it as an rc file
  * rather easily.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] CFFlags Flags to describe the config file. @see cfflags
*/
void CCnfFileHandler::setFlags (const char *file, cfflags CfFlags)
  {
    if (currFileName != file)
      setGroup(file, cfConstants::CFC_NoGroupName);

    currFile->setFlags(CfFlags);
  }


/** @brief set data and comment to a groups key in a specific file
  *
  * This method sets the current file to @a file and the current group
  * to @a group. To this @a group in @a file the data of @a key is set
  * to @a data and the comment to @a comment.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled file
  * to @a file and the currently handled group to @a group.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @param[in] data The data to set
  * @param[in] comment The comment to set
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::setKey(const char* file, const char* group, const char* key, const char* data, const char* comment)
{
  if ((currFileName != file) || (currGroup != group))
    {
      PWX_TRY(setGroup(file, group))
      PWX_THROW_FURTHER
    }

  return setKey(key, data, comment);
}


/** @brief set data and comment to a groups key in the current file
  *
  * This method sets the current group to @a group. In this @a group
  * @a key is set to @a date.
  *
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled group to @a group.
  *
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @param[in] data The data to set
  * @param[in] comment The comment to set
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::setKey(const char* group, const char* key, const char* data, const char* comment)
{
  if (currGroup != group)
    setGroup(group);

  return setKey(key, data, comment);
}


/** @brief set data and comment of a key of the current group in the current file
  *
  * This method sets @a data of the key @a key in the currently set group of
  * the currently set file.
  *
  * If the key does not exist, it will be created.
  *
  * @param[in] key The key to modify
  * @param[in] data The data to set
  * @param[in] comment The comment to set
  * @return The number of data items this file->group->key currently holds.
**/
int32_t CCnfFileHandler::setKey(const char* key, const char* data, const char* comment)
{
  return currFile->setData(currGroup.c_str(), key, data, comment);
}


/** @brief sort all data items of a specific key
  *
  * This method sorts the data items of a @a key in the @a group of
  * the config file identified by @a file.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled file
  * to @a file and the currently handled group to @a group.
  * Further more this method sorts the data whether it is created as being
  * a sorting config file or not.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @return The number of data items this @a file -> @a group -> @a key holds after sorting.
**/
int32_t CCnfFileHandler::sort(const char *file, const char *group, const char *key)
{
  if ((currFileName != file) || (currGroup != group))
    {
      PWX_TRY(setGroup(file, group))
      PWX_THROW_FURTHER
    }

  return sort(key);
}


/** @brief sort all data items of a specific key
  *
  * This method sorts the data items of a @a key in the @a group of
  * the current config file.
  *
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled group to @a group.
  * Further more this method sorts the data whether it is created as being
  * a sorting config file or not.
  *
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @return The number of data items this @a group -> @a key holds after sorting.
**/
int32_t CCnfFileHandler::sort(const char *group, const char *key)
{
  if (currGroup != group)
    setGroup(group);

  return sort(key);
}


/** @brief sort all data items of a specific key
  *
  * This method sorts the data items of a @a key in the current
  * group of the current config file.
  *
  * If the key does not exist, it will be created.
  *
  * Further more this method sorts the data whether it is created as being
  * a sorting config file or not.
  *
  * @param[in] key The key to modify
  * @return The number of data items this @a key holds after sorting.
**/
int32_t CCnfFileHandler::sort(const char *key)
{
  return currFile->sort(currGroup.c_str(), key);
}


/** @brief make all data items of a specific key unique
  *
  * This method first sorts the data items of a @a key in the @a group of
  * the config file identified by @a file and then erases doublets.
  *
  * If the file does not exist, a pwx::mrf::unknownFileName exception
  * is thrown.
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled file
  * to @a file and the currently handled group to @a group.
  * Further more this method sorts the data whether it is created as being
  * a sorting config file or not.
  *
  * @param[in] file The internal name of the file as set by load() or create()
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @return The number of data items this @a file -> @a group -> @a key holds after sorting and filtering.
**/
int32_t CCnfFileHandler::unique(const char *file, const char *group, const char *key)
{
  if ((currFileName != file) || (currGroup != group))
    {
      PWX_TRY(setGroup(file, group))
      PWX_THROW_FURTHER
    }

  return unique(key);
}


/** @brief make all data items of a specific key unique
  *
  * This method first sorts the data items of a @a key in the @a group of
  * the current config file and then erases doublets.
  *
  * If the group and/or the key does not exist, it will be created.
  *
  * Please note that this method sets the currently handled group to @a group.
  * Further more this method sorts the data whether it is created as being
  * a sorting config file or not.
  *
  * @param[in] group The name of the group to modify
  * @param[in] key The key to modify
  * @return The number of data items this @a group -> @a key holds after sorting and filtering.
**/
int32_t CCnfFileHandler::unique(const char *group, const char *key)
{
  if (currGroup != group)
    setGroup(group);

  return unique(key);
}


/** @brief make all data items of a specific key unique
  *
  * This method first sorts the data items of a @a key in the current
  * group of the current config file and then erases doublets.
  *
  * If the key does not exist, it will be created.
  *
  * Further more this method sorts the data whether it is created as being
  * a sorting config file or not.
  *
  * @param[in] key The key to modify
  * @return The number of data items this @a key holds after sorting and filtering.
**/
int32_t CCnfFileHandler::unique(const char *key)
{
  return currFile->unique(currGroup.c_str(), key);
}


} // End of namespace cfh
} // End of namespace pwx

#endif // PWX_CFHIMPL_H_INCLUDED

