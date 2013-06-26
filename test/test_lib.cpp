#include "test_lib.h"

int32_t main(int argc, char* argv[])
{
	int32_t result  = EXIT_SUCCESS;
	sEnv env;

	// Disable speed test?
	if ((argc > 1) && (STREQ(argv[1], "--no-speed-test")))
		env.doSpeed = false;

	// Wrap a giant try/catch around just everything to trace immediately
	try {
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

		// --- test the speed of the containers ---
		if (EXIT_SUCCESS == result) {
			uint32_t localMaxElem = env.doSpeed ? maxElements : maxThreads * 100;
			cout << "Testing the speed of the containers\n-----------------------------------" << endl;
			cout << " (Inserting " << localMaxElem;
			cout << " random elements and clear up)" << endl;
			cout << "                                               Add /      Clear" << endl;

// Little evil shortcut
#define do_testSpeed(container_type, key_type, value_type, thread_type, thread_count) \
try { \
result = testSpeed<container_type, key_type, value_type, \
	thread_type<container_type, key_type, value_type>, \
	thClearer<container_type>>(env, testCont, thread_count); \
} PWX_THROW_PWX_FURTHER

/// DEBUG
			// Chained Hash Tables
			if (EXIT_SUCCESS == result) {
				chash_t testCont(static_cast<uint32_t>(std::ceil(localMaxElem / 2.9)),
								 do_not_destroy, nullptr, 3.0, 1.5);
				do_testSpeed(chash_t, keydata_t, hashval_t, thAdderHash, 1)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(chash_t, keydata_t, hashval_t, thAdderHash, maxThreads)
				}
			}
			// Open Hash Tables
			if (EXIT_SUCCESS == result) {
				ohash_t testCont(static_cast<uint32_t>(std::ceil(localMaxElem / 0.79)),
								 do_not_destroy, nullptr, 0.81, 1.5);
				do_testSpeed(ohash_t, keydata_t, hashval_t, thAdderHash, 1)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(ohash_t, keydata_t, hashval_t, thAdderHash, maxThreads)
				}
			}
/// !DEBUG
			// Singly Linked Lists
			if (EXIT_SUCCESS == result) {
				single_list_t testCont(do_not_destroy);
				do_testSpeed(single_list_t, keydata_t, keydata_t, thAdderList, 1)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(single_list_t, keydata_t, keydata_t, thAdderList, maxThreads)
				}
			}
			// Doubly Linked Lists
			if (EXIT_SUCCESS == result) {
				double_list_t testCont(do_not_destroy);
				do_testSpeed(double_list_t, keydata_t, keydata_t, thAdderList, 1)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(double_list_t, keydata_t, keydata_t, thAdderList, maxThreads)
				}
			}
			// Singly Linked rings
			if (EXIT_SUCCESS == result) {
				single_ring_t testCont(do_not_destroy);
				do_testSpeed(single_ring_t, keydata_t, keydata_t, thAdderList, 1)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(single_ring_t, keydata_t, keydata_t, thAdderList, maxThreads)
				}
			}
			// Doubly Linked Rings
			if (EXIT_SUCCESS == result) {
				double_ring_t testCont(do_not_destroy);
				do_testSpeed(double_ring_t, keydata_t, keydata_t, thAdderList, 1)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(double_ring_t, keydata_t, keydata_t, thAdderList, maxThreads)
				}
			}
			// Stacks
			if (EXIT_SUCCESS == result) {
				stack_t testCont(do_not_destroy);
				do_testSpeed(stack_t, keydata_t, keydata_t, thAdderList, 1)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(stack_t, keydata_t, keydata_t, thAdderList, maxThreads)
				}
			}
			// Queues
			if (EXIT_SUCCESS == result) {
				queue_t testCont(do_not_destroy);
				do_testSpeed(queue_t, keydata_t, keydata_t, thAdderList, 1)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(queue_t, keydata_t, keydata_t, thAdderList, maxThreads)
				}
			}
			// Sets
			if (EXIT_SUCCESS == result) {
				set_t testCont(do_not_destroy);
				do_testSpeed(set_t, keydata_t, keydata_t, thAdderList, 1)
				if (EXIT_SUCCESS == result) {
					do_testSpeed(set_t, keydata_t, keydata_t, thAdderList, maxThreads)
				}
			}
//			// Chained Hash Tables
//			if (EXIT_SUCCESS == result) {
//				chash_t testCont(static_cast<uint32_t>(std::ceil(localMaxElem / 2.9)),
//								 do_not_destroy, nullptr, 3.0, 1.5);
//				do_testSpeed(chash_t, keydata_t, hashval_t, thAdderHash, 1)
//				if (EXIT_SUCCESS == result) {
//					do_testSpeed(chash_t, keydata_t, hashval_t, thAdderHash, maxThreads)
//				}
//			}
//			// Open Hash Tables
//			if (EXIT_SUCCESS == result) {
//				ohash_t testCont(static_cast<uint32_t>(std::ceil(localMaxElem / 0.79)),
//								 do_not_destroy, nullptr, 0.81, 1.5);
//				do_testSpeed(ohash_t, keydata_t, hashval_t, thAdderHash, 1)
//				if (EXIT_SUCCESS == result) {
//					do_testSpeed(ohash_t, keydata_t, hashval_t, thAdderHash, maxThreads)
//				}
//			}
		} // End of speed tests

#undef do_testSpeed

	// --- Test RNG worker ---
	if (EXIT_SUCCESS == result ) {
		PWX_TRY_PWX_FURTHER (result = testRNG (env))
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
