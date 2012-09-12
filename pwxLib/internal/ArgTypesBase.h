#pragma once
#ifndef PWX_ARGTYPESBASE_H_INCLUDED
#define PWX_ARGTYPESBASE_H_INCLUDED 1

/** @internal
  * @file ArgTypesBase.h
  *
  * @brief This file defines a simple base class to add type information to TArgMapItem
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
  * History and Changelog maintained in tools/Args.h.
**/

#ifndef PWX_ARGS_H_INCLUDED
# error "Do not include ArgTypesBase.h, include tools/Args.h!"
#endif // Check for main file

#include "pwxLib/internal/MSVCwarnPush.h"
#include <vector>
#include <limits>
#include "pwxLib/internal/MSVCwarnPop.h"

#include "pwxLib/tools/StreamHelpers.h"

namespace pwx {
namespace args {

/** @namespace pwx::args::constants
  * @brief this is just a little namespace to hold the enum constants and limits for int and float
  *
  * Due to this sub-namespace, you can use a using statement like
  * "using namespace pwx::args::constants" to pull in the needed enum values
  * to determine argument types.
  *
  * There are some helper constants for the min/max arguments of addArg():
  * min/maxInt32/64Limit and min/maxFloat/DoubleLimit
  *
  * @see pwx::args for the surrounding namespace using these constants
**/
namespace constants
{
/** @enum eTargets
  * @brief the possible targets for tArgMapItem
**/
enum eTargets
{
  ETT_TRUE,    //!< Set the target to be true
  ETT_FALSE,   //!< Set the target to be false
  ETT_INC,     //!< Add 1 to the target
  ETT_DEC,     //!< Substract 1 from the target
  ETT_ADD,     //!< Adds the component(s) to the target (int and float only)
  ETT_SUB,     //!< Substracts the component(s) from the target (int and float only)
  ETT_INT,     //!< The target is an int32_t or int64_t value
  ETT_FLOAT,   //!< The target is a float or double value
  ETT_CHAR,    //!< The target takes one character
  ETT_STRING,  //!< The target takes a whole string
  ETT_CALLBACK //!< No target but a callback function of the type void (*cb)(const char*, void*)
};

/// Minimum numeric limit of int32_t
const int32_t minInt32Limit  = ::std::numeric_limits<int32_t>::min();
/// Maximum numeric limit of int32_t
const int32_t maxInt32Limit  = ::std::numeric_limits<int32_t>::max();
/// Minimum numeric limit of int64_t
const int64_t minInt64Limit  = ::std::numeric_limits<int64_t>::min();
/// Maximum numeric limit of int64_t
const int64_t maxInt64Limit  = ::std::numeric_limits<int64_t>::max();
/// Minimum numeric limit of float
const float   minFloatLimit  = ::std::numeric_limits<float>::min();
/// Maximum numeric limit of float
const float   maxFloatLimit  = ::std::numeric_limits<float>::max();
/// Minimum numeric limit of double
const double  minDoubleLimit = ::std::numeric_limits<double>::min();
/// Maximum numeric limit of double
const double  maxDoubleLimit = ::std::numeric_limits<double>::max();

} // End of namespace constants

/** @namespace pwx::args::base
  *
  * @brief Sub-namespace under namespace args for base classes.
  *
  * All base classes for argument parsing are defined in this namespace.
  * @see pwx::args for classes using these base classes.
  * @see pwx::args::constants for enum values and constants used by these classes.
**/
namespace base
{
using namespace ::pwx::args::constants;

/** @class ArgBase
  * @brief Base class for TArgMapItem to allow untyped access
**/
class PWX_INTERNAL ArgBase
{
  typedef ::std::vector< ::std::string > tCompList;
  typedef tCompList::const_iterator    tCompIter_c;

  /* --- members --- */
  int32_t      compc;
  const char * compName; //!< What is shown as --file &lt;compName&gt; or --help [detail]
  tCompList    components;
  bool         copied; //!< set by copy ctor so the dtor knows it has to free() the strings
  int32_t      dash;
  const char * desc;
  const char * longArg;
  double       maxVal;
  double       minVal;
  const char * shortArg;
  int32_t      state; //!< 0 = unset, 1+ = set, -1 = processed

protected:
  /* --- members TArgMapItem needs to access directly --- */
  eTargets     type;

public:
  /* --- ctor/dtor --- */
  virtual ~ArgBase();
  explicit ArgBase(const char * aShortArg, const char * aLongArg, int32_t aDash,
                   const char * aDesc, int32_t aCompc, const char * aCompName,
                   eTargets aType, double aMin, double aMax,
                   void (*aCb)(const char*, void*),
                   void * aUserData):
                  compc(aCompc), compName(aCompName),
#if defined(PWX_HAS_CXX11_INIT)
                  components({}),
#endif
                  copied(false), dash(aDash), desc(aDesc),
                  longArg(aLongArg), maxVal(aMax), minVal(aMin), shortArg(aShortArg),
                  state(0), type(aType), cb(aCb), user_data(aUserData)
    { }


  /* --- methods --- */
  /** @brief add one to the state whenever this arg is found **/
  void            add() { ++state; };
  PWX_INLINE void addComp(const char * comp);
  /** @brief substract one to the state whenever this arg is processed **/
  void            dec()                { --state; }
  /** @brief set state to -1 once this arg is finished processing **/
  void            done()               { state = -1; }
  int32_t         getCompC()     const { return compc;    }
  const char *    getCompName()  const { return compName; }
  size_t          getCompCount() const { return components.size(); }
  const char *    getComponent(size_t num)
    {
      return components.size() > num ? components[num].c_str() : NULL;
    }
  int32_t         getDash()     const  { return dash;     }
  const char *    getDesc()     const  { return desc;     }
  const char *    getLongArg()  const  { return longArg;  }
  double          getMaxValue() const  { return maxVal;   }
  double          getMinValue() const  { return minVal;   }
  eTargets        getType()     const  { return type;     }
  const char *    getShortArg() const  { return shortArg; }
  int32_t         getState()    const  { return state;    }
  void       *    getUserData() const  { return user_data;}

  /* --- callback function stuff --- */
  void (*cb)(const char*, void*); // Callback function to use multiple components of an argument
  void * user_data;

  /** @brief copy ctor
    *
    * While the strings are malloc'd copies, the target pointer is shared afterwards.
  **/
  ArgBase(const ArgBase &src):
          compc(src.compc), compName(src.compName ? pwx_strdup(src.compName) : NULL),
          components(src.components), copied(true), dash(src.dash),
          desc(src.desc ? pwx_strdup(src.desc) : NULL),
          longArg(src.longArg ? pwx_strdup(src.longArg) : NULL),
          maxVal(src.maxVal), minVal(src.minVal),
          shortArg(src.shortArg ? pwx_strdup(src.shortArg) : NULL),
          state(src.state), type(src.type),cb(src.cb), user_data(src.user_data)
    { }

  /** @brief assignment operator
    *
    * While the strings are malloc'd copies, the target pointer is shared afterwards.
  **/
  ArgBase &operator=(const ArgBase &src)
    {
      if (&src != this)
        {
          compc      = src.compc;
          compName   = src.compName ? pwx_strdup(src.compName) : NULL;
          components = src.components;
          copied     = true;
          dash       = src.dash;
          desc       = src.desc ? pwx_strdup(src.desc) : NULL;
          longArg    = src.longArg ? pwx_strdup(src.longArg) : NULL;
          maxVal     = src.maxVal;
          minVal     = src.minVal;
          shortArg   = src.shortArg ? pwx_strdup(src.shortArg) : NULL;
          state      = src.state;
          type       = src.type;
          cb         = src.cb;
          user_data  = src.user_data;
        }
      return *this;
    }
};

  /** @brief default dtor
    *
    * virtual non-inline dtor to force the vtable to be placed here
    * If this is a copy, then the strings are malloc'd and have to be freed
  **/
ArgBase::~ArgBase()
  {
    if (copied)
      {
        if (compName) free (const_cast<char *>(compName));
        if (desc)     free (const_cast<char *>(desc));
        if (longArg)  free (const_cast<char *>(longArg));
        if (shortArg) free (const_cast<char *>(shortArg));
      }
  }

} // End of namespace base

} // End of namespace args

} // End of namespace pwx

#endif // PWX_ARGTYPESBASE_H_INCLUDED
