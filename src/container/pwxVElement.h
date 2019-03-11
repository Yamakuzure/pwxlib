#pragma once
#ifndef PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED
#define PWX_LIBPWX_BASE_VELEMENT_H_INCLUDED 1

/** @file pwxVElement.h
  *
  * @brief Virtual base class for all container elements.
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


#include "CLockable.h"
#include "CLockGuard.h"
#include "pwx_macros.h"
#include "TVarDeleter.h"


namespace pwx {


// Forward CThreadElementStore, there is a circular dependency.
namespace private_ {
class CThreadElementStore;
}


/** @class VElement
  *
  * @brief Virtual base class for all container elements.
  *
  * This class is strictly virtual. ALl element templates have
  * to inherit public from this base class.
  *
  * Notes: <UL>
  * <LI>eNr does not need a lock, it is atomic.</LI>
  * <LI>eNr needs no const_cast, it is mutable.</LI>
  * </UL>
**/
class PWX_API VElement : public CLockable {
  public:

    /* ===============================================
     * === Public types                            ===
     * ===============================================
    */

    typedef CLockable                     base_t;
    typedef VElement                      elem_t;
    typedef private_::CThreadElementStore store_t;


    /* ===============================================
     * === Public constructors and destructors     ===
     * ===============================================
    */

    explicit VElement() noexcept;
    VElement ( const VElement& src ) noexcept;
    virtual ~VElement() noexcept;


    /* ===============================================
     * === Public methods                          ===
     * ===============================================
    */

    virtual void     disable_thread_safety()    noexcept;                //!< turn off locking
    virtual void     enable_thread_safety()     noexcept;                //!< turn on locking
    virtual void     insert( store_t* new_store ) noexcept;              //!< mark as inserted
    bool     inserted()           const noexcept PWX_WARNUNUSED; //!< return true if marked as inserted
    uint32_t nr()                 const noexcept PWX_WARNUNUSED; //!< return current number
    virtual void     remove()                   noexcept;                //!< mark as removed
    bool     removed()            const noexcept PWX_WARNUNUSED; //!< return true if marked as removed


    /* ===============================================
     * === Public operators                        ===
     * ===============================================
    */

    virtual VElement& operator=( const VElement& src ) noexcept;


    /* ===============================================
     * === Public members                          ===
     * ===============================================
    */

    mutable
    aui32_t eNr = ATOMIC_VAR_INIT( 0 ); //!< Number of the element


  protected:

    /* ===============================================
     * === Protected members                       ===
     * ===============================================
     */

    using base_t::isDestroyed;
    using base_t::memOrdLoad;
    using base_t::memOrdStore;


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
