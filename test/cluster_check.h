#ifndef PWX_LIBPWX_TEST_CLUSTER_CHECK_H_INCLUDED
#define PWX_LIBPWX_TEST_CLUSTER_CHECK_H_INCLUDED

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

#include <pwxlib.h>
using   pwx::RNG;
typedef pwx::CAdjRight adjRight;


// The names are long, make them shorter:
typedef std::chrono::high_resolution_clock hrClock;
typedef std::chrono::high_resolution_clock::time_point hrTime_t;
using std::chrono::duration_cast;
using std::chrono::milliseconds;


#endif // PWX_LIBPWX_TEST_CLUSTER_CHECK_H_INCLUDED
