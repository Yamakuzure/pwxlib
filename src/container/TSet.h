#ifndef PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED
#define PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED 1
#pragma once

/** @file TSet.h
  *
  * @brief Declaration of the TSet template to store unique elements
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


#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"

#include "basic/CLockable.h"
#include "container/TDoubleList.h"
#include "container/TChainHash.h"
#include "functions/pwx_set_fwd.h"


namespace pwx {


/** @class TSet PSet <PSet>
  *
  * @brief Template to build sets of variable types
  *
  * A set is a group of elements, where each element exists exactly once. Two
  * sets are equal, if their members are equal. Therefore the sets {1, 2, 3} and
  * {3, 2, 1} are equal, although sets are explicitly unordered. To speed up
  * access TSet uses a chained hash table to search and verify data.
  *
  * The set is derived from `pwx::TDoubleList` to manage its element. Every element
  * is checked for uniqueness before storing it in a set.
  *
  * This is done on the data level, not pointer level. This makes it necessary for
  * any data to support operator== and operator>.
  *
  * The constructor takes an optional `destroy(data_t*)` function pointer that is
  * used to destroy the data when the element is deleted. If no such function was
  * set, the standard delete operator is used instead.
  *
  * Set operations to build unions, differences and intersections are implemented
  * outside the class with functions prefixed with set_.
  *
  * To maintain the order of elements as they are added, the standard operations
  * to add are `push()`, add an element to the end of the set, and `pop()`, take
  * an element from the beginning of the set.
  * The two reversal methods `unshift()` and `shift()` to add to the beginning and
  * to take from the end are available for convenience.
  *
  * It is recommended that you use the much more advanced `std::set` unless you need
  * to store a very large number of elements and can not live with the downside of
  * every element having to be copied into the std container.
  *
  * @see `pwx::TDoubleList` for further information.
**/
template<typename data_t>
class PWX_API TSet : public TDoubleList<data_t> {
public:
	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef TDoubleElement<data_t>      elem_t; //!< Type of the stored elements
	typedef TDoubleList<data_t, elem_t> base_t; //!< Base type of the set
	typedef TSet<data_t>                list_t; //!< Type of this set
	typedef TChainHash<data_t, elem_t>  hash_t; //!< Chained hash for data lookup


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	*/

	/** @brief default constructor
	  *
	  * The default constructor not only initializes an empty set
	  * and sets the desired destroy method, but also initializes
	  * the lookup table to non-default values.
	  *
	  * You can use this ctor if you already know that you need
	  * non-default values like a different maximum load factor
	  * or a higher initial size.
	  *
	  * Note: You can not set an own destroy function for the lookup
	  * table, as the data of the lookup hash elements is never
	  * destroyed; they are the regular set elements.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data of the set elements
	  * @param[in] initSize The initial size of the lookup table.
	  * @param[in] hash_func A pointer to a function that can hash the data that is stored in the set elements
	  * @param[in] keyLen optional limiting key length for C-Strings, std::string keys or @a hash_func
	  * @param[in] maxLoad maximum load factor that triggers automatic growth of the lookup table.
	  * @param[in] dynGrow growth rate applied when the maximum load factor is reached.
	**/
	TSet ( void ( *destroy_ ) ( data_t* data ),
	       uint32_t initSize,
	       uint32_t ( *hash_func ) ( const key_t* key, uint32_t keyLen ),
	       uint32_t keyLen,
	       double maxLoad, double dynGrow ) noexcept :
		base_t( destroy_ ),
		lookup( initSize, list_t::do_not_destroy, hash_func, keyLen, maxLoad, dynGrow )
	{ }



	/** @brief destroy constructor
	  *
	  * The destroy constructor initializes an empty set.
	  *
	  * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
	**/
	TSet ( void ( *destroy_ ) ( data_t* data ) ) noexcept :
		base_t( destroy_ ),
		lookup( list_t::do_not_destroy )
	{ }


	/** @brief empty constructor
	  *
	  * The empty constructor sets the data destroy method to the null pointer.
	**/
	TSet() noexcept :
		base_t ( nullptr ),
		lookup( list_t::do_not_destroy )
	{ }


	/** @brief copy constructor
	  *
	  * Builds a copy of all elements of @a src.
	  *
	  * @param[in] src reference of the list to copy.
	**/
	TSet ( const list_t& src ) noexcept :
		base_t ( src ),
		lookup( list_t::do_not_destroy ) {
		// The copy ctor of base_t has already copied all elements.
		// However, lookup is empty and needs to be filled:
		elem_t* xCurr = head();
		elem_t* xTail = tail();
		bool    done  = false;
		while ( xCurr && !done ) {
			lookup.add( **xCurr, xCurr );
			if ( xCurr == xTail )
				done = true;
			else
				xCurr = xCurr->getNext();
		}
	}


	virtual ~TSet() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	using base_t::clear;
	using base_t::delData;
	using base_t::delElem;
	using base_t::delNext;
	using base_t::delNextElem;
	using base_t::delPrev;
	using base_t::delPrevElem;
	using base_t::disable_thread_safety;
	using base_t::empty;
	using base_t::enable_thread_safety;
	using base_t::find;
	using base_t::get;
	using base_t::getData;


	/** @brief return true if @a elem is an element of this set
	  *
	  * @param[in] elem reference to the element to test
	  * @return true if the element is a member of this set, false otherwise
	**/
	virtual bool hasMember( const elem_t& elem ) const noexcept {
		return ( protFindData( *elem ) ? true : false );
	}


	/** @brief return true if @a data is an element of this set
	  *
	  * @param[in] data reference to the data to test
	  * @return true if the data is a member of this set, false otherwise
	**/
	virtual bool hasMember( const data_t& data ) const noexcept {
		return ( protFindData( data ) ? true : false );
	}


	using base_t::insNext;
	using base_t::insNextElem;
	using base_t::insPrev;
	using base_t::insPrevElem;


	/** @brief return true if this set is a subset of @a src
	  *
	  * A set A is a subset of set B, if set B has all elements
	  * of set A.
	  *
	  * @param[in] src reference of the set to test against
	  * return true if this set is a subset of @a src
	**/
	virtual bool isSubsetOf( const list_t& src ) const noexcept {
		bool     result     = true;

		// The empty set is always a subset of everything.
		if ( eCount.load( memOrdLoad )
		                && ( this != &src ) ) {
			if ( src.eCount.load( memOrdLoad ) ) {
				PWX_DOUBLE_LOCK_GUARD( this, &src );
				elem_t* xCurr  = head();
				bool    isDone = false;

				// A simple loop will do, because we can use protFindData directly.
				while ( result && xCurr && xCurr->data.get() && !isDone ) {
					if ( src.protFindData( **xCurr ) ) {
						if ( xCurr == tail() )
							isDone = true;
						else
							xCurr = xCurr->getNext();
					} else
						result = false;
				}
			} else
				result = false;
		}

		return result;
	}


	using base_t::pop;
	using base_t::pop_back;
	using base_t::pop_front;
	using base_t::push;
	using base_t::push_back;
	using base_t::push_front;
	using base_t::remData;
	using base_t::remElem;
	using base_t::remNext;
	using base_t::remNextElem;
	using base_t::remPrev;
	using base_t::remPrevElem;


	/** @brief reset a set to a predefined state of a different set
	  *
	  * This method can be used to clear a set and copy both the
	  * sorted switch and the destroy method from another set.
	  * Basically this is meant to build a working set without
	  * having to use the copy constructor, which would copy all
	  * members as well.
	  *
	  * If a set is reseted using itself, it is cleared of all elements
	  * nevertheless.
	  *
	  * @param[in] src reference of the set to take the base values from
	**/
	virtual void reset( const list_t& src ) noexcept {
		// A big lock on both is needed to ensure that
		// a) Nothing is added by another thread and
		// b) src can not go away before the reset is done.
		if ( beThreadSafe() ) {
			PWX_LOCK_OBJ( this );
			if ( this != &src ) {
				while ( !const_cast<list_t*>( &src )->try_lock() ) {
					PWX_UNLOCK_OBJ( this );
					PWX_LOCK_OBJ( this );
				}
			}
		}

		// Now do the work
		clear();
		if ( &src != this )
			destroy  = src.destroy;

		// Unlock if needed
		if ( beThreadSafe() ) {
			PWX_UNLOCK_OBJ( const_cast<list_t*>( &src ) )
			PWX_UNLOCK_OBJ( this );
		}
	}


	/** @brief shift an element from the set
	  *
	  * This is the irregular set operation shifting an element
	  * from the end of the set.
	  *
	  * To get an element from the start, use pop() or pop_front().
	  *
	  * The element is removed from the set so you have to take
	  * care of its deletion once you are finished with it.
	  *
	  * If there is no element to shift, nullptr is returned.
	  *
	  * @return the top element on the set.
	**/
	virtual elem_t* shift() noexcept {
		return privRemove( tail() );
	}


	using base_t::size;


	/** @brief unshift an element onto the set
	  *
	  * This is the irregular set operation unshifting an element
	  * onto the start of the set.
	  *
	  * To add an element to the end, use push() or push_back().
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] data data pointer to store.
	  * @return number of elements stored after the operation.
	**/
	virtual uint32_t unshift( data_t* data ) {
		PWX_TRY_PWX_FURTHER( return privInsDataBeforeElem ( head(), data ) )
	}


	/** @brief unshift an element copy onto the set
	  *
	  * This is the irregular set operation unshifting an element
	  * copy onto the start of the set.
	  *
	  * To add an element to the end, use push() or push_back().
	  *
	  * If the new element can not be created, a pwx::CException with
	  * the name "ElementCreationFailed" is thrown.
	  *
	  * @param[in] src element to copy.
	  * @return number of elements stored after the operation.
	**/
	virtual uint32_t unshift( const elem_t& src ) {
		PWX_TRY_PWX_FURTHER( return privInsElemBeforeElem ( head(), src ) )
	}


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief assignment operator
	  *
	  * Clears this set and copies all elements from @a rhs
	  * onto this set.
	  *
	  * @param[in] rhs reference of the set to copy.
	  * @return reference to this.
	**/
	virtual list_t& operator= ( const list_t& rhs ) {
		if ( &rhs != this ) {
			PWX_DOUBLE_LOCK_GUARD ( this, const_cast<list_t*> ( &rhs ) );
			clear();
			destroy = rhs.destroy;
			PWX_TRY_PWX_FURTHER ( *this += rhs )
		}
		return *this;
	}


	using base_t::operator+=;
	using base_t::operator-=;
	using base_t::operator[];


protected:
	/* ===============================================
	 * === Protected methods                       ===
	 * ===============================================
	*/

	using base_t::beThreadSafe;
	using base_t::head;
	using base_t::tail;
	using base_t::destroy;


	/// @brief Search until the current element contains the searched data
	virtual const elem_t* protFind ( const data_t* data ) const noexcept {
		// Having a lookup hash this is simple,
		if ( !data ) return nullptr;
		elem_t* result = const_cast<elem_t*>( protFindData( *data ) );
		// But the *pointer* must match and not only the data
		return ( result && result->data.get() == data ) ? result : nullptr;
	}


	/// @brief Search until the current element contains the searched data content
	virtual const elem_t* protFindData ( const data_t& data ) const noexcept {
		// Very simple with a lookup hash:
		auto hashElem = lookup.get( data );
		return ( hashElem ? hashElem->data.get() : nullptr );
	}


	/** @brief simple method to insert an element into the list
	  *
	  * If either @a insPre or @a insElem is marked as destroyed,
	  * a pwx::CException is thrown. Such a condition implies that
	  * there is something seriously wrong.
	  *
	  * Note: As the private preparation methods that prepare the insert
	  *       have to lock the set to ensure consistency, this variant
	  *       is different from the one in TDoubleList as it does not
	  *       need to lock by itself.
	  *
	  * @param[in] insPre Element after which the new element is to be inserted
	  * @param[in] insElem The element to insert.
	  * @return The number of elements in the list after the insertion
	**/
	virtual uint32_t protInsert ( elem_t* insPre, elem_t* insElem ) {
		/* There are four possibilities:
		 * 1: The list is empty
		 *    head, tail and curr have to be set to the new
		 *    element, no full renumbering is needed then.
		 * 2: insPre is nullptr
		 *    head has to be changed to be the new element
		 * 3: insPre is tail
		 *    tail has to be set to the new element, no full
		 *    renumbering is needed then.
		 * 4: Otherwise insPre->insertNext() can do the insertion
		*/

		if ( size() ) {
			if ( insPre ) {
				if ( tail() == insPre ) {
					// Case 3: A new tail is to be set
					insElem->nr( tail()->nr() + 1 );
					PWX_TRY_PWX_FURTHER( tail()->insertNext( insElem, &currStore ) )
					tail( insElem );
				} else {
					// Case 4: A normal insert
					this->doRenumber.store( true, memOrdStore );
					PWX_TRY_PWX_FURTHER( insPre->insertNext( insElem, &currStore ) )
				}
			} else {
				// Case 2: A new head is to be set
				PWX_TRY_PWX_FURTHER( head()->insertPrev( insElem, &currStore ) )
				head( insElem ); // Does the renumbering already
			}
		} else {
			// Case 1: The list is empty
			PWX_TRY_PWX_FURTHER( insElem->insertBefore( nullptr, &currStore ) )
			head( insElem );
			tail( insElem );
		}

		// Do this last or non-locking methods would
		// find the element before it is really inserted.
		lookup.add( **insElem, insElem );

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

	/// @internal do nothing with the element. (lookup must not delete!)
	static void do_not_destroy( elem_t* ) { }


	/// @brief clear this list
	/// Difference to TSingleList::privClear(): Here the lookup table
	/// must be maintained instead of the currStore. It is therefore
	/// not possible to simply "declare" the Set as being empty.
	virtual void privClear() noexcept {
		elem_t* xTail = nullptr;
		while ( tail() ) {
			PWX_LOCK_OBJ( this );
			xTail = privRemove( tail() );
			PWX_LOCK( xTail );
			PWX_UNLOCK_OBJ( this );
			if ( xTail && !xTail->destroyed() ) {
				PWX_UNLOCK( xTail );
				delete xTail;
			}
		}
	}


	/// @brief Search until the next element contains the searched data
	virtual elem_t* privFindPrev ( const data_t* data ) const noexcept {
		elem_t* xCurr = const_cast<elem_t*>( protFind( data ) );
		return xCurr ? xCurr->getPrev() : nullptr;
	}


	/// @brief preparation method to insert data behind data
	virtual uint32_t privInsDataBehindData( data_t* prev, data_t* data ) {
		// Return at once if the data is already known:
		if ( !data || protFindData( *data ) )
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_NAMED_LOCK_GUARD( the_set, this );

		// Return at once if the data is now known the lock is in place:
		if ( protFindData( *data ) )
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>( protFind( prev ) ) : nullptr;
		if ( prev && ( nullptr == prevElement ) ) {
			PWX_THROW ( "ElementNotFound",
			            "Element not found",
			            "The searched element can not be found in this set" )
		}

		// Now prevElement must not change any more
		PWX_NAMED_LOCK_GUARD( the_prev_element, prevElement );

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( data, destroy ) )
		catch( std::exception& e ) {
			PWX_THROW( "ElementCreationFailed", e.what(),
			           "The Creation of a new list element failed." )
		}
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER( return protInsert( prevElement, newElement ) )
	}


	/// @brief preparation method to insert data behind an element
	virtual uint32_t privInsDataBehindElem( elem_t* prev, data_t* data ) {
		// Return at once if the data is already known:
		if ( !data || protFindData( *data ) )
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_DOUBLE_LOCK_GUARD( this, prev );

		// Return at once if the data is now known the lock is in place:
		if ( protFindData( *data ) )
			return size();

		// 1: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( data, destroy ) )
		catch( std::exception& e ) {
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." )
		}
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER( return protInsert( prev, newElement ) )
	}


	/// @brief preparation method to insert an element copy behind data
	virtual uint32_t privInsElemBehindData( data_t* prev, const elem_t& src ) {
		// Return at once if the source data is already known:
		if ( protFindData( *src ) )
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_NAMED_DOUBLE_LOCK_GUARD( set_and_source, this, &src );

		// Return at once if the source data is now known the lock is in place:
		if ( protFindData( *src ) )
			return size();

		// 1: Prepare the previous element
		elem_t* prevElement = prev ? const_cast<elem_t*>( protFind( prev ) ) : nullptr;
		if ( prev && ( nullptr == prevElement ) ) {
			PWX_THROW ( "ElementNotFound",
			            "Element not found",
			            "The searched element can not be found in this singly linked list" )
		}

		// Now prevElement must not change any more
		PWX_NAMED_LOCK_GUARD( the_prev_element, prevElement );

		// 2: Check source:
		if ( src.destroyed() ) {
			// What on earth did the caller think?
			PWX_THROW( "Illegal Condition", "Source element destroyed",
			           "An element used as source for insertion is destroyed." )
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( src ) )
		catch( std::exception& e ) {
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." )
		}

		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 4: Do the real insert
		PWX_TRY_PWX_FURTHER( return protInsert( prevElement, newElement ) )
	}


	/// @brief preparation method to insert an element copy behind an element
	virtual uint32_t privInsElemBehindElem( elem_t* prev, const elem_t& src ) {
		// Return at once if the source data is already known:
		if ( protFindData( *src ) )
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_TRIPLE_LOCK_GUARD( this, prev, &src );

		// Return at once if the source data is now known the lock is in place:
		if ( protFindData( *src ) )
			return size();

		// 1: Check source:
		if ( src.destroyed() ) {
			// What on earth did the caller think?
			PWX_THROW( "Illegal Condition", "Source element destroyed",
			           "An element used as source for insertion is destroyed." )
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( src ) )
		catch( std::exception& e ) {
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." )
		}

		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 4: Do the real insert
		PWX_TRY_PWX_FURTHER( return protInsert( prev, newElement ) )
	}


	/// @brief preparation method to insert data behind data
	virtual uint32_t privInsDataBeforeData( data_t* next, data_t* data ) {
		// Return at once if the data is already known:
		if ( !data || protFindData( *data ) )
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_NAMED_LOCK_GUARD( the_set, this );

		// Return at once if the data is now known the lock is in place:
		if ( protFindData( *data ) )
			return size();

		// 1: Prepare the nextious element
		elem_t* nextElement = next ? const_cast<elem_t*>( protFind( next ) ) : nullptr;
		if ( next && ( nullptr == nextElement ) ) {
			PWX_THROW ( "ElementNotFound",
			            "Element not found",
			            "The searched element can not be found in this set" )
		}
		elem_t* prevElement = nextElement ? nextElement->getPrev() : nullptr;


		// Now prevElement must not change any more
		PWX_NAMED_LOCK_GUARD( the_prev_element, prevElement );

		// 2: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( data, destroy ) )
		catch( std::exception& e ) {
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." )
		}
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER( return protInsert( prevElement, newElement ) )
	}


	/// @brief preparation method to insert data behind an element
	virtual uint32_t privInsDataBeforeElem( elem_t* next, data_t* data ) {
		// Return at once if the data is already known:
		if ( !data || protFindData( *data ) )
			return size();

		// It is important to have the previous element, not the next:
		elem_t* prevElement = next ? next->getPrev() : nullptr;

		// To ensure set consistency, a big lock is required:
		PWX_DOUBLE_LOCK_GUARD( this, prevElement );

		// Return at once if the data is now known the lock is in place:
		if ( protFindData( *data ) )
			return size();

		// 1: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( data, destroy ) )
		catch( std::exception& e ) {
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." )
		}
		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 3: Do the real insert
		PWX_TRY_PWX_FURTHER( return protInsert( prevElement, newElement ) )
	}


	/// @brief preparation method to insert an element copy behind data
	virtual uint32_t privInsElemBeforeData( data_t* next, const elem_t& src ) {
		// Return at once if the source data is already known:
		if ( protFindData( *src ) )
			return size();

		// To ensure set consistency, a big lock is required:
		PWX_NAMED_DOUBLE_LOCK_GUARD( set_and_source, this, &src );

		// Return at once if the source data is now known the lock is in place:
		if ( protFindData( *src ) )
			return size();

		// 1: Prepare the nextious element
		elem_t* nextElement = next ? const_cast<elem_t*>( protFind( next ) ) : nullptr;
		if ( next && ( nullptr == nextElement ) ) {
			PWX_THROW ( "ElementNotFound",
			            "Element not found",
			            "The searched element can not be found in this singly linked list" )
		}
		elem_t* prevElement = nextElement ? nextElement->getPrev() : nullptr;

		// Now prevElement must not change any more
		PWX_NAMED_LOCK_GUARD( the_next_element, prevElement );

		// 2: Check source:
		if ( src.destroyed() ) {
			// What on earth did the caller think?
			PWX_THROW( "Illegal Condition", "Source element destroyed",
			           "An element used as source for insertion is destroyed." )
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( src ) )
		catch( std::exception& e ) {
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." )
		}

		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 4: Do the real insert
		PWX_TRY_PWX_FURTHER( return protInsert( prevElement, newElement ) )
	}


	/// @brief preparation method to insert an element copy behind an element
	virtual uint32_t privInsElemBeforeElem( elem_t* next, const elem_t& src ) {
		// Return at once if the source data is already known:
		if ( protFindData( *src ) )
			return size();

		// Use prev, not next:
		elem_t* prevElement = next ? next->getPrev() : nullptr;

		// To ensure set consistency, a big lock is required:
		PWX_TRIPLE_LOCK_GUARD( this, prevElement, &src );

		// Return at once if the source data is now known the lock is in place:
		if ( protFindData( *src ) )
			return size();

		// 1: Check source:
		if ( src.destroyed() ) {
			// What on earth did the caller think?
			PWX_THROW( "Illegal Condition", "Source element destroyed",
			           "An element used as source for insertion is destroyed." )
		}

		// 3: Create a new element
		elem_t* newElement = nullptr;
		PWX_TRY( newElement = new elem_t ( src ) )
		catch( std::exception& e ) {
			PWX_THROW( "ElementCreationFailed", e.what(), "The Creation of a new list element failed." )
		}

		if ( !this->beThreadSafe() )
			newElement->disable_thread_safety();

		// 4: Do the real insert
		PWX_TRY_PWX_FURTHER( return protInsert( prevElement, newElement ) )
	}


	/// @brief simple method to remove an element from the list.
	virtual elem_t* privRemove ( elem_t* elem ) noexcept {
		// return at once if there is no element to remove
		if ( !elem || elem->removed() || elem->destroyed() )
			return nullptr;

		// Take out of lookup, so it isn't found any more:
		lookup.delKey( **elem );

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
				head( nullptr );
				tail( nullptr );
			}
			PWX_UNLOCK_OBJ( this );
		}

		return elem;
	}


	/// @brief simple wrapper to prepare the direct removal of data
	virtual elem_t* privRemoveData( data_t* data ) noexcept {
		return privRemove ( data ? const_cast<elem_t*>( protFindData( *data ) ) : nullptr );
	}


	/** @brief remove the element after the specified data
	  *
	  * If @a prev data can not be found, nothing happens and nullptr is returned.
	  *
	  * Being a set this method searches for the data content and <U>not</U> the pointer!
	  *
	  * @return nullptr if the element holding @a prev is the last element or the list is empty.
	**/
	virtual elem_t* privRemoveAfterData( data_t* prev ) noexcept {
		elem_t* xPrev = prev ? const_cast<elem_t*>( protFindData ( *prev ) ) : nullptr;
		elem_t* toRemove = xPrev ? xPrev->getNext() : prev ? nullptr : head();

		if ( toRemove )
			return privRemove ( toRemove );

		return nullptr;
	}


	/** @brief simple wrapper to prepare the removal of an element before data
	  *
	  * If @a next data can not be found, nothing happens and nullptr is returned.
	  *
	  * Being a set this method searches for the data content and <U>not</U> the pointer!
	  *
	  * @return nullptr if @a next is held by the first element or the list is empty.
	**/
	virtual elem_t* privRemoveBeforeData( data_t* next ) noexcept {
		elem_t* xNext = next ? const_cast<elem_t*>( protFindData( *next ) ) : nullptr;
		elem_t* toRemove = xNext ? xNext->getPrev() : next ? nullptr : tail();

		if ( toRemove )
			return privRemove ( toRemove );

		return nullptr;
	}


	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	hash_t lookup; //!< Quick search for data without having to traverse (and lock) the whole set.

}; // class TSet


/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the set.
**/
template<typename data_t>
TSet<data_t>::~TSet() noexcept
{ /* deletion is done in base_t dtor */ }


} // namespace pwx

#include "functions/pwx_set_func.h"
#endif // PWX_PWXLIB_PWX_CONTAINER_TSET_H_INCLUDED

