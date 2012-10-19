#include "main.h"

int32_t main()
{
	int32_t result = EXIT_SUCCESS;
	sEnv env;

	// Wrap a giant try/catch around just everything to trace immediately
	try {

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
