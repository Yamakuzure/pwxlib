/** @file testRNG_func.cpp
  * (c) 2007 - 2020 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include <cstring>


#include "testRNG_func.h"

const double fullMaxDouble = std::numeric_limits<double>::max();
const double fullMinDouble = std::numeric_limits<double>::lowest();


/// @internal test the C-String hash function
void testRNG_hash_char( sEnv& env ) {
	char     buf[18];
	uint32_t curHash   = 0;
	uint32_t minHash   = maxUInt32;
	uint32_t maxHash   = 0;
	size_t   strLen    = 0;
	memset( buf, 0, 18 * sizeof( char ) );

	hrTime_t tStart, tUsed;
	hrTime_t tFull = hrClock::now();
	for ( size_t i = 0; i < maxHashVals; ++i ) {
		strLen = RNG.random( buf, 8, 17 );
		tStart = hrClock::now();
		curHash = RNG.hash( buf, strLen );
		tUsed += hrClock::now() - tStart;
		if ( curHash > maxHash ) maxHash = curHash;
		if ( curHash < minHash ) minHash = curHash;
	}

	hrTime_t tEnd = hrClock::now();
	auto tHashTime = duration_cast<milliseconds>( tUsed - hrTime_t() ).count();
	auto tFullTime = duration_cast<milliseconds>( tEnd - tFull ).count();
	cout << adjRight( 10, 0 ) << minHash << " / " << adjRight( 10, 0 ) << maxHash;
	cout << " (" << tHashTime << " / " << tFullTime << " ms) ";
	if ( maxHash > minHash ) {
		cout << "SUCCESS" << endl;
		++env.testSuccess;
	} else {
		cout << "FAILED!" << endl;
		++env.testFail;
	}
}


/// @internal test the std::string hash function
void testRNG_hash_str( sEnv& env ) {
	char        buf[18];
	std::string str;
	uint32_t    curHash   = 0;
	uint32_t    minHash   = maxUInt32;
	uint32_t    maxHash   = 0;
	memset( buf, 0, 18 * sizeof( char ) );

	hrTime_t tStart, tUsed;
	hrTime_t tFull = hrClock::now();
	for ( size_t i = 0; i < maxHashVals; ++i ) {
		RNG.random( buf, 8, 17 );
		str.assign( buf );
		tStart = hrClock::now();
		curHash = RNG.hash( str );
		tUsed += hrClock::now() - tStart;
		if ( curHash > maxHash ) maxHash = curHash;
		if ( curHash < minHash ) minHash = curHash;
	}

	hrTime_t tEnd = hrClock::now();
	auto tHashTime = duration_cast<milliseconds>( tUsed - hrTime_t() ).count();
	auto tFullTime = duration_cast<milliseconds>( tEnd - tFull ).count();
	cout << adjRight( 10, 0 ) << minHash << " / " << adjRight( 10, 0 ) << maxHash;
	cout << " (" << tHashTime << " / " << tFullTime << " ms) ";
	if ( maxHash > minHash ) {
		cout << "SUCCESS" << endl;
		++env.testSuccess;
	} else {
		cout << "FAILED!" << endl;
		++env.testFail;
	}
}


/// @internal test function for RNG.rndName()
void testRNG_name( sEnv& env, bool longName, bool multiWord ) {
	char* name = nullptr;
	bool  isOK = true;

	cout << adjRight( 4, 0 ) << ++env.testCount;
	cout << ( longName ? " long names, " : " short names, " );
	cout << ( multiWord ? "multi word" : "single word" ) << endl;
	cout << "      --------------------" << endl;

	for ( int i = 1 ; i <= 10 ; ++i ) {
		double x = RNG.random( 100.0 );
		double y = RNG.random( 100.0 );
		double z = RNG.random( 100.0 );
		name = RNG.rndName( x, y, z, longName, multiWord );
		if ( name ) {
			cout << "  " << adjRight( 4, 0 ) << i << ".: \"" << name << "\"" << endl;
			pwx_free( name );
		} else {
			cerr << "ERROR: rndName(" << x << ", " << y << ", " << z << ", ";
			cerr << ( longName ? "true, " : "false, " );
			cerr << ( multiWord ? "true)" : "false)" );
			cerr << " returned nullptr!" << endl;
			++env.testFail;
		}
	}

	if ( isOK )
		++env.testSuccess;
	cout << "      --------------------" << endl;
}


/// @internal test function for RNG.noise()
void testRNG_noise( sEnv& env, int32_t dimensions, int32_t maxVal, int32_t miss ) {
	double curNoise = 0.;
	double maxNoise = fullMinDouble;
	double minNoise = fullMaxDouble;

	hrTime_t tStart = hrClock::now();

	if ( 1 == dimensions ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " 1 dimension  x       : "; cout.flush();
		for ( int32_t x = 0; x < maxVal; ++x ) {
			curNoise = RNG.noise( x );
			if ( curNoise > maxNoise ) maxNoise = curNoise;
			if ( curNoise < minNoise ) minNoise = curNoise;
		}
		// Add "miss"ed iterations
		for ( int32_t val = 0; val < miss; ++val ) {
			curNoise = RNG.noise( maxVal + val );
			if ( curNoise > maxNoise ) maxNoise = curNoise;
			if ( curNoise < minNoise ) minNoise = curNoise;
		}
	} else if ( 2 == dimensions ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " 2 dimensions x,y     : "; cout.flush();
		for ( int32_t x = 0; x < maxVal; ++x ) {
			for ( int32_t y = 0; y < maxVal; ++y ) {
				curNoise = RNG.noise( x, y );
				if ( curNoise > maxNoise ) maxNoise = curNoise;
				if ( curNoise < minNoise ) minNoise = curNoise;
			}
		}
		// Add "miss"ed iterations
		for ( int32_t val = 0; val < miss; ++val ) {
			curNoise = RNG.noise( maxVal + val, maxVal + val );
			if ( curNoise > maxNoise ) maxNoise = curNoise;
			if ( curNoise < minNoise ) minNoise = curNoise;
		}
	} else if ( 3 == dimensions ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " 3 dimensions x,y,z   : "; cout.flush();
		for ( int32_t x = 0; x < maxVal; ++x ) {
			for ( int32_t y = 0; y < maxVal; ++y ) {
				for ( int32_t z = 0; z < maxVal; ++z ) {
					curNoise = RNG.noise( x, y, z );
					if ( curNoise > maxNoise ) maxNoise = curNoise;
					if ( curNoise < minNoise ) minNoise = curNoise;
				}
			}
		}
		// Add "miss"ed iterations
		for ( int32_t val = 0; val < miss; ++val ) {
			curNoise = RNG.noise( maxVal + val, maxVal + val, maxVal + val );
			if ( curNoise > maxNoise ) maxNoise = curNoise;
			if ( curNoise < minNoise ) minNoise = curNoise;
		}
	} else if ( 4 == dimensions ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " 4 dimensions x,y,z,w : "; cout.flush();
		for ( int32_t x = 0; x < maxVal; ++x ) {
			for ( int32_t y = 0; y < maxVal; ++y ) {
				for ( int32_t z = 0; z < maxVal; ++z ) {
					for ( int32_t w = 0; w < maxVal; ++w ) {
						curNoise = RNG.noise( x, y, z, w );
						if ( curNoise > maxNoise ) maxNoise = curNoise;
						if ( curNoise < minNoise ) minNoise = curNoise;
					}
				}
			}
		}
		// Add "miss"ed iterations
		for ( int32_t val = 0; val < miss; ++val ) {
			curNoise = RNG.noise( maxVal + val, maxVal + val, maxVal + val, maxVal + val );
			if ( curNoise > maxNoise ) maxNoise = curNoise;
			if ( curNoise < minNoise ) minNoise = curNoise;
		}
	} else {
		cerr << "Meh!\n" << dimensions << " dimensions in testRNG_noise() ?\nOh no no!" << endl;
		std::terminate();
	}


	// Now evaluate the results:
	hrTime_t tEnd = hrClock::now();
	auto tNoiseTime = duration_cast<milliseconds>( tEnd - tStart ).count();
	cout << adjRight( 2, 7 ) << minNoise << " / " << adjRight( 2, 7 ) << maxNoise;
	cout << " (" << tNoiseTime << " ms)";
	if ( maxNoise > minNoise ) {
		cout << " : SUCCESS" << endl;
		++env.testSuccess;
	} else {
		cout << " : FAILED!" << endl;
		++env.testFail;
	}
}
