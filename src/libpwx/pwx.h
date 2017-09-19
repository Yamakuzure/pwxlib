#pragma once
#ifndef PWX_LIBPWX_PWX_H_INCLUDED
#define PWX_LIBPWX_PWX_H_INCLUDED 1

/** @file pwx.h
  *
  * @brief Main include file for the PrydeWorX library
  *
  * This include file adds all necessary includes to make the pwxLib workers
  * available. These workers are:
  * <TABLE border='1'>
  * <TR><TH>Worker</TH><TH>Task</TH><TH>Include file</TH></TR>
  * <TR><TD>PAH</TD><TD>Handle arguments to, and help texts from a program.</TD>
  *   <TD>CArgHandler.h</TD></TR>
  * <TR><TD>RNG</TD><TD>Thread safe random number, random names, noise, simplex
  *   noise and hash generation.</TD><TD>CRandom.h</TD></TR>
  * <TR><TD>SCT</TD><TD>Provides sine/cosine tables of variable precision</TD>
  *   <TD>CSinCosTable.h</TD></TR>
  * </TABLE>
  *
  * Further include files you might be interested in and the respective classes
  * and templates they provide, are:
  *
  * <TABLE border='1'>
  * <TR><TH>Class/Template</TH><TH>Task</TH><TH>Include file</TH></TR>
  * <TR><TD>CHashBuilder</TD><TD>Class to get the hash of a value of variable
  *   type, available as a functor. </TD><TD>CHashBuilder.h</TD></TR>
  * <TR><TD>CWaveColor</TD><TD>Convert light waves in RGB values and vice versa.
  *   </TD><TD>CWaveColor.h</TD></TR>
  * <TR><TD>TChainHash</TD><TD>Thread safe chained hash table.</TD>
  *   <TD>TChainHash.h</TD></TR>
  * <TR><TD>TDoubleList</TD><TD>Thread safe doubly linked list.</TD>
  *   <TD>TDoubleList.h</TD></TR>
  * <TR><TD>TDoubleRing</TD><TD>Thread safe doubly linked ring.</TD>
  *   <TD>TDoubleRing.h</TD></TR>
  * <TR><TD>TOpenHash</TD><TD>Thread safe open hash with optional robin hood
  *    insertion.</TD><TD>TOpenHash.h</TD></TR>
  * <TR><TD>TQueue</TD><TD>Thread safe 'queue' container (FiFo).</TD>
  *    <TD>TQueue.h</TD></TR>
  * <TR><TD>TSet</TD><TD>Thread safe 'set' container (unique items).</TD>
  *    <TD>TSet.h</TD></TR>
  * <TR><TD>TSingleList</TD><TD>Thread safe singly linked list.</TD>
  *   <TD>TSingleList.h</TD></TR>
  * <TR><TD>TSingleRing</TD><TD>Thread safe singly linked ring.</TD>
  *   <TD>TSingleRing.h</TD></TR>
  * <TR><TD>TStack</TD><TD>Thread safe 'stack' container (FiLo).</TD>
  *    <TD>TStack.h</TD></TR>
  * </TABLE>
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
  * 0.8.6.12  2012-10-29  sed, PrydeWorX  Imported pwxLib RNG (CRandom)
  * 0.8.6.13  2012-11-21  sed, PrydeWorX  Changed all containers to a new scheme: public methods do no real work, private
  *                                       and protected methods do the work and are overwritten where necessary.
  * 0.8.6.14  2012-11-22  sed, PrydeWorX  Removed PWX_INLINE macro, it is of no use any more in a real library!
  * 0.8.7.0   2013-06-11  sed, PrydeWorX  After a lot of work all basic containers are now thread safe without using mutexes.
  * 0.8.7.1   2013-06-21  sed, PrydeWorX  Chained and open hash table containers are now available.
  * 0.8.7.2   2013-07-29  sed, PrydeWorX  Optimized open hash table, added tools/MathHelpers.h
  * 0.8.7.3   2013-08-25  sed, PrydeWorX  Changed TSet to use a TChainHash for lookup and optimized all containers.
  *                                       pwxLibNG now supports Spinlocks without yield() and using mutexes via Makefile.
  * 0.8.7.4   2013-08-27  sed, PrydeWorX  Imported pwxLib CWaveColor.
  @endverbatim
**/


#include "CArgHandler.h"
#include "CRandom.h"
#include "CSinCosTable.h"


namespace pwx {


/// @brief Global argument handler that can be used centrally in any project
extern PWX_API CArgHandler  PAH; // [P]rogram [A]rgument [H]andler

/// @brief Global random value generator that can be used centrally in any project
extern PWX_API CRandom      RNG; // [R]andom [N]-Value [G]enerator

/// @brief Global sine-/cosine-table that can be used centrally in any project
extern PWX_API CSinCosTable SCT; // [S]ine-/[C]osine-[T]able


}


#endif // PWX_LIBPWX_PWX_H_INCLUDED
