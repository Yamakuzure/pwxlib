#pragma once
#ifndef PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED
#define PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED 1

#include "main.h" // This is here for IDE Parsers to find the other stuff


/** @brief Unified container speed test - single threaded
  *
  * The test is simple: Add maxElements random elements,
  * clear the container and measure how long this takes.
**/
template<typename list_t>
int32_t testSpeedST (sEnv &env)
{
	int32_t result = EXIT_SUCCESS;

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
	else if (isSameType (list_t, set_t))
		cout << "sets                ( 1 thread ) : ";
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


#ifdef PWX_THREADS
// little thread class to add elements to a list
template<typename list_t, typename data_t>
class worker
{
public:
	worker() PWX_DEFAULT;

	void init(size_t tNr_, list_t* cont_, data_t start_, data_t toAdd_, data_t maxAdd_)
	{
		tNr    = tNr_;
		cont   = cont_;
		start  = start_;
		toAdd  = toAdd_;
		maxAdd = maxAdd_;
	}

	void operator()()
	{
		for (data_t nr = 0; nr < toAdd; ++nr) {
			PWX_TRY(cont->push(new data_t(start)))
			PWX_CATCH_AND_FORGET(std::exception)
			start += pwx::RNG.random(1, maxAdd);
		}
		isRunning = false;
	}

	bool isRunning = true;

private:
	size_t  tNr    = 0;
	list_t* cont   = nullptr;
	data_t  start  = 0;
	data_t  toAdd  = 0;
	data_t  maxAdd = 0;
};
#endif


/** @brief Unified container speed test - multi threaded
  *
  * The test is simple: Add maxElements random elements,
  * clear the container and measure how long this takes.
**/
template<typename list_t>
int32_t testSpeedMT (sEnv &env)
{
	int32_t result = EXIT_SUCCESS;

#ifdef PWX_THREADS
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
	else if (isSameType (list_t, set_t))
		cout << "sets                (" << adjRight (2,0) << maxThreads << " threads)  : ";
	else {
		cout << "nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}
	cout.flush();

	list_t intCont; // The list

	// To make the testing of the sets easier, we use a counting loop, so no doublets
	// are tried to be pushed onto a set.
	int32_t lowest, highest, maxAdd, part, interval, rest;

	// Find a range where we can have maxElements evenly scattered
	do {
		lowest  = pwx::RNG.random(::std::numeric_limits<int32_t>::min() + 1, 0);
		highest = pwx::RNG.random(::std::numeric_limits<int32_t>::max() - 1, 0);
		maxAdd  = (highest - lowest + 1) / maxElements;
	} while ((maxAdd < 2) || (static_cast<uint32_t>((highest - lowest) / maxAdd) < maxElements));

	// The part is the number of elements each thread has to add:
	part     = maxElements / maxThreads;
	// The interval is the portion each thread has to add:
	interval = part * maxAdd;
	// The rest is the number of elements the final thread has to add
	// to achieve the total number of maxElements
	rest     = maxElements - (part * maxThreads);

	// Create the worker array:
	worker<list_t, data_t> workers[maxThreads];
	// Create the thread array:
    std::thread* addThreads[maxThreads];
    clock_t startTime = clock();

	// Starting in a loop ...
    for (size_t nr = 0; nr < maxThreads; ++nr) {
		workers[nr].init(nr + 1, &intCont, lowest, part + (nr == (maxThreads - 1) ? rest : 0), maxAdd);
		PWX_TRY_STD_FURTHER(addThreads[nr] = new std::thread(std::ref(workers[nr])),
							"Thread creation failed", "testSpeedMT could not call new operator on std::thread")
		lowest += interval;
    }
    // ... joining in a loop.
    size_t active = maxThreads;
    while (active) {
		for (size_t nr = 0; nr < maxThreads; ++nr) {
			if (	addThreads[nr]
				&&	addThreads[nr]->joinable()
				&&	!workers[nr].isRunning) {
				addThreads[nr]->join();
				delete addThreads[nr];
				addThreads[nr] = nullptr;
				--active;
			}
		}
    }

	// Now clear the container up again:
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
#endif // PWX_THREADS

	return result;
}


#endif // PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED

