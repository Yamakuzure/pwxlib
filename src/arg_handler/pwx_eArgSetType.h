#ifndef PWX_LIBPWX_PWX_TYPES_EARGSETTYPE_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_EARGSETTYPE_H_INCLUDED

/** @file pwx_eArgSetType.h
  *
  * @brief enum to describe what PAH should do when an ATT_SET target receives more than one parameter
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
  * History and change log are maintained in pwx.h
**/


namespace pwx {


/** @enum eArgSetType
  * @brief determine what to do when multiple parameters are submitted to an ATT_SET target.
  *
  * This enum only applies to ATT_SET targets to be able to change the
  * default behaviour of the ATT_SET target, which is to overwrite the
  * set value on each call to process().
**/
enum eArgSetType {
    STT_ERROR     = 0, //!< Receiving more than one parameter is an error.
    STT_OVERWRITE = 1, //!< Overwrite the set value (default)
    STT_IGNORE    = 2  //!< Only set the value once, silently ignore all further parameters.
};


} // namespace pwx


#endif // PWX_LIBPWX_PWX_TYPES_EARGSETTYPE_H_INCLUDED

