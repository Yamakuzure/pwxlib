#include "testRNG_func.h"

#include <limits>
#include <cmath>


/** @brief central function to test pwx::RNG
  *
  *************************************************************************
  ** === All tests include time measurement with maxThreads threads  === **
  ** A) Random number functions                                          **
  ** B) Hash functions                                                   **
  ** C) Noise functions                                                  **
  ** D) Random name functions                                            **
  ** E) Simplex Noise functions                                          **
  *************************************************************************
**/
int32_t testRNG (sEnv &env)
{
	int32_t result = EXIT_SUCCESS;

	cout << "\nTest CRandom instance pwx::RNG\n------------------------------" << endl;

	/************************************************************************
	** A) Random number functions                                          **
	************************************************************************/
	cout << " A) Random functions (max) / (min, max)" << endl;

	cout << adjRight (4, 0) << ++env.testCount << " (u)int16_t  (-999, 999) / (500)" << endl;
	for (size_t i = 1; i < 11; ++i) {
		cout << adjRight(8,0) << i << ": ";
		cout << adjRight(5,0) << RNG.random((int16_t)-999, (int16_t)999) << " / ";
		cout << adjRight(3,0) << RNG.random((uint16_t)500) << endl;
	}
	++env.testSuccess;

	cout << adjRight (4, 0) << ++env.testCount << " (u)int32_t  (-1.0e6, 1.0e6) / (1.0e9)" << endl;
	for (size_t i = 1; i < 11; ++i) {
		cout << adjRight( 8,0) << i << ": ";
		cout << adjRight( 8,0) << RNG.random((int32_t)-1e6, (int32_t)1e6) << " / ";
		cout << adjRight(10,0) << RNG.random((uint32_t)1e9) << endl;
	}
	++env.testSuccess;

	cout << adjRight (4, 0) << ++env.testCount << " (u)int64_t  (-1.0e12, 1.0e12) / (1.0e15)" << endl;
	for (size_t i = 1; i < 11; ++i) {
		cout << adjRight( 8,0) << i << ": ";
		cout << adjRight(14,0) << RNG.random((int64_t)-1e12, (int64_t)1e12) << " / ";
		cout << adjRight(16,0) << RNG.random((uint64_t)1e15) << endl;
	}
	++env.testSuccess;

	cout << adjRight (4, 0) << ++env.testCount << " float       (-1.0, 1.0) / (1.0e-4)" << endl;
	for (size_t i = 1; i < 11; ++i) {
		cout << adjRight(8,0) << i << ": ";
		cout << adjRight(2,8) << RNG.random((float)-1.0, (float)1.0) << " / ";
		cout << adjRight(2,8) << RNG.random((float)1e-4) << endl;
	}
	++env.testSuccess;

	cout << adjRight (4, 0) << ++env.testCount << " double      (-9.999, 9.999) / (1.0e-8)" << endl;
	for (size_t i = 1; i < 11; ++i) {
		cout << adjRight(8,0) << i << ": ";
		cout << adjRight(2,12) << RNG.random((double)-9.999, (double)9.999) << " / ";
		cout << adjRight(2,12) << RNG.random((double)1e-8) << endl;
	}
	++env.testSuccess;

	cout << adjRight (4, 0) << ++env.testCount << " long double (-1.0e-4, 1.0e-4) / (1.0e-12)" << endl;
	for (size_t i = 1; i < 11; ++i) {
		cout << adjRight(8,0) << i << ": ";
		cout << adjRight(2,16) << RNG.random((long double)-1.0e-4, (long double)1.0e-4) << " / ";
		cout << adjRight(2,16) << RNG.random((long double)1e-12) << endl;
	}
	++env.testSuccess;

	cout << adjRight (4, 0) << ++env.testCount << " char        ( 8, 12) / (12)" << endl;
	{
		char   bufA[13]; // Left side
		char   bufB[13]; // Right side

		for (size_t i = 1; i < 11; ++i) {
			RNG.random(bufA, 8, 12);
			RNG.random(bufB, 0, 12);
			cout << adjRight(8,0) << i << ": \"" << adjLeft(12,0) << bufA;
			cout << "\" / \"" << adjLeft(12,0) << bufB << "\"" << endl;
		}
	}
	++env.testSuccess;


	/************************************************************************
	** B) Hash functions                                                   **
	************************************************************************/
	cout << " B) Hash functions with " << maxHashVals;
	cout << " keys (min/max (hash / loop ms))" << endl;
	cout << adjRight (4, 0) << ++env.testCount << " int16_t    : "; cout.flush();
	testRNG_hash<int16_t>(env);
	cout << adjRight (4, 0) << ++env.testCount << " uint16_t   : "; cout.flush();
	testRNG_hash<uint16_t>(env);
	cout << adjRight (4, 0) << ++env.testCount << " int32_t    : "; cout.flush();
	testRNG_hash<int32_t>(env);
	cout << adjRight (4, 0) << ++env.testCount << " uint32_t   : "; cout.flush();
	testRNG_hash<uint32_t>(env);
	cout << adjRight (4, 0) << ++env.testCount << " int64_t    : "; cout.flush();
	testRNG_hash<int64_t>(env);
	cout << adjRight (4, 0) << ++env.testCount << " uint64_t   : "; cout.flush();
	testRNG_hash<uint64_t>(env);
	cout << adjRight (4, 0) << ++env.testCount << " float      : "; cout.flush();
	testRNG_hash<float>(env);
	cout << adjRight (4, 0) << ++env.testCount << " double     : "; cout.flush();
	testRNG_hash<double>(env);
	cout << adjRight (4, 0) << ++env.testCount << " long double: "; cout.flush();
	testRNG_hash<long double>(env);
	cout << adjRight (4, 0) << ++env.testCount << " char*      : "; cout.flush();
	testRNG_hash_char(env);
	cout << adjRight (4, 0) << ++env.testCount << " string     : "; cout.flush();
	testRNG_hash_str(env);

	/************************************************************************
	** C) Noise functions                                                  **
	************************************************************************/
	cout << " C) Noise functions with " << maxElements << " keys (min/max (ms))" << endl;

	int32_t maxX = maxElements;
	int32_t maxY = static_cast<int32_t>(std::floor(std::pow(static_cast<float>(maxX), 1. / 2.)));
	int32_t maxZ = static_cast<int32_t>(std::floor(std::pow(static_cast<float>(maxX), 1. / 3.)));
	int32_t maxW = static_cast<int32_t>(std::floor(std::pow(static_cast<float>(maxX), 1. / 4.)));
	int32_t miss = 0; // to correct rounding errors
	if (maxX < 1) maxX = 1;
	if (maxY < 1) maxY = 1;
	if (maxZ < 1) maxZ = 1;
	if (maxW < 1) maxW = 1;

	// A) 1 dimension
	testRNG_noise(env, 1, maxX, miss);

	// B) 2 dimensions
	miss     = maxElements - static_cast<int32_t>(std::pow(maxY, 2.));
	testRNG_noise(env, 2, maxY, miss);

	// C) 3 dimensions
	miss     = maxElements - static_cast<int32_t>(std::pow(maxZ, 3.));
	testRNG_noise(env, 3, maxZ, miss);

	// D) 4 dimensions
	miss     = maxElements - static_cast<int32_t>(std::pow(maxW, 4.));
	testRNG_noise(env, 4, maxW, miss);


	/************************************************************************
	** D) Random name functions                                            **
	************************************************************************/
	cout << " D) Random name functions, 10 names each" << endl;
	testRNG_name(env, false, false);
	testRNG_name(env, false, true);
	testRNG_name(env, true,  false);
	testRNG_name(env, true,  true);

	/************************************************************************
	** E) Simplex Noise functions                                          **
	************************************************************************/

	return result;
}

