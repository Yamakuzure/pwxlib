/** @file
  * This file is part of the PrydeWorX Library (pwxLib).
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


#include <atomic>

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "container/VElement.h"
#include "container/CThreadElementStore.h"


/// @namespace pwx
namespace pwx {


VElement::VElement() noexcept
{ }

VElement::VElement( const VElement& src ) noexcept
	:   base_t( src )
{ }


VElement::~VElement() noexcept {
	this->remove();
}


void VElement::disable_thread_safety() noexcept {
	this->do_locking( false );
}


void VElement::enable_thread_safety() noexcept {
	this->do_locking( true );
}


void VElement::insert( store_t* new_store ) noexcept {
	isRemoved.store( false, memOrdStore );
	if ( currStore )
		currStore->invalidate( this );
	currStore = new_store;
}


bool VElement::inserted() const noexcept {
	return !isRemoved.load( memOrdLoad );
}


uint32_t VElement::nr() const noexcept {
	return eNr.load( memOrdLoad );
}


uint32_t VElement::nr( uint32_t const nr_ ) const noexcept {
	return eNr.exchange( nr_, memOrdStore );
}


void VElement::remove() noexcept {
	isRemoved.store( true, memOrdStore );
	if ( currStore )
		currStore->invalidate( this );
}


bool VElement::removed() const noexcept {
	return isRemoved.load( memOrdLoad );
}


VElement& VElement::operator=( const VElement& src ) noexcept {
	beThreadSafe( src.beThreadSafe() );
	return *this;
}


} // namespace pwx
