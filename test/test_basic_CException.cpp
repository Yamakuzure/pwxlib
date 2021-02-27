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


#include <PException>

#include <exception>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;


static void inner_pwx_exception() {
	PWX_THROW( "Test", "Test Exception", "Exception to test PException" )
}


static void inner_std_exception() {
	throw ( std::runtime_error( "Testing runtime error" ) );
}


static void outer_pwx_exception() {
	PWX_TRY_PWX_FURTHER( inner_pwx_exception() )
}


static void outer_std_exception() {
	PWX_TRY_STD_FURTHER( inner_std_exception(), "Test", "Exception to test std::runtime_error" )
}


static int test_pwx_exception() {
	int result = EXIT_SUCCESS;

	PWX_TRY( outer_pwx_exception() )
	catch ( pwx::CException &e ) {
		cout << " ==\n-----\npwx exception \"" << e.name() << "\" caught!\n"
		     << "What : \"" << e.what() << "\"\n"
		     << "Desc : \"" << e.desc() << "\"\n"
		     << "Where: \"" << e.where() << "\"\n"
		     << "pFunc: \"" << e.pfunc() << "\"\n"
		     << "\nTrace:\n" << e.trace()
		     << "\n-----" << endl;
	} catch ( ... ) {
		cerr << " ==\n-----\nSomething wrong was caught!" << endl;
		result = EXIT_FAILURE;
	}

	return result;
}


static int test_std_exception() {
	int result = EXIT_SUCCESS;

	PWX_TRY( outer_std_exception() )
	catch ( pwx::CException &e ) {
		cout << " ==\n-----\npwx exception \"" << e.name() << "\" caught!\n"
		     << "What : \"" << e.what() << "\"\n"
		     << "Desc : \"" << e.desc() << "\"\n"
		     << "Where: \"" << e.where() << "\"\n"
		     << "pFunc: \"" << e.pfunc() << "\"\n"
		     << "\nTrace:\n" << e.trace()
		     << "\n-----" << endl;
	} catch ( ... ) {
		cerr << " ==\n-----\nSomething wrong was caught!" << endl;
		result = EXIT_FAILURE;
	}

	return result;
}


int main() {
	int result = EXIT_SUCCESS;

	pwx::init( true, nullptr, 0 );

	int r = test_pwx_exception();
	if ( EXIT_SUCCESS != r ) {
		result = EXIT_FAILURE;
	}

	r = test_std_exception();
	if ( EXIT_SUCCESS != r ) {
		result = EXIT_FAILURE;
	}

	pwx::finish();

	return result;
}
