#pragma once
#ifndef PWX_MSVCWRAP_H_INCLUDED
#define PWX_MSVCWRAP_H_INCLUDED 1

/** @internal
  * @file MSVCwrap.h
  * ------------------------
  *
  * @brief Wrap functions that are different between gcc and MSVC++
  *
  * This header simply defines methods to point to the correct version
  * of either MSVC++ or gcc. This should not have any impact, as it is
  * done using the pre-processor only.
  *
  * (c) 2007-2012 Sven Eden, PrydeWorX
  * @author Sven Eden, PrydeWorX - Hamburg, Germany
  *         yamakuzure@users.sourceforge.net
  *         http://pwxlib.sourceforge.net
  *
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.1.0     2011-01-23  sed, PrydeWorX  Initial Release with strdup, pow and round/floor
  * 0.1.1     2011-03-21  sed, PrydeWorX  Spring starts today, and Sleep/usleep have a wrapper now.
  * 0.1.2     2011-03-22  sed, PrydeWorX  Another wrapper: pwx_file_exists() for [_]access(fname, F_OK/0)
  * 0.1.3     2011-04-06  sed, PrydeWorX  Added doxygen documentation
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version correction and bump to new pwxLib release version
  * 0.7.2.1   2011-04-20  sed, PrydeWorX  Extended the (_)access() wrappers to read/write/execute permissions
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5     2012-03-01  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5.1   2012-04-12  sed, PrydeWorX  Added pwx_snprintf to get rid of (unsecure) sprintf
  * 0.8.5.2   2012-04-12  sed, PrydeWorX  Added pwx_strncpy to get rid of (unsecure) strcpy
  * 0.8.5.3   2012-04-17  sed, PrydeWorX  Added pwx_unlink to wrap to _unlink with VC++
  * 0.8.5.4   2012-04-23  sed, PrydeWorX  Added pwx_vsnprintf and changed windows _sleep() to Sleep()
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

#if defined(_MSC_VER)
// Microsoft versions of the functions
/// Check whether file @a f exists
#  define pwx_file_exists(f) (-1 != (_access(f, 0)))
/// Check whether file @a f is executable
#  define pwx_file_isX(f) (-1 != (_access(f, 1)))
/// Check whether file @a f is writable
#  define pwx_file_isW(f) (-1 != (_access(f, 2)))
/// Check whether file @a f is readable
#  define pwx_file_isR(f) (-1 != (_access(f, 4)))
/// Check whether file @a f is read- and writable
#  define pwx_file_isRW(f) (-1 != (_access(f, 6)))
/// Calculate the @a e power of @a x
#  define pwx_pow(x, e)     ::std::pow(static_cast<double>(x), e)
/// Round @a x
#  define pwx_round(x)      floor(0.5 + (x))
/// sleep for @a t milliseconds
#  define pwx_sleep(t)      Sleep (t)
/// give back a malloc'd C-String copy of @a s
#  define pwx_strdup(s)     _strdup(s)
/// Use sprintf_s on windows
#  define pwx_snprintf(target, size, fmt, ...) sprintf_s(target, size, fmt, __VA_ARGS__)
/// Use vsprintf_s on windows
#  define pwx_vsnprintf(target, size, fmt, ...) vsprintf_s(target, size, fmt, __VA_ARGS__)
/// Use strcpy_s on windows
#  define pwx_strncpy(target, source, size) strcpy_s(target, size, source)
/// Use ISO C++ conformant name for unlink on windows
#  define pwx_unlink(target) _unlink(target)
#else
// gcc versions of the functions
/// Check whether file @a f exists
#  define pwx_file_exists(f) (0 == access(f, F_OK))
/// Check whether file @a f is executable
#  define pwx_file_isX(f) (0 == access(f, X_OK))
/// Check whether file @a f is writable
#  define pwx_file_isW(f) (0 == access(f, W_OK))
/// Check whether file @a f is readable
#  define pwx_file_isR(f) (0 == access(f, R_OK))
/// Check whether file @a f is read- and writable
#  define pwx_file_isRW(f) (0 == access(f, W_OK | W_OK))
/// Calculate the @a e power of @a x
#  define pwx_pow(x, e)      ::std::pow(x, e)
/// Round @a x
#  define pwx_round(x)       round(x)
/// sleep for @a t milliseconds
#  define pwx_sleep(t)       usleep(1000 * t)
/// give back a malloc'd C-String copy of @a s
#  define pwx_strdup(s)      strdup(s)
/// Use snprintf on linux
#  define pwx_snprintf(target, size, fmt, ...) snprintf(target, size, fmt, __VA_ARGS__)
/// Use vsnprintf on linux
#  define pwx_vsnprintf(target, size, fmt, ...) vsnprintf(target, size, fmt, __VA_ARGS__)
/// Use strncpy on linux
#  define pwx_strncpy(target, source, size) strncpy(target, source, size)
/// Use POSIX name for unlink on linux
#  define pwx_unlink(target) unlink(target)
#endif // MSVC++ versus gcc

#endif // PWX_MSVCWRAP_H_INCLUDED

