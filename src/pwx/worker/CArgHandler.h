#ifndef PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED
#define PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED

/** @file CArgHandler.h
  *
  * @brief Declaration of CArgHandler
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


#include <pwx/types/TArgTarget.h>
#include <pwx/types/CArgCallback.h>
#include <pwx/types/sArgError.h>
#include <pwx/types/CException.h>

#if defined(PWX_EXPORTS)
#  undef PWX_EXPORTS
#  define PWX_REDEF_EXPORTS 1
# endif

#include <pwx/container/TQueue.h>
#include <pwx/container/TChainHash.h>

#if defined(PWX_REDEF_EXPORTS)
#  define PWX_EXPORTS 1
#  undef PWX_REDEF_EXPORTS
# endif


namespace pwx {


/** @class CArgHandler
  * @brief Handler for command line arguments
  *
  * This class is intended to be instantiated exactly once
  * which is done in the library, namely pwx::PAH.
  *
  * Usage is quite simple.
  * <LIST>
  * <LI>Call <I>addArg()</>I for each argument your program
  * should support.</LI>
  * <LI>Use <I>addPassthrough()</I> if command line arguments
  * must be preserved for later distribution to another
  * program.</LI>
  * <LI>Call <I>parseArgs(argc, argv)</I> to have them applied
  * to your arguments.</LI>
  * <LI>With <I>getErrorCount()</I> the number of errors
  * encountered can be retrieved.</LI>
  * <LI><I>getError(nr)</I> returns the error number for error
  * number <I>nr</I>, <I>getErrorStr(nr)</I> returns a string
  * with an error text.</LI>
  * <LI><I>getHelpArg(arg)</I> returns a string with the short
  * and/or long argument and parameter if needed.</LI>
  * <LI><I>getHelpDesc(arg)</I> returns a string with the
  * argument descriptions.</LI>
  * <LI><I>getHelpStr(arg, length)</LI> returns a string with
  * both the short and/or long argument plus parameter and
  * description. This string is formatted using the found
  * maximum lengths of short arguments, long argumens and
  * parameter neames according to the given line length. If the
  * resulting string is too long, it will line break.</LI>
  * <LI>Finally <I>clearArgs()</I> frees all allocated memory.
  * </LI></LIST>
  *
  * Supported types:<BR />
  * The system directly supports bool, [u]int{8,16,32,64}_t,
  * float, [long] double and std::string targets.<BR />
  * If a different type is to be handled, a callback function
  * must be installed that converts a const char* parameter
  * into the target type and handles the processing.<BR />
  * Further it is advised to use a callback function if an
  * argument should be able to receive and store more than
  * one parameter.<BR />
  * For this reason there are two different kinds of the
  * addArg() function. One to set a target type and a target
  * pointer, the other to install a callback function.
  *
  * Adding a target pointer using <I>addArg()</I><BR />
  * There are some basic tests to ensure that the
  * <I>arg_target</I> and the <I>arg_type</I> make sense.
  * If they do not, bad things may happen, at least the
  * argument might not do what you expect. This condition
  * is tested with an assert.
  *
  * Both the short argument and the long argument must be
  * unique. If a given argument is already known to the
  * handler, it will be <B>ignored</B>! This condition is
  * tested with an assert.
  *
  * Either of the arguments <I>arg_short</I> or <I>arg_long</I>
  * can be nullptr, but not both. If both are set to
  * nullptr, the method does <B>nothing</B>! This condition
  * is tested with an assert.
  *
  * If the creation of an argument target instance fails, a
  * pwx::CException with the name "ArgTargetCreationFailed" is
  * thrown. Argument targets are organized using pwx::TChainHash,
  * if the creation of a hash element fails, the thrown
  * exception has the name "ElementCreationFailed".
  *
  * If you need to pass arguments to a called process,
  * add the marker separating the command line arguments
  * from the called process arguments with the method
  * <I>addPassthrough()</I> and not <I>addArg()</I>.
  *
  * Adding a callback function using <I>addArg()</I><BR />
  * Here the same rules apply. The callback function is
  * supposed to work as follows:<BR />
  * The callback function will receive the long argument, if
  * set, otherwise the short argument as a first parameter.
  * The second parameter will be the command line parameter(s)
  * that follow(s) the argument as a const char*.
**/
class PWX_API CArgHandler
{
public:

	/* ===============================================
	 * === Public types                            ===
	 * ===============================================
	 */

	typedef VArgTargetBase            data_t;
	typedef std::string               key_t;
	typedef TChainHash<key_t, data_t> hash_t;
	typedef sArgError                 error_t;
	typedef TQueue<error_t>           errlist_t;

	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	 */

	explicit CArgHandler() noexcept;
	~CArgHandler() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	 */

	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, bool* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, int8_t* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, uint8_t* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, int16_t* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, uint16_t* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, int32_t* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, uint32_t* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, int64_t* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, uint64_t* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, float* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, double* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, long double* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   eArgTargetType arg_type, std::string* arg_target,
							   const char* arg_desc, const char* param_name);
	bool		addArg		  (const char* arg_short, const char* arg_long,
							   void (*arg_cb)(const char*, const char*),
							   const char* arg_desc, const char* param_name);
	void        addPassthrough(const char* init_arg, int32_t* pass_argc, char*** pass_argv);
	void        clearArgs     () noexcept;
	int32_t     getError      (const int32_t nr) const noexcept;
	int32_t     getErrorCount () const noexcept;
	const char* getErrorStr   (const int32_t nr) const noexcept;
	std::string getHelpArg    (const char* argument, size_t length = 0, size_t indent = 0,
							   char argSep = 0x20, char paramSep = 0x20,
							   bool emptyLine = false, bool autoSep = true,
							   bool autoSpace = false) const noexcept;
	std::string getHelpDesc   (const char* argument, size_t* pos = nullptr, size_t length = 0,
							   char descSep = 0x20, bool autoSep = true,
							   bool autoSpace = false) const noexcept;
	std::string getHelpStr    (const char* argument, size_t length, size_t indent = 0,
							   char argSep = 0x20, char paramSep = 0x20,
							   char descSep = 0x20, bool autoSep = true,
							   bool autoSpace = false) const noexcept;
	const char* getPrgCall    () const noexcept;
	int32_t     parseArgs     (const int32_t argc, const char* argv[]) noexcept;


private:

	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	 */

	/// @internal do nothing with the element.
	/// (or cleaning short args, destroys targets of long args!)
	static void do_not_destroy(data_t*) { }

	data_t* getTarget  (const char* arg)                       const noexcept;
	void    passThrough(const int32_t argc, const char** argv)       noexcept;


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


extern CArgHandler PAH; //!< External instance of CArgHandler to be used

} // namespace pwx

#endif // PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED

