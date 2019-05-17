/** @file
  * This file is part of the PrydeWorX Library (pwxLib).
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


#include "arg_handler/VArgTargetBase.h"


/// @namespace pwx
namespace pwx {


VArgTargetBase::VArgTargetBase( char const* arg_short, char const* arg_long,
                                eArgTargetType arg_type,
                                char const* arg_desc, char const* param_name ) noexcept
	: aShort ( arg_short  ? arg_short  : "" )
	, aLong  ( arg_long   ? arg_long   : "" )
	, desc   ( arg_desc   ? arg_desc   : "" )
	, pName  ( param_name ? param_name : "" )
	, type   ( arg_type )
	, setType( STT_OVERWRITE )
{ /* nothing to do here */ }


VArgTargetBase::VArgTargetBase( char const* arg_short, char const* arg_long,
                                eArgSetType set_type,
                                char const* arg_desc, char const* param_name ) noexcept
	: VArgTargetBase( arg_short, arg_long, ATT_SET, arg_desc, param_name ) {
	setType = set_type;
}


VArgTargetBase::~VArgTargetBase() noexcept {
	aShort.clear();
	aLong.clear();
	desc.clear();
	pName.clear();
}


bool VArgTargetBase::hasParameter() const noexcept {
	return gotParameter;
}


bool VArgTargetBase::needsParameter() const noexcept {
	if ( ( ATT_ADD == type )
	                || ( ATT_SUB == type )
	                || ( ATT_SET == type )
	                || ( ( ATT_CB  == type ) && pName.length() ) )
		return true;
	return false;
}


bool operator==( const VArgTargetBase& lhs, const VArgTargetBase& rhs ) noexcept {
	return ( ( lhs.aShort.size() == rhs.aShort.size() )
	         && ( lhs.aLong.size()  == rhs.aLong.size() )
	         && ( !lhs.aShort.size() || ( lhs.aShort == rhs.aShort ) )
	         && ( !lhs.aLong.size()  || ( lhs.aLong == rhs.aLong ) ) ) ;
}


bool operator>( const VArgTargetBase& lhs, const VArgTargetBase& rhs ) noexcept {
	return ( ( lhs.aShort.size() && ( !rhs.aShort.size() || ( lhs.aShort > rhs.aShort ) ) )
	         || ( lhs.aLong.size()  && ( !rhs.aLong.size()  || ( lhs.aLong  > rhs.aLong ) ) ) );
}


} // namespace pwx

