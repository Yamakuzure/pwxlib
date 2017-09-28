#pragma once
#ifndef PWX_LIBPWX_PWX_TYPES_ENAMESOURCETYPE_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_ENAMESOURCETYPE_H_INCLUDED 1

/** @file eNameSourceType.h
  *
  * @brief Automatically generated header for the name generator.
  *
  * <B>Automatically generated Header! Do NOT modify!</B>
  *
  * (c) 2007 - 2017 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
  *
  * The PrydeWorX Library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public License as
  * published by the Free Software Foundation; either version 2.1 of the
  * License, or (at your option) any later version.
  *
  * The PrydeWorX Library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public License
  * along with pwxLib; If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
**/


#include "compiler.h"


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

eNameSourceType PWX_API &operator++(eNameSourceType &type);
eNameSourceType PWX_API  operator++(eNameSourceType &type, int);
eNameSourceType PWX_API &operator--(eNameSourceType &type);
eNameSourceType PWX_API  operator--(eNameSourceType &type, int);

} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_ENAMESOURCETYPE_H_INCLUDED

