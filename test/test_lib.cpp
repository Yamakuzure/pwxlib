#include "test_lib.h"

int32_t main()
{
	int32_t result = EXIT_SUCCESS;
	sEnv env;

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

		// --- test the speed of the containers ---
		if (EXIT_SUCCESS == result) {
			cout << "Testing the speed of the containers\n-----------------------------------" << endl;
			cout << " (Inserting " << maxElements << " random elements and clear up)" << endl;
			cout << "                                               Add /      Clear" << endl;
			PWX_TRY_PWX_FURTHER (result = testSpeedST<single_list_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedMT<single_list_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedST<double_list_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedMT<double_list_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedST<single_ring_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedMT<single_ring_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedST<double_ring_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedMT<double_ring_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedST<stack_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedMT<stack_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedST<queue_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedMT<queue_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedST<set_t> (env))
		}
		if (EXIT_SUCCESS == result) {
			PWX_TRY_PWX_FURTHER (result = testSpeedMT<set_t> (env))
		}

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
