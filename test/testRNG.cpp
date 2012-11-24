#include <limits>
#include "testRNG.h"

using pwx::RNG;

const uint32_t hashMaxElements = maxElements * 20;
const int16_t maxInt16  = std::numeric_limits<int16_t>::max();
const int16_t minInt16  = std::numeric_limits<int16_t>::min();
const int32_t maxUInt32 = std::numeric_limits<uint32_t>::max();

/// @internal the test method for hash functions
template<typename T>
void testRNG_hash(sEnv &env)
{
	const T maxTval = std::numeric_limits<T>::max();
	const T minTval = std::numeric_limits<T>::min();
	uint32_t curHash  = 0;
	uint32_t minHash  = maxUInt32;
	uint32_t maxHash  = 0;
	T start    = static_cast<T>(RNG.random(minInt16, maxInt16));
	T interval = RNG.random((T)3, (T)9);

	auto tStart = hrClock::now();
	for (size_t i = 0; i < hashMaxElements; ++i) {
		curHash = RNG.hash(start);
		if (curHash > maxHash) maxHash = curHash;
		if (curHash < minHash) minHash = curHash;
		if (start < (maxTval - interval))
			start += interval;
		else
			start = minTval + (maxTval - start); // flip
	}

	auto tStop    = hrClock::now();
	auto tElapsed = duration_cast<milliseconds>(tStop - tStart).count();
	cout << adjRight(10,0) << minHash << " / " << adjRight(10,0) << maxHash << " (" << tElapsed << " ms) ";
	if (maxHash > minHash) {
		cout << "SUCCESS" << endl;
		++env.testSuccess;
	} else {
		cout << "FAILED!" << endl;
		++env.testFail;
	}
}

// The following two can not be templated and are defined below:
void testRNG_hash_char(sEnv &env);
void testRNG_hash_str(sEnv &env);


/** @brief central function to test pwx::RNG
  *
  *************************************************************************
  ** === All tests include time measurement with maxThreads threads  === **
  ** A) Hash functions                                                   **
  ** B) Noise functions                                                  **
  ** C) Random number functions                                          **
  ** D) Random name functions                                            **
  ** E) Simplex Noise functions                                          **
  *************************************************************************
**/
int32_t testRNG (sEnv &env)
{
	int32_t result = EXIT_SUCCESS;

	cout << "Test CRandom instance pwx::RNG\n------------------------------" << endl;

	/************************************************************************
	** A) Hash functions                                                   **
	************************************************************************/
	cout << " A) Hash functions with " << hashMaxElements << " keys (min/max)" << endl;
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
	** B) Noise functions                                                  **
	************************************************************************/

	/************************************************************************
	** C) Random number functions                                          **
	************************************************************************/

	/************************************************************************
	** D) Random name functions                                            **
	************************************************************************/

	/************************************************************************
	** E) Simplex Noise functions                                          **
	************************************************************************/

	return result;
}

/// @internal test the C-String hash function
void testRNG_hash_char(sEnv &env)
{
	char buf[18];
	uint32_t curHash  = 0;
	uint32_t minHash  = maxUInt32;
	uint32_t maxHash  = 0;
	size_t   strLen   = 0;
	memset(buf, 0, 18 * sizeof(char));

	auto tStart = hrClock::now();
	for (size_t i = 0; i < hashMaxElements; ++i) {
		strLen = RNG.random(buf, 8, 17);
		curHash = RNG.hash(buf, strLen);
		if (curHash > maxHash) maxHash = curHash;
		if (curHash < minHash) minHash = curHash;
	}

	auto tStop    = hrClock::now();
	auto tElapsed = duration_cast<milliseconds>(tStop - tStart).count();
	cout << adjRight(10,0) << minHash << " / " << adjRight(10,0) << maxHash << " (" << tElapsed << " ms) ";
	if (maxHash > minHash) {
		cout << "SUCCESS" << endl;
		++env.testSuccess;
	} else {
		cout << "FAILED!" << endl;
		++env.testFail;
	}

}

/// @internal test the std::string hash function
void testRNG_hash_str(sEnv &env)
{

}
