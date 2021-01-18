#ifndef PWX_PWXLIB_SRC_LIBPWX_FINISH_H_INCLUDED
#define PWX_PWXLIB_SRC_LIBPWX_FINISH_H_INCLUDED 1
#pragma once


/** @file finish.h
  *
  * @brief Function to clean up on programs end and to clear up in debugging mode. (see pwx::finish())
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


#include "basic/pwx_compiler.h"


/// @namespace pwx
namespace pwx {


/** @brief De-initialize pwxLib and free all acquisitions
  *
  * Call this function last in your program. The best would be as the last statement before the final return
  * in your `main()`.
  *
  * Apart from freeing the needed resources acquired during the runtime of your program, the library also
  * cleans up all memory you allocated through the pwxLib memory functions that you forget to free.
  *
  * **Important**: This is not a garbage collector! The cleanup will issue *error*-messages, and you should
  *                fix your code asap! The cleanup is only done to avoid known memory leaks.
  *
  * **Also Important**: The memory allocation cleanup and reporting is only done in debugging mode, as it
  *                     needs quite some overhead, which would cripple programs needing lots of allocations.
**/
void PWX_API finish();


} // namespace pwx


#endif // PWX_PWXLIB_SRC_LIBPWX_FINISH_H_INCLUDED

