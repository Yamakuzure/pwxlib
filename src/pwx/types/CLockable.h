#pragma once
#ifndef PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED
#define PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED 1

/** @file CLockable.h
  *
  * @brief Declaration of a general base class to add locking mechanisms to classes derived from CLockable.
  *
  * (c) 2007 - 2012 PrydeWorX
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
#if defined(PWX_THREADS)
#  include <mutex>
#endif
#include <cstring>

namespace pwx
{

/** @class CLockable
  *
  * @brief Base class to make objects lockable via mutexes
  *
  * First things first: You need to compile both the library and your
  * application with the macro PWX_THREADS being defined, or there will
  * be no locking functionality.
  *
  * Any class that is derived from this class gains four methods
  * lock(), try_lock(), clear_locks() and unlock() to use a recursive mutex
  * for locking.
  * Recursive mutex means, that the class can be locked several times
  * by the locking thread, but needs the same amount of unlocking calls.
  *
  * If the owning thread destroys the lockable mutex, the destructor will
  * try to unlock completely before going away. If another thread waits
  * for a lock in the meantime, or if the destroying thread is not the
  * lock owner, the behavior is undefined.
  *
  * Although very unlikely, the three mutex methods can throw system errors.
  * These are caught and translated into pwx::CException with proper names,
  * the system errno as message and a proper description.
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
	CLockable (const CLockable&) noexcept;
	virtual ~CLockable() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

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
	bool clear_locks() noexcept
	{
#if defined(PWX_THREADS)
		if (mutex.native_handle()->__data.__lock && try_lock()) {
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
	void lock()
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
	bool try_lock() noexcept
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
	void unlock()
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


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	CLockable& operator= (const CLockable&) noexcept;


private:

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

#if defined(PWX_THREADS)
	::std::recursive_mutex mutex;
#endif
}; // class CLockable

} // namespace pwx

#endif // PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED

