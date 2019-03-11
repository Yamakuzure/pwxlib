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
  * History and change log are maintained in pwxlib.h
**/


#include "CThreadElementStore.h"
#include "pwxVElement.h"


namespace pwx {


/// @brief VElement default constructor.
VElement::VElement() noexcept
{ }

/// @brief VElement copy constructor.
VElement::VElement( const VElement& src ) noexcept
    :	base_t( src )
{ }


/// @brief ~VElement default destructor.
VElement::~VElement() noexcept {
    this->remove();
}


/** @brief disable thread safety
  *
  * This method disables all thread safety measures.
  *
  * <B>Warning</B>: It is completely unchecked whether the
  * element is used by more than one thread. If concurrent
  * threads work with this element while this method is
  * called, the outcome is unpredictable.
  */
void VElement::disable_thread_safety() noexcept {
    this->do_locking( false );
}


/** @brief enable thread safety
  *
  * This method enables all thread safety measures.
  */
void VElement::enable_thread_safety() noexcept {
    this->do_locking( true );
}


/** @brief mark this element as being inserted
  *
  * This method should be called by all deriving element classes
  * upon insertion to mark this element as being inserted.
  *
  * Additionally this method will store the given pointer to
  * the handling CThreadElementStore and invalidate itself there
  * upon removal.
  *
  * @param[in] new_store pointer to the CThreadElementStore that might handle this element from now on.
**/
void VElement::insert( store_t* new_store ) noexcept {
    isRemoved.store( false, memOrdStore );
    if ( currStore )
        currStore->invalidate( this );
    currStore = new_store;
}


/** @brief return true if the element is a member of a container
  *
  * For this to work derived elements and containers using these
  * have to use insert()/remove() accordingly.
  *
  * @return true if the element is a member of any container
**/
bool VElement::inserted() const noexcept {
    return !isRemoved.load( memOrdLoad );
}


/** @brief return the current number of the element in a thread safe way
  * @return the current number of the element
**/
uint32_t VElement::nr() const noexcept {
    return eNr.load( memOrdLoad );
}


/** @brief Mark this element as being removed
  *
  * This method should be called by all deriving element classes
  * upon removal to mark this element as being removed.
  *
  * Additionally this method will call invalidate(this) on that
  * a set CThreadElementStore store if it has been set upon insertion.
**/
void VElement::remove() noexcept {
    isRemoved.store( true, memOrdStore );
    if ( currStore )
        currStore->invalidate( this );
}


/** @brief return true if the element is not a member of a container
  *
  * For this to work derived elements and containers using these
  * have to use insert()/remove() accordingly.
  *
  * @return true if the element is not a member of any container
**/
bool VElement::removed() const noexcept {
    return isRemoved.load( memOrdLoad );
}


/// Copy nothing but CLockable values, number and CThreadElementStore must be set by containers.
VElement& VElement::operator=( const VElement& src ) noexcept {
    beThreadSafe( src.beThreadSafe() );
    return *this;
}


} // namespace pwx
