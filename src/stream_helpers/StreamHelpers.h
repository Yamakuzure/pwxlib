#ifndef PWX_LIBPWX_PWX_TOOLS_STREAMHELPERS_H_INCLUDED
#define PWX_LIBPWX_PWX_TOOLS_STREAMHELPERS_H_INCLUDED 1
#pragma once

/** @file StreamHelpers.h
  *
  * @brief This file defines some tools to work with streams.
  *
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  * History and change log are maintained in pwxlib.h
**/


#include <cstddef>
#include <cstdint>
#include <fstream>
#include <sstream>

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"


namespace pwx {


bool        cropShell    ( char const* key, std::string& data )  noexcept PWX_API;
void        forwardTo    ( std::ifstream& is, char value )       noexcept PWX_API;
void        ltrim        ( std::string& text, char extra = 0x0 ) noexcept PWX_API;
char const* makeTemp     ( char const* aPath, char const* aTemplate,
                           char const* aSuffix, std::ofstream& ofs,
                           std::ios_base::openmode mode
                           = std::ios_base::out | std::ios_base::trunc )
noexcept PWX_WARNUNUSED PWX_API;
void        rtrim        ( std::string& text, char extra = 0x0       ) noexcept PWX_API;
bool        skipLineBreak( std::ifstream& is                         ) noexcept PWX_WARNUNUSED PWX_API;
void        tabToSpace   ( std::string& text, size_t spacePerTab = 1 ) noexcept PWX_API;
void        trim         ( std::string& text, char extra = 0x0       ) noexcept PWX_API;

// --- Classes for stream manipulation ---

/** @brief CFormat a numerical stream output
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
class PWX_API CFormat {
public:
	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/
	CFormat ( int32_t left_, int32_t right_ ) noexcept;
	CFormat() noexcept;

	virtual ~CFormat() noexcept PWX_DEFAULT;

	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/
	void setFields ( std::ostream& os ) const noexcept;
protected:
	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/
	int32_t left  = 0; //!< The number of digits left of the floating point
	int32_t right = 0; //!< The number of digits right of the floating point
}; // class CFormat

/** @brief CAdjLeft
  *
  * Simple class to set an output stream to left adjustment
  * with optional width and precision.
  *
  * @see CFormat ctor for argument description
**/
class PWX_API CAdjLeft: public CFormat {
public:
	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/
	CAdjLeft ( int32_t left_, int32_t right_ ) noexcept;
	CAdjLeft() noexcept;

	~CAdjLeft() noexcept PWX_DEFAULT;
};


/** @brief CAdjRight
  *
  * Simple class to set an output stream to right adjustment
  * with optional width and precision.
  *
  * @see CFormat ctor for argument description
**/
class PWX_API CAdjRight: public CFormat {
public:
	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/
	CAdjRight ( int32_t left_, int32_t right_ ) noexcept;
	CAdjRight() noexcept;

	~CAdjRight() noexcept PWX_DEFAULT;
};


std::ostream& operator<< ( std::ostream& os, const CAdjLeft& l ) noexcept PWX_API;
std::ostream& operator<< ( std::ostream& os, const CAdjRight& r ) noexcept PWX_API;


/** @brief get the next seperated value
  *
  * This function extracts a value from an ifstream into @a value.
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
bool readNextValue ( Tval& value, std::ifstream& is, char separator,
                     bool search, bool emptyAllowed ) noexcept {
	bool result   = false;
	bool sepFound = true;

	if ( is.good() ) {

		// First check whether we have to jump behind a separator or not:
		if ( separator ) {
			// We need a separator
			if ( search ) {
				// How fortunate, we can fast forward if necessary
				forwardTo ( is, separator );
				if ( !is.good() )
					sepFound = false;
			} else {
				// The next one must be a separator value:
				if ( is.peek() == separator )
					is.ignore ( 1 );
				else
					sepFound = false;
			}
			// Now there could be the case of two separators:
			if ( is.peek() == separator ) {
				// Aha... so we either fail, or are finished:
				sepFound = false; // Skip the reading part
				if ( emptyAllowed )
					result = true; // But it's alright
			}
		} // End of separator extraction
		// Now we have succeeded if our separator is found or non needed:
		if ( sepFound && !is.eof() )
			result = true;
	}

	if ( result && sepFound && is.good() )
		is >> value;

	if ( !is.good() )
		result = false;

	return ( result );
}


/** @brief convert a value to bool
  *
  * This function uses a stringstream to convert @a val to a bool.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting bool
**/
template <typename T>
bool to_bool ( const T val ) noexcept {
	bool result = false;
	std::stringstream ss;
	ss << val;
	ss >> result;
	return result;
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
float to_float ( const T val ) noexcept {
	float result = 0.;
	std::stringstream ss;
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
double to_double ( const T val ) noexcept {
	double result = 0.;
	std::stringstream ss;
	ss << val;
	ss >> result;
	return result;
}


/** @brief convert a value to long double
  *
  * This function uses a stringstream to convert @a val to a long
  * double. The value will not be type-checked, so it is the users
  * responsibility to use a type that is compatible with stringstream.
  * Of course the compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting long double
**/
template <typename T>
long double to_long_double ( const T val ) noexcept {
	long double result = 0.;
	std::stringstream ss;
	ss << val;
	ss >> result;
	return result;
}


/** @brief convert a value to int8_t
  *
  * This function uses a stringstream to convert @a val to a int8_t.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting int8_t
**/
template <typename T>
int8_t to_int8 ( const T val ) noexcept {
	int8_t result = 0;
	std::stringstream ss;
	ss << val;
	ss >> result;
	return result;
}


/** @brief convert a value to uint8_t
  *
  * This function uses a stringstream to convert @a val to a uint8_t.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting uint8_t
**/
template <typename T>
uint8_t to_uint8 ( const T val ) noexcept {
	uint8_t result = 0;
	std::stringstream ss;
	ss << val;
	ss >> result;
	return result;
}


/** @brief convert a value to int16_t
  *
  * This function uses a stringstream to convert @a val to a int16_t.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting int16_t
**/
template <typename T>
int16_t to_int16 ( const T val ) noexcept {
	int16_t result = 0;
	std::stringstream ss;
	ss << val;
	ss >> result;
	return result;
}


/** @brief convert a value to uint16_t
  *
  * This function uses a stringstream to convert @a val to a uint16_t.
  * The value will not be type-checked, so it is the users responsibility
  * to use a type that is compatible with stringstream. Of course the
  * compiler will error out if the type is incompatible.
  *
  * @param[in] val the value to be converted
  * @return the resulting uint16_t
**/
template <typename T>
uint16_t to_uint16 ( const T val ) noexcept {
	uint16_t result = 0;
	std::stringstream ss;
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
int32_t to_int32 ( const T val ) noexcept {
	int32_t result = 0;
	std::stringstream ss;
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
uint32_t to_uint32 ( const T val ) noexcept {
	uint32_t result = 0;
	std::stringstream ss;
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
int64_t to_int64 ( const T val ) noexcept {
	int64_t result = 0;
	std::stringstream ss;
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
uint64_t to_uint64 ( const T val ) noexcept {
	uint64_t result = 0;
	std::stringstream ss;
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
std::string to_string ( const T val ) noexcept {
	std::stringstream ss;
	ss << val;
	return ss.str();
}


} // End of namespace pwx


#endif // PWX_LIBPWX_PWX_TOOLS_STREAMHELPERS_H_INCLUDED

