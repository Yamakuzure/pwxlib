#pragma once
#ifndef PWX_PWX_BASIC_H_INCLUDED
#define PWX_PWX_BASIC_H_INCLUDED

/** @file basic.h
  *
  * @brief This file adds the basic functionality of the pwxLib.
  *
  * This include file adds the most basic includes, that most other parts depend
  * upon. In detail these are:
  *
  * <TABLE border='1'>
  * <TR><TH>Task</TH><TH>Include file</TH></TR>
  * <TR><TD>Add CException, a basic exception class with tracing functionality</TD>
  *   <TD>CException.h</TD></TR>
  * <TR><TD>Add CLockable, a base class to make objects lockable via atomic_flag
  *   and lock counting</TD><TD>CLockable.h</TD></TR>
  * <TR><TD>Add CLockGuard, a basic RAII lock guard to lock/unlock one, two or
  *   three objects within its ctor/dtor</TD><TD>CLockGuard.h</TD></TR>
  * <TR><TD>General preprocessor macros and defines that are compiler dependent</TD>
  *   <TD>compiler.h</TD></TR>
  * <TR><TD>Debugging utilities that do nothing unless LIBPWX_DEBUG is not 0</TD>
  *   <TD>debug.h</TD></TR>
  * <TR><TD>Preprocessor macros for general usage</TD><TD>macros.h</TD></TR>
  * <TR><TD>Helper templates and macros for various little issues</TD>
  *   <TD>templates.h</TD></TR>
  * <TR><TD>Includes and typedefs for types used throughout libpwx</TD>
  *   <TD>types.h</TD></TR>
  * </TABLE>
  *
  * (c) 2007 - 2020 PrydeWorX
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
  * History and change log are maintained in pwxlib.h
**/


#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "basic/CException.h"
#include "basic/CLockable.h"
#include "basic/CLockGuard.h"
#include "basic/alloc_utils.h"
#include "basic/mem_utils.h"
#include "basic/pwx_templates.h"
#include "basic/pwx_types.h"
#include "basic/string_utils.h"


#endif // PWX_PWX_BASIC_H_INCLUDED

