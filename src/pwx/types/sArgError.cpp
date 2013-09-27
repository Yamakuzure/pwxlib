#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif
#include "sArgError.h"

#include <cstdlib>

namespace pwx {


/** @brief default ctor
  *
  * @param[in] errno_ error number of the error
  * @param[in] error_ text describing the error
  */
sArgError::sArgError(eArgErrorNumber errno_, const char* error_) noexcept :
	arg_errno(static_cast<int32_t>(errno_)),
	arg_error(error_ ? strdup(error_) : nullptr)
{
	/* nothing to do here */
}


/** @brief default dtor
  */
sArgError::~sArgError() noexcept
{
	if (arg_error) free(const_cast<char*>(arg_error));
}


/// @brief return true if both errors have the same errno
bool operator==(const sArgError &lhs, const sArgError &rhs) noexcept
{
	return lhs.arg_errno == rhs.arg_errno;
}

/// @brief return true if lhs.errno is greater than rhs.errno
bool operator>(const sArgError &lhs, const sArgError &rhs) noexcept
{
	return lhs.arg_errno > rhs.arg_errno;
}



} // namespace pwx
