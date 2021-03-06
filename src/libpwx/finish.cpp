/** @file
  * This file is part of the PrydeWorX Library (pwxLib).
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


#include "basic/mem_utils.h"
#include "basic/compiler.h"
#include "basic/debug.h"
#include "basic/macros.h"
#include "libpwx/finish.h"
#include "libpwx/worker_PAH.h"
#include "libpwx/worker_SCT.h"
#include "log/log.h"


/// @namespace pwx
namespace pwx {


void finish() noexcept {

	PAH.clearArgs();
	SCT.clearTables();

	// Let's see what the memory map has caught (if debugging is enabled)
	if ( !mem_map_report() )
		log_debug_error( "pwxLib Finish", "%s", "The Memory Map reported errors! Fix those ASAP!" );

	// Close log if any
	log_close();
}


} // namespace pwx
