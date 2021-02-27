#ifndef PWX_PWXLIB_SRC_LOG_LOG_H_INCLUDED
#define PWX_PWXLIB_SRC_LOG_LOG_H_INCLUDED 1
#pragma once
/** @file log.h
  *
  * @brief Declaration of logging functions
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

#include "basic/compiler.h"
#include "basic/trace_info.h"
#include "log/log_level.h"


#include <cstdint>


/// @namespace pwx
namespace pwx {


/** @brief Central logging function
  *
  * The unified output is:
  *
  *    `%Y-%m-%d %H:%M:%S|<level>|<filename>:<lineno>:<function>|<message>`
  *
  * The message is split to not be longer than 120 characters per line. Follow-up lines
  * begin two spaces left of the first pipe, only singling out the date and time.
  *
  * If a @a title is set, it will be logged **as is** on the first line. This means that
  * a title is *never* truncated and you are responsible for its length!
  *
  * Please use the helper macros `log_debug()`, `log_info()`, `log_status()`,
  * `log_warning()`, `log_error()` and `log_critical()`, which already fill in
  * `location` and `level`.
  *
  * This function is thread safe, it uses a central lock for concurrent writing.
  *
  * @param[in] location The location in the form <filename>:<lineno>:<function>
  * @param[in] level The severity of the message
  * @param[in] title If set, a title will be set as the first log line, the message then follows on line two.
  * @param[in] message Whatever you wish to tell your readers, printf style, with arguments following.
**/
void log( char const* location, log_level_t level, char const* title, char const* message, ... ) noexcept PWX_API;


/// @brief Flush and close the current log file, if any
void log_close();


/** @brief Enable logger threads
  *
  * By default every log message is assembled to a unified format and then send to
  * the log file or stdout/stderr. This assembling takes some time.
  * If a program logs many messages this can slow down the application significantly.
  *
  * With this function all log messages are send to a queue, and the configured amount
  * of threads then pull the information from the queue, assemble the log messages and
  * send them to the log file or stdout/stderr.
  *
  * This allows the program to continue its work after sending the message to the queue
  * instead of having to wait for the assembling and writing. Further one thread can
  * assemble the next message while another is still writing to the log file.
  *
  * A unique id of the queue items ensures that the messages reach the log file in the
  * correct order.
  *
  * Although any integer can be used for the amount of threads to set up, any lower
  * than 2 is ignored (*) and any higher than 4 is capped to 4. The recommended amount of
  * threads is 2, this should be sufficient in most cases.
  *
  * (*): If you set @a thread_count to 0 (zero), any running threads will be gracefully
  *      stopped and joined.
  *
  * When the queue is empty, the log threads will poll the queue with increasing sleep
  * times for 500ms, and then go to indefinite sleep, in which they don't hog any cpu.
  *
  * **Important**: This is a one-time action! The amount of logger threads can not
  *                be changed, unless you deactivate and reactivate threadding completely!
  *
  * @param[in] thread_count Number of threads to start, valid are 2, 3, 4 and 0 to disable threading
**/
void log_enable_threads( int thread_count ) noexcept PWX_API;


/** @brief Return the name of the current log file
  *
  * If a log file is set, a `char const*` representation is given back. You must **not** free it!
  *
  * @return constant representation of the current log file name or NULL if none was set
**/
char const* log_file_name() noexcept PWX_API;


/** @brief open a logfile for further writes
  *
  * If another log file is already open, it will be flushed and closed first.
  *
  * @param[in] file_name The path to the file to write.
  * @param[in] mode The open mode. Useful are "a" to append and "w" to overwrite.
  * @return Zero on success, the negative content of errno if opening fails. (See fopen(3), malloc(3), open(2))
**/
int log_open( char const* file_name, char const* mode ) noexcept PWX_API;


/** @brief set the log verbosity
  *
  * The log verbosity determines up to which messages are written into the
  * log file and onto the console.
  *
  * The log levels are:
  *  0 = debug (Only printed when built in debug mode)
  *  1 = info
  *  2 = status            (default for log files)
  *  3 = warnings
  *  4 = errors            (default for console)
  *  5 = critical failures
  *  6 = Turn off logging completely
  * The default is to write everything from status messages up into the log file, and only
  * errors and critical failures onto the console.
  *
  * @param[in] level_logfile  Level from which on to write into the log file
  * @param[in] level_console  Level from which on to write to the console
**/
void log_set_verbosity( int32_t level_logfile, int32_t level_console ) noexcept PWX_API;


/** @brief Show a line of progess information without breaking the line
  *
  * This function can be used to produce a line with progress information on STDOUT,
  * that gets removed by any successive call, or call to any of the log functions.
  *
  * Important: You have 127 characters. If your message is longer, it will be cut off.
  *            No discussion! But honestly, it is wise to keep it shorter than 80 chars.
  *
  * @param[in] message Whatever you wish to tell your readers, printf style, with arguments following.
**/
void show_progress( char const* message, ... ) noexcept PWX_API;


} // namespace pwx


/// @brief Log wrapper - Used by the log helper macros
#define PWX_log_wrapper( _l_, _t_, _m_, ... ) \
    ::pwx::log( \
              ::pwx::get_trace_info(__FILE__, __LINE__, __func__), \
              _l_, _t_, _m_, __VA_ARGS__ \
            )


/// @brief Log wrapper with errno message fetch - Used by log_errno()
#define PWX_log_errno( _e_, _t_, _m_, ... ) { \
    char const* _err_msg_ = pwx_strerror( _e_ ); \
    PWX_log_wrapper( LOG_ERROR, _t_, _m_ ": %s", __VA_ARGS__, _err_msg_ ); \
} do {} while(0)


/* --- Log helper macros --- */

/** @def log_info
  * @brief Log an info message
  * @params[in] title_ An optional title, the message then startes on the next line, or nullptr to not show a title
  * @params[in] message_ The message to display, following `printf()` rules
**/
#define log_info( title_, message_, ... )          PWX_log_wrapper(LOG_INFO,     title_, message_, __VA_ARGS__)

/** @def log_status
  * @brief Log a status message
  * @params[in] title_ An optional title, the message then startes on the next line, or nullptr to not show a title
  * @params[in] message_ The message to display, following `printf()` rules
**/
#define log_status( title_, message_, ... )        PWX_log_wrapper(LOG_STATUS,   title_, message_, __VA_ARGS__)

/** @def log_warning
  * @brief Log a warning message
  * @params[in] title_ An optional title, the message then startes on the next line, or nullptr to not show a title
  * @params[in] message_ The message to display, following `printf()` rules
**/
#define log_warning( title_, message_, ... )       PWX_log_wrapper(LOG_WARNING,  title_, message_, __VA_ARGS__)

/** @def log_error
  * @brief Log an error message
  * @params[in] title_ An optional title, the message then startes on the next line, or nullptr to not show a title
  * @params[in] message_ The message to display, following `printf()` rules
**/
#define log_error( title_, message_, ... )         PWX_log_wrapper(LOG_ERROR,    title_, message_, __VA_ARGS__)

/** @def log_errno
  * @brief Log an error message with errno text substitution
  * @params[in] title_ An optional title, the message then startes on the next line, or nullptr to not show a title
  * @param[in] errno_ The errno to get the `strerror()` from
  * @params[in] message_ The message to display, following `printf()` rules
**/
#define log_errno( title_, errno_, message_, ... ) PWX_log_errno(        errno_, title_, message_, __VA_ARGS__)

/** @def log_critical
  * @brief Log a critical message
  * @params[in] title_ An optional title, the message then startes on the next line, or nullptr to not show a title
  * @params[in] message_ The message to display, following `printf()` rules
**/
#define log_critical( title_, message_, ... )      PWX_log_wrapper(LOG_CRITICAL, title_, message_, __VA_ARGS__)


#endif // PWX_PWXLIB_SRC_LOG_LOG_H_INCLUDED

