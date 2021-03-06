#ifndef PWX_LIBPWX_BASE_VCONTAINER_H_INCLUDED
#define PWX_LIBPWX_BASE_VCONTAINER_H_INCLUDED 1
#pragma once

/** @file VContainer.h
  *
  * @brief Virtual base class for all containers.
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


#include "basic/compiler.h"

#include "basic/CLockable.h"
#include "basic/types.h"


/// @namespace pwx
namespace pwx {


/** @class VContainer
  *
  * @brief Virtual base class for all containers.
  *
  * This class is strictly virtual. ALl container templates have
  * to inherit public from this base class.
  *
  * Notes:
  * + Both doRenumber and eCount are atomic and need no lock
  * + Both doRenumber and eCount are mutable and need no const_cast
**/
class PWX_API VContainer : public CLockable {
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef CLockable  base_t; //!< Base type of VContainer
	typedef VContainer list_t; //!< List type of VContainer


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	explicit VContainer() noexcept;                /// @brief The default constructor does nothing.
	VContainer ( const VContainer& src ) noexcept; /// @brief The copy constructor does a full renumbering.
	virtual ~VContainer() noexcept;                /// @brief The default destructor does nothing.


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	virtual void     clear()                 noexcept PWX_VIRTUAL_PURE; //!< Remove all elements
	virtual void     disable_thread_safety() noexcept PWX_VIRTUAL_PURE; //!< Turn off thread safety measures
	virtual bool     empty()           const noexcept PWX_VIRTUAL_PURE; //!< Return true if empty
	virtual void     enable_thread_safety()  noexcept PWX_VIRTUAL_PURE; //!< Turn on thread safety measures
	virtual uint32_t size()            const noexcept PWX_VIRTUAL_PURE; //!< Return number of elements


protected:

	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	mutable
	abool_t doRenumber = ATOMIC_VAR_INIT( false ); //!< If set to true, a renumbering is done before retrieving elements by index
	mutable
	aui32_t eCount     = ATOMIC_VAR_INIT( 0 );     //!< Current number of elements

	using base_t::memOrdLoad;
	using base_t::memOrdStore;
}; // class VContainer

} // namespace pwx

#endif // PWX_LIBPWX_BASE_VCONTAINER_H_INCLUDED

