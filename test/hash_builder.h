#pragma once
#ifndef PWX_LIBPWX_TEST_HASH_BUILDER_H_INCLUDED
#define PWX_LIBPWX_TEST_HASH_BUILDER_H_INCLUDED 1

/** @file hash_builder.h
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include <PBasic>
#include <PStreamHelpers>
#include <RNG>
using pwx::RNG;
typedef pwx::CAdjRight adjRight;

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

#endif // PWX_LIBPWX_TEST_HASH_BUILDER_H_INCLUDED
