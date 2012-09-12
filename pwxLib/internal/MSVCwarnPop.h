/** @internal
  * @file MSVCwarnPop.h
  * ------------------------
  *
  * @brief Reverse the effect of MSVCwarnPush.h
  *
  * This header simply reinstates the warnings disabled by MSVCwarnPush.h.
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
  * 0.1.1     2011-01-20  sed, PrydeWorX  Took some warnings out permanently, as are more of a
  *                                       flooding nuisance than any valuable help.
  * 0.1.2     2011-01-24  sed, PrydeWorX  Added informational warning 4571
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version correction and bump to new pwxLib release version
  @endverbatim
**/

#if defined(_MSC_VER)
#  pragma warning( pop )
/* These warnings, to be taken out permanently, are:
 * 4515 : not referenced inline function removed. So what?
 * 4571 : catchall changed from VS7.1, SEH is no longer caught. Fine.
 * 4710 : the function is not inline. And what should I do about this?
 * 4711 : class x selected for automatic inline extension. Yes, I know, I wrote it.
 * 4820 : x Bytes distance added behind data member foo. Yes. And?
 */
#  pragma warning( disable : 4514 4571 4710 4711 4820 )
#endif
