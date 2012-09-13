#pragma once
#ifndef PWX_CFHTYPES_H_INCLUDED
#define PWX_CFHTYPES_H_INCLUDED

/** @internal
  * @file CfhTypes.h
  *
  * @brief This file defines types used by pwxLib/CCnfFileHandler.h
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
# error "Do not include CnfTypes.h, include CCnfFileHandler.h!"
#endif // Check for main file

/// @namespace pwx
namespace pwx {
/// @namespace cfh
namespace cfh {
// note: The namespaces are documented in CCnfFileHandler.h!

/** @namespace pwx::cfh::cfConstants
  *
  * @brief In this namespace some constants with special meaning are defined.
**/
namespace cfConstants {

/// This group has no group name to be printed into a file upon saving of its data
const char *CFC_NoGroupName = "_PWX_NO_GROUP_NAME_";

/// The line has no content
const char *CFC_BlankLine   = "_PWX_BLANK_LINE_";

/// The line has neither key nor data
const char *CFC_PureComment = "_PWX_PURE_COMMENT_LINE_";

/// The line is a source entry, that is to be interpreted by a shell not CFH
const char *CFC_SourceEntry = "_PWX_SOURCE_ENTRY_";

} // End of namespace cfConstants

/** @namespace pwx::cfh::cfFlags
  *
  * @brief configuration flags for config files
  *
  * In this namespace the configuration flags of the type cfflags are
  * defined. They are used to configure how the content of a configuration
  * file is handled
  *
  * This extra namespace is used to enable the import of all flags without
  * filling in other types and structures.
**/
namespace cfFlags {

/** @enum eConfigFlags
  * @brief base flags for config file presets and configuration
  *
  * These are the basis for the bit mask configuration of the config file
  * handling. Being bit mask values you can expect the right thing to happen
  * when you use the bit operators &, |, ^, ~, &=, |= and ^=.
**/
enum eConfigFlags
{
  CF_CommSepMesh      = 1L <<  0,
  CF_CommSepSemiColon = 1L <<  1,
  CF_CommSepSpace     = 1L <<  2,
  CF_DataSepColon     = 1L <<  3,
  CF_DataSepComma     = 1L <<  4,
  CF_DataSepPipe      = 1L <<  5,
  CF_DataSepSemiColon = 1L <<  6,
  CF_DataSepSpace     = 1L <<  7,
  CF_KeySepAssign     = 1L <<  8,
  CF_KeySepColon      = 1L <<  9,
  CF_KeySepSemiColon  = 1L << 10,
  CF_KeySepSpace      = 1L << 11,
  CF_NoDataEncase     = 1L << 12,
  CF_NoGroups         = 1L << 13,
  CF_NoEmptyGroupName = 1L << 14,
  CF_NoLineWrap       = 1L << 15,
  CF_SortData         = 1L << 16,
  CF_UseSource        = 1L << 17,
  CF_WriteEmptyKey    = 1L << 18,
  CF_File_Config      = CF_CommSepMesh | CF_DataSepComma | CF_KeySepColon | CF_NoGroups | CF_SortData,
  CF_File_INI         = CF_CommSepSemiColon | CF_DataSepComma | CF_KeySepAssign | CF_NoDataEncase
                      | CF_NoEmptyGroupName | CF_NoLineWrap | CF_WriteEmptyKey,
  CF_File_List        = CF_CommSepMesh | CF_DataSepSpace | CF_KeySepSpace | CF_NoDataEncase | CF_NoGroups | CF_NoLineWrap
                      | CF_SortData | CF_WriteEmptyKey,
  CF_File_Rc          = CF_CommSepMesh | CF_DataSepSpace | CF_KeySepAssign | CF_NoGroups | CF_UseSource
                      | CF_WriteEmptyKey
};

inline eConfigFlags
operator&(eConfigFlags lhs, eConfigFlags rhs)
{ return eConfigFlags(static_cast<int64_t>(lhs) & static_cast<int64_t>(rhs)); }

inline eConfigFlags
operator|(eConfigFlags lhs, eConfigFlags rhs)
{ return eConfigFlags(static_cast<int64_t>(lhs) | static_cast<int64_t>(rhs)); }

inline eConfigFlags
operator^(eConfigFlags lhs, eConfigFlags rhs)
{ return eConfigFlags(static_cast<int64_t>(lhs) ^ static_cast<int64_t>(rhs)); }

inline eConfigFlags&
operator|=(eConfigFlags& lhs, eConfigFlags rhs)
{ return lhs = lhs | rhs; }

inline eConfigFlags&
operator&=(eConfigFlags& lhs, eConfigFlags rhs)
{ return lhs = lhs & rhs; }

inline eConfigFlags&
operator^=(eConfigFlags& lhs, eConfigFlags rhs)
{ return lhs = lhs ^ rhs; }

inline eConfigFlags
operator~(eConfigFlags lhs)
{ return eConfigFlags(~static_cast<int64_t>(lhs)); }

// The following is done to easily document (and shorten!) the value names
typedef eConfigFlags cfflags;

/// Comments are separated from the rest of the line using a semicolon ';'
static const cfflags cfCSemicolon = CF_CommSepSemiColon;

/// Comments are separated from the rest of the line using a mesh '#'
static const cfflags cfCMesh      = CF_CommSepMesh;

/// Comments are separated from the rest of the line using a space ' '
static const cfflags cfCSpace     = CF_CommSepSpace;

/// Data items are separated from each other using a colon ':'
static const cfflags cfDColon     = CF_DataSepColon;

/// Data items are separated from each other using a comma ','
static const cfflags cfDComma     = CF_DataSepComma;

/// Data items are separated from each other using a pipe char '|'
static const cfflags cfDPipe      = CF_DataSepPipe;

/// Data items are separated from each other using a semicolon ';'
static const cfflags cfDSemicolon = CF_DataSepSemiColon;

/// Data items are separated from each other using a space ' '
static const cfflags cfDSpace     = CF_DataSepSpace;

/// Keys are separated from their data using an assign char '='
static const cfflags cfKAssign    = CF_KeySepAssign;

/// Keys are separated from their data using a colon ':'
static const cfflags cfKColon     = CF_KeySepColon;

/// Keys are separated from their data using a semicolon ';'
static const cfflags cfKSemicolon = CF_KeySepSemiColon;

/// Keys are separated from their data using a space ' '
static const cfflags cfKSpace     = CF_KeySepSpace;

/// Data items will not be encased in double quotation marks when written
static const cfflags cfNoEncase   = CF_NoDataEncase;

/// The config file does not support groups
static const cfflags cfNoGroups   = CF_NoGroups;

/// The config file is not allowed to have a general group without a name
static const cfflags cfNoEmptyName= CF_NoEmptyGroupName;

/// The config file will not wrap lines when written
static const cfflags cfNoWrap     = CF_NoLineWrap;

/// Data items are sorted by default
static const cfflags cfSort       = CF_SortData;

/// Source entries marked with "." and "source" are processed, too.
static const cfflags cfUseSource  = CF_UseSource;

/// Keys without data are not omitted when writing the file
static const cfflags cfWriteEmpty = CF_WriteEmptyKey;

/// Preset for simple configuration files, format 'key:"data1,data2,..." # comment', no groups supported
static const cfflags cfConfig     = CF_File_Config;

/// Preset for Ini files, format 'key=data1,data2,... ; comment' with groups being mandatory
static const cfflags cfINI        = CF_File_INI;

/// Preset for list configs, format "key data1 data2 ... # comment' without groups and line wrap
static const cfflags cfList       = CF_File_List;

/// Preset for shell rc files, format 'key="data1 data2 ...." # comment', no groups but source entries supported
static const cfflags cfRc         = CF_File_Rc;

} // End of namespace cfFlags

/* --- Used constants and enums --- */
// Import directly:
using namespace cfConstants;
using namespace cfFlags;
// Note: This is done this way to allow users to only import the constants and/or flags at
// need instead of importing the whole namespace. The latter might mean a lot more pollution
// than is wanted. Furthermore the user won't get the eFileStages then, which might be
// confusing as they are completely internal.

/** @enum eFileStages
  * @brief The Stages used for loading/saving files
**/
enum eFileStages
{
  FS_Begin,   //!< A new line is loaded and no unfinished data exists
  FS_Group,   //!< The line consists the starting tag of a new [group]
  FS_Key,     //!< The line starts with a new key that is to be read
  FS_Data,    //!< The line satrts or continues data
  FS_Comment, //!< The line starts a comment
  FS_Finish   //!< All data is read, the line/block is fully read and can be saved
};

} // End of namespace cfh
} // End of namespace pwx

// The following order for the other types is necessary and must not be changed!
#include "pwxLib/internal/CConfigData.h"
#include "pwxLib/internal/CConfigGroup.h"
#include "pwxLib/internal/CConfigFile.h"

#endif // PWX_CFHTYPES_H_INCLUDED

