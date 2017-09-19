#ifndef PWX_LIBPWX_PWX_INTERNAL_CTHREADELEMENTSTORE_H_INCLUDED
#define PWX_LIBPWX_PWX_INTERNAL_CTHREADELEMENTSTORE_H_INCLUDED

/** @file CThreadElementStore.h
  *
  * @brief Store for the currently handled element by each thread
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


#include "TOpenHash.h"


namespace pwx {

namespace private_ {


/** @internal
  * @brief Hash based store for thread individual element handling
  *
  * This class is used by all list based containers to store
  * the currently handled element for each thread.
  *
  * For this to work there is an important rule:
  * Any container methods that removes an element from the container
  * using this storage <B>must</B> report this to their element
  * store instance using its invalidate() method.
  * The default elements already handle this if their insert() and
  * remove() methods are used. But if an element is inserted manually,
  * and no store is registered, or if an element is removed manually,
  * the invalidation must be done, too.
  * Failure to do so might result in a thread trying to work with
  * an element that has been moved to a different container, or
  * worse, has been deleted.
  *
  * The other methods are curr(), which will return the currently
  * stored element for the calling thread and curr(elem) which will
  * store a new element for the calling thread.
  *
  * If beThreadSafe(false) is called, the storage will no longer
  * use the internal hash table but simply change/retrieve on general
  * curr pointer. This can be reversed using beThreadSafe(true).
**/
class CThreadElementStore : public CLockable
{
public:

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef CLockable                    base_t;
	typedef CThreadElementStore          store_t;
	typedef VElement                     curr_t;
	typedef TOpenHash<size_t, curr_t>    hash_t;
	typedef THashElement<size_t, curr_t> elem_t;


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	explicit
	CThreadElementStore(uint32_t initial_size) PWX_API;
	CThreadElementStore()             noexcept PWX_API;
	virtual ~CThreadElementStore()    noexcept PWX_API;


	// No copying:
    CThreadElementStore(CThreadElementStore&) PWX_DELETE;
    CThreadElementStore &operator=(const CThreadElementStore&) PWX_DELETE;


	/* ===============================================
	 * === Public Methods                          ===
	 * ===============================================
	*/

	void    clear()                                  noexcept PWX_API;
	curr_t* curr()                             const noexcept PWX_API;
	curr_t* curr()                                   noexcept PWX_API;
	void    curr(const curr_t* new_curr)       const noexcept PWX_API;
	void    curr(curr_t* new_curr)                   noexcept PWX_API;
	void    disable_thread_safety()                  noexcept PWX_API;
	void    enable_thread_safety()                   noexcept PWX_API;
	void    invalidate(const curr_t* old_curr) const noexcept PWX_API;
	void    invalidate(curr_t* old_curr)             noexcept PWX_API;


protected:

	using base_t::memOrdLoad;
	using base_t::memOrdStore;


private:

	/* ===============================================
	 * === Private Members                         ===
	 * ===============================================
	*/

	mutable
	hash_t  currs;                                 //!< Used when thread saftey is enabled (default)
	mutable
	abool_t invalidating = ATOMIC_VAR_INIT(false); //!< If set to true by invalidate(), curr() wait for a lock
	mutable
	curr_t* oneCurr      = nullptr;                //!< Used when thread safety is disabled
};


} // namespace private

} // namespace pwx

#endif // PWX_LIBPWX_PWX_INTERNAL_CTHREADELEMENTSTORE_H_INCLUDED

