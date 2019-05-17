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

#include "arg_handler/CArgCallback.h"
#include "basic/CException.h"
#include "basic/pwx_macros.h"


/// @namespace pwx
namespace pwx {


CArgCallback::CArgCallback( char const* arg_short, char const* arg_long,
                            void ( *arg_cb )( char const*, char const* ),
                            char const* arg_desc, char const* param_name ) noexcept
	: VArgTargetBase( arg_short, arg_long, ATT_CB, arg_desc, param_name )
	, cb( arg_cb ) {
	/* nothing to do here */
}


CArgCallback::~CArgCallback() noexcept {
	/* nothing to do here */
}


eArgErrorNumber CArgCallback::process( char const* param ) {
	if ( cb ) {
		try {
			cb( aLong.size() ? aLong.c_str() : aShort.c_str(), param );
			this->gotParameter = true;
		}
		PWX_THROW_STD_FURTHER( "ArgCbException", "" )
		catch ( ... )
			PWX_THROW( "ArgCbException", "Unknown exception", "" )
		}

	return AEN_OK;
}

} // namespace pwx
