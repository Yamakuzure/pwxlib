#pragma once
#ifndef PWX_EXCEPTION_H_INCLUDED
#define PWX_EXCEPTION_H_INCLUDED 1

/** @file Exception.h
  *
  * @brief This file defines the basic, trace generating exception for pwx projects
  *
  * (c) 2007-2012 Sven Eden, PrydeWorX
  * @author Sven Eden, PrydeWorX - Hamburg, Germany
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
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.0.1     2010-11-03  sed, PrydeWorX  First Design
  * 0.0.2     2010-11-08  sed, PrydeWorX  Added try-catch safety measures to addToTrace()
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.0.1   2011-03-06  sed, PrydeWorX  Welcome to 2011 !
  * 0.7.0.2   2011-03-15  sed, PrydeWorX  added static char buffers for information assembly
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2.1   2011-04-20  sed, PrydeWorX  Added exception naming for simpler handling.
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0.1   2011-08-02  sed, PrydeWorX  Added missing PWX_INLINE to ~Exception()
  * 0.8.0.2   2011-09-18  sed, PrydeWorX  Reverted pwx::Exception and its helper macros to local helper C-Strings, the static
  *                                       buffer is problematic pwx::Exception using objects shows up in more than one
  *                                       compilation unit.
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3.1   2012-02-05  sed, PrydeWorX  Added default includes for this file to be usable stand-alone
  * 0.8.5     2012-03-01  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5.1   2012-03-22  sed, PrydeWorX  Renamed ctor arguments, the old names shadowed the public functions
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

#include "pwxLib/internal/MSVCwarnPush.h"
#include <cstdlib> // For free
#include <cstdio>  // For sprintf
#include <cstring> // For basename
#include "pwxLib/internal/MSVCwarnPop.h"
#include "pwxLib/tools/DefaultDefines.h"

/** @namespace pwx
  *
  * @brief Namespace for everything PrydeWorX related
  *
  * All PrydeWorX projects reside in this namespace and it's sub-namespaces.
**/
namespace pwx
{


/** @brief base class for all exceptions
  *
  * This is the base class all pwx exceptions are derived of. It is meant to provide
  * a common interface to get as much information as possible.
  *
  * To make the most out of this system, the file "pwxLib/tools/DefaultDefines.h"
  * provides the macros PWX_TRY(), PWX_THROW() and PWX_THROW_FURTHER.
  *
  * name() : This will return the name of the thrown exception, all decendants have to set a name.
  * what() : This will return information about what went wrong
  * where(): This will return the name of the method that threw the exception in
  *          the style "file:line - method name"
  * desc() : This will return a description that possibly contains data about what happened
  * pfunc(): This will return the full function name and type of template parameters if applicable
  * trace(): This will return a trace of all methods this exceptions passed through. If it didn't
  *           pass through anywhere, the trace is empty.
**/
class Exception : public ::std::exception
{
public:
  explicit Exception (const char *const aName, const char *const aWhat, const char *const aWhere,
                      const char *const prettyfunc, const char *const aDesc):
    ::std::exception(), eName(aName), txtWhat(aWhat), txtWhere(aWhere), txtFunc(prettyfunc),
     txtDesc(aDesc), txtTrace("Thrown from : ")
    {
      PWX_TRY(txtTrace += txtWhere)
      PWX_CATCH_AND_FORGET(::std::exception)
    }
  Exception          (const Exception &e):
    ::std::exception(e), eName(e.name()), txtWhat(e.what()), txtWhere(e.where()),
    txtFunc(e.pfunc()), txtDesc(e.desc()), txtTrace(e.trace())  { }
  virtual const char *name()  const throw() { return eName;            }
  virtual const char *what()  const throw() { return txtWhat;          }
  virtual const char *where() const throw() { return txtWhere;         }
  virtual const char *desc()  const throw() { return txtDesc;          }
  virtual const char *pfunc() const throw() { return txtFunc;          }
  virtual const char *trace() const throw() { return txtTrace.c_str(); }
  void addToTrace (const char *aTrace)
   {
     if (txtTrace.empty())
       {
         PWX_TRY(txtTrace  = "Thrown from : "; txtTrace += txtWhere)
         PWX_CATCH_AND_FORGET(::std::exception)
       }
    PWX_TRY(txtTrace += "\n"; txtTrace += aTrace;)
    PWX_CATCH_AND_FORGET(::std::exception)
  }
  PWX_INLINE virtual ~Exception() throw();

private:
  // No empty ctor!
  Exception() PWX_UNUSED;

  // No assignement!
  Exception &operator=(Exception &src PWX_UNUSED) { return *this; }

  // Text Strings:
  const char *const eName;    //!< the name of the exception
  const char *const txtWhat;  //!< the classic what() text
  const char *const txtWhere; //!< A malloc'd C-String with "file:line - method"
  const char *const txtFunc;  //!< The result of __PRETTY_FUNC__
  const char *const txtDesc;  //!< Optional description
  ::std::string txtTrace;     //!< A trace, that can hopefully be added together
};

// The base dtor needs to do something;
Exception::~Exception() throw()
{
  // These are malloc'd strings, so free them:
  if (txtWhat)  free (const_cast<char *>(txtWhat));
  if (txtWhere) free (const_cast<char *>(txtWhere));
  if (txtDesc)  free (const_cast<char *>(txtDesc));
  // Note: They are generated by strdup() but saved as const const by the ctor.
}

} // End of namespace pwx
#endif // PWX_EXCEPTION_H_INCLUDED
