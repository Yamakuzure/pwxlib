/***
  This file is part of the PrydeWorX Library (pwxLib).

  Copyright 2007 - 2017 Sven Eden

  The PrydeWorX Library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  The PrydeWorX Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with pwxLib; If not, see <http://www.gnu.org/licenses/>.

  History and Changelog are maintained in pwx.h
***/


#include "eNameSourceType.h"
#include "types.h"


namespace pwx {


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
