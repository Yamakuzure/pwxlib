#pragma once
#ifndef PWX_LIBPWX_PWX_H_INCLUDED
#define PWX_LIBPWX_PWX_H_INCLUDED 1

/** @file pwx.h
  *
	* @brief Main include file for the complete pwx library
	*
	* This is a "catch-all" include file that simply adds everything there
	* is. It is not necessary to include anything else for the library to
	* be fully usable.
	*
  * (c) 2007 - 2012 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
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
  * 0.0.1     2007-12-21  sed, PrydeWorX  First Design
  * 0.1.0     2008-04-09  sed, PrydeWorX  initial private release
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to first public pwxLib release version
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5     2012-03-01  sed, PrydeWorX  Welcome to 2012. Version bump to new pwxLib release version
  * 0.8.6.1   2012-06-01  sed, PrydeWorX  Major change from JBoH to library.
  * 0.8.6.2   2012-08-20  sed, PrydeWorX  Layout and test program complete, CException, CLockable, TSingleList and
  *                                       TDoubleList added.
  * 0.8.6.3   2012-09-10  sed, PrydeWorX  Lists reorganized, StreamHelpers re-added, speed tests added.
  * 0.8.6.4   2012-09-11  sed, PrydeWorX  Added TStack and TQueue.
  * 0.8.6.5   2012-09-13  sed, PrydeWorX  Added TSingelRing, TDoubleRing and TSet
  * 0.8.6.6   2012-09-25  sed, PrydeWorX  Added general/templates.h and some more macros from old DefaultTempTools.h
  * 0.8.6.7   2012-09-27  sed, PrydeWorX  Testing of the lists unified, lists, rings TStack and TQueue finished.
  * 0.8.6.8   2012-09-30  sed, PrydeWorX  Basic TSet added and tested. Mathematical methods follow.
  * 0.8.6.9   2012-10-02  sed, PrydeWorX  Added TVarDeleter that delegates given destroy methods for shared_ptr
  * 0.8.6.10  2012-10-08  sed, PrydeWorX  Added operator=/+/-/+=/-= to all containers and finished TSet implementation.
  * 0.8.6.11  2012-10-24  sed, PrydeWorX  Many fixes, completed TSet and and set functions.
  @endverbatim
**/

#include <pwx/general.h>
#include <pwx/tools.h>
#include <pwx/types.h>
#include <pwx/container.h>
#include <pwx/worker.h>

#endif // PWX_LIBPWX_PWX_H_INCLUDED
