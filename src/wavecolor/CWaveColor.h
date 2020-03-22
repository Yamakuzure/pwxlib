#ifndef PWX_LIBPWX_PWX_WORKER_CWAVECOLOR_H_INCLUDED
#define PWX_LIBPWX_PWX_WORKER_CWAVECOLOR_H_INCLUDED

/** @file CWaveColor.h
  *
  * @brief Helper class to work with colors via wavelength versus RGB
  *
  * (c) 2007 - 2020 PrydeWorX
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


#include "basic/pwx_compiler.h"

#include "container/TSingleList.h"


namespace pwx {


/** @struct sWave
  * @brief tiny and simple struct to store wavelengths and their gamma
**/
struct PWX_API sWave {
	double gamma      = 0.0; //!< Specific gamma value derived out of the source RGB
	double wavelength = 0.0; //!< The wavelength in nanometer

	/** @brief default constructor
	  *
	  * @param[in] wavelength_ The wavelength to use.
	  * @param[in] gamma_ The gamma to use.
	**/
	explicit sWave( double wavelength_, double gamma_ ) noexcept;

	/// @brief copy constructor
	sWave( const sWave& src ) noexcept;

	sWave() PWX_DELETE;
	~sWave() PWX_DEFAULT;

	/// @brief assignement operator
	sWave& operator= ( const sWave& src ) noexcept;

	/// @brief Return true if this and @a rhs are equal
	bool   operator==( const sWave& rhs ) const noexcept;

	/// @brief Retrun true if thes is larger than @a rhs
	bool   operator> ( const sWave& rhs ) const noexcept;
};


/** @class CWaveColor PWaveColor <PWaveColor>
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
  * Important: Wavelengths are considered to be nanometer and frequencies
  * are considered to be gigahertz by the methods of this class.
**/
class PWX_API CWaveColor : public CLockable {
public:

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef CLockable          base_t; //!< Base type of CWaveColor
	typedef TSingleList<sWave> list_t; //!< List type of CWaveColor


	/* ===============================================
	 * === Public constructors and destructor      ===
	 * ===============================================
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
	explicit
	CWaveColor( uint8_t r, uint8_t g, uint8_t b, double gamma_ = 1.0 );


	/** @brief empty constructor
	  *
	  * add Wavelength later...
	**/
	CWaveColor()  noexcept;


	/** @brief Copy constructor
	  *
	  * The full data of @a src is copied.
	  *
	  * If the creation of a wave length entry fails, this method throws a
	  * pwx::CException with the name "ElementCreationFailed".
	  *
	  * @param[in] src The CWaveColor instance to copy.
	  */
	CWaveColor( const CWaveColor& src );


	/// @brief default dtor
	~CWaveColor() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

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
	uint32_t addRGB( uint8_t r, uint8_t g, uint8_t b );


	/// @brief remove all wavelengths and reset global gamma value to 1.0.
	void clear() noexcept;


	/** @brief get the number of spectral colors this color is made of.
	  * @return the number of spectral colors used.
	**/
	uint32_t count() const noexcept;


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
	uint32_t delRGB ( uint8_t r, uint8_t g, uint8_t b );


	/** @brief apply a Doppler effect according to movement relative to position
	  *
	  * This method applies a simple Doppler effect to all stored
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
	  * the parameters for the camera and object positions are, as long as they
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
	void doppler( double camX, double camY, double camZ,
	              double objX, double objY, double objZ,
	              double movX, double movY, double movZ ) noexcept;


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
	  * the parameters for the object positions are. The movement parameters
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
	void doppler( double objX, double objY, double objZ,
	              double movX, double movY, double movZ ) noexcept;


	/** @brief apply a Doppler effect according to absolute movement
	  *
	  * This method applies a simple Doppler effect to all stored
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
	void doppler( double movX, double movY, double movZ ) noexcept;


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
	double getFrequency( int32_t index ) const noexcept;


	/// @brief return current global gamma value of this Color
	double   getGamma() const noexcept;


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
	uint32_t getRGB( uint8_t& r, uint8_t& g, uint8_t& b ) const noexcept;


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
	double getWavelength( int32_t index ) const noexcept;


	/** @brief modify the frequency of the wavelength with the given @a index with @a modifier
	  *
	  * Unlike setFrequency(), this method does not reset the wave specific gamma
	  * value that setRGB() might have generated. To have the generated gamma value
	  * reset, setFrequency() must be used.
	  *
	  * The @a index is not wrapped and must match the range of 0 <= @a index < size().
	  * However, the special index -1 to access the last frequency in the set is handled.
	  * If the index is out bounds, nothing happens.
	  *
	  * The @a modifier must be a positive multiplier larger or equal than zero.
	  *
	  * @param[in] index the frequency to change, if none with this index is found, nothing happens.
	  * @param[in] modifier The modifier to multiply the frequency with.
	**/
	void modFrequency( int32_t index, double modifier ) noexcept;


	/** @brief modify the wavelength with the given @a index with @a modifier
	  *
	  * Unlike setWavelength(), this method does not reset the wave specific gamma
	  * value that setRGB() might have generated. To have the generated gamma value
	  * reset, setWavelength() must be used.
	  *
	  * The @a index is not wrapped and must match the range of 0 <= @a index < size().
	  * However, the special index -1 to access the last wavelength in the set is handled.
	  * If the index is out bounds, nothing happens.
	  *
	  * The @a modifier must be a positive multiplier larger or equal than zero.
	  *
	  * @param[in] index the wavelength to change, if none with this index is found, nothing happens.
	  * @param[in] modifier The modifier to multiply the frequency with.
	**/
	void modWavelength( int32_t index, double modifier ) noexcept;


	/** @brief set the frequency of the wavelength with the given @a index to @a frequency
	  *
	  * Unlike setRGB(), this method has no plan to generate a wave specific gamma
	  * value to match a defined color, and (re)sets the wave specific gamma
	  * to 1.0. To keep the generated gamma value, modFrequency() must be used.
	  *
	  * The @a index is not wrapped and must match the range of 0 <= @a index < size(),
	  * or a new frequency is added to the set. However, the special index -1 to
	  * modify the last frequency in the set is handled correctly.
	  *
	  * If the creation of a wave length entry fails, this method throws a
	  * pwx::CException with the name "ElementCreationFailed".
	  *
	  * @param[in] index the frequency to change, if none with this index is found, a new one will be added.
	  * @param[in] frequency the desired frequency in gigahertz.
	**/
	void setFrequency( int32_t index, double frequency );


	/** @brief set a new general gamma value
	  *
	  * Set a new gamma value (default is 1.0) that is applied to the resulting
	  * RGB value the stored wavelengths represent.
	  *
	  * @param[in] gamma_ the gamma value to apply
	**/
	void setGamma( double gamma_ ) noexcept;


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
	uint32_t setRGB( uint8_t r, uint8_t g, uint8_t b );


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
	void setWavelength( int32_t index, double wavelength );


	/** @brief get the number of spectral colors this color is made of.
	  * @return the number of spectral colors used.
	**/
	uint32_t size() const noexcept;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	/** @brief assignment operator
	  *
	  * If the creation of a wave length entry fails, this method throws a
	  * pwx::CException with the name "ElementCreationFailed".
	  *
	  * @param[in] src rhs operand
	  * @return reference to this
	**/
	CWaveColor& operator=( const CWaveColor& src );


private:

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	double gamma = 1.0; //!< General gamma value, applied to the resulting RGB value
	list_t waves;       //!< Storage of wavelength
};


} // namespace pwx


#endif // PWX_LIBPWX_PWX_WORKER_CWAVECOLOR_H_INCLUDED

