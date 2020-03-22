/** @file testPAH.cpp
  * (c) 2007 - 2020 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include <PAH>
#include <string>
using std::string;

#include "testPAH.h"
using pwx::PAH;
using pwx::eArgType::AT_ZERO_OR_ONE;
using pwx::eArgType::AT_ZERO_OR_MANY;

// array to test callback function arguments
static string cbtarget[5] = {"", "", "", "", ""};

// And the callback function
static void cb_addstr( char const* arg, char const* param );
// Helper to clear the strings
static void cb_clrstr();

// array for fake argv
static int32_t xArgc = 0;
static char**  xArgv = nullptr;

// Helper to add/set/free the fake argc/argv
// index 0 is always "./testlib" !
static void addFakeArg( int32_t nr, char const* arg ); // Set arg number nr to arg
static void addFakePar( int32_t nr, char const* arg, char const* param ); // Quickly set both
static void clrFakeArg();                              // free xArgv
static void setFakeArg( int32_t aSize );               // (re)alloc to size aSize

/** @brief central function to test pwx::PAH
  *
  *************************************************************************
  ** Add some arguments and test them with fake argc/argv.               **
  *************************************************************************
**/
int32_t testPAH ( sEnv& env ) {
	int32_t result   = EXIT_SUCCESS;
	int32_t errCount = 0;

	// --- Targets for the argument parsing ---
	int    tgt_inc  = 0;
	int    tgt_add  = 0;
	string tgt_str;

	cout << "\nTest CArgHandler instance pwx::PAH\n-----------------------------------" << endl;

	/* 1) Add a predefined argument to increase tgt_inc
	 * --------------------------------------------------*/
	++env.testCount;
	pwx::TArgTarget inc_arg( "-i",
	                         "--increase",
	                         pwx::eArgTargetType::ATT_INC,
	                         pwx::eArgType::AT_EXACTLY_ONCE,
	                         &tgt_inc,
	                         nullptr,
	                         "Increase tgt_inc by one,",
	                         nullptr
	);
	try {
		PAH.addArg( inc_arg );
		++env.testSuccess;
	} catch ( pwx::CException& e ) {
		cout << "PAH.addArg( object ) failed: " << e.desc() << endl;
		++env.testFail;
	}
	/* 2) Add a mandatory anonymous argument to add a number to tgt_add
	 * ---------------------------------------------------------*/
	++env.testCount;
	try {
		PAH.addArg( pwx::TArgTarget( "-a",
					      "--add",
					      pwx::eArgTargetType::ATT_ADD,
					      pwx::eArgType::AT_EXACTLY_ONCE,
					      &tgt_add,
					      nullptr,
					      "Add parameter to tgt_add",
					      "value" )
		);
		++env.testSuccess;
	} catch ( pwx::CException& e ) {
		cout << "PAH.addArg( anonymous ) failed: " << e.desc() << endl;
		++env.testFail;
	}

	/* 3) Add an optiona anonymous argument to sub a number from tgt_add without parameter name
	 * -----------------------------------------------------------------------------------------*/
	++env.testCount;
	try {
		PAH.addArg( pwx::TArgTarget( "-s",
					      "--sub",
					      pwx::eArgTargetType::ATT_SUB,
					      pwx::eArgType::AT_ZERO_OR_ONE,
					      &tgt_add,
					      nullptr,
					      "Decrease tgt_add by parameter",
					      nullptr )
		);
		++env.testSuccess;
	} catch ( pwx::CException& e ) {
		cout << "PAH.addArg( anonymous ) failed: " << e.desc() << endl;
		++env.testFail;
	}

	/* 4) Add a positional parameter by moving a pointer
	 * ---------------------------------------------------------*/
	++env.testCount;
	pwx::TArgTarget<string>* arg_ptr = new pwx::TArgTarget(
		nullptr,
		nullptr,
		pwx::eArgTargetType::ATT_SET,
		pwx::eArgType::AT_ZERO_OR_ONE,
		&tgt_str,
		nullptr,
		"Source File",
		"file path"
	);
	try {
		PAH.addArg( arg_ptr );
		// No delete, the pointer was moved.
		++env.testSuccess;
	} catch ( pwx::CException& e ) {
		cout << "PAH.addArg( pointer ) failed: " << e.desc() << endl;
		++env.testFail;
	}

	/* 4) Let's give out our help text
	 * ---------------------------------------------------------*/
	PAH.setHelpParams( 80, 0, ' ', '|' );

	cout << "testPAH [OPTIONS] <argument>\n\n"
	     << PAH.getHelpStr( 1 ) << "\n"
	     << "\n\n[OPTIONS]\n"
	     << PAH.getHelpStr( "-a" ) << "\n"
	     << PAH.getHelpStr( "-i" ) << "\n"
	     << PAH.getHelpStr( "-s" ) << "\n"
	     << endl;

	/* 5) Add some fake arguments to test what we have with errors
	---------------------------------------------------------------*/
	++env.testCount;
	setFakeArg(4);
	addFakeArg( 1, "-x" ); // Doesn't exist
	addFakeArg( 2, "-i" ); // Simple increase
	addFakeArg( 3, "42" ); // Will be interpreted as positional, -a has the parameter
	// -a is mandatory and missing.

	errCount = PAH.parseArgs( xArgc, xArgv );
	cout << "  argv: " << xArgv[1] << ", " << xArgv[2] << ", " << xArgv[3] << endl;
	cout << "  -> Errors (must be 2) : " << errCount;
	if ( 2 == errCount ) {
		cout << " => Success" << endl;
		cout << "  -> Errors found: " << endl;
		++env.testSuccess;
	} else {
		cout << " => FAILED!" << endl;
		++env.testFail;
	}
	for ( int i = 1; i <= errCount; ++i ) {
		cout << adjRight( 7, 0 ) << i << ": ";
		cout << PAH.getErrorStr( i ) << " [";
		cout << PAH.getError( i ) << "]" << endl;
	}
	cout << "tgt_add : " << tgt_add << endl;
	cout << "tgt_inc : " << tgt_inc << endl;
	cout << "tgt_str ; " << tgt_str << endl;


	/* 6) Eventually test with working parameters
	-----------------------------------------------*/
	++env.testCount;
	PAH.resetTargets();
	setFakeArg( 7 );
	addFakePar( 1, "-a", "42" );
	addFakeArg( 3, "-i" );
	addFakePar( 4, "-s", "69" );
	addFakeArg( 6, "/foo/bar/baz" );
	errCount = PAH.parseArgs( xArgc, xArgv );
	cout << "\n  argv: " << xArgv[1] << ", " << xArgv[2] << ", " << xArgv[3]
	     << ", "       << xArgv[4] << ", " << xArgv[5] << ", " << xArgv[6] << endl;
	if ( errCount ) {
		cout << "  -> Errors found: " << endl;
		for ( int i = 1; i <= errCount; ++i ) {
			cout << adjRight( 7, 0 ) << i << ": ";
			cout << PAH.getErrorStr( i ) << " [";
			cout << PAH.getError( i ) << "]" << endl;
		}
		++env.testFail;
	} else
		++env.testSuccess;
	cout << "tgt_add : " << tgt_add << endl;
	cout << "tgt_inc : " << tgt_inc << endl;
	cout << "tgt_str ; " << tgt_str << endl;


	// clean up before returning
	clrFakeArg();
	cb_clrstr();

	return result;
}


// --- callback test function ---
// simply add the argument to the string array (push = back, unshift = front)
static void cb_addstr( char const* arg, char const* param ) {
	if ( arg && param ) {
		if ( STREQ( arg, "push" ) ) {
			for ( int i = 4; i >= 0; --i ) {
				if ( !cbtarget[i].size() ) {
					cbtarget[i].assign( param );
					i = -1;
				}
			}
		} else if ( STREQ( arg, "unshift" ) ) {
			for ( int i = 0; i < 5; ++i ) {
				if ( !cbtarget[i].size() ) {
					cbtarget[i].assign( param );
					i = 5;
				}
			}
		}
	}
}



// --- internal helpers ---

/// @internal clear the string array
static void cb_clrstr() {
	for ( int i = 0; i < 5; ++i )
		cbtarget[i].clear();
}


/// @internal Set arg number nr to arg
static void addFakeArg( int32_t nr, char const* arg ) {
	size_t argSize = arg ? strlen( arg ) + 1 : 0;
	if ( argSize ) {
		if ( nr >= xArgc )
			setFakeArg( nr + 1 );
		if ( xArgv[nr] )
			delete [] xArgv[nr];
		xArgv[nr] = new char[argSize + 1];
		memset( xArgv[nr], 0, argSize + 1 );
		strncpy( xArgv[nr], arg, argSize );
	}
}


/// @internal Set arg number nr to arg and nr+1 to param
static void addFakePar( int32_t nr, char const* arg, char const* param ) {
	size_t argSize = arg ? strlen( arg ) : 0;
	size_t parSize = param ? strlen( param ) : 0;
	if ( argSize && parSize ) {
		if ( ( nr + 1 ) >= xArgc )
			setFakeArg( nr + 2 );
		addFakeArg( nr, arg );
		addFakeArg( nr + 1, param );
	}
}


/// @internal free xArgv
static void clrFakeArg() {
	if ( xArgv ) {
		for ( int32_t i = 0; i < xArgc; ++i ) {
			if ( xArgv[i] ) {
				delete [] xArgv[i];
				xArgv[i] = nullptr;
			}
		}
		delete [] xArgv;
		xArgv = nullptr;
	}
	xArgc = 0;
}


/// @internal (re)alloc to size aSize
static void setFakeArg( int32_t aSize ) {
	if ( aSize > xArgc ) {
		int32_t oldArgc = xArgc;
		char**  oldArgv = xArgv;
		xArgc = aSize;
		xArgv = new char* [xArgc];
		for ( int32_t i = 0; i < xArgc; ++i ) {
			xArgv[i] = nullptr;
			if ( i < oldArgc ) {
				xArgv[i]   = oldArgv[i];
				oldArgv[i] = nullptr;
			} else if ( 0 == i )
				addFakeArg( 0, "./testLib" ); // this must be there!
		}
		if ( oldArgc )
			delete [] oldArgv;
	}
}
