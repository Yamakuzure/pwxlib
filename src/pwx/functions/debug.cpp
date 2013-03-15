#include <pwx/functions/debug.h>
#include <cstdarg>

namespace pwx {

void debug_log(const char* fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    vfprintf (stderr, fmt, ap);
	va_end(ap);
}

} // namespace pwx
