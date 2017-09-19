/***
  This file is part of the PrydeWorX Library (pwxLib).

  Copyright 2007 - 2017 Sven Eden

  The PrydeWorX Library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  The PrydeWorX Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with pwxLib; If not, see <http://www.gnu.org/licenses/>.

  History and Changelog are maintained in pwx.h
***/


#include "CThreadElementStore.h"


namespace pwx {

namespace private_ {


/** @internal
  * @brief do nothing with the given element
  * This is just a little "nothing-to-see-here"-function
  * that is used by the CThreadElementStore below as
  * a deleter for shared_ptr.
**/
static void do_not_destroy (CThreadElementStore::curr_t*)
	{ /* Don't you dare! */ }


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
CThreadElementStore::CThreadElementStore(uint32_t initial_size) :
	currs(initial_size, do_not_destroy, nullptr, 0.6667, 2.063829787234043)
{ }

/// @brief delegating ctor setting default size of 47
CThreadElementStore::CThreadElementStore() noexcept : store_t(47) { }


/// @brief default dtor
CThreadElementStore::~CThreadElementStore() noexcept
{
	currs.clear();
}


/* ===============================================
 * === Public Methods                          ===
 * ===============================================
*/


/// @brief remove all elements:
void CThreadElementStore::clear() noexcept
{
	if (beThreadSafe()) {
		if (currs.size() && !invalidating.load(memOrdLoad)) {
			invalidating.store(true, memOrdStore);
			PWX_LOCK_GUARD(this)
			// Is a clearing still needed?
			if (currs.size())
				currs.clear();
			invalidating.store(false, memOrdStore);
		}
	} else
		oneCurr = nullptr;
}


/// @brief return the calling threads current element
CThreadElementStore::curr_t* CThreadElementStore::curr() const noexcept
{
	if (beThreadSafe()) {
		bool needLock = invalidating.load(memOrdLoad);
		if (needLock) const_cast<CThreadElementStore*>(this)->lock();
		elem_t* elem = currs.get(CURRENT_THREAD_ID);
		if (needLock) const_cast<CThreadElementStore*>(this)->unlock();
		return elem ? elem->data.get() : nullptr;
	} else
		return oneCurr;
}


/// @brief return the calling threads current element
/// Note: Copied, not called, as these two lines are
///       too less of a burden to add a const_cast
///       calling layer
CThreadElementStore::curr_t* CThreadElementStore::curr() noexcept
{
	if (beThreadSafe()) {
		bool needLock = invalidating.load(memOrdLoad);
		if (needLock) lock();
		elem_t* elem = currs.get(CURRENT_THREAD_ID);
		if (needLock) unlock();
		return elem ? elem->data.get() : nullptr;
	} else
		return oneCurr;
}


/// @brief delete old element and add a new one unless @a new_curr is nullptr
void CThreadElementStore::curr(const CThreadElementStore::curr_t* new_curr) const noexcept
{
	if (beThreadSafe()) {
		bool needLock = invalidating.load(memOrdLoad);
		if (needLock) const_cast<CThreadElementStore*>(this)->lock();
		currs.delKey(CURRENT_THREAD_ID);
		if (new_curr) {
			PWX_TRY(currs.add(CURRENT_THREAD_ID, const_cast<curr_t*>(new_curr)))
			PWX_CATCH_AND_FORGET(CException)
		}
		if (needLock) const_cast<CThreadElementStore*>(this)->unlock();
	} else
		oneCurr = const_cast<curr_t*>(new_curr);
}


/// @brief delete old element and add a new one unless @a new_curr is nullptr
void CThreadElementStore::curr(CThreadElementStore::curr_t* new_curr) noexcept
{
	if (beThreadSafe()) {
		bool needLock = invalidating.load(memOrdLoad);
		if (needLock) lock();
		currs.delKey(CURRENT_THREAD_ID);
		if (new_curr) {
			PWX_TRY(currs.add(CURRENT_THREAD_ID, new_curr))
			PWX_CATCH_AND_FORGET(CException)
		}
		if (needLock) unlock();
	} else
		oneCurr = new_curr;
}


/// @brief stop using the hash table, maintain one pointer directly
void CThreadElementStore::disable_thread_safety() noexcept
{
	if (beThreadSafe()) {
		oneCurr = nullptr;
		beThreadSafe(false);
	}
}


/// @brief stop maintaining one pointer, use the hash table
void CThreadElementStore::enable_thread_safety() noexcept
{
	if (!beThreadSafe()) {
		currs.clear();
		beThreadSafe(true);
	}
}


/// @brief delete all entries that point to @a old_curr
void CThreadElementStore::invalidate(const CThreadElementStore::curr_t* old_curr) const noexcept
{
	if (beThreadSafe()) {
		// Only do anything if there are any elements stored:
		if (currs.size() && !invalidating.load(memOrdLoad)) {
			invalidating.store(true, memOrdLoad);
			PWX_LOCK_GUARD(this)

			// Do a double check, maybe the invalidating is no longer needed
			if (currs.size()) {
				uint32_t currSize = currs.sizeMax();
				elem_t*  elem     = nullptr;
				for (uint32_t i = 0; i < currSize; ++i) {
					elem = currs[i];
					if (elem && (elem->data.get() == old_curr))
						currs.delKey(elem->key);
				}
			} // End of having to invalidate
			invalidating.store(false, memOrdStore);
		} // End of entry check
	} else if (oneCurr && (oneCurr == old_curr))
		oneCurr = nullptr;
}


/// @brief delete all entries that point to @a old_curr
void CThreadElementStore::invalidate(CThreadElementStore::curr_t* old_curr) noexcept
{
	this->invalidate(static_cast<const curr_t*>(old_curr));
}


} // namespace private_

} // namespace pwx
