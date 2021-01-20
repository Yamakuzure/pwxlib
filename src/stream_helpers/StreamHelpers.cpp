/**
  * This file is part of the PrydeWorX Library (pwxLib).
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


#include <charconv>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <vector>

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "basic/mem_utils.h"
#include "basic/string_utils.h"
#include "stream_helpers/StreamHelpers.h"


namespace pwx {


/** @brief operator<< for moving an CAdjLeft object upon a stream
  *
  * @param[in] os an open and valid output stream to format
  * @param[in] l an instance of CAdjLeft
  * @return the stream object
  *
**/
std::ostream& operator<<( std::ostream& os, const CAdjLeft& l ) noexcept {
	if ( os.good() ) {
		os.setf( std::ios_base::left, std::ios_base::adjustfield | std::ios_base::floatfield );
		l.setFields( os );
	}
	return ( os );
}


/** @brief operator<< for moving an CAdjRight object upon a stream
  *
  * @param[in] os an open and valid output stream to format
  * @param[in] r an instance of CAdjRight
  * @return the stream object
  *
**/
std::ostream& operator<<( std::ostream& os, const CAdjRight& r ) noexcept {
	if ( os.good() ) {
		os.setf( std::ios_base::right, std::ios_base::adjustfield | std::ios_base::floatfield );
		r.setFields( os );
	}
	return ( os );
}


/** @brief Delete shell representations out of a string
  *
  * This function is internally used to handle shell script like
  * configuration files.
  *
  * @param[in] key The key to search for
  * @param[in,out] data The data string to search
  * @return true if a representation was found.
**/
bool cropShell( char const* key, std::string& data ) noexcept {
	std::string shLike = "$";
	shLike += key;
	size_t pos    = data.find( shLike );
	bool   hasRep = ( pos != data.npos ) ? true : false;

	// If we have not found one, this might say nothing if the keyword is encased in curly brackets:
	if ( !hasRep ) {
		shLike.insert( 1, "{" );
		shLike += "}";
		pos = data.find( shLike );
		if ( pos != data.npos ) {
			hasRep = true;
		}
	}

	// If we have found one, it has to be erased from the given string
	if ( hasRep ) {
		while ( pos != data.npos ) {
			data.erase( pos, shLike.size() );
			pos = data.find( shLike );
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
void forwardTo( std::ifstream& is, char value ) noexcept {
	while ( is.good() && ( is.peek() != value ) ) {
		is.ignore( 1 );
	}
	if ( is.good() && is.peek() == value ) {
		is.ignore( 1 );
	}
}


/** @brief delete all newline, carriage return, tabs and spaces at the beginning of a string
  * @param[in,out] text a string reference that will be trimmed
  * @param[in] extra optional extra character to be trimmed as well, defaults to 0
**/
void ltrim( std::string& text, char extra ) noexcept {
	if ( ( extra == '\r' ) || ( extra == '\n' ) || ( extra == ' ' ) || ( extra == '\t' ) ) {
		extra = 0x0;
	}
	while ( text.size()
	        && ( ( text[0] == '\r' ) || ( text[0] == '\n' )
	             || ( text[0] == ' ' ) || ( text[0] == '\t' )
	             || ( extra && text[0] == extra )
	        ) ) {
		text.erase( 0, 1 );
	}
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
char const* makeTemp( char const* aPath, char const* aTemplate, char const* aSuffix,
                      std::ofstream& ofs, std::ios_base::openmode mode ) noexcept {
	try {
		std::string fileName( aPath );
		int32_t     suffLen = aSuffix ? strlen( aSuffix ) : 0;

		// If a template is given, use it
		if ( aTemplate && strlen( aTemplate ) ) {
			if ( fileName.size() && fileName[fileName.size() - 1] != '\\' ) {
				fileName += "/";
			}
			fileName += aTemplate;
			// We need to ensure that the last six characters are capital X:
			while ( STRNE ( fileName.substr( fileName.size() > 6 ? fileName.size() - 6 : 0 ).c_str(), "XXXXXX" ) ) {
				fileName += "X";
			}
		} else {
			// Otherwise we have our default here:
			fileName += "/pwxTemp_XXXXXX";
		}

		// Add the suffix if we have one:
		if ( suffLen ) {
			if ( aSuffix[0] != '.' ) {
				fileName += ".";
				++suffLen;
			}
			fileName += aSuffix;
		}

		// Unfortunately mkstemp() can't use a const_char, so c_str() isn't our method
		std::vector< char > destination( fileName.begin(), fileName.end() );
		destination.push_back( '\0' );

		int32_t fd = -1;

		if ( suffLen ) {
			fd = mkstemps( &destination[0], suffLen );
		} else {
			fd = mkstemp( &destination[0] );
		}

		if ( fd != -1 ) {
			fileName.assign( destination.begin(), destination.end() - 1 );

			ofs.open( fileName.c_str(), mode );
			close( fd );
			return ( pwx_strdup ( fileName.c_str() ) );
		}
	}
	PWX_CATCH_AND_FORGET ( std::exception )
	// This simple catch'all should ensure that nothing leaves this.
	return ( NULL );
}


/** @brief delete all newline, carriage return, tabs and spaces at the end of a string
  * @param[in,out] text a string reference that will be trimmed
  * @param[in] extra optional extra character to be trimmed as well, defaults to 0
**/
void rtrim( std::string& text, char extra ) noexcept {
	if ( ( extra == '\r' ) || ( extra == '\n' ) || ( extra == ' ' ) || ( extra == '\t' ) ) {
		extra = 0x0;
	}
	size_t end = text.size() ? text.size() - 1 : 0;

	while ( text.size()
	        && ( ( text[end] == '\r' ) || ( text[end] == '\n' )
	             || ( text[end] == ' ' ) || ( text[end] == '\t' )
	             || ( extra && text[end] == extra )
	        ) ) {
		text.erase( end--, 1 );
	}
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
bool skipLineBreak( std::ifstream& is ) noexcept {
	bool    result = false;
	int32_t peek   = 0;

	if ( is.good() ) {
		peek = is.peek();
		while ( is.good() && ( ( peek == 0x0a ) || ( peek == 0x0d ) ) ) {
			is.ignore( 1 );
			peek = is.peek();
		}
	}
	// If >>is<< is still good (or eof), we have finished:
	if ( is.good() || ( 0 == ( is.rdstate() ^ std::ios_base::eofbit ) ) ) {
		// Note: we use XOR, because this leaves all other failbits untouched,
		// resulting in !0 if something else is set as well
		result = true;
	}

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
void tabToSpace( std::string& text, size_t spacePerTab ) noexcept {
	if ( text.size() ) {
		size_t pos = text.find_first_of( '\t' );

		while ( pos != text.npos ) {
			text.erase( pos, 1 );
			if ( spacePerTab ) {
				text.insert( pos, spacePerTab, ' ' );
			}
			// Now search next tab
			pos = text.find_first_of( '\t', pos );
		}
	} // End of having text
}


/** @brief delete all newline, carriage return, tabs and spaces at the beginning and end of a string
  * @param[in,out] text a string reference that will be trimmed
  * @param[in] extra optional extra character to be trimmed as well, defaults to 0
**/
void trim( std::string& text, char extra ) noexcept {
	ltrim( text, extra );
	rtrim( text, extra );
}


/** @brief default constructor
  * @param[in] left_ set left number of digits to this value
  * @param[in] right_ set right number of digits to this value
  */
CFormat::CFormat( int32_t left_, int32_t right_ ) noexcept:
	  left( left_ ), right( right_ ) {}


/** @brief empty constructor
  *
  * Default values for left and right are both zero.
  */
CFormat::CFormat() noexcept {}


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
void CFormat::setFields( std::ostream& os ) const noexcept {
	if ( os.good() && ( left || right ) ) {
		if ( left ) {
			os.width( left + ( right ? 1 + right : 0 ) );
		}
		if ( right ) {
			os.setf( std::ios_base::fixed );
			os.precision( right );
		}
	}
}


/** @brief default constructor
  * @param[in] left_ set left number of digits to this value
  * @param[in] right_ set right number of digits to this value
  */
CAdjLeft::CAdjLeft( int32_t left_, int32_t right_ ) noexcept:
	  CFormat( left_, right_ ) {}


/** @brief empty constructor
  *
  * Default values for left and right are both zero.
  */
CAdjLeft::CAdjLeft() noexcept {}


/** @brief default constructor
  * @param[in] left_ set left number of digits to this value
  * @param[in] right_ set right number of digits to this value
  */
CAdjRight::CAdjRight( int32_t left_, int32_t right_ ) noexcept:
	  CFormat( left_, right_ ) {}


/** @brief empty constructor
  *
  * Default values for left and right are both zero.
  */
CAdjRight::CAdjRight() noexcept {}

// === Static helpers for the conversion from std::string ===
template< typename T >
	static inline T _get_int_from_String( std::string& str ) {
		auto const tail = str.find_last_not_of( ' ' );
		if ( tail == std::string::npos ) {
			return (T) 0;
		}

		auto const st_end = str.data() + tail + 1; // End of data to be converted
		T          num;

		return ( std::from_chars( str.data() + str.find_first_not_of( ' ' ), st_end, num, 10 ).ptr == st_end )
		       ? num : (T) 0;
	}

#ifdef HAVE_STD__CHARS_FORMAT__GENERAL
template<typename T>
static inline T _get_flt_from_String( std::string &str )
{
	auto const tail = str.find_last_not_of(' ');
	if ( tail == std::string::npos )
		return (T)0;

	auto const st_end = str.data() + tail + 1; // End of data to be converted
	T num;

	return ( std::from_chars( str.data() + str.find_first_not_of(' '), st_end, num, std::chars_format::general).ptr == st_end )
		   ? num : (T)0;
}
#endif // HAVE_STD__CHARS_FORMAT__GENERAL

// === Specializations of the to_*() functions ===

#ifndef PWX_NODOX
template<> bool to_bool( char const* val ) noexcept {
	return ( val
	         && val[0]
	         && ( STRNCEQ( val, "true", 4 )
	              || STRNCEQ( val, "yes", 3 )
	              || STRNNE( val, "0", 1 )
	         )
	);
}

template<> bool to_bool( std::string& val ) noexcept {
	return to_bool( val.c_str() );
}

template<> float to_float( char const* val ) noexcept {
	if ( val && val[0] ) {
		return strtof( val, nullptr );
	}
	return 0.f;
}

template<> float to_float( std::string& val ) noexcept {
#ifdef HAVE_STD__CHARS_FORMAT__GENERAL
	return _get_flt_from_String<float>( val );
#else
	return to_float( val.c_str() );
#endif // HAVE_STD__CHARS_FORMAT__GENERAL
}


template<> double to_double( char const* val ) noexcept {
	if ( val && val[0] ) {
		return strtod( val, nullptr );
	}
	return 0.;
}

template<> double to_double( std::string& val ) noexcept {
#ifdef HAVE_STD__CHARS_FORMAT__GENERAL
	return _get_flt_from_String<double>( val );
#else
	return to_double( val.c_str() );
#endif // HAVE_STD__CHARS_FORMAT__GENERAL
}

template<> long double to_long_double( char const* val ) noexcept {
	if ( val && val[0] ) {
		return strtold( val, nullptr );
	}
	return 0.;
}

template<> long double to_long_double( std::string& val ) noexcept {
#ifdef HAVE_STD__CHARS_FORMAT__GENERAL
	return _get_flt_from_String<long double>( val );
#else
	return to_long_double( val.c_str() );
#endif // HAVE_STD__CHARS_FORMAT__GENERAL
}

template<> int8_t to_int8( char const* val ) noexcept {
	if ( val && val[0] ) {
		return static_cast<int8_t>( 0xff & std::strtol( val, nullptr, 10 ) );
	}
	return 0;
}

template<> int8_t to_int8( std::string& val ) noexcept {
	return _get_int_from_String< int8_t >( val );
}

template<> uint8_t to_uint8( char const* val ) noexcept {
	if ( val && val[0] ) {
		return static_cast<uint8_t>( 0xff & std::strtoul( val, nullptr, 10 ) );
	}
	return 0;
}


template<> uint8_t to_uint8( std::string& val ) noexcept {
	return _get_int_from_String< uint8_t >( val );
}

template<> int16_t to_int16( char const* val ) noexcept {
	if ( val && val[0] ) {
		return static_cast<int16_t>( 0xffff & std::strtol( val, nullptr, 10 ) );
	}
	return 0;
}

template<> int16_t to_int16( std::string& val ) noexcept {
	return _get_int_from_String< int16_t >( val );
}

template<> uint16_t to_uint16( char const* val ) noexcept {
	if ( val && val[0] ) {
		return static_cast<uint16_t>( 0xffff & std::strtoul( val, nullptr, 10 ) );
	}
	return 0;
}

template<> uint16_t to_uint16( std::string& val ) noexcept {
	return _get_int_from_String< uint16_t >( val );
}

template<> int32_t to_int32( char const* val ) noexcept {
	if ( val && val[0] ) {
		return static_cast<int32_t>( 0xffffffff & std::strtol( val, nullptr, 10 ) );
	}
	return 0;
}

template<> int32_t to_int32( std::string& val ) noexcept {
	return _get_int_from_String< int32_t >( val );
}

template<> uint32_t to_uint32( char const* val ) noexcept {
	if ( val && val[0] ) {
		return static_cast<uint32_t>( 0xffffffff & std::strtoul( val, nullptr, 10 ) );
	}
	return 0;
}

template<> uint32_t to_uint32( std::string& val ) noexcept {
	return _get_int_from_String< uint32_t >( val );
}

template<> int64_t to_int64( char const* val ) noexcept {
	if ( val && val[0] ) {
		return static_cast<int64_t>( std::strtol( val, nullptr, 10 ) );
	}
	return 0;
}

template<> int64_t to_int64( std::string& val ) noexcept {
	return _get_int_from_String< int64_t >( val );
}

template<> uint64_t to_uint64( char const* val ) noexcept {
	if ( val && val[0] ) {
		return static_cast<uint64_t>( std::strtoul( val, nullptr, 10 ) );
	}
	return 0;
}

template<> uint64_t to_uint64( std::string& val ) noexcept {
	return _get_int_from_String< uint64_t >( val );
}

#ifdef HAVE___INT128_T
template<> __int128_t to_int128( char const* val )  {
	if ( val && val[0] )
		return static_cast<__int128_t>( std::strtoll(val, nullptr, 10) );
	return 0;
}

template<> __int128_t to_int128( std::string &val )  {
	return _get_int_from_String<__int128_t>( val );
}

#endif // HAVE___INT128_T
#ifdef HAVE___UINT128_T
template<> __uint128_t to_uint128( char const* val )  {
	if ( val && val[0] )
		return static_cast<__uint128_t>( std::strtoull(val, nullptr, 10) );
	return 0;
}

template<> __uint128_t to_uint128( std::string &val )  {
	return _get_int_from_String<__uint128_t>( val );
}

#endif // HAVE___UINT128_T

#endif // PWX_NODOX

} // namespace pwx
