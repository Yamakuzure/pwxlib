#ifndef PWX_LIBPWX_PWX_TYPES_CARGCALLBACK_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_CARGCALLBACK_H_INCLUDED

/** @file pwxCArgCallback.h
  *
  * @brief Declaration of the CArgCallback class
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
  * History and change log are maintained in pwxlib.h
**/


#include "pwxVArgTargetBase.h"


namespace pwx {


/** @class CArgCallback
  * @brief Definition of one command line argument using a callback function
  *
  * This is meant to be used with a callback function. If a target pointer
  * is to be used, use TArgTarget instead.
**/
class CArgCallback : public VArgTargetBase {
  public:
    explicit CArgCallback( const char* arg_short, const char* arg_long,
                           void ( *arg_cb )( const char*, const char* ),
                           const char* arg_desc, const char* param_name ) noexcept;

    virtual ~CArgCallback() noexcept;

    virtual eArgErrorNumber process( const char* param );

  private:

    // callback
    void ( *cb )( const char*, const char* );

};


} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_CARGCALLBACK_H_INCLUDED

