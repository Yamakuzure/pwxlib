/** @file testRandom.cpp
  *
  * @brief console test program for CRandom.h
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
  * This little program test the CRandom in (hopefully) every aspect
  *
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.0.1     2008-04-09  sed, PrydeWorX  I daresay the first version was finished then...
  * 1.0.0	    2010-05-02  sed, PrydeWorX  Initial Release with documented header. Sorry, the original log is gone.
  * 1.0.1     2010-05-16  sed, PrydeWorX  Applied GPLv3 and full author information
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.0.1   2010-11-11  sed, PrydeWorX  changed to <cstdint> types for 32bit compatibility
  * 0.7.0.2   2010-11-15  sed, PrydeWorX  eventually got rid of printf
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5     2012-03-01  sed, PrydeWorX  Welcome to 2012. Version bump to new pwxLib release version
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

#include "pwxLib/internal/MSVCwarnPush.h"
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctime>
#include <limits>
#include "pwxLib/internal/MSVCwarnPop.h"
#include "pwxLib/CRandom.h"
#include "pwxLib/tools/StreamHelpers.h"

using namespace std;
using pwx::RNG;
using pwx::StreamHelpers::adjLeft;
using pwx::StreamHelpers::adjRight;

int main()
{
  int32_t i, j, k;
  clock_t     startTime     = 0;
  clock_t     endTime       = 0;
  int32_t     minInt        = 500;
  int32_t     maxInt        = 1000;
  int64_t     minLongInt    = 1000000000;
  int64_t     maxLongInt    = -500000000;
  float       minFloat      = -50.0;
  float       maxFloat      = 100.0;
  double      minDouble     = -150.0;
  double      maxDouble     = -250.0;
  long double minLongDouble = -0.0075;
  long double maxLongDouble = 0.00025;

  cout << "---------------------------" << endl;
  cout << "Integer Values      " << minInt << " -> " << maxInt << endl;
  for (i = 0; (i + 1) < 5; i++)
    cout << i << ".: " << adjRight(4,0) << RNG.random(minInt, maxInt) << endl;
  cout << "---------------------------" << endl;
  cout << "Long Integer Values " << minLongInt << " -> " << maxLongInt << endl;
  for (i = 0; (i + 1) < 5; i++)
    cout << i << ".: " << adjRight(10,0) << RNG.random(minLongInt, maxLongInt) << endl;
  cout << "---------------------------" << endl;
  cout << "Float Values        " << minFloat << " -> " << maxFloat << endl;
  for (i = 0; (i + 1) < 5; i++)
    cout << i << ".: " << adjRight(2,8) << RNG.random(minFloat, maxFloat) << endl;
  cout << "---------------------------" << endl;
  cout << "Double Values       " << minDouble << " -> " << maxDouble << endl;
  for (i = 0; (i + 1) < 5; i++)
    cout << i << ".: " << adjRight(3,8) << RNG.random(minDouble, maxDouble) << endl;
  cout << "---------------------------" << endl;
  cout << "Long Double Values  " << minLongDouble << " -> " << maxLongDouble << endl;
  for (i = 0; (i + 1) < 5; i++)
    cout << i << ".: " << adjRight(1,12) << RNG.random(minLongDouble, maxLongDouble) << endl;
  cout << "---------------------------" << endl;
  cout << "Hashing int32        -> " << maxInt << endl;
  for (i = 0; i < 5; i++)
    {
      cout << (i + 1) << ".: " << adjRight(20,0) << RNG.hash(maxInt + i);
      cout << " (Noise(i)     : " << adjRight(1,12) << RNG.noise(maxInt + i) << ")" << endl;
    }
  cout << "---------------------------" << endl;
  cout << "Hashing uint32       -> " << maxInt << endl;
  for (i = 0; i < 5; i++)
    {
      cout << (i + 1) << ".: " << adjRight(20,0) << RNG.hash(static_cast<uint32_t>(maxInt + i));
      cout << " (Noise(i,i)   : " << adjRight(1,12) << RNG.noise(maxInt + i, maxInt) << ")" << endl;
    }
  cout << "---------------------------" << endl;
  cout << "Hashing int64        -> " << maxInt << endl;
  for (i = 0; i < 5; i++)
    {
      cout << (i + 1) << ".: " << adjRight(20,0) << RNG.hash(static_cast<int64_t>(maxInt + i));
      cout << " (Noise(i,i,i) : " << adjRight(1,12) << RNG.noise(maxInt + i, maxInt, maxInt - i) << ")" << endl;
    }
  cout << "---------------------------" << endl;


  // For Random Names and Simplex Noise set a seed first:
  RNG.setSeed(21075); // My zip :)
  char * nameA = NULL; // To hold and free the result of rndName(i,false,*)
  char * nameB = NULL; // To hold and free the result of rndName(i, true,*)

  cout << "generating random names (single, short and long):" << endl;
  for (i = 1; i < 11; i++)
    {
      nameA = RNG.rndName(i);
      nameB = RNG.rndName(i, true);
      cout << adjRight(2,0) << i << ".: " << adjLeft(18,0) << nameA;
      cout << " - " << adjLeft(22,0) << nameB << endl;
      if (nameA) free (nameA);
      if (nameB) free (nameB);
    }
  cout << "---------------------------" << endl;
  cout << "generating random names (multi, short and long):" << endl;
  for (i = 1; i < 11; i++)
    {
      nameA = RNG.rndName(i, false, true);
      nameB = RNG.rndName(i, true, true);
      cout << adjRight(2,0) << i << ".: " << adjLeft(18,0) << nameA;
      cout << " - " << adjLeft(22,0) << nameB << endl;
      if (nameA) free (nameA);
      if (nameB) free (nameB);
    }
  cout << "---------------------------" << endl;

  // Test Simplex:
  cout << "Simplex 1D Noise     -> (1 - 9)" << endl;
  for (i = 0; i < 3; i++)
    {
      cout << (1 + (i * 3)) << ": " << adjRight(2, 12) << RNG.simplex1D(1 + (i * 3)) << " | ";
      cout << (2 + (i * 3)) << ": " << adjRight(2, 12) << RNG.simplex1D(2 + (i * 3)) << " | ";
      cout << (3 + (i * 3)) << ": " << adjRight(2, 12) << RNG.simplex1D(3 + (i * 3)) << endl;
    }

  cout << "Simplex 2D Noise     -> (0-2 x 0-2)" << endl;
  for (i = 0; i < 3; i++)
    {
      cout << (1 + (i * 3)) << ": " << adjRight(2, 12) << RNG.simplex2D(0, i) << " | ";
      cout << (2 + (i * 3)) << ": " << adjRight(2, 12) << RNG.simplex2D(1, i) << " | ";
      cout << (3 + (i * 3)) << ": " << adjRight(2, 12) << RNG.simplex2D(2, i) << endl;
    }

  cout << "Simplex 3D Noise     -> (1-2 x 1-2 x 0-1)" << endl;
  for (j = 0; j < 2; j++)
    for (i = 0; i < 3; i++)
      {
        cout << adjRight(2,0) << (1 + (i * 3) + (j * 9)) << ": " << adjRight(2, 12) << RNG.simplex3D(0, i, j) << " | ";
        cout << adjRight(2,0) << (2 + (i * 3) + (j * 9)) << ": " << adjRight(2, 12) << RNG.simplex3D(1, i, j) << " | ";
        cout << adjRight(2,0) << (3 + (i * 3) + (j * 9)) << ": " << adjRight(2, 12) << RNG.simplex3D(2, i, j) << endl;
      }

  cout << "Simplex 4D Noise     -> (1-2 x 1-2 x 0-1 x 0-1)" << endl;
  for (k = 0; k < 2; k++)
    for (j = 0; j < 2; j++)
      for (i = 0; i < 3; i++)
        {
          cout << adjRight(2,0) << (1 + (i * 3) + (j * 9) + (k * 18)) << ": " << adjRight(2, 12) << RNG.simplex4D(0, i, j, k) << " | ";
          cout << adjRight(2,0) << (2 + (i * 3) + (j * 9) + (k * 18)) << ": " << adjRight(2, 12) << RNG.simplex4D(1, i, j, k) << " | ";
          cout << adjRight(2,0) << (3 + (i * 3) + (j * 9) + (k * 18)) << ": " << adjRight(2, 12) << RNG.simplex4D(2, i, j, k) << endl;
        }

  // Speed Tests:
  double   dCur    = 0.0;
  uint32_t uiCur   = 0;
  uint32_t uminInt = numeric_limits<uint32_t>::max();
  uint32_t umaxInt = 0;
  uint64_t luiCur  = 0;
  uint64_t uminLongInt = numeric_limits<uint64_t>::max();
  uint64_t umaxLongInt = 0;
  minInt   = numeric_limits<int32_t>::max();
  maxInt   = numeric_limits<int32_t>::min();;

  cout << "---------------------------" << endl;
  cout << "Speed tests upon 10M operations each:" << endl;

  // Testing hashes
  cout << "Testing int32 hash...  ";
  startTime = clock();
  for (i = 1 ; i < 10000000 ; i++)
    {
      uiCur = RNG.hash(i);
      if (uiCur > umaxInt) umaxInt = uiCur;
      if (uiCur < uminInt) uminInt = uiCur;
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjRight(12,0) << uminInt << " | Max: " << adjRight(20,0) << umaxInt << endl;

  cout << "Testing uint32 hash... ";
  uminInt = numeric_limits<uint32_t>::max();
  umaxInt = 0;
  startTime = clock();
  for (i = 1 ; i < 10000000 ; i++)
    {
      uiCur = RNG.hash((uint32_t)i);
      if (uiCur > umaxInt) umaxInt = uiCur;
      if (uiCur < uminInt) uminInt = uiCur;
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjRight(12,0) << uminInt << " | Max: " << adjRight(20,0) << umaxInt << endl;

  cout << "Testing int64 hash...  ";
  startTime = clock();
  for (i = 1 ; i < 10000000 ; i++)
    {
      luiCur = RNG.hash(static_cast<int64_t>(i));
      if (luiCur > umaxLongInt) umaxLongInt = luiCur;
      if (luiCur < uminLongInt) uminLongInt = luiCur;
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjRight(12,0) << uminLongInt << " | Max: " << adjRight(20,0) << umaxLongInt << endl;

  cout << "Testing uint64 hash... ";
  uminInt = numeric_limits<uint32_t>::max();
  umaxInt = 0;
  startTime = clock();
  for (i = 1 ; i < 10000000 ; i++)
    {
      uiCur = RNG.hash(static_cast<uint64_t>(i));
      if (uiCur > umaxInt) umaxInt = uiCur;
      if (uiCur < uminInt) uminInt = uiCur;
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjRight(12,0) << uminInt << " | Max: " << adjRight(20,0) << umaxLongInt << endl;

  cout << "---------------------------" << endl;
  cout << "Speed tests upon 1M operations each:" << endl;

  // Testing simplex noises
  int32_t x, y, z, w;

  /* 1D Noise */
  /*----------*/
  /* Wave: 1  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 1D Noise, wave 1 ";
  startTime = clock();
  for (x = 0 ; x < 1000000 ; x++)
    {
      dCur = RNG.simplex1D(x);
      if (dCur > maxDouble) maxDouble = dCur;
      if (dCur < minDouble) minDouble = dCur;
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 2  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 1D Noise, wave 2 ";
  startTime = clock();
  for (x = 0 ; x < 1000000 ; x++)
    {
      dCur = RNG.simplex1D(x, 2.0, 1.0, 4.0, 2);
      if (dCur > maxDouble) maxDouble = dCur;
      if (dCur < minDouble) minDouble = dCur;
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 6  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 1D Noise, wave 6 ";
  startTime = clock();
  for (x = 0 ; x < 1000000 ; x++)
    {
      dCur = RNG.simplex1D(x, 2.0, 1.0, 4.0, 6);
      if (dCur > maxDouble) maxDouble = dCur;
      if (dCur < minDouble) minDouble = dCur;
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 8  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 1D Noise, wave 8 ";
  startTime = clock();
  for (x = 0 ; x < 1000000 ; x++)
    {
      dCur = RNG.simplex1D(x, 2.0, 1.0, 4.0, 8);
      if (dCur > maxDouble) maxDouble = dCur;
      if (dCur < minDouble) minDouble = dCur;
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));

  /* 2D Noise */
  /*----------*/
  /* Wave: 1  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 2D Noise, wave 1 ";
  startTime = clock();
  for (y = 1000; y < 2000; y++)
    {
      for (x = 2000 ; x < 3000 ; x++)
        {
          dCur = RNG.simplex2D(x, y);
          if (dCur > maxDouble) maxDouble = dCur;
          if (dCur < minDouble) minDouble = dCur;
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 2  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 2D Noise, wave 2 ";
  startTime = clock();
  for (y = 1000; y < 2000; y++)
    {
      for (x = 2000 ; x < 3000 ; x++)
        {
          dCur = RNG.simplex2D(x, y, 2.0, 1.0, 4.0, 2);
          if (dCur > maxDouble) maxDouble = dCur;
          if (dCur < minDouble) minDouble = dCur;
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 6  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 2D Noise, wave 6 ";
  startTime = clock();
  for (y = 1000; y < 2000; y++)
    {
      for (x = 2000 ; x < 3000 ; x++)
        {
          dCur = RNG.simplex2D(x, y, 2.0, 1.0, 4.0, 6);
          if (dCur > maxDouble) maxDouble = dCur;
          if (dCur < minDouble) minDouble = dCur;
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 8  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 2D Noise, wave 8 ";
  startTime = clock();
  for (y = 1000; y < 2000; y++)
    {
      for (x = 2000 ; x < 3000 ; x++)
        {
          dCur = RNG.simplex2D(x, y, 2.0, 1.0, 4.0, 8);
          if (dCur > maxDouble) maxDouble = dCur;
          if (dCur < minDouble) minDouble = dCur;
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));

  /* 3D Noise */
  /*----------*/
  /* Wave: 1  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 3D Noise, wave 1 ";
  startTime = clock();
  for (z = 1000; z < 1100; z++)
    {
      for (y = 2000; y < 2100; y++)
        {
          for (x = 3000 ; x < 3100 ; x++)
            {
              dCur = RNG.simplex3D(x, y, z);
              if (dCur > maxDouble) maxDouble = dCur;
              if (dCur < minDouble) minDouble = dCur;
            }
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 2  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 3D Noise, wave 2 ";
  startTime = clock();
  for (z = 1000; z < 1100; z++)
    {
      for (y = 2000; y < 2100; y++)
        {
          for (x = 3000 ; x < 3100 ; x++)
            {
              dCur = RNG.simplex3D(x, y, z, 2.0, 1.0, 4.0, 2);
              if (dCur > maxDouble) maxDouble = dCur;
              if (dCur < minDouble) minDouble = dCur;
            }
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 6  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 3D Noise, wave 6 ";
  startTime = clock();
  for (z = 1000; z < 1100; z++)
    {
      for (y = 2000; y < 2100; y++)
        {
          for (x = 3000 ; x < 3100 ; x++)
            {
              dCur = RNG.simplex3D(x, y, z, 2.0, 1.0, 4.0, 6);
              if (dCur > maxDouble) maxDouble = dCur;
              if (dCur < minDouble) minDouble = dCur;
            }
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 8  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 3D Noise, wave 8 ";
  startTime = clock();
  for (z = 1000; z < 1100; z++)
    {
      for (y = 2000; y < 2100; y++)
        {
          for (x = 3000 ; x < 3100 ; x++)
            {
              dCur = RNG.simplex3D(x, y, z, 2.0, 1.0, 4.0, 8);
              if (dCur > maxDouble) maxDouble = dCur;
              if (dCur < minDouble) minDouble = dCur;
            }
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));

  /* 4D Noise */
  /*----------*/
  /* Wave: 1  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 4D Noise, wave 1 ";
  startTime = clock();
  for (w = 1100; w < 1200; w++)
    {
      for (z = 2200; z < 2210; z++)
        {
          for (y = 5000; y < 5100; y++)
            {
              for (x = 7000 ; x < 7010 ; x++)
                {
                  dCur = RNG.simplex4D(x, y, z, w);
                  if (dCur > maxDouble) maxDouble = dCur;
                  if (dCur < minDouble) minDouble = dCur;
                }
            }
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 2  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 4D Noise, wave 2 ";
  startTime = clock();
  for (w = 1100; w < 1200; w++)
    {
      for (z = 2200; z < 2210; z++)
        {
          for (y = 5000; y < 5100; y++)
            {
              for (x = 7000 ; x < 7010 ; x++)
                {
                  dCur = RNG.simplex4D(x, y, z, w, 2.0, 1.0, 4.0, 2);
                  if (dCur > maxDouble) maxDouble = dCur;
                  if (dCur < minDouble) minDouble = dCur;
                }
            }
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 6  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 4D Noise, wave 6 ";
  startTime = clock();
  for (w = 1100; w < 1200; w++)
    {
      for (z = 2200; z < 2210; z++)
        {
          for (y = 5000; y < 5100; y++)
            {
              for (x = 7000 ; x < 7010 ; x++)
                {
                  dCur = RNG.simplex4D(x, y, z, w, 2.0, 1.0, 4.0, 6);
                  if (dCur > maxDouble) maxDouble = dCur;
                  if (dCur < minDouble) minDouble = dCur;
                }
            }
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));
  /* Wave: 8  */
  minDouble = 100000.0;
  maxDouble = -100000.0;
  cout << "Testing Simplex 4D Noise, wave 8 ";
  startTime = clock();
  for (w = 1100; w < 1200; w++)
    {
      for (z = 2200; z < 2210; z++)
        {
          for (y = 5000; y < 5100; y++)
            {
              for (x = 7000 ; x < 7010 ; x++)
                {
                  dCur = RNG.simplex4D(x, y, z, w, 2.0, 1.0, 4.0, 8);
                  if (dCur > maxDouble) maxDouble = dCur;
                  if (dCur < minDouble) minDouble = dCur;
                }
            }
        }
    }
  endTime = clock();
  cout << adjRight(5,0) << (1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC))) << " ms ";
  cout << "Min: " << adjLeft(2,4) << minDouble << " | Max: " << adjLeft(2,4) << maxDouble << endl;
//      printf("Factor: % 2.32f\n", 1.0 / ::std::max(abs(minDouble), maxDouble));

  return 0;
}
