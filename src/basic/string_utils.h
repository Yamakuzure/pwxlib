#ifndef PWX_PWXLIB_SRC_BASIC_STRING_UTILS_H_INCLUDED
#define PWX_PWXLIB_SRC_BASIC_STRING_UTILS_H_INCLUDED 1
#pragma once


/** @file string_utils.h
  *
  * @brief Utilities for working with C-style strings
  *
  * (c) 2007 - 2020 PrydeWorX
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


#include "pwx_compiler.h"
#include "pwx_macros.h"

#include <cassert>
#include <cstring>
#include <strings.h>


/** @brief return `true` if two C-Strings are equal ignoring case
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `false`.
  *
  * @param a  Left hand C-String
  * @param b  Right hand C-String
  * @return `true` if both C-Strings are equal ignoring case
**/
#define STRCEQ(a,b) (a && b && (strcasecmp(a,b) == 0))


/** @brief return `true` if two C-Strings are equal ignoring case with length limit
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `false`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @param[in] n  Number of characters to compare
  * @return `true` if both C-Strings are equal ignoring case
**/
#define STRNCEQ(a,b,n) (a && b && (strncasecmp(a,b,n) == 0))


/** @brief return `true` if two C-Strings are not equal ignoring case
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `true`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @return `true` if both C-Strings are not equal ignoring case
**/
#define STRCNE(a,b) ((nullptr == (a)) || (nullptr == (b)) || (strcasecmp(a,b) != 0))


/** @brief return `true` if two C-Strings are not equal ignoring case with length limit
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `true`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @param[in] n  Number of characters to compare
  * @return `true` if both C-Strings are not equal ignoring case
**/
#define STRNCNE(a,b,n) ((nullptr == (a)) || (nullptr == (b)) || (strncasecmp(a,b,n) != 0))


/** @brief `true` if @a a is "lower" than @a b ignoring case
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `true` if @a a is `nullptr` or `false` if @a b is `nullptr`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @return `true` if @a a is "lower" than @a b ignoring case
**/
#define STRCLT(a,b) ((nullptr == (a)) ? true : (nullptr == (b)) ? false : (strcasecmp(a,b) < 0))


/** @brief `true` if @a a is "lower" than @a b ignoring case with length limit
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `true` if @a a is `nullptr` or `false` if @a b is `nullptr`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @param[in] n  Number of characters to compare
  * @return `true` if @a a is "lower" than @a b ignoring case
**/
#define STRNCLT(a,b,n) ((nullptr == (a)) ? true : (nullptr == (b)) ? false : (strncasecmp(a,b,n) < 0))


/** @brief `true` if @a a is "greater" than @a b ignoring case
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `false` if @a a is `nullptr` or `true` if @a b is `nullptr`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @return `true` if @a a is "greater" than @a b ignoring case
**/
#define STRCGT(a,b) ((nullptr == (a)) ? false : (nullptr == (b)) ? true : (strcasecmp(a,b) > 0))


/** @brief `true` if @a a is "greater" than @a b ignoring case with length limit
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `false` if @a a is `nullptr` or `true` if @a b is `nullptr`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @param[in] n  Number of characters to compare
  * @return `true` if @a a is "greater" than @a b ignoring case
**/
#define STRNCGT(a,b,n) ((nullptr == (a)) ? false : (nullptr == (b)) ? true : (strncasecmp(a,b,n) > 0))


/** @brief return `true` if two C-Strings are equal
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `false`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @return `true` if both C-Strings are equal
**/
#define STREQ(a,b) ((a) && (b) && (std::strcmp(a,b) == 0))


/** @brief return `true` if two C-Strings are equal with length limit
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `false`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @param[in] n  Number of characters to compare
  * @return `true` if both C-Strings are equal
**/
#define STRNEQ(a,b,n) ((a) && (b) && (std::strncmp(a,b,n) == 0))


/** @brief return `true` if two C-Strings are not equal
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `true`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @return `true` if both C-Strings are not equal
**/
#define STRNE(a,b) ((nullptr == (a)) || (nullptr == (b)) || (std::strcmp(a,b) != 0))


/** @brief return `true` if two C-Strings are not equal with length limit
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `true`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @param[in] n  Number of characters to compare
  * @return `true` if both C-Strings are not equal
**/
#define STRNNE(a,b,n) ((nullptr == (a)) || (nullptr == (b)) || (std::strncmp(a,b,n) != 0))


/** @brief `true` if @a a is "lower" than @a b
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `true` if @a a is `nullptr` or `false` if @a b is `nullptr`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @return `true` if @a a is "lower" than @a b
**/
#define STRLT(a,b) ((nullptr == (a)) ? true : (nullptr == (b)) ? false : (std::strcmp(a,b) < 0))


/** @brief `true` if @a a is "lower" than @a b with length limit
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `true` if @a a is `nullptr` or `false` if @a b is `nullptr`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @param[in] n  Number of characters to compare
  * @return `true` if @a a is "lower" than @a b
**/
#define STRNLT(a,b,n) ((nullptr == (a)) ? true : (nullptr == (b)) ? false : (std::strncmp(a,b,n) < 0))


/** @brief `true` if @a a is "greater" than @a b
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `false` if @a a is `nullptr` or `true` if @a b is `nullptr`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @return `true` if @a a is "greater" than @a b
**/
#define STRGT(a,b) ((nullptr == (a)) ? false : (nullptr == (b)) ? true : (std::strcmp(a,b) > 0))


/** @brief `true` if @a a is "greater" than @a b with length limit
  *
  * Note: `nullptr` @a a and/or `nullptr` @a b are handled and cause
  * the result being `false` if @a a is `nullptr` or `true` if @a b is `nullptr`.
  *
  * @param[in] a  Left hand C-String
  * @param[in] b  Right hand C-String
  * @param[in] n  Number of characters to compare
  * @return `true` if @a a is "greater" than @a b
**/
#define STRNGT(a,b,n) ((nullptr == (a)) ? false : (nullptr == (b)) ? true : (std::strncmp(a,b,n) > 0))


/// @namespace pwx
namespace pwx {


/// @brief Return @a s or an empty string if @a s is `nullptr`
static inline char const* strempty( char const* s ) {
	return s ? s : "";
}


/// @brief Return @a s or "(null)" if @a s is `nullptr`
static inline char const* strnull( char const* s ) {
	return s ? s : "(null)";
}


/// @brief Return @a s or "n/a" if @a s is `nullptr`
static inline char const* strna( char const* s ) {
	return s ? s : "n/a";
}


/// @brief Return `true` if @a p is `nullptr` or empty
static inline bool isempty( char const* p ) {
	return !p || !p[0];
}


/** @brief Return the remainder of a string @a s if it starts with @a prefix
  * @param[in] s  The string to investigate
  * @param[in] prefix  The string @a s has to start with
  * @return A pointer to the string behind @a prefix, or nullptr if @a s does not start with @a prefix.
**/
static inline char* startswith( char const* s, char const* prefix ) {
	size_t l;

	l = strlen( prefix );
	if ( STRNEQ( s, prefix, l ) )
		return ( char* ) s + l;

	return nullptr;
}


/** @brief Return the remainder of a string @a s if it starts with @a prefix ignoring case
  * @param[in] s  The string to investigate
  * @param[in] prefix  The string @a s has to start with, the case is ignored
  * @return A pointer to the string behind @a prefix, or nullptr if @a s does not start with @a prefix.
**/
static inline char* startswith_no_case( char const* s, char const* prefix ) {
	size_t l;

	l = strlen( prefix );
	if ( STRNCEQ( s, prefix, l ) )
		return ( char* ) s + l;

	return nullptr;
}


/** @brief Return the @a postfix of a string @a s if it ends with @a postfix
  * @param[in] s  The string to investigate
  * @param[in] postfix  The string @a s has to end with
  * @return A pointer to part in @a s where @a postfix starts, or nullptr if @a s does not end with @a postfix.
**/
static inline char* endswith( char const* s, char const* postfix ) {
	size_t sl, pl;

	assert( s );
	assert( postfix );

	sl = strlen( s );
	pl = strlen( postfix );

	if ( pl == 0 )
		return ( char* ) s + sl;

	if ( sl < pl )
		return nullptr;

	if ( memcmp( s + sl - pl, postfix, pl ) != 0 )
		return nullptr;

	return ( char* ) s + sl - pl;
}


/** @brief Return the @a postfix of a string @a s if it ends with @a postfix ignoring case
  * @param[in] s  The string to investigate
  * @param[in] postfix  The string @a s has to end with ignoring case
  * @return A pointer to part in @a s where @a postfix starts, or nullptr if @a s does not end with @a postfix.
**/
static inline char* endswith_no_case( char const* s, char const* postfix ) {
	size_t sl, pl;

	assert( s );
	assert( postfix );

	sl = strlen( s );
	pl = strlen( postfix );

	if ( pl == 0 )
		return ( char* ) s + sl;

	if ( sl < pl )
		return nullptr;

	if ( STRCEQ( s + sl - pl, postfix ) )
		return nullptr;

	return ( char* ) s + sl - pl;
}


/// @brief Return a NULL terminated char array with the binary representation of @a b
#define byte_to_binary(b) ({                                       \
		char   _d_[9] = { 0x0 };                           \
		for (size_t _i_ = 0; _i_ < 8; ++_i_)               \
			_d_[7-_i_] = ((b >> _i_) & 1) ? '1' : '0'; \
		_d_;                                               \
	})                                                         \


} // namespace pwx


#endif // PWX_PWXLIB_SRC_BASIC_STRING_UTILS_H_INCLUDED

