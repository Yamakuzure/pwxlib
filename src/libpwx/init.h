#ifndef PWX_PWXLIB_SRC_LIBPWX_INIT_H_INCLUDED
#define PWX_PWXLIB_SRC_LIBPWX_INIT_H_INCLUDED 1
#pragma once


/** @file finish.h
  *
  * @brief Function to clean up on programs end and to clear up in debugging mode. (see pwx::finish())
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


/// @namespace pwx
namespace pwx {


/** @brief Initialize pwxLib
  *
  * Call this function first in your program.
  * If you set @a enable_memory_mapping_ to true, which is default in debugging mode, all allocations
  * and deallocations done via the utilities in basic/mem_utils.h are recorded. Call `pwx::finish()`
  * at the end of your program to get an analysis whether everything allocated was freed or not.
  *
  * @see pwx::finish()
  *
  * @param[in] enable_memory_mapping_  If set to true, all allocations/deallocations are recorded.
  * @param[in] log_file_path  Full path to the logfile to write (appended). Use NULL or empty string to disable.
  * @param[in] log_threads  Number of background threads writing log messages. < 1 to disable, maximum is 4.
**/
void init( bool enable_memory_mapping_, char const* log_file_path, int log_threads ) noexcept PWX_API;


} // namespace pwx


#endif // PWX_PWXLIB_SRC_LIBPWX_INIT_H_INCLUDED
