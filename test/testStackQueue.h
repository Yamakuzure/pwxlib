#pragma once
#ifndef PWX_LIBPWX_TEST_TESTSTACKQUEUE_H_INCLUDED
#define PWX_LIBPWX_TEST_TESTSTACKQUEUE_H_INCLUDED 1

#include "main.h" // This is here for IDE Parsers to find the other stuff

/// Typedefs for the specific stack/queue:
typedef pwx::TStack<data_t> stack_t;
typedef pwx::TQueue<data_t> queue_t;

/** @brief test a stack or queue type template
  *
  ****************************************************************************
  ** === Generic template function to test stack/queue type containers ===  **
  ** A) Create a container and add 5 integers                               **
  ** B) List elements, must be 5, 4, 3, 2, 1                                **
  ** C) Pop three integers, must be 1, 2, 3 (Queue) or 5, 4, 3 (Stack)      **
  ** D) Shift two integers, must be 5, 4 (Queue) or 1, 2 (Stack)            **
  ** E) Build two containers (1, 3, 5) and (2, 4), test += (4, 2, 1, 3, 5)  **
  ** F) Substract first from the union (4, 2)                               **
  ****************************************************************************
**/
template<typename list_t>
int32_t testStackQueue(sEnv &env)
{
	int32_t result = EXIT_SUCCESS;

	cout << "Test ";
	if (isSameType(list_t, stack_t))
		cout << "stacks" << endl;
	else if (isSameType(list_t, queue_t))
		cout << "queues" << endl;
	else {
		cout << " : nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}
	cout << "----------------------------" << endl;

	list_t intCont(do_not_destroy); // The list
	int32_t numbers[5] = { 1, 2, 3, 4, 5 }; // The integers to store

	/***************************************************************************
	** A) Create a container and add 5 integers                               **
	***************************************************************************/
	cout << adjRight(4,0) << ++env.testCount << " A) Add five integers : ";

	// Add the first via unshift (doesn't matter with the order)
	uint32_t intCount = intCont.unshift(&numbers[0]);

	// Add the remaining using push
	for (int i = 1; i < 5; ++i)
		intCount = intCont.push(&numbers[i]);

	cout << intCount << " elements added = ";
	if (5 != intCount) {
		cout << "FAIL" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else {
		cout << "Success" << endl;
		++env.testSuccess;
	}

	/***************************************************************************
	** B) List elements, must be 5, 4, 3, 2, 1                                **
	***************************************************************************/
	if (EXIT_SUCCESS == result) {
		int32_t check[5] = { **intCont[0], **intCont[1], **intCont[2], **intCont[3], **intCont[4] };
		cout << adjRight(4,0) << ++env.testCount << " B) Elements are (5, 4, 3, 2, 1) : ";
		cout << check[0] << ", " << check[1] << ", " << check[2] << ", " << check[3] << ", " << check[4];
		if ( (5 == check[0]) && (4 == check[1]) && (3 == check[2]) && (2 == check[3]) && (1 == check[4]) ) {
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
	if (EXIT_SUCCESS == result) {
		cout << adjRight(4,0) << ++env.testCount << " C) Pop three elements (";
		if (isSameType(list_t, queue_t))
			cout << "1, 2, 3";
		else
			cout << "5, 4, 3";
		cout << ") : ";
		auto elemA = intCont.pop();
		auto elemB = intCont.pop();
		auto elemC = intCont.pop();
		cout << **elemA << ", " << **elemB << ", " << **elemC << " - ";
		if ( (isSameType(list_t, queue_t)
			  && (1 == **elemA) && (2 == **elemB) && (3 == **elemC) )
			 || ( (5 == **elemA) && (4 == **elemB) && (3 == **elemC) ) ) {
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
	if (EXIT_SUCCESS == result) {
		cout << adjRight(4,0) << ++env.testCount << " D) Shift two elements (";
		if (isSameType(list_t, queue_t))
			cout << "5, 4";
		else
			cout << "1, 2";
		cout << ") : ";
		auto elemA = intCont.shift();
		auto elemB = intCont.shift();
		cout << **elemA << ", " << **elemB << " - ";
		if ( (isSameType(list_t, queue_t)
			  && (5 == **elemA) && (4 == **elemB) )
			 || ( (1 == **elemA) && (2 == **elemB) ) ) {
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

	/***************************************************************************
	** E) Build two containers (1, 3, 5) and (2, 4), test += (4, 2, 1, 3, 5)  **
	***************************************************************************/

	/***************************************************************************
	** F) Substract first from the union (4, 2)                               **
	***************************************************************************/

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


#endif // PWX_LIBPWX_TEST_TESTSTACKQUEUE_H_INCLUDED

