#ifndef PWX_LIBPWX_PWX_TYPES_EARGERRORNUMBER_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_EARGERRORNUMBER_H_INCLUDED 1
#pragma once

/** @file eArgErrorNumber.h
  *
  * @brief enum describing all valid error numbers for argument parsing
  *
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
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


#include "basic/pwx_compiler.h"


/// @namespace pwx
namespace pwx {


/** @enum eArgErrorNumber
  * @brief defines valid error numbers for argument parsing
**/
enum eArgErrorNumber {
	AEN_OK                  = 0,          //!< Everything in order
	AEN_ARGUMENT_UNKNOWN    = 0x00000001, //!< The found argument is not known
	AEN_PARAM_TYPE_MISMATCH = 0x00000002, //!< The type of the parameter doesn't match the target
	AEN_PARAMETER_MISSING   = 0x00000004, //!< An argument that needs a parameter got none to process
	AEN_PROCESSING_ERROR    = 0x00000008, //!< Set when target->process() threw an exception.
	AEN_MULTIPLE_SET_PARAM  = 0x00000010, //!< More than one parameter provided for a STT_ERROR set target
	AEN_MANDATORY_MISSING   = 0x00000020  //!< A mandatory argument was not found on the command line
};


} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_EARGERRORNUMBER_H_INCLUDED

