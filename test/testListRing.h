#pragma once
#ifndef PWX_LIBPWX_TESTLIB_TESTLISTRING_H_INCLUDED
#define PWX_LIBPWX_TESTLIB_TESTLISTRING_H_INCLUDED 1

#include "test_lib.h" // This is here for IDE Parsers to find the other stuff

/// Typedefs for the specific lists:
typedef pwx::TSingleList<data_t> single_list_t;
typedef pwx::TSingleRing<data_t> single_ring_t;
typedef pwx::TDoubleList<data_t> double_list_t;
typedef pwx::TDoubleRing<data_t> double_ring_t;


/** @brief test a specific list based template
  *
  *************************************************************************
  ** === Generic template function to test list based containers ===     **
  ** A) Create a container and add 5 integers                            **
  ** B) Print out elements 1, 3 and 5 which should equal their numbers   **
  ** C) Remove elements 1, 3 and 5, resulting size should be 2           **
  ** D) Print out all remaining elements, which should be 2 and 4        **
  ** E) Speed test inserting, checking and removing MAXINTCOUNT integers **
  *************************************************************************
**/
template<typename list_t>
int32_t testListRing (sEnv &env)
{
	int32_t result = EXIT_SUCCESS;

	cout << "Test ";
	if (isSameType (list_t, single_list_t))
		cout << "singly linked lists" << endl;
	else if (isSameType (list_t, double_list_t))
		cout << "doubly linked lists" << endl;
	else if (isSameType (list_t, single_ring_t))
		cout << "singly linked rings" << endl;
	else if (isSameType (list_t, double_ring_t))
		cout << "doubly linked rings" << endl;
	else {
		cout << " : nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}
	cout << "----------------------------" << endl;

	list_t intCont (do_not_destroy); // The list
	intCont.disable_thread_safety(); // This is strictly single threaded.

	/** A) Create a container and add 5 integers **/
	cout << adjRight (4, 0) << ++env.testCount << " A) Add five integers : ";

	int32_t numbers[5] = { 1, 2, 3, 4, 5 }; // The integers to store
	// Add the first using ins_next:
	intCont.insNext (nullptr, &numbers[0]);
	// Add the next three using ins_next_elem
	for (int i = 1; i < 4; ++i)
		intCont.insNextElem (intCont[-1], &numbers[i]);
	// Add the last after tail
	uint32_t intCount = intCont.insNext (&numbers[3], &numbers[4]);

	cout << intCount << " elements added = ";
	if (5 != intCount) {
		cout << "FAIL" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else {
		cout << "Success" << endl;
		++env.testSuccess;
	}

	/** B) Print out elements 1, 3 and 5 which should equal their numbers **/
	if (EXIT_SUCCESS == result) {
		cout << adjRight (4, 0) << ++env.testCount << " B) Print out element 1, 3, 5: ";
		int32_t intOne   = *intCont[0]->data;
		int32_t intThree = * (*intCont[2]).data.get();
		int32_t intFive  = **intCont[4];
		cout << intOne << ", " << intThree << ", " << intFive << " = ";
		if ( (1 != intOne) || (3 != intThree) || (5 != intFive)) {
			cout << "FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		} else {
			cout << "Success" << endl;
			++env.testSuccess;
		}
	} // end of sub test B

	/** C) Remove elements 1, 3 and 5, resulting size should be 2         **/
	if (EXIT_SUCCESS == result) {
		cout << adjRight (4, 0) << ++env.testCount << " C) Remove elements 1, 3 and 5: ";
		intCont.delNext (nullptr); // remove 1, which is head
		intCont.delNextElem (intCont.find (&numbers[1])); // remove 3 via pointer search for "2"
		intCont.delNext (&numbers[3]); // remove 5 via direct pointer to "4"
		intCount = intCont.size();
		if (2 != intCount) {
			cout << "(" << intCount << "/2) FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		} else {
			cout << "(2/2) Success" << endl;
			++env.testSuccess;
		}
	} // end of sub test C

	/** D) Print out all remaining elements, which should be 2 and 4       **/
	if (EXIT_SUCCESS == result) {
		cout << adjRight (4, 0) << ++env.testCount << " D) Remaining elements: ";
		int32_t intOne, intTwo;
		PWX_TRY_PWX_FURTHER (intOne = **intCont[0])
		PWX_TRY_PWX_FURTHER (intTwo = **intCont[1])
		cout << intOne << ", " << intTwo << " = ";
		if ( (2 != intOne) || (4 != intTwo)) {
			cout << "FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		} else {
			cout << "Success" << endl;
			++env.testSuccess;
		}
	} // End of sub test D

	cout << endl;

	return result;
}

#endif // PWX_LIBPWX_TESTLIB_TESTLISTRING_H_INCLUDED
