#ifndef PWX_LIBPWX_PWX_TYPES_CLOCKGUARD_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_CLOCKGUARD_H_INCLUDED

/** @file pwxCLockGuard.h
  *
  * @brief Simple Class to RAII-lock up to 3 objects derived from pwx::Clockable
  *
  * This class have been created to be able to RAII-autolock
  * up to three objects at once. This is helpful handling
  * doubly linked list based containers and elements.
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


namespace pwx {


/** @class CLockGuard
  * @brief Basic RAII lock guard to lock/unlock one, two or three objects within its ctor/dtor
  *
  * There are the following advantages when using this class instead of
  * doing locks directly:
  * <list=1><li>Locking is done in the ctor, unlocking in the dtor automatically.</li>
  * <li>The class is exception free and can handle null pointers.</li>
  * <li>The guard can be assigned or copied, making overlapping locks easy to do.</li>
  * <li>If it is not possible to wait for the destructor, the lock(s) can be unlocked
  * by resetting to nullptr.</li></list>
**/
class PWX_API CLockGuard {
  public:

    /* ===============================================
     * === Public constructors and destructors     ===
     * ===============================================
    */

    CLockGuard( const CLockable* objA ) noexcept;
    CLockGuard( const CLockable* objA, const CLockable* objB ) noexcept;
    CLockGuard( const CLockable* objA, const CLockable* objB, const CLockable* objC ) noexcept;
    CLockGuard( const CLockGuard& src ) noexcept;
    ~CLockGuard() noexcept;

    // No empty ctor
    CLockGuard() PWX_DELETE;


    /* ===============================================
     * === Public operators                        ===
     * ===============================================
    */

    CLockGuard& operator=( const CLockGuard& src ) noexcept;


    /* ===============================================
     * === Public Methods                          ===
     * ===============================================
    */

    void reset( const CLockable* objA ) noexcept;
    void reset( const CLockable* objA, const CLockable* objB ) noexcept;
    void reset( const CLockable* objA, const CLockable* objB, const CLockable* objC ) noexcept;


  private:

    /* ===============================================
     * === Private Members                         ===
     * ===============================================
    */

    CLockable* l_a = nullptr;
    CLockable* l_b = nullptr;
    CLockable* l_c = nullptr;
};


} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_CLOCKGUARD_H_INCLUDED

