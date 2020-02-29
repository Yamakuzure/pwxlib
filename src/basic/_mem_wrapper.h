#ifndef PWX_PWXLIB_SRC_BASIC__MEM_WRAPPER_H_INCLUDED
#define PWX_PWXLIB_SRC_BASIC__MEM_WRAPPER_H_INCLUDED 1
#pragma once


/** @internal
  * @file _mem_wrapper.h
  *
  * @brief Simple wrapper to help with the mem utils
  *
  * For memory utilities that have to circumvent the memory map debug tooling
  * @see alloc_utils.h
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


#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_types.h"

#include <type_traits>

#ifndef PWX_NODOX

/* --- Internal helper macros --- */
#define PWX_alloc_internal(_s_) \
	::pwx::allocate( ::pwx::get_trace_info(__FILE__, __LINE__, PWX_FUNC), (_s_) )

#define PWX_calloc_internal(_s_) \
	::pwx::callocate( ::pwx::get_trace_info(__FILE__, __LINE__, PWX_FUNC), (_s_) )

#define PWX_dealloc_internal(_p_) {                                        \
		if ( (_p_) ) {                                             \
			::pwx::deallocate( ::pwx::get_trace_info(__FILE__, \
			                                        __LINE__,  \
			                                        PWX_FUNC), \
			                   (void*)(_p_) );                 \
		} }

#define PWX_realloc_internal(_p_, _s_) \
	::pwx::reallocate( ::pwx::get_trace_info(__FILE__, __LINE__, PWX_FUNC), (_p_), (_s_) );                         \



#endif // None of Doxygens business.


#endif // PWX_PWXLIB_SRC_BASIC__MEM_WRAPPER_H_INCLUDED

