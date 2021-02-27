#ifndef PWX_PWXLIB_SRC_BASIC_ALLOC_UTIL_H_INCLUDED
#define PWX_PWXLIB_SRC_BASIC_ALLOC_UTIL_H_INCLUDED 1
#pragma once


/** @file alloc_utils.h
  *
  * @brief Memory utilities with extra perks
  *
  * For memory utilities that utilize the optional memory map debug tooling
  * @see mem_utils.h
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

#include <cstdlib>
#include <cstring>


/** @brief Allocator with overflow detection (like `malloc`)
  * @param[in] t  Type of the resulting pointer
  * @param[in] n  Number of bytes to allocate
  * @return A pointer to the allocated space, or `nullptr` if `sizeof(t) * n` is too large.
**/
#define pwx_new( t, n ) ((t*)::pwx::malloc_multiply(sizeof(t), (n)))


/** @brief Nullifying allocator with overflow detection (like `calloc`)
  * @param[in] t  Type of the resulting pointer
  * @param[in] n  Number of bytes to allocate
  * @return A pointer to the allocated space, or `nullptr` if `sizeof(t) * n` is too large.
**/
#define pwx_new0( t, n ) ::pwx::pwx_new0_impl<t>(n)


/** @brief Free memory at pointer @a p if it is not `nullptr` and set @a p to nullptr
  * @param[in,out] p  The pointer to free and to set to nullptr
**/
#define FREE_PTR( p )   (p) = (decltype(p))::pwx::mfree((void*)(p))


/** @brief Take over pointer @a ptr and set it to nullptr
  * @param[in] ptr  The pointer to take over, is set to `nullptr` after the transition
  * @return The address of `ptr`. It is not checked. If `ptr` is `nullptr`, that is what is returned.
**/
static auto TAKE_PTR = []( auto &ptr ) {
	decltype( ptr ) _ptr_ = ( ptr );
	( ptr ) = nullptr;
	return _ptr_;
};


/// @namespace pwx
namespace pwx {


/** @brief Free @a p if it is set
  * @param[in,out] p  The pointer to free if it is set.
**/
static inline void freep( void* p ) {
	if ( p ) ::free( p );
}


/** @brief Free @a p if it is set and return nullptr.
  * @param[in,out] p  The pointer to free if it is set.
  * @return nullptr - This is a convenient function.
**/
static inline void* mfree( void* p ) {
	if ( p ) freep( p );
	return nullptr;
}


#ifndef PWX_NODOX
static inline bool size_multiply_overflow( size_t s, size_t n ) {
	return PWX_UNLIKELY( n && ( s > ( SIZE_MAX / n ) ) );
}


/** @brief convenient allocator that only calls `malloc` if `SIZE_MAX` is not exceeded.
  * @param[in]  s  Size of the type to allocate
  * @param[in]  n  Number of elements of type @a s to allocate
  * @return A pointer to the allocated space or nullptr if the allocation failed.
**/
PWX_MALLOC PWX_ALLOC( 1, 2 ) static inline void* malloc_multiply( size_t s, size_t n ) {
	if ( size_multiply_overflow( s, n ) ) {
		return nullptr;
	}

	return ::malloc( s * n );
}


/// @brief Implementation template for pwx_new0
/// @internal
template< typename T > static inline T* pwx_new0_impl( size_t num ) {
	T* _ptr_ = pwx_new( T, num );
	if ( _ptr_ ) memset( _ptr_, 0, num );
	return _ptr_;
}


#endif // Do not document with doxygen


} // namespace pwx


#endif // PWX_PWXLIB_SRC_BASIC_ALLOC_UTIL_H_INCLUDED

