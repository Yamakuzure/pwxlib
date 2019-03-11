#pragma once
#ifndef PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED
#define PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED 1

/** @file CLockable.h
  *
  * @brief Declaration of a general base class to add locking mechanisms to classes derived from CLockable.
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


#ifndef PWX_USE_FLAGSPIN
#  include <mutex>
#endif // PWX_USE_FLAGSPIN

#include "pwx_compiler.h"
#include "pwx_types.h"


namespace pwx {


/** @class CLockable
  *
  * @brief Base class to make objects lockable via atomic_flag and lock counting.
  *
  * Any class that is derived from this class gains the following methods:
  * <UL>
  * <LI>beThreadSafe() - alias for <I>is_locking()</I>.</LI>
  * <LI>beThreadSafe(bool) - alias for <I>do_locking(bool)</I>.</LI>
  * <LI>clear_locks() - remove all locks for the current thread.</LI>
  * <LI>do_locking(bool) - if set to false, no real locking is done.
  * This is useful if you use anything derived from CLockable in a single
  * threaded environment. The default is to do locking.</LI>
  * <LI>is_locked() - returns true if the object is currently locked.</LI>
  * <LI>is_locking() - returns true if the locking mechanism is turned on.</LI>
  * <LI>lock() - acquire lock for the current thread.</LI>
  * <LI>lock_count() - return the number of locks the current thread has.</LI>
  * <LI>try_lock() - try to acquire lock for the current thread and return true on success.</LI>
  * <LI>unlock() - release lock for the current thread if it holds one.</LI>
  * </UL>
  *
  * If the owning thread destroys the CLockable instance, the destructor will
  * unlock completely before going away. If another thread waits for a lock in
  * the meantime, or if the destroying thread is not the lock owner, the
  * behavior is undefined.
  *
  * This class implements a recursive behavior. Every call to lock() by the
  * current lock owner is counted. To unlock, the same amount of calls of the
  * unlock() method is required.<BR/>
  * However, there are two methods that might help out of tight spots, enabling
  * otherwise impossible techniques: The method clear_locks() will completely
  * unlock, no matter how often lock() was called. Further a thread can ask the
  * object with lock_count() how many locks it currently holds.<BR />
  * Please keep in mind, however, that your design might be flawed if you find
  * yourself in a situation in which you really need either method.
  *
  * <I>Enabling/Disabling locking</I> : If you want to switch to a non-locking
  * mode, this will require the switching thread to get a lock first. This way
  * you will not leave your object in a state where turning locking back on
  * suddenly leaves you with a lock from a long-gone thread.<BR/>
  * Rule of thumb: Do only turn off locking directly after instanciating your
  * object and before any other thread might have accessed it!

  * <B>Important</B>: It is strongly recommended that you use std::lock_guard
  * or std::unique_lock to do the locking of any object derived from
  * pwx::CLockable. You can use PWX_LOCK_GUARD(pointer) and
  * PWX_DOUBLE_LOCK_GUARD(ptrA, ptrB) to do this rather simply. They
  * are defined in pwx/general/pwx_macros.h.
  *
  * <I>atomic_flag spinlock</I> versus <I>mutex</I>:<BR/>
  * By default CLockable is compiled with PWX_USE_FLAGSPIN defined. This can
  * be controlled by setting USE_SPINLOCK in the main Makefile to either YES
  * or NO.<BR/>
  * If you think using the full std::mutex for doing the locking then you can
  * set this value to NO and compare the outcome of the speed tests of the
  * testlib program.
**/
class PWX_API CLockable {
  public:

    /* ===============================================
     * === Public Types                            ===
     * ===============================================
    */

#ifndef PWX_USE_FLAGSPIN
    typedef std::mutex lock_t; //!< Use standard mutex if no spinlocks are used.
#endif // !PWX_USE_FLAGSPIN


    /* ===============================================
     * === Public Constructors and destructor      ===
     * ===============================================
    */

    explicit CLockable() noexcept;
    CLockable ( const CLockable& src ) noexcept;
    virtual ~CLockable() noexcept;


    /* ===============================================
     * === Public methods                          ===
     * ===============================================
    */

    bool     beThreadSafe()      const noexcept PWX_WARNUNUSED; //!< true if thread safety is turned on
    void     beThreadSafe( bool doLock ) noexcept;              //!< set thread safety to @a doLock
    bool     clear_locks ()            noexcept;                //!< remove all locks
    bool     destroyed   ()      const noexcept PWX_WARNUNUSED; //!< if true the object will no longer lock
    void     do_locking  ( bool doLock ) noexcept;              //!< set thread safety to @a doLock
    bool     is_locked   ()      const noexcept PWX_WARNUNUSED; //!< return true if this object is locked
    bool     is_locking  ()      const noexcept PWX_WARNUNUSED; //!< true if thread safety is turned on
    void     lock        ()            noexcept;                //!< lock this object
    uint32_t lock_count  ()      const noexcept PWX_WARNUNUSED; //!< number of locks this thread holds on this object
    bool     try_lock    ()            noexcept PWX_WARNUNUSED; //!< try to lock and return at once
    void     unlock      ()            noexcept;                //!< unlock this object


    /* ===============================================
     * === Public operators                        ===
     * ===============================================
    */

    CLockable& operator= ( const CLockable& src ) noexcept;


  protected:

    /* ===============================================
     * === Protected members                       ===
     * ===============================================
    */

    mutable
    abool_t isDestroyed  = ATOMIC_VAR_INIT( false ); //!< Should be set to true by the destructors of deriving classes.
    mord_t  memOrdLoad   = PWX_MEMORDER_ACQUIRE;   //!< to be used with atomic::load()
    mord_t  memOrdStore  = PWX_MEMORDER_RELEASE;   //!< to be used with atomic::store()


// If the DEBUG_LOCK_STATE is enabled, the members need to be accessible from
// the callers, which must be derived from CLockable anyway.
#ifdef PWX_THREADDEBUG
  public:
#else
  private:
#endif // PWX_THREADDEBUG

    /* ===============================================
     * === Private members                         ===
     * ===============================================
    */

    abool_t CL_Do_Locking = ATOMIC_VAR_INIT( true ); //!< If set to false with do_locking(false), no real locking is done.
    abool_t CL_Is_Locked  = ATOMIC_VAR_INIT( false ); //!< Set to true if a lock is imposed, atomic_flag can't do it.

#ifdef PWX_USE_FLAGSPIN
    aflag_t CL_Lock       = ATOMIC_FLAG_INIT;        //!< Instead of a costly mutex atomic_flag spinlocks are used.
#else
    lock_t CL_Lock;                                  //!< Use standard mutex to handle locking
#endif // PWX_USE_FLAGSPIN

    aui32_t CL_Lock_Count = ATOMIC_VAR_INIT( 0 );    //!< How many times the current thread has locked.
    asize_t CL_Thread_ID  = ATOMIC_VAR_INIT( 0 );    //!< The owning thread of a lock
}; // class CLockable


/* ===============================================================
 * === Helper functions to work with CLockable derived objects ===
 * ===============================================================
 */

bool PWX_API are_locked( const CLockable* objA, const CLockable* objB )                        noexcept PWX_WARNUNUSED;
bool PWX_API are_locked( const CLockable* objA, const CLockable* objB, const CLockable* objC ) noexcept PWX_WARNUNUSED;
bool PWX_API try_locks ( const CLockable* objA, const CLockable* objB )                        noexcept PWX_WARNUNUSED;
bool PWX_API try_locks ( const CLockable* objA, const CLockable* objB, const CLockable* objC ) noexcept PWX_WARNUNUSED;
bool PWX_API unlock_all( const CLockable* objA, const CLockable* objB )                        noexcept;
bool PWX_API unlock_all( const CLockable* objA, const CLockable* objB, const CLockable* objC ) noexcept;

// Note: A RAII-based class that uses these helpers is pwx::CLockGuard in pwx/types/pwxCLockGuard.h


} // namespace pwx

#endif // PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED

