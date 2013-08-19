#include "test_lib.h"

int32_t main(int argc, char* argv[])
{
	int32_t result  = EXIT_SUCCESS;
	sEnv env;

	// Handle arguments
	uint32_t doWhichTests = 0;
	bool     showHelp     = false;
	for (int i = 1; !showHelp && (i < argc); ++i) {
		if (STRCEQ(argv[i], "--help") || STRCEQ(argv[i], "-h")) showHelp = true;
		else if (STRCEQ(argv[i], "--cont")  || STRCEQ(argv[i], "-c")) doWhichTests |= doTestContainers;
		else if (STRCEQ(argv[i], "--speed") || STRCEQ(argv[i], "-p")) doWhichTests |= doTestSpeed;
		else if (STRCEQ(argv[i], "--rng")   || STRCEQ(argv[i], "-r")) doWhichTests |= doTestRNG;
		else if (STRCEQ(argv[i], "--sct")   || STRCEQ(argv[i], "-s")) doWhichTests |= doTestSCT;
	}

	// Need help?
	if (showHelp) {
		cout << "Usage: " << argv[0] << " [options]\n" << endl;
		cout << "Options:\n";
		cout << "  -c  --cont   test containers\n";
		cout << "  -h  --help   Show this help and exit\n";
		cout << "  -p  --speed  test the speed of the containers\n";
		cout << "  -r  --rng    test RNG\n";
		cout << "  -s  --sct    test SCT\n";
		cout << "All tests are done by default." << endl;
		return EXIT_SUCCESS;
	}

	// do all tests?
	if (0 == doWhichTests)
		doWhichTests = doTestAll;

	// Now tell env about the tests
	if (doWhichTests & doTestContainers) env.doCont  = true;
	if (doWhichTests & doTestSpeed)      env.doSpeed = true;
	if (doWhichTests & doTestRNG)        env.doRNG   = true;
	if (doWhichTests & doTestSCT)        env.doSCT   = true;

	// Wrap a giant try/catch around just everything to trace immediately
	try {
		if (env.doCont) {
			// --- General container tests ---
			if (EXIT_SUCCESS == result) {
				PWX_TRY_PWX_FURTHER (result = testListRing<single_list_t> (env))
			}
			if (EXIT_SUCCESS == result) {
				PWX_TRY_PWX_FURTHER (result = testListRing<double_list_t> (env))
			}
			if (EXIT_SUCCESS == result) {
				PWX_TRY_PWX_FURTHER (result = testListRing<single_ring_t> (env))
			}
			if (EXIT_SUCCESS == result) {
				PWX_TRY_PWX_FURTHER (result = testListRing<double_ring_t> (env))
			}
			if (EXIT_SUCCESS == result) {
				PWX_TRY_PWX_FURTHER (result = testStackQueue<stack_t> (env))
			}
			if (EXIT_SUCCESS == result) {
				PWX_TRY_PWX_FURTHER (result = testStackQueue<queue_t> (env))
			}
			if (EXIT_SUCCESS == result) {
				PWX_TRY_PWX_FURTHER (result = testSet<set_t> (env))
			}
			if (EXIT_SUCCESS == result) {
				PWX_TRY_PWX_FURTHER (result = testHash<chash_t> (env))
			}
			if (EXIT_SUCCESS == result) {
				PWX_TRY_PWX_FURTHER (result = testHash<ohash_t> (env))
			}
		} // End of container tests

		// --- test the speed of the containers ---
		if ((EXIT_SUCCESS == result) && env.doSpeed) {
			cout << "Testing the speed of the containers\n-----------------------------------" << endl;
			cout << " (Inserting " << maxElements << " random elements and clear up)" << endl;
			cout << "                                               Add /   Search /   Clear" << endl;

// Little evil shortcut
#define do_testSpeed(container_type, key_type, value_type, adder_type, searcher_type, thread_count, values, retrieves) \
try { \
result = testSpeed<container_type, key_type, value_type, \
	adder_type<container_type, key_type, value_type>, \
	searcher_type<container_type, key_type, value_type>, \
	thClearer<container_type>>(env, testCont, thread_count, values, retrieves); \
} PWX_THROW_PWX_FURTHER

			keydata_t* values    = nullptr;
			keydata_t* retrieves = nullptr;

			// Singly Linked Lists
			if (EXIT_SUCCESS == result) {
				single_list_t testCont(do_not_destroy);
				do_testSpeed(single_list_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(single_list_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves)
				}
			}
			// Doubly Linked Lists
			if (EXIT_SUCCESS == result) {
				double_list_t testCont(do_not_destroy);
				do_testSpeed(double_list_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(double_list_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves)
				}
			}
			// Singly Linked rings
			if (EXIT_SUCCESS == result) {
				single_ring_t testCont(do_not_destroy);
				do_testSpeed(single_ring_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(single_ring_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves)
				}
			}
			// Doubly Linked Rings
			if (EXIT_SUCCESS == result) {
				double_ring_t testCont(do_not_destroy);
				do_testSpeed(double_ring_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(double_ring_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves)
				}
			}
			// Stacks
			if (EXIT_SUCCESS == result) {
				stack_t testCont(do_not_destroy);
				do_testSpeed(stack_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(stack_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves)
				}
			}
			// Queues
			if (EXIT_SUCCESS == result) {
				queue_t testCont(do_not_destroy);
				do_testSpeed(queue_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(queue_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves)
				}
			}
			// Sets
			if (EXIT_SUCCESS == result) {
				set_t testCont(do_not_destroy);
				do_testSpeed(set_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(set_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves)
				}
			}
			// Chained Hash Tables
			if (EXIT_SUCCESS == result) {
				chash_t testCont(static_cast<uint32_t>(std::ceil(maxElements / 2.873)),
								 do_not_destroy, nullptr, 3.0, 1.5);
				do_testSpeed(chash_t, hashval_t, keydata_t, thAdderHash, thSearcherHash, 1, &values, &retrieves)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(chash_t,hashval_t,  keydata_t, thAdderHash, thSearcherHash, maxThreads, &values, &retrieves)
				}
			}
			// Open Hash Tables
			if (EXIT_SUCCESS == result) {
				ohash_t testCont(static_cast<uint32_t>(std::ceil(maxElements / 0.79)),
								 do_not_destroy, nullptr, 0.81, 1.5);
				do_testSpeed(ohash_t, hashval_t, keydata_t, thAdderHash, thSearcherHash, 1, &values, &retrieves)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(ohash_t, hashval_t, keydata_t, thAdderHash, thSearcherHash, maxThreads, &values, &retrieves)
				}
			}
			// === Clear up values ===
			if (values)    delete [] values;
			if (retrieves) delete [] retrieves;
		} // End of speed tests

#undef do_testSpeed

	// --- Test RNG worker ---
	if ((EXIT_SUCCESS == result) && env.doRNG) {
		PWX_TRY_PWX_FURTHER (result = testRNG (env))
	}

	// --- Test SCT worker ---
	if ((EXIT_SUCCESS == result) && env.doSCT) {
		PWX_TRY_PWX_FURTHER (result = testSCT (env))
	}

		// End of giant try
	} catch (pwx::CException &e) {
		cerr << "\n-----\npwx exception \"" << e.name() << "\" caught!" << endl;
		cerr << "What : \"" << e.what() << "\"" << endl;
		cerr << "What : \"" << e.desc() << "\"" << endl;
		cerr << "Where: \"" << e.where() << "\"" << endl;
		cerr << "pFunc: \"" << e.pfunc() << "\"" << endl;
		cerr << "\nTrace:\n" << e.trace() << "\n-----" << endl;
		result = EXIT_FAILURE;
	} catch (std::exception &e) {
		cerr << "\n-----\nstd exception caught!" << endl;
		cerr << "What : \"" << e.what() << "\"" << endl;
		result = EXIT_FAILURE;
	} catch (...) {
		cerr << "\n-----\nSomething completely unknown was caught!" << endl;
		result = EXIT_FAILURE;
	}

	cout << "----------------" << endl;
	cout << " Tests done      : " << env.testCount << endl;
	cout << " Tests successful: " << env.testSuccess << endl;
	cout << " Tests failed    : " << env.testFail << endl;

	return result;
}
