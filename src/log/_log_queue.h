#ifndef PWXLIB_SRC_LOG_LOG_QUEUE_H_INCLUDED
#define PWXLIB_SRC_LOG_LOG_QUEUE_H_INCLUDED 1
#pragma once


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


#include "basic/alloc_utils.h"
#include "basic/mem_utils.h"
#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/string_utils.h"
#include "log_level.h"


#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <climits>
#include <exception>


#ifndef PWX_NODOX


/// @namespace pwx
namespace pwx {

/// @brief Simple struct for enqueueing and/or sending log messages
struct log_message_t {
	size_t      intro_size;
	log_level_t level;
	char* location;
	char* msg_buf;
	uint32_t msg_id;
	char* time;
	char* title;

	log_message_t* next;
	log_message_t* prev;


	explicit log_message_t(
		  char const* tme, log_level_t lvl, char const* loc,
		  size_t is, char const* ttl, char const* fmt, va_list* ap )
		  : intro_size( is )
		    , level( lvl )
		    , location( nullptr )
		    , msg_id( 0 )
		    , time( nullptr )
		    , title( nullptr )
		    , next( nullptr )
		    , prev( nullptr ) {

		// Preparation 1 : We can't use pwx_strdup, or any other pwx memory functions, now,
		// because @a loc is most probably a pointer to the static buffer of our own
		// pwx::get_trace_info() function.
		// So, first copy away @a loc by hand.
#if PWX_IS_WINDOWS
		static size_t const loc_buflen = _MAX_FNAME + _MAX_EXT;
		static thread_local char loc_buffer[loc_buflen] = { 0 };
#else
		static size_t const      loc_buflen             = PATH_MAX;
		static thread_local char loc_buffer[loc_buflen] = { 0 };
#endif // MSVC or not
		strncpy( loc_buffer, strempty( loc ), loc_buflen - 1 );

		// Preparation 2: Now that the location is "stowed away", we can eventually use pwx_strdup
		location = pwx_strdup( loc_buffer );
		time     = pwx_strdup( strempty( tme ) );
		title    = ttl ? pwx_strdup( ttl ) : nullptr;


		// First determine the buffer size needed
		// We have to make a copy, or *ap would be invalidated for the later
		// application to the allocated buffer.
		size_t  text_len;
		va_list ap_test;
		va_copy( ap_test, *ap );
		text_len = vsnprintf( msg_buf, 0, fmt, ap_test ) + 1;
		va_end( ap_test );

		// Now get the buffer
		try {
			msg_buf = new char[text_len];
			memset( msg_buf, 0, sizeof( char ) * ( text_len ) );
		} catch ( std::bad_alloc &e ) {
			fprintf(
				  stderr, "%s:%d : Unable to allocate %lu bytes for message buffer!\n",
				  __FILE__, __LINE__ - 3, (unsigned long) ( sizeof( char ) * ( text_len ) )
			);
			throw e;
		}

		// And finally store the message:
		vsnprintf( msg_buf, text_len + 1, fmt, *ap );
	}


	~log_message_t() {
		pwx_free( location );
		pwx_free( time );
		pwx_free( title );
		if ( msg_buf ) {
			delete[] msg_buf;
			msg_buf = nullptr;
		}
		next = nullptr;
		prev = nullptr;
	}
}; // log_message_t


/** @brief Log a new message without going through the multi thread queue
  *
  * @param[in] time When the message was generated
  * @param[in] level The severity of the message, @see log_level.h
  * @param[in] location Location where this message comes from
  * @param[in] title An optional title. Will be first line, not truncated, if set.
  * @param[in] fmt The format string
  * @param[in] ap The arguments to @a fmt
**/
void log_direct_out(
	  char const* time, log_level_t level, char const* location,
	  size_t instro_size, char const* title, char const* fmt, va_list* ap );


/** @brief get the last message (first added) in the internal queue
  *
  * @return The last message in the queue, which is the oldest.
**/
log_message_t* log_queue_pop();


/** @brief Add a new message to the logging queue.
  *
  * @param[in] time When the message was generated
  * @param[in] level The severity of the message, @see log_level.h
  * @param[in] location Location where this message comes from
  * @param[in] title An optional title. Will be first line, not truncated, if set.
  * @param[in] fmt The format string
  * @param[in] ap The arguments to @a fmt
**/
void log_queue_push(
	  char const* time, log_level_t level, char const* location,
	  size_t intro_size, char const* title, char const* fmt, va_list* ap );


/** @brief re-add a message to the queue
 *
 * @param[in] msg message to push
 */
void log_queue_push_msg( log_message_t* msg );


/// @return the current size of the log queue
size_t log_queue_size();


/** @brief Create the threads if needed and/or activate them
  * @param[in] do_activate Wakeup the threads if true
**/
void log_threads_activate( bool do_activate );


/// @brief End the logging threads
void log_threads_end();


} // namespace pwx


#endif // Do not document with doxygen

#endif // PWXLIB_SRC_LOG_LOG_QUEUE_H_INCLUDED

