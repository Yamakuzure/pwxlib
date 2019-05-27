#ifndef PWX_LIBPWX_TYPES_TSINGLEELEMENT_H_INCLUDED
#define PWX_LIBPWX_TYPES_TSINGLEELEMENT_H_INCLUDED 1
#pragma once

/** @file TSingleElement.h
  *
  * @brief Declaration of a basic template for singly linked list elements
  *
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
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


#include "basic/CException.h"
#include "basic/pwx_debug.h"
#include "container/TVarDeleter.h"
#include "container/VElement.h"
#include "math_helpers/MathHelpers.h"


namespace pwx {


/** @struct TSingleElement
  *
  * @brief Template for an element of a singly linked list or ring of variable types
  *
  * This is a very simple and basic type to wrap a pointer of variable type into
  * an object that is used with pwx::TSingleList and pwx::TSingleRing.
  *
  * The constructor takes an optional destroy(T*) function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * The data pointer itself is wrapped into an std::shared_ptr. It is therefore
  * completely safe to copy TSingleElement instances.
  *
  * The data pointer itself is public. You can use foo->data.get() to access it.
  * Further the operator* is overloaded and **foo will result in a reference to the
  * data.
  *
  * The next element in the list can be retrieved using the public foo->next pointer.
  *
  * If you plan to use this type in a multi-threaded environment, you can use the
  * getNext() and setNext() functions to manipulate the next pointer. See below for
  * more on multi threaded usage.
  *
  * To insert any element into a list you can use insertNext() to have it inserted
  * after the called element safely. If you just added an element to a container
  * you can use insert() to tell the element that it has been inserted into a
  * container.
  *
  * To remove an element from a list, you can use removeNext() to have the element
  * after this to be removed safely. If there is no other element to call removeNext()
  * from, you can use an elements remove() function to tell it that it has been
  * removed from its container. This will set the next pointer to nullptr as well.
  *
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * <B>Notes on multi threaded environments</B>
  *
  * If you plan to use an element in a strictly single-threaded way, you can use
  * the function disable_thread_safety() inherited from VElement to disable the
  * locking mechanism and have the getter and setter methods be less restrictive.
  * You can then use insertNext() / removeNext() without the locking overhead.
  * However, as the locking is enabled by default, it might be more convenient
  * to simply use the next pointers directly.<BR />
  *
  * <I>Critical work flows</I>
  *
  * The following work flows can be troublesome if multiple threads perform
  * concurrent tasks on an element:
  * <TABLE border="1">
  * <TR>
  *   <TH>Task</TH><TH>Problematic action</TH><TH>Solution</TH>
  * </TR><TR>
  *   <TD>Retrieve next element</TD><TD>Remove this element</TD>
  *   <TD>Elements know when they are removed. getNext() then delivers the
  * previously stored pointer, if any.</TD>
  * </TR><TR>
  *   <TD>Retrieve next element</TD><TD>Move element to different container</TD>
  *   <TD>This is not detectable, so never move an element. Remove and copy insert it!</TD>
  * </TR><TR>
  *   <TD>Insert an element after this</TD><TD>Either element destroyed by another thread</TD>
  *   <TD>insertNext() will lock both this and the new next element. Further it checks
  * whether any is destroyed and only inserts the element if both are not marked as destroyed.<BR />
  * If either is marked as destroyed, a pwx::CException is thrown, as those conditions imply
  * serious bugs.</TD>
  * </TR><TR>
  *   <TD>Remove the next element</TD><TD>The next element gets removed or another element is
  * inserted between the two elements by another thread</TD>
  *   <TD>removeNext() will try to lock both elements after one another in a Release->Yield->Lock
  * cycle until both are locked or the next element changes. In the latter event the method does
  * not remove the element, as it is gone.<BR />
  * If the next element goes away before it can be removed, a pwx::CException is thrown.</TD>
  * </TR>
  * </TABLE>
**/
template<typename data_t>
struct PWX_API TSingleElement : public VElement {

    /* ===============================================
     * === Public types                            ===
     * ===============================================
    */

    typedef VElement                base_t;     //!< Base type of this element
    typedef TSingleElement<data_t>  elem_t;     //!< Type of this element
    typedef std::shared_ptr<data_t> share_t;    //!< data_t wrapped in std::shared_ptr
    typedef std::atomic<elem_t*>    neighbor_t; //!< elem_t* wrapped in std::atomic
    typedef base_t::store_t         store_t;    //!< The element store type to register this element with


    /* ===============================================
     * === Public Constructors and destructors     ===
     * ===============================================
    */

    /** @brief default constructor
      *
      * The default constructor sets both the data pointer and the destroy method.
      *
      * @param[in] data_ A pointer to the data this list element is to hold.
      * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
    **/
    TSingleElement ( data_t* data_, void ( *destroy_ ) ( data_t* data_ ) ) noexcept
        : data ( data_, TVarDeleter<data_t> ( destroy_ ) )
    { }


    /** @brief explicit constructor
      *
      * Delegating ctor that calls the default ctor with destroy_ being the nullptr
      *
      * @param[in] data_ A pointer to the data this list element is to hold.
    **/
    explicit
    TSingleElement ( data_t* data_ ) noexcept
        : elem_t ( data_, nullptr )
    { }


    TSingleElement() PWX_DELETE; // nullptr data is not allowed


    /** @brief copy ctor
      *
      * The copy ctor creates a stand-alone element without neighbors
      * copying the data pointer and destroy method from @a src.
      * As the data is wrapped in a shared_ptr, data will not get
      * deleted unless the last reference is gone.
      *
      * <B>Important</B>: Whether the element does locking or not
      * is *not* copied. It will silently be turned on by default!
      *
      * @param[in] src reference to the element to copy.
    **/
    TSingleElement ( const elem_t& src ) noexcept
        : base_t ( src ),
          data ( src.data )
    { }


    virtual ~TSingleElement() noexcept;


    /* ===============================================
     * === Public methods                          ===
     * ===============================================
    */

    /** @brief compare this element with some data and return -1, 0, +1
      *
      * This is a convenient method that safely compares this element to
      * some data. If this elements data is larger than the other, the
      * method returns 1. If both are equal it returns 0 and 1 if the
      * other data is larger.
      *
      * This element gets locked and checked against destruction and
      * nullptr data.
      *
      * @param[in] other reference to the data to compare with
      * @return +1 one if this data is larger, -1 if the other is larger, 0 if both are equal.
    **/
    int32_t compare( const data_t& other ) const noexcept PWX_WARNUNUSED {
        if ( &other != this->data.get() ) {
            PWX_LOCK_GUARD( this )

            // A: Check destruction status
            if ( this->destroyed() ) return -1;

            // B: check Data status
            data_t* thisData = this->data.get();

            if ( thisData ) {
                if ( isFloatType( data_t ) && areAlmostEqual( *thisData, other ) )
                    return 0;
                return *thisData > other ?  1
                       : other > *thisData ? -1 : 0;
            } else
                return -1;
        } // No else. compare(this->data.get()) always returns 0

        return 0;
    }


    /** @brief compare this element with another and return -1, 0, +1
      *
      * This is a convenient method that safely compares this element to
      * another. If this elements data is larger than the others data,
      * the method returns 1. If both are equal it returns 0 and 1 if
      * the other elements data is larger.
      *
      * Both elements get locked and checked against destruction and
      * nullptr data.
      *
      * @param[in] other pointer to the element to compare with
      * @return +1 one if this data is larger, -1 if the other is larger, 0 if both are equal.
    **/
    int32_t compare( const elem_t* const other ) const noexcept PWX_WARNUNUSED {
        if ( other ) {
            if ( other != this ) {
                PWX_DOUBLE_LOCK_GUARD( this, other )

                // A: Check destruction status
                bool thisDest = this->destroyed();
                bool otheDest = other->destroyed();

                if ( thisDest && otheDest )	return  0;
                if ( thisDest )				return -1;
                if ( otheDest )				return  1;

                // B: check Data status
                data_t* thisData = this->data.get();
                data_t* otheData = other->data.get();

                if ( thisData && otheData ) {
                    if ( isFloatType( data_t ) && areAlmostEqual( *thisData, *otheData ) )
                        return 0;
                    return *thisData > *otheData ?  1
                           : *otheData > *thisData ? -1 : 0;
                }
                if ( thisData )	return  1;
                if ( otheData )	return -1;
            } // No else. compare(this) always returns 0
        } else
            return 1; // The other is nullptr, this is always larger

        return 0;
    }


    /** @brief returns a pointer to the next element or nullptr if there is none.
      *
      * This method uses atomic::load() and is therefore safe to use
      * in a multi-threaded environment.
      *
      * @return the next pointer or nullptr if there is none.
    **/
    elem_t* getNext() const noexcept PWX_WARNUNUSED {
        if ( beThreadSafe() ) {
            elem_t* curNext = next.load( memOrdLoad );
            if ( !curNext && removed() )
                return oldNext.load( memOrdLoad );
            return curNext;
        }
        return next.load( memOrdLoad );
    }


    /** @brief insert an element before another
      *
      * This is a special insertion method that is to be used if
      * this element is to become the new head of a container.
      * In this special case there is no element to use insertNext()
      * from, so this method does the handling.
      *
      * If either this or the @a new_next element is marked as destroyed,
      * a pwx::CException is thrown. Such a condition implies that
      * there is something seriously wrong.
      *
      * if @a new_next is either nullptr or this element, the element
      * will only be marked as inserted.
      *
      * @param[in] new_next target where the next pointer should point at.
      * @param[in] new_store optional pointer to the CThreadElementStore that will handle this element
    **/
    void insertBefore( elem_t* new_next, store_t* new_store ) {
        if ( !new_next || ( new_next == this ) ) {
            insert( new_store );
            return;
        }

        if ( !destroyed() && !new_next->destroyed() ) {
            if ( beThreadSafe() ) {
                // Do locking and double checks if this has to be thread safe
                PWX_DOUBLE_LOCK_GUARD( this, new_next )

                /* Now that we have the double lock, it is crucial to
                 * check again. Otherwise we might just insert a destroyed element.
                */
                if ( destroyed() )
                    PWX_THROW( "Illegal_Insert", "Can't insert a destroyed element",
                               "The element to insert has been destroyed while waiting for the lock!" )

                    if ( new_next->destroyed() )
                        PWX_THROW( "Illegal_Insert", "Destroyed elements can't insert",
                                   "The inserting element has been destroyed while waiting for the lock!" )

                        // Store new next neighbor
                        setNext( new_next );

            } else
                // Otherwise do it directly
                next.store( new_next, memOrdStore );

            // Mark as inserted
            insert( new_store );
        } else if ( destroyed() )
            PWX_THROW( "Illegal_Insert", "Can't insert a destroyed element",
                       "Tried to insert an element that has already been destroyed!" )
            else
                PWX_THROW( "Illegal_Insert", "Destroyed elements can't insert",
                           "Tried to insert an element after an already destroyed element!" )
            }


    /** @brief insert an element after this element.
      *
      * This is an extra method to not only set the next pointer
      * of this element, but the next pointer of the inserted element
      * safely, too, in a multi-threaded environment.
      *
      * If either this or the new element is marked as destroyed,
      * a pwx::CException is thrown. Such a condition implies that
      * there is something seriously wrong.
      *
      * If @a new_next is either this element or nullptr, the method
      * simply does nothing.
      *
      * @param[in] new_next target where the next pointer should point at.
      * @param[in] new_store optional pointer to the CThreadElementStore that will handle this element from now on
    **/
    void insertNext( elem_t* new_next, store_t* new_store ) {
        if ( !new_next || ( new_next == this ) )
            return;

        if ( beThreadSafe() ) {
            // Do locking and double checks if this has to be thread safe
            if ( !destroyed() && !new_next->destroyed() ) {
                PWX_DOUBLE_LOCK_GUARD( this, new_next )

                /* Now that we have the double lock, it is crucial to
                 * check again. Otherwise we might just insert a destroyed element.
                */
                if ( destroyed() )
                    PWX_THROW( "Illegal_Insert", "Destroyed elements can't insert",
                               "The inserting element has been destroyed while waiting for the lock!" )

                    if ( new_next->destroyed() )
                        PWX_THROW( "Illegal_Insert", "Can't insert a destroyed element",
                                   "The element to insert has been destroyed while waiting for the lock!" )

                        // Insert the new element
                        new_next->setNext( this->getNext() );
                new_next->insert( new_store );

                // Store new next neighbor
                setNext( new_next );
            } else if ( destroyed() )
                PWX_THROW( "Illegal_Insert", "Destroyed elements can't insert",
                           "Tried to insert an element after an already destroyed element!" )
                else
                    PWX_THROW( "Illegal_Insert", "Can't insert a destroyed element",
                               "Tried to insert an element that has already been destroyed!" )
                } else {
            // Otherwise do it directly
            new_next->next.store( next.load( memOrdLoad ),memOrdStore );
            new_next->insert( new_store );
            next.store( new_next, memOrdStore );
        }
    }


    /** @brief tell the element that it has been removed.
      *
      * Whenever you remove an element from a container you should
      * call this method to tell it that it has been removed.
      * The next pointer of the element will be set to nullptr
      * by this method.
    **/
    virtual void remove() noexcept {
        if ( beThreadSafe() ) {
            PWX_LOCK_GUARD( this )
            base_t::remove();
            setNext( nullptr );
        } else {
            next.store( nullptr, memOrdStore );
            base_t::remove();
        }
    }


    /** @brief remove the next element from a list.
      *
      * This method removes the successor of this element
      * from a list in a thread safe way.
      *
      * @return the removed element
    **/
    elem_t* removeNext() noexcept {
        elem_t* toRemove = next.load( memOrdLoad );

        // Exit at once if there is no next to remove:
        if ( !toRemove )
            return nullptr;

        // Do an acquiring test before the element is actually locked
        if ( beThreadSafe() ) {
            PWX_DOUBLE_LOCK_GUARD( this, toRemove )

            // Do a reset cycle until toRemove is really this elements next
            bool toRemoveIsNext = toRemove == next.load( memOrdLoad );
            while ( !toRemoveIsNext ) {
                toRemove = next.load( memOrdLoad );
                PWX_DOUBLE_LOCK_GUARD_RESET( this, toRemove )
                toRemoveIsNext = toRemove == next.load( memOrdLoad );
            }

            // Continue if we actually have an element to remove now,
            // and the element is not this, like in ring containers
            if ( toRemove && ( toRemove != this ) )
                this->setNext( toRemove->getNext() );

        } else if ( this != toRemove )
            // Without the thread safety needs, this is a lot simpler:
            next.store( toRemove->next.load( memOrdLoad ), memOrdStore );

        // Remove neighborhood:
        if ( toRemove && ( toRemove != this ) ) {
            toRemove->remove();
            return toRemove;
        }

        return nullptr;
    }


    /** @brief set the next pointer to another element.
      *
      * This method uses atomic::store() and is therefore safe to use
      * in a multi-threaded environment.
      *
      * @param[in] new_next target where the next pointer should point at.
    **/
    void setNext( elem_t* new_next ) noexcept {
        if ( beThreadSafe() ) {
            elem_t* currNext = next.load( memOrdLoad );
            next.store( new_next, memOrdStore );
            if ( currNext )
                oldNext.store( currNext, memOrdStore );
        } else
            next.store( new_next, memOrdStore );
    }


    /* ===============================================
     * === Public operators                        ===
     * ===============================================
    */

    /** @brief assignment operator
      *
      * The assignment operator copies over the element and
      * the destroy method. This element will stay where it
      * is, and not change its position.
      *
      * @param[in] src const reference of the element to copy
      * @return reference to this element
    **/
    elem_t& operator= ( const elem_t& src ) noexcept {
        if ( ( this != &src ) && !destroyed() && !src.destroyed() ) {
            PWX_DOUBLE_LOCK_GUARD( this, &src )
            if ( !destroyed() && !src.destroyed() )
                data = src.data;
            // note: destroy method wrapped in data!
        }
        return *this;
    }


    /** @brief dereferencing an element returns a reference to the stored data
      *
      * If the data pointer is nullptr, a pwx::CException with the name
      * "NullDataException" is thrown.
      *
      * @return a read/write reference to the stored data.
    **/
    data_t& operator*() PWX_WARNUNUSED {
        PWX_LOCK_GUARD( this )
        if ( nullptr == data.get() )
            PWX_THROW ( "NullDataException",
                        "nullptr element data",
                        "The pointer lhs->data to dereference is nullptr." )
            return *data;
    }


    /** @brief dereferencing a constant element returns a constant reference to the stored data
      *
      * If the data pointer is nullptr, a pwx::CException with the name
      * "NullDataException" is thrown.
      *
      * @return a read only reference to the stored data.
    **/
    const data_t& operator*() const PWX_WARNUNUSED {
        PWX_LOCK_GUARD( this )
        if ( nullptr == data.get() )
            PWX_THROW ( "NullDataException",
                        "nullptr element data",
                        "The pointer lhs->data to dereference is nullptr." )
            return *data;
    }


    /** @brief return true if this element has the data @a data
      * @param[in] data_ const reference of the data to check
      * @return true if this element has the same data
    **/
    bool operator==( const data_t& data_ ) const noexcept PWX_WARNUNUSED {
        if ( isFloatType( data_t ) )
            return areAlmostEqual( *data, data_ );
        return *data == data_;
    }


    /** @brief return true if this element has differne data than @a data
      * @param[in] data_ const reference of the data to check
      * @return true if this element has different data
    **/
    bool operator!=( const data_t& data_ ) const noexcept PWX_WARNUNUSED {
        if ( isFloatType( data_t ) )
            return !areAlmostEqual( *data, data_ );
        return !( *data == data_ );
    }


    /* ===============================================
     * === Public members                          ===
     * ===============================================
    */

    share_t    data; 							//!< The data this list element points to, wrapped in a shared_ptr.
    neighbor_t next = ATOMIC_VAR_INIT( nullptr ); //!< The next element in the list or nullptr if this is the tail.


  protected:

    /* ===============================================
     * === Protected members                       ===
     * ===============================================
     */

    using base_t::isDestroyed;


  private:

    /* ===============================================
     * === Private members                         ===
     * ===============================================
     */

    neighbor_t oldNext = ATOMIC_VAR_INIT( nullptr );

}; // struct TSingleElement


/** @brief destructor
  *
  * The destructor invokes a lock on the instance to allow
  * other threads to react before the object itself is gone.
  *
  * Because of the usage of shared_ptr wrapping the data this
  * is only deleted if, and only if, this is the very last
  * element referencing this data.
**/
template<typename data_t>
TSingleElement<data_t>::~TSingleElement() noexcept {
    if ( beThreadSafe() )
        isDestroyed.store( true );

    if ( 1 == data.use_count() ) {
        if ( beThreadSafe() ) {
            // Lock the element before checking again.
            DEBUG_LOCK_STATE( "PWX_LOCK", this, this )
            PWX_LOCK_OBJ( this )
            // So the lock is only generated if there is a possibility
            // that we have to delete data, but another thread might
            // have made a copy in the mean time before "isDestroyed"
            // was finished setting to true.
            if ( 1 == data.use_count() ) {
                PWX_TRY( data.reset() ) // the shared_ptr will delete the data now
                catch( ... ) { }

                // Do a lock cycle, so that threads having had to wait while the data
                // was destroyed have a chance now to react before the object is gone.
                DEBUG_LOCK_STATE( "PWX_UNLOCK", this, this )
                PWX_UNLOCK( this )
                DEBUG_LOCK_STATE( "PWX_LOCK_GUARD", this, this )
                PWX_LOCK_GUARD( this )
            } else {
                DEBUG_LOCK_STATE( "PWX_UNLOCK", this, this )
                PWX_UNLOCK( this )
            }
        } else {
            // No thread safety? Then just do it!
            PWX_TRY( data.reset() )
            catch( ... ) { }
        }
    }
}

} // namespace pwx

#endif // PWX_LIBPWX_TYPES_TSINGLEELEMENT_H_INCLUDED
