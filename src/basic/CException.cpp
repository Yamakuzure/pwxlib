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

#include "basic/CException.h"
#include "basic/pwx_macros.h"


/// @namespace pwx
namespace pwx {

CException::CException ( char const* const name_, char const* const what_, char const* const where_,
                         char const* const func_, char const* const desc_ ) noexcept
        : std::exception()
        , txtName  ( strdup ( name_ ) )
        , txtWhat  ( strdup ( what_ ) )
        , txtWhere ( strdup ( where_ ) )
        , txtFunc  ( strdup ( func_ ) )
        , txtDesc  ( strdup ( desc_ ) )
        , txtTrace ( "Thrown from :\n\t" ) {
        PWX_TRY ( txtTrace += where_ )
        PWX_CATCH_AND_FORGET ( std::exception )
}


CException::CException ( const CException &src ) noexcept
        : std::exception ( src )
        , txtName  ( strdup ( src.txtName ) )
        , txtWhat  ( strdup ( src.txtWhat ) )
        , txtWhere ( strdup ( src.txtWhere ) )
        , txtFunc  ( strdup ( src.txtFunc ) )
        , txtDesc  ( strdup ( src.txtDesc ) )
        , txtTrace ( src.txtTrace )
{ /* nothing to be done here */ }


void CException::addToTrace ( char const* trace_ ) noexcept {
        if ( txtTrace.empty() ) {
                PWX_TRY ( txtTrace  = "Thrown from :\n\t"; txtTrace += txtWhere )
                PWX_CATCH_AND_FORGET ( std::exception )
        }
        PWX_TRY ( txtTrace += "\n\t"; txtTrace += trace_; )
        PWX_CATCH_AND_FORGET ( std::exception )
}


CException::~CException() noexcept {
        if ( txtName )  free( const_cast<char*>( txtName ) );
        if ( txtWhat )  free( const_cast<char*>( txtWhat ) );
        if ( txtWhere ) free( const_cast<char*>( txtWhere ) );
        if ( txtFunc )  free( const_cast<char*>( txtFunc ) );
        if ( txtDesc )  free( const_cast<char*>( txtDesc ) );
}


char const* CException::name() const noexcept {
        return txtName;
}


char const* CException::what() const noexcept {
        return txtWhat;
}


char const* CException::where() const noexcept {
        return txtWhere;
}


char const* CException::desc() const noexcept {
        return txtDesc;
}


char const* CException::pfunc() const noexcept {
        return txtFunc;
}


char const* CException::trace() const noexcept {
        return txtTrace.c_str();
}

} // namespace pwx
