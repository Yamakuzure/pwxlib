#ifndef PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED
#define PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED 1
#pragma once

/** @file CArgHandler.h
  *
  * @brief Declaration of CArgHandler
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

#include "arg_handler/CArgCallback.h"
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

	typedef VArgTargetBase            data_t;     //!< Default data type is VArgTargetBase - handles all derivates
	typedef std::string               key_t;      //!< Default key type is std::string
	typedef TChainHash<key_t, data_t> hash_t;     //!< Shortcut to TChainhash with key_t and data_t
	typedef sArgError                 error_t;    //!< Shortcut to sArgError
	typedef TQueue<error_t>           errlist_t;  //!< Shortcut to TQueue for error_t
	typedef TQueue<data_t>            arg_list_t; //!< Shortcut to TQueue for data_t
	typedef arg_list_t::elem_t        arg_elem_t; //!< Shortcut to the arg_list_t element type


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, bool* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, int8_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, uint8_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, int16_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, uint16_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, int32_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, uint32_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, int64_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, uint64_t* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, float* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, double* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, long double* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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
	  * @param[in] set_type Determines the type of a set target, default is STT_OVERWRITE.
	  * @return true if an argument was added, false otherwise.
	**/
	bool		addArg		  ( char const* arg_short, char const* arg_long,
	                                    eArgTargetType arg_type, std::string* arg_target,
	                                    char const* arg_desc, char const* param_name,
	                                    eArgSetType set_type = STT_OVERWRITE );


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


	/** @brief get Help string with short/long argument plus parameter
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
	  * In the default configuration the returned string is simply both
	  * arguments and the possible parameter concatenated together.
	  * Several optional parameters can be used to configure the output:
	  * <BR />
	  * The optional parameter @a length can be used to set a minimum
	  * length. If this is set to a value different than 0, the string
	  * returned will be filled with preceeding spaces.<BR />
	  * The parameter @a argSep defines a separator character, defaulting
	  * to ' ', that is placed between the short and long argument.<BR />
	  * The parameter @a paramSep defines a separator character, defaulting
	  * to ' ', that is placed between the argument(s) and the parameter.
	  * <BR />
	  * If @a emptyLine is set to true, the string will consist of spaces
	  * and possibly the separators, according to @a autoSep, only.<BR />
	  * This parameter @a autoSep, that defaults to true, can be changed
	  * to false to have @a argSep added in any way. By default separators
	  * are only added if @a emptyLine is false. But even if @a emptyLine
	  * is false, @a argSep is only added if there are both a short and
	  * long version of the argument, and @a paramSep if there is a
	  * parameter. If @a autoSep is false, @a argSep is placed in any way
	  * and @a paramSep if there is at least one argument known that has
	  * a parameter.<BR />
	  * The last parameter @a autoSpace defaults to false and causes, if
	  * changed to true, that extra space characters are placed around the
	  * separators if they are something else than a space character.
	  *
	  * @param[in] argument Either short or long argument to search for.
	  * @param[in] length Minimum length of each line.
	  * @param[in] indent Prefix these number of spaces. Default: 0
	  * @param[in] argSep Optional separator between the short and the long argument. Default: ' '
	  * @param[in] paramSep Optional separator between the arguments and the description. Default: ' '
	  * @param[in] emptyLine If set to true, a blank line with possible separators is returned.
	  * @param[in] autoSep Only display a separator if there is a value on each side. Default: true
	  * @param[in] autoSpace If set to true, extra spaces will be added around the separators if they are not spaces. Default: false
	  * @return a string with the help text or an error message if @a argument could not be found.
	  */
	std::string getHelpArg    ( char const* argument, size_t length = 0, size_t indent = 0,
	                            char argSep = 0x20, char paramSep = 0x20,
	                            bool emptyLine = false, bool autoSep = true,
	                            bool autoSpace = false ) const noexcept;


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
	  * In the default configuration the description string is simply
	  * returned in whole. Several optional parameters can be used to
	  * configure the output:<BR />
	  * The optional parameter @a pos can be used to define a different
	  * starting position. If @a pos is larger than the size of the
	  * description, an empty string is returned.<BR />
	  * The optional parameter @a length can be used to set a maximum
	  * length. If this is set to a value different than 0, the string
	  * returned will be constructed from @a pos forward to the last
	  * space character found before @a length is reached, or up to
	  * @a length if either the next character is a space, or no space
	  * could be found before @a length was reached.<BR />
	  * With @a descSep a separator character, that defaults to ' ',
	  * can be set to be added in front of the description returned.
	  * <BR />
	  * The parameter @a autoSep, that defaults to true, can be changed
	  * to false to have @a descSep added in any way. By default the
	  * separator character is only added as is on the first line, that
	  * is @a pos is set to 0, and is substituted with a space character
	  * on all other lines.<BR />
	  * The last parameter @a autoSpace defaults to false and causes, if
	  * changed to true, that extra space characters are placed around the
	  * @a descSep separator if it is set to something else than a space
	  * character.
	  *
	  * @param[in] argument Either short or long argument to search for.
	  * @param[in,out] pos Pointer to the starting position of the description, receives end position, default nullptr.
	  * @param[in] length Maximum length of the description, default all.
	  * @param[in] descSep Optional separator before the description. Default: ' '
	  * @param[in] autoSep Only display a separator if there is a value on each side. Default: true
	  * @param[in] autoSpace If set to true, extra spaces will be added around the separators if they are not spaces. Default: false
	  * @return a string with the description or an error message if @a argument could not be found.
	  */
	std::string getHelpDesc   ( char const* argument, size_t* pos = nullptr, size_t length = 0,
	                            char descSep = 0x20, bool autoSep = true,
	                            bool autoSpace = false ) const noexcept;


	/** @brief get a formatted help string
	  *
	  * This method returns a formatted help string with both
	  * the short and long argument plus parameter are
	  * formatted according to the found maximum lengths of all
	  * arguments and parameters.
	  *
	  * If the full string does not fit into a line of the length
	  * @a length, the description will be broken into several
	  * lines. The minimum number of description characters, however,
	  * is eight, even if this results in lines longer than @a length.
	  *
	  * If no such argument exists, the returned string will contain a
	  * message that the argument couldn't be found. This condition is
	  * tested with an assert.
	  *
	  * If @a argument is nullptr or an empty string, an error message will
	  * be returned. This condition is tested with an assert, too.
	  *
	  * With the three characters @a argSep, @a paramSep and @a
	  * descSep a seperator between the short and long argument,
	  * the argument(s) and the parameter, and between the argument
	  * string and the description can be set.
	  *
	  * If a description is long enough to cause line breakse, the set
	  * separators are not displayed and substituted by a space
	  * character. If the separators shall be displayed on every line,
	  * @a autoSep can be set to false.
	  *
	  * The last parameter @a autoSpace defaults to false and causes, if
	  * changed to true, that extra space characters are placed around the
	  * separators if they are something else than a space character.
	  *
	  * @param[in] argument Either short or long argument to search for.
	  * @param[in] length Maximum length of each line.
	  * @param[in] indent Prefix these number of spaces. Default: 0
	  * @param[in] argSep Optional separator between the short and the long argument. Default: ' '
	  * @param[in] paramSep Optional separator between the arguments and the description. Default: ' '
	  * @param[in] descSep Optional separator before the description. Default: ' '
	  * @param[in] autoSep Only display a separator if there is a value on each side. Default: true
	  * @param[in] autoSpace If set to true, extra spaces will be added around the separators if they are not spaces. Default: false
	  * @return a string with the description or an error message if @a argument could not be found.
	  */
	std::string getHelpStr    ( char const* argument, size_t length, size_t indent = 0,
	                            char argSep = 0x20, char paramSep = 0x20,
	                            char descSep = 0x20, bool autoSep = true,
	                            bool autoSpace = false ) const noexcept;


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


private:

	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	 */

	/// @brief do nothing with the element.
	/// (or cleaning short args, destroys targets of long args!)
	static void do_not_destroy( data_t* ) { }


	/// @brief get target for short/long arg @arg or return nullptr if not found
	data_t* getTarget  ( char const* arg )                       const noexcept;


	/// @brief store argc/argv in pass_cnt/pass_args values
	void    passThrough( const int32_t argc, char const** argv )       noexcept;


	/** @brief try to split @a arg into short arguments and push on @a arg_list.
	  * arg_list will be empty if false is returned.
	  * @return true if all characters could be found, false otherwise.
	**/
	bool    uncombine  ( char const* arg, arg_list_t& arg_list );


	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	 */

	errlist_t errlist;     //!< stores generated error messages
	hash_t    longArgs;    //!< stores targets using their long argument as key
	size_t    maxLongLen;  //!< longest "long" argument size
	size_t    maxParamLen; //!< longest parameter name size
	size_t    maxShortLen; //!< longest "short" argument size
	char***   pass_args;   //!< The target to store arguments to pass through
	char*     pass_init;   //!< The character sequence starting the pass through distribution
	int32_t*  pass_cnt;    //!< The target to store the number of passed through arguments
	char*     prgCall;     //!< If set, argv[0] containing the program call is stored in here.
	hash_t    shortArgs;   //!< stores targets using their short argument as key

};

} // namespace pwx

#endif // PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED

