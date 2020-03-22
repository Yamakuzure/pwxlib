#ifndef PWX_LIBPWX_TEST_TESTHASH_H_INCLUDED
#define PWX_LIBPWX_TEST_TESTHASH_H_INCLUDED

/** @file testHash.h
  * (c) 2007 - 2020 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include "test_lib.h" // This is here for IDE Parsers to find the other stuff

/// Typedefs for the specific set:
#include <PChainHash>
#include <POpenHash>
#include <RNG>
typedef PChainHash<keydata_t, hashval_t>        chash_t;
typedef POpenHash<keydata_t, hashval_t>         ohash_t;
typedef pwx::THashElement<keydata_t, hashval_t> elem_t;

/** @brief test a specific set type template
  *
  ****************************************************************************
  ** === Generic template function to test hash type containers ===         **
  ** A) Create a container and add 5 keydata_t/hashval_t pairs              **
  ** B) List three elements with keys 1, 3, 4 (1.1, 2.2, 4.4)               **
  ** C) Remove elements with the keys 2, 4 (2.2, 4.4)                       **
  ** D) Loop all remaining elements (1->1.1, 3->3.3, 5->5.5)                **
  ** E) Build a second hash (all 5 elements) and add to first (5 elements)  **
  ** F) Remove elements 2,4 from the second, substract remaining from first **
  **    (2->2.2, 4->4.4)                                                    **
  ** G) Fill in 500 random elements. The OpenHash then has to grow several  **
  **    times, so this is tested, too.
  ****************************************************************************
**/
template<typename hash_t>
int32_t testHash ( sEnv& env ) {
	int32_t result = EXIT_SUCCESS;

	cout << "Test ";
	if ( isSameType ( hash_t, chash_t ) )
		cout << "chained hash" << endl;
	else if ( isSameType ( hash_t, ohash_t ) )
		cout << "open hash" << endl;
	else {
		cout << " : nothing - the type is unknown!" << endl;
		return EXIT_FAILURE;
	}
	cout << "----------------------------" << endl;

	hash_t ifHash ( do_not_destroy, nullptr ); // The hash table
	ifHash.disable_thread_safety(); // This is strictly single threaded.
	keydata_t keys[5]   = { 1,   2,   3,   4,   5   }; // These are the keys
	hashval_t values[5] = { 1.1, 2.2, 3.3, 4.4, 5.5 }; // These are the values
	size_t  pairCount = 0;

	/***************************************************************************
	** A) Create a container and add 5 keydata_t/hashval_t pairs              **
	***************************************************************************/
	cout << adjRight ( 4, 0 ) << ++env.testCount << " A) Add five pairs : ";

	// As we only have the ADD method, we do a simple loop:
	// (All other are just wrappers to add() ! )
	for ( int i = 0; i < 5; ++i )
		pairCount = ifHash.add( keys[i], &values[i] );

	cout << pairCount << " pairs added : ";
	if ( 5 != pairCount ) {
		cout << "FAIL" << endl;
		++env.testFail;
		result = EXIT_FAILURE;
	} else {
		cout << "Success" << endl;
		++env.testSuccess;
	}

	/***************************************************************************
	** B) List three elements with keys 1, 3, 4 (1.1, 2.2, 4.4)               **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " B) Elements 1->1.1, 3->3.3, 4->4.4 are : ";
		hashval_t a = ifHash.getData( 1 );
		hashval_t b = **( ifHash.get( 3 ) );
		hashval_t c = *( ifHash.get( 4 )->data.get() );
		cout << adjLeft( 1, 1 ) << ( std::round( a * 10.0 ) / 10.0 ) << ", ";
		cout << adjLeft( 1, 1 ) << ( std::round( b * 10.0 ) / 10.0 ) << ", ";
		cout << adjLeft( 1, 1 ) << ( std::round( c * 10.0 ) / 10.0 ) << ": ";
		if ( ( 11 == static_cast<int32_t>( std::round( a * 10.0 ) ) )
		                && ( 33 == static_cast<int32_t>( std::round( b * 10.0 ) ) )
		                && ( 44 == static_cast<int32_t>( std::round( c * 10.0 ) ) ) ) {
			cout << " - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " - FAIL" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
	}

	/***************************************************************************
	** C) Remove elements with the keys 2, 4 (2.2, 4.4)                       **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " C) Remove elements 2->2.2, 4->4.4 : ";
		elem_t* ea = ifHash.remKey( 2 ); // Delete here
		elem_t* eb = ifHash.get( 4 );  // Let the container delete it
		hashval_t a = ea ? **ea : 0;
		hashval_t b = eb ? **eb : 0;
		if ( eb ) ifHash.delElem( *eb ); // aaaaaand it's gone.
		cout << adjLeft( 1, 1 ) << ( std::round( a * 10.0 ) / 10.0 ) << ", ";
		cout << adjLeft( 1, 1 ) << ( std::round( b * 10.0 ) / 10.0 ) << ": ";
		if ( ( 22 == static_cast<int32_t>( std::round( a * 10.0 ) ) )
		                && ( 44 == static_cast<int32_t>( std::round( b * 10.0 ) ) )
		                && ( 3 == ifHash.size() ) ) {
			cout << " - Success" << endl;
			++env.testSuccess;
		} else {
			cout << " - FAIL (" << ifHash.size();
			cout << "/" << static_cast<int32_t>( std::round( a * 10.0 ) );
			cout << "/" << static_cast<int32_t>( std::round( b * 10.0 ) );
			cout << ")" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
		if ( ea ) delete ea;
	}

	/***************************************************************************
	** D) Loop all remaining elements (1->1.1, 3->3.3, 5->5.5)                **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " D) Elements (1->1.1, 3->3.3, 5->5.5) are :\n        ";

		// Here we loop using operator[] to directly access the hash table:
		size_t maxCnt = ifHash.sizeMax();
		size_t found  = 0;
		hashval_t a = 0., b = 0., c = 0; // Values
		size_t x = 0, y = 0, z = 0; // indexes
		elem_t* curr;

		for ( size_t i = 0; i < maxCnt; ++i ) {
			curr = ifHash[i];
			while ( curr ) {
				if ( 1 == curr->key ) { a = **curr; x = i; }
				if ( 3 == curr->key ) { b = **curr; y = i; }
				if ( 5 == curr->key ) { c = **curr; z = i; }
				++found;
				curr = curr->getNext();
			}
		} // End of looping hash table

		cout << adjLeft( 1, 1 ) << ( std::round( a * 10.0 ) / 10.0 ) << " at index " << x << "\n        ";
		cout << adjLeft( 1, 1 ) << ( std::round( b * 10.0 ) / 10.0 ) << " at index " << y << "\n        ";
		cout << adjLeft( 1, 1 ) << ( std::round( c * 10.0 ) / 10.0 ) << " at index " << z << "\n        Result: ";
		if ( ( 11 == static_cast<int32_t>( std::round( a * 10.0 ) ) )
		                && ( 33 == static_cast<int32_t>( std::round( b * 10.0 ) ) )
		                && ( 55 == static_cast<int32_t>( std::round( c * 10.0 ) ) )
		                && ( 3 == found ) ) {
			cout << "Success" << endl;
			++env.testSuccess;
		} else {
			cout << "FAIL (" << found << ")" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
	}

	/***************************************************************************
	** E) Build a second hash (all 5 elements) and add to first (5 elements)  **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " E) Add hash with original elements : \n        ";

		hash_t ifSecond ( do_not_destroy, nullptr ); // The hash table
		ifSecond.disable_thread_safety();
		for ( int i = 0; i < 5; ++i )
			ifSecond.add( keys[i], &values[i] );

		ifHash += ifSecond;

		// just loop and print. The count must be valid, and all five keys must
		// be found with the correct values
		size_t maxCnt = ifHash.sizeMax();
		size_t found  = 0;
		bool   hasElem[5] = { false, false, false, false, false };
		elem_t* curr;

		for ( size_t i = 0; i < maxCnt; ++i ) {
			curr = ifHash[i];
			while ( curr ) {
				if ( ( curr->key >= 1 ) && ( curr->key <= 5 ) ) {
					hasElem[curr->key - 1] = true;
					++found;
					cout << i << " : " << curr->key << " -> ";
					cout << adjLeft( 1, 1 ) << ( std::round( **curr * 10.0 ) / 10.0 ) << "\n        ";
				} else {
					cout << "ERROR: found invalid key " << curr->key;
					cout << " at index " << i << "\n        ";
					// Quit this:
					result = EXIT_FAILURE; // But we search the rest anyway
				}
				curr = curr->getNext();
			}
		} // End of looping hash table

		cout << "Result : ";
		if ( ( EXIT_SUCCESS == result ) && ( 5 == found )
		                && hasElem[0] && hasElem[1] && hasElem[2]
		                && hasElem[3] && hasElem[4] ) {
			cout << "Success" << endl;
			++env.testSuccess;
		} else {
			cout << "FAIL (" << found << ")" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}

		/***************************************************************************
		** F) Remove elements 2,4 from the second, substract remaining from first **
		**    (2->2.2, 4->4.4)                                                    **
		** Note: This is the second part to E), so it is "in there".              **
		***************************************************************************/
		if ( EXIT_SUCCESS == result ) {
			cout << adjRight ( 4, 0 ) << ++env.testCount << " F) Remove 2 elements from 2nd hash and substract : \n        ";

			ifSecond.delKey( 2 );
			ifSecond.delKey( 4 );

			ifHash -= ifSecond;

			// Now loop again, we should find the just removed elements 2 and 4
			found   = 0;
			memset( hasElem, 0, sizeof( hasElem ) );

			for ( size_t i = 0; i < maxCnt; ++i ) {
				curr = ifHash[i];
				while ( curr ) {
					if ( ( curr->key >= 1 ) && ( curr->key <= 5 ) ) {
						hasElem[curr->key - 1] = true;
						++found;
						cout << i << " : " << curr->key << " -> ";
						cout << adjLeft( 1, 1 ) << ( std::round( **curr * 10.0 ) / 10.0 ) << "\n        ";
					} else {
						cout << "ERROR: found invalid key " << curr->key;
						cout << " at index " << i << "\n        ";
						// Quit this:
						result = EXIT_FAILURE; // But we search the rest anyway
					}
					curr = curr->getNext();
				}
			} // End of looping hash table

			cout << "Result : ";
			if ( ( EXIT_SUCCESS == result ) && ( 2 == found )
			                && !hasElem[0] && hasElem[1] && !hasElem[2]
			                && hasElem[3] && !hasElem[4] ) {
				cout << "Success" << endl;
				++env.testSuccess;
			} else {
				cout << "FAIL (" << found << ")" << endl;
				++env.testFail;
				result = EXIT_FAILURE;
			}
		}
	} // End of outer guard at E)

	/***************************************************************************
	** G) Add 500 random elements                                             **
	***************************************************************************/
	if ( EXIT_SUCCESS == result ) {
		cout << adjRight ( 4, 0 ) << ++env.testCount << " G) Add 500 random elements         : \n        ";

		keydata_t  new_key = keys[4];
		hashval_t* new_val = nullptr;

		// Note: The keys 1 to 5 are already reserved by our array above
		// Further the container sets a 'do_not_destroy' fucntion as the Deleter,
		// which we can not use here, so we have to create the elements ourselves
		for ( size_t i = 5; i < 505; ++i ) {
			new_key += pwx::RNG.random( 1, 10 );
			new_val  = new hashval_t( pwx::RNG.random( 999999.999999 ) );
			elem_t elem(new_key, new_val);
			ifHash.push( elem );
		}

		size_t hash_size = ifHash.size();

		cout << "Result : ";
		if ( ( EXIT_SUCCESS == result ) && ( 502 == hash_size ) ) {
			cout << "Success" << endl;
			++env.testSuccess;
		} else {
			cout << "FAIL (" << hash_size << " / 502)" << endl;
			++env.testFail;
			result = EXIT_FAILURE;
		}
	}

	cout << endl;
	return result;
}

#endif // PWX_LIBPWX_TEST_TESTHASH_H_INCLUDED

