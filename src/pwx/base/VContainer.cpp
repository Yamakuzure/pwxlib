#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "pwx/base/VContainer.h"

namespace pwx {

/// @brief VContainer default constructor.
VContainer::VContainer() noexcept :
	beThreadSafe(true),doRenumber(true), eCount(0)
{ }

/// @brief VContainer copy constructor.
VContainer::VContainer(const VContainer& src) noexcept :
	base_t(src),
	beThreadSafe((bool)src.beThreadSafe),
	doRenumber((bool)src.doRenumber),
	eCount((uint32_t)src.eCount)
{ }


} // namespace pwx
