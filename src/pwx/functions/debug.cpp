#include <pwx/functions/debug.h>
#include <cstdarg>

namespace pwx {

#if defined(LIBPWX_DEBUG) || defined(PWX_THREADDEBUG)

void debug_log(const char* fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    vfprintf (stderr, fmt, ap);
	va_end(ap);
}

#endif // LIBPWX_DEBUG || PWX_THREADDEBUG

} // namespace pwx
