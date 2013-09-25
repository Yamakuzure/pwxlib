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
#include <pwx/types/CException.h>
#include <pwx/container/TChainHash.h>


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

	typedef std::string               key_t;
	typedef VArgTargetBase            data_t;
	typedef TChainHash<key_t, data_t> hash_t;


	/* ===============================================
	 * === Public Constructors and destructors     ===
	 * ===============================================
	 */

	explicit CArgHandler();
	~CArgHandler() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	 */

	/** @brief add one command line argument to the handler
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
	  * Set either @a arg_target or @a arg_cb. If both are set,
	  * the callback function is called <I>after</I> the
	  * @a arg_target is handled according to @a arg_type. If
	  * the @a arg_type is <I>ATT_CB</I>, the @a arg_target is
	  * ignored and only @a arg_cb is called.
	  * If both @a arg_target and @a arg_cb are set to nullptr,
	  * the method does <B>nothing</B>! This condition is
	  * tested with an assert.
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
	  * @param[out] arg_cb Callback function to handle argument parameters.
	  * @param[in] arg_desc Help text for this argument.
	  * @param[in] param_name Name shown in <> int the help text.
	**/
	template<typename T>
	void addArg(const char* arg_short, const char* arg_long,
				eArgTargetType arg_type, T* arg_target,
				void (*arg_cb)(const char*, T*),
				const char* arg_desc, const char* param_name)
	{
		// === Check arguments against double nullptr ===
		assert( (arg_short || arg_long)
			&&  ((arg_short && strlen(arg_short))
			  || (arg_long  && strlen(arg_long)))
			&& "ERROR: At least one of arg_short and arg_long *MUST* be a string of length>0!");
		if ( (!arg_short || !strlen(arg_short)) && (!arg_long || !strlen(arg_long)) )
			return;

		// === Check target and cb against double nullptr ===
		assert( (arg_target || arg_cb)
			&& "ERROR: At least one of arg_target and arg_cb must be set!");
		if (!arg_type && !arg_cb)
			return;

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


	}


	void        addPassthrough(const char init_arg, int* pass_argc, char** pass_argv);
	void        clearArgs     () noexcept;
	int         getError      (const int nr) noexcept;
	int         getErrorCount () noexcept;
	const char* getErrorStr   (const int nr) noexcept;
	int         parseArgs     (const int argc, const char** argv) noexcept;

};


extern CArgHandler PAH; //!< External instance of CArgHandler to be used

} // namespace pwx

#endif // PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED

