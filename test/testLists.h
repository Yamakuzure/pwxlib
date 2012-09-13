#pragma once
#ifndef PWX_LIBPWX_TESTLIB_TESTLISTS_H_INCLUDED
#define PWX_LIBPWX_TESTLIB_TESTLISTS_H_INCLUDED 1

#include <ctime>

// These are the functions that test TSingleList, TDoubleList, TSingleRing and TDoubleRing

int testSingleList(int &testCount, int &testSuccess, int &testFail);
int testDoubleList(int &testCount, int &testSuccess, int &testFail);
int testStack(int &testCount, int &testSuccess, int &testFail);
int testQueue(int &testCount, int &testSuccess, int &testFail);

#endif // PWX_LIBPWX_TESTLIB_TESTLISTS_H_INCLUDED

