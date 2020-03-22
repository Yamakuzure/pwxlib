#ifndef PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED
#define PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED 1
#pragma once

/** @file CLockable.h
  *
  * @brief Declaration of a general base class to add locking mechanisms to classes derived from CLockable.
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


#if !PWX_USE_FLAGSPIN
#  include <mutex>
#endif // PWX_USE_FLAGSPIN

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_types.h"

#include "basic/mem_utils.h"


/// @namespace pwx
namespace pwx {


/** @class CLockable PLockable <PLockable>
  *
  * @brief Base class to make objects lockable via atomic_flag and lock counting.
  *
  * Any class that is derived from this class gains the following methods:
  *
  * | Method               | Description                                                |
  * | -------------------- | ---------------------------------------------------------- |
  * | `beThreadSafe()`     | alias for `is_locking()`                                   |
  * | `beThreadSafe(bool)` | alias for `do_locking(bool)`                               |
  * | `clear_locks()`      | remove all locks for the current thread                    |
  * | `do_locking(bool)`   | if set to false, no real locking is done (1)               |
  * | `is_locked()`        | returns true if the object is currently locked             |
  * | `is_locking()`       | returns true if the locking mechanism is turned on         |
  * | `lock()`             | acquire lock for the current thread. (Blocking)            |
  * | `lock_count()`       | return the number of locks the current thread has          |
  * | `try_lock()`         | try to acquire lock for the current thread. (Not blocking) |
  * | `unlock()`           | release lock for the current thread if it holds one        |
  *
  * (1) This is useful if you use anything derived from `CLockable` in a single
  * threaded environment. The default is to do locking.
  *
  *
  * If the owning thread destroys the CLockable instance, the destructor will
  * unlock completely before going away. If another thread waits for a lock in
  * the meantime, or if the destroying thread is not the lock owner, the
  * behaviour is undefined.
  *
  * This class implements a recursive behaviour. Every call to lock() by the
  * current lock owner is counted. To unlock, the same amount of calls of the
  * unlock() method is required.
  * However, there are two methods that might help out of tight spots, enabling
  * otherwise impossible techniques: The method clear_locks() will completely
  * unlock, no matter how often lock() was called. Further a thread can ask the
  * object with lock_count() how many locks it currently holds.
  * Please keep in mind, however, that your design might be flawed if you find
  * yourself in a situation in which you really need either method.
  *
  * #### Enabling/Disabling locking ####
  * If you want to switch to a non-locking mode, this will require the switching
  * thread to get a lock first. This way you will not leave your object in a
  * state where turning locking back on suddenly leaves you with a lock from a
  * long-gone thread.
  * Rule of thumb: Do only turn off locking directly after instantiating your
  * object and before any other thread might have accessed it!

  * **Important** : It is strongly recommended that you use std::lock_guard
  * or std::unique_lock to do the locking of any object derived from
  * pwx::CLockable. You can use PWX_LOCK_GUARD(pointer); and
  * PWX_DOUBLE_LOCK_GUARD(ptrA, ptrB); to do this rather simply. They
  * are defined in `basic/pwx_macros.h`.
  *
  * #### `atomic_flag spinlock` versus `std::mutex` ####
  * By default `CLockable` is compiled with PWX_USE_FLAGSPIN defined. This can
  * be controlled by the `-Dspinlocks` configuration option.
  * If you think using the full `std::mutex` for doing the locking then you can
  * set this value to `false` and compare the outcome of the speed tests of the
  * testlib program.
  *
  * #### Thread debugging versus spinlocks ####
  * If you enable annotations with the `-Dannotations=true` configuration
  * option, `std::mutex` is enforced. Using spinlocks here would lead to an
  * avalanche of false positives in Helgrind and DND.
**/
class PWX_API CLockable {
public:

	/* ===============================================
	 * === Public Types                            ===
	 * ===============================================
	*/

	#if !PWX_USE_FLAGSPIN
	typedef std::mutex lock_t; //!< Use standard mutex if no spinlocks are used.
	#endif // !PWX_USE_FLAGSPIN


	/* ===============================================
	 * === Public Constructors and destructor      ===
	 * ===============================================
	*/

	/** @brief Default constructor
	  *
	  * The Default constructor has nothing to do.
	**/
	explicit CLockable() noexcept;


	/** @brief Copy constructor
	  *
	  * All objects have their private locking.
	  * Only the state whether to actually do the locking is copied.
	  */
	CLockable ( CLockable const& src ) noexcept;


	/** @brief Destructor
	  *
	  * The destructor will try to unlock a held mutex until it succeeds.
	**/
	virtual ~CLockable() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	/// @return true if thread safety mode is turned on
	bool beThreadSafe() const noexcept PWX_WARNUNUSED;


	/** @brief set thread safety mode to @a doLock
	  *
	  * This is just an alias for do_locking().
	  *
	  * @param[in] doLock Whether to do locking or not
	**/
	void beThreadSafe( bool doLock ) noexcept;


	/** @brief clear all locks from this thread.
	  *
	  * If this thread is the current owner of the lock,
	  * and if there are locks in place, they are all cleared.
	  *
	  * If this thread is not the owner, the method simply returns false.
	  *
	  * @return true if this thread is the owner and all locks could be cleared.
	**/
	bool clear_locks() noexcept;


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
	bool destroyed() const noexcept PWX_WARNUNUSED;


	/** @brief switch whether to really use locking or not.
	  *
	  * With this method you can switch the locking mechanics on/off
	  * for objects to be used in concurrency or strictly single threaded.
	  * The default is to turn locking on.
	  *
	  * @param[in] doLock true to turn locking on, false to turn it off.
	**/
	void do_locking( bool doLock ) noexcept;


	/// @return true if this object is currently locked
	bool is_locked() const noexcept PWX_WARNUNUSED;


	/// @return true if the locking is turned on.
	bool is_locking() const noexcept PWX_WARNUNUSED;


	/** @brief Lock this object for the current thread if locking is enabled.
	  *
	  * This is a blocking method that returns once the lock is acquired.
	  * If this thread only has a lock, the lock count is raised.
	  */
	void lock() noexcept;


	/// @return the number of locks on this object *this* thread has
	uint32_t lock_count() const noexcept PWX_WARNUNUSED;


	/** @brief Try to lock this object.
	  *
	  * This is a non-blocking method that returns immediately.
	  * If this thread only has a lock, the lock count is raised.
	  *
	  * @return true if the object could be locked, false otherwise.
	  */
	bool try_lock() noexcept PWX_WARNUNUSED;


	/** @brief unlock one held lock
	  *
	  * If locking is disabled or if the current thread does not hold
	  * the lock, nothing happens. Otherwise the last lock is released.
	  *
	  * You have to unlock as many times as you have locked the object
	  * to fully unlock.
	  */
	void unlock() noexcept;


	/** @brief How many threads are waiting for a lock.
	  * @return The number of threads currently waiting for a lock.
	**/
	uint32_t waiting() const {
		return CL_Waiting.load( memOrdLoad );
	}


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief Assignment operator
	  *
	  * All objects have their private locking.
	  * Only the state whether to actually do the locking is copied.
	  */
	CLockable& operator= ( CLockable const& src ) noexcept;


protected:

	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	mutable
	abool_t isDestroyed  = ATOMIC_VAR_INIT( false ); //!< Should be set to true by the destructors of deriving classes.
	mord_t  memOrdLoad   = std::memory_order_acquire;     //!< to be used with atomic::load()
	mord_t  memOrdStore  = std::memory_order_release;     //!< to be used with atomic::store()


private:

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	abool_t CL_Do_Locking = ATOMIC_VAR_INIT( true );  //!< If set to false with do_locking(false), no real locking is done.
	abool_t CL_Is_Locked  = ATOMIC_VAR_INIT( false ); //!< Set to true if a lock is imposed, atomic_flag can't do it.

	#if PWX_USE_FLAGSPIN
	aflag_t CL_Lock       = ATOMIC_FLAG_INIT;         //!< Instead of a costly mutex atomic_flag spinlocks are used.
	#else
	lock_t CL_Lock;                                   //!< Use standard mutex to handle locking
	#endif // PWX_USE_FLAGSPIN

	aui32_t CL_Lock_Count = ATOMIC_VAR_INIT( 0 );     //!< How many times the current thread has locked.
	asize_t CL_Thread_ID  = ATOMIC_VAR_INIT( 0 );     //!< The owning thread of a lock
	aui32_t CL_Waiting    = ATOMIC_VAR_INIT( 0 );     //!< How many threads are waiting for a lock.


/* --- Override new/delete, so we can do memory allocation recording if wanted --- */
#ifndef PWX_NODOX
public:
	void* operator new     ( decltype( sizeof( 0 ) ) s )                   { return ( void* )pwx_calloc( uint8_t, s ); }
	void* operator new[]   ( decltype( sizeof( 0 ) ) s )                   { return ( void* )pwx_calloc( uint8_t, s ); }
	void  operator delete  ( void* ptr )                          noexcept { pwx_free( ptr ); }
	void  operator delete  ( void* ptr, decltype( sizeof( 0 ) ) ) noexcept { pwx_free( ptr ); }
	void  operator delete[]( void* ptr )                          noexcept { pwx_free( ptr ); }
	void  operator delete[]( void* ptr, decltype( sizeof( 0 ) ) ) noexcept { pwx_free( ptr ); }
#endif // NODOX
}; // class CLockable


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
bool are_locked( CLockable const* objA, CLockable const* objB ) noexcept PWX_WARNUNUSED PWX_API;


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
bool are_locked( CLockable const* objA, CLockable const* objB, CLockable const* objC ) noexcept PWX_WARNUNUSED PWX_API;


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
bool try_locks ( CLockable const* objA, CLockable const* objB ) noexcept PWX_WARNUNUSED PWX_API;


/** @brief try to lock three objects at once
  *
  * This function tries to lock three objects at once, returning
  * true if all three could be locked. If any can not be locked,
  * the others are unlocked again if necessary and false is
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
bool try_locks ( CLockable const* objA, CLockable const* objB, CLockable const* objC ) noexcept PWX_WARNUNUSED PWX_API;


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
bool unlock_all( CLockable const* objA, CLockable const* objB ) noexcept PWX_API;


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
  * @param[in] objC the third object to unlock
  * @return true if all three could be unlocked, false if at least one was not locked
**/
bool unlock_all( CLockable const * objA, CLockable const * objB, CLockable const * objC ) noexcept PWX_API;

// Note: A RAII-based class that uses these helpers is pwx::CLockGuard in basic/CLockGuard.h


} // namespace pwx

#endif // PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED

