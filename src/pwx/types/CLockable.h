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
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef std::memory_order         mord_t;
	typedef std::atomic_bool          abool_t;
	typedef std::atomic_flag          aflag_t;
	typedef std::atomic_uint_fast32_t aui32_t;
	typedef std::atomic_size_t        asize_t;


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

	bool     beThreadSafe()            const noexcept; //!< true if thread safety is turned on
	void     beThreadSafe(bool doLock)       noexcept; //!< set thread safety to @a doLock
	bool     clear_locks ()                  noexcept; //!< remove all locks
	void     do_locking  (bool doLock)       noexcept; //!< set thread safety to @a doLock
	bool     is_locked   ()            const noexcept; //!< return true if this object is locked by this thread
	bool     is_locking  ()            const noexcept; //!< true if thread safety is turned on
	void     lock        ()                  noexcept; //!< lock this object
	uint32_t lock_count  ()            const noexcept; //!< number of locks this thread holds on this object
	bool     try_lock    ()                  noexcept; //!< try to lock and return at once
	void     unlock      ()                  noexcept; //!< unlock this object


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	CLockable& operator= (const CLockable &src) noexcept;


protected:

	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	mord_t  memOrdLoad   = PWX_MEMORDER_ACQUIRE;   //!< to be used with atomic::load()
	mord_t  memOrdStore  = PWX_MEMORDER_RELEASE;   //!< to be used with atomic::store()


// If the DEBUG_LOCK_STATE is enabled, the members need to be acceddible from
// the callers, which must be derived from CLockable anyway.
#ifndef PWX_THREADDEBUG
private:
#endif // PWX_THREADDEBUG

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	abool_t CL_Do_Locking = ATOMIC_VAR_INIT(true);   //!< If set to false with do_locking(false), no real locking is done.
	abool_t CL_Is_Locked  = ATOMIC_VAR_INIT(false);  //!< Set to true if a lock is imposed, atomic_flag can't do it.
	aflag_t CL_Lock       = ATOMIC_FLAG_INIT;        //!< Instead of a costly mutex atomic_flag spinlocks are used.
	aui32_t CL_Lock_Count = ATOMIC_VAR_INIT(0);      //!< How many times the current thread has locked.
	asize_t CL_Thread_ID  = ATOMIC_VAR_INIT(0);      //!< The owning thread of a lock
}; // class CLockable

} // namespace pwx

#endif // PWX_PWXLIB_THREADS_CLOCKABLE_H_INCLUDED

