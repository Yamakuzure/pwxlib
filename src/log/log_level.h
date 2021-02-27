#ifndef PWXLIB_SRC_LOG_LOG_LEVEL_H_INCLUDED
#define PWXLIB_SRC_LOG_LOG_LEVEL_H_INCLUDED 1
#pragma once
/** @file log_level.h
  *
  * @brief Declaration of pwx::log_level_t
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


/// @namespace pwx
namespace pwx {

typedef enum {
	LOG_DEBUG = 0, //!< Log everything including debug level messages
	LOG_INFO,      //!< Informational messages that are only interesting in development
	LOG_STATUS,    //!< Status messages like progress and counts
	LOG_WARNING,   //!< Something is off, but does not interrupt operation
	LOG_ERROR,     //!< A non-critical error has been encountered, the program can proceed
	LOG_CRITICAL,  //!< A critical error has been encountered, the program must be terminated
	LOG_DISABLED   //!< A border enum to enable not logging anything at all
} log_level_t;


}


#endif // PWXLIB_SRC_LOG_LOG_LEVEL_H_INCLUDED

