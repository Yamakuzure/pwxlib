#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "CLockable.h"

namespace pwx
{

/** @brief Default ctor
  */
CLockable::CLockable() noexcept
{ /* nothing to do here */ }


/** @brief Copy ctor
  *
  * All objects have their private locking.
  * Only the state whether to actually do the locking is copied.
  */
CLockable::CLockable (const CLockable &src) noexcept
 :	memOrdLoad(src.memOrdLoad),
	memOrdStore(src.memOrdStore),
	CL_Do_Locking(ATOMIC_VAR_INIT(src.CL_Do_Locking.load(PWX_MEMORDER_RELAXED)))
{ /* --- nothing to do here. ---*/ }



/** @brief ~CLockable
  *
  * The default dtor will try to unlock a held mutex until it succeeds.
  */
CLockable::~CLockable() noexcept
{
	DEBUG_LOCK_STATE("~CLockable", this, this)
	clear_locks();
	// the return value is unimportant, we can't do
	// anything about it in the middle of a dtor anyway.
}


/** @brief Assignment operator
  *
  * All objects have their private locking.
  * Only the state whether to actually do the locking is copied.
  */
CLockable &CLockable::operator= (const CLockable &src) noexcept
{
	do_locking(src.CL_Do_Locking.load(PWX_MEMORDER_RELAXED));
	return *this;
}


// ======================================
// === Public methods implementation ====
// ======================================


/// @brief return true if thread safety mode is turned on
bool CLockable::beThreadSafe() const noexcept
{
	return CL_Do_Locking.load(PWX_MEMORDER_RELAXED);
}


/// @brief set thread safety mode to @a doLock
/// This is just an alias for do_locking().
void CLockable::beThreadSafe(bool doLock) noexcept
{
	this->do_locking(doLock);
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
bool CLockable::clear_locks() noexcept
{
	if (CL_Do_Locking.load(PWX_MEMORDER_RELAXED)) {
		if (CL_Is_Locked.load(memOrdLoad)) {
			if (CURRENT_THREAD_ID == CL_Thread_ID.load(memOrdLoad) ) {
				THREAD_LOG("base", "clear_locks(), Owner id 0x%08lx, %u locks [%s]",
						CL_Thread_ID.load(memOrdLoad),
						CL_Lock_Count.load(memOrdLoad),
						CL_Is_Locked.load(memOrdLoad) ? "locked" : "not locked")
				CL_Lock_Count.store(0, PWX_MEMORDER_RELAXED);
				CL_Thread_ID.store(0, PWX_MEMORDER_RELAXED);
#ifdef PWX_USE_FLAGSPIN
				CL_Is_Locked.store(false, PWX_MEMORDER_RELAXED);
				CL_Lock.clear(memOrdStore); // This *must* be last!
#else
				CL_Lock.unlock();
				CL_Is_Locked.store(false, memOrdStore); // an dthis mujst be last with mutexes

#endif // PWX_USE_FLAGSPIN
			} // End of trying from within the right thread
			else
				return false; // Not from this thread!
		} // End of needing unlock
	} // End of having to care about locking

	return true;
}


/** @brief switch whether to really use locking or not.
  *
  * With this method you can switch the locking mechanics on/off
  * for objects to be used in concurrency or strictly single threaded.
  * The default is to turn locking on.
  *
  * @param[in] doLock true to turn locking on, false to turn it off.
**/
void CLockable::do_locking(bool doLock) noexcept
{
	if (doLock != CL_Do_Locking.load(memOrdLoad)) {
		// If locking is enabled, change memory order now to strict
		if (doLock) {
			memOrdLoad  = PWX_MEMORDER_ACQUIRE;
			memOrdStore = PWX_MEMORDER_RELEASE;
		}

		// Switch now, so other threads stop locking
		// If this is a switch "on", it is finished anyway.
		CL_Do_Locking.store(doLock, memOrdStore);

		if (!doLock) {
			bool needUnlock = false;
			if (CL_Is_Locked.load(memOrdLoad)) {
				needUnlock = true;
				/* If this is locked it is either by this or another
				 * thread, so an additional unlock() is needed.
				 * Unfortunately, if another thread owns the lock,
				 * this thread has first to claim the lock.
				 */
				if (CL_Thread_ID.load(memOrdLoad) != CURRENT_THREAD_ID) {
					/* Sorry for the code doubling, but lock() would listen
					 * to CL_Do_Locking and that has to be false by now.
					 */
					std::this_thread::yield(); // to be sure this thread is last
#ifdef PWX_USE_FLAGSPIN
					while (CL_Lock.test_and_set()) {
# ifdef PWX_USE_FLAGSPIN_YIELD
						std::this_thread::yield();
# endif // PWX_USE_FLAGSPIN_YIELD
					}
#else
					CL_Lock.lock();
#endif // PWX_USE_FLAGSPIN
				} // end of having to gain the lock
			} // End of having a locked state

			// Nuke all data:
			CL_Thread_ID.store(0, PWX_MEMORDER_RELAXED);
			CL_Lock_Count.store(0, PWX_MEMORDER_RELAXED);
			if (needUnlock) {
#ifdef PWX_USE_FLAGSPIN
				CL_Lock.clear(PWX_MEMORDER_RELAXED);
#else
				CL_Lock.unlock();
#endif // PWX_USE_FLAGSPIN
			}
			CL_Is_Locked.store(false, memOrdStore);
			// The memory order is relaxed last
			memOrdLoad  = PWX_MEMORDER_RELAXED;
			memOrdStore = PWX_MEMORDER_RELAXED;
		} // End of switching locking off
	} // End of having a change
}


/// @brief return true if this object is currently locked
bool CLockable::is_locked() const noexcept
{
	return CL_Is_Locked.load(memOrdLoad);
}


/// @brief return true if the locking is turned on.
bool CLockable::is_locking() const noexcept
{
	return CL_Do_Locking.load(PWX_MEMORDER_RELAXED);
}


/** @brief lock
  *
  * Lock this object for the current thread if locking is enabled.
  */
void CLockable::lock() noexcept
{
	if ( CL_Do_Locking.load(PWX_MEMORDER_RELAXED) ) {
		size_t ctid = CURRENT_THREAD_ID;
		THREAD_LOG("base", "lock(), Owner id 0x%08lx, %u locks [%s]",
				ctid, CL_Lock_Count.load(memOrdLoad),
				CL_Is_Locked.load(memOrdLoad) ? "locked" : "not locked")

		// For both the spinlock and the mutex an action
		// is only taken if this object is not already
		// locked by this thread
		if ( ctid != CL_Thread_ID.load(memOrdLoad) ) {
#ifdef PWX_USE_FLAGSPIN
			while (CL_Lock.test_and_set()) {
# ifdef PWX_USE_FLAGSPIN_YIELD
				std::this_thread::yield();
# endif // PWX_USE_FLAGSPIN_YIELD
			}
#else
			CL_Lock.lock();
#endif // PWX_USE_FLAGSPIN

			// Got it now, so note it:
			CL_Is_Locked.store(true, memOrdStore);
			CL_Thread_ID.store(ctid, PWX_MEMORDER_RELAXED);
			CL_Lock_Count.store(1, PWX_MEMORDER_RELAXED);
		} else
			// If this thread already has a lock, the call is just counted
			CL_Lock_Count.fetch_add(1, PWX_MEMORDER_RELAXED);
	} // End of doing locking
}


/** @brief return the number of locks on this object *this* thread has
  * @return the number of current locks held by the calling thread
**/
uint32_t CLockable::lock_count() const noexcept
{
	if (CURRENT_THREAD_ID == CL_Thread_ID.load(memOrdLoad))
		return CL_Lock_Count.load(PWX_MEMORDER_RELAXED);
	return 0;
}


/** @brief try_lock
  *
  * Try to lock this object.
  *
  * @return true if the object could be locked, false otherwise.
  */
bool CLockable::try_lock() noexcept
{
	if ( CL_Do_Locking.load(PWX_MEMORDER_RELAXED) ) {
		size_t ctid = CURRENT_THREAD_ID;
		THREAD_LOG("base", "try_lock(), Owner id 0x%08lx, %u locks [%s]",
				CL_Thread_ID.load(), CL_Lock_Count.load(),
				CL_Is_Locked.load(memOrdLoad) ? "locked" : "not locked")

		// Same as with locking: Only try if this thread does
		// not already own the lock
		if (ctid != CL_Thread_ID.load(memOrdLoad)) {
#ifdef PWX_USE_FLAGSPIN
			if (!CL_Lock.test_and_set()) {
#else
			if (CL_Lock.try_lock()) {
#endif // PWX_USE_FLAGSPIN
				// Got it now, so note it:
				CL_Thread_ID.store(ctid, PWX_MEMORDER_RELAXED);
				CL_Lock_Count.store(1, PWX_MEMORDER_RELAXED);
				CL_Is_Locked.store(true, memOrdStore);
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
void CLockable::unlock() noexcept
{
	if ( CL_Do_Locking.load(PWX_MEMORDER_RELAXED)
	  && (CURRENT_THREAD_ID == CL_Thread_ID.load(memOrdLoad))) {
		THREAD_LOG("base", "unlock(), Owner id 0x%08lx, %u locks [%s]",
				CL_Thread_ID.load(), CL_Lock_Count.load(),
				CL_Is_Locked.load(memOrdLoad) ? "locked" : "not locked")

		if (1 == CL_Lock_Count.fetch_sub(1, PWX_MEMORDER_RELAXED)) {
			// The lock will go away now:
			CL_Thread_ID.store(0, PWX_MEMORDER_RELAXED);
			CL_Is_Locked.store(false, PWX_MEMORDER_RELAXED);
#ifdef PWX_USE_FLAGSPIN
			CL_Lock.clear(memOrdStore);
#else
			CL_Lock.unlock();
#endif // PWX_USE_FLAGSPIN
		} // end of reducing the lock count
	}
}


} // namespace pwx
