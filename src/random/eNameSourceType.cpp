#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
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
