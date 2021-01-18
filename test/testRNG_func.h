#pragma once
#ifndef PWX_LIBPWX_TEST_TESTRNG_FUNC_H_INCLUDED
#define PWX_LIBPWX_TEST_TESTRNG_FUNC_H_INCLUDED 1

/** @file testRNG_func.h
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include <limits>
#include <RNG>

#include "testRNG.h"
using pwx::RNG;


const int16_t maxInt16  = std::numeric_limits<int16_t>::max();
const int16_t minInt16  = std::numeric_limits<int16_t>::min();
const int32_t maxUInt32 = std::numeric_limits<uint32_t>::max();


/// @internal the test method for hash functions
template<typename T>
void testRNG_hash( sEnv& env ) {
	static const T maxTval = std::numeric_limits<T>::max();
	static const T minTval = std::numeric_limits<T>::lowest();
	uint32_t curHash   = 0;
	uint32_t minHash   = maxUInt32;
	uint32_t maxHash   = 0;
	T start    = static_cast<T>( RNG.random( minInt16, maxInt16 ) );
	T interval = RNG.random( ( T )3, ( T )9 );

	hrTime_t tStart, tUsed;
	hrTime_t tFull = hrClock::now();
	for ( size_t i = 0; i < maxHashVals; ++i ) {
		tStart = hrClock::now();
		curHash = RNG.hash( start );
		tUsed += hrClock::now() - tStart;
		if ( curHash > maxHash ) maxHash = curHash;
		if ( curHash < minHash ) minHash = curHash;
		if ( start < ( maxTval - interval ) )
			start += interval;
		else
			start = minTval + ( maxTval - start ); // flip
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

void testRNG_hash_char( sEnv& env );
void testRNG_hash_str( sEnv& env );
void testRNG_name( sEnv& env, bool longName, bool multiWord );
void testRNG_noise( sEnv& env, int32_t dimensions, int32_t maxVal, int32_t miss );

#endif // PWX_LIBPWX_TEST_TESTRNG_FUNC_H_INCLUDED

