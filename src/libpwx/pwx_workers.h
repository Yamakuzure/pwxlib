#pragma once
#ifndef PWX_PWX_WORKERS_H_INCLUDED
#define PWX_PWX_WORKERS_H_INCLUDED

/** @file pwx_workers.h
  *
  * @brief This file makes the workers (PAH, RNG, SCT) available.
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


#endif // PWX_PWX_WORKERS_H_INCLUDED

