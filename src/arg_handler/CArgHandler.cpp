/** @file
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  * History and change log are maintained in pwxlib.h
**/


#include <string>

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "arg_handler/CArgHandler.h"
#include "basic/string_utils.h"
#include "stream_helpers/StreamHelpers.h"


/// @namespace pwx
namespace pwx {


/** @internal
  * @brief Template to add targets and callbacks in one place.
  *
  * does all the checking, creation and everything.
  *
  * @param[in] arg_short Short argument like "-a" or "x".
  * @param[in] arg_long Long argument like "--foo" or "-bar".
  * @param[in] arg_type Determines what to do with the target.
  * @param[in] set_type Determines the type of a set target.
  * @param[in] arg_target Pointer to the value to handle.
  * @param[in] arg_cb Callback function to install
  * @param[in] arg_desc Help text for this argument.
  * @param[in] param_name Name shown in <> in the help text.
  * @param[in] set_type Determines the type of a set target, default is AT_ZERO_OR_MANY.
  * @param[out] tgtShort The CArgHandler::hash_t to store short arguments in
  * @param[out] tgtLong The CArgHandler::hash_t to store long arguments in
  * @param[out] maxLongLen Receives the length of @a arg_long if greater than @a maxLongLen
  * @param[out] maxParamLen Receives the length of @a param_name if greater than @a maxParamLen
  * @param[out] maxShortLen Receives the length of @a arg_short if greater than @a maxShortLen
  * @return true if an argument was added, false otherwise.
**/
template<typename T>
static bool internalAddArg( char const* arg_short, char const* arg_long,
                            eArgTargetType arg_type,
                            eArgType set_type,
                            T* arg_target,
                            void ( *arg_cb )( char const*, char const* ),
                            char const* arg_desc, char const* param_name,
                            CArgHandler::hash_t& tgtShort, CArgHandler::hash_t& tgtLong,
                            size_t& maxLongLen, size_t& maxParamLen, size_t& maxShortLen
                          ) {
	typedef VArgTargetBase              data_t;
	typedef std::string                 key_t;
	typedef THashElement<key_t, data_t> elem_t;

	// === Check arguments against double nullptr ===
	bool hasArgLong  = arg_long  && strlen( arg_long );
	bool hasArgShort = arg_short && strlen( arg_short );

	assert( ( hasArgLong || hasArgShort )
	        && "ERROR: At least one of arg_short and arg_long *MUST* be a string of length>0!" );
	if ( !hasArgLong && !hasArgShort )
		return false;


	// === Check target  ===
	assert( ( arg_target || ( ATT_CB == arg_type ) )
	        && "ERROR: At least one of arg_target and arg_cb must be set!" );
	if ( !arg_target && ( ATT_CB != arg_type ) )
		return false;


	// === Check target type against arg_type ===
	/* The following combinations are valid:
	 *  ATT_FALSE : bool and any numeric
	 *  ATT_TRUE  : bool and any numeric
	 *  ATT_ADD   : any numeric
	 *  ATT_INC   : any numeric
	 *  ATT_SUB   : any numeric
	 *  ATT_DEC   : any numery
	 *  ATT_SET   : everything that supports operator=
	 *  ATT_CB    : everything
	 */
	assert( ( // ATT_FALSE and ATT_TRUE accept bool and numeric
	                ( ( ( ATT_TRUE == arg_type ) || ( ATT_FALSE == arg_type ) )
	                  && isSameType( bool, T ) )
	                ||
	                // All accept numerics
	                // Note: This includes ATT_TRUE/ATT_FALSE, so only
	                //       one check is needed
	                isNumericType( T )
	                ||
	                // ATT_SET and ATT_CB accept everything
	                ( ATT_SET == arg_type )
	                || ( ATT_CB  == arg_type )
	        ) && "ERROR: The combination of type and arg_type is illegal!" );
	// No break in release version.

	// === Check argument against uniqueness ===
	std::string key_long ( hasArgLong  ? arg_long  : "" );
	std::string key_short( hasArgShort ? arg_short : "" );
	bool isLongNew  = hasArgLong  ? !tgtLong.exists( key_long )   : true;
	bool isShortNew = hasArgShort ? !tgtShort.exists( key_short ) : true;
	assert( isShortNew && isLongNew && "ERROR: long or short argument already known!" );
	if ( !isLongNew || !isShortNew )
		return false;

	// === Now create a new target and add it to the hashes ===
	data_t* new_target = nullptr;
	try {
		new_target = new TArgTarget<T>( arg_short, arg_long, arg_type, set_type, arg_target, arg_cb, arg_desc, param_name );
	} catch( std::bad_alloc& e )
		PWX_THROW( "ArgTargetCreationFailed", e.what(), "The creation of a new argument target failed!" );

	// Before adding the new_target, a simple fact:
	// If the pointer is added twice, the first clear() will delete
	// it so the second crashes. We therefore must be sure to actually
	// *copy* the hash element into tgtShort if tgtLong has been
	// filled with a target.
	if ( hasArgLong )
		PWX_TRY_PWX_FURTHER( tgtLong.add( key_long, new_target ) );

	if ( hasArgShort ) {
		if ( hasArgLong ) {
			elem_t new_element( *( tgtLong.get( key_long ) ) );
			new_element.key = key_short;
			PWX_TRY_PWX_FURTHER( tgtShort.add( new_element ) );
		} else {
			PWX_TRY_PWX_FURTHER( tgtShort.add( key_short, new_target ) );
		}
	}

	// === Finally record length if a new maximum is found ===
	if ( key_long.size() > maxLongLen )
		maxLongLen = key_long.size();
	if ( param_name && ( strlen( param_name ) > maxParamLen ) )
		maxParamLen = strlen( param_name );
	if ( key_short.size() > maxShortLen )
		maxShortLen = key_short.size();

	return true;
}


// === CArgHandler implementation ===


CArgHandler::CArgHandler() noexcept
	: longArgs ( 37, 64, 4.0, 1.733 )
	, maxLongLen( 0 )
	, maxParamLen( 0 )
	, maxShortLen( 0 )
	, pass_args( nullptr )
	, pass_init( nullptr )
	, pass_cnt( nullptr )
	, prgCall( nullptr )
	, shortArgs( 37, 64, 4.0, 1.733 ) {
	/* Turn of thread safety for the hashes and the error list.
	 * Multithreading does not make any sense, here! */
	errlist.disable_thread_safety();
	longArgs.disable_thread_safety();
	shortArgs.disable_thread_safety();
}

CArgHandler::~CArgHandler() noexcept {
	this->clearArgs();
	if ( prgCall )
		free( prgCall );
}


// === The bitter evil addArg() legion === ( ;-) ) [deprecated!]


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, bool* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg(
	                                    arg_short, arg_long, arg_type, set_type,
	                                    arg_target, nullptr, arg_desc, param_name,
	                                    this->shortArgs, this->longArgs,
	                                    this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, int8_t* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, uint8_t* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, int16_t* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, uint16_t* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, int32_t* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, uint32_t* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, int64_t* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, uint64_t* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, float* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, double* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, long double* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type, std::string* arg_target,
                          char const* arg_desc, char const* param_name,
                          eArgType set_type ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, set_type,
	                            arg_target, nullptr, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          eArgTargetType arg_type,
                          void ( *arg_cb )( char const*, char const* ),
                          char const* arg_desc, char const* param_name ) {
	assert( ATT_CB == arg_type );
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, arg_type, AT_ZERO_OR_MANY,
	                            ( uint8_t* )nullptr, arg_cb, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


bool CArgHandler::addArg( char const* arg_short, char const* arg_long,
                          void ( *arg_cb )( char const*, char const* ),
                          char const* arg_desc, char const* param_name ) {
	PWX_TRY_PWX_FURTHER( return internalAddArg( arg_short, arg_long, ATT_CB, AT_ZERO_OR_MANY,
	                            ( uint8_t* )nullptr, arg_cb, arg_desc, param_name,
	                            this->shortArgs, this->longArgs,
	                            this->maxLongLen, this->maxParamLen, this->maxShortLen ) );
}


// === Other more harmless methods ===


void CArgHandler::addPassthrough( char const* init_arg, int32_t* pass_argc, char*** pass_argv ) {
	if ( !init_arg || !pass_argc || !pass_argv )
		PWX_THROW( "IllegalParameters", "nullptr parameter",
		           "CArgHandler::addPassthrough must not be called with any parameter being nullptr!" )
		pass_args = pass_argv;
	pass_cnt  = pass_argc;
	pass_init = strdup( init_arg );
}


void CArgHandler::clearArgs() noexcept {
	errlist.clear();
	longArgs.clear();
	posQueue.clear();
	shortArgs.clear();

	if ( pass_init )
		free( pass_init );

	pass_args = nullptr;
	pass_init = nullptr;
	pass_cnt  = nullptr;

	if ( prgCall )
		free( prgCall );
	prgCall = nullptr;
}


int32_t CArgHandler::getError( const int32_t nr ) const noexcept {
	const int32_t idx  = nr - 1;
	const int32_t size = static_cast<int32_t>( errlist.size() );

	// no wrapping here!
	if ( ( idx >= 0 ) && ( idx < size ) )
		return errlist.getData( idx ).arg_errno;

	return 0;
}


int CArgHandler::getErrorCount() const noexcept {
	return static_cast<int>( errlist.size() );
}


char const* CArgHandler::getErrorStr( const int32_t nr ) const noexcept {
	int idx  = nr - 1;
	int size = static_cast<int>( errlist.size() );

	// no wrapping here!
	if ( ( idx >= 0 ) && ( idx < size ) )
		return errlist.getData( idx ).arg_error;

	return nullptr;
}


std::string CArgHandler::getHelpArg( char const* argument, size_t length, size_t indent,
                                     char argSep, char paramSep,
                                     bool emptyLine, bool autoSep, bool autoSpace ) const noexcept {
	std::string result = "";

	assert ( argument && strlen( argument )
	         && "ERROR: getHelpArg called with nullptr/empty argument!" );

	data_t* target = getTarget( argument );

	assert( target && "ERROR: Couldn't find given argument!" );

	if ( target ) {
		size_t shortSize = target->arg_short.size();
		size_t longSize  = target->arg_long.size();
		size_t paramSize = target->param_name.size();
		size_t paramNeed = paramSize ? paramSize + 2 : 0;
		bool   addArgSpc = autoSpace && argSep && ( 0x20 != argSep );
		bool   addParSpc = autoSpace && paramSep && ( 0x20 != paramSep );

		// Start with indentation if set
		if ( indent )
			result.assign( indent, ' ' );

		// === First: short argument ===
		if ( shortSize ) {
			// a) insert spaces to right align the short arg
			if ( shortSize < maxShortLen )
				result.append( maxShortLen - shortSize, ' ' );

			// b) Add short arg or blanks
			if ( emptyLine )
				result.append( shortSize, ' ' );
			else
				result.append( target->arg_short );

			// c) Add possible separator
			if ( argSep ) {
				if ( addArgSpc ) result += ' ';

				if ( !autoSep || ( !emptyLine && longSize ) )
					result += argSep;
				else
					result += ' ';

				if ( addArgSpc ) result += ' ';
			}
		} else {
			result.append( maxShortLen + ( argSep && autoSep ? addArgSpc ? 3 : 1 : 0 ), ' ' );
			if ( argSep && !autoSep ) {
				if ( addArgSpc ) result += ' ';
				result += argSep;
				if ( addArgSpc ) result += ' ';
			}
		} // end of handling short argument

		// === Second: long argument ===
		if ( longSize ) {
			// a) Add long arg or blanks
			if ( emptyLine )
				result.append( longSize, ' ' );
			else
				result.append( target->arg_long );

			// b) insert spaces to left align the long arg
			if ( longSize < maxLongLen )
				result.append( maxLongLen - longSize, ' ' );
		} else {
			result.append( maxLongLen + ( paramSep && autoSep && maxParamLen ? addParSpc ? 3 : 1 : 0 ), ' ' );
			if ( paramSep && !autoSep && maxParamLen ) {
				if ( addParSpc ) result += ' ';
				result += paramSep;
				if ( addParSpc ) result += ' ';
			}
		} // end of handling long argument

		// === Third: argument parameter ===
		if ( maxParamLen ) {
			// a) Add possible separator
			if ( paramSep && longSize ) {
				if ( addParSpc ) result += ' ';

				if ( !autoSep || ( !emptyLine && paramSize ) )
					result += paramSep;
				else
					result += ' ';

				if ( addParSpc ) result += ' ';
			}

			// b) Add parameter or blanks
			if ( emptyLine || !paramSize )
				result.append( paramNeed, ' ' );
			else
				result += "<" + target->param_name + ">";

			// c) insert spaces to left align the parameter
			if ( paramNeed < ( maxParamLen + 2 ) )
				result.append( maxParamLen + 2 - paramNeed, ' ' );
		} // End of handling parameters

		// Fourth: Before the result can be returned, it might need to be indented:
		if ( result.size() < length )
			result.insert( 0, length - result.size(), ' ' );
	} else {
		result = "Unknown argument: ";
		result += argument;
	}

	return result;
}


std::string CArgHandler::getHelpDesc( char const* argument, size_t* pos,
                                      size_t length, char descSep,
                                      bool autoSep, bool autoSpace ) const noexcept {
	assert ( argument && strlen( argument )
	         && "ERROR: getHelpArg called with nullptr/empty argument!" );

	data_t* target = getTarget( argument );

	assert( target && "ERROR: Couldn't find given argument!" );

	std::string result;

	if ( target ) {
		size_t descSize = target->description.size();
		size_t xPos     = pos ? *pos : 0;

		// Exit now if length is zero, it means the
		// whole (or remainder) of the description
		// is to be returned.
		if ( !length )
			return xPos < descSize ? target->description.substr( xPos ) : "";

		// If xPos is too large, the method can return now, too
		if ( xPos >= descSize )
			return "";

		// Are extra spaces needed ?
		bool addDescSpc = autoSpace && descSep && ( 0x20 != descSep );

		// The result is a substring, possibly trimmed at a space
		// character, preceded by either descSep or a ' ' according
		// to the autoSep setting.
		if ( descSep ) {
			if ( addDescSpc ) result.append( 1, ' ' );
			if ( xPos && autoSep )
				result.append( 1, ' ' );
			else
				result.append( 1, descSep );
			if ( addDescSpc ) result.append( 1, ' ' );
		}

		// If the next character at xPos+length is not a space,
		// the last space before xPos+length must be found to
		// build the substring to return.
		if ( ( xPos + length ) <= descSize ) {
			size_t endpos = target->description.find_last_of( ' ', xPos + length );

			if ( endpos > xPos ) {
				result += target->description.substr( xPos, endpos - xPos );
				if ( pos ) *pos += endpos + 1;
			} else {
				result += target->description.substr( xPos, length - 1 );
				if ( pos ) *pos += length;
			}
		} else {
			result += target->description.substr( xPos );
			if ( pos ) *pos = target->description.size();
		}
	} else {
		result = "Unknown argument: ";
		result += argument;
	}

	return result;
}


std::string CArgHandler::getHelpStr( char const* argument, size_t length, size_t indent,
                                     char argSep, char paramSep, char descSep,
                                     bool autoSep, bool autoSpace ) const noexcept {
	std::string result;

	assert ( argument && strlen( argument )
	         && "ERROR: getHelpArg called with nullptr/empty argument!" );

	data_t* target = getTarget( argument );

	assert( target && "ERROR: Couldn't find given argument!" );

	if ( target ) {
		/** @todo : The three methods do the checking all the time.
		  * This should be done differently, with private non-checking
		  * methods doing the work, and the public versions only check
		  * and delegate.
		  * Priority: Low.
		**/
		size_t leftSize  = maxShortLen + maxLongLen + maxParamLen + 2 + indent;
		size_t rightSize = length > ( leftSize + 8 ) ? length - leftSize : 8;
		size_t descSize  = target->description.size();
		size_t pos       = 0;
		std::string desc;

		while ( pos < descSize ) {
			// Add left side:
			result.append( getHelpArg( argument, leftSize, indent,
			                           argSep, paramSep,
			                           pos ? true : false, autoSep, autoSpace ) );

			// Add right side
			desc.assign( getHelpDesc( argument, &pos, rightSize, descSep, autoSep, autoSpace ) );
			result.append( desc );

			// Any characters left, then add a line break:
			if ( pos < descSize )
				result += '\n';
		}

	} else {
		result = "Unknown argument: ";
		result += argument;
	}

	return result;
}


int32_t CArgHandler::parseArgs( const int32_t argc, char* argv[] ) noexcept {
	return parseArgs( argc, const_cast<char const**>( argv ) );
}


int32_t CArgHandler::parseArgs( const int32_t argc, char const* argv[] ) noexcept {
	// New round, new errors:
	if ( errlist.size() )
		errlist.clear();

	if ( ( argc > 0 ) && argv && argv[0] && ( !prgCall || !strlen( prgCall ) ) )
		/* Store argv[0] in prgCall */
		prgCall = strdup( argv[0] );

	if ( ( argc > 1 ) && argv && argv[1] ) {

		data_t*    lastTarget = nullptr; // holds the last target expecting a parameter
		int32_t    idx        = 1;
		arg_list_t arg_list( do_not_destroy );

		// The argument list does not need thread saftey:
		arg_list.disable_thread_safety();

		/* Move through argv until
		 * a) argc is reached or
		 * b) pass_init is reached.
		 */
		while ( ( idx < argc ) && argv[idx] && STRNE( argv[idx], pass_init ) ) {
			data_t*     target      = nullptr;
			arg_elem_t* arg_elem    = nullptr;
			bool        callProcess = false;

			// The target is either the current argv[idx] or
			// the oldest queued argument.
			if ( arg_list.empty() )
				target = getTarget( argv[idx] );
			else {
				arg_elem = arg_list.pop();
				target   = arg_elem->data.get();
			}


			/* Now there are four possibilities:
			 * 1.: target is set and will be processed.
			 * 2.: target is not set, but a parameter is waited for.
			 * 3.: argv[idx] is a combination of short args.
			 * 4.: This argv[idx] is unknown.
			*/
			if ( target ) {
				// === 1.: target is set and will be processed. ===
				/* There are three possible conditions here:
				 * a) The target does not need a parameter:
				 *    -> call process(nullptr)
				 * b) The target needs an argument:
				 *    -> substitute lastTarget after it has been checked for c)
				 * c) lastTarget did not get a parameter
				 *    -> Add error message about that
				*/
				if ( target->needsParameter() ) {
					if ( lastTarget && lastTarget->needsParameter()
					                && ( false == lastTarget->hasParameter() ) ) {
						// This is situation c)
						std::string param_error = "Argument "
						                          + ( lastTarget->arg_long.size() ? lastTarget->arg_long : lastTarget->arg_short )
						                          + " needs a parameter \""
						                          + lastTarget->param_name
						                          + "\"";
						sArgError* argError = new sArgError( AEN_PARAMETER_MISSING, param_error.c_str() );
						errlist.push( argError );
					} else
						// Situation b) after checking c)
						lastTarget = target;
				} else
					// Situation a)
					callProcess = true;

			} else if ( lastTarget && lastTarget->needsParameter() ) {
				// === 2.: target is not set, but a parameter is waited for. ===
				callProcess = true;
			} else {
				// === 3.: argv[idx] is a combination of short args. ? ===
				bool isCombined = false;
				try {
					isCombined = uncombine( argv[idx], arg_list );

					if ( !isCombined ) {
						// === 4.: This argv[idx] is unknown. ===
						std::string parse_error = "Unknown argument ";
						parse_error += argv[idx];
						sArgError* argError = new sArgError( AEN_ARGUMENT_UNKNOWN, parse_error.c_str() );
						errlist.push( argError );
					}
				} catch ( CException& e ) {
					// This must be noted, too!
					std::string parse_error = "Exception \"";
					parse_error += e.name();
					parse_error += "\" occurred while parsing \"";
					parse_error += argv[idx];
					parse_error += "\"\nWhat : \"";
					parse_error += e.what();
					parse_error += "\"\nWhere: \"";
					parse_error += e.where();
					parse_error += "\"\npFunc: \"";
					parse_error += e.pfunc();
					parse_error += "\"\nTrace: ";
					parse_error += e.trace();
					parse_error += "\n--------";
					sArgError* argError = new sArgError( AEN_PROCESSING_ERROR, parse_error.c_str() );
					errlist.push( argError );
					arg_list.clear();
				}
			} // End of argv tests

			// Final Step: Call process
			if ( callProcess ) {
				eArgErrorNumber argErrno = AEN_OK;
				try {
					argErrno = target ? target->process( nullptr ) :
					           lastTarget ? lastTarget->process( argv[idx] ) :
					           AEN_PROCESSING_ERROR;

					if ( AEN_OK != argErrno ) {
						std::string process_error = "Parameter \"";
						process_error += argv[idx];
						process_error += "\" error: \"";
						switch ( argErrno ) {
							case AEN_PARAM_TYPE_MISMATCH:
								process_error += "Wrong type for argument ";
								break;
							case AEN_PROCESSING_ERROR:
								process_error += "Processing error for argument ";
								break;
							case AEN_MULTIPLE_SET_PARAM:
								process_error += "More than one parameter for set argument ";
								break;
							default:
								process_error += "Unhandled errno " + to_string( ( int )argErrno ) + " for argument ";
						}
						process_error += ( lastTarget->arg_long.size() ? lastTarget->arg_long : lastTarget->arg_short );
						process_error += "\"";
						sArgError* argError = new sArgError( argErrno, process_error.c_str() );
						errlist.push( argError );
					}
				} catch( CException& e ) {
					std::string process_error = e.name();
					process_error += ": ";
					process_error += e.what();
					sArgError* argError = new sArgError( AEN_PROCESSING_ERROR, process_error.c_str() );
					errlist.push( argError );
				}
			}

			// Clean up queued and processed element
			if ( arg_elem ) {
				delete arg_elem;
				arg_elem = nullptr;
			}

			// Advance the index if nothing is queued
			if ( arg_list.empty() )
				++idx;
		} // End of looping argv

		// At this point idx might be smaller than argc, which
		// only means we have reached a passthrough condition.
		if ( idx < ( argc - 1 ) )
			// idx still points to the found pass_init, which is not passed.
			passThrough( argc - idx - 1, &argv[idx + 1] );

		return errlist.size();
	} // End of having arguments to parse.

	return 0;
}


CArgHandler::data_t* CArgHandler::getTarget( char const* arg ) const noexcept {
	data_t* target = nullptr;

	if ( arg && strlen( arg ) ) {
		std::string key = arg;

		if ( shortArgs.exists( key ) )
			target = shortArgs.get( key )->data.get();
		else if ( longArgs.exists( key ) )
			target = longArgs.get( key )->data.get();
	}

	return target;
}


char const* CArgHandler::getPrgCall() const noexcept {
	return prgCall;
}


void CArgHandler::passThrough( const int32_t argc, char const** argv ) noexcept {
	if ( argc > 0 ) {
		*pass_args = ( char** )calloc( argc, sizeof( char* ) );
		for ( int32_t i = 0; *pass_args && ( i < argc ); ++i )
			( *pass_args )[i] = strdup( argv[i] );
		*pass_cnt = argc;
	}
}


bool CArgHandler::uncombine( char const* arg, arg_list_t& arg_list ) {
	bool        allFound = true;
	std::string dashes   = "";
	char const* pChr     = arg;
	data_t*     target   = nullptr;

	// Step one: Get number of dashes
	while ( pChr && ( *pChr == '-' ) ) {
		dashes += '-';
		++pChr;
	}

	// Step two, loop through the remaining characters:
	while ( allFound && pChr && *pChr ) {
		std::string to_find = dashes + *pChr;
		if ( to_find.size() && ( target = getTarget( to_find.c_str() ) ) ) {
			arg_list.push( target );
			++pChr;
		} else {
			allFound = false;
			arg_list.clear();
		}
	}

	return allFound;
}


} // namespace pwx

