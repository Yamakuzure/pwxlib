#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "pwx/types/CLockable.h"

namespace pwx
{

/** @brief Default ctor
  */
CLockable::CLockable() noexcept
 :	CL_Lock_Count(0),
	CL_Do_Locking(true),
	CL_Lock(ATOMIC_FLAG_INIT),
	CL_Thread_ID(std::thread::id())
{ /* --- nothing to do here. ---*/ }


/** @brief Copy ctor
  *
  * Nothing to do, all objects have their private locking.
  */
CLockable::CLockable (const CLockable&) noexcept
{ /* --- nothing to do here. ---*/ }



/** @brief ~CLockable
  *
  * The default dtor will try to unlock a held mutex until it succeeds.
  */
CLockable::~CLockable() noexcept
{
	clear_locks();
	// the return value is unimportant, we can't do
	// anything about it in the middle of a dtor anyway.
}


/** @brief Assignment operator
  *
  * Nothing to do, all objects have their private mutex.
  */
CLockable &CLockable::operator= (const CLockable&) noexcept
{ return *this; }

} // namespace pwx
