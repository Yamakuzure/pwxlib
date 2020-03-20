#ifndef PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED 1
#pragma once

/** @file TArgTarget.h
  *
  * @brief Declaration of the TArgTarget class template
  *
  * (c) 2007 - 2020 PrydeWorX
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


#include <cstddef>
#include <cstdint>
#include <cstring>

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "arg_handler/eArgErrorNumber.h"
#include "arg_handler/VArgTargetBase.h"
#include "basic/CException.h"


/// @namespace pwx
namespace pwx {


/** @struct TArgTarget
  * @brief Definition of one command line argument using a templated member
  *
  * You can use a target, a callback function, or both.
**/
template<typename T>
class PWX_API TArgTarget : public VArgTargetBase {

public:

	/** @brief default ctor
	  *
	  * No parameter check, the caller must ensure consistent
	  * values that make the instance usable.
	  *
	  * If neither @a short_ nor @a long_ are given, the argument is considered to be positional.
	  * If @a cb_ is set, @a name_ determines whether a parameter is expected or not.
	  *
	  * If you give both @a target_ and @a cb_, the target is first handled, and the
	  * callback function is called next.
	  *
	  * @param[in] short_  Short argument like "-a" or "x".       (optional)
	  * @param[in] long_   Long argument like "--foo" or "-bar".  (optional)
	  * @param[in] type_   Determines what to do with the target.
	  * @param[in] set_    Rules for zero to multiple occurances.
	  * @param[in] target_ Target value to handle.                (optional)
	  * @param[in] cb_     Callback function to call when the argument is processed (optional)
	  * @param[in] desc_   Help text for this argument.           (optional)
	  * @param[in] name_   Name shown in <> int the help text.    (optional)
	**/
	explicit TArgTarget( char const* short_, char const* long_,
	                     eArgTargetType type_, eArgType set_,
	                     T* target_, arg_cb_t cb_,
	                     char const* desc_, char const* name_ ) noexcept
		: VArgTargetBase( short_, long_, type_, set_, cb_, desc_, name_ )
		, target( target_ )
	{ /* nothing to do here */ }


	/// @brief Copy ctor
	TArgTarget( TArgTarget<T> const& rhs ) noexcept
		: VArgTargetBase( rhs )
		, target( rhs.target )
	{ }


	// No empty ctor
	TArgTarget() PWX_DELETE;


	/// @brief Move ctor
	TArgTarget( TArgTarget<T>&& rhs ) noexcept
		: VArgTargetBase( rhs )
		, target( rhs.target ) {
		rhs.target = nullptr;
	}


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


	/// @brief Copying assignment operator
	TArgTarget& operator=( TArgTarget const& rhs ) noexcept {
		if ( &rhs != this ) {
			VArgTargetBase::operator=( rhs );
			target = rhs.target;
		}
		return *this;
	}


	/// @brief Moving assignment operator
	TArgTarget& operator=( TArgTarget&& rhs ) noexcept {
		if ( &rhs != this ) {
			VArgTargetBase::operator=( rhs );
			target     = rhs.target;
			rhs.target = nullptr;
		}
		return *this;
	}


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
eArgErrorNumber PWX_API TArgTarget<std::string>::process( char const* param );


/// @brief Generic non-specialized handler
template<typename T>
eArgErrorNumber PWX_API TArgTarget<T>::process( char const* param ) {
	eArgErrorNumber argErrno = AEN_OK;

	if ( target ) {
		T val = ( T )0;
		if ( param && param[0] )
			this->par_to_val( &val, param );

		switch( arg_type ) {
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
				if ( ( ( AT_ZERO_OR_ONE == this->set_type )
				    || ( AT_EXACTLY_ONCE == this->set_type ) )
				  && this->gotParameter ) {
					argErrno = AEN_MULTIPLE_SET_PARAM;
				} else {
					*target = val;
					this->gotParameter = true;
				}
				break;
			case ATT_CB:
				/* is handled below */
				break;
			default:
				PWX_THROW( "UnhandledTargetType",
				           "The given target type is not implemented, yet!", "" )
		}
	} // Have parameter

	// If we have a callback function, process it now
	if ( AEN_OK == argErrno )
		argErrno = process_cb( param );

	// note down if this was processed successfully
	if ( AEN_OK == argErrno )
		this->wasProcessed = true;

	return argErrno;
}


// === par_to_val specializations prototypes ===

#ifndef PWX_NODOX

template <> void TArgTarget<float>::par_to_val( float* tgt, char const* param ) noexcept;
template <> void TArgTarget<double>::par_to_val( double* tgt, char const* param ) noexcept;
template <> void TArgTarget<long double>::par_to_val( long double* tgt, char const* param ) noexcept;

template <> void TArgTarget<bool>::par_to_val( bool* tgt, char const* param ) noexcept;
template <> void TArgTarget<int8_t>::par_to_val( int8_t* tgt, char const* param ) noexcept;
template <> void TArgTarget<uint8_t>::par_to_val( uint8_t* tgt, char const* param ) noexcept;
template <> void TArgTarget<int16_t>::par_to_val( int16_t* tgt, char const* param ) noexcept;
template <> void TArgTarget<uint16_t>::par_to_val( uint16_t* tgt, char const* param ) noexcept;
template <> void TArgTarget<int32_t>::par_to_val( int32_t* tgt, char const* param ) noexcept;
template <> void TArgTarget<uint32_t>::par_to_val( uint32_t* tgt, char const* param ) noexcept;
template <> void TArgTarget<int64_t>::par_to_val( int64_t* tgt, char const* param ) noexcept;
template <> void TArgTarget<uint64_t>::par_to_val( uint64_t* tgt, char const* param ) noexcept;

#ifdef HAVE___INT128_T
template <> void TArgTarget<__int128_t>::par_to_val( __int128_t* tgt, char const* param ) noexcept;
#endif // HAVE___INT128_T

#ifdef HAVE___UINT128_T
template <> void TArgTarget<__uint128_t>::par_to_val( __uint128_t* tgt, char const* param ) noexcept;
#endif // HAVE___UINT128_T

#endif // PWX_NODOX


} // namespace pwx


#endif // PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED

