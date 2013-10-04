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

	/** @brief add one command line argument to the handler using a simple target
	  *
	  * This method adds knowledge about one command line argument
	  * to the handler.
	  *
	  * There are some basic tests to ensure that the
	  * @a arg_target and the @a arg_type make sense.
	  * If they do not, bad things may happen, at least the
	  * argument might not do what you expect. This condition
	  * is tested with an assert.
	  *
	  * Both the short argument and the long argument must be
	  * unique. If a given argument is already known to the
	  * handler, it will be <B>ignored</B>! This condition is
	  * tested with an assert.
	  *
	  * Either of the arguments @a arg_short or @a arg_long
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
	  * addPassthrough() and not this one!.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[in] arg_type Determines what to do with the target.
	  * @param[out] arg_target Pointer to the value to handle.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> int the help text.
	  * @return true if an argument was added, false otherwise.
	**/
	template<typename T>
	bool addArg(const char* arg_short, const char* arg_long,
				eArgTargetType arg_type, T* arg_target,
				const char* arg_desc, const char* param_name)
	{
		// === Check arguments against double nullptr ===
		bool hasArgLong  = arg_long  && strlen(arg_long);
		bool hasArgShort = arg_short && strlen(arg_short);

		assert( (hasArgLong || hasArgShort)
			&& "ERROR: At least one of arg_short and arg_long *MUST* be a string of length>0!");
		if ( !hasArgLong && !hasArgShort )
			return false;

		// === Check target  ===
		assert( arg_target
			&& "ERROR: At least one of arg_target and arg_cb must be set!");
		if (!arg_type)
			return false;

		// === Check target type against arg_type ===
		/* The following combinations are valid:
		 *  ATT_FALSE : bool and any numeric
		 *  ATT_TRUE  : bool and any numeric
		 *  ATT_ADD   : any numeric
		 *  ATT_INC   : any numeric
		 *  ATT_SUB   : any numeric
		 *  ATT_DEC   : any numery
		 *  ATT_SET   : everything that supports operator=
		 *  ATT_CB    : everything
		 */
		assert( ( (
					// ATT_FALSE and ATT_TRUE accept bool and numeric
					( (ATT_TRUE == arg_type) || (ATT_FALSE == arg_type) )
				  && isSameType(bool, T)
				) ||
					// All accept numerics
					// Note: This includes ATT_TRUE/ATT_FALSE, so only
					//       one check is needed
					isNumericType(T)
				  ||
					// ATT_SET and ATT_CB accept everything
					( ATT_SET == arg_type )
				  ||( ATT_CB  == arg_type )
			) && "ERROR: The combination of type and arg_type is illegal!" );
		// No break in release version.

		// === Check argument against uniqueness ===
		std::string key_long (hasArgLong  ? arg_long  : "");
		std::string key_short(hasArgShort ? arg_short : "");
		bool isLongNew  = hasArgLong  ? !longArgs.exists(key_long)   : true;
		bool isShortNew = hasArgShort ? !shortArgs.exists(key_short) : true;
		assert( isShortNew && isLongNew && "ERROR: long or short argument already known!");
		if ( !isLongNew || !isShortNew )
			return false;

		// === Now create a new target and add it to the hashes ===
		data_t* new_target = nullptr;
		try {
			new_target = new TArgTarget<T>(arg_short, arg_long, arg_type,
										arg_target, arg_desc, param_name);
		} catch(std::bad_alloc &e) {
			PWX_THROW("ArgTargetCreationFailed", e.what(),
					"The creation of a new argument target failed!")
		}
		if (hasArgLong)
			PWX_TRY_PWX_FURTHER(longArgs.add(key_long, new_target))
		if (hasArgShort)
			PWX_TRY_PWX_FURTHER(shortArgs.add(key_short, new_target))

		// === Finally record length if a new maximum is found ===
		if (key_long.size() > maxLongLen)
			maxLongLen = key_long.size();
		if (param_name && (strlen(param_name) > maxParamLen))
			maxParamLen = strlen(param_name);
		if (key_short.size() > maxShortLen)
			maxShortLen = key_short.size();

		return true;
	}


	/** @brief add one command line argument to the handler using a callback function
	  *
	  * This method adds knowledge about one command line argument
	  * to the handler.
	  *
	  * Both the short argument and the long argument must be
	  * unique. If a given argument is already known to the
	  * handler, it will be <B>ignored</B>! This condition is
	  * tested with an assert.
	  *
	  * Either of the arguments @a arg_short or @a arg_long
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
	  * addPassthrough() and not this one!.
	  *
	  * @param[in] arg_short Short argument like "-a" or "x".
	  * @param[in] arg_long Long argument like "--foo" or "-bar".
	  * @param[out] arg_cb Callback function to handle argument parameters.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> int the help text.
	  * @return true if an argument was added, false otherwise.
	**/
	template<typename T>
	bool addArg(const char* arg_short, const char* arg_long,
				void (*arg_cb)(const char*, T*),
				const char* arg_desc, const char* param_name)
	{
		// === Use nullptr target for basic addition ===
		bool result = false;
		PWX_TRY_PWX_FURTHER(result = this->addArg<T>(arg_short, arg_long,
													ATT_CB, (T*)nullptr,
													arg_desc, param_name))

		// == If this argument was added, add callback function. ===
		if (result) {
			std::string key_long ((arg_long  && strlen(arg_long))  ? arg_long  : "");
			std::string key_short((arg_short && strlen(arg_short)) ? arg_short : "");

			if (key_long.length() && longArgs.exists(arg_short))
				static_cast<TArgTarget<T>&>(longArgs.getData(key_long)).setCb(arg_cb);
			else
				static_cast<TArgTarget<T>&>(shortArgs.getData(key_short)).setCb(arg_cb);
		}

		return result;
	}


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
	int32_t     parseArgs     (const int32_t argc, const char** argv) noexcept;


private:

	/* ===============================================
	 * === Private methods                         ===
	 * ===============================================
	 */

	/// @internal do nothing with the element.
	/// (or cleaning short args, destroys targets of long args!)
	static void do_not_destroy(data_t*) { }


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
	hash_t    shortArgs;   //!< stores targets using their short argument as key

};


extern CArgHandler PAH; //!< External instance of CArgHandler to be used

} // namespace pwx

#endif // PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED

