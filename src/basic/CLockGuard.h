#ifndef PWX_LIBPWX_PWX_TYPES_CLOCKGUARD_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_CLOCKGUARD_H_INCLUDED

/** @file CLockGuard.h
  *
  * @brief Simple Class to RAII-lock up to 3 objects derived from pwx::Clockable
  *
  * This class have been created to be able to RAII-autolock
  * up to three objects at once. This is helpful handling
  * doubly linked list based containers and elements.
  *
  * (c) 2007 - 2017 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
  *
  * The PrydeWorX Library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public License as
  * published by the Free Software Foundation; either version 2.1 of the
  * License, or (at your option) any later version.
  *
  * The PrydeWorX Library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public License
  * along with pwxLib; If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
**/


#include "CLockable.h"


namespace pwx {


/** @class CLockGuard
  * @brief Basic RAII lock guard to lock/unlock one, two or three objects within its ctor/dtor
  *
  * There are the following advantages when using this class instead of
  * doing locks directly:
  * <list=1><li>Locking is done in the ctor, unlocking in the dtor automatically.</li>
  * <li>The class is exception free and can handle null pointers.</li>
  * <li>The guard can be assigned or copied, making overlapping locks easy to do.</li>
  * <li>If it is not possible to wait for the destructor, the lock(s) can be unlocked
  * by resetting to nullptr.</li></list>
**/
class PWX_API CLockGuard
{
public:

	/* ===============================================
	 * === Public constructors and destructors     ===
	 * ===============================================
	*/

	CLockGuard(const CLockable* objA) noexcept;
	CLockGuard(const CLockable* objA, const CLockable* objB) noexcept;
	CLockGuard(const CLockable* objA, const CLockable* objB, const CLockable* objC) noexcept;
	CLockGuard(const CLockGuard &src) noexcept;
	~CLockGuard() noexcept;

	// No empty ctor
	CLockGuard() PWX_DELETE;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	CLockGuard &operator=(const CLockGuard &src) noexcept;


	/* ===============================================
	 * === Public Methods                          ===
	 * ===============================================
	*/

	void reset(const CLockable* objA) noexcept;
	void reset(const CLockable* objA, const CLockable* objB) noexcept;
	void reset(const CLockable* objA, const CLockable* objB, const CLockable* objC) noexcept;


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

