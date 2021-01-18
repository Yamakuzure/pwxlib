#pragma once
#ifndef PWX_LIBPWX_TEST_TESTSTACKQUEUE_H_INCLUDED
#define PWX_LIBPWX_TEST_TESTSTACKQUEUE_H_INCLUDED 1

/** @file testStackQueue.h
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include <PStack>
#include <PQueue>

#include "test_lib.h" // This is here for IDE Parsers to find the other stuff

/// Typedefs for the specific stack/queue:
typedef PStack<keydata_t> pstack_t;
typedef PQueue<keydata_t> pqueue_t;

/** @brief test a stack or queue type template
  *
  ****************************************************************************
  ** === Generic template function to test stack/queue type containers ===  **
  ** A) Create a container and add 5 integers                               **
  ** B) List elements, must be 5, 4, 3, 2, 1                                **
  ** C) Pop three integers, must be 1, 2, 3 (Queue) or 5, 4, 3 (Stack)      **
  ** D) Shift two integers, must be 5, 4 (Queue) or 1, 2 (Stack)            **
  ** E) Build two containers (1, 3, 5), (2, 4) and 2 x operator+ on third   **
  ** F) Check third container values (2, 4, 1, 3, 5)                        **
  ** G) Substract first from the union (2, 4)                               **
  ****************************************************************************
**/
template<typename list_t>
int32_t testStackQueue ( sEnv& env ) {
	int32_t result = EXIT_SUCCESS;

	cout << "Test ";
	if ( isSameType ( list_t, pstack_t ) )
		cout << "stacks" << endl;
	else if ( isSameType ( list_t, pqueue_t ) )
		cout << "queues" << endl;
	else {
		cout << " : nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}
	cout << "----------------------------" << endl;

	list_t intCont ( do_not_destroy ); // The list
	intCont.disable_thread_safety(); // This is strictly single threaded.
	int32_t numbers[5] = { 1, 2, 3, 4, 5 }; // The integers to store

	/***************************************************************************
	** A) Create a container and add 5 integers                               **
	***************************************************************************/
	cout << adjRight ( 4, 0 ) << ++env.testCount << " A) Add five integers : ";

	// Add the first via push_back (doesn't matter with the order)
	uint32_t intCount = intCont.unshift ( &numbers[0] );

	// Add the remaining using push
	for ( int i = 1; i < 5; ++i )
		intCount = intCont.push ( &numbers[i] );

	cout << intCount << " elements added = ";
	if ( 5 != intCount ) {
		cout << "FAIL" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else {
		cout << "Success" << endl;
		++env.testSuccess;
	}

	/***************************************************************************
	** B) List elements, must be 1, 2, 3, 4, 5                                **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		int32_t check[5] = { ** intCont[0], ** intCont[1], ** intCont[2], ** intCont[3], ** intCont[4] };
		cout << adjRight ( 4, 0 ) << ++env.testCount << " B) Elements are (1, 2, 3, 4, 5) : ";
		cout << check[0] << ", " << check[1] << ", " << check[2] << ", " << check[3] << ", " << check[4];
		if ( ( 1 == check[0] ) && ( 2 == check[1] ) && ( 3 == check[2] ) && ( 4 == check[3] ) && ( 5 == check[4] ) ) {
			cout << " - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
	}

	/***************************************************************************
	** C) Pop three integers, must be 1, 2, 3 (Queue) or 5, 4, 3 (Stack)      **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " C) Pop three elements (";
		if ( isSameType ( list_t, pqueue_t ) )
			cout << "1, 2, 3";
		else
			cout << "5, 4, 3";
		cout << ") : ";
		auto elemA = intCont.pop();
		auto elemB = intCont.pop();
		auto elemC = intCont.pop();
		cout << **elemA << ", " << **elemB << ", " << **elemC << " - ";
		if ( ( isSameType ( list_t, pqueue_t )
		                && ( 1 == **elemA ) && ( 2 == **elemB ) && ( 3 == **elemC ) )
		                || ( ( 5 == **elemA ) && ( 4 == **elemB ) && ( 3 == **elemC ) ) ) {
			cout << "Success" << endl;
			++env.testSuccess;
		} else {
			cout << "FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
		delete elemA;
		delete elemB;
		delete elemC;
	} // end of sub test C

	/***************************************************************************
	** D) Shift two integers, must be 5, 4 (Queue) or 1, 2 (Stack)            **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " D) Shift two elements (";
		if ( isSameType ( list_t, pqueue_t ) )
			cout << "5, 4";
		else
			cout << "1, 2";
		cout << ") : ";
		auto elemA = intCont.shift();
		auto elemB = intCont.shift();
		cout << **elemA << ", " << **elemB << " - ";
		if ( ( isSameType ( list_t, pqueue_t )
		                && ( 5 == **elemA ) && ( 4 == **elemB ) )
		                || ( ( 1 == **elemA ) && ( 2 == **elemB ) ) ) {
			cout << "Success" << endl;
			++env.testSuccess;
		} else {
			cout << "FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
		delete elemA;
		delete elemB;
	} // end of sub test D

	list_t testContA( do_not_destroy ), testContB( do_not_destroy ), testContC( do_not_destroy );

	/***************************************************************************
	** E) Build two containers (1, 3, 5), (2, 4) and 2 x operator+ on third   **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " E) Build three containers :" << endl;

		// First container with 1, 3, 5 - pushed in reversed to result in the correct order
		testContA.push( &numbers[4] );
		testContA.push( &numbers[2] );
		testContA.push( &numbers[0] );
		cout << "         Container 1: ";
		cout << **testContA[0] << ", " << **testContA[1] << ", " << **testContA[2] << endl;

		// Second container, same as above
		testContB.push( &numbers[3] );
		testContB.push( &numbers[1] );
		cout << "         Container 2: ";
		cout << **testContB[0] << ", " << **testContB[1] << endl;

		// The third will use operator +=
		cout << "         Container 3 += Container 1, Container 2 (size 5) : ";
		testContC += testContA;
		testContC += testContB;
		cout << testContC.size();
		if ( testContC.size() == 5 ) {
			cout << " - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
	}// End of test E

	/***************************************************************************
	** F) Check third container values (5, 3, 1, 4, 2)                        **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " F) Check third container" << endl;
		cout << "         (5, 3, 1, 4, 2) - ";
		int32_t check[5] = { ** testContC[0], ** testContC[1], ** testContC[2], ** testContC[3], ** testContC[4] };
		cout << check[0] << ", " << check[1] << ", " << check[2] << ", " << check[3] << ", " << check[4];
		if ( ( 5 == check[0] ) && ( 3 == check[1] ) && ( 1 == check[2] ) && ( 4 == check[3] ) && ( 2 == check[4] ) ) {
			cout << " - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
	} // End of test F


	/***************************************************************************
	** G) Substract first from the union (4, 2)                               **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " G) Substract first container (4, 2) - ";
		testContC -= testContA;
		int32_t check[2] = { ** testContC[0], ** testContC[1] };
		cout << check[0] << ", " << check[1];
		if ( ( 4 == check[0] ) && ( 2 == check[1] ) ) {
			cout << " - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
	} // End of test G

	cout << endl;

	return result;
}

#endif // PWX_LIBPWX_TEST_TESTSTACKQUEUE_H_INCLUDED

