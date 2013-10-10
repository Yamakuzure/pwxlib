#ifndef PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED
#define PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED

/** @file VArgTargetBase.h
  *
  * @brief Simple basic struct holding all non-templated members for TArgTarget
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


#include <pwx/general/compiler.h>
#include <pwx/general/macros.h>
#include <pwx/types/eArgTargetType.h>
#include <pwx/types/eSetType.h>
#include <string>


namespace pwx {


/** @struct VArgTargetBase
  * @brief data collection of non-templated values for TArgTarget
**/
struct VArgTargetBase
{
	// Members
	std::string    aShort;
	std::string    aLong;
	std::string    desc;
	std::string    pName;
	eArgTargetType type;
	eArgSetType    setType;

	explicit VArgTargetBase(const char* arg_short, const char* arg_long,
				eArgTargetType arg_type,
				const char* arg_desc, const char* param_name)
			noexcept;
	VArgTargetBase(const char* arg_short, const char* arg_long,
				eArgSetType set_type,
				const char* arg_desc, const char* param_name)
			noexcept;
	virtual ~VArgTargetBase() noexcept;

	// Must be defined by TArgTarget:
	virtual void process(const char*) PWX_VIRTUAL_PURE;

	// Public methods that do not need a templated value:
	bool hasParameter  () const noexcept;
	bool needsParameter() const noexcept;

protected:

	bool gotParameter = false; //!< Must be set to true by process() if a parameter was processed
};


} // namespace pwx


#endif // PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED

