#ifndef PWX_PWXLIB_SRC_BASIC__MEM_MAP_H_INCLUDED
#define PWX_PWXLIB_SRC_BASIC__MEM_MAP_H_INCLUDED 1
#pragma once


/** @internal
  * @file _mem_map.h
  *
  * @brief internal header to use the memory mapper facilities in debugging mode
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


#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"


#include <cstddef>
#include <cstdint>


#ifndef PWX_NODOX


/// @namespace pwx
namespace pwx {


/** @internal
  * @brief Add one entry to the memory map
  *
  * The address is the key, location and size are the information stored.
  *
  * If @a memory is already mapped, an error is logged.
  *
  * @param[in] location Location in the form <filename>:<lineno>:<function>
  * @param[in] mem_size Size of the allocated memory in bytes
  * @param[in] memory neutral void pointer to the memory to remember
**/
void mem_map_add( char const* location, size_t mem_size, void const* memory );


/** @brief delete one entry from the memory map
  *
  * The entry @a memory points to is removed from the memory map.
  *
  * If @a memory is not mapped, an error is logged.
  *
  * @param[in] memory neutral void pointer to the memory to remember
**/
void mem_map_del( void const* memory );


/** @brief walk the memory map and report all entries as errors.
  *
  * To not leak the memory although we know it isn't freed, all entries
  * are regularly freed here. Please note, that this function always uses
  * `free()`.
  *
  * All entries are logged as errors!
**/
bool mem_map_report_internal();


/** @brief Get the currently recorded size of mapped memory
  *
  * This is just a convenient function. It will not log if the questioned
  * memory isn't mapped, it only returns `false` in that case.
  *
  * @param[in] memory The pointer to check
  * @param[out] old_size Pointer to a `size_t` to receive the result
  * @return true on success, false if @a memory isn't mapped.
**/
bool mem_map_sizeof( void const* memory, size_t* old_size );


} // namespace pwx


#endif // Do not document with doxygen


#endif // PWX_PWXLIB_SRC_BASIC__MEM_MAP_H_INCLUDED

