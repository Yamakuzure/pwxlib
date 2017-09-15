#include "pwx.h"


#if !defined(PWX_INITIAL_SCT_PRECISION)
#  define PWX_INITIAL_SCT_PRECISION -1
#endif // defined(PWX_INITIAL_SCT_PRECISION)


/* --- Instantiations of global workers --- */


pwx::CArgHandler  pwx::PAH;    // [P]wxlib [A]rgument [H]andler
pwx::CRandom      pwx::RNG;    // [R]andom [N]-Value [G]enerator
pwx::CSinCosTable pwx::SCT(PWX_INITIAL_SCT_PRECISION); // [S]ine-/[C]osine-[T]able
