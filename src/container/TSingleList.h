#ifndef PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED
#define PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED 1
#pragma once

/** @file TSingleList.h
  *
  * @brief Declaration of a basic template for singly linked lists
  *
  * (c) 2007 - 2020 PrydeWorX
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


#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"

#include "container/CThreadElementStore.h"
#include "container/TSingleElement.h"
#include "container/VContainer.h"


/// @namespace pwx
namespace pwx {


/** @class TSingleList PSingleList <PSingleList>
  *
  * @brief Template to build singly linked lists of variable types
  *
  * The singly linked list is the most basic container. Elements of type
  * `pwx::TSingleElement<T>` are chained together using their next pointers.
  *
  * The data is held using shared pointers. So if you delete an element from
  * this list which data is referenced in another container, the data won't
  * be deleted until no other container references it.
  *
  * The constructor takes an optional `destroy(T*)` function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * This container is designed to work safely in a multi-threaded environment.
  * See below for more on single and multi threaded usage.
  *
  * It is recommended that you use the much more advanced `std::list`, unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the `std::list`.
  *
  * **Notes on multi threaded environments**
  *
  * If you plan to use this container in a strictly single-threaded way, you can
  * turn off most of the thread safety measures with `disable_thread_safety()`.
**/
template<typename data_t, typename elem_t = TSingleElement<data_t> >
class PWX_API TSingleList : public VContainer {
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef VContainer                    base_t;     //!< Base type of the list
	typedef TSingleList<data_t, elem_t>   list_t;     //!< Type of this list
	typedef typename elem_t::neighbor_t   neighbor_t; //!< Type of elements neighbours, used for curr, head and tail.
	typedef private_::CThreadElementStore store_t;    //!< Storage for the thread id bound curr pointer


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor initializes an empty list.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TSingleList ( void ( *destroy_ ) ( data_t* data_ ) ) noexcept
		: destroy ( destroy_ )
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor uses the default constructor to set the data
	  * destroy method to the null pointer.
	**/
	TSingleList() noexcept
		: list_t ( nullptr )
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * If a new element can not be created, a `pwx::CException`
	  * with the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference of the list to copy.
	**/
	TSingleList ( list_t const& src ) :
		base_t ( src ) {
		operator+=( src );
	}


	/** @brief default destructor
	  *
	  * This destructor will delete all elements currently stored. There is no
	  * need to clean up manually before deleting the list.
	**/
	virtual ~TSingleList() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	/** @brief delete all elements
	  *
	  * This is a quick way to get rid of all elements
	  * at once. If a `destroy()` function was set, it is
	  * used for the data deletion. Otherwise it is
	  * assumed that data_t responds to the delete
	  * operator.
	**/
	virtual void clear() noexcept {
		if ( !destroyed() )
			this->privClear();
	}


	/** @brief delete the element after the element holding the specified data
	  *
	  * This method deletes the element in the list after the element
	  * that holds @a prev.
	  *
	  * If @a prev is set to `nullptr`, the root element (aka head) is
	  * deleted.
	  *
	  * If you intent to work with the element, use `remNext()` instead.
	  *
	  * Nothing happens if there is no element behind the element holding @a prev.
	  * If the deletion of the data throws an exception, it will be turned
	  * into a `pwx::CException` and passed on.
	  *
	  * @param[in] prev the data the element that precedes the element to delete holds
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delNext ( data_t* prev ) {
		PWX_TRY_PWX_FURTHER( return protDelete( remNext( prev ) ) )
	}


	/** @brief delete the element after the specified element
	  *
	  * This method deletes the element in the list after the element
	  * @a prev.
	  *
	  * If @a prev is set to `nullptr`, the root element (aka head) is
	  * deleted.
	  *
	  * If you intent to work with the element, use `remNextElem()` instead.
	  *
	  * If @a prev is not an element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * Nothing happens if there is no element behind the element holding @a prev.
	  * If the deletion of the data throws an exception, it will be turned
	  * into a `pwx::CException` and passed on.
	  *
	  * @param[in] prev the element that precedes the element to delete
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delNextElem ( elem_t* prev ) {
		PWX_TRY_PWX_FURTHER( return protDelete( remNextElem( prev ) ) )
	}


	/** @brief disable thread safety
	  *
	  * This method disables all thread safety measures.
	  *
	  * **Warning**: It is completely unchecked whether the
	  * container is used by more than one thread. If concurrent
	  * threads work with this container while this method is
	  * called, the outcome is unpredictable.
	  * Further this method disables all locking mechanisms in
	  * all elements stored and all elements that are added
	  * after calling this method. Calling this method with a
	  * lot of elements stored is therefore rather costly!
	  */
	void disable_thread_safety() noexcept {
		PWX_LOCK_OBJ( this );
		this->do_locking( false );
		this->currStore.disable_thread_safety();
		elem_t* xCurr = head();
		do {
			if ( xCurr ) {
				xCurr->disable_thread_safety();
				xCurr = xCurr->getNext();
			}
		} while ( xCurr && xCurr != tail() );
		this->beThreadSafe( false );
	}


	/// @return true if this container is empty
	virtual bool empty() const noexcept {
		return !eCount.load( memOrdLoad );
	}


	/** @brief enable thread safety
	  *
	  * This method enables all thread safety measures.
	  *
	  * **Warning**: This method enables all locking
	  * mechanisms in all elements stored and all elements
	  * that are added after calling this method. Calling
	  * this method with a lot of elements stored is
	  * therefore rather costly!
	  */
	void enable_thread_safety() noexcept {
		this->do_locking( true );
		this->currStore.enable_thread_safety();
		elem_t* xCurr = head();
		do {
			if ( xCurr ) {
				xCurr->enable_thread_safety();
				xCurr = xCurr->getNext();
			}
		} while ( xCurr && xCurr != tail() );
		this->beThreadSafe( true );
	}


	/** @brief find the element with the given @a data pointer
	  *
	  * This method searches through the list and returns the element
	  * with the given @a data or `nullptr` if @a data is not stored in
	  * this list.
	  * This method searches for the pointer and **not** the data
	  * itself! Use `find(data_t& data)` to search for the content.
	  *
	  * @param data pointer to find
	  * @return return a pointer to the element storing @a data
	**/
	virtual elem_t* find ( data_t* data ) noexcept {
		return const_cast<elem_t* > ( protFind ( static_cast<data_t const* > ( data ) ) );
	}


	/** @brief find the element with the given @a data pointer
	  *
	  * This method searches through the list and returns a const pointer
	  * to the element with the given @a data or `nullptr` if @a data is
	  * not stored in this list.
	  * This method  searches for the pointer and **not** the data
	  * itself! Use `find(data_t const& data)` to search for the
	  * content.
	  *
	  * @param data pointer to find
	  * @return return a const pointer to the element storing @a data
	**/
	virtual elem_t const* find ( data_t const* data ) const noexcept {
		return protFind( data );
	}


	/** @brief find the element with the given @a data content
	  *
	  * This method searches through the list and returns the element
	  * with the given @a data or `nullptr` if @a data is not stored in this
	  * list. This method searches for the content behind the pointer and
	  * **not** for the pointer itself! Use `find(data_t* data)` to search
	  * for the pointer.
	  *
	  * @param data pointer to the data content to find
	  * @return return a pointer to the element storing @a data
	**/
	virtual elem_t* find ( data_t& data ) noexcept {
		return const_cast<elem_t* > ( protFindData ( static_cast<data_t const&> ( data ) ) );
	}


	/** @brief find the element with the given @a data content
	  *
	  * This method searches through the list and returns the element
	  * with the given @a data or `nullptr` if @a data is not stored in this
	  * list. This method  searches for the content behind the pointer and
	  * **not** for the pointer itself! Use `find(data_t const* data)` to
	  * search for the pointer.
	  *
	  * @param data pointer to the data content to find
	  * @return return a const pointer to the element storing @a data
	**/
	virtual elem_t const* find ( data_t const& data ) const noexcept {
		return protFindData( data );
	}


	/** @brief return a read-only pointer to the element with the given @a index
	  *
	  * This method retrieves an element by index like an array. The pointer given
	  * back is read-only.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * If the list is empty, the method returns `nullptr`.
	  *
	  * If you use this method to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read-only pointer to the element, or `nullptr` if the list is empty.
	**/
	virtual elem_t const* get ( int32_t const index ) const noexcept {
		return privGetElementByIndex ( index );
	}


	/** @brief return a read/write pointer to the element with the given @a index
	  *
	  * This method retrieves an element by index like an array. The pointer given
	  * back is write enabled, so use with care.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * If the list is empty, the method returns `nullptr`.
	  *
	  * If you use this method to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read/write pointer to the element, or `nullptr` if the list is empty.
	**/
	virtual elem_t* get ( int32_t index ) noexcept {
		return const_cast<elem_t* > ( privGetElementByIndex ( index ) );
	}


	/** @brief return a read-only reference to the data of the element with the given @a index
	  *
	  * This method retrieves an element by index like an array and returns a reference
	  * to the stored data. The reference given back is read-only.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * However, if the list is empty, the function throws a `pwx::CException` with
	  * the name "OutOfRange".
	  *
	  * If you use this method to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read-only reference to the elements data.
	**/
	virtual data_t const& getData ( int32_t const index ) const {
		PWX_LOCK_GUARD( this );
		// Note: The guard is needed or another thread can
		// delete the retrieved element between the retrieval
		// and the delivery. A segfault would be the result.
		elem_t const* elem = privGetElementByIndex ( index );
		if ( nullptr == elem ) {
			PWX_THROW( "OutOfRange", "The list is empty", "getData(); used on an empty list." )
		}
		return **elem;
	}


	/** @brief return a read/write reference to the data of the element with the given @a index
	  *
	  * This method retrieves an element by index like an array and returns a reference
	  * to the stored data. The reference given back is write enabled, so use with care.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * However, if the list is empty, the function throws a `pwx::CException` with
	  * the name "OutOfRange".
	  *
	  * If you use this method to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read/write reference to the elements data.
	**/
	virtual data_t& getData ( int32_t index ) {
		PWX_TRY_PWX_FURTHER( return const_cast<data_t&>( getData( index ) ) )
	}


	/** @brief insert a new data pointer at its sorted position
	  *
	  * This method inserts a new element in the list before the
	  * first greater element.
	  *
	  * If you insert all elements using this method, the list will
	  * be fully sorted.
	  *
	  * If the new element can not be created, a `pwx::CException`
	  * with the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t insert_sorted ( data_t* data ) {
		PWX_TRY_PWX_FURTHER( return privInsDataSorted( data ) )
	}


	/** @brief insert an element copy at its sorted position
	  *
	  * This method inserts a new element in the list before the
	  * first greater element.
	  *
	  * If you insert all elements using this method, the list will
	  * be fully sorted.
	  *
	  * If the new element can not be created, a `pwx::CException`
	  * with the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src the element that is to be copied.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t insert_sorted ( elem_t const& src ) {
		PWX_TRY_PWX_FURTHER( return privInsElemSorted( src ) )
	}


	/** @brief insert a new data pointer after the specified data
	  *
	  * This method inserts a new element in the list after the element
	  * holding @a prev.
	  *
	  * If @a prev is set to `nullptr`, the new element will become the
	  * new head of the list.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] prev the data the element that should precede the new element holds
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t insNext ( data_t* prev, data_t* data ) {
		PWX_TRY_PWX_FURTHER( return privInsDataBehindData( prev, data ) )
	}


	/** @brief insert an element copy after the specified data
	  *
	  * This method inserts a new element in the list after the element
	  * holding @a prev that is a copy from element @a src.
	  *
	  * If @a prev is set to `nullptr`, the new element will become the
	  * new head of the list.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] prev the data the element that should precede the new element holds.
	  * @param[in] src a reference of the element to copy.
	  * @return the number of elements in this list after the insertion.
	**/
	virtual uint32_t insNext ( data_t* prev, elem_t const& src ) {
		PWX_TRY_PWX_FURTHER( return privInsElemBehindData( prev, src ) )
	}


	/** @brief insert a new data pointer after the specified element
	  *
	  * This method inserts a new element in the list after the element
	  * @a prev.
	  *
	  * If @a prev is set to `nullptr`, the new element will become the
	  * new head of the list.
	  *
	  * If @a prev is not an element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] prev the element that should precede the new element
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t insNextElem ( elem_t* prev, data_t* data ) {
		PWX_TRY_PWX_FURTHER( return privInsDataBehindElem( prev, data ) )
	}


	/** @brief insert an element copy after the specified element
	  *
	  * This method inserts a new element in the list after the element
	  * @a prev that is a copy of @a src.
	  *
	  * If @a prev is set to `nullptr`, the new element will become the
	  * new head of the list.
	  *
	  * If @a prev is not an element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] prev the element that should precede the new element.
	  * @param[in] src reference of the lement to copy.
	  * @return the number of elements in this list after the insertion.
	**/
	virtual uint32_t insNextElem ( elem_t* prev, elem_t const& src ) {
		PWX_TRY_PWX_FURTHER( return privInsElemBehindElem( prev, src ) )
	}


	/** @brief short alias for pop_front()
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use `delNext()` instead.
	  *
	  * If the list is empty, `nullptr` is returned.
	  *
	  * @return a pointer to the removed element or `nullptr` if the list is empty
	**/
	virtual elem_t* pop() {
		return privRemoveAfterElement( nullptr );
	}


	/** @brief alias to remove the last element (tail)
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use `delNext()` instead.
	  *
	  * If the list is empty, `nullptr` is returned.
	  *
	  * @return a pointer to the removed element or `nullptr` if the list is empty
	**/
	virtual elem_t* pop_back() noexcept {
		PWX_LOCK_GUARD( this );
		// Note: The guard is needed to ensure that no thread changes the
		// number of elements beyond the border of eCount > 1
		return ( eCount.load( memOrdLoad ) > 1
		         ? privRemoveAfterElement( const_cast<elem_t* > ( privGetElementByIndex ( -2 ) ) )
		         : privRemoveAfterData( nullptr ) );
	}


	/** @brief alias to remove the first element (head)
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use `delNext()` instead.
	  *
	  * If the list is empty, `nullptr` is returned.
	  *
	  * @return a pointer to the removed element or `nullptr` if the list is empty
	**/
	virtual elem_t* pop_front() noexcept {
		return privRemoveAfterData( nullptr );
	}


	/** @brief short alias for `push_back(data_t *data)`
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push ( data_t* data ) {
		PWX_TRY_PWX_FURTHER ( return privInsDataBehindElem ( tail(), data ) )
	}


	/** @brief short alias for push_back(elem_t const &src)
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push ( elem_t const& src ) {
		PWX_TRY_PWX_FURTHER ( return privInsElemBehindElem ( tail(), src ) )
	}


	/** @brief alias to add a data pointer to the end of the list.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_back ( data_t* data ) {
		PWX_TRY_PWX_FURTHER ( return privInsDataBehindElem ( tail(), data ) )
	}


	/** @brief alias to add an element copy to the end of the list.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_back ( elem_t const& src ) {
		PWX_TRY_PWX_FURTHER ( return privInsElemBehindElem ( tail(), src ) )
	}


	/** @brief alias to add a data pointer to the head of the list.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_front ( data_t* data ) {
		PWX_TRY_PWX_FURTHER ( return privInsDataBehindElem ( nullptr, data ) )
	}


	/** @brief alias to add an element copy to the head of the list.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src reference to the element to copy
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t push_front ( elem_t const& src ) {
		PWX_TRY_PWX_FURTHER ( return privInsElemBehindElem ( nullptr, src ) )
	}


	/** @brief remove the element after the element holding the specified data
	  *
	  * This method removes the element in the list after the element
	  * that holds @a prev and returns a pointer to the removed element.
	  *
	  * If @a prev is set to `nullptr`, the root element (aka head) is
	  * removed.
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use `delNext()` instead.
	  *
	  * If there is no element behind the element @a prev or if the list is
	  * empty, `nullptr` is returned.
	  *
	  * @param[in] prev the data the element that precedes the element to remove holds
	  * @return a pointer to the removed element or `nullptr` if @a prev is held by the last element or the list is empty
	**/
	virtual elem_t* remNext ( data_t* prev ) noexcept {
		return privRemoveAfterData( prev );
	}


	/** @brief remove the element after the specified element
	  *
	  * This method removes the element in the list after the element
	  * @a prev.
	  *
	  * If @a prev is set to `nullptr`, the root element (aka head) is
	  * removed.
	  *
	  * If @a prev is not an element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use `delNextElem()` instead.
	  *
	  * If there is no element behind the element @a prev or if the list is
	  * empty, `nullptr` is returned.
	  *
	  * @param[in] prev the element that precedes the element to remove
	  * @return a pointer to the removed element or nullptr if the list is empty or @a prev is the last element.
	**/
	virtual elem_t* remNextElem ( elem_t* prev ) noexcept {
		return privRemoveAfterElement( prev );
	}


	/// @return the number of stored elements
	uint32_t size() const noexcept {
		return this->eCount.load( memOrdLoad );
	}


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief assignment operator
	  *
	  * Clears this list and copies all elements from @a rhs
	  * into this list. Both the destroy method (if any) and
	  * the setting whether the container is using its thread
	  * safety measures is copied as well.
	  *
	  * @param[in] rhs reference of the list to copy.
	  * @return reference to this.
	**/
	virtual list_t& operator= ( list_t const& rhs ) {
		if ( &rhs != this ) {
			PWX_DOUBLE_LOCK_GUARD( this, &rhs );
			clear();
			destroy     = rhs.destroy;
			beThreadSafe( rhs.beThreadSafe() );
			PWX_TRY_PWX_FURTHER ( return *this += rhs )
		}
		return *this;
	}


	/** @brief addition assignment operator
	  *
	  * Add all elements from @a rhs to this list.
	  *
	  * @param[in] rhs reference of the list to add.
	  * @return reference to this.
	**/
	virtual list_t& operator+= ( list_t const& rhs ) {
		if ( &rhs != this ) {
			PWX_DOUBLE_LOCK_GUARD( this, &rhs );
			elem_t* rhsCurr = rhs.head();
			bool    isDone  = false;
			bool    isTS    = this->beThreadSafe();

			while ( rhsCurr && !isDone ) {
				PWX_TRY_PWX_FURTHER ( this->privInsElemBehindElem ( tail(), *rhsCurr ) )
				if ( !isTS )
					tail()->disable_thread_safety();
				if ( rhsCurr == rhs.tail() )
					isDone = true;
				else
					rhsCurr = rhsCurr->getNext();
			}
		}
		return *this;
	}


	/** @brief substraction assignment operator
	  *
	  * Remove all elements from @a rhs from this list.
	  *
	  * @param[in] rhs reference of the list to substract.
	  * @return reference to this.
	**/
	virtual list_t& operator-= ( list_t const& rhs ) {
		if ( &rhs != this ) {
			PWX_DOUBLE_LOCK_GUARD( this, &rhs );
			elem_t* rhsCurr = rhs.head();
			elem_t* lhsPrev = nullptr;
			data_t* rhsData = nullptr;
			bool    isDone  = false;

			while ( rhsCurr && !isDone ) {
				rhsData = rhsCurr->data.get();

				// Head must be treated first, privFindPrev won't help.
				if ( rhsData == head()->data.get() )
					PWX_TRY_PWX_FURTHER( protDelete( this->remNextElem( nullptr ) ) )
					else {
						lhsPrev = this->privFindPrev( rhsData );
						if ( lhsPrev )
							PWX_TRY_PWX_FURTHER( protDelete( this->remNextElem( lhsPrev ) ) )
						}

				if ( rhsCurr == rhs.tail() )
					isDone = true;
				else
					rhsCurr = rhsCurr->getNext();
			}

		} else {
			PWX_LOCK_GUARD( this );
			// Here we lock before calling clear, or the operator might
			// end up removing elements other threads just inserted.
			clear();
		}

		return *this;
	}


	/** @brief return a read-only pointer to the element with the given @a index
	  *
	  * This operator retrieves an element by index like an array. The pointer given
	  * back is read-only.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * If the list is empty, the operator returns `nullptr`.
	  *
	  * If you use this operator to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read-only pointer to the element, or `nullptr` if the list is empty.
	**/
	virtual elem_t const* operator[] ( int32_t const index ) const noexcept {
		return privGetElementByIndex ( index );
	}


	/** @brief return a read/write pointer to the element with the given @a index
	  *
	  * This operator retrieves an element by index like an array. The pointer given
	  * back is write enabled, so use with care.
	  *
	  * There will be no exception if the index is out of range, it will be wrapped
	  * to press it into the valid range. This means that an index of -1 can be used
	  * to retrieve the last element (tail) for instance.
	  *
	  * However, if the list is empty, the operator returns `nullptr`.
	  *
	  * If you use this operator to quickly access head or tail, neither the currently
	  * used internal pointer nor number are changed. Head and tail are given back
	  * directly.
	  *
	  * @param[in] index the index of the element to find.
	  * @return read/write pointer to the element, or `nullptr` if the list is empty.
	**/
	virtual elem_t* operator[] ( int32_t index ) noexcept {
		return const_cast<elem_t* > ( this->privGetElementByIndex ( index ) );
	}


protected:
	/* ===============================================
	 * === Protected methods                       ===
	 * ===============================================
	*/

	/// @brief function pointer to a user provided destroy method
	void ( *destroy ) ( data_t* data_ );

	// accessors for curr/head/tail

	/// @brief return curr according to thread safety setting
	elem_t* curr() const {
		elem_t* result = static_cast<elem_t*>( currStore.curr() );
		return result ? result : head();
	}


	/// @brief set curr to @a new_curr according to thread safety settings
	void curr( elem_t* new_curr ) const {
		currStore.curr( new_curr );
	}


	/// @brief return head according to thread safety setting
	elem_t* head() const {
		return head_.load( memOrdLoad );
	}


	/// @brief set head to @a new_head according to thread safety settings
	void head( elem_t* new_head ) {
		head_.store( new_head, memOrdStore );
	}


	/// @brief return tail according to thread safety setting
	elem_t* tail() const {
		return tail_.load( memOrdLoad );
	}


	/// @brief set tail to @a new_tail according to thread safety settings
	void tail( elem_t* new_tail ) {
		tail_.store( new_tail, memOrdStore );
	}


	// Overridable protected methods

	/** @brief Delete the element @a removed
	  *
	  * **Important**: this method will throw "illegal_delete" if
	  * @a removed is actually not removed from the list. Making sure
	  * this method is only called with a removed element allows to
	  * use it without a lock on the list itself.
	**/
	virtual uint32_t protDelete( elem_t* removed ) {
		if ( removed && removed->inserted() )
			PWX_THROW( "illegal_delete", "delete non-removed element", "Deleting an element that is not removed is illegal!" );

		if ( removed ) {
			// Use double check technique to avoid illegal double deletion
			if ( !removed->destroyed() ) {
				PWX_LOCK( removed );
				if ( !removed->destroyed() ) {
					PWX_TRY( delete removed ) // Will unlock automatically
					PWX_THROW_PWXSTD_FURTHER ( "delete", "Deleting an element failed." );
				} else
					PWX_UNLOCK( removed );
			} // end of if not destroyed yet
		} // end of having an element to delete
		return eCount.load( memOrdLoad );
	}


	/// @brief Search until the current element contains the searched data
	virtual elem_t const* protFind ( const data_t* data ) const noexcept {
		elem_t* result = nullptr;

		/* Some rules about searching for elements:
		 * 1) root, tail and curr are checked first in a locked state.
		 * 2) The point in time where the search starts is relevant.
		 *    This means, that if the list is to be traversed, it
		 *    will not be locked. If the searched element is inserted
		 *    by another thread in a position the searching thread has
		 *    already traversed, it will not be found.
		 *    This is in order, because the element was not there when
		 *    the search started.
		 * 3) As new elements might be added to the end, the traversal
		 *    stops after checking the actual tail if the element could
		 *    not be found earlier.
		 * However, as most people will insert using push(), push_back()
		 * or add(), the element would replace tail and therefore be
		 * found anyway.
		 */

		// Return at once if the list is empty
		if ( empty() )
			return nullptr;

		// Rule 1: Lock for the basic tests.
		PWX_LOCK_OBJ( const_cast<list_t*>( this ) );

		// Exit if the list has been emptied while we waited for the lock
		if ( empty() ) {
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
			return nullptr;
		}

		// Quick exit if curr is already what we want:
		elem_t* xCurr = curr();
		if ( xCurr && ( xCurr->data.get() == data ) ) {
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )
			return xCurr;
		}

		// The next does only make sense if we have more than one element
		if ( size() > 1 ) {

			// Exit if head is wanted...
			elem_t* xHead = head();
			if ( ( xHead != xCurr ) && ( xHead->data.get() == data ) ) {
				curr( xHead );
				PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )
				return xHead;
			}

			// ...or tail
			elem_t* xTail = tail();
			if ( ( xTail != xCurr ) && ( xTail->data.get() == data ) ) {
				curr( xTail );
				PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )
				return xTail;
			}

			// Otherwise we have to search for it.
			bool isDone = false;
			xCurr = xHead->getNext(); // head is already checked.

			// This is rule 2: Unlock for traversal
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )

			while ( !result && !isDone && xCurr ) {
				// Rule 3: Re-check tail. It might be
				// different from what was checked above
				// once xCurr gets there.
				if ( xCurr->data.get() == data ) {
					result = xCurr;
					curr( xCurr );
				} else if ( xCurr == tail() )
					isDone = true;
				else
					xCurr = xCurr->getNext();
			} // End of manual traversing the container

		} // End of handling a search with more than one element
		else
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )

			return result;
	}


	/// @brief Search until the current element contains the searched data content
	virtual elem_t const* protFindData ( data_t const& data ) const noexcept {
		elem_t* result = nullptr;

		/* The same three rules as when searching for an
		 * element via its pointer apply here, too.
		 */

		// Return at once if the list is empty
		if ( empty() )
			return nullptr;

		// Rule 1
		PWX_LOCK_OBJ( const_cast<list_t*>( this ) );

		if ( empty() ) {
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
			return nullptr;
		}

		// Quick exit if curr is already what we want:
		elem_t* xCurr = curr();
		if ( *xCurr == data ) {
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )
			return xCurr;
		}

		// The next does only make sense if we have more than one element
		if ( size() > 1 ) {

			// Exit if head is wanted...
			elem_t* xHead = head();
			if ( ( xHead != xCurr ) && ( *xHead == data ) ) {
				curr( xHead );
				PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )
				return xHead;
			}

			// ...or tail
			elem_t* xTail = tail();
			if ( ( xTail != xCurr ) && ( *xTail == data ) ) {
				curr( xTail );
				PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )
				return xTail;
			}

			// Otherwise we have to search for it.
			bool isDone = false;
			xCurr = xHead->getNext(); // head is already checked.

			// Rule 2:
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )

			while ( !result && !isDone && xCurr ) {
				// Note: The container is not locked any more,
				// locking and checks are done on element level
				// by compare() then.
				if ( 0 == xCurr->compare( data ) ) {
					result = xCurr;
					curr( xCurr );
				} else if ( xCurr == tail() )
					isDone = true;
				else
					xCurr = xCurr->getNext();
			}

		} // End of handling a search with more than one element
		else
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )

			return result;
	}


	/// @brief Search until the next element after curr has greater data content
	virtual elem_t const* protFindGreaterNext ( data_t const& data ) const noexcept {
		elem_t* result = nullptr;

		/* The same three rules as when searching for an
		 * element via its pointer apply here, too.
		 */

		// Return at once if the list is empty
		if ( empty() )
			return nullptr;

		// Rule 1
		PWX_LOCK_OBJ( const_cast<list_t*>( this ) );

		if ( empty() ) {
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
			return nullptr;
		}

		// Quick exit if curr->next is already what we want:
		elem_t* xCurr = curr();
		elem_t* xNext = xCurr->getNext(); // Note: xCurr can not be nullptr at this point.
		int32_t comp  = xCurr->compare( data );
		if ( ( comp < 0 )
		                && ( ( nullptr == xNext ) || ( xNext->compare( data ) > -1 ) ) ) {
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )
			return xNext ? xNext : nullptr;
		}

		// Quick exit if head is already greater...
		elem_t* xHead = head();
		if ( xHead && ( xHead->compare( data ) > -1 ) ) {
			curr( xHead );
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )
			return xHead;
		}

		// ...or tail is lower
		elem_t* xTail = tail();
		if ( xTail && ( xTail->compare( data ) < 0 ) ) {
			curr( xTail );
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )
			return nullptr; // tail is prev of nullptr by definition.
		}

		// Otherwise we have to search for it.
		bool isDone = false;
		if ( comp > -1 )
			xCurr = xHead->getNext(); // head is already checked.

		// Rule 2:
		PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )

		while ( !result && !isDone && xCurr && xNext ) {
			// Note: The container is not locked any more,
			// locking and checks are done on element level
			// by compare() then.
			xNext = xCurr->getNext();
			xTail = tail();
			if ( xNext->compare( data ) > -1 ) {
				result = xNext;
				curr( xCurr );
			} else if ( xCurr == xTail ) {
				isDone = true; // result stays being nullptr
				curr( xTail );
			} else
				// Be safe and use getNext() again.
				xCurr = xCurr->getNext();
		}

		return result;
	}


	/** @brief simple method to insert an element into the list
	  *
	  * If either @a insPrev or @a insElem is marked as destroyed,
	  * a `pwx::CException` is thrown. Such a condition implies that
	  * there is something seriously wrong.
	  *
	  * @param[in] insPrev Element after which the new element is to be inserted
	  * @param[in] insElem The element to insert.
	  * @return The number of elements in the list after the insertion
	**/
	virtual uint32_t protInsert ( elem_t* insPrev, elem_t* insElem ) {
		/* There are four possibilities:
		 * 1: The list is empty
		 *    head, tail and curr have to be set to the new
		 *    element, no full renumbering is needed then.
		 * 2: insPrev is nullptr
		 *    head has to be changed to be the new element
		 * 3: insPrev is tail
		 *    tail has to be set to the new element, no full
		 *    renumbering is needed then.
		 * 4: Otherwise insPrev->insertNext() can do the insertion
		*/
		curr( insElem ); // always use the new element henceforth

		if ( size() && insPrev && ( tail() != insPrev ) ) {
			// Case 4: A normal insert
			this->doRenumber.store( true, memOrdStore );
			PWX_TRY_PWX_FURTHER( insPrev->insertNext( insElem, &currStore ) );
		} else {
			PWX_LOCK_GUARD( this );
			if ( empty() ) {
				// Case 1: The list is empty
				head( insElem );
				tail( insElem );
				PWX_TRY_PWX_FURTHER( insElem->insertBefore( nullptr, &currStore ) );
			} else if ( nullptr == insPrev ) {
				// Case 2: A new head is to be set
				PWX_TRY_PWX_FURTHER( insElem->insertBefore( head(), &currStore ) );
				head( insElem );
				this->doRenumber.store( true, memOrdStore );
			} else if ( ( insPrev == tail() ) || insPrev->destroyed() ) {
				// Case 3: A new tail is to be set
				insElem->nr( tail()->nr() + 1 );
				PWX_TRY_PWX_FURTHER( tail()->insertNext( insElem, &currStore ) );
				tail( insElem );
			} else {
				// Case 4, but only after acquiring the lock
				this->doRenumber.store( true, memOrdStore );
				PWX_TRY_PWX_FURTHER( insPrev->insertNext( insElem, &currStore ) );
			}
		}

		eCount.fetch_add( 1, memOrdStore );
		return eCount.load( memOrdLoad );
	}


	/// @brief renumber all elements
	virtual void protRenumber() const noexcept {
		if ( doRenumber.load( memOrdLoad ) ) {

			// Do a big lock, so multiple threads calling this function
			// won't renumber multiple times when once is enough.
			PWX_LOCK_GUARD( this );

			// Check again, maybe this does not need any renumbering any more:
			if ( !doRenumber.load( memOrdLoad ) )
				return;

			elem_t*           xCurr  = head();
			uint32_t          xNr    = 0;
			bool              isDone = false;

			while ( xCurr && !isDone ) {
				// Lock guard xCurr, so inserting threads won't give us a headache
				PWX_NAMED_LOCK_GUARD( ReNamer, xCurr );
				xCurr->nr( xNr++ );
				if ( xCurr == tail() )
					isDone = true;
				else
					xCurr = xCurr->getNext();
			}

			this->doRenumber.store( false, memOrdStore );
		}
	}


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	store_t currStore;         //!< Storage for the thread id bound curr pointers
	using base_t::memOrdLoad;
	using base_t::memOrdStore;


private:
	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	/* Important: Public methods should not lock unless it is critical.
	 *            All necessary checks are done by the public methods,
	 *            work is done by the private methods which will lock
	 *            where appropriate.
	*/

	/// @brief clear this list
	virtual void privClear() noexcept {
		/* The fastest way to clear a list based container is
		 * to simply declare it as being empty. Of course the
		 * formerly stored elements then must be deleted anyway.
		 * However, if someone does anything stupid, like calling
		 * clear() in one thread while other threads add and/or
		 * remove elements (like torture does ;)), some
		 * additional checks must be performed for such a
		 * situation not to go haywire.
		 */
		elem_t* xHead = nullptr;
		if ( size() ) {
			PWX_LOCK_OBJ( this );
			xHead = head();
			PWX_LOCK( xHead );
			while ( xHead && ( xHead->removed() || xHead->destroyed() ) ) {
				while ( waiting () ) {
					PWX_RELOCK_OBJ( this );
				}
				PWX_UNLOCK( xHead );
				xHead = head();
				PWX_LOCK( xHead ); // Note: Won't do anything if xHead is nullptr
			}
			if ( xHead && size() ) {
				eCount.store( 0, memOrdStore );
				if ( !currStore.destroyed() )
					curr( nullptr );
				head( nullptr );
				tail( nullptr );
				PWX_UNLOCK( xHead );
			}
			PWX_UNLOCK_OBJ( this );
		} // End of catching head()

		// Now if this call found anything, it can be destroyed:
		if ( xHead ) {
			currStore.clear(); // will all be gone soon anyway
			elem_t* xNext = nullptr;
			xHead->beThreadSafe( false );
			while ( xHead && ( xNext = xHead->removeNext() ) ) {
				if ( xNext && !xNext->destroyed() )
					delete xNext;
			}
			if ( xHead && !xHead->destroyed() )
				delete xHead;
		}
	}

	/// @brief Search until the next element contains the searched data
	// Note: This method is only used by operator-=. But the implementation
	//       is different in TDoubleList, so both can use the same operator with
	//       only this detail being individual.
	virtual elem_t* privFindPrev ( const data_t* data ) const noexcept {
		elem_t*  xPrev = head();
		elem_t*  xCurr = xPrev->getNext();

		while ( xPrev && ( xPrev != tail() ) ) {
			if ( xCurr->data.get() == data ) {
				curr( xCurr );
				return xPrev;
			}
			xPrev = xCurr;
			xCurr = xCurr->getNext();
		}

		// If we are here, prev points to tail. No match found.
		return nullptr;
	}


	/// @brief wrapping method to retrieve an element by any index or nullptr if the list is empty
	virtual elem_t const* privGetElementByIndex ( int32_t index ) const noexcept {
		// return at once if the list is empty
		if ( empty() )
			return nullptr;

		protRenumber();

		// It is necessary to lock briefly to ensure a consistent
		// start of the search with a minimum of checks
		PWX_LOCK_OBJ( const_cast<list_t*>( this ) );

		uint32_t locCnt = size();

		if ( locCnt ) {
			elem_t* xCurr = curr();
			uint32_t xNr = xCurr ? xCurr->nr() : 0;
			if ( nullptr == xCurr )
				xCurr = head();

			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );

			// Mod index into range
			uint32_t xIdx = static_cast<uint32_t> ( index < 0
			                                        ? locCnt - ( std::abs ( index ) % locCnt )
			                                        : index % locCnt );

			// Unfortunately this results in xIdx equaling locCnt
			// (which is wrong) if index is a negative multiple of
			// locCnt:
			if ( xIdx >= locCnt )
				xIdx %= locCnt;

			// Is xCurr already correct?
			if ( xIdx == xNr )
				return xCurr;

			// Is it the head we want?
			if ( 0 == xIdx )
				return head();

			// Or tail ?
			if ( ( locCnt - 1 ) == xIdx )
				/* Note: Although, after unlocking, tail might be pointing
				 * somewhere else, the check is against locCnt to ensure
				 * that any call to index -1 retrieves the current tail.
				 */
				return tail();

			// Is xIdx the next member, like in a for loop?
			if ( xIdx == ( xNr + 1 ) ) {
				xCurr = xCurr->getNext();
				curr( xCurr );
				return xCurr;
			}

			// Ok, let's go. But only start from head if we currently are beyond.
			if ( xIdx < xNr ) {
				xCurr = head()->getNext();
				xNr  = 1;
			}

			// Otherwise the next of xCurr is already checked, so skip it
			else {
				xCurr = xCurr->getNext();
				++xNr;
			}

			// Now look into the rest
			while ( xCurr && ( xNr++ < xIdx ) ) {
				/* If removals lead to an xIdx beyond eCount it doesn't
				 * matter. xNr is a manual count and simply raised until
				 * xIdx is met.
				 * There are are only two special conditions:
				 * a) if xCurr is tail(), xCurr must continue with head().
				 * b) if the list is emptied before a result is found,
				 *    nullptr must be returned. (handled by the loop
				 *    condition!)
				 */
				// check special condition a)
				if ( tail() == xCurr )
					xCurr = head();
				else
					xCurr = xCurr->getNext();

				// Be sure special condition b) doesn't creep on us:
				if ( ( nullptr == xCurr ) && size() )
					xCurr = head();
			} // end of searching loop

			// xCurr is sure to be pointing where it should now
			// or is nullptr if the list has been emptied
			assert ( ( xCurr || empty() ) && "ERROR: xCurr is nullptr but the list is not empty!" );
			curr( xCurr );
			return xCurr;
		} else
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );

		return nullptr;
	}


	/// @brief preparation method to insert data behind data
	virtual uint32_t privInsDataBehindData( data_t* prev, data_t* data ) {
		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>( protFind( prev ) ) : nullptr;
		if ( prev && ( nullptr == prevElement ) )
			PWX_THROW ( "ElementNotFound",
			            "Element not found",
			            "The searched element can not be found in this singly linked list" );

		// Now prevElement must not change any more
		if ( prevElement )
			PWX_LOCK( prevElement );

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( data, destroy ) )
		catch( std::exception& e ) {
			if ( prevElement )
				PWX_UNLOCK( prevElement );
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." );
		}
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 3: Do the real insert
		if ( prevElement )
			PWX_UNLOCK( prevElement );
		PWX_TRY_PWX_FURTHER( return protInsert( prevElement, newElement ) );
	}


	/// @brief preparation method to insert data behind an element
	virtual uint32_t privInsDataBehindElem( elem_t* prev, data_t* data ) {
		// 1: prev, if set, must not change any more
		if ( prev )
			PWX_LOCK( prev );

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( data, destroy ) )
		catch( std::exception& e ) {
			if ( prev )
				PWX_UNLOCK( prev );
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." );
		}
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 3: Do the real insert
		if ( prev )
			PWX_UNLOCK( prev );
		PWX_TRY_PWX_FURTHER( return protInsert( prev, newElement ) );
	}


	/// @brief preparation method to insert data before the first greater element
	virtual uint32_t privInsDataSorted( data_t* data ) {
		// 1: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( data, destroy ) )
		catch( std::exception& e )
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." );

		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 2: Find the previous element the new one will become next of
		elem_t const* prev = protFindGreaterNext( *data );

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER( return protInsert( const_cast<elem_t*>( prev ), newElement ) );
	}


	/// @brief preparation method to insert an element copy behind data
	virtual uint32_t privInsElemBehindData( data_t* prev, elem_t const& src ) {
		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>( protFind( prev ) ) : nullptr;
		if ( prev && ( nullptr == prevElement ) )
			PWX_THROW ( "ElementNotFound",
			            "Element not found",
			            "The searched element can not be found in this singly linked list" );

		// Now prevElement must not change any more
		if ( prevElement )
			PWX_LOCK( prevElement );

		// 2: Check source:
		PWX_LOCK_OBJ( const_cast<elem_t*>( &src ) );

		if ( src.destroyed() ) {
			// What on earth did the caller think?
			PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) );
			if ( prevElement )
				PWX_UNLOCK( prevElement );
			PWX_THROW( "Illegal Condition", "Source element destroyed",
			           "An element used as source for insertion is destroyed." );
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( src ) )
		catch( std::exception& e ) {
			PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) );
			if ( prevElement )
				PWX_UNLOCK( prevElement );
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." );
		}
		PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) );
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 4: Do the real insert
		if ( prevElement )
			PWX_UNLOCK( prevElement );
		PWX_TRY_PWX_FURTHER( return protInsert( prevElement, newElement ) );
	}


	/// @brief preparation method to insert an element copy behind an element
	virtual uint32_t privInsElemBehindElem( elem_t* prev, elem_t const& src ) {
		// 1: prev, if set, must not change any more
		if ( prev )
			PWX_LOCK( prev );

		// 2: Check source:
		PWX_LOCK_OBJ( const_cast<elem_t*>( &src ) );

		if ( src.destroyed() ) {
			// What on earth did the caller think?
			PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) )
			if ( prev )
				PWX_UNLOCK( prev );
			PWX_THROW( "Illegal Condition", "Source element destroyed",
			           "An element used as source for insertion is destroyed." );
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( src ) )
		catch( std::exception& e ) {
			PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) )
			if ( prev )
				PWX_UNLOCK( prev );
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." );
		}
		PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) );
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 4: Do the real insert
		if ( prev )
			PWX_UNLOCK( prev );
		PWX_TRY_PWX_FURTHER( return protInsert( prev, newElement ) )
	}


	/// @brief preparation method to insert data before the first greater element
	virtual uint32_t privInsElemSorted( elem_t const& src ) {
		// 1: Check source:
		PWX_LOCK_OBJ( const_cast<elem_t*>( &src ) );

		if ( src.destroyed() ) {
			// What on earth did the caller think?
			PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) );
			PWX_THROW( "Illegal Condition", "Source element destroyed",
			           "An element used as source for insertion is destroyed." );
		}

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( src ) )
		catch( std::exception& e )
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." );
		PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) );
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 2: Find the previous element the new one will become next of
		elem_t const* prev = protFindGreaterNext( *src );

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER( return protInsert( const_cast<elem_t*>( prev ), newElement ) )
	}


	/** @brief remove the element after the specified data
	  * If @a prev data can not be found, nothing happens and nullptr is returned.
	  * @return nullptr if the element holding @a prev is the last element or the list is empty.
	**/
	virtual elem_t* privRemoveAfterData( data_t* prev ) noexcept {
		/* Two possibilities:
		 * 1) prev is nullptr thus head() is to be removed
		 * 2) prev is any data that must be searched for.
		 */
		if ( prev ) {
			elem_t* xPrev = const_cast<elem_t*>( protFind ( prev ) );
			if ( xPrev && xPrev->inserted() && !xPrev->destroyed() )
				return privRemoveAfterElement( xPrev );
		} else
			// possibility 1 is simple:
			return privRemoveAfterElement( nullptr );

		return nullptr;
	}


	/** @brief remove the element after the specified element
	  * @param[in] prev pointer to the element after which to remove an element
	  * @return the removed element or nullptr if prev->next is nullptr or the list is empty
	**/
	virtual elem_t* privRemoveAfterElement( elem_t* prev ) noexcept {
		elem_t* removed = nullptr;

		/* The three possibilities are:
		 * a) prev is nullptr and head must be removed
		 * b) prev->next is tail thus tail must be removed
		 * c) prev and prev->next are somewhere else
		 */
		if ( prev ) {
			if ( tail() == prev->getNext() ) {
				// This is possibility 2
				PWX_LOCK_OBJ( this );
				removed = prev->removeNext();
				if ( tail() == removed )
					/* Reason for the lock and the double check:
					 * removeNext() only returns once it can give back
					 * the true next neighbor of prev. Therefore, if
					 * another thread puts in an element between prev
					 * and its current next while tail() is maintained
					 * here, the following can happen:
					 * this thread:  prev->next is tail, set tail to prev
					 * other thread: insert element after tail, prev->next
					 *               becomes new element and new tail.
					 * this thread:  remove this->next which is the new
					 *               element.
					 * The old prev->next is now lost.
					 */
					tail( prev );
				else
					this->doRenumber.store( true, memOrdStore );
				PWX_UNLOCK_OBJ( this );
			} else {
				// Possibility 3: Just do it
				removed = prev->removeNext();
				this->doRenumber.store( true, memOrdStore );
			}
		} else {
			// This is possibility 1
			PWX_LOCK_OBJ( this );
			// Same reason for the lock as above
			removed = head();
			if ( removed ) {
				head( removed->getNext() );
				removed->remove();
				this->doRenumber.store( true, memOrdStore );
			}
			PWX_UNLOCK_OBJ( this );
		}

		// If something was removed, the count must be decreased:
		if ( removed ) {
			if ( 1 == eCount.fetch_sub( 1, memOrdStore ) ) {
				PWX_LOCK_OBJ( this );
				// The list is empty. Or is it?
				if ( 0 == eCount.load( memOrdLoad ) ) {
					head( nullptr );
					tail( nullptr );
					currStore.clear();
				}
				PWX_UNLOCK_OBJ( this );
			}
		}

		return removed;
	}


	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	neighbor_t head_ = ATOMIC_VAR_INIT( nullptr ); //!< pointer to the first element
	neighbor_t tail_ = ATOMIC_VAR_INIT( nullptr ); //!< pointer to the last element
}; // class TSingleList


template<typename data_t, typename elem_t>
TSingleList<data_t, elem_t>::~TSingleList() noexcept {
	PWX_LOCK_GUARD( this );
	isDestroyed.store( true );

	// Before clearing, make sure the queue waiting for a lock is empty
	while ( waiting() ) {
		PWX_LOCK_GUARD_RESET( this );
	}

	// Now go ahead, but directly. The public method cancels on isDestroyed.
	this->privClear();

	// Do another lock queue emptying before finishing this destructor.
	while ( waiting() ) {
		PWX_LOCK_GUARD_RESET( this );
	}
}


/** @brief addition operator
  *
  * Create and return a new list consisting of all elements of @a lhs and @a rhs.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local list is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: All elements
  * will be copied twice!
  * Only use this operator if you really think you need a construct like
  * 'listC = listA + listB'.
  *
  * @param[in] lhs left hand side reference
  * @param[in] rhs right hand side reference
  * @return the new list.
**/
template<typename data_t, typename elem_t>
TSingleList<data_t, elem_t> operator+ ( const TSingleList<data_t, elem_t>& lhs, const TSingleList<data_t, elem_t>& rhs ) {
	TSingleList<data_t, elem_t> result( lhs );

	if ( &lhs != &rhs )
		PWX_TRY_PWX_FURTHER( result += rhs );

	return result;
}


/** @brief substraction operator
  *
  * Remove all elements from @a rhs from this list.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local list is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: The difference
  * elements are copied twice!
  *
  * @param[in] lhs reference of the list from which to substract.
  * @param[in] rhs reference of the list to substract.
  * @return reference to this.
**/
template<typename data_t, typename elem_t>
TSingleList<data_t, elem_t> operator- ( const TSingleList<data_t, elem_t>& lhs, const TSingleList<data_t, elem_t>& rhs ) {
	TSingleList<data_t, elem_t> result( lhs );

	if ( &lhs != &rhs ) {
		PWX_TRY_PWX_FURTHER( result -= rhs );
	} else
		result.clear();

	return result;
}

} // namespace pwx

#endif // PWX_LIBPWX_CONTAINER_TSINGLELIST_H_INCLUDED

