#ifndef PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED

/** @file TArgTarget.h
  *
  * @brief Declaration of the TArgTarget struct template
  *
  * (c) 2007 - 2013 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
  *
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
**/

#include <pwx/base/VArgTargetBase.h>

namespace pwx {


/** @struct TArgTarget
  * @brief data collection to define one command line argument
**/
template<typename T>
struct TArgTarget : public VArgTargetBase
{
	// Members
	T*             target;

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
	  * @param[out] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> int the help text.
	**/
	explicit TArgTarget(const char* arg_short, const char* arg_long,
				eArgTargetType arg_type, T* arg_target,
				const char* arg_desc, const char* param_name)
			noexcept :
		VArgTargetBase(arg_short, arg_long, arg_type, arg_desc, param_name),
		target(arg_target),
		cb    (nullptr)
	{ /* nothing to do here */ }

	/** @brief destructor
	  * has nothing to do.
	**/
	virtual ~TArgTarget() noexcept { /* nothing to do here */ }

	// processor
	eArgErrorNumber process(const char* param)
	{
		eArgErrorNumber argErrno = AEN_OK;



		return argErrno;
	}

	// set callback function
	void setCb(void (*arg_cb)(const char*, T*))
	{
		cb = arg_cb;
	}

private:

	// callback
	void (*cb)(const char*, T*);
};


} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED

