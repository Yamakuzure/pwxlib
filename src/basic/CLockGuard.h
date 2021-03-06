#ifndef PWX_LIBPWX_PWX_TYPES_CLOCKGUARD_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_CLOCKGUARD_H_INCLUDED 1
#pragma once

/** @file CLockGuard.h
  *
  * @brief Simple Class to RAII-lock up to 3 objects derived from pwx::Clockable
  *
  * This class have been created to be able to RAII-autolock
  * up to three objects at once. This is helpful handling
  * doubly linked list based containers and elements.
  *
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  * History and change log are maintained in pwxlib.h
**/


#include "basic/compiler.h"
#include "basic/CLockable.h"


/** @brief Create a lock guard on the given object, that is unlocked when leaving the current scope
  *
  * Note: If you only need exactly one lock guard in your function/method, you can
  *       use `PWX_LOCK_GUARD()` instead. It will use the function name.
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param object pointer to the object to lock
**/
#define PWX_NAMED_LOCK_GUARD( Name, object ) ::pwx::CLockGuard pwx_libpwx_lock_guard_##Name(object)


/** @brief Create a lock guard on the given object, that is unlocked when leaving the current scope
  *
  * Note: If you need more than one lock guard, you have to use
  *       `PWX_NAMED_LOCK_GUARD()` for any additional ones instead.
  *
  * @param object pointer to the object to lock
**/
#define PWX_LOCK_GUARD( object ) PWX_NAMED_LOCK_GUARD(PWX_FUNC, object)


/** @brief Clear a named lock guard, unlocking all currently locked objects
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
**/
#define PWX_NAMED_LOCK_GUARD_CLEAR( Name ) pwx_libpwx_lock_guard_##Name.reset(NULL_LOCK, NULL_LOCK, NULL_LOCK)


/// @brief Clear the lock guard named after the enclosing function, unlocking all currently held objects
#define PWX_LOCK_GUARD_CLEAR() PWX_NAMED_LOCK_GUARD_CLEAR(PWX_FUNC)


/** @brief Reset a lock guard to a new value
  *
  * **Important**: Do not use nullptr or NULL for @a object! Use `PWX_NAMED_LOCK_GUARD_CLEAR()` instead!
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param object pointer to the object to reset the lock guard to
**/
#define PWX_NAMED_LOCK_GUARD_RESET( Name, object ) pwx_libpwx_lock_guard_##Name.reset(object)


/** @brief Reset a lock guard to a new value
  *
  * **Important**: Do not use nullptr or NULL for @a object! Use `PWX_LOCK_GUARD_CLEAR()` instead!
  *
  * @param object pointer to the object to reset the lock guard to
**/
#define PWX_LOCK_GUARD_RESET( object ) PWX_NAMED_LOCK_GUARD_RESET(PWX_FUNC, object)


/** @brief Create a lock guard on two given objects, which are unlocked when leaving the current scope
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
**/
#define PWX_NAMED_DOUBLE_LOCK_GUARD( Name, objA, objB ) \
    ::pwx::CLockGuard pwx_libpwx_double_lock_guard_##Name(objA, objB)


/** @brief Create a lock guard on two given objects, which are unlocked when leaving the current scope
  *
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
**/
#define PWX_DOUBLE_LOCK_GUARD( objA, objB ) PWX_NAMED_DOUBLE_LOCK_GUARD(PWX_FUNC, objA, objB)


/** @brief Clear a named double lock guard, unlocking all currently locked objects
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
**/
#define PWX_NAMED_DOUBLE_LOCK_GUARD_CLEAR( Name ) \
    pwx_libpwx_double_lock_guard_##Name.reset(NULL_LOCK, NULL_LOCK, NULL_LOCK)



/// @brief Clear the double lock guard named after the enclosing function, unlocking all currently held objects
#define PWX_DOUBLE_LOCK_GUARD_CLEAR() PWX_NAMED_DOUBLE_LOCK_GUARD_CLEAR(PWX_FUNC)


/** @brief Reset a double lock guard to two new values
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
**/
#define PWX_NAMED_DOUBLE_LOCK_GUARD_RESET( Name, objA, objB ) \
    pwx_libpwx_double_lock_guard_##Name.reset(objA, objB)


/** @brief Reset a double lock guard to two new values
  *
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
**/
#define PWX_DOUBLE_LOCK_GUARD_RESET( objA, objB ) \
    PWX_NAMED_DOUBLE_LOCK_GUARD_RESET(PWX_FUNC, objA, objB)


/** @brief Create a lock guard on three given objects, which are unlocked when leaving the current scope
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
  * @param objC pointer to the second object to lock
**/
#define PWX_NAMED_TRIPLE_LOCK_GUARD( Name, objA, objB, objC )    \
    ::pwx::CLockGuard pwx_libpwx_triple_lock_guard_##Name(objA, objB, objC)


/** @brief Create a lock guard on three given objects, which are unlocked when leaving the current scope
  *
  * @param objA pointer to the first object to lock
  * @param objB pointer to the second object to lock
  * @param objC pointer to the second object to lock
**/
#define PWX_TRIPLE_LOCK_GUARD( objA, objB, objC ) \
    PWX_NAMED_TRIPLE_LOCK_GUARD(PWX_FUNC, objA, objB, objC)


/** @brief Clear a named triple lock guard, unlocking all currently locked objects
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
**/
#define PWX_NAMED_TRIPLE_LOCK_GUARD_CLEAR( Name ) \
    pwx_libpwx_triple_lock_guard_##Name.reset(NULL_LOCK, NULL_LOCK, NULL_LOCK)


/// @brief Clear the triple lock guard named after the enclosing function, unlocking all currently held objects
#define PWX_TRIPLE_LOCK_GUARD_CLEAR() PWX_NAMED_TRIPLE_LOCK_GUARD_CLEAR(PWX_FUNC)


/** @brief Reset a triple lock guard to two new values
  *
  * @param Name a string to add to the local variable name to be able to use more than one guard
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
  * @param objC pointer to the third object to reset the lock guard to
**/
#define PWX_NAMED_TRIPLE_LOCK_GUARD_RESET( Name, objA, objB, objC ) \
    pwx_libpwx_triple_lock_guard_##Name.reset(objA, objB, objC)


/** @brief Reset a triple lock guard to two new values
  *
  * @param objA pointer to the first object to reset the lock guard to
  * @param objB pointer to the second object to reset the lock guard to
  * @param objC pointer to the third object to reset the lock guard to
**/
#define PWX_TRIPLE_LOCK_GUARD_RESET( objA, objB, objC ) \
    PWX_NAMED_TRIPLE_LOCK_GUARD_RESET(PWX_FUNC, objA, objB, objC)


/// @namespace pwx
namespace pwx {


/** @class CLockGuard PLockGuard <PLockGuard>
  * @brief Basic RAII lock guard to lock/unlock one, two or three objects within its ctor/dtor
  *
  * There are the following advantages when using this class instead of
  * doing locks directly:
  *  1. Locking is done in the ctor, unlocking in the dtor automatically.
  *  2. The class is exception free and can handle null pointers.
  *  3. The guard can be assigned or copied, making overlapping locks easy to do.
  *  4. If it is not possible to wait for the destructor, the lock(s) can be unlocked
  * by resetting to nullptr.
**/
class PWX_API CLockGuard {
public:

	/* ===============================================
	 * === Public constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief One object locking constructor
	  *
	  * RAII constructor that returns once @a objA is locked.
	  * **Important**: @a objA must be derived from pwx::CLockable !
	  *
	  * @param[in,out] objA Pointer to the object that is to be locked.
	**/
	explicit
	CLockGuard( CLockable* objA ) noexcept;


	/** @brief One object locking constructor via reference
	  *
	  * RAII constructor that returns once @a objA is locked.
	  * **Important**: @a objA must be derived from pwx::CLockable !
	  *
	  * @param[in,out] objA Reference to the object that is to be locked.
	**/
	explicit
	CLockGuard( CLockable &objA ) noexcept
		  : CLockGuard( &objA ) {}


	/** @brief One object locking constructor ; const wrapper
	  *
	  * RAII constructor that returns once @a objA is locked.
	  * **Important**: @a objA must be derived from pwx::CLockable !
	  *
	  * @param[in,out] objA Const pointer to the object that is to be locked.
	**/
	explicit
	CLockGuard( CLockable const* objA ) noexcept
		  : CLockGuard( const_cast<CLockable*>( objA ) ) {}


	/** @brief Two objects locking constructor
	  *
	  * RAII constructor that returns once @a objA and @a objB are locked.
	  * **Important**: @a objA and @a objB must be derived from pwx::CLockable !
	  *
	  * @param[in,out] objA Pointer to the first object that is to be locked.
	  * @param[in,out] objB Pointer to the second object that is to be locked.
	**/
	CLockGuard( CLockable* objA, CLockable* objB ) noexcept;


	/** @brief Two objects locking constructor via reference
	  *
	  * RAII constructor that returns once @a objA and @a objB are locked.
	  * **Important**: @a objA and @a objB must be derived from pwx::CLockable !
	  *
	  * @param[in,out] objA Reference to the first object that is to be locked.
	  * @param[in,out] objB Reference to the second object that is to be locked.
	**/
	CLockGuard( CLockable &objA, CLockable &objB ) noexcept
		  : CLockGuard( &objA, &objB ) {}


	/** @brief Two objects locking constructor ; const wrapper
	  *
	  * RAII constructor that returns once @a objA and @a objB are locked.
	  * **Important**: @a objA and @a objB must be derived from pwx::CLockable !
	  *
	  * @param[in,out] objA Const pointer to the first object that is to be locked.
	  * @param[in,out] objB Const pointer to the second object that is to be locked.
	**/
	CLockGuard( CLockable const* objA, CLockable const* objB ) noexcept
		  : CLockGuard( const_cast<CLockable*>( objA ), const_cast<CLockable*>( objB ) ) {}


	/** @brief Three objects locking constructor
	  *
	  * RAII constructor that returns once @a objA, @a objB and @a objC are locked.
	  * **Important**: @a objA, @a objB and @a objC must be derived from pwx::CLockable !
	  *
	  * @param[in,out] objA Pointer to the first object that is to be locked.
	  * @param[in,out] objB Pointer to the second object that is to be locked.
	  * @param[in,out] objC Pointer to the third object that is to be locked.
	**/
	CLockGuard( CLockable* objA, CLockable* objB, CLockable* objC ) noexcept;


	/** @brief Three objects locking constructor via reference
	  *
	  * RAII constructor that returns once @a objA, @a objB and @a objC are locked.
	  * **Important**: @a objA, @a objB and @a objC must be derived from pwx::CLockable !
	  *
	  * @param[in,out] objA Reference to the first object that is to be locked.
	  * @param[in,out] objB Reference to the second object that is to be locked.
	  * @param[in,out] objC Reference to the third object that is to be locked.
	**/
	CLockGuard( CLockable &objA, CLockable &objB, CLockable &objC ) noexcept
		  : CLockGuard( &objA, &objB, &objC ) {}


	/** @brief Three objects locking constructor ; const wrapper
	  *
	  * RAII constructor that returns once @a objA, @a objB and @a objC are locked.
	  * **Important**: @a objA, @a objB and @a objC must be derived from pwx::CLockable !
	  *
	  * @param[in,out] objA Const pointer to the first object that is to be locked.
	  * @param[in,out] objB Const pointer to the second object that is to be locked.
	  * @param[in,out] objC Const pointer to the third object that is to be locked.
	**/
	CLockGuard( CLockable const* objA, CLockable const* objB, CLockable const* objC ) noexcept
		  : CLockGuard( const_cast<CLockable*>( objA ), const_cast<CLockable*>( objB ), const_cast<CLockable*>( objC ) ) {}


	/** @brief Copy constructor that takes over the locks from another CLockGuard instance
	  *
	  * Before the objects can be locked, they have to be unlocked by @a src, which
	  * means that there is a tiny window in which another thread might lock any of
	  * these objects.
	  *
	  * @param[in,out] src Reference to the source to copy.
	**/
	CLockGuard( CLockGuard &src ) noexcept;


	/** @brief Copy constructor that takes over the locks from another CLockGuard instance ; const wrapper
	  *
	  * Before the objects can be locked, they have to be unlocked by @a src, which
	  * means that there is a tiny window in which another thread might lock any of
	  * these objects.
	  *
	  * @param[in,out] src Const reference to the source to copy.
	**/
	CLockGuard( CLockGuard const &src ) noexcept
		  : CLockGuard( const_cast<CLockGuard &>( src ) ) {}


	/// @brief The default destructor unlocks all objects currently held locked.
	~CLockGuard() noexcept;

	// No empty ctor
	CLockGuard() PWX_DELETE;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief The assignment operator takes over the locks from another CLockGuard instance
	  *
	  * Before the objects can be locked, they have to be unlocked by @a src, which
	  * means that there is a tiny window in which another thread might lock any of
	  * these objects.
	  *
	  * @param[in,out] src Reference to the source to take over from.
	**/
	CLockGuard &operator=( CLockGuard &&src ) noexcept;


	/* ===============================================
	 * === Public Methods                          ===
	 * ===============================================
	*/

	/** @brief Unlock all objects, and switch to only lock @a objA instead.
	  * @param[in,out] objA Pointer to the object that is to be locked.
	**/
	void reset( CLockable* objA ) noexcept;


	/** @brief Unlock all objects, and switch to only lock @a objA instead ; const wrapper.
	  * @param[in,out] objA Const pointer to the object that is to be locked.
	**/
	void reset( CLockable const* objA ) noexcept {
		this->reset( const_cast<CLockable*>( objA ) );
	}


	/** @brief Unlock all objects, and switch to lock @a objA and @a objB instead.
	  * @param[in,out] objA Pointer to the first object that is to be locked.
	  * @param[in,out] objB Pointer to the second object that is to be locked.
	**/
	void reset( CLockable* objA, CLockable* objB ) noexcept;


	/** @brief Unlock all objects, and switch to lock @a objA and @a objB instead ; const wrapper.
	  * @param[in,out] objA Const pointer to the first object that is to be locked.
	  * @param[in,out] objB Const pointer to the second object that is to be locked.
	**/
	void reset( CLockable const* objA, CLockable const* objB ) noexcept {
		this->reset( const_cast<CLockable*>( objA ), const_cast<CLockable*>( objB ) );
	}


	/** @brief Unlock all objects, and switch to only lock @a objA, @a objB and @a objC instead.
	  * @param[in,out] objA Pointer to the first object that is to be locked.
	  * @param[in,out] objB Pointer to the second object that is to be locked.
	  * @param[in,out] objC Pointer to the third object that is to be locked.
	**/
	void reset( CLockable* objA, CLockable* objB, CLockable* objC ) noexcept;


	/** @brief Unlock all objects, and switch to only lock @a objA, @a objB and @a objC instead ; const wrapper.
	  * @param[in,out] objA Const pointer to the first object that is to be locked.
	  * @param[in,out] objB Const pointer to the second object that is to be locked.
	  * @param[in,out] objC Const pointer to the third object that is to be locked.
	**/
	void reset( CLockable const* objA, CLockable const* objB, CLockable const* objC ) noexcept {
		this->reset( const_cast<CLockable*>( objA ), const_cast<CLockable*>( objB ), const_cast<CLockable*>( objC ) );
	}


private:

	/* ===============================================
	 * === Private Members                         ===
	 * ===============================================
	*/

	CLockable* l_a = nullptr;
	CLockable* l_b = nullptr;
	CLockable* l_c = nullptr;
};


} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_CLOCKGUARD_H_INCLUDED
