#pragma once
#ifndef PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED
#define PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED 1

#include "test_lib.h" // This is here for IDE Parsers to find the other stuff
#include <pwx/types/CLockable.h>
#include <pwx/types/CException.h>
#include <pwx/general/macros.h>

// Shortcut - I'm lazy. ;)
typedef std::atomic_bool aBool;


/// @brief struct doing synchronized start/stop for additions of items into containers
/// IMPORTANT: Single threaded calls _MUST_ set autostart to true on creation !
template<typename list_t>
struct thAdder
{
	volatile
	aBool   isRunning;
	list_t* cont  = nullptr;

	explicit thAdder(bool autostart) : isRunning(autostart) { }
	thAdder():thAdder(false) { }

	/// @brief the working functions, called manually or by std::thread
	void operator()(list_t* cont_, data_t start, data_t toAdd, data_t maxAdd)
	{
		cont = cont_;
		while (!this->isRunning.load(std::memory_order_acquire)) { }

		if (cont) {
			for (data_t nr = 0; nr < toAdd; ++nr) {
				PWX_TRY_PWX_FURTHER(cont->push(new data_t(start)))
				start += pwx::RNG.random(1, maxAdd);
			}
		}

		DEBUG_LOCK_STATE("clear_locks", thAdder, cont)
		uint32_t remaining = 0;
		if ( ( remaining = cont->lock_count() ) ) {
			cerr << "ERROR: " << remaining << " locks upon thAdder exit!" << endl;
			cont->clear_locks();
		}

		// Set thread to not running in a loop to be absolutely sure
		// this operator does not exit until isRunning is false.
		while (this->isRunning.load(std::memory_order_acquire))
			this->isRunning.store(false, std::memory_order_release);
	}
};

/// @brief struct doing synchronized start/stop for clearing containers
/// IMPORTANT: Single threaded calls _MUST_ set autostart on creation !
template<typename list_t>
struct thClearer
{
	volatile
	aBool   isRunning;
	list_t* cont = nullptr;

	explicit thClearer(bool autostart) : isRunning(autostart) { }
	thClearer() { thClearer(false); }

	/// @brief the working functions, called manually or by std::thread
	void operator()(list_t* cont_)
	{
		cont = cont_;
		while (!this->isRunning.load(std::memory_order_acquire)) { }
		if (cont) cont->clear();

		DEBUG_LOCK_STATE("clear_locks", thClearer, cont)
		uint32_t remaining = 0;
		if ( ( remaining = cont->lock_count() ) ) {
			cerr << "ERROR: " << remaining << " locks upon thClearer exit!" << endl;
			cont->clear_locks();
		}

		// Set thread to not running in a loop to be absolutely sure
		// this operator does not exit until isRunning is false.
		while (this->isRunning.load(std::memory_order_acquire))
			this->isRunning.store(false, std::memory_order_release);
	}
};


/** @brief Unified container speed test - single threaded
  *
  * The test is simple: Add maxElements random elements,
  * clear the container and measure how long this takes.
**/
template<typename list_t>
int32_t testSpeedST (sEnv &env)
{
	int32_t  result       = EXIT_SUCCESS;
	uint32_t localMaxElem = env.doSpeed ? maxElements : maxThreads * 100;

	cout << adjRight (4, 0) << ++env.testCount;
	if (isSameType (list_t, single_list_t))
		cout << " Singly linked lists   ( 1 thread ) : ";
	else if (isSameType (list_t, double_list_t))
		cout << " Doubly linked lists   ( 1 thread ) : ";
	else if (isSameType (list_t, single_ring_t))
		cout << " Singly linked rings   ( 1 thread ) : ";
	else if (isSameType (list_t, double_ring_t))
		cout << " Doubly linked rings   ( 1 thread ) : ";
	else if (isSameType (list_t, stack_t))
		cout << " Stacks                ( 1 thread ) : ";
	else if (isSameType (list_t, queue_t))
		cout << " Queues                ( 1 thread ) : ";
	else if (isSameType (list_t, set_t))
		cout << " Sets                  ( 1 thread ) : ";
	else {
		cout << " Nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}
	cout.flush();

	list_t intCont; // The list
	intCont.disable_thread_safety(); // This is strictly single threaded.

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
	thAdder<list_t> adder(true);
	hrTime_t startTimeAdd = hrClock::now();
    adder(&intCont, lowest, localMaxElem, maxAdd);

	// Stop time, safe container size and do a consistency check
	hrTime_t endTimeAdd = hrClock::now();
	uint32_t contSize = intCont.size();

	bool isNextOK  = true;
	bool isOrderOK = true;
	uint32_t currNr = 1;
	auto*  curr   = intCont[0]; // Is head and number 0
	auto*  next   = curr;

	while (isNextOK && isOrderOK && (currNr < contSize) ) {
		next = intCont[currNr];
		if (next != curr->getNext())
			isNextOK = false;
		else if (isSameType(list_t, set_t) && (**curr >= **next))
			isOrderOK = false;
		else {
			++currNr;
			curr = next;
		}
	} // End of consistency checking


	// Now clear them up again:
	hrTime_t startTimeClr = hrClock::now();
	intCont.clear();

	// Bring out the needed time in ms:
	hrTime_t endTimeClr = hrClock::now();
	auto elapsedAdd = duration_cast<milliseconds>(endTimeAdd - startTimeAdd).count();
	auto elapsedClr = duration_cast<milliseconds>(endTimeClr - startTimeClr  ).count();
	cout << adjRight(5,0) << elapsedAdd << " ms /";
	cout << adjRight(5,0) << elapsedClr << " ms" << endl;

	// Do we have had enough elements?
	if (localMaxElem != contSize) {
		cout << "    FAIL! Only " << contSize << "/" << localMaxElem << " elements inserted!" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else if (!isNextOK) {
		cout << "    FAIL! idx " << (currNr - 1) << " has a wrong next neighbor!" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else if (!isOrderOK) {
		cout << "    FAIL! TSet ordering broken at idx " << (currNr - 1);
		cout << ": " << **curr << " >= " << **next << "!" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else
		++env.testSuccess;

	return result;
}


/** @brief Unified container speed test - multi threaded
  *
  * The test is simple: Add maxElements random elements,
  * clear the container and measure how long this takes.
**/
template<typename list_t>
int32_t testSpeedMT (sEnv &env)
{
	int32_t  result       = EXIT_SUCCESS;
	uint32_t localMaxElem = env.doSpeed ? maxElements : maxThreads * 100;

	cout << adjRight (4, 0) << ++env.testCount;
	if (isSameType (list_t, single_list_t))
		cout << " Singly linked lists   (" << adjRight (2,0) << maxThreads << " threads) : ";
	else if (isSameType (list_t, double_list_t))
		cout << " Doubly linked lists   (" << adjRight (2,0) << maxThreads << " threads) : ";
	else if (isSameType (list_t, single_ring_t))
		cout << " Singly linked rings   (" << adjRight (2,0) << maxThreads << " threads) : ";
	else if (isSameType (list_t, double_ring_t))
		cout << " Doubly linked rings   (" << adjRight (2,0) << maxThreads << " threads) : ";
	else if (isSameType (list_t, stack_t))
		cout << " Stacks                (" << adjRight (2,0) << maxThreads << " threads) : ";
	else if (isSameType (list_t, queue_t))
		cout << " Queues                (" << adjRight (2,0) << maxThreads << " threads) : ";
	else if (isSameType (list_t, set_t))
		cout << " Sets                  (" << adjRight (2,0) << maxThreads << " threads) : ";
	else {
		cout << " Nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}
	cout.flush();

	// Now the threaded part:
	list_t intCont; // The list
	intCont.enable_thread_safety(); // Although superfluous (enabled by default), test the call.

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

	// Create the thread and the worker arrays:
    std::thread*      threads [maxThreads];
    thAdder<list_t>   adders  [maxThreads];
    thClearer<list_t> clearers[maxThreads];

	// Creation in a loop ...
    for (size_t nr = 0; nr < maxThreads; ++nr) {
		PWX_TRY_STD_FURTHER(threads[nr] = new std::thread(std::ref(adders[nr]), &intCont, lowest,
														part + (nr == (maxThreads - 1) ? rest : 0),
														maxAdd),
							"Thread creation failed", "testSpeedMT could not call new operator on std::thread")
		lowest += interval;
    }

	// Starting in a loop
	hrTime_t startTimeAdd = hrClock::now();
    for (size_t nr = 0; nr < maxThreads; ++nr)
		adders[nr].isRunning.store(true);

    // ... joining in a loop.
    bool isFinished = false;
    while (!isFinished) {
		isFinished = true;
		for (size_t nr = 0; isFinished && (nr < maxThreads); ++nr) {
			if (adders[nr].isRunning.load())
				isFinished = false;
		}
		if (isFinished) {
			for (size_t nr = 0; nr < maxThreads; ++nr) {
				threads[nr]->join();
				delete threads[nr];
				threads[nr] = nullptr;
			}
		}
    }

	// Stop time, safe container size and do a consistency check
	hrTime_t endTimeAdd = hrClock::now();
	uint32_t contSize = intCont.size();

	bool isNextOK  = true;
	bool isOrderOK = true;
	uint32_t currNr = 1;
	auto*  curr   = intCont[0]; // Is head and number 0
	auto*  next   = curr;

	while (isNextOK && isOrderOK && (currNr < contSize) ) {
		next = intCont[currNr];
		if (next != curr->getNext())
			isNextOK = false;
		else if (isSameType(list_t, set_t) && (**curr >= **next))
			isOrderOK = false;
		else {
			++currNr;
			curr = next;
		}
	} // End of consistency checking

	// Now clear the container up again:
    for (size_t nr = 0; nr < maxThreads; ++nr)
		PWX_TRY_STD_FURTHER(threads[nr] = new std::thread(std::ref(clearers[nr]), &intCont),
							"Thread creation failed", "testSpeedMT could not call new operator on std::thread")

	// Starting in a loop
	hrTime_t startTimeClr = hrClock::now();
    for (size_t nr = 0; nr < maxThreads; ++nr)
		clearers[nr].isRunning.store(true);

    // ... joining in a loop.
    isFinished = false;
    while (!isFinished) {
		isFinished = true;
		for (size_t nr = 0; isFinished && (nr < maxThreads); ++nr) {
			if (clearers[nr].isRunning.load())
				isFinished = false;
		}
		if (isFinished) {
			for (size_t nr = 0; nr < maxThreads; ++nr) {
				threads[nr]->join();
				delete threads[nr];
				threads[nr] = nullptr;
			}
		}
    }

	// Bring out the needed time in ms:
	hrTime_t endTimeClr = hrClock::now();
	auto elapsedAdd = duration_cast<milliseconds>(endTimeAdd - startTimeAdd).count();
	auto elapsedClr = duration_cast<milliseconds>(endTimeClr - startTimeClr).count();
	cout << adjRight(5,0) << elapsedAdd << " ms /";
	cout << adjRight(5,0) << elapsedClr << " ms" << endl;

	// Do we have had enough elements?
	if (localMaxElem != contSize) {
		cout << "    FAIL! Only " << contSize << "/" << localMaxElem << " elements inserted!" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else if (!isNextOK) {
		cout << "    FAIL! idx " << (currNr - 1) << " has a wrong next neighbor!" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else if (!isOrderOK) {
		cout << "    FAIL! TSet ordering broken at idx " << (currNr - 1);
		cout << ": " << **curr << " >= " << **next << "!" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else
		++env.testSuccess;

	return result;
}


#endif // PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED

