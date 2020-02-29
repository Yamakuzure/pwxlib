#ifndef PWX_PWXLIB_SRC_BASIC_PWX_TRACE_INFO_H_INCLUDED
#define PWX_PWXLIB_SRC_BASIC_PWX_TRACE_INFO_H_INCLUDED 1
#pragma once


#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"


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
  * The exception tracers and debug log macros make extensive use of this function.
  * @see basic/pwx_debug.h
  * @see basic/pwx_macros.h
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
  * @see basic/pwx_macros.h
  *
  * @param[in] msg A message to show first. Formatting sequences will be ignored.
  * @param[in] path The path to the file, the function will extract the file name from it.
  * @param[in] line The line number to show
  * @param[in] func The name of the function from which this was called.
  * @return thread_local and static C-string buffer, NULL-terminated.
**/
char const* get_trace_msg ( char const* msg, char const* path, size_t line, char const* func ) noexcept PWX_WARNUNUSED PWX_API;


} // namespace pwx


#endif // PWX_PWXLIB_SRC_BASIC_PWX_TRACE_INFO_H_INCLUDED

