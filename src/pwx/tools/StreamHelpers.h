#pragma once
#ifndef PWX_LIBPWX_PWX_TOOLS_STREAMHELPERS_H_INCLUDED
#define PWX_LIBPWX_PWX_TOOLS_STREAMHELPERS_H_INCLUDED 1

/** @file StreamHelpers.h
  *
  * @brief This file defines some tools to work with streams.
  *
  * (c) 2007-2012 Sven Eden, PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
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
  * History and Changelog are maintained in pwx.h
**/

#include <fstream>
#include <sstream>

#include <pwx/general/compiler.h>
#include <pwx/general/macros.h>

namespace pwx
{

// --- forwarding for the prototypes
class adjLeft;
class adjRight;

PWX_INLINE ::std::ostream PWX_API &operator<< (::std::ostream &os, const adjLeft& l)  noexcept;
PWX_INLINE ::std::ostream PWX_API &operator<< (::std::ostream &os, const adjRight& r) noexcept;
PWX_INLINE bool           PWX_API cropShell (const char *key, ::std::string &data)  noexcept;
PWX_INLINE void           PWX_API forwardTo (::std::ifstream &is, char value)       noexcept;
PWX_INLINE void           PWX_API ltrim (::std::string &text, char extra = 0x0) noexcept;
PWX_INLINE const char     PWX_API *makeTemp (const char *aPath, const char *aTemplate,
											const char *aSuffix, ::std::ofstream &ofs,
											::std::ios_base::openmode mode = ::std::ios_base::out |::std::ios_base::trunc)
											noexcept PWX_WARNUNUSED;
template <typename Tval>
PWX_INLINE bool           PWX_API readNextValue (Tval &value, ::std::ifstream &is, char separator,
												bool search = false, bool emptyAllowed = true)
												noexcept PWX_WARNUNUSED;
PWX_INLINE void           PWX_API rtrim (::std::string &text, char extra = 0x0)	noexcept;
PWX_INLINE bool           PWX_API skipLineBreak (::std::ifstream &is) noexcept PWX_WARNUNUSED;
PWX_INLINE void           PWX_API tabToSpace (::std::string &text, size_t spacePerTab = 1) noexcept;
template <typename T>
PWX_INLINE double         PWX_API to_double (const T val) noexcept;
template <typename T>
PWX_INLINE float          PWX_API to_float (const T val) noexcept;
template <typename T>
PWX_INLINE int32_t        PWX_API to_int32 (const T val) noexcept;
template <typename T>
PWX_INLINE uint32_t       PWX_API to_uint32 (const T val) noexcept;
template <typename T>
PWX_INLINE int64_t        PWX_API to_int64 (const T val) noexcept;
template <typename T>
PWX_INLINE uint64_t       PWX_API to_uint64 (const T val) noexcept;
template <typename T>
PWX_INLINE ::std::string  PWX_API to_string (const T val) noexcept;
PWX_INLINE void           PWX_API trim (::std::string &text, char extra = 0x0) noexcept;

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
class PWX_API format
{
public:
	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/
	format (int32_t left_, int32_t right_) noexcept;
	format() noexcept;

	virtual ~format() noexcept PWX_DEFAULT;

	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/
	void setFields (::std::ostream &os) const noexcept;
protected:
	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/
	int32_t left  = 0; //!< The number of digits left of the floating point
	int32_t right = 0; //!< The number of digits right of the floating point
}; // class format

/** @brief adjLeft
  *
  * Simple class to set an output stream to left adjustment
  * with optional width and precision.
  *
  * @see format ctor for argument description
**/
class PWX_API adjLeft: public format
{
public:
	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/
	adjLeft (int32_t left_, int32_t right_) noexcept;
	adjLeft() noexcept;

	~adjLeft() noexcept PWX_DEFAULT;
};

/** @brief adjRight
  *
  * Simple class to set an output stream to right adjustment
  * with optional width and precision.
  *
  * @see format ctor for argument description
**/
class PWX_API adjRight: public format
{
public:
	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/
	adjRight (int32_t left_, int32_t right_) noexcept;
	adjRight() noexcept;

	~adjRight() noexcept PWX_DEFAULT;
};



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
bool readNextValue (Tval &value, ::std::ifstream &is, char separator,
					bool search, bool emptyAllowed) noexcept {
	bool result   = false;
	bool sepFound = true;

	if (is.good()) {

		// First check whether we have to jump behind a separator or not:
		if (separator) {
			// We need a separator
			if (search) {
				// How fortunate, we can fast forward if necessary
				forwardTo (is, separator);
				if (!is.good())
					sepFound = false;
			} else {
				// The next one must be a separator value:
				if (is.peek() == separator)
					is.ignore (1);
				else
					sepFound = false;
			}
			// Now there could be the case of two separators:
			if (is.peek() == separator) {
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
float to_float (const T val) noexcept {
	float result = 0.;
	::std::stringstream ss;
	ss << val;
	ss >> result;
	return result;
}


/** @brief convert a value to double
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
double to_double (const T val) noexcept {
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
int32_t to_int32 (const T val) noexcept {
	int32_t result = 0;
	::std::stringstream ss;
	ss << val;
	ss >> result;
	return result;
}


/** @brief convert a value to uint32_t
  *
  * This function uses a stringstream to convert @a val to a uint32_t.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting uint32_t
**/
template <typename T>
uint32_t to_uint32 (const T val) noexcept {
	uint32_t result = 0;
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
int64_t to_int64 (const T val) noexcept {
	int64_t result = 0;
	::std::stringstream ss;
	ss << val;
	ss >> result;
	return result;
}


/** @brief convert a value to uint64_t
  *
  * This function uses a stringstream to convert @a val to a int64_t.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting uint64_t
**/
template <typename T>
uint64_t to_uint64 (const T val) noexcept {
	uint64_t result = 0;
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
::std::string to_string (const T val) noexcept {
	::std::stringstream ss;
	ss << val;
	return ss.str();
}


} // End of namespace pwx


#endif // PWX_LIBPWX_PWX_TOOLS_STREAMHELPERS_H_INCLUDED

