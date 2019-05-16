#ifndef PWX_LIBPWX_PWX_TYPES_ENAMESOURCETYPE_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_ENAMESOURCETYPE_H_INCLUDED 1
#pragma once

/** @file eNameSourceType.h
  *
  * @brief Automatically generated header for the name generator.
  *
  * <B>Automatically generated Header! Do NOT modify!</B>
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


#include "basic/pwx_compiler.h"


namespace pwx {


/** @brief [N]ame[S]ource[T]ype enum with NST_<type>_<language>
  * Note: The special language "all" is merged from all others.
  * Note: The type "texts" includes everything from the type "names"
**/
enum eNameSourceType {
    NST_NAMES_DE  = 0, // 1 file analyzed
    NST_NAMES_EN  = 1, // 8 files analyzed
    NST_NAMES_ALL = 2, // merged from all names
    NST_TEXTS_DE  = 3, // 729 files analyzed
    NST_TEXTS_EN  = 4, // 2975 files analyzed
    NST_TEXTS_ALL = 5, // merged from all texts
    NST_NUM_TYPES = 6  // End-of-list marker! No valid source type!
};

eNameSourceType PWX_API& operator++( eNameSourceType& type );
eNameSourceType PWX_API  operator++( eNameSourceType& type, int );
eNameSourceType PWX_API& operator--( eNameSourceType& type );
eNameSourceType PWX_API  operator--( eNameSourceType& type, int );

} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_ENAMESOURCETYPE_H_INCLUDED

