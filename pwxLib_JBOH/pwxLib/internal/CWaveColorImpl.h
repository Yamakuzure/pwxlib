#pragma once
#ifndef PWX_CWAVECOLORIMPL_H_INCLUDED
#define PWX_CWAVECOLORIMPL_H_INCLUDED 1

/** @internal
  * @file CWaveColorImpl.h
  *
  * @brief This file defines the methods of CWaveColor
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
  * History and Changelog maintained in CWaveColor.h.
**/

#ifndef PWX_CWAVECOLOR_H_INCLUDED
# error "Do not include CWaveColorImpl.h, include CWaveColor.h!"
#endif // Check for main file

namespace pwx {
namespace CWC {
// note: The namespaces are documented in tools/CWaveColor.h!

/* =========================================== */
/* === Implementations for CWaveColor      === */
/* =========================================== */

/*------------------------*/
/* --- public methods --- */
/*------------------------*/

/** @brief Add an RGB color to the current set of wavelengths
  *
  * This method is used to add color to the current set of wavelengths.
  * After splitting the given @a r, @a g and @a b values into
  * wavelengths the count of wavelengths added is returned.
  *
  * <B>Important</B>: If adding a number leads to a color part with
  * a value beyond the RGB limit of 0xff (255), the resulting color
  * is not clipped but all parts get scaled. Thus adding violet to
  * yellow will not result in white, but in a bright red. Or more
  * concrete: 0xe000e0 + 0xe0e000 = 0xff8080
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::WaveColor::cantCreateWaveEntry exception
  *
  * @param[in] r red part of the color
  * @param[in] g green part of the color
  * @param[in] b blue part of the color
  * @return the number of wavelengths this color consists of after the addition
**/
size_t CWaveColor::addRGB(uint8_t r, uint8_t g, uint8_t b)
  {
    size_t result = 0;

    // Step 1: As it is not possible to really add single wavelengths/gamma
    // values, we go by manipulating the current RGB set.
    uint8_t currR, currG, currB;
    getRGB(currR, currG, currB);

    // Step 2: Add up the color parts
    int32_t red   = static_cast<int32_t>(currR) + static_cast<int32_t>(r);
    int32_t green = static_cast<int32_t>(currG) + static_cast<int32_t>(g);
    int32_t blue  = static_cast<int32_t>(currB) + static_cast<int32_t>(b);

    // Step 3: If we get over 255, a modifier is needed
    double maxPart = ::std::max(::std::max(red, green), blue);
    if (maxPart > 255.0)
      {
        double maxMod = 255.0 / maxPart;
        red   = static_cast<int32_t>(pwx_round(maxMod * red));
        green = static_cast<int32_t>(pwx_round(maxMod * green));
        blue  = static_cast<int32_t>(pwx_round(maxMod * blue));
      }

    // Step 4: set current values to resulting legal colors (catch rounding errors)
    currR = red   < 0 ? 0 : red   > 255 ? 255 : static_cast<uint8_t>(red);
    currG = green < 0 ? 0 : green > 255 ? 255 : static_cast<uint8_t>(green);
    currB = blue  < 0 ? 0 : blue  > 255 ? 255 : static_cast<uint8_t>(blue);

    // Step 5: Re-add the resulting RGB set
    PWX_TRY(result = setRGB(currR, currG, currB))
    PWX_THROW_FURTHER

    return result;
  }


/** @brief Remove an RGB color from the current set of wavelengths
  *
  * This method is used to remove color from the current set of
  * wavelengths. After removing the given @a r, @a g and @a b values
  * from the existing wavelengths the count of the remaining
  * wavelengths is returned.
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::WaveColor::cantCreateWaveEntry exception
  *
  * @param[in] r red part of the color
  * @param[in] g green part of the color
  * @param[in] b blue part of the color
  * @return the number of wavelengths this color consists of after the removal
**/
size_t CWaveColor::delRGB(uint8_t r, uint8_t g, uint8_t b)
  {
    size_t result = 0;

    // Step 1: As it is not possible to really remove single wavelengths/gamma
    // values, we go by manipulating the current RGB set.
    uint8_t currR, currG, currB;
    getRGB(currR, currG, currB);

    // Step 2: Remove the given colors
    int32_t red   = static_cast<int32_t>(currR) - static_cast<int32_t>(r);
    int32_t green = static_cast<int32_t>(currG) - static_cast<int32_t>(g);
    int32_t blue  = static_cast<int32_t>(currB) - static_cast<int32_t>(b);

    // Step 3: set current values to resulting legal colors (clip)
    currR = red   < 0 ? 0 : red   > 255 ? 255 : static_cast<uint8_t>(red);
    currG = green < 0 ? 0 : green > 255 ? 255 : static_cast<uint8_t>(green);
    currB = blue  < 0 ? 0 : blue  > 255 ? 255 : static_cast<uint8_t>(blue);

    // Step 5: Re-add the resulting RGB set
    PWX_TRY(result = setRGB(currR, currG, currB))
    PWX_THROW_FURTHER

    return result;
  }


/** @brief Get an RGB color out of the current set of wavelengths
  *
  * This method is used to get the RGB color that the current set
  * of wavelengths defines.
  *
  * <B>Important</B>: If the current gamma leads to a color part with
  * a value beyond the RGB limit of 0xff (255), the resulting color
  * is not clipped but all parts get scaled. Thus raising the gamma
  * of a mixed color like sailor blue will not eventually lead to white,
  * but stay being Sailor blue. Or concrete: Raising gamma from 1.0 to
  * 3.0 for 0x5f9ea0 (Sailor Blue) results in 0x97fcff.
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::WaveColor::cantCreateWaveEntry exception
  *
  * @param[in] r red part of the color
  * @param[in] g green part of the color
  * @param[in] b blue part of the color
  * @return the number of wavelengths this color consists of after the addition
**/
size_t CWaveColor::getRGB(uint8_t &r, uint8_t &g, uint8_t &b)
  {
    uint8_t red    = 0, green = 0, blue = 0;
    int32_t sumR   = 0, sumG = 0, sumB = 0;
    size_t  result = 0;

    sWave* curr = wave;

    // Step one, walk through the waves and add up the colors they produce
    while (curr)
      {
        wavelengthToRGB(curr->wavelength, red, green, blue, curr->gamma);
        sumR += red;
        sumG += green;
        sumB += blue;
        ++result;
        curr = curr->next;
      }

    // Step 4: Apply global gamma
    sumR = static_cast<int32_t>(pwx_round(gamma * sumR));
    sumG = static_cast<int32_t>(pwx_round(gamma * sumG));
    sumB = static_cast<int32_t>(pwx_round(gamma * sumB));

    // Step 3: If we get over 255, a modifier is needed
    int32_t maxPart = ::std::max(::std::max(sumR, sumG), sumB);
    if (maxPart > 255)
      {
        double maxMod = 255.0 / static_cast<double>(maxPart);
        sumR = static_cast<int32_t>(pwx_round(maxMod * sumR));
        sumG = static_cast<int32_t>(pwx_round(maxMod * sumG));
        sumB = static_cast<int32_t>(pwx_round(maxMod * sumB));
      }

    // Step 4: Clip and return
    r = sumR > 255 ? 255 : sumR < 0 ? 0 : static_cast<uint8_t>(sumR);
    g = sumG > 255 ? 255 : sumG < 0 ? 0 : static_cast<uint8_t>(sumG);
    b = sumB > 255 ? 255 : sumB < 0 ? 0 : static_cast<uint8_t>(sumB);

    return result;
  }


/** @brief Set the set of wavelength to a specific RGB color
  *
  * This method is used to set the current set of wavelengths to a
  * specific color. After splitting the given @a r, @a g and @a b
  * values into wavelengths the count of wavelengths added is returned.
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::WaveColor::cantCreateWaveEntry exception
  *
  * @param[in] r red part of the color
  * @param[in] g green part of the color
  * @param[in] b blue part of the color
  * @return the number of wavelengths added
**/
size_t CWaveColor::setRGB(uint8_t r, uint8_t g, uint8_t b)
  {
    size_t result = 0;

    sWave* currWave = NULL;
    sWave* newWave  = NULL;

    // Preparation 1: Clear all waves
    clear();

    // Preparation 2: get the RGB values needed to result in the argument RGB with applied global gamma
    uint8_t red   = unapplyGamma(r);
    uint8_t green = unapplyGamma(g);
    uint8_t blue  = unapplyGamma(b);

    while (red || green || blue)
      {
        // Step one: Find wavelength and gamma value to add
        double  wavelength  = 0.0;
        double  wavegamma   = 0.0;
        if (red >= ::std::max(green, blue))
          {
            // Main Color Part is red
            wavegamma = static_cast<double>(red) / 255.0;
            if (green >= blue)
              // Secondary color is green
              wavelength = rgbToWavelength(red, green, 0, wavegamma);
            else
              /* Secondary color is blue
               * IMPORTANT: There is _NO_ wavelength representing a color with
               *            more red than blue! red can be larger than green,
               *            but when mixed with blue, red must not exceed the
               *            blue part!
              */
              wavelength = rgbToWavelength(::std::min(red, blue), 0, blue, wavegamma);
          }
        else if (green >= ::std::max(red, blue))
          {
            // Main Color Part is green
            wavegamma = static_cast<double>(green) / 255.0;
            if (red >= blue)
              // Secondary color is red
              wavelength = rgbToWavelength(red, green, 0, wavegamma);
            else
              // Secondary color is blue
              wavelength  = rgbToWavelength(0, green, blue, wavegamma);
          }
        else
          {
            // Main Color Part is blue
            wavegamma = static_cast<double>(blue) / 255.0;
            if (red >= green)
              // Secondary color is red
              wavelength = rgbToWavelength(red, 0, blue, wavegamma);
            else
              // Secondary color is green
              wavelength  = rgbToWavelength(0, green, blue, wavegamma);
          } // end of setting wavelength and gamma

        // Step 2: Add wavelength and gamma.
        try
          {
            newWave  = new sWave(currWave, wavelength, wavegamma);
            currWave = newWave;

            // If this is the first wave, record it
            if (NULL == wave)
              wave = currWave;

            // and count:
            ++result;
          }
        catch (::std::bad_alloc &e)
          {
            PWX_THROW(cantCreateWaveEntry, e.what(), "Error caught while creating new sWave entry")
          }

        // Step 3: normalize the generated wave and get finalRGB set
        normalize(currWave, red, green, blue);
        uint8_t currR, currG, currB;
        wavelengthToRGB(currWave->wavelength, currR, currG, currB, currWave->gamma);

        // Step 4: reduce the color parts by the values added
        red   -= currR < red   ? currR : red;
        green -= currG < green ? currG : green;
        blue  -= currB < blue  ? currB : blue;
      } // End of having colors left

    return result;
  }


/*-------------------------*/
/* --- private methods --- */
/*-------------------------*/

/// @brief apply the global gamma value to the given r, g or b value @a source
template<typename T>
uint8_t CWaveColor::applyGamma(T source)
  {
    return applyGamma(source, gamma);
  }

/// @brief apply the given gamma value @a aGamma to the given r, g or b value @a source
template<typename T>
uint8_t CWaveColor::applyGamma(T source, double aGamma)
  {
    double  xSource = static_cast<double>(source);
    int32_t interim = 0;
    uint8_t result  = 0;

    if ((xSource > 0.0) && (aGamma > 0.0))
      {
        interim = static_cast<int32_t>(pwx_round(xSource * aGamma));
        if (interim <   0) interim = 0;
        if (interim > 255) interim = 255;
        result = static_cast<uint8_t>(interim);
      }

    return result;
  }


/** @brief normalize a wave to a target RGB value
  *
  * This method will modify the gamma value of @a tgt until it reaches
  * a result of the given @a r, @a g and @a b value. Only two of the
  * three parts is relevant, as spectral colors always have at least
  * one zero part. The modification is done with the part of the other
  * two, where the distance to the arguments is smallest, thus there
  * won't be an over-brightening of the third.
  *
  * The normalization will stop if a) the target values are met, or b)
  * if the algorithm detects that it can't get any nearer.
  *
  * @param tgt pointer to the sWave instance to normalize
  * @param r red part of the color to normalize to
  * @param g green part of the color to normalize to
  * @param b blue part of the color to normalize to
**/
void CWaveColor::normalize(sWave* tgt, uint8_t r, uint8_t g, uint8_t b)
  {
    assert(tgt && "ERROR: Called CWaveColor::normalize() without a target!");

    if (tgt)
      {
        // First get the current set
        uint8_t red, green, blue;
        wavelengthToRGB(tgt->wavelength, red, green, blue, tgt->gamma);

        // Second determine which one or two colors are relevant
        bool hasRed   = red   ? true : false;
        bool hasGreen = green ? true : false;
        bool hasBlue  = blue  ? true : false;

        int32_t distRed   = hasRed   ? r - red   : 0;
        int32_t distGreen = hasGreen ? g - green : 0;
        int32_t distBlue  = hasBlue  ? b - blue  : 0;

        // Third determine which color part to handle actually
        if (  hasRed
           && (!hasGreen || (distGreen >= distRed))
           && (!hasBlue  || (distBlue  >= distRed)) )
          {
            // The Color to manage is red
            hasGreen = false;
            hasBlue  = false;
          }
        else if (  hasGreen
                && (!hasRed  || (distRed  >= distGreen))
                && (!hasBlue || (distBlue >= distGreen)) )
          {
            // The Color to manage is green
            hasRed  = false;
            hasBlue = false;
          }
        else
          {
            // The Color to manage is blue
            hasGreen = false;
            hasRed   = false;
          }

        // Fourth modify tgt->gamma until a) the color to manage
        // hits its target, or b) there is no progress.
        int32_t oldDist = hasRed ? distRed : hasGreen ? distGreen : distBlue;
        while (oldDist)
          {
            // 1: Modify gamma and get resulting RGB
            tgt->gamma *= hasRed   ? static_cast<double>(r) / static_cast<double>(red)
                        : hasGreen ? static_cast<double>(g) / static_cast<double>(green)
                        :            static_cast<double>(b) / static_cast<double>(blue);
            wavelengthToRGB(tgt->wavelength, red, green, blue, tgt->gamma);

            // 2: Determine actual distance
            int32_t newDist = hasRed ? r - red : hasGreen ? g - green : b - blue;

            // 3: Save or nullify oldDist:
            if (newDist != oldDist)
              // We have made progress, so continue (although we'd stop if newDist is zero)
              oldDist = newDist;
            else
              // No progress made, stop here
              oldDist = 0;
          } // End of having normalization to be done
      } // End of having a target
  }


/** @brief RGB To Wavelength
  *
  * This method returns the approximate raw wavelength in nanometer
  * according to @a r, @a g and @a b.
  *
  * <B>IMPORTANT</B>: If you plan to convert between RGB and frequency or
  * wavelength and vice versa to do frequency shifting, make sure to use
  * the same value for @a gamma in all calls, or you <i>will</i> end up with
  * garbage!
  *
  * @param[out] r the red part of the color
  * @param[out] g the green part of the color
  * @param[out] b the blue part of the color
  * @param[in] aGamma optional gamma value, defaults to 0.8
  * @return nm The wave length of the light in nanometers
**/
double CWaveColor::rgbToWavelength(uint8_t r, uint8_t g, uint8_t b, double aGamma)
  {
    double wavelength = 0.0;

    // Note: This is a reversal of wavelengthToRGB and thus nothing spectacular

    // Step 1: remove gamma value and 255.0 range:
    double red   = static_cast<double>(unapplyGamma(r, aGamma)) / 255.0;
    double green = static_cast<double>(unapplyGamma(g, aGamma)) / 255.0;
    double blue  = static_cast<double>(unapplyGamma(b, aGamma)) / 255.0;

    // Step 2: Consider wavelength range
    int32_t xRed   = static_cast<int32_t>(pwx_round(1000.0 * red));
    int32_t xGreen = static_cast<int32_t>(pwx_round(1000.0 * green));
    int32_t xBlue  = static_cast<int32_t>(pwx_round(1000.0 * blue));
    // Note: These three integers are solely used to determine the wavelength range,
    // it is needed to check == 0 which is a fishy thing to do with floating point.
    // Note 2: Ranges one and six are the ranges where a darkening factor is applied.
    // Range one might catch other ranges if checked first, so it is checked after
    // range five.
    if ((xRed == 0) && (xGreen <= xBlue) && (xBlue == 1000))
      {
        // This is range two, 440nm to 490 nm
        wavelength = 440. + (green * 50.);
      }
    else if ((xRed == 0) && (xGreen == 1000) && (xBlue <= xGreen))
      {
        // This is range three, 490nm to 510nm
        wavelength = 510. - (blue * 20.);
      }
    else if ((xRed <= xGreen) && (xGreen == 1000) && (xBlue == 0))
      {
        // This is range four, 510nm to 580nm
        wavelength = 510. + (red * 70.);
      }
    else if ((xRed == 1000) && (xGreen <= xRed) && (xBlue == 0))
      {
        // This is range five, 580nm to 650nm (<701nm)
        // Note: The range between 645nm and 700nm is always 1000/0/0,
        // so every color that is made out of a value within this
        // range will result in 645nm on reversal.
        wavelength = 645 - (green * 65.);
      }
    else if ((xRed <= xBlue) && (xGreen == 0) && (xBlue > 0))
      {
        // This is range one, 380nm to 440nm
        if (xBlue < 1000)
          // blue is always 1.0 in this range resulting in the applied factor!
          wavelength = 380. + (40. * (blue - 0.3) / 0.7);
        else
          // And here it is the red value counting
          wavelength = 440. - (red * 60.);
      }
    else if ((xRed > 0) && (xGreen == 0) && (xBlue == 0))
      {
        // This is range six, 701nm to 780nm
        // Here red is always 1.0 thus set to the factor itself
        wavelength = 780. - (80. * (red - 0.3) / 0.7);
      }

    // Step 3: return result
    return wavelength;
  }

/// @brief remove the global gamma value from the given r, g or b value @a source
template<typename T>
uint8_t CWaveColor::unapplyGamma(T source)
  {
    return unapplyGamma(source, gamma);
  }

/// @brief remove the global gamma value from the given r, g or b value @a source
template<typename T>
uint8_t CWaveColor::unapplyGamma(T source, double aGamma)
  {
    double xGamma = 1. / aGamma;
    double xSource = static_cast<double>(source);
    return applyGamma(xSource, xGamma);
  }

/** @brief Wavelength To RGB
  *
  * This method sets @a r, @a g and @a b to the approximate raw RGB values
  * according to @a nm.
  *
  * This method is based upon the work of Dan Bruton and his fortran code
  * to calculate RGB values out of wavelengths. See
  * http://www.physics.sfasu.edu/astro/color.html
  *
  * <B>IMPORTANT</B>: If you plan to convert between RGB and frequency or
  * wavelength and vice versa to do frequency shifting, make sure to use
  * the same value for @a gamma in all calls, or you <i>will</i> end up with
  * garbage!
  *
  * @param[in] nm The wave length of the light in nanometers
  * @param[out] r the red part of the resulting color
  * @param[out] g the green part of the resulting color
  * @param[out] b the blue part of the resulting color
  * @param[in] aGamma optional gamma value, defaults to 0.8
**/
template<typename T> void CWaveColor::wavelengthToRGB(T nm, uint8_t &r, uint8_t &g, uint8_t &b, double aGamma)
  {
    double wavelength = static_cast<double>(nm);
    double red = 0.0, green = 0.0, blue = 0.0, factor = 0.0;

    assert((wavelength >=380.0) && "ERROR: WavelengthToRGB called with nm < 380.0 !");
    assert((wavelength <=780.0) && "ERROR: WavelengthToRGB called with nm > 780.0 !");
    assert((aGamma > 0.) && "ERROR: WavelengthToRGB called with aGamma not > 0.0 !");

    /* Basically there areas of wavelength, in which the color parts
       follow different functions. */

    if ((wavelength >= 380.0) && (aGamma > 0.0))
      {
        /* Note: lower limit checked once, so the following can be single comparisons
         * against the upper limit, removing the need for a superfluous lower limit check
        */

        // Step 1: generate basic red, green and blue parts
        if (wavelength < 440.0)
          {
            red   = -(wavelength - 440.) / 60.;
            green = 0.0;
            blue  = 1.0;
          }
        else if (wavelength < 490.0)
          {
            red   = 0.0;
            green = (wavelength - 440.) / 50.;
            blue  = 1.0;
          }
        else if (wavelength < 510.0)
          {
            red   = 0.0;
            green = 1.0;
            blue  = -(wavelength - 510.) / 20.;
          }
        else if (wavelength < 580.0)
          {
            red   = (wavelength - 510.) / 70.;
            green = 1.0;
            blue  = 0.0;
          }
        else if (wavelength < 645.0)
          {
            red   = 1.0;
            green = -(wavelength - 645.) / 65.;
            blue  = 0.0;
          }
        else if (wavelength <= 780.0)
          {
            red   = 1.0;
            green = 0.0;
            blue  = 0.0;
          }
        // no else, wavelength above 780.0 do not result in a visible color

        // Step 2: Let the intensity fall off near the vision limits
        if (wavelength < 420.0)
          factor = 0.3 + (0.7 * (wavelength - 380.) / 40.);
        else if (wavelength < 701.0)
          factor = 1.0;
        else if (wavelength <= 780.0)
          factor = 0.3 + (0.7 * (780. - wavelength) / 80.);
        // no else, wavelength above 780.0 have a factor of 0.0
      } // End of wavelength being above the lower limit and gamma greater than zero

    // Step 3: Set r, g, b according to the calculated parts
    r = applyGamma(red   * 255.0 * factor, aGamma);
    g = applyGamma(green * 255.0 * factor, aGamma);
    b = applyGamma(blue  * 255.0 * factor, aGamma);
  }


} // End of namespace CWC
} // End of namespace pwx


#endif // PWX_CWAVECOLORIMPL_H_INCLUDED

