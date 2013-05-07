#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include <pwx/types/CLockable.h>

namespace pwx
{

/** @brief Default ctor
  */
CLockable::CLockable() noexcept
{ /* --- nothing to do here. ---*/ }


/** @brief Copy ctor
  *
  * All objects have their private locking.
  * Only the state whether to actually do the locking is copied.
  */
CLockable::CLockable (const CLockable &src) noexcept
 :	CL_Do_Locking(ATOMIC_VAR_INIT(src.CL_Do_Locking.load(PWX_MEMORDER_ACQUIRE)))
{ /* --- nothing to do here. ---*/ }



/** @brief ~CLockable
  *
  * The default dtor will try to unlock a held mutex until it succeeds.
  */
CLockable::~CLockable() noexcept
{
	DEBUG_LOCK_STATE("~CLockable", this, this)
	clear_locks();
	// the return value is unimportant, we can't do
	// anything about it in the middle of a dtor anyway.
}


/** @brief Assignment operator
  *
  * All objects have their private locking.
  * Only the state whether to actually do the locking is copied.
  */
CLockable &CLockable::operator= (const CLockable &src) noexcept
{
	CL_Do_Locking.store(src.CL_Do_Locking.load(PWX_MEMORDER_ACQUIRE), PWX_MEMORDER_RELEASE);
	return *this;
}

} // namespace pwx
