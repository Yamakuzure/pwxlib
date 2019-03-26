#pragma once
#ifndef PWX_LIBPWX_PWX_TYPES_CEXCEPTION_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_CEXCEPTION_H_INCLUDED 1

/** @file CException.h
  *
  * @brief Declaration of CException, a tracing exception
  *
  * CException serves the purpose of a base class that allows to trace the
  * path an exception takes until it is caught. To enable the tracing the
  * macros (see pwx/general/pwx_macros.h) PWX_THROW and PWX_THROW_FURTHER have
  * to be used.
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


#include <exception>
#include <string>

#include "pwx_compiler.h"


namespace pwx {


/** @class CException PException <PException>
  * @brief Basic exception class with tracing functionality
  *
  * This class is meant to provide a tracing exception to get as much information as possible.
  *
  * To make the most out of this system, the file "pwxlib/pwx_macros.h" provides many macros
  * to try, catch and throw further exceptions with tracing information.
  *
  * name() : Return the name of the thrown exception.
  * what() : Return information about what went wrong
  * where(): Return the name of the method that threw the exception in the style "file:line - method name"
  * desc() : Return a description that possibly contains data about what happened
  * pfunc(): Return the full function name and type of template parameters if applicable
  * trace(): Return a trace of all methods this exceptions passed through. If it didn't pass through anywhere,
  *          the trace is empty.
**/
class PWX_API CException : public std::exception {

  public:

    /* ===============================================
     * === Public Constructors and destructor      ===
     * ===============================================
    */

    explicit
    CException ( char const* const name_, char const* const what_, char const* const where_,
                 char const* const func_, char const* const desc_ ) noexcept;
    CException ( const CException& src ) noexcept;
    CException () PWX_DELETE;          // No empty ctor!
    virtual ~CException() noexcept;


    /* ===============================================
     * === Public methods                          ===
     * ===============================================
    */

    char const* name       ()                   const noexcept;
    char const* what       ()                   const noexcept;
    char const* where      ()                   const noexcept;
    char const* desc       ()                   const noexcept;
    char const* pfunc      ()                   const noexcept;
    char const* trace      ()                   const noexcept;
    void        addToTrace ( char const* trace_ )       noexcept;


    /* ===============================================
     * === Public operators                        ===
     * ===============================================
    */

    CException& operator= ( const CException& e ) PWX_DELETE; // No assignment


  private:

    /* ===============================================
     * === Private members                         ===
     * ===============================================
    */

    char const* const txtName;  //!< the name of the exception
    char const* const txtWhat;  //!< the classic what() text
    char const* const txtWhere; //!< A malloc'd C-String with "file:line - method"
    char const* const txtFunc;  //!< The result of __PRETTY_FUNC__
    char const* const txtDesc;  //!< Optional description
    std::string txtTrace;     //!< A trace, that can hopefully be added together
};


} // namespace pwx
#endif // PWX_LIBPWX_PWX_TYPES_CEXCEPTION_H_INCLUDED


