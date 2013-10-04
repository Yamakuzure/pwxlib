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
  * In the default configuration the returned string is simply both
  * arguments and the possible parameter concatenated together.
  * Several optional parameters can be used to configure the output:
  * <BR />
  * The optional parameter @a length can be used to set a minimum
  * length. If this is set to a value different than 0, the string
  * returned will be filled with preceeding spaces.<BR />
  * The parameter @a argSep defines a separator character, defaulting
  * to ' ', that is placed between the short and long argument.<BR />
  * The parameter @a paramSep defines a separator character, defaulting
  * to ' ', that is placed between the argument(s) and the parameter.
  * <BR />
  * If @a emptyLine is set to true, the string will consist of spaces
  * and possibly the separators, according to @a autoSep, only.<BR />
  * Finally the parameter @a autoSep, that defaults to true, can be
  * changed to false to have @a argSep added in any way. By default
  * separators are only added if @a emptyLine is false. But even if
  * @a emptyLine is false, @a argSep is only added if there are both
  * a short and long version of the argument, and @a paramSep if there
  * is a parameter. If @a autoSep is false, @a argSep is placed in
  * any way and @a paramSep if there is at least one argument known
  * that has a parameter.
  *
  * @param[in] argument Either short or long argument to search for.
  * @param[in] length Minimum length of each line.
  * @param[in] indent Prefix these number of spaces. Default: 0
  * @param[in] argSep Optional separator between the short and the long argument. Default: ' '
  * @param[in] paramSep Optional separator between the arguments and the description. Default: ' '
  * @param[in] emptyLine If set to true, a blank line with possible separators is returned.
  * @param[in] autoSep Only display a separator if there is a value on each side. Default: true
  * @return a string with the help text or an error message if @a argument could not be found.
  */
std::string CArgHandler::getHelpArg(const char* argument, size_t length, size_t indent,
									char argSep, char paramSep,
									bool emptyLine, bool autoSep) const noexcept
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
		size_t paramSize = target->pName.size();
		size_t paramNeed = paramSize ? paramSize + 2 : 0;

		// Start with indentation if set
		if (indent)
			result.assign(indent, ' ');

		// === First: short argument ===
		if (shortSize) {
			// a) insert spaces to right align the short arg
			if (shortSize < maxShortLen)
				result.append(maxShortLen - shortSize, ' ');

			// b) Add short arg or blanks
			if (emptyLine)
				result.append(shortSize, ' ');
			else
				result.append(target->aShort);

			// c) Add possible separator
			if (argSep) {
				if (!autoSep || (!emptyLine && longSize))
					result += argSep;
				else
					result += ' ';
			}
		} else {
			result.append(maxShortLen + (argSep && autoSep ? 1 : 0), ' ');
			if (argSep && !autoSep)
				result += argSep;
		} // end of handling short argument

		// === Second: long argument ===
		if (longSize) {
			// a) Add long arg or blanks
			if (emptyLine)
				result.append(longSize, ' ');
			else
				result.append(target->aLong);

			// b) insert spaces to left align the long arg
			if (longSize < maxLongLen)
				result.append(maxLongLen - longSize, ' ');
		} else {
			result.append(maxLongLen + (paramSep && autoSep && maxParamLen ? 1 : 0), ' ');
			if (paramSep && !autoSep && maxParamLen)
				result += paramSep;
		} // end of handling long argument

		// === Third: argument parameter ===
		if (maxParamLen) {
			// a) Add possible separator
			if (paramSep && longSize) {
				if (!autoSep || (!emptyLine && paramSize))
					result += paramSep;
				else
					result += ' ';
			}

			// b) Add parameter or blanks
			if (emptyLine || !paramSize)
				result.append(paramNeed, ' ');
			else
				result += "<" + target->pName + ">";

			// c) insert spaces to left align the parameter
			if (paramNeed < (maxParamLen + 2))
				result.append(maxParamLen + 2 - paramNeed, ' ');
		} // End of handling parameters

		// Fourth: Before the result can be returned, it might need to be indented:
		if (result.size() < length)
			result.insert(0, length - result.size(), ' ');
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
  * In the default configuration the description string is simply
  * returned in whole. Several optional parameters can be used to
  * configure the output:<BR />
  * The optional parameter @a pos can be used to define a different
  * starting position. If @a pos is larger than the size of the
  * description, an empty string is returned.<BR />
  * The optional parameter @a length can be used to set a maximum
  * length. If this is set to a value different than 0, the string
  * returned will be constructed from @a pos forward to the last
  * space character found before @a length is reached, or up to
  * @a length if either the next character is a space, or no space
  * could be found before @a length was reached.<BR />
  * With @a descSep a separator character, that defaults to ' ',
  * can be set to be added in front of the description returned.<BR />
  * Finally the parameter @a autoSep, that defaults to true, can be
  * changed to false to have @a descSep added in any way. By default
  * the separator character is only added as is on the first line,
  * that is @a pos is set to 0, and is substituted with a space
  * character on all other lines.
  *
  * @param[in] argument Either short or long argument to search for.
  * @param[in] pos Starting position of the description, default 0.
  * @param[in] length Maximum length of the description, default all.
  * @param[in] descSep Optional separator before the description. Default: ' '
  * @param[in] autoSep Only display a separator if there is a value on each side. Default: true
  * @return a string with the description or an error message if @a argument could not be found.
  */
std::string CArgHandler::getHelpDesc(const char* argument, size_t pos,
									 size_t length, char descSep, bool autoSep) const noexcept
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

	if (target) {
		size_t descSize = target->desc.size();

		// Exit now if length is zero, it means the
		// whole (or remainder) of the description
		// is to be returned.
		if (!length)
			return pos < descSize ? target->desc.substr(pos) : "";

		// If pos is too large, the method can return now, too
		if (pos >= descSize)
			return "";

		// The result is a substring, possibly trimmed at a space
		// character, preceded by either descSep or a ' ' according
		// to the autoSep setting.
		std::string result;
		if (descSep) {
			if (pos && autoSep)
				result.assign(1, ' ');
			else
				result.assign(1, descSep);
		}

		// If the next character at pos+length is not a space,
		// the last space before pos+length must be found to
		// build the substring to return.
		if ( (pos + length) <= descSize ) {
			size_t endpos = target->desc.find_last_of(' ' , pos + length);

			if (endpos > pos)
				result += target->desc.substr(pos, endpos - pos);
			else
				result += target->desc.substr(pos, length - 1);
		} else
			result += target->desc.substr(pos);

		return result;
	}
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
  * @param[in] indent Prefix these number of spaces. Default: 0
  * @param[in] argSep Optional separator between the short and the long argument. Default: ' '
  * @param[in] paramSep Optional separator between the arguments and the description. Default: ' '
  * @param[in] descSep Optional separator before the description. Default: ' '
  * @param[in] autoSep Only display a separator if there is a value on each side. Default: true
  * @return a string with the description or an error message if @a argument could not be found.
  */
std::string CArgHandler::getHelpStr(const char* argument, size_t length, size_t indent,
									char argSep, char paramSep, char descSep,
									bool autoSep) const noexcept
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
		/** @todo : The three methods do the checking all the time.
		  * This should be done differently, with private non-checking
		  * methods doing the work, and the public versions only check
		  * and delegate.
		  * Priority: Low.
		**/
		size_t leftSize  = maxShortLen + maxLongLen + maxParamLen + 2 + indent;
		size_t rightSize = length > (leftSize + 8) ? length - leftSize : 8;
		size_t descSize  = target->desc.size();
		size_t pos       = 0;
		std::string desc;

		while (pos < descSize) {
			// Add left side:
			result.append(getHelpArg(argument, leftSize, indent,
									 argSep, paramSep,
									 pos ? true : false, autoSep));

			// Add right side
			desc.assign(getHelpDesc(argument, pos, rightSize, descSep, autoSep));
			pos += desc.size() ? desc.size() : 1;
			result.append(desc);

			// Any characters left, then add a line break:
			if (pos < descSize)
				result += '\n';
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

