#pragma once
#ifndef PWX_LIBPWX_PWX_TYPES_TCONTSTATE_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_TCONTSTATE_H_INCLUDED 1

/** @file TContState.h
  *
  * @brief Definition of a template to track container states per thread.
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
#if defined(PWX_THREADS)
#  include <thread>
#endif

namespace pwx {

/** @struct TContState
  *
  * @brief Little container used for per-thread state tracking in all pwxLib containers
  *
  * When an object is created the member this->thId is initialized
  * with the creating threads id. If PWX_THREADS is not defined, the
  * id is always '1'.
  *
  * The members this->curr and this->eNr can be used to track the
  * state of a container.
  *
  * for this to work all containers hold a TContStateList that is used
  * retrieve the correct TContState instance for each thread.
  *
  * @see TContStateList for further information.
**/
template<typename elem_t>
struct TContState
{
public:

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	 */

#if defined(PWX_THREADS)
	typedef ::std::thread::id thId_t;
#else
	typedef uint32_t thId_t;
#endif


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	 */

	/// @brief default ctor, initializes an empty TContState for the current thread
	explicit TContState()
#if defined(PWX_THREADS)
	: thId(::std::this_thread::get_id())
#else
	: thId(0)
#endif
	{ }

	TContState(const TContState&) PWX_DELETE;
	~TContState() PWX_DEFAULT;


	/* ===============================================
	 * === Public members						  ===
	 * ===============================================
	 */

	elem_t*  curr    = nullptr;
	uint32_t eNr     = 0;
	thId_t   thId;
	bool     reseted = true;
}; // class TContState


} // namespace pwx
#endif // PWX_LIBPWX_PWX_TYPES_TCONTSTATE_H_INCLUDED

