#ifndef PWX_LIBPWX_PWX_H_INCLUDED
#define PWX_LIBPWX_PWX_H_INCLUDED 1
#pragma once

/** @file pwxlib.h
  * @brief Include everything at once.
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
  * The PrydeWorX Library is split into sections, which have their own
  * subdirectories. For simpler access all major API components have a wrapper
  * include file in the main include directory, which is by default
  * `/usr/include/pwxlib`.
  * So, instead of writing
  * ~~~~~~~~ .c
  * #include <math_helpers/MathHelpers.h>
  * ~~~~~~~~
  * you can simply write
  * ~~~~~~~~ .c
  * #include <PMath>
  * ~~~~~~~~
  *
  * The main components are:
  * | Folder            | Content                                                       |
  * | ----------------- | ------------------------------------------------------------- |
  * | `arg_handler/`    | Components of PAH, the program argument handler               |
  * | `basic/`          | Core tools for exception and thread lock handling             |
  * | `container/`      | Threadsafe containers from lists to hashes                    |
  * | `math_helpers/`   | Tools helping with angles, degrees and floating point numbers |
  * | `random/`         | Components of RNG, the Random NVal Generator                  |
  * | `stream_helpers/` | Tools helping with handling streams, like alignment           |
  * | `wavecolor/`      |  Helper for using wavelengths to describe RGB                 |
  *
  * @section ghLinks GitHub
  * You can find additional information on the [GitHub project page][pgpp].
  *
  * @subsection ghIssues Issues
  * Please report any issues you find on the [GitHub issues page][pgip].
  *
  * @subsection ghPR Pull Request
  * If you fix things yourself in your own fork, please send  your fixes back
  * using the [GitHub Pull Request][pgpr] feature.
  *
  * [pgpp]: https://github.com/Yamakuzure/pwxlib        "pwxLib on GitHub"
  * [pgip]: https://github.com/Yamakuzure/pwxlib/issues "GitHub issues page"
  * [pgpr]: https://github.com/Yamakuzure/pwxlib/pulls  "GitHub pull request"
  *
  * @section BuildInst Building and Installation
  * The PrydeWorX library is configured with meson and built with ninja.
  *
  * If you are uncomfortable with this build system, you can use the wrapper
  * configure script and Makefile:
  * - Use `./configure --help` to see all options.
  * - Build with a simple `make`
  *
  * @subsection contWorkers Workers
  * Currently there are three workers, which basically are global instances of
  * classes you can use yourself, too.
  *
  * 1. pwx::PAH : This is an instance of pwx::CArgHandler and can manage both
  * the program arguments of your software, and the arguments help texts.
  * 2. pwx::RNG : This is an instance of pwx::CRandom and can generate random
  * numbers, [simplex] noise, names and hashes on various types.
  * 3. pwx::SCT : This is an instance of pwx::CSinCosTable and might be able to
  * help and/or speed up degree calculations.
  *
  * @subsection contContain Containers
  * All containers are based on pointers instead of copying objects. Further they
  * are internally threadsafe. But they lack many convenient tools and utilities the
  * standard containers offer, so you are encouraged to use them instead.
  *
  * The containers are, in alphabetical order:
  * | Container        | Include     | Description                                                       |
  * | ---------------- | ----------- | ----------------------------------------------------------------- |
  * | pwx::TChainHash  | PChainHash  | Chained hash container.                                           |
  * | pwx::TDoubleList | PDoubleList | Doubly linked list.                                               |
  * | pwx::TDoubleRing | PDoubleRing | Doubly linked list where the head and tail are connected.         |
  * | pwx::TOpenHash   | POpenHash   | Open hash container with auto grow and Robin Hood Insertion.      |
  * | pwx::TQueue      | PQueue      | Doubly linked list variant that pop()s head and push()es tail.    |
  * | pwx::TSet        | PSet        | A set container supporting unions, differences and intersections. |
  * | pwx::TSingleList | PSingleList | Singly linked list.                                               |
  * | pwx::TSingleRing | PSingleRing | Singly linked list where head is next of tail.                    |
  * | pwx::TStack      | PStack      | Singly linked list variant that pop()s tail and push()es tail.    |
  *
  * @subsection contTools Tools
  * Apart from the workers and the containers, there are some tools that might be
  * helpful. 
  * @subsection contTypes Types
  * @todo write
  * @subsection contGeneral General
  * @todo write
  *
  * @section contMain Motivation
  * The original motivation to write this library were random numbers. In 2007
  * I had a problem with a bug in a game. In a particular place a random number
  * was generated, and it was always the same.
  *
  * To work around this phenomenon, I wrote a tiny random wrapper that ensured
  * that the random number generated was different than the last. It was, of
  * course, useless, as two times the same number in a row is completely in
  * order for random number generation.
  *
  * The real issue was, that srand() was called in a place right before the
  * &quot;malfunctioning&quot; function was called. But I already had added
  * some tiny tools to get random numbers in a specific range, as the project
  * I worked on was full of `rand() % x + y` and similar.
  * Additionally the project made heavy use of the classic Perlin noise
  * algorithm. But at that time, Ken Perlin had already published his Simplex
  * Noise algorithm, which was faster and less prone to build artefacts.
  * I wanted that!
  *
  * This was the birth of the RNG worker, the *R*andom *N*umber *G*enerator.
  * Later I added name generation, which I needed for another project. And as
  * both &quot;Name&quot; and &quot;Noise&quot; start with an 'N', everything
  * was fine. Now it also includes hash generation, so it is *N*Value now.
  * 
  * To speed things up in a game, I added the SCT worker, the *S*ine *C*osine
  * *T*able. And as I do not want to write the same stuff again and again, the
  * PAH worker, the *P*rogram *A*rgument *H*andler followed.
  *
  * For a project that needed thousands of objects of the same type, I started
  * inventing the container classes. The standard containers were too slow and
  * using them with multithreading was a real pain. Well, it was only C++03/tr1
  * back then! And adding a dependency to BOOST to a library was a big no-no!
  *
  * So the containers here can handle pointers just fine and are internally
  * threadsafe. But they lack a lot of the convenience the standard containers
  * offer, so don't use them unless you have a real gain!
  *
  * The whole project started as JBoH, just a bunch of headers, which shot over
  * its possibilities rather quickly. But it wasn't until 2012 that I changed
  * the whole project into a real library. Since then the project paused every
  * now and so often, as I have very limited spare time, and am maintaining
  * other projects as well. But the pwxLib is still an active project, and will
  * remain to be so for a long time.
  * 
  * @section CopyLic Copyright and license
  *
  * (c) 2007 - 2019 PrydeWorX
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
  * 0.8.7.2   2013-07-29  sed, PrydeWorX  Optimized open hash table, added tools/MathHelpers.h
  * 0.8.7.3   2013-08-25  sed, PrydeWorX  Changed TSet to use a TChainHash for lookup and optimized all containers.
  *                                       pwxLibNG now supports Spinlocks without yield() and using mutexes via Makefile.
  * 0.8.7.4   2013-08-27  sed, PrydeWorX  Imported pwxLib CWaveColor.
  * 0.8.7.5   2013-11-04  sed, PrydeWorX  Imported pwxLib CArgHandler
  * 0.8.8.0   2015-08-13  sed, PrydeWorX  Reordered and modernized
  * 0.8.9     2018-12-13  sed, PrydeWorX  First release on GitHub.
  * 0.8.9.1   2019-03-14  sed, PrydeWorX  Layout of the files resetted and reordered. Doxygen config fixed.
  * 0.8.9.2   2019-05-16  sed, PrydeWorX  Doxygen documentation (almost) completed, at least doxygen is happy now.
  @endverbatim
**/


#include "PBasic"
#include "PContainers"
#include "PTools"
#include "PUtils"
#include "PWorkers"


#endif // PWX_LIBPWX_PWX_H_INCLUDED
