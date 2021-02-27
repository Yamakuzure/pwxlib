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


#include "basic/string_utils.h"
#include "basic/trace_info.h"
#include "libpwx/init.h"
#include "log/log.h"


#include <cerrno>
#include <cstring>
#include <iostream>


/// @namespace pwx
namespace pwx {


#ifndef PWX_NODOX
extern bool enable_memory_mapping;
#endif // No doxygen on private globals!


void init( bool enable_memory_mapping_, char const* log_file_path, int log_threads ) noexcept {
	enable_memory_mapping = enable_memory_mapping_;

	log_enable_threads( log_threads );

	if ( log_file_path && strlen( log_file_path ) ) {
		int r = log_open( log_file_path, "a" );
		if ( r ) {
			r = errno;
			::std::cerr << pwx::get_trace_info( __FILE__, __LINE__, __func__ )
			            << " FAILED: " << pwx_strerror( r ) << ::std::endl;
		}
	}
}


} // namespace pwx

