#ifndef PWX_LIBPWX_PWX_WORKER_CRANDOM_H_INCLUDED
#define PWX_LIBPWX_PWX_WORKER_CRANDOM_H_INCLUDED 1
#pragma once

/** @file CRandom.h
  *
  * @brief Declaration of CRandom worker class
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
#include <string>

#include "basic/pwx_compiler.h"

#include "basic/CLockable.h"
#include "random/eNameSourceType.h"


namespace pwx {


/** @class CRandom PRandom <PRandom>
  *
  * @brief Static class to produce unique or pseudo random numbers, hashes and names.
  *
  * This class produces a static instance called pwx::RNG, meaning
  * "Random Name/Noise/Number Generator".
  *
  * The following sets of functions are available:
  *
  * - random()
  * These return random numbers as int32_t, int64_t, float, double and
  * long double. They can be used without or with up to two arguments
  * to get results between those two or from zero to the one argument.
  *
  * - hash()
  * Hash functions for integer arguments, mostly taken from:
  * http://www.burtleburtle.net/bob/hash/index.html (Robert Jenkins)
  * http://www.cris.com/~Ttwang/tech/inthash.htm (Thomas Wang)
  * The hash functions for strings and floating point numbers are of
  * my own invention.
  * The result is always an uint32_t.
  *
  * - noise()
  * These are not the classic Perlin noise functions, but simple
  * wrappers that transform hash() results into a -1.0 to 1.0 double
  * range.
  *
  * - simplex()
  * This set of functions produce pseudo random numbers using
  * Simplex Noise (2D, 3D and 4D) by Ken Perlin, and are called
  * simplex().
  * The documentation is taken from:
  * http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
  *  (Stefan Gustavson)
  *
  * - rndName()
  * A method that returns a random name built by combining random
  * letters into syllables.
  * Important: The returned name is a malloc'd C-String. You are
  *            responsible to free it after usage!
  *
**/
class PWX_API CRandom: public CLockable {
public:

	/* ===============================================
	* === Public types							===
	* ===============================================
	*/


	/* ===============================================
	* === Public Constructors and destructors	 ===
	* ===============================================
	*/

	explicit CRandom() noexcept;
	~CRandom() noexcept;

	// No copying:
	CRandom( CRandom const&  ) PWX_DELETE;
	CRandom( CRandom const&& ) PWX_DELETE;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	 */
	int32_t     getSeed  () const noexcept;
	uint32_t    hash     ( int16_t     key ) const noexcept;
	uint32_t    hash     ( uint16_t    key ) const noexcept;
	uint32_t    hash     ( int32_t     key ) const noexcept;
	uint32_t    hash     ( uint32_t    key ) const noexcept;
	uint32_t    hash     ( int64_t     key ) const noexcept;
	uint32_t    hash     ( uint64_t    key ) const noexcept;
	uint32_t    hash     ( float       key ) const noexcept;
	uint32_t    hash     ( double      key ) const noexcept;
	uint32_t    hash     ( long double key ) const noexcept;
	uint32_t    hash     ( char const* key, size_t keyLen = 0 ) const noexcept;
	uint32_t    hash     ( std::string& key ) const noexcept;
	eNameSourceType
	nextNST  ( void ) noexcept;
	double      noise    ( int32_t x ) const noexcept;
	double      noise    ( int32_t x, int32_t y ) const noexcept;
	double      noise    ( int32_t x, int32_t y, int32_t z ) const noexcept;
	double      noise    ( int32_t x, int32_t y, int32_t z, int32_t w ) const noexcept;
	eNameSourceType
	prevNST  ( void ) noexcept;
	int16_t     random   ( int16_t max ) noexcept;
	int16_t     random   ( int16_t min, int16_t max ) noexcept;
	uint16_t    random   ( uint16_t max ) noexcept;
	uint16_t    random   ( uint16_t min, uint16_t max ) noexcept;
	int32_t     random   ( int32_t max = RAND_MAX ) noexcept;
	int32_t     random   ( int32_t min, int32_t max ) noexcept;
	uint32_t    random   ( uint32_t max = RAND_MAX ) noexcept;
	uint32_t    random   ( uint32_t min, uint32_t max ) noexcept;
	int64_t     random   ( int64_t max ) noexcept;
	int64_t     random   ( int64_t min, int64_t max ) noexcept;
	uint64_t    random   ( uint64_t max ) noexcept;
	uint64_t    random   ( uint64_t min, uint64_t max ) noexcept;
	float       random   ( float max ) noexcept;
	float       random   ( float min, float max ) noexcept;
	double      random   ( double max ) noexcept;
	double      random   ( double min, double max ) noexcept;
	long double random   ( long double max ) noexcept;
	long double random   ( long double min, long double max ) noexcept;
	size_t      random   ( char* dest, size_t minLen, size_t maxLen ) noexcept;
	char*       rndName  ( double x,                               bool lN = false, bool mW = false ) noexcept;
	char*       rndName  ( double x, double y,                     bool lN = false, bool mW = false ) noexcept;
	char*       rndName  ( double x, double y, double z,           bool lN = false, bool mW = false ) noexcept;
	char*       rndName  ( double x, double y, double z, double w, bool lN = false, bool mW = false ) noexcept;
	char*       rndName  ( double x,                               int32_t chars, int32_t sylls, int32_t parts ) noexcept;
	char*       rndName  ( double x, double y,                     int32_t chars, int32_t sylls, int32_t parts ) noexcept;
	char*       rndName  ( double x, double y, double z,           int32_t chars, int32_t sylls, int32_t parts ) noexcept;
	char*       rndName  ( double x, double y, double z, double w, int32_t chars, int32_t sylls, int32_t parts ) noexcept;
	void        setNST   ( eNameSourceType type ) noexcept;
	void        setSeed  ( int32_t newSeed ) noexcept;
	double      simplex1D( double x,                               double zoom = 1.0, double smooth = 1.0 ) noexcept;
	double      simplex1D( double x,                               double zoom, double smooth, double reduction, int32_t waves ) noexcept;
	double      simplex2D( double x, double y,                     double zoom = 1.0, double smooth = 1.0 ) noexcept;
	double      simplex2D( double x, double y,                     double zoom, double smooth, double reduction, int32_t waves ) noexcept;
	double      simplex3D( double x, double y, double z,           double zoom = 1.0, double smooth = 1.0 ) noexcept;
	double      simplex3D( double x, double y, double z,           double zoom, double smooth, double reduction, int32_t waves ) noexcept;
	double      simplex4D( double x, double y, double z, double w, double zoom = 1.0, double smooth = 1.0 ) noexcept;
	double      simplex4D( double x, double y, double z, double w, double zoom, double smooth, double reduction, int32_t waves ) noexcept;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	// No copying:
	CRandom& operator=( CRandom const&  ) PWX_DELETE;
	CRandom& operator=( CRandom const&& ) PWX_DELETE;


	/* ===============================================
	 * === Public members                          ===
	 * ===============================================
	*/


private:

	/* ===============================================
	 * === Private types						   ===
	 * ===============================================
	*/

	/* ===============================================
	 * === Private Constructors and destructor	 ===
	 * ===============================================
	*/

	/* ===============================================
	 * === Private methods						 ===
	 * ===============================================
	*/

	PWX_PRIVATE_INLINE void    checkRule  ( uint32_t& state, char const first, char const second, char const third ) noexcept;
	PWX_PRIVATE_INLINE int32_t genSyllable( double& idx, double step, char*  syll, uint32_t& state, char*  lastChrs ) noexcept PWX_WARNUNUSED;
	PWX_PRIVATE_INLINE double  getStepping( double i, double x, double y, double z, double w, int32_t cl, int32_t sl,
	                                        int32_t pl ) noexcept PWX_WARNUNUSED;

	/* === Helper methods for Simplex Noise === */
	PWX_PRIVATE_INLINE double  getSimpDot( int32_t index, double x ) noexcept;
	PWX_PRIVATE_INLINE double  getSimpDot( int32_t index, double x, double y ) noexcept;
	PWX_PRIVATE_INLINE double  getSimpDot( int32_t index, double x, double y, double z ) noexcept;
	PWX_PRIVATE_INLINE double  getSimpDot( int32_t index, double x, double y, double z, double w ) noexcept;
	/* Note: These are four functions, because using 1 with default values would cause alot of overhead with 0
		   multiplication. Testing 10M Iterations with 2 dimensions was 500ms slower with such an "universal"
		   getSimpDot() method. */
	PWX_PRIVATE_INLINE double  getSpx1D ( double x ) noexcept;
	PWX_PRIVATE_INLINE double  getSpx2D ( double x, double y ) noexcept;
	PWX_PRIVATE_INLINE double  getSpx3D ( double x, double y, double z ) noexcept;
	PWX_PRIVATE_INLINE double  getSpx4D ( double x, double y, double z, double w ) noexcept;

	// These are helpers to make the functions using raw noise more powerful when calculating with doubles
	PWX_PRIVATE_INLINE double  noiseD   ( double x ) const noexcept;
	PWX_PRIVATE_INLINE double  noiseD   ( double x, double y ) const noexcept;
	PWX_PRIVATE_INLINE double  noiseD   ( double x, double y, double z ) const noexcept;
	PWX_PRIVATE_INLINE double  noiseD   ( double x, double y, double z, double w ) const noexcept;
	PWX_PRIVATE_INLINE int32_t doubToInt( double val ) const noexcept;

	/* ===============================================
	 * === Private operators					   ===
	 * ===============================================
	*/

	/* ===============================================
	 * === Private members						 ===
	 * ===============================================
	*/

	eNameSourceType nst; //!< Type of the name source
	int32_t         seed;          //!< General seed, can be changed with setSeed(new_value)
	double          spxCorn[5];    //!< The Corners contributing to a simplex noise. (1D: 2, 4D: 5 corners)
	double          spxDist[5][4]; //!< Simplex distance of a point to the simplex' corners
	int32_t         spxGrads[5];   //!< Gradient table index for the simplex corners
	int32_t         spxNorms[4];   //!< Normalized Coordinates for x, y, z, w
	int32_t         spxOffs[3][4]; //!< Offsets for determining which vertice a dot is in
	int32_t         spxPerms[4];   //!< Permutation table indices for x, y, z, w
	int32_t         spxTab[512];   //!< A permutation table for simplex noise
};

} // namespace pwx

#endif // PWX_LIBPWX_PWX_WORKER_CRANDOM_H_INCLUDED

