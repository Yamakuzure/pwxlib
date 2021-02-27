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


#include "basic/CLockable.h"
#include "basic/CLockGuard.h"
#include "_log_queue.h"
#include "_log_thread.h"
#include "log_level.h"


#include <atomic>
#include <cassert>
#include <condition_variable>
#include <cstring>
#include <mutex>
#include <thread>


#ifndef PWX_NODOX


namespace pwx {


// This one is used from log.cpp, too, and must not be static
int log_thread_count                 = 0;


// The logger thread variables
static LoggerThread log_handlers[5]; // 4 threads plus one for the single threaded use
static bool         log_have_threads = ATOMIC_VAR_INIT( false );
static std::thread* log_threads[4] = { nullptr, nullptr, nullptr, nullptr };


// Note: Here mutexes must be used, condition_variable
// can not use anything else.
std::mutex              log_Mutex;
std::condition_variable log_Condition;


// Threads count their sleep/awake status themselves
aui32_t threads_sleeping = ATOMIC_VAR_INIT( 0 );

// This is the "queue" - A fully fletched class isn't needed here.
static log_message_t* q_head = nullptr;
static CLockable q_lock;
static log_message_t* q_tail = nullptr;
static aui32_t q_size   = ATOMIC_VAR_INIT( 0 );
static aui32_t q_msg_id = ATOMIC_VAR_INIT( 1 );


// Static helper functions
static bool log_threads_start();


} // namespace pwx


// Will be called by LoggerThread
pwx::log_message_t* pwx::log_queue_pop() {

	// Lock the queue for manipulation!
	CLockGuard guard( q_lock );

	log_message_t* result = q_tail;

	if ( result ) {
		q_size--;
		if ( result->prev ) {
			q_tail = result->prev;
			q_tail->next = nullptr;
			result->prev = nullptr;
		} else {
			// Otherwise tail was head
			q_head = nullptr;
			q_tail = nullptr;
			assert( q_size == 0 );
		}
	}

	return result;
}


void pwx::log_threads_end() {

	// Lock the queue for manipulation!
	q_lock.lock();

	log_Mutex.lock();
	for ( int i = 0 ; i < log_thread_count ; ++i ) {
		if ( log_threads[i] ) log_handlers[i].finish();
	}
	log_Mutex.unlock();

	log_Condition.notify_all();

	// Wait and join all threads
	bool has_thread = true;

	while ( has_thread ) {
		has_thread = false;

		for ( int i = 0 ; i < log_thread_count ; ++i ) {
			if ( log_threads[i] ) {
				if ( log_handlers[i].hasExited() ) {
					log_threads[i]->join();
					delete log_threads[i];
					log_threads[i] = nullptr;
				} else {
					has_thread = true;
				}
			}
		}
		if ( has_thread ) {
			// If any thread was just starting to work, they must not wait forever
			q_lock.unlock();
			std::this_thread::yield();
			q_lock.lock();
		}
	} // end of waiting for all threads to join

	log_thread_count = 0;

	q_lock.unlock();

	// Before we can return, we need to clear the message queue
	log_message_t* msg = log_queue_pop();
	while ( msg ) {
		log_handlers[4].message_deploy(
			  msg->time, msg->level, msg->location, msg->intro_size,
			  msg->title, msg->msg_buf, nullptr
		);
		delete msg;
		msg = log_queue_pop();
	}
}


/// @brief Start the logging threads
static bool pwx::log_threads_start() {
	try {
		for ( int i = 0 ; i < log_thread_count ; ++i ) {
			if ( !log_threads[i] ) log_threads[i] = new std::thread( std::ref( log_handlers[i] ) );
		}
	} catch ( std::bad_alloc &e ) {
		std::string err_msg = "Unable to start log thread: ";
		err_msg += e.what();
		log_out_internal( LOG_CRITICAL, err_msg.c_str() );
		return false;
	}
	return true;
}


void pwx::log_threads_activate( bool do_activate ) {

	// Note: If we end up here without at least one threads requested, something is FUBAR!
	assert( log_thread_count > 0 );

	// Lock the queue for manipulation, and the threads against firing off too early
	CLockGuard                     guard_q( q_lock );
	std::unique_lock< std::mutex > log_Lock( log_Mutex );

	// Create the logger threads if they aren't there, yet
	if ( !log_have_threads && log_thread_count ) {
		log_have_threads = log_threads_start();
	}

	// Only activate the threads if wanted and available.
	if ( !( log_have_threads && do_activate ) ) {
		log_Lock.unlock();
		return;
	}

	// If at least one thread sleeps, wake-up all
	if ( threads_sleeping ) {
		for ( int i = 0 ; i < log_thread_count ; ++i ) {
			if ( log_threads[i] ) log_handlers[i].start();
		}
		log_Lock.unlock();
		log_Condition.notify_all();
	}
}


void pwx::log_direct_out(
	  char const* time, log_level_t level, char const* location,
	  size_t intro_size, char const* title, char const* fmt, va_list* ap ) noexcept {
	log_handlers[4].message_deploy( time, level, location, intro_size, title, fmt, ap );
}


void pwx::log_queue_push (
	  char const* time, log_level_t level, char const* location,
	  size_t intro_size, char const* title, char const* fmt, va_list* ap ) noexcept {

	log_message_t* new_msg;
	CLockGuard guard( q_lock );

	try {
		new_msg = new log_message_t( time, level, location, intro_size, title, fmt, ap );
		new_msg->msg_id = q_msg_id.fetch_add( 1 ); // Get current and raise by one
	} catch ( std::bad_alloc &e ) {
		std::string bad_msg = "Can't create new log message: ";
		bad_msg += e.what();
		log_out_internal( LOG_CRITICAL, bad_msg.c_str() );
		return;
	}

	log_queue_push_msg( new_msg );
}


void pwx::log_queue_push_msg( log_message_t* msg ) {
	CLockGuard guard( q_lock );

	// Simple for a queue: Just replace head
	if ( q_head ) {
		q_head->prev = msg;
		msg->next    = q_head;
	}

	q_head = msg;
	q_size++;

	// And check whether there needs to be a new tail:
	if ( nullptr == q_tail ) {
		q_tail = msg;
	}

	// Let the logger threads do their work
	log_threads_activate( true );
}


/// @return the current size of the log queue
size_t pwx::log_queue_size() {
	return q_size.load();
}


#endif // Do not document with doxygen
