#pragma once
#ifndef PWX_RANDOM_H_INCLUDED
#define PWX_RANDOM_H_INCLUDED

/** @file CRandom.h
  *
	* @brief Declaration of CRandom
	*
  * (c) 2007 - 2013 PrydeWorX
  * @author Sven Eden, PrydeWorX - Hamburg, Germany
  *         yamakuzure@users.sourceforge.net
  *         http://pwxlib.sourceforge.net
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
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.0.1     2007-12-21  sed, PrydeWorX  First Design
  * 0.1.0     2008-04-09  sed, PrydeWorX  initial release
  * 0.1.1     2008-11-11  sed, PrydeWorX  added documentation and int64_t/double versions
  * 0.1.2     2009-04-04  sed, PrydeWorX  forced all methods to be inlined with newer gcc
  * 0.1.3     2009-07-31  sed, PrydeWorX  some optimizations to favour mod instead of division
  * 0.2.0     2009-09-20  sed, PrydeWorX  restructured for a cleaner Class Body and seperate implementation
  * 0.2.1     2009-09-21  sed, PrydeWorX  Some optimizations by using some new tools in pwxDefaultDefines
  * 0.3.0     2009-09-22  sed, PrydeWorX  Added hash() and noise() functions
  * 0.4.0     2009-09-23  sed, PrydeWorX  Added Simplex Noise by Ken Perlin
  * 0.4.1     2009-09-25  sed, PrydeWorX  Simplified Simplex by using members for data instead of local vars
  * 0.4.2     2009-09-29  sed, PrydeWorX  Optimized and tested Simplex Noise - Re-organized The Class a bit.
  * 0.4.2-r1  2009-10-04  sed, PrydeWorX  Refined simplex1D.
  * 0.4.2-r2  2009-10-05  sed, PrydeWorX  Added version string. Sorry, completely forgot that!
  * 0.4.3     2009-10-06  sed, PrydeWorX  Added optional wave overlay to simplex*D() methods
  * 0.4.4     2009-11-10  sed, PrydeWorX  Finally added noise() for double
  *                                       Replaced all old-school casts to proper c++ versions.
  * 0.4.5     2010-01-28  sed, PrydeWorX  enforced new PrydeWorX Style Guide by getting rid of all hungarian
  *                                       notation stýle type abbreviations. (Not needed in C++ anyway)
  * 0.5.0     2010-02-05  sed, PrydeWorX  added random name generator
  * 0.5.1     2010-02-08  sed, PrydeWorX  fixed a bug in simplex generation that didn't check smooth values correctly
  * 0.5.2     2010-02-09  sed, PrydeWorX  optimized random name generation, and made it depend on 4 dimensions by
  *                                       default
  * 0.5.2-r1  2010-04-16  sed, PrydeWorX  Some rework to have pwxLib comply as much as possible with Items 57
  *                                       to 59 in "C++ Coding Standards" (Herb Sutter, Andrei Alexandrescu, 2005)
  * 0.5.3     2010-04-28  sed, PrydeWorX  genSyllable() of the name generator has been rewritten from scratch, now
  *                                       utilizing automatically generated character sequence rules.
  *                                       ( see CRandomWordConsatnts.h )
  * 0.5.4     2010-04-29  sed, PrydeWorX  fixed some issues with the name generator (calls to genStepping()) GREATLY
  *                                       enhancing the rate of unique names! (See internal/CRandomName.h)
  * 0.5.5     2010-05-16  sed, PrydeWorX  Applied GPLv3 and full author information
  * 0.5.5-r1  2010-05-17  sed, PrydeWorX  Added missing include for std::string
  * 0.5.6     2010-05-17  sed, PrydeWorX  Some minor changes to make inclusion in multiple object files easier
  *                                       without the linker throwing up
  * 0.6.0     2010-05-19  sed, PrydeWorX  rndName can now be called with max chars, sylls and parts, too.
  * 0.6.1     2010-07-09  sed, PrydeWorX  minor Bugfix in setSeed
  * 0.6.2     2010-07-12  sed, PrydeWorX  Added PWX_NO_RNG_INSTANCE
  * 0.6.4     2010-11-03  sed, PrydeWorX  eventually removed all (bad) underscore prefixes from PWX_* defines and macros
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.0.1   2010-11-11  sed, PrydeWorX  Corrected issues with 32bit compilers, and a problem with random(int64_t)
  * 0.7.0.2   2010-11-12  sed, PrydeWorX  Finally got rid of the bug that made rndName() choke in very rare cases
  *                                       Added a security check to simplex noise to really really have a -1.0 to 1.0 range
  *                                       Changed the naming rules, the textanalyzer did not filter out www adresses.
  * 0.7.0.3   2010-11-19  sed, PrydeWorX  Added Filters to textAnalyzer.pl and applied a lot new rules.
  * 0.7.0.4   2011-01-19  sed, PrydeWorX  Fixed a bug that caused middle part rules to never be used.
  * 0.7.0.5   2011-01-20  sed, PrydeWorX  Returning std::string.c_str() from stack is not portable to VC++. Thus
  *                                       rndName is changed to return a malloc'd C-string now
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Some restructuring for the documentation, some corrections and version bump to
  *                                       the new pwxLib release.
  * 0.7.2.1   2011-05-13  sed, PrydeWorX  Changed integer hash functions to return unsigned types.
  *                                       Added a 64 to 32 bit integer hash function.
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0.1   2011-09-20  sed, PrydeWorX  From now on CRandom is derived from pwx::Lockable
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5     2012-03-01  sed, PrydeWorX  Welcome to 2012. Version bump to new pwxLib release version
  * 0.8.5.1   2012-04-17  sed, PrydeWorX  Added noise() for double arguments to strengthen methods using them
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

#include "pwxLib/tools/DefaultDefines.h"
#include "pwxLib/internal/MSVCwarnPush.h"
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <limits>
#include "pwxLib/internal/MSVCwarnPop.h"

/** @namespace pwx
  *
  * @brief Namespace for everything PrydeWorX related
  *
  * All PrydeWorX projects reside in this namespace and it's sub-namespaces.
**/
namespace pwx
{

/** @namespace pwx::rng
  *
  * @brief Namespace for CRandom
  *
  * Everything concerning CRandom is defined in this namespace.
  *
  * @see pwx::rng::constants for the namespace holding consatnts used by CRandom
  * @see pwx::rng::NameConstants for special constants used by the name generator
**/
namespace rng
{

/** @class CRandom
  *
  * @brief Static class to produce unique or pseudo random numbers, hashes and names.
  *
  * This class produces a static instance called pwx::RNG, meaning
  * "Random Name/Noise/Number Generator", unless you have defined
  * PWX_NO_RNG_INSTANCE.
  *
  * If you define PWX_NO_RNG_INSTANCE prior including this header,
  * you have to instanciate your own version from pwx::SCT::CRandom.
  *
  * The following sets of functions are available:
  *
  * - random()
  * These return random numbers as int32_t, int64_t, float, double and
  * long double. They can be used without or with up to two arguments
  * to get results between those two or from zero to the one argument.
  *
  * - hash()
  * Hashing functions for integer arguments, mostly taken from:
  * http://www.burtleburtle.net/bob/hash/index.html (Robert Jenkins)
  * http://www.cris.com/~Ttwang/tech/inthash.htm (Thomas Wang)
  * The result is always a positive number.
  *
  * - noise()
  * These are not the classic perlin noise functions, but simple
  * wrappers that transform hash() results into a -1.0 to 1.0 double
  * range.
  *
  * - simplex()
  * This set of functions produce pseudo random numbers using
  * Simplex Noise (2D, 3D and 4D) by Ken Perlin, and are called
  * simplex().
  * Documentsation taken from:
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
class PWX_INTERNAL CRandom: public ::pwx::Lockable, private Uncopyable
{
public:
  //  ---  methods  ---
  PWX_INLINE int32_t        getSeed    ()              const;
  PWX_INLINE uint32_t       hash       (int32_t   key) const;
  PWX_INLINE uint32_t       hash       (uint32_t  key) const;
  PWX_INLINE uint64_t       hash       (int64_t   key) const;
  PWX_INLINE uint32_t       hash       (uint64_t  key) const;
  PWX_INLINE double         noise      (int32_t x)     const;
  PWX_INLINE double         noise      (int32_t x, int32_t y) const;
  PWX_INLINE double         noise      (int32_t x, int32_t y, int32_t z) const;
  PWX_INLINE double         noise      (int32_t x, int32_t y, int32_t z, int32_t w) const;
  PWX_INLINE int32_t        random     (int32_t max = RAND_MAX);
  PWX_INLINE int32_t        random     (int32_t min, int32_t max);
  PWX_INLINE int64_t        random     (int64_t max);
  PWX_INLINE int64_t        random     (int64_t min, int64_t max);
  PWX_INLINE float          random     (float max);
  PWX_INLINE float          random     (float min, float max);
  PWX_INLINE double         random     (double max);
  PWX_INLINE double         random     (double min, double max);
  PWX_INLINE long double    random     (long double max);
  PWX_INLINE long double    random     (long double min, long double max);
  PWX_INLINE char         * rndName    (double x,                               bool lN = false, bool mW = false);
  PWX_INLINE char         * rndName    (double x, double y,                     bool lN = false, bool mW = false);
  PWX_INLINE char         * rndName    (double x, double y, double z,           bool lN = false, bool mW = false);
  PWX_INLINE char         * rndName    (double x, double y, double z, double w, bool lN = false, bool mW = false);
  PWX_INLINE char         * rndName    (double x,                               int32_t chars, int32_t sylls, int32_t parts);
  PWX_INLINE char         * rndName    (double x, double y,                     int32_t chars, int32_t sylls, int32_t parts);
  PWX_INLINE char         * rndName    (double x, double y, double z,           int32_t chars, int32_t sylls, int32_t parts);
  PWX_INLINE char         * rndName    (double x, double y, double z, double w, int32_t chars, int32_t sylls, int32_t parts);
  PWX_INLINE void           setSeed    (int32_t newSeed);
  PWX_INLINE double         simplex1D  (double x,                               double zoom = 1.0, double smooth = 1.0);
  PWX_INLINE double         simplex1D  (double x,                               double zoom, double smooth, double reduction, int32_t waves);
  PWX_INLINE double         simplex2D  (double x, double y,                     double zoom = 1.0, double smooth = 1.0);
  PWX_INLINE double         simplex2D  (double x, double y,                     double zoom, double smooth, double reduction, int32_t waves);
  PWX_INLINE double         simplex3D  (double x, double y, double z,           double zoom = 1.0, double smooth = 1.0);
  PWX_INLINE double         simplex3D  (double x, double y, double z,           double zoom, double smooth, double reduction, int32_t waves);
  PWX_INLINE double         simplex4D  (double x, double y, double z, double w, double zoom = 1.0, double smooth = 1.0);
  PWX_INLINE double         simplex4D  (double x, double y, double z, double w, double zoom, double smooth, double reduction, int32_t waves);

protected:
private:
  //  ---  attributes  (non static variables) ---
  int32_t lastRndValue;  //!< The last by rand() generated value
  int32_t seed;          //!< General seed, can be changed with setSeed(new_value)
  double  spxCorn[5];    //!< The Corners contributing to a simplex noise. (1D: 2, 4D: 5 corners)
  double  spxDist[5][4]; //!< Simplex distance of a point to the simplex' corners
  int32_t spxGrads[5];   //!< Gradient table indices for the simplex corners
  int32_t spxNorms[4];   //!< Normalized Coordinates for x, y, z, w
  int32_t spxOffs[3][4]; //!< Offsets for determining which vertice a dot is in
  int32_t spxPerms[4];   //!< Permutation table indices for x, y, z, w
  int32_t spxTab[512];   //!< A permutation table for simplex noise

  //  ---  methods  ---

  /* === Helper methods for random name generation === */
  PWX_INLINE void    PWX_INTERNAL checkRule  (int32_t &state, const char first, const char second, const char third);
  PWX_INLINE int32_t PWX_INTERNAL genSyllable(double &idx, double step, char * syll, int32_t &state,
      char * lastChrs)                                 PWX_WARNUNUSED;
  PWX_INLINE double  PWX_INTERNAL getStepping(double i, double x, double y, double z, double w,
      int32_t cl, int32_t sl, int32_t pl)              PWX_WARNUNUSED;

  /* === Helper methods for Simplex Noise === */
  PWX_INLINE double PWX_INTERNAL getSimpDot(int32_t index, double x);
  PWX_INLINE double PWX_INTERNAL getSimpDot(int32_t index, double x, double y);
  PWX_INLINE double PWX_INTERNAL getSimpDot(int32_t index, double x, double y, double z);
  PWX_INLINE double PWX_INTERNAL getSimpDot(int32_t index, double x, double y, double z, double w);
  /* Note: These are four functions, because using 1 with default values would cause alot of overhead with 0
           multiplication. Testing 10M Iterations with 2 dimensions was 500ms slower with such an "universal"
           getSimpDot() method. */
  PWX_INLINE double PWX_INTERNAL getSpx1D (double x);
  PWX_INLINE double PWX_INTERNAL getSpx2D (double x, double y);
  PWX_INLINE double PWX_INTERNAL getSpx3D (double x, double y, double z);
  PWX_INLINE double PWX_INTERNAL getSpx4D (double x, double y, double z, double w);

  // These are helpers to make the functions using raw noise more powerful when calculating with doubles
  PWX_INLINE double  PWX_INTERNAL noiseD   (double x)     const;
  PWX_INLINE double  PWX_INTERNAL noiseD   (double x, double y) const;
  PWX_INLINE double  PWX_INTERNAL noiseD   (double x, double y, double z) const;
  PWX_INLINE double  PWX_INTERNAL noiseD   (double x, double y, double z, double w) const;
  PWX_INLINE int32_t PWX_INTERNAL doubToInt(double val) const;
public:
  // ctor needs to be inlined, or the linker wreaks havoc on multiple inclusion of this header!
  /** @brief default ctor
    *
    * Initializes the random number generator and assigns a first random value
    * to lastRndValue. The seed and Simplex data are initialized as well.
  **/
  explicit CRandom():lastRndValue(0),seed(0)
  {
    uint32_t currTime = static_cast<uint32_t>(time(NULL));
    srand(currTime);
    lastRndValue  =  rand();
    setSeed(currTime);

    // Initialize Simplex values:
    for(int32_t i = 0; i < 5; i++)
      {
        spxCorn[i]    = 0.0;
        spxGrads[i]   = 0;
        for(int32_t j= 0; j < 4; j++)
          spxDist[i][j] = 0.0;

        if (i < 4)
          {
            spxNorms[i] = 0;
            spxPerms[i] = 0;
          }

        if (i < 3)
          {
            for(int32_t j= 0; j < 4; j++)
              spxOffs[i][j] = 0;
          }
      }
  }

  /** @brief default dtor
    *
    * Empty default dtor, nothing to be done.
    *
  **/
  ~CRandom()
  {

  }
};

} // namespace rng

#ifndef PWX_NO_RNG_INSTANCE
static rng::CRandom RNG; //!< Static instance of Random to be used
#endif // PWX_NO_RNG_INSTANCE

} // End of pwx namespace

/* --- Implementation: --- */
#include "pwxLib/internal/CRNGImpl.h"

#endif // PWX_RANDOM_H_INCLUDED
