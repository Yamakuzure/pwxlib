#pragma once
#ifndef PWX_LIBPWX_PWX_H_INCLUDED
#define PWX_LIBPWX_PWX_H_INCLUDED 1

/** @file pwx.h
  *
  * @brief Main include file for the PrydeWorX library
  *
  * This include file makes everything in the pwxLib available.
  *
  * As you most probably do not need everything, you might be better off
  * including only the files you really need.
  *
  * The individual components of the library can be added by including:
  *
  * <TABLE border='1'>
  * <TR><TH>Class/Template/Tools</TH><TH>Task</TH><TH>Include file</TH></TR>
  * <TR><TD>Basics</TD><TD>Add basic functionality most parts use.</TD>
  *   <TD>pwx_basic.h</TD></TR>
  * <TR><TD>Containers</TD><TD>Add all container definitions currently provided.</TD>
  *   <TD>pwx_containers.h</TD></TR>
  * <TR><TD>Helpers</TD><TD>Add all helper classes and functions currently provided.</TD>
  *   <TD>pwx_helpers.h</TD></TR>
  * <TR><TD>Utilities</TD><TD>Add all utility classes and templates currently provided.</TD>
  *   <TD>pwx_utils.h</TD></TR>
  * <TR><TD>PAH, RNG, SCT</TD><TD>Make the worker instances available</TD>
  *   <TD>pwx_workers.h</TD></TR>
  * <TR><TD>CArgHandler</TD><TD>Make CArgHandler available, without instantiating PAH</TD>
  *   <TD>CArgHandler.h</TD></TR>
  * <TR><TD>CRandom</TD><TD>Make CRandom available, without instantiating RNG</TD>
  *   <TD>CRandom.h</TD></TR>
  * <TR><TD>CSinCosTable</TD><TD>Make CSinCosTable available, without instantiating SCT</TD>
  *   <TD>CSinCosTable.h</TD></TR>
  * <TR><TD>CHashBuilder</TD><TD>Class to get the hash of a value of variable
  *   type, available as a functor. </TD><TD>pwx/pwxCHashBuilder.h</TD></TR>
  * <TR><TD>CWaveColor</TD><TD>Convert light waves in RGB values and vice versa.
  *   </TD><TD>pwx/pwxCWaveColor.h</TD></TR>
  * <TR><TD>TChainHash</TD><TD>Thread safe chained hash table.</TD>
  *   <TD>pwx/pwxTChainHash.h</TD></TR>
  * <TR><TD>TDoubleList</TD><TD>Thread safe doubly linked list.</TD>
  *   <TD>pwx/pwxTDoubleList.h</TD></TR>
  * <TR><TD>TDoubleRing</TD><TD>Thread safe doubly linked ring.</TD>
  *   <TD>pwx/pwxTDoubleRing.h</TD></TR>
  * <TR><TD>TOpenHash</TD><TD>Thread safe open hash with optional robin hood
  *    insertion.</TD><TD>pwx/pwxTOpenHash.h</TD></TR>
  * <TR><TD>TQueue</TD><TD>Thread safe 'queue' container (FiFo).</TD>
  *    <TD>pwx/pwxTQueue.h</TD></TR>
  * <TR><TD>TSet</TD><TD>Thread safe 'set' container (unique items).</TD>
  *    <TD>pwx/pwxTSet.h</TD></TR>
  * <TR><TD>TSingleList</TD><TD>Thread safe singly linked list.</TD>
  *   <TD>pwx/pwxTSingleList.h</TD></TR>
  * <TR><TD>TSingleRing</TD><TD>Thread safe singly linked ring.</TD>
  *   <TD>pwx/pwxTSingleRing.h</TD></TR>
  * <TR><TD>TStack</TD><TD>Thread safe 'stack' container (FiLo).</TD>
  *    <TD>pwx/pwxTStack.h</TD></TR>
  * </TABLE>
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
  * 0.8.6.6   2012-09-25  sed, PrydeWorX  Added general/pwx_templates.h and some more macros from old DefaultTempTools.h
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
  * 0.8.7.2   2013-07-29  sed, PrydeWorX  Optimized open hash table, added tools/pwxMathHelpers.h
  * 0.8.7.3   2013-08-25  sed, PrydeWorX  Changed TSet to use a TChainHash for lookup and optimized all containers.
  *                                       pwxLibNG now supports Spinlocks without yield() and using mutexes via Makefile.
  * 0.8.7.4   2013-08-27  sed, PrydeWorX  Imported pwxLib CWaveColor.
  * 0.8.7.5   2013-11-04  sed, PrydeWorX  Imported pwxLib CArgHandler
  * 0.8.8.0   2015-08-13  sed, PrydeWorX  Reordered and modernized
  * 0.8.9     2018-12-13  sed, PrydeWorX  First release on GitHub.
  @endverbatim
**/


#include <pwx_basic.h>
#include <pwx_containers.h>
#include <pwx_tools.h>
#include <pwx_utils.h>
#include <pwx_workers.h>


#endif // PWX_LIBPWX_PWX_H_INCLUDED
