#pragma once
#ifndef PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED
#define PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED 1

/** @file testSpeed.h
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include <RNG>

#include "test_lib.h" // This is here for IDE Parsers to find the other stuff
#include "testThreads.h"

/// @brief simple template to print the test description
template<typename cont_t>
int32_t printDescription( sEnv& env, uint32_t threadCount, bool sorted ) {
	cout << adjRight ( 4, 0 ) << ++env.testCount;
	if ( isSameType ( cont_t, single_list_t ) )
		cout << " Singly linked lists   ";
	else if ( isSameType ( cont_t, double_list_t ) )
		cout << " Doubly linked lists   ";
	else if ( isSameType ( cont_t, single_ring_t ) )
		cout << " Singly linked rings   ";
	else if ( isSameType ( cont_t, double_ring_t ) )
		cout << " Doubly linked rings   ";
	else if ( isSameType ( cont_t, pstack_t ) )
		cout << " Stacks                ";
	else if ( isSameType ( cont_t, pqueue_t ) )
		cout << " Queues                ";
	else if ( isSameType ( cont_t, set_t ) )
		cout << " Sets                  ";
	else if ( isSameType ( cont_t, chash_t ) )
		cout << " Chained Hash Tables   ";
	else if ( isSameType ( cont_t, ohash_t ) )
		cout << " Open Hash Tables      ";
	else {
		cout << " Nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}

	cout << "(" << adjRight( 2, 0 ) << threadCount << " thread";
	if ( threadCount > 1 )
		cout << "s; ";
	else
		cout << " ; ";

	if ( sorted )
		cout << "sorted) ";
	else
		cout << "pushed) ";

	cout.flush();

	return EXIT_SUCCESS;
}


/** @brief Unified list container speed test
  *
  * The test is simple: Add maxElements random elements,
  * clear the container and measure how long this takes.
  *
  * @param[in,out] env Environment
  * @param[in,out] testCont the container to work with
  * @param[in] threadCount Number of threads to start, 1 for single threaded work with disabled thread safety
  * @param[in,out] values pointer to the values array to add. If *values is nullptr, the array will be created
  * @param[in,out] retrieves pointer to the array holding the values to search. If *retrieves is nullptr,
  * the array will be created
  * @param[in] sorted tell the function that thAdd_t uses insert_sorted() instead of push()
  * @return EXIT_SUCCESS or EXIT_FAILURE.
**/
template <
        typename cont_t,  //!< Type of the container to test
        typename key_t,   //!< Type of the key (if it is a hash) - also used for loop counting
        typename value_t, //!< Type of the values to store in the container
        typename thAdd_t, //!< Type of the thread class that adds elements
        typename thSrc_t, //!< Type of the thread class that searches for the retrieve values
        typename thClr_t  //!< Type of the thread class that clears the container
        > int32_t testSpeed ( sEnv& env, cont_t& testCont, uint32_t threadCount, value_t** values, value_t** retrieves, bool sorted ) {
	// Needed anyway:
	static const value_t lo = std::numeric_limits<value_t>::lowest() + static_cast<value_t>( 1 );
	static const value_t hi = std::numeric_limits<value_t>::max()    - static_cast<value_t>( 1 );
	uint32_t localMaxElem = maxElements;
	uint32_t localMaxRet  = maxElements / 1000;
	int64_t  maxNeededMS  = 0;

	// Hashes search for more elements:
	if ( isSameType ( cont_t, chash_t )
	                || isSameType ( cont_t, ohash_t )
	                || isSameType ( cont_t, set_t ) ) {
		localMaxElem = maxHashVals;
		localMaxRet = maxHashVals / 10;
	}

	/* --------------------------------------------------------------------
	 * --- Pre-Step: Create values/retrieves arrays if not done already ---
	 * --------------------------------------------------------------------
	 */
	// --- values ---
	if ( nullptr == *values ) {
		PWX_TRY_STD_FURTHER( *values = new value_t[localMaxElem],
		                     "alloc_failed",
		                     "Allocation of the values array failed!" );
		DEBUG_LOG( "testSpeed", "Generating %u values to fill containers with", localMaxElem );
		for ( uint32_t i = 0; i < localMaxElem; ++i )
			( *values )[i] = pwx::RNG.random( lo, hi );

	}
	// --- retrieves ---
	if ( nullptr == *retrieves ) {
		PWX_TRY_STD_FURTHER( *retrieves = new value_t[localMaxRet],
		                     "alloc_failed",
		                     "Allocation of the values array failed!" );
		DEBUG_LOG( "testSpeed", "Generating %u values to search in containers for", localMaxRet );
		for ( uint32_t i = 0; i < localMaxRet; ++i )
			( *retrieves )[i] = ( *values )[pwx::RNG.random( static_cast<uint32_t>( 0 ), localMaxElem - 1 )];
	}


	/* -----------------------------
	 * --- Get a description out ---
	 * -----------------------------
	 */
	int32_t result = printDescription<cont_t>( env, threadCount, sorted );
	if ( EXIT_SUCCESS != result )
		return result; // printDescription() doesn't like cont_t

	if ( threadCount == 1 )
		testCont.disable_thread_safety(); // This is strictly single threaded.
	else
		testCont.enable_thread_safety(); // This is a multi threaded approach.

	key_t part = localMaxElem, rest = 0;
	if ( threadCount > 1 ) {
		// The part is the number of elements each thread has to add:
		part     = localMaxElem / threadCount;
		// The rest is the number of elements the final thread has to add
		// to achieve the total number of localMaxElem
		rest     = localMaxElem - ( part * threadCount );
	}

	// Create the thread and the worker arrays:
	std::thread** threads;
	thAdd_t*      adders;
	thClr_t*      clearers;
	thSrc_t*      searchers;

	// Create arrays:
	PWX_TRY_STD_FURTHER( threads = new std::thread*[threadCount], "Thread array creation failed",
	                     "operator new on std::thread* threw an exception" )
	PWX_TRY_STD_FURTHER( adders = new thAdd_t[threadCount], "Adder thread array creation failed",
	                     "operator new thAdd_t threw an exception" )
	PWX_TRY_STD_FURTHER( clearers = new thClr_t[threadCount], "Clearer thread array creation failed",
	                     "operator new on thClr_t threw an exception" )
	PWX_TRY_STD_FURTHER( searchers = new thSrc_t[threadCount], "Searcher thread array creation failed",
	                     "operator new on thSrc_t threw an exception" )

	/* --------------------------------------------------------------------
	 * --- 1 A) Create the adder threads to start filling the container ---
	 * --------------------------------------------------------------------
	 */
	for ( size_t nr = 0; nr < threadCount; ++nr ) {
		key_t thPart = part + ( nr == ( threadCount - 1 ) ? rest : 0 );
		PWX_TRY_STD_FURTHER( threads[nr] = new std::thread( std::ref( adders[nr] ), &testCont,
		                *values, nr * part, thPart ),
		                     "Thread creation failed", "testSpeed could not call new operator on std::thread" )
	}

	/* ---------------------------------------------------------
	 * --- 1 B) Start the adder threads now they are created ---
	 * ---------------------------------------------------------
	 */
	for ( size_t nr = 0; nr < threadCount; ++nr )
		adders[nr].isRunning.store( true, std::memory_order_release );

	/* ----------------------------------------
	 * --- 1 C) Join the adder threads back ---
	 * ----------------------------------------
	 */
	bool isFinished    = false;
	while ( !isFinished ) {
		isFinished = true;
		for ( size_t nr = 0; isFinished && ( nr < threadCount ); ++nr ) {
			if ( adders[nr].isRunning.load( std::memory_order_acquire ) )
				isFinished = false;
		}
		if ( isFinished ) {
			for ( size_t nr = 0; nr < threadCount; ++nr ) {
				threads[nr]->join();
				if ( adders[nr].timeMS > maxNeededMS )
					maxNeededMS = adders[nr].timeMS;
				delete threads[nr];
				threads[nr] = nullptr;
			}
		}
	} // End of loop-joining threads

	/* --------------------------------------------------------------------
	 * --- 2) Stop time, safe container size and do a consistency check ---
	 * --------------------------------------------------------------------
	 */
	uint32_t contSize = testCont.size();

	// Do the calculations
	bool     isNextOK  = true;
	uint32_t currNr    = 1;
	auto*    curr      = testCont[0]; // Is head and number 0
	auto*    next      = curr;

	// The consistency checks for the elements are only relevant for
	// list type containers. Hash Tables can not be checked like that.
	if ( !isSameType( cont_t, chash_t ) && !isSameType( cont_t, ohash_t ) ) {
		while ( isNextOK && ( currNr < contSize ) ) {
			next = testCont[currNr];
			if ( !curr || ( next != curr->getNext() ) )
				isNextOK = false;
			else {
				++currNr;
				curr = next;
			}
		} // End of consistency checking
	}

	// Bring out the needed time in ms:
	cout << adjRight( 6, 0 ) << maxNeededMS << " ms /"; cout.flush();

	/* -----------------------------------------------------------
	 * --- 3 A) Create the searcher threads to test retrieving ---
	 * -----------------------------------------------------------
	 */
	part = localMaxRet;
	rest = 0;
	if ( threadCount > 1 ) {
		part = localMaxRet / threadCount;
		rest = localMaxRet - ( part * threadCount );
	}
	for ( size_t nr = 0; nr < threadCount; ++nr ) {
		key_t thPart = part + ( nr == ( threadCount - 1 ) ? rest : 0 );
		PWX_TRY_STD_FURTHER( threads[nr] = new std::thread( std::ref( searchers[nr] ), &testCont,
		                *retrieves, nr * part, thPart ),
		                     "Thread creation failed", "testSpeed could not call new operator on std::thread" )
	}

	/* ------------------------------------------------------------
	 * --- 3 B) Start the searcher threads now they are created ---
	 * ------------------------------------------------------------
	 */
	for ( size_t nr = 0; nr < threadCount; ++nr )
		searchers[nr].isRunning.store( true, std::memory_order_release );

	/* -------------------------------------------
	 * --- 3 C) Join the searcher threads back ---
	 * -------------------------------------------
	 */
	isFinished  = false;
	maxNeededMS = 0;
	while ( !isFinished ) {
		isFinished = true;
		for ( size_t nr = 0; isFinished && ( nr < threadCount ); ++nr ) {
			if ( searchers[nr].isRunning.load( std::memory_order_acquire ) )
				isFinished = false;
		}
		if ( isFinished ) {
			for ( size_t nr = 0; nr < threadCount; ++nr ) {
				threads[nr]->join();
				if ( searchers[nr].timeMS > maxNeededMS )
					maxNeededMS = searchers[nr].timeMS;
				delete threads[nr];
				threads[nr] = nullptr;
			}
		}
	} // End of loop-joining threads

	// Bring out the needed time in ms:
	cout << adjRight( 6, 0 ) << maxNeededMS << " ms /"; cout.flush();

	/* ----------------------------------------------
	 * --- 4) Now clear the container up again    ---
	 * ---    Using clear() this is much simpler! ---
	 * ----------------------------------------------
	 */
	for ( size_t nr = 0; nr < threadCount; ++nr )
		PWX_TRY_STD_FURTHER( threads[nr] = new std::thread( std::ref( clearers[nr] ), &testCont ),
		                     "Thread creation failed", "testSpeedMT could not call new operator on std::thread" )

		// Starting in a loop
		for ( size_t nr = 0; nr < threadCount; ++nr )
			clearers[nr].isRunning.store( true );

	// ... joining in a loop.
	isFinished  = false;
	maxNeededMS = 0;
	while ( !isFinished ) {
		isFinished = true;
		for ( size_t nr = 0; isFinished && ( nr < threadCount ); ++nr ) {
			if ( clearers[nr].isRunning.load() )
				isFinished = false;
		}
		if ( isFinished ) {
			for ( size_t nr = 0; nr < threadCount; ++nr ) {
				threads[nr]->join();
				if ( clearers[nr].timeMS > maxNeededMS )
					maxNeededMS = clearers[nr].timeMS;
				delete threads[nr];
				threads[nr] = nullptr;
			}
		}
	}

	// Clear the dynamic arrays
	if ( threads )   delete [] threads;
	if ( adders )    delete [] adders;
	if ( clearers )  delete [] clearers;
	if ( searchers ) delete [] searchers;

	// Bring out the needed time in ms:
	cout << adjRight( 5, 0 ) << maxNeededMS << " ms" << endl;

	// Do we have had enough elements?
	// Note: Sets and Hashes might have less if values[] consists of doublets.
	if ( !isSameType( cont_t, set_t )
	                && !isSameType( cont_t, chash_t )
	                && !isSameType( cont_t, ohash_t )
	                && ( localMaxElem != contSize ) ) {
		cerr << "    FAIL! Only " << contSize << "/" << localMaxElem << " elements inserted!" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	}
	// Is the ordering ok? (chain equals index)
	else if ( !isNextOK ) {
		cerr << "    FAIL! idx " << ( currNr - 1 ) << " has a wrong next neighbor!" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	}
	// Is the container cleared?
	else if ( testCont.size() || testCont[0] || testCont[-1] ) {
		cerr << "    FAIL! The container has " << testCont.size() << " elements left\n";
		cerr << "    with root being " << testCont[0] << " and tail being " << testCont[-1];
		cerr << endl;
	} else
		++env.testSuccess;

	return result;
}


#endif // PWX_LIBPWX_TESTLIB_TESTSPEED_H_INCLUDED

