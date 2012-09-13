/** @file testSinCosTables.cpp
  *
  * @brief console test program for CSinCosTables.h
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
  * This little program test the pwxSinCosTables and does some comparison between
  * the precision of the tables versus calculating on-the-fly.
  *
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.1.0     2007-08-08  sed, PrydeWorX  Initial release
  * 0.1.1     2007-08-09  sed, PrydeWorX  Added optional output
  * 0.2.0     2007-08-10  sed, PrydeWorX  Added random test values
  * 0.2.0     2007-08-13  sed, PrydeWorX  Added try/catch blocks
  * 1.0.0     2007-08-13  sed, PrydeWorX  Initial Release after final testing
  * 1.0.1     2010-04-16  sed, PrydeWorX  Changed layout of the test program results (now better readable!)
  * 1.0.2     2010-05-02  sed, PrydeWorX  Upgraded the test program to make use of the new features.
  * 1.0.3     2010-05-16  sed, PrydeWorX  Applied GPLv3 and full author information
  * 1.0.4     2010-06-19  sed, PrydeWorX  Corrected output tables
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.0.1   2010-11-12  sed, PrydeWorX  Tweaked for 32bit compiler compatibility
  * 0.7.0.2   2010-11-16  sed, PrydeWorX  Eventually got rid of printf
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0.1   2011-08-01  sed, PrydeWorX  Extended test run 1 by other "regular" angles
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5     2012-03-01  sed, PrydeWorX  Welcome to 2012. Version bump to new pwxLib release version
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
  */

#include "pwxLib/internal/MSVCwarnPush.h"
#include <ctime>
#include "pwxLib/internal/MSVCwarnPop.h"

//! we need output here!
#define PWX_SCT_WITH_OUTPUT 1

#include "pwxLib/CSinCosTables.h"
#include "pwxLib/CRandom.h"
#include "pwxLib/tools/StreamHelpers.h"

// Although use/using declarations are something I avoid,
// this is a small test program, so I think they're okay.
using std::cout;
using std::endl;
using pwx::SCT;
using pwx::RNG;
using pwx::MathHelpers::degToRad;
using pwx::MathHelpers::getNormalizedDegree;
using pwx::StreamHelpers::adjLeft;
using pwx::StreamHelpers::adjRight;

// Three little piggies...
void makeSin(double degree, double &life, double &fromSCT, double &diff);
void makeCos(double degree, double &life, double &fromSCT, double &diff);
void printHeader();
void printLine();
void printValues(int32_t nr, double deg, double old, double valA, double valB, double diff, const char * type);

int main()
{
  int32_t speedNum = 10000000;
  int32_t i, precision;
  clock_t startTime = 0;
  clock_t endTime   = 0;
  int32_t timeP0, timeP2, timeP3, timePR;
  double testSine, testCoSine;
  double SCTsine, SCTcoSine;
  double minVal = 1.0L, maxVal = -1.0L; // Simple values to avoid optimizing out the speed tests
  double diff;			 	// for testVal[x] - sin/cosTable[x * 1000]
  double degree,oldDeg;// for the random number generator below
  cout << "Testing pwxSinCosTables" << endl;
  cout << "-----------------------" << endl;

  // Now calculate some on-the-fly Sine and Cosine values to compare with the tables
  i = 1;
  precision = SCT.getPrecision();
  cout << "Testing accuracy with default precision (" << precision << ") :" << endl;
  printHeader();
  /// === 90 degree sin and cos, as these both result in 1/0 ===
  for (degree = 0.0; degree < 360.0 ; degree += 45.0)
    {
      makeSin(degree, testSine, SCTsine, diff);
      printValues(i++, degree, degree, testSine, SCT.sin(degree), diff, "sin");
      makeCos(degree, testCoSine, SCTcoSine, diff);
      printValues(i++, degree, degree, testCoSine, SCT.cos(degree), diff, "cos");
    }

  oldDeg = degree;
  // third: 8 random degrees
  while (i < 16)
    {
      oldDeg      = RNG.random(-720.0L, 720.0L);
      degree      = getNormalizedDegree(oldDeg);
      // Note: This construct shall test the neutral version of getNormalizedDegree()
      makeSin(degree, testSine, SCTsine, diff);
      printValues(i++, degree, oldDeg, testSine, SCT.sin(degree), diff, "sin");
      makeCos(degree, testCoSine, SCTcoSine, diff);
      printValues(i++, degree, oldDeg, testCoSine, SCT.cos(degree), diff, "cos");
    }
  printLine();
  // Now measure time:
  cout << "\nTesting precision 3 speed with " << speedNum << " iterations ...";
  cout.flush();
  startTime = clock();
  for (i = 0; i < speedNum; ++i)
    {
      testSine = SCT.sin(RNG.random(359.99L));
      if (testSine < minVal) minVal = testSine;
      if (testSine > maxVal) maxVal = testSine;
    }
  endTime = clock();
  cout << " done (" << minVal << " - " << maxVal << ")" << endl;
  timeP3 = static_cast<int32_t>(pwx_round(
            (1000.0 *
              (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) ));


  // Next we switch to life calculation and do the same test:
  cout << endl << "-----------------------------------------------------------------------" << endl;
  cout << " Switching to life calculation:" << endl;
  int32_t newPrec = SCT.setPrecision(-1);
  assert(newPrec == -1);
  cout << "Testing accuracy with life calculation (" << newPrec << ") :" << endl;
  printHeader();
  i         = 1;
  while (i < 10)
    {
      oldDeg      = RNG.random(-720.0L, 720.0L);
      degree      = getNormalizedDegree(oldDeg);
      makeSin(degree, testSine, SCTsine, diff);
      printValues(i++, degree, oldDeg, testSine, SCT.sin(degree), diff, "sin");
      makeCos(degree, testCoSine, SCTcoSine, diff);
      printValues(i++, degree, oldDeg, testCoSine, SCT.cos(degree), diff, "cos");
    }
  printLine();
  // Now measure time:
  cout << "\nTesting precision 0 speed with " << speedNum << " iterations ...";
  cout.flush();
  startTime = clock();
  for (i = 0; i < speedNum; ++i)
    {
      testSine = SCT.sin(RNG.random(359.99L));
      if (testSine < minVal) minVal = testSine;
      if (testSine > maxVal) maxVal = testSine;
    }
  endTime = clock();
  cout << " done (" << minVal << " - " << maxVal << ")" << endl;
  timeP0 = static_cast<int32_t>(pwx_round(
            (1000.0 *
              (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) ));


  // Finally we re-initialize with rpecision 2 and do the test once more
  cout << endl << "-----------------------------------------------------------------------" << endl;
  cout << " Switching to precision " << (precision - 1) << ":" << endl;
  newPrec = SCT.setPrecision(precision -1);
  // No assert here, memory problems could occur, but will be handled by SCT!
  cout << "Testing accuracy with new precision (" << newPrec << ") :" << endl;
  printHeader();
  i         = 1;
  while (i < 10)
    {
      oldDeg      = RNG.random(-720.0L, 720.0L);
      degree      = getNormalizedDegree(oldDeg);
      makeSin(degree, testSine, SCTsine, diff);
      printValues(i++, degree, oldDeg, testSine, SCT.sin(degree), diff, "sin");
      makeCos(degree, testCoSine, SCTcoSine, diff);
      printValues(i++, degree, oldDeg, testCoSine, SCT.cos(degree), diff, "cos");
    }
  printLine();
  // Now measure time:
  cout << "\nTesting precision 2 speed with " << speedNum << " iterations ...";
  cout.flush();
  startTime = clock();
  for (i = 0; i < speedNum; ++i)
    {
      testSine = SCT.sin(RNG.random(359.99L));
      if (testSine < minVal) minVal = testSine;
      if (testSine > maxVal) maxVal = testSine;
    }
  endTime = clock();
  cout << " done (" << minVal << " - " << maxVal << ")" << endl;
  timeP2 = static_cast<int32_t>(pwx_round(
            (1000.0 *
              (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) ));


  // Special real function test:
  cout << "Testing std::sin speed with " << speedNum << " iterations ...";
  cout.flush();
  startTime = clock();
  for (i = 0; i < speedNum; ++i)
    {
      testSine = sin(degToRad(RNG.random(359.99L)));
      if (testSine < minVal) minVal = testSine;
      if (testSine > maxVal) maxVal = testSine;
    }
  endTime = clock();
  cout << " done (" << minVal << " - " << maxVal << ")" << endl;
  timePR = static_cast<int32_t>(pwx_round(
            (1000.0 *
              (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) ));


  // And get em out:
  cout << endl << "-----------------------------------------------------------------------" << endl;
  cout << "Precision 0 : " << timeP0 << " ms" << endl;
  cout << "Precision 2 : " << timeP2 << " ms" << endl;
  cout << "Precision 3 : " << timeP3 << " ms" << endl;
  cout << "std::sin    : " << timePR << " ms" << endl;
  cout << endl << "-----------------------------------------------------------------------" << endl;
  cout << "Result:" << endl;
  if (timeP0 < timePR)
    cout << "Strange... SCT with life calculation is faster than using std::sin directly..." << endl;
  if (timeP2 < timePR)
    cout << "You could use SCT with precision 2 to improve speed on your machine." << endl;
  if (timeP3 < timePR)
    cout << "Unbelievable... even with precision 3 SCT is faster than std::sin on your machine." << endl;
  if ( (timePR < timeP0)
       && (timePR < timeP2)
       && (timePR < timeP3) )
    cout << "SCT is too slow on your machine, you should use std::sin/cos directly!" << endl;

  return 0;
}

// Helper:
void makeSin(double degree, double &life, double &fromSCT, double &diff)
  {
    life    = sin(degToRad(degree));
    fromSCT = SCT.sin(degree);
    diff    = life - fromSCT;
  }

void makeCos(double degree, double &life, double &fromSCT, double &diff)
  {
    life    = cos(degToRad(degree));
    fromSCT = SCT.cos(degree);
    diff    = life - fromSCT;
  }


// Output functions:
void printHeader()
{
  cout << adjLeft(2) << "nr" << ".: ";
  cout << adjRight(6) << "Degree" << "/" << adjRight(7) << "old" << " | ";
  cout << adjRight(13) << "Direct Calc" << " | ";
  cout << adjRight(13) << "SinCosTable" << " | ";
  cout << adjRight(17) << "Difference" << " | ";
  cout << adjLeft(4) << "Type" << endl;
  printLine();
}

void printLine()
{
  cout << "--------------------+---------------+---------------+-------------------+-----" << endl;
}

void printValues(int32_t nr, double deg, double old, double valA, double valB, double diff, const char * type)
{
  cout << adjLeft(2) << nr << ".: ";
  cout << adjRight(3,2) << deg << "/" << adjRight(4,2) << old << " | ";
  cout << adjRight(2,10) << valA << " | ";
  cout << adjRight(2,10) << valB << " | ";
  cout << adjRight(2,14) << diff << " | ";
  cout << adjLeft(4) << type << endl;
}
