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
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
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


#include "basic/CLockable.h"


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
        CLockGuard( CLockable const* objA ) noexcept;


        /** @brief Two objects locking constructor
          *
          * RAII constructor that returns once @a objA and @a objB are locked.  
          * **Important**: @a objA and @a objB must be derived from pwx::CLockable !
          *
          * @param[in,out] objA Pointer to the first object that is to be locked.
          * @param[in,out] objB Pointer to the second object that is to be locked.
        **/
        CLockGuard( CLockable const* objA, CLockable const* objB ) noexcept;


        /** @brief Three objects locking constructor
          *
          * RAII constructor that returns once @a objA, @a objB and @a objC are locked.  
          * **Important**: @a objA, @a objB and @a objC must be derived from pwx::CLockable !
          *
          * @param[in,out] objA Pointer to the first object that is to be locked.
          * @param[in,out] objB Pointer to the second object that is to be locked.
          * @param[in,out] objC Pointer to the third object that is to be locked.
        **/
        CLockGuard( CLockable const* objA, CLockable const* objB, CLockable const* objC ) noexcept;


        /** @brief Copy constructor that takes over the locks from another CLockGuard instance
          *
          * Before the objects can be locked, they have to be unlocked by @a src, which
          * means that there is a tiny window in which another thread might lock any of
          * these objects.
          *
          * @param[in,out] src Reference to the source to copy.
        **/
        CLockGuard( CLockGuard const &src ) noexcept;


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
        CLockGuard &operator=( CLockGuard const &src ) noexcept;


        /* ===============================================
         * === Public Methods                          ===
         * ===============================================
        */

        /** @brief Unlock all objects, and switch to only lock @a objA instead.
          * @param[in,out] objA Pointer to the object that is to be locked.
        **/
        void reset( CLockable const* objA ) noexcept;


        /** @brief Unlock all objects, and switch to lock @a objA and @a objB instead.
          * @param[in,out] objA Pointer to the first object that is to be locked.
          * @param[in,out] objB Pointer to the second object that is to be locked.
        **/
        void reset( CLockable const* objA, CLockable const* objB ) noexcept;


        /** @brief Unlock all objects, and switch to only lock @a objA, @a objB and @a objC instead.
          * @param[in,out] objA Pointer to the first object that is to be locked.
          * @param[in,out] objB Pointer to the second object that is to be locked.
          * @param[in,out] objC Pointer to the third object that is to be locked.
        **/
        void reset( CLockable const* objA, CLockable const* objB, CLockable const* objC ) noexcept;


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
