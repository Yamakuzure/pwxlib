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


#include <PLog>
#include <PStringUtils>

#include <cerrno>


static int test_pwx_asprintf() {
	int result = EXIT_SUCCESS;

	char* test_string = nullptr;

	if ( pwx_asprintf( &test_string, "%s", "Hello World" ) ) {
		log_error( nullptr, "%s FAILED (return -1)", "pwx_asprintf()" );
		result = EXIT_FAILURE;
	}

	if ( nullptr == test_string ) {
		log_error( nullptr, "%s FAILED (test_string is nullptr)", "pwx_asprintf()" );
		return EXIT_FAILURE; // No need to check nullptr against "Hello World"
	}

	if ( STRNE( "Hello World", pwx::strnull( test_string ) ) ) {
		log_error( nullptr, "%s FAILED (Should be \"%s\", is \"%s\")",
		           "pwx_asprintf()", "Hello World", pwx::strnull( test_string ) );
		result = EXIT_FAILURE;
	}

	pwx_free( test_string );

	return result;
}


static int test_pwx_basename() {
	int result = EXIT_SUCCESS;

	char const* test_path = "root/dir/file.dat";
	char const* test_file = pwx_basename( test_path );

	if ( nullptr == test_file ) {
		log_error( nullptr, "%s FAILED (test_path is nullptr)", "pwx_basename()" );
		result = EXIT_FAILURE;
	}

	if ( STRNE( "file.dat", pwx::strnull( test_file ) ) ) {
		log_error( nullptr, "%s FAILED (Should be \"%s\", is \"%s\")",
		           "pwx_basename()", "file.dat", pwx::strnull( test_file ) );
		result = EXIT_FAILURE;
	}

	return result;
}


static int test_pwx_dirname() {
	int result = EXIT_SUCCESS;

	char const* test_path = "root/dir/file.dat";
	char const* test_dir  = pwx_dirname( test_path );

	if ( nullptr == test_dir ) {
		log_error( nullptr, "%s FAILED (test_path is nullptr)", "pwx_dirname()" );
		result = EXIT_FAILURE;
	}

	if ( STRNE( "root/dir", pwx::strnull( test_dir ) ) ) {
		log_error( nullptr, "%s FAILED (Should be \"%s\", is \"%s\")",
		           "pwx_dirname()", "root/dir", pwx::strnull( test_dir ) );
		result = EXIT_FAILURE;
	}

	return result;
}


static int test_pwx_strerror() {
	int result = EXIT_SUCCESS;

	char const* error_msg = pwx_strerror( ENOENT );

	if ( nullptr == error_msg ) {
		log_error( nullptr, "%s FAILED (error_msg is nullptr)", "pwx_strerror()" );
		result = EXIT_FAILURE;
	}

	return result;
}


int main() {
	int result = EXIT_SUCCESS;

	pwx::init( true, nullptr, 0 );

	if ( EXIT_SUCCESS != test_pwx_asprintf() ) {
		result = EXIT_FAILURE;
	}

	if ( EXIT_SUCCESS != test_pwx_basename() ) {
		result = EXIT_FAILURE;
	}

	if ( EXIT_SUCCESS != test_pwx_dirname() ) {
		result = EXIT_FAILURE;
	}

	if ( EXIT_SUCCESS != test_pwx_strerror() ) {
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
