#pragma once
#ifndef PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED
#define PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED 1

#include "main.h" // This is here for IDE Parsers to find the other stuff

/// === ALPHA STAGE TESTING REQUIREMENTS ===
#ifndef PWX_THREADS
#define PWX_THREADS 1
#endif
/// === REMOVE AFTER TESTING



/** @brief Unified container speed test - single threaded
  *
  * The test is simple: Add maxElements random elements,
  * clear the container and measure how long this takes.
  *
  * Note: If this is set_t, only 10% of maxElements are used.
**/
template<typename list_t>
int32_t testSpeedST (sEnv &env)
{
	int32_t  result       = EXIT_SUCCESS;
	uint32_t localMaxElem = maxElements;

	cout << adjRight (4, 0) << ++env.testCount << " Test the speed of ";
	if (isSameType (list_t, single_list_t))
		cout << "singly linked lists ( 1 thread ) : ";
	else if (isSameType (list_t, double_list_t))
		cout << "doubly linked lists ( 1 thread ) : ";
	else if (isSameType (list_t, single_ring_t))
		cout << "singly linked rings ( 1 thread ) : ";
	else if (isSameType (list_t, double_ring_t))
		cout << "doubly linked rings ( 1 thread ) : ";
	else if (isSameType (list_t, stack_t))
		cout << "stacks              ( 1 thread ) : ";
	else if (isSameType (list_t, queue_t))
		cout << "queues              ( 1 thread ) : ";
	else if (isSameType (list_t, set_t)) {
		cout << "sets                ( 1 thread ) : ";
		localMaxElem /= 10;
	} else {
		cout << "nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}

	list_t intCont; // The list

	// To make the testing of the sets easier, we use a counting loop, so no doublets
	// are tried to be pushed onto a set.
	int32_t lowest, highest, maxAdd;

	// Find a range where we can have localMaxElem evenly scattered
	do {
		lowest  = pwx::RNG.random(::std::numeric_limits<int32_t>::min() + 1, 0);
		highest = pwx::RNG.random(::std::numeric_limits<int32_t>::max() - 1, 0);
		maxAdd  = (highest - lowest + 1) / localMaxElem;
	} while ((maxAdd < 2) || (static_cast<uint32_t>((highest - lowest) / maxAdd) < localMaxElem));

	// Now add localMaxElem integers and start the counting.
    clock_t startTime = clock();
	int32_t value = lowest;
	for (uint32_t pos = 0; pos < localMaxElem; ++pos) {
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
	if (localMaxElem != contSize) {
		cout << "    FAIL! Only " << contSize << "/" << localMaxElem << " elements inserted!" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else
		++env.testSuccess;

	return result;
}


#ifdef PWX_THREADS
// Little thread method to clear a container
template<typename list_t>
void thClr(list_t* cont)
{
	if (cont)
		cont->clear();
}

// little thread method to add elements to a container
template<typename list_t>
void thAdd(list_t* cont, data_t start, data_t toAdd, data_t maxAdd)
{
	if (cont) {
		for (data_t nr = 0; nr < toAdd; ++nr) {
			PWX_TRY(cont->push(new data_t(start)))
			PWX_CATCH_AND_FORGET(std::exception)
			start += pwx::RNG.random(1, maxAdd);
		}
	}
}
#endif


/** @brief Unified container speed test - multi threaded
  *
  * The test is simple: Add maxElements random elements,
  * clear the container and measure how long this takes.
  *
  * Note: If this is set_t, only 10% of maxElements are used.
**/
template<typename list_t>
int32_t testSpeedMT (sEnv &env)
{
	int32_t  result       = EXIT_SUCCESS;

#ifdef PWX_THREADS
	uint32_t localMaxElem = maxElements;

	cout << adjRight (4, 0) << ++env.testCount << " Test the speed of ";
	if (isSameType (list_t, single_list_t))
		cout << "singly linked lists (" << adjRight (2,0) << maxThreads << " threads)  : ";
	else if (isSameType (list_t, double_list_t))
		cout << "doubly linked lists (" << adjRight (2,0) << maxThreads << " threads)  : ";
	else if (isSameType (list_t, single_ring_t))
		cout << "singly linked rings (" << adjRight (2,0) << maxThreads << " threads)  : ";
	else if (isSameType (list_t, double_ring_t))
		cout << "doubly linked rings (" << adjRight (2,0) << maxThreads << " threads)  : ";
	else if (isSameType (list_t, stack_t))
		cout << "stacks              (" << adjRight (2,0) << maxThreads << " threads)  : ";
	else if (isSameType (list_t, queue_t))
		cout << "queues              (" << adjRight (2,0) << maxThreads << " threads)  : ";
	else if (isSameType (list_t, set_t)) {
		cout << "sets                (" << adjRight (2,0) << maxThreads << " threads)  : ";
		localMaxElem /= 10;
	} else {
		cout << "nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}
	cout.flush();

	list_t intCont; // The list

	// To make the testing of the sets easier, we use a counting loop, so no doublets
	// are tried to be pushed onto a set.
	int32_t lowest, highest, maxAdd, part, interval, rest;

	// Find a range where we can have localMaxElem evenly scattered
	do {
		lowest  = pwx::RNG.random(::std::numeric_limits<int32_t>::min() + 1, 0);
		highest = pwx::RNG.random(::std::numeric_limits<int32_t>::max() - 1, 0);
		maxAdd  = (highest - lowest + 1) / localMaxElem;
	} while ((maxAdd < 2) || (static_cast<uint32_t>((highest - lowest) / maxAdd) < localMaxElem));

	// The part is the number of elements each thread has to add:
	part     = localMaxElem / maxThreads;
	// The interval is the portion each thread has to add:
	interval = part * maxAdd;
	// The rest is the number of elements the final thread has to add
	// to achieve the total number of localMaxElem
	rest     = localMaxElem - (part * maxThreads);

	// Create the thread array:
    std::thread* threads[maxThreads];
    clock_t startTime = clock();

	// Starting in a loop ...
    for (size_t nr = 0; nr < maxThreads; ++nr) {
		PWX_TRY_STD_FURTHER(threads[nr] = new std::thread(thAdd<list_t>, &intCont, lowest,
														part + (nr == (maxThreads - 1) ? rest : 0),
														maxAdd),
							"Thread creation failed", "testSpeedMT could not call new operator on std::thread")
		lowest += interval;
    }
    // ... joining in a loop.
	for (size_t nr = 0; nr < maxThreads; ++nr) {
		threads[nr]->join();
		delete threads[nr];
		threads[nr] = nullptr;
    }

	// Now clear the container up again:
	uint32_t contSize = intCont.size();

	// Starting in a loop ...
    for (size_t nr = 0; nr < maxThreads; ++nr) {
		PWX_TRY_STD_FURTHER(threads[nr] = new std::thread(thClr<list_t>, &intCont),
							"Thread creation failed", "testSpeedMT could not call new operator on std::thread")
		lowest += interval;
    }
    // ... joining in a loop.
	for (size_t nr = 0; nr < maxThreads; ++nr) {
		threads[nr]->join();
		delete threads[nr];
		threads[nr] = nullptr;
    }

	// Bring out the needed time in ms:
	clock_t endTime = clock();
	double msNeeded = 1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC));
	cout << adjRight(5,0) << msNeeded << " ms" << endl;

	// Do we have had enough elements?
	if (localMaxElem != contSize) {
		cout << "    FAIL! Only " << contSize << "/" << localMaxElem << " elements inserted!" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else
		++env.testSuccess;
#endif // PWX_THREADS

	return result;
}


#endif // PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED

