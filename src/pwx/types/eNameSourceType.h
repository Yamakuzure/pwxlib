#pragma once
#ifndef PWX_LIBPWX_PWX_TYPES_ENAMESOURCETYPE_H_INCLUDED
#define PWX_LIBPWX_PWX_TYPES_ENAMESOURCETYPE_H_INCLUDED 1

/** @file eNameSourceType.h
  *
  * @brief Automatically generated header for the name generator.
  *
  * <B>Automatically generated Header! Do NOT modify!</B>
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

/// @brief pre-increment for the source type
eNameSourceType &operator++(eNameSourceType &type)
{
	if (type < NST_NUM_TYPES)
		type = static_cast<eNameSourceType>(1 + static_cast<uint32_t>(type));

	return type;
}

/// @brief post-increment for the source type
eNameSourceType operator++(eNameSourceType &type, int)
{
	eNameSourceType tmp = type;
	++type;
	return tmp;
}

/// @brief pre-decrement for the source type
eNameSourceType &operator--(eNameSourceType &type)
{
	if (type > NST_NAMES_DE)
		type = static_cast<eNameSourceType>(-1 + static_cast<uint32_t>(type));

	return type;
}

/// @brief post-decrement for the source type
eNameSourceType operator--(eNameSourceType &type, int)
{
	eNameSourceType tmp = type;
	--type;
	return tmp;
}
	
} // namespace pwx

#endif // PWX_LIBPWX_PWX_TYPES_ENAMESOURCETYPE_H_INCLUDED

