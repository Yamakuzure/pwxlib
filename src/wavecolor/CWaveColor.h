#ifndef PWX_LIBPWX_PWX_WORKER_CWAVECOLOR_H_INCLUDED
#define PWX_LIBPWX_PWX_WORKER_CWAVECOLOR_H_INCLUDED

/** @file CWaveColor.h
  *
  * @brief Helper class to work with colors via wavelength versus RGB
  *
  * (c) 2007 - 2017 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
  *
  * The PrydeWorX Library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public License as
  * published by the Free Software Foundation; either version 2.1 of the
  * License, or (at your option) any later version.
  *
  * The PrydeWorX Library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public License
  * along with pwxLib; If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
**/


#include "TSingleList.h"


namespace pwx {


/** @struct sWave
  * @brief tiny and simple struct to store wavelengths and their gamma
**/
struct sWave
{
	double gamma      = 0.0; //!< Specific gamma value derived out of the source RGB
	double wavelength = 0.0; //!< The wavelength in nanometers

	explicit sWave(double aWavelength, double gamma_) noexcept;
	sWave(const sWave &src) noexcept;
	sWave() PWX_DELETE;
	~sWave() PWX_DEFAULT;

	sWave &operator= (const sWave &src) noexcept;
	bool   operator==(const sWave &rhs) const noexcept;
	bool   operator> (const sWave &rhs) const noexcept;
};


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
class PWX_API CWaveColor : public CLockable
{
public:

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	*/

	typedef CLockable          base_t;
	typedef TSingleList<sWave> list_t;

	/* ===============================================
	 * === Public constructors and destructor      ===
	 * ===============================================
	*/

	explicit
	CWaveColor(uint8_t r, uint8_t g, uint8_t b, double gamma_ = 1.0);
	CWaveColor()  noexcept;
	CWaveColor(const CWaveColor &src);
	~CWaveColor() noexcept;

	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	uint32_t addRGB       (uint8_t r, uint8_t g, uint8_t b);
	void     clear        ()                                         noexcept;
	uint32_t count        ()                                   const noexcept;
	uint32_t delRGB       (uint8_t r, uint8_t g, uint8_t b);
	void     doppler      (double camX, double camY, double camZ,
						   double objX, double objY, double objZ,
						   double movX, double movY, double movZ)    noexcept;
	void     doppler      (double objX, double objY, double objZ,
						   double movX, double movY, double movZ)    noexcept;
	void     doppler      (double movX, double movY, double movZ)    noexcept;
	double   getFrequency (int32_t index)                      const noexcept;
	double   getGamma     ()                                   const noexcept;
	uint32_t getRGB       (uint8_t &r, uint8_t &g, uint8_t &b) const noexcept;
	double   getWavelength(int32_t index)                      const noexcept;
	void     modFrequency (int32_t index, double modifier)           noexcept;
	void     modWavelength(int32_t index, double modifier)           noexcept;
	void     setFrequency (int32_t index, double frequency);
	void     setGamma     (double gamma_)                            noexcept;
	uint32_t setRGB       (uint8_t r, uint8_t g, uint8_t b);
	void     setWavelength(int32_t index, double wavelength);
	uint32_t size         ()                                   const noexcept;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	CWaveColor &operator=(const CWaveColor &src);


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

