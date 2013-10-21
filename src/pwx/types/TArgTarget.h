#ifndef PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED

/** @file TArgTarget.h
  *
  * @brief Declaration of the TArgTarget class template
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
#include <pwx/types/CException.h>

namespace pwx {


/** @struct TArgTarget
  * @brief Definition of one command line argument using a templated member
  *
  * This is meant to be used with a target pointer. If a callback function
  * is to be used, use CArgCallback instead.
**/
template<typename T>
class TArgTarget : public VArgTargetBase
{

public:

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
		target(arg_target)
	{ /* nothing to do here */ }


	/** @brief destructor
	  * has nothing to do.
	**/
	virtual ~TArgTarget() noexcept { /* nothing to do here */ }


	/** @brief process one command line parameter or argument occurence
	  *
	  * This method is called whenever the type of an argument needs
	  * direct action, or a parameter to that argument is met.
	  *
	  * @param[in] param The parameter found or nullptr if no parameter is needed.
	**/
	virtual eArgErrorNumber process(const char* param);


private:

	// Members
	T* target;

	// === par_to_val conversion chain prototype ===
	void par_to_val(T* tgt, const char* param) noexcept;
};


// === Processing specializations prototypes ===
template<>
eArgErrorNumber TArgTarget<std::string>::process(const char* param);


// === Generic non-specialized handler ===
template<typename T>
eArgErrorNumber TArgTarget<T>::process(const char* param)
{
	eArgErrorNumber argErrno = AEN_OK;

	T val = (T)0;
	if (param && strlen(param))
		this->par_to_val(&val, param);

	switch(this->type) {
		case ATT_FALSE:
			*target = (T)false;
			break;
		case ATT_TRUE:
			*target = (T)true;
			break;
		case ATT_INC:
			*target += (T)1;
			break;
		case ATT_DEC:
			*target -= (T)1;
			break;
		case ATT_ADD:
			*target  += val;
			break;
		case ATT_SUB:
			*target  -= val;
			break;
		case ATT_SET:
			// This needs handling for all three set types:
			if ( (STT_OVERWRITE == this->setType)
			  || (!this->gotParameter) ) {
				*target = val;
				this->gotParameter = true;
			} else if (STT_ERROR == this->setType)
				argErrno = AEN_MULTIPLE_SET_PARAM;
			// Last possibility is STT_IGNORE, which is, well, ignored. ;)
			break;
		case ATT_CB:
			PWX_THROW("UnhandledTargetType", "ATT_CB not supported, use CArgCallback instead!", "")
			break;
		default:
			PWX_THROW("UnhandledTargetType", "The given target type is not implemented, yet!", "")
	}

	return argErrno;
}


// === par_to_val specializations prototypes ===
template<>
void TArgTarget<bool>::par_to_val(bool* tgt, const char* param) noexcept;
template<>
void TArgTarget<int8_t>::par_to_val(int8_t* tgt, const char* param) noexcept;
template<>
void TArgTarget<int16_t>::par_to_val(int16_t* tgt, const char* param) noexcept;
template<>
void TArgTarget<int32_t>::par_to_val(int32_t* tgt, const char* param) noexcept;
template<>
void TArgTarget<int64_t>::par_to_val(int64_t* tgt, const char* param) noexcept;
template<>
void TArgTarget<uint8_t>::par_to_val(uint8_t* tgt, const char* param) noexcept;
template<>
void TArgTarget<uint16_t>::par_to_val(uint16_t* tgt, const char* param) noexcept;
template<>
void TArgTarget<uint32_t>::par_to_val(uint32_t* tgt, const char* param) noexcept;
template<>
void TArgTarget<uint64_t>::par_to_val(uint64_t* tgt, const char* param) noexcept;
template<>
void TArgTarget<float>::par_to_val(float* tgt, const char* param) noexcept;
template<>
void TArgTarget<double>::par_to_val(double* tgt, const char* param) noexcept;
template<>
void TArgTarget<long double>::par_to_val(long double* tgt, const char* param) noexcept;

} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_TARGTARGET_H_INCLUDED

