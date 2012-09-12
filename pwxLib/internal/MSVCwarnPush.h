/** @internal
  * @file MSVCwarnPush.h
  * ------------------------
  *
  * @brief Disable warnings from MSVC++ Headers
  *
  * This file is meant to be included before system headers get included to
  * disable warnings from Microsoft Visual C++ Headers. Those warnings can
  * not be "fixed", but might flood the compilers output, possibly
  * obfuscating important warnings from pwxLib Headers.
  *
  * If pwxLib is not built by MSVC++, this header does nothing.
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
  * 0.1.0     2011-01-19  sed, PrydeWorX  Initial Release
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version correction and bump to new pwxLib release version
  @endverbatim
**/

#if defined(_MSC_VER)
#  pragma warning( push )
#  pragma warning( disable : 243 4180 4548 4710 4820 )
#endif
// Note: Do not forget to include MSVCwarnPop.h once all system headers are included!
