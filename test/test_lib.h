#pragma once
#ifndef PWX_LIBPWX_TEST_MAIN_H_INCLUDED
#define PWX_LIBPWX_TEST_MAIN_H_INCLUDED 1

#include <pwx.h>
using pwx::adjLeft;
using pwx::adjRight;


#include <thread>
#include <chrono>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

// The names are long, make them shorter:
typedef std::chrono::high_resolution_clock hrClock;
typedef std::chrono::high_resolution_clock::time_point hrTime_t;
using std::chrono::duration_cast;
using std::chrono::milliseconds;


/// Type for the data to test with containers
typedef int32_t data_t;

/// Number of elements to use with all mass and speed tests
const uint32_t maxElements = 500000;

/// Number of threads to launch for parallel testing
const uint32_t maxThreads  = 8;

#include "env.h"
#include "destroy.h"
#include "testListRing.h"
#include "testStackQueue.h"
#include "testSet.h"
#include "testSpeed.h"
#include "testRNG.h"

#endif // PWX_LIBPWX_TEST_MAIN_H_INCLUDED
