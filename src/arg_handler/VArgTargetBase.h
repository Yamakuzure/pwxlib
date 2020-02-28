#ifndef PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED
#define PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED 1
#pragma once

/** @file VArgTargetBase.h
  *
  * @brief Simple basic struct holding all non-templated members for TArgTarget
  *
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  * History and change log are maintained in pwxlib.h
**/


#include <string>

#include "basic/pwx_compiler.h"

#include "arg_handler/eArgTargetType.h"
#include "arg_handler/eArgSetType.h"
#include "arg_handler/eArgErrorNumber.h"


/// @namespace pwx
namespace pwx {


/** @struct VArgTargetBase
  * @brief data collection of non-templated values for TArgTarget
**/
struct VArgTargetBase {
	// Members
	std::string    aShort;   //!< Short argument (one character) variant
	std::string    aLong;    //!< Long argument (multiple characters) variant
	std::string    desc;     //!< Description of the argument
	std::string    pName;    //!< Parameter name/description
	eArgTargetType type;     //!< eArgTargetType describing what to do with the target
	eArgSetType    setType;  //!< eArgSetType describing what to do with ATT_SET targets


	/** @brief default ctor
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
	explicit VArgTargetBase( char const* arg_short, char const* arg_long,
	                         eArgTargetType arg_type,
	                         char const* arg_desc, char const* param_name )
	noexcept;


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
	VArgTargetBase( char const* arg_short, char const* arg_long,
	                eArgSetType set_type,
	                char const* arg_desc, char const* param_name )
	noexcept;


	/// @brief The destructor has nothing to do.
	virtual ~VArgTargetBase() noexcept;

	/// @brief Must be defined by TArgTarget:
	virtual eArgErrorNumber process( char const* ) PWX_VIRTUAL_PURE;


	// Public methods that do not need a templated value:

	/** @brief returns true if at least one parameter was processed
	  * @return true if at least one parameter was processed, false otherwise.
	  */
	bool hasParameter  () const noexcept;


	/** @brief return true if a parameter is needed according type
	  * @return true if the type needs a aprameter, false otehrwise
	  */
	bool needsParameter() const noexcept;

protected:

	bool gotParameter = false; //!< Must be set to true by process() if a parameter was processed
};


// Operators needed for pwx container storage:

/** @brief return true if @a lhs and @a rhs are the same arguments.
  * @param[in] lhs left hand side instance.
  * @param[in] rhs rhs hand side instance.
  * @return true if the short/long args are equal, false otherwise.
**/
bool operator==( const VArgTargetBase& lhs, const VArgTargetBase& rhs ) noexcept;


/** @brief return true if @a lhs is "greater" than @a rhs according to the arguments.
  * @param[in] lhs left hand side instance.
  * @param[in] rhs rhs hand side instance.
  * @return true if the short/long args of lhs are greater than the args of rhs.
**/
bool operator>( const VArgTargetBase& lhs, const VArgTargetBase& rhs ) noexcept;


} // namespace pwx


#endif // PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED

