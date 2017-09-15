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

/// @brief ~VContainer default destructor.
VContainer::~VContainer() noexcept
{ }

} // namespace pwx
