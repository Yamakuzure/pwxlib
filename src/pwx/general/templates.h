#pragma once
#ifndef PWX_LIBPWX_PWX_GENERAL_TEMPLATES_H_INCLUDED
#define PWX_LIBPWX_PWX_GENERAL_TEMPLATES_H_INCLUDED 1

/** @file templates.h
  *
  * @brief Helper templates and macros for various little issues
  *
  * (c) 2007 - 2013 PrydeWorX
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
  * History and Changelog are maintained in pwx.h
**/

#include <type_traits>

/** @namespace pwx
  *
  * @brief Name space for the PrydWorX Library - aka 'pwxLib'
  *
  * Everything declared and defined in this name space belongs to the pwxLib.
  * All classes, structs, constants, functions and macros should be documented.
  * Therefore what follows are some general notes and discussions.
  *
  * <ol>
  * <li>Workers<br />
  * The library instantiates standard 'workers' for various tasks. There is a
  * central worker <i>RNG</i> that is used for thread safe random number, random
  * names and hash generation. <i>SCT</i> is a simple object providing sine/cosine
  * tables of variable precision. <i>DCF</i> is used to generate and manage data
  * containers of variable types. <i>WCH</i> is the wave color handler that can
  * transform wavelengths to RGB and vice versa. <i>PAP</i> is the program argument
  * parser that can be used to handle arguments to, and help texts from a program.
  * Finally <i>CFH</i> is a handler for configuration files of various styles like
  * 'shell' or 'ini'.</li>
  * <li>Thread safety<br />
  * Basically there are three basic principles on how to handle multi threaded
  * environments:
  * <table border='1'>
  * <tr><th>Principle</th><th>Advantages</th><th>Disadvantages</th></tr>
  * <tr>
  *   <td>Users responsibility</td>
  *   <td><ul>
  *     <li>Simplest interface</li>
  *     <li>Simplest implementation</li>
  *     <li>Best performance in single threaded environment</li>
  *     <li>No overhead</li>
  *   </ul></td><td><ul>
  *     <li>User has to know when to lock</li>
  *     <li>User has to debug all issues from multi threaded usage</li>
  *     <li>All locks are external, blocking all parallel work</li>
  *     <li>Multi threading can become useless if the work done outside of
  * locks is too minimal.</li>
  *   </ul></td>
  * </tr><tr>
  *   <td>API big locks</td>
  *   <td><ul>
  *     <li>Interface stays being simple, but relies on accessors.</li>
  *     <li>Interface methods simply lock on all accesses</li>
  *     <li>User does not need to care about anything, everything is
  * automatically thread safe.</li>
  *   </ul></td><td><ul>
  *     <li>All parallel work is automatically blocked</li>
  *     <li>Same impact (and overhead) on single threaded usage</li>
  *     <li>Multi threading can become useless if the work done outside of
  * locks is too minimal.</li>
  *   </ul></td>
  * </tr><tr>
  *   <td>Internal locking on demand</td>
  *   <td><ul>
  *     <li>Memory barriers/fences can be used instead of locks where appropriate</li>
  *     <li>No user intervention needed</li>
  *     <li>A lot more parallel work can be done</li>
  *   </ul></td><td><ul>
  *     <li>More complex interface</li>
  *     <li>Most complex implementation</li>
  *     <li>Additional memory overhead to catch &quot;unfortunate&quot; locking order</li>
  *   </ul></td>
  * </tr>
  * </table>
  * Most libraries offering functionality like the pwxLib use the first principle.
  * It helps greatly to focus on functionality instead of tiny implementation details.
  * However it is a principle where single threaded usage is often better (and faster)
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
  * <ul><li>All classes derived from pwx::CLockable inherit the method do_locking(bool)
  * which can be used to turn off (and later on) the locking mechanisms.</li>
  * <li>As the basic calls are still present, all containers defined in pwx/container
  * allow their members to be used directly. So if you just want to &quot;march&quot;
  * a container, you can just get the head element and then walk through the container
  * using each elements next pointer.</li></ul>
  * </ol>
**/
namespace pwx
{

/** @brief compile time assertion
  *
  * This little trick is taken from Andrei Alexandrescu: 'Modern C++ Design'.
  * It can be used to check an expression at compile time.
  * Of course this does not substitute cassert completely, only expressions
  * that are known while the compiler does its work can be checked with this.
**/
template <bool> struct PWX_API pwxCompAssert;
template <>     struct PWX_API pwxCompAssert<true> {};
#define compAssert(expression) (::pwx::pwxCompAssert<(expression) != 0>())

} // namespace pwx
#endif // PWX_LIBPWX_PWX_GENERAL_TEMPLATES_H_INCLUDED
