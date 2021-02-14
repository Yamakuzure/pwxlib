/// @file log.cpp

#include "basic/CLockable.h"
#include "basic/CLockGuard.h"
#include "basic/pwx_compiler.h"
#include "basic/pwx_debug.h"
#include "basic/string_utils.h"
#include "log.h"
#include "log_level.h"
#include "_log_queue.h"
#include "_log_thread.h"

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <thread>
#include <ctime>


#ifndef PWX_NODOX

namespace pwx {

/***************************************************
*** Global values used by the log implementation ***
***************************************************/

extern int log_thread_count; // From _log_queue.cpp

static abool_t     have_progress_msg = ATOMIC_VAR_INIT( false );
static CLockable   input_lock;
static CLockable   logfile_lock;
static std::string logfile_name;
static FILE* logfile_p             = nullptr;
static CLockable output_lock;
static int32_t   progress_len      = -1;
static char      progress_msg[129] = { 0x0 };
static int32_t   verbose_log       = 2;
static int32_t   verbose_out       = 4;


/************************************************
*** Static internal functions prototypes     ***
************************************************/

static void log_close_internal( bool with_lock );
static int log_open_internal( char const* file_name, char const* mode );
static void remove_progress_msg_internal();

} // namespace pwx

#endif // Do not document with doxygen


/***************************************
*** Public functions implementations ***
***************************************/

void pwx::log( char const* location, log_level_t level, char const* title, char const* message, ... ) {

	CLockGuard input_guard( input_lock ); // Make sure log messages really come in the order they are issued.

	// Don't do anything if the verbosity settings cut this out of everything
	if ( ( verbose_log > level ) && ( verbose_out > level ) ) {
		return;
	}

	// First, get the date and time string
	thread_local
	static char timebuf[20] = { 0x0 }; // YYYY-mm-dd HH:MM:SS + 0x0
	time_t      t           = time( nullptr );
	struct tm   tm;
#if PWX_IS_MSVC
	if ( 0 == localtime_s( &tm, &t ) ) {
#else
	if ( localtime_r( &t, &tm ) ) {
#endif // Thanks MS for having their own ideas. Not even compliant to C11 localtime_s()...
		snprintf(
			  timebuf, 20, "%04hu-%02hhu-%02hhu %02hhu:%02hhu:%02hhu",
			  (uint16_t) ( 1900 + (uint8_t) tm.tm_year ),
			  (uint8_t) ( tm.tm_mon + 1 ),
			  (uint8_t) tm.tm_mday,
			  (uint8_t) tm.tm_hour,
			  (uint8_t) tm.tm_min,
			  (uint8_t) tm.tm_sec
		);
	}

	// If no location is provided, offer a default
	char const* real_loc = location ? location : "<unknown>";
	// Intro Size: date (19) + level (9) + 3 pipes + 0x0 = 32
	size_t intro_size = 32 + strlen( real_loc );

	// Now that we have the point in time, delegate to the log message builder
	va_list ap;
	va_start ( ap, message );
	if ( log_thread_count ) {
		log_queue_push( timebuf, level, real_loc, intro_size, title, message, &ap );
	} else {
		log_direct_out( timebuf, level, real_loc, intro_size, title, message, &ap );
	}
	va_end( ap );
}


void pwx::log_enable_threads( int thread_count ) {
	if ( ( 0 == log_thread_count ) && ( thread_count > 0 ) ) {
		log_thread_count = thread_count > 4 ? 4 : thread_count;
		log_threads_activate( false );
	} else if ( log_thread_count && ( 1 > thread_count ) ) {
		log_threads_end();
	} // Also sets log_thread_count to 0
}


void pwx::log_close() {
	log_close_internal( true );
}


char const* pwx::log_file_name() {
	if ( logfile_p ) {
		return logfile_name.c_str();
	}
	return nullptr;
}


int pwx::log_open( char const* file_name, char const* mode ) {
	return log_open_internal( file_name, mode );
}


void pwx::log_set_verbosity( int32_t level_logfile, int32_t level_console ) {
	CLockGuard guard( output_lock, logfile_lock );

	verbose_log = level_logfile < LOG_DEBUG ? LOG_DEBUG :
	              level_logfile > LOG_DISABLED ? LOG_DISABLED : level_logfile;
	verbose_out = level_console < LOG_DEBUG ? LOG_DEBUG :
	              level_console > LOG_DISABLED ? LOG_DISABLED : level_console;
}


void pwx::show_progress( char const* message, ... ) {
	CLockGuard guard( output_lock, logfile_lock );

	remove_progress_msg_internal();

	va_list ap;
	va_start ( ap, message );
	vsnprintf( progress_msg, 128, message, ap );
	va_end( ap );

	progress_len = strlen( progress_msg );
	fprintf( stdout, "\r%s", progress_msg );
	have_progress_msg.store( true );
	fflush( stdout );
}


/*****************************************
*** Internal functions implementations ***
*****************************************/

#ifndef PWX_NODOX

void pwx::log_out_internal( log_level_t lvl, char const* msg ) {
	CLockGuard guard( output_lock, logfile_lock );

	// Write into log file if set and covered by verbosity
	if ( logfile_p && ( verbose_log <= lvl ) ) {
		fprintf( logfile_p, "%s", msg );
		fflush( logfile_p );
	}

	// Write to console if covered by verbosity
	if ( verbose_out <= lvl ) {
		FILE* target = lvl > LOG_WARNING ? stderr : stdout;
		remove_progress_msg_internal();
		fprintf( target, "%s", msg );
		fflush( target );
	}
	// The LockGuardDouble dtor frees the locks in reverse order
}


/************************************************
*** Static internal functions implementations ***
************************************************/

static void pwx::log_close_internal( bool with_lock ) {
	if ( logfile_p ) {
		if ( with_lock && ( log_thread_count > 1 ) ) {
			logfile_lock.lock();
		}

		fflush( logfile_p );
		fclose( logfile_p );
		logfile_p = nullptr;

		if ( with_lock && ( log_thread_count > 1 ) ) {
			logfile_lock.unlock();
		}

	}
	logfile_name = "";
}


static int pwx::log_open_internal( char const* file_name, char const* mode ) {
	int r = 0;

	if ( file_name || !logfile_name.empty() ) {
		log_debug( nullptr, "Opening log file \"%s\" with mode \"%s\"", strnull( file_name ), strempty( mode ) );
	}

	// Always lock before doing anything
	if ( log_thread_count > 1 ) {
		logfile_lock.lock();
	}

	// Now close the current file if any
	if ( logfile_p ) {
		log_close_internal( false );
	}

	logfile_name = strempty( file_name );
#if PWX_IS_MSVC
	errno = fopen_s( &logfile_p, file_name, mode );
#else
	logfile_p = file_name ? fopen( file_name, mode ) : nullptr;
#endif // Weird stuff, MS!

	if ( file_name && ( nullptr == logfile_p ) ) {
		char err_buf[128] = { 0x0 };

		r = errno; // Safe it, it might be changed now

		// Let's see what it did
		log_error( "fopen() failed!", "Could not open %s: (%d) \"%s\"",
		           file_name, r, strerror_r( r, err_buf, 128 ) );

		logfile_name = "";
	}

	// release the lock
	if ( log_thread_count > 1 ) {
		logfile_lock.unlock();
	}

	return r;
}


static void pwx::remove_progress_msg_internal() {
	if ( have_progress_msg.load() ) {
		memset( progress_msg, ' ', progress_len );
		fprintf( stdout, "\r%s\r", progress_msg );
		fflush( stdout );
		have_progress_msg.store( false );
		progress_len = -1;
	}
}

#endif // Do not document with doxygen

