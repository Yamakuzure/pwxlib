#pragma once
#ifndef PWX_LIBPWX_PWX_TOOLS_CRANDOM_H_INCLUDED
#define PWX_LIBPWX_PWX_TOOLS_CRANDOM_H_INCLUDED 1

/** @file CRandom.h
  *
  * @brief Declaration of CRandom tool class
  *
  * (c) 2007 - 2012 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
  *
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
**/

#include <pwx/general/compiler.h>
#include <pwx/types/CLockable.h>

namespace pwx {

/** @class CRandom
  *
  * @brief Static class to produce unique or pseudo random numbers, hashes and names.
  *
  * This class produces a static instance called pwx::RNG, meaning
  * "Random Name/Noise/Number Generator".
  *
  * If PWX_THREADS is defined, all operations will be thread save.
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
class PWX_API CRandom: public CLockable
{
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
	CRandom(const CRandom&) PWX_DELETE; // No copying

	~CRandom() noexcept;

	/* ===============================================
	* === Public methods						  ===
	* ===============================================
	*/
	int32_t     getSeed  () const noexcept;
	uint32_t    hash     (int32_t        key) const noexcept;
	uint32_t    hash     (uint32_t       key) const noexcept;
	uint32_t    hash     (int64_t        key) const noexcept;
	uint32_t    hash     (uint64_t       key) const noexcept;
	uint32_t    hash     (float          key) const noexcept;
	uint32_t    hash     (double         key) const noexcept;
	uint32_t    hash     (const char*    key) const noexcept;
	uint32_t    hash     (::std::string &key) const noexcept;
	double      noise    (int32_t x) const noexcept;
	double      noise    (int32_t x, int32_t y) const noexcept;
	double      noise    (int32_t x, int32_t y, int32_t z) const noexcept;
	double      noise    (int32_t x, int32_t y, int32_t z, int32_t w) const noexcept;
	int32_t     random   (int32_t max = RAND_MAX) noexcept;
	int32_t     random   (int32_t min, int32_t max) noexcept;
	int64_t     random   (int64_t max) noexcept;
	int64_t     random   (int64_t min, int64_t max) noexcept;
	float       random   (float max) noexcept;
	float       random   (float min, float max) noexcept;
	double      random   (double max) noexcept;
	double      random   (double min, double max) noexcept;
	long double random   (long double max) noexcept;
	long double random   (long double min, long double max) noexcept;
	char*       rndName  (double x,                               bool lN = false, bool mW = false) noexcept;
	char*       rndName  (double x, double y,                     bool lN = false, bool mW = false) noexcept;
	char*       rndName  (double x, double y, double z,           bool lN = false, bool mW = false) noexcept;
	char*       rndName  (double x, double y, double z, double w, bool lN = false, bool mW = false) noexcept;
	char*       rndName  (double x,                               int32_t chars, int32_t sylls, int32_t parts) noexcept;
	char*       rndName  (double x, double y,                     int32_t chars, int32_t sylls, int32_t parts) noexcept;
	char*       rndName  (double x, double y, double z,           int32_t chars, int32_t sylls, int32_t parts) noexcept;
	char*       rndName  (double x, double y, double z, double w, int32_t chars, int32_t sylls, int32_t parts) noexcept;
	void        setSeed  (int32_t newSeed) noexcept;
	double      simplex1D(double x,                               double zoom = 1.0, double smooth = 1.0) noexcept;
	double      simplex1D(double x,                               double zoom, double smooth, double reduction, int32_t waves) noexcept;
	double      simplex2D(double x, double y,                     double zoom = 1.0, double smooth = 1.0) noexcept;
	double      simplex2D(double x, double y,                     double zoom, double smooth, double reduction, int32_t waves) noexcept;
	double      simplex3D(double x, double y, double z,           double zoom = 1.0, double smooth = 1.0) noexcept;
	double      simplex3D(double x, double y, double z,           double zoom, double smooth, double reduction, int32_t waves) noexcept;
	double      simplex4D(double x, double y, double z, double w, double zoom = 1.0, double smooth = 1.0) noexcept;
	double      simplex4D(double x, double y, double z, double w, double zoom, double smooth, double reduction, int32_t waves) noexcept;


	/* ===============================================
	 * === Public operators						===
	 * ===============================================
	*/

	CRandom &operator=(const CRandom&) PWX_DELETE; // No copying


	/* ===============================================
	 * === Public members						  ===
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

	PWX_PRIVATE_INLINE void    checkRule  (int32_t &state, const char first, const char second, const char third) noexcept;
	PWX_PRIVATE_INLINE int32_t genSyllable(double &idx, double step, char*  syll, int32_t &state, char*  lastChrs) noexcept PWX_WARNUNUSED;
	PWX_PRIVATE_INLINE double  getStepping(double i, double x, double y, double z, double w, int32_t cl, int32_t sl, int32_t pl) noexcept PWX_WARNUNUSED;

	/* === Helper methods for Simplex Noise === */
	PWX_PRIVATE_INLINE double  getSimpDot(int32_t index, double x) noexcept;
	PWX_PRIVATE_INLINE double  getSimpDot(int32_t index, double x, double y) noexcept;
	PWX_PRIVATE_INLINE double  getSimpDot(int32_t index, double x, double y, double z) noexcept;
	PWX_PRIVATE_INLINE double  getSimpDot(int32_t index, double x, double y, double z, double w) noexcept;
	/* Note: These are four functions, because using 1 with default values would cause alot of overhead with 0
		   multiplication. Testing 10M Iterations with 2 dimensions was 500ms slower with such an "universal"
		   getSimpDot() method. */
	PWX_PRIVATE_INLINE double  getSpx1D (double x) noexcept;
	PWX_PRIVATE_INLINE double  getSpx2D (double x, double y) noexcept;
	PWX_PRIVATE_INLINE double  getSpx3D (double x, double y, double z) noexcept;
	PWX_PRIVATE_INLINE double  getSpx4D (double x, double y, double z, double w) noexcept;

	// These are helpers to make the functions using raw noise more powerful when calculating with doubles
	PWX_PRIVATE_INLINE double  noiseD   (double x) const noexcept;
	PWX_PRIVATE_INLINE double  noiseD   (double x, double y) const noexcept;
	PWX_PRIVATE_INLINE double  noiseD   (double x, double y, double z) const noexcept;
	PWX_PRIVATE_INLINE double  noiseD   (double x, double y, double z, double w) const noexcept;
	PWX_PRIVATE_INLINE int32_t doubToInt(double val) const noexcept;


	/* ===============================================
	 * === Private operators					   ===
	 * ===============================================
	*/

	/* ===============================================
	 * === Private members						 ===
	 * ===============================================
	*/

	int32_t lastRndValue;  //!< The last by rand() generated value
	int32_t seed;          //!< General seed, can be changed with setSeed(new_value)
	double  spxCorn[5];    //!< The Corners contributing to a simplex noise. (1D: 2, 4D: 5 corners)
	double  spxDist[5][4]; //!< Simplex distance of a point to the simplex' corners
	int32_t spxGrads[5];   //!< Gradient table index for the simplex corners
	int32_t spxNorms[4];   //!< Normalized Coordinates for x, y, z, w
	int32_t spxOffs[3][4]; //!< Offsets for determining which vertice a dot is in
	int32_t spxPerms[4];   //!< Permutation table indices for x, y, z, w
	int32_t spxTab[512];   //!< A permutation table for simplex noise
};

extern CRandom RNG; //!< External instance of Random to be used

} // namespace pwx

#endif // PWX_LIBPWX_PWX_TOOLS_CRANDOM_H_INCLUDED

