#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "CLockable.h"

namespace pwx
{

/** @brief Default ctor
  */
CLockable::CLockable() noexcept
{ /* --- nothing to do here. ---*/ }


/** @brief Copy ctor
  *
  * All objects have their private locking.
  * Only the state whether to actually do the locking is copied.
  */
CLockable::CLockable (const CLockable &src) noexcept
 :	memOrdLoad(src.memOrdLoad),
	memOrdStore(src.memOrdStore),
	CL_Do_Locking(ATOMIC_VAR_INIT(src.CL_Do_Locking.load(memOrdLoad)))
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
	do_locking(src.CL_Do_Locking.load(memOrdLoad));
	return *this;
}


// ======================================
// === Public methods implementation ====
// ======================================


/// @brief return true if thread safety mode is turned on
bool CLockable::beThreadSafe() const noexcept
{
	return CL_Is_Locked.load(memOrdLoad);
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
  * @return true if this thread is the owner and all locks could be cleared.
**/
bool CLockable::clear_locks() noexcept
{
	if (CURRENT_THREAD_ID == CL_Thread_ID.load(memOrdLoad)) {
		if (is_locked() || try_lock()) {
			if (CL_Do_Locking.load(memOrdLoad))
				THREAD_LOG("base", "clear_locks(), Owner id 0x(%lx), %u locks [%s]",
						CL_Thread_ID.load(), CL_Lock_Count.load(),
						is_locked() ? "locked" : "not locked")
			CL_Lock_Count.store(1, memOrdStore);
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
  * Note: If a thread holds a lock while you turn locking off, you
  * might get screwed. Simply don't do this.
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

		CL_Do_Locking.store(doLock, memOrdStore);

		if (!doLock) {
			// Nuke all locks:
			CL_Lock_Count.store(0, memOrdStore);
			CL_Thread_ID.store(0, memOrdStore);
			CL_Is_Locked.store(false, memOrdStore);
			CL_Lock.clear(memOrdStore);

			// The memory order is relaxed last
			memOrdLoad  = PWX_MEMORDER_RELAXED;
			memOrdStore = PWX_MEMORDER_RELAXED;
		}
	}
}


/// @brief return true if this object is currently locked
bool CLockable::is_locked() const noexcept
{
	return CL_Is_Locked.load(memOrdLoad);
}


/// @brief return true if the locking is turned on.
bool CLockable::is_locking() const noexcept
{
	return CL_Do_Locking.load(memOrdLoad);
}


/** @brief lock
  *
  * Lock this object for the current thread if locking is enabled.
  */
void CLockable::lock() noexcept
{
	if ( CL_Do_Locking.load(memOrdLoad) ) {
		size_t ctid = CURRENT_THREAD_ID;
		if ( (ctid != CL_Thread_ID.load(memOrdLoad))
		  || !is_locked()) {
			THREAD_LOG("base", "lock(), Owner id 0x(%lx), %u locks [%s]",
					CL_Thread_ID.load(), CL_Lock_Count.load(),
					is_locked() ? "locked" : "not locked")
			while (CL_Lock.test_and_set()) { }

			// Got it now, so note it:
			CL_Thread_ID.store(ctid, memOrdStore);
			CL_Lock_Count.store(1, memOrdStore);
			CL_Is_Locked.store(true, memOrdStore);
		} else
			CL_Lock_Count.fetch_add(1, memOrdStore);
	}
}


/** @brief return the number of locks on this object *this* thread has
  * @return the number of current locks held by the calling thread
**/
uint32_t CLockable::lock_count() const noexcept
{
	if (CURRENT_THREAD_ID == CL_Thread_ID.load(memOrdLoad))
		return CL_Lock_Count.load(memOrdLoad);
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
	if ( CL_Do_Locking.load(memOrdLoad) ) {
		size_t ctid = CURRENT_THREAD_ID;
		if ( (ctid != CL_Thread_ID.load(memOrdLoad))
		  || !is_locked() ) {
			THREAD_LOG("base", "try_lock(), Owner id 0x(%lx), %u locks [%s]",
					CL_Thread_ID.load(), CL_Lock_Count.load(),
					is_locked() ? "locked" : "not locked")
			if (!CL_Lock.test_and_set()) {
				// Got it now, so note it:
				CL_Thread_ID.store(ctid, memOrdStore);
				CL_Lock_Count.store(1, memOrdStore);
				CL_Is_Locked.store(true, memOrdStore);
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
void CLockable::unlock() noexcept
{
	if ( CL_Do_Locking.load(memOrdLoad)
	  && (CURRENT_THREAD_ID == CL_Thread_ID.load(memOrdLoad))) {
		THREAD_LOG("base", "unlock(), Owner id 0x(%lx), %u locks [%s]",
				CL_Thread_ID.load(), CL_Lock_Count.load(),
				is_locked() ? "locked" : "not locked")
		if ((1 == CL_Lock_Count.fetch_sub(1)) || !is_locked()) {
			CL_Thread_ID.store(0, memOrdStore);
			CL_Lock_Count.store(0, memOrdStore);
			CL_Is_Locked.store(false, memOrdStore);
			CL_Lock.clear(memOrdStore);
		}
	}
}


} // namespace pwx
