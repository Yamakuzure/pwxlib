#ifndef PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED
#define PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED 1
#pragma once

/** @file CArgHandler.h
  *
  * @brief Declaration of CArgHandler
  *
  * (c) 2007 - 2020 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
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
#include "basic/pwx_macros.h"

#include "arg_handler/sArgError.h"
#include "arg_handler/TArgTarget.h"
#include "basic/CException.h"
#include "container/TChainHash.h"
#include "container/TQueue.h"


/// @namespace pwx
namespace pwx {

/** @class CArgHandler PArgHandler <PArgHandler>
  * @brief Handler for command line arguments
  *
  * This class is intended to be instantiated exactly once
  * which is done in the library, namely pwx::PAH.
  *
  * Usage is quite simple.
  *
  * 1. Call `addArg()` for each argument your program
  * should support.
  * 2. Use `addPassthrough()` if command line arguments
  * must be preserved for later distribution to another
  * program.
  * Be aware, though, that passed arguments are malloc'd
  * C-Strings that you have to free yourself.
  * 3. Call `parseArgs(argc, argv)` to have them applied
  * to your arguments.
  * 4. With `getErrorCount()` the number of errors
  * encountered can be retrieved.
  * 5. `getError(nr)` returns the error number for error
  * number `nr`, `getErrorStr(nr)` returns a string
  * with an error text.
  * 6. `getHelpArg(arg)` returns a string with the short
  * and/or long argument and parameter if needed.
  * 7. `getHelpDesc(arg)` returns a string with the
  * argument descriptions.
  * 8. `getHelpStr(arg, length)` returns a string with
  * both the short and/or long argument plus parameter and
  * description. This string is formatted using the found
  * maximum lengths of short arguments, long arguments and
  * parameter names according to the given line length. If the
  * resulting string is too long, it will line break.
  * 9. Finally `clearArgs()` frees all allocated memory.
  *
  * Supported types:
  * The system directly supports bool, [u]int{8,16,32,64}_t,
  * float, [long] double and std::string targets.
  * If a different type is to be handled, a callback function
  * must be installed that converts a char const* parameter
  * into the target type and handles the processing.
  * Further it is advised to use a callback function if an
  * argument should be able to receive and store more than
  * one parameter.
  * For this reason there are two different kinds of the
  * addArg() function. One to set a target type and a target
  * pointer, the other to install a callback function.
  *
  * Adding a target pointer using `addArg()`
  * There are some basic tests to ensure that the
  * `arg_target` and the `arg_type` make sense.
  * If they do not, bad things may happen, at least the
  * argument might not do what you expect. This condition
  * is tested with an assertion.
  *
  * Both the short argument and the long argument must be
  * unique. If a given argument is already known to the
  * handler, it will be **ignored**! This condition is
  * tested with an assertion.
  *
  * Either of the arguments `arg_short` or `arg_long`
  * can be nullptr, but not both. If both are set to
  * nullptr, the method does **nothing**! This condition
  * is tested with an assert.
  *
  * If the creation of an argument target instance fails, a
  * pwx::CException with the name "ArgTargetCreationFailed" is
  * thrown. Argument targets are organized using pwx::TChainHash.
  * If the creation of a hash element fails, the thrown
  * exception has the name "ElementCreationFailed".
  *
  * If you need to pass arguments to a called process,
  * add the marker separating the command line arguments
  * from the called process arguments with the method
  * `addPassthrough()` and not `addArg()`.
  *
  * Adding a callback function using `addArg()`
  * Here the same rules apply. The callback function is
  * supposed to work as follows:
  * The callback function will receive the long argument, if
  * set, otherwise the short argument as a first parameter.
  * The second parameter will be the command line parameter(s)
  * that follow(s) the argument as a char const*.
**/
class PWX_API CArgHandler {
public:

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	 */

	typedef VArgTargetBase              data_t;        //!< Default data type is VArgTargetBase - handles all derivates
	typedef std::string                 key_t;         //!< Default key type is std::string
	typedef THashElement<key_t, data_t> elem_t;        //!< The element for the hash containers
	typedef TChainHash<key_t, data_t>   hash_t;        //!< Shortcut to TChainhash with key_t and data_t
	typedef sArgError                   error_t;       //!< Shortcut to sArgError
	typedef TQueue<error_t>             errlist_t;     //!< Shortcut to TQueue for error_t
	typedef TQueue<data_t>              arg_queue_t;   //!< Shortcut to TQueue for data_t
	typedef arg_queue_t::elem_t         arg_elem_t;    //!< Shortcut to the arg_queue_t element type
	typedef TQueue<std::string>         param_queue_t; //!< Shortcut to TQueue for std::string
	typedef param_queue_t::elem_t       param_elem_t;  //!< Shortcut to the param_queue_t element type


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	 */

	/// @brief default empty ctor
	explicit CArgHandler() noexcept;

	/// @brief default dtor
	~CArgHandler() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	 */

	/** @brief Add a predefined TArgTarget as pointer
	  *
	  * @param[in] arg  The TArgTarget instance to add
	  * @return true if an argument was added, false otherwise.
	**/
	template<typename T>
	bool addArg( TArgTarget<T>* arg ) {
		assert( arg && "arg is nullptr" );

		if ( nullptr == arg )
			PWX_THROW("ArgTargetCreationFailed", "arg is nullptr",
			          "addArg( arg ) called with nullptr argument");

		bool hasLong    = arg->arg_long.length();
		bool hasShort   = arg->arg_short.length();
		bool hasParam   = arg->param_name.length();
		bool isLongNew  = hasLong  ? !longArgs.exists( arg->arg_long )   : true;
		bool isShortNew = hasShort ? !shortArgs.exists( arg->arg_short ) : true;

		assert( ( hasLong || hasParam || hasShort ) && "Either short, long or param must be set!");
		if ( !(hasLong || hasParam || hasShort) )
			PWX_THROW( "ArgTargetCreationFailed", "arg empty",
			           "Neither short nor long argument or a parameter name were given" );

		if ( !(isLongNew && isShortNew) )
			PWX_THROW( "ArgTargetCreationFailed", "arg exists",
			           "Short and/or long argument is already registered" );


		// If we have both short and long arguments, the target must be copied.
		// Otherwise clearing one list later will invalidate the pointer in the
		// other and kaboom.
		if ( hasLong || hasShort ) {
			if ( hasLong )
				PWX_TRY_PWX_FURTHER( longArgs.add( arg->arg_long, arg ) );

			if ( hasShort ) {
				if ( hasLong ) {
					elem_t new_element( *( longArgs.get( arg->arg_long ) ) );
					new_element.key = arg->arg_short;
					PWX_TRY_PWX_FURTHER( shortArgs.add( new_element ) );
				} else {
					PWX_TRY_PWX_FURTHER( shortArgs.add( arg->arg_short, arg ) );
				}
			}
		} else
			PWX_TRY_PWX_FURTHER( posQueue.push( arg ) );

		// === Finally record length if a new maximum is found ===
		if ( arg->arg_long.size() > maxLongLen )
			maxLongLen = arg->arg_long.size();
		if ( arg->param_name.size() > maxParamLen )
			maxParamLen = arg->param_name.size();
		if ( arg->arg_short.size() > maxShortLen )
			maxShortLen = arg->arg_short.size();

		return true;
	}


	/** @brief Add a predefined TArgTarget by copying
	  *
	  * @param[in] arg  The TArgTarget instance to add
	  * @return true if an argument was added, false otherwise.
	**/
	template<typename T>
	bool addArg( TArgTarget<T> const& arg ) {
		TArgTarget<T>* new_target = nullptr;

		try {
			new_target = new TArgTarget<T>( arg );
		} catch( std::bad_alloc& e )
			PWX_THROW( "ArgTargetCreationFailed", e.what(),
			          "The creation of a new argument target failed!" );

		return addArg( new_target );
	}


	/** @brief Add a predefined TArgTarget by moving
	  *
	  * @param[in] arg  The TArgTarget instance to move in
	  * @return true if an argument was added, false otherwise.
	**/
	template<typename T>
	bool addArg( TArgTarget<T>&& arg ) {
		TArgTarget<T>* new_target = nullptr;

		try {
			new_target = new TArgTarget<T>( arg );
		} catch( std::bad_alloc& e )
			PWX_THROW( "ArgTargetCreationFailed", e.what(),
			          "The creation of a new argument target failed!" );

		return addArg( new_target );
	}


	/** @brief Add an argument for a bool target
	  *
	  * This is the wrapper for target type bool.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, bool* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for an int8_t target
	  *
	  * This is the wrapper for target type int8_t.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, int8_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for an uint8_t target
	  *
	  * This is the wrapper for target type uint8_t.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, uint8_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for an int16_t target
	  *
	  * This is the wrapper for target type int16_t.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, int16_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for an uint16_t target
	  *
	  * This is the wrapper for target type uint16_t.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, uint16_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for an int32_t target
	  *
	  * This is the wrapper for target type int32_t.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, int32_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for an uint32_t target
	  *
	  * This is the wrapper for target type uint32_t.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, uint32_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for an int64_t target
	  *
	  * This is the wrapper for target type int64_t.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, int64_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for an uint64_t target
	  *
	  * This is the wrapper for target type uint64_t.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, uint64_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for a float target
	  *
	  * This is the wrapper for target type float.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, float* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for a double target
	  *
	  * This is the wrapper for target type double.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, double* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for a long double target
	  *
	  * This is the wrapper for target type long double.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, long double* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument for an std::string target
	  *
	  * This is the wrapper for target type std::string.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[in] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, std::string* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgType set_type = AT_ZERO_OR_MANY );


	/** @brief Add an argument that uses a callback function - convenience wrapper
	 *
	 * This is a convenience wrapper for installing a callback function. Although
	 * @a arg_type is not needed, it is helpful to have this variant, so in a list
	 * of addArg() calls, the callback arguments do not stand out this excessively.
	 *
	 * Further it might be useful for source analysis, where someone might grep for
	 * target types.
	 *
	 * @param[in] arg_short Short argument like "-a" or "x".
	 * @param[in] arg_long Long argument like "--foo" or "-bar".
	 * @param[in] arg_type Determines what to do with the target.
	 * @param[in] arg_cb Pointer to the callback function to install.
	 * @param[in] arg_desc Help text for this argument.
	 * @param[in] param_name Name shown in <> in the help text.
	 * @return true if an argument was added, false otherwise.
	 **/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type,
	                                    void ( *arg_cb )( char const*, char const* ),
	                                    char const* arg_desc, char const* param_name );


	/** @brief Add an argument that uses a callback function
	  *
	  * This is the wrapper for installing a callback function.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_cb Pointer to the callback function to install.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> in the help text.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    void ( *arg_cb )( char const*, char const* ),
	                                    char const* arg_desc, char const* param_name );


	/** @brief add a pass through system
	  *
	  * This method sets an identifier using @a init_arg, that
	  * identifies a point in the argument list that is followed
	  * by arguments that are not to be processed.
	  * Instead, the following arguments are stored in @a pass_argc
	  * and @a pass_argv.
	  *
	  * If any of the three parameters is nullptr, a pwx::Cexception
	  * with the name "IllegalParameters" is thrown.
	  *
	  * If @a init_arg points to an empty string, <B>all</B>
	  * parameters are passed through.
	  *
	  * @a pass_argc will receive the number of entries in @a pass_argv
	  * received, when the real processing is done. Therefore *pass_argv
	  * will result in an array of *pass_argc malloc'd C-Strings, that
	  * you'll have to free yourself.<BR />
	  * Please note, that this implies that *pass_argv must be nullptr when
	  * the arguments are processed, as a new array will be malloc'd.
	  *
	  * @param[in] init_arg the character sequence identifying the passthrough start
	  * @param[out] pass_argc pointer receiving the number of passed arguments
	  * @param[out] pass_argv pointer receiving the passed through arguments
	  */
	void        addPassthrough( char const* init_arg, int32_t* pass_argc, char*** pass_argv );


	/// @brief releases allocated memory
	void        clearArgs     () noexcept;


	/** @brief get error number with number @a nr
	  *
	  * This method returns the error number of the error
	  * number @a nr. The first error is error 1.
	  *
	  * If no such error exists, the method returns 0.
	  *
	  * @param[in] nr Number of the error to retrieve the error number from
	  * @return retrieved error number or 0, if no such error exists.
	  */
	int32_t     getError      ( const int32_t nr ) const noexcept;


	/** @brief return the number of found parsing errors
	  * @return the number of parsing errors
	  */
	int32_t     getErrorCount () const noexcept;


	/** @brief get error text with number @a nr
	  *
	  * This method returns the error text of the error
	  * number @a nr. The first error is error 1.
	  *
	  * If no such error exists, the method returns nullptr.
	  *
	  * @param[in] nr Number of the error to retrieve the error text from
	  * @return retrieved error text or nullptr, if no such error exists.
	  */
	char const* getErrorStr   ( const int32_t nr ) const noexcept;


	/** @brief get Help string for a short/long argument without description
	  *
	  * This method returns a string of the form
	  * "<[short arg] [long arg]> [parameter]" for the argument identified
	  * by @a argument.
	  *
	  * If no such argument exists, the returned string will contain a
	  * message that the argument couldn't be found. This condition is
	  * tested with an assert.
	  *
	  * If @a argument is nullptr or an empty string, an error message will
	  * be returned. This condition is tested with an assert, too.
	  *
	  * You can change the way the string is formatted with the `setHelpParams()`
	  * method.
	  *
	  * If @a emptyLine is set to true, the string will consist of spaces
	  * and possibly the separators.
	  *
	  * @param[in] argument Either short or long argument to search for.
	  * @param[in] emptyLine If set to true, a blank line with possible separators is returned.
	  * @return a string with the help text or an error message if @a argument could not be found.
	  */
	std::string& getHelpArg( char const* argument, bool emptyLine = false ) const noexcept;


	/** @brief get Help string for a positional parameter without description
	  *
	  * This method returns a string of the form "<parameter>" for the parameter
	  * identified by @a position.
	  *
	  * If @a position is out of bounds, an error message will be returned.
	  * This condition is tested with an assert, too.
	  *
	  * You can change the way the string is formatted with the `setHelpParams()`
	  * method.
	  *
	  * If @a emptyLine is set to true, the string will consist of spaces
	  * and possibly the separators.
	  *
	  * @param[in] position The position of the parameter, starting with 1.
	  * @param[in] emptyLine If set to true, a blank line with possible separators is returned.
	  * @return a string with the help text or an error message if @a parameter could not be found.
	  */
	std::string& getHelpArg( uint32_t position, bool emptyLine = false ) const noexcept;


	/** @brief get Help string with the description of an argument
	  *
	  * This method returns the description of an argument identified
	  * by @a argument.
	  *
	  * If no such argument exists, the returned string will contain a
	  * message that the argument couldn't be found. This condition is
	  * tested with an assert.
	  *
	  * If @a argument is nullptr or an empty string, an error message will
	  * be returned. This condition is tested with an assert, too.
	  *
	  * You can change the way the string is formatted with the `setHelpParams()`
	  * method.
	  *
	  * @param[in] argument Either short or long argument to search for.
	  * @param[in,out] pos Get string from this position on, or all if *pos is zero. Writes final position into *pos.
	  * @param[in] length How long the resulting string is allowed to be as a maximum.
	  * @return a string with the description or an error message if @a argument could not be found.
	  */
	std::string& getHelpDesc ( char const* argument, size_t* pos, size_t length ) const noexcept;


	/** @brief get Help string with the description of a positional parameter
	  *
	  * This method returns the description of the parameter identified by @a position.
	  *
	  * If no such parameter exists, the returned string will contain a
	  * message that the parameter could not be found. This condition is
	  * tested with an assert.
	  *
	  * If @a position is out of bounds, an error message will be returned.
	  * This condition is tested with an assert, too.
	  *
	  * You can change the way the string is formatted with the `setHelpParams()`
	  * method.
	  *
	  * @param[in] position The position of the parameter, starting with 1.
	  * @param[in,out] pos Get string from this position on, or all if *pos is zero. Writes final position into *pos.
	  * @param[in] length How long the resulting string is allowed to be as a maximum.
	  * @return a string with the description or an error message if @a position could not be found.
	  */
	std::string& getHelpDesc( uint32_t position, size_t* pos, size_t length ) const noexcept;


	/** @brief get a formatted help string for a long or short argument
	  *
	  * This method returns a formatted help string with both
	  * the short and long argument plus parameter formatted according to
	  * the found maximum lengths of all arguments and parameters.
	  *
	  * You can change the way the string is formatted with the `setHelpParams()`
	  * method.
	  *
	  * If the full string does not fit into a line of the set line length, the
	  * description will be broken into several lines. The minimum number of
	  * description characters, however, is eight, even if this results in lines
	  * longer than @a length.
	  *
	  * If no such argument exists, the returned string will contain a
	  * message that the argument could not be found. This condition is
	  * tested with an assert.
	  *
	  * If @a argument is nullptr or an empty string, an error message will
	  * be returned. This condition is tested with an assert, too.
	  *
	  * @param[in] argument Either short or long argument to search for.
	  * @return a string with the description or an error message if @a argument could not be found.
	  */
	std::string& getHelpStr( char const* argument ) const noexcept;


	/** @brief get a formatted help string for a positional parameter
	  *
	  * This method returns a formatted help string with the positional parameter
	  * formatted according to the found maximum lengths of all arguments and
	  * parameters.
	  *
	  * You can change the way the string is formatted with the `setHelpParams()`
	  * method.
	  *
	  * If the full string does not fit into a line of the set line length, the
	  * description will be broken into several lines. The minimum number of
	  * description characters, however, is eight, even if this results in lines
	  * longer than @a length.
	  *
	  * If no such parameter exists, the returned string will contain a
	  * message that the parameter could not be found. This condition is
	  * tested with an assert.
	  *
	  * If @a parameter is out of bounds, an error message will be returned.
	  * This condition is tested with an assert, too.
	  *
	  * @param[in] position The position of the parameter, starting with 1.
	  * @return a string with the description or an error message if @a position could not be found.
	  */
	std::string& getHelpStr( uint32_t position ) const noexcept;


	/// @return the program call, if <I>parseArgs()</I> has found one, nullptr otherwise.
	char const* getPrgCall    () const noexcept;


	/** @brief parse given arguments
	  *
	  * This method parses the given array @a argv of C-Strings
	  * with @a argc expected entries.
	  *
	  * <B>Important</B>: It is assumed that argv[0] contains the program
	  * call. This can be retrieved using the method <I>getPrgCall()</I>
	  * after the arguments have been parsed.
	  *
	  * @param[in] argc The number of strings in @a argv.
	  * @param[in] argv array of C-Strings.
	  * @return Number of errors encountered.
	  */
	int32_t     parseArgs     ( const int32_t argc, char* argv[] ) noexcept;


	/** @brief parse given arguments
	  *
	  * This method parses the given array @a argv of C-Strings
	  * with @a argc expected entries.
	  *
	  * <B>Important</B>: It is assumed that argv[0] contains the program
	  * call. This can be retrieved using the method <I>getPrgCall()</I>
	  * after the arguments have been parsed.
	  *
	  * @param[in] argc The number of strings in @a argv.
	  * @param[in] argv array of C-Strings.
	  * @return Number of errors encountered.
	  */
	int32_t     parseArgs     ( const int32_t argc, char const* argv[] ) noexcept;


	/// @brief reset all stored targets as if they were never processed.
	void resetTargets() noexcept;


	/** @brief Set help string parameters parameter
	  *
	  * This method sets new display parameters after which the methods
	  * `getHelpArg()`, `getHelpDesc()` and `getHelpStr()` format their
	  * output.
	  *
	  * The parameter @a length can be used to set a minimum length. It's
	  * default value is 80 characters.
	  * If this is set to a value different than 0, the string returned by
	  * the mentioned methods will be filled with preceeding spaces.<BR />
	  * The parameter @a argSep defines a separator character, defaulting
	  * to ' ', that is placed between the short and long argument.<BR />
	  * The parameter @a paramSep defines a separator character, defaulting
	  * to ' ', that is placed between the argument(s) and the parameter.
	  *
	  * The parameter @a autoSep, that defaults to true, can be changed
	  * to false to have @a argSep added in any way, even if one of the
	  * get methods is asked to return an empty line. By default separators
	  * are only added on not empty lines. But even if an empty line is
	  * requested, @a argSep is only added if there are both a short and
	  * long version of the argument, and @a paramSep if there is a
	  * parameter. If @a autoSep is false, @a argSep is placed in any way
	  * and @a paramSep if there is at least one argument known that has
	  * a parameter.<BR />
	  * The last parameter @a autoSpace defaults to false and causes, if
	  * changed to true, that extra space characters are placed around the
	  * separators if they are something else than a space character.
	  *
	  * @param[in] length Minimum length of each line.
	  * @param[in] indent Prefix these number of spaces. Default: 0
	  * @param[in] argSep Optional separator between the short and the long argument. Default: ' '
	  * @param[in] descSep Optional separator between the argument display and its description. Default: ' '
	  * @param[in] paramSep Optional separator between the arguments and the description. Default: ' '
	  * @param[in] autoSep Only display a separator if there is a value on each side. Default: true
	  * @param[in] autoSpace If set to true, extra spaces will be added around the separators if they are not spaces. Default: false
	  */
	void setHelpParams( size_t length, size_t indent = 0,
	                    char argSep = 0x20, char descSep = 0x20, char paramSep = 0x20,
	                    bool autoSep = true, bool autoSpace = false ) noexcept;

private:

	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	 */

        /// @brief do nothing with the element.
        /// (or cleaning short args, destroys targets of long args!)
        static void do_not_destroy( data_t * ) { }


        /// @brief Internal helper to format the left side of an argument help string
        std::string& formatHelpArg( data_t* target, bool emptyLine ) const noexcept;


        /// @brief Internal helper to format the right side of an argument help string
        std::string& formatHelpDesc( data_t* target, size_t* pos, size_t length ) const noexcept;


        /// @brief Internal helper to format the full help string
        std::string& formatHelpStr( data_t* target ) const noexcept;


	/// @brief get target for short/long argument @a arg or return nullptr if not found
	data_t* getTarget( char const* arg ) const noexcept;


	/// @brief get target for position @a pos or return nullptr if not found
	data_t* getTarget( uint32_t pos ) const noexcept;


	/// @brief store argc/argv in pass_cnt/pass_args values
	void passThrough( const int32_t argc, char const** argv ) noexcept;


	/// @brief Helper to process one target, noting down caught errors.
	void procTarget( data_t* target, char const* param );

	/** @brief try to split @a arg into short arguments and push on @a arg_list.
	  * arg_list will be empty if false is returned.
	  * @return true if all characters could be found, false otherwise.
	**/
	bool uncombine( char const* arg, arg_queue_t& arg_list, uint32_t& dash_count );


	/// @brief Update left/right side of help strings according to what was recorded
	void updateLayout( void ) const noexcept;


	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	 */

	errlist_t   errlist;                 //!< stores generated error messages
	char        helpArgSep    = 0x20;    //!< Separator between the short and the long argument.
	bool        helpAutoSep   = true;    //!< Only display a separator if there is a value on each side.
	bool        helpAutoSpace = false;   //!< Add extra spaces around separators if they are not spaces.
	char        helpDescSep   = 0x20;    //!< Separator between argument display and description.
	size_t      helpIndent    = 0;       //!< Prefix each help line with this number of spaces.
	size_t      helpLength    = 80;      //!< Minimum length of each help line length
	char        helpParSep    = 0x20;    //!< Separator between the arguments and the description.
	mutable
	size_t      helpSizeLeft  = 0;       //!< Size of the left size of argument/parameter help strings
	mutable
	size_t      helpSizeRight = 0;       //!< Size of the right size of argument/parameter help strings
	hash_t      longArgs;                //!< stores targets using their long argument as key
	size_t      maxLongLen    = 0;       //!< longest "long" argument size
	size_t      maxParamLen   = 0;       //!< longest parameter name size
	size_t      maxShortLen   = 0;       //!< longest "short" argument size
	char***     pass_args     = nullptr; //!< The target to store arguments to pass through
	int32_t*    pass_cnt      = nullptr; //!< The target to store the number of passed through arguments
	char*       pass_init     = nullptr; //!< The character sequence starting the pass through distribution
	arg_queue_t posQueue;                //!< Queue of positional arguments
	char*       prgCall       = nullptr; //!< If set, argv[0] containing the program call is stored in here.
	hash_t      shortArgs;               //!< stores targets using their short argument as key
};

} // namespace pwx

#endif // PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED

