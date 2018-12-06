/**
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2018 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://prydeworx.com/pwxlib/
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
  * History and change log are maintained in pwx.h
**/


#if defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)
#  include <cstdarg>
#  include <thread>

#  include "compiler.h"
#endif // defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)
#include "debug.h"


namespace pwx {


#if defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)

// The central log needs a log lock:
std::atomic_flag _pwx_internal_LOG_output_lock = ATOMIC_FLAG_INIT;

static void debug_log_out(_IO_FILE* target, const char* fmt, va_list ap)
{
	while (_pwx_internal_LOG_output_lock.test_and_set(PWX_MEMORDER_ACQUIRE))
		std::this_thread::yield();

	vfprintf (target, fmt, ap);

	_pwx_internal_LOG_output_lock.clear(PWX_MEMORDER_RELEASE);
}

void debug_log(const char* fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);
	debug_log_out(stdout, fmt, ap);
	va_end(ap);
}

void debug_err(const char* fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);
	debug_log_out(stderr, fmt, ap);
	va_end(ap);
}


#endif // LIBPWX_DEBUG || PWX_THREADDEBUG

} // namespace pwx
