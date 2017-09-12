#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "VContainer.h"

namespace pwx {

/// @brief VContainer default constructor.
VContainer::VContainer() noexcept
{ }

/// @brief VContainer copy constructor.
VContainer::VContainer(const VContainer& src) noexcept :
	base_t(src),
	doRenumber(src.doRenumber.load(memOrdLoad))
	// eCount is maintained when deriving copy ctors copy elements
{ }


} // namespace pwx
