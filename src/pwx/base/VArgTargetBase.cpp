#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "VArgTargetBase.h"

namespace pwx {


/** brief default ctor
  *
  * No parameter check, the caller must ensure consistent
  * values that make the instance usable.
  *
  * @see pwx::CArgHandler::addArg()
  *
  * @param[in] arg_short Short argument like "-a" or "x".
  * @param[in] arg_long Long argument like "--foo" or "-bar".
  * @param[in] arg_type Determines what to do with the target.
  * @param[in] arg_desc Help text for this argument.
  * @param[in] param_name Name shown in <> int the help text.
**/
VArgTargetBase::VArgTargetBase(const char* arg_short, const char* arg_long,
			eArgTargetType arg_type,
			const char* arg_desc, const char* param_name)
		noexcept :
	aShort(arg_short  ? arg_short  : ""),
	aLong (arg_long   ? arg_long   : ""),
	desc  (arg_desc   ? arg_desc   : ""),
	pName (param_name ? param_name : ""),
	type  (arg_type)
{ /* nothing to do here */ }


/** @brief destructor
  * has nothing to do.
**/
VArgTargetBase::~VArgTargetBase() noexcept
{
	/* nothing to do here */
}


} // namespace pwx

