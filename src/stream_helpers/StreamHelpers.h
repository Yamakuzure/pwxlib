#ifndef PWX_LIBPWX_PWX_TOOLS_STREAMHELPERS_H_INCLUDED
#define PWX_LIBPWX_PWX_TOOLS_STREAMHELPERS_H_INCLUDED 1
#pragma once

/** @file StreamHelpers.h
  *
  * @brief This file defines some tools to work with streams.
  *
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
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

#include "basic/compiler.h"
#include "basic/macros.h"
#include "basic/types.h"
#include "math_helpers/MathHelpers.h"

#if defined(HAVE___INT128_T) || defined(HAVE___UINT128_T)
#  include <cstring>
#  include "basic/CException.h"
#endif // to_[u]int128() can throw!


namespace pwx {


bool cropShell( char const* key, std::string& data ) noexcept PWX_API;
void forwardTo( std::ifstream& is, char value ) noexcept PWX_API;
void ltrim( std::string& text, char extra = 0x0 ) noexcept PWX_API;
char const* makeTemp( char const* aPath, char const* aTemplate,
                      char const* aSuffix, std::ofstream& ofs,
                      std::ios_base::openmode mode
                      = std::ios_base::out | std::ios_base::trunc )
noexcept PWX_WARNUNUSED PWX_API;
void rtrim( std::string& text, char extra = 0x0 ) noexcept PWX_API;
bool skipLineBreak( std::ifstream& is ) noexcept PWX_WARNUNUSED PWX_API;
void tabToSpace( std::string& text, size_t spacePerTab = 1 ) noexcept PWX_API;
void trim( std::string& text, char extra = 0x0 ) noexcept PWX_API;

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
	CFormat( int32_t left_, int32_t right_ ) noexcept;
	CFormat() noexcept;

	virtual ~CFormat() noexcept PWX_DEFAULT;

	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/
	void setFields( std::ostream& os ) const noexcept;
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
class PWX_API CAdjLeft : public CFormat {
public:
	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/
	CAdjLeft( int32_t left_, int32_t right_ ) noexcept;
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
class PWX_API CAdjRight : public CFormat {
public:
	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/
	CAdjRight( int32_t left_, int32_t right_ ) noexcept;
	CAdjRight() noexcept;

	~CAdjRight() noexcept PWX_DEFAULT;
};


std::ostream& operator<<( std::ostream& os, const CAdjLeft& l ) noexcept PWX_API;
std::ostream& operator<<( std::ostream& os, const CAdjRight& r ) noexcept PWX_API;


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
template< typename Tval >
	bool readNextValue( Tval& value, std::ifstream& is, char separator,
	                    bool search, bool emptyAllowed ) noexcept {
		bool result   = false;
		bool sepFound = true;

		if ( is.good() ) {

			// First check whether we have to jump behind a separator or not:
			if ( separator ) {
				// We need a separator
				if ( search ) {
					// How fortunate, we can fast forward if necessary
					forwardTo( is, separator );
					if ( !is.good() ) {
						sepFound = false;
					}
				} else {
					// The next one must be a separator value:
					if ( is.peek() == separator ) {
						is.ignore( 1 );
					} else {
						sepFound = false;
					}
				}
				// Now there could be the case of two separators:
				if ( is.peek() == separator ) {
					// Aha... so we either fail, or are finished:
					sepFound = false; // Skip the reading part
					if ( emptyAllowed ) {
						result = true;
					} // But it's alright
				}
			} // End of separator extraction
			// Now we have succeeded if our separator is found or non needed:
			if ( sepFound && !is.eof() ) {
				result = true;
			}
		}

		if ( result && sepFound && is.good() ) {
			is >> value;
		}

		if ( !is.good() ) {
			result = false;
		}

		return ( result );
	}


typedef std::stringstream ss_t;


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
template< typename T >
	inline bool to_bool( const T val ) noexcept {
		if ( isIntType( T ) ) {
			return ( 0 != val );
		} else if ( isFloatType( T ) ) {
			return areAlmostEqual( val, (T) 0 );
		}

		bool result = false;
		ss_t ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> bool to_bool( char const* val ) noexcept PWX_API;
template<> bool to_bool( std::string& val ) noexcept PWX_API;
template<> inline bool to_bool( char* val ) noexcept {
	char const* v = val;
	return to_bool( v );
}
#endif // PWX_NODOX


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
template< typename T >
	inline float to_float( const T val ) noexcept {
		if ( isIntType( T ) || isFloatType( T ) ) {
			return static_cast<float>( val );
		}

		float result = 0.f;
		ss_t  ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> float to_float( char const* val ) noexcept PWX_API;
template<> float to_float( std::string& val ) noexcept PWX_API;
template<> inline float to_float( char* val ) noexcept {
	char const* v = val;
	return to_float( v );
}
#endif // PWX_NODOX


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
template< typename T >
	inline double to_double( const T val ) noexcept {
		if ( isIntType( T ) || isFloatType( T ) ) {
			return static_cast<double>( val );
		}

		double result = 0.;
		ss_t   ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> double to_double( char const* val ) noexcept PWX_API;
template<> double to_double( std::string& val ) noexcept PWX_API;
template<>
	inline double to_double( char* val ) noexcept {
		char const* v = val;
		return to_double( v );
	}
#endif // PWX_NODOX


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
template< typename T >
	inline long double to_long_double( const T val ) noexcept {
		if ( isIntType( T ) || isFloatType( T ) ) {
			return static_cast<long double>( val );
		}

		long double result = 0.;
		ss_t        ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> long double to_long_double( char const* val ) noexcept PWX_API;
template<> long double to_long_double( std::string& val ) noexcept PWX_API;
template<>
	inline long double to_long_double( char* val ) noexcept {
		char const* v = val;
		return to_long_double( v );
	}
#endif // PWX_NODOX


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
template< typename T >
	inline int8_t to_int8( const T val ) noexcept {
		if ( isIntType( T ) ) {
			return static_cast<int8_t>( 0xff & val );
		} else if ( isFloatType( T ) ) {
			return static_cast<int8_t>( 0xff & static_cast<int32_t>( val + .5f ) );
		}

		int8_t result = 0;
		ss_t   ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> int8_t to_int8( char const* val ) noexcept PWX_API;
template<> int8_t to_int8( std::string& val ) noexcept PWX_API;
template<>
	inline int8_t to_int8( char* val ) noexcept {
		char const* v = val;
		return to_int8( v );
	}
#endif // PWX_NODOX


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
template< typename T >
	inline uint8_t to_uint8( const T val ) noexcept {
		if ( isIntType( T ) ) {
			return static_cast<uint8_t>( 0xff & val );
		} else if ( isFloatType( T ) ) {
			return static_cast<uint8_t>( 0xff & static_cast<uint32_t>( val + .5f ) );
		}

		uint8_t result = 0;
		ss_t    ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> uint8_t to_uint8( char const* val ) noexcept PWX_API;
template<> uint8_t to_uint8( std::string& val ) noexcept PWX_API;
template<>
	inline uint8_t to_uint8( char* val ) noexcept {
		char const* v = val;
		return to_uint8( v );
	}
#endif // PWX_NODOX


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
template< typename T >
	inline int16_t to_int16( const T val ) noexcept {
		if ( isIntType( T ) ) {
			return static_cast<int16_t>( 0xffff & val );
		} else if ( isFloatType( T ) ) {
			return static_cast<int16_t>( 0xffff & static_cast<int32_t>( val + .5f ) );
		}

		int16_t result = 0;
		ss_t    ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> int16_t to_int16( char const* val ) noexcept PWX_API;
template<> int16_t to_int16( std::string& val ) noexcept PWX_API;
template<>
	inline int16_t to_int16( char* val ) noexcept {
		char const* v = val;
		return to_int16( v );
	}
#endif // PWX_NODOX


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
template< typename T >
	inline uint16_t to_uint16( const T val ) noexcept {
		if ( isIntType( T ) ) {
			return static_cast<uint16_t>( 0xffff & val );
		} else if ( isFloatType( T ) ) {
			return static_cast<uint16_t>( 0xffff & static_cast<uint32_t>( val + .5f ) );
		}

		uint16_t result = 0;
		ss_t     ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> uint16_t to_uint16( char const* val ) noexcept PWX_API;
template<> uint16_t to_uint16( std::string& val ) noexcept PWX_API;
template<>
	inline uint16_t to_uint16( char* val ) noexcept {
		char const* v = val;
		return to_uint16( v );
	}
#endif // PWX_NODOX


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
template< typename T >
	inline int32_t to_int32( const T val ) noexcept {
		if ( isIntType( T ) ) {
			return static_cast<int32_t>( 0xffffffff & val );
		} else if ( isFloatType( T ) ) {
			return ( 0xffffffff & static_cast<int32_t>( val + .5f ) );
		}

		int32_t result = 0;
		ss_t    ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> int32_t to_int32( char const* val ) noexcept PWX_API;
template<> int32_t to_int32( std::string& val ) noexcept PWX_API;
template<>
	inline int32_t to_int32( char* val ) noexcept {
		char const* v = val;
		return to_int32( v );
	}
#endif // PWX_NODOX


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
template< typename T >
	inline uint32_t to_uint32( const T val ) noexcept {
		if ( isIntType( T ) ) {
			return static_cast<uint32_t>( 0xffffffff & val );
		} else if ( isFloatType( T ) ) {
			return ( 0xffffffff & static_cast<uint32_t>( val + .5f ) );
		}

		uint32_t result = 0;
		ss_t     ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> uint32_t to_uint32( char const* val ) noexcept PWX_API;
template<> uint32_t to_uint32( std::string& val ) noexcept PWX_API;
template<>
	inline uint32_t to_uint32( char* val ) noexcept {
		char const* v = val;
		return to_uint32( v );
	}
#endif // PWX_NODOX


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
template< typename T >
	inline int64_t to_int64( const T val ) noexcept {
		if ( isIntType( T ) ) {
			return static_cast<int64_t>( val );
		} else if ( isFloatType( T ) ) {
			return static_cast<int64_t>( val + .5f );
		}

		int64_t result = 0;
		ss_t    ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> int64_t to_int64( char const* val ) noexcept PWX_API;
template<> int64_t to_int64( std::string& val ) noexcept PWX_API;
template<>
	inline int64_t to_int64( char* val ) noexcept {
		char const* v = val;
		return to_int64( v );
	}
#endif // PWX_NODOX


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
template< typename T >
	inline uint64_t to_uint64( const T val ) noexcept {
		if ( isIntType( T ) ) {
			return static_cast<int64_t>( val );
		} else if ( isFloatType( T ) ) {
			return static_cast<int64_t>( val + .5f );
		}

		uint64_t result = 0;
		ss_t     ss;
		ss << val;
		ss >> result;
		return result;
	}
#ifndef PWX_NODOX
template<> uint64_t to_uint64( char const* val ) noexcept PWX_API;
template<> uint64_t to_uint64( std::string& val ) noexcept PWX_API;
template<>
	inline uint64_t to_uint64( char* val ) noexcept {
		char const* v = val;
		return to_uint64( v );
	}
#endif // PWX_NODOX


#if HAVE___INT128_T
/** @brief convert a value to __int128_t
  *
  * This function can cast integer and float types to __int128_t.
  *
  * If something else is found, that is not handled by the char array or
  * std::string specializations, a pwx::CException with the name "UnsupportedType"
  * is thrown.
  *
  * @param[in] val the value to be converted
  * @return the resulting __int128_t
**/
template <typename T>
inline __int128_t to_int128 ( const T val ) {
	if ( isIntType( T ) )
		return static_cast<__int128_t>( val );
	else if ( isFloatType( T ) )
		return static_cast<__int128_t>( val + .5f );

	// We can't do anything here
	PWX_THROW( "UnsupportedType", "unsupported type for conversion to __int128_t", "" );
}
#ifndef PWX_NODOX
template<> __int128_t to_int128( char const* val ) PWX_API;
template<> __int128_t to_int128( std::string& val ) PWX_API;
template<>
inline __int128_t to_int128( char* val ) { char const* v = val; return to_int128( v ); }
#endif // PWX_NODOX
#endif // HAVE___INT128_T


#if HAVE___UINT128_T
/** @brief convert a value to __uint128_t
  *
  * This function can cast integer and float types to __uint128_t.
  *
  * If something else is found, that is not handled by the char array or
  * std::string specializations, a pwx::CException with the name "UnsupportedType"
  * is thrown.
  *
  * @param[in] val the value to be converted
  * @return the resulting __uint128_t
**/
template <typename T>
inline __uint128_t to_uint128 ( const T val ) {
	if ( isIntType( T ) )
		return static_cast<__uint128_t>( val );
	else if ( isFloatType( T ) )
		return static_cast<__uint128_t>( val + .5f );

	// We can't do anything here
	PWX_THROW( "UnsupportedType", "unsupported type for conversion to __uint128_t", "" );
}
#ifndef PWX_NODOX
template<> __uint128_t to_uint128( char const* val ) PWX_API;
template<> __uint128_t to_uint128( std::string& val ) PWX_API;
template<>
inline __uint128_t to_uint128( char* val ) { char const* v = val; return to_uint128( v ); }
#endif // PWX_NODOX
#endif // HAVE___UINT128_T

/** @brief convert a value to a string
  *
  * This function uses std::string::to_string() to convert @a val to a string.
  *
  * @param[in] val the value to be converted
  * @return the resulting int64_t
**/
template< typename T >
	inline std::string to_string( const T val ) noexcept {
		return std::to_string( val );
	}
#ifndef PWX_NODOX
template<> inline std::string to_string( char const* val ) noexcept { return std::string( val ); }
template<> inline std::string to_string( char* val ) noexcept { return std::string( val ); }
template<> inline std::string to_string( std::string const val ) noexcept { return std::string( val ); }
template<> inline std::string to_string( std::string const& val ) noexcept { return std::string( val ); }
template<> inline std::string to_string( std::string const&& val ) noexcept { return std::string( val ); }
#endif // PWX_NODOX


} // End of namespace pwx


#endif // PWX_LIBPWX_PWX_TOOLS_STREAMHELPERS_H_INCLUDED

