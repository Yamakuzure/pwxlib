#ifndef PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED
#define PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED 1
#pragma once

/** @file VArgTargetBase.h
  *
  * @brief Simple basic struct holding all non-templated members for TArgTarget
  *
  * (c) 2007 - 2020 PrydeWorX
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
#include "arg_handler/eArgType.h"
#include "arg_handler/eArgErrorNumber.h"


/// @namespace pwx
namespace pwx {


/** @brief Callback function the parameter of the option is given to
  *
  * The callback function will get two arguments.
  * The first will be the long
  * or short name of the argument encountered, or an empty string if neither
  * is set.
  * The second will be the parameter encountered, or an empty string if
  * no parameter was defined for this argument.
**/
typedef void ( *arg_cb_t )( char const*, char const* );


/** @struct VArgTargetBase
  * @brief data collection of non-templated values for TArgTarget
**/
struct VArgTargetBase {
	// Members
	std::string    arg_short;    //!< Short argument (one character) variant
	std::string    arg_long;     //!< Long argument (multiple characters) variant
	std::string    description;  //!< Description of the argument
	std::string    param_name;   //!< Parameter name/description
	arg_cb_t       arg_callback; //!< Optional callback function to use
	eArgTargetType arg_type;     //!< eArgTargetType describing what to do with the target
	eArgType       set_type;     //!< eArgType describing what to do with multiple occurances


	/** @brief default ctor
	  *
	  * No parameter check, the caller must ensure consistent
	  * values that make the instance usable.
	  *
	  * If neither @a short_ nor @a long_ are given, the argument is considered to be positional.
	  * If @a cb_ is set, @a name_ determines whether a parameter is expected or not.
	  *
	  * @param[in] short_ Short argument like "-a" or "x".       (optional)
	  * @param[in] long_  Long argument like "--foo" or "-bar".  (optional)
	  * @param[in] type_  Determines what to do with the target.
	  * @param[in] set_   Rules for zero to multiple occurances
	  * @param[in] cb_    Callback function to call when the argument is processed (optional)
	  * @param[in] desc_  Help text for this argument.           (optional)
	  * @param[in] name_  Name shown in <> int the help text.    (optional)
	**/
	explicit VArgTargetBase( char const* short_, char const* long_,
	                         eArgTargetType type_, eArgType set_,
	                         arg_cb_t cb_,
	                         char const* desc_, char const* name_ ) noexcept;


	// no empty ctor
	VArgTargetBase() PWX_DELETE;

	/// @brief The destructor has nothing to do.
	virtual ~VArgTargetBase() noexcept;


	/// @brief Copy constructor
	VArgTargetBase( VArgTargetBase const& rhs ) noexcept;


	/// @brief Move constructor
	VArgTargetBase( VArgTargetBase&& rhs ) noexcept;


	/// @brief Must be defined by TArgTarget:
	virtual eArgErrorNumber process( char const* ) PWX_VIRTUAL_PURE;


	// ==== Public methods that do not need a templated value ====


	/** @brief returns true if at least one parameter was processed
	  * @return true if at least one parameter was processed, false otherwise.
	  */
	bool hasParameter  () const noexcept { return gotParameter; };


	/** @brief return true if a parameter is needed according type
	  * @return true if the type needs a parameter, false otherwise
	  */
	bool needsParameter() const noexcept;


	/// @brief Copying assignment operator
	VArgTargetBase& operator=( VArgTargetBase const& rhs ) noexcept;


	/// @brief Moving assignment operator
	VArgTargetBase& operator=( VArgTargetBase&& rhs ) noexcept;


protected:

	bool gotParameter = false; //!< Must be set to true by process() if a parameter was processed


	/** @brief process an argument parameter via callback function
	  *
	  * Simple method that calls the stored callback function with arg_short/arg_long and @a param.
	  *
	  * If no callback function was installed, then this method does silently nothing.
	  *
	  * @return AEN_OK if no exception occured.
	**/
	virtual eArgErrorNumber process_cb( char const* param );
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

