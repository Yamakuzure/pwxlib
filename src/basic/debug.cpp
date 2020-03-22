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


#if LIBPWX_DEBUG
#  include <cstdarg>
#  include <thread>

#  include "basic/pwx_compiler.h"
#  include "basic/pwx_macros.h"
#  include "basic/pwx_types.h"
#endif // LIBPWX_DEBUG

#include "basic/pwx_debug.h"


/// @namespace pwx
namespace pwx {


#if LIBPWX_DEBUG

/** @namespace private_
  * @internal
**/
namespace private_ {


/// @brief The central log needs a log lock:
aflag_t _pwx_internal_LOG_output_lock = ATOMIC_FLAG_INIT;

static void debug_log_out( _IO_FILE* target, char const* fmt, va_list ap ) {
	while ( _pwx_internal_LOG_output_lock.test_and_set( std::memory_order_acquire ) )
		std::this_thread::yield();

	vfprintf ( target, fmt, ap );

	_pwx_internal_LOG_output_lock.clear( std::memory_order_release );
}

} // namespace private_

void debug_log( char const* fmt, ... ) {
	va_list ap;
	va_start ( ap, fmt );
	private_::debug_log_out( stdout, fmt, ap );
	va_end( ap );
}

void debug_err( char const* fmt, ... ) {
	va_list ap;
	va_start ( ap, fmt );
	private_::debug_log_out( stderr, fmt, ap );
	va_end( ap );
}


#endif // LIBPWX_DEBUG

} // namespace pwx
