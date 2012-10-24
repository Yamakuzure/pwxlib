#pragma once
#ifndef PWX_LIBPWX_TEST_TESTSET_H_INCLUDED
#define PWX_LIBPWX_TEST_TESTSET_H_INCLUDED 1

#include "main.h" // This is here for IDE Parsers to find the other stuff

/// Typedefs for the specific set:
typedef pwx::TSet<data_t> set_t;

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
  ** G) Build difference A-B, must be (1, 5)                                **
  ** H) Build union A+B, must be (1, 2, 3, 4, 5)                            **
  ****************************************************************************
**/
template<typename list_t>
int32_t testSet (sEnv& env)
{
	int32_t result = EXIT_SUCCESS;

	cout << "Test ";
	if (isSameType (list_t, set_t))
		cout << "sets" << endl;
	else {
		cout << " : nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}
	cout << "----------------------------" << endl;

	list_t intCont (do_not_destroy); // The list
	int32_t numbers[5] = { 1, 2, 3, 4, 5 }; // The integers to store

	/***************************************************************************
	** A) Create a container and add 5 integers                               **
	***************************************************************************/
	cout << adjRight (4, 0) << ++env.testCount << " A) Add five integers : ";

	// Add the first via unshift (doesn't matter with the order)
	uint32_t intCount = intCont.unshift (&numbers[0]);

	// Add the remaining using push
	for (int i = 1; i < 5; ++i)
		intCount = intCont.push (&numbers[i]);

	cout << intCount << " elements added : ";
	if (5 != intCount) {
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
	if (EXIT_SUCCESS == result) {
		int32_t check[5] = { **intCont[0], **intCont[1], **intCont[2], **intCont[3], **intCont[4] };
		cout << adjRight (4, 0) << ++env.testCount << " B) Elements are (1, 2, 3, 4, 5) : ";
		cout << check[0] << ", " << check[1] << ", " << check[2] << ", " << check[3] << ", " << check[4];
		if ( (1 == check[0]) && (2 == check[1]) && (3 == check[2]) && (4 == check[3]) && (5 == check[4])) {
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
	if (EXIT_SUCCESS == result) {
		cout << adjRight (4, 0) << ++env.testCount << " C) Pop three elements (1, 2, 3) : ";
		auto elemA = intCont.pop();
		auto elemB = intCont.pop();
		auto elemC = intCont.pop();
		cout << **elemA << ", " << **elemB << ", " << **elemC << " - ";
		if ( (1 == **elemA) && (2 == **elemB) && (3 == **elemC)) {
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
	if (EXIT_SUCCESS == result) {
		cout << adjRight (4, 0) << ++env.testCount << " D) unshift another \"4\" - ";
		PWX_TRY_PWX_FURTHER (intCont.unshift (&numbers[3]))
		cout << "new size (2) : " << intCont.size();
		if (2 == intCont.size()) {
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
	if (EXIT_SUCCESS == result) {
		cout << adjRight (4, 0) << ++env.testCount << " E) Shift two elements (5, 4) : ";
		auto elemA = intCont.shift();
		auto elemB = intCont.shift();
		cout << **elemA << ", " << **elemB << " - ";
		if ( (5 == **elemA) && (4 == **elemB)) {
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

	list_t testContA(do_not_destroy), testContB(do_not_destroy), testContC(do_not_destroy);

	/***************************************************************************
	** F) Build sets (1, 3, 5) and (2, 3, 4) - Build intersection, must be (3)**
	***************************************************************************/
	if (EXIT_SUCCESS == result) {
		cout << adjRight (4, 0) << ++env.testCount << " F) Build an intersection : " << endl;

		// A: (1, 3, 5)
		testContA.push(&numbers[0]);
		testContA.push(&numbers[2]);
		testContA.push(&numbers[4]);
		cout << "         Container 1: ";
		cout << **testContA[0] << ", " << **testContA[1] << ", " << **testContA[2] << endl;

		// B: (2, 3, 4)
		testContB.push(&numbers[1]);
		testContB.push(&numbers[2]);
		testContB.push(&numbers[3]);
		cout << "         Container 2: ";
		cout << **testContB[0] << ", " << **testContB[1] << ", " << **testContB[2] << endl;

		// C: (3)
		// Note: Using the (costly) reference version tests the pointer version as well!
		testContC = set_intersection(testContA, testContB);
		cout << "        Intersection: ";
		cout << **testContC[0];

		if ((3 == testContA.size()) && (3 == testContB.size()) && (1 == testContC.size()) && (3 == **testContC[0])) {
			cout << " - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}

	} // End of test F

	/***************************************************************************
	** G) Build difference A-B, must be (1, 5)                                **
	***************************************************************************/
	if (EXIT_SUCCESS == result) {
		cout << adjRight (4, 0) << ++env.testCount << " F) Build a difference : ";
		// Note: The (very costly) operator- uses the reference version of set_difference(),
		//       which uses the pointer version of set_difference(). Three tests in one.
		testContC = testContA - testContB;
		cout << **testContC[0] << ", " << **testContC[1];

		if ((2 == testContC.size()) && (1 == **testContC[0]) && (5 == **testContC[1])) {
			cout << " - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}

	} // End of test G

	/***************************************************************************
	** H) Build union A+B, must be (1, 2, 3, 4, 5)                            **
	***************************************************************************/
	if (EXIT_SUCCESS == result) {
		cout << adjRight (4, 0) << ++env.testCount << " F) Build a union : ";
		// Note: The (very costly) operator+ uses the reference version of set_union(),
		//       which uses the pointer version of set_union(). Three tests in one.
		testContC = testContA + testContB;
		cout << **testContC[0] << ", " << **testContC[1] << ", ";
		cout << **testContC[2] << ", " << **testContC[3] << ", ";
		cout << **testContC[4];

		if (	(5 == testContC.size()) && (1 == **testContC[0]) && (2 == **testContC[1])
			&&	(3 == **testContC[2])   && (4 == **testContC[3]) && (5 == **testContC[4])) {
			cout << " - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}

	} // End of test G



/// @todo : Put into unified speed test once RNG is available for random access
//  /** D) Speed test inserting, checking and removing MAXINTCOUNT integers **/
//  if (EXIT_SUCCESS == result)
//    {
//      clock_t startTime = 0;
//      clock_t endTime   = 0;
//
//      cout << adjRight(4,0) << ++env.testCount << " D) Speed test (" << MAXINTCOUNT << " elements) : "; cout.flush();
//
//      // for this test we need a container that does delete data:
//      list_t spdCont; // The list without the do-not-destroy-destroy()-method
//
//      cout << "push"; cout.flush();
//
//      startTime = clock();
//
//      for (int32_t i = 1; i < (MAXINTCOUNT + 1); ++i)
//        {
//          // Note: As main() does a catch-all, no try{} needed here.
//          int32_t* newInt = new int32_t(i);
//          spdCont.push(newInt);
//        }
//
//      if ((EXIT_SUCCESS == result) && (MAXINTCOUNT != static_cast<int32_t>(spdCont.size())))
//        {
//          cout << " FAILED: spdCont has " << spdCont.size() << " elements" << endl;
//          result = EXIT_FAILURE;
//        }
//
//      if (EXIT_SUCCESS == result)
//        {
//          cout << ", pop"; cout.flush();
//
//          for (int32_t i = 1; i < (MAXINTCOUNT + 1); ++i)
//            {
//              auto elem = spdCont.pop();
//              if ( (isSameType(list_t, stack_t) && ((MAXINTCOUNT + 1 - i) != **elem) )
//                || (i != **elem) )
//                {
//                  cout << "FAIL" << endl;
//                  i = MAXINTCOUNT + 1;
//                  result = EXIT_FAILURE;
//                }
//            }
//        }
//
//      if (EXIT_SUCCESS == result)
//        {
//          if (spdCont.size())
//            {
//              cout << " FAILED: " << spdCont.size() << " items remain" << endl;
//              result = EXIT_FAILURE;
//            }
//          else
//            {
//              endTime = clock();
//              double msNeeded = 1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC));
//              cout << adjRight(5,0) << msNeeded << " ms - Success" << endl;
//            }
//        }
//
//      if (EXIT_SUCCESS == result)
//        ++env.testSuccess;
//      else
//        ++env.testFail;
//
//    } // End of sub test E
	cout << endl;

	return result;
}

#endif // PWX_LIBPWX_TEST_TESTSET_H_INCLUDED
