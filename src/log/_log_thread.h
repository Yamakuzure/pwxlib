#ifndef PWX_SRC_LOG_LOG_THREAD_H_INCLUDED
#define PWX_SRC_LOG_LOG_THREAD_H_INCLUDED 1
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


#include "_log_msg_id.h"
#include "_log_queue.h"
#include "basic/alloc_utils.h"
#include "basic/mem_utils.h"
#include "basic/compiler.h"
#include "log_level.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>


#ifndef PWX_NODOX


/// @namespace pwx
namespace pwx {

// Implementation is in log.cpp
void log_out_internal( log_level_t lvl, char const* msg ) noexcept;


// Tiny shortcuts...
#define ATOMIC_READ  std::memory_order_acquire
#define ATOMIC_WRITE std::memory_order_release


// Note: Here mutexes must be used, condition_variable
// can not use anything else.
extern std::mutex              log_Mutex;
extern std::condition_variable log_Condition;


// To count sleep/awake state
extern std::atomic_uint_fast32_t threads_sleeping;


// Spaces prefix for follow up lines
#define PREFIX_SPACES "                      "
//                     1234567890123456789012
//                              1         2 2


/// @brief Simple log_level_t to char const*
static char const level_str[6][9] = {
	  /* log_DEBUG    */ "*debug**",
	  /* log_INFO     */ "==info==",
	  /* log_STATUS   */ "-Status-",
	  /* LOG_WARNING  */ "Warning ",
	  /* log_ERROR    */ " ERROR  ",
	  /* log_CRITICAL */ "CRITICAL"
};


/** @brief Thread logging messages from the log queue
  *
  * Two threads are to be started. So while one writes its message, the other
  * can build theirs. Callers can hence return right after sending their
  * message to the queue.
**/
class LoggerThread {

public:

	explicit LoggerThread() noexcept
		  : doExit( false ), doStart( false ), isDone( false ), isExited( false ) { /* nothing to see here */ }

	~LoggerThread() {
		FREE_PTR( buffer );
		buffer_size = 0;
	}

	/// @brief the main thread handler.
	void operator()() {

		isDone.store( true, ATOMIC_WRITE );
		threads_sleeping++;

		// The thread is valid until someone tells it to exit
		while ( !doExit.load() ) {

			// Sleep until called
			std::unique_lock< std::mutex > log_Lock( log_Mutex );
			log_Condition.wait(
				  log_Lock, [this] {
					  return ( doStart.load( ATOMIC_READ ) || doExit.load( ATOMIC_READ ) );
				  }
			);

			// We are awake!
			threads_sleeping--;
			log_Lock.unlock();


			// Early quit if this is a call to do so:
			if ( doExit.load() ) {
				continue;
			}

			// We loop a bit to fetch all queued messages there are
			while ( log_queue_size() ) {
				// Check whether there is a message
				log_message_t* item = log_queue_pop();

				/* Get to the next message that shall be logged
				 * Note: We do a pop-push-cycle here, because of the chaos that more threads creating
				 *       messages than processing them can create. The log threads might pop messages
				 *       not due with the next message still waiting.
				 */
				while ( item && ( item->msg_id > _get_next_msg_id() ) && !doExit.load() ) {
					log_queue_push_msg( item );
					std::this_thread::yield();
					item = log_queue_pop();
				}

				// If we have one, prepare the message
				if ( item && build(
					  item->time, item->level,
					  item->location, item->intro_size,
					  item->title, item->msg_buf, nullptr
				) ) {
					// Now get it out!
					log_out_internal( item->level, msg.c_str() );
				} else {
					// yield if nothing was done
					std::this_thread::yield();
				}

				// If we got an item, it must be deleted now
				if ( item ) {
					delete item;
					// And notify that this id is done
					_inc_next_msg_id();
				}

				// If there is no new message, we do not sprint out at once.
				// it is better to poll a short while before giving up.
				if ( ( 0 == log_queue_size() ) && !doExit.load() ) {
					/* The sleeping intervals are:
					 *  50ms  ->  50ms total
					 * 100ms  -> 150ms total
					 * 150ms  -> 300ms total
					 * 200ms  -> 500ms total
					 * Half a second is more than enough to before
					 * going to sleep with a shrug...
					 */
					std::chrono::milliseconds wait_done( 0 );
					std::chrono::milliseconds wait_incr( 50 );
					std::chrono::milliseconds wait_stop( 500 );
					std::chrono::milliseconds wait_time( 0 );
					while ( ( 0 == log_queue_size() )
					        && ( wait_done < wait_stop )
					        && !doExit.load() ) {
						wait_time += wait_incr;
						std::this_thread::sleep_for( wait_time );
						wait_done += wait_time;
					}
				} // end of having no new messages
			} // End of whiling the log queue

			// All done
			isDone.store( true, ATOMIC_WRITE );
			doStart.store( false, ATOMIC_WRITE );

			// going back to sleep...
			threads_sleeping++;
		} // End of while not exiting

		isExited.store( true, ATOMIC_WRITE );
	}

	void finish() { doExit.store( true ); }
	[[nodiscard]] bool hasDone() const { return isDone.load( ATOMIC_READ ); }
	[[nodiscard]] bool hasExited() const { return isExited.load( ATOMIC_READ ); }

	bool message_deploy( char const* tme, log_level_t lvl, char const* loc,
	                     size_t is, char const* ttl, char const* fmt, va_list* ap ) noexcept {

		if ( build( tme, lvl, loc, is, ttl, fmt, ap ) ) {
			log_out_internal( lvl, msg.c_str() );
			return true;
		}
		return false;
	}

	void start() {
		isDone.store( false, ATOMIC_WRITE );
		doStart.store( true, ATOMIC_WRITE );
	}


private:
	char* buffer = nullptr;
	size_t           buffer_size = 0;
	std::atomic_bool doExit;
	std::atomic_bool doStart;
	std::atomic_bool isDone;
	std::atomic_bool isExited;
	std::string      msg;

	bool build( char const* tme, log_level_t lvl, char const* loc,
	            size_t is, char const* ttl, char const* fmt, va_list* ap ) {
		int r;

		// Now generate the intro
		r = resize_buf( is );
		if ( r ) {
			log_out_internal(
				  LOG_CRITICAL,
				  "Failed to allocate buffer for log message!"
			);
			return false;
		}
		snprintf( buffer, is, "%s|%s|%s|", tme, level_str[lvl], loc );
		msg = buffer;
		memset( buffer, 0, buffer_size );


		// Before we can add the title, the full message must be expanded. The buffer is free now.
		// Note: The queued items don't have a va_list copy, as it would be invalid by now.
		size_t text_len;
		if ( ap ) {
			// We have to make a copy, or *ap would be invalidated for the later
			// application if the buffer wasn't big enough an *ap had to be reapplied.
			va_list ap_test;
			va_copy( ap_test, *ap );
			text_len = vsnprintf( buffer, buffer_size, fmt, ap_test );
			va_end( ap_test );
		} else {
			text_len = strlen( fmt );
		}

		// Is the buffer big enough?
		if ( text_len >= buffer_size ) {
			r = resize_buf( text_len + 1 );
			if ( r ) {
				log_out_internal( LOG_CRITICAL, "Failed to allocate buffer for log message!" );
				return false;
			}
			if ( ap ) {
				// Re-apply, the buffer is big enough, now.
				vsnprintf( buffer, buffer_size, fmt, *ap );
			}
		}

		// Eventually copy fmt if this was a queued item:
		if ( nullptr == ap ) {
			memcpy( buffer, fmt, strlen( fmt ) );
		}


		// Now add the title. The line should not have more than 120 characters, but if
		// ttl is set, it is fully used and *NOT* truncated.
		bool have_title      = false;
		if ( ttl ) {
			msg += ttl;
			msg += '\n';
			have_title = true;
		}

		// The remaining of the text is easy. Just grap lines and add them to msg
		size_t end_pos;
		char   line_buf[122] = { 0x0 }; // 120 + \n + 0x0
		size_t max_len       = have_title ? 98 : 120 - is;
		char* last_char = buffer + text_len;
		char* l_p       = buffer;  // Left position of the current substring
		char* r_p;                 // Right position of the current substring

		while ( l_p <= last_char ) {
			// Each line has 22 spaces + 98 characters = 120 line width + line break
			// The title line is limited to 120 - is, which is the "intro_size"
			for ( r_p = std::min( l_p + max_len, last_char ) ; r_p && *r_p && ( r_p > l_p ) && ( ( ' ' != *r_p ) && ( '\t' != *r_p ) ) ; --r_p ) {}

			// If we have no space or tab, set r_p to the max length
			if ( l_p == r_p ) {
				r_p = l_p + max_len;
			}

			// Copy the substring to the line buffer, so we can add it to the message string
			end_pos = r_p - l_p;
			memcpy( line_buf, l_p, end_pos );

			// If this does not end in a found whitespace or null byte, append the line break
			if ( *r_p && ( ' ' != *r_p ) && ( '\t' != *r_p ) ) {
				++end_pos;
			}
			line_buf[end_pos]     = '\n';
			line_buf[end_pos + 1] = 0x0;

			// If we already have a title, add 22 spaces prefix first
			if ( have_title ) {
				msg += PREFIX_SPACES;
			}

			// Now the line can follow. l_p then has to start at the next character.
			msg += line_buf;
			l_p                   = r_p + 1;

			// If we just created a title, adapt max_len
			if ( !have_title ) {
				max_len    = 98;
				have_title = true;
			}

			// If we have less than max_len characters left, adapt
			if ( ( l_p + max_len ) > last_char ) {
				max_len = last_char - l_p;
			}
		} // End of crafting lines

		return true;
	}


	int resize_buf( size_t new_size ) {
		if ( new_size > buffer_size ) {
			/* Note: We can _NOT_ use the official pwx_alloc/realloc functions,
			 *       as they would try to log what they are doing in debugging
			 *       mode. A Backtrace into the sky would be the consequence!
			 */
			char* new_buffer;

			if ( buffer_size ) {
				new_buffer = (char*) realloc( buffer, new_size );
			} else {
				new_buffer = pwx_new0( char, new_size );
			}

			if ( nullptr == new_buffer ) {
				FREE_PTR( buffer );
				buffer_size = 0;
				return -ENOMEM;
			}
			buffer      = new_buffer;
			buffer_size = new_size;
		}
		return 0;
	}
};


} // namespace pwx


#endif // Do not document with doxygen

#endif // PWX_SRC_LOG_LOG_THREAD_H_INCLUDED

