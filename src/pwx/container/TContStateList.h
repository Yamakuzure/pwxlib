#pragma once
#ifndef PWX_LIBPWX_PWX_CONTAINER_TCONTSTATELIST_H_INCLUDED
#define PWX_LIBPWX_PWX_CONTAINER_TCONTSTATELIST_H_INCLUDED 1

/** @file TContStateList.h
  *
  * @brief Definition of a list to manage instances of TContState instances
  *
  * (c) 2007 - 2012 PrydeWorX
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

#include <pwx/types/CLockable.h>
#include <pwx/types/TContState.h>
#include <pwx/types/TSingleElement.h>
#include <pwx/general/macros.h>

namespace pwx {

/** @class TContStateList
  *
  * @brief Internal container to track per-thread container states
**/
template<typename list_elem_t>
class TContStateList : public CLockable
{
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	 */

	typedef TContState<list_elem_t> data_t;
	typedef TSingleElement<data_t>  elem_t;
#if defined(PWX_THREADS)
	typedef ::std::thread::id thId_t;
#else
	typedef uint32_t thId_t;
#endif

	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/// @brief default ctor - initialize the list with one entry for this thread
	explicit TContStateList()
	{
		try {
			head = new elem_t(new data_t());
			curr = head;
			tail = head;
		}
		PWX_CATCH_AND_FORGET(::std::exception)
	}

	/// @brief default dtor - deletes all elements
	~TContStateList() noexcept
	{
		while (eCount)
			clear();
	}


	/* ===============================================
	 * === Public methods						  ===
	 * ===============================================
	 */

	/// @brief clear the list of all items
	void clear() noexcept
	{
		try {
			PWX_LOCK_NOEXCEPT(this)
			while (eCount) {
				curr = head->next;
				delete head;
				head = curr;
				--eCount;
			}
			head = nullptr;
			curr = nullptr;
			tail = nullptr;
			PWX_UNLOCK_NOEXCEPT(this)
		}
		PWX_CATCH_AND_FORGET(::std::exception)
	}


	/// @brief get the tracker for the calling thread. Create one if it does not exist, yet.
	data_t* getState() noexcept
	{
		data_t* result = nullptr;
#if defined(PWX_THREADS)
		thId_t wanted = ::std::this_thread::get_id();

		try {
			PWX_LOCK_NOEXCEPT(this)

			if (eCount) {
				curr = head;
				while (curr && ((**curr).thId != wanted))
					curr = curr->next;
			}

			if (curr && ((**curr).thId == wanted))
				result = curr->data.get();
			else {
				result = new data_t();
				elem_t* newElement = new elem_t(result);
				if (tail)
					tail->next = newElement;
				tail = newElement;
				if (!head)
					head = newElement;
				++eCount;
			}

			PWX_UNLOCK_NOEXCEPT(this)
		}
		PWX_CATCH_AND_FORGET(::std::exception)
#else
		if (head)
			result = head->data.get();
#endif

		return result;
	}


	/// @brief call this method whenever an element is inserted or deleted
	void resetStates() noexcept
	{
#if defined(PWX_THREADS)
		PWX_LOCK_NOEXCEPT(this)
		curr = head;
		while (curr) {
			(**curr).reseted = true;
			curr = curr->next;
		}
		PWX_UNLOCK_NOEXCEPT(this)
#else
		if (head)
			**head.reseted = true;
#endif
	}


private:

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	uint32_t eCount = 0;
	elem_t*  head   = nullptr;
	elem_t*  curr   = nullptr;
	elem_t*  tail   = nullptr;

}; // class TContStateList


} // namespace pwx
#endif // PWX_LIBPWX_PWX_CONTAINER_TCONTSTATELIST_H_INCLUDED
