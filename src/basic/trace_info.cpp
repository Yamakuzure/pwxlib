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


#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>


#include "basic/compiler.h"
#include "basic/macros.h"
#include "basic/trace_info.h"


#if PWX_IS_LINUX
#  include <climits>
#endif // Only for Linux, MSVC has it in cstdlib


/// @namespace pwx
namespace pwx {


#ifndef PWX_NODOX // this is local.
#if PWX_IS_MSVC
#  define MAX_FILE_NAME_BUF_SIZE (_MAX_FNAME + _MAX_EXT)
#else
#  define MAX_FILE_NAME_BUF_SIZE (PATH_MAX)
#endif // MSVC versus gcc/clang
#endif // NODOX


char const* get_trace_info( char const* path, size_t line, char const* func ) noexcept {
#if PWX_IS_MSVC
	static char base_buffer  [_MAX_FNAME + _MAX_EXT] = { 0 };
	static char base_filename[_MAX_FNAME]            = { 0 };
	static char base_fileext [_MAX_EXT]              = { 0 };
	_splitpath_s( path, NULL, 0, NULL, 0, base_filename, _MAX_FNAME, base_fileext, _MAX_EXT );
	_snprintf_s( base_buffer, _MAX_FNAME + _MAX_EXT - 1, _MAX_FNAME + _MAX_EXT, "%s%s:%lu:%s",
				 base_filename, base_fileext, ( unsigned long )line, func );
#else
	static char base_buffer[PATH_MAX]   = { 0 };
	static char base_filename[PATH_MAX] = { 0 };
	strncpy( base_filename, path, PATH_MAX - 1 );
	snprintf(
		  base_buffer, PATH_MAX - 1, "%s:%lu:%s",
		  basename( base_filename ), line, func
	);
#endif // MSVC or not

	return base_buffer;
}


char const* get_trace_msg( char const* msg, char const* path, size_t line, char const* func ) noexcept {
	thread_local static char msg_buf[MAX_FILE_NAME_BUF_SIZE * 2] = { 0 };

#if PWX_IS_MSVC
	_snprintf_s( msg_buf, MAX_FILE_NAME_BUF_SIZE - 1, MAX_FILE_NAME_BUF_SIZE,
				 msg, pwx_trace_info( path, line, func ) );
#else
	snprintf( msg_buf, PATH_MAX - 1, "%s %s", msg, get_trace_info( path, line, func ) );
#endif // MSVC or not

	return msg_buf;
}


} // namespace pwx

