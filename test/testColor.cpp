/** @file testColor.cpp
  *
  * @brief console test program for CWaveColor.h
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
  * This little program tests the CWaveColor in (hopefully) every aspect
  *
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.0.1     2012-02-04  sed, PrydeWorX  First Design
  * 0.0.2     2012-02-08  sed, PrydeWorX  Extended test result display for better comparison call->result
  * 0.0.3     2012-02-09  sed, PrydeWorX  Added tests for mixing up colors
  * 0.0.4     2012-02-10  sed, PrydeWorX  Added tests for deleting colors
  * 0.0.5     2012-02-14  sed, PrydeWorX  Added tests for changing gamma, frequency and wavelength
  * 0.8.5     2012-03-01  sed, PrydeWorX  Welcome to 2012. Version bump to new pwxLib release version
  * 0.8.5.1   2012-04-18  sed, PrydeWorX  Got rid of (unsafe) sprintf calls.
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
  */

#include "pwxLib/internal/MSVCwarnPush.h"
#include <string>
#include <cstdio>
#include <iostream>
#include "pwxLib/internal/MSVCwarnPop.h"
#include <pwxLib/CWaveColor.h>

using namespace std;

void addRGB  (pwx::CWC::CWaveColor &wc, const char* const name, uint8_t r, uint8_t g, uint8_t b);
void delRGB  (pwx::CWC::CWaveColor &wc, const char* const name, uint8_t r, uint8_t g, uint8_t b);
void printRGB(pwx::CWC::CWaveColor &wc, const char* const name, const char* const call);
void printRGB(int32_t nr, pwx::CWC::CWaveColor &wc, const char* const name, const char* const call);
void setFreq (pwx::CWC::CWaveColor &wc, const char* const name, double modifier);
void setGamma(pwx::CWC::CWaveColor &wc, const char* const name, double gamma);
void setRGB  (int32_t nr, pwx::CWC::CWaveColor &wc, const char* const name, uint8_t r, uint8_t g, uint8_t b);
void setWave (pwx::CWC::CWaveColor &wc, const char* const name, double modifier);



int main()
{
  pwx::CWC::CWaveColor WC;

  /*****************************************
  * === Test 1: Try 10 standard colors === *
  *****************************************/
  cout << "Test 1: Set 10 standard colors\n";
  cout << "------------------------------" << endl;

  // 1: Black
  setRGB(1, WC, "Black", 0x0, 0x0, 0x0);

  // 2: Red
  setRGB(2, WC, "Red", 0xff, 0x0, 0x0);

  // 3: Green
  setRGB(3, WC, "Green", 0x0, 0xff, 0x0);

  // 4: Blue
  setRGB(4, WC, "Blue", 0x0, 0x0, 0xff);

  // 5: Yellow
  setRGB(5, WC, "Yellow", 0xff, 0xff, 0x0);

  // 6: Turquoise
  setRGB(6, WC, "Turquoise", 0x0, 0xff, 0xff);

  // 7: Violet
  setRGB(7, WC, "Violet", 0xff, 0x0, 0xff);

  // 8: Silver
  setRGB(8, WC, "Silver", 0xc0, 0xc0, 0xc0);

  // 9: Khaki 3
  setRGB(9, WC, "Khaki 3", 0xcd, 0xc6, 0x73);

  // 10: Sailor
  setRGB(10, WC, "Sailor Blue", 0x5f, 0x9e, 0xa0);

  /*****************************************
  * === Test 2: Mix up 5 colors        === *
  *****************************************/
  cout << "\nTest 2: Mix up 5 colors";
  cout << "\n-----------------------" << endl;

  // 1.: Red and Green
  setRGB(11, WC, "Mix red",   0xe0, 0x00, 0x00);
  addRGB(    WC, "and green", 0x00, 0xe0, 0x00);

  // 2.: Red and Blue
  setRGB(12, WC, "Mix red",  0xe0, 0x00, 0x00);
  addRGB(    WC, "and blue", 0x00, 0x00, 0xe0);

  // 3.: Violet and yellow
  setRGB(13, WC, "Mix violet", 0xe0, 0x00, 0xe0);
  addRGB(    WC, "and yellow", 0xe0, 0xe0, 0x00);

  // 4.: Khaki 3 and Blue
  setRGB(14, WC, "Mix khaki 3", 0xcd, 0xc6, 0x73);
  addRGB(    WC, "and blue",    0x00, 0x00, 0xe0);

  // 5.: Red and violet
  setRGB(15, WC, "Mix red",    0xe0, 0x00, 0x00);
  addRGB(    WC, "and violet", 0xff, 0x00, 0xff);

  /************************************************
  * === Test 3: Set 5 colors and remove parts === *
  ************************************************/
  cout << "\nTest 3: Set 5 colors and remove parts from them";
  cout << "\n-----------------------------------------------" << endl;

  // 16.: Silver and red
  setRGB(16, WC, "Set silver", 0xc0, 0xc0, 0xc0);
  delRGB(    WC, "remove red", 0xff, 0x00, 0x00);

  // 17.: Violet and Blue
  setRGB(17, WC, "Set violet",  0xff, 0x00, 0xff);
  delRGB(    WC, "remove blue", 0x00, 0x00, 0xff);

  // 18.: Turquoise and Green
  setRGB(18, WC, "Set turquoise", 0x00, 0xff, 0xff);
  delRGB(    WC, "remove green",  0x00, 0xff, 0x00);

  // 19.: Khaki 3 and Blue
  setRGB(19, WC, "Set khaki 3",   0xcd, 0xc6, 0x73);
  delRGB(    WC, "remove silver", 0xc0, 0xc0, 0xc0);

  // 20.: Sailor Blue and violet
  setRGB(20, WC, "Set sailor blue", 0x5f, 0x9e, 0xa0);
  delRGB(    WC, "remove violet",   0xff, 0x00, 0xff);

  /************************************************
  * === Test 4: Set 5 colors and change gamma === *
  ************************************************/
  cout << "\nTest 4: Set 5 colors and change they gamma value";
  cout << "\n------------------------------------------------" << endl;

  // 21.: Silver, raise to 1.5
  setRGB  (21, WC, "Set silver",  0xc0, 0xc0, 0xc0);
  setGamma(    WC, "Raise Gamma", 1.5);

  // 22.: Violet, raise to 2.5
  setRGB  (22, WC, "Set violet",  0xff, 0x00, 0xff);
  setGamma(    WC, "Raise Gamma", 2.5);

  // 23.: Turquoise, reduce to 0.8
  setRGB  (23, WC, "Set turquoise", 0x00, 0xff, 0xff);
  setGamma(    WC, "Lower Gamma",   0.8);

  // 24.: Khaki 3, reduce to 0.5
  setRGB  (24, WC, "Set khaki 3", 0xcd, 0xc6, 0x73);
  setGamma(    WC, "Lower Gamma", 0.5);

  // 25.: Sailor Blue, triple gamma
  setRGB  (25, WC, "Set sailor blue", 0x5f, 0x9e, 0xa0);
  setGamma(    WC, "Triple Gamma",    3.0);

  /***************************************************************
  * === Test 5: Set 5 colors and modify Frequency/Wavelength === *
  ***************************************************************/
  cout << "\nTest 5: Set 5 colors and modify\n        frequency / wavelength";
  cout << "\n-------------------------------" << endl;

  // 26.: Silver, wavelength * 0.9
  setRGB (26, WC, "Set silver",  0xc0, 0xc0, 0xc0);
  setWave(    WC, "Lower wave", 0.9);

  // 27.: Violet, wavelength * 1.1
  setRGB (27, WC, "Set violet",  0xff, 0x00, 0xff);
  setWave(    WC, "Raise wave", 1.1);

  // 28.: Turquoise, Frequency * 0.9
  setRGB (28, WC, "Set turquoise", 0x00, 0xff, 0xff);
  setFreq(    WC, "Lower freq",   0.8);

  // 29.: Khaki 3, Frequency * 1.1
  setRGB (29, WC, "Set khaki 3", 0xcd, 0xc6, 0x73);
  setFreq(    WC, "Raise freq", 1.1);

  // 30.: Sailor Blue, Raise both Frequency and wavelength
  setRGB (30, WC, "Set sailor blue", 0x5f, 0x9e, 0xa0);
  setFreq(    WC, "Raise freq", 1.15);
  setWave(    WC, "Raise wave", 1.15);


  /*** Tell the audience that we have finished ***/
  cout << "\nAll tests finished." << endl;

  return 0;
}

void addRGB(pwx::CWC::CWaveColor &wc, const char* const name, uint8_t r, uint8_t g, uint8_t b)
  {
    char call[9] = "";
    pwx_snprintf(call, 8, "%02x %02x %02x", r, g, b);
    wc.addRGB(r, g, b);
    printRGB(wc, name, call);
  }

void delRGB(pwx::CWC::CWaveColor &wc, const char* const name, uint8_t r, uint8_t g, uint8_t b)
  {
    char call[9] = "";
    pwx_snprintf(call, 8, "%02x %02x %02x", r, g, b);
    wc.delRGB(r, g, b);
    printRGB(wc, name, call);
  }

void printRGB(pwx::CWC::CWaveColor &wc, const char* const name, const char* const call)
  {
    uint8_t r, g, b;
    wc.getRGB(r, g, b);
    printf ("--> %-15s [Call %8s]: %2d waves -> RGB %02x %02x %02x\n",
            name, call, static_cast<int32_t>(wc.size()), r, g, b);
  }

void printRGB(int32_t nr, pwx::CWC::CWaveColor &wc, const char* const name, const char* const call)
  {
    uint8_t r, g, b;
    wc.getRGB(r, g, b);
    printf ("%02d. %-15s [Call %8s]: %2d waves -> RGB %02x %02x %02x\n",
            nr, name, call, static_cast<int32_t>(wc.size()), r, g, b);
  }

void setFreq(pwx::CWC::CWaveColor &wc, const char* const name, double modifier)
  {
    char call[9]  = "";
    char fTxt[32] = "";
    string modTxt = "--> ";

    pwx_snprintf(call, 8, "%2.3f", modifier);

    for (size_t i = 0; i < wc.size(); ++i)
      {
        if (i > 0)
          modTxt += ", ";
        pwx_snprintf(fTxt, 31, "%g", wc.getFrequency(i) / 1000.0);
        modTxt += fTxt;
        modTxt += "THz -> ";
        wc.setFrequency(i, modifier * wc.getFrequency(i));
        pwx_snprintf(fTxt, 31, "%g", wc.getFrequency(i) / 1000.0);
        modTxt += fTxt;
        modTxt += "THz";
      }
    printRGB(wc, name, call);
    cout << modTxt << endl;
  }

void setGamma(pwx::CWC::CWaveColor &wc, const char* const name, double gamma)
  {
    char call[9] = "";
    pwx_snprintf(call, 8, "%2.3f", gamma);
    wc.setGamma(gamma);
    printRGB(wc, name, call);
  }

void setRGB(int32_t nr, pwx::CWC::CWaveColor &wc, const char* const name, uint8_t r, uint8_t g, uint8_t b)
  {
    char call[9] = "";
    pwx_snprintf(call, 8, "%02x %02x %02x", r, g, b);
    wc.setRGB(r, g, b);
    printRGB(nr, wc, name, call);
  }

void setWave(pwx::CWC::CWaveColor &wc, const char* const name, double modifier)
  {
    char call[9] = "";
    char fTxt[32] = "";
    string modTxt = "--> ";

    pwx_snprintf(call, 8, "%2.3f", modifier);

    for (size_t i = 0; i < wc.size(); ++i)
      {
        if (i > 0)
          modTxt += ", ";
        pwx_snprintf(fTxt, 31, "%g", wc.getWavelength(i));
        modTxt += fTxt;
        modTxt += "nm -> ";
        wc.setWavelength(i, modifier * wc.getWavelength(i));
        pwx_snprintf(fTxt, 31, "%g", wc.getWavelength(i));
        modTxt += fTxt;
        modTxt += "nm";
      }
    printRGB(wc, name, call);
    cout << modTxt << endl;
  }

