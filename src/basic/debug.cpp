#include "debug.h"
#include "compiler.h"
#include <cstdarg>
#include <thread>

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
