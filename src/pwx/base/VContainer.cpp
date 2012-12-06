#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "pwx/base/VContainer.h"

namespace pwx {

/// @brief VContainer default constructor.
VContainer::VContainer() noexcept
{ }

/// @brief VContainer copy constructor.
VContainer::VContainer(const VContainer& src) noexcept :
	base_t(src)
{ }


} // namespace pwx
