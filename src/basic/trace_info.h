#ifndef PWX_PWXLIB_SRC_BASIC_PWX_TRACE_INFO_H_INCLUDED
#define PWX_PWXLIB_SRC_BASIC_PWX_TRACE_INFO_H_INCLUDED 1
#pragma once

/** @file pwx_trace_info.h
  *
  * @brief Helper functions to get tracing information strings out of location data
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
#include "basic/macros.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>


/** @def PWX_TRACE_INFO
  * @brief Convenience shortcut
**/
#define PWX_TRACE_INFO ::pwx::get_trace_info(__FILE__, __LINE__, PWX_FUNC)


/// @namespace pwx
namespace pwx {


/** @brief Return a string with `filename`:`line number`:`function`
  *
  * This is a convenient function to print the filename within @a path, the @a line number and the function
  * name in @a func into a C-String and return a pointer to it.
  *
  * The string is static and thread_local, so you do neither have to lock anything nor to free the string
  * after usage.
  *
  * The exception tracers and log functions/macros make extensive use of this function.
  * @see basic/debug.h
  * @see basic/macros.h
  * @see log/log.h
  *
  * @param[in] path The path to the file, the function will extract the file name from it.
  * @param[in] line The line number to show
  * @param[in] func The name of the function from which this was called.
  * @return thread_local and static C-string buffer, NULL-terminated.
**/
char const* get_trace_info( char const* path, size_t line, char const* func ) noexcept PWX_WARNUNUSED PWX_API;


/** @brief Return a string with `filename`:`line number`:`function`
  *
  * This is a convenient function to print a user defined message, the filename within @a path, the @a line
  * number and the function name in @a func into a C-String and return a pointer to it.
  *
  * The string is static and thread_local, so you do neither have to lock anything nor to free the string
  * after usage.
  *
  * The exception tracer `PWX_THROW_PWX_FURTHER` makes use of this.
  * @see basic/CException.h
  *
  * @param[in] msg A message to show first. Formatting sequences will be ignored.
  * @param[in] path The path to the file, the function will extract the file name from it.
  * @param[in] line The line number to show
  * @param[in] func The name of the function from which this was called.
  * @return thread_local and static C-string buffer, NULL-terminated.
**/
char const* get_trace_msg( char const* msg, char const* path, size_t line, char const* func ) noexcept PWX_WARNUNUSED PWX_API;


} // namespace pwx


#endif // PWX_PWXLIB_SRC_BASIC_PWX_TRACE_INFO_H_INCLUDED

