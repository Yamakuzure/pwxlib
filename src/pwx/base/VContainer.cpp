#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include <pwx/base/VContainer.h>

namespace pwx {

/// @brief VContainer default constructor.
VContainer::VContainer() noexcept
{ }

/// @brief VContainer copy constructor.
VContainer::VContainer(const VContainer& src) noexcept :
	base_t(src),
	beThreadSafe(src.beThreadSafe.load(PWX_MEMORDER_RELAXED)),
	doRenumber(src.doRenumber.load(PWX_MEMORDER_ACQUIRE)),
	eCount(src.eCount.load(PWX_MEMORDER_ACQUIRE))
{ }


} // namespace pwx
