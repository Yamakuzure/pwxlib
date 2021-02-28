#ifndef PWX_PWXLIB_BASE_MACROS_H_INCLUDED
#define PWX_PWXLIB_BASE_MACROS_H_INCLUDED 1
#pragma once

/** @file pwx_macros.h
  *
  * @brief Preprocessor macros for general usage.
  *
  * Note: You have to include the appropriate (system) headers yourself
  *       when using any of these macros.
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


#include "basic/compiler.h"


/* -----------------------------------------------------------------------
 * --- Various attributes and functions that are C++17 and upwards     ---
 * -------------------------------------------------------------------- */
#define PWX_CARRIES_DEP        [[carries_dependency]]  //!< (C++11)	indicates that dependency chain in release-consume
                                                       //!<           std::memory_order propagates in and out of the function
#define PWX_DEPRECATED_SIMPLE  [[deprecated]]          //!< (C++14) indicates that the use of the name or entity declared with
#define PWX_DEPRECATED(reason) [[deprecated(#reason)]] //!< (C++14)	  this attribute is allowed, but discouraged for some reason
#define PWX_FALLTHROUGH        [[fallthrough]]         //!< (C++17)	indicates that the fall through from the previous case label
                                                       //!<           is intentional and should not be diagnosed by a compiler
                                                       //!<           that warns on fall-through
#define PWX_MAYBE_UNUSED       [[maybe_unused]]        //!< (C++17) suppresses compiler warnings on unused entities, if any
#define PWX_NODISCARD_SIMPLE   [[nodiscard]]           //!< (C++17) encourages the compiler to issue a warning if the return
                                                       //!<           value is discarded
#if PWX_HAS_CXX20
#define PWX_NODISCARD(reason)  [[nodiscard(#reason)]]  //!< (C++20)	Same as @a see PWX_NODISCARD_SIMPLE with @a reason parameter
#else
#define PWX_NODISCARD(reason)
#endif // Only in C++20 and later
#define PWX_NORETURN           [[noreturn]]            //!< (C++11)	indicates that the function does not return


/* -----------------------------------------------------------------------
 * --- Various attributes and functions that are not universal         ---
 * -------------------------------------------------------------------- */


#ifndef PWX_NODOX
#if PWX_IS_MSVC
// Attributes
#  define PWX_PRINTF(...)
#  define PWX_ALLOC(...)
#  define PWX_PACKED
#  define PWX_MALLOC
#  define PWX_WEAK
#  define PWX_LIKELY(x)       (x)
#  define PWX_UNLIKELY(x)     (x)
#  define PWX_WARNUNUSED      _Check_return_
// Functions
#  define strcasecmp          _stricmp
#  define strdup              _strdup
#  define strerror_r(n, b, l) ( strerror_s(b, l, n) ? b : "strerror_s failed" )
#  define strncasecmp         _strnicmp
#else
#  define PWX_PRINTF(a,b) __attribute__ ((format (printf, a, b)))
#  if PWX_IS_CLANG
#    define PWX_ALLOC(...)
#  else
#    define PWX_ALLOC(...) __attribute__ ((alloc_size(__VA_ARGS__)))
#  endif // CLang does not know this attribute, yet.
#  define PWX_PACKED       __attribute__ ((packed))
#  define PWX_MALLOC       __attribute__ ((malloc))
#  define PWX_WEAK         __attribute__ ((weak))
#  define PWX_LIKELY(x)   (__builtin_expect(!!(x),1))
#  define PWX_UNLIKELY(x) (__builtin_expect(!!(x),0))
#  define PWX_WARNUNUSED  __attribute__ ((warn_unused_result))
#endif // Macros only for gcc/clang


#define VOID_0 ((void)0)
#define ELEMENTSOF(x) ({                              \
	static_assert( std::rank<typeof(x)>::value ); \
	size_t _res_ = sizeof(x)/sizeof((x)[0]);      \
	_res_;                                        \
})


#define PWX_FUNC  __func__


#endif // ignored by doxygen


/** @brief Alias for the current threads get_id()
  *
  * *Prerequisites*: thread
  *
  * @return The std::thread::id of the current thread.
**/
#if PWX_IS_MSVC
#  define CURRENT_THREAD_ID static_cast<size_t>(_Thrd_id())
#else
#  define CURRENT_THREAD_ID static_cast<size_t>(__gthread_self())
#endif // MSVC vs gcc/clang


/* -----------------------------------------------------------------------
 * --- General purpose macros with documentation                       ---
 * -------------------------------------------------------------------- */

/** @brief Return the sign as -1 or +1 of an expression
  *
  * *Prerequisites*: none
  * @param[in] expr any expression that can be compared against 0 (zero)
  * @return -1 if @a expr < 0, 1 otherwise
**/
#define SIGN(expr) (((expr) < 0) ? -1 : 1)


/** @brief Check whether file @a f exists
  *
  * *Prerequisites*: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a exists
**/
#define pwx_file_exists(f) (0 == access(f, F_OK))


/** @brief Check whether file @a f is executable
  *
  * *Prerequisites*: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is executable
**/
#define pwx_file_isX(f) (0 == access(f, X_OK))


/** @brief Check whether file @a f is writable
  *
  * *Prerequisites*: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is writable
**/
#define pwx_file_isW(f) (0 == access(f, W_OK))


/** @brief Check whether file @a f is readable
  *
  * *Prerequisites*: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is readable
**/
#define pwx_file_isR(f) (0 == access(f, R_OK))


/** @brief Check whether file @a f is readable and writable
  *
  * *Prerequisites*: unistd.h
  *
  * @param f A file or directory to check
  * @return true if @a is readable and writable
**/
#define pwx_file_isRW(f) (0 == access(f, W_OK | W_OK))


// Many of them are useless without our trace info getter
#include "basic/trace_info.h"


#endif // PWX_PWXLIB_BASE_MACROS_H_INCLUDED
