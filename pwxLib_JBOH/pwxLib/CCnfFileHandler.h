#pragma once
#ifndef PWX_CCNFFILEHANDLER_H_INCLUDED
#define PWX_CCNFFILEHANDLER_H_INCLUDED 1

/** @file CCnfFileHandler.h
  *
  * @brief Declaration of CCnfFileHandler.
  *
  * <B>Important</B>: If you plan to use CFH in a multi-threaded environment, please make sure
  * the macro "PWX_THREADS" is defined <I>before</I> including this file!
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
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.0.1     2011-03-04  sed, PrydeWorX  First Design
  * 0.0.2     2011-03-06  sed, PrydeWorX  Moved from tools to main as IFH instead of being
  *                                       just a set of global functions.
  * 0.0.3     2011-04-11  sed, PrydeWorX  Renamed to CCnfFileHandler (CFH) as the system is not
  *                                       limited to ini files.
  * 0.0.4     2011-04-20  sed, PrydeWorX  finished base layout.
  * 0.1.0     2011-06-03  sed, PrydeWorX  finished first working version with all base functions added
  * 0.2.0     2011-06-10  sed, PrydeWorX  added unique() methods to have data item doublettes being filtered out
  * 0.3.0     2011-06-16  sed, PrydeWorX  added getData(), getDataCount() and getDataItem()
  * 0.3.1     2011-06-17  sed, PrydeWorX  added getComment() and getCommentSize()
  * 0.4.0     2011-07-03  sed, PrydeWorX  added config flags, those are easier to use than a myriad of function arguments
  * 0.4.1     2011-07-04  sed, PrydeWorX  added config wrappers for INI, RC, CONFIG and LIST files.
  * 0.4.2     2011-07-05  sed, PrydeWorX  corrected loading and saving of data. All test files seem to be clean now.
  * 0.5.0     2011-07-08  sed, PrydeWorX  comment lines below a regular key are now correctly added to that key.
  * 0.5.1     2011-07-14  sed, PrydeWorX  added CFH.close() to close open config files.
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0.1   2011-08-02  sed, PrydeWorX  Added missing PWX_INLINE to the CFH exceptions dtors
  * 0.8.0.2   2011-08-16  sed, PrydeWorX  Switched to MRInterface where possible and added container locks everywhere else
  *                                       to ensure thread safety as long as CFH is used.
  * 0.8.0.3   2011-08-23  sed, PrydeWorX  Added operator> to CConfigFile, CConfigGroup and CConfigData to be able to
  *                                       utilize the new sorting methods of MRF.
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3.1   2012-01-06  sed, PrydeWorX  Fixed the sorting issue
  * 0.8.3.2   2012-01-07  sed, PrydeWorX  Added the possibility to change the dataFlags of a config file on the fly
  * 0.8.3.3   2012-01-08  sed, PrydeWorX  Cleaned up the API, it is no longer possible (and never was necessary) to have
  *                                       added/changed data to be sorted - the flags are responsible for that.
  * 0.8.3.4   2012-01-09  sed, PrydeWorX  Fixed some minor issues cppcheck moaned about.
  * 0.8.5     2012-03-01  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5.1   2012-03-20  sed, PrydeWorX  Fixed a memory leak - CCnfFileHandler::contInterface was not deleted
  * 0.8.5.2   2012-03-22  sed, PrydeWorX  Fixed a bug that caused the removal of data flags from a file to remove all flags
  * 0.8.5.3   2012-04-03  sed, PrydeWorX  Fixed two doxygen issues with the operator<< methods on ostream for groups and data
  * 0.8.5.4   2012-04-17  sed, PrydeWorX  Fixed two annoying issues with VC++10
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  *
  @endverbatim
**/

#include "pwxLib/internal/MSVCwarnPush.h"
#include <string>
#include <list>
#include <limits>
#include "pwxLib/internal/MSVCwarnPop.h"

#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
// Add MRF without an instance, we build our own to not clash with other instances
#  ifndef PWX_NO_MRF_INSTANCE
#    define CNFFILE_REMOVE_NO_INSTANCE
#    define PWX_NO_MRF_INSTANCE
#  endif // define no instance
#  include "pwxLib/CMemRingFactory.h"
#endif // has no MRF yet
#include "pwxLib/tools/StreamHelpers.h"
#include "pwxLib/internal/CFHexceptions.h"

#include "pwxLib/internal/CfhTypes.h"

/** @namespace pwx
  *
  * @brief Namespace for everything PrydeWorX related
  *
  * All PrydeWorX projects reside in this namespace and it's sub-namespaces.
**/
namespace pwx
{

/** @namespace pwx::cfh
  *
  * @brief Namespace for CCnfFileHandler
  *
  * <B>Important</B>: If you plan to use CFH in a multi-threaded environment, please make sure
  * the macro "PWX_THREADS" is defined <I>before</I> including CCnfFileHandler.h!
  *
  * This Namespace is the home for the [C]onfig[F]ile[H]andler CFH, which is a static
  * instance of CCnfFileHandler.
  * If you do not wish the automatic instance to be generated, define
  * PWX_NO_CFH_INSTANCE before including this file.
  *
  * The workflow is as follows:
  *
  * -# Use CFH.create(name, path, CFFlags, maxLineLength, doOverwrite) to
  *    create a new config file
  *     - @a name         : A name to identify the file internally.
  *     - @a path         : The actual file to load or to create.
  *     - @a CFFlags      : Flags defining the rules about how a config file is structured
  *     - @a maxLineLength: If data exceeds the number of characters set here, data
  *       is saved in multiple lines. The key and the separator are counted and indented
  *       on the next line. The default is 80 chars. A value of 0 disables the max line
  *       length limit.
  *     - @a doOverwrite  : If set to true, an existing file at that path is overwritten.
  *       if set to false, an existing file at that path leads to a
  *       pwx::cfh::fileExists exception to be thrown.
  *     .
  *    Instead of specifying all flags by yourself, you can use four preset combinations as follows:
  *      - cfConfig : Simple configuration file, format 'key:"data1,data2,..." # comment', no groups supported
  *      - cfINI    : Ini files, format 'key=data1,data2,... ; comment' with groups being mandatory (*)
  *      - cfList   : List format configs, format "key data1 data2 ... # comment' without groups and line wrap
  *      - cfRc     : Shell rc files, format 'key="data1 data2 ...." # comment', no groups but source entries supported
  *    (*): INI files have no multiple line feature. They might, but only regarding Perl's
  *         Config::IniFiles module with the Unix "Here-Document"-Syntax. Windows wouldn't
  *         like that.
  * -# Use CFH.load(name, path, CFFlags, maxLineLength, doCreate)
  *    to add a config file to the system.
  *     - @a name         : A name to identify the file internally.
  *     - @a path         : The actual file to load or to create.
  *     - @a CFFlags      : Flags defining the rules about how a config file is structured
  *     - @a maxLineLength: If data exceeds the number of characters set here, data
  *       is saved in multiple lines. The key and the separator are counted and indented
  *       on the next line. The default is 80 chars. A value of 0 disables the max line
  *       length limit.
  *     - @a doCreate     : If set to true, a new file will be created if none can be found
  *       at the given @a path. If set to false, a non-existant file leads to a
  *       pwx::cfh::fileNotFound exception to be thrown.
  *     .
  *    Instead of specifying all flags by yourself, you can use the same presets as with create().
  *
  *  -# Use CFH.setGroup([name, ]group) to set in which group of which file
  *     you are interested in.
  *     - @a name : The internal name of the file. If omitted, the current file is used.
  *     - @a group : The group within this file. If this group does not exist, it will
  *       be created.
  *
  *  -# Retrieve data:
  *     - Get data by using CFH.getData(key) to get the data from the set name and group.
  *     - Get data by using CFH.getData(group, key) to get the data from the set
  *       name and the specific group.
  *     - Get data by using CFH.getData(name, group, key) to get data from
  *       any config file and group.
  *     The same scheme can be used to get single data items, if a config line holds multiple values:
  *     - Get the count of data items with CFH.getDataCount([[name,] group,] key)
  *     - Get one data item with CFH.getDataItem([[name,] group,] key, index number)
  *
  *  -# Changing and setting data:
  *     - Set data by using CFH.setData(key, data) to set the data of the set name and group.
  *     - Set data by using CFH.setData(group, key, data) to set the data of the set
  *       name and the specific group.
  *     - Set data by using CFH.setData(name, group, key, data) to set data of
  *       any config file and group.
  *     Note: all setData() methods have an optional parameter with which you can set a char to define
  *           how data items are separated. The default is to not change the currently set value.
  *
  *  -# Adding data to a key:
  *     - Add to existing data by using CFH.addData(key, data) to add to the data of the set name and group.
  *     - Add to existing data by using CFH.addData(group, key, data) to add to the data of the set
  *       name and the specific group.
  *     - Add to existing data by using CFH.addData(name, group, key, data) to add to the data of
  *       any config file and group.
  *     Note: all addData() methods have an optional parameter with which you can set a char to define
  *           how data items are separated. The default is to not change the currently set value.
  *
  *  -# To retrieve, change or set comments, the methods CFH.getComment(...), CFH.setComment(...) and
  *     CFH.addComment(...)  work like CFH.getData(...), CFH.addData(...) and CFH.setData(...) but with
  *     comments. Comments are written behind the data they belong to and can stretch over multiple lines
  *     as well. If they do use multiple lines, each line is started with a "commenter" char.
  *
  *  -# To change both the data and the comment at once, you can use CFH.setKey([[name,] group,] key, data, comment).
  *
  *  -# Save config file
  *     - With CFH.save(name) you can save a specific file identified by the set @a name.
  *     - If the name is omitted, CFH.save() saves all opened files.
  *     - Alternatively you can set CFH to automatically save all open files upon its destruction via
  *       CFH.setAutoSave(bool).
  *     .
  *     Warning: If you use CFH.setAutoSave(false) to disable automatic saving, all changes not
  *     saved when CFH is destroyed will be lost without further notion. For this reason the
  *     default is to turn automatic saving on.
  *
  * One note about thread safety:
  * Although the classes use pwx::mrf::MRInterface wherever possible and lock the containers
  * saving files, groups and keys otherwise, thread safety is only ensured as long as CFH is
  * used to handle all data.
  *
**/
namespace cfh
{

/* --- Used types: --- */
using ::pwx::mrf::CMemRingFactory;
using ::pwx::mrf::MRInterface;

/** @class CCnfFileHandler
  * @brief Management class for config files
  *
  * This class is used to manage all opened, created and manipulated config files.
  * The following key methods are used for the handling and management:
  *
  * -# create(name, path, CFFlags, maxLineLength, doOverwrite) is used to
  *    explicitedly create a new config file. Instead of specifying all @a CFFlags by
  *    hand, you can use four preset combinations.
  *    If the file already exists, it will be overwritten if @a overwrite is true, or
  *    a pwx::cfh::fileExists exception is thrown.
  *
  * -# load(name, path, CFFlags, maxLineLength, doCreate) is used to load an existing
  *    config file. Instead of specifying all @a CFFlags by hand, you can use the same
  *    four preset combinations as with create().
  *    If the file does not exist, it will be created if @a create is set to true, or
  *    a pwx::cfh::fileNotFound exception is thrown.
  *
  * -# setGroup([name, ]group) is used to set a name and a group to work with. The data
  *    manipulation methods then do not need this information. The name is the internal
  *    name, not the filename. (@see create() and load())
  *
  * -# get/setData/Comment/Key can be used to manipulate the data and comments stored in the
  *    config file.
  *
  * -# save(name) can be used to save one file. If the name is omitted, all files are
  *    saved.
  *
  * One note about groups:
  * The constant pwx::cfh::cfConstants::CFC_NoGroupName can be used to declare that keys are
  * managed without a preceding "[GroupName]" tag. This is illegal for regular Inifiles, but
  * allows the handling of shell-scripts that are sourced as configuration files and thereby
  * have no such groups.
  *
  * If the initialization of the files container fails, the constructor throws a
  * pwx::cfh::initFailed exception.
  *
  * One note about thread safety:
  * Although the classes use pwx::mrf::MRInterface wherever possible and lock the containers
  * saving files, groups and keys otherwise, thread safety is only ensured as long as CFH is
  * used to handle all data.
  *
  * @see pwx::cfh for a more detailed workflow.
**/
class PWX_VISIBLE CCnfFileHandler: private ::pwx::Uncopyable
{
  /* --- private members --- */
  bool            autoSave;      //!< Whether or not to use autoSave by default
  CConfigFile    *currFile;      //!< The CConfigFile instance of the currently used file
  ::std::string   currFileName;  //!< The name of the currently used config file
  ::std::string   currGroup;     //!< Currently used group in the current config file
  CMemRingFactory localMRF;      //!< We use our own instance to not clash with user instances.
  tFileRing      *files;         //!< Container with open config files.
  tFileInterface *contInterface; //!< Thread safe interface to the config files container.

public:
  /* --- ctor/dtor --- */
  /** @brief default ctor, setting up the default values. **/
  explicit CCnfFileHandler():
      autoSave(true), currFile(NULL), currFileName(""), currGroup(CFC_NoGroupName), localMRF(true,true),
      files(NULL), contInterface(NULL)
    {
      try
        {
          files = localMRF.create(static_cast<CConfigFile *>(NULL));
          contInterface = new tFileInterface(files);
        }
      catch (::pwx::Exception &e)
        {
          PWX_THROW(initFailed, "Initialization failed", e.desc());
        }
      catch (::std::bad_alloc &e)
        {
          PWX_THROW(initFailed, "Initialization failed", e.what());
        }
    }
  /** @brief default dtor which does the autosave if wanted **/
  ~CCnfFileHandler()
    {
      if (autoSave)
        save();
      if (contInterface)
        delete contInterface;
    }

  /* --- indirect inline methods --- */
  PWX_INLINE int32_t      addComment    (const char *file, const char *group, const char *key, const char *comment);
  PWX_INLINE int32_t      addComment    (const char *group, const char *key, const char *comment);
  PWX_INLINE int32_t      addComment    (const char *key, const char *comment);
  PWX_INLINE int32_t      addData       (const char *file, const char *group, const char *key, const char *data);
  PWX_INLINE int32_t      addData       (const char *group, const char *key, const char *data);
  PWX_INLINE int32_t      addData       (const char *key, const char *data);
  PWX_INLINE void         addFlags      (const char *file, cfflags CfFlags);
  PWX_INLINE int32_t      close         (const char *name, bool forceSave);
  PWX_INLINE int32_t      create        (const char *name, const char *path, cfflags CfFlags, uint32_t maxLineLength,
                                         bool doOverwrite);
  PWX_INLINE const char * getComment    (const char *file, const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE const char * getComment    (const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE const char * getComment    (const char *key) PWX_WARNUNUSED;
  PWX_INLINE int32_t      getCommentSize(const char *file, const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE int32_t      getCommentSize(const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE int32_t      getCommentSize(const char *key) PWX_WARNUNUSED;
  PWX_INLINE const char * getData       (const char *file, const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE const char * getData       (const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE const char * getData       (const char *key) PWX_WARNUNUSED;
  PWX_INLINE int32_t      getDataCount  (const char *file, const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE int32_t      getDataCount  (const char *group, const char *key) PWX_WARNUNUSED;
  PWX_INLINE int32_t      getDataCount  (const char *key) PWX_WARNUNUSED;
  PWX_INLINE const char * getDataItem   (const char *file, const char *group, const char *key, int32_t nr) PWX_WARNUNUSED;
  PWX_INLINE const char * getDataItem   (const char *group, const char *key, int32_t nr) PWX_WARNUNUSED;
  PWX_INLINE const char * getDataItem   (const char *key, int32_t nr) PWX_WARNUNUSED;
  PWX_INLINE cfflags      getFlags      (const char *file) PWX_WARNUNUSED;
  PWX_INLINE bool         isChanged     (const char *file);
  PWX_INLINE int32_t      load          (const char *name, const char *path, cfflags CFFlags, uint32_t maxLineLength,
                                         bool doCreate) PWX_WARNUNUSED;
  PWX_INLINE void         removeFlags   (const char *file, cfflags CfFlags);
  PWX_INLINE int32_t      setComment    (const char *file, const char *group, const char *key, const char *comment);
  PWX_INLINE int32_t      setComment    (const char *group, const char *key, const char *comment);
  PWX_INLINE int32_t      setComment    (const char *key, const char *comment);
  PWX_INLINE int32_t      setData       (const char *file, const char *group, const char *key, const char *data);
  PWX_INLINE int32_t      setData       (const char *group, const char *key, const char *data);
  PWX_INLINE int32_t      setData       (const char *key, const char *data);
  PWX_INLINE void         setFlags      (const char *file, cfflags CfFlags);
  PWX_INLINE int32_t      setKey        (const char *file, const char *group, const char *key, const char *data, const char *comment);
  PWX_INLINE int32_t      setKey        (const char *group, const char *key, const char *data, const char *comment);
  PWX_INLINE int32_t      setKey        (const char *key, const char *data, const char *comment);
  PWX_INLINE void         save          ();
  PWX_INLINE void         save          (::std::ostream &os);
  PWX_INLINE void         save          (const char *name);
  PWX_INLINE void         save          (const char *name, ::std::ostream &os);
  PWX_INLINE int32_t      sort          (const char *file, const char *group, const char *key);
  PWX_INLINE int32_t      sort          (const char *group, const char *key);
  PWX_INLINE int32_t      sort          (const char *key);
  PWX_INLINE int32_t      unique        (const char *file, const char *group, const char *key);
  PWX_INLINE int32_t      unique        (const char *group, const char *key);
  PWX_INLINE int32_t      unique        (const char *key);


  /* --- direct inline methods --- */

  /** @brief get the current auto save state. **/
  bool getAutoSave()           const { return autoSave; }

  /** @brief set the auto save state to @a aAutoSave **/
  void setAutoSave(bool aAutoSave)   { autoSave = aAutoSave; }

  /** @brief set current group
    *
    * Set the handling of get/setData/Comment to the given @a group
    * of the current file.
    *
    * If the current file does not have such a group, it will be created.
    * If there is no current file, this method does nothing.
    *
    * @param[in] group The name of the group to use
  **/
  void setGroup(const char *group)
    {
      if (group && strlen(group) && (currGroup != group))
        {
          if (!currFile->hasGroup(group))
            createGroup(currFileName.c_str(), group);
          currGroup = group;
        }
    }

  /** @brief set current file and group
    *
    * Set the handling of get/setData/Comment to the given @a file and
    * @a group.
    *
    * If there is no file with the given name known, a
    * pwx::cfh::unknownFileName exception is thrown.
    *
    * @param[in] file The internal name of the file to be used
    * @param[in] group The name of the group to use
  **/
  void setGroup(const char *file, const char *group)
    {
      if (file && strlen(file) && contInterface->exists(file))
        {
          if (currFileName != file)
            currFileName = file;
          PWX_TRY(currFile = contInterface->getData(file))
          catch (::pwx::mrf::nameNotFound &e)
            {
              PWX_THROW(unknownFileName, "The given file is unknown", e.desc())
            }
          setGroup(group);
        }
    }

private:
  /* --- indirect inline methods --- */
  PWX_INLINE void createGroup(const char *file, const char *group);
  PWX_INLINE void loadFile();

  /* --- direct inline methods --- */

  /* --- copying is not allowed at all: --- */
  CCnfFileHandler(CCnfFileHandler&);
  CCnfFileHandler &operator=(CCnfFileHandler&);
};


} // End of namespace cfh

#ifndef PWX_NO_CFH_INSTANCE
static cfh::CCnfFileHandler CFH; //!< Static instance of CCnfFileHandler to be used
#endif // PWX_NO_SCT_INSTANCE

} // End of namespace pwx

/* --- Implementation: --- */
#include "pwxLib/internal/CfhImpl.h"
#include "pwxLib/internal/CConfigDataImpl.h"
#include "pwxLib/internal/CConfigGroupImpl.h"
#include "pwxLib/internal/CConfigFileImpl.h"

#if defined(CNFFILE_REMOVE_NO_INSTANCE)
#  undef CNFFILE_REMOVE_NO_INSTANCE
#  undef PWX_NO_MRF_INSTANCE
#endif // clean up

#endif // PWX_CCNFFILEHANDLER_H_INCLUDED


