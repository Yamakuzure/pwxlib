#include "torture.h"

int32_t main(int32_t argc, char** argv)
{
	int32_t result = EXIT_SUCCESS;
	uint32_t  numThreads = 32;
	eTestType testType   = E_TEST_ALL;

	// Check the arguments:
	for (int32_t i = 1; (EXIT_SUCCESS == result) && (i < argc); ++i) {
		if (STREQ("-c", argv[i])) {
			if ((argc > (i + 1)) && (argv[i+1][0] != '-'))
				result = setTestType(argv[++i], testType);
			else {
				cerr << "Argument -c needs a type option!\n";
				cerr << "  Use either -h or --help for further information." << endl;
				result = EXIT_FAILURE;
			}
		} else if (STREQ("--help", argv[i]) || STREQ("-h", argv[i])) {
			cout << "Usage:\n  " << basename(argv[0]) << " [[-h|--help] | [-c type] [-t num]]\n";
			cout << "  -c type     : set the type of container to check\n";
			cout << "       all    : Test all containers (default)\n";
			cout << "       list   : Test TSingleList and TDoubleList\n";
			cout << "       list_d : Test TDoubleList\n";
			cout << "       list_s : Test TSingleList\n";
			cout << "       queue  : Test TQueue\n";
			cout << "       ring   : Test TSingleRing and TDoubleRing\n";
			cout << "       ring_d : Test TDoubleRing\n";
			cout << "       ring_s : Test TSingleRing\n";
			cout << "       set    : Test TSet\n";
			cout << "       stack  : Test TStack\n";
			cout << "  -h / --help : Show this help and exit\n";
			cout << "  -t num      : Set the number of threads (default: 32)" << endl;
			return result;
		} else if (STREQ("-t", argv[i])) {
			if ((argc > (i + 1)) && (argv[i+1][0] != '-'))
				result = setNumThreads(argv[++i], numThreads);
			else {
				cerr << "Argument -t needs a number option!\n";
				cerr << "  Use either -h or --help for further information." << endl;
				result = EXIT_FAILURE;
			}
		}
	} // Finished checking arguments


	if (EXIT_SUCCESS == result)
		result = startTest(numThreads, testType);

	return result;
}


// Small helper to get the number out of an argument and check it
int32_t setNumThreads(const char* chNum, uint32_t &numThreads)
{
	int32_t result = EXIT_SUCCESS;

	numThreads = pwx::to_uint32(chNum);

	if (numThreads < 2) {
		cerr << "\"" << chNum << "\" is invalid as a number of threads to start.\n";
		cerr << "Anything not even or greater than 2 is absolutely pointless." << endl;
		result = EXIT_FAILURE;
	}

	return result;
}


// Small helper to get the test type out of an argument and check it
int32_t setTestType(const char* chType, eTestType &testType)
{
	int32_t result = EXIT_SUCCESS;

	if (STREQ("all", chType))
		testType = E_TEST_ALL;
	else if (STREQ("list", chType))
		testType = E_TEST_LIST;
	else if (STREQ("list_d", chType))
		testType = E_TEST_LIST_D;
	else if (STREQ("list_s", chType))
		testType = E_TEST_LIST_S;
	else if (STREQ("queue", chType))
		testType = E_TEST_QUEUE;
	else if (STREQ("ring", chType))
		testType = E_TEST_RING;
	else if (STREQ("ring_d", chType))
		testType = E_TEST_RING_D;
	else if (STREQ("ring_s", chType))
		testType = E_TEST_RING_S;
	else if (STREQ("set", chType))
		testType = E_TEST_SET;
	else if (STREQ("stack", chType))
		testType = E_TEST_STACK;
	else {
		cerr << "\"" << chType << "\" is invalid as a check type.\n";
		result = EXIT_FAILURE;
	}

	return result;
}


/// @brief the template that produces the test results
template<typename list_t>
int32_t do_test(uint32_t numThreads)
{
	int32_t result = EXIT_SUCCESS;



	return result;
}


/// @brief main test starting function
int32_t startTest(uint32_t numThreads, eTestType testType)
{
	int32_t result = EXIT_SUCCESS;

	if ((EXIT_SUCCESS == result)
	  && ( (testType == E_TEST_ALL)
		|| (testType == E_TEST_LIST)
		|| (testType == E_TEST_LIST_S)))
		result = do_test<pwx::TSingleList<int32_t> >(numThreads);
	if ((EXIT_SUCCESS)
	  && ( (testType == E_TEST_ALL)
		|| (testType == E_TEST_LIST)
		|| (testType == E_TEST_LIST_D)))
		result = do_test<pwx::TDoubleList<int32_t>>(numThreads);
	if ((EXIT_SUCCESS)
	  && ( (testType == E_TEST_ALL)
		|| (testType == E_TEST_QUEUE)))
		result = do_test<pwx::TQueue<int32_t>>(numThreads);
	if ((EXIT_SUCCESS == result)
	  && ( (testType == E_TEST_ALL)
		|| (testType == E_TEST_RING)
		|| (testType == E_TEST_RING_S)))
		result = do_test<pwx::TSingleRing<int32_t>>(numThreads);
	if ((EXIT_SUCCESS)
	  && ( (testType == E_TEST_ALL)
		|| (testType == E_TEST_RING)
		|| (testType == E_TEST_RING_D)))
		result = do_test<pwx::TDoubleRing<int32_t>>(numThreads);
	if ((EXIT_SUCCESS)
	  && ( (testType == E_TEST_ALL)
		|| (testType == E_TEST_SET)))
		result = do_test<pwx::TSet<int32_t>>(numThreads);
	if ((EXIT_SUCCESS)
	  && ( (testType == E_TEST_ALL)
		|| (testType == E_TEST_STACK)))
		result = do_test<pwx::TStack<int32_t>>(numThreads);

	return result;
}


