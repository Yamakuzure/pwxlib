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
  * History and change log are maintained in pwx.h
**/


#include <pwx_worker_PAH.h>
#include <pwx_worker_RNG.h>
#include <pwx_worker_SCT.h>


#endif // PWX_PWX_WORKERS_H_INCLUDED

