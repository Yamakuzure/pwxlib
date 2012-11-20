#pragma once
#ifndef PWX_LIBPWX_TEST_MAIN_H_INCLUDED
#define PWX_LIBPWX_TEST_MAIN_H_INCLUDED 1

/// === ALPHA STAGE TESTING REQUIREMENTS ===
#ifndef LIBPWX_DEBUG
#define LIBPWX_DEBUG 1
#endif
#ifndef PWX_THREADS
#define PWX_THREADS 1
#endif
#ifndef PWX_THREADDEBUG
#define PWX_THREADDEBUG 1
#endif
/// === REMOVE AFTER TESTING


#ifdef PWX_THREADS
# include <thread>
# include <chrono>
#endif
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <pwx.h>
using pwx::adjRight;

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

#endif // PWX_LIBPWX_TEST_MAIN_H_INCLUDED
