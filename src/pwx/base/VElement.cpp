#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "pwx/base/VElement.h"

namespace pwx {

/// @brief VElement default constructor.
VElement::VElement() noexcept
{ }

/// @brief VElement copy constructor.
VElement::VElement(const VElement& src) noexcept :
	base_t(src)
{ }

} // namespace pwx
