/** @file
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


#include "basic/_mem_map.h"
#include "basic/alloc_utils.h"
#include "basic/compiler.h"
#include "basic/debug.h"
#include "basic/mem_utils.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>


/// @namespace pwx
namespace pwx {


#ifndef PWX_NODOX

#if PWX_IS_DEBUG_MODE
bool enable_memory_mapping = true;
#else
bool enable_memory_mapping = false;
#endif // PWXLIB_DEBUG

#endif // No doxygen on private globals!


/***************************************
*** Public functions implementations ***
***************************************/

void* allocate( char const* location, size_t new_size ) {
	void* result = malloc_multiply( 1, new_size );

	if ( result && enable_memory_mapping ) {
		mem_map_add( location, new_size, result );
	}

	if ( !result )
		log_debug_there( location, "Allocation failed!", "Unable to allocate %ul bytes at %s", new_size );

	return result;
}


void deallocate( void* mem ) {
	if ( mem ) {

		if ( enable_memory_mapping ) {
			mem_map_del( mem );
		}

		FREE_PTR( mem );
	}
}


void* reallocate( char const* location, void* mem, size_t new_size ) {

	// Reroute at once if mem is nullptr
	if ( nullptr == mem ) {
		return allocate( location, new_size );
	}

	// Fine, it is a reallocation.
	void* old_mem = mem;
	void* result  = realloc( mem, new_size );

	if ( result && enable_memory_mapping && ( old_mem != result ) ) {
		mem_map_del( old_mem );
		mem_map_add( location, new_size, result );
	}

	if ( !result )
		log_debug_there( location, "Reallocation failed!",
		                 "Unable to reallocate to %ul bytes at %s", new_size );

	return result;
}


char* strdup( char const* location, char const* src ) {
	size_t nmem = src ? strlen( src ) : 0;
	char* result;

	result = nmem ? (char*) allocate( location, ( nmem + 1 ) * sizeof( char ) ) : nullptr;

	if ( result ) {
		memcpy( result, src, nmem * sizeof( char ) );
		result[nmem] = 0x0;
	}

	return result;
}


bool mem_map_report() {
	if ( enable_memory_mapping ) {
		return mem_map_report_internal();
	}
	return true;
}


} // namespace pwx
