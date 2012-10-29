#pragma once
#ifndef PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED
#define PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED 1

#include "main.h" // This is here for IDE Parsers to find the other stuff

/** @brief Unified container speed test
  *
  * The test is simple: Add maxElements random elements,
  * clear the container and measure how long this takes.
**/
template<typename list_t>
int32_t testSpeed (sEnv &env)
{
	int32_t result = EXIT_SUCCESS;

	cout << adjRight (4, 0) << ++env.testCount << " Test the speed of ";
	if (isSameType (list_t, single_list_t))
		cout << "singly linked lists : ";
	else if (isSameType (list_t, double_list_t))
		cout << "doubly linked lists : ";
	else if (isSameType (list_t, single_ring_t))
		cout << "singly linked rings : ";
	else if (isSameType (list_t, double_ring_t))
		cout << "doubly linked rings : ";
	else if (isSameType (list_t, stack_t))
		cout << "stacks              : ";
	else if (isSameType (list_t, queue_t))
		cout << "queues              : ";
	else if (isSameType (list_t, set_t))
		cout << "sets                : ";
	else {
		cout << "nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}

	list_t intCont; // The list

	// To make the testing of the sets easier, we use a counting loop, so no doublets
	// are tried to be pushed onto a set.
	int32_t lowest, highest, maxAdd;

	// Find a range where we can have maxElements evenly scattered
	do {
		lowest  = pwx::RNG.random(::std::numeric_limits<int32_t>::min() + 1, 0);
		highest = pwx::RNG.random(::std::numeric_limits<int32_t>::max() - 1, 0);
		maxAdd  = (highest - lowest + 1) / maxElements;
	} while ((maxAdd < 2) || (static_cast<uint32_t>((highest - lowest) / maxAdd) < maxElements));

	// Now add maxElements integers and start the counting.
    clock_t startTime = clock();
	int32_t value = lowest;
	for (uint32_t pos = 0; pos < maxElements; ++pos) {
		PWX_TRY_PWX_FURTHER(intCont.push(new data_t(value)))
		value += pwx::RNG.random(1, maxAdd);
	}

	// Now clear them up again:
	uint32_t contSize = intCont.size();
	intCont.clear();

	// Bring out the needed time in ms:
	clock_t endTime = clock();
	double msNeeded = 1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC));
	cout << adjRight(5,0) << msNeeded << " ms" << endl;

	// Do we have had enough elements?
	if (maxElements != contSize) {
		cout << "    FAIL! Only " << contSize << "/" << maxElements << " elements inserted!" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else
		++env.testSuccess;

	return result;
}

#endif // PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED

