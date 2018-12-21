#pragma once
#ifndef PWX_PWXLIB_PWX_CONTAINER_TSINGLERING_H_INCLUDED
#define PWX_PWXLIB_PWX_CONTAINER_TSINGLERING_H_INCLUDED 1

/** @file pwxTSingleRing.h
  *
  * @brief Declaration of a basic template for singly linked rings
  *
  * (c) 2007 - 2018 PrydeWorX
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


#include "pwxTSingleList.h"


namespace pwx {


/** @class TSingleRing
  *
  * @brief Template to build singly linked rings of variable types
  *
  * The singly linked ring is a singly linked list with a tail having a next
  * pointer to head instead of being a nullptr.
  *
  * The constructor takes an optional destroy(T*) function pointer that is used
  * to destroy the data when the element is deleted. If no such function was set,
  * the standard delete operator is used instead.
  *
  * It is recommended that you use the much more advanced std::list unless you
  * need to store a very large number of elements and can not live with the
  * downside of every element having to be copied into the std::list.
  *
  * @see pwx::TSingleList for further information.
**/
template<typename data_t, typename elem_t = TSingleElement<data_t> >
class PWX_API TSingleRing : public TSingleList<data_t, elem_t> {
  public:
    /* ===============================================
     * === Public types                            ===
     * ===============================================
    */

    typedef TSingleList<data_t, elem_t> base_t; //!< Base type of the ring
    typedef TSingleRing<data_t, elem_t> list_t; //!< Type of this ring


    /* ===============================================
     * === Public Constructors and destructors     ===
     * ===============================================
    */

    /** @brief default constructor
      *
      * The default constructor initializes an empty ring.
      *
      * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
    **/
    TSingleRing ( void ( *destroy_ ) ( data_t* data ) ) noexcept :
        base_t ( destroy_ )
    { }


    /** @brief empty constructor
      *
      * The empty constructor uses the base constructor to set the data
      * destroy method to the null pointer.
    **/
    TSingleRing() noexcept :
        base_t ( nullptr )
    { }


    /** @brief copy constructor
      *
      * Builds a copy of all elements of @a src.
      *
      * @param[in] src reference of the ring to copy.
    **/
    TSingleRing ( const list_t& src ) noexcept :
        base_t ( src ) {
        // TSingleList copies the elements
        privConnectEnds(); // All we have to do here!
    }


    virtual ~TSingleRing() noexcept;


    /* ===============================================
     * === Public methods                          ===
     * ===============================================
    */


    using base_t::clear;


    /** @brief delete the element after the element holding the specified data
      *
      * This method deletes the element in the ring after the element
      * that holds @a prev.
      *
      * If @a prev is set to nullptr, the root element (aka head) is
      * deleted.
      *
      * If you intent to work with the element, use remNext instead.
      *
      * If there is no element behind the element holding @a prev a
      * pwx::CException with the name "OutOfRange" is thrown.
      *
      * @param[in] prev the data the element that precedes the element to delete holds
      * @return the number of elements remaining in the ring after the deletion.
    **/
    virtual uint32_t delNext ( data_t* prev ) {
        PWX_TRY_PWX_FURTHER ( base_t::delNext ( prev ) )
        return privConnectEnds();
    }


    /** @brief delete the element after the specified element
      *
      * This method deletes the element in the ring after the element
      * @a prev.
      *
      * If @a prev is set to nullptr, the root element (aka head) is
      * deleted.
      *
      * If you intent to work with the element, use remNextElem instead.
      *
      * If @a prev is no element of this list, the wrong ring is updated
      * and both element counts will be wrong then. So please make sure to
      * use the correct element on the correct list!
      *
      * If there is no element behind the element @a prev a
      * pwx::CException with the name "OutOfRange" is thrown.
      *
      * @param[in] prev the element that precedes the element to delete
      * @return the number of elements remaining in the ring after the deletion.
    **/
    virtual uint32_t delNextElem ( elem_t* prev ) {
        PWX_TRY_PWX_FURTHER ( base_t::delNextElem ( prev ) )
        return privConnectEnds();
    }


    using base_t::empty;
    using base_t::find;
    using base_t::get;
    using base_t::getData;


    /** @brief insert a new data pointer after the specified data
      *
      * This method inserts a new element in the ring after the element
      * holding @a prev.
      *
      * If @a prev is set to nullptr, the new element will become the new
      * head of the list.
      *
      * If the new element can not be created, a pwx::CException with
      * the name "ElementCreationFailed" is thrown.
      *
      * @param[in] prev the data the element that should precede the new element holds
      * @param[in] data the pointer that is to be added.
      * @return the number of elements in this ring after the insertion
    **/
    virtual uint32_t insNext ( data_t* prev, data_t* data ) {
        PWX_TRY_PWX_FURTHER ( base_t::insNext ( prev, data ) )
        return privConnectEnds();
    }


    /** @brief insert an element copy after the specified data
      *
      * This method inserts a new element in the ring after the element
      * holding @a prev that is a copy of @a src.
      *
      * If @a prev is set to nullptr, the new element will become the new
      * head of the list.
      *
      * If the new element can not be created, a pwx::CException with
      * the name "ElementCreationFailed" is thrown.
      *
      * @param[in] prev the data the element that should precede the new element holds.
      * @param[in] src reference to the element to copy.
      * @return the number of elements in this ring after the insertion.
    **/
    virtual uint32_t insNext ( data_t* prev, const elem_t& src ) {
        PWX_TRY_PWX_FURTHER ( base_t::insNext ( prev, src ) )
        return privConnectEnds();
    }


    /** @brief insert a new data pointer after the specified element
      *
      * This method inserts a new element in the ring after the element
      * @a prev.
      *
      * If @a prev is set to nullptr, the new element will become the new
      * head of the list.
      *
      * If @a prev is no element of this list, the wrong ring is updated
      * and both element counts will be wrong then. So please make sure to
      * use the correct element on the correct list!
      *
      * If the new element can not be created, a pwx::CException with
      * the name "ElementCreationFailed" is thrown.
      *
      * @param[in] prev the element that should precede the new element
      * @param[in] data the pointer that is to be added.
      * @return the number of elements in this ring after the insertion
    **/
    virtual uint32_t insNextElem ( elem_t* prev, data_t* data ) {
        PWX_TRY_PWX_FURTHER ( base_t::insNextElem ( prev, data ) )
        return privConnectEnds();
    }


    /** @brief insert an element copy after the specified element
      *
      * This method inserts a new element in the ring after the element
      * @a prev that is a copy of @a src.
      *
      * If @a prev is set to nullptr, the new element will become the new
      * head of the list.
      *
      * If @a prev is no element of this list, the wrong ring is updated
      * and both element counts will be wrong then. So please make sure to
      * use the correct element on the correct list!
      *
      * If the new element can not be created, a pwx::CException with
      * the name "ElementCreationFailed" is thrown.
      *
      * @param[in] prev the element that should precede the new element.
      * @param[in] src reference of the element to copy.
      * @return the number of elements in this ring after the insertion.
    **/
    virtual uint32_t insNextElem ( elem_t* prev, const elem_t& src ) {
        PWX_TRY_PWX_FURTHER ( base_t::insNextElem ( prev, src ) )
        return privConnectEnds();
    }


    /** @brief short alias for pop_front()
      *
      * You have to delete the removed element by yourself. If you do not intent
      * to work with the removed element, use delNext instead.
      *
      * If the ring is empty, nullptr is returned.
      *
      * @return a pointer to the removed element or nullptr if the ring is empty
    **/
    virtual elem_t* pop() noexcept {
        elem_t* toRemove = base_t::pop_front();
        if ( toRemove )
            privConnectEnds();
        return toRemove;
    }


    /** @brief alias to remove the last element (tail)
      *
      * You have to delete the removed element by yourself. If you do not intent
      * to work with the removed element, use delNext instead.
      *
      * If the ring is empty, nullptr is returned.
      *
      * @return a pointer to the removed element or nullptr if the ring is empty
    **/
    virtual elem_t* pop_back() noexcept {
        elem_t* toRemove = base_t::pop_back();
        if ( toRemove )
            privConnectEnds();
        return toRemove;
    }


    /** @brief alias to remove the first element (head)
      *
      * You have to delete the removed element by yourself. If you do not intent
      * to work with the removed element, use delNext instead.
      *
      * If the ring is empty, nullptr is returned.
      *
      * @return a pointer to the removed element or nullptr if the ring is empty
    **/
    virtual elem_t* pop_front() noexcept {
        elem_t* toRemove = base_t::pop_front();
        if ( toRemove )
            privConnectEnds();
        return toRemove;
    }


    /** @brief short alias for push_back(data_t *data)
      *
      * If the new element can not be created, a pwx::CException with
      * the name "ElementCreationFailed" is thrown.
      *
      * @param[in] data the pointer that is to be added.
      * @return the number of elements in this ring after the insertion
    **/
    virtual uint32_t push ( data_t* data ) {
        PWX_TRY_PWX_FURTHER ( base_t::push_back ( data ) )
        return privConnectEnds();
    }


    /** @brief short alias for push_back(const elem_t &src)
      *
      * If the new element can not be created, a pwx::CException with
      * the name "ElementCreationFailed" is thrown.
      *
      * @param[in] src reference to the element to copy
      * @return the number of elements in this ring after the insertion
    **/
    virtual uint32_t push ( const elem_t& src ) {
        PWX_TRY_PWX_FURTHER ( base_t::push_back ( src ) )
        return privConnectEnds();
    }


    /** @brief alias to add a data pointer to the end of the ring.
      *
      * If the new element can not be created, a pwx::CException with
      * the name "ElementCreationFailed" is thrown.
      *
      * @param[in] data the pointer that is to be added.
      * @return the number of elements in this ring after the insertion
    **/
    virtual uint32_t push_back ( data_t* data ) {
        PWX_TRY_PWX_FURTHER ( base_t::push_back ( data ) )
        return privConnectEnds();
    }


    /** @brief alias to add an element copy to the end of the ring.
      *
      * If the new element can not be created, a pwx::CException with
      * the name "ElementCreationFailed" is thrown.
      *
      * @param[in] src reference to the element to copy
      * @return the number of elements in this ring after the insertion
    **/
    virtual uint32_t push_back ( const elem_t& src ) {
        PWX_TRY_PWX_FURTHER ( base_t::push_back ( src ) )
        return privConnectEnds();
    }


    /** @brief alias to add a data pointer to the head of the ring.
      *
      * If the new element can not be created, a pwx::CException with
      * the name "ElementCreationFailed" is thrown.
      *
      * @param[in] data the pointer that is to be added.
      * @return the number of elements in this ring after the insertion
    **/
    virtual uint32_t push_front ( data_t* data ) {
        PWX_TRY_PWX_FURTHER ( base_t::insNext ( nullptr, data ) )
        return privConnectEnds();
    }


    /** @brief alias to add an element copy to the head of the ring.
      *
      * If the new element can not be created, a pwx::CException with
      * the name "ElementCreationFailed" is thrown.
      *
      * @param[in] src reference to the element to copy
      * @return the number of elements in this ring after the insertion
    **/
    virtual uint32_t push_front ( const elem_t& src ) {
        PWX_TRY_PWX_FURTHER ( base_t::insNext ( nullptr, src ) )
        return privConnectEnds();
    }


    /** @brief remove the element after the element holding the specified data
      *
      * This method removes the element in the ring after the element
      * that holds @a prev and returns a pointer to the removed element.
      *
      *
      * If @a prev is set to nullptr, the root element (aka head) is
      * removed.
      *
      * You have to delete the removed element by yourself. If you do not intent
      * to work with the removed element, use delNext instead.
      *
      * If there is no element behind the element @a prev a nullptr is returned.
      *
      * @param[in] prev the data the element that precedes the element to remove holds
      * @return a pointer to the removed element or nullptr if there is none
    **/
    virtual elem_t* remNext ( data_t* prev ) noexcept {
        elem_t* toRemove = base_t::remNext ( prev );
        if ( toRemove )
            privConnectEnds();
        return toRemove;
    }


    /** @brief remove the element after the specified element
      *
      * This method removes the element in the ring after the element
      * @a prev.
      *
      * If @a prev is set to nullptr, the root element (aka head) is
      * removed.
      *
      * If @a prev is no element of this list, the wrong ring is updated
      * and both element counts will be wrong then. So please make sure to
      * use the correct element on the correct list!
      *
      * If there is no element behind the element @a prev or if the ring is
      * empty, nullptr is returned.
      *
      * @param[in] prev the element that precedes the element to remove
      * @return a pointer to the removed element or nullptr if there is none
    **/
    virtual elem_t* remNextElem ( elem_t* prev ) noexcept {
        elem_t* toRemove = base_t::remNextElem ( prev );
        if ( toRemove )
            privConnectEnds();
        return toRemove;
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
        if ( &rhs != this ) {
            PWX_TRY_PWX_FURTHER ( base_t::operator= ( rhs ) )
            privConnectEnds();
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
    virtual list_t& operator+= ( const list_t& rhs ) {
        if ( &rhs != this ) {
            PWX_TRY_PWX_FURTHER ( base_t::operator+= ( rhs ) )
            privConnectEnds();
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
    virtual list_t& operator-= ( const list_t& rhs ) {
        if ( &rhs != this ) {
            PWX_TRY_PWX_FURTHER ( base_t::operator-= ( rhs ) )
            privConnectEnds();
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

    using base_t::head;
    using base_t::tail;


    /* ===============================================
     * === Protected members                       ===
     * ===============================================
    */

    using base_t::eCount;
    using base_t::memOrdLoad;
    using base_t::memOrdStore;

  private:
    /* ===============================================
     * === Private methods                         ===
     * ===============================================
    */

    /// @brief simple private method to make sure the ring is closed
    virtual uint32_t privConnectEnds() noexcept {
        // Return early if nothing is to be done:
        if ( !head() || !tail() || ( head() == tail()->getNext() ) )
            return eCount.load( memOrdLoad );

        if ( this->beThreadSafe() ) {
            // In this case we do a lock cycle until a valid tail is
            // found or the ring is empty
            PWX_LOCK_OBJ( this )
            while ( tail() && tail()->destroyed() ) {
                PWX_UNLOCK( this )
                PWX_LOCK_OBJ( this )
            }
            // Now tail is either nullptr (ring is empty) or valid and locked.
            if ( tail() && ( head() != tail()->getNext() ) )
                tail()->setNext( head() );
            PWX_UNLOCK( this )
        } // End of thread safe connection
        else
            tail()->next.store( head(), memOrdStore );

        return eCount.load( memOrdLoad );
    }
}; // class TSingleRing


/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the list.
**/
template<typename data_t, typename elem_t>
TSingleRing<data_t, elem_t>::~TSingleRing() noexcept
{ }


/** @brief addition operator
  *
  * Create and return a new ring consisting of all elements of @a lhs and @a rhs.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local ring is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: All elements
  * will be copied twice!
  *
  * @param[in] lhs left hand side reference
  * @param[in] rhs right hand side reference
  * @return the new ring.
**/
template<typename data_t, typename elem_t>
TSingleRing<data_t, elem_t> operator+ ( const TSingleRing<data_t, elem_t>& lhs, const TSingleRing<data_t, elem_t>& rhs ) {
    TSingleRing<data_t, elem_t> result ( lhs );

    if ( &lhs != &rhs ) {
        PWX_TRY_PWX_FURTHER ( result += rhs )
    }

    return result;
}


/** @brief substraction operator
  *
  * Remove all elements from @a rhs from this ring.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local ring is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: The difference
  * elements are copied twice!
  *
  * @param[in] lhs reference of the ring from which to substract.
  * @param[in] rhs reference of the ring to substract.
  * @return reference to this.
**/
template<typename data_t, typename elem_t>
TSingleRing<data_t, elem_t> operator- ( const TSingleRing<data_t, elem_t>& lhs, const TSingleRing<data_t, elem_t>& rhs ) {
    TSingleRing<data_t, elem_t> result ( lhs );

    if ( &lhs != &rhs ) {
        PWX_TRY_PWX_FURTHER ( result -= rhs )
    } else
        result.clear();

    return result;
}

} // namespace pwx
#endif // PWX_PWXLIB_PWX_CONTAINER_TSINGLERING_H_INCLUDED
