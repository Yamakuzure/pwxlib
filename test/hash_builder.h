#pragma once
#ifndef PWX_LIBPWX_TEST_HASH_BUILDER_H_INCLUDED
#define PWX_LIBPWX_TEST_HASH_BUILDER_H_INCLUDED 1

#include <chrono>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

// The names are long, make them shorter:
typedef std::chrono::high_resolution_clock hrClock;
typedef std::chrono::high_resolution_clock::time_point hrTime_t;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

#include <pwx.h>


#endif // PWX_LIBPWX_TEST_HASH_BUILDER_H_INCLUDED
