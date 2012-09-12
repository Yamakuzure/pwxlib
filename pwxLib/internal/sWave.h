#pragma once
#ifndef PWX_SWAVE_H_INCLUDED
#define PWX_SWAVE_H_INCLUDED 1

/** @internal
  * @file sWave.h
  *
  * @brief This file declares and defines the the helper struct sWave for CWaveColor
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
# error "Do not include sWave.h, include CWaveColor.h!"
#endif // Check for main file

namespace pwx {
namespace CWC {
// note: The namespaces are documented in tools/CWaveColor.h!

/** @struct sWave
  * @brief tiny and simple struct to organize wavelengths and gamma value chains
**/
struct sWave
{
  double wavelength; //!< The wavelength in nanometers
  double gamma;      //!< Specific gamma value derived out of the source RGB
  sWave* next;       //!< Pointer to the next sWave or NULL if this is the last

  /// @brief default ctor, do not use any else
  explicit sWave (sWave* parent, double aWavelength, double aGamma):
    wavelength(aWavelength), gamma(aGamma), next(NULL)
    {
      if (parent)
        parent->next = this;
    }

  /// @brief default dtor - This automatically erases a chain, ONLY delete the root element!
  ~sWave()
    {
      if (next)
        delete next;
    }

  /// @brief copy ctor, inserts into a chain
  sWave (sWave &src): wavelength(src.wavelength), gamma(src.gamma), next(src.next)
    {
      src.next   = this;
    }

  /// @brief assignment operator, omits next pointer
  sWave &operator=(sWave &src)
    {
      if (this != &src)
        {
          wavelength = src.wavelength;
          gamma      = src.gamma;
        }
      return *this;
    }

private:
  sWave(); // do not use
};

} // End of namespace CWC
} // End of namespace pwx


#endif // PWX_SWAVE_H_INCLUDED

