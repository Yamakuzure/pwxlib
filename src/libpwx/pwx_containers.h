#pragma once
#ifndef PWX_CONTAINERS_H_INCLUDED
#define PWX_CONTAINERS_H_INCLUDED

/** @file pwx_containers.h
  *
  * @brief This file adds all container types from pwxLib.
  *
  * This include file adds the definition of all container types currently
  * included in pwxLib. In detail these are:
  *
  * <TABLE border='1'>
  * <TR><TH>Template</TH><TH>Task</TH><TH>Include file</TH></TR>
  * <TR><TD>TChainHash</TD><TD>A Chained hash table</TD><TD>TChainHash.h</TD></TR>
  * <TR><TD>TDoubleList</TD><TD>A doubly linked list</TD><TD>TDoubleList.h</TD></TR>
  * <TR><TD>TDoubleRing</TD><TD>A doubly linked ring</TD><TD>TDoubleRing.h</TD></TR>
  * <TR><TD>TOpenHash</TD><TD>An open hash table</TD><TD>TOpenHash.h</TD></TR>
  * <TR><TD>TQueue</TD><TD>A FiFo container</TD><TD>TQueue.h</TD></TR>
  * <TR><TD>TSet</TD><TD>A unique content container</TD><TD>TSet.h</TD></TR>
  * <TR><TD>TSingleList</TD><TD>A singly linked list</TD><TD>TSingleList.h</TD></TR>
  * <TR><TD>TSingleRing</TD><TD>A singly linked ring</TD><TD>TSingleRing.h</TD></TR>
  * <TR><TD>TStack</TD><TD>A FiLo container</TD><TD>TStack.h</TD></TR>
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
  * History and change log are maintained in pwxlib.h
**/


#include "TChainHash.h"
#include "TDoubleList.h"
#include "TDoubleRing.h"
#include "TOpenHash.h"
#include "TQueue.h"
#include "pwxTSet.h"
#include "pwxTSingleList.h"
#include "pwxTSingleRing.h"
#include "pwxTStack.h"


#endif // PWX_CONTAINERS_H_INCLUDED

