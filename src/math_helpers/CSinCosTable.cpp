/** @file
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


#include <cstring>

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "basic/CException.h"
#include "basic/CLockGuard.h"
#include "math_helpers/CSinCosTable.h"
#include "math_helpers/MathHelpers.h"


namespace pwx {


CSinCosTable::CSinCosTable( int32_t initial_precision ) :
	precision( -1 ),
	precision_last( -1 ),
	tableCos( nullptr ),
	tableMultiplier( 0 ),
	tableSin( nullptr ),
	tableSize( 0 ) {
	if ( initial_precision > -1 ) {
		PWX_TRY( this->setPrecision( initial_precision ) )
		PWX_CATCH_AND_FORGET( CException );
		// Note: if the table allocation fails, there
		//       is nothing to be done. setPrecision()
		//       reverts to live calculation anyway.
	}
}


CSinCosTable::~CSinCosTable() noexcept {
	isDestroyed.store( true );

	// Make sure the coast is clear before destroying data
	PWX_LOCK_GUARD( this );

	// Reset locks so that other threads may notice that this is gone
	while ( waiting() ) {
		PWX_LOCK_GUARD_RESET( this );
	}

	this->clearTables();

	// Reset locks again before finishing the destructor
	while ( waiting() ) {
		PWX_LOCK_GUARD_RESET( this );
	}
}


void CSinCosTable::clearTables() noexcept {
	PWX_LOCK_GUARD( this );
	if ( tableCos ) delete [] tableCos;
	if ( tableSin ) delete [] tableSin;
	tableCos = nullptr;
	tableSin = nullptr;
}


int32_t CSinCosTable::getPrecision() const noexcept {
	return precision;
}


void CSinCosTable::setPrecision( const int32_t newPrecision ) {
	if ( newPrecision != precision ) {
		if (isDestroyed.load())
			return;

		/* New tables are not needed when:
		 * a) This is a switch to live calculation or
		 * b) This is a switch from live calculation
		 *    but the precision was the same before.
		 */
		PWX_LOCK_GUARD( this );
		if ( ( -1 != newPrecision ) // Not situation a)
		  && ( ( newPrecision != precision_last ) || ( -1 != precision ) ) // Not situation b)
		   ) {
			// Neither of the two, create new tables:
			this->clearTables();

			tableMultiplier = std::pow( 10, newPrecision );
			tableSize       = 360 * tableMultiplier;

			// Remove old tables
			try {
				tableCos = new double[tableSize];
				tableSin = new double[tableSize];
			} catch ( std::exception& e ) {
				// If the new operator fails, revert to live
				// and rethrow
				precision      = -1;
				precision_last = -1;
				if ( tableCos ) delete [] tableCos;
				if ( tableSin ) delete [] tableSin;
				tableCos = nullptr;
				tableSin = nullptr;
				PWX_THROW( "bad_alloc", e.what(), "Allocating new tables in SCT failed" );
			}

			// If both tables are allocated, fill them.
			double radiant;
			for ( int i = 0; i < tableSize; ++i ) {
				radiant = degToRad( static_cast<double>( i ) / static_cast<double>( tableMultiplier ) );
				tableCos[i] = std::cos( radiant );
				tableSin[i] = std::sin( radiant );
			}

			DEBUG_LOG( "SCT.setPrecision", "Initialized %u values needing %7.2f MiB",
			           tableSize * 2,
			           static_cast<float>( sizeof( double ) ) * 2.f
			           * static_cast<float>( tableSize )
			           / 1024.f / 1024.f );
		} // end of needing new tables


		precision_last = precision;
		precision = newPrecision;
	} // end of having a new precision
}


/** @brief return the cosine of @a degree
  * @param[in] degree The degree to get the cosine for.
  * @return The cosine of @a degree.
  */
double CSinCosTable::privGetCos( const double degree ) const noexcept {
	if ( precision > -1 ) {
		int32_t normDeg = static_cast<int32_t>( std::round( degree * tableMultiplier ) );

		if      ( normDeg >= tableSize ) normDeg %= tableSize;
		else if ( normDeg <  0        ) normDeg  = tableSize - ( -normDeg % tableSize );

		return tableCos[normDeg];
	}
	return std::cos( degToRad( degree ) );
}


/** @brief return the sine of @a degree
  * @param[in] degree The degree to get the sine for.
  * @return The sine of @a degree.
  */
double CSinCosTable::privGetSin( const double degree ) const noexcept {
	if ( precision > -1 ) {
		int32_t normDeg = static_cast<int32_t>( std::round( degree * tableMultiplier ) );

		if      ( normDeg >= tableSize ) normDeg %= tableSize;
		else if ( normDeg <  0        ) normDeg  = tableSize - ( -normDeg % tableSize );

		return tableSin[normDeg];
	}
	return std::sin( degToRad( degree ) );
}


/** @brief set @a cosDest to the cosine and @a sinDest to the sine of @a degree
  * @param[in] degree The degree to get the sine and cosine for.
  * @param[out] cosDest Target for the cosine of @a degree.
  * @param[out] sinDest Target for the sine of @a degree.
**/
void CSinCosTable::privGetSinCos( const double degree, double& cosDest, double& sinDest ) const noexcept {
	if ( precision > -1 ) {
		int32_t normDeg = static_cast<int32_t>( std::round( degree * tableMultiplier ) );

		if      ( normDeg >= tableSize ) normDeg %= tableSize;
		else if ( normDeg <  0        ) normDeg  = tableSize - ( -normDeg % tableSize );

		cosDest = tableCos[normDeg];
		sinDest = tableSin[normDeg];
	} else {
		double radiant = degToRad( degree );
		cosDest = std::cos( radiant );
		sinDest = std::sin( radiant );
	}
}


} // namespace pwx
