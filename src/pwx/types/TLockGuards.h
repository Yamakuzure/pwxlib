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
	explicit TLockGuard(const T* obj_) :
		obj(const_cast<T*>(obj_))
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


	/* ===============================================
	 * === Public Methods                          ===
	 * ===============================================
	*/

	/** @brief reset all locks
	  *
	  * This method can be used to replace the locked object
	  * with another one. The first will be unlocked, and the
	  * new one locked. If @a new_obj is nullptr, only the
	  * old one is unlocked.
	  *
	  * If you submit the same pointer again, it is unlocked
	  * and then locked again.
	  *
	  * @param[in] new_obj pointer to the new object to lock
	**/
	void reset(const T* new_obj)
	{
		if (obj)            obj->unlock();
		if (obj != new_obj) obj = const_cast<T*>(new_obj);
		if (obj)            obj->lock();
	}


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
	explicit TDoubleLockGuard(const Ta* objA_, const Tb* objB_) :
		objA(const_cast<Ta*>(objA_)),
		objB(const_cast<Tb*>(objB_))
	{
		privLockAll();
	}


	/** @brief default destructor
	  * This is non-virtual. Do _NOT_ derive from this class!
	**/
	~TDoubleLockGuard()
	{
		privUnlockAll();
	}


	// No empty ctor, no assigning, no copying
	TDoubleLockGuard() PWX_DELETE;
	TDoubleLockGuard(const TDoubleLockGuard&) PWX_DELETE;
	TDoubleLockGuard &operator=(const TDoubleLockGuard&) PWX_DELETE;


	/* ===============================================
	 * === Public Methods                          ===
	 * ===============================================
	*/

	/** @brief reset all locks
	  *
	  * This method can be used to replace the locked objects
	  * with other ones. The first will be unlocked, and the
	  * new ones locked. If either @a new_objA or @a new_objB
	  * is nullptr, only the corresponding old ones are
	  * unlocked.
	  *
	  * If you submit the same pointer again, it is unlocked
	  * and then locked again.
	  *
	  * @param[in] new_objA pointer to the first new object to lock
	  * @param[in] new_objB pointer to the second new object to lock
	**/
	void reset(const Ta* new_objA, const Tb* new_objB)
	{
		privUnlockAll();

		if (objA != new_objA) objA = const_cast<Ta*>(new_objA);
		if (objB != new_objB) objB = const_cast<Tb*>(new_objB);

		privLockAll();
	}


private:

	/* ===============================================
	 * === Private Methods                         ===
	 * ===============================================
	*/

	/// @brief the internal method that does the actual locking
	void privLockAll()
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


	/// @brief The internal method that does the unlocking
	void privUnlockAll()
	{
		if (objA) objA->unlock();
		if (objB) objB->unlock();
	}


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
	explicit TTripleLockGuard(const Ta* objA_, const Tb* objB_, const Tc* objC_) :
		objA(const_cast<Ta*>(objA_)),
		objB(const_cast<Tb*>(objB_)),
		objC(const_cast<Tc*>(objC_))
	{
		privLockAll();
	}


	/** @brief default destructor
	  * This is non-virtual. Do _NOT_ derive from this class!
	**/
	~TTripleLockGuard()
	{
		privUnlockAll();
	}


	// No empty ctor, no assigning, no copying
	TTripleLockGuard() PWX_DELETE;
	TTripleLockGuard(const TTripleLockGuard&) PWX_DELETE;
	TTripleLockGuard &operator=(const TTripleLockGuard&) PWX_DELETE;


	/* ===============================================
	 * === Public Methods                          ===
	 * ===============================================
	*/

	/** @brief reset all locks
	  *
	  * This method can be used to replace the locked objects
	  * with other ones. The first will be unlocked, and the
	  * new ones locked. If either @a new_objA, @a new_objB or
	  * @a new_objC is nullptr, only the corresponding old
	  * ones are unlocked.
	  *
	  * If you submit the same pointer again, it is unlocked
	  * and then locked again.
	  *
	  * @param[in] new_objA pointer to the first new object to lock
	  * @param[in] new_objB pointer to the second new object to lock
	  * @param[in] new_objC pointer to the third new object to lock
	**/
	void reset(const Ta* new_objA, const Tb* new_objB, const Tc* new_objC)
	{
		privUnlockAll();

		if (objA != new_objA) objA = const_cast<Ta*>(new_objA);
		if (objB != new_objB) objB = const_cast<Tb*>(new_objB);
		if (objC != new_objC) objC = const_cast<Tc*>(new_objC);

		privLockAll();
	}


private:

	/* ===============================================
	 * === Private Methods                         ===
	 * ===============================================
	*/

	/// @brief the internal method that does the actual locking
	void privLockAll()
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


	/// @brief The internal method that does the unlocking
	void privUnlockAll()
	{
		if (objA) objA->unlock();
		if (objB) objB->unlock();
		if (objC) objC->unlock();
	}


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

