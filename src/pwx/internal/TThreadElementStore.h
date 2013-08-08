#ifndef PWX_LIBPWX_PWX_TYPES_TTHREADELEMENTSTORE_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_TTHREADELEMENTSTORE_H_INCLUDED

/** @file TThreadElementStore.h
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
	  * @brief do nothing with the given element
	  * This is just a little "nothing-to-see-here"-function
	  * that is used by the TThreadElementStore below as
	  * a deleter for shared_ptr.
	**/
	template<typename ptr_t>
	void do_not_destroy (ptr_t*)
		{ /* Don't you dare! */ }


/** @internal
  * @brief Hash based store for thread individual element handling
  *
  * This template is used by all list based containers to store
  * the currently handled element for each thread.
  *
  * For this to work there is an important rule:
  * any container methods that remove an element from the container
  * using this storage <B>must</B> report this to their element
  * store instance using its invalidateElement() method.
  * Failure to do so might result in a thread trying to work with
  * an element that has been moved to a different container, or
  * worse, has been deleted.
  *
  * The other two methods are curr(), which will return the currently
  * stored element for the calling thread and curr(elem) which will
  * store a new element for the calling thread.
  *
  * If beThreadSafe(false) is called, the storage will no longer
  * use the internal hash table but simply change/retrieve on general
  * curr pointer. This can be reversed using beThreadSafe(true).
**/
template<typename curr_t>
class PWX_API TThreadElementStore : public CLockable
{
public:

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef CLockable                    base_t;
	typedef TThreadElementStore<curr_t>  store_t;
	typedef TOpenHash<size_t, curr_t>    hash_t;
	typedef THashElement<size_t, curr_t> elem_t;


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default ctor
	  *
	  * Set the initial size of the used hash table here.
	  * The default initial size is 47, which is a prime
	  * number almost exactly between 2^5 and 2^6.
	  *
	  * If you intent to use a different size, please try
	  * to find a prime number that is as exactly between
	  * 2^x and 2^(x+1) as possible.
	  *
	  * @param[in] initial_size the size to use for the hash table
	**/
	explicit TThreadElementStore(uint32_t initial_size) noexcept :
		currs(initial_size, private_::do_not_destroy, nullptr, 0.6667, 2.063829787234043)
	{ }

	/// @brief delegating ctor setting default size of 47
	TThreadElementStore() noexcept : store_t(47) { }


	/// @brief default dtor
	virtual ~TThreadElementStore() noexcept
	{
		currs.clear();
	}


	// No copying:
    TThreadElementStore(TThreadElementStore&) PWX_DELETE;
    TThreadElementStore &operator=(const TThreadElementStore&) PWX_DELETE;


	/* ===============================================
	 * === Public Methods                          ===
	 * ===============================================
	*/

	/// @brief return the calling threads current element
	curr_t* curr() const noexcept
	{
		if (beThreadSafe()) {
			elem_t* elem = currs.get(CURRENT_THREAD_ID);
			return elem ? elem->data.get() : nullptr;
		} else
			return oneCurr;
	}


	/// @brief return the calling threads current element
	/// Note: Copied, not called, as these two lines are
	///       too less of a burdon to add a const_cast
	///       calling layer
	curr_t* curr() noexcept
	{
		if (beThreadSafe()) {
			elem_t* elem = currs.get(CURRENT_THREAD_ID);
			return elem ? elem->data.get() : nullptr;
		} else
			return oneCurr;
	}


	/// @brief delete old element and add a new one unless @a new_curr is nullptr
	void curr(const curr_t* new_curr) const noexcept
	{
		if (beThreadSafe()) {
			currs.delKey(CURRENT_THREAD_ID);
			if (new_curr) {
				PWX_TRY(currs.add(CURRENT_THREAD_ID, const_cast<curr_t*>(new_curr)))
				PWX_CATCH_AND_FORGET(CException)
			}
		} else
			oneCurr = const_cast<curr_t*>(new_curr);
	}


	/// @brief delete old element and add a new one unless @a new_curr is nullptr
	void curr(curr_t* new_curr) noexcept
	{
		if (beThreadSafe()) {
			currs.delKey(CURRENT_THREAD_ID);
			if (new_curr) {
				PWX_TRY(currs.add(CURRENT_THREAD_ID, new_curr))
				PWX_CATCH_AND_FORGET(CException)
			}
		} else
			oneCurr = new_curr;
	}


	/// @brief stop using the hash table, maintain one pointer directly
	void disable_thread_safety()
	{
		if (beThreadSafe()) {
			oneCurr = nullptr;
			beThreadSafe(false);
			memOrdLoad  = PWX_MEMORDER_RELAXED;
			memOrdStore = PWX_MEMORDER_RELAXED;
		}
	}


	/// @brief stop maintaining one pointer, use the hash table
	void enable_thread_safety()
	{
		if (!beThreadSafe()) {
			memOrdLoad  = PWX_MEMORDER_ACQUIRE;
			memOrdStore = PWX_MEMORDER_RELEASE;
			this->currs.clear();
			beThreadSafe(true);
		}
	}


	/// @brief delete all entries that point to @a old_curr
	void invalidateElement(const curr_t* old_curr) const noexcept
	{
		if (beThreadSafe()) {
			uint32_t currSize = currs.sizeMax();
			elem_t*  elem     = nullptr;
			for (uint32_t i = 0; i < currSize; ++i) {
				elem = currs[i];
				if (elem && (elem->data.get() == old_curr))
					currs.delKey(elem->key);
			}
		} else if (oneCurr && (oneCurr == old_curr))
			oneCurr = nullptr;
	}


	/// @brief delete all entries that point to @a old_curr
	void invalidateElement(curr_t* old_curr) noexcept
	{
		this->invalidateElement(static_cast<const curr_t*>(old_curr));
	}


protected:

	using base_t::memOrdLoad;
	using base_t::memOrdStore;


private:

	/* ===============================================
	 * === Private Members                         ===
	 * ===============================================
	*/

	mutable
	hash_t  currs;                                //!< Used when thread saftey is enabled (default)
	mutable
	curr_t* oneCurr      = nullptr;               //!< Used when thread safety is disabled
};

} // namespace private

} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_TTHREADELEMENTSTORE_H_INCLUDED

