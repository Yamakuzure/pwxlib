#ifndef PWX_EXPORTS
#  define PWX_EXPORTS 1
#endif

#include "CArgHandler.h"

namespace pwx {

CArgHandler PAH;

/** @brief default empty ctor
  */
CArgHandler::CArgHandler() noexcept :
    longArgs (7, do_not_destroy, nullptr, 256, 4.0, 1.733),
    maxLongLen(0),
    maxParamLen(0),
    maxShortLen(0),
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


/** @brief get Help string with short/long argument plus parameter
  *
  * This method returns a string of the form
  * "<[short arg] [long arg]> [parameter]" for the argument identified
  * by @a argument.
  *
  * If no such argument exists, the returned string will contain a
  * message that the argument couldn't be found. This condition is
  * tested with an assert.
  *
  * If @a argument is nullptr or an empty string, an error message will
  * be returned. This condition is tested with an assert, too.
  *
  * @param[in] argument Either short or long argument to search for.
  * @return a string with the help text or an error message if @a argument could not be found.
  */
std::string CArgHandler::getHelpArg(const char* argument) const noexcept
{
	std::string result = "";

	assert (argument && strlen(argument)
		&& "ERROR: getHelpArg called with nullptr/empty argument!");

	std::string key    = argument;
	data_t*     target = nullptr;

	if (shortArgs.exists(key))
		target = shortArgs.get(key)->data.get();
	else if (longArgs.exists(key))
		target = longArgs.get(key)->data.get();

	assert(target && "ERROR: Couldn't find given argument!");

	if (target) {
		size_t shortSize = target->aShort.size();
		size_t longSize  = target->aLong.size();
		if (shortSize)
			result += target->aShort;
		if (longSize) {
			if (shortSize)
				result += " ";
			result += target->aLong;
		}
		if (target->pName.size())
			result += " <" + target->pName + ">";
	} else
		result = "Unknown argument: " + key;

	return result;
}


/** @brief get Help string with the description of an argument
  *
  * This method returns the description of an argument identified
  * by @a argument.
  *
  * If no such argument exists, the returned string will contain a
  * message that the argument couldn't be found. This condition is
  * tested with an assert.
  *
  * If @a argument is nullptr or an empty string, an error message will
  * be returned. This condition is tested with an assert, too.
  *
  * @param[in] argument Either short or long argument to search for.
  * @return a string with the description or an error message if @a argument could not be found.
  */
std::string CArgHandler::getHelpDesc(const char* argument) const noexcept
{
	assert (argument && strlen(argument)
		&& "ERROR: getHelpArg called with nullptr/empty argument!");

	std::string key    = argument;
	data_t*     target = nullptr;

	if (shortArgs.exists(key))
		target = shortArgs.get(key)->data.get();
	else if (longArgs.exists(key))
		target = longArgs.get(key)->data.get();

	assert(target && "ERROR: Couldn't find given argument!");

	if (target)
		return target->desc;
	return std::string("Unknown argument: " + key);
}


/** @brief get a formatted help string
  *
  * This method returns a formatted help string with both
  * the short and long argument plus parameter are
  * formatted according to the found maximum lengths of all
  * arguments and parameters.
  *
  * If the full string does not fit into a line of the length
  * @a length, the description will be broken into several
  * lines. The minimum number of description characters, however,
  * is eight, even if this results in lines longer than @a length.
  *
  * If no such argument exists, the returned string will contain a
  * message that the argument couldn't be found. This condition is
  * tested with an assert.
  *
  * If @a argument is nullptr or an empty string, an error message will
  * be returned. This condition is tested with an assert, too.
  *
  * With the three characters @a argSep, @a paramSep and @a
  * descSep a seperator between the short and long argument,
  * the argument(s) and the parameter, and between the argument
  * string and the description can be set.
  *
  * @param[in] argument Either short or long argument to search for.
  * @param[in] length Maximum length of each line.
  * @param[in] argSep Optional separator between the short and the long argument. Default: ' '
  * @param[in] paramSep Optional separator between the arguments and the description. Default: ' '
  * @param[in] descSep Optional separator before the description. Default: ' '
  * @return a string with the description or an error message if @a argument could not be found.
  */
std::string CArgHandler::getHelpStr(const char* argument, size_t length,
									char argSep, char paramSep, char descSep) const noexcept
{
	std::string result;

	assert (argument && strlen(argument)
		&& "ERROR: getHelpArg called with nullptr/empty argument!");

	std::string key    = argument;
	data_t*     target = nullptr;

	if (shortArgs.exists(key))
		target = shortArgs.get(key)->data.get();
	else if (longArgs.exists(key))
		target = longArgs.get(key)->data.get();

	assert(target && "ERROR: Couldn't find given argument!");

	if (target) {
		size_t shortSize = target->aShort.size();
		size_t longSize  = target->aLong.size();
		size_t paramSize = target->pName.size();
		size_t paramNeed = paramSize ? paramSize + 2 : 0;
		size_t descSize  = target->desc.size();

		// Add short argument, aligend right
		if (shortSize < maxShortLen)
			result.append(maxShortLen - shortSize, ' ');
		if (shortSize)
			result += target->aShort;

		// Optional separator
		if (argSep)
			result += argSep;

		// Add long argument, aligned left
		if (longSize)
			result += target->aLong;
		if (longSize < maxLongLen)
			result.append(maxLongLen - longSize, ' ');

		// Optional separator
		if (paramSep)
			result += paramSep;

		// Add parameter, aligned left
		if (maxParamLen) {
			if (paramSize)
				result += "<" + target->pName + ">";
			if (paramNeed < (maxParamLen + 2))
				result.append(maxParamLen + 2 - paramNeed, ' ');

			// Optional separator
			if (descSep)
				result += descSep;
		}

		// Now possibly distribute the description
		if (descSize) {
			size_t leftLen  = result.size() - (descSep ? 1 : 0);
			size_t rightLen = (leftLen + 8) > length ? 8 : length - leftLen;
			size_t pos      = 0;
			while (pos < descSize) {
				result += target->desc.substr(pos, rightLen);
				pos += rightLen;
				if (pos < descSize) {
					result += '\n';
					result.append(leftLen, ' ');
					if (descSep)
						result += descSep;
				}
			}
		}
	} else
		result = "Unknown argument: " + key;

	return result;
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

