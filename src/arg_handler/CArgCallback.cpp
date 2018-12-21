/**
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2018 PrydeWorX
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
  * History and change log are maintained in pwx.h
**/


#include <cstring>

#include "pwxCArgCallback.h"
#include "pwxCException.h"
#include "pwx_macros.h"


namespace pwx {


/** brief default ctor
  *
  * No parameter check, the caller must ensure consistent
  * values that make the instance usable.
  *
  * @see pwx::CArgHandler::addArg()
  *
  * @param[in] arg_short Short argument like "-a" or "x".
  * @param[in] arg_long Long argument like "--foo" or "-bar".
  * @param[out] arg_cb Pointer to the callback function to use.
  * @param[in] arg_desc Help text for this argument.
  * @param[in] param_name Name shown in <> int the help text.
**/
CArgCallback::CArgCallback( const char* arg_short, const char* arg_long,
                            void ( *arg_cb )( const char*, const char* ),
                            const char* arg_desc, const char* param_name )
noexcept :
    VArgTargetBase( arg_short, arg_long, ATT_CB, arg_desc, param_name ),
    cb    ( arg_cb ) {
    /* nothing to do here */
}


/** @brief destructor
  * has nothing to do.
**/
CArgCallback::~CArgCallback() noexcept {
    /* nothing to do here */
}


/** @brief process an argument parameter
  *
  * Simple method that calls the stored callback function with @a param.
  *
  * If no callback function was installed using the constructor,
  * this method does silently nothing.
  *
  * @return AEN_OK if the callback function was set.
**/
eArgErrorNumber CArgCallback::process( const char* param ) {
    if ( cb ) {
        try {
            cb( aLong.size() ? aLong.c_str() : aShort.c_str(), param );
            this->gotParameter = true;
        }
        PWX_THROW_STD_FURTHER( "ArgCbException","" )
        catch ( ... ) {
            PWX_THROW( "ArgCbException", "Unknown exception", "" )
        }
    }

    return AEN_OK;
}

} // namespace pwx
