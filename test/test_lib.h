#pragma once
#ifndef PWX_LIBPWX_TEST_MAIN_H_INCLUDED
#define PWX_LIBPWX_TEST_MAIN_H_INCLUDED 1

#include <thread>
#include <chrono>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <pwxlib.h>
typedef pwx::CAdjLeft  adjLeft;
typedef pwx::CAdjRight adjRight;

// The names are long, make them shorter:
typedef std::chrono::high_resolution_clock hrClock;
typedef std::chrono::high_resolution_clock::time_point hrTime_t;
using std::chrono::duration_cast;
using std::chrono::milliseconds;


typedef int32_t keydata_t; //!< Type for the data to test with list containers, and type for the keys of hash containers
typedef float   hashval_t; //!< Type for hash container values

extern uint32_t maxElements;
extern uint32_t maxHashVals;
extern uint32_t maxThreads;

/// The types of tests to do
const uint32_t doTestContainers =  1;
const uint32_t doTestSpeed      =  2;
const uint32_t doTestRNG        =  4;
const uint32_t doTestSCT        =  8;
const uint32_t doTestCWaveColor = 16;
const uint32_t doTestPAH        = 32;

const uint32_t doTestAll        = doTestContainers
                                | doTestSpeed
                                | doTestRNG
                                | doTestSCT
                                | doTestCWaveColor
                                | doTestPAH;


#include "env.h"
#include "destroy.h"

#endif // PWX_LIBPWX_TEST_MAIN_H_INCLUDED
