/** @file
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2020 PrydeWorX
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
#include "arg_handler/eArgTargetType.h"
#include "arg_handler/eArgType.h"
#include "basic/mem_utils.h"
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
	, shortArgs( 37, 64, 4.0, 1.733 ) {
	/* Turn of thread safety for the hashes and the error list.
	 * Multithreading does not make any sense, here! */
	errlist.disable_thread_safety();
	longArgs.disable_thread_safety();
	posQueue.disable_thread_safety();
	shortArgs.disable_thread_safety();
}

CArgHandler::~CArgHandler() noexcept {
	this->clearArgs();
	if ( prgCall )
		pwx_free( prgCall );
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
	pass_init = pwx_strdup( init_arg );
}


void CArgHandler::clearArgs() noexcept {
	errlist.clear();
	longArgs.clear();
	posQueue.clear();
	shortArgs.clear();

	if ( pass_init )
		pwx_free( pass_init );

	pass_args = nullptr;
	pass_init = nullptr;
	pass_cnt  = nullptr;

	if ( prgCall )
		pwx_free( prgCall );
	prgCall = nullptr;
}


std::string& CArgHandler::formatHelpArg( data_t* target, bool emptyLine ) const noexcept {
	static std::string result = "";

	size_t shortSize = target->arg_short.size();
	size_t longSize  = target->arg_long.size();
	size_t paramSize = target->param_name.size();
	size_t paramNeed = paramSize ? paramSize + 2 :target->needsParameter() ? 3 : 0;
	bool   addArgSpc = helpAutoSpace && helpArgSep && ( 0x20 != helpArgSep );
	bool   addParSpc = helpAutoSpace && helpParSep && ( 0x20 != helpParSep );

	// Start with indentation if set
	if ( helpIndent )
		result.assign( helpIndent, ' ' );
	else
		result.assign("");

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
		if ( helpArgSep ) {
			if ( addArgSpc ) result += ' ';

			if ( !helpAutoSep || ( !emptyLine && longSize ) )
				result += helpArgSep;
			else
				result += ' ';

			if ( addArgSpc ) result += ' ';
		}
	} else if ( longSize ) {
		result.append( maxShortLen + ( helpArgSep && helpAutoSep ? addArgSpc ? 3 : 1 : 0 ), ' ' );
		if ( helpArgSep && !helpAutoSep ) {
			if ( addArgSpc ) result += ' ';
			result += helpArgSep;
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
	} else if ( shortSize ) {
		result.append( maxLongLen + ( helpParSep && helpAutoSep && maxParamLen ? addParSpc ? 3 : 1 : 0 ), ' ' );
		if ( helpParSep && !helpAutoSep && maxParamLen ) {
			if ( addParSpc ) result += ' ';
			result += helpParSep;
			if ( addParSpc ) result += ' ';
		}
	} // end of handling long argument

	// === Third: argument parameter ===
	if ( (shortSize || longSize) && maxParamLen ) {
		// a) Add possible separator
		if ( helpParSep && longSize ) {
			if ( addParSpc ) result += ' ';

			if ( !helpAutoSep || ( !emptyLine && paramSize ) )
				result += helpParSep;
			else
				result += ' ';

			if ( addParSpc ) result += ' ';
		}

		// b) Add parameter or blanks
		if ( emptyLine || !target->needsParameter() )
			result.append( paramNeed, ' ' );
		else {
			result.append("<");
			result.append( paramSize ? target->param_name : "x" );
			result.append(">");
		}

		// c) insert spaces to left align the parameter
		if ( paramNeed < ( maxParamLen + 2 ) )
			result.append( maxParamLen + 2 - paramNeed, ' ' );
	} else if ( paramSize ) {
		if ( target->isMandatory() )
			result.append( "<" );
		else
			result.append( "[" );

		result.append( target->param_name );

		if ( target->isMandatory() )
			result.append( ">" );
		else
			result.append( "]" );

		if ( result.size() < helpSizeLeft )
			result.append( helpSizeLeft - result.size(), ' ' );
	} // End of handling parameters

	// Fourth: Before the result can be returned, it might need to be indented:
	if ( result.size() < helpSizeLeft )
		result.insert( 0, helpSizeLeft - result.size(), ' ' );

	return result;
}


std::string& CArgHandler::formatHelpDesc( data_t* target, size_t* pos, size_t length ) const noexcept {
	static std::string result;

	size_t descSize = target->description.size();
	size_t xPos     = pos ? *pos : 0;

	result.assign("");

	// Exit now if length is zero, it means the whole (or remainder) of the description
	// is to be returned, or when there isn't enough left for any further formatting.
	if ( !length || ( (target->description.size() - xPos) < length) ) {
		if (xPos < descSize)
			result.assign(target->description.substr( xPos ));
		if ( pos ) *pos = target->description.size();
		return result;
	}

	// If xPos is too large, the method can return now, too
	if ( xPos >= descSize )
		return result;

	// Are extra spaces needed ?
	bool addDescSpc = helpAutoSpace && helpDescSep && ( 0x20 != helpDescSep );

	// The result is a substring, possibly trimmed at a space
	// character, preceded by either helpDescSep or a ' ' according
	// to the autoSep setting.
	if ( helpDescSep ) {
		if ( addDescSpc ) result.append( 1, ' ' );
		if ( xPos && helpAutoSep )
			result.append( 1, ' ' );
		else
			result.append( 1, helpDescSep );
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

	return result;
}


std::string& CArgHandler::formatHelpStr( data_t* target ) const noexcept {
	static std::string result;

	size_t descSize  = target->description.size();
	size_t pos       = 0;
	size_t remaining = descSize;
	std::string desc;

	result.assign("");

	while ( pos < descSize ) {
		// Add left side:
		result.append( formatHelpArg( target, pos ? true : false ) );

		// Add right side
		desc.assign( formatHelpDesc( target, &pos, remaining < helpSizeRight ? 0 : helpSizeRight ) );
		result.append( desc );

		// Any characters left, then add a line break:
		if ( pos < descSize )
			result += '\n';

		remaining -= desc.size();
	}

	return result;
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


std::string& CArgHandler::getHelpArg( char const* argument, bool emptyLine ) const noexcept {
	static std::string errstr = "";

	assert ( argument && strlen( argument )
	         && "ERROR: getHelpArg called with nullptr/empty argument!" );

	data_t* target = getTarget( argument );

	assert( target && "ERROR: Couldn't find given argument!" );

	if ( target ) {
		updateLayout();
		return formatHelpArg( target, emptyLine );
	}

	errstr.assign( "Unknown argument: " );
	errstr.append( argument );
	return errstr;
}


std::string& CArgHandler::getHelpArg(uint32_t position, bool emptyLine ) const noexcept {
	static std::string errstr = "";

	assert ( position && "ERROR: getHelpArg called with zero position!" );

	data_t* target = getTarget( position );

	assert( target && "ERROR: Couldn't find given argument!" );

	if ( target ) {
		updateLayout();
		return formatHelpArg( target, emptyLine );
	}

	errstr.assign( "Unknown parameter for position " + std::to_string( position ) );
	return errstr;
}


std::string& CArgHandler::getHelpDesc( char const* argument, size_t* pos, size_t length ) const noexcept {
	static std::string errstr = "";

	assert ( argument && strlen( argument )
	         && "ERROR: getHelpDesc called with nullptr/empty argument!" );

	data_t* target = getTarget( argument );

	assert( target && "ERROR: Couldn't find given argument!" );

	if ( target ) {
		updateLayout();
		return formatHelpDesc( target, pos, length );
	}

	errstr.assign( "Unknown argument: " );
	errstr.append( argument );
	return errstr;
}


std::string& CArgHandler::getHelpDesc( uint32_t position, size_t* pos, size_t length ) const noexcept {
	static std::string errstr = "";

	assert ( position && "ERROR: getHelpDesc called with zero position!" );

	data_t* target = getTarget( position );

	assert( target && "ERROR: Couldn't find given argument!" );

	if ( target ) {
		updateLayout();
		return formatHelpDesc( target, pos, length );
	}

	errstr.assign( "Unknown parameter for position " + std::to_string( position ) );
	return errstr;
}


std::string& CArgHandler::getHelpStr( char const* argument ) const noexcept {
	static std::string errstr = "";

	assert ( argument && strlen( argument )
	         && "ERROR: getHelpStr called with nullptr/empty argument!" );

	data_t* target = getTarget( argument );

	assert( target && "ERROR: Couldn't find given argument!" );

	if ( target ) {
		updateLayout();
		return formatHelpStr( target );
	}

	errstr.assign( "Unknown argument: " );
	errstr.append( argument );
	return errstr;
}


std::string& CArgHandler::getHelpStr( uint32_t position ) const noexcept {
	static std::string errstr = "";

	assert ( position && "ERROR: getHelpStr called with zero position!" );

	data_t* target = getTarget( position );

	assert( target && "ERROR: Couldn't find given argument!" );

	if ( target ) {
		updateLayout();
		return formatHelpStr( target );
	}

	errstr.assign( "Unknown parameter for position " + std::to_string( position ) );
	return errstr;
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
		prgCall = pwx_strdup( argv[0] );

	// Early exit if there is nothing to parse
	if ( !(( argc > 1 ) && argv && argv[1]) )
		return 0;

	/***************************************************************************************************
	 * The schedule for our parsing is rather simple.
	 *
	 * 1) Walk through argv and prepare each entry:
	 *   a: If the entry leads to a stored target, add it to the arg_queue and proceed with 2)
	 *   b: Otherwise check whether we can 'unpack' the string, process the result and proceed with 2)
	 *   c: If no unpacking is possible, add an error if the entry starts with a dash
	 *   d: If the entry does not start with a dash, add it to param_queue
	 *
	 * 2) Walk through the arg_queue and process every target.
	 *   a: If the target needs a parameter, pop it from param_queue
	 *   b: Process the target with or without a parameter from a:
	 *
	 * 3) When the regular arguments are processed, check left over items in the param queue
	 *    against the positional targets queue.
	 *
	 * 4) Eventually walk through our target queues and add an error for each mandatory arguments that
	 *    was not met in 1/2/3.
	 **************************************************************************************************/

	arg_queue_t   arg_queue(   do_not_destroy ); //!< Queue of found arguments
	arg_queue_t   comb_queue(  do_not_destroy ); //!< Helper for uncombining clusters
	int32_t       idx(1);                        //!< The position in argv we are at
	std::string   param;                         //!< One parameter
	param_queue_t param_queue;                   //!< Queue of found argument parameters
	std::string   process_error;                 //!< Helper for building error messages
	data_t*       target;                        //!< TargTarget on VArgTargetBase pointer

	// The queues do not need thread saftey:
	arg_queue.disable_thread_safety();
	comb_queue.disable_thread_safety();
	param_queue.disable_thread_safety();


	/// === 1) Walk through argv and prepare each entry until argc or pass_init is reached ===
	/// --------------------------------------------------------------------------------------
	for ( ; ( idx < argc ) && argv[idx] && STRNE( argv[idx], pass_init ) ; ++idx ) {
		bool        isUncombined;
		uint32_t    dashCount;

		/* a: If the entry leads to a stored target, add it to the arg_queue *
		 * ----------------------------------------------------------------- */
		target = getTarget( argv[idx] );

		if ( target ) {
			arg_queue.push( target );
			continue;
		}

		/* b: Otherwise check whether we can 'unpack' the string, process the result *
		 * ------------------------------------------------------------------------- */
		isUncombined = uncombine( argv[idx], comb_queue, dashCount );
		if ( isUncombined ) {
			arg_queue += comb_queue;
			comb_queue.clear();
			continue;
		}

		/* c: If the entry has no leading dash, assume it to be an argument parameter *
		 * ---------------------------------------------------------------------------- */
		if ( 0 == dashCount ) {
			param_queue.push( new std::string( argv[idx] ) );
			continue;
		}

		// d: Oh, an error...
		process_error = "Unknown argument ";
		process_error += argv[idx];
		sArgError* argError = new sArgError( AEN_ARGUMENT_UNKNOWN, process_error.c_str() );
		errlist.push( argError );
	} // End of Step 1


	/// === 2) Walk through arg_queue and process what we found                            ===
	/// --------------------------------------------------------------------------------------
	arg_elem_t* arg_elem = nullptr;
	while ( (arg_elem = arg_queue.pop()) ) {
		param = "";

		target = arg_elem->data.get();

		/* a: If the target needs a parameter, pop it from param_queue *
		 * ----------------------------------------------------------- */
		if ( target->needsParameter() && param_queue.size() ) {
			param_elem_t* param_elem = param_queue.pop();
			param.assign( *(param_elem->data.get()) );
			delete param_elem;
		}

		/* b: Process the target with or without a parameter from (a) *
		 * ---------------------------------------------------------- */
		procTarget( target, param.size() ? param.c_str() : nullptr );

		delete arg_elem;
	} // End of processing the argument queue


	/// === 3) When the regular arguments are processed, check left over items in the param queue ===
	/// ===    against the positional targets queue.                                              ===
	/// ---------------------------------------------------------------------------------------------
	while ( param_queue.size() ) {
		param_elem_t* param_elem = param_queue.pop();
		param.assign( *(param_elem->data.get()) );
		delete param_elem;

		if ( posQueue.size() ) {
			arg_elem = posQueue.pop();
			target = arg_elem->data.get();

			procTarget( target, param.c_str() );
			PWX_TRY( posQueue.push( target ) )
			PWX_CATCH_AND_FORGET( CException )

			continue;
		}

		process_error  = "Unknown argument \"";
		process_error += param + "\"";
		sArgError* argError = new sArgError( AEN_ARGUMENT_UNKNOWN, process_error.c_str() );
		errlist.push( argError );
	}

	/// === 4) Eventually walk through our target queues and add an error for each mandatory ===
	/// ===    arguments that was not met in 1/2                                             ===
	/// ----------------------------------------------------------------------------------------
	size_t hash_size = shortArgs.size();
	for ( size_t i = 0; i < hash_size ; ++i ) {
		elem_t const* elem = shortArgs[i];

		while ( elem ) {
			elem_t* next = elem->getNext();
			target = elem->data.get();

			if ( target->isMandatory() && !target->isProcessed() ) {
				process_error  = "Mandatory argument \"";
				process_error += ( target->arg_long.size() ? target->arg_long : target->arg_short );
				process_error += "\" not found in command line!";
				sArgError* argError = new sArgError( AEN_MANDATORY_MISSING, process_error.c_str() );
				errlist.push( argError );
			}

			elem = next;
		}
	} // End of walking the short args list

	hash_size = longArgs.size();
	for ( size_t i = 0; i < hash_size ; ++i ) {
		elem_t const* elem = longArgs[i];

		while ( elem ) {
			elem_t* next = elem->getNext();
			target = elem->data.get();

			// Note: If the target has a short variant, it was already processed above.
			if ( (0 == target->arg_short.size()) && target->isMandatory() && !target->isProcessed() ) {
				process_error  = "Mandatory argument \"";
				process_error += target->arg_long;
				process_error += "\" not found in command line!";
				sArgError* argError = new sArgError( AEN_MANDATORY_MISSING, process_error.c_str() );
				errlist.push( argError );
			}

			elem = next;
		}
	} // End of walking the long args list


	// At this point idx might be smaller than argc, which
	// only means we have reached a passthrough condition.
	if ( (0 == errlist.size()) && (idx < ( argc - 1 )) )
		// idx still points to the found pass_init, which is not passed.
		passThrough( argc - idx - 1, &argv[idx + 1] );

	return errlist.size();
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


CArgHandler::data_t* CArgHandler::getTarget( uint32_t pos ) const noexcept {
	data_t* target = nullptr;

	if ( (pos > 0) && (pos <= posQueue.size()) )
		target = posQueue[pos]->data.get();

	return target;
}


char const* CArgHandler::getPrgCall() const noexcept {
	return prgCall;
}


void CArgHandler::passThrough( const int32_t argc, char const** argv ) noexcept {
	if ( argc > 0 ) {
		*pass_args = pwx_calloc(char*, argc);
		for ( int32_t i = 0; *pass_args && ( i < argc ); ++i )
			( *pass_args )[i] = pwx_strdup( argv[i] );
		*pass_cnt = argc;
	}
}


void CArgHandler::procTarget( data_t* target, char const* param ) {
	std::string     process_error;

	// Exit early if we are missing a needed parameter
	if ( target->needsParameter() && ( nullptr == param ) ) {
		process_error  = "Argument \"";
		process_error +=  ( target->arg_long.size() ? target->arg_long : target->arg_short );
		process_error += "\" needs a parameter <";
		process_error += ( target->param_name.size() ? target->param_name : "x" );
		process_error += ">";
		sArgError* argError = new sArgError( AEN_PARAMETER_MISSING, process_error.c_str() );
		errlist.push( argError );
		return;
	}

	try {
		eArgErrorNumber argErrno = target->process( param );

		if ( AEN_OK != argErrno ) {
			if ( param ) {
				process_error  = "Parameter \"";
				process_error += param ;
				process_error += "\" error: ";
			} else
				process_error = "Processing failed: ";

			switch ( argErrno ) {
				case AEN_PARAM_TYPE_MISMATCH:
					process_error += "Wrong type for argument \"";
					break;
				case AEN_PROCESSING_ERROR:
					process_error += "Processing error for argument \"";
					break;
				case AEN_MULTIPLE_SET_PARAM:
					process_error += "More than one parameter for set argument \"";
					break;
				default:
					process_error += "Unhandled errno " + to_string( ( int )argErrno ) + " for argument \"";
			}
			process_error += ( target->arg_long.size()  ? target->arg_long
			                 : target->arg_short.size() ? target->arg_short : target->param_name );
			process_error += "\"";
			sArgError* argError = new sArgError( argErrno, process_error.c_str() );
			errlist.push( argError );
		}
	} catch( CException& e ) {
		process_error  = "Exception caught while processing \"";
		process_error += ( target->arg_long.size() ? target->arg_long : target->arg_short );
		process_error += "\" [";
		process_error += e.name();
		process_error += "]: ";
		process_error += e.what();
		sArgError* argError = new sArgError( AEN_PROCESSING_ERROR, process_error.c_str() );
		errlist.push( argError );
	}
}

void CArgHandler::resetTargets() noexcept {
	elem_t*     hash_elem;
	size_t      hash_size = shortArgs.size();
	elem_t*     hash_next;
	arg_elem_t* pos_elem;
	size_t      pos_size  = posQueue.size();
	data_t*     target;

	for ( size_t i = 0; i < hash_size; ++i ) {
		hash_elem = shortArgs[i];

		while ( hash_elem ) {
			hash_next = hash_elem->getNext();
			target    = hash_elem->data.get();
			hash_elem = hash_next;
			target->reset();
		}
	} // End of walking the short args list

	hash_size = longArgs.size();
	for ( size_t i = 0; i < hash_size; ++i ) {
		hash_elem = longArgs[i];

		while ( hash_elem ) {
			hash_next = hash_elem->getNext();
			target    = hash_elem->data.get();
			hash_elem = hash_next;
			target->reset();
		}
	} // End of walking the long args list

	for ( size_t i = 0; i < pos_size; ++i ) {
		pos_elem = posQueue[i];
		pos_elem->data.get()->reset();
	}
}


void CArgHandler::setHelpParams( size_t length, size_t indent, char argSep, char descSep,
                                 char paramSep, bool autoSep, bool autoSpace ) noexcept {
	helpLength    = length;
	helpIndent    = indent;
	helpArgSep    = argSep;
	helpDescSep   = descSep;
	helpParSep    = paramSep;
	helpAutoSep   = autoSep;
	helpAutoSpace = autoSpace;
}


bool CArgHandler::uncombine( char const* arg, arg_queue_t& arg_list, uint32_t &dash_count ) {
	bool        allFound = true;
	std::string dashes   = "";
	char const* pChr     = arg;
	data_t*     target   = nullptr;

	// Step one: Get number of dashes
	dash_count = 0;
	while ( pChr && ( *pChr == '-' ) ) {
		dashes += '-';
		++pChr;
		++dash_count;
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


void CArgHandler::updateLayout( void ) const noexcept {
		helpSizeLeft  = maxShortLen + maxLongLen + maxParamLen + 2 + helpIndent;
		helpSizeRight = helpLength > ( helpSizeLeft + 8 ) ? helpLength - helpSizeLeft : 8;
}


} // namespace pwx

