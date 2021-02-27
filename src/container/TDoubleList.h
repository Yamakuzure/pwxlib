#ifndef PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED
#define PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED 1
#pragma once

/** @file TDoubleList.h
  *
  * @brief Declaration of a basic template for doubly linked lists
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

#include "container/TSingleList.h"
#include "container/TDoubleElement.h"


namespace pwx {


/** @class TDoubleList PDoubleList <PDoubleList>
  *
  * @brief Template to build doubly linked lists of variable types
  *
  * The doubly linked list is an extension to the singly linked list.
  * Elements of type `pwx::TDoubleElement<T>` are chained together using their next
  * and prev pointers. Being able to look into what lies before an element
  * speeds up certain operations at the cost of one additional pointer per element.
  *
  * The constructor takes an optional `destroy(T*)` function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * This container is designed to work safely in a multi-threaded environment.
  * See below for more on single and multi threaded usage.
  *
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * **Notes on multi threaded environments**
  *
  * If you plan to use this container in a strictly single-threaded way, you can
  * turn off most of the thread safety measures with disable_thread_safety().
**/
template<typename data_t, typename elem_t = TDoubleElement<data_t> >
class PWX_API TDoubleList : public TSingleList<data_t, elem_t> {
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef TSingleList<data_t, elem_t> base_t;  //!< Base type of this list
	typedef TDoubleList<data_t, elem_t> list_t;  //!< Type of this list
	typedef CThreadElementStore         store_t; //!< Storage for the thread id bound curr pointer


	/* ===============================================
	 * === Public constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor initializes an empty list.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TDoubleList ( void ( *destroy_ ) ( data_t* ) ) noexcept :
		base_t ( destroy_ )
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor uses the default constructor to set the data
	  * destroy method to the null pointer.
	**/
	TDoubleList() noexcept :
		base_t ( nullptr )
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * @param[in] src reference of the list to copy.
	**/
	TDoubleList ( const list_t& src ) noexcept :
		base_t ( src ) {
		// No need to do anything. The base ctor copies all elements using
		// protInsert() which is actually in this class.
	}


	virtual ~TDoubleList() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	using base_t::clear;


	/** @brief delete the element holding the specified data
	  *
	  * This method deletes the element in the list that holds @a data.
	  *
	  * If you intent to work with the element, use remData instead.
	  *
	  * Nothing happens if there is no element holding @a data. If the
	  * deletion of the data throws an exception, it will be turned
	  * into a `pwx::CException` and passed on.
	  *
	  * @param[in] data the data the element that is to be deleted holds
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delData ( data_t* data ) {
		PWX_TRY_PWX_FURTHER( return protDelete( remData( data ) ) );
	}


	/** @brief delete the specified element
	  *
	  * This method deletes the element @a elem.
	  *
	  * If you intent to work with the element, use `remElem()` instead.
	  *
	  * If @a elem is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * @param[in] elem the element that is to be deleted
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delElem ( elem_t* elem ) {
		PWX_TRY_PWX_FURTHER( return protDelete( remElem( elem ) ) );
	}


	using base_t::delNext;
	using base_t::delNextElem;


	/** @brief delete the element before the element holding the specified data
	  *
	  * This method deletes the element in the list before the element
	  * that holds @a next.
	  *
	  * If @a next is set to `nullptr`, the last element (aka tail) is
	  * deleted.
	  *
	  * If you intent to work with the element, use `remPrev()` instead.
	  *
	  * Nothing happens if there is no element before the element holding @a next.
	  * If the deletion of the data throws an exception, it will be turned
	  * into a `pwx::CException` and passed on.
	  *
	  * @param[in] next the data the element that succeeds the element to delete holds
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delPrev ( data_t* next ) {
		PWX_TRY_PWX_FURTHER( return protDelete( remPrev( next ) ) );
	}


	/** @brief delete the element before the specified element
	  *
	  * This method deletes the element in the list before the element
	  * @a next.
	  *
	  * If @a next is set to `nullptr`, the last element (aka tail) is
	  * deleted.
	  *
	  * If you intent to work with the element, use `remPrevElem()` instead.
	  *
	  * If @a next is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * Nothing happens if there is no element behind @a prev.
	  * If the deletion of the data throws an exception, it will be turned
	  * into a `pwx::CException` and passed on.
	  *
	  * @param[in] next the element that succeeds the element to delete
	  * @return the number of elements remaining in the list after the deletion.
	**/
	virtual uint32_t delPrevElem ( elem_t* next ) {
		PWX_TRY_PWX_FURTHER( return protDelete( remPrevElem( next ) ) );
	}


	using base_t::disable_thread_safety;
	using base_t::empty;
	using base_t::enable_thread_safety;
	using base_t::find;
	using base_t::get;
	using base_t::getData;
	using base_t::insNext;
	using base_t::insNextElem;


	/** @brief insert a new data pointer before the specified data
	  *
	  * This method inserts a new element in the list before the element
	  * holding @a next.
	  *
	  * If @a next is set to `nullptr`, the new element will become the new
	  * tail of the list.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] next the data the element that should succeed the new element
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t insPrev ( data_t* next, data_t* data ) {
		PWX_TRY_PWX_FURTHER( return privInsDataBeforeData( next, data ) );
	}


	/** @brief insert an element copy before the specified data
	  *
	  * This method inserts a new element in the list before the element
	  * holding @a next as a copy of @a src.
	  *
	  * If @a next is set to `nullptr`, the new element will become the new
	  * tail of the list.
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] next the data the element that should succeed the new element.
	  * @param[in] src reference to the element to copy.
	  * @return the number of elements in this list after the insertion.
	**/
	virtual uint32_t insPrev ( data_t* next, const elem_t& src ) {
		PWX_TRY_PWX_FURTHER( return privInsElemBeforeData( next, src ) );
	}


	/** @brief insert a new data pointer before the specified element
	  *
	  * This method inserts a new element in the list before the element
	  * @a next.
	  *
	  * If @a next is set to `nullptr`, the new element will become the new
	  * tail of the list.
	  *
	  * If @a next is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] next the element that should succeed the new element
	  * @param[in] data the pointer that is to be added.
	  * @return the number of elements in this list after the insertion
	**/
	virtual uint32_t insPrevElem ( elem_t* next, data_t* data ) {
		PWX_TRY_PWX_FURTHER( return privInsDataBeforeElem( next, data ) );
	}


	/** @brief insert an element copy before the specified element
	  *
	  * This method inserts a new element in the list before the element
	  * @a next as a copy of @a src.
	  *
	  * If @a next is set to `nullptr`, the new element will become the new
	  * tail of the list.
	  *
	  * If @a next is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If the new element can not be created, a `pwx::CException` with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] next the element that should succeed the new element.
	  * @param[in] src reference to the element to copy.
	  * @return the number of elements in this list after the insertion.
	**/
	virtual uint32_t insPrevElem ( elem_t* next, const elem_t& src ) {
		PWX_TRY_PWX_FURTHER( return privInsElemBeforeElem( next, src ) );
	}


	using base_t::pop;


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
		return privRemove( tail() );
	}


	using base_t::pop_front;
	using base_t::push;
	using base_t::push_back;
	using base_t::push_front;


	/** @brief remove the element holding the specified data
	  *
	  * This method removes the element in the list that holds @a data
	  * and returns a pointer to the removed element.
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use `delData()` instead.
	  *
	  * @param[in] data the data the element is to be removed holds
	  * @return a pointer to the removed element
	**/
	virtual elem_t* remData ( data_t* data ) noexcept {
		return privRemoveData( data );
	}


	/** @brief remove the specified element
	  *
	  * This method removes the element @a elem and returns a pointer
	  * to the removed element.
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use `delElem()` instead.
	  *
	  * If @a elem is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * @param[in] elem the element that is to be removed
	  * @return a pointer to the removed element
	**/
	virtual elem_t* remElem ( elem_t* elem ) noexcept {
		return privRemove( elem );
	}


	using base_t::remNext;
	using base_t::remNextElem;


	/** @brief remove the element before the element holding the specified data
	  *
	  * This method removes the element in the list before the element
	  * that holds @a next and returns a pointer to the removed element.
	  *
	  * If @a next is set to `nullptr? , the last element (aka tail) is
	  * removed.
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use `delPrev()` instead.
	  *
	  * If there is no element before the element @a next holds, or if the
	  * list is empty, `nullptr` is returned.
	  *
	  * @param[in] next the data the element that succeeds the element to remove holds
	  * @return a pointer to the removed element or nullptr if there is no previous element or the list is empty
	**/
	virtual elem_t* remPrev ( data_t* next ) noexcept {
		return privRemoveBeforeData( next );
	}


	/** @brief remove the element before the specified element
	  *
	  * This method removes the element in the list before the element
	  * @a next and returns a pointer to the removed element.
	  *
	  * If @a next is set to `nullptr`, the last element (aka tail) is
	  * removed.
	  *
	  * You have to delete the removed element by yourself. If you do not intent
	  * to work with the removed element, use `delPrevElem()` instead.
	  *
	  * If @a next is no element of this list, the wrong list is updated
	  * and both element counts will be wrong then. So please make sure to
	  * use the correct element on the correct list!
	  *
	  * If there is no element before @a next or if the list is empty,
	  * a `nullptr` is returned.
	  *
	  * @param[in] next the element that succeeds the element to remove
	  * @return a pointer to the removed element or `nullptr` if @a next is the first element or the list is empty.
	**/
	virtual elem_t* remPrevElem ( elem_t* next ) noexcept {
		return privRemoveBeforeElem( next );
	}


	using base_t::size;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief assignment operator
	  *
	  * Clears this list and copies all elements from @a rhs
	  * into this list.
	  *
	  * @param[in] rhs reference of the list to copy.
	  * @return reference to this.
	**/
	virtual list_t& operator= ( const list_t& rhs ) {
		if ( &rhs != this )
			PWX_TRY_PWX_FURTHER ( base_t::operator= ( rhs ) );
		return *this;
	}


	/** @brief addition assignment operator
	  *
	  * Add all elements from @a rhs to this list.
	  *
	  * @param[in] rhs reference of the list to add.
	  * @return reference to this.
	**/
	virtual list_t& operator+= ( const list_t& rhs ) {
		if ( &rhs != this )
			PWX_TRY_PWX_FURTHER ( base_t::operator+= ( rhs ) );
		return *this;
	}


	/** @brief substraction assignment operator
	  *
	  * Remove all elements from @a rhs from this list.
	  *
	  * @param[in] rhs reference of the list to substract.
	  * @return reference to this.
	**/
	virtual list_t& operator-= ( const list_t& rhs ) {
		if ( &rhs != this ) {
			PWX_TRY_PWX_FURTHER ( base_t::operator-= ( rhs ) );
		} else
			clear();
		return *this;
	}


	using base_t::operator[];


protected:
	/* ===============================================
	 * === Protected methods                       ===
	 * ===============================================
	*/

	using base_t::curr;
	using base_t::head;
	using base_t::tail;
	using base_t::destroy;
	using base_t::protDelete;


	/// @brief Search until the current element contains the searched data
	virtual const elem_t* protFind ( const data_t* data ) const noexcept {
		elem_t* result = nullptr;

		/* The same rules apply as in the TSingleList version,
		 * but being able to traverse backwards there are the
		 * following changes:
		 * A) Start with curr, not with root()
		 * B) When traversing backwards, do not skip head for
		 *    the same reasons tail isn't skipped when traversing
		 *    forwards.
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
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
			return xCurr;
		}

		// The next does only make sense if we have more than one element
		if ( size() > 1 ) {

			// Exit if head is wanted...
			elem_t* xHead = head();
			if ( ( xHead != xCurr ) && ( head()->data.get() == data ) ) {
				curr( xHead );
				PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
				return xHead;
			}

			// ...or tail
			elem_t* xTail = tail();
			if ( ( xTail != xCurr ) && ( tail()->data.get() == data ) ) {
				curr( xTail );
				PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
				return xTail;
			}

			// Otherwise we have to search for it.
			elem_t* oldCurr = xCurr;
			bool    isDone = false;

			xCurr   = oldCurr->getNext(); // curr is already checked.

			// oldCurr must be locked, it must not be removed
			// or deleted now!
			PWX_LOCK( oldCurr );

			// This is rule 2: Unlock for traversal
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );

			// Move upwards first
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
			}

			// Move downwards if there is no result, yet
			if ( !result ) {
				xCurr  = oldCurr->getPrev();
				isDone = false;
				PWX_UNLOCK( oldCurr );

				while ( !result && !isDone && xCurr ) {
					// Again, do not spare head.
					if ( xCurr->data.get() == data ) {
						result = xCurr;
						curr( xCurr );
					} else if ( xCurr == head() )
						isDone = true;
					else
						xCurr = xCurr->getPrev();
				}
				// End of moving downwards
			} else
				PWX_UNLOCK( oldCurr );

		} // End of handling a search with more than one element
		else
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );

		return result;
	}


	/// @brief Search until the current element contains the searched data content
	virtual const elem_t* protFindData ( const data_t& data ) const noexcept {
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
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
			return xCurr;
		}

		// The next does only make sense if we have more than one element
		if ( size() > 1 ) {

			// Exit if head is wanted...
			elem_t* xHead = head();
			if ( ( xHead != xCurr ) && ( *xHead == data ) ) {
				curr( xHead );
				PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
				return xHead;
			}

			// ...or tail
			elem_t* xTail = tail();
			if ( ( xTail != xCurr ) && ( *xTail == data ) ) {
				curr( xTail );
				PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
				return xTail;
			}

			// Otherwise we have to search for it.
			elem_t* oldCurr = xCurr;
			bool    isDone  = false;

			xCurr   = oldCurr->getNext(); // curr is already checked.

			// oldCurr must be locked, it must not be removed
			// or deleted now!
			PWX_LOCK( oldCurr );

			// This is rule 2: Unlock for traversal
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );

			// Move upwards first, unless the search starts with tail
			if ( oldCurr != tail() ) {
				// Note: This distinction is necessary, because rings
				//       connect tail to head and vice versa. If this
				//       is a ring, oldcurr would point to tail and
				//       xCurr to head, dead locking when more than
				//       one thread do this.
				while ( !result && !isDone && xCurr ) {
					// Rule 3: Re-check tail.
					// As the list is no longer locked,
					// use compare() for element level
					// locking.
					if ( 0 == xCurr->compare( data ) ) {
						curr( xCurr );
						result = xCurr;
					} else if ( xCurr == tail() )
						isDone = true;
					else
						xCurr = xCurr->getNext();
				}
			}

			// Move downwards if there is no result, yet
			if ( !result && ( oldCurr != head() ) ) {
				xCurr  = oldCurr->getPrev();
				isDone = false;
				PWX_UNLOCK( oldCurr );

				while ( !result && !isDone && xCurr ) {
					// Again, do not spare head.
					if ( 0 == xCurr->compare( data ) ) {
						result = xCurr;
						curr( xCurr );
					} else if ( xCurr == head() )
						isDone = true;
					else
						xCurr = xCurr->getPrev();
				}
				// End of moving downwards
			} else
				PWX_UNLOCK( oldCurr );
		} // End of handling a search with more than one element
		else
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );

		return result;
	}


	/// @brief Search until the next element after curr has greater data content
	virtual const elem_t* protFindGreaterNext ( const data_t& data ) const noexcept {
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
		if ( ( comp < 0 ) && ( ( nullptr == xNext ) || ( xNext->compare( data ) > -1 ) ) ) {
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
			return xNext ? xNext : nullptr;
		}

		// Quick exit if curr itself is already what we want:
		elem_t* xPrev = xCurr->getPrev();
		if ( ( comp > -1 ) && ( ( nullptr == xPrev ) || ( xPrev->compare( data ) < 0 ) ) ) {
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
			return xCurr;
		}

		// Quick exit if head is already greater...
		elem_t* xHead = head();
		if ( xHead && ( xHead->compare( data ) > -1 ) ) {
			curr( xHead );
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
			return xHead;
		}

		// ...or tail is lower
		elem_t* xTail = tail();
		if ( xTail && ( xTail->compare( data ) < 0 ) ) {
			curr( xTail );
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );
			return nullptr; // tail is prev of nullptr by definition.
		}

		// Otherwise we have to search for it.
		bool isDone = false;
		bool goUp   = true;
		if ( xCurr->compare( data ) > -1 ) {
			goUp = false;
			// xCurr is already checked and xPrev can not be nullptr at this point.
			xCurr = xPrev;
			xPrev = xCurr->getPrev();
		}

		// Rule 2:
		PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) );

		while ( !result && !isDone && xCurr && ( xNext || xPrev ) ) {
			// Note: The container is not locked any more,
			// locking and checks are done on element level
			// by compare() then.

			// Here we can do a two way search.
			if ( goUp ) {
				xNext = xCurr->getNext();
				xTail = tail();
			} else {
				xPrev = xCurr->getPrev();
				xHead = head();
			}

			// Search upwards:
			if ( goUp && ( xNext->compare( data ) > -1 ) ) {
				result = xNext;
				curr( xCurr );
			}
			// Search downwards:
			else if ( !goUp && ( xPrev->compare( data ) < 0 ) ) {
				result = xCurr;
				curr( xPrev );
			}
			// Up might end in tail (although already checked, but another
			// thread might have changed the rules.)
			else if ( goUp && ( xCurr == xTail ) ) {
				isDone = true; // result stays being nullptr
				curr( xTail );
			}
			// Down might end in head (although already checked, but another
			// thread might have changed the rules.)
			else if ( !goUp && ( xCurr == xHead ) ) {
				result = xHead;
				curr( xHead );
			} else {
				// Be safe and use getPrev()/getNext() again.
				if ( goUp )
					xCurr = xCurr->getNext();
				else
					xCurr = xCurr->getPrev();
			}
		}

		return result;
	}


	/** @brief simple method to insert an element into the list
	  *
	  * If either @a insPrev or @a insElem is marked as destroyed,
	  * a pwx::CException is thrown. Such a condition implies that
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
			if ( !size() ) {
				// Case 1: The list is empty
				PWX_TRY_PWX_FURTHER( insElem->insertBefore( nullptr, &currStore ) )
				head( insElem );
				tail( insElem );
			} else if ( nullptr == insPrev ) {
				// Case 2: A new head is to be set
				PWX_TRY_PWX_FURTHER( head()->insertPrev( insElem, &currStore ) );
				head( insElem ); // Does the renumbering already
			} else if ( insPrev == tail() ) {
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


	using base_t::protRenumber;


	/* ===============================================
	 * === Protected members                       ===
	 * ===============================================
	*/

	using base_t::currStore;
	using base_t::eCount;
	using base_t::memOrdLoad;
	using base_t::memOrdStore;


private:
	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	*/

	/* Important: Public methods should not lock unless it is crucial.
	 *            All necessary checks are done by the public methods,
	 *            work is done by the private methods which will lock
	 *            where appropriate.
	*/


	/// @brief Search until the next element contains the searched data
	// Note: This method is only used by operator-=. But the implementation
	//       is different in TSingleList, so both can use the same operator with
	//       only this detail being individual.
	virtual elem_t* privFindPrev ( const data_t* data ) const noexcept {
		elem_t*  xPrev   = curr();
		elem_t*  xCurr   = xPrev->getNext();

		while ( xPrev && ( xPrev != tail() ) ) {
			if ( xCurr->data.get() == data ) {
				curr( xCurr );
				return xPrev;
			}
			xPrev = xCurr;
			xCurr = xCurr->getNext();
		}

		// If we are here, prev points to tail. No match found.
		xCurr = curr();
		xPrev = xCurr->getPrev();

		while ( xCurr && ( xCurr != head() ) ) {
			if ( xCurr->data.get() == data ) {
				curr( xCurr );
				return xPrev;
			}
			xPrev = xCurr;
			xCurr = xCurr->getPrev();
		}

		// Here xCurr points to head, no match found.
		return nullptr;
	}


	/** @brief wrapping method to retrieve an element by any index or nullptr if the list is empty
	  * Note: The difference to the TSingleList version is, that in this variant the list
	  *       can be traversed backwards. This makes it possible to actually determine the
	  *       shortest path:
	  *              head     curr      tail
	  *              | idx idx | idx | idx |
	  * go up from head +   |     |     + go down from tail
	  *   go down from curr +     + go up from curr.
	**/
	virtual const elem_t* privGetElementByIndex ( int32_t index ) const noexcept {
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
			uint32_t xNr  = xCurr ? xCurr->nr() : 0;
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

			// Is xIdx the prev member, like in a for loop?
			if ( xIdx == ( xNr - 1 ) ) {
				xCurr = xCurr->getPrev();
				curr( xCurr );
				return xCurr;
			}

			// Ok, let's go. Find a good start and direction:
			int32_t  distCurr = xIdx - xNr; // positive: Move up, negative: move down
			uint32_t distAbs  = std::abs( distCurr );
			bool     moveUp   = true;

			if ( xIdx < distAbs ) {
				// Moving upwards from head is shortest:
				xCurr = head();
				xNr   = 0;
			} else if ( ( locCnt - xIdx ) < distAbs ) {
				// Moving downwards from tail is shortest
				xCurr  = tail();
				xNr    = locCnt - 1;
				moveUp = false;
			} else {
				// xCurr is already nearest (But xCurr and its neighbors are already checked!)
				if ( SIGN( distCurr ) < 0 ) {
					moveUp = false;
					xCurr = xCurr->getPrev();
					--xNr;
				} else {
					xCurr = xCurr->getNext();
					++xNr;
				}
			}


			// Now look into the rest
			while ( xCurr && ( xNr != xIdx ) ) {
				/* If removals lead to an xIdx beyond eCount it doesn't
				 * matter. xNr is a manual count and simply raised until
				 * xIdx is met.
				 * There are are only two special conditions:
				 * a) if xCurr is tail(), xCurr must continue with head() and vice versa.
				 * b) if the list is emptied before a result is found,
				 *    nullptr must be returned. (handled by the loop
				 *    condition!)
				 */
				// check special condition a)
				if ( ( moveUp && ( tail() == xCurr ) )
				                || ( !moveUp && ( head() == xCurr ) ) )
					xCurr = moveUp ? head() : tail();
				else
					xCurr = moveUp ? xCurr->getNext() : xCurr->getPrev();

				// Be sure special condition b) doesn't creep on us:
				if ( ( nullptr == xCurr ) && size() )
					xCurr = moveUp ? head() : tail();

				xNr += moveUp ? 1 : -1;
			} // end of searching loop

			// xCurr is sure to be pointing where it should now
			// or is nullptr if the list has been emptied
			assert ( ( xCurr || empty() ) && "ERROR: xCurr is nullptr but the list is not empty!" );
			curr( xCurr );
			return xCurr;
		} else
			PWX_UNLOCK_OBJ( const_cast<list_t*>( this ) )

			return nullptr;
	}


	/// @brief preparation method to insert data before data
	virtual uint32_t privInsDataBeforeData( data_t* next, data_t* data ) {
		// 1: Prepare the next element
		elem_t* nextElement = next ? const_cast<elem_t*>( protFind( next ) ) : nullptr;
		if ( next && ( nullptr == nextElement ) ) {
			PWX_THROW ( "ElementNotFound",
			            "Element not found",
			            "The searched element can not be found in this doubly linked list" );
		}

		// Now nextElement must not change any more
		if ( nextElement )
			PWX_LOCK( nextElement );

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( data, destroy ) )
		catch( std::exception& e ) {
			if ( nextElement )
				PWX_UNLOCK( nextElement );
			PWX_THROW( "ElementCreationFailed", e.what(),
			           "The Creation of a new list element failed." );
		}
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 3: Do the real insert
		elem_t* prev = nextElement ? nextElement->getPrev() : tail();
		if ( nextElement )
			PWX_UNLOCK( nextElement );
		PWX_TRY_PWX_FURTHER( return protInsert( prev, newElement ) );
	}


	/// @brief preparation method to insert data before an element
	virtual uint32_t privInsDataBeforeElem( elem_t* next, data_t* data ) {
		// 1: next, if set, must not change any more
		if ( next )
			PWX_LOCK( next );

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( data, destroy ) )
		catch( std::exception& e ) {
			if ( next )
				PWX_UNLOCK( next );
			PWX_THROW( "ElementCreationFailed", e.what(),
			           "The Creation of a new list element failed." );
		}
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 3: Do the real insert
		elem_t* prev = next ? next->getPrev() : tail();
		if ( next )
			PWX_UNLOCK( next );
		PWX_TRY_PWX_FURTHER( return protInsert( prev, newElement ) );
	}


	/// @brief preparation method to insert an element copy before data
	virtual uint32_t privInsElemBeforeData( data_t* next, const elem_t& src ) {
		// 1: Prepare the next element
		elem_t* nextElement = next ? const_cast<elem_t*>( protFind( next ) ) : nullptr;
		if ( next && ( nullptr == nextElement ) ) {
			PWX_THROW ( "ElementNotFound",
			            "Element not found",
			            "The searched element can not be found in this doubly linked list" );
		}

		// Now nextElement must not change any more
		if ( nextElement )
			PWX_LOCK( nextElement );

		// 2: Check source:
		PWX_LOCK_OBJ( const_cast<elem_t*>( &src ) );

		if ( src.destroyed() ) {
			// What on earth did the caller think?
			PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) );
			if ( nextElement )
				PWX_UNLOCK( nextElement );
			PWX_THROW( "Illegal Condition", "Source element destroyed",
			           "An element used as source for insertion is destroyed." );
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( src ) )
		catch( std::exception& e ) {
			if ( nextElement )
				PWX_UNLOCK( nextElement );
			PWX_THROW( "ElementCreationFailed", e.what(),
			           "The Creation of a new list element failed." );
		}
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 4: Do the real insert
		elem_t* prev = nextElement ? nextElement->getPrev() : tail();
		if ( nextElement )
			PWX_UNLOCK( nextElement );
		PWX_TRY_PWX_FURTHER( return protInsert( prev, newElement ) );
	}


	/// @brief preparation method to insert an element copy before an element
	virtual uint32_t privInsElemBeforeElem( elem_t* next, const elem_t& src ) {
		// 1: next, if set must not change any more
		if ( next )
			PWX_LOCK( next );

		// 2: Check source:
		PWX_LOCK_OBJ( const_cast<elem_t*>( &src ) );

		if ( src.destroyed() ) {
			// What on earth did the caller think?
			PWX_UNLOCK_OBJ( const_cast<elem_t*>( &src ) );
			if ( next )
				PWX_UNLOCK( next );
			PWX_THROW( "Illegal Condition", "Source element destroyed",
			           "An element used as source for insertion is destroyed." );
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( src ) )
		catch( std::exception& e ) {
			if ( next )
				PWX_UNLOCK( next );
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." );
		}
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 4: Do the real insert
		elem_t* prev = next ? next->getPrev() : tail();
		if ( next )
			PWX_UNLOCK( next );
		PWX_TRY_PWX_FURTHER( return protInsert( prev, newElement ) );
	}


	/// @brief simple method to remove an element from the list.
	virtual elem_t* privRemove ( elem_t* elem ) noexcept {
		// return at once if there is no element to remove
		if ( !elem || elem->removed() || elem->destroyed() )
			return nullptr;

		/* The following scenarios are possible:
		 * 1: elem is head
		 * 2: elem is tail
		 * 3: elem is something else.
		*/
		if ( head() == elem ) {
			// Case 1
			PWX_LOCK_GUARD( this );
			/* The reasons for the double check are the same as
			 * with TSingleList::privRemoveNextElem()
			 */
			if ( head() == elem ) {
				PWX_LOCK_GUARD_RESET( NULL_LOCK );
				head( head()->getNext() );
			}
		} else if ( tail() == elem ) {
			// Case 2:
			PWX_LOCK_OBJ( this );
			if ( tail() == elem )
				tail( tail()->getPrev() );
			PWX_UNLOCK_OBJ( this );
		} else
			this->doRenumber.store( true, memOrdStore );
		elem->remove();

		if ( 1 == eCount.fetch_sub( 1 ) ) {
			// The list is empty!
			PWX_LOCK_OBJ( this );
			// Is it really?
			if ( 0 == eCount.load( memOrdLoad ) ) {
				curr( nullptr );
				head( nullptr );
				tail( nullptr );
			}
			PWX_UNLOCK_OBJ( this );
		}

		return elem;
	}


	/// @brief simple wrapper to prepare the direct removal of data
	virtual elem_t* privRemoveData( data_t* data ) noexcept {
		return privRemove ( data ? const_cast<elem_t*>( protFind( data ) ) : nullptr );
	}


	/** @brief remove the element after the specified data
	  * If @a prev data can not be found, nothing happens and nullptr is returned.
	  * @return nullptr if the element holding @a prev is the last element or the list is empty.
	**/
	virtual elem_t* privRemoveAfterData( data_t* prev ) noexcept {
		elem_t* xPrev    =  prev ? const_cast<elem_t*>( protFind ( prev ) ) : nullptr;
		elem_t* toRemove = xPrev ? xPrev->getNext() : prev ? nullptr : head();

		if ( toRemove )
			return privRemove ( toRemove );

		return nullptr;
	}


	/** @brief remove the element after the specified element
	  * @param[in] prev pointer to the element after which to remove an element
	  * @return the removed element or nullptr if prev->next is nullptr or the list is empty
	**/
	virtual elem_t* privRemoveAfterElement( elem_t* prev ) noexcept {
		return privRemove( prev ? prev->getNext() : head() );
	}


	/** @brief simple wrapper to prepare the removal of an element before data
	  * If @a next data can not be found, nothing happens and nullptr is returned.
	  * @return nullptr if @a next is held by the first element or the list is empty.
	**/
	virtual elem_t* privRemoveBeforeData( data_t* next ) noexcept {
		elem_t* xNext = next ? const_cast<elem_t*>( protFind ( next ) ) : nullptr;
		elem_t* toRemove = xNext ? xNext->getPrev() : next ? nullptr : tail();

		if ( toRemove )
			return privRemove ( toRemove );

		return nullptr;
	}


	/** @brief simple wrapper to prepare the removal of an element before another element
	  * @return nullptr if @a next is the first element or the list is empty.
	**/
	virtual elem_t* privRemoveBeforeElem( elem_t* next ) noexcept {
		return privRemove( next ? next->getPrev() : tail() );
	}

}; // class TDoubleList

/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the list.
**/
template<typename data_t, typename elem_t>
TDoubleList<data_t, elem_t>::~TDoubleList() noexcept
{ }


/** @brief addition operator
  *
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
TDoubleList<data_t, elem_t> operator+ ( const TDoubleList<data_t, elem_t>& lhs, const TDoubleList<data_t, elem_t>& rhs ) {
	TDoubleList<data_t, elem_t> result ( lhs );

	if ( &lhs != &rhs )
		PWX_TRY_PWX_FURTHER ( result += rhs );

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
TDoubleList<data_t, elem_t> operator- ( const TDoubleList<data_t, elem_t>& lhs, const TDoubleList<data_t, elem_t>& rhs ) {
	TDoubleList<data_t, elem_t> result ( lhs );

	if ( &lhs != &rhs )
		PWX_TRY_PWX_FURTHER ( result -= rhs )
		else
			result.clear();

	return result;
}


} // namespace pwx


#endif // PWX_LIBPWX_CONTAINER_TDOUBLELIST_H_INCLUDED
