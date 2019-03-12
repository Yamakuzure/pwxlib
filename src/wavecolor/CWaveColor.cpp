/**
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2018 PrydeWorX
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


#include "CWaveColor.h"


namespace pwx {


/* =========================================
 * ===    internal function prototypes   ===
 * =========================================
 */
template<typename T>
static uint8_t applyGamma     ( T source, double gamma_ ) noexcept;
static void    normalize      ( sWave& tgt, uint8_t r, uint8_t g, uint8_t b ) noexcept;
static double  rgbToWavelength( uint8_t r, uint8_t g, uint8_t b, double gamma_ ) noexcept;
static void    wavelengthToRGB( double wavelength, uint8_t& r, uint8_t& g, uint8_t& b, double gamma_ = 0.8 ) noexcept;
static uint8_t unapplyGamma   ( uint8_t source, double gamma_ ) noexcept;



/* ==========================================
 * ===         internal functions         ===
 * ==========================================
 */

/// @brief apply the given @a gamma_ value to the given r, g or b value @a source
template<typename T>
static uint8_t applyGamma ( T source, double gamma_ ) noexcept {
    double  xSource = static_cast<double>( source );
    int32_t interim = 0;
    uint8_t result  = 0;

    if ( ( xSource > 0.0 ) && ( gamma_ > 0.0 ) ) {
        interim = static_cast<int32_t>( std::round( xSource * gamma_ ) );
        if ( interim <   0 ) interim = 0;
        if ( interim > 255 ) interim = 255;
        result = static_cast<uint8_t>( interim );
    }

    return result;
}


/** @internal
  * @brief normalize a wave to a target RGB value.
  *
  * This method will modify the gamma value of @a tgt until it reaches
  * a result of the given @a r, @a g and @a b value. Only two of the
  * three parts are relevant, as spectral colors always have at least
  * one zero part. The modification is done with the part of the other
  * two, where the distance to the arguments is smallest, thus there
  * won't be an over-brightening of the third.
  *
  * The normalization will stop if a) the target values are met, or b)
  * if the algorithm detects that it can't get any nearer.
  *
  * @param tgt reference of the sWave instance to normalize.
  * @param r red part of the color to normalize to.
  * @param g green part of the color to normalize to.
  * @param b blue part of the color to normalize to.
**/
static void normalize( sWave& tgt, uint8_t r, uint8_t g, uint8_t b ) noexcept {
    // First get the current set
    uint8_t red, green, blue;
    wavelengthToRGB( tgt.wavelength, red, green, blue, tgt.gamma );

    // Second determine which one or two colors are relevant
    bool hasRed   = red   ? true : false;
    bool hasGreen = green ? true : false;
    bool hasBlue  = blue  ? true : false;

    int32_t distRed   = hasRed   ? r - red   : 0;
    int32_t distGreen = hasGreen ? g - green : 0;
    int32_t distBlue  = hasBlue  ? b - blue  : 0;

    // Third determine which color part to handle actually
    if (  hasRed
            && ( !hasGreen || ( distGreen >= distRed ) )
            && ( !hasBlue  || ( distBlue  >= distRed ) ) ) {
        // The Color to manage is red
        hasGreen = false;
        hasBlue  = false;
    } else if (  hasGreen
                 && ( !hasRed  || ( distRed  >= distGreen ) )
                 && ( !hasBlue || ( distBlue >= distGreen ) ) ) {
        // The Color to manage is green
        hasRed  = false;
        hasBlue = false;
    } else {
        // The Color to manage is blue
        hasGreen = false;
        hasRed   = false;
    }

    // Fourth modify tgt.gamma until a) the color to manage
    // hits its target, or b) there is no progress.
    int32_t oldDist = hasRed ? distRed : hasGreen ? distGreen : distBlue;
    while ( oldDist ) {
        // 1: Modify gamma and get resulting RGB
        tgt.gamma *=  hasRed   ? static_cast<double>( r ) / static_cast<double>( red )
                      : hasGreen ? static_cast<double>( g ) / static_cast<double>( green )
                      :            static_cast<double>( b ) / static_cast<double>( blue );
        wavelengthToRGB( tgt.wavelength, red, green, blue, tgt.gamma );

        // 2: Determine actual distance
        int32_t newDist = hasRed ? r - red : hasGreen ? g - green : b - blue;

        // 3: Save or nullify oldDist:
        if ( newDist != oldDist )
            // Progress has been made, so continue (although we'd stop if newDist is zero)
            oldDist = newDist;
        else
            // No progress made, stop here
            oldDist = 0;
    } // End of having normalization to be done
}


/** @internal
  * @brief RGB To Wavelength
  *
  * This method returns the approximate raw wavelength in nanometer
  * according to @a r, @a g and @a b.
  *
  * <B>IMPORTANT</B>: If you plan to convert between RGB and frequency or
  * wavelength and vice versa to do frequency shifting, make sure to use
  * the same value for @a gamma_ in all calls, or you <i>will</i> end up with
  * garbage!
  *
  * @param[out] r the red part of the color.
  * @param[out] g the green part of the color.
  * @param[out] b the blue part of the color.
  * @param[in] gamma_ optional gamma value, defaults to 0.8.
  * @return nm The wave length of the light in nanometers.
**/
static double rgbToWavelength( uint8_t r, uint8_t g, uint8_t b, double gamma_ ) noexcept {
    double wavelength = 0.0;

    // Note: This is a reversal of wavelengthToRGB and thus nothing spectacular

    // Step 1: remove gamma value and 255.0 range:
    double red   = static_cast<double>( unapplyGamma( r, gamma_ ) ) / 255.0;
    double green = static_cast<double>( unapplyGamma( g, gamma_ ) ) / 255.0;
    double blue  = static_cast<double>( unapplyGamma( b, gamma_ ) ) / 255.0;

    // Note: Ranges one and six are the ranges where a darkening factor is applied.
    //       Range one might catch other ranges if checked first, so it is checked
    //       after range five.
    if ( areAlmostEqual( red, 0. ) && ( green <= blue ) && areAlmostEqual( blue, 1. ) )
        // This is range two, 440nm to 490 nm
        wavelength = 440. + ( green * 50. );
    else if ( areAlmostEqual( red, 0. ) && areAlmostEqual( green, 1. ) && ( blue <= green ) )
        // This is range three, 490nm to 510nm
        wavelength = 510. - ( blue * 20. );
    else if ( ( red <= green ) && areAlmostEqual( green, 1. ) && areAlmostEqual( blue, 0. ) )
        // This is range four, 510nm to 580nm
        wavelength = 510. + ( red * 70. );
    else if ( areAlmostEqual( red, 1. ) && ( green <= red ) && areAlmostEqual( blue, 0. ) )
        // This is range five, 580nm to 650nm (<701nm)
        // Note: The range between 645nm and 700nm is always 1000/0/0,
        // so every color that is made out of a value within this
        // range will result in 645nm on reversal.
        wavelength = 645 - ( green * 65. );
    else if ( ( red <= blue ) && areAlmostEqual( green, 0. ) && ( blue > 0. ) ) {
        // This is range one, 380nm to 440nm
        if ( blue < 1. )
            // blue is always 1.0 in this range resulting in the applied factor!
            wavelength = 380. + ( 40. * ( blue - 0.3 ) / 0.7 );
        else
            // And here it is the red value counting
            wavelength = 440. - ( red * 60. );
    } else if ( ( red > 0. ) && areAlmostEqual( green, 0. ) && areAlmostEqual( blue, 0. ) )
        // This is range six, 701nm to 780nm
        // Here red is always 1.0 thus set to the factor itself
        wavelength = 780. - ( 80. * ( red - 0.3 ) / 0.7 );

    return wavelength;
}


/** @brief Wavelength To RGB
  *
  * This method sets @a r, @a g and @a b to the approximate raw RGB values
  * according to @a wavelength.
  *
  * This method is based upon the work of Dan Bruton and his fortran code
  * to calculate RGB values out of wavelengths. See
  * http://www.physics.sfasu.edu/astro/color.html
  *
  * <B>IMPORTANT</B>: If you plan to convert between RGB and frequency or
  * wavelength and vice versa to do frequency shifting, make sure to use
  * the same value for @a gamma_ in all calls, or you <i>will</i> end up with
  * garbage!
  *
  * <I>Note</I>: Only a @a wavelength between 380nm and 780nm with a @a gamma_
  * of more than 0.0 produces a visible color.
  *
  * <I>Important</I>: If a @a gamma "overbrightens" any color, all three get
  * scaled.
  *
  * @param[in] wavelength The wave length of the light in nanometers.
  * @param[out] r the red part of the resulting color.
  * @param[out] g the green part of the resulting color.
  * @param[out] b the blue part of the resulting color.
  * @param[in] gamma_ optional gamma value, defaults to 0.8.
**/
static void wavelengthToRGB( double wavelength, uint8_t& r, uint8_t& g, uint8_t& b, double gamma_ ) noexcept {
    double red = 0.0, green = 0.0, blue = 0.0, factor = 0.0;

    if ( ( wavelength >=380.0 ) && ( wavelength <=780.0 ) && ( gamma_ > 0. ) ) {
        // Only theese ranges produce visible light.

        /* Basically there areas of wavelength, in which the color parts
        follow different functions. */

        // Step 1: generate basic red, green and blue parts
        if ( wavelength < 440.0 ) {
            red   = -( wavelength - 440. ) / 60.;
            green = 0.0;
            blue  = 1.0;
        } else if ( wavelength < 490.0 ) {
            red   = 0.0;
            green = ( wavelength - 440. ) / 50.;
            blue  = 1.0;
        } else if ( wavelength < 510.0 ) {
            red   = 0.0;
            green = 1.0;
            blue  = -( wavelength - 510. ) / 20.;
        } else if ( wavelength < 580.0 ) {
            red   = ( wavelength - 510. ) / 70.;
            green = 1.0;
            blue  = 0.0;
        } else if ( wavelength < 645.0 ) {
            red   = 1.0;
            green = -( wavelength - 645. ) / 65.;
            blue  = 0.0;
        } else {
            red   = 1.0;
            green = 0.0;
            blue  = 0.0;
        }

        // Step 2: Let the intensity fall off near the vision limits
        if ( wavelength < 420.0 )
            factor = 0.3 + ( 0.7 * ( wavelength - 380. ) / 40. );
        else if ( wavelength < 645.0 )
            factor = 1.0;
        else
            factor = 0.3 + ( 0.7 * ( 780. - wavelength ) / 135. );

    } // End of having a wavelength and gamma suitable for visible light

    // Step 3: If we get over 255, a modifier is needed
    red   *= 255.0 * factor * gamma_;
    green *= 255.0 * factor * gamma_;
    blue  *= 255.0 * factor * gamma_;
    double maxPart = std::max( std::max( red, green ), blue );
    if ( maxPart > 255.0 ) {
        double maxMod = 255.0 / maxPart;
        red   = std::round( maxMod * red );
        green = std::round( maxMod * green );
        blue  = std::round( maxMod * blue );
    }

    // Step 4: Set r, g, b according to the calculated parts
    r = red   < 0.0 ? 0 : red   > 255.0 ? 255 : static_cast<uint8_t>( red );
    g = green < 0.0 ? 0 : green > 255.0 ? 255 : static_cast<uint8_t>( green );
    b = blue  < 0.0 ? 0 : blue  > 255.0 ? 255 : static_cast<uint8_t>( blue );
}


/// @brief remove the @a gamma_ value from the given r, g or b value @a source
static uint8_t unapplyGamma ( uint8_t source, double gamma_ ) noexcept {
    double xGamma = 1. / ( gamma_< 0.0001 ? 0.0001 : gamma_ );
    return applyGamma( source, xGamma );
}


/* ======================================================================
 * === Method implementations of sWave                                ===
 * ======================================================================
 */

/** @brief default constructor
  *
  * @param[in] wavelength_ The wavelength to use.
  * @param[in] gamma_ The gamma to use.
**/
sWave::sWave( double wavelength_, double gamma_ ) noexcept :
    gamma( gamma_ ),
    wavelength( wavelength_ )
{ }


/// @brief copy constructor
sWave::sWave( const sWave& src ) noexcept :
    gamma( src.gamma ),
    wavelength( src.wavelength )
{ }


/// @brief assignement operator
sWave& sWave::operator=( const sWave& src ) noexcept {
    if ( &src != this ) {
        gamma = src.gamma;
        wavelength = src.wavelength;
    }
    return *this;
}


/// @brief Return true if this and @a rhs are equal
bool sWave::operator==( const sWave& rhs ) const noexcept {
    if ( &rhs != this )
        return areAlmostEqual( this->wavelength, rhs.wavelength )
               && areAlmostEqual( this->gamma,      rhs.gamma );
    return true;
}


/// @brief Retrun true if thes is larger than @a rhs
bool sWave::operator> ( const sWave& rhs ) const noexcept {
    if ( &rhs != this )
        return ( this->wavelength * this->gamma ) > ( rhs.wavelength * rhs.gamma );
    return false;
}


/* ======================================================================
 * === Method implementations of CWaveColor                           ===
 * ======================================================================
 */

/** @brief Auto add RGB constructor
  *
  * This ctor automatically adds all spectral color wavelengths to represent
  * the given RGB values.
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::CException with the name "ElementCreationFailed".
  *
  * @param[in] r red part of the color.
  * @param[in] g green part of the color.
  * @param[in] b blue part of the color.
  * @param[in] gamma_ General gamma value, defaults to 1.0.
**/
CWaveColor::CWaveColor( uint8_t r, uint8_t g, uint8_t b, double gamma_ ) :
    gamma( gamma_ ) {
    PWX_TRY_PWX_FURTHER( setRGB( r, g, b ) )
}


/** @brief empty constructor
  *
  * add Wavelength later...
**/
CWaveColor::CWaveColor() noexcept :
    gamma( 1.0 ),
    waves( {} )
{  }


/** @brief Copy constructor
  *
  * The full data of @a src is copied.
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::CException with the name "ElementCreationFailed".
  *
  * @param[in] src The CWaveColor instance to copy.
  */
CWaveColor::CWaveColor( const CWaveColor& src ) :
    base_t( src ),
    gamma( src.gamma ) {
    PWX_TRY_PWX_FURTHER( waves = src.waves )
}


/// @brief default dtor
CWaveColor::~CWaveColor() noexcept
{ /* nothing to do here */ }


/** @brief Add an RGB color to the current set of wavelengths.
  *
  * This method is used to add color to the current set of wavelengths.
  * After splitting the given @a r, @a g and @a b values into
  * wavelengths the count of wavelengths added is returned.
  *
  * <B>Important</B>: If adding a number leads to a color part with
  * a value beyond the RGB limit of 0xff (255), the resulting color
  * is not clipped but all parts get scaled. Thus adding violet to
  * yellow will not result in white, but in a bright red. Or more
  * concrete: 0xe000e0 + 0xe0e000 = 0xff8080.
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::CException with the name "ElementCreationFailed".
  *
  * @param[in] r red part of the color.
  * @param[in] g green part of the color.
  * @param[in] b blue part of the color.
  * @return the number of wavelengths this color consists of after the addition.
**/
uint32_t CWaveColor::addRGB( uint8_t r, uint8_t g, uint8_t b ) {
    uint32_t result = 0;

    // Step 1: As it is not possible to really add single wavelengths/gamma
    // values, we go by manipulating the current RGB set.
    uint8_t currR, currG, currB;
    getRGB( currR, currG, currB );

    // Step 2: Add up the color parts
    float red   = static_cast<float>( currR ) + static_cast<float>( r );
    float green = static_cast<float>( currG ) + static_cast<float>( g );
    float blue  = static_cast<float>( currB ) + static_cast<float>( b );

    // Step 3: If we get over 255, a modifier is needed
    float maxPart = std::max( std::max( red, green ), blue );
    if ( maxPart > 255.f ) {
        float maxMod = 255.f / maxPart;
        red   = std::round( maxMod * red );
        green = std::round( maxMod * green );
        blue  = std::round( maxMod * blue );
    }

    // Step 4: set current values to resulting legal colors (catch rounding errors)
    currR = red   < 0.f ? 0 : red   > 255.f ? 255 : static_cast<uint8_t>( red );
    currG = green < 0.f ? 0 : green > 255.f ? 255 : static_cast<uint8_t>( green );
    currB = blue  < 0.f ? 0 : blue  > 255.f ? 255 : static_cast<uint8_t>( blue );

    // Step 5: Re-add the resulting RGB set
    PWX_TRY_PWX_FURTHER( result = setRGB( currR, currG, currB ) )

    return result;
}


/// @brief remove all wavelengths and reset global gamma value to 1.0.
void CWaveColor::clear() noexcept {
    PWX_LOCK_GUARD( this )
    gamma = 1.0;
    waves.clear();
}


/** @brief get the number of spectral colors this color is made of.
  * @return the number of spectral colors used.
**/
uint32_t CWaveColor::count() const noexcept {
    return waves.size();
}


/** @brief Remove an RGB color from the current set of wavelengths.
  *
  * This method is used to remove color from the current set of
  * wavelengths. After removing the given @a r, @a g and @a b values
  * from the existing wavelengths the count of the remaining
  * wavelengths is returned.
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::CException with the name "ElementCreationFailed".
  *
  * @param[in] r red part of the color.
  * @param[in] g green part of the color.
  * @param[in] b blue part of the color.
  * @return the number of wavelengths this color consists of after the removal.
**/
uint32_t CWaveColor::delRGB( uint8_t r, uint8_t g, uint8_t b ) {
    uint32_t result = 0;

    // Step 1: As it is not possible to really remove single wavelengths/gamma
    // values, we go by manipulating the current RGB set.
    uint8_t currR, currG, currB;
    getRGB( currR, currG, currB );

    // Step 2: Remove the given colors
    float red   = static_cast<float>( currR ) - static_cast<float>( r );
    float green = static_cast<float>( currG ) - static_cast<float>( g );
    float blue  = static_cast<float>( currB ) - static_cast<float>( b );

    // Step 3: set current values to resulting legal colors (clip)
    currR = red   < 0.f ? 0 : red   > 255.f ? 255 : static_cast<uint8_t>( red );
    currG = green < 0.f ? 0 : green > 255.f ? 255 : static_cast<uint8_t>( green );
    currB = blue  < 0.f ? 0 : blue  > 255.f ? 255 : static_cast<uint8_t>( blue );

    // Step 5: Re-add the resulting RGB set
    PWX_TRY_PWX_FURTHER( result = setRGB( currR, currG, currB ) )

    return result;
}


/** @brief apply a doppler effect according to movement relative to position
  *
  * This method applies a simple doppler effect to all stored
  * wavelengths of this color.
  *
  * Calculated is the movement of the object (@a objX, @a objY, @a objZ)
  * relative to the camera (@a camX, @a camY, @a camZ) using the objects
  * absolute movement (@a movX, @a movY, @a movZ).
  *
  * <I>Important</I>: This can shift a perfectly visible color out of
  * the visible range if the movement is extreme enough!
  *
  * <I>Even more important</I>: It does <B>not</B> matter in which units
  * the paramters for the camera and object positions are, as long as they
  * fit together. The movement parameters are always assumed to be in m/s.
  *
  * Please note that a negative movement vector is considered to be a
  * movement towards the camera, while a positive movement vector is
  * considered to be a movement away from the camera.
  *
  * @param[in] camX the X-Coordinate of the camera
  * @param[in] camY the Y-Coordinate of the camera
  * @param[in] camZ the Z-Coordinate of the camera
  * @param[in] objX the X-Coordinate of the colored object
  * @param[in] objY the Y-Coordinate of the colored object
  * @param[in] objZ the Z-Coordinate of the colored object
  * @param[in] movX the absolute X-movement of the colored object in m/s
  * @param[in] movY the absolute Y-movement of the colored object in m/s
  * @param[in] movZ the absolute Z-movement of the colored object in m/s
**/
void CWaveColor::doppler( double camX, double camY, double camZ,
                          double objX, double objY, double objZ,
                          double movX, double movY, double movZ ) noexcept {
    // Return at once if there is nothing to do:
    if ( waves.empty() )
        return;

    // First the relative movement vector must be determined:
    double dist   = absDistance( camX, camY, camZ, objX, objY, objZ );
    double distXY = absDistance( camX, camY, objX, objY );
    /* There is nothing wrong with distXY being 0.0, but dist must
     * at least be one.
     */
    if ( dist < 1.0 )
        dist = 1.0;

    /* Basically the camera is the center of a sphere the object is a point on.
     * To find out how the movement effect is to be applied, the position
     * relative to the camera is needed, but luckily it is already known.
     * However, although the angles (alpha, beta) are neither known nor needed,
     * the distances are, so the movement effect can be modified.
     * Or in other words: As the position on the "sphere" is already known,
     * only the relations have to be calculated back. distXY is therefore
     * sin(beta), which is the the flat distance, meaning the point on a
     * (virtual) circle around the camera, with the radius of distXY.
     */
    double modXY = distXY / dist; // sin(beta)  (unsigned)
    double modX  = ( objX - camX )  / dist // cos(alpha) (signed)
                   * modXY;
    double modY  = ( objY - camY )  / dist // sin(alpha) (signed)
                   * modXY;
    double modZ  = ( objZ - camZ )  / dist; // cos(beta)  (signed)
    /* Sphere Coordinates:
     * X = cos(alpha) * sin(beta) == modX * modXY
     * Y = sin(alpha) * sin(beta) == modY * modXY
     * Z =              cos(beta) == modZ
     */

    double v = ( modX * movX ) + ( modY * movY ) + ( modZ * movZ );

    /* Simple Doppler effect:
     * fE = fS / (1 - (v / c))
     * fE : resulting frequency
     * fS : original frequency
     * c  : speed of light in m/s
     * v  : relative movement vector of the object
     * Thus a modifier to use with modFrequency would be:
     * 1 / (1 - (v / c))
     */
    double modifier = 1.0 / ( 1.0 - ( v / 299792458.0 ) );

    lock();
    int32_t wSize = waves.size();
    for ( int32_t i = 0; i < wSize; ++i ) {
        modFrequency( i, modifier );
    }
    unlock();
}


/** @brief apply a doppler effect according to movement relative to position
  *
  * This method applies a simple doppler effect to all stored
  * wavelengths of this color.
  *
  * Calculated is the movement of the object (@a objX, @a objY, @a objZ)
  * relative to a neutral positioned camera (0, 0, 0) using the objects
  * absolute movement (@a movX, @a movY, @a movZ).
  *
  * <I>Important</I>: This can shift a perfectly visible color out of
  * the visible range if the movement is extreme enough!
  *
  * <I>Even more important</I>: It does <B>not</B> matter in which units
  * the paramters for the object positions are. The movement parameters
  * are always assumed to be in m/s.
  *
  * Please note that a negative movement vector is considered to be a
  * movement towards the camera, while a positive movement vector is
  * considered to be a movement away from the camera.
  *
  * @param[in] objX the X-Coordinate of the colored object
  * @param[in] objY the Y-Coordinate of the colored object
  * @param[in] objZ the Z-Coordinate of the colored object
  * @param[in] movX the absolute X-movement of the colored object in m/s
  * @param[in] movY the absolute Y-movement of the colored object in m/s
  * @param[in] movZ the absolute Z-movement of the colored object in m/s
**/
void CWaveColor::doppler( double objX, double objY, double objZ,
                          double movX, double movY, double movZ ) noexcept {
    this->doppler( 0.0, 0.0, 0.0, objX, objY, objZ, movX, movY, movZ );
}


/** @brief apply a doppler effect according to absolute movement
  *
  * This method applies a simple doppler effect to all stored
  * wavelengths of this color.
  *
  * Calculated is the absolute movement of the object (@a objX, @a objY,
  * @a objZ) as if both the object and the camera where in the same
  * position, but the z-coordinate being 1.0 distance.
  *
  * <I>Important</I>: This can shift a perfectly visible color out of
  * the visible range if the movement is extreme enough!
  *
  * <I>Even more important</I>: The movement parameters
  * are always assumed to be in m/s.
  *
  * Please note that a negative movement vector is considered to be a
  * movement towards the camera, while a positive movement vector is
  * considered to be a movement away from the camera.
  *
  * @param[in] movX the absolute X-movement of the colored object in m/s
  * @param[in] movY the absolute Y-movement of the colored object in m/s
  * @param[in] movZ the absolute Z-movement of the colored object in m/s
**/
void CWaveColor::doppler( double movX, double movY, double movZ ) noexcept {
    this->doppler( 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, movX, movY, movZ );
}


/** @brief return the frequency of the wavelength with the given @a index.
  *
  * This method can be used to retrieve a specific frequency
  * in a set of wavelengths.
  *
  * If the set is empty, the method returns 0.0.
  *
  * If the given @a index is out of bounds, it will be scaled.
  *
  * @param[in] index index of the wavelength to retrieve
  * @return the frequency of the corresponding wavelength or 0.0 if the set is empty.
**/
double CWaveColor::getFrequency ( int32_t index ) const noexcept {
    double wl = getWavelength( index );
    if ( !areAlmostEqual( wl, 0. ) )
        return 299792458.0 / wl;
    else
        return 0.0;
}


/// @brief return current global gamma value of this Color
double CWaveColor::getGamma() const noexcept {
    return gamma;
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
  * but stay being Sailor blue, just brighter. Or concrete: Raising
  * gamma from 1.0 to 3.0 for 0x5f9ea0 (Sailor Blue) results in 0x97fcff.
  *
  * @param[in] r red part of the color.
  * @param[in] g green part of the color.
  * @param[in] b blue part of the color.
  * @return the number of wavelengths this color consists of.
**/
uint32_t CWaveColor::getRGB( uint8_t& r, uint8_t& g, uint8_t& b ) const noexcept {
    uint8_t   red    = 0, green = 0, blue = 0;
    float     sumR   = 0, sumG  = 0, sumB = 0;
    uint32_t  result = 0;

    auto currWave = waves[0];

    // Step one, walk through the waves and add up the colors they produce
    while ( currWave ) {
        wavelengthToRGB( ( **currWave ).wavelength, red, green, blue, ( **currWave ).gamma );
        sumR += red;
        sumG += green;
        sumB += blue;
        if ( currWave == waves[-1] )
            currWave = nullptr;
        else
            currWave = currWave->getNext();
        ++result;
    }

    // Step 4: Apply global gamma
    sumR = static_cast<int32_t>( std::round( gamma * sumR ) );
    sumG = static_cast<int32_t>( std::round( gamma * sumG ) );
    sumB = static_cast<int32_t>( std::round( gamma * sumB ) );

    // Step 3: If we get over 255, a modifier is needed
    float maxPart = std::max( std::max( sumR, sumG ), sumB );
    if ( maxPart > 255.f ) {
        float maxMod = 255.f / maxPart;
        sumR = std::round( maxMod * sumR );
        sumG = std::round( maxMod * sumG );
        sumB = std::round( maxMod * sumB );
    }

    // Step 4: Clip and return
    r = sumR > 255.f ? 255 : sumR < 0.f ? 0 : static_cast<uint8_t>( sumR );
    g = sumG > 255.f ? 255 : sumG < 0.f ? 0 : static_cast<uint8_t>( sumG );
    b = sumB > 255.f ? 255 : sumB < 0.f ? 0 : static_cast<uint8_t>( sumB );

    return result;
}


/** @brief return the wavelength with the given @a index.
  *
  * This method can be used to retrieve a specific wavelength
  * in a set of wavelengths.
  *
  * If the set is empty, the method returns 0.0.
  *
  * If the given @a index is out of bounds, it will be scaled.
  *
  * @param[in] index index of the wavelength to retrieve
  * @return the corresponding wavelength or 0.0 if the set is empty.
**/
double CWaveColor::getWavelength( int32_t index ) const noexcept {
    auto elem = waves[index];
    return elem ? ( **elem ).wavelength : 0.0;
}


/** @brief modifiy the frequency of the wavelength with the given @a index with @a modifier
  *
  * Unlike setFrequency(), this method does not reset the wave specific gamma
  * value that setRGB() might have generated. To have the generated gamma value
  * resetted, setFrequency() must be used.
  *
  * The @a index is not wrapped and must match the range of 0 <= @a index < size().
  * However, the special index -1 to access the last frequency in the set is handled.
  * If the index is out bounds, nothing happens.
  *
  * The @a modifier must be a positive multiplicator larger or equal than zero.
  *
  * @param[in] index the frequency to change, if none with this index is found, nothing happens.
  * @param[in] modifier The modifier to multiply the frequency with.
**/
void CWaveColor::modFrequency( int32_t index, double modifier ) noexcept {
    if ( !( modifier >= 0. ) )
        return;

    auto elem = ( ( index >= -1 ) && ( index < static_cast<int32_t>( waves.size() ) ) ) ? waves[index] : nullptr;

    if ( elem )
        ( **elem ).wavelength /= modifier;
    /* Note to self: It is not necessary to really modify the frequency,
     *               As it is just a reversal of a wavelength modification:
     * f = l/w, w = l/f
     * be f*m = (l*m)/w then
     *     l / ((l/w) * m) = f (modifier applied to transformed wavelength)
     * =>  l = f * (l/w) * m   (bring the divisor to the frequency)
     * =>  l = (f * m * l) / w (another form in one division)
     * =>  l * w = l * f * m   (bring wavelength back to the left)
     * =>  w = f*m             (annihilate l, this is what is wanted, modify the frequency.)
     * =>  w/m = f             (And this is what the modifier then must do to the wavelength.)
     */
}


/** @brief modifiy the wavelength with the given @a index with @a modifier
  *
  * Unlike setWavelength(), this method does not reset the wave specific gamma
  * value that setRGB() might have generated. To have the generated gamma value
  * resetted, setWavelength() must be used.
  *
  * The @a index is not wrapped and must match the range of 0 <= @a index < size().
  * However, the special index -1 to access the last wavelength in the set is handled.
  * If the index is out bounds, nothing happens.
  *
  * The @a modifier must be a positive multiplicator larger or equal than zero.
  *
  * @param[in] index the wavelength to change, if none with this index is found, nothing happens.
  * @param[in] modifier The modifier to multiply the frequency with.
**/
void CWaveColor::modWavelength( int32_t index, double modifier ) noexcept {
    if ( !( modifier >= 0. ) )
        return;

    auto elem = ( ( index >= -1 ) && ( index < static_cast<int32_t>( waves.size() ) ) ) ? waves[index] : nullptr;

    if ( elem )
        ( **elem ).wavelength *= modifier;
}


/** @brief set the frequency of the wavelength with the given @a index to @a frequency
  *
  * Unlike setRGB(), this method has no plan to generate a wave specific gamma
  * value to match a defined color, and (re)sets the wave specific gamma
  * to 1.0. To keep the generated gamma value, modFrequency() must be used.
  *
  * The @a index is not wrapped and must match the range of 0 <= @a index < size(),
  * or a new frequency is added to the set. However, the special index -1 to
  * modifiy the last frequency in the set is handled correctly.
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::CException with the name "ElementCreationFailed".
  *
  * @param[in] index the frequency to change, if none with this index is found, a new one will be added.
  * @param[in] frequency the desired frequency in gigahertz.
**/
void CWaveColor::setFrequency( int32_t index, double frequency ) {
    if ( !areAlmostEqual( frequency, 0.0 ) )
        PWX_TRY_PWX_FURTHER( setWavelength( index, 299792458.0 / frequency ) )
        else
            PWX_TRY_PWX_FURTHER( setWavelength( index, 0.0 ) )
        }


/** @brief set a new general gamma value
  *
  * Set a new gamma value (default is 1.0) that is applied to the resulting
  * RGB value the stored wavelengths represent.
  *
  * @param[in] gamma_ the gamma value to apply
**/
void CWaveColor::setGamma( double gamma_ ) noexcept {
    assert( ( gamma_ >= 0.0 ) && "ERROR: CWaveColor::setGamma() must be called with a positive value for gamma_!" );

    if ( gamma_ >= 0.0 )
        gamma = gamma_;
}


/** @brief Set the set of wavelength to a specific RGB color
  *
  * This method is used to set the current set of wavelengths to a
  * specific color. After splitting the given @a r, @a g and @a b
  * values into wavelengths the count of wavelengths added is returned.
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::CException with the name "ElementCreationFailed".
  *
  * @param[in] r red part of the color
  * @param[in] g green part of the color
  * @param[in] b blue part of the color
  * @return the number of wavelengths added
**/
uint32_t CWaveColor::setRGB( uint8_t r, uint8_t g, uint8_t b ) {
    PWX_LOCK_GUARD( this )

    sWave* newWave = nullptr;

    // Preparation 1: Clear all waves
    waves.clear();

    // Preparation 2: get the RGB values needed to result in the argument RGB with applied global gamma
    uint8_t red   = unapplyGamma( r, gamma );
    uint8_t green = unapplyGamma( g, gamma );
    uint8_t blue  = unapplyGamma( b, gamma );

    while ( red || green || blue ) {
        // Step one: Find wavelength and gamma value to add
        double  wavelength  = 0.0;
        double  wavegamma   = 0.0;

        if ( red >= std::max( green, blue ) ) {
            // Main Color Part is red
            wavegamma = static_cast<double>( red ) / 255.0;

            if ( green >= blue )
                // Secondary color is green
                wavelength = rgbToWavelength( red, green, 0, wavegamma );
            else
                /* Secondary color is blue
                * IMPORTANT: There is _NO_ wavelength representing a color with
                *            more red than blue, unless blue is zero!
                *            The red part can be higher than green, but when
                *            mixed with blue, red must not exceed the blue part.
                */
                wavelength = rgbToWavelength( ::std::min( red, blue ), 0, blue, wavegamma );
        } else if ( green >= blue ) {
            // Main Color Part is green
            wavegamma = static_cast<double>( green ) / 255.0;
            if ( red >= blue )
                // Secondary color is red
                wavelength = rgbToWavelength( red, green, 0, wavegamma );
            else
                // Secondary color is blue
                wavelength  = rgbToWavelength( 0, green, blue, wavegamma );
        } else {
            // Main Color Part is blue
            wavegamma = static_cast<double>( blue ) / 255.0;
            if ( red >= green )
                // Secondary color is red
                wavelength = rgbToWavelength( red, 0, blue, wavegamma );
            else
                // Secondary color is green
                wavelength  = rgbToWavelength( 0, green, blue, wavegamma );
        } // end of setting wavelength and gamma

        // Step 2: Add wavelength and gamma.
        PWX_TRY_STD_FURTHER( newWave  = new sWave( wavelength, wavegamma ),
                             "ElementCreationFailed", "Exception caught while creating new sWave entry" )
        PWX_TRY_PWX_FURTHER( waves.push( newWave ) )

        // Step 3: normalize the generated wave and get finalRGB set
        normalize( *newWave, red, green, blue );
        uint8_t currR, currG, currB;
        wavelengthToRGB( newWave->wavelength, currR, currG, currB, newWave->gamma );

        // Step 4: reduce the color parts by the values added
        red   -= currR < red   ? currR : red;
        green -= currG < green ? currG : green;
        blue  -= currB < blue  ? currB : blue;
    } // End of having colors left

    return waves.size();
}


/** @brief set the wavelength with the given @a index to @a wavelength.
  *
  * Unlike setRGB(), this method has no plan to generate a wave specific gamma
  * value to match a defined color, and (re)sets the wave specific gamma
  * to 1.0. To keep the generated gamma value, modWavelength() must be used.
  *
  * The @a index is not wrapped and must match the range of 0 <= @a index < size(),
  * or a new frequency is added to the set. However, the special index -1 to
  * modifiy the last wavelength in the set is handled correctly.
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::CException with the name "ElementCreationFailed".
  *
  * <I>Note</I>: Only a @a wavelength between 380nm and 780nm produces a visible color.
  *
  * @param[in] index the wavelength to change, if none with this index is found, a new one will be added.
  * @param[in] wavelength the desired wavelength in nanometers.
**/
void CWaveColor::setWavelength( int32_t index, double wavelength ) {
    auto elem = ( ( index >= -1 ) && ( index < static_cast<int32_t>( waves.size() ) ) ) ? waves[index] : nullptr;

    if ( elem ) {
        ( **elem ).wavelength = wavelength;
        ( **elem ).gamma      = 1.0;
    } else {
        sWave* newWave = nullptr;
        PWX_TRY_STD_FURTHER( newWave  = new sWave( wavelength, 1.0 ),
                             "ElementCreationFailed", "Exception caught while creating new sWave entry" )
        PWX_TRY_PWX_FURTHER( waves.push( newWave ) )
    }
}


/** @brief get the number of spectral colors this color is made of.
  * @return the number of spectral colors used.
**/
uint32_t CWaveColor::size() const noexcept {
    return waves.size();
}


/** @brief assignment operator
  *
  * If the creation of a wave length entry fails, this method throws a
  * pwx::CException with the name "ElementCreationFailed".
  *
  * @param[in] src rhs operand
  * @return reference to this
**/
CWaveColor& CWaveColor::operator=( const CWaveColor& src ) {
    if ( &src != this ) {
        PWX_DOUBLE_LOCK_GUARD( this, &src )
        gamma = src.gamma;
        waves = src.waves;
    }
    return *this;
}


} // namespace pwx
