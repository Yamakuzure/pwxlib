/** @file test_lib.cpp
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include "test_lib.h"

#include "testListRing.h"
#include "testStackQueue.h"
#include "testHash.h"
#include "testPAH.h"
#include "testSet.h"
#include "testSpeed.h"
#include "testRNG.h"
#include "testSCT.h"
#include "testColor.h"

#if PWX_SMALL_TESTS
/// Default number of elements to use with all mass and speed tests (but hashes and sets)
uint32_t maxElements = 2500;
/// Default number of elements to use with all mass and speed tests (hashes and sets ONLY)
uint32_t maxHashVals = 7439;
#else
/// Default number of elements to use with all mass and speed tests (but hashes and sets)
uint32_t maxElements = 250000;
/// Default number of elements to use with all mass and speed tests (hashes and sets ONLY)
uint32_t maxHashVals = 741839;
#endif // PWX_SMALL_TESTS

/// Default number of threads to launch for parallel testing
uint32_t maxThreads  = 8;


int32_t main( int argc, char* argv[] ) {
	int32_t result  = EXIT_SUCCESS;
	sEnv env;

	// Handle arguments
	uint32_t doWhichTests = 0;
	bool     showHelp     = false;
	for ( int i = 1; !showHelp && ( i < argc ); ++i ) {
		if ( STRCEQ( argv[i], "--help" ) || STRCEQ( argv[i], "-h" ) ) showHelp = true;
		else if ( STRCEQ( argv[i], "--args" )  || STRCEQ( argv[i], "-a" ) ) doWhichTests |= doTestPAH;
		else if ( STRCEQ( argv[i], "--cont" )  || STRCEQ( argv[i], "-c" ) ) doWhichTests |= doTestContainers;
		else if (                              STRCEQ( argv[i], "-m" ) ) {
			int m = atoi( argv[++i] );
			if ( m > 9999 ) {
				maxElements = static_cast<uint32_t>( m );
				maxHashVals = std::floor( maxElements / 0.337 );
			} else {
				cerr << "Error: \"" << argv[i] << "\" is no valid number for\n";
				cerr << "       the maximum number of elements. (Minimum 10000)" << endl;
				showHelp = true;
			}

		} else if ( STRCEQ( argv[i], "--speed" ) || STRCEQ( argv[i], "-p" ) ) doWhichTests |= doTestSpeed;
		else if ( STRCEQ( argv[i], "--rng" )   || STRCEQ( argv[i], "-r" ) ) doWhichTests |= doTestRNG;
		else if ( STRCEQ( argv[i], "--sct" )   || STRCEQ( argv[i], "-s" ) ) doWhichTests |= doTestSCT;
		else if (                              STRCEQ( argv[i], "-w" ) ) doWhichTests |= doTestCWaveColor;
		else if (                              STRCEQ( argv[i], "-t" ) ) {
			int t = atoi( argv[++i] );
			if ( ( t > 1 ) && ( t < 65 ) )
				maxThreads = static_cast<uint32_t>( t );
			else {
				cerr << "Error: \"" << argv[i] << "\" is no valid number for\n";
				cerr << "       the maximum number of threads. (2 - 64)" << endl;
				showHelp = true;
			}

		} else {
			cerr << "Error: \"" << argv[i] << "\" unknown!" << endl;
			showHelp = true;
		}
	}

	// Need help?
	if ( showHelp ) {
		cout << "Usage: " << argv[0] << " [options]\n" << endl;
		cout << "Options:\n";
		cout << "  -a  --args   Test PAH\n";
		cout << "  -c  --cont   Test containers\n";
		cout << "  -h  --help   Show this help and exit\n";
		cout << "  -m <10000+>  Maximum elements for speed tests (200,000)\n";
		cout << "  -p  --speed  Test the speed of the containers\n";
		cout << "  -r  --rng    Test RNG\n";
		cout << "  -s  --sct    Test SCT\n";
		cout << "  -t <2-64>    Number of threads for speed tests (8)\n";
		cout << "  -w           Test CWaveColor\n";
		cout << "All tests are done by default." << endl;
		return EXIT_SUCCESS;
	}

	// do all tests?
	if ( 0 == doWhichTests )
		doWhichTests = doTestAll;

	// Now tell env about the tests
	if ( doWhichTests & doTestContainers ) env.doCont  = true;
	if ( doWhichTests & doTestSpeed )      env.doSpeed = true;
	if ( doWhichTests & doTestRNG )        env.doRNG   = true;
	if ( doWhichTests & doTestSCT )        env.doSCT   = true;
	if ( doWhichTests & doTestCWaveColor ) env.doCWC   = true;
	if ( doWhichTests & doTestPAH )        env.doPAH   = true;

	// Wrap a giant try/catch around just everything to trace immediately
	try {
		if ( env.doCont ) {
			// --- General container tests ---
			if ( EXIT_SUCCESS == result ) {
				PWX_TRY_PWX_FURTHER ( result = testListRing<single_list_t> ( env ) )
			}
			if ( EXIT_SUCCESS == result ) {
				PWX_TRY_PWX_FURTHER ( result = testListRing<double_list_t> ( env ) )
			}
			if ( EXIT_SUCCESS == result ) {
				PWX_TRY_PWX_FURTHER ( result = testListRing<single_ring_t> ( env ) )
			}
			if ( EXIT_SUCCESS == result ) {
				PWX_TRY_PWX_FURTHER ( result = testListRing<double_ring_t> ( env ) )
			}
			if ( EXIT_SUCCESS == result ) {
				PWX_TRY_PWX_FURTHER ( result = testStackQueue<pstack_t> ( env ) )
			}
			if ( EXIT_SUCCESS == result ) {
				PWX_TRY_PWX_FURTHER ( result = testStackQueue<pqueue_t> ( env ) )
			}
			if ( EXIT_SUCCESS == result ) {
				PWX_TRY_PWX_FURTHER ( result = testSet<set_t> ( env ) )
			}
			if ( EXIT_SUCCESS == result ) {
				PWX_TRY_PWX_FURTHER ( result = testHash<chash_t> ( env ) )
			}
			if ( EXIT_SUCCESS == result ) {
				PWX_TRY_PWX_FURTHER ( result = testHash<ohash_t> ( env ) )
			}
		} // End of container tests

		// --- test the speed of the containers ---
		if ( ( EXIT_SUCCESS == result ) && env.doSpeed ) {
			cout << "Testing the speed of the containers\n-----------------------------------" << endl;
			cout << " (Inserting " << maxElements << " random elements, search ";
			cout << ( maxElements / 1000 ) << " elements and clear up)" << endl;
			cout << " (Note: Hashes and sets use ";
			cout << maxHashVals << "/" << ( maxHashVals / 10 ) << " elements.)" << endl;
			cout << "                                                      ";
			cout << "Add /   Search /   Clear" << endl;

			// Little evil shortcut
#define do_testSpeed(container_type, key_type, value_type, adder_type, searcher_type, thread_count, values, retrieves, sorted) \
{ try { \
			result = testSpeed<container_type, key_type, value_type, \
			         adder_type<container_type, key_type, value_type>, \
			         searcher_type<container_type, key_type, value_type>, \
			         thClearer<container_type>>(env, testCont, thread_count, values, retrieves, sorted); \
		} PWX_THROW_PWX_FURTHER }

			keydata_t* values    = nullptr;
			keydata_t* retrieves = nullptr;

			// Singly Linked Lists
			if ( EXIT_SUCCESS == result ) {
				single_list_t testCont( do_not_destroy );
				do_testSpeed( single_list_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves, false );
				if ( EXIT_SUCCESS == result )
					do_testSpeed( single_list_t, keydata_t, keydata_t, thAdderSorted, thSearcherList, 1, &values, &retrieves, true );

				if ( EXIT_SUCCESS == result )
					do_testSpeed( single_list_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves, false );

				if ( EXIT_SUCCESS == result )
					do_testSpeed( single_list_t, keydata_t, keydata_t, thAdderSorted, thSearcherList, maxThreads, &values, &retrieves, true );
			}
			// Doubly Linked Lists
			if ( EXIT_SUCCESS == result ) {
				double_list_t testCont( do_not_destroy );
				do_testSpeed( double_list_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves, false );
				if ( EXIT_SUCCESS == result )
					do_testSpeed( double_list_t, keydata_t, keydata_t, thAdderSorted, thSearcherList, 1, &values, &retrieves, true );

				if ( EXIT_SUCCESS == result )
					do_testSpeed( double_list_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves, false );

				if ( EXIT_SUCCESS == result )
					do_testSpeed( double_list_t, keydata_t, keydata_t, thAdderSorted, thSearcherList, maxThreads, &values, &retrieves, true );
			}
			// Singly Linked rings
			if ( EXIT_SUCCESS == result ) {
				single_ring_t testCont( do_not_destroy );
				do_testSpeed( single_ring_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves, false );
				if ( EXIT_SUCCESS == result )
					do_testSpeed( single_ring_t, keydata_t, keydata_t, thAdderSorted, thSearcherList, 1, &values, &retrieves, true );

				if ( EXIT_SUCCESS == result )
					do_testSpeed( single_ring_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves, false );

				if ( EXIT_SUCCESS == result )
					do_testSpeed( single_ring_t, keydata_t, keydata_t, thAdderSorted, thSearcherList, maxThreads, &values, &retrieves, true );
			}
			// Doubly Linked Rings
			if ( EXIT_SUCCESS == result ) {
				double_ring_t testCont( do_not_destroy );
				do_testSpeed( double_ring_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves, false );
				if ( EXIT_SUCCESS == result )
					do_testSpeed( double_ring_t, keydata_t, keydata_t, thAdderSorted, thSearcherList, 1, &values, &retrieves, true );

				if ( EXIT_SUCCESS == result )
					do_testSpeed( double_ring_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves, false );

				if ( EXIT_SUCCESS == result )
					do_testSpeed( double_ring_t, keydata_t, keydata_t, thAdderSorted, thSearcherList, maxThreads, &values, &retrieves, true );
			}
			// Stacks
			if ( EXIT_SUCCESS == result ) {
				pstack_t testCont( do_not_destroy );
				do_testSpeed( pstack_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves, false );
				if ( EXIT_SUCCESS == result )
					do_testSpeed( pstack_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves, false );
			}
			// Queues
			if ( EXIT_SUCCESS == result ) {
				pqueue_t testCont( do_not_destroy );
				do_testSpeed( pqueue_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves, false );
				if ( EXIT_SUCCESS == result )
					do_testSpeed( pqueue_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves, false );
			}

			// Delete the values, as hashes search for more. Otherwise the
			// enormous speed of sets and hashes results in ~0ms results.
			if ( values ) {
				delete [] values;
				values = nullptr;
			}
			if ( retrieves ) {
				delete [] retrieves;
				retrieves = nullptr;
			}

			// Sets
			if ( EXIT_SUCCESS == result ) {
				set_t testCont( do_not_destroy, static_cast<uint32_t>( std::ceil( maxHashVals / 2.873 ) ),
				                nullptr, 0, 3.0, 1.5 );
				do_testSpeed( set_t, keydata_t, keydata_t, thAdderList, thSearcherList, 1, &values, &retrieves, false );
				if ( EXIT_SUCCESS == result )
					do_testSpeed( set_t, keydata_t, keydata_t, thAdderList, thSearcherList, maxThreads, &values, &retrieves, false );
			}
			// Chained Hash Tables
			if ( EXIT_SUCCESS == result ) {
				chash_t testCont( static_cast<uint32_t>( std::ceil( maxHashVals / 2.873 ) ),
				                  do_not_destroy, nullptr, 3.0, 1.5 );
				do_testSpeed( chash_t, hashval_t, keydata_t, thAdderHash, thSearcherHash, 1, &values, &retrieves, false );
				if ( EXIT_SUCCESS == result )
					do_testSpeed( chash_t, hashval_t,  keydata_t, thAdderHash, thSearcherHash, maxThreads, &values, &retrieves, false );
			}
			// Open Hash Tables
			if ( EXIT_SUCCESS == result ) {
				ohash_t testCont( static_cast<uint32_t>( std::ceil( maxHashVals / 0.79 ) ),
				                  do_not_destroy, nullptr, 0.81, 1.5 );
				do_testSpeed( ohash_t, hashval_t, keydata_t, thAdderHash, thSearcherHash, 1, &values, &retrieves, false );
				if ( EXIT_SUCCESS == result )
					do_testSpeed( ohash_t, hashval_t, keydata_t, thAdderHash, thSearcherHash, maxThreads, &values, &retrieves, false );
			}
			// === Clear up values ===
			if ( values )    delete [] values;
			if ( retrieves ) delete [] retrieves;
		} // End of speed tests

#undef do_testSpeed

		// --- Test RNG worker ---
		if ( ( EXIT_SUCCESS == result ) && env.doRNG ) {
			PWX_TRY_PWX_FURTHER ( result = testRNG ( env ) )
		}

		// --- Test SCT worker ---
		if ( ( EXIT_SUCCESS == result ) && env.doSCT ) {
			PWX_TRY_PWX_FURTHER ( result = testSCT ( env ) )
		}

		// --- Test PAH worker ---
		if ( ( EXIT_SUCCESS == result ) && env.doPAH ) {
			PWX_TRY_PWX_FURTHER ( result = testPAH ( env ) )
		}

		// --- Test CWavecolor type ---
		if ( ( EXIT_SUCCESS == result ) && env.doCWC ) {
			PWX_TRY_PWX_FURTHER ( result = testColor ( env ) )
		}

		// End of giant try
	} catch ( pwx::CException& e ) {
		cerr << "\n-----\npwx exception \"" << e.name() << "\" caught!" << endl;
		cerr << "What : \"" << e.what() << "\"" << endl;
		cerr << "What : \"" << e.desc() << "\"" << endl;
		cerr << "Where: \"" << e.where() << "\"" << endl;
		cerr << "pFunc: \"" << e.pfunc() << "\"" << endl;
		cerr << "\nTrace:\n" << e.trace() << "\n-----" << endl;
		result = EXIT_FAILURE;
	} catch ( std::exception& e ) {
		cerr << "\n-----\nstd exception caught!" << endl;
		cerr << "What : \"" << e.what() << "\"" << endl;
		result = EXIT_FAILURE;
	} catch ( ... ) {
		cerr << "\n-----\nSomething completely unknown was caught!" << endl;
		result = EXIT_FAILURE;
	}

	cout << "----------------" << endl;
	cout << " Tests done      : " << env.testCount << endl;
	cout << " Tests successful: " << env.testSuccess << endl;
	cout << " Tests failed    : " << env.testFail << endl;

	pwx::finish();

	return result;
}
