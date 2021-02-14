/** @file
  *
  * Implementations of the specialized methods of the TArgTarget class template
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


#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

#include "basic/pwx_compiler.h"
#include "basic/pwx_types.h"

#include "arg_handler/TArgTarget.h"
#include "stream_helpers/StreamHelpers.h"

/// @namespace pwx
namespace pwx {


// === Specialized processing handlers ===


template <> eArgErrorNumber TArgTarget<std::string>::process( char const* param ) {
	eArgErrorNumber argErrno = AEN_OK;

	if ( param ) {
		switch( this->arg_type ) {
			case ATT_FALSE:
			case ATT_TRUE:
			case ATT_INC:
			case ATT_DEC:
			case ATT_ADD:
			case ATT_SUB:
				PWX_THROW( "IllegalTargetType",
				           "std::string is only supported with ATT_CB and ATT_SET", "" )
				break;
			case ATT_SET:
				if ( ( ( AT_ZERO_OR_ONE  == this->set_type )
				    || ( AT_EXACTLY_ONCE == this->set_type ) )
				  && this->gotParameter ) {
					argErrno = AEN_MULTIPLE_SET_PARAM;
				} else {
					target->assign( param );
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
	}

	// If we have a callback function, process it now
	if ( AEN_OK == argErrno )
		argErrno = process_cb( param );

	// note down if this was processed successfully
	if ( AEN_OK == argErrno )
		this->wasProcessed = true;

	return argErrno;
}


// === par_to_val conversion methods ===


// --- bool ---
template <> void TArgTarget<bool>::par_to_val( bool* tgt, char const* param ) noexcept {
	if ( tgt ) {
		*tgt = to_bool  ( param );
	}
}


// --- integers ---
template <> void TArgTarget<int8_t>::par_to_val( int8_t*  tgt, char const* param ) noexcept {
	if ( tgt ) {
		*tgt = to_int8  ( param );
	}
}

template <> void TArgTarget<int16_t>::par_to_val( int16_t* tgt, char const* param ) noexcept {
	if ( tgt ) {
		*tgt = to_int16 ( param );
	}
}

template <> void TArgTarget<int32_t>::par_to_val( int32_t* tgt, char const* param ) noexcept {
	if ( tgt ) {
		*tgt = to_int32 ( param );
	}
}

template <> void TArgTarget<int64_t>::par_to_val( int64_t* tgt, char const* param ) noexcept {
	if ( tgt ) {
		*tgt = to_int64 ( param );

	}
}

#if HAVE___INT128_T
template <> void TArgTarget<__int128_t>::par_to_val( __int128_t* tgt, char const* param ) noexcept {
	if ( tgt ) {
		*tgt = to_int128 ( param );
	}
}
#endif // HAVE___INT128_T

template <> void TArgTarget<uint8_t>::par_to_val( uint8_t*  tgt, char const* param ) noexcept {
	if ( tgt )
		*tgt = to_uint8  ( param );
}

template <> void TArgTarget<uint16_t>::par_to_val( uint16_t* tgt, char const* param ) noexcept {
	if ( tgt ) {
		*tgt = to_uint16 ( param );
	}
}

template <> void TArgTarget<uint32_t>::par_to_val( uint32_t* tgt, char const* param ) noexcept {
	if ( tgt ) {
		*tgt = to_uint32 ( param );
	}
}

template <> void TArgTarget<uint64_t>::par_to_val( uint64_t* tgt, char const* param ) noexcept {
	if ( tgt ) {
		*tgt = to_uint64 ( param );
	}
}

#if HAVE___UINT128_T
template <> void TArgTarget<__uint128_t>::par_to_val( __uint128_t* tgt, char const* param ) noexcept {
	if ( tgt ) {
		*tgt = to_uint128 ( param );
	}
}
#endif // HAVE___UINT128_T

// --- floats ---
template <> void TArgTarget<float>::par_to_val( float*      tgt, char const* param ) noexcept {
	if ( tgt )
		*tgt = to_float ( param );
}

template <> void TArgTarget<double>::par_to_val( double*     tgt, char const* param ) noexcept {
	if ( tgt )
		*tgt = to_double( param );
}

template <> void TArgTarget<long double>::par_to_val( long double* tgt, char const* param ) noexcept {
	if ( tgt )
		*tgt = to_long_double( param );
}


} // namespace pwx

