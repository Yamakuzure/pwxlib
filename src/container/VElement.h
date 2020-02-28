#ifndef PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED
#define PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED 1
#pragma once

/** @file VElement.h
  *
  * @brief Virtual base class for all container elements.
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

#include "basic/CLockable.h"
#include "basic/pwx_types.h"


/// @namespace pwx
namespace pwx {


/** @namespace private_
  * @internal
**/
namespace private_ {
/// Forward CThreadElementStore, there is a circular dependency.
class CThreadElementStore;
}


/** @class VElement
  *
  * @brief Virtual base class for all container elements.
  *
  * This class is strictly virtual. ALl element templates have
  * to inherit public from this base class.
  *
  * Notes:
  * + eNr does not need a lock, it is atomic.
  * + eNr needs no const_cast, it is mutable.
**/
class PWX_API VElement : public CLockable {
public:

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef CLockable                     base_t;  //!< Base type of VElement
	typedef VElement                      elem_t;  //!< Element type, which is VElement itself
	typedef private_::CThreadElementStore store_t; //!< Storage type for thread ids


	/* ===============================================
	 * === Public constructors and destructors     ===
	 * ===============================================
	*/

	explicit VElement() noexcept;              //!< @brief The default constructor, does nothing
	VElement ( const VElement& src ) noexcept; //!< @brief The copy constructor only delegates to CLockable
	virtual ~VElement() noexcept;              //!< @brief The destructor removes this from wherever it is in.


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	/** @brief disable thread safety
	  *
	  * This method disables all thread safety measures.
	  *
	  * <B>Warning</B>: It is completely unchecked whether the
	  * element is used by more than one thread. If concurrent
	  * threads work with this element while this method is
	  * called, the outcome is unpredictable.
	  */
	virtual void disable_thread_safety() noexcept;


	/** @brief enable thread safety
	  *
	  * This method enables all thread safety measures.
	  */
	virtual void enable_thread_safety() noexcept;


	/** @brief mark this element as being inserted
	  *
	  * This method should be called by all deriving element classes
	  * upon insertion to mark this element as being inserted.
	  *
	  * Additionally this method will store the given pointer to
	  * the handling CThreadElementStore and will invalidate itself
	  * there upon removal.
	  *
	  * @param[in] new_store pointer to the CThreadElementStore that might handle this element from now on.
	**/
	virtual void insert( store_t* new_store ) noexcept;


	/** @brief return true if the element is a member of a container
	  *
	  * For this to work derived elements and containers using these
	  * have to use `insert()` / `remove()` accordingly.
	  *
	  * @return true if the element is a member of any container
	**/
	bool inserted() const noexcept PWX_WARNUNUSED;


	/** @brief return the current number of the element in a thread safe way
	  * @return the current number of the element
	**/
	uint32_t nr() const noexcept PWX_WARNUNUSED;


	/** @brief Set the current number of the element in a thread safe way
	  * @param[in] nr_ The new number
	  * @return The previous number
	**/
	uint32_t nr( uint32_t const nr_ ) const noexcept;


	/** @brief Mark this element as being removed
	  *
	  * This method should be called by all deriving element classes
	  * upon removal to mark this element as being removed.
	  *
	  * Additionally this method will call invalidate(this) on that
	  * a set CThreadElementStore store if it has been set upon insertion.
	**/
	virtual void remove() noexcept;


	/** @brief return true if the element is not a member of a container
	  *
	  * For this to work derived elements and containers using these
	  * have to use insert()/remove() accordingly.
	  *
	  * @return true if the element is not a member of any container
	**/
	bool removed() const noexcept PWX_WARNUNUSED;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief The copy operator only delegates to CLockable.
	  *
	  * Number and CThreadElementStore must be set by containers.
	**/
	virtual VElement& operator=( const VElement& src ) noexcept;


protected:

	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	 */

	using base_t::isDestroyed;
	using base_t::memOrdLoad;
	using base_t::memOrdStore;

	mutable
	aui32_t eNr = ATOMIC_VAR_INIT( 0 ); //!< Number of the element


private:

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	 */

	store_t* currStore = nullptr;                //!< If set by a container the element will invalidate itself upon removal.
	mutable
	abool_t  isRemoved = ATOMIC_VAR_INIT( true ); //!< Set to true by ctor and remove*(), set to false by insert*() methods.

}; // class VContainer


} // namespace pwx


#endif // PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED
