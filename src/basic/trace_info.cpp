/// @file


#include "pwx_trace_info.h"


#if PWX_IS_MSC
#  include <cstdlib>
#else
#  include <linux/limits.h>
#endif // MSVC or not


#include <cstdio>
#include <cstring>


/// @namespace pwx
namespace pwx {


#ifndef PWX_NODOX // this is local.
#if PWX_IS_MSC
#  define MAX_FILE_NAME_BUF_SIZE (_MAX_FNAME + _MAX_EXT)
#else
#  define MAX_FILE_NAME_BUF_SIZE (PATH_MAX)
#endif // MSVC versus gcc/clang
#endif // NODOX


char const* get_trace_info( char const* path, size_t line, char const* func ) noexcept {
	thread_local static char base_buffer[MAX_FILE_NAME_BUF_SIZE] = { 0 };

	#if PWX_IS_MSC
	thread_local static char base_filename[_MAX_FNAME] = { 0 };
	thread_local static char base_fileext [_MAX_EXT]   = { 0 };
	_splitpath_s( path, NULL, 0, NULL, 0, base_filename, _MAX_FNAME, base_fileext, _MAX_EXT );
	_snprintf_s( base_buffer, MAX_FILE_NAME_BUF_SIZE - 1, MAX_FILE_NAME_BUF_SIZE, "%s%s:%zu:%s",
	             base_filename, base_fileext, line, func );
	#else
	thread_local static char base_filename[PATH_MAX] = { 0 };
	strncpy ( base_filename, path, PATH_MAX - 1 );
	snprintf( base_buffer, PATH_MAX - 1, "%s:%zu:%s",
	          basename( base_filename ), line, func );
	#endif // MSVC or not

	return base_buffer;
}


char const* get_trace_msg ( char const* msg, char const* path, size_t line, char const* func ) noexcept {
	thread_local static char msg_buf [MAX_FILE_NAME_BUF_SIZE * 2] = { 0 };

	#if PWX_IS_MSC
	_snprintf_s( msg_buf, MAX_FILE_NAME_BUF_SIZE - 1, MAX_FILE_NAME_BUF_SIZE,
	             msg, pwx_trace_info( path, line, func ) );
	#else
	snprintf( msg_buf, PATH_MAX - 1, "%s %s", msg, get_trace_info( path, line, func ) );
	#endif // MSVC or not

	return msg_buf;
}


} // namespace pwx

