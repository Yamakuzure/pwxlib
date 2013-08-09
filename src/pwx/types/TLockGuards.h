#ifndef PWX_LIBPWX_PWX_TYPES_TLOCKGUARDS_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_TLOCKGUARDS_H_INCLUDED

/** @file TLockGuards.h
  *
  * @brief Templates to do guard locking for one, two or three objects
  *
  * These templates have been created to be able to RAII-autolock
  * up to three objects at once. This is helpful handling doubly linked
  * list based containers and elements.
  *
  * (c) 2007 - 2013 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
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

#include <thread>

namespace pwx {


/** @class TLockGuard
  * @brief Basic RAII lock guard to lock/unlock one object within its ctor/dtor
**/
template<typename T>
class PWX_API TLockGuard
{
public:

	/* ===============================================
	 * === Public constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor that locks @a obj_
	  *
	  * The constructor locks @a obj_ upon creation
	  * unless it is nullptr, in which case it does nothing.
	  * @param[in] obj_ the object to lock, it must have a lock() method.
	**/
	explicit TLockGuard(T* obj_) :
		obj(obj_)
	{
		if (obj) obj->lock();
	}


	/** @brief default destructor
	  * This is non-virtual. Do _NOT_ derive from this class!
	**/
	~TLockGuard()
	{
		if (obj) obj->unlock();
	}


	// No empty ctor, no assigning, no copying
	TLockGuard() PWX_DELETE;
	TLockGuard(const TLockGuard&) PWX_DELETE;
	TLockGuard &operator=(const TLockGuard&) PWX_DELETE;


private:

	/* ===============================================
	 * === Private Members                         ===
	 * ===============================================
	*/

	T* obj;
};


/** @class TDoubleLockGuard
  * @brief Basic RAII lock guard to lock/unlock two objects within its ctor/dtor
**/
template<typename Ta, typename Tb>
class PWX_API TDoubleLockGuard
{
public:

	/* ===============================================
	 * === Public constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor that locks @a objA_ and @a objB_
	  *
	  * The constructor locks @a objA_ and @a objB_ upon
	  * creation. If both are nullptr, the ctor does nothing.
	  * @param[in] objA_ the first object to lock, it must have a try_lock() method.
	  * @param[in] objB_ the second object to lock, it must have a try_lock() method.
	**/
	explicit TDoubleLockGuard(Ta* objA_, Tb* objB_) :
		objA(objA_), objB(objB_)
	{
		bool lockedA = objA ? false : true;
		bool lockedB = objB ? false : true;

		while (!lockedA || !lockedB) {
			if (objA) lockedA = objA->try_lock();
			if (objB) lockedB = objB->try_lock();

			if (!lockedA || !lockedB) {
				if (objA && lockedA) objA->unlock();
				if (objB && lockedB) objB->unlock();

				// Yield, so the threads that have the lock(s)
				// have time to do their work and release them.
					std::this_thread::yield();
			}
		}
	}


	/** @brief default destructor
	  * This is non-virtual. Do _NOT_ derive from this class!
	**/
	~TDoubleLockGuard()
	{
		if (objA) objA->unlock();
		if (objB) objB->unlock();
	}


	// No empty ctor, no assigning, no copying
	TDoubleLockGuard() PWX_DELETE;
	TDoubleLockGuard(const TDoubleLockGuard&) PWX_DELETE;
	TDoubleLockGuard &operator=(const TDoubleLockGuard&) PWX_DELETE;


private:

	/* ===============================================
	 * === Private Members                         ===
	 * ===============================================
	*/

	Ta* objA;
	Tb* objB;
};


/** @class TTripleLockGuard
  * @brief Basic RAII lock guard to lock/unlock three objects within its ctor/dtor
**/
template<typename Ta, typename Tb, typename Tc>
class PWX_API TTripleLockGuard
{
public:

	/* ===============================================
	 * === Public constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor that locks @a objA_, @a objB_ and @a objC_
	  *
	  * The constructor locks @a objA_, @a objB_ and @a objC_
	  * upon creation. If all three are nullptr, the ctor does nothing.
	  * @param[in] objA_ the first object to lock, it must have a try_lock() method.
	  * @param[in] objB_ the second object to lock, it must have a try_lock() method.
	  * @param[in] objC_ the third object to lock, it must have a try_lock() method.
	**/
	explicit TTripleLockGuard(Ta* objA_, Tb* objB_, Tc* objC_) :
		objA(objA_), objB(objB_), objC(objC_)
	{
		bool lockedA = objA ? false : true;
		bool lockedB = objB ? false : true;
		bool lockedC = objC ? false : true;

		while (!lockedA || !lockedB || !lockedC) {
			if (objA) lockedA = objA->try_lock();
			if (objB) lockedB = objB->try_lock();
			if (objC) lockedC = objC->try_lock();

			if (!lockedA || !lockedB || !lockedC) {
				if (objA && lockedA) objA->unlock();
				if (objB && lockedB) objB->unlock();
				if (objC && lockedC) objC->unlock();

				// Yield, so the threads that have the lock(s)
				// have time to do their work and release them.
					std::this_thread::yield();
			}
		}
	}


	/** @brief default destructor
	  * This is non-virtual. Do _NOT_ derive from this class!
	**/
	~TTripleLockGuard()
	{
		if (objA) objA->unlock();
		if (objB) objB->unlock();
		if (objC) objC->unlock();
	}


	// No empty ctor, no assigning, no copying
	TTripleLockGuard() PWX_DELETE;
	TTripleLockGuard(const TTripleLockGuard&) PWX_DELETE;
	TTripleLockGuard &operator=(const TTripleLockGuard&) PWX_DELETE;


private:

	/* ===============================================
	 * === Private Members                         ===
	 * ===============================================
	*/

	Ta* objA;
	Tb* objB;
	Tc* objC;
};


}

#endif // PWX_LIBPWX_PWX_TYPES_TLOCKGUARDS_H_INCLUDED

