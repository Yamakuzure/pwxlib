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


#include <thread>

#include "pwxCLockable.h"
#include "pwx_debug.h"
#include "pwx_macros.h"


namespace pwx {


/* -----------------------------------------------------------------------------------------------------
 * --- Some notes about the private members:                                                         ---
 * --- CL_Do_Locking : This is a value that will most probably never change. The general use case to ---
 * ---                 touch it would be disabling it right after object creation. It is therefore   ---
 * ---                 save to load relaxed in any case. However, a change must be visible at once,  ---
 * ---                 so any change must be in release memory order.                                ---
 * --- CL_Is_Locked    This is set by lock() and try_lock() to true, and false by unlock(). This is  ---
 * ---                 of no interest to the internal methods. But any thread might check this value ---
 * ---                 from anywhere anytime. It is therefore inevitable to store in release and to  ---
 * ---                 load in acquire memory order.                                                 ---
 * --- CL_Lock         The lock, if using a spinlock, must be cleared in release memory order to be  ---
 * ---                 sure that a waiting thread does not waste a cycle by a superfluous yield().   ---
 * --- CL_Lock_Count   This is a value that is only used by the currently owning thread. There is no ---
 * ---                 reason why any access shouldn't be in relaxed memory order.                   ---
 * --- CL_Thread_ID    The same applies for the stored thread id. Whether a thread sets it to its own---
 * ---                 id during locking, or sets it to 0x0 during unlocking, it will always be      ---
 * ---                 different to not owning threads.                                              ---
 * --- About memOrdLoad and memOrdStore:                                                             ---
 * --- As the memory order is an enum, that can be used directly, both should not be used inside the ---
 * --- implementation, unless it would require to question CL_Do_Locking otherwise. It does mean     ---
 * --- nothing for the performance, but for clarity.                                                 ---
 * ---------------------------------------------------------------------------------------------------*/


/** @brief Default ctor
  */
CLockable::CLockable() noexcept
{ /* nothing to do here */ }


/** @brief Copy ctor
  *
  * All objects have their private locking.
  * Only the state whether to actually do the locking is copied.
  */
CLockable::CLockable ( const CLockable& src ) noexcept
    :	memOrdLoad( src.memOrdLoad ),
      memOrdStore( src.memOrdStore ),
      CL_Do_Locking( ATOMIC_VAR_INIT( src.CL_Do_Locking.load( PWX_MEMORDER_RELAXED ) ) )
{ /* --- nothing to do here. ---*/ }



/** @brief ~CLockable
  *
  * The default dtor will try to unlock a held mutex until it succeeds.
  */
CLockable::~CLockable() noexcept {
    DEBUG_LOCK_STATE( "~CLockable", this, this )
    isDestroyed.store( true, memOrdStore );
#ifdef PWX_USE_FLAGSPIN
    // Simply move the id to this thread:
    CL_Thread_ID.store( CURRENT_THREAD_ID, PWX_MEMORDER_RELAXED );
#else
    // Otherwise we have to wait for a real lock.
    lock();
#endif // PWX_USE_FLAGSPIN
    clear_locks();
    // the return value is unimportant, we can't do
    // anything about it in the middle of a dtor anyway.
}


/** @brief Assignment operator
  *
  * All objects have their private locking.
  * Only the state whether to actually do the locking is copied.
  */
CLockable& CLockable::operator= ( const CLockable& src ) noexcept {
    do_locking( src.CL_Do_Locking.load( PWX_MEMORDER_RELAXED ) );
    return *this;
}


// ======================================
// === Public methods implementation ====
// ======================================


/// @brief return true if thread safety mode is turned on
bool CLockable::beThreadSafe() const noexcept {
    return CL_Do_Locking.load( PWX_MEMORDER_RELAXED );
}


/// @brief set thread safety mode to @a doLock
/// This is just an alias for do_locking().
void CLockable::beThreadSafe( bool doLock ) noexcept {
    this->do_locking( doLock );
}


/** @brief clear all locks from this thread.
  *
  * If this thread is the current owner of the lock,
  * and if there are locks in place, they are all cleared.
  *
  * If this thread is not the owner, the method simply returns false.
  *
  * @return true if this thread is the owner and all locks could be cleared.
**/
bool CLockable::clear_locks() noexcept {
    if ( CL_Do_Locking.load( PWX_MEMORDER_RELAXED ) ) {
        if ( CURRENT_THREAD_ID == CL_Thread_ID.load( PWX_MEMORDER_RELAXED ) ) {
            THREAD_LOG( "base", "clear_locks(), Owner id 0x%08lx, %u locks [%s]",
                        CL_Thread_ID.load( PWX_MEMORDER_RELAXED ),
                        CL_Lock_Count.load( PWX_MEMORDER_RELAXED ),
                        CL_Is_Locked.load( PWX_MEMORDER_ACQUIRE ) ? "locked" : "not locked" )
            CL_Lock_Count.store( 0, PWX_MEMORDER_RELAXED );
            CL_Thread_ID.store( 0, PWX_MEMORDER_RELAXED );
            CL_Is_Locked.store( false, PWX_MEMORDER_RELAXED );
#ifdef PWX_USE_FLAGSPIN
            CL_Lock.clear( memOrdStore ); // This *must* be last!
#else
            CL_Lock.unlock();
#endif // PWX_USE_FLAGSPIN
        } // End of trying from within the right thread
        else
            return false; // Not from this thread!
    } // End of having to care about locking

    return true;
}


/** @brief returns true if the data was destroyed
  *
  * The destructor of TSingleElement and TDoubleElement
  * will try to get a final lock on the element when it
  * is destroyed. If another thread acquires a lock
  * between the data destruction and this final dtor lock,
  * destroyed() will return "true".
  *
  * @return true if the element is within its destruction process.
**/
bool CLockable::destroyed() const noexcept {
    return isDestroyed.load( memOrdLoad );
}


/** @brief switch whether to really use locking or not.
  *
  * With this method you can switch the locking mechanics on/off
  * for objects to be used in concurrency or strictly single threaded.
  * The default is to turn locking on.
  *
  * @param[in] doLock true to turn locking on, false to turn it off.
**/
void CLockable::do_locking( bool doLock ) noexcept {
    if ( doLock != CL_Do_Locking.load( PWX_MEMORDER_RELAXED ) ) {
        // If locking is enabled, change memory order now to strict
        if ( doLock ) {
            memOrdLoad  = PWX_MEMORDER_ACQUIRE;
            memOrdStore = PWX_MEMORDER_RELEASE;
        }

        // Switch now, so other threads stop locking
        // If this is a switch "on", it is finished anyway.
        CL_Do_Locking.store( doLock, PWX_MEMORDER_RELEASE );

        if ( !doLock ) {
            /* If this is not locked by the calling thread, it
             * is either not locked or locked by another thread.
             * In any case before disabling locking, this very
             * thread must be the exclusive user.
             */
            if ( CL_Thread_ID.load( PWX_MEMORDER_RELAXED ) != CURRENT_THREAD_ID ) {
                /* Sorry for the code doubling, but lock() would listen
                 * to CL_Do_Locking and that has to be false by now.
                 */
                std::this_thread::yield(); // to be sure this thread is last
#ifdef PWX_USE_FLAGSPIN
                while ( CL_Lock.test_and_set() ) {
# ifdef PWX_USE_FLAGSPIN_YIELD
                    std::this_thread::yield();
# endif // PWX_USE_FLAGSPIN_YIELD
                }
#else
                CL_Lock.lock();
#endif // PWX_USE_FLAGSPIN
            } // end of having to gain the lock

            // Nuke all data:
            CL_Thread_ID.store( 0, PWX_MEMORDER_RELAXED );
            CL_Lock_Count.store( 0, PWX_MEMORDER_RELAXED );
#ifdef PWX_USE_FLAGSPIN
            // Note: Here it is in order to clear relaxed, as no
            //       other thread should be waitng right now.
            CL_Lock.clear( PWX_MEMORDER_RELAXED );
#else
            CL_Lock.unlock();
#endif // PWX_USE_FLAGSPIN
            CL_Is_Locked.store( false, PWX_MEMORDER_RELEASE );
            // The memory order is relaxed last
            memOrdLoad  = PWX_MEMORDER_RELAXED;
            memOrdStore = PWX_MEMORDER_RELAXED;
        } // End of switching locking off
    } // End of having a change
}


/// @brief return true if this object is currently locked
bool CLockable::is_locked() const noexcept {
    return CL_Is_Locked.load( memOrdLoad );
}


/// @brief return true if the locking is turned on.
bool CLockable::is_locking() const noexcept {
    return CL_Do_Locking.load( PWX_MEMORDER_RELAXED );
}


/** @brief lock
  *
  * Lock this object for the current thread if locking is enabled.
  */
void CLockable::lock() noexcept {
    // return at once if this object is in destruction
    if ( isDestroyed.load( memOrdLoad ) )
        return;

    if ( CL_Do_Locking.load( PWX_MEMORDER_RELAXED ) ) {
        size_t ctid = CURRENT_THREAD_ID;
        THREAD_LOG( "base", "lock(), Owner id 0x%08lx, %u locks [%s]",
                    ctid, CL_Lock_Count.load( PWX_MEMORDER_RELAXED ),
                    CL_Is_Locked.load( PWX_MEMORDER_ACQUIRE ) ? "locked" : "not locked" )

        // For both the spinlock and the mutex an action
        // is only taken if this object is not already
        // locked by this thread
        if ( ctid != CL_Thread_ID.load( PWX_MEMORDER_RELAXED ) ) {
#ifdef PWX_USE_FLAGSPIN
            while ( CL_Lock.test_and_set() ) {
# ifdef PWX_USE_FLAGSPIN_YIELD
                std::this_thread::yield();
# endif // PWX_USE_FLAGSPIN_YIELD
            }
#else
            CL_Lock.lock();
#endif // PWX_USE_FLAGSPIN

            // Got it now, so note it:
            CL_Is_Locked.store( true, PWX_MEMORDER_RELEASE );
            CL_Thread_ID.store( ctid, PWX_MEMORDER_RELAXED );
            CL_Lock_Count.store( 1, PWX_MEMORDER_RELAXED );
        } else
            // If this thread already has a lock, the call is just counted
            CL_Lock_Count.fetch_add( 1, PWX_MEMORDER_RELAXED );
    } // End of doing locking
}


/** @brief return the number of locks on this object *this* thread has
  * @return the number of current locks held by the calling thread
**/
uint32_t CLockable::lock_count() const noexcept {
    if ( CURRENT_THREAD_ID == CL_Thread_ID.load( PWX_MEMORDER_RELAXED ) )
        return CL_Lock_Count.load( PWX_MEMORDER_RELAXED );
    return 0;
}


/** @brief try_lock
  *
  * Try to lock this object.
  *
  * @return true if the object could be locked, false otherwise.
  */
bool CLockable::try_lock() noexcept {
    // return at once if this object is in destruction
    if ( isDestroyed.load( memOrdLoad ) )
        return false;

    if ( CL_Do_Locking.load( PWX_MEMORDER_RELAXED ) ) {
        size_t ctid = CURRENT_THREAD_ID;
        THREAD_LOG( "base", "try_lock(), Owner id 0x%08lx, %u locks [%s]",
                    CL_Thread_ID.load( PWX_MEMORDER_RELAXED ),
                    CL_Lock_Count.load( PWX_MEMORDER_RELAXED ),
                    CL_Is_Locked.load( PWX_MEMORDER_ACQUIRE ) ? "locked" : "not locked" )

        // Same as with locking: Only try if this thread does
        // not already own the lock
        if ( ctid != CL_Thread_ID.load( PWX_MEMORDER_RELAXED ) ) {
#ifdef PWX_USE_FLAGSPIN
            if ( !CL_Lock.test_and_set() ) {
#else
            if ( CL_Lock.try_lock() ) {
#endif // PWX_USE_FLAGSPIN
                // Got it now, so note it:
                CL_Is_Locked.store( true, PWX_MEMORDER_RELEASE );
                CL_Thread_ID.store( ctid, PWX_MEMORDER_RELAXED );
                CL_Lock_Count.store( 1, PWX_MEMORDER_RELAXED );
                return true;
            }
            return false; // Nope, and only condition for a no-no.
        } // end of really having to try
    }

    // return true otherwise, we are fine.
    return true;
}


/** @brief unlock
  *
  * If locking is disabled or if the current thread does not hold
  * the lock, nothing happens. Otherwise the last lock is released.
  */
void CLockable::unlock() noexcept {
    if ( CL_Do_Locking.load( PWX_MEMORDER_RELAXED )
            && ( CURRENT_THREAD_ID == CL_Thread_ID.load( PWX_MEMORDER_RELAXED ) ) ) {
        THREAD_LOG( "base", "unlock(), Owner id 0x%08lx, %u locks [%s]",
                    CL_Thread_ID.load( PWX_MEMORDER_RELAXED ),
                    CL_Lock_Count.load( PWX_MEMORDER_RELAXED ),
                    CL_Is_Locked.load( PWX_MEMORDER_ACQUIRE ) ? "locked" : "not locked" )

        if ( 1 == CL_Lock_Count.fetch_sub( 1, PWX_MEMORDER_RELAXED ) ) {
            // The lock will go away now:
            CL_Thread_ID.store( 0, PWX_MEMORDER_RELAXED );
            CL_Is_Locked.store( false, PWX_MEMORDER_RELAXED );
#ifdef PWX_USE_FLAGSPIN
            CL_Lock.clear( PWX_MEMORDER_RELEASE );
#else
            CL_Lock.unlock();
#endif // PWX_USE_FLAGSPIN
        } // end of reducing the lock count
    }
}


/* ===============================================================
 * === Helper functions to work with CLockable derived objects ===
 * ===============================================================
 */

/** @brief return true if two given objects are both locked
  *
  * This function returns true if both given objects are currently
  * locked by the calling thread. If at least one is not locked,
  * the function returns false.
  *
  * This function can handle nullptr arguments, assuming nullptr to
  * be locked; It can't be manipulated anyway.
  *
  * @param[in] objA the first object to check
  * @param[in] objB the second object to check
  * @return true if both are locked, false if at least one is not locked
**/
bool are_locked( const CLockable* objA, const CLockable* objB ) noexcept {
    bool isLockedA = objA ? objA->is_locked() : true;
    bool isLockedB = objB ? objB->is_locked() : true;
    return isLockedA && isLockedB;
}


/** @brief return true if three given objects are both locked
  *
  * This function returns true if all three given objects are
  * currently locked by the calling thread. If at least one is
  * not locked, the function returns false.
  *
  * This function can handle nullptr arguments, assuming nullptr to
  * be locked; It can't be manipulated anyway.
  *
  * @param[in] objA the first object to check
  * @param[in] objB the second object to check
  * @param[in] objC the third object to check
  * @return true if all three are locked, false if at least one is not locked
**/
bool are_locked( const CLockable* objA, const CLockable* objB, const CLockable* objC ) noexcept {
    bool isLockedA = objA ? objA->is_locked() : true;
    bool isLockedB = objB ? objB->is_locked() : true;
    bool isLockedC = objC ? objC->is_locked() : true;
    return isLockedA && isLockedB && isLockedC;
}


/** @brief try to lock two objects at once
  *
  * This function tries to lock two objects at once, returning
  * true if both could be locked. If any can not be locked, the
  * other is unlocked again if neccessary and false is returned.
  *
  * This function can handle nullptr arguments, assuming nullptr to
  * be locked; It can't be manipulated anyway.
  *
  * @param[in] objA the first object to lock
  * @param[in] objB the second object to lock
  * @return true if both could be locked, false if at least one lock failed
**/
bool try_locks( const CLockable* objA, const CLockable* objB ) noexcept {
    CLockable* xObjA = const_cast<CLockable*>( objA );
    CLockable* xObjB = const_cast<CLockable*>( objB );

    bool lockedA = objA ? false : true;
    bool lockedB = objB ? false : true;

    if ( !lockedA || !lockedB ) {
        if ( xObjA ) lockedA = xObjA->try_lock();
        if ( xObjB ) lockedB = xObjB->try_lock();

        if ( !lockedA || !lockedB ) {
            if ( xObjA && lockedA ) xObjA->unlock();
            if ( xObjB && lockedB ) xObjB->unlock();
        }
    }

    return lockedA && lockedB;
}


/** @brief try to lock three objects at once
  *
  * This function tries to lock three objects at once, returning
  * true if all three could be locked. If any can not be locked,
  * the others are unlocked again if neccessary and false is
  * returned.
  *
  * This function can handle nullptr arguments, assuming nullptr to
  * be locked; It can't be manipulated anyway.
  *
  * @param[in] objA the first object to lock
  * @param[in] objB the second object to lock
  * @param[in] objC the third object to lock
  * @return true if all three could be locked, false if at least one lock failed
**/
bool try_locks( const CLockable* objA, const CLockable* objB, const CLockable* objC ) noexcept {
    CLockable* xObjA = const_cast<CLockable*>( objA );
    CLockable* xObjB = const_cast<CLockable*>( objB );
    CLockable* xObjC = const_cast<CLockable*>( objC );

    bool lockedA = xObjA ? false : true;
    bool lockedB = xObjB ? false : true;
    bool lockedC = xObjC ? false : true;

    if ( !lockedA || !lockedB || !lockedC ) {
        if ( xObjA ) lockedA = xObjA->try_lock();
        if ( xObjB ) lockedB = xObjB->try_lock();
        if ( xObjC ) lockedC = xObjC->try_lock();

        if ( !lockedA || !lockedB || !lockedC ) {
            if ( xObjA && lockedA ) xObjA->unlock();
            if ( xObjB && lockedB ) xObjB->unlock();
            if ( xObjC && lockedC ) xObjC->unlock();
        }
    }

    return lockedA && lockedB && lockedC;
}


/** @brief unlock two objects if both are currently locked.
  *
  * This function unlocks two objects if both are currently
  * locked by the calling thread. If any is not locked, the
  * function does nothing and returns false.
  *
  * This function can handle nullptr arguments, assuming nullptr to
  * be successfully unlocked; It can't be manipulated anyway.
  *
  * @param[in] objA the first object to unlock
  * @param[in] objB the second object to unlock
  * @return true if both could be unlocked, false if at least one was not locked
**/
bool unlock_all( const CLockable* objA, const CLockable* objB ) noexcept {
    if ( are_locked( objA, objB ) ) {
        if ( objA ) const_cast<CLockable*>( objA )->unlock();
        if ( objB ) const_cast<CLockable*>( objB )->unlock();
        return true;
    }
    return false;
}


/** @brief unlock three objects if all are currently locked.
  *
  * This function unlocks three objects if all are currently
  * locked by the calling thread. If any is not locked, the
  * function does nothing and returns false.
  *
  * This function can handle nullptr arguments, assuming nullptr to
  * be successfully unlocked; It can't be manipulated anyway.
  *
  * @param[in] objA the first object to unlock
  * @param[in] objB the second object to unlock
  * @param[in] objB the third object to unlock
  * @return true if all three could be unlocked, false if at least one was not locked
**/
bool unlock_all( const CLockable* objA, const CLockable* objB, const CLockable* objC ) noexcept {
    if ( are_locked( objA, objB, objC ) ) {
        if ( objA ) const_cast<CLockable*>( objA )->unlock();
        if ( objB ) const_cast<CLockable*>( objB )->unlock();
        if ( objC ) const_cast<CLockable*>( objC )->unlock();
        return true;
    }
    return false;
}


} // namespace pwx
