#ifndef PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED
#define PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED

/** @file VArgTargetBase.h
  *
  * @brief Simple basic struct holding all non-templated members for TArgTarget
  *
  * (c) 2007 - 2017 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
  *
  * The PrydeWorX Library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public License as
  * published by the Free Software Foundation; either version 2.1 of the
  * License, or (at your option) any later version.
  *
  * The PrydeWorX Library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public License
  * along with pwxLib; If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
**/


#include <string>

#include "compiler.h"
#include "eArgTargetType.h"
#include "eArgSetType.h"
#include "eArgErrorNumber.h"


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
	virtual eArgErrorNumber process(const char*) PWX_VIRTUAL_PURE;

	// Public methods that do not need a templated value:
	bool hasParameter  () const noexcept;
	bool needsParameter() const noexcept;

protected:

	bool gotParameter = false; //!< Must be set to true by process() if a parameter was processed
};


// Operators needed for pwx container storage:
bool operator==(const VArgTargetBase &lhs, const VArgTargetBase &rhs) noexcept;
bool operator>(const VArgTargetBase &lhs, const VArgTargetBase &rhs) noexcept;


} // namespace pwx


#endif // PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED

