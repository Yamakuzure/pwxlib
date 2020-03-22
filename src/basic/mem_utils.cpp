/** @file
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2020 PrydeWorX
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


#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "basic/alloc_utils.h"
#include "basic/mem_utils.h"

#if LIBPWX_DEBUG
#  include "basic/_mem_map.h"
#endif // LIBPWX_DEBUG


/// @namespace pwx
namespace pwx {


/// @namespace private_
namespace private_ {
	#ifndef PWX_NODOX
	#if defined(PWXLIB_DEBUG)
	bool enable_memory_mapping = true;
	#else
	bool enable_memory_mapping = false;
	#endif // PWXLIB_DEBUG
	#endif // No doxygen on private globals!
}


/***************************************
*** Public functions implementations ***
***************************************/

void* allocate( [[maybe_unused]] char const* location, size_t new_size ) {
	void* result = nullptr;

	result = malloc_multiply( 1, new_size );

	if ( result && private_::enable_memory_mapping )
		private_::mem_map_add( location, new_size, result );

	if ( !result )
		DEBUG_ERR_THERE( location, "Allocation failed!", "Unable to allocate %ul bytes at %s", new_size );

	return result;
}


void deallocate( [[maybe_unused]] char const* location, void*  mem ) {
	if ( mem ) {

		if ( private_::enable_memory_mapping )
			private_::mem_map_del( mem );

		FREE_PTR( mem );
	}
}


void* reallocate( [[maybe_unused]] char const* location, void* mem, size_t new_size ) {

	// Reroute at once if mem is nullptr
	if ( nullptr == mem )
		return allocate( location, new_size );

	// Fine, it is a reallocation.
	void*  old_mem  = mem;
	void*  result   = nullptr;

	result = realloc( mem, new_size );

	if ( result && private_::enable_memory_mapping && ( old_mem != result ) ) {
		private_::mem_map_del( old_mem );
		private_::mem_map_add( location, new_size, result );
	}

	if ( !result )
		DEBUG_ERR_THERE( location, "Reallocation failed!",
		                 "Unable to reallocate to %ul bytes at %s", new_size );

	return result;
}


char* strdup( [[maybe_unused]] char const* location, char const* src ) {
	size_t nmem   = src ? strlen(src) : 0;
	char*  result = nmem ? (char*)allocate( location, (nmem + 1) * sizeof( char) ) : nullptr;

	if ( result  ) {
		memcpy( result, src, nmem * sizeof( char ) );
		result[ nmem ] = 0x0;
	}

	return result;
}


bool mem_map_report() {
	if ( private_::enable_memory_mapping )
		return private_::mem_map_report();
	return true;
}


} // namespace pwx


/* ------------------------------------------------------------------------
 * --- If this is a build of the library in debugging mode, then we     ---
 * --- have overriden the global new/delete to use our memory mapping   ---
 * --- (de)allocator in basic/pwx_compiler.h. Here are the definitions. ---
 * --------------------------------------------------------------------- */
#ifndef PWX_NODOX
#if defined(PWX_EXPORTS) && LIBPWX_DEBUG && defined(PWX_HAVE_DEBUG_NEW_DELETE)
void *operator new     (decltype(sizeof(0)) s) { return (void*)pwx_calloc(uint8_t, s); }
void *operator new[]   (decltype(sizeof(0)) s) { return (void*)pwx_calloc(uint8_t, s); }
void  operator delete  (void* ptr)                       noexcept { pwx_free(ptr); }
void  operator delete  (void* ptr, decltype(sizeof(0)) ) noexcept { pwx_free(ptr); }
void  operator delete[](void* ptr)                       noexcept { pwx_free(ptr); }
void  operator delete[](void* ptr, decltype(sizeof(0)) ) noexcept { pwx_free(ptr); }
#endif // exports in debugging mode
#endif // NODOX
