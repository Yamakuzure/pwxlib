#ifndef PWX_LIBPWX_PWX_TYPES_EARGTARGETTYPE_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_EARGTARGETTYPE_H_INCLUDED

/** @file eArgTargetType.h
  *
  * @brief Enum to describe what to do with a PAH target
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


namespace pwx {


/** @enum eArgTargetType
  * @brief determine what to do with a program argument handler target
  *
  * The value taken when adding an argument to PAH (or any other
  * instance of CArgHAndler) decides upon what to do with a
  * target when parsing command line arguments.
**/
enum eArgTargetType {
    ATT_FALSE = 0, //!< Set target to false (or (T)0 if not bool)
    ATT_TRUE  = 1, //!< Set target to true  (or (T)1 if not bool)
    ATT_ADD   = 2, //!< Add parameter to target
    ATT_INC   = 3, //!< Increase target by 1
    ATT_SUB   = 4, //!< Substract parameter from target
    ATT_DEC   = 5, //!< Decrease target by 1
    ATT_SET   = 6, //!< Set target to the value of the parameter
    ATT_CB    = 7  //!< The target is a callback function of the type void (*cb)(const char*, T*)
};


} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_EARGTARGETTYPE_H_INCLUDED

