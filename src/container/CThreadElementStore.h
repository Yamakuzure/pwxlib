#ifndef PWX_LIBPWX_PWX_INTERNAL_CTHREADELEMENTSTORE_H_INCLUDED
#define PWX_LIBPWX_PWX_INTERNAL_CTHREADELEMENTSTORE_H_INCLUDED

/** @file CThreadElementStore.h
  *
  * @brief Store for the currently handled element by each thread
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

#include "container/TOpenHash.h"


/// @namespace pwx
namespace pwx {


/** @class CThreadElementStore
  * @brief Hash based store for thread individual element handling
  *
  * This class is used by all list based containers to store
  * the currently handled element for each thread.
  *
  * For this to work there is an important rule:
  * Any container methods that removes an element from the container
  * using this storage **must** report this to their element
  * store instance using its `invalidate()` method.
  * The default elements already handle this if their `insert()` and
  * `remove()` methods are used. But if an element is inserted manually,
  * and no store is registered, or if an element is removed manually,
  * the invalidation must be done, too.
  * Failure to do so might result in a thread trying to work with
  * an element that has been moved to a different container, or
  * worse, has been deleted.
  *
  * The other methods are `curr()`, which will return the currently
  * stored element for the calling thread and `curr(elem)` which will
  * store a new element for the calling thread.
  *
  * If `beThreadSafe(false)` is called, the storage will no longer
  * use the internal hash table but simply change/retrieve on general
  * `curr` pointer. This can be reversed using `beThreadSafe(true)`.
**/
class CThreadElementStore final : public CLockable {
public:

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef CLockable                      base_t;  //!< Base type of `CThreadElementStore`
	typedef CThreadElementStore            store_t; //!< Storage type, thus `CThreadElementStore` itself
	typedef VElement                       curr_t;  //!< Type of the `curr` element to handle
	typedef TOpenHash< size_t, curr_t >    hash_t;  //!< Hash container type with `size_t` and `curr_t`
	typedef THashElement< size_t, curr_t > elem_t;  //!< Hash element type with `size_t` and `curr_t`


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default ctor
	  *
	  * Set the initial size of the used hash table here.
	  * The default initial size is 47, which is a prime number almost exactly between 2^5 and 2^6.
	  *
	  * If you intent to use a different size, please try to find a prime number that is as exactly between
	  * 2^x and 2^(x+1) as possible.
	  *
	  * @param[in] initial_size the size to use for the hash table
	**/
	explicit
	CThreadElementStore( uint32_t initial_size ) PWX_API;

	CThreadElementStore() noexcept              PWX_API;
	~CThreadElementStore() noexcept final
	PWX_API;


	// No copying:
	CThreadElementStore( CThreadElementStore & ) PWX_DELETE;
	CThreadElementStore( CThreadElementStore && ) PWX_DELETE;
	CThreadElementStore &operator=( CThreadElementStore const & ) PWX_DELETE;
	CThreadElementStore &operator=( CThreadElementStore const && ) PWX_DELETE;


	/* ===============================================
	 * === Public Methods                          ===
	 * ===============================================
	*/

	/// @brief remove all elements
	void clear() noexcept PWX_API;

	/// @brief return the calling threads current element
	curr_t* curr() const noexcept PWX_API;

	/// @brief return the calling threads current element
	curr_t* curr() noexcept PWX_API;

	/// @brief delete old element and add a new one unless const @a new_curr is nullptr
	void curr( const curr_t* new_curr ) const noexcept PWX_API;

	/// @brief delete old element and add a new one unless @a new_curr is nullptr
	void curr( curr_t* new_curr ) noexcept PWX_API;

	/// @brief stop using the hash table, maintain one pointer directly
	void disable_thread_safety() noexcept PWX_API;

	/// @brief stop maintaining one pointer, use the hash table
	void enable_thread_safety() noexcept PWX_API;

	/// @brief delete all entries that point to const @a old_curr
	void invalidate( const curr_t* old_curr ) const noexcept PWX_API;

	/// @brief delete all entries that point to @a old_curr
	[[maybe_unused]] void invalidate( curr_t* old_curr ) const noexcept PWX_API;


protected:

	using base_t::memOrdLoad;
	using base_t::memOrdStore;


private:

	/* ===============================================
	 * === Private Members                         ===
	 * ===============================================
	*/

	mutable hash_t  currs;                   //!< Used when thread safety is enabled (default)
	mutable abool_t invalidating{ false }; //!< If set to true by `invalidate()`, `curr()` will wait for a lock
	mutable curr_t* oneCurr{ nullptr };      //!< Used when thread safety is disabled
};


} // namespace pwx


#endif // PWX_LIBPWX_PWX_INTERNAL_CTHREADELEMENTSTORE_H_INCLUDED

