#ifndef PWX_LIBPWX_PWX_TYPES_EARGSETTYPE_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_EARGSETTYPE_H_INCLUDED 1
#pragma once

/** @file eArgType.h
  *
  * @brief enum to describe what PAH should do when an ATT_SET target receives more than one parameter
  *
  * (c) 2007 - 2020 PrydeWorX
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


#include "basic/pwx_compiler.h"


/// @namespace pwx
namespace pwx {


/** @enum eArgType
  * @brief determine what to do when multiple parameters of one type are submitted.
**/
enum eArgType {
	AT_ZERO_OR_ONE  = 0, //!< The option is optional but must not be given more than once
	AT_ZERO_OR_MANY = 1, //!< The option is optional and can be given multiple times
	AT_EXACTLY_ONCE = 2, //!< The option is mandatory and must be given exactly once
	AT_ONE_OR_MANY  = 3  //!< The option is mandatory and can be given multiple times
};


} // namespace pwx


#endif // PWX_LIBPWX_PWX_TYPES_EARGSETTYPE_H_INCLUDED

