/** @file
  * This file is part of the PrydeWorX Library (pwxLib).
  * (c) 2007 - 2020 PrydeWorX
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
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "arg_handler/VArgTargetBase.h"
#include "basic/CException.h"
#include "basic/string_utils.h"


/// @namespace pwx
namespace pwx {


VArgTargetBase::VArgTargetBase( char const* short_, char const* long_,
                                eArgTargetType type_, eArgType set_,
                                arg_cb_t cb_,
                                char const* desc_, char const* name_ ) noexcept
	: arg_short   ( strempty(short_) )
	, arg_long    ( strempty(long_ ) )
	, description ( strempty(desc_ ) )
	, param_name  ( strempty(name_ ) )
	, arg_callback( cb_   )
	, arg_type    ( type_ )
	, set_type    ( set_  )
{ /* nothing to do here */ }


VArgTargetBase::~VArgTargetBase() noexcept {
	arg_callback = nullptr;
	arg_short.clear();
	arg_long.clear();
	description.clear();
	param_name.clear();
}


VArgTargetBase::VArgTargetBase( VArgTargetBase const& rhs ) noexcept
	: arg_short   ( rhs.arg_short    )
	, arg_long    ( rhs.arg_long     )
	, description ( rhs.description  )
	, param_name  ( rhs.param_name   )
	, arg_callback( rhs.arg_callback )
	, arg_type    ( rhs.arg_type     )
	, set_type    ( rhs.set_type     )
{ /* nothing to do here */ }


VArgTargetBase::VArgTargetBase( VArgTargetBase&& rhs ) noexcept
	: arg_short   ( rhs.arg_short    )
	, arg_long    ( rhs.arg_long     )
	, description ( rhs.description  )
	, param_name  ( rhs.param_name   )
	, arg_callback( rhs.arg_callback )
	, arg_type    ( rhs.arg_type     )
	, set_type    ( rhs.set_type     ) {
	rhs.arg_callback = nullptr;
	rhs.arg_long.clear();
	rhs.arg_short.clear();
	rhs.description.clear();
	rhs.param_name.clear();
}


bool VArgTargetBase::needsParameter() const noexcept {
	if ( ( ATT_ADD == arg_type )
	  || ( ATT_SUB == arg_type )
	  || ( ATT_SET == arg_type )
	  || ( ( ATT_CB  == arg_type ) && param_name.length() ) )
		return true;
	return false;
}

void VArgTargetBase::reset() noexcept {
	this->gotParameter = false;
	this->wasProcessed = false;
}

VArgTargetBase& VArgTargetBase::operator=( VArgTargetBase const& rhs ) noexcept {
	if (&rhs != this) {
		this->arg_callback = rhs.arg_callback;
		this->arg_long     = rhs.arg_long;
		this->arg_short    = rhs.arg_short;
		this->arg_type     = rhs.arg_type;
		this->description  = rhs.description;
		this->param_name   = rhs.param_name;
		this->set_type     = rhs.set_type;
	}
	return *this;
}


VArgTargetBase& VArgTargetBase::operator=( VArgTargetBase&& rhs ) noexcept {
	if (&rhs != this) {
		this->arg_callback = rhs.arg_callback; rhs.arg_callback = nullptr;
		this->arg_long     = rhs.arg_long;     rhs.arg_long.clear();
		this->arg_short    = rhs.arg_short;    rhs.arg_short.clear();
		this->arg_type     = rhs.arg_type;
		this->description  = rhs.description;  rhs.description.clear();
		this->param_name   = rhs.param_name;   rhs.param_name.clear();
		this->set_type     = rhs.set_type;
	}
	return *this;
}


eArgErrorNumber VArgTargetBase::process_cb( char const* param ) {
	if ( arg_callback ) {
		try {
			arg_callback( arg_long.size() ? arg_long.c_str() : arg_short.c_str(), strempty( param ) );
			if ( param && param[0] )
				this->gotParameter = true;
		}
		PWX_THROW_STD_FURTHER( "ArgCbException", "" )
		catch ( ... )
			PWX_THROW( "ArgCbException", "Unknown exception", "" )
		}

	return AEN_OK;
}


bool operator==( const VArgTargetBase& lhs, const VArgTargetBase& rhs ) noexcept {
	return ( ( lhs.arg_short.size() == rhs.arg_short.size() )
	      && ( lhs.arg_long.size()  == rhs.arg_long.size() )
	      && ( !lhs.arg_short.size() || ( lhs.arg_short == rhs.arg_short ) )
	      && ( !lhs.arg_long.size()  || ( lhs.arg_long  == rhs.arg_long ) ) ) ;
}


bool operator>( const VArgTargetBase& lhs, const VArgTargetBase& rhs ) noexcept {
	return ( ( lhs.arg_short.size() && ( !rhs.arg_short.size() || ( lhs.arg_short > rhs.arg_short ) ) )
	      || ( lhs.arg_long.size()  && ( !rhs.arg_long.size()  || ( lhs.arg_long  > rhs.arg_long  ) ) ) );
}


} // namespace pwx

