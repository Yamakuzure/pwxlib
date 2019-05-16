#ifndef PWX_LIBPWX_PWX_CONTAINER_TCHAINHASH_H_INCLUDED
#define PWX_LIBPWX_PWX_CONTAINER_TCHAINHASH_H_INCLUDED

/** @file TChainHash.h
  *
  * @brief Declaration of a basic template for chained hash containers.
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


#include "container/VTHashBase.h"


namespace pwx {


/** @class TChainHash PChainHash <PChainHash>
  *
  * @brief Chained hash container for variable types
  *
  * @todo describe properly
**/
template<typename key_t, typename data_t, typename elem_t = THashElement<key_t, data_t> >
class PWX_API TChainHash : public VTHashBase<key_t, data_t, elem_t> {
  public:
    /* ===============================================
     * === Public types                            ===
     * ===============================================
    */

    typedef VTHashBase<key_t, data_t, elem_t> base_t;     //!< Base type of the hash
    typedef TChainHash<key_t, data_t, elem_t> hash_t;     //!< Type of this hash


    /* ===============================================
     * === Public Constructors and destructors     ===
     * ===============================================
    */

    /** @brief default constructor
      *
      * The default destructor takes a length for the initial size and
      * a length for the key. Further it initializes the hash table.
      *
      * The key length is only needed if you use C-String keys without 0-byte delimiter
      * or if you are using C-Strings or std::string keys that can be so long that you
      * want to limit the length of the key itself and ignore further characters.
      *
      * To set any of the user methods, one of the specialized constructors
      * can be used.
      *
      * The @a maxLoad_ and @a dynGrow_ parameters can be used to tell the hash table
      * how much it should grow when the the specified load factor is reached.
      * The default for chained hash tables is to grow by a factor of 1.25 (25%) when
      * a load factor of 3.0 is reached.
      *
      * @param[in] initSize Initial size of the hash table.
      * @param[in] keyLen_ Length of the key to limit hash generation.
      * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
      * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
    **/
    TChainHash(	uint32_t initSize, uint32_t keyLen_,
                double maxLoad_, double dynGrow_ ) noexcept:
        base_t( initSize, keyLen_, maxLoad_, dynGrow_ )
    { }


    /** @brief full constructor with key length
      *
      * The full constructor initializes an empty hash with user defined delete
      * method, hashing method and key length. The initial size is the @a initSize
      *
      * The @a maxLoad_ and @a dynGrow_ parameters can be used to tell the hash table
      * how much it should grow when the the specified load factor is reached.
      * The default for chained hash tables is to grow by a factor of 1.25 (25%) when
      * a load factor of 3.0 is reached.
      *
      * @param[in] initSize The initial size of the table.
      * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
      * @param[in] hash_ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
      * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
      * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
      * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
    **/
    TChainHash(	uint32_t initSize,
                void ( *destroy_ ) ( data_t* data ),
                uint32_t ( *hash_ ) ( const key_t* key, uint32_t keyLen ),
                uint32_t keyLen_,
                double maxLoad_, double dynGrow_ ) noexcept :
        base_t( initSize, destroy_, hash_, keyLen_, maxLoad_, dynGrow_ )
    { }


    /** @brief full constructor without key length
      *
      * The full constructor initializes an empty hash with user defined delete
      * method and hashing method withour key length. The initial size is the
      * @a initSize
      *
      * The @a maxLoad_ and @a dynGrow_ parameters can be used to tell the hash table
      * how much it should grow when the the specified load factor is reached.
      * The default for chained hash tables is to grow by a factor of 1.25 (25%) when
      * a load factor of 3.0 is reached.
      *
      * @param[in] initSize The initial size of the table.
      * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
      * @param[in] hash_ A pointer to a function that can hash the keys that are stored
      * @param[in] maxLoad_ maximum load factor that triggers automatic growth.
      * @param[in] dynGrow_ growth rate applied when the maximum load factor is reached.
    **/
    TChainHash(	uint32_t initSize,
                void ( *destroy_ ) ( data_t* data ),
                uint32_t ( *hash_ ) ( const key_t* key ),
                double maxLoad_, double dynGrow_ ) noexcept :
        base_t( initSize, destroy_, hash_, maxLoad_, dynGrow_ )
    { }


    /** @brief limiting user method constructor
      *
      * This constructor only takes a destroy method and a hash
      * method with explicit key length.
      *
      * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
      * @param[in] hash_ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
      * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
    **/
    TChainHash(	void ( *destroy_ ) ( data_t* data ),
                uint32_t ( *hash_ ) ( const key_t* key, uint32_t keyLen ),
                uint32_t keyLen_ ) noexcept :
        base_t( 97, destroy_, hash_, keyLen_, 3.0, 1.25 )
    { }


    /** @brief user method constructor
      *
      * This constructor only takes a destroy method and a hash
      * method without explicit key length.
      *
      * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
      * @param[in] hash_ A pointer to a function that can hash the keys that are stored and takes an optional keyLen
    **/
    TChainHash(	void ( *destroy_ ) ( data_t* data ),
                uint32_t ( *hash_ ) ( const key_t* key ) ) noexcept :
        base_t( 97, destroy_, hash_, 3.0, 1.25 )
    { }


    /** @brief destroy method constructor
      *
      * This constructor only takes a destroy method.
      *
      * @param[in] destroy_ A pointer to a function that is to be used to destroy the data
    **/
    TChainHash(	void ( *destroy_ ) ( data_t* data ) ) noexcept :
        base_t( 97, destroy_, nullptr, 3.0, 1.25 )
    { }


    /** @brief key length constructor
      *
      * This constructor only takes a key length and sets the destroy
      * and hash methods to nullptr.
      *
      * @param[in] keyLen_ optional limiting key length for C-Strings and std::string keys
    **/
    TChainHash(	uint32_t keyLen_ ) noexcept :
        base_t ( 97, keyLen_, 3.0, 1.25 )
    { }


    /** @brief empty constructor
      *
      * The empty constructor uses the default constructor to set the data
      * destroy method and the hash method to the null pointer with
      * full key usage
    **/
    TChainHash() noexcept :
        base_t ( 97, 0, 3.0, 1.25 )
    { }


    /** @brief copy constructor
      *
      * Builds a copy of all elements of @a src.
      *
      * If a new element can not be created, a pwx::CException with
      * the name "ElementCreationFailed" is thrown.
      *
      * @param[in] src reference of the hash to copy.
    **/
    TChainHash(	const hash_t& src ) :
        base_t ( src ) {
        operator+=( src );
    }


    virtual ~TChainHash() noexcept;


    /* ===============================================
     * === Public methods                          ===
     * ===============================================
    */

    using base_t::add;
    using base_t::clear;
    using base_t::disable_thread_safety;
    using base_t::empty;
    using base_t::exists;
    using base_t::enable_thread_safety;
    using base_t::get;
    using base_t::getData;
    using base_t::getHops;
    using base_t::grow;
    using base_t::pop;
    using base_t::pop_back;
    using base_t::pop_front;
    using base_t::push;
    using base_t::push_back;
    using base_t::push_front;
    using base_t::shift;
    using base_t::size;
    using base_t::sizeMax;
    using base_t::unshift;


    /* ===============================================
     * === Public operators                        ===
     * ===============================================
    */

    using base_t::operator=;
    using base_t::operator+=;
    using base_t::operator-=;
    using base_t::operator[];


  protected:

    /* ===============================================
     * === Protected methods                       ===
     * ===============================================
    */

    /* Important:
     * All protected and private methods must not lock the container!
     * Container locking is done by the public methods!
     * Further all protected and private
     * methods must not use public methods, or they might end up
     * freezing while waiting for themselves to end!
     */

    using base_t::protGetHash;


    /* ===============================================
     * === Protected members                       ===
     * ===============================================
    */

    using base_t::CHMethod;
    using base_t::eCount;
    using base_t::hashSize;
    using base_t::hashTable;
    using base_t::memOrdLoad;
    using base_t::memOrdStore;


  private:

    /* ===============================================
     * === Private methods                         ===
     * ===============================================
    */

    /* Important:
     * All protected and private methods must not lock the container!
     * Container locking is done by the public methods!
     * Further all protected and private
     * methods must not use public methods, or they might end up
     * freezing while waiting for themselves to end!
     */

    /** @brief get the index of a key
      *
      * In a chained hash the index is calculated directly using either
      * the division method or the multiplication method.
      * Which method to use is determined when setting the size of the
      * hash table upon creation and whenever the table is enlarged.
      *
      * This method just returns the index and does not check whether
      * the wanted key actually exists in the hash table.
      *
      * @param[in] key const reference of the key to evaluate
      * @return the index an element with this key would have in the table
    **/
    virtual uint32_t privGetIndex( const key_t& key ) const noexcept {
        uint32_t xHash = this->protGetHash( &key );
        uint32_t tabSize = hashSize.load( memOrdLoad );

        if ( CHM_Division == CHMethod )
            return xHash % tabSize;
        else {
            double dHash = static_cast<double>( xHash ) * 0.618;
            return static_cast<uint32_t>( std::floor( ( dHash - std::floor( dHash ) ) * tabSize ) );
        }
    }


    /// @brief private insertion doing bucket filling to resolve collisions
    virtual uint32_t privInsert( elem_t* elem ) {
        uint32_t idx  = privGetIndex( elem->key );
        elem_t*  root = this->hashTable[idx];
        if ( root ) {
            elem_t* next = root->getNext();
            elem->hops   = 1;
            while ( next ) {
                root = next;
                next = root->getNext();
                ++elem->hops;
            }
            root->insertNext( elem );
        } else {
            this->hashTable[idx] = elem;
            elem->insertAsFirst();
        }

        eCount.fetch_add( 1, memOrdStore );
        return this->eCount.load( memOrdLoad );
    }


    /** @brief removes the element with the specified index and returns it.
      *
      * Always the first element is removed, the next will become "root"
      * or the place is set to nullptr if this is the last.
      *
      * If the index is out of bounds, nullptr is returned.
      *
      * @param[in] index the index to remove
      * @return a pointer to the removed element or nullptr if no such element exists
    **/
    virtual elem_t* privRemoveIdx ( uint32_t index ) noexcept {
        elem_t* result = index < this->hashSize.load( memOrdLoad )
                         ? hashTable[index]
                         : nullptr;

        if ( result ) {
            // Note: Chained Hashes do not use "vacated" sentries, no check needed.
            elem_t* xNext = result->getNext();
            hashTable[index] = xNext != result ? xNext : nullptr;
            result->remove();
            eCount.fetch_sub( 1, memOrdStore );
        } // End of outer check

        return result;
    }


    /** @brief removes the element with the specified key and returns it.
      *
      * @param[in] key the key of the element to remove has
      * @return a pointer to the removed element or nullptr if no such element exists
    **/
    virtual elem_t* privRemoveKey ( const key_t& key ) noexcept {
        uint32_t index   = privGetIndex( key );
        elem_t* result = hashTable[index];
        elem_t* prev   = nullptr;

        while ( result && ( *result != key ) && ( result != prev ) ) {
            prev   = result;
            result = result->getNext();
        }

        // Now, if result is set, there is either a prev or
        // result is the root of the bucket:
        if ( result && ( *result == key ) && result->inserted() ) {
            if ( prev )
                prev->removeNext();
            else {
                elem_t* xNext = result->getNext();
                hashTable[index] = xNext != result ? xNext : nullptr;
                result->remove();
            }
            eCount.fetch_sub( 1, memOrdStore );
            return result;
        }

        return nullptr;
    }


}; // class TChainHash


/** @brief default destructor
  *
  * This destructor will delete all elements currently stored. There is no
  * need to clean up manually before deleting the hash.
**/
template<typename key_t, typename data_t, typename elem_t>
TChainHash<key_t, data_t, elem_t>::~TChainHash() noexcept
{ /* all done in base_t dtor */ }


/** @brief addition operator
  *
  * Create and return a new hash consisting of all elements of @a lhs and @a rhs.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local hash is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: All elements
  * will be copied twice!
  * Only use this operator if you really think you need a construct like
  * 'hashC = hashA + hashB'.
  *
  * @param[in] lhs left hand side reference
  * @param[in] rhs right hand side reference
  * @return the new hash.
**/
template<typename data_t, typename elem_t>
TChainHash<data_t, elem_t> operator+ ( const TChainHash<data_t, elem_t>& lhs, const TChainHash<data_t, elem_t>& rhs ) {
    TChainHash<data_t, elem_t> result( lhs );

    if ( &lhs != &rhs )
        PWX_TRY_PWX_FURTHER( result += rhs )

        return result;
}


/** @brief substraction operator
  *
  * Remove all elements from @a rhs from this hash.
  *
  * The destroy method from @a lhs will be used in the result as well.
  *
  * Warning: For this to work a local hash is created on the stack. Giving it back
  * means that it will be copied onto your result. In other words: The difference
  * elements are copied twice!
  *
  * @param[in] lhs reference of the hash from which to substract.
  * @param[in] rhs reference of the hash to substract.
  * @return reference to this.
**/
template<typename data_t, typename elem_t>
TChainHash<data_t, elem_t> operator- ( const TChainHash<data_t, elem_t>& lhs, const TChainHash<data_t, elem_t>& rhs ) {
    TChainHash<data_t, elem_t> result( lhs );

    if ( &lhs != &rhs ) {
        PWX_TRY_PWX_FURTHER( result -= rhs )
    } else
        result.clear();

    return result;
}

} // namespace pwx


#endif // PWX_LIBPWX_PWX_CONTAINER_TCHAINHASH_H_INCLUDED

