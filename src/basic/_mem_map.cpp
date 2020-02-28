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


#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "basic/_mem_map.h"
#include "basic/alloc_utils.h"
#include "basic/mem_utils.h"




/// @namespace pwx
namespace pwx {


#ifndef PWX_NODOX
/// @namespace private_
namespace private_ {


/// @brief information struct with location and size of a map entry
struct map_item_t {
	char const* location = nullptr;
	size_t      mem_size = 0;

	explicit
	map_item_t( char const* loc, size_t siz )
		: location( strdup( loc ? loc : "<nowhere>" ) )
		, mem_size( siz )
	{ }
	map_item_t( const map_item_t &rhs ) {
		location = rhs.location;
		mem_size = rhs.mem_size;
	}
	map_item_t( map_item_t &&rhs ) {
		location = rhs.location;
		mem_size = rhs.mem_size;
		rhs.location = nullptr;
		rhs.mem_size = 0;
	}
	map_item_t &operator=( const map_item_t &rhs ) {
		if ( &rhs != this ) {
			location = rhs.location;
			mem_size = rhs.mem_size;
		}
		return *this;
	}
	map_item_t &operator=( map_item_t &&rhs ) {
		if ( &rhs != this ) {
			location = rhs.location;
			mem_size = rhs.mem_size;
			rhs.location = nullptr;
			rhs.mem_size = 0;
		}
		return *this;
	}
	~map_item_t() {
		FREE_PTR( location );
	}
};

// some shortcuts
typedef std::map<void const*, map_item_t>                 map_t;
typedef std::map<void const*, map_item_t>::const_iterator citer_t;

/** @brief global memory map
  * We are using the std::map here to have something neutral.
**/
map_t mem_map;


void mem_map_add( char const* location, size_t mem_size, void const* memory ) {
	assert ( location );
	assert ( mem_size );
	assert ( memory );

	// First check whether the key exists:
	citer_t iter = mem_map.find( memory );
	if ( iter != mem_map.cend() ) {
		DEBUG_ERR( "Memory Map Addition Error!",
		           "The address 0x%08lx is already registered with size %lu from %s",
		           iter->first, iter->second.mem_size, iter->second.location );
		// No return, we have to record the new size anyway
	}

	// Second, put in the new item (Or replace the old)
	mem_map.insert_or_assign( memory, map_item_t( location, mem_size ) );
}


void mem_map_del( void const* memory ) {
	assert ( memory );

	citer_t citer = mem_map.find( memory );
	if ( citer == mem_map.cend() ) {
		DEBUG_ERR( "Memory Map Deletion Error!",
		           "The address 0x%08lx is _NOT_ recorded in the memory map!",
		           memory );
		return;
	}

	mem_map.erase( memory );
}


bool mem_map_report() {
	bool result = true;

	// A simple loop will do
	for ( citer_t iter = mem_map.cbegin(); iter != mem_map.cend(); iter++ ) {
		result = false;
		DEBUG_ERR( "Memory Map Leak Error!",
		           "The address 0x%08lx is *STILL* registered with size %lu from %s",
		           iter->first, iter->second.mem_size, iter->second.location );
		// The final clearing of the memory map does not touch the pointers,
		// so it is safe to free the key here.
		mfree( const_cast<void*>( iter->first ) );
	}

	// Now clear the map and we are done
	mem_map.clear();

	return result;
}


bool mem_map_sizeof( void const* memory, size_t* old_size ) {
	assert ( memory );
	assert ( old_size );

	citer_t iter = mem_map.find( memory );
	if ( iter != mem_map.cend() ) {
		if ( old_size )
			*old_size = iter->second.mem_size;
		return true;
	}
	return false;
}



} // namespace private_
#endif // Private stuff of no concern to Doxygen

} // namespace pwx


