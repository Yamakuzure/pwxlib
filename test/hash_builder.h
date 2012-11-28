#pragma once
#ifndef PWX_LIBPWX_TEST_HASH_BUILDER_H_INCLUDED
#define PWX_LIBPWX_TEST_HASH_BUILDER_H_INCLUDED 1

#include <chrono>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <limits>
#include <string>
using std::string;

#include <unistd.h>
#include <fstream>
using std::ofstream;

// The names are long, make them shorter:
typedef std::chrono::high_resolution_clock hrClock;
typedef std::chrono::high_resolution_clock::time_point hrTime_t;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

#include <pwx.h>
using pwx::RNG;

/// @internal build a numerical hash list
template<typename T>
int32_t build_hash_list_num(string &outfile, int32_t cnt_)
{
	int32_t result = EXIT_SUCCESS;
	T maxTval = std::numeric_limits<T>::max();
	T minTval = std::numeric_limits<T>::lowest();
	ofstream out(outfile.c_str());

	if (!out.is_open() || !out.good()) {
		cerr << "Error opening \"" << outfile << "\"" << endl;
		return EXIT_FAILURE;
	}

	// Correct maxTval/minTval if this is no integer
	if (isSameType(T, float)
		|| isSameType(T, double)
		|| isSameType(T, long double)) {
		maxTval = (T)5000.0;
		minTval = (T)-5000.0;
	}

	T randVal;

	cout << "Writing " << cnt_ << " values into \"" << outfile << "\" ...";
	cout.flush();

	hrTime_t tStart = hrClock::now();

	for (int32_t cnt = 0 ; out.good() && (cnt < cnt_); ++cnt) {
		randVal = RNG.random(minTval, maxTval);
		out << randVal << ";" << RNG.hash(randVal) << endl;
	}

	hrTime_t tEnd = hrClock::now();
	auto tTimeNeeded = duration_cast<milliseconds>(tEnd - tStart).count();
	cout << "(" << tTimeNeeded << " ms)";

	if (!out.good())
		result = EXIT_FAILURE;

	if (out.is_open())
		out.close();

	if (EXIT_SUCCESS == result)
		cout << " done" << endl;
	else
		cout << " ERROR!" << endl;

	return result;
}


/// @internal build a C-String based hash list
int32_t build_hash_list_char(string &outfile, int32_t cnt_)
{
	int32_t result = EXIT_SUCCESS;
	ofstream out(outfile.c_str());

	if (!out.is_open() || !out.good()) {
		cerr << "Error opening \"" << outfile << "\"" << endl;
		return EXIT_FAILURE;
	}

	char randVal[17];

	cout << "Writing " << cnt_ << " values into \"" << outfile << "\" : ";
	cout.flush();

	hrTime_t tStart = hrClock::now();

	for (int32_t cnt = 0 ; out.good() && (cnt < cnt_); ++cnt) {
		RNG.random(randVal, 17, 17);
		out << randVal << ";" << RNG.hash(randVal) << endl;
	}

	hrTime_t tEnd = hrClock::now();
	auto tTimeNeeded = duration_cast<milliseconds>(tEnd - tStart).count();
	cout << "(" << tTimeNeeded << " ms)";

	if (!out.good())
		result = EXIT_FAILURE;

	if (out.is_open())
		out.close();

	if (EXIT_SUCCESS == result)
		cout << " done" << endl;
	else
		cout << " ERROR!" << endl;

	return result;
}


#endif // PWX_LIBPWX_TEST_HASH_BUILDER_H_INCLUDED
