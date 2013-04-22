#pragma once
#ifndef PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED
#define PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED 1

/** @file CLockable.h
  *
  * @brief Declaration of a general base class to add locking mechanisms to classes derived from CLockable.
  *
  * (c) 2007 - 2013 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         yamakuzure@users.sourceforge.net
  *         http://pwxlib.sourceforge.net
  *
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
**/

#include <pwx/general/compiler.h>
#include <pwx/types/CException.h>
#include <pwx/general/macros.h>
#include <atomic>
#include <mutex>
#include <thread>
#include <cstring>

namespace pwx
{

/** @class CLockable
  *
  * @brief Base class to make objects lockable via atomic_flag and lock counting.
  *
  * Any class that is derived from this class gains the following methods:
  * <UL>
  * <LI>clear_locks() - remove all locks for the current thread.</LI>
  * <LI>do_locking(bool) - if set to false, no real locking is done.
  * This is useful if you use anything derived from CLockable in a single
  * threaded environment. The default is to do locking.</LI>
  * <LI>is_locking() - returns true if the locking mechanism is turned on.</LI>
  * <LI>lock() - acquire lock for the current thread.</LI>
  * <LI>lock_count() - return the number of locks the current thread has.</LI>
  * <LI>try_lock() - try to acquire lock for the current thread and return true on success.</LI>
  * <LI>unlock() - release lock for the current thread if it holds one.</LI>
  * </UL>
  *
  * If the owning thread destroys the CLockable instance, the destructor will
  * try to unlock completely before going away. If another thread waits
  * for a lock in the meantime, or if the destroying thread is not the
  * lock owner, the behavior is undefined.
  *
  * <B>Important</B>: It is strongly recommended that you use std::lock_guard
  * or std::unique_lock to do the locking of any object derived from
  * pwx::CLockable. You can use PWX_LOCK_GUARD(type, pointer) and
  * PWX_DOUBLE_LOCK(typeA, ptrA, typeB, ptrB) to do this rather simply. They
  * are defined in pwx/general/macros.h.
**/
class PWX_API CLockable
{

public:

	/* ===============================================
	 * === Public Constructors and destructor      ===
	 * ===============================================
	*/

	explicit CLockable() noexcept;
	CLockable (const CLockable &src) noexcept;
	virtual ~CLockable() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	/** @brief clear all locks from this thread.
	  *
	  * If this thread is the current owner of the lock,
	  * and if there are locks in place, they are all cleared.
	  *
	  * @return true if this thread is the owner and all locks could be cleared.
	**/
	bool clear_locks() noexcept
	{
		if (CURRENT_THREAD_ID == CL_Thread_ID) {
			if (is_locked() || try_lock()) {
				THREAD_LOG("base", "clear_locks(), Owner id (%lu), %u locks [%s]",
						CL_Thread_ID, CL_Lock_Count,
						is_locked() ? "locked" : "not locked")
				CL_Lock_Count = 1;
				this->unlock();
			}

			return true;
		}

		// Return false we can not unlock it from this thread
		return false;
	}


	/** @brief switch whether to really use locking or not.
	  *
	  * With this method you can switch the locking mechanics on/off
	  * for objects to be used in concurrency or strictly single threaded.
	  * The default is to turn locking on.
	  *
	  * Note: If a thread holds a lock while you turn locking off, this
	  * method waits until a lock is acquired before turning locking off.
	  *
	  * @param[in] doLock true to turn locking on, false to turn it off.
	**/
	void do_locking(bool doLock) noexcept
	{
        if (doLock != CL_Do_Locking.load(std::memory_order_acquire)) {
			if (!doLock)
				this->lock(); // Need this to ensure nobody relies on a lock now.
			CL_Do_Locking.store(doLock, std::memory_order_release);
			if (!doLock) {
				// Nuke the lock acquired above:
				CL_Lock_Count = 0;
				CL_Thread_ID  = 0;
				CL_Is_Locked.store(false, std::memory_order_release);
				CL_Lock.clear(std::memory_order_release);
			}
        }
	}


	/// @brief return true if this object is currently locked
	bool is_locked() const noexcept
	{
		return CL_Is_Locked.load(std::memory_order_acquire);
	}


	/// @brief return true if the locking is turned on.
	bool is_locking() const noexcept
	{
		return CL_Do_Locking.load(std::memory_order_acquire);
	}


	/** @brief lock
	  *
	  * Lock this object for the current thread if locking is enabled.
	  */
	void lock() noexcept
	{
		if ( CL_Do_Locking.load(std::memory_order_acquire) ) {
			size_t ctid = CURRENT_THREAD_ID;
			if ( (ctid != CL_Thread_ID) || !is_locked()) {
				THREAD_LOG("base", "lock(), Owner id (%lu), %u locks [%s]",
						CL_Thread_ID, CL_Lock_Count,
						is_locked() ? "locked" : "not locked")
				while (CL_Lock.test_and_set())
					std::this_thread::yield();
				// Got it now, so note it:
				CL_Thread_ID  = ctid;
				CL_Lock_Count = 1;
				CL_Is_Locked.store(true, std::memory_order_release);
			} else
				++CL_Lock_Count;
		}
	}


	/** @brief return the number of locks on this object *this* thread has
	  * @return the number of current locks held by the calling thread
	**/
	uint32_t lock_count() const noexcept
	{
		if (CURRENT_THREAD_ID == CL_Thread_ID)
			return CL_Lock_Count;
		return 0;
	}


	/** @brief try_lock
	  *
	  * Try to lock this object.
	  *
	  * @return true if the object could be locked, false otherwise.
	  */
	bool try_lock() noexcept
	{
		if ( CL_Do_Locking.load(std::memory_order_acquire) ) {
			size_t ctid = CURRENT_THREAD_ID;
			if ( (ctid != CL_Thread_ID) || !is_locked() ) {
				THREAD_LOG("base", "try_lock(), Owner id (%lu), %u locks [%s]",
						CL_Thread_ID, CL_Lock_Count,
						is_locked() ? "locked" : "not locked")
				if (!CL_Lock.test_and_set()) {
					// Got it now, so note it:
					CL_Thread_ID  = ctid;
					CL_Lock_Count = 1;
					CL_Is_Locked.store(true, std::memory_order_release);
					return true;
				}
				return false; // Nope, and only condition for a no-no.
			}
		}

		// return true otherwise, we are fine.
		return true;
	}


	/** @brief unlock
	  *
	  * If locking is disabled or if the current thread does not hold
	  * the lock, nothing happens. Otherwise the last lock is released.
	  */
	void unlock() noexcept
	{
        if ( CL_Do_Locking.load(std::memory_order_acquire)
		  && (CURRENT_THREAD_ID == CL_Thread_ID)) {
			THREAD_LOG("base", "unlock(), Owner id (%lu), %u locks [%s]",
					CL_Thread_ID, CL_Lock_Count,
					is_locked() ? "locked" : "not locked")
			if ((0 == --CL_Lock_Count) || !is_locked()) {
				CL_Thread_ID  = 0;
				CL_Lock_Count = 0;
				CL_Is_Locked.store(false, std::memory_order_release);
				CL_Lock.clear(std::memory_order_release);
			}
		}
	}


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	CLockable& operator= (const CLockable &src) noexcept;


// If the DEBUG_LOCK_STATE is enabled, the members need to be acceddible from
// the callers, which must be derived from CLockable anyway.
#ifndef PWX_THREADDEBUG
private:
#endif // PWX_THREADDEBUG

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	std::atomic_bool CL_Do_Locking; //!< If set to false with do_locking(false), no real locking is done.
	std::atomic_bool CL_Is_Locked;  //!< Set to true if a lock is imposed, atomic_flag can't do it.
	std::atomic_flag CL_Lock;       //!< Instead of a costly mutex atomic_flag spinlocks are used.
	uint32_t         CL_Lock_Count; //!< How many times the current thread has locked.
	size_t           CL_Thread_ID;  //!< The owning thread of a lock
}; // class CLockable

} // namespace pwx

#endif // PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED

