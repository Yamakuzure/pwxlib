#ifndef PWX_LIBPWX_PWX_WORKER_CSINCOSTABLE_H_INCLUDED
#define PWX_LIBPWX_PWX_WORKER_CSINCOSTABLE_H_INCLUDED

/** @file CSinCosTable.h
  *
  * @brief Declaration of CSinCosTables
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

#include "basic/CLockable.h"

namespace pwx {

/** @class CSinCosTable PSinCos <PSinCos>
  *
  * @brief Provides pre-calculated sine and cosine tables
  *
  * This class produces a static instance called pwx::SCT, meaning
  * "Sine-/Cosine-Table".
  *
  * <I>Note</I>: If you want the initial precision to be anything
  * else than the default of -1, define PWX_INITIAL_SCT_PRECISION
  * to the desired value when compiling pwxlib.
  *
  * Calculating sine and cosine values does not take much time
  * nowadays as FPUs get stronger every other day. On the other
  * hand, if you need these values for on-the-fly calculations
  * of something CPU-consuming like the display of 3D objects,
  * this little bit of CPU/FPU resources might better be saved.
  * Sin-/Cos-Tables with a precision of 3 use about 5.49 MiB RAM
  * (total), and this is not very much either.
  *
  * Tests show, that a Sin-/Cos-Tables with a precision of 3,
  * meaning 2 x 360,000 values in two arrays, do not differ from
  * on-the-fly calculation until the (worst case!) 6th digit.
  * (normally the first 7-8 digits are equal, which should be
  * enough for most applications.
  *
  * The default precision, however, is -1. There will be no
  * pre-defined tables, but all sine and cosine values calculated
  * on-the-fly. You still do not have to care about the range of
  * your angles, and do not need to transform angles to radians
  * though.
  *
  * Usage:
  *
  * sin() - return the sine of a given angle.
  * cos() - return the cosine of a given angle.
  * sincos() - get both at once.
  * setPrecision() - set a new precision. (Default is 3)
  *                  set this to -1 to enable life calculation.
  * getPrecision() - get the current precision.
  *
  * Please be aware, however, that changing the precision means
  * a recalculation of the sine and cosine arrays. Switching
  * between -1 (life calculation) and the initial value
  * does not trigger a re-initialization of the tables.
**/
class PWX_API CSinCosTable: public CLockable {
  public:

    /* ===============================================
     * === Public Constructors and destructors     ===
     * ===============================================
     */

    explicit CSinCosTable( const int32_t initial_precision );
    virtual ~CSinCosTable() noexcept;

    CSinCosTable() PWX_DELETE;

    /* ===============================================
     * === Public methods                          ===
     * ===============================================
     */

    int32_t getPrecision() const noexcept;
    void    setPrecision( const int32_t newPrecision );


    /** @brief return the cosine of @a degree
      *
      * The type T must be a type that can be cast into
      * a double and back.
      *
      * @param[in] degree The degree to calculate the cosine of.
      * @return The cosine of @a degree.
    **/
    template<typename T> T cos( const T degree ) const noexcept {
        return this->privGetCos( degree );
    }


    /** @brief return the cosine of @a degree
      *
      * The type T must be a type that can be cast into
      * a double and back.
      *
      * @param[in] degree The degree to calculate the cosine of.
      * @return The cosine of @a degree.
    **/
    template<typename T> T sin( const T degree ) const noexcept {
        return this->privGetCos( degree );
    }


    /** @brief set @a cosDest to the cosine and @a sinDest to the sine of @a degree
      *
      * The type T must be a type that can be cast into
      * a double and back.
      *
      * @param[in] degree The degree to calculate the cosine of.
      * @param[out] cosDest The target for the cosine of @a degree.
      * @param[out] sinDest The target for the sine of @a degree.
    **/
    template<typename T> void sincos( const T degree, T& cosDest, T& sinDest ) const noexcept {
        return this->privGetSinCos( degree, cosDest, sinDest );
    }


    /* ===============================================
     * === Public operators                        ===
     * ===============================================
     */

    CSinCosTable& operator=( CSinCosTable& ) PWX_DELETE;


  private:

    /* ===============================================
     * === Private methods                         ===
     * ===============================================
     */

    double      privGetCos( const double degree ) const noexcept;
    template<typename T>
    T           privGetCos( const T      degree ) const noexcept {
        return static_cast<T>( this->privGetCos( static_cast<double>( degree ) ) );
    }

    double privGetSin( const double degree ) const noexcept;
    template<typename T>
    T      privGetSin( const T      degree ) const noexcept {
        return static_cast<T>( this->privGetSin( static_cast<double>( degree ) ) );
    }

    void privGetSinCos( const double degree, double& cosDest, double& sinDest ) const noexcept;
    template<typename T>
    void privGetSinCos( const T      degree, T&      cosDest, T&      sinDest ) const noexcept {
        double xDegree  = static_cast<double>( degree );
        double xCosDest = static_cast<double>( cosDest );
        double xSinDest = static_cast<double>( sinDest );

        this->privGetSinCos( xDegree, xCosDest, xSinDest );

        cosDest = static_cast<T>( xCosDest );
        sinDest = static_cast<T>( xSinDest );
    }


    /* ===============================================
     * === Private members                         ===
     * ===============================================
     */

    int32_t precision;
    int32_t precision_last;
    double* tableCos;
    int32_t tableMultiplier;
    double* tableSin;
    int32_t tableSize;

};

} // namespace pwx

#endif // PWX_LIBPWX_PWX_WORKER_CSINCOSTABLE_H_INCLUDED

