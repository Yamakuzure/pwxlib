#pragma once
#ifndef PWX_ARGIMPL_H_INCLUDED
#define PWX_ARGIMPL_H_INCLUDED 1

/** @internal
  * @file ArgImpl.h
  *
  * @brief This file implements all functions declared in pwxLib/tools/Args.h
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
  * History and Changelog maintained in tools/Args.h
**/

#ifndef PWX_ARGS_H_INCLUDED
# error "Do not include ArgImpl.h, include tools/Args.h!"
#endif // Check for main file

namespace pwx {
namespace args {

using pwx::StreamHelpers::to_int32;
using pwx::StreamHelpers::to_int64;
using pwx::StreamHelpers::to_float;
using pwx::StreamHelpers::to_double;
using pwx::StreamHelpers::to_string;

/** @brief add a component to this argument
  *
  * @param[in] comp component to add
  **/
void base::ArgBase::addComp(const char * comp)
{
  if (comp && strlen(comp))
    {
      if ((components.size() < static_cast<uint32_t>(abs(compc))))
        components.push_back(::std::string(comp));
      else
        {
          ::std::string error = "Superfluous component \"";
          error.append(comp);
          error += "\" for option \"";
          if (longArg && strlen(longArg))
            {
              error.append(abs(dash), '-');
              error.append(longArg);
            }
          else
            {
              error.append(abs(dash) - (dash ? 1 : 0), '-');
              error.append(shortArg);
            }
          error += "\"";
          argMap.addError(error.c_str());
        }
    }
}



/** @brief add a string to the list of errors
  *
  * @param[in] error The error string
**/
void ArgMap::addError(const char * error)
{
  if (error && strlen(error))
    errors.push_back(error);
}


/** @brief add an error message for every argument that has missing components
  *
  * This method walks through the container of type Tcont and checks
  * all items of type Titem whether they a) have mandatory components
  * and b) a component count that is below the wanted number.
  *
  * For each hit a message is added to the argMap error list.
**/
template<typename Tcont, typename Titem>
void ArgMap::checkComponents()
{
  ::std::string error;
  int32_t diff;

  // get a pointer to the wanted container of type Tcomp. This is save
  // because MRF.find() returns NULL if no such container exists.
  Tcont * cont = argMRF.find(static_cast<Titem *>(NULL));

  if (cont)
    {
      // We have a container of this type, so we can have a look around:
      int32_t xSize = cont->size();

      for (int32_t i = 0; i < xSize; ++i)
        {
          Titem * item = cont->getData(i);
          diff = item->getCompC() - item->getCompCount();
          if ((item->getState() > 0) && (diff > 0))
            {
              error = "Missing ";
              error += to_string(diff);
              error += " component";
              if (diff > 1)
                error += "s";
              error += " for argument \"";
              if (item->getLongArg() && strlen(item->getLongArg()))
                {
                  error.append(abs(item->getDash()), '-');
                  error.append(item->getLongArg());
                }
              else
                {
                  error.append(abs(item->getDash()) - (item->getDash() ? 1 : 0), '-');
                  error.append(item->getShortArg());
                }
              error += "\"";
              argMap.addError(error.c_str());
            } // End of item misses an argument
        } // End of looping through all int32 items
    } // End of having a container
}


/** @brief return a base pointer to the map item that holds an argument with the given short/long arg
  *
  * This method finds a specific map item and returns a base pointer. Which type it is can be determined
  * by the @see getType() method.
  *
  * Please use the long version of the argument only if there is no short version.
  *
  * @param[in] arg The argument to search for.
  * @return a base class pointer for further processing or NULL if no such argument can be found.
**/
base::ArgBase * ArgMap::find(const char * arg)
{
  bool isFound = false;
  base::ArgBase * result = NULL;

  // --- 1: Search in the bool container
  tBoolArgCont   * BoolCont   = argMRF.find(static_cast<tBoolArgItem *>(NULL));
  tBoolArgTItem  * BoolItem   = NULL;
  int32_t          BoolSize   = BoolCont ? BoolCont->size() : 0;
  if (BoolSize && (NULL != (BoolItem = BoolCont->getItem(arg))))
    {
      isFound = true;
      result  = BoolItem->getDataPointer();
    }

  // --- 2: Search in the int32 container if needed
  tInt32ArgCont   * Int32Cont   = argMRF.find(static_cast<tInt32ArgItem *>(NULL));
  tInt32ArgTItem  * Int32Item   = NULL;
  int32_t           Int32Size   = Int32Cont && !isFound ? Int32Cont->size() : 0;
  if (!isFound && Int32Size&& (NULL != (Int32Item = Int32Cont->getItem(arg))))
    {
      isFound = true;
      result  = Int32Item->getDataPointer();
    }

  // --- 3: Search in the int64 container if needed
  tInt64ArgCont   * Int64Cont   = argMRF.find(static_cast<tInt64ArgItem *>(NULL));
  tInt64ArgTItem  * Int64Item   = NULL;
  int32_t           Int64Size   = Int64Cont && !isFound ? Int64Cont->size() : 0;
  if (!isFound && Int64Size && (NULL != (Int64Item = Int64Cont->getItem(arg))))
    {
      isFound = true;
      result  = Int64Item->getDataPointer();
    }

  // --- 4: Search in the float container if needed
  tFloatArgCont   * FloatCont   = argMRF.find(static_cast<tFloatArgItem *>(NULL));
  tFloatArgTItem  * FloatItem   = NULL;
  int32_t           FloatSize   = FloatCont && !isFound ? FloatCont->size() : 0;
  if (!isFound && FloatSize && (NULL != (FloatItem = FloatCont->getItem(arg))))
    {
      isFound = true;
      result  = FloatItem->getDataPointer();
    }

  // --- 5: Search in the double container if needed
  tDoubleArgCont   * DoubleCont   = argMRF.find(static_cast<tDoubleArgItem *>(NULL));
  tDoubleArgTItem  * DoubleItem   = NULL;
  int32_t            DoubleSize   = DoubleCont && !isFound ? DoubleCont->size() : 0;
  if (!isFound && DoubleSize && (NULL != (DoubleItem = DoubleCont->getItem(arg))))
    {
      isFound = true;
      result  = DoubleItem->getDataPointer();
    }

  // --- 6: Search in the char container if needed
  tCharArgCont   * CharCont   = argMRF.find(static_cast<tCharArgItem *>(NULL));
  tCharArgTItem  * CharItem   = NULL;
  int32_t          CharSize   = CharCont && !isFound ? CharCont->size() : 0;
  if (!isFound && CharSize && (NULL != (CharItem = CharCont->getItem(arg))))
    {
      isFound = true;
      result  = CharItem->getDataPointer();
    }

  // --- 7: Search in the string container if needed
  tStringArgCont   * StringCont   = argMRF.find(static_cast<tStringArgItem *>(NULL));
  tStringArgTItem  * StringItem   = NULL;
  int32_t            StringSize   = StringCont && !isFound ? StringCont->size() : 0;
  if (!isFound && StringSize && (NULL != (StringItem = StringCont->getItem(arg))))
    {
      isFound = true;
      result  = StringItem->getDataPointer();
    }

  // If we were not successful now, we have to look up the long args of each stored argument.
  // And that is the reason why we have pointers to each container and their sizes now:
  if (!isFound)
    {
      // 1.: Wander through all bool args that have a long option:
      for (int32_t idx = 0; !isFound && (idx < BoolSize); ++idx)
        {
          result = (*BoolCont)[idx];
          if (strlen(result->getLongArg()) && STREQ(arg, result->getLongArg()))
            isFound = true;
        }

      // 2.: Wander through all 32 bit integer args that have a long option:
      for (int32_t idx = 0; !isFound && (idx < Int32Size); ++idx)
        {
          result = (*Int32Cont)[idx];
          if (strlen(result->getLongArg()) && STREQ(arg, result->getLongArg()))
            isFound = true;
        }

      // 3.: Wander through all 64 bit integer args that have a long option:
      for (int32_t idx = 0; !isFound && (idx < Int64Size); ++idx)
        {
          result = (*Int64Cont)[idx];
          if (strlen(result->getLongArg()) && STREQ(arg, result->getLongArg()))
            isFound = true;
        }

      // 4.: Wander through all float args that have a long option:
      for (int32_t idx = 0; !isFound && (idx < FloatSize); ++idx)
        {
          result = (*FloatCont)[idx];
          if (strlen(result->getLongArg()) && STREQ(arg, result->getLongArg()))
            isFound = true;
        }

      // 5.: Wander through all doule args that have a long option:
      for (int32_t idx = 0; !isFound && (idx < DoubleSize); ++idx)
        {
          result = (*DoubleCont)[idx];
          if (strlen(result->getLongArg()) && STREQ(arg, result->getLongArg()))
            isFound = true;
        }

      // 6.: Wander through all char args that have a long option:
      for (int32_t idx = 0; !isFound && (idx < CharSize); ++idx)
        {
          result = (*CharCont)[idx];
          if (strlen(result->getLongArg()) && STREQ(arg, result->getLongArg()))
            isFound = true;
        }

      // 7.: Wander through all string args that have a long option:
      for (int32_t idx = 0; !isFound && (idx < StringSize); ++idx)
        {
          result = (*StringCont)[idx];
          if (strlen(result->getLongArg()) && STREQ(arg, result->getLongArg()))
            isFound = true;
        }
    }

  return (isFound ? result : NULL);
}


/** @brief return the error with the number @a num
  *
  * @param[in] num The number of the error message to retrieve
  * @return a const char* string with the error message or NULL if num is out of range.
**/
const char * ArgMap::getError(size_t num) const
{
  return (num < errors.size() ? errors[num].c_str() : NULL);
}


/** @brief return the number of arguments currently recognized **/
size_t ArgMap::size ()
{
  tBoolArgCont   * boolCont   = argMRF.find(static_cast<tBoolArgItem *  >(NULL));
  tInt32ArgCont  * int32Cont  = argMRF.find(static_cast<tInt32ArgItem * >(NULL));
  tInt64ArgCont  * int64Cont  = argMRF.find(static_cast<tInt64ArgItem * >(NULL));
  tFloatArgCont  * floatCont  = argMRF.find(static_cast<tFloatArgItem * >(NULL));
  tDoubleArgCont * doubleCont = argMRF.find(static_cast<tDoubleArgItem *>(NULL));
  tCharArgCont   * charCont   = argMRF.find(static_cast<tCharArgItem *  >(NULL));
  tStringArgCont * stringCont = argMRF.find(static_cast<tStringArgItem *>(NULL));

  return ( (boolCont   ? boolCont->size()   : 0)
           +(int32Cont  ? int32Cont->size()  : 0)
           +(int64Cont  ? int64Cont->size()  : 0)
           +(floatCont  ? floatCont->size()  : 0)
           +(doubleCont ? doubleCont->size() : 0)
           +(charCont   ? charCont->size()   : 0)
           +(stringCont ? stringCont->size() : 0) );
}


// To have an easier time with the exceptions that might get caught, we use a macro here:
#define PWX_ARGMAP_ADDARG_CATCHER \
catch (::std::bad_alloc &e) { \
  ::std::string error = "Can't create tStringArgItem ("; \
  error += e.what(); \
  error += ")"; \
  argMap.addError(error.c_str()); \
} catch (::pwx::Exception &e) { \
  ::std::string error = "Can't add string argument ("; \
  error += e.what(); \
  error += ")\nDescription : \""; \
  error += e.desc(); \
  error += "\"\n--- Trace ---\n"; \
  error += e.trace(); \
  argMap.addError(error.c_str()); \
}

/** @brief add a targetless argument that has to use a callback funtion to the argument map
  *
  * This function adds an entry into the string container without a target. The string container
  * is processed last, so you can use this to add arguments to your program that use a callback
  * function only and depend on other input data from the other containers.
  *
  * The following rules must be followed:
  *
  * - Either @a aShortArg, @a aLongArg or both have to be set.
  * - If the argument already exists in any of the containers, this function does nothing.
  *
  * Note on Components: If you do not set components, the callback function will be called with the
  * short option if set or the long option as the first argument.
  *
  * @param[in] aShortArg the short, one-character version, of the argument/option
  * @param[in] aLongArg the long version of the argument/option
  * @param[in] aDash the number of dashes for the long arg, the short arg has one less.
  * a negative value makes the dashes optional. To be able to combine short
  * arguments/options aDash needs to be at least +2.
  * @param[in] aDesc a description to show for help texts.
  * @param[in] aCompc the count of components following this argument/option. A negative number
  * makes the components optional.
  * @param[in] aCompName an optional name that is displayed in the help texts.
  * @param[in] aCb a callback function to be used if aCompc is greater than one.
  * @param[in] aUserData a pointer to user data added to the callback call.
  * @return The number of arguments currently known or -1 on failure
**/
int32_t addArgCb(const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                 int32_t aCompc, const char * aCompName, void (*aCb)(const char*, void*), void *aUserData)
{
  int32_t result = -1;
  bool hasShort  = (aShortArg && strlen(aShortArg)) ? true : false;
  bool hasLong   = (aLongArg && strlen(aLongArg))   ? true : false;

  if ((NULL == argMap.find(hasShort ? aShortArg : aLongArg)) && (hasShort || hasLong))
    {
      try
        {
          tStringArgCont * cont = argMap.argMRF.get<tStringArgItem>(static_cast<tStringArgItem *>(NULL));
          tStringArgItem * item = new tStringArgItem(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName,
              static_cast< ::std::string* >(NULL), ETT_CALLBACK, 0, 0,
              aCb, aUserData);
          cont->add(item, 0, hasShort ? aShortArg : aLongArg);
          result = argMap.size();
        }
      PWX_ARGMAP_ADDARG_CATCHER
    }
  return result;
}


/** @brief add a bool argument/option to the argument map
  *
  * This function adds an entry into the bool map according to the arguments provided.
  *
  * The following rules must be followed:
  *
  * - Either @a aShortArg, @a aLongArg or both have to be set.
  * - ETT_TRUE sets the target to "true" and ETT_FALSE to "false".
  * - If the argument already exists in any of the maps, this function does nothing.
  *
  * @param[in] aShortArg the short, one-character version, of the argument/option
  * @param[in] aLongArg the long version of the argument/option
  * @param[in] aDash the number of dashes for the long arg, the short arg has one less.
  * a negative value makes the dashes optional. To be able to combine short
  * arguments/options aDash needs to be at least +2.
  * @param[in] aDesc a description to show for help texts.
  * @param[in] aTarget a pointer to the variable that has to be manipulated by this argument/option
  * @param[in] aType the type of this argument. @see eTargets for details on this values.
  * @return The number of arguments currently known or -1 on failure
**/
int32_t addArgBool(const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                   bool * aTarget, eTargets aType)
{
  int32_t result = -1;
  bool hasShort  = (aShortArg && strlen(aShortArg)) ? true : false;
  bool hasLong   = (aLongArg && strlen(aLongArg))   ? true : false;

  if ((NULL == argMap.find(hasShort ? aShortArg : aLongArg)) && (hasShort || hasLong))
    {
      try
        {
          tBoolArgCont * cont = argMap.argMRF.get<tBoolArgItem>(static_cast<tBoolArgItem *>(NULL));
          tBoolArgItem * item = new tBoolArgItem(aShortArg, aLongArg, aDash, aDesc, 0, NULL,
                                                 aTarget, aType, 0, 0, NULL, NULL);
          cont->add(item, 0, hasShort ? aShortArg : aLongArg);
          result = argMap.size();
        }
      PWX_ARGMAP_ADDARG_CATCHER
    }
  return result;
}

/** @brief add a 32bit integer argument/option to the argument map
  *
  * This function adds an entry into the 32bit integer map according to the arguments provided.
  *
  * The following rules must be followed:
  *
  * - Either @a aShortArg, @a aLongArg or both have to be set.
  * - If the argument can have more than one component following it, you should use addArgCb
  *   instead unless the type is ETT_ADD or ETT_SUB.
  * - You can use the constants constants::minIntLimit and constants::maxIntLimit for
  *   @a aMin and @a aMax to allow all values in that direction.
  * - If the argument already exists in any of the maps, this function does nothing.
  *
  * @param[in] aShortArg the short, one-character version, of the argument/option
  * @param[in] aLongArg the long version of the argument/option
  * @param[in] aDash the number of dashes for the long arg, the short arg has one less.
  * a negative value makes the dashes optional. To be able to combine short
  * arguments/options aDash needs to be at least +2.
  * @param[in] aDesc a description to show for help texts.
  * @param[in] aCompc the count of components following this argument/option. A negative number
  * makes the components optional.
  * @param[in] aCompName an optional name that is displayed in the help texts.
  * @param[in] aTarget a pointer to the variable that has to be manipulated by this argument/option
  * @param[in] aType the type of this argument. @see eTargets for details on this values.
  * @param[in] aMin the minimum value allowed. Applies for int and float types only.
  * @param[in] aMax the maximum value allowed. Applies for int and float types only.
  * @return The number of arguments currently known or -1 on failure
**/
int32_t addArgInt32(const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                    int32_t aCompc, const char * aCompName, int32_t * aTarget, eTargets aType, int32_t aMin,
                    int32_t aMax)
{
  int32_t result = -1;
  bool hasShort  = (aShortArg && strlen(aShortArg)) ? true : false;
  bool hasLong   = (aLongArg && strlen(aLongArg))   ? true : false;

  if ((NULL == argMap.find(hasShort ? aShortArg : aLongArg)) && (hasShort || hasLong))
    {
      try
        {
          tInt32ArgCont * cont = argMap.argMRF.get<tInt32ArgItem>(static_cast<tInt32ArgItem *>(NULL));
          tInt32ArgItem * item = new tInt32ArgItem(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName,
              aTarget, aType, aMin, aMax, NULL, NULL);
          cont->add(item, 0, hasShort ? aShortArg : aLongArg);
          result = argMap.size();
        }
      PWX_ARGMAP_ADDARG_CATCHER
    }
  return result;
}

/** @brief add a 64bit integer argument/option to the argument map
  *
  * This function adds an entry into the 64bit integer map according to the arguments provided.
  *
  * The following rules must be followed:
  *
  * - Either @a aShortArg, @a aLongArg or both have to be set.
  * - If the argument can have more than one component following it, you should use addArgCb
  *   instead unless the type is ETT_ADD or ETT_SUB.
  * - You can use the constants constants::minIntLimit and constants::maxIntLimit for
  *   @a aMin and @a aMax to allow all values in that direction.
  * - If the argument already exists in any of the maps, this function does nothing.
  *
  * @param[in] aShortArg the short, one-character version, of the argument/option
  * @param[in] aLongArg the long version of the argument/option
  * @param[in] aDash the number of dashes for the long arg, the short arg has one less.
  * a negative value makes the dashes optional. To be able to combine short
  * arguments/options aDash needs to be at least +2.
  * @param[in] aDesc a description to show for help texts.
  * @param[in] aCompc the count of components following this argument/option. A negative number
  * makes the components optional.
  * @param[in] aCompName an optional name that is displayed in the help texts.
  * @param[in] aTarget a pointer to the variable that has to be manipulated by this argument/option
  * @param[in] aType the type of this argument. @see eTargets for details on this values.
  * @param[in] aMin the minimum value allowed. Applies for int and float types only.
  * @param[in] aMax the maximum value allowed. Applies for int and float types only.
  * @return The number of arguments currently known or -1 on failure
**/
int32_t addArgInt64(const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                    int32_t aCompc, const char * aCompName, int64_t * aTarget, eTargets aType, int64_t aMin,
                    int64_t aMax)
{
  int32_t result = -1;
  bool hasShort  = (aShortArg && strlen(aShortArg)) ? true : false;
  bool hasLong   = (aLongArg && strlen(aLongArg))   ? true : false;

  if ((NULL == argMap.find(hasShort ? aShortArg : aLongArg)) && (hasShort || hasLong))
    {
      try
        {
          tInt64ArgCont * cont = argMap.argMRF.get<tInt64ArgItem>(static_cast<tInt64ArgItem *>(NULL));
          tInt64ArgItem * item = new tInt64ArgItem(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName,
              aTarget, aType, static_cast<double>(aMin), static_cast<double>(aMax), NULL, NULL);
          cont->add(item, 0, hasShort ? aShortArg : aLongArg);
          result = argMap.size();
        }
      PWX_ARGMAP_ADDARG_CATCHER
    }
  return result;
}

/** @brief add a float argument/option to the argument map
  *
  * This function adds an entry into the float map according to the arguments provided.
  *
  * The following rules must be followed:
  *
  * - Either @a aShortArg, @a aLongArg or both have to be set.
  * - If the argument can have more than one component following it, you should use addArgCb
  *   instead unless the type is ETT_ADD or ETT_SUB.
  * - You can use the constants constants::minFloatLimit and constants::maxFloatLimit for
  *   @a aMin and @a aMax to allow all values in that direction.
  * - If the argument already exists in any of the maps, this function does nothing.
  *
  * @param[in] aShortArg the short, one-character version, of the argument/option
  * @param[in] aLongArg the long version of the argument/option
  * @param[in] aDash the number of dashes for the long arg, the short arg has one less.
  * a negative value makes the dashes optional. To be able to combine short
  * arguments/options aDash needs to be at least +2.
  * @param[in] aDesc a description to show for help texts.
  * @param[in] aCompc the count of components following this argument/option. A negative number
  * makes the components optional.
  * @param[in] aCompName an optional name that is displayed in the help texts.
  * @param[in] aTarget a pointer to the variable that has to be manipulated by this argument/option
  * @param[in] aType the type of this argument. @see eTargets for details on this values.
  * @param[in] aMin the minimum value allowed. Applies for int and float types only.
  * @param[in] aMax the maximum value allowed. Applies for int and float types only.
  * @return The number of arguments currently known or -1 on failure
**/
int32_t addArgFloat(const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                    int32_t aCompc, const char * aCompName, float * aTarget, eTargets aType, float aMin,
                    float aMax)
{
  int32_t result = -1;
  bool hasShort  = (aShortArg && strlen(aShortArg)) ? true : false;
  bool hasLong   = (aLongArg && strlen(aLongArg))   ? true : false;

  if ((NULL == argMap.find(hasShort ? aShortArg : aLongArg)) && (hasShort || hasLong))
    {
      try
        {
          tFloatArgCont * cont = argMap.argMRF.get<tFloatArgItem>(static_cast<tFloatArgItem *>(NULL));
          tFloatArgItem * item = new tFloatArgItem(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName,
              aTarget, aType, aMin, aMax, NULL, NULL);
          cont->add(item, 0, hasShort ? aShortArg : aLongArg);
          result = argMap.size();
        }
      PWX_ARGMAP_ADDARG_CATCHER
    }
  return result;
}

/** @brief add a double argument/option to the argument map
  *
  * This function adds an entry into the double map according to the arguments provided.
  *
  * The following rules must be followed:
  *
  * - Either @a aShortArg, @a aLongArg or both have to be set.
  * - If the argument can have more than one component following it, you should use addArgCb
  *   instead unless the type is ETT_ADD or ETT_SUB.
  * - You can use the constants constants::minFloatLimit and constants::maxFloatLimit for
  *   @a aMin and @a aMax to allow all values in that direction.
  * - If the argument already exists in any of the maps, this function does nothing.
  *
  * @param[in] aShortArg the short, one-character version, of the argument/option
  * @param[in] aLongArg the long version of the argument/option
  * @param[in] aDash the number of dashes for the long arg, the short arg has one less.
  * a negative value makes the dashes optional. To be able to combine short
  * arguments/options aDash needs to be at least +2.
  * @param[in] aDesc a description to show for help texts.
  * @param[in] aCompc the count of components following this argument/option. A negative number
  * makes the components optional.
  * @param[in] aCompName an optional name that is displayed in the help texts.
  * @param[in] aTarget a pointer to the variable that has to be manipulated by this argument/option
  * @param[in] aType the type of this argument. @see eTargets for details on this values.
  * @param[in] aMin the minimum value allowed. Applies for int and float types only.
  * @param[in] aMax the maximum value allowed. Applies for int and float types only.
  * @return The number of arguments currently known or -1 on failure
**/
int32_t addArgDouble(const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                     int32_t aCompc, const char * aCompName, double * aTarget, eTargets aType, double aMin,
                     double aMax)
{
  int32_t result = -1;
  bool hasShort  = (aShortArg && strlen(aShortArg)) ? true : false;
  bool hasLong   = (aLongArg && strlen(aLongArg))   ? true : false;

  if ((NULL == argMap.find(hasShort ? aShortArg : aLongArg)) && (hasShort || hasLong))
    {
      try
        {
          tDoubleArgCont * cont = argMap.argMRF.get<tDoubleArgItem>(static_cast<tDoubleArgItem *>(NULL));
          tDoubleArgItem * item = new tDoubleArgItem(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName,
              aTarget, aType, aMin, aMax, NULL, NULL);
          cont->add(item, 0, hasShort ? aShortArg : aLongArg);
          result = argMap.size();
        }
      PWX_ARGMAP_ADDARG_CATCHER
    }
  return result;
}

/** @brief add a char argument/option to the argument map
  *
  * This function adds an entry into the char map according to the arguments provided.
  *
  * The following rules must be followed:
  *
  * - Either @a aShortArg, @a aLongArg or both have to be set.
  * - If the argument can have more than one component following it, you should use
  *   addArgCb instead.
  * - @a aType ETT_CHAR means "The first character" and ETT_STRING means "the whole string".
  * - If the argument already exists in any of the maps, this function does nothing.
  *
  * @param[in] aShortArg the short, one-character version, of the argument/option
  * @param[in] aLongArg the long version of the argument/option
  * @param[in] aDash the number of dashes for the long arg, the short arg has one less.
  * a negative value makes the dashes optional. To be able to combine short
  * arguments/options aDash needs to be at least +2.
  * @param[in] aDesc a description to show for help texts.
  * @param[in] aCompc the count of components following this argument/option. A negative number
  * makes the components optional.
  * @param[in] aCompName an optional name that is displayed in the help texts.
  * @param[in] aTarget a pointer to the variable that has to be manipulated by this argument/option
  * @param[in] aType the type of this argument. @see eTargets for details on this values.
  * @return The number of arguments currently known or -1 on failure
**/
int32_t addArgChar(const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                   int32_t aCompc, const char * aCompName, char * aTarget, size_t maxSize, eTargets aType)
{
  int32_t result = -1;
  bool hasShort  = (aShortArg && strlen(aShortArg)) ? true : false;
  bool hasLong   = (aLongArg && strlen(aLongArg))   ? true : false;

  if ((NULL == argMap.find(hasShort ? aShortArg : aLongArg)) && (hasShort || hasLong))
    {
      try
        {
          tCharArgCont * cont = argMap.argMRF.get<tCharArgItem>(static_cast<tCharArgItem *>(NULL));
          tCharArgItem * item = new tCharArgItem(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName,
                                                 aTarget, aType, 0, 0, NULL, NULL);
          // Set max size of the char string:
          item->maxSize = maxSize;
          cont->add(item, 0, hasShort ? aShortArg : aLongArg);
          result = argMap.size();
        }
      PWX_ARGMAP_ADDARG_CATCHER
    }
  return result;
}

/** @brief add a string argument/option to the argument map
  *
  * This function adds an entry into the string map according to the arguments provided.
  *
  * The following rules must be followed:
  *
  * - Either @a aShortArg, @a aLongArg or both have to be set.
  * - ETT_CHAR means "one character", ETT_STRING means "whole string".
  * - If the argument can have more than one component following it, you should use
  *   addArgCb instead.
  * - If the argument already exists in any of the maps, this function does nothing.
  *
  * @param[in] aShortArg the short, one-character version, of the argument/option
  * @param[in] aLongArg the long version of the argument/option
  * @param[in] aDash the number of dashes for the long arg, the short arg has one less.
  * a negative value makes the dashes optional. To be able to combine short
  * arguments/options aDash needs to be at least +2.
  * @param[in] aDesc a description to show for help texts.
  * @param[in] aCompc the count of components following this argument/option. A negative number
  * makes the components optional.
  * @param[in] aCompName an optional name that is displayed in the help texts.
  * @param[in] aTarget a pointer to the variable that has to be manipulated by this argument/option
  * @param[in] aType the type of this argument. @see eTargets for details on this values.
  * @return The number of arguments currently known or -1 on failure
**/
int32_t addArgString(const char * aShortArg, const char * aLongArg, int32_t aDash, const char * aDesc,
                     int32_t aCompc, const char * aCompName, ::std::string * aTarget, eTargets aType)
{
  int32_t result = -1;
  bool hasShort  = (aShortArg && strlen(aShortArg)) ? true : false;
  bool hasLong   = (aLongArg && strlen(aLongArg))   ? true : false;

  if ((NULL == argMap.find(hasShort ? aShortArg : aLongArg)) && (hasShort || hasLong))
    {
      try
        {
          tStringArgCont * cont = argMap.argMRF.get<tStringArgItem>(static_cast<tStringArgItem *>(NULL));
          tStringArgItem * item = new tStringArgItem(aShortArg, aLongArg, aDash, aDesc, aCompc, aCompName,
              aTarget, aType, 0, 0, NULL, NULL);
          cont->add(item, 0, hasShort ? aShortArg : aLongArg);
          result = argMap.size();
        }
      PWX_ARGMAP_ADDARG_CATCHER
    }
  return result;
}

// Clear up after ourselves, this macro must not be used anywhere else:
#undef PWX_ARGMAP_ADDARG_CATCHER


/** @brief clear all arguments
  *
  * This function clears all maps. You might want to use it if your
  * program has to support a lot arguments and options and you do
  * not want to wait until the end of the program to clear up.
**/
void clear()
{
  argMap.argMRF.clear();
}


/** @brief clear all errors currently reported **/
void clearErrors()
{
  argMap.errors.clear();
}


/** @brief return a malloc'd C-string with a help text
  *
  * This function returns a malloc'd C-String (You have to free it) with
  * the short and long version plus the description of the argument.
  * The length arguments will be taken to build up the string. If the
  * description is longer than the set length, line breaks will be inserted
  * and the text alligned. If the dashes are optional, they will be enclosed
  * in square brackets.
  *
  * If you set a length argument to 0 (zero), the specific field will be omitted.
  *
  * If the specified argument could not be found, the function returns NULL.
  *
  * @param[in] arg The short or long version of the argument.
  * @param[in] shortLen The length of the field displaying the short version of the argument.
  * @param[in] longLen The length of the field displaying the long version of the argument.
  * @param[in] descLen The length of the field displaying the description of the argument.
  * @return a malloc'd C-string containing the result or NULL if such an @a arg does not exist.
**/
::std::string getArgHelp(const char * arg, size_t shortLen, size_t longLen, size_t descLen)
{
  base::ArgBase *item = argMap.find(arg);

  if (item)
    {
      // Now the result can be adapted together:
      ::std::string result    = "";
      ::std::string partComp  = item->getCompName() ? item->getCompName() : "";
      ::std::string partShort = item->getShortArg() ? item->getShortArg() : "";
      ::std::string partLong  = item->getLongArg()  ? item->getLongArg()  : "";
      ::std::string partDesc  = item->getDesc()     ? item->getDesc()     : "";
      int32_t dashes          = item->getDash();
      int32_t compc           = item->getCompC();
      int32_t descOffset      = 0; // Only adapt if needed

      size_t minShortLen = partShort.size() + (dashes ? abs(dashes) - 1 : 0) + (dashes < 0 ? 2 : 0);
      size_t minLongLen  = partLong.size() + abs(dashes) + (dashes < 0 ? 2 : 0);

      // The first thing to prepare is the component name, as it is either added to the short
      // or long version.
      if (compc && partComp.size())
        {
          if (compc < 0)
            // Optional start
            partComp.insert(0, "[");
          else
            // this is mandatory
            partComp.insert(0, "<");

          // do we have more than one optional component?
          if (-1 == compc)
            // Only one component allowed
            partComp += "]";
          else if (compc < -1)
            // multiple optional components
            partComp += " [...]]";
          else
            // Note: mandatory components should be named correctly
            partComp += ">";
        }

      if (shortLen)
        {
          // Do we have anything to add?
          if (partShort.size())
            {
              // Add dashes if needed:
              if (abs(dashes) > 1)
                {
                  if (dashes < 0)
                    partShort.insert(0, "]");
                  partShort.insert(0, abs(dashes) - 1, '-');
                  if (dashes < 0)
                    partShort.insert(0, "[");
                }

              // If we do not want to display the long version, the components have to be added here:
              if (compc && partComp.size() && ((0 == longLen) || (0 == partLong.size())))
                {
                  partShort += " ";
                  partShort += partComp;
                }

              // we need a space as seperator:
              partShort += " ";
            }
          else
            // No, so fill with spaces:
            partShort.assign(shortLen, ' ');

          // Now check lengths and adapt if needed
          if (minShortLen < shortLen)
            minShortLen = shortLen;
          if (partShort.size() < minShortLen)
            partShort.append(minShortLen - partShort.size(), ' ');
          descOffset += partShort.size();
        }

      if (longLen)
        {
          // Do we have anything to add?
          if (partLong.size())
            {
              // Add dashes if needed:
              if (dashes)
                {
                  if (dashes < 0)
                    partLong.insert(0, "]");
                  partLong.insert(0, abs(dashes), '-');
                  if (dashes < 0)
                    partLong.insert(0, "[");
                }

              // So we have a long version, the components have to be displayed here:
              if (compc && partComp.size())
                {
                  partLong += " ";
                  partLong += partComp;
                }

              // we need a space as seperator:
              partLong += " ";
            }
          else
            // No, so fill with spaces:
            partLong.assign(longLen, ' ');

          // Now check lengths and adapt if needed
          if (minLongLen < longLen)
            minLongLen = longLen;
          if (partLong.size() < minLongLen)
            partLong.append(minLongLen - partLong.size(), ' ');
          descOffset += partLong.size();
        }

      if (descLen)
        {
          if (partDesc.size() > descLen)
            {
              // We have to insert line breaks and blanks first:
              size_t pos    = descLen;
              size_t newPos = 0;
              while (pos < partDesc.size())
                {
                  // Search backwards for the previous space:
                  newPos = partDesc.rfind(" ", pos);
                  if (newPos != ::std::string::npos)
                    pos = newPos;
                  // Note: If pos==newPos, then we push a blank aside, that has to be counted for the offset
                  partDesc.insert(pos, descOffset - (pos == newPos ? 1 : 0), ' '); // First the blanks
                  partDesc.insert(pos, 1, '\n'); // Then push the blanks aside with a line break
                  pos += descOffset + (pos == newPos ? 0 : 1) + descLen;
                } // end of preparing description
            } // end of need to prepare description
        } // End óf if description wanted

      // Now sew them together
      if (shortLen)
        result += partShort;
      if (longLen)
        result += partLong;
      if (descLen)
        result += partDesc;

      // And give it back:
      return result;
    }
  return "";
}


/** @brief get the error string with the number @a num or NULL if @a num is out of range **/
const char * getError(size_t num)
{
  return argMap.getError(num);
}


/** @brief get the number of currently reported errors from the argument map **/
size_t getErrorCount()
{
  return argMap.errors.size();
}


/** @brief This function loads the comand line arguments into the prepared containers
  *
  * Use this function to parse the command line for all arguments/options your program
  * supports. If an argument is not known, it will be added to @a unknown. The function
  * returns a positive number of parsed arguments if everything was well, or a negative
  * number of unknown options if they are encountered.
  *
  * Note: The first argument (*argv[0]) is concidered to be the programs name and will be skipped.
  *
  * @param[in] argc The number of arguments given.
  * @param[in] argv An array of C-Strings with the arguments.
  * @return Number of arguments parsed (>0) or number of unknown arguments encountered (<0).
**/
int32_t loadArgs(int32_t argc, char *argv[])
{
  int32_t  argsFound        = 0;
  int32_t  argsUnknown      = 0;
  int32_t  currArg          = 1; // Ignore the first, which is the programs name
  int32_t  dashes           = 0;
  char     shortArg[2]      = { 0x0, 0x0 };
  uint32_t currPos          = 0;
  bool     isDone           = false;
  bool     isUnknown        = false;
  bool     lookForShortArg  = false;
  ::std::string arg         = "";
  ::std::string error       = "";
  int32_t needComp          = 0;
  int32_t lastNeedComp      = 0;
  base::ArgBase * currItem  = NULL;
  base::ArgBase * lastItem  = NULL;

  while (currArg < argc)
    {
      arg    = argv[currArg];
      dashes = 0;
      while (arg[0] == '-')
        {
          dashes++;
          arg.erase(0, 1);
        }

      // If the string left has only one char, this is a short
      // option. And if it has had at least one preceding dash,
      // it must be increased to match the count of the specific
      // arg. Otherwise, while legal, it would be logged as unknown.
      if (dashes && (1 == arg.size()))
        ++dashes;
      isDone          = false;
      lookForShortArg = false;
      currPos         = 0;

      // Now process the remaining string
      while (!isDone)
        {
          if (lookForShortArg)
            {
              // Just fetch the next char
              shortArg[0] = arg[currPos];
              currItem     = argMap.find(shortArg);
              if (++currPos >= arg.size())
                isDone = true;
            }
          else
            // Try th whole thing, then go looking
            currItem = argMap.find(arg.c_str());

          // Where we successful?
          if (currItem)
            {
              if ((abs(currItem->getDash()) == dashes)
                  ||( (currItem->getDash() <= 0) && (0 == dashes) ) )
                {
                  // Yes, we have one
                  currItem->add();
                  needComp = abs(currItem->getCompC());
                  if (needComp && (currItem != lastItem))
                    {
                      // Save for the next arg that might be a component
                      lastItem     = currItem;
                      lastNeedComp = needComp;
                    }
                  ++argsFound;
                }
              else
                // Nope, this is an error
                isUnknown = true;

              // If this isn't a search for short options, we are done now
              if (!lookForShortArg)
                isDone = true;
            } // end of havin an item
          else if (lookForShortArg)
            // This option is unknown:
            isUnknown = true;
          else
            {
              // This is either a string consisting of more than
              // One short options, or the component of the last option
              if (lastNeedComp && !dashes)
                {
                  lastItem->addComp(arg.c_str());
                  isDone = true;
                }
              else
                {
                  lookForShortArg = true;
                  // Short args have one less than recorded...
                  if (dashes > 0)
                    ++dashes; // ...so add the missing one
                }
            } // End of arg type possibilities chain
          // We have to report if this is unknown:
          if (isUnknown)
            {
              error  = "Unknown option \"";
              if (lookForShortArg)
                {
                  error.append(dashes - (dashes > 1 ? 1 : 0), '-');
                  error.append(shortArg);
                }
              else
                {
                  error.append(dashes - (1 == arg.size() ? 1 : 0), '-');
                  error.append(arg);
                }
              error += "\"";
              argMap.addError(error.c_str());
              ++argsUnknown;
              isUnknown = false;
            }
        } // End of processing current argument
      ++currArg;
    } // End of processing argc/argv

  // The last thing to do is to go through all args to find ones that miss components:
  argMap.checkComponents<tBoolArgCont  , tBoolArgItem  >();
  argMap.checkComponents<tInt32ArgCont , tInt32ArgItem >();
  argMap.checkComponents<tInt64ArgCont , tInt64ArgItem >();
  argMap.checkComponents<tFloatArgCont , tFloatArgItem >();
  argMap.checkComponents<tDoubleArgCont, tDoubleArgItem>();
  argMap.checkComponents<tCharArgCont  , tCharArgItem  >();
  argMap.checkComponents<tStringArgCont, tStringArgItem>();

  return argsUnknown ? -argsUnknown : argsFound;
}


/** @brief push a help text upon an ostream
  *
  * This function pushes a help text with the short and long version plus
  * the description of the argument upn an ostream.
  * The length arguments will be taken to build up the string. If the
  * description is longer than the set length, line breaks will be inserted
  * and the text alligned. If the dashes are optional, they will be enclosed
  * in square brackets.
  *
  * If you set a length argument to 0 (zero), the specific field will be omitted.
  *
  * If the specified argument could not be found, the function simply does nothing
  *
  * @param[in] os A reference to an instance of (std::)ostream.
  * @param[in] arg The short or long version of the argument.
  * @param[in] shortLen The length of the field displaying the short version of the argument.
  * @param[in] longLen The length of the field displaying the long version of the argument.
  * @param[in] descLen The length of the field displaying the description of the argument.
  * @return @a os as a reference.
**/
::std::ostream& printArgHelp(::std::ostream &os, const char * arg, int32_t shortLen, int32_t longLen, int32_t descLen)
{
  ::std::string helpText = getArgHelp(arg, shortLen, longLen, descLen);
  if (helpText.size())
    os << helpText << "\n";
  return os;
}


/** @brief process all loaded arguments
  *
  * This function finally does all the processing of the arguments, manipulating
  * targets and calling callback functions.
**/
void procArgs()
{
  size_t compNum = 0;
  size_t state   = 0; // Either state or the number of components

  // Get pointers to all saved containers. If such a container does not exist, find() will return NULL
  tBoolArgCont   * boolCont   = argMap.argMRF.find(static_cast<tBoolArgItem *  >(NULL));
  tInt32ArgCont  * int32Cont  = argMap.argMRF.find(static_cast<tInt32ArgItem * >(NULL));
  tInt64ArgCont  * int64Cont  = argMap.argMRF.find(static_cast<tInt64ArgItem * >(NULL));
  tFloatArgCont  * floatCont  = argMap.argMRF.find(static_cast<tFloatArgItem * >(NULL));
  tDoubleArgCont * doubleCont = argMap.argMRF.find(static_cast<tDoubleArgItem *>(NULL));
  tCharArgCont   * charCont   = argMap.argMRF.find(static_cast<tCharArgItem *  >(NULL));
  tStringArgCont * stringCont = argMap.argMRF.find(static_cast<tStringArgItem *>(NULL));

  /* --- Step one: Boolean Arguments --- */
  if (boolCont)
    {
      int32_t size = boolCont->size();
      bool *target = NULL;
      for (int32_t i = 0; i < size; ++i)
        {
          tBoolArgItem *item = (*boolCont)[i];
          target = *(*item);
          if (item->getState() > 0)
            {
              if (item->getType() == ETT_TRUE)
                *target = true;
              else if (item->getType() == ETT_FALSE)
                *target = false;
            }
          item->done();
        }
    }

  /* --- Step two: 32 bit Integer Arguments --- */
  if (int32Cont)
    {
      int32_t  size   = int32Cont->size();
      int32_t *target = NULL;
      int32_t  minVal = 0;
      int32_t  maxVal = 0;
      for (int32_t i = 0; i < size; ++i)
        {
          tInt32ArgItem *item  = (*int32Cont)[i];
          double         itMin = item->getMinValue();
          double         itMax = item->getMaxValue();
          state   = ::std::max(static_cast<size_t>(item->getState()), item->getCompCount());
          target  = *(*item);
          minVal  = itMin < constants::minInt32Limit ? constants::minInt32Limit : static_cast<int32_t>(itMin);
          minVal  = itMax > constants::maxInt32Limit ? constants::maxInt32Limit : static_cast<int32_t>(itMax);
          compNum = 0;

          while (state > 0)
            {
              --state;
              if (item->getType() == ETT_DEC)
                --*target;
              else if (item->getType() == ETT_INC)
                ++*target;
              else
                {
                  if (compNum < item->getCompCount())
                    {
                      if (ETT_INT == item->getType())
                        *target = to_int32(item->getComponent(compNum));
                      else if (ETT_ADD == item->getType())
                        *target += to_int32(item->getComponent(compNum));
                      else if (ETT_SUB == item->getType())
                        *target -= to_int32(item->getComponent(compNum));
                      ++compNum;
                    }
                } // End of handling components
              // enforce minimum maximum values
              if (*target < minVal)
                *target = minVal;
              if (*target > maxVal)
                *target = maxVal;
            } // end of while state > 0
          // We are done
          item->done();
        } // End while state is not null;
    }

  /* --- Step three: 64 bit Integer Arguments --- */
  if (int64Cont)
    {
      int32_t  size   = int64Cont->size();
      int64_t *target = NULL;
      int64_t  minVal = 0;
      int64_t  maxVal = 0;
      for (int32_t i = 0; i < size; ++i)
        {
          tInt64ArgItem *item  = (*int64Cont)[i];
          double         itMin = item->getMinValue();
          double         itMax = item->getMaxValue();
          state   = ::std::max(static_cast<size_t>(item->getState()), item->getCompCount());
          target  = *(*item);
          minVal  = itMin < constants::minInt64Limit ? constants::minInt64Limit : static_cast<int64_t>(itMin);
          minVal  = itMax > constants::maxInt64Limit ? constants::maxInt64Limit : static_cast<int64_t>(itMax);
          compNum = 0;

          while (state > 0)
            {
              --state;
              if (item->getType() == ETT_DEC)
                --*target;
              else if (item->getType() == ETT_INC)
                ++*target;
              else
                {
                  if (compNum < item->getCompCount())
                    {
                      if (ETT_INT == item->getType())
                        *target = to_int64(item->getComponent(compNum));
                      else if (ETT_ADD == item->getType())
                        *target += to_int64(item->getComponent(compNum));
                      else if (ETT_SUB == item->getType())
                        *target -= to_int64(item->getComponent(compNum));
                      ++compNum;
                    }
                } // End of handling components
              // enforce minimum maximum values
              if (*target < minVal)
                *target = minVal;
              if (*target > maxVal)
                *target = maxVal;
            } // end of while state > 0
          // We are done
          item->done();
        } // End while state is not null;
    }

  /* --- Step four: Floating Point Arguments --- */
  if (floatCont)
    {
      int32_t size  = floatCont->size();
      float *target = NULL;
      float minVal  = 0;
      float maxVal  = 0;
      for (int32_t i = 0; i < size; ++i)
        {
          tFloatArgItem *item  = (*floatCont)[i];
          double         itMin = item->getMinValue();
          double         itMax = item->getMaxValue();
          state   = ::std::max(static_cast<size_t>(item->getState()), item->getCompCount());
          target  = *(*item);
          minVal  = itMin < constants::minFloatLimit ? constants::minFloatLimit : static_cast<float>(itMin);
          minVal  = itMax > constants::maxFloatLimit ? constants::maxFloatLimit : static_cast<float>(itMax);
          compNum = 0;
          while (state > 0)
            {
              --state;
              if (item->getType() == ETT_DEC)
                *target -= 1.0;
              else if (item->getType() == ETT_INC)
                *target += 1.0;
              else
                {
                  if (compNum < item->getCompCount())
                    {
                      if (ETT_FLOAT == item->getType())
                        *target = to_float(item->getComponent(compNum));
                      else if (ETT_ADD == item->getType())
                        *target += to_float(item->getComponent(compNum));
                      else if (ETT_SUB == item->getType())
                        *target -= to_float(item->getComponent(compNum));
                      ++compNum;
                    }
                } // End of handling components
              // enforce minimum maximum values
              if (*target < minVal)
                *target = minVal;
              if (*target > maxVal)
                *target = maxVal;
            } // end of while state > 0
          // We are done
          item->done();
        }
    }

  /* --- Step five: Double Arguments --- */
  if (doubleCont)
    {
      int32_t size    = doubleCont->size();
      double *target  = NULL;
      double  minVal  = 0;
      double  maxVal  = 0;
      for (int32_t i = 0; i < size; ++i)
        {
          tDoubleArgItem *item = (*doubleCont)[i];
          state   = ::std::max(static_cast<size_t>(item->getState()), item->getCompCount());
          target = *(*item);
          minVal  = item->getMinValue();
          maxVal  = item->getMaxValue();
          compNum = 0;
          while (state > 0)
            {
              --state;
              if (item->getType() == ETT_DEC)
                *target -= 1.0;
              else if (item->getType() == ETT_INC)
                *target += 1.0;
              else
                {
                  if (compNum < item->getCompCount())
                    {
                      if (ETT_FLOAT == item->getType())
                        *target = to_double(item->getComponent(compNum));
                      else if (ETT_ADD == item->getType())
                        *target += to_double(item->getComponent(compNum));
                      else if (ETT_SUB == item->getType())
                        *target -= to_double(item->getComponent(compNum));
                      ++compNum;
                    }
                } // End of handling components
              // enforce minimum maximum values
              if (*target < minVal)
                *target = minVal;
              if (*target > maxVal)
                *target = maxVal;
            } // end of while state > 0
          // We are done
          item->done();
        }
    }

  /* --- Step six: Char Arguments --- */
  if (charCont)
    {
      int32_t size = charCont->size();
      char *target = NULL;
      for (int32_t i = 0; i < size; ++i)
        {
          tCharArgItem *item = (*charCont)[i];
          state   = ::std::max(static_cast<size_t>(item->getState()), item->getCompCount());
          target = *(*item);
          compNum = 0;
          while (state > 0)
            {
              --state;
              if (compNum < item->getCompCount())
                {
                  // Is it a char? then we need the first character only
                  if (item->getType() == ETT_CHAR)
                    *target = item->getComponent(compNum)[0];
                  else if (item->getType() == ETT_STRING)
                    pwx_strncpy(target, item->getComponent(compNum), item->maxSize);
                  ++compNum;
                } // end of having components left
            } // end of while state > 0
          // We are done
          item->done();
        }
    }

  /* --- Step seven: String Arguments --- */
  if (stringCont)
    {
      int32_t size          = stringCont->size();
      ::std::string *target = NULL;
      for (int32_t i = 0; i < size; ++i)
        {
          tStringArgItem *item = (*stringCont)[i];
          state   = ::std::max(static_cast<size_t>(item->getState()), item->getCompCount());
          target = *(*item);
          compNum = 0;
          while (state > 0)
            {
              --state;
              if (compNum < item->getCompCount())
                {
                  // Use a callback function if available, otherwise do it directly:
                  if (item->cb)
                    item->cb(item->getComponent(compNum), item->getUserData());
                  else
                    {
                      // Is it a char? then we need the first character only
                      if (item->getType() == ETT_CHAR)
                        {
                          char ch[2] = { 0x0, 0x0 };
                          ch[0] = item->getComponent(compNum)[0];
                          target->assign(ch);
                        }
                      else if (item->getType() == ETT_STRING)
                        target->assign(item->getComponent(compNum));
                    } // End of handling components directly
                  ++compNum;
                } // end of having components left
              // If it is a pure callback function, it can be used without components, but the major key:
              else if ((0 == item->getCompC()) && item->cb && (ETT_CALLBACK == item->getType()))
                item->cb(strlen(item->getShortArg()) ? item->getShortArg() : item->getLongArg(), item->getUserData());
            } // end of while state > 0
          // We are done
          item->done();
        }
    }
}


/** @brief return the number of arguments currently recognized **/
int32_t size ()
{
  return argMap.size();
}

} // end of namespace args
} // end of namespace pwx

#endif // PWX_ARGIMPL_H_INCLUDED

