#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "VArgTargetBase.h"

namespace pwx {


/** brief default ctor
  *
  * No parameter check, the caller must ensure consistent
  * values that make the instance usable. If @a arg_type
  * is type ATT_SET, the behaviour defaults to STT_OVERWRITE.
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
	aShort (arg_short  ? arg_short  : ""),
	aLong  (arg_long   ? arg_long   : ""),
	desc   (arg_desc   ? arg_desc   : ""),
	pName  (param_name ? param_name : ""),
	type   (arg_type),
	setType(STT_OVERWRITE)
{ /* nothing to do here */ }


/** brief alternative ctor to alter ATT_SET behaviour.
  *
  * No parameter check, the caller must ensure consistent
  * values that make the instance usable.
  * This is a special constructor that produces an ATT_SET
  * target with variable behaviour when more than one parameter
  * is to be processed.
  *
  * Note: The default is to overwrite the set paraemeter on each
  * processing(). If you do not want to change this, the default
  * constructor should be used instead. It will be called by this
  * one anyway.
  *
  * @see pwx::CArgHandler::addArg()
  * @see pwx::eArgSetType
  *
  * @param[in] arg_short Short argument like "-a" or "x".
  * @param[in] arg_long Long argument like "--foo" or "-bar".
  * @param[in] set_type Determines what to do if more than one parameter is processed.
  * @param[in] arg_desc Help text for this argument.
  * @param[in] param_name Name shown in <> int the help text.
**/
VArgTargetBase::VArgTargetBase(const char* arg_short, const char* arg_long,
			eArgSetType set_type,
			const char* arg_desc, const char* param_name)
		noexcept :
	VArgTargetBase(arg_short, arg_long, ATT_SET, arg_desc, param_name),
	setType(set_type)
{ /* nothing to do here */ }


/** @brief destructor
  * has nothing to do.
**/
VArgTargetBase::~VArgTargetBase() noexcept
{
	/* nothing to do here */
}


/** @brief returns true if at least one parameter was processed
  * @return true if at least one parameter was processed, false otherwise.
  */
bool VArgTargetBase::hasParameter() const noexcept
{
	return gotParameter;
}


/** @brief return true if a parameter is needed according type
  * @return true if the type needs a aprameter, false otehrwise
  */
bool VArgTargetBase::needsParameter() const noexcept
{
	if ( (ATT_ADD == type)
	  || (ATT_SUB == type)
	  || (ATT_SET == type)
	  || (ATT_CB  == type) )
		return true;
	return false;
}

} // namespace pwx

