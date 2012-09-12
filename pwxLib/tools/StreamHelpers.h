#pragma once
#ifndef PWX_STREAMHELPERS_H_INCLUDED
#define PWX_STREAMHELPERS_H_INCLUDED 1

/** @file StreamHelpers.h
  *
  * @brief This file defines some tools to work with streams.
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
  * 0.0.1     2010-10-29  sed, PrydeWorX  First Design
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.0.1   2010-11-16  sed, PrydeWorX  Added stream formatters
  * 0.7.0.2   2010-11-17  sed, PrydeWorX  Fixed a bug that could cause readNextValue() to try to read
  *                                       empty fields (double separator condition)
  * 0.7.0.3   2011-01-05  sed, PrydeWorX  Some updates for compatibility with MSVC++ 2008.
  *                                       open, close and strdup are now wrappers for gcc-MSVC++-compatibility.
  * 0.7.0.4   2011-01-07  sed, PrydeWorX  Changed makeTemp() to work with MSVC++, too.
  * 0.7.0.5   2011-01-21  sed, PrydeWorX  added forwarding for adj* and prototypes for their ostream operators
  * 0.7.0.6   2011-02-28  sed, PrydeWorX  added to_string template for val->string functionality
  * 0.7.0.7   2011-03-06  sed, PrydeWorX  Welcome to 2011 !
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2.1   2011-05-03  sed, PrydeWorX  Added trim() for string references
  * 0.7.2.2   2011-05-30  sed, PrydeWorX  added cropShell() to free strings from shell variable representations
  * 0.7.2.3   2011-06-24  sed, PrydeWorX  Added the removal of nl and cr to trim()
  * 0.7.2.4   2011-07-06  sed, PrydeWorX  Fixed cropShell()
  * 0.7.2.5   2011-07-08  sed, PrydeWorX  Added optional character to remove from trim()
  *                                       Added ltrim() and rtrim() for mor trimming choice
  * 0.7.2.6   2011-07-13  sed, PrydeWorX  Added tabToSpace()
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0.1   2011-08-02  sed, PrydeWorX  Added missing PWX_INLINE to adjLeft/Rigth shift operators
  * 0.8.0.2   2011-09-05  sed, PrydeWorX  Fixed a "bug" (thinking error) in forwardTo()
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.1.1   2011-10-05  sed, PrydeWorX  makeTemp now works with suffix under VisualC++, too.
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5     2012-03-01  sed, PrydeWorX  Welcome to 2012. Version bump to new pwxLib release version
  * 0.8.5.1   2012-03-20  sed, PrydeWorX  Added to_float, to_double, to_int32 and to_int64
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

#include "pwxLib/internal/MSVCwarnPush.h"
#if defined (_MSC_VER)
#  include <io.h>
#  include <cerrno>
#  include <fcntl.h>
#  include <sys/stat.h>
#else
#  include <cstdlib>
#endif // include for mktemp
#include <fstream>
#include <ostream>
#include <vector>
#include <cstring>
#include <sstream>
#include "pwxLib/internal/MSVCwarnPop.h"
#include "pwxLib/tools/DefaultDefines.h"

/** @namespace pwx
  *
  * @brief Namespace for everything PrydeWorX related
  *
  * All PrydeWorX projects reside in this namespace and it's sub-namespaces.
**/
namespace pwx
{

/** @namespace pwx::StreamHelpers
  *
  * @brief Namespace for global stream helper functions
  *
  * This namespace will be filled whenever a helper function to work with streams is needed.
**/
namespace StreamHelpers
{
// --- forwarding for the prototypes
class adjLeft;
class adjRight;

// --- prototypes, used for attribute attachement and an overview: ---
PWX_INLINE ::std::ostream &operator<<(::std::ostream &os, const adjLeft& l);
PWX_INLINE ::std::ostream &operator<<(::std::ostream &os, const adjRight& r);
PWX_INLINE bool          cropShell    (const char *key, ::std::string &data);
PWX_INLINE void          forwardTo    (::std::ifstream &is, char value);
PWX_INLINE void          ltrim        (::std::string &text, char extra = 0x0);
PWX_INLINE const char  * makeTemp     (const char *aPath, const char *aTemplate,
                                       const char *aSuffix, ::std::ofstream &ofs,
                                       ::std::ios_base::openmode mode = ::std::ios_base::out|::std::ios_base::trunc)
                                       PWX_WARNUNUSED;
template <typename Tval>
PWX_INLINE bool          readNextValue(Tval &value, ::std::ifstream &is, char separator,
                                       bool search = false, bool emptyAllowed = true) PWX_WARNUNUSED;
PWX_INLINE void          rtrim        (::std::string &text, char extra = 0x0);
PWX_INLINE bool          skipLineBreak(::std::ifstream &is)                           PWX_WARNUNUSED;
PWX_INLINE void          tabToSpace   (::std::string &text, size_t spacePerTab = 1);
template <typename T>
PWX_INLINE double        to_double    (const T val);
template <typename T>
PWX_INLINE float         to_float     (const T val);
template <typename T>
PWX_INLINE int32_t       to_int32     (const T val);
template <typename T>
PWX_INLINE int64_t       to_int64     (const T val);
template <typename T>
PWX_INLINE ::std::string to_string    (const T val);
PWX_INLINE void          trim         (::std::string &text, char extra = 0x0);

// --- Classes for stream manipulation ---

/** @brief format a numerical stream output
  *
  * This is a base class for other stream formatters, allowing to set
  * the numerical width and floating point precision.
  *
  * The constructor can be used with two values:
  *   -# @a aLeft - The number of digits left of the floating point and
  *   -# @a aRight - The number of digits right of the floating point.
  *
  * Both default to zero which allows this class to be used as a base
  * class for formatters which do not deal with output width and
  * precision.
**/
class format
{
public:
  virtual ~format() { }
  explicit format(int32_t aLeft = 0, int32_t aRight = 0):
    left(aLeft), right(aRight) { }

  /** @brief set formatting fields on a stream
    *
    * This method sets the fields for width and precision on a stream
    * if the left and right number of digits is set. If a right
    * digit count is set, the stream will be set to ios_bas::fixed.
    * If both values are zero, the method does nothing.
    *
    * generally speaking this method is meant to be called from
    * operator<< implementations on child classes.
    *
    * @param[in,out] os an open and valid output stream to format
  **/
  void setFields(::std::ostream &os) const
  {
    if (os.good() && (left || right))
      {
        if (left)
          os.width(left + (right ? 1 + right : 0));
        if (right)
          {
            os.setf(::std::ios_base::fixed);
            os.precision(right);
          }
      }
  }

protected:
  int32_t left;  //!< The number of digits left of the floating point
  int32_t right; //!< The number of digits right of the floating point
};


/** @brief adjLeft
  *
  * Simple class to set an output stream to left adjustment
  * with optional width and precision.
  *
  * @see format ctor for argument description
**/
class adjLeft: public format
{
public:
  explicit adjLeft(int32_t aLeft = 0, int32_t aRight = 0):format(aLeft, aRight) { }
};

/** @brief operator<< for moving an adjLeft object upon a stream
  *
  * @param[in] os an open and valid output stream to format
  * @param[in] l an instance of adjLeft
  * @return the stream object
  *
**/
::std::ostream &operator<<(::std::ostream &os, const adjLeft& l)
{
  if (os.good())
    {
      os.setf(::std::ios_base::left, ::std::ios_base::adjustfield | ::std::ios_base::floatfield);
      l.setFields(os);
    }
  return (os);
}

/** @brief adjRight
  *
  * Simple class to set an output stream to right adjustment
  * with optional width and precision.
  *
  * @see format ctor for argument description
**/
class adjRight: public format
{
public:
  explicit adjRight(int32_t aLeft = 0, int32_t aRight = 0):format(aLeft, aRight) { }
};

/** @brief operator<< for moving an adjRight object upon a stream
  *
  * @param[in] os an open and valid output stream to format
  * @param[in] r an instance of adjRight
  * @return the stream object
  *
**/
::std::ostream &operator<<(::std::ostream &os, const adjRight& r)
{
  if (os.good())
    {
      os.setf(::std::ios_base::right, ::std::ios_base::adjustfield | ::std::ios_base::floatfield);
      r.setFields(os);
    }
  return (os);
}



// --- function definitions

/** @brief internal method to delete shell representations out of a string
  *
  * @param[in] key The key to search for
  * @param[in,out] data The data string to search
  * @return true if a representation was found.
**/
bool cropShell(const char *key, ::std::string &data)
  {
    ::std::string shLike = "$";
    shLike += key;
    size_t pos = data.find(shLike);
    bool hasRep = (pos != data.npos) ? true : false;

    // If we have not found one, this might say nothing if the keyword is encased in curly brackets:
    if (!hasRep)
      {
        shLike.insert(1, "{");
        shLike += "}";
        pos = data.find(shLike);
        if (pos != data.npos)
          hasRep = true;
      }

    // If we have found one, it has to be erased from the given string
    if (hasRep)
      {
        while (pos != data.npos)
          {
            data.erase(pos, shLike.size());
            pos = data.find(shLike);
          }
      }

    return hasRep;
  }


/** @brief forward to a specific value
  *
  * This method will forward the given ifstream @a is until
  * a) eof() is reached, or b) the wanted @a value is found.
  * Note: @a value is discarded, and _not_ read! This function
  *       is meant to help forwarding to fields after specific
  *       separators or identifiers!
  *
  * @param[in] is the ifstream to manipulate
  * @param[in] value the value to look for and jump after
**/
void forwardTo(::std::ifstream &is, char value)
{
  while (is.good() && (is.peek() != value))
    is.ignore(1);
  if (is.good() && is.peek() == value)
    is.ignore(1);
}


/** @brief delete all newline, carriage return, tabs and spaces at the beginning of a string
  * @param[in,out] text a string reference that will be trimmed
  * @param[in] extra optional extra character to be trimmed as well, defaults to 0
**/
void ltrim (::std::string &text, char extra)
  {
    if ((extra == '\r') || (extra == '\n') || (extra == ' ') || (extra == '\t'))
      extra = 0x0;
    while (text.size() &&
          ( (text[0] == '\r') || (text[0] == '\n')
          ||(text[0] == ' ')  || (text[0] == '\t')
          ||(extra && text[0] == extra)) )
      text.erase(0, 1);
  }


/** @brief create a temporary file
  *
  * This method is a wrapper to use mkstemp() with ofstream.
  * Furthermore it ensures, that the template you provide is
  * safe, meaning it is ensured to end with XXXXXX.
  *
  * The return string is a malloc'd C-String, so you have to free() it!
  *
  * @param[in] aPath The path where to create the file. The path will *NOT* be created!
  * @param[in] aTemplate Your naming template. It can be anything but has to end with XXXXXX, or will be made to do so
  * @param[in] aSuffix an optional suffix for your temp file, or NULL if you do not want one
  * @param[in] ofs an ofstream that will be set to and opened with the created temp file
  * @param[in] mode you can specify a non-default openmode if you like.
  * @return the file name on success, so you can get rid of the file, or NULL if something went wrong
**/
const char *makeTemp(const char *aPath, const char *aTemplate, const char *aSuffix,
                      ::std::ofstream &ofs, ::std::ios_base::openmode mode)
{
  try
    {
      ::std::string fileName(aPath);
      int32_t suffLen = aSuffix ? strlen(aSuffix) : 0;

      // If a template is given, use it
      if (aTemplate && strlen(aTemplate))
        {
          if (fileName.size() && fileName[fileName.size() - 1] != '\\')
            fileName += "/";
          fileName += aTemplate;
          // We need to ensure that the last six characters are capital X:
          while (STRNE(fileName.substr(fileName.size() > 6 ? fileName.size() - 6 : 0).c_str(), "XXXXXX"))
            fileName += "X";
        }
      else
        // Otherwise we have our default here:
        fileName += "/pwxTemp_XXXXXX";

#if !defined(_MSC_VER) // MSVC++ mktemp_s can't handle a suffix!
      // Add the suffix if we have one:
      if (suffLen)
        {
          if (aSuffix[0] != '.')
            {
              fileName += ".";
              ++suffLen;
            }
          fileName += aSuffix;
        }
#endif // Suffix for non-MS only here

      // Unfortunately mkstemp() can't use a const_char, so c_str() isn't our method
      ::std::vector<char> destination(fileName.begin(), fileName.end());
      destination.push_back('\0');

      int32_t fd = -1;

#if defined(_MSC_VER)
      if (EINVAL != (_mktemp_s(&destination[0], fileName.size() + 1)))
        fd = 0;
      // The Microsoft version does not open the file. This is bad, but we can't help it
#else
      if (suffLen)
        fd = mkstemps(&destination[0], suffLen);
      else
        fd = mkstemp(&destination[0]);
#endif // mktemp with MSC or GNU

      if(fd != -1)
        {
          fileName.assign(destination.begin(), destination.end() - 1);

#if defined(_MSC_VER) // MS has no suffix, yet, add it now:
          if (suffLen)
            {
              if (aSuffix[0] != '.')
                {
                  fileName += ".";
                  ++suffLen;
                }
              fileName += aSuffix;
            }
#endif // Late suffix for MS version

          ofs.open(fileName.c_str(), mode);
#if !defined(_MSC_VER)
          close(fd); // The MSVC++ version does not need this
#endif
          return (pwx_strdup(fileName.c_str()));
        }
    }
  PWX_CATCH_AND_FORGET(::std::exception)
  // This simple catch'all should ensure that nothing leaves this.
  return (NULL);
}


/** @brief get the next seperated value
  *
  * This method extracts a value from an ifstream into @a value.
  * If a @a separator is given, the method fails if the next value
  * in the stream is something else if @a search is false. But if
  * @a search is set to true, the method will forward to the next
  * @a separator in the stream, and extract the wanted value from
  * after that.
  * If the found separator is followed by another separator, the
  * method either fails if @a emptyAllowed is set to false, or will
  * simply not touch @a value if @a emptyAllowed is set to true.
  *
  * If you use objects with this method, please make sure they
  * support operator>>(&ifstream).
  *
  * This method never throws, but returns false on failure.
  *
  * @tparam Tval a type that supports operator>> to load data into values of this type
  * @param[in,out] value a read/write reference of the value to write into
  * @param[in] is the stream to extract from
  * @param[in] separator a char to advance to if not zero
  * @param[in] search if set to true, the method forwards to the next @a separator, otherwise it has to be the next char
  * @param[in] emptyAllowed if set to false, the method fails, if the next @a separator is followed by another one
  * @return true on success, false on error
  *
**/
template <typename Tval>
bool readNextValue( Tval &value, ::std::ifstream &is, char separator,
                    bool search, bool emptyAllowed)
{
  bool result   = false;
  bool sepFound = true;

  if (is.good())
    {

      // First check whether we have to jump behind a separator or not:
      if (separator)
        {
          // We need a separator
          if (search)
            {
              // How fortunate, we can fast forward if necessary
              forwardTo(is, separator);
              if (!is.good())
                sepFound = false;
            }
          else
            {
              // The next one must be a separator value:
              if (is.peek() == separator)
                is.ignore(1);
              else
                sepFound = false;
            }
          // Now there could be the case of two separators:
          if (is.peek() == separator)
            {
              // Aha... so we either fail, or are finished:
              sepFound = false; // Skip the reading part
              if (emptyAllowed)
                result = true; // But it's alright
            }
        } // End of separator extraction
      // Now we have succeeded if our separator is found or non needed:
      if (sepFound && !is.eof())
        result = true;
    }

  if (result && sepFound && is.good())
    is >> value;

  if (!is.good())
    result = false;

  return (result);
}


/** @brief delete all newline, carriage return, tabs and spaces at the end of a string
  * @param[in,out] text a string reference that will be trimmed
  * @param[in] extra optional extra character to be trimmed as well, defaults to 0
**/
void rtrim (::std::string &text, char extra)
  {
    if ((extra == '\r') || (extra == '\n') || (extra == ' ') || (extra == '\t'))
      extra = 0x0;
    size_t end = text.size() ? text.size() - 1 : 0;

    while (text.size() &&
          ( (text[end] == '\r') || (text[end] == '\n')
          ||(text[end] == ' ')  || (text[end] == '\t')
          ||(extra && text[end] == extra)) )
      text.erase(end--, 1);
  }


/** @brief skip the following line break and carriage return
  *
  * This function ignores all following CR/LF characters in the
  * open ifstream @a is until a different character is met.
  * If afterwards the stream is in order or eof is ahead, the
  * function returns true, false if a different failbit is set.
  *
  * @param[in] is an open ifstream
  * @return false if something fails, true if a different char or eof is met
**/
bool skipLineBreak(::std::ifstream &is)
{
  bool    result = false;
  int32_t peek   = 0;

  if (is.good())
    {
      peek = is.peek();
      while (is.good() && ((peek == 0x0a) || (peek == 0x0d)) )
        {
          is.ignore(1);
          peek = is.peek();
        }
    }
  // If >>is<< is still good (or eof), we have finished:
  if (is.good() || (0 == (is.rdstate() ^ ::std::ios_base::eofbit)))
    // Note: we use XOR, because this leaves all other failbits untouched,
    // resulting in !0 if something else is set as well
    result = true;

  return result;
}


/** @brief convert all tabs in a string to spaces
  *
  * This functions replaces all tabs in a string with a variable number
  * of spaces, one space per tabulator character by default.
  *
  * If the second parameter @a spacePerTab is set to zero, tabs are
  * not replaced but simply deleted.
  *
  * @param[in,out] text reference on the string to search
  * @param[in] spacePerTab number of spaces per tab.
**/
void tabToSpace (::std::string &text, size_t spacePerTab)
  {
    if (text.size())
      {
        size_t pos = text.find_first_of('\t');

        while (pos != text.npos)
          {
            text.erase(pos, 1);
            if (spacePerTab)
              text.insert(pos, spacePerTab, ' ');
            // Now search next tab
            pos = text.find_first_of('\t', pos);
          }
      } // End of having text
  }


/** @brief convert a value to float
  *
  * This function uses a stringstream to convert @a val to a float.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting float
**/
template <typename T>
float to_float(const T val)
  {
    float result = 0.;
    ::std::stringstream ss;
    ss << val;
    ss >> result;
    return result;
  }


/** @brief convert a value to dlouble
  *
  * This function uses a stringstream to convert @a val to a double.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting double
**/
template <typename T>
double to_double(const T val)
  {
    double result = 0.;
    ::std::stringstream ss;
    ss << val;
    ss >> result;
    return result;
  }


/** @brief convert a value to int32_t
  *
  * This function uses a stringstream to convert @a val to a int32_t.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting int32_t
**/
template <typename T>
int32_t to_int32(const T val)
  {
    int32_t result = 0;
    ::std::stringstream ss;
    ss << val;
    ss >> result;
    return result;
  }


/** @brief convert a value to int64_t
  *
  * This function uses a stringstream to convert @a val to a int64_t.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting int64_t
**/
template <typename T>
int64_t to_int64(const T val)
  {
    int64_t result = 0;
    ::std::stringstream ss;
    ss << val;
    ss >> result;
    return result;
  }


/** @brief convert a value to a string
  *
  * This function uses a stringstream to convert @a val to a string.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting int64_t
**/
template <typename T>
::std::string to_string(const T val)
  {
    ::std::stringstream ss;
    ss << val;
    return ss.str();
  }


/** @brief delete all newline, carriage return, tabs and spaces at the beginning and end of a string
  * @param[in,out] text a string reference that will be trimmed
  * @param[in] extra optional extra character to be trimmed as well, defaults to 0
**/
void trim (::std::string &text, char extra)
  {
    ltrim(text, extra);
    rtrim(text, extra);
  }


} // End of namespace StreamHelpers

} // End of namespace pwx

#endif // PWX_STREAMHELPERS_H_INCLUDED
