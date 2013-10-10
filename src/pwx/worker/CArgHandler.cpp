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
  * @a pass_argc will receive the number of entries in @a pass_argv
  * received, when the real processing is done. Therefore *pass_argv
  * will result in an array of *pass_argc malloc'd C-Strings, that
  * you'll have to free yourself.<BR />
  * Please note, that this implies that *pass_argv must be nullptr when
  * the arguments are processed, as a new array will be malloc'd.
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
  * This parameter @a autoSep, that defaults to true, can be changed
  * to false to have @a argSep added in any way. By default separators
  * are only added if @a emptyLine is false. But even if @a emptyLine
  * is false, @a argSep is only added if there are both a short and
  * long version of the argument, and @a paramSep if there is a
  * parameter. If @a autoSep is false, @a argSep is placed in any way
  * and @a paramSep if there is at least one argument known that has
  * a parameter.<BR />
  * The last parameter @a autoSpace defaults to false and causes, if
  * changed to true, that extra space characters are placed around the
  * separators if they are something else than a space character.
  *
  * @param[in] argument Either short or long argument to search for.
  * @param[in] length Minimum length of each line.
  * @param[in] indent Prefix these number of spaces. Default: 0
  * @param[in] argSep Optional separator between the short and the long argument. Default: ' '
  * @param[in] paramSep Optional separator between the arguments and the description. Default: ' '
  * @param[in] emptyLine If set to true, a blank line with possible separators is returned.
  * @param[in] autoSep Only display a separator if there is a value on each side. Default: true
  * @param[in] autoSpace If set to true, extra spaces will be added around the separators if they are not spaces. Default: false
  * @return a string with the help text or an error message if @a argument could not be found.
  */
std::string CArgHandler::getHelpArg(const char* argument, size_t length, size_t indent,
									char argSep, char paramSep,
									bool emptyLine, bool autoSep, bool autoSpace) const noexcept
{
	std::string result = "";

	assert (argument && strlen(argument)
		&& "ERROR: getHelpArg called with nullptr/empty argument!");

	data_t* target = getTarget(argument);

	assert(target && "ERROR: Couldn't find given argument!");

	if (target) {
		size_t shortSize = target->aShort.size();
		size_t longSize  = target->aLong.size();
		size_t paramSize = target->pName.size();
		size_t paramNeed = paramSize ? paramSize + 2 : 0;
		bool   addArgSpc = autoSpace && argSep && (0x20 != argSep);
		bool   addParSpc = autoSpace && paramSep && (0x20 != paramSep);

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
				if (addArgSpc) result += ' ';

				if (!autoSep || (!emptyLine && longSize))
					result += argSep;
				else
					result += ' ';

				if (addArgSpc) result += ' ';
			}
		} else {
			result.append(maxShortLen + (argSep && autoSep ? addArgSpc ? 3 : 1 : 0), ' ');
			if (argSep && !autoSep) {
				if (addArgSpc) result += ' ';
				result += argSep;
				if (addArgSpc) result += ' ';
			}
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
			result.append(maxLongLen + (paramSep && autoSep && maxParamLen ? addParSpc ? 3 : 1 : 0), ' ');
			if (paramSep && !autoSep && maxParamLen) {
				if (addParSpc) result += ' ';
				result += paramSep;
				if (addParSpc) result += ' ';
			}
		} // end of handling long argument

		// === Third: argument parameter ===
		if (maxParamLen) {
			// a) Add possible separator
			if (paramSep && longSize) {
				if (addParSpc) result += ' ';

				if (!autoSep || (!emptyLine && paramSize))
					result += paramSep;
				else
					result += ' ';

				if (addParSpc) result += ' ';
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
	} else {
		result = "Unknown argument: ";
		result += argument;
	}

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
  * can be set to be added in front of the description returned.
  * <BR />
  * The parameter @a autoSep, that defaults to true, can be changed
  * to false to have @a descSep added in any way. By default the
  * separator character is only added as is on the first line, that
  * is @a pos is set to 0, and is substituted with a space character
  * on all other lines.<BR />
  * The last parameter @a autoSpace defaults to false and causes, if
  * changed to true, that extra space characters are placed around the
  * @a descSep separator if it is set to something else than a space
  * character.
  *
  * @param[in] argument Either short or long argument to search for.
  * @param[in,out] pos Pointer to the starting position of the description, receives end position, default nullptr.
  * @param[in] length Maximum length of the description, default all.
  * @param[in] descSep Optional separator before the description. Default: ' '
  * @param[in] autoSep Only display a separator if there is a value on each side. Default: true
  * @param[in] autoSpace If set to true, extra spaces will be added around the separators if they are not spaces. Default: false
  * @return a string with the description or an error message if @a argument could not be found.
  */
std::string CArgHandler::getHelpDesc(const char* argument, size_t* pos,
									 size_t length, char descSep,
									 bool autoSep, bool autoSpace) const noexcept
{
	assert (argument && strlen(argument)
		&& "ERROR: getHelpArg called with nullptr/empty argument!");

	data_t* target = getTarget(argument);

	assert(target && "ERROR: Couldn't find given argument!");

	if (target) {
		size_t descSize = target->desc.size();
		size_t xPos     = pos ? *pos : 0;

		// Exit now if length is zero, it means the
		// whole (or remainder) of the description
		// is to be returned.
		if (!length)
			return xPos < descSize ? target->desc.substr(xPos) : "";

		// If xPos is too large, the method can return now, too
		if (xPos >= descSize)
			return "";

		// Are extra spaces needed ?
		bool addDescSpc = autoSpace && descSep && (0x20 != descSep);

		// The result is a substring, possibly trimmed at a space
		// character, preceded by either descSep or a ' ' according
		// to the autoSep setting.
		std::string result;
		if (descSep) {
			if (addDescSpc) result.append(1, ' ');
			if (xPos && autoSep)
				result.append(1, ' ');
			else
				result.append(1, descSep);
			if (addDescSpc) result.append(1, ' ');
		}

		// If the next character at xPos+length is not a space,
		// the last space before xPos+length must be found to
		// build the substring to return.
		if ( (xPos + length) <= descSize ) {
			size_t endpos = target->desc.find_last_of(' ' , xPos + length);

			if (endpos > xPos) {
				result += target->desc.substr(xPos, endpos - xPos);
				if (pos) *pos += endpos + 1;
			} else {
				result += target->desc.substr(xPos, length - 1);
				if (pos) *pos += length;
			}
		} else {
			result += target->desc.substr(xPos);
			if (pos) *pos = target->desc.size();
		}
	} else {
		result = "Unknown argument: ";
		result += argument;
	}

	return result;
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
  * If a description is long enough to cause line breakse, the set
  * separators are not displayed and substituted by a space
  * character. If the separators shall be displayed on every line,
  * @a autoSep can be set to false.
  *
  * The last parameter @a autoSpace defaults to false and causes, if
  * changed to true, that extra space characters are placed around the
  * separators if they are something else than a space character.
  *
  * @param[in] argument Either short or long argument to search for.
  * @param[in] length Maximum length of each line.
  * @param[in] indent Prefix these number of spaces. Default: 0
  * @param[in] argSep Optional separator between the short and the long argument. Default: ' '
  * @param[in] paramSep Optional separator between the arguments and the description. Default: ' '
  * @param[in] descSep Optional separator before the description. Default: ' '
  * @param[in] autoSep Only display a separator if there is a value on each side. Default: true
  * @param[in] autoSpace If set to true, extra spaces will be added around the separators if they are not spaces. Default: false
  * @return a string with the description or an error message if @a argument could not be found.
  */
std::string CArgHandler::getHelpStr(const char* argument, size_t length, size_t indent,
									char argSep, char paramSep, char descSep,
									bool autoSep, bool autoSpace) const noexcept
{
	std::string result;

	assert (argument && strlen(argument)
		&& "ERROR: getHelpArg called with nullptr/empty argument!");

	data_t* target = getTarget(argument);

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
									 pos ? true : false, autoSep, autoSpace));

			// Add right side
			desc.assign(getHelpDesc(argument, &pos, rightSize, descSep, autoSep, autoSpace));
			result.append(desc);

			// Any characters left, then add a line break:
			if (pos < descSize)
				result += '\n';
		}

	} else {
		result = "Unknown argument: ";
		result += argument;
	}

	return result;
}


/** @brief parse given arguments
  *
  * This method parses the given array @a argv of C-Strings
  * with @a argc expected entries.
  *
  * @param[in] argc The number of strings in @a argv.
  * @param[in] argv array of C-Strings.
  * @return Number of errors encountered.
  */
int32_t CArgHandler::parseArgs(const int32_t argc, const char** argv) noexcept
{
	if (argc > 0) {

		/* Catch special condition: Everything is passed through. */
		if (pass_init && !strlen(pass_init)) {
			passThrough(argc, argv);
			return 0;
		}

		/* Move through argv until
		 * a) argc is reached or
		 * b) pass_init is reached.
		 */
		data_t* lastTarget = nullptr; // holds the last target expecting a parameter
		int32_t idx        = 0;
		for ( ; (idx < argc) && argv[idx] && STRNE(argv[idx], pass_init); ++idx) {
			data_t* target      = getTarget(argv[idx]);
			bool    callProcess = false;

			if (target) {
				/* There are three possible conditions here:
				 * a) The target does not need a parameter:
				 *    -> call process(nullptr)
				 * b) The target needs an argument:
				 *    -> substitute lastTarget after it has been checked for c)
				 * c) lastTarget did not get a parameter
				 *    -> Add error message about that
				*/
				if (target->needsParameter()) {
					if (lastTarget && (false == lastTarget->hasParameter())) {
						// This is situation c)
						std::string param_error = "Argument "
							+ (lastTarget->aLong.size() ? lastTarget->aLong : lastTarget.aShort)
							+ " needs a parameter \""
							+ lastTarget->pName
							+ "\"";
						sArgError* argError = new sArgError(AEN_PARAMETER_MISSING, param_error.c_str());
						errlist.push(argError);
					}
					// Situation b) after checking c)
					lastTarget = target;
				} else
					// Situation a)
					callProcess = true;
			} else if (lastTarget && lastTarget->needsParameter())
				// Process with this value, the target knows what to do
				callProcess = true;
			else {
				std::string parse_error = "Unknown argument ";
				parse_error += argv[idx];
				sArgError* argError = new sArgError(AEN_ARGUMENT_UNKNOWN, parse_error.c_str());
				errlist.push(argError);
			} // End of argv tests

			// Final Step: Call process
			if (callProcess) {
				PWX_TRY(target->process(argv[idx]))
				catch(CException &e) {
					std::string process_error = e.name();
					process_error += ": ";
					process_error += e.what();
					sArgError* argError = new sArgError(AEN_PARAMETER_MISSING, param_error.c_str());
					errlist.push(argError);
				}
			}
		} // End of looping argv

		// At this point idx might be smaller than argc, which
		// only means we have reached a passthrough condition.
		if (idx < argc)
			// idx still points to the found pass_init, which is not passed.
			passThrough(argc - idx - 1, &argv[idx + 1]);

		return errlist.size();
	} // End of having arguments to parse.

	return 0;
}


/** @brief get target for short/long arg @arg or return nullptr if not found
**/
data_t* CArgHandler::getTarget(const char* arg) const noexcept
{
	data_t* target = nullptr;

	if (arg && strlen(arg)) {
		std::string key = arg;

		if (shortArgs.exists(key))
			target = shortArgs.get(key)->data.get();
		else if (longArgs.exists(key))
			target = longArgs.get(key)->data.get();
	}

	return target;
}

/** @brief store argc/argv in pass_cnt/pass_args values
**/
void CArgHandler::passThrough(const int32_t argc, const char** argv) noexcept
{
	if (argc > 0) {
		*pass_args = (char**)calloc(argc, sizeof(char*));
		for (int32_t i = 0; *pass_args && (i < argc); ++i)
			(*pass_args)[i] = strdup(argv[i]);
	}
}


} // namespace pwx

