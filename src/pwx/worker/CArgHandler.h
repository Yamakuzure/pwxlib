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


#include <pwx/general/compiler.h>


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
	 * === Public Constructors and destructors     ===
	 * ===============================================
	 */

	explicit CArgHandler();
	~CArgHandler() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	 */

	template<typename T>
	void addArg()
	{

	}


	void        addPassthrough(const char init_arg, int* pass_argc, char** pass_argv);
	void        clearArgs     () noexcept;
	int         getError      (const int nr) noexcept;
	int         getErrorCount () noexcept;
	const char* getErrorStr   (const int nr) noexcept;
	int         parseArgs     (const int argc, const char** argv) noexcept;

};

} // namespace pwx


extern CArgHandler PAH; //!< External instance of CArgHandler to be used


#endif // PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED

