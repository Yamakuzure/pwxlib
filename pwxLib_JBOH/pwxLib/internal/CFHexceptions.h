#pragma once
#ifndef PWX_CFH_CFHEXCEPTIONS_H_INCLUDED
#define PWX_CFH_CFHEXCEPTIONS_H_INCLUDED 1

/** @internal
  * @file CFHexceptions.h
  *
  * @brief This file defines the exceptions used by CFH (pwxLib/CCnfFileHandler.h)
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
# error "Do not include Cfhexceptions.h, include CCnfFileHandler.h!"
#endif // Check for main file

#include "pwxLib/tools/Exception.h"

namespace pwx {
namespace cfh {

/* === template === */
///** @brief aDescription
//**/
//class XXX : public ::pwx::Exception
//{
//public:
//  explicit XXX (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
//                       const char *const aDesc): Exception ("XXX", aWhat, aWhere, prettyfunc, aDesc) { }
//  XXX (const XXX &e):Exception(e) { }
//  PWX_INLINE virtual ~XXX() throw();
//};
/* === template === */


/** @brief Neccessary objects (CConfigFile, CConfigGroup or CConfigData) could not be created
**/
class cantCreateObjects : public ::pwx::Exception
{
public:
  explicit cantCreateObjects (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("cantCreateObjects", aWhat, aWhere, prettyfunc, aDesc) { }
  cantCreateObjects (const cantCreateObjects &e):Exception(e) { }
  PWX_INLINE virtual ~cantCreateObjects() throw();
};


/** @brief A file can't be created, because it already exists and shall not be overwritten.
**/
class fileExists : public ::pwx::Exception
{
public:
  explicit fileExists (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("fileExists", aWhat, aWhere, prettyfunc, aDesc) { }
  fileExists (const fileExists &e):Exception(e) { }
  PWX_INLINE virtual ~fileExists() throw();
};


/** @brief A file could not be found but shouldn't be created either.
**/
class fileNotFound : public ::pwx::Exception
{
public:
  explicit fileNotFound (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("fileNotFound", aWhat, aWhere, prettyfunc, aDesc) { }
  fileNotFound (const fileNotFound &e):Exception(e) { }
  PWX_INLINE virtual ~fileNotFound() throw();
};

/** @brief If a bareword is found, the content is illegal as it can not be identified.
**/
class illegalContent : public ::pwx::Exception
{
public:
  explicit illegalContent (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("illegalContent", aWhat, aWhere, prettyfunc, aDesc) { }
  illegalContent (const illegalContent &e):Exception(e) { }
  PWX_INLINE virtual ~illegalContent() throw();
};

/** @brief A given file name is empty or already exists
**/
class illegalFileName : public ::pwx::Exception
{
public:
  explicit illegalFileName (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("illegalFileName", aWhat, aWhere, prettyfunc, aDesc) { }
  illegalFileName (const illegalFileName &e):Exception(e) { }
  PWX_INLINE virtual ~illegalFileName() throw();
};

/** @brief A group name given or read from a file is illegal
**/
class illegalGroupName : public ::pwx::Exception
{
public:
  explicit illegalGroupName (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("illegalGroupName", aWhat, aWhere, prettyfunc, aDesc) { }
  illegalGroupName (const illegalGroupName &e):Exception(e) { }
  PWX_INLINE virtual ~illegalGroupName() throw();
};

/** @brief A given path is empty or unaccessible
**/
class illegalPath : public ::pwx::Exception
{
public:
  explicit illegalPath (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("illegalPath", aWhat, aWhere, prettyfunc, aDesc) { }
  illegalPath (const illegalPath &e):Exception(e) { }
  PWX_INLINE virtual ~illegalPath() throw();
};

/** @brief Initialization of the files container failed
**/
class initFailed : public ::pwx::Exception
{
public:
  explicit initFailed (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("initFailed", aWhat, aWhere, prettyfunc, aDesc) { }
  initFailed (const initFailed &e):Exception(e) { }
  PWX_INLINE virtual ~initFailed() throw();
};

/** @brief reading a config file failed.
**/
class readFailed : public ::pwx::Exception
{
public:
  explicit readFailed (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("readFailed", aWhat, aWhere, prettyfunc, aDesc) { }
  readFailed (const readFailed &e):Exception(e) { }
  PWX_INLINE virtual ~readFailed() throw();
};

/** @brief aDescription
**/
class unknownFileName : public ::pwx::Exception
{
public:
  explicit unknownFileName (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("unknownFileName", aWhat, aWhere, prettyfunc, aDesc) { }
  unknownFileName (const unknownFileName &e):Exception(e) { }
  PWX_INLINE virtual ~unknownFileName() throw();
};

/** @brief writing to a config file failed
**/
class writeFailed : public ::pwx::Exception
{
public:
  explicit writeFailed (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("writeFailed", aWhat, aWhere, prettyfunc, aDesc) { }
  writeFailed (const writeFailed &e):Exception(e) { }
  PWX_INLINE virtual ~writeFailed() throw();
};

// --destructors heaven ---
//XXX::~XXX()           throw() { }
cantCreateObjects::~cantCreateObjects() throw() { }
fileExists::~fileExists()               throw() { }
fileNotFound::~fileNotFound()           throw() { }
illegalContent::~illegalContent()       throw() { }
illegalFileName::~illegalFileName()     throw() { }
illegalGroupName::~illegalGroupName()   throw() { }
illegalPath::~illegalPath()             throw() { }
initFailed::~initFailed()               throw() { }
readFailed::~readFailed()               throw() { }
unknownFileName::~unknownFileName()     throw() { }
writeFailed::~writeFailed()             throw() { }

} // End of namespace cfh

} // End of namespace pwx

#endif // PWX_CFH_CFHEXCEPTIONS_H_INCLUDED

