#ifndef PWX_LIBPWX_PWX_TYPES_CARGCALLBACK_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_CARGCALLBACK_H_INCLUDED 1
#pragma once

/** @file CArgCallback.h
  *
  * @brief Declaration of the CArgCallback class
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

#include "arg_handler/VArgTargetBase.h"


/// @namespace pwx
namespace pwx {


/** @class CArgCallback
  * @brief Definition of one command line argument using a callback function
  *
  * This is meant to be used with a callback function. If a target pointer
  * is to be used, use TArgTarget instead.
**/
class CArgCallback : public VArgTargetBase {

public:

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
	explicit CArgCallback( char const* _short, char const* _long,
	                       arg_cb_t _cb,
	                       char const* _desc, char const* _name ) noexcept;

	/// @brief The destructor has nothing to do.
	virtual ~CArgCallback() noexcept;

	/** @brief process an argument parameter
	  *
	  * Simple method that calls the stored callback function with @a param.
	  *
	  * If no callback function was installed using the constructor,
	  * this method does silently nothing.
	  *
	  * @return AEN_OK if the callback function was set.
	**/
	virtual eArgErrorNumber process( char const* param );

private:

	void ( *cb )( char const*, char const* );   //!< callback function to use

};


} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_CARGCALLBACK_H_INCLUDED

