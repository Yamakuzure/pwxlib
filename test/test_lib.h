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


typedef int32_t keydata_t; //!< Type for the data to test with list containers, and type for the keys of hash containers
typedef float   hashval_t; //!< Type for hash container values

/// Number of elements to use with all mass and speed tests
const uint32_t maxElements = 500000;

/// Number of threads to launch for parallel testing
const uint32_t maxThreads  = 8;

/// The types of tests to do
const uint32_t doTestContainers = 1;
const uint32_t doTestSpeed      = 2;
const uint32_t doTestRNG        = 4;
const uint32_t doTestSCT        = 8;

const uint32_t doTestAll        = doTestContainers
                                | doTestSpeed
                                | doTestRNG
                                | doTestSCT;


#include "env.h"
#include "destroy.h"
#include "testListRing.h"
#include "testStackQueue.h"
#include "testHash.h"
#include "testSet.h"
#include "testSpeed.h"
#include "testRNG.h"
#include "testSCT.h"

#endif // PWX_LIBPWX_TEST_MAIN_H_INCLUDED
