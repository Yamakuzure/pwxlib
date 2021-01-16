/**
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2020 PrydeWorX
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


#include "cluster_check.h"

#include "basic/string_utils.h"


/// @internal build a numerical hash list
template<typename T>
int32_t build_cluster_num( string& outfile_chain, string& outfile_open, int32_t cnt_, bool useBigHash ) {
	typedef PChainHash<T, char> c_hash_t; //!< Type of the chained hash
	typedef POpenHash <T, char> o_hash_t; //!< Type of the open hash

	static char hopBuf[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	int32_t result = EXIT_SUCCESS;
	T maxTval = std::numeric_limits<T>::max();
	T minTval = std::numeric_limits<T>::lowest();

	// Chained hash output file:
	ofstream out_c( outfile_chain.c_str() );
	if ( !out_c.is_open() || !out_c.good() ) {
		cerr << "Error opening \"" << outfile_chain << "\"" << endl;
		return EXIT_FAILURE;
	}

	// Open hash output file
	ofstream out_o( outfile_open.c_str() );
	if ( !out_o.is_open() || !out_o.good() ) {
		cerr << "Error opening \"" << outfile_open << "\"" << endl;
		return EXIT_FAILURE;
	}

	// Correct maxTval/minTval if this is no integer
	if ( isSameType( T, float ) ) {
		maxTval = ( T )500000.0;
		minTval = ( T ) - 500000.0;
	} else if ( isSameType( T, double ) ) {
		maxTval = ( T )50000.0;
		minTval = ( T ) - 50000.0;
	} else if ( isSameType( T, long double ) ) {
		maxTval = ( T )5000.0;
		minTval = ( T ) - 5000.0;
	} else if ( ( size_t )cnt_ > ( size_t )maxTval )
		// Otherwise limit cnt_ to the known maximum value
		cnt_ = maxTval;

	T		 randVal; // This is the random key to use
	c_hash_t hash_c( static_cast<uint32_t>( cnt_ / ( useBigHash ? 0.1 : 3.0 ) ) + 3, 0, 3.0, 1.25 );
	o_hash_t hash_o( static_cast<uint32_t>( cnt_ / ( useBigHash ? 0.1 : 0.8 ) ) + 3, 0, 0.8, 1.5 );

	cout << "Writing " << cnt_ << " values into \"" << outfile_chain;
	cout << "\" and \"" << outfile_open << "\" ...";
	cout.flush();

	hrTime_t tStart = hrClock::now();

	// Fill the hash containers
	for ( int32_t cnt = 0 ; cnt < cnt_; ++cnt ) {
		randVal = RNG.random( minTval, maxTval );

		// Test and insert into the chained hash:
		if ( !hash_c.exists( randVal ) ) {
			char* storeVal = new char( 0x20 );
			hash_c.add( randVal, storeVal );
		}

		// Test and insert into the open hash:
		if ( !hash_o.exists( randVal ) ) {
			char* storeVal = new char( 0x20 );
			hash_o.add( randVal, storeVal );
		}
	}

	// Print out results:
	uint32_t size_c = hash_c.sizeMax();
	uint32_t size_o = hash_o.sizeMax();
	for ( uint32_t idx = 0; out_c.good() && ( idx < size_c ); ++idx ) {
		auto elem = hash_c[idx];
		if ( elem ) {
			snprintf( hopBuf, 9, "%08u", elem->hops );
			out_c << hopBuf << ";" << elem->key << "\n";
		}
	}
	for ( uint32_t idx = 0; out_o.good() && ( idx < size_o ); ++idx ) {
		auto elem = hash_o[idx];
		if ( elem ) {
			snprintf( hopBuf, 9, "%08u", elem->hops );
			out_o << hopBuf << ";" << elem->key << "\n";
		}
	}

	hrTime_t tEnd = hrClock::now();
	auto tTimeNeeded = duration_cast<milliseconds>( tEnd - tStart ).count();
	cout << "(" << tTimeNeeded << " ms)";

	if ( ( !out_c.good() ) || ( !out_o.good() ) )
		result = EXIT_FAILURE;

	if ( out_c.is_open() ) out_c.close();
	if ( out_o.is_open() ) out_o.close();

	if ( EXIT_SUCCESS == result )
		cout << " done" << endl;
	else
		cout << " ERROR!" << endl;

	return result;
}


/// @internal build a string based cluster list
static int32_t build_cluster_string( string& outfile_chain, string& outfile_open, int32_t cnt_, bool useBigHash ) {
	typedef PChainHash<string, char> c_hash_t; //!< Type of the chained hash
	typedef POpenHash <string, char> o_hash_t; //!< Type of the open hash

	static char hopBuf[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	int32_t result = EXIT_SUCCESS;

	// Chained hash output file:
	ofstream out_c( outfile_chain.c_str() );
	if ( !out_c.is_open() || !out_c.good() ) {
		cerr << "Error opening \"" << outfile_chain << "\"" << endl;
		return EXIT_FAILURE;
	}

	// Open hash output file
	ofstream out_o( outfile_open.c_str() );
	if ( !out_o.is_open() || !out_o.good() ) {
		cerr << "Error opening \"" << outfile_open << "\"" << endl;
		return EXIT_FAILURE;
	}

	char     randVal[17];
	c_hash_t hash_c( static_cast<uint32_t>( cnt_ / ( useBigHash ? 0.1 : 3.0 ) ) + 3, 0, 3.0, 1.25 );
	o_hash_t hash_o( static_cast<uint32_t>( cnt_ / ( useBigHash ? 0.1 : 0.8 ) ) + 3, 0, 0.8, 1.5 );

	cout << "Writing " << cnt_ << " values into \"" << outfile_chain;
	cout << "\" and \"" << outfile_open << "\" ...";
	cout.flush();

	hrTime_t tStart = hrClock::now();

	// Fill the hash containers
	for ( int32_t cnt = 0 ; out_c.good() && ( cnt < cnt_ ); ++cnt ) {
		RNG.random( randVal, 17, 17 );

		// Test and insert into the chained hash:
		if ( !hash_c.exists( randVal ) ) {
			char* storeVal = new char( 0x20 );
			hash_c.add( string( randVal ), storeVal );
			snprintf( hopBuf, 9, "%08u", hash_c.getHops( randVal ) );
			out_c << hopBuf << ";" << randVal << "\n";
		}

		// Test and insert into the open hash:
		if ( !hash_o.exists( randVal ) ) {
			char* storeVal = new char( 0x20 );
			hash_o.add( string( randVal ), storeVal );
			snprintf( hopBuf, 9, "%08u", hash_o.getHops( randVal ) );
			out_o << hopBuf << ";" << randVal << "\n";
		}
	}

	hrTime_t tEnd = hrClock::now();
	auto tTimeNeeded = duration_cast<milliseconds>( tEnd - tStart ).count();
	cout << "(" << tTimeNeeded << " ms)";

	if ( ( !out_c.good() ) || ( !out_o.good() ) )
		result = EXIT_FAILURE;

	if ( out_c.is_open() ) out_c.close();
	if ( out_o.is_open() ) out_o.close();

	if ( EXIT_SUCCESS == result )
		cout << " done" << endl;
	else
		cout << " ERROR!" << endl;

	return result;
}


int32_t main( int32_t argc, char** argv ) {
	int32_t result = EXIT_SUCCESS;

	pwx::init( true, nullptr, 0 );

	if ( ( argc < 3 ) || ( argc > 5 ) ) {
		cerr << "Usage:\n  " << basename( argv[0] )
		     << " <destination directory> <number of hashes> [type] [bighash]\n"
		     << " type is one of:\n"
		     << "   all   : build cluster lists for all key types (default)\n"
		     << "   string: build the cluster list for string keys\n"
		     << "   float : build cluster lists for float, double and long double keys\n"
		     << "   int   : build cluster lists for [u]int16_t to [u]int64_t keys\n"
		     << "\n bighash:\n"
		     << "If this keyword is seen the hash tables are initialized with ten\n"
		     << "times the number of hashes to build. This is useful to detect\n"
		     << "secondary clustering." << endl;
		return EXIT_FAILURE;
	}

	string destdir = argv[1];
	while ( destdir.size() && ( destdir[destdir.size() - 1] == '/' ) )
		destdir.erase( destdir.size() - 1 );

	// check first argument
	if ( !destdir.size() ) {
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
	if ( !( argMax > 0 ) ) {
		cerr << "Number \"" << argv[2] << "\" is no legal number (" << argMax << ")" << endl;
		return EXIT_FAILURE;
	}

	// Set the type and check optional third argument
	int32_t hash_type  = 7; // this is all
	bool    useBigHash = false; // Never by default
	if ( argc >= 4 ) {
		string hType = argv[3];
		if ( !hType.compare( "string" ) )
			hash_type = 1;
		else if ( !hType.compare( "float" ) )
			hash_type = 2;
		else if ( !hType.compare( "int" ) )
			hash_type = 4;
		else if ( !hType.compare( "bighash" ) )
			useBigHash = true;
		else if ( hType.compare( "all" ) ) {
			cerr << "Hash type \"" << hType << "\" is unknown." << endl;
			return EXIT_FAILURE;
		}
	}

	// Check whether a fourth argument enables bighash
	if ( argc >= 5 ) {
		if ( STRCEQ( argv[4], "bighash" ) )
			useBigHash = true;
		else {
			cerr << "Option \"" << argv[4] << "\" is unknown." << endl;
			return EXIT_FAILURE;
		}
	}

	cout << "Building cluster lists in \"" << destdir << "\"." << endl;

	try {
		string destfile_chain, destfile_open;

		// --- int16_t ---
		//-----------------
		if ( hash_type & 4 ) {
			destfile_chain = destdir + "/cluster_int16_chain.csv";
			destfile_open  = destdir + "/cluster_int16_open.csv";
			result = build_cluster_num<int16_t>( destfile_chain, destfile_open, argMax, useBigHash );
		}

		// --- uint16_t ---
		//------------------
		if ( ( EXIT_SUCCESS == result ) && ( hash_type & 4 ) ) {
			destfile_chain = destdir + "/cluster_uint16_chain.csv";
			destfile_open  = destdir + "/cluster_uint16_open.csv";
			result = build_cluster_num<uint16_t>( destfile_chain, destfile_open, argMax, useBigHash );
		}

		// --- int32_t ---
		//-----------------
		if ( ( EXIT_SUCCESS == result ) && ( hash_type & 4 ) ) {
			destfile_chain = destdir + "/cluster_int32_chain.csv";
			destfile_open  = destdir + "/cluster_int32_open.csv";
			result = build_cluster_num<int32_t>( destfile_chain, destfile_open, argMax, useBigHash );
		}

		// --- uint32_t ---
		//------------------
		if ( ( EXIT_SUCCESS == result ) && ( hash_type & 4 ) ) {
			destfile_chain = destdir + "/cluster_uint32_chain.csv";
			destfile_open  = destdir + "/cluster_uint32_open.csv";
			result = build_cluster_num<uint32_t>( destfile_chain, destfile_open, argMax, useBigHash );
		}

		// --- int64_t ---
		//-----------------
		if ( ( EXIT_SUCCESS == result ) && ( hash_type & 4 ) ) {
			destfile_chain = destdir + "/cluster_int64_chain.csv";
			destfile_open  = destdir + "/cluster_int64_open.csv";
			result = build_cluster_num<int64_t>( destfile_chain, destfile_open, argMax, useBigHash );
		}

		// --- uint64_t ---
		//------------------
		if ( ( EXIT_SUCCESS == result ) && ( hash_type & 4 ) ) {
			destfile_chain = destdir + "/cluster_uint64_chain.csv";
			destfile_open  = destdir + "/cluster_uint64_open.csv";
			result = build_cluster_num<uint64_t>( destfile_chain, destfile_open, argMax, useBigHash );
		}

		// --- float ---
		//---------------
		if ( ( EXIT_SUCCESS == result ) && ( hash_type & 2 ) ) {
			destfile_chain = destdir + "/cluster_float_chain.csv";
			destfile_open  = destdir + "/cluster_float_open.csv";
			result = build_cluster_num<float>( destfile_chain, destfile_open, argMax, useBigHash );
		}

		// --- double ---
		//----------------
		if ( ( EXIT_SUCCESS == result ) && ( hash_type & 2 ) ) {
			destfile_chain = destdir + "/cluster_double_chain.csv";
			destfile_open  = destdir + "/cluster_double_open.csv";
			result = build_cluster_num<double>( destfile_chain, destfile_open, argMax, useBigHash );
		}

		// --- long double ---
		//---------------------
		if ( ( EXIT_SUCCESS == result ) && ( hash_type & 2 ) ) {
			destfile_chain = destdir + "/cluster_long_double_chain.csv";
			destfile_open  = destdir + "/cluster_long_double_open.csv";
			result = build_cluster_num<long double>( destfile_chain, destfile_open, argMax, useBigHash );
		}

		// --- string ---
		//---------------
		if ( ( EXIT_SUCCESS == result ) && ( hash_type & 1 ) ) {
			destfile_chain = destdir + "/cluster_string_chain.csv";
			destfile_open  = destdir + "/cluster_string_open.csv";
			result = build_cluster_string( destfile_chain, destfile_open, argMax, useBigHash );
		}

		// End of giant try
	} catch ( pwx::CException& e ) {
		cerr << "\n-----\npwx exception \"" << e.name() << "\" caught!" << endl;
		cerr << "What : \"" << e.what() << "\"" << endl;
		cerr << "What : \"" << e.desc() << "\"" << endl;
		cerr << "Where: \"" << e.where() << "\"" << endl;
		cerr << "pFunc: \"" << e.pfunc() << "\"" << endl;
		cerr << "\nTrace:\n" << e.trace() << "\n-----" << endl;
		result = EXIT_FAILURE;
	} catch ( std::exception& e ) {
		cerr << "\n-----\nstd exception caught!" << endl;
		cerr << "What : \"" << e.what() << "\"" << endl;
		result = EXIT_FAILURE;
	} catch ( ... ) {
		cerr << "\n-----\nSomething completely unknown was caught!" << endl;
		result = EXIT_FAILURE;
	}

	pwx::finish();

	return result;
}

