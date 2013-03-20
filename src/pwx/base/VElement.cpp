#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "pwx/base/VElement.h"

namespace pwx {

/// @brief VElement default constructor.
VElement::VElement() noexcept
 :	eNr(0),
	isDestroyed(ATOMIC_VAR_INIT(false))
{ }

/// @brief VElement copy constructor.
VElement::VElement(const VElement& src) noexcept
 :	base_t(src),
	eNr(0),
	isDestroyed(ATOMIC_VAR_INIT(false))
{ }

} // namespace pwx
