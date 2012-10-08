#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include <pwx/base/VContainer.h>

namespace pwx {

/// @brief VContainer default constructor.
VContainer::VContainer() noexcept
{ }

/// @brief VContainer copy constructor.
VContainer::VContainer(const VContainer& src) noexcept
: base_t(src)
{ }

/// @brief Returns true if the container is empty.
bool VContainer::empty() const noexcept
{
  return !eCount;
}

/// @brief Returns the number of stored elements.
uint32_t VContainer::size() const noexcept
{
  return eCount;
}

} // namespace pwx
