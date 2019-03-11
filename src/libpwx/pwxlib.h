#pragma once
#ifndef PWX_LIBPWX_PWX_H_INCLUDED
#define PWX_LIBPWX_PWX_H_INCLUDED 1

/** @file pwxlib.h
  *
  * This is a "catch-all" include file that simply adds everything there
  * is. It is not necessary to include anything else for the library to
  * be fully usable.
  *
  * @mainpage PrydeWorX Library
  *
  * This library provides some tools and utilities that may make your life
  * as a programmer easier. There are some mathematical tools as well as
  * standard containers for variable data types and some input and output
  * stream helpers. Further there are some larger helper classes called
  * &quot;workers&quot; that do more than the ordinary.
  *
  * @tableofcontents
  *
  * @section overview Overview
  * @todo write overview
  *
  * @section ghLinks GitHub
  * Additional information on the GitHub project page:
  * @subsection ghOverview
  * Overview page on GitHub
  *
  * @section BuildInst Building and Installation
  *
  * @section contMain Content
  * @todo write
  * @subsection contWorkers Workers
  * @subpage CSimplexTexture CSimplexTexture
  * @todo write
  * @subsection contContain Containers
  * @todo write
  * @subsection contTools Tools
  * @todo write
  * @subsection contTypes Types
  * @todo write
  * @subsection contGeneral General
  * @todo write
  * @section CopyLic Copyright and license
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


#include <basic.h>
#include <pwx_containers.h>
#include <pwx_tools.h>
#include <pwx_utils.h>
#include <pwx_workers.h>


#endif // PWX_LIBPWX_PWX_H_INCLUDED
