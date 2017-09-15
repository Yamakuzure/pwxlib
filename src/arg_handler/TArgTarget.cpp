/***
  This file is part of the PrydeWorX Library (pwxLib).

  Copyright 2007 - 2017 Sven Eden

  The PrydeWorX Library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  The PrydeWorX Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with pwxLib; If not, see <http://www.gnu.org/licenses/>.

  History and Changelog are maintained in pwx.h
***/


#include "StreamHelpers.h"
#include "TArgTarget.h"


namespace pwx {


// === Specialized processing handlers ===


template<>
eArgErrorNumber TArgTarget<std::string>::process( const char* param )
{
	eArgErrorNumber argErrno = AEN_OK;

	switch( this->type ) {
		case ATT_FALSE:
		case ATT_TRUE:
		case ATT_INC:
		case ATT_DEC:
		case ATT_ADD:
		case ATT_SUB:
			PWX_THROW( "IllegalTargetType",
			           "std::string is only supported with ATT_CB and ATT_SET",
			           "" )
			break;
		case ATT_SET:
			// This needs handling for all three set types:
			if ( ( STT_OVERWRITE == this->setType )
			        || ( !this->gotParameter ) ) {
				target->assign( param );
				this->gotParameter = true;
			} else if ( STT_ERROR == this->setType )
				argErrno = AEN_MULTIPLE_SET_PARAM;
			// Last possibility is STT_IGNORE, which is, well, ignored. ;)
			break;
		case ATT_CB:
			PWX_THROW( "UnhandledTargetType", "ATT_CB not supported, use CArgCallback instead!", "" )
			break;
		default:
			PWX_THROW( "UnhandledTargetType", "The given target type is not implemented, yet!", "" )
	}

	return argErrno;
}


// === par_to_val conversion methods ===


// --- bool ---
template<>
void TArgTarget<bool>::par_to_val( bool*    tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_bool  ( param );
}


// --- integers ---
template<>
void TArgTarget<int8_t>::par_to_val( int8_t*  tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_int8  ( param );
}

template<>
void TArgTarget<int16_t>::par_to_val( int16_t* tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_int16 ( param );
}

template<>
void TArgTarget<int32_t>::par_to_val( int32_t* tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_int32 ( param );
}

template<>
void TArgTarget<int64_t>::par_to_val( int64_t* tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_int64 ( param );
}

template<>
void TArgTarget<uint8_t>::par_to_val( uint8_t* tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_uint8 ( param );
}

template<>
void TArgTarget<uint16_t>::par_to_val( uint16_t* tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_uint16( param );
}

template<>
void TArgTarget<uint32_t>::par_to_val( uint32_t* tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_uint32( param );
}

template<>
void TArgTarget<uint64_t>::par_to_val( uint64_t* tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_uint64( param );
}

// --- floats ---
template<>
void TArgTarget<float>::par_to_val( float*      tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_float ( param );
}

template<>
void TArgTarget<double>::par_to_val( double*     tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_double( param );
}

template<>
void TArgTarget<long double>::par_to_val( long double* tgt, const char* param ) noexcept
{
	if ( tgt )
		*tgt = to_long_double( param );
}



} // namespace pwx
