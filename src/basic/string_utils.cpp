/** @file
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


#include "basic/alloc_utils.h"
#include "basic/mem_utils.h"
#include "basic/debug.h"
#include "basic/string_utils.h"
#include "log/log.h"


#if PWX_IS_WINDOWS
  #include <cstdlib>
#else
  #include <libgen.h>
  #include <climits>
#endif // MSVC or not


#include <cstdarg>
#include <cstdio>
#include <cstring>


int pwx::pwx_asprintf( char** strp, const char* fmt, ... ) noexcept {
	va_list ap;
	va_list ap_test;
	char* msg_buf = nullptr;
	size_t text_len;

	// First of all get the full message size
	va_start( ap, fmt );
	va_copy( ap_test, ap );
	text_len = vsnprintf( msg_buf, 0, fmt, ap_test ) + 1;
	va_end( ap_test );

	// Now generate the buffer
	if ( text_len > 0 ) {
		msg_buf = pwx_calloc( char, text_len );

		if ( nullptr == msg_buf ) {
			log_critical( "out of memory", "Unable to allocate %zu bytes for asprintf buffer!", sizeof( char ) * text_len );
			return -1;
		}

		text_len = vsnprintf( msg_buf, text_len + 1, fmt, ap );
	}

	if ( msg_buf && strp ) {
		*strp = TAKE_PTR( msg_buf );
	}

	va_end( ap );

	return text_len > 0 ? 0 : -1;
}


char const* pwx::pwx_basename( char const* full_path ) noexcept {
#if PWX_IS_WINDOWS
	static thread_local char base_buffer  [_MAX_FNAME + _MAX_EXT] = { 0 };
	static thread_local char base_filename[_MAX_FNAME]            = { 0 };
	static thread_local char base_fileext [_MAX_EXT]              = { 0 };
	_splitpath_s( full_path, NULL, 0, NULL, 0, base_filename, _MAX_FNAME, base_fileext, _MAX_EXT );
	_snprintf_s( base_buffer, _MAX_FNAME + _MAX_EXT - 1, _MAX_FNAME + _MAX_EXT, "%s%s",
				 base_filename, base_fileext );
#else
	static thread_local char base_buffer[PATH_MAX]   = { 0 };
	static thread_local char base_filename[PATH_MAX] = { 0 };
	strncpy( base_filename, full_path, PATH_MAX - 1 );
	snprintf( base_buffer, PATH_MAX - 1, "%s", basename( base_filename ) );
#endif // MSVC or not
	return base_buffer;
}


char const* pwx::pwx_dirname( char const* full_path ) noexcept {
#if PWX_IS_WINDOWS
	static thread_local char base_buffer[_MAX_FNAME + _MAX_EXT] = { 0 };
	static thread_local char base_dir   [_MAX_FNAME]            = { 0 };
	static thread_local char base_drive [_MAX_EXT]              = { 0 };
	_splitpath_s( full_path, base_drive, _MAX_EXT, base_dir, _MAX_FNAME, NULL, 0, NULL, 0 );
	_snprintf_s( base_buffer, _MAX_FNAME + _MAX_EXT - 1, _MAX_FNAME + _MAX_EXT, "%s/%s",
				 base_drive, base_dir );
#else
	static thread_local char base_buffer[PATH_MAX]   = { 0 };
	static thread_local char base_filename[PATH_MAX] = { 0 };
	strncpy( base_filename, full_path, PATH_MAX - 1 );
	snprintf( base_buffer, PATH_MAX - 1, "%s", dirname( base_filename ) );
#endif // MSVC or not
	log_debug( nullptr, "%s -> %s", full_path, base_buffer );
	return base_buffer;
}


char const* pwx::pwx_strerror( int errno_ ) noexcept {
	static thread_local char errmsg[256] = { 0x0 };

#if defined(_GNU_SOURCE)
	return strerror_r( errno_, errmsg, 255 );
#else
	if ( strerror_r( errno_, errmsg, 255 ) )
		return errmsg;
	return nullptr;
#endif // _GNU_SOURCE versus XSI conformity
}
