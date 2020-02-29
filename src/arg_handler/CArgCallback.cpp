/** @file
  * This file is part of the PrydeWorX Library (pwxLib).
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


#include <cstring>

#include "basic/pwx_compiler.h"
#include "basic/pwx_debug.h"
#include "basic/pwx_macros.h"

#include "arg_handler/CArgCallback.h"
#include "basic/CException.h"

/// @namespace pwx
namespace pwx {


CArgCallback::CArgCallback( char const* _short, char const* _long,
	                       arg_cb_t _cb,
	                       char const* _desc, char const* _name ) noexcept
	: VArgTargetBase( _short, _long, ATT_CB, AT_ZERO_OR_MANY, _cb, _desc, _name )
	, cb( _cb ) {
	/* nothing to do here */
}


CArgCallback::~CArgCallback() noexcept {
	/* nothing to do here */
}


eArgErrorNumber CArgCallback::process( char const* param ) {
	if ( cb ) {
		try {
			cb( arg_long.size() ? arg_long.c_str() : arg_short.c_str(), param );
			this->gotParameter = true;
		}
		PWX_THROW_STD_FURTHER( "ArgCbException", "" )
		catch ( ... )
			PWX_THROW( "ArgCbException", "Unknown exception", "" )
		}

	return AEN_OK;
}

} // namespace pwx
