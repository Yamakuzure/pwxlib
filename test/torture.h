#pragma once
#ifndef PWX_LIBPWX_TEST_TORTURE_H_INCLUDED
#define PWX_LIBPWX_TEST_TORTURE_H_INCLUDED 1

#include <pwx.h>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;


/// @brief enum to determine what is to be tested
enum eTestType
{
	E_TEST_ALL    =  1,
	E_TEST_LIST   =  2,
	E_TEST_LIST_D =  3,
	E_TEST_LIST_S =  4,
	E_TEST_QUEUE  =  5,
	E_TEST_RING   =  6,
	E_TEST_RING_D =  7,
	E_TEST_RING_S =  8,
	E_TEST_SET    =  9,
	E_TEST_STACK  = 10,
};

// --- Prototypes, the implementations are in torture.cpp below main() ---
int32_t setNumThreads(const char* chNum, uint32_t &numThreads);
int32_t setTestType(const char* chType, eTestType &testType);
int32_t startTest(uint32_t numThreads, eTestType testType);

#endif // PWX_LIBPWX_TEST_TORTURE_H_INCLUDED
