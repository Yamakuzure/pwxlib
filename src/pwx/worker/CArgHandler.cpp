#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif

#include "CArgHandler.h"

namespace pwx {




/** @brief default empty ctor
  */
CArgHandler::CArgHandler() noexcept :
    longArgs (7, do_not_destroy, nullptr, 256, 4.0, 1.733),
    pass_args(nullptr),
    pass_init(nullptr),
    pass_cnt(nullptr),
    shortArgs(7, do_not_destroy, nullptr, 256, 4.0, 1.733)
{
	/* nothing to do here */
}

/** @brief default dtor
  */
CArgHandler::~CArgHandler() noexcept
{
	this->clearArgs();
}

/** @brief add a pass through system
  *
  * This method sets an identifier using @a init_arg, that
  * identifies a point in the argument list that is followed
  * by arguments that are not to be processed.
  * Instead, the following arguments are stored in @a pass_argc
  * and @a pass_argv.
  *
  * If any of the three parameters is nullptr, a pwx::Cexception
  * with the name "IllegalParameters" is thrown.
  *
  * If @a init_arg points to an empty string, <B>all</B>
  * parameters are passed through.
  *
  * @param[in] init_arg the character sequence identifying the passthrough start
  * @param[out] pass_argc pointer receiving the number of passed arguments
  * @param[out] pass_argv pointer receiving the passed through arguments
  */
void CArgHandler::addPassthrough(const char* init_arg, int32_t* pass_argc, char*** pass_argv)
{
	if (!init_arg || !pass_argc || !pass_argv)
		PWX_THROW("IllegalParameters", "nullptr parameter",
			"CArgHandler::addPassthrough must not be called with any parameter being nullptr!")
	pass_args = pass_argv;
	pass_cnt  = pass_argc;
	pass_init = strdup(init_arg);
}

/** @brief releases allocated memory
  */
void CArgHandler::clearArgs() noexcept
{
	errlist.clear();
	longArgs.clear();
	shortArgs.clear();
	if (pass_init)
		free(pass_init);
	pass_args = nullptr;
	pass_init = nullptr;
	pass_cnt  = nullptr;
}

/** @brief get error number with number @a nr
  *
  * This method returns the error number of the error
  * number @a nr. The first error is error 1.
  *
  * If no such error exists, the method returns 0.
  *
  * @param[in] nr Number of the error to retrieve the error number from
  * @return retrieved error number or 0, if no such error exists.
  */
int32_t CArgHandler::getError(const int32_t nr) const noexcept
{
	const int32_t idx  = nr - 1;
	const int32_t size = static_cast<int32_t>(errlist.size());

	// no wrapping here!
	if ( (idx >= 0) && (idx < size) )
		return errlist.getData(idx).arg_errno;

	return 0;
}

/** @brief return the number of found parsing errors
  * @return the number of parsing errors
  */
int CArgHandler::getErrorCount() const noexcept
{
	return static_cast<int>(errlist.size());
}

/** @brief get error text with number @a nr
  *
  * This method returns the error text of the error
  * number @a nr. The first error is error 1.
  *
  * If no such error exists, the method returns nullptr.
  *
  * @param[in] nr Number of the error to retrieve the error text from
  * @return retrieved error text or nullptr, if no such error exists.
  */
const char* CArgHandler::getErrorStr(const int32_t nr) const noexcept
{
	int idx  = nr - 1;
	int size = static_cast<int>(errlist.size());

	// no wrapping here!
	if ( (idx >= 0) && (idx < size) )
		return errlist.getData(idx).arg_error;

	return nullptr;
}

/** @brief parseArgs
  *
  * @todo: document this function
  */
int32_t CArgHandler::parseArgs(const int32_t argc, const char** argv) noexcept
{
	int32_t result = 0;


	return result;
}



} // namespace pwx

