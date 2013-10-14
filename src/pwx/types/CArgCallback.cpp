#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "CArgCallback.h"
#include "CException.h"


/** brief default ctor
  *
  * No parameter check, the caller must ensure consistent
  * values that make the instance usable.
  *
  * @see pwx::CArgHandler::addArg()
  *
  * @param[in] arg_short Short argument like "-a" or "x".
  * @param[in] arg_long Long argument like "--foo" or "-bar".
  * @param[out] arg_cb Pointer to the callback function to use.
  * @param[in] arg_desc Help text for this argument.
  * @param[in] param_name Name shown in <> int the help text.
**/
explicit CArgCallback::CArgCallback(const char* arg_short, const char* arg_long,
			void (*arg_cb)(const char*, const const char*),
			const char* arg_desc, const char* param_name)
		noexcept :
	VArgTargetBase(arg_short, arg_long, ATT_CB, arg_desc, param_name),
	cb    (arg_cb)
{
	/* nothing to do here */
}


/** @brief destructor
  * has nothing to do.
**/
virtual CArgCallback::~CArgCallback() noexcept
{
	/* nothing to do here */
}


/** @brief process an argument parameter
  *
  * Simple method that calls the stored callback function with @a param.
  *
  * If no callback function was installed using the constructor,
  * this method does silently nothing.
  *
  * @return AEN_OK if the callback function was set.
**/
eArgErrorNumber CArgCallback::process(const char *param) const noexcept
{
	if (cb) cb(aLong.size() ? aLong.c_str() : aShort.c_str(), param);

	return AEN_OK;
}
