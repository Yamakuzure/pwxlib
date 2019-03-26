/**
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

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <PBasic>
#include <PStreamHelpers>
#include <RNG>
using pwx::RNG;
using pwx::to_int32;
using pwx::to_string;

typedef pwx::CAdjLeft  adjLeft;
typedef pwx::CAdjRight adjRight;
typedef pwx::eNameSourceType nst_t;

static const int32_t ss_len = 14;
static const int32_t sl_len = 21;
static const int32_t ms_len = 17;
static const int32_t ml_len = 23;

static int32_t max_nc_len;

static void print_arg_err( char const *prog, char const *arg );
static void print_arg_unknown( char const *prog, char const *arg, char const *param );
static void print_help( char const *prog );
static void print_table( bool ss, bool sl, bool ms, bool ml );
static int  print_type( nst_t type );

int main( int argc, char *argv[] ) {
        int     result     = EXIT_SUCCESS;
        int32_t name_count = 100;
        bool    do_ss = true, do_sl = true, do_ms = true, do_ml = true;
        bool    len_short = true, len_long = true, type_single = true, type_multi = true;

        max_nc_len = 3;

        // First the arguments
        for ( int i = 1; i < argc; ++i ) {
                if ( STREQ( argv[i], "-c" ) || STREQ( argv[i], "--count" ) ) {
                        if ( ++i < argc ) {
                                name_count = to_int32( argv[i] );
                                max_nc_len = to_string( name_count ).size();
                                if ( max_nc_len < 3 ) max_nc_len = 3;
                        } else {
                                print_arg_err( argv[0], argv[i - 1] );
                                return EXIT_FAILURE;
                        }
                } else if ( STREQ( argv[i], "-h" ) || STREQ( argv[i], "--help" ) ) {
                        print_help( argv[0] );
                        return result;
                } else if ( STREQ( argv[i], "-l" ) || STREQ( argv[i], "--length" ) ) {
                        if ( ++i < argc ) {
                                if ( STRCEQ( argv[i], "short" ) ) {
                                        len_short = true;
                                        len_long  = false;
                                } else if ( STRCEQ( argv[i], "long" ) ) {
                                        len_short = false;
                                        len_long  = true;
                                } else if ( STRCEQ( argv[i], "both" ) ) {
                                        len_short = true;
                                        len_long  = true;
                                } else {
                                        print_arg_unknown( argv[0], argv[i - 1], argv[i] );
                                        return EXIT_FAILURE;
                                }
                        } else {
                                print_arg_err( argv[0], argv[i - 1] );
                                return EXIT_FAILURE;
                        }
                } else if ( STREQ( argv[i], "-s" ) || STREQ( argv[i], "--seed" ) ) {
                        if ( ++i < argc )
                                RNG.setSeed( to_int32( argv[i] ) );
                        else {
                                print_arg_err( argv[0], argv[i - 1] );
                                return EXIT_FAILURE;
                        }
                } else if ( STREQ( argv[i], "-t" ) || STREQ( argv[i], "--type" ) ) {
                        if ( ++i < argc )
                                if ( STRCEQ( argv[i], "single" ) ) {
                                        type_single = true;
                                        type_multi  = false;
                                } else if ( STRCEQ( argv[i], "multi" ) ) {
                                        type_single = false;
                                        type_multi  = true;
                                } else if ( STRCEQ( argv[i], "both" ) ) {
                                        type_single = true;
                                        type_multi  = true;
                                } else {
                                        print_arg_unknown( argv[0], argv[i - 1], argv[i] );
                                        return EXIT_FAILURE;
                                } else {
                                print_arg_err( argv[0], argv[i - 1] );
                                return EXIT_FAILURE;
                        }
                }
        }

        // Determine which classes to print:
        if ( !len_short || !type_single ) do_ss = false;
        if ( !len_short || !type_multi ) do_ms = false;
        if ( !len_long  || !type_single ) do_sl = false;
        if ( !len_long  || !type_multi ) do_ml = false;

        // Now loop through the name source types
        for ( nst_t type = ( nst_t )0; ( EXIT_SUCCESS == result ) && ( type < pwx::NST_NUM_TYPES ); ++type ) {
                result = print_type( type );

                if ( EXIT_SUCCESS == result ) {
                        double x = 0., y = 0., z = 0., w = 0.;

                        RNG.setNST( type );

                        cout << adjLeft( max_nc_len, 0 ) << "nr" << " |";
                        if ( do_ss )
                                cout << " " << adjLeft( ss_len, 0 ) << "single/short" << " |";
                        if ( do_sl )
                                cout << " " << adjLeft( sl_len, 0 ) << "single/long" << " |";
                        if ( do_ms )
                                cout << " " << adjLeft( ms_len, 0 ) << "multi/short" << " |";
                        if ( do_ml )
                                cout << " " << adjLeft( ml_len, 0 ) << "multi/long" << " |";
                        cout << endl;
                        print_table( do_ss, do_sl, do_ms, do_ml );

                        // Now print the names:
                        for ( int i = 0 ; ( EXIT_SUCCESS == result ) && ( i < name_count ) ; ++i ) {
                                char *single_short = do_ss ? RNG.rndName( x, y, z, w, false, false ) : nullptr;
                                char *single_long  = do_sl ? RNG.rndName( x, y, z, w, true,  false ) : nullptr;
                                char *multi_short  = do_ms ? RNG.rndName( x, y, z, w, false, true ) : nullptr;
                                char *multi_long   = do_ml ? RNG.rndName( x, y, z, w, true,  true ) : nullptr;

                                // Check single short
                                if ( do_ss && ( !single_short || !strlen( single_short ) ) ) {
                                        cerr << "CRITICAL: RNG.rndName(x, y, z, w, false, false) failed!" << endl;
                                        result = EXIT_FAILURE;
                                }
                                // Check single long
                                if ( do_sl && ( !single_long || !strlen( single_long ) ) ) {
                                        cerr << "CRITICAL: RNG.rndName(x, y, z, w, true, false) failed!" << endl;
                                        result = EXIT_FAILURE;
                                }
                                // Check multi short
                                if ( do_ms && ( !multi_short || !strlen( multi_short ) ) ) {
                                        cerr << "CRITICAL: RNG.rndName(x, y, z, w, false, true) failed!" << endl;
                                        result = EXIT_FAILURE;
                                }
                                // Check multi long
                                if ( do_ml && ( !multi_long || !strlen( multi_long ) ) ) {
                                        cerr << "CRITICAL: RNG.rndName(x, y, z, w, true, true) failed!" << endl;
                                        result = EXIT_FAILURE;
                                }

                                // If everything is fine, print the names:
                                if ( EXIT_SUCCESS == result ) {
                                        cout << adjLeft( max_nc_len, 0 ) << i << " |";
                                        if ( do_ss )
                                                cout << " " << adjLeft( ss_len, 0 ) << single_short << " |";
                                        if ( do_sl )
                                                cout << " " << adjLeft( sl_len, 0 ) << single_long << " |";
                                        if ( do_ms )
                                                cout << " " << adjLeft( ms_len, 0 ) << multi_short << " |";
                                        if ( do_ml )
                                                cout << " " << adjLeft( ml_len, 0 ) << multi_long << " |";
                                        cout << endl;

                                        // Advance coordinates:
                                        int32_t r = RNG.random( 1, 4 );
                                        if      ( 1 == r ) ++x;
                                        else if ( 2 == r ) ++y;
                                        else if ( 3 == r ) ++z;
                                        else if ( 4 == r ) ++w;
                                } // End of having four names

                                // Remove C-Strings if any
                                if ( single_short ) free( single_short );
                                if ( single_long )  free( single_long );
                                if ( multi_short )  free( multi_short );
                                if ( multi_long )   free( multi_long );
                        } // End of printing names loop

                        print_table( do_ss, do_sl, do_ms, do_ml );
                } // End of having a legal type;
        } // End of looping name source types

        return result;
}

static void print_arg_unknown( char const *prog, char const *arg, char const *param ) {
        cerr << "ERROR: Parameter \"" << param << "\" invalid for option \"" << arg << "\"" << endl;
        print_help( prog );
}

static void print_arg_err( char const *prog, char const *arg ) {
        cerr << "ERROR: Option \"" << arg << "\" needs a parameter!\n" << endl;
        print_help( prog );
}

static void print_help( char const *prog ) {
        cout << "Name Generator for " << PACKAGE_STRING << endl;
        cout << "----------------------------------" << endl;
        cout << "Usage: " << prog << " <options>\n" << endl;
        cout << "Options:" << endl;
        cout << "  -c / --count <number> : Number of names to generate (100)" << endl;
        cout << "  -h / --help           : print this help and exit" << endl;
        cout << "  -l / --length <type>  : 'short', 'long' or 'both' (default)" << endl;
        cout << "  -s / --seed  <number> : Set a manual seed (random)" << endl;
        cout << "  -t / --type  <type>   : 'single', 'multi' or 'both' (default)" << endl;

}

static void print_table( bool ss, bool sl, bool ms, bool ml ) {
        for ( int j = 0; j < max_nc_len; ++j ) cout << "-";
        cout << "-+";

        if ( ss ) {
                for ( int j = 0; j < ss_len; ++j ) cout << "-";
                cout << "--+";
        }
        if ( sl ) {
                for ( int j = 0; j < sl_len; ++j ) cout << "-";
                cout << "--+";
        }
        if ( ms ) {
                for ( int j = 0; j < ms_len; ++j ) cout << "-";
                cout << "--+";
        }
        if ( ml ) {
                for ( int j = 0; j < ml_len; ++j ) cout << "-";
                cout << "--+";
        }
        cout << endl;
}

static int print_type( nst_t type ) {
        int result = EXIT_SUCCESS;

        switch( type ) {
                case pwx::NST_NAMES_ALL:
                        cout << " === NST_NAMES_ALL ===" << endl;
                        break;
                case pwx::NST_NAMES_DE:
                        cout << " === NST_NAMES_DE ===" << endl;
                        break;
                case pwx::NST_NAMES_EN:
                        cout << " === NST_NAMES_EN ===" << endl;
                        break;
                case pwx::NST_TEXTS_ALL:
                        cout << " === NST_TEXTS_ALL ===" << endl;
                        break;
                case pwx::NST_TEXTS_DE:
                        cout << " === NST_TEXTS_DE ===" << endl;
                        break;
                case pwx::NST_TEXTS_EN:
                        cout << " === NST_TEXTS_EN ===" << endl;
                        break;
                default:
                        cerr << "unknown type " << type << " in " << __FUNCTION__ << endl;
                        result = EXIT_FAILURE;
        }

        return result;
}
