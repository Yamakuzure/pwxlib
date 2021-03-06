/** @file hash_builder.cpp
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
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


#include <PChainHash>
#include <PStreamHelpers>
#include <PStringUtils>
#include <RNG>
using pwx::RNG;
typedef pwx::CAdjRight adjRight;

#include <chrono>
typedef std::chrono::high_resolution_clock             hrClock;
typedef std::chrono::high_resolution_clock::time_point hrTime_t;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <limits>
#include <string>
using std::string;

#include <unistd.h>
using std::ofstream;


/// @internal build a numerical hash list
template< typename T >
int32_t build_hash_list_num( string &outfile, int32_t cnt_ ) {
	int32_t  result  = EXIT_SUCCESS;
	T        maxTval = std::numeric_limits< T >::max();
	T        minTval = std::numeric_limits< T >::lowest();
	ofstream out( outfile.c_str() );

	if ( !out.is_open() || !out.good() ) {
		cerr << "Error opening \"" << outfile << "\"" << endl;
		return EXIT_FAILURE;
	}

	// Correct maxTval/minTval if this is no integer
	if ( isSameType( T, float ) ) {
		maxTval = (T) 500000.0;
		minTval = (T) -500000.0;
	} else if ( isSameType( T, double ) ) {
		maxTval = (T) 50000.0;
		minTval = (T) -50000.0;
	} else if ( isSameType( T, long double ) ) {
		maxTval = (T) 5000.0;
		minTval = (T) -5000.0;
	}

	T randVal;

	cout << "Writing " << cnt_ << " values into \"" << outfile << "\" ...";
	cout.flush();

	hrTime_t tStart = hrClock::now();

	for ( int32_t cnt = 0 ; out.good() && ( cnt < cnt_ ) ; ++cnt ) {
		randVal = RNG.random( minTval, maxTval );
		if ( isSameType( T, float ) ) {
			out << adjRight( 7, 4 ) << randVal << ";" << RNG.hash( randVal ) << endl;
		} else if ( isSameType( T, double ) ) {
			out << adjRight( 6, 8 ) << randVal << ";" << RNG.hash( randVal ) << endl;
		} else if ( isSameType( T, long double ) ) {
			out << adjRight( 5, 12 ) << randVal << ";" << RNG.hash( randVal ) << endl;
		} else {
			out << randVal << ";" << RNG.hash( randVal ) << endl;
		}
	}

	hrTime_t tEnd        = hrClock::now();
	auto     tTimeNeeded = duration_cast< milliseconds >( tEnd - tStart ).count();
	cout << "(" << tTimeNeeded << " ms)";

	if ( !out.good() ) {
		result = EXIT_FAILURE;
	}

	if ( out.is_open() ) {
		out.close();
	}

	if ( EXIT_SUCCESS == result ) {
		cout << " done" << endl;
	} else {
		cout << " ERROR!" << endl;
	}

	return result;
}


/// @internal specialized variant to build an [u]int16_t hash list
template< typename T >
int32_t build_hash_list_int16( string &outfile ) {
	int32_t  result  = EXIT_SUCCESS;
	T        maxTval = std::numeric_limits< T >::max();
	T        minTval = std::numeric_limits< T >::lowest();
	ofstream out( outfile.c_str() );

	if ( !out.is_open() || !out.good() ) {
		cerr << "Error opening \"" << outfile << "\"" << endl;
		return EXIT_FAILURE;
	}

	cout << "Writing " << static_cast<int32_t>( maxTval - minTval ) << " values into \"" << outfile << "\" ...";
	cout.flush();

	hrTime_t tStart = hrClock::now();

	for ( T cnt = minTval ; out.good() && ( cnt < maxTval ) ; ++cnt ) {
		out << cnt << ";" << RNG.hash( cnt ) << endl;
	}

	hrTime_t tEnd        = hrClock::now();
	auto     tTimeNeeded = duration_cast< milliseconds >( tEnd - tStart ).count();
	cout << "(" << tTimeNeeded << " ms)";

	if ( !out.good() ) {
		result = EXIT_FAILURE;
	}

	if ( out.is_open() ) {
		out.close();
	}

	if ( EXIT_SUCCESS == result ) {
		cout << " done" << endl;
	} else {
		cout << " ERROR!" << endl;
	}

	return result;
}


/// @internal build a C-String based hash list
static int32_t build_hash_list_char( string &outfile, int32_t cnt_ ) {
	int32_t  result = EXIT_SUCCESS;
	ofstream out( outfile.c_str() );

	if ( !out.is_open() || !out.good() ) {
		cerr << "Error opening \"" << outfile << "\"" << endl;
		return EXIT_FAILURE;
	}

	char randVal[17];

	cout << "Writing " << cnt_ << " values into \"" << outfile << "\" : ";
	cout.flush();

	hrTime_t tStart = hrClock::now();

	for ( int32_t cnt = 0 ; out.good() && ( cnt < cnt_ ) ; ++cnt ) {
		RNG.random( randVal, 17, 17 );
		out << randVal << ";" << RNG.hash( randVal ) << endl;
	}

	hrTime_t tEnd        = hrClock::now();
	auto     tTimeNeeded = duration_cast< milliseconds >( tEnd - tStart ).count();
	cout << "(" << tTimeNeeded << " ms)";

	if ( !out.good() ) {
		result = EXIT_FAILURE;
	}

	if ( out.is_open() ) {
		out.close();
	}

	if ( EXIT_SUCCESS == result ) {
		cout << " done" << endl;
	} else {
		cout << " ERROR!" << endl;
	}

	return result;
}


int32_t main( int32_t argc, char** argv ) {
	int32_t result = EXIT_SUCCESS;

	pwx::init( true, nullptr, 0 );

	if ( ( argc < 3 ) || ( argc > 4 ) ) {
		cerr << "Usage:\n  " << basename( argv[0] ) << " <destination directory> <number of lines> [type]\n";
		cerr << " type is one of:\n";
		cerr << "   all   : build hash lists for all types (default)\n";
		cerr << "   char  : build the hash list for char* strings\n";
		cerr << "   float : build hash lists for float, double and long double\n";
		cerr << "   int   : build hash lists for [u]int16_t to [u]int64_t" << endl;
		return EXIT_FAILURE;
	}

	string destdir = argv[1];
	while ( !destdir.empty() && ( destdir[destdir.size() - 1] == '/' ) ) {
		destdir.erase( destdir.size() - 1 );
	}

	// check first argument
	if ( destdir.empty() ) {
		cerr << "No valid destination directory given!" << endl;
		return EXIT_FAILURE;
	} else if ( !pwx_file_exists( destdir.c_str() ) ) {
		cerr << "output path \"" << destdir << "\" does not exist!" << endl;
		return EXIT_FAILURE;
	} else if ( !pwx_file_isW( destdir.c_str() ) ) {
		cerr << "output path \"" << destdir << "\" is not writable!" << endl;
		return EXIT_FAILURE;
	}

	// check second argument
	int32_t argMax = pwx::to_int32( argv[2] );
	if ( argMax <= 0 ) {
		cerr << "Number \"" << argv[2] << "\" is no legal number (" << argMax << ")" << endl;
		return EXIT_FAILURE;
	}

	// Set the type and check optional third argument
	int32_t hash_type = 7; // this is all
	if ( argc == 4 ) {
		string hType = argv[3];
		if ( hType != "char" ) {
			hash_type = 1;
		} else if ( hType != "float" ) {
			hash_type = 2;
		} else if ( hType != "int" ) {
			hash_type = 4;
		} else if ( hType != "all" ) {
			cerr << "Hash type \"" << hType << "\" is unknown." << endl;
			return EXIT_FAILURE;
		}
	}

	cout << "Building hash lists in \"" << destdir << "\"." << endl;

	// --- int16_t ---
	//-----------------
	string destfile;
	if ( hash_type & 4 ) {
		destfile = destdir + "/hash_int16.csv";
		result   = build_hash_list_int16< int16_t >( destfile );
	}

	// --- uint16_t ---
	//------------------
	if ( ( EXIT_SUCCESS == result ) && ( hash_type & 4 ) ) {
		destfile = destdir + "/hash_uint16.csv";
		result   = build_hash_list_int16< uint16_t >( destfile );
	}

	// --- int32_t ---
	//-----------------
	if ( ( EXIT_SUCCESS == result ) && ( hash_type & 4 ) ) {
		destfile = destdir + "/hash_int32.csv";
		result   = build_hash_list_num< int32_t >( destfile, argMax );
	}

	// --- uint32_t ---
	//------------------
	if ( ( EXIT_SUCCESS == result ) && ( hash_type & 4 ) ) {
		destfile = destdir + "/hash_uint32.csv";
		result   = build_hash_list_num< uint32_t >( destfile, argMax );
	}

	// --- int64_t ---
	//-----------------
	if ( ( EXIT_SUCCESS == result ) && ( hash_type & 4 ) ) {
		destfile = destdir + "/hash_int64.csv";
		result   = build_hash_list_num< int64_t >( destfile, argMax );
	}

	// --- uint64_t ---
	//------------------
	if ( ( EXIT_SUCCESS == result ) && ( hash_type & 4 ) ) {
		destfile = destdir + "/hash_uint64.csv";
		result   = build_hash_list_num< uint64_t >( destfile, argMax );
	}

	// --- float ---
	//---------------
	if ( ( EXIT_SUCCESS == result ) && ( hash_type & 2 ) ) {
		destfile = destdir + "/hash_float.csv";
		result   = build_hash_list_num< float >( destfile, argMax );
	}

	// --- double ---
	//----------------
	if ( ( EXIT_SUCCESS == result ) && ( hash_type & 2 ) ) {
		destfile = destdir + "/hash_double.csv";
		result   = build_hash_list_num< double >( destfile, argMax );
	}

	// --- long double ---
	//---------------------
	if ( ( EXIT_SUCCESS == result ) && ( hash_type & 2 ) ) {
		destfile = destdir + "/hash_long_double.csv";
		result   = build_hash_list_num< long double >( destfile, argMax );
	}

	// --- char ---
	//--------------
	if ( ( EXIT_SUCCESS == result ) && ( hash_type & 1 ) ) {
		destfile = destdir + "/hash_string.csv";
		result   = build_hash_list_char( destfile, argMax );
	}

	pwx::finish();

	return result;
}
