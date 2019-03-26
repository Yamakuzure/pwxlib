#ifndef PWX_LIBPWX_TEST_CLUSTER_CHECK_H_INCLUDED
#define PWX_LIBPWX_TEST_CLUSTER_CHECK_H_INCLUDED

/** @file cluster_check.h
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

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

#include "PChainHash"
#include "POpenHash"
#include "PStreamHelpers"
#include "RNG"
using   pwx::RNG;
typedef pwx::CAdjRight adjRight;


// The names are long, make them shorter:
typedef std::chrono::high_resolution_clock             hrClock;
typedef std::chrono::high_resolution_clock::time_point hrTime_t;
using   std::chrono::duration_cast;
using   std::chrono::milliseconds;


#endif // PWX_LIBPWX_TEST_CLUSTER_CHECK_H_INCLUDED
