#pragma once
#ifndef PWX_ARGS_H_INCLUDED
#define PWX_ARGS_H_INCLUDED 1

/** @file Args.h
  *
  * @brief This file defines some tools to work with program arguments.
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
  * 0.0.1     2011-01-22  sed, PrydeWorX  First Design
  * 0.7.0     2011-02-28  sed, PrydeWorX  First working version and version bump
  * 0.7.0.1   2011-03-02  sed, PrydeWorX  Arguments no longer error out when they have mandatory components
  *                                       but haven't been set on the command line.
  * 0.7.0.2   2011-03-09  sed, PrydeWorX  Added named versions of addArg<name>(), addArg() is now an overloaded
  *                                       short version alias of those.
  *                                       Added two new types ETT_ADD and ETT_SUB to allow to add up components.
  * 0.7.0.3   2011-03-11  sed, PrydeWorX  Corrected an issue with the callback arguments
  *                                       Added functions for int32_t, int64_t and double arguments
  * 0.7.0.4   2011-03-12  sed, PrydeWorX  Changed the data storage from "map" to "MRF".
  * 0.7.0.5   2011-03-25  sed, PrydeWorX  Generalized argMap::checkComponents() - much shorter and easier now!
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Some restructuring for the documentation, some corrections and version bump to
  *                                       the new pwxLib release.
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version.
  * 0.8.0.1   2011-08-13  sed, PrydeWorX  addArgFloat/Double now uses correct limits instead of general int32_t.
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3.1   2012-01-05  sed, PrydeWorX  min/max members renamed to minVal/maxVal, it is much clearer like that
  * 0.8.5     2012-03-01  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5.1   2012-03-22  sed, PrydeWorX  Fixed various issues
  * 0.8.5.2   2012-04-12  sed, PrydeWorX  Changed to use the new wrappers for strncpy and the new string stream converters
  *                                       to_int32, to_int64, to_float and to_double instead atoi and atof.
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
// Add MRF without an instance, we build our own to not clash with other instances
#  ifndef PWX_NO_MRF_INSTANCE
#    define ARGS_REMOVE_NO_INSTANCE
#    define PWX_NO_MRF_INSTANCE
#  endif // define no instance
#  include "pwxLib/CMemRingFactory.h"
#endif // has no MRF yet

#include "pwxLib/internal/ArgTypes.h"


/** @namespace pwx
  *
  * @brief Namespace for everything PrydeWorX related
  *
  * All PrydeWorX projects reside in this namespace and it's sub-namespaces.
**/
namespace pwx
{

/** @namespace pwx::args
  *
  * @brief Namespace for global argument parsing helper functions
  *
  * The functions in this namespace are used to load up all program
  * arguments through a small number of functions.
  *
  * The workflow is as follows:
  *
  *  a) Use addArg(...) for every argument/option you wish to be recognized
  *
  *  b) Use loadArgs(argc, argv) to have the command line arguments parsed
  *
  *  c) Process all loaded arguments by using procArgs()
  *
  *  d) If you need space, you can clear up with clearArgs()
  *
  * To get help strings of an argument, you can use getArgHelp(...) to get a
  * malloc'd C-String, or printArgHelp(ostream, ...) on an ostream to print
  * the help text out directly.
  *
  * @see pwx::args::base for base classes used in this namespace.
  * @see pwx::args::constants for enum values and helper constants used in this namespace
**/
namespace args
{

/* --- Function prototypes --- */

// These are the real functions, adding args
int32_t addArgCb    (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                     int32_t aCompc, const char * aCompName, void (*aCb)(const char*, void*), void *aUserData);
int32_t addArgBool  (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                     bool * aTarget, eTargets aType);
int32_t addArgInt32 (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                     int32_t aCompc, const char * aCompName, int32_t * aTarget, eTargets aType, int32_t aMin,
                     int32_t aMax);
int32_t addArgInt64 (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                     int32_t aCompc, const char * aCompName, int64_t * aTarget, eTargets aType, int64_t aMin,
                     int64_t aMax);
int32_t addArgFloat (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                     int32_t aCompc, const char * aCompName, float * aTarget, eTargets aType, float aMin,
                     float aMax);
int32_t addArgDouble(const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                     int32_t aCompc, const char * aCompName, double * aTarget, eTargets aType, double aMin,
                     double aMax);
int32_t addArgChar  (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                     int32_t aCompc, const char * aCompName, char * aTarget, size_t maxSize, eTargets aType);
int32_t addArgString(const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                     int32_t aCompc, const char * aCompName, ::std::string * aTarget, eTargets aType);

// Other functions:
void            clear         ();
void            clearErrors   ();
::std::string   getArgHelp    (const char * arg, size_t shortLen, size_t longLen, size_t descLen);
const char *    getError      (size_t num);
size_t          getErrorCount ();
int32_t         loadArgs      (int32_t argc, char *argv[]);
::std::ostream& printArgHelp  (::std::ostream &os, const char * arg, size_t shortLen, size_t longLen, size_t descLen);
void            procArgs      ();
int32_t         size          ();


// These are overloaded short aliases, simply using the real functions

/** @brief overloaded short alias for @see addArgCb() **/
int32_t addArg (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                int32_t aCompc, const char * aCompName, void (*aCb)(const char*, void*), void *aUserData)
  {
    return addArgCb(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName, aCb, aUserData);
  }

/** @brief overloaded short alias for @see addArgBool() **/
int32_t addArg (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                bool * aTarget, eTargets aType)
  {
    return addArgBool(aShortArg, aLongArg, aDash, aDesc, aTarget, aType);
  }

/** @brief overloaded short alias for @see addArgInt32() **/
int32_t addArg (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                int32_t aCompc, const char * aCompName, int32_t * aTarget, eTargets aType, int32_t aMin,
                int32_t aMax)
  {
    return addArgInt32(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName, aTarget, aType, aMin, aMax);
  }

/** @brief overloaded short alias for @see addArgInt64() **/
int32_t addArg (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                int32_t aCompc, const char * aCompName, int64_t * aTarget, eTargets aType, int64_t aMin,
                int64_t aMax)
  {
    return addArgInt64(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName, aTarget, aType, aMin, aMax);
  }

/** @brief overloaded short alias for @see addArgFloat() **/
int32_t addArg (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                int32_t aCompc, const char * aCompName, float * aTarget, eTargets aType, float aMin,
                float aMax)
  {
    return addArgFloat(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName, aTarget, aType, aMin, aMax);
  }

/** @brief overloaded short alias for @see addArgDouble() **/
int32_t addArg (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                int32_t aCompc, const char * aCompName, double * aTarget, eTargets aType, double aMin,
                double aMax)
  {
    return addArgDouble(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName, aTarget, aType, aMin, aMax);
  }

/** @brief overloaded short alias for @see addArgChar() **/
int32_t addArg (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                int32_t aCompc, const char * aCompName, char * aTarget, size_t aMaxSize, eTargets aType)
  {
    return addArgChar(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName, aTarget, aMaxSize, aType);
  }

/** @brief overloaded short alias for @see addArgString() **/
int32_t addArg (const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                int32_t aCompc, const char * aCompName, ::std::string * aTarget, eTargets aType)
  {
    return addArgString(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName, aTarget, aType);
  }


} // End of namespace args

} // End of namespace pwx

#include "pwxLib/internal/ArgImpl.h"

#if defined(ARGS_REMOVE_NO_INSTANCE)
#  undef ARGS_REMOVE_NO_INSTANCE
#  undef PWX_NO_MRF_INSTANCE
#endif // clean up

#endif // PWX_ARGS_H_INCLUDED

