#ifndef PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED
#pragma once

/** @file TArgTarget.h
  *
  * @brief Declaration of the TArgTarget class template
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


#include <cstring>

#include "arg_handler/VArgTargetBase.h"
#include "basic/CException.h"
#include "basic/pwx_macros.h"


/// @namespace pwx
namespace pwx {


/** @struct TArgTarget
  * @brief Definition of one command line argument using a templated member
  *
  * This is meant to be used with a target pointer. If a callback function
  * is to be used, use CArgCallback instead.
**/
template<typename T>
class TArgTarget : public VArgTargetBase {

  public:

    /** brief default ctor
      *
      * No parameter check, the caller must ensure consistent
      * values that make the instance usable.
      *
      * @see pwx::CArgHandler::addArg()
      *
      * @param[in] arg_short Short argument like "-a" or "x".
      * @param[in] arg_long Long argument like "--foo" or "-bar".
      * @param[in] arg_type Determines what to do with the target.
      * @param[out] arg_target Pointer to the value to handle.
      * @param[in] arg_desc Help text for this argument.
      * @param[in] param_name Name shown in <> int the help text.
    **/
    explicit TArgTarget( char const* arg_short, char const* arg_long,
                         eArgTargetType arg_type,T* arg_target,
                         char const* arg_desc, char const* param_name )
    noexcept :
        VArgTargetBase( arg_short, arg_long, arg_type, arg_desc, param_name ),
        target( arg_target )
    { /* nothing to do here */ }


    /** brief ATT_SET ctor
      *
      * No parameter check, the caller must ensure consistent
      * values that make the instance usable.
      *
      * This is a constructor variant for ATT_SET targets.
      * Use this constructor if the set target type is
      * STT_IGNORE or STT_ERROR, STT_OVERWRITE is the default.
      *
      * @see pwx::CArgHandler::addArg()
      *
      * @param[in] arg_short Short argument like "-a" or "x".
      * @param[in] arg_long Long argument like "--foo" or "-bar".
      * @param[in] set_type Determines the set target type.
      * @param[out] arg_target Pointer to the value to handle.
      * @param[in] arg_desc Help text for this argument.
      * @param[in] param_name Name shown in <> int the help text.
    **/
    TArgTarget( char const* arg_short, char const* arg_long,
                eArgSetType set_type,T* arg_target,
                char const* arg_desc, char const* param_name )
    noexcept :
        VArgTargetBase( arg_short, arg_long, set_type, arg_desc, param_name ),
        target( arg_target )
    { /* nothing to do here */ }


    /** @brief destructor
      * has nothing to do.
    **/
    virtual ~TArgTarget() noexcept {
        target = nullptr;
    }


    /** @brief process one command line parameter or argument occurrence
      *
      * This method is called whenever the type of an argument needs
      * direct action, or a parameter to that argument is met.
      *
      * @param[in] param The parameter found or nullptr if no parameter is needed.
    **/
    virtual eArgErrorNumber process( char const* param );


  private:

    T* target; //!< The target to handle

    /** @brief par_to_val conversion chain prototype
      *
      * All supported types have specializations.
      *
      * @param[out] tgt target to create
      * @param[in] param sring to interpret
    **/
    void par_to_val( T* tgt, char const* param ) noexcept;
};


// === Processing specializations prototypes ===

/// @brief Specialization for std::string targets
template<>
eArgErrorNumber TArgTarget<std::string>::process( char const* param );


/// @brief Generic non-specialized handler
template<typename T>
eArgErrorNumber TArgTarget<T>::process( char const* param ) {
    eArgErrorNumber argErrno = AEN_OK;

    T val = ( T )0;
    if ( param && strlen( param ) )
        this->par_to_val( &val, param );

    switch( this->type ) {
        case ATT_FALSE:
            *target = ( T )false;
            break;
        case ATT_TRUE:
            *target = ( T )true;
            break;
        case ATT_INC:
            *target += ( T )1;
            break;
        case ATT_DEC:
            *target -= ( T )1;
            break;
        case ATT_ADD:
            *target  += val;
            break;
        case ATT_SUB:
            *target  -= val;
            break;
        case ATT_SET:
            // This needs handling for all three set types:
            if ( ( STT_OVERWRITE == this->setType )
                    || ( !this->gotParameter ) )
                *target = val;
            else if ( STT_ERROR == this->setType )
                argErrno = AEN_MULTIPLE_SET_PARAM;
            // Last possibility is STT_IGNORE, which is, well, ignored. ;)
            break;
        case ATT_CB:
            PWX_THROW( "UnhandledTargetType", "ATT_CB not supported, use CArgCallback instead!", "" )
            break;
        default:
            PWX_THROW( "UnhandledTargetType", "The given target type is not implemented, yet!", "" )
    }

    // if a parameter was given, it must be noted now:
    if ( param && strlen( param ) )
        this->gotParameter = true;

    return argErrno;
}


// === par_to_val specializations prototypes ===

/// @brief Specialization for bool targets
template<>
void TArgTarget<bool>::par_to_val( bool* tgt, char const* param ) noexcept;

/// @brief Specialization for int8_t targets
template<>
void TArgTarget<int8_t>::par_to_val( int8_t* tgt, char const* param ) noexcept;

/// @brief Specialization for int16_t targets
template<>
void TArgTarget<int16_t>::par_to_val( int16_t* tgt, char const* param ) noexcept;

/// @brief Specialization for int32_t targets
template<>
void TArgTarget<int32_t>::par_to_val( int32_t* tgt, char const* param ) noexcept;

/// @brief Specialization for int64_t targets
template<>
void TArgTarget<int64_t>::par_to_val( int64_t* tgt, char const* param ) noexcept;

/// @brief Specialization for uint8_t targets
template<>
void TArgTarget<uint8_t>::par_to_val( uint8_t* tgt, char const* param ) noexcept;

/// @brief Specialization for uint16_t targets
template<>
void TArgTarget<uint16_t>::par_to_val( uint16_t* tgt, char const* param ) noexcept;

/// @brief Specialization for uint32_t targets
template<>
void TArgTarget<uint32_t>::par_to_val( uint32_t* tgt, char const* param ) noexcept;

/// @brief Specialization for uint64_t targets
template<>
void TArgTarget<uint64_t>::par_to_val( uint64_t* tgt, char const* param ) noexcept;

/// @brief Specialization for float targets
template<>
void TArgTarget<float>::par_to_val( float* tgt, char const* param ) noexcept;

/// @brief Specialization for double targets
template<>
void TArgTarget<double>::par_to_val( double* tgt, char const* param ) noexcept;

/// @brief Specialization for long double targets
template<>
void TArgTarget<long double>::par_to_val( long double* tgt, char const* param ) noexcept;

} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED

