#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif


#if !defined(PWX_INITIAL_SCT_PRECISION)
#  define PWX_INITIAL_SCT_PRECISION -1
#endif // defined(PWX_INITIAL_SCT_PRECISION)


#include "macros.h"
#include "MathHelpers.h"
#include "CLockGuard.h"
#include "CSinCosTable.h"

namespace pwx {


CSinCosTable SCT(PWX_INITIAL_SCT_PRECISION);


/** @brief CSinCosTable default ctor
  *
  * Default constructor that builds tables for precision
  * @a initial_precision
  *
  * Note: If the memory allocation fails, the precision is changed
  *       to -1 - live calculation.
  *
  * @param[in] initial_precision The precision to use initially
  */
CSinCosTable::CSinCosTable(int32_t initial_precision) :
	precision(-1),
	precision_last(-1),
	tableCos(nullptr),
	tableMultiplier(0),
	tableSin(nullptr),
	tableSize(0)
{
	if (initial_precision > -1) {
		PWX_TRY(this->setPrecision(initial_precision))
		PWX_CATCH_AND_FORGET(CException)
		// Note: if the table allocation fails, there
		//       is nothing to be done. setPrecision()
		//       reverts to live calculation anyway.
	}
}


/// @brief CSinCosTable default dtor
CSinCosTable::~CSinCosTable() noexcept
{
	if (tableCos) delete [] tableCos;
	if (tableSin) delete [] tableSin;
	tableCos = nullptr;
	tableSin = nullptr;
}


/** @brief get the currently used precision
  * @return the currently used precision
  */
int32_t CSinCosTable::getPrecision() const noexcept
{
	return precision;
}


/** @brief set a new precsion
  *
  * This method changes the precision that is used.
  * New tables for the sine and cosine values are built,
  * if neither the @ newPrecision is -1, nor @a newPrecision
  * equals the last precision while the current precision is
  * -1. In those cases the tables are saved/reused.
  *
  * @param newPrecision The precision to use from now on
  */
void CSinCosTable::setPrecision(const int32_t newPrecision)
{
	if (newPrecision != precision) {
		PWX_LOCK_GUARD(this)

		/* New tables are not needed when:
		 * a) This is a switch to live calculation or
		 * b) This is a switch from live calculation
		 *    but the precision was the same before.
		 */

		if ( ( -1 != newPrecision) // Not situation a)
		  && ( (newPrecision != precision_last)
			|| (-1 != precision) ) // Not situation b)
		   ) {
		   	// Neither of the two, create new tables:
		   	if (tableCos) delete [] tableCos;
		   	if (tableSin) delete [] tableSin;

			tableMultiplier = std::pow(10, newPrecision);
			tableSize       = 360 * tableMultiplier;

			// Remove old tables
			try {
				tableCos = new double[tableSize];
				tableSin = new double[tableSize];
			} catch (std::exception &e) {
				// If the new operator fails, revert to live
				// and rethrow
				precision      = -1;
				precision_last = -1;
				if (tableCos) delete [] tableCos;
				if (tableSin) delete [] tableSin;
				tableCos = nullptr;
				tableSin = nullptr;
				PWX_THROW("bad_alloc", e.what(), "Allocating new tables in SCT failed")
			}

			// If both tables are allocated, fill them.
			double radiant;
			for (int i = 0; i < tableSize; ++i) {
				radiant = degToRad(static_cast<double>(i) / static_cast<double>(tableMultiplier));
				tableCos[i] = std::cos( radiant );
				tableSin[i] = std::sin( radiant );
			}

			DEBUG_LOG("SCT.setPrecision", "Initialized %u values needing %7.2f MiB",
						tableSize * 2,
						static_cast<float>(sizeof(double)) * 2.f
						* static_cast<float>(tableSize)
						/ 1024.f / 1024.f)
		} // end of needing new tables


		precision_last = precision;
		precision = newPrecision;
	} // end of having a new precision
}


/** @brief return the cosine of @a degree
  * @param[in] degree The degree to get the cosine for.
  * @return The cosine of @a degree.
  */
double CSinCosTable::privGetCos(const double degree) const noexcept
{
	if (precision > -1) {
		int32_t normDeg = static_cast<int32_t>(std::round(degree * tableMultiplier));

		if      (normDeg >= tableSize) normDeg %= tableSize;
		else if (normDeg <  0        ) normDeg  = tableSize - (-normDeg % tableSize);

		return tableCos[normDeg];
	}
	return std::cos(degToRad(degree));
}


/** @brief return the sine of @a degree
  * @param[in] degree The degree to get the sine for.
  * @return The sine of @a degree.
  */
double CSinCosTable::privGetSin(const double degree) const noexcept
{
	if (precision > -1) {
		int32_t normDeg = static_cast<int32_t>(std::round(degree * tableMultiplier));

		if      (normDeg >= tableSize) normDeg %= tableSize;
		else if (normDeg <  0        ) normDeg  = tableSize - (-normDeg % tableSize);

		return tableSin[normDeg];
	}
	return std::sin(degToRad(degree));
}


/** @brief set @a cosDest to the cosine and @a sinDest to the sine of @a degree
  * @param[in] degree The degree to get the sine and cosine for.
  * @param[out] cosDest Target for the cosine of @a degree.
  * @param[out] sinDest Target for the sine of @a degree.
**/
void CSinCosTable::privGetSinCos(const double degree, double &cosDest, double &sinDest) const noexcept
{
	if (precision > -1) {
		int32_t normDeg = static_cast<int32_t>(std::round(degree * tableMultiplier));

		if      (normDeg >= tableSize) normDeg %= tableSize;
		else if (normDeg <  0        ) normDeg  = tableSize - (-normDeg % tableSize);

		cosDest = tableCos[normDeg];
		sinDest = tableSin[normDeg];
	} else {
		double radiant = degToRad(degree);
		cosDest = std::cos(radiant);
		sinDest = std::sin(radiant);
	}
}


} // namespace pwx
