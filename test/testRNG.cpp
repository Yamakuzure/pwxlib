#include "testRNG_func.h"

#include <limits>
#include <cmath>

const double fullMaxDouble = std::numeric_limits<double>::max();
const double fullMinDouble = std::numeric_limits<double>::lowest();

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
	int32_t result           = EXIT_SUCCESS;
	uint32_t testMaxElements = env.doSpeed ? rngMaxElements : 10;

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
	cout << " B) Hash functions with " << testMaxElements;
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
	cout << " C) Noise functions with " << testMaxElements << " keys (min/max)" << endl;
	// This is so small that we can do it here:
	int32_t maxX = testMaxElements;
	int32_t maxY = static_cast<int32_t>(std::floor(std::pow(static_cast<float>(maxX), 1. / 2.)));
	int32_t maxZ = static_cast<int32_t>(std::floor(std::pow(static_cast<float>(maxX), 1. / 3.)));
	int32_t maxW = static_cast<int32_t>(std::floor(std::pow(static_cast<float>(maxX), 1. / 4.)));
	int32_t miss = 0; // to correct rounding errors
	if (maxX < 1) maxX = 1;
	if (maxY < 1) maxY = 1;
	if (maxZ < 1) maxZ = 1;
	if (maxW < 1) maxW = 1;

	DEBUG_LOG("noise", "maxX: %d, maxY: %d, maxZ: %d, maxW: %d", maxX, maxY, maxZ, maxW)

	double curNoise = 0.;

	// A) 1 dimension
	double maxNoise = fullMinDouble;
	double minNoise = fullMaxDouble;
	DEBUG_LOG("noise", "%d + %d missed", maxX, miss)
	cout << adjRight (4, 0) << ++env.testCount << " 1 dimension  x       : "; cout.flush();
	for (int32_t x = 0; x < maxX; ++x) {
		curNoise = RNG.noise(x);
		if (curNoise > maxNoise) maxNoise = curNoise;
		if (curNoise < minNoise) minNoise = curNoise;
	}
	cout << adjRight(2,7) << minNoise << " / " << adjRight(2,7) << maxNoise;
	if (maxNoise > minNoise) {
		cout << " : SUCCESS" << endl;
		++env.testSuccess;
	} else {
		cout << " : FAILED!" << endl;
		++env.testFail;
	}

	// B) 2 dimensions
	maxNoise = fullMinDouble;
	minNoise = fullMaxDouble;
	miss     = testMaxElements - static_cast<int32_t>(std::pow(maxY, 2.));
	DEBUG_LOG("noise", "%d + %d missed", static_cast<int32_t>(std::pow(maxY, 2.)), miss)
	cout << adjRight (4, 0) << ++env.testCount << " 2 dimensions x,y     : "; cout.flush();
	for (int32_t x = 0; x < maxY; ++x) {
		for (int32_t y = 0; y < maxY; ++y) {
			curNoise = RNG.noise(x,y);
			if (curNoise > maxNoise) maxNoise = curNoise;
			if (curNoise < minNoise) minNoise = curNoise;
		}
	}
	// Add "miss"ed iterations
	for (int32_t val = 0; val < miss; ++val) {
		curNoise = RNG.noise(maxY + val, maxY + val);
		if (curNoise > maxNoise) maxNoise = curNoise;
		if (curNoise < minNoise) minNoise = curNoise;
	}
	cout << adjRight(2,7) << minNoise << " / " << adjRight(2,7) << maxNoise;
	if (maxNoise > minNoise) {
		cout << " : SUCCESS" << endl;
		++env.testSuccess;
	} else {
		cout << " : FAILED!" << endl;
		++env.testFail;
	}

	// C) 3 dimensions
	maxNoise = fullMinDouble;
	minNoise = fullMaxDouble;
	miss     = testMaxElements - static_cast<int32_t>(std::pow(maxZ, 3.));
	DEBUG_LOG("noise", "%d + %d missed", static_cast<int32_t>(std::pow(maxZ, 3.)), miss)
	cout << adjRight (4, 0) << ++env.testCount << " 3 dimensions x,y,z   : "; cout.flush();
	for (int32_t x = 0; x < maxZ; ++x) {
		for (int32_t y = 0; y < maxZ; ++y) {
			for (int32_t z = 0; z < maxZ; ++z) {
				curNoise = RNG.noise(x,y,z);
				if (curNoise > maxNoise) maxNoise = curNoise;
				if (curNoise < minNoise) minNoise = curNoise;
			}
		}
	}
	// Add "miss"ed iterations
	for (int32_t val = 0; val < miss; ++val) {
		curNoise = RNG.noise(maxZ + val, maxZ + val, maxZ + val);
		if (curNoise > maxNoise) maxNoise = curNoise;
		if (curNoise < minNoise) minNoise = curNoise;
	}
	cout << adjRight(2,7) << minNoise << " / " << adjRight(2,7) << maxNoise;
	if (maxNoise > minNoise) {
		cout << " : SUCCESS" << endl;
		++env.testSuccess;
	} else {
		cout << " : FAILED!" << endl;
		++env.testFail;
	}

	// D) 4 dimensions
	maxNoise = fullMinDouble;
	minNoise = fullMaxDouble;
	miss     = testMaxElements - static_cast<int32_t>(std::pow(maxW, 4.));
	DEBUG_LOG("noise", "%d + %d missed", static_cast<int32_t>(std::pow(maxW, 4.)), miss)
	cout << adjRight (4, 0) << ++env.testCount << " 4 dimensions x,y,z,w : "; cout.flush();
	for (int32_t x = 0; x < maxW; ++x) {
		for (int32_t y = 0; y < maxW; ++y) {
			for (int32_t z = 0; z < maxW; ++z) {
				for (int32_t w = 0; w < maxW; ++w) {
					curNoise = RNG.noise(x,y,z,w);
					if (curNoise > maxNoise) maxNoise = curNoise;
					if (curNoise < minNoise) minNoise = curNoise;
				}
			}
		}
	}
	// Add "miss"ed iterations
	for (int32_t val = 0; val < miss; ++val) {
		curNoise = RNG.noise(maxW + val, maxW + val, maxW + val, maxW + val);
		if (curNoise > maxNoise) maxNoise = curNoise;
		if (curNoise < minNoise) minNoise = curNoise;
	}
	cout << adjRight(2,7) << minNoise << " / " << adjRight(2,7) << maxNoise;
	if (maxNoise > minNoise) {
		cout << " : SUCCESS" << endl;
		++env.testSuccess;
	} else {
		cout << " : FAILED!" << endl;
		++env.testFail;
	}


	/************************************************************************
	** D) Random name functions                                            **
	************************************************************************/

	/************************************************************************
	** E) Simplex Noise functions                                          **
	************************************************************************/

	return result;
}

