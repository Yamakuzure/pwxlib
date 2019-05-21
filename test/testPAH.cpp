/** @file testPAH.cpp
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include <PAH>
#include <string>

#include "testPAH.h"
using pwx::PAH;
using pwx::eArgSetType::STT_ERROR;
using pwx::eArgSetType::STT_IGNORE;
using pwx::eArgSetType::STT_OVERWRITE;

// array to test callback function arguments
static std::string cbtarget[5] = {"", "", "", "", ""};

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
static void clrFakeArg();                            // free xArgv
static void setFakeArg( int32_t aSize );              // (re)alloc to size aSize

/** @brief central function to test pwx::PAH
  *
  *************************************************************************
  ** Add some arguments and test them with fake argc/argv.               **
  ** A) Add three arguments, one short, one long, one short and long.    **
  ** B) Print Help text using automatic text generation.                 **
  ** C) Parse fake argc/argv with invalid values and print errors.       **
  ** D) Parse fake argc/argv with valid values and print result.         **
  ** E) Clear all resources and add args for everything                  **
  ** F) Print Help text using automatic text generation.                 **
  ** G) Parse fake argc/argv with valid values and print result.         **
  ** H) Test three more params for the three set targets.                **
  ** I) Test combinated arguments with shifted parameters                **
  *************************************************************************
**/
int32_t testPAH ( sEnv& env ) {
    int32_t result = EXIT_SUCCESS;

    // --- Targets for the argument parsing ---
    bool tgt_bool = false;
    int  tgt_inc  = 0;
    int  tgt_add  = 0;

    cout << "\nTest CArgHandler instance pwx::PAH\n-----------------------------------" << endl;

    /************************************************************************
    ** A) Add three arguments, one short, one long, one short and long.    **
    ************************************************************************/
    cout << adjRight ( 4, 0 ) << ++env.testCount << " Add three arguments : ";
    try {
        PAH.addArg( "-h", "--help", pwx::eArgTargetType::ATT_TRUE, &tgt_bool,
                    "Print this help text", nullptr );
        PAH.addArg( "i", nullptr, pwx::eArgTargetType::ATT_INC, &tgt_inc,
                    "Increase value tgt_inc by one on each occurence", nullptr );
        PAH.addArg( nullptr, "add", pwx::eArgTargetType::ATT_ADD, &tgt_add,
                    "Add <num> to tgt_add on each occurence", "num" );

        // As none has thrown, this is a success.
        ++env.testSuccess;
        cout << "Success" << endl;
    } catch ( pwx::CException& e ) {
        cout << "FAILURE" << endl;
        cout << "   exception name()/what(): \"" <<e.name();
        cout << "\" / \"" << e.what() << "\"" << endl;
        ++env.testFail;
        return EXIT_FAILURE;
    }


    /************************************************************************
    ** B) Print Help text using automatic text generation.                 **
    ************************************************************************/
    cout << adjRight ( 4, 0 ) << ++env.testCount << " Print auto help text : " << endl;
    cout << "\n--- help text begin (40, 2, '|', ' ', ':', true, true) ---" << endl;
    cout << PAH.getHelpStr( "add", 40, 2, '|', ' ', ':', true, true ) << endl;
    cout << PAH.getHelpStr( "-h", 40, 2, '|', ' ', ':', true, true ) << endl;
    cout << PAH.getHelpStr( "i", 40, 2, '|', ' ', ':', true, true ) << endl;
    cout << "--- help text end ---\n" << endl;
    ++env.testSuccess;

    /************************************************************************
    ** C) Parse fake argc/argv with invalid values and print errors.       **
    ************************************************************************/
    cout << adjRight ( 4, 0 ) << ++env.testCount << " Parse invalid argv : " << endl;
    setFakeArg( 4 );
    addFakeArg( 1, "--hwlp" );  // typo
    addFakeArg( 2, "--add" );   // wrong, must be without --
    addFakeArg( 3, "42" );      // Should have been added.
    int errCount = PAH.parseArgs( xArgc, xArgv );
    cout << "  argv: " << xArgv[1] << ", " << xArgv[2] << ", " << xArgv[3] << endl;
    cout << "  -> Errors (must be 3) : " << errCount;
    if ( 3 == errCount ) {
        cout << " => Success" << endl;
        ++env.testSuccess;
    } else {
        cout << " => FAILURE" << endl;
        ++env.testFail;
    }
    if ( errCount ) {
        cout << "  -> Errors found: " << endl;
        for ( int i = 1; i <= errCount; ++i ) {
            cout << adjRight( 7, 0 ) << i << ": ";
            cout << PAH.getErrorStr( i ) << " [";
            cout << PAH.getError( i ) << "]" << endl;
        }
    }


    /************************************************************************
    ** D) Parse fake argc/argv with valid values and print result.         **
    ************************************************************************/
    cout << "\n" << adjRight ( 4, 0 ) << ++env.testCount << " Parse valid argv : " << endl;
    addFakeArg( 1, "i" );  // increase tgt_inc by 1
    addFakeArg( 2, "add" ); // add next value to tgt_add
    addFakeArg( 3, "2" );  // So the result is 1 and 2
    errCount = PAH.parseArgs( xArgc, xArgv );
    cout << "  argv: " << xArgv[1] << ", " << xArgv[2] << ", " << xArgv[3] << endl;
    cout << "  -> Errors (must be 0) : " << errCount;
    if ( 0 == errCount ) {
        cout << " => Success" << endl;

        // tgt_inc must be 1 now
        cout << "  -> tgt_inc (must be 1) : " << tgt_inc;
        if ( 1 == tgt_inc ) cout << " => Success" << endl;
        else              cout << " => FAILURE" << endl;

        // tgt_add must be 2 now
        cout << "  -> tgt_add (must be 2) : " << tgt_add;
        if ( 2 == tgt_add ) cout << " => Success" << endl;
        else              cout << " => FAILURE" << endl;

        if ( ( 1 == tgt_inc ) && ( 2 == tgt_add ) )
            ++env.testSuccess;
        else
            ++env.testFail;
    } else {
        cout << " => FAILURE" << endl;
        ++env.testFail;
    }
    if ( errCount ) {
        cout << "  -> Errors found: " << endl;
        for ( int i = 1; i <= errCount; ++i ) {
            cout << adjRight( 7, 0 ) << i << ": ";
            cout << PAH.getErrorStr( i ) << " [";
            cout << PAH.getError( i ) << "]" << endl;
        }
    }



    /************************************************************************
    ** E) Clear all resources and add args for everything                  **
    ************************************************************************/
    cout << "\n" << adjRight ( 4, 0 ) << ++env.testCount << " Create new argument set : ";
    PAH.clearArgs();

    // reset old targets and add new ones:
    tgt_bool = true; // Will be used to test ATT_FALSE now.
    tgt_inc  = 0;
    tgt_add  = 0;
    float       tgt_dec     = 0.f;
    double      tgt_sub     = 0.0;
    uint32_t    tgt_set_err = 0; // set type STT_ERROR
    long double tgt_set_ign = 0.0; // set type STT_IGNORE
    int64_t     tgt_set_ovw = 0; // set type STT_OVERWRITE

    // Pass through targets:
    // char const* init_arg, int32_t* pass_argc, char*** pass_argv
    int32_t pass_argc = 0;
    char**  pass_argv = nullptr;

    try {
        PAH.addArg( "-a", "--add", pwx::eArgTargetType::ATT_ADD, &tgt_add,
                    "Add num to tgt_add", "num" );
        PAH.addArg( "-d", "--dec", pwx::eArgTargetType::ATT_DEC, &tgt_dec,
                    "Decrease tgt_dec by 1.0", nullptr );
        PAH.addArg( "-f", "--false", pwx::eArgTargetType::ATT_FALSE, &tgt_bool,
                    "Set tgt_bool to false", nullptr );
        PAH.addArg( "-i", "--inc", pwx::eArgTargetType::ATT_INC, &tgt_inc,
                    "Increase tgt_inc by 1", nullptr );
        PAH.addArg( "-E", "--set_err", pwx::eArgTargetType::ATT_SET, &tgt_set_err,
                    "Set tgt_set_err to num", "num", STT_ERROR );
        PAH.addArg( "-I", "--set_ign", pwx::eArgTargetType::ATT_SET, &tgt_set_ign,
                    "Set tgt_set_ign to num", "num", STT_IGNORE );
        PAH.addArg( "-O", "--set_ovw", pwx::eArgTargetType::ATT_SET, &tgt_set_ovw,
                    "Set tgt_set_ovw to num", "num" ); // STT_OVERWRITE is default!
        PAH.addArg( "-s", "--sub", pwx::eArgTargetType::ATT_SUB, &tgt_sub,
                    "Substract num from tgt_sub", "num" );
        // add callbacks
        PAH.addArg( nullptr, "push",    cb_addstr, "Add word to end of cbtarget", "word" );
        PAH.addArg( nullptr, "unshift", cb_addstr, "Add word to start of cbtarget", "word" );

        // add passthrough system:
        PAH.addPassthrough( "--", &pass_argc, &pass_argv );

        // As none has thrown, this is a success.
        ++env.testSuccess;
        cout << "Success" << endl;
    } catch ( pwx::CException& e ) {
        cout << "FAILURE" << endl;
        cout << "   exception name()/what(): \"" <<e.name();
        cout << "\" / \"" << e.what() << "\"" << endl;
        ++env.testFail;
        return EXIT_FAILURE;
    }

    /************************************************************************
    ** F) Print Help text using automatic text generation.                 **
    ************************************************************************/
    cout << adjRight ( 4, 0 ) << ++env.testCount << " Print auto help text : " << endl;
    cout << "\n--- help text begin (50, 2, '|', ' ', ':', true, true) ---" << endl;
    cout << PAH.getHelpStr( "-a", 50, 2, '|', ' ', ':', true, true ) << endl;
    cout << PAH.getHelpStr( "-d", 50, 2, '|', ' ', ':', true, true ) << endl;
    cout << PAH.getHelpStr( "-f", 50, 2, '|', ' ', ':', true, true ) << endl;
    cout << PAH.getHelpStr( "-i", 50, 2, '|', ' ', ':', true, true ) << endl;
    cout << PAH.getHelpStr( "-E", 50, 2, '|', ' ', ':', true, true ) << endl;
    cout << PAH.getHelpStr( "-I", 50, 2, '|', ' ', ':', true, true ) << endl;
    cout << PAH.getHelpStr( "-O", 50, 2, '|', ' ', ':', true, true ) << endl;
    cout << PAH.getHelpStr( "-s", 50, 2, '|', ' ', ':', true, true ) << endl;
    cout << PAH.getHelpStr( "push", 50, 2, '|', ' ', ':', true, true ) << endl;
    cout << PAH.getHelpStr( "unshift", 50, 2, '|', ' ', ':', true, true ) << endl;

    cout << "--- help text end ---\n" << endl;
    ++env.testSuccess;

    /************************************************************************
    ** G) Parse fake argc/argv with valid values and print result.         **
    ************************************************************************/
    cout << "\n" << adjRight ( 4, 0 ) << ++env.testCount << " Parse big argv : " << endl;

    setFakeArg( 22 );

    addFakePar( 1, "-a", "1" );
    addFakeArg( 3, "-d" );
    addFakeArg( 4, "-f" );
    addFakeArg( 5, "-i" );
    addFakePar( 6, "-E", "2" );
    addFakePar( 8, "-I", "3" );
    addFakePar( 10, "-O", "4" );
    addFakePar( 12, "-s", "5" );
    addFakePar( 14, "push", "push_first" );
    addFakeArg( 16, "push_second" );
    addFakePar( 17, "unshift", "unshift_first" );
    addFakeArg( 19, "unshift_second" );
    addFakeArg( 20, "--" ); // Next are passed through
    addFakePar( 21, "Hello", "World!" );

    cout << " =>\"";
    for ( int32_t i = 0; i < xArgc; ++i )
        cout << ( i > 0 ? " " : "" ) << xArgv[i];
    cout << "\"<=" << endl;

    errCount = PAH.parseArgs( xArgc, xArgv );
    cout << "  -> Errors (must be 0) : " << errCount;
    if ( 0 == errCount ) {
        cout << " => Success" << endl;

        cout << "  -> tgt_bool (must be false)   : " << ( tgt_bool ? "true" : "false" );
        cout << " => " << ( tgt_bool ? "FAILURE" : "Success" ) << endl;

        cout << "  -> tgt_add (must be 1)        : " << tgt_add;
        cout << " => " << ( 1 == tgt_add ? "Success" : "FAILURE" ) << endl;

        cout << "  -> tgt_inc (must be 1)        : " << tgt_inc;
        cout << " => " << ( 1 == tgt_inc ? "Success" : "FAILURE" ) << endl;

        cout << "  -> tgt_dec (must be -1.0)     : " << tgt_dec;
        cout << " => " << ( pwx::areAlmostEqual( -1.f, tgt_dec ) ? "Success" : "FAILURE" ) << endl;

        cout << "  -> tgt_sub (must be -5.0)     : " << tgt_sub;
        cout << " => " << ( pwx::areAlmostEqual( -5.0, tgt_sub ) ? "Success" : "FAILURE" ) << endl;

        cout << "  -> set_err (must be 2)        : " << tgt_set_err;
        cout << " => " << ( 2 == tgt_set_err ? "Success" : "FAILURE" ) << endl;

        cout << "  -> set_ign (must be 3.0)      : " << tgt_set_ign;
        cout << " => " << ( pwx::areAlmostEqual( 3.L, tgt_set_ign ) ? "Success" : "FAILURE" ) << endl;

        cout << "  -> set_ovw (must be 4)        : " << tgt_set_ovw;
        cout << " => " << ( 4 == tgt_set_ovw ? "Success" : "FAILURE" ) << endl;

        cout << "  -> passed through argc (2)    : " << pass_argc;
        cout << " => " << ( 2 == pass_argc ? "Success" : "FAILURE" ) << endl;

        std::string passed = "";
        if ( pass_argv ) {
            if ( pass_argc > 0 ) {
                passed += pass_argv[0] ? pass_argv[0] : "";
                passed += " ";
            }
            if ( pass_argc > 1 )
                passed += pass_argv[1] ? pass_argv[1] : "";
        }
        cout << "  -> passed argv (Hello World!) : " << passed;
        cout << " => " << ( STREQ( "Hello World!", passed.c_str() ) ? "Success" : "FAILURE" ) << endl;

        // Dump pass_argv if pass_Argc is not 2:
        if ( 2 != pass_argc ) {
            cout << " => received faulty passed argv:" << endl;
            cout << " \"";
            for ( int32_t i = 0; i < pass_argc; ++i )
                cout << ( i ? " " : "" ) << pass_argv[i];
            cout << "\"" << endl;
        }

        if ( !tgt_bool
                && ( 1 == tgt_add )
                && ( 1 == tgt_inc )
                && pwx::areAlmostEqual( -1.f, tgt_dec )
                && pwx::areAlmostEqual( -5.0, tgt_sub )
                && ( 2 == tgt_set_err )
                && pwx::areAlmostEqual( 3.L, tgt_set_ign )
                && ( 4 == tgt_set_ovw )
                && ( 2 == pass_argc )
                && STREQ( "Hello World!", passed.c_str() ) )
            ++env.testSuccess;
        else
            ++env.testFail;
    } else {
        cout << " => FAILURE" << endl;
        ++env.testFail;
    }
    if ( errCount ) {
        cout << "  -> Errors found: " << endl;
        for ( int i = 1; i <= errCount; ++i ) {
            cout << adjRight( 7, 0 ) << i << ": ";
            cout << PAH.getErrorStr( i ) << " [";
            cout << PAH.getError( i ) << "]" << endl;
        }
    }


    /************************************************************************
    ** H) Test three more params for the three set targets.                **
    ************************************************************************/
    cout << "\n" << adjRight ( 4, 0 ) << ++env.testCount << " test set target types : " << endl;
    clrFakeArg();
    setFakeArg( 7 );
    addFakePar( 1, "-I", "6" ); // should be ignored
    addFakePar( 3, "-O", "7" ); // should be overwritten
    addFakePar( 5, "-E", "5" ); // should result in an error

    cout << " =>\"";
    for ( int32_t i = 0; i < xArgc; ++i )
        cout << ( i > 0 ? " " : "" ) << xArgv[i];
    cout << "\"<=" << endl;

    errCount = PAH.parseArgs( xArgc, xArgv );
    int32_t errNum = errCount ? PAH.getError( 1 ) : 0 ;
    cout << "  -> Errors (must be 1)    : " << errCount;
    cout << " => " << ( 1 == errCount ? "Success" : "FAILURE" ) << endl;
    cout << "  -> Error (must be 16)    : " << errNum;
    cout << " => " << ( 16 == errNum ? "Success" : "FAILURE" ) << endl;

    cout << "  -> set_err (must be 2)   : " << tgt_set_err;
    cout << " => " << ( 2 == tgt_set_err ? "Success" : "FAILURE" ) << endl;

    cout << "  -> set_ign (must be 3.0) : " << tgt_set_ign;
    cout << " => " << ( pwx::areAlmostEqual( 3.L, tgt_set_ign ) ? "Success" : "FAILURE" ) << endl;

    cout << "  -> set_ovw (must be 7)   : " << tgt_set_ovw;
    cout << " => " << ( 7 == tgt_set_ovw ? "Success" : "FAILURE" ) << endl;

    if ( ( 1 == errCount )
            && ( 16 == errNum )
            && ( 2 == tgt_set_err )
            && pwx::areAlmostEqual( 3.L, tgt_set_ign )
            && ( 7 == tgt_set_ovw ) )
        ++env.testSuccess;
    else
        ++env.testFail;

    if ( errCount ) {
        cout << "  -> Errors found: " << endl;
        for ( int i = 1; i <= errCount; ++i ) {
            cout << adjRight( 7, 0 ) << i << ": ";
            cout << PAH.getErrorStr( i ) << " [";
            cout << PAH.getError( i ) << "]" << endl;
        }
    }

    /************************************************************************
    ** I) Test combinated arguments with shifted parameters                **
    ************************************************************************/
    cout << "\n" << adjRight ( 4, 0 ) << ++env.testCount << " Test combined arguments : " << endl;

    clrFakeArg();
    setFakeArg( 2 );

    addFakePar( 1, "-dsfi", "5" );

    cout << " =>\"";
    for ( int32_t i = 0; i < xArgc; ++i )
        cout << ( i > 0 ? " " : "" ) << xArgv[i];
    cout << "\"<=" << endl;

    errCount = PAH.parseArgs( xArgc, xArgv );
    cout << "  -> Errors (must be 0) : " << errCount;
    if ( 0 == errCount ) {
        cout << " => Success" << endl;

        cout << "  -> tgt_bool (must be false)   : " << ( tgt_bool ? "true" : "false" );
        cout << " => " << ( tgt_bool ? "FAILURE" : "Success" ) << endl;

        cout << "  -> tgt_inc (must be 2)        : " << tgt_inc;
        cout << " => " << ( 2 == tgt_inc ? "Success" : "FAILURE" ) << endl;

        cout << "  -> tgt_dec (must be -2.0)     : " << tgt_dec;
        cout << " => " << ( pwx::areAlmostEqual( -2.f, tgt_dec ) ? "Success" : "FAILURE" ) << endl;

        cout << "  -> tgt_sub (must be -10.0)     : " << tgt_sub;
        cout << " => " << ( pwx::areAlmostEqual( -10.0, tgt_sub ) ? "Success" : "FAILURE" ) << endl;

        if ( !tgt_bool
                && ( 2 == tgt_inc )
                && pwx::areAlmostEqual( -2.f, tgt_dec )
                && pwx::areAlmostEqual( -10.0, tgt_sub ) )
            ++env.testSuccess;
        else
            ++env.testFail;
    } else {
        cout << " => FAILURE" << endl;
        ++env.testFail;
    }
    if ( errCount ) {
        cout << "  -> Errors found: " << endl;
        for ( int i = 1; i <= errCount; ++i ) {
            cout << adjRight( 7, 0 ) << i << ": ";
            cout << PAH.getErrorStr( i ) << " [";
            cout << PAH.getError( i ) << "]" << endl;
        }
    }


    // clean up before returning
    clrFakeArg();
    cb_clrstr();
    if ( pass_argv ) {
        for ( int32_t i = 0; i < pass_argc; ++i ) {
            if ( pass_argv[i] )
                free( pass_argv[i] );
        }
        free( pass_argv );
    }

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
    size_t argSize = arg ? strlen( arg ) + 1: 0;
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
