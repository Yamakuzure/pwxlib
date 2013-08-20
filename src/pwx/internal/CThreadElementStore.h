#ifndef PWX_LIBPWX_PWX_INTERNAL_CTHREADELEMENTSTORE_H_INCLUDED
#define PWX_LIBPWX_PWX_INTERNAL_CTHREADELEMENTSTORE_H_INCLUDED

/** @file CThreadElementStore.h
  *
  * @brief Store for the currently handled element by each thread
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

#include <pwx/container/TOpenHash.h>

namespace pwx {

namespace private_ {

/** @internal
  * @brief Hash based store for thread individual element handling
  *
  * This class is used by all list based containers to store
  * the currently handled element for each thread.
  *
  * For this to work there is an important rule:
  * any container methods that remove an element from the container
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
class PWX_API CThreadElementStore : public CLockable
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

	explicit CThreadElementStore(uint32_t initial_size) noexcept;
	CThreadElementStore() noexcept;
	virtual ~CThreadElementStore() noexcept;


	// No copying:
    CThreadElementStore(CThreadElementStore&) PWX_DELETE;
    CThreadElementStore &operator=(const CThreadElementStore&) PWX_DELETE;


	/* ===============================================
	 * === Public Methods                          ===
	 * ===============================================
	*/

	void    clear()                                  noexcept;
	curr_t* curr()                             const noexcept;
	curr_t* curr()                                   noexcept;
	void    curr(const curr_t* new_curr)       const noexcept;
	void    curr(curr_t* new_curr)                   noexcept;
	void    disable_thread_safety()                  noexcept;
	void    enable_thread_safety()                   noexcept;
	void    invalidate(const curr_t* old_curr) const noexcept;
	void    invalidate(curr_t* old_curr)             noexcept;


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

