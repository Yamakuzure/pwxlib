/***
  This file is part of the PrydeWorX Library (pwxLib).

  Copyright 2007 - 2017 Sven Eden

  The PrydeWorX Library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  The PrydeWorX Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with pwxLib; If not, see <http://www.gnu.org/licenses/>.

  History and Changelog are maintained in pwx.h
***/


#include "pwx_workers.h"


#if !defined(PWX_INITIAL_SCT_PRECISION)
#  define PWX_INITIAL_SCT_PRECISION -1
#endif // defined(PWX_INITIAL_SCT_PRECISION)


/* --- Instantiations of global workers --- */


pwx::CArgHandler  pwx::PAH;    // [P]rogram [A]rgument [H]andler
pwx::CRandom      pwx::RNG;    // [R]andom [N]-Value [G]enerator
pwx::CSinCosTable pwx::SCT(PWX_INITIAL_SCT_PRECISION); // [S]ine-/[C]osine-[T]able
