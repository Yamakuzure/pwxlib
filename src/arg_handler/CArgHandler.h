#ifndef PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED
#define PWX_LIBPWX_PWX_WORKER_CARGHANDLER_H_INCLUDED

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

#include "CArgCallback.h"
#include "CException.h"
#include "sArgError.h"
#include "TArgTarget.h"
#include "TChainHash.h"
#include "TQueue.h"


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
  * must be installed that converts a const char* parameter
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
  * that follow(s) the argument as a const char*.
**/
class PWX_API CArgHandler {
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
    typedef TQueue<data_t>            arg_list_t;
    typedef arg_list_t::elem_t        arg_elem_t;


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

    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, bool* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, int8_t* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, uint8_t* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, int16_t* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, uint16_t* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, int32_t* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, uint32_t* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, int64_t* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, uint64_t* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, float* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, double* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, long double* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type, std::string* arg_target,
                                const char* arg_desc, const char* param_name,
                                eArgSetType set_type = STT_OVERWRITE );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                eArgTargetType arg_type,
                                void ( *arg_cb )( const char*, const char* ),
                                const char* arg_desc, const char* param_name );
    bool		addArg		  ( const char* arg_short, const char* arg_long,
                                void ( *arg_cb )( const char*, const char* ),
                                const char* arg_desc, const char* param_name );
    void        addPassthrough( const char* init_arg, int32_t* pass_argc, char*** pass_argv );
    void        clearArgs     () noexcept;
    int32_t     getError      ( const int32_t nr ) const noexcept;
    int32_t     getErrorCount () const noexcept;
    const char* getErrorStr   ( const int32_t nr ) const noexcept;
    std::string getHelpArg    ( const char* argument, size_t length = 0, size_t indent = 0,
                                char argSep = 0x20, char paramSep = 0x20,
                                bool emptyLine = false, bool autoSep = true,
                                bool autoSpace = false ) const noexcept;
    std::string getHelpDesc   ( const char* argument, size_t* pos = nullptr, size_t length = 0,
                                char descSep = 0x20, bool autoSep = true,
                                bool autoSpace = false ) const noexcept;
    std::string getHelpStr    ( const char* argument, size_t length, size_t indent = 0,
                                char argSep = 0x20, char paramSep = 0x20,
                                char descSep = 0x20, bool autoSep = true,
                                bool autoSpace = false ) const noexcept;
    const char* getPrgCall    () const noexcept;
    int32_t     parseArgs     ( const int32_t argc, char* argv[] ) noexcept;
    int32_t     parseArgs     ( const int32_t argc, const char* argv[] ) noexcept;


  private:

    /* ===============================================
     * === Private methods                         ===
     * ===============================================
     */

    /// @internal do nothing with the element.
    /// (or cleaning short args, destroys targets of long args!)
    static void do_not_destroy( data_t* ) { }

    data_t* getTarget  ( const char* arg )                       const noexcept;
    void    passThrough( const int32_t argc, const char** argv )       noexcept;
    bool    uncombine  ( const char* arg, arg_list_t& arg_list );


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

