/** @file
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2020 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
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
#include "basic/mem_utils.h"
#include "basic/pwx_compiler.h"
#include "basic/pwx_debug.h"


#include <cstddef>


/// @namespace pwx
namespace pwx {


/***************************************
*** Public functions implementations ***
***************************************/

void* allocate( char const* location, size_t new_size ) {
	void* result = nullptr;

	DEBUG_LOG_THERE( location, "allocate", "allocating %lu bytes", new_size );

	result = malloc_multiply( 1, new_size );

	#if defined(PWXLIB_DEBUG)
	if ( result )
		private_::mem_map_add( location, new_size, result );
	#endif // remember this allocation in debug mode

	if ( !result )
		DEBUG_ERR_THERE( location, "Allocation failed!", "Unable to allocate %ul bytes at %s", new_size );

	return result;
}


void deallocate( char const* location, void*  mem ) {
	if ( mem ) {
		DEBUG_LOG_THERE( location, "deallocate", "freeing memory at 0x%08lx", mem );

		#if defined(PWXLIB_DEBUG)
		private_::mem_map_del( mem );
		#endif // remove this allocation

		FREE_PTR( mem );
	}
}


void* reallocate( char const* location, void* mem, size_t new_size ) {

	// Reroute at once if mem is nullptr
	if ( nullptr == mem )
		return allocate( location, new_size );

	// Fine, it is a reallocation.
	void* result = nullptr;

	DEBUG_LOG_THERE( location, "reallocate", "reallocating to %lu bytes", new_size );

	#if defined(PWXLIB_DEBUG)
	// Warn if the reallocation makes no sense
	size_t old_size = 0;
	if ( private_::mem_map_sizeof( mem, &old_size ) ) {
		if ( old_size >= new_size ) {
			DEBUG_ERR_THERE( location, "reallocate",
			                 "Asked to reallocate from %ul to %ul bytes from %s",
			                 old_size, new_size );
		}
	}
	// Remember the old pointer, it might differ. (Size will anyway)
	void* old_mem = mem;
	#endif

	result = realloc( mem, new_size );

	#if defined(PWXLIB_DEBUG)
	if ( result ) {
		private_::mem_map_del( old_mem );
		private_::mem_map_add( location, new_size, result );
	}
	#endif // remember this allocation in debug mode

	if ( !result )
		DEBUG_ERR_THERE( location, "Reallocation failed!",
		                 "Unable to reallocate to %ul bytes at %s", new_size );

	return result;
}


bool mem_map_report() {
	#if defined(PWXLIB_DEBUG)
	return private_::mem_map_report();
	#else
	return true;
	#endif // Debug Mode
}


} // namespace pwx

