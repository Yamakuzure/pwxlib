#ifndef PWX_PWXLIB_PWX_FUNCTIONS_SET_FWD_H_INCLUDED
#define PWX_PWXLIB_PWX_FUNCTIONS_SET_FWD_H_INCLUDED 1
#pragma once

/** @file pwx_set_fwd.h
  *
  * @brief Forward declarations and function prototypes for pwx/container/TSet.h
  *
  * (c)  2007 - 2021 PrydeWorX
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

#ifndef PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED
# error "Do not include pwx_set_fwd.h, include pwx/container/TSet.h!"
#endif // Check for main file

#include "basic/compiler.h"

#include "container/TSet.h" // Make IDE-Parsers happy

namespace pwx {

// Forward TSet for prototype definitions
template<typename data_t> class TSet;

// --- Function Prototypes ---
template<typename data_t>
TSet<data_t>* set_difference  ( const TSet<data_t>* const lhs, const TSet<data_t>* const rhs ) PWX_API;
template<typename data_t>
TSet<data_t>  set_difference  ( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) PWX_API;
template<typename data_t>
TSet<data_t>* set_intersection( const TSet<data_t>* const lhs, const TSet<data_t>* const rhs ) PWX_API;
template<typename data_t>
TSet<data_t>  set_intersection( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) PWX_API;
template<typename data_t>
TSet<data_t>* set_union       ( const TSet<data_t>* const lhs, const TSet<data_t>* const rhs ) PWX_API;
template<typename data_t>
TSet<data_t>  set_union       ( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) PWX_API;

// --- operator prototypes ---
template<typename data_t>
bool          operator==( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) noexcept PWX_API;
template<typename data_t>
bool          operator!=( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) noexcept PWX_API;
template<typename data_t>
TSet<data_t>  operator+ ( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) PWX_API;
template<typename data_t>
TSet<data_t>  operator- ( const TSet<data_t>& lhs, const TSet<data_t>& rhs ) PWX_API;

} // namespace pwx
#endif // PWX_PWXLIB_PWX_FUNCTIONS_SET_FWD_H_INCLUDED

