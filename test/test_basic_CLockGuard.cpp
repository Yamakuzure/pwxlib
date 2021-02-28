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


#include <PLockable>
#include <PLockGuard>
#include <PLog>


static int test_lock_guard( PLockable* a, PLockable* b, PLockable* c ) {
	int result = EXIT_SUCCESS;

	// A single lock guard for a
	PWX_NAMED_LOCK_GUARD( Single_Guard, a );

	if ( !a->is_locked() ) {
		log_error( nullptr, "%s has FAILED on 'a'", "Single_Guard" );
		result = EXIT_FAILURE;
	}

	// A double lock guard for b and c
	PWX_NAMED_DOUBLE_LOCK_GUARD( Double_Guard, b, c );

	if ( !pwx::are_locked( b, c ) ) {
		log_error( nullptr, "%s has FAILED on 'b', 'c'", "Double_Guard" );
		result = EXIT_FAILURE;
	}

	// We need three new logs to test the resetting
	PLockable d, e, f;

	// Reset Single Guard to d
	PWX_NAMED_LOCK_GUARD_RESET( Single_Guard, &d );

	if ( a->is_locked() || !d.is_locked() ) {
		log_error( nullptr, "%s has FAILED on 'a' -> 'd'", "Single_Guard Reset" );
		result = EXIT_FAILURE;
	}

	// Reset Double Guard to e, f
	PWX_NAMED_DOUBLE_LOCK_GUARD_RESET( Double_Guard, &e, &f );

	if ( b->is_locked() || c->is_locked() || !e.is_locked() || !f.is_locked() ) {
		log_error( nullptr, "%s has FAILED on 'b','c' -> 'e','f'", "Double_Guard Reset" );
		result = EXIT_FAILURE;
	}

	// Clear the single guard
	PWX_NAMED_LOCK_GUARD_CLEAR( Single_Guard );

	if ( d.is_locked() ) {
		log_error( nullptr, "%s has FAILED on 'd'", "Single_Guard Clear" );
		result = EXIT_FAILURE;
	}

	// Clear the double guard
	PWX_NAMED_DOUBLE_LOCK_GUARD_CLEAR( Double_Guard );

	if ( e.is_locked() || f.is_locked() ) {
		log_error( nullptr, "%s has FAILED on 'e', 'f'", "Double_Guard Clear" );
		result = EXIT_FAILURE;
	}

	// Eventually come up with the triple guard to test
	PWX_NAMED_TRIPLE_LOCK_GUARD( Triple_Guard, a, b, c );

	if ( !pwx::are_locked( a, b, c ) ) {
		log_error( nullptr, "%s has FAILED on 'a', 'b', 'c'", "Triple_Guard" );
		result = EXIT_FAILURE;
	}

	// Move it to d, e, f
	PWX_NAMED_TRIPLE_LOCK_GUARD_RESET( Triple_Guard, &d, &e, &f );

	if ( a->is_locked() || b->is_locked() || c->is_locked() || !d.is_locked() || !e.is_locked() || !f.is_locked() ) {
		log_error( nullptr, "%s has FAILED on 'a','b','c' -> 'd','e','f'", "Triple_Guard Reset" );
		result = EXIT_FAILURE;
	}

	// And clear it again.
	PWX_NAMED_TRIPLE_LOCK_GUARD_CLEAR( Triple_Guard );

	if ( d.is_locked() || e.is_locked() || f.is_locked() ) {
		log_error( nullptr, "%s has FAILED on 'd', 'e', 'f'", "Triple_Guard Clear" );
		result = EXIT_FAILURE;
	}


	return result;
}


int main() {
	int result = EXIT_SUCCESS;

	pwx::init( true, nullptr, 0 );

	PLockable lock_a;
	PLockable lock_b;
	PLockable lock_c;

	if ( !lock_a.is_locking() || !lock_b.is_locking() || !lock_c.is_locking() ) {
		log_error( nullptr, "Setup error: %", "Not all lock objects are locking!" );
		return EXIT_FAILURE; // makes no sense to continue here.
	}

	if ( EXIT_SUCCESS != test_lock_guard( &lock_a, &lock_b, &lock_c ) ) {
		result = EXIT_FAILURE;
	}

	pwx::finish();

	if ( EXIT_SUCCESS == result )
		log_info( nullptr, "%s", "Test successful" );
	else
		log_error( nullptr, "%s", "Test FAILED" );

	return result;
}
