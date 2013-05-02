#include <pwx/general/compiler.h>
#include <cstdarg>
#include <thread>

namespace pwx {

#if defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)

// The central log needs a log lock:
std::atomic_flag _pwx_internal_LOG_output_lock = ATOMIC_FLAG_INIT;

void debug_log(const char* fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);

	while (_pwx_internal_LOG_output_lock.test_and_set(PWX_MEMORDER_ACQUIRE))
		std::this_thread::yield();

    vfprintf (stderr, fmt, ap);

	_pwx_internal_LOG_output_lock.clear(PWX_MEMORDER_RELEASE);

	va_end(ap);
}

#endif // LIBPWX_DEBUG || PWX_THREADDEBUG

} // namespace pwx
