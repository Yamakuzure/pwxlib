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


#include <PAllocUtils>
#include <PLog>


static int test_pwx_new() {
	char* test_array = pwx_new( char, 32 );

	if ( test_array ) {
		FREE_PTR( test_array );
		if ( test_array ) {
			log_error(nullptr, "==> %s FAILED", "FREE_PTR()" );
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}

	log_error(nullptr, "==> %s FAILED", "pwx_new()" );

	return EXIT_FAILURE;
}


static int test_pwx_new0() {
	char* test_array = pwx_new0( char, 32 );

	if ( test_array ) {
		FREE_PTR( test_array );
		if ( test_array ) {
			log_error(nullptr, "==> %s FAILED", "FREE_PTR()" );
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}

	log_error(nullptr, "==> %s FAILED", "pwx_new0()" );

	return EXIT_FAILURE;
}


int main() {
	int result = EXIT_SUCCESS;

	pwx::init( true, nullptr, 0 );

	if ( EXIT_SUCCESS != test_pwx_new() )
		result = EXIT_FAILURE;

	if ( EXIT_SUCCESS != test_pwx_new0() )
		result = EXIT_FAILURE;

	pwx::finish();

	if ( EXIT_SUCCESS == result )
		log_info( nullptr, "%s", "Test successful");
	else
		log_error( nullptr, "%s", "Test FAILED");

	return result;
}
