/**
  * This file is part of the PrydeWorX Library (pwxLib).
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


#include "basic/macros.h"
#include "container/CThreadElementStore.h"

#include <cstddef>
#include <cstdint>


/// @namespace pwx
namespace pwx {


/** @brief do nothing with the given element
  * This is just a little "nothing-to-see-here"-function
  * that is used by the `CThreadElementStore` below as
  * a deleter for `std::shared_ptr`.
**/
static void do_not_destroy( CThreadElementStore::curr_t* ) { /* Don't you dare! */ }


/* ===============================================
 * === Public Constructors and destructors     ===
 * ===============================================
*/

CThreadElementStore::CThreadElementStore( uint32_t initial_size ) :
	  currs( initial_size, do_not_destroy, nullptr, 0.6667, 2.063829787234043 ) {}

/// @brief delegating ctor setting default size of 47
CThreadElementStore::CThreadElementStore() noexcept: store_t( 47 ) {}


/// @brief default dtor
CThreadElementStore::~CThreadElementStore() noexcept {
	invalidating.store( true );
	PWX_DOUBLE_LOCK_GUARD( this, &currs );

	isDestroyed.store( true );

	// Reset locks so that other threads may notice that this is gone
	while ( waiting() ) {
		PWX_DOUBLE_LOCK_GUARD_RESET( this, &currs );
	}

	// Now the coast is clear
	currs.clear();


	invalidating.store( false );

	// Reset locks again before finishing the destructor eventually
	while ( waiting() ) {
		PWX_DOUBLE_LOCK_GUARD_RESET( this, &currs );
	}
}


/* ===============================================
 * === Public Methods                          ===
 * ===============================================
*/


void CThreadElementStore::clear() noexcept {
	if ( beThreadSafe() ) {
		if ( !currs.empty() && !invalidating.load( memOrdLoad ) ) {
			invalidating.store( true, memOrdStore );
			PWX_LOCK_GUARD( this );
			// Is a clearing still needed?
			if ( !currs.empty() ) {
				currs.clear();
			}
			invalidating.store( false, memOrdStore );
		}
	} else {
		oneCurr = nullptr;
	}
}


CThreadElementStore::curr_t* CThreadElementStore::curr() const noexcept {
	if ( beThreadSafe() ) {
		bool needLock = invalidating.load( memOrdLoad );
		if ( needLock ) const_cast<CThreadElementStore*>( this )->lock();
		elem_t* elem = currs.get( CURRENT_THREAD_ID );
		if ( needLock ) const_cast<CThreadElementStore*>( this )->unlock();
		return elem ? elem->data.get() : nullptr;
	} else {
		return oneCurr;
	}
}


/// Note: Copied, not called, as these two lines are
///       too less of a burden to add a const_cast
///       calling layer
CThreadElementStore::curr_t* CThreadElementStore::curr() noexcept {
	if ( beThreadSafe() ) {
		bool needLock = invalidating.load( memOrdLoad );
		if ( needLock ) lock();
		elem_t* elem = currs.get( CURRENT_THREAD_ID );
		if ( needLock ) unlock();
		return elem ? elem->data.get() : nullptr;
	} else {
		return oneCurr;
	}
}


void CThreadElementStore::curr( const CThreadElementStore::curr_t* new_curr ) const noexcept {
	if ( beThreadSafe() ) {
		PWX_DOUBLE_LOCK_GUARD( const_cast<CThreadElementStore*>( this ), &currs );

		if ( !isDestroyed.load() && !currs.destroyed() ) {
			currs.delKey( CURRENT_THREAD_ID );
			if ( new_curr ) {
				PWX_TRY( currs.add( CURRENT_THREAD_ID, const_cast<curr_t*>( new_curr ) ) )
				PWX_CATCH_AND_FORGET( CException )
			}
		}
	} else {
		oneCurr = const_cast<curr_t*>( new_curr );
	}
}


void CThreadElementStore::curr( CThreadElementStore::curr_t* new_curr ) noexcept {
	if ( beThreadSafe() ) {
		PWX_DOUBLE_LOCK_GUARD( this, &currs );

		if ( !isDestroyed.load() && !currs.destroyed() ) {
			currs.delKey( CURRENT_THREAD_ID );
			if ( new_curr ) {
				PWX_TRY( currs.add( CURRENT_THREAD_ID, new_curr ) )
				PWX_CATCH_AND_FORGET( CException )
			}
		}
	} else {
		oneCurr = new_curr;
	}
}


void CThreadElementStore::disable_thread_safety() noexcept {
	if ( beThreadSafe() ) {
		oneCurr = nullptr;
		beThreadSafe( false );
	}
}


void CThreadElementStore::enable_thread_safety() noexcept {
	if ( !beThreadSafe() ) {
		currs.clear();
		beThreadSafe( true );
	}
}


void CThreadElementStore::invalidate( const CThreadElementStore::curr_t* old_curr ) const noexcept {
	if ( beThreadSafe() ) {
		// Only do anything if there are any elements stored:
		if ( !currs.empty() && !invalidating.load( memOrdLoad ) ) {
			invalidating.store( true, memOrdStore );
			PWX_LOCK_GUARD( this );

			// Do a double check, maybe the invalidating is no longer needed
			if ( !currs.empty() ) {
				uint32_t currSize = currs.sizeMax();
				elem_t* elem;
				for ( uint32_t i = 0 ; i < currSize ; ++i ) {
					elem = currs[i];
					if ( elem && ( elem->data.get() == old_curr ) ) {
						currs.delKey( elem->key );
					}
				}
			} // End of having to invalidate
			invalidating.store( false, memOrdStore );
		} // End of entry check
	} else if ( oneCurr && ( oneCurr == old_curr ) ) {
		oneCurr = nullptr;
	}
}


[[maybe_unused]] void CThreadElementStore::invalidate( CThreadElementStore::curr_t* old_curr ) const noexcept {
	this->invalidate( static_cast<const curr_t*>( old_curr ) );
}


} // namespace pwx
