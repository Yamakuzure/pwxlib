#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include <pwx/types/CLockable.h>
#include <pwx/general/macros.h>

namespace pwx
{

/** @brief Default ctor
  */
CLockable::CLockable() noexcept
{ /* --- nothing to do here. ---*/ }


/** @brief Copy ctor
  *
  * Nothing to do, all objects have their private mutex.
  */
CLockable::CLockable (const CLockable&) noexcept
{ /* --- nothing to do here. ---*/ }



/** @brief ~CLockable
  *
  * The default dtor will try to unlock a held mutex until it succeeds.
  */
CLockable::~CLockable() noexcept
{
#if defined(PWX_THREADS)
	clear_locks();
	// the return value is unimportant, we can't do
	// anything about it in the middle of a dtor anyway.
#endif
}


/** @brief clear all locks from this thread.
  *
  * If this thread is the current owner of the mutex' lock,
  * and if there are locks in place, they are all cleared.
  *
  * Warning: This method throws away all exceptions. Use
  * it only if a normal unlocking is, for whatever reason,
  * not an option. Regular unlock() calls are always to be
  * preferred.
  *
  * If the macro PWX_THREADS is not defined, this method
  * always returns true.
  *
  * @return true if this thread is the owner and all locks could be cleared.
**/
bool CLockable::clear_locks() noexcept
{
#if defined(PWX_THREADS)
	if (mutex.native_handle()->__data.__lock && try_lock()){
		/** @todo : There must be an easier way, or more direct way,
		  * to find out whether this thread is the owner or not.
		**/
		while (mutex.native_handle()->__data.__count) {
			PWX_TRY(this->unlock())
			PWX_CATCH_AND_FORGET(CException)
		}
	}

	// Return false if the mutex is still locked:
	if (mutex.native_handle()->__data.__lock)
		return false;
#endif
	return true;
}


/** @brief lock
  *
  * Lock the recursive mutex of this object. If a system error occurs a
  * pwx::CException with the name "LockFailed" will be thrown. The system
  * error name can be retrieved by the exceptions what() method, a short
  * explanation by the desc() method.
  *
  * Caught system errors are:
  *   -# EINVAL  - Invalid Argument
  *   -# EAGAIN  - Try again
  *   -# EBUSY   - Device or resource busy
  *   -# EDEADLK - Resource deadlock would occur
  *
  * If the macro PWX_THREADS is not defined, this method does nothing.
  */
void CLockable::lock()
{
#if defined(PWX_THREADS)
	PWX_TRY (mutex.lock())
	catch (int &e) {
		if      (EINVAL  == e) PWX_THROW ("LockFailed", "EINVAL",  "Invalid argument")
		else if (EAGAIN  == e) PWX_THROW ("LockFailed", "EAGAIN",  "Try again")
		else if (EBUSY   == e) PWX_THROW ("LockFailed", "EBUSY",   "Device or resource busy")
		else if (EDEADLK == e) PWX_THROW ("LockFailed", "EDEADLK", "Resource deadlock would occur")
	}
#endif
}


/** @brief try_lock
  *
  * Try to lock the recursive mutex of this object.
  *
  * If the locking was successful, the method returns true, false otherwise.
  * Although this method can be used to determine whether a thread is the
  * current owner of a lock, please remember that a return value of
  * "true" means that you have to unlock this extra lock.
  *
  * If the macro PWX_THREADS is not defined, this method does nothing.
  *
  * @return true if the object could be locked, false otherwise.
  */
bool CLockable::try_lock() noexcept
{
#if defined(PWX_THREADS)
	PWX_TRY(return mutex.try_lock())
	PWX_CATCH_AND_FORGET (int)
	return false; // A system error occurred.
#else
	return true;
#endif
}


/** @brief unlock
  *
  *
  * Unlock the recursive mutex of this object. If a system error occurs a
  * pwx::CException with the name "LockFailed" will be thrown. The system
  * error name can be retrieved by the exceptions what() method, a short
  * explanation by the desc() method.
  *
  * Caught system errors are:
  *   -# EINVAL  - Invalid Argument
  *   -# EAGAIN  - Try again
  *   -# EBUSY   - Device or resource busy
  *
  * If the macro PWX_THREADS is not defined, this method does nothing.
  */
void CLockable::unlock()
{
#if defined(PWX_THREADS)
	PWX_TRY(mutex.unlock())
	catch (int &e) {
		if      (EINVAL  == e) PWX_THROW ("UnlockFailed", "EINVAL",  "Invalid argument")
		else if (EAGAIN  == e) PWX_THROW ("UnlockFailed", "EAGAIN",  "Try again")
		else if (EBUSY   == e) PWX_THROW ("UnlockFailed", "EBUSY",   "Device or resource busy")
	}
#endif
}

/** @brief Assignment operator
  *
  * Nothing to do, all objects have their private mutex.
  */
CLockable &CLockable::operator= (const CLockable&) noexcept
{ return *this; }

} // namespace pwx
