#pragma once
#ifndef PWX_LIBPWX_PWX_GENERAL_TEMPLATES_H_INCLUDED
#define PWX_LIBPWX_PWX_GENERAL_TEMPLATES_H_INCLUDED 1

/** @file templates.h
  *
  * @brief Helper templates and macros for various little issues
  *
  * (c) 2007 - 2018 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://prydeworx.com/pwxlib/
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
  * History and change log are maintained in pwx.h
**/


#include "compiler.h"


/** @namespace pwx
  *
  * @brief Name space for the PrydWorX Library - aka 'pwxLib'
  *
  * Everything declared and defined in this name space belongs to the pwxLib.
  * All classes, structs, constants, functions and macros should be documented.
  * Therefore what follows are some general notes and discussions.
  *
  * <OL>
  * <LI>Workers<br />
  * The library instantiates standard 'workers' for various tasks. There is a
  * central worker <I>RNG</I> that is used for thread safe random number, random
  * names and hash generation. <I>SCT</I> is a simple object providing sine/cosine
  * tables of variable precision.<I>PAH</I> is the program argument handler that
  * can be used to handle arguments to, and help texts from a program. Finally
  * <I>CFH</I> is a handler for configuration files of various styles like 'shell'
  * or 'ini'.<BR /><I>Note</I>: <I>CFH</I> has not been imported, yet!</LI>
  * <LI>Thread safety<br />
  * Basically there are three basic principles on how to handle multi threaded
  * environments:
  * <TABLE border='1'>
  * <TR><TH>Principle</TH><TH>Advantages</TH><TH>Disadvantages</TH></TR>
  * <TR>
  *   <TD>Users responsibility</TD>
  *   <TD><UL>
  *     <LI>Simplest interface</LI>
  *     <LI>Simplest implementation</LI>
  *     <LI>Best performance in single threaded environment</LI>
  *     <LI>No overhead</LI>
  *   </UL></TD><TD><UL>
  *     <LI>User has to know when to lock.</LI>
  *     <LI>User has to debug all issues from multi threaded usage.</LI>
  *     <LI>All locks are external, blocking all parallel work.</LI>
  *     <LI>Multi threading can become useless if the work done outside of
  * locks is too minimal.</LI>
  *   </UL></TD>
  * </TR><TR>
  *   <TD>API big locks</TD>
  *   <TD><UL>
  *     <LI>Interface stays being simple, but relies on accessors.</LI>
  *     <LI>Interface methods simply lock on all accesses.</LI>
  *     <LI>User does not need to care about anything, everything is
  * automatically thread safe.</LI>
  *   </UL></TD><TD><UL>
  *     <LI>All parallel work is automatically blocked.</LI>
  *     <LI>Same impact (and overhead) on single threaded usage.</LI>
  *     <LI>Multi threading can become useless if the work done outside of
  * locks is too minimal.</LI>
  *   </UL></TD>
  * </TR><TR>
  *   <TD>Internal locking on demand</TD>
  *   <TD><UL>
  *     <LI>Memory barriers/fences can be used instead of locks where appropriate.</LI>
  *     <LI>No user intervention needed.</LI>
  *     <LI>A lot more parallel work can be done.</LI>
  *   </UL></TD><TD><UL>
  *     <LI>More complex interface.</LI>
  *     <LI>Most complex implementation.</LI>
  *     <LI>Additional memory overhead to catch &quot;unfortunate&quot; locking order.</LI>
  *   </UL></TD>
  * </TR>
  * </TABLE>
  * Most libraries offering functionality like the pwxLib use the first principle.
  * It helps greatly to focus on functionality instead of tiny implementation details.
  * However, it is a principle where single threaded usage is often better (and faster)
  * than multi threaded. An example would be a container that has to be filled with
  * a lot items which are simply gathered. Only if the generation of those items is
  * very costly, a multi threaded design can be used to gain more performance.
  *
  * The pwxLib basically uses the third principle. But as it is not advisable, like
  * with the example above, to handle tasks that are better done in a single threaded
  * way, like they were multi threaded, all objects in the library allow to override
  * the internal locking and use the first principle instead.
  *
  * This can be done in two ways:
  * <UL><LI>All classes derived from pwx::CLockable inherit the method do_locking(bool)
  * which can be used to turn off (and later on) the locking mechanisms.</LI>
  * <LI>As the basic calls are still present, all containers defined in pwx/container
  * allow their members to be used directly. So if you just want to traverse
  * a container, you can just get the head element and then walk through the container
  * using each elements next pointer.</LI>
  * <LI>Even if the traversing of a container is needed to be thread save, all element
  * types contain a method <I>getNext()</I>, and <I>getPrev()</I> where appropriate,
  * that is thread safe.</LI></UL>
  * </OL>
**/


namespace pwx {


/** @brief compile time assertion
  *
  * This little trick is taken from Andrei Alexandrescu: 'Modern C++ Design'.
  * It can be used to check an expression at compile time.
  * Of course this does not substitute cassert completely, only expressions
  * that are known while the compiler does its work can be checked with this.
**/
template <bool> struct PWX_API TCompAssert;
template <>     struct PWX_API TCompAssert<true> {};
#define compAssert(expression) (::pwx::TCompAssert<(expression) != 0>())

} // namespace pwx
#endif // PWX_LIBPWX_PWX_GENERAL_TEMPLATES_H_INCLUDED
