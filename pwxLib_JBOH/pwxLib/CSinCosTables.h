#pragma once
#ifndef PWX_SINCOSTABLES_H_INCLUDED
#define PWX_SINCOSTABLES_H_INCLUDED 1

/** @file CSinCosTables.h
  * ---------------------
  *
  * @brief Declaration of CSinCosTables
  *
  * (c) 2007-2012 Sven Eden, PrydeWorX
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
  * 0.0.1     2008-02-12  sed, PrydeWorX  Initial Release
  * 0.1.0     2008-02-14  sed, PrydeWorX  changed to be a class rather than global arrays
  * 0.1.1     2008-02-20  sed, PrydeWorX  added sincos()
  * 0.2.0     2009-01-16  sed, PrydeWorX  added life calculation for PWX_SCTPREC == 0
  * 0.2.1     2009-06-09  sed, PrydeWorX  some optimizations and new forcing of inlining
  * 0.3.0     2009-10-08  sed, PrydeWorX  cleaned up and finally added methods for float and double
  * 0.3.0-r1  2010-04-15  sed, PrydeWorX  Some rework to have pwxLib comply as much as possible with Items 57
  *                                       to 59 in "C++ Coding Standards" (Herb Sutter, Andrei Alexandrescu, 2005)
  * 0.4.0     2010-04-30  sed, PrydeWorX  Changed (and rewrote) SCT to be flexible with precision (can be changed
  *                                       on the fly now) and switched to templated functions in pwx::sct::SCTHelpers
  * 0.5.0     2010-05-02  sed, PrydeWorX  With dynamic precision, the usage could finally be changed to a more intuitive
  *                                       way: precision now represents the number of digits to pre-calculate
  * 0.5.1     2010-05-02  sed, PrydeWorX  Finally fixed versioning and changelogs
  * 0.5.2     2010-05-16  sed, PrydeWorX  Applied GPLv3 and full author information
  * 0.5.2-r1  2010-05-17  sed, PrydeWorX  Added missing include for std::string
  * 0.5.3     2010-05-17  sed, PrydeWorX  Some minor changes to make inclusion in multiple object files easier
  *                                       without the linker throwing up
  * 0.6.2     2010-07-12  sed, PrydeWorX  Added PWX_NO_SCT_INSTANCE and version bump
  * 0.6.4     2010-11-03  sed, PrydeWorX  eventually removed all (bad) underscore prefixes from PWX_* defines and macros
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.0.1   2010-11-12  sed, PrydeWorX  Corrected issues with 32bit compilers.
  * 0.7.0.2   2011-01-06  sed, PrydeWorX  output can be changed on the fly, so iostream/StreamHelpers.h are no longer
  *                                       dependent on the output preprocessor macro.
  *                                       Corrected versioning.
  * 0.7.0.3   2011-01-23  sed, PrydeWorX  Changed pow() and round() to use the new gcc-msvc++-wrappers
  * 0.7.0.4   2011-03-06  sed, PrydeWorX  Welcome to 2011 !
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0.1   2011-09-20  sed, PrydeWorX  From now on CSinCosTables is derived from pwx::Lockable
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5     2012-03-01  sed, PrydeWorX  Welcome to 2012. Version bump to new pwxLib release version
  * 0.8.5.1   2012-03-22  sed, PrydeWorX  Fixed some Effective C++ issues
  * 0.8.5.2   2012-04-18  sed, PrydeWorX  Fixed some value size conversion issues with MSVC++10
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

// Initial customization point is the definition of the precision SCT starts with.

#if !defined(PWX_SCT_INITIALPRECISION)
//! Define the depth of the arrays we start with
#  define PWX_SCT_INITIALPRECISION 3
#endif // PWX_SCT_INITIALPRECISION

#ifdef PWX_SCT_WITH_OUTPUT
#  if (PWX_SCT_WITH_OUTPUT < 1)
#    undef  PWX_SCT_WITH_OUTPUT
#    define PWX_SCT_WITH_OUTPUT 0
#  endif // reset if not true
#else
#  define PWX_SCT_WITH_OUTPUT 0
#endif // PWX_SCT_WITH_OUTPUT

#include "pwxLib/internal/MSVCwarnPush.h"
#include <cmath>
#include <iostream>
#include "pwxLib/internal/MSVCwarnPop.h"
#include "tools/DefaultDefines.h"
#include "pwxLib/tools/MathHelpers.h"
#include "pwxLib/tools/StreamHelpers.h"

/** @namespace pwx
  *
  * @brief Namespace for everything PrydeWorX related
  *
  * All PrydeWorX projects reside in this namespace and it's sub-namespaces.
**/
namespace pwx
{

/** @namespace pwx::SCTHelpers
  *
  * @brief importing namespace for math helpers
**/
namespace SCTHelpers
{
  using namespace ::pwx::MathHelpers;
}

/** @namespace pwx::sct
  *
  * @brief Namespace for CSinCosTables.
  *
  * Everything concerning SCT is implemented here.
**/
namespace sct
{

/** @class CSinCosTables
  *
  * @brief Provides pre-calculated(*) sine and cosine tables
  *
  * Calculating sine and cosine values does not take much time
  * nowadays as FPUs get stronger every other day. On the other
  * hand, if you need these values for on-the-fly calculations
  * of something CPU-consuming like the display of 3D objects,
  * this little bit of CPU/FPU resources might better be saved.
  * Sin-/Cos-Tables with a precision of 3 use about 8MB RAM
  * (total), and this is not very much either.
  *
  * Tests show, that a Sin-/Cos-Tables with a precision of 3,
  * meaning 2 x 360,000 values in two arrays, do not differ from
  * on-the-fly calculation until the (worst case!) 6th digit.
  * (normally the first 7-8 digits are equal, which should be
  * enough for most applications. See the Test program
  * for the tests!)
  *
  * (*) If you set the precision to -1, there will be no
  * pre-defined tables, but all sine and cosine values calculated
  * on-the-fly. But you do not have to care about the range of
  * your angles, and do not need to transform angles to radiants.
  *
  * Usage:
  *
  * - Set PWX_SCT_INITIALPRECISION before including this file
  *   if you want to start with a different precision. The default
  *   is a precision of 3.
  *   You can, however, change the precision used later.
  *
  * - Set PWX_SCT_INITIALPRECISION to -1 if you prefer life
  *   calculations over pre-calculated tables from the beginning.
  *   You can switch to life calculation by using setPrecision(-1)
  *   later, of course
  *
  * - Normally the initialization is done silent. But if you need
  *   a console output, when using a higher initial precision for
  *   instance, you can predefine PWX_SCT_WITH_OUTPUT to be 1
  *   instead of the default 0.
  *   Please be aware that this means output if you change the
  *   precision later in a way that makes a re-initialization of
  *   the tables necessary.
  *   However, you can change whether text is printed or not with
  *   the help of the method setVerbose(true|false), and get the
  *   current value with isVerbose().
  *
  * - The file provides a static and initialized instance of the
  *   class, pwx::SCT - everything is done with it - unless you
  *   define PWX_NO_SCT_INSTANCE
  *
  * - If you define PWX_NO_SCT_INSTANCE prior including this header,
  *   you have to instantiate your own version from
  *   pwx::SCT::CSinCosTables
  *
  * - Use the following functions to get the values:
  *   sin() for sine, cos() for cosine, and sincos() to get both at
  *   once.
  *
  * - Use setPrecision() to set a new precision. resetPrecision()
  *   resets it to the initial value and getPrecision() returns
  *   the currently used value.
  *   Please be aware, however, that changing the precision means
  *   a recalculation of the sine and cosine arrays. Switching
  *   between -1 (life calculation) and the initial value
  *   does not trigger a re-initialization of the tables.
**/
class PWX_INTERNAL CSinCosTables: public ::pwx::Lockable, private Uncopyable
{
public:
  //  ---  functions  ---

  /** @brief return the currently used precision
  **/
  int32_t PWX_INLINE getPrecision  () const { return (currPrec); }
  /** @brief return true if SCT currently prints stats during re-initialization
  **/
  bool    PWX_INLINE isVerbose     () const { return (withOutput); }
  int32_t PWX_INLINE resetPrecision()                        PWX_WARNUNUSED;
  int32_t PWX_INLINE setPrecision  (const int32_t precision) PWX_WARNUNUSED;
  /** @brief set whether SCT shall print stats during re-intialization or not
  **/
  void    PWX_INLINE setVerbose    (bool aWithOutput) { withOutput = aWithOutput; }

  // --- templated functions ---
  template<typename T> PWX_INLINE double cos   (const T aDegree) const;
  template<typename T> PWX_INLINE double sin   (const T aDegree) const;
  template<typename T> PWX_INLINE void   sincos(const T aDegree, T &aSin, T &aCos) const;

private:
  //  ---  attributes  ---
  double      * CosTable;  //!< Internal array with precalculated cosine values
  int32_t       currPrec;  //!< the currently used precision
  const int32_t initPrec;  //!< The initial precision resetPrecision() resets to
  int32_t       lastPrec;  //!< last used precision
  double      * SinTable;  //!< Internal array with precalculated sine values
  int32_t       tableSize; //!< simply 360 * currPrec
  bool          withOutput;//!< Write statements if we (re-)initialize

public:
  // ctor/dtor need to be inlined directly for the linker to not wreak havoc on multiple inclusion
  /** @brief default ctor
    *
    * @param[in] aWithOutput When set to true, the ctor will print it's progress to stdout. Only really usefull for debugging.
  **/
  explicit CSinCosTables(bool aWithOutput = false):CosTable(NULL),currPrec(PWX_SCT_INITIALPRECISION),
    initPrec(PWX_SCT_INITIALPRECISION), lastPrec(PWX_SCT_INITIALPRECISION), SinTable(NULL),tableSize(0),
    withOutput(aWithOutput)
  {
    if (initPrec > -1)
      {
        int32_t i       = 0;
        double  radiant = 0.0;

        while ((lastPrec > -1) && (!CosTable || !SinTable))
          {
            tableSize = static_cast<int32_t>(pwx_pow(10, lastPrec)) * 360;
            // a) Try to allocate memory for the cosine table
            CosTable = static_cast<double *>(calloc(tableSize, sizeof(double)));
            if (CosTable)
              {
                // b) as we now have a cosine table, try to allocate memory for the sine table
                SinTable = static_cast<double *>(calloc(tableSize, sizeof(double)));
                if (!SinTable)
                  {
                    // c) The sine table allocation failed. Free the other, reduce lastPrec and retry
                    if (CosTable)
                      free (CosTable);
                    if (withOutput)
                      {
                        // But notify user if wanted:
                        ::std::cerr << "Failed to allocate " << (tableSize * sizeof(double));
                        ::std::cerr << " bytes for the sine table !" << ::std::endl;
                        ::std::cerr << " --> Reducing precision to " << (lastPrec - 1) << " !" << ::std::endl;
                      }
                    --lastPrec;
                  } // End check if sine table allocation succeeded
              } // End check if cosine table allocation succeeded
            else
              {
                // We failed, reduce lastPrec
                if (withOutput)
                  {
                    // But notify user if wanted:
                    ::std::cerr << "Failed to allocate " << (tableSize * sizeof(double));
                    ::std::cerr << " bytes for the cosine table !" << ::std::endl;
                    ::std::cerr << " --> Reducing precision to " << (lastPrec - 1) << " !" << ::std::endl;
                  }
                --lastPrec;
              }
          } // End allocation loop

        if (lastPrec > -1)
          {
            double  dPrec   = static_cast<double>(pwx_pow(10, lastPrec));
            while ( i < tableSize )
              {
                radiant  = SCTHelpers::degToRad(static_cast<double>(i) / dPrec);
                SinTable[i]  =  ::std::sin ( radiant );
                CosTable[i]  =  ::std::cos ( radiant );
                i++;
                if ( withOutput && ( i % ( tableSize / 10 ) == 0 ))
                  {
                    ::std::cout << " " << (i * 100 / tableSize) << "%";
                    ::std::cout.flush();
                  }
              }
          }
        else
          tableSize = 0;
      } // End initialization with precision > 0

    if (withOutput)
      {
        if (initPrec > 0)
          {
            ::std::cout << ::std::endl << "SinCosTables initialized!";

            int32_t size	  =	sizeof(double) * 2 * tableSize;
            double  mibSize	=	static_cast<double>(size) / 1024. / 1024.;
            ::std::cout << "Size of the Tables: " << size << " bytes (";
            ::std::cout << ::pwx::StreamHelpers::adjRight(0, 2) << mibSize << " MiB)" << ::std::endl << ::std::endl;
          }
        else
          ::std::cout << "SinCosTables omited, SCT configured for life calculations!";
        ::std::cout << ::std::endl;
      }
    // Finally we need to store the currently used precision:
    currPrec = lastPrec;
  }

  /** @brief default dtor
    *
    * deallocates the memory for sine and cosine tables if applicable
    *
  **/
  ~CSinCosTables()
  {
    if (CosTable) free (CosTable);
    if (SinTable) free (SinTable);
    CosTable = NULL;
    SinTable = NULL;
  }

private:
  /* --- no copying allowed --- */
  CSinCosTables(CSinCosTables&);
  CSinCosTables &operator=(CSinCosTables&);
};

/* --- Implementation: --- */
#include "pwxLib/internal/CSCTImpl.h"

} // End namespace sct

#ifndef PWX_NO_SCT_INSTANCE
static sct::CSinCosTables SCT(PWX_SCT_WITH_OUTPUT); //!< Static instance of CSinCosTables to be used
#endif // PWX_NO_SCT_INSTANCE

} // End of pwx namespace

#endif // PWX_SINCOSTABLES_H_INCLUDED
