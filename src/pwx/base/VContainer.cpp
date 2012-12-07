#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "pwx/base/VContainer.h"

namespace pwx {

/// @brief VContainer default constructor.
VContainer::VContainer() noexcept :
	doRenumber(true)
{ }

/// @brief VContainer copy constructor.
VContainer::VContainer(const VContainer& src) noexcept :
	base_t(src), doRenumber((bool)src.doRenumber)
{ }


} // namespace pwx
