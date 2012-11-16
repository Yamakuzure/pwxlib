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
#if defined(PWX_THREADS)
#  include <pwx/types/CException.h>
#  include <mutex>
#  include <cstring>
#endif

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

	bool clear_locks() noexcept;
	void lock();
	bool try_lock() noexcept;
	void unlock();


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

