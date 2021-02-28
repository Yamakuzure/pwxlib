/**
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
  *
  * History and change log are maintained in pwxlib.h
**/


#include <PMemUtils>
#include <PLog>
#include <PStringUtils>


static int test_pwx_alloc() {
	char* test_array = pwx_alloc( char, 32 );

	if ( test_array ) {
		pwx_free( test_array );
		return EXIT_SUCCESS;
	}

	log_error( nullptr, "==> %s FAILED", "pwx_alloc()" );

	return EXIT_FAILURE;
}


static int test_pwx_calloc() {
	int result = EXIT_FAILURE;

	char* test_array = pwx_calloc( char, 32 );

	if ( test_array ) {
		result = EXIT_SUCCESS;

		for ( size_t i = 0 ; i < 32 ; ++i ) {
			if ( test_array[i] ) {
				log_error( nullptr, "pwx_calloc() failed, Index %zu is %0x02", i, test_array[i] );
				result = EXIT_FAILURE;
			}
		}
		pwx_free( test_array );
	} else {
		log_error( nullptr, "==> %s FAILED", "pwx_calloc()" );
	}

	return result;
}


static int test_pwx_realloc() {
	int result = EXIT_SUCCESS;

	char* test_array = pwx_alloc( char, 32 );

	if ( test_array ) {
		char* new_array = pwx_realloc( char, test_array, 48 );
		if ( new_array ) {
			test_array = new_array;
		} else {
			log_error( nullptr, "pwx_realloc() failed to increase test_array from %d to %d bytes", 32, 48 );
			result = EXIT_FAILURE;
		}
		pwx_free( test_array );
	} else {
		log_error( nullptr, "%s", "Unable to test pwx_realloc(), as pwx_alloc() failed." );
		result = EXIT_FAILURE;
	}

	return result;
}


static int test_pwx_strdup() {
	int result = EXIT_SUCCESS;

	char const test_array[] = "Test Array";
	char const* new_array = pwx_strdup( test_array );

	if ( nullptr == new_array ) {
		log_error( nullptr, "Failed to pwx_strdup(\"%s\")", test_array );
		return EXIT_FAILURE;
	}

	if ( STRNE( test_array, new_array ) ) {
		log_error( nullptr, "pwx_strdup(\"%s\") failed, copy is: \"%s\"", test_array, new_array );
		return EXIT_FAILURE;
	}

	pwx_free( new_array );

	return result;
}

int main() {
	int result = EXIT_SUCCESS;

	pwx::init( true, nullptr, 0 );

	if ( EXIT_SUCCESS != test_pwx_alloc() ) {
		result = EXIT_FAILURE;
	}

	if ( EXIT_SUCCESS != test_pwx_calloc() ) {
		result = EXIT_FAILURE;
	}

	if ( EXIT_SUCCESS != test_pwx_realloc() ) {
		result = EXIT_FAILURE;
	}

	if ( EXIT_SUCCESS != test_pwx_strdup() ) {
		result = EXIT_FAILURE;
	}

	pwx::finish();

	if ( EXIT_SUCCESS == result ) {
		log_info( nullptr, "%s", "Test successful" );
	} else {
		log_error( nullptr, "%s", "Test FAILED" );
	}

	return result;
}
