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
  * @todo: document this function
  */
CLockable::~CLockable() noexcept
{ /* --- nothing to do here. ---*/ }


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
  * If the locking was successful, the method returns true, false otherwise
  *
  * If the macro PWX_THREADS is not defined, this method does nothing.
  *
  * @return true if the object could be locked, false otherwise.
  */
bool CLockable::try_lock() noexcept
{
#if defined(PWX_THREADS)
	PWX_TRY (return mutex.try_lock())
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
#if defined(PWC_THREADS)
	PWX_TRY (mutex.unlock())
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
