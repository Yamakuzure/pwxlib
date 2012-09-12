#pragma once
#ifndef PWX_CWAVECOLOR_H_INCLUDED
#define PWX_CWAVECOLOR_H_INCLUDED 1

/** @file CWaveColor.h
  *
  * @brief Helper system to work with colors via wavelength versus RGB
  *
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
  * 0.0.1     2012-02-04  sed, PrydeWorX  First Design
  * 0.0.2     2012-02-07  sed, PrydeWorX  First working version
  * 0.0.3     2012-02-08  sed, PrydeWorX  Promoted from tool to library part, moving the file out of the tools folder.
  * 0.0.4     2012-02-14  sed, PrydeWorX  Fixed an issue with removing colors from a set
  * 0.0.5     2012-02-14  sed, PrydeWorX  Added methods to get and set frequencies and wavelengths directly
  * 0.8.5     2012-03-01  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5.1   2012-03-19  sed, PrydeWorX  Added two assertion tests to setWavelength() to check for a valid range
  * 0.8.5.2   2012-04-03  sed, PrydeWorX  Fixed a doxygen issue with color hex numbers
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

#include "pwxLib/internal/CWCexceptions.h"
#include "pwxLib/internal/sWave.h"

/** @namespace pwx
  *
  * @brief Namespace for everything PrydeWorX related
  *
  * All PrydeWorX projects reside in this namespace and it's sub-namespaces.
**/
namespace pwx
{
/** @namespace pwx::CWC
  *
  * @brief Namespace for the wavelength color helper class CWaveColor
  *
  * This namespace is the home of the CWaveColor class consisting of various methods
  * to help with the transformation between wavelength, frequency and RGB values. The
  * theory this class is based upon is, that all RGB colors can be described using a
  * small set of spectral colors, normally not more than two, and individual gamma
  * values for all wavelengths. The CWaveColor instance then can be used to do
  * frequency shifting, like applying a Doppler or gravitational effect, and mix the
  * resulting wavelength back into an RGB color.
**/
namespace CWC
{

/** @class CWaveColor
  * @brief Class to work with the frequencies any RGB color is made of.
  *
  * The main idea of this class is, to build an instance out of RGB
  * values. The resulting frequencies can be modified by various effects,
  * like Doppler or gravitation, and then mixed back into a resulting
  * RGB color on demand.
  *
  * Instead of using RGB colors an empty instance can be used and filled
  * with RGB colors or set to a specific wavelength or frequency.
  *
  * Important: Wavelengths are considered to be nanometers and frequencies
  * are considered to be gigahertz by the methods of this class.
**/
class CWaveColor
{
  /* --- members --- */
  double gamma; //!< General gamma value, applied to the resulting RGB value
  sWave* wave;  //!< Chain of wavelengths

public:
  /* --- ctor and dtor --- */

  /// @brief default empty ctor
  CWaveColor() : gamma(1.0), wave(NULL) { /* nothing to be done here */ }

  /** @brief Auto add RGB ctor
    *
    * This ctor automatically adds all spectral color wavelengths to represent
    * the given RGB values.
    *
    * If the creation of a wave length entry fails, this method throws a
    * pwx::WaveColor::cantCreateWaveEntry exception
    *
    * @param[in] r red part of the color
    * @param[in] g green part of the color
    * @param[in] b blue part of the color
    * @param[in] aGamma General gamma value, defaults to 1.0
  **/
  CWaveColor(uint8_t r, uint8_t g, uint8_t b, double aGamma = 1.0) : gamma(aGamma), wave(NULL)
    {
      PWX_TRY(setRGB(r, g, b))
      PWX_THROW_FURTHER
    }

  /// @brief copy ctor
  CWaveColor(CWaveColor &src) : gamma(1.0), wave(NULL)
    {
      operator=(src);
    }

  /// @brief default dtor
  ~CWaveColor()
    {
      if (wave)
        delete wave;
    }


  /* --- public explicit inline methods --- */

  PWX_INLINE size_t addRGB       (uint8_t r, uint8_t g, uint8_t b);
  PWX_INLINE size_t delRGB       (uint8_t r, uint8_t g, uint8_t b);
  PWX_INLINE size_t getRGB       (uint8_t &r, uint8_t &g, uint8_t &b);
  PWX_INLINE size_t setRGB       (uint8_t r, uint8_t g, uint8_t b);

  /* --- public implicit inline methods --- */

  /// @brief remove all wavelengths and reset global gamma value to 1.0
  void clear()
    {
      gamma = 1.0;
      if (wave)
        delete wave;
      wave = NULL;
    }

  /// @return get the number of spectral colors this color is made of
  size_t count() const
    {
      size_t cnt = 0;
      sWave* curr = wave;
      while (curr && ++cnt)
        curr = curr->next;
      return cnt;
    }

  /// @brief return the frequency of the wavelength with the given @a index or 0.0 if there is none with this index
  double getFrequency (size_t index) const { return 299792458.0 / getWavelength(index); }

  /// @brief return current global gamma value of this Color
  double getGamma() const { return gamma; }

  /// @brief return the wavelength with the given @a index or 0.0 if there is none with this index
  double getWavelength(size_t index) const
    {
      double result = 0.0;
      sWave* curr   = wave;
      size_t idx    = 0;

      // Find the right wavelength (if it is there)
      for (; curr && (index > idx); ++idx)
        curr = curr->next;

      // If the index is met, set result
      if (curr && (idx == index))
        result = curr->wavelength;

      return result;
    }

  /** @brief set the frequency of the wavelength with the given @a index to @a frequency
    *
    * If the creation of a wave length entry fails, this method throws a
    * pwx::WaveColor::cantCreateWaveEntry exception
    *
    * @param[in] index the frequency to change, if none with this index is found, a new one will be added.
    * @param[in] frequency the desired frequency in gigahertz.
  **/
  void setFrequency (size_t index, double frequency)
    {
      PWX_TRY(setWavelength(index, 299792458.0 / frequency))
      PWX_THROW_FURTHER
    }

  /** @brief set a new general gamma value
    *
    * Set a new gamma value (default is 1.0) that is applied to the resulting
    * RGB value the stored wavelengths represent.
    *
    * @param[in] aGamma the gamma value to apply
  **/
  void setGamma(double aGamma)
    {
      assert((aGamma >= 0.0) && "ERROR: CWaveColor::setGamma() must be called with a positive value for aGamma!");

      if (aGamma >= 0.0)
        gamma = aGamma;
    }

  /** @brief set the wavelength with the given @a index to @a wavelength
    *
    * If the creation of a wave length entry fails, this method throws a
    * pwx::WaveColor::cantCreateWaveEntry exception
    *
    * @param[in] index the wavelength to change, if none with this index is found, a new one will be added.
    * @param[in] wavelength the desired wavelength in nanometers.
  **/
  void setWavelength (size_t index, double wavelength)
    {
      sWave* curr   = wave;
      size_t idx    = 0;

      assert((wavelength >=380.0) && "ERROR: setWavelength() called with nm < 380.0 !");
      assert((wavelength <=780.0) && "ERROR: setWavelength() called with nm > 780.0 !");

      // If NDEBUG is defined, the following "normalization is to be done (do not cancel the call!)
      if (wavelength < 380.0) wavelength = 380.0;
      if (wavelength > 780.0) wavelength = 780.0;

      // Find the right wavelength (if it is there)
      for (; curr && (index > idx); ++idx)
        curr = curr->next;

      // If the index is met, set the wavelength
      if (curr && (idx == index))
        curr->wavelength = wavelength;
      // Otherwise add a new wavelength
      else
        {
          try
            {
              sWave* newWave = new sWave(curr, wavelength, 1.0);

              // If this is the first wave, record it
              if (NULL == wave)
                wave = newWave;
            }
          catch (::std::bad_alloc &e)
            {
              PWX_THROW(cantCreateWaveEntry, e.what(), "Error caught while creating new sWave entry")
            }
        } // End of adding a new wavelength
    }

  /// @return get the number of spectral colors this color is made of
  size_t size() const
    {
      return count();
    }

  /* --- public operators --- */

  /** @brief assignment operator
    *
    * If the creation of a wave length entry fails, this method throws a
    * pwx::WaveColor::cantCreateWaveEntry exception
  **/
  CWaveColor &operator=(CWaveColor &src)
    {
      if (&src != this)
        {
          uint8_t r, g, b;
          src.getRGB(r, g, b);
          gamma = src.gamma;
          PWX_TRY(setRGB(r, g, b))
          PWX_THROW_FURTHER
        }
    }

private:

  /* --- private explicit inline methods --- */

  template<typename T> PWX_INLINE uint8_t applyGamma     (T source);
  template<typename T> PWX_INLINE uint8_t applyGamma     (T source, double aGamma);
                       PWX_INLINE void    normalize      (sWave* tgt, uint8_t r, uint8_t g, uint8_t b);
                       PWX_INLINE double  rgbToWavelength(uint8_t r, uint8_t g, uint8_t b, double aGamma = 0.8);
  template<typename T> PWX_INLINE void    wavelengthToRGB(T nm, uint8_t &r, uint8_t &g, uint8_t &b, double aGamma = 0.8);
  template<typename T> PWX_INLINE uint8_t unapplyGamma   (T source);
  template<typename T> PWX_INLINE uint8_t unapplyGamma   (T source, double aGamma);

  /* --- private implicit inline methods --- */


};


} // End of namespace CWC

} // End of namespace pwx

// Add implementations:
#include "pwxLib/internal/CWaveColorImpl.h"


#endif // PWX_CWAVECOLOR_H_INCLUDED

