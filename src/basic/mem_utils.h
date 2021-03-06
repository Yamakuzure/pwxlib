
#ifndef PWX_PWXLIB_SRC_BASIC_MEM_UTILS_H_INCLUDED
#define PWX_PWXLIB_SRC_BASIC_MEM_UTILS_H_INCLUDED 1
#pragma once


/** @file mem_utils.h
  *
  * @brief Memory utilities with optional debugging capabilities
  *
  * For memory utilities that have to circumvent the memory map debug tooling
  * @see alloc_utils.h
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


#include <cstring>

#include "basic/compiler.h"
#include "basic/macros.h"


/* --- Memory helper macros --- */
#define pwx_alloc( type_, nmem_ )         (type_*)::pwx::allocate(   PWX_TRACE_INFO,       sizeof(type_) * (nmem_) )
#define pwx_calloc( type_, nmem_ )        (type_*)::pwx::callocate(  PWX_TRACE_INFO,       sizeof(type_) * (nmem_) )
#define pwx_free( mem_ )                          ::pwx::deallocate( (void*)(mem_) )
#define pwx_realloc( type_, mem_, nmem_ ) (type_*)::pwx::reallocate( PWX_TRACE_INFO, mem_, sizeof(type_) * (nmem_) )
#define pwx_strdup( src_ )                        ::pwx::strdup(     PWX_TRACE_INFO, src_                          )
/** @def pwx_alloc
  * @brief Convenient wrapper to call pwx::allocate() with automatic location
  * Makes use of an internal memory map to detect leaks in debug mode. (@see pwx::finish())
  *
  * @def pwx_calloc
  * @brief Convenient wrapper to call pwx::callocate() with automatic location
  * Makes use of an internal memory map to detect leaks in debug mode. (@see pwx::finish())
  *
  * @def pwx_free
  * @brief Convenient wrapper to call pwx::deallocate() with automatic location
  * Makes use of an internal memory map to detect leaks in debug mode. (@see pwx::finish())
  *
  * @def pwx_realloc
  * @brief Convenient wrapper to call pwx::reallocate() with automatic location
  * Makes use of an internal memory map to detect leaks in debug mode. (@see pwx::finish())
  *
  * @def pwx_strdup
  * @brief Convenient wrapper to call pwx::strdup() with automatic location
  * Alternative string duplication using internal memory mapping in debug mode. (@see pwx::finish())
**/

/// @namespace pwx
namespace pwx {


/** @brief Central memory allocation function
  *
  * This function allocates memory with two extras when built in debugging mode:
  *
  * 1. The location, type and size of the allocation is logged.
  * 2. The allocation is recorded, and all records are checked against memory
  *    leaks on exit.
  *
  * Please use the helper macros `pwx_alloc()` and `pwx_calloc()`, which already
  * fill in the `location`.
  *
  * @param[in] location The location in the form &lt;filename&gt;:&lt;lineno&gt;:&lt;function&gt;
  * @param[in] new_size The size in bytes
  * @return void pointer to the allocated memory or nullptr on error.
**/
void* allocate( char const* location, size_t new_size ) PWX_API;


/** @brief Simple helper to already nullify the memory allocated with `allocate()`
  * @see allocate()
  * @param[in] location The location in the form &lt;filename&gt;:&lt;lineno&gt;:&lt;function&gt;
  * @param[in] new_size The size in bytes
  * @return void pointer to the allocated memory or nullptr on error.
**/
static inline
void* callocate( char const* location, size_t new_size ) {
	void* new_mem = allocate( location, new_size );
	if ( new_mem ) {
		memset( new_mem, 0, new_size );
	}
	return new_mem;
}


/** @brief Central memory deallocation function
  *
  * This function deallocates memory with two extras when built in debugging mode:
  *
  * 1. The location, type and size of the deallocation is logged.
  * 2. The freed allocation is taken out of the records.
  *
  * Please use the helper macro `pwx_free()`, which already fills in the `location`.
  *
  * @param[in] mem The pointer to the memory to be freed.
**/
void deallocate( void* mem ) PWX_API;


/** @brief Central memory reallocation function
  *
  * This function reallocates memory with two extras when built in debugging mode:
  *
  * 1. If the memory was not recorded, it will be now and the incident is logged.
  * 2. If the reallocation makes no sense (new_size <= current size), the incident
  *    will be logged.
  *
  * Please use the helper macro `pwx_realloc()`, which already fills in the `location`.
  *
  * @param[in] location The location in the form &lt;filename&gt;:&lt;lineno&gt;:&lt;function&gt;
  * @param[in] mem Pointer to the memory that is to be reallocated
  * @param[in] new_size The size in bytes
  * @return void pointer to the reallocated memory or nullptr on error.
**/
void* reallocate( char const* location, void* mem, size_t new_size ) PWX_API;


/** @brief Central string duplication function
  *
  * This function duplicates a C-string with an extra when built in debugging mode:
  *
  * If the memory was not recorded, it will be now and the incident is logged.
  *
  * Please use the helper macro `pwx_strdup()`, which already fills in the `location`.
  *
  * @param[in] location The location in the form &lt;filename&gt;:&lt;lineno&gt;:&lt;function&gt;
  * @param[in] src Pointer to the C-string to be duplicated
  * @return Pointer to the duplicated C-String or nullptr on error.
**/
char* strdup( char const* location, char const* src ) PWX_API;


/** @brief walk the memory map and report all entries as errors.
  *
  * *Note*: If PWXLIB_DEBUG is not defined, this function does nothing.
  *
  * All entries are logged as errors!
**/
bool mem_map_report() PWX_API;


} // namespace pwx


#endif // PWX_PWXLIB_SRC_BASIC_MEM_UTILS_H_INCLUDED

