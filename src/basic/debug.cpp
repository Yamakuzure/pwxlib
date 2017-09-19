/***
  This file is part of the PrydeWorX Library (pwxLib).

  Copyright 2007 - 2017 Sven Eden

  The PrydeWorX Library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  The PrydeWorX Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with pwxLib; If not, see <http://www.gnu.org/licenses/>.

  History and Changelog are maintained in pwx.h
***/


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
