#pragma once
#ifndef PWX_PWX_BASIC_H_INCLUDED
#define PWX_PWX_BASIC_H_INCLUDED

/** @file pwx_basic.h
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
  * <TR><TD>Debugging utilities that do nothing unless LIBPWX_DEBUG is defined</TD>
  *   <TD>debug.h</TD></TR>
  * <TR><TD>Preprocessor macros for general usage</TD><TD>macros.h</TD></TR>
  * <TR><TD>Helper templates and macros for various little issues</TD>
  *   <TD>templates.h</TD></TR>
  * <TR><TD>Includes and typedefs for types used throughout libpwx</TD>
  *   <TD>types.h</TD></TR>
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
  * History and Changelog are maintained in pwx.h
**/


#include "CException.h"
#include "CLockable.h"
#include "CLockGuard.h"
#include "compiler.h"
#include "debug.h"
#include "macros.h"
#include "templates.h"
#include "types.h"


#endif // PWX_PWX_BASIC_H_INCLUDED

