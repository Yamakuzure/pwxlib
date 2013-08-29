#pragma once
#ifndef PWX_LIBPWX_PWX_TYPES_CEXCEPTION_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_CEXCEPTION_H_INCLUDED 1

/** @file CException.h
  *
  * @brief Declaration of CException, a tracing exception
  *
  * CException serves the purpose of a base class that allows to trace the
  * path an exception takes until it is caught. To enable the tracing the
  * macros (see pwx/general/macros.h) PWX_THROW and PWX_THROW_FURTHER have
  * to be used.
  *
  * (c) 2007 - 2013 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         yamakuzure@users.sourceforge.net
  *         http://pwxlib.sourceforge.net
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
#include <string>
#include <exception>


namespace pwx
{

/** @class CException
  * @brief Basic exception class with tracing functionality
  *
  * This class is meant to provide a tracing exception to get as much information as possible.
  *
  * To make the most out of this system, the file "pwx/general/macros.h" provides many macros
  * to try, catch and throw further exceptions with tracing informations
  *
  * name() : Return the name of the thrown exception.
  * what() : Return information about what went wrong
  * where(): Return the name of the method that threw the exception in the style "file:line - method name"
  * desc() : Return a description that possibly contains data about what happened
  * pfunc(): Return the full function name and type of template parameters if applicable
  * trace(): Return a trace of all methods this exceptions passed through. If it didn't pass through anywhere,
  *          the trace is empty.
**/
class PWX_API CException : public std::exception
{

public:

	/* ===============================================
	 * === Public Constructors and destructor      ===
	 * ===============================================
	*/

	explicit
	CException (const char* const name_, const char* const what_, const char* const where_,
				const char* const func_, const char* const desc_) noexcept;
	CException (const CException &src) noexcept;
	CException () PWX_DELETE;          // No empty ctor!
	virtual ~CException() noexcept;


	/* ===============================================
	 * === Public methods                          ===
	 * ===============================================
	*/

	const char* name       ()                   const noexcept;
	const char* what       ()                   const noexcept;
	const char* where      ()                   const noexcept;
	const char* desc       ()                   const noexcept;
	const char* pfunc      ()                   const noexcept;
	const char* trace      ()                   const noexcept;
	void        addToTrace (const char* trace_)       noexcept;


	/* ===============================================
	 * === Public operators                        ===
	 * ===============================================
	*/

	CException &operator= (const CException &e) PWX_DELETE; // No assignment


private:

	/* ===============================================
	 * === Private members                         ===
	 * ===============================================
	*/

	const char *const txtName;  //!< the name of the exception
	const char *const txtWhat;  //!< the classic what() text
	const char *const txtWhere; //!< A malloc'd C-String with "file:line - method"
	const char *const txtFunc;  //!< The result of __PRETTY_FUNC__
	const char *const txtDesc;  //!< Optional description
	std::string txtTrace;     //!< A trace, that can hopefully be added together
};


} // namespace pwx
#endif // PWX_LIBPWX_PWX_TYPES_CEXCEPTION_H_INCLUDED


