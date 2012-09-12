#pragma once
#ifndef PWX_ARGTYPES_H_INCLUDED
#define PWX_ARGTYPES_H_INCLUDED 1

/** @internal
  * @file ArgTypes.h
  *
  * @brief This file defines types used by pwxLib/tools/Args.h
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
# error "Do not include ArgTypes.h, include tools/Args.h!"
#endif // Check for main file

#include "pwxLib/internal/ArgTypesBase.h"

namespace pwx {
namespace args {
using namespace constants;
using ::pwx::mrf::CMemRingFactory;

/** @class TArgMapItem
  * @brief template to describe one command line argument.
  *
  * This is the main datatype to describe one command line argument. The command line argument map
  * is made up of instances of this class, and will be used to parse the command line arguments
  * and options..
**/
template<typename T>
class PWX_INTERNAL TArgMapItem : public base::ArgBase
{
  /* --- members --- */
  T * target;

public:
  /* --- max size of char args - only needed there --- */
  size_t maxSize;

  /* --- methods --- */

  /** @brief access operator to manipulate the target via pointer **/
  T * operator*()
  {
    return target;
  }

  /** @brief default ctor setting everything up for this item
    *
    * @param[in] aShortArg the short, one-character version, of the argument/option
    * @param[in] aLongArg the long version of the argument/option
    * @param[in] aDash the number of dashes for the long arg, the short arg has one less.
    * a negative value makes the dashes optional. To be able to combine short
    * arguments/options aDash needs to be at least +2.
    * @param[in] aDesc a description to show for help texts.
    * @param[in] aCompc the count of components following this argument/option. A negative number
    * makes the components optional.
    * @param[in] aCompName The name of the component as shown in the help string
    * @param[in] aTarget a pointer to the variable that has to be manipulated by this argument/option
    * @param[in] aType the type of this argument. @see eTargets for details on this values.
    * @param[in] aMin the minimum value allowed. Applies for float and int types only.
    * @param[in] aMax the maximum value allowed. Applies for float and int types only.
    * @param[in] aCb a callback function "void foo(const char*, void*)" to be used if aCompc is greater than one.
    * @param[in] aUserData optional user data to give to the callback function, normally used for an object wrapper
  **/
  explicit TArgMapItem(const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                       int32_t aCompc, const char * aCompName, T * aTarget, eTargets aType, double aMin,
                       double aMax, void (*aCb)(const char*, void*), void * aUserData):
    ArgBase(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName, aType, aMin, aMax, aCb, aUserData),
    target(aTarget),
    maxSize(0)
  {
    // We need either a target or a callback function:
    assert(target || cb || "There has to be a target or a callback function at least!");

    // Check for allowed types only:
#if !defined(_MSC_VER)
    compAssert(  isSameType(T, bool)  || isSameType(T, int32_t) || isSameType(T, int64_t)
              || isSameType(T, float) || isSameType(T, double)  || isSameType(T, char)
              || isSameType(T, ::std::string) );
    // Note to self: It would be great to find out what Microsoft thinks is wrong with my compiler
    //               assertion technique, and how to do (wrap) it to make it work with Visual C++ ...
#endif
    // Normal assertions can be used for the type vs T checking:
    assert ((isSameType(T, bool) == ((ETT_TRUE == type) || (ETT_FALSE == type)))
            && "Boolean targets have to use either ETT_TRUE or ETT_FALSE as type!");
    assert ( (!isSameType(T, int32_t) || (ETT_INC == type) || (ETT_DEC == type) || (ETT_INT == type)
                                      || (ETT_ADD == type) || (ETT_SUB == type) )
            && "Integer32 targets have to use either ETT_INC, ETT_DEC, ETT_ADD, ETT_SUB or ETT_INT as type!");
    assert ( (!isSameType(T, int64_t) || (ETT_INC == type) || (ETT_DEC == type) || (ETT_INT == type)
                                      || (ETT_ADD == type) || (ETT_SUB == type) )
            && "Integer64 targets have to use either ETT_INC, ETT_DEC, ETT_ADD, ETT_SUB or ETT_INT as type!");
    assert ( (!isSameType(T, float) || (ETT_INC == type) || (ETT_DEC == type) || (ETT_FLOAT == type)
                                    || (ETT_ADD == type) || (ETT_SUB == type) )
            && "Floating point targets have to use either ETT_INC, ETT_DEC, ETT_ADD, ETT_SUB or ETT_FLOAT as type!");
    assert ( (!isSameType(T, double) || (ETT_INC == type) || (ETT_DEC == type) || (ETT_FLOAT == type)
                                     || (ETT_ADD == type) || (ETT_SUB == type) )
            && "Floating point targets have to use either ETT_INC, ETT_DEC, ETT_ADD, ETT_SUB or ETT_FLOAT as type!");
    assert ( (!isSameType(T, char) || (ETT_CHAR == type) || (ETT_STRING == type) )
            && "Character string targets have to use either ETT_CHAR or ETT_STRING as type!");
    assert ( (!isSameType(T, ::std::string) || (ETT_CHAR == type) || (ETT_STRING == type) || (ETT_CALLBACK == type) )
            && "std::string targets have to use either ETT_CHAR, ETT_STRING or ETT_CALLBACK as type!");
  }


  virtual ~TArgMapItem() { }

  /** @brief copy ctor
    *
    * While the strings are malloc'd copies, the target pointer is shared afterwards.
  **/
  TArgMapItem(const TArgMapItem & src):ArgBase(src),target(src.target),maxSize(src.maxSize) { }

private:
  /** @brief the empty ctor is private, we definetly need some values **/
  TArgMapItem() {}
  /** @brief no assignment, use copy ctor instead! **/
  TArgMapItem &operator=(const TArgMapItem&);
};

// To be able to use MRF, every arg map item needs an operator<< upon an out stream:
template<typename T>
::std::ostream &operator<<(::std::ostream &os, TArgMapItem<T> &i)
{
  if (os.good())
    os << i.getShortArg() << ";" << i.getLongArg() << ::std::endl;
  return os;
}

/** typedefs of ArgMapItems for all eTargets: **/
typedef TArgMapItem< bool >          tBoolArgItem;
typedef TArgMapItem< int32_t >       tInt32ArgItem;
typedef TArgMapItem< int64_t >       tInt64ArgItem;
typedef TArgMapItem< float >         tFloatArgItem;
typedef TArgMapItem< double >        tDoubleArgItem;
typedef TArgMapItem< char >          tCharArgItem;
typedef TArgMapItem< ::std::string > tStringArgItem;

/** typedefs for TMemRing, utilizing each map item type **/
typedef ::pwx::mrf::TMemRing<tBoolArgItem>   tBoolArgCont;
typedef ::pwx::mrf::TMemRing<tInt32ArgItem>  tInt32ArgCont;
typedef ::pwx::mrf::TMemRing<tInt64ArgItem>  tInt64ArgCont;
typedef ::pwx::mrf::TMemRing<tFloatArgItem>  tFloatArgCont;
typedef ::pwx::mrf::TMemRing<tDoubleArgItem> tDoubleArgCont;
typedef ::pwx::mrf::TMemRing<tCharArgItem>   tCharArgCont;
typedef ::pwx::mrf::TMemRing<tStringArgItem> tStringArgCont;

/** typedefs for TItem, utilizing each map item type **/
typedef ::pwx::mrf::TItem<tBoolArgItem>   tBoolArgTItem;
typedef ::pwx::mrf::TItem<tInt32ArgItem>  tInt32ArgTItem;
typedef ::pwx::mrf::TItem<tInt64ArgItem>  tInt64ArgTItem;
typedef ::pwx::mrf::TItem<tFloatArgItem>  tFloatArgTItem;
typedef ::pwx::mrf::TItem<tDoubleArgItem> tDoubleArgTItem;
typedef ::pwx::mrf::TItem<tCharArgItem>   tCharArgTItem;
typedef ::pwx::mrf::TItem<tStringArgItem> tStringArgTItem;

/** @struct ArgMap
  * @brief Collection of TMemRing containers, handled by a local mrf instance
  *
  * If anything goes wrong, error messages are generated that can
  * be reviewd via @see getErrorCount() and @see getError(num).
**/
struct ArgMap : private ::pwx::Uncopyable
{
  typedef ::std::vector< ::std::string > tErrList;

  // We use a local instance of mrf to not clash with (a) user instance(s)
  CMemRingFactory argMRF;
  tErrList        errors;

  /* --- indirect inline public methods --- */
  PWX_INLINE void            addError(const char * error);
  PWX_INLINE base::ArgBase * find    (const char * arg)       PWX_WARNUNUSED;
  PWX_INLINE const char    * getError(size_t num)       const PWX_WARNUNUSED;
  PWX_INLINE size_t          size    ();

  /* --- templated methods --- */
  template <typename Tcont, typename Titem>
  PWX_INLINE void checkComponents();

  /* --- ctor and dtor --- */
  /** @brief default ctor **/
  explicit ArgMap (): argMRF(false, true) // We don't need the ID map, but the name map.
#if defined(PWX_HAS_CXX11_INIT)
    , errors({})
#endif
  { /* noting to do here */  }

  /** @brief default dtor **/
  ~ArgMap()
  {
    argMRF.clear();
  }
};

/* --- global instance of ArgMap --- */
static ArgMap argMap;

} // end of namespace args

} // end of namespace pwx

#endif // PWX_ARGTYPES_H_INCLUDED
