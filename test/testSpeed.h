#pragma once
#ifndef PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED
#define PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED 1

#include "test_lib.h" // This is here for IDE Parsers to find the other stuff
#include <pwx/types/CLockable.h>
#include <pwx/types/CException.h>
#include <pwx/general/macros.h>

/// @brief struct doing synchronized start/stop for additions of items into containers
/// IMPORTANT: Single threaded calls _MUST_ set autostart to true on creation !
template<typename list_t>
struct thAdder : public pwx::CLockable
{
	bool    isRunning = false;
	list_t* cont      = nullptr;

	explicit thAdder(bool autostart) : isRunning(autostart) { }
	thAdder():thAdder(false) { }

	/// @brief the working functions, called manually or by std::thread
	void operator()(list_t* cont_, data_t start, data_t toAdd, data_t maxAdd)
	{
		PWX_LOCK_NOEXCEPT(this)
		cont = cont_;
#ifdef PWX_THREADS
		milliseconds sleepTime( 1 );
		while (!isRunning) {
			PWX_UNLOCK_NOEXCEPT(this)
			std::this_thread::sleep_for( sleepTime );
			std::this_thread::yield();
			PWX_LOCK_NOEXCEPT(this)
		}
#endif
		if (cont) {
			for (data_t nr = 0; nr < toAdd; ++nr) {
				PWX_TRY_PWX_FURTHER(cont->push(new data_t(start)))
				start += pwx::RNG.random(1, maxAdd);
			}
		}

		isRunning = false;
		PWX_UNLOCK_NOEXCEPT(this)
	}
};

/// @brief struct doing synchronized start/stop for clearing containers
/// IMPORTANT: Single threaded calls _MUST_ set autostart on creation !
template<typename list_t>
struct thClearer : public pwx::CLockable
{
	bool    isRunning = false;
	list_t* cont      = nullptr;

	explicit thClearer(bool autostart) : isRunning(autostart) { }
	thClearer() { thClearer(false); }

	/// @brief the working functions, called manually or by std::thread
	void operator()(list_t* cont_)
	{
		PWX_LOCK(this)
		cont = cont_;
#ifdef PWX_THREADS
		milliseconds sleepTime( 1 );
		while (!isRunning) {
			PWX_UNLOCK(this)
			std::this_thread::sleep_for( sleepTime );
			std::this_thread::yield();
			PWX_LOCK(this)
		}
#endif
		if (cont) cont->clear();

		isRunning = false;
		PWX_UNLOCK(this)
	}
};


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
	else if (isSameType (list_t, set_t)) {
		localMaxElem /= 10;
		cout << " Sets " << adjRight(7,0) << localMaxElem << " elements ( 1 thread ) : ";
	} else {
		cout << " Nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}
	cout.flush();

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
	thAdder<list_t> adder(true);
	hrTime_t startTimeAdd = hrClock::now();
    adder(&intCont, lowest, localMaxElem, maxAdd);

	// Now clear them up again:
	uint32_t contSize = intCont.size();
	hrTime_t endTimeAdd = hrClock::now();
	intCont.clear();

	// Bring out the needed time in ms:
	hrTime_t endTimeClr = hrClock::now();
	auto elapsedAdd = duration_cast<milliseconds>(endTimeAdd - startTimeAdd).count();
	auto elapsedClr = duration_cast<milliseconds>(endTimeClr - endTimeAdd  ).count();
	cout << adjRight(5,0) << elapsedAdd << " ms /";
	cout << adjRight(5,0) << elapsedClr << " ms" << endl;

	// Do we have had enough elements?
	if (localMaxElem != contSize) {
		cout << "    FAIL! Only " << contSize << "/" << localMaxElem << " elements inserted!" << endl;
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
  *
  * Note: If this is set_t, only 10% of maxElements are used.
**/
template<typename list_t>
int32_t testSpeedMT (sEnv &env)
{
	int32_t  result       = EXIT_SUCCESS;

	uint32_t localMaxElem = maxElements;

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
	else if (isSameType (list_t, set_t)) {
		localMaxElem /= 10;
		cout << " Sets " << adjRight(7,0) << localMaxElem << " elements (" << adjRight (2,0) << maxThreads << " threads) : ";
	} else {
		cout << " Nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}
	cout.flush();

#ifdef PWX_THREADS
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
    for (size_t nr = 0; nr < maxThreads; ++nr) {
		PWX_LOCK_NOEXCEPT( (&adders[nr]) )
		adders[nr].isRunning = true;
		PWX_UNLOCK_NOEXCEPT( (&adders[nr]) )
    }

    // ... joining in a loop.
    bool isFinished = false;
    while (!isFinished) {
		isFinished = true;
		for (size_t nr = 0; isFinished && (nr < maxThreads); ++nr) {
			PWX_LOCK_NOEXCEPT( (&adders[nr]) )
			if (adders[nr].isRunning)
				isFinished = false;
			PWX_UNLOCK_NOEXCEPT( (&adders[nr]) )
		}
		if (isFinished) {
			for (size_t nr = 0; nr < maxThreads; ++nr) {
				threads[nr]->join();
				delete threads[nr];
				threads[nr] = nullptr;
			}
		} else
			std::this_thread::yield();
    }

	uint32_t contSize = intCont.size();
	hrTime_t endTimeAdd = hrClock::now();

	// Now clear the container up again:
    for (size_t nr = 0; nr < maxThreads; ++nr)
		PWX_TRY_STD_FURTHER(threads[nr] = new std::thread(std::ref(clearers[nr]), &intCont),
							"Thread creation failed", "testSpeedMT could not call new operator on std::thread")

	// Starting in a loop
	hrTime_t startTimeClr = hrClock::now();
    for (size_t nr = 0; nr < maxThreads; ++nr) {
		PWX_LOCK_NOEXCEPT( (&clearers[nr]) )
		clearers[nr].isRunning = true;
		PWX_UNLOCK_NOEXCEPT( (&clearers[nr]) )
    }

    // ... joining in a loop.
    isFinished = false;
    while (!isFinished) {
		isFinished = true;
		for (size_t nr = 0; isFinished && (nr < maxThreads); ++nr) {
			PWX_LOCK_NOEXCEPT( (&clearers[nr]) )
			if (clearers[nr].isRunning)
				isFinished = false;
			PWX_UNLOCK_NOEXCEPT( (&clearers[nr]) )
		}
		if (isFinished) {
			for (size_t nr = 0; nr < maxThreads; ++nr) {
				threads[nr]->join();
				delete threads[nr];
				threads[nr] = nullptr;
			}
		} else
			std::this_thread::yield();
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
	} else
		++env.testSuccess;
#else
	cout << adjRight(5,0) << "n/a" << "    /";
	cout << adjRight(5,0) << "n/a" << endl;
	++env.testSuccess;
#endif // PWX_THREADS

	return result;
}


#endif // PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED

