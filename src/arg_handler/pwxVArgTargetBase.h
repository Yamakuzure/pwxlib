#ifndef PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED
#define PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED

/** @file pwxVArgTargetBase.h
  *
  * @brief Simple basic struct holding all non-templated members for TArgTarget
  *
  * (c) 2007 - 2018 PrydeWorX
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

#include "pwx_compiler.h"
#include "eArgTargetType.h"
#include "eArgSetType.h"
#include "eArgErrorNumber.h"


namespace pwx {


/** @struct VArgTargetBase
  * @brief data collection of non-templated values for TArgTarget
**/
struct VArgTargetBase {
    // Members
    std::string    aShort;
    std::string    aLong;
    std::string    desc;
    std::string    pName;
    eArgTargetType type;
    eArgSetType    setType;

    explicit VArgTargetBase( const char* arg_short, const char* arg_long,
                             eArgTargetType arg_type,
                             const char* arg_desc, const char* param_name )
    noexcept;
    VArgTargetBase( const char* arg_short, const char* arg_long,
                    eArgSetType set_type,
                    const char* arg_desc, const char* param_name )
    noexcept;
    virtual ~VArgTargetBase() noexcept;

    // Must be defined by TArgTarget:
    virtual eArgErrorNumber process( const char* ) PWX_VIRTUAL_PURE;

    // Public methods that do not need a templated value:
    bool hasParameter  () const noexcept;
    bool needsParameter() const noexcept;

  protected:

    bool gotParameter = false; //!< Must be set to true by process() if a parameter was processed
};


// Operators needed for pwx container storage:
bool operator==( const VArgTargetBase& lhs, const VArgTargetBase& rhs ) noexcept;
bool operator>( const VArgTargetBase& lhs, const VArgTargetBase& rhs ) noexcept;


} // namespace pwx


#endif // PWX_LIBPWX_PWX_BASE_VARGTARGETBASE_H_INCLUDED

