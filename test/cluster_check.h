#ifndef PWX_LIBPWX_TEST_CLUSTER_CHECK_H_INCLUDED
#define PWX_LIBPWX_TEST_CLUSTER_CHECK_H_INCLUDED

#include <pwx.h>
using pwx::RNG;
using pwx::adjRight;

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


/// @internal build a numerical hash list
template<typename T>
int32_t build_cluster_num(string &outfile_chain, string &outfile_open, int32_t cnt_)
{
	typedef pwx::TChainHash<T, char> c_hash_t; //!< Type of the chained hash
	typedef pwx::TOpenHash<T, char>  o_hash_t; //!< Type of the open hash

	int32_t result = EXIT_SUCCESS;
	T maxTval = std::numeric_limits<T>::max();
	T minTval = std::numeric_limits<T>::lowest();

	// Chained hash output file:
	ofstream out_c(outfile_chain.c_str());
	if (!out_c.is_open() || !out_c.good()) {
		cerr << "Error opening \"" << outfile_chain << "\"" << endl;
		return EXIT_FAILURE;
	}

	// Open hash output file
	ofstream out_o(outfile_open.c_str());
	if (!out_o.is_open() || !out_o.good()) {
		cerr << "Error opening \"" << outfile_open << "\"" << endl;
		return EXIT_FAILURE;
	}

	// Correct maxTval/minTval if this is no integer
	if (isSameType(T, float)) {
		maxTval = (T)500000.0;
		minTval = (T)-500000.0;
	} else if (isSameType(T, double)) {
		maxTval = (T)50000.0;
		minTval = (T)-50000.0;
	} else if (isSameType(T, long double)) {
		maxTval = (T)5000.0;
		minTval = (T)-5000.0;
	} else if ((size_t)cnt_ > (size_t)maxTval)
		// Otherwise limit cnt_ to the known maximum value
		cnt_ = maxTval;

	T		 randVal; // This is the random key to use
	c_hash_t hash_c(static_cast<uint32_t>(cnt_ / 3.0) + 3, 0, 3.0, 1.25);
	o_hash_t hash_o(static_cast<uint32_t>(cnt_ / 0.8) + 3, 0, 0.8, 1.5 );

	cout << "Writing " << cnt_ << " values into \"" << outfile_chain;
	cout << "\" and \"" << outfile_open << "\" ...";
	cout.flush();

	hrTime_t tStart = hrClock::now();

	// Fill the hash containers
	for (int32_t cnt = 0 ; out_c.good() && (cnt < cnt_); ++cnt) {
		randVal = RNG.random(minTval, maxTval);

		// Test and insert into the chained hash:
		if (!hash_c.exists(randVal)) {
			char* storeVal = new char(0x20);
			hash_c.add(randVal, storeVal);
			out_c << hash_c.getHops(randVal) << ";" << randVal << "\n";
		}

		// Test and insert into the open hash:
		if (!hash_o.exists(randVal)) {
			char* storeVal = new char(0x20);
			hash_o.add(randVal, storeVal);
			out_o << hash_o.getHops(randVal) << ";" << randVal << "\n";
		}
	}

	hrTime_t tEnd = hrClock::now();
	auto tTimeNeeded = duration_cast<milliseconds>(tEnd - tStart).count();
	cout << "(" << tTimeNeeded << " ms)";

	if ((!out_c.good()) || (!out_o.good()))
		result = EXIT_FAILURE;

	if (out_c.is_open()) out_c.close();
	if (out_o.is_open()) out_o.close();

	if (EXIT_SUCCESS == result)
		cout << " done" << endl;
	else
		cout << " ERROR!" << endl;

	return result;
}


/// @internal build a string based cluster list
int32_t build_cluster_string(string &outfile_chain, string &outfile_open, int32_t cnt_)
{
	typedef pwx::TChainHash<string, char> c_hash_t; //!< Type of the chained hash
	typedef pwx::TOpenHash<string, char>  o_hash_t; //!< Type of the open hash

	int32_t result = EXIT_SUCCESS;

	// Chained hash output file:
	ofstream out_c(outfile_chain.c_str());
	if (!out_c.is_open() || !out_c.good()) {
		cerr << "Error opening \"" << outfile_chain << "\"" << endl;
		return EXIT_FAILURE;
	}

	// Open hash output file
	ofstream out_o(outfile_open.c_str());
	if (!out_o.is_open() || !out_o.good()) {
		cerr << "Error opening \"" << outfile_open << "\"" << endl;
		return EXIT_FAILURE;
	}

	char     randVal[17];
	c_hash_t hash_c(static_cast<uint32_t>(cnt_ / 3.0) + 3, 0, 3.0, 1.25);
	o_hash_t hash_o(static_cast<uint32_t>(cnt_ / 0.8) + 3, 0, 0.8, 1.5 );

	cout << "Writing " << cnt_ << " values into \"" << outfile_chain;
	cout << "\" and \"" << outfile_open << "\" ...";
	cout.flush();

	hrTime_t tStart = hrClock::now();

	// Fill the hash containers
	for (int32_t cnt = 0 ; out_c.good() && (cnt < cnt_); ++cnt) {
		RNG.random(randVal, 17, 17);

		// Test and insert into the chained hash:
		if (!hash_c.exists(randVal)) {
			char* storeVal = new char(0x20);
			hash_c.add(string(randVal), storeVal);
			out_c << hash_c.getHops(randVal) << ";" << randVal << "\n";
		}

		// Test and insert into the open hash:
		if (!hash_o.exists(randVal)) {
			char* storeVal = new char(0x20);
			hash_o.add(string(randVal), storeVal);
			out_o << hash_o.getHops(randVal) << ";" << randVal << "\n";
		}
	}

	hrTime_t tEnd = hrClock::now();
	auto tTimeNeeded = duration_cast<milliseconds>(tEnd - tStart).count();
	cout << "(" << tTimeNeeded << " ms)";

	if ((!out_c.good()) || (!out_o.good()))
		result = EXIT_FAILURE;

	if (out_c.is_open()) out_c.close();
	if (out_o.is_open()) out_o.close();

	if (EXIT_SUCCESS == result)
		cout << " done" << endl;
	else
		cout << " ERROR!" << endl;

	return result;
}


#endif // PWX_LIBPWX_TEST_CLUSTER_CHECK_H_INCLUDED
