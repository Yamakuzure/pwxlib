#pragma once
#ifndef PWX_LIBPWX_TEST_TESTSET_H_INCLUDED
#define PWX_LIBPWX_TEST_TESTSET_H_INCLUDED 1

/** @file testSet.h
  * (c) 2007 - 2020 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include <PSet>

#include "test_lib.h" // This is here for IDE Parsers to find the other stuff

/// Typedefs for the specific set:
typedef PSet<keydata_t> set_t;

/** @brief test a specific set type template
  *
  ****************************************************************************
  ** === Generic template function to test set type containers ===          **
  ** A) Create a container and add 5 integers                               **
  ** B) List elements, must be 5, 4, 3, 2, 1                                **
  ** C) Pop three integers, must be 1, 2, 3                                 **
  ** D) Try to add another "2", must not be possible.                       **
  ** E) Shift two integers, must be 5, 4                                    **
  ** F) Build sets (1, 3, 5) and (2, 3, 4) - Build intersection, must be (3)**
  **    Build difference A-B, must be (1, 5)                                **
  **    Build union A+B, must be (1, 2, 3, 4, 5)                            **
  ** G) Build sets (1, 3) and (1, 2, 3) - test ==/!=/isSubsetOf             **
  ****************************************************************************
**/
template<typename list_t>
int32_t testSet ( sEnv& env ) {
	int32_t result = EXIT_SUCCESS;

	cout << "Test ";
	if ( isSameType ( list_t, set_t ) )
		cout << "sets" << endl;
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

	// Add the first via unshift (doesn't matter with the order)
	uint32_t intCount = intCont.unshift ( &numbers[0] );

	// Add the remaining using push
	for ( int i = 1; i < 5; ++i )
		intCount = intCont.push ( &numbers[i] );

	cout << intCount << " elements added : ";
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
	** C) Pop three integers, must be 1, 2, 3                                 **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " C) Pop three elements (1, 2, 3) : ";
		auto elemA = intCont.pop();
		auto elemB = intCont.pop();
		auto elemC = intCont.pop();
		cout << **elemA << ", " << **elemB << ", " << **elemC << " - ";
		if ( ( 1 == **elemA ) && ( 2 == **elemB ) && ( 3 == **elemC ) ) {
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
	} // end of sub test B

	/***************************************************************************
	** D) Try to add another "4", must not be possible.                       **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " D) unshift another \"4\" - ";
		PWX_TRY_PWX_FURTHER ( intCont.unshift ( &numbers[3] ) )
		cout << "new size (2) : " << intCont.size();
		if ( 2 == intCont.size() ) {
			cout << " - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
	}

	/***************************************************************************
	** E) Shift two integers, must be 5, 4                                    **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " E) Shift two elements (5, 4) : ";
		auto elemA = intCont.shift();
		auto elemB = intCont.shift();
		cout << **elemA << ", " << **elemB << " - ";
		if ( ( 5 == **elemA ) && ( 4 == **elemB ) ) {
			cout << "Success" << endl;
			++env.testSuccess;
		} else {
			cout << "FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
		delete elemA;
		delete elemB;
	} // end of sub test B

	list_t testContA( do_not_destroy ), testContB( do_not_destroy ), testContC( do_not_destroy );

	/***************************************************************************
	** F) Build sets (1, 3, 5) and (2, 3, 4) - Build intersection, must be (3)**
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout  << "     F) Test set operations : " << endl;

		// A: (1, 3, 5)
		testContA.push( &numbers[0] );
		testContA.push( &numbers[2] );
		testContA.push( &numbers[4] );
		cout << "         Container 1: ";
		cout << **testContA[0] << ", " << **testContA[1] << ", " << **testContA[2] << endl;

		// B: (2, 3, 4)
		testContB.push( &numbers[1] );
		testContB.push( &numbers[2] );
		testContB.push( &numbers[3] );
		cout << "         Container 2: ";
		cout << **testContB[0] << ", " << **testContB[1] << ", " << **testContB[2] << endl;

		// C: (3)
		// Note: Using the (costly) reference version tests the pointer version as well!
		testContC = set_intersection( testContA, testContB );
		cout << adjRight ( 4, 0 ) << ++env.testCount << "    Intersection (3)             : ";
		cout << **testContC[0];

		if ( ( 3 == testContA.size() ) && ( 3 == testContB.size() ) && ( 1 == testContC.size() ) && ( 3 == **testContC[0] ) ) {
			cout << "             - Success" << endl;
			++env.testSuccess;
		} else {
			cout << "             - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}

		/***************************************************************************
		**   Build difference A-B, must be (1, 5)                                **
		***************************************************************************/
		cout << adjRight ( 4, 0 ) << ++env.testCount << "    Difference   (1, 5)          : ";
		// Note: The (very costly) operator- uses the reference version of set_difference(),
		//       which uses the pointer version of set_difference(). Three tests in one.
		testContC = testContA - testContB;
		cout << **testContC[0] << ", " << **testContC[1];

		if ( ( 2 == testContC.size() ) && ( 1 == **testContC[0] ) && ( 5 == **testContC[1] ) ) {
			cout << "          - Success" << endl;
			++env.testSuccess;
		} else {
			cout << "          - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}

		/***************************************************************************
		**    Build union A+B, must be (1, 3, 5, 2, 4)                            **
		***************************************************************************/
		cout << adjRight ( 4, 0 ) << ++env.testCount << "    Union        (1, 3, 5, 2, 4) : ";
		// Note: The (very costly) operator+ uses the reference version of set_union(),
		//       which uses the pointer version of set_union(). Three tests in one.
		testContC = testContA + testContB;
		cout << **testContC[0] << ", " << **testContC[1] << ", ";
		cout << **testContC[2] << ", " << **testContC[3] << ", ";
		cout << **testContC[4];

		if (	( 5 == testContC.size() ) && ( 1 == **testContC[0] ) && ( 3 == **testContC[1] )
		                &&	( 5 == **testContC[2] )   && ( 2 == **testContC[3] ) && ( 4 == **testContC[4] ) ) {
			cout << " - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}

	} // End of test F

	/***************************************************************************
	** G) Build sets (1, 3) and (1, 2, 3) - test ==/!=/isSubsetOf             **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << "     G) Test comparisons : " << endl;

		// A: (1, 3)
		testContA.clear();
		testContA.push( &numbers[0] );
		testContA.push( &numbers[2] );
		cout << "         Container 1: ";
		cout << **testContA[0] << ", " << **testContA[1] << endl;

		// B: (1, 2, 3)
		testContB.clear();
		testContB.push( &numbers[0] );
		testContB.push( &numbers[1] );
		testContB.push( &numbers[2] );
		cout << "         Container 2: ";
		cout << **testContB[0] << ", " << **testContB[1] << ", " << **testContB[2] << endl;

		cout << adjRight ( 4, 0 ) << ++env.testCount << "    Container 1 == Container 2 ? (false)  : ";
		if ( !( testContA == testContB ) ) {
			cout << " false - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " true  - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}

		cout << adjRight ( 4, 0 ) << ++env.testCount << "    Container 1 != Container 2 ? (true)   : ";
		if ( testContA != testContB ) {
			cout << " true  - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " false - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}

		cout << adjRight ( 4, 0 ) << ++env.testCount << "    Container 1 is a subset of 2 ? (true) : ";
		if ( testContA.isSubsetOf( testContB ) ) {
			cout << " true  - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " false - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
	} // End of test I

	cout << endl;

	return result;
}

#endif // PWX_LIBPWX_TEST_TESTSET_H_INCLUDED
