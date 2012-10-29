#pragma once
#ifndef PWX_LIBPWX_TEST_MAIN_H_INCLUDED
#define PWX_LIBPWX_TEST_MAIN_H_INCLUDED 1

#include <ctime>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#ifndef PWX_THREADS
# define PWX_THREADS 1
#endif
#include <pwx.h>
using pwx::adjRight;

/// Type for the data to test with containers
typedef int32_t data_t;

/// Number of elements to use with all mass and speed tests
const uint32_t maxElements = 1000000;

#include "env.h"
#include "destroy.h"
#include "testListRing.h"
#include "testStackQueue.h"
#include "testSet.h"
#include "testSpeed.h"

#endif // PWX_LIBPWX_TEST_MAIN_H_INCLUDED
