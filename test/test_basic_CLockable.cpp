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
#include <PLog>

#include <cassert>


static int test_lock( PLockable &a, PLockable &b, PLockable &c ) {
	int result = EXIT_SUCCESS;

	PWX_LOCK_OBJ( &a );
	PWX_LOCK( &b );
	bool locked = PWX_TRY_LOCK( &c );

	if ( !a.is_locked() ) {
		log_error( nullptr, "%s FAILED", "PWX_LOCK_OBJ" );
		result = EXIT_FAILURE;
	}
	if ( !b.is_locked() ) {
		log_error( nullptr, "%s FAILED", "PWX_LOCK" );
		result = EXIT_FAILURE;
	}
	if ( !locked || !c.is_locked() ) {
		log_error( nullptr, "%s FAILED", "PWX_TRY_LOCK" );
		result = EXIT_FAILURE;
	}

	if ( 1 != a.lock_count() ) {
		log_error( nullptr, "%s resulted in 'a' having %d/1 locks", "PWX_LOCK_OBJ", a.lock_count() );
		result = EXIT_FAILURE;
	}
	if ( 1 != b.lock_count() ) {
		log_error( nullptr, "%s resulted in 'b' having %d/1 locks", "PWX_LOCK", b.lock_count() );
		result = EXIT_FAILURE;
	}
	if ( 1 != c.lock_count() ) {
		log_error( nullptr, "%s resulted in 'c' having %d/1 locks", "PWX_TRY_LOCK", c.lock_count() );
		result = EXIT_FAILURE;
	}

	return result;
}


static int test_relock( PLockable &a, PLockable &b ) {
	int result = EXIT_SUCCESS;

	PWX_RELOCK_OBJ( &a );
	PWX_RELOCK( &b );

	if ( !a.is_locked() ) {
		log_error( nullptr, "%s FAILED", "PWX_RELOCK_OBJ" );
		result = EXIT_FAILURE;
	}
	if ( !b.is_locked() ) {
		log_error( nullptr, "%s FAILED", "PWX_RELOCK" );
		result = EXIT_FAILURE;
	}

	if ( 1 != a.lock_count() ) {
		log_error( nullptr, "%s resulted in 'a' having %d/1 locks", "PWX_RELOCK_OBJ", a.lock_count() );
		result = EXIT_FAILURE;
	}
	if ( 1 != b.lock_count() ) {
		log_error( nullptr, "%s resulted in 'b' having %d/1 locks", "PWX_RELOCK", b.lock_count() );
		result = EXIT_FAILURE;
	}

	return result;
}


static int test_try_locks( PLockable &a, PLockable &b, PLockable &c ) {
	int result = EXIT_SUCCESS;

	if ( !pwx::try_locks( a, b ) ) {
		log_error( nullptr, "%s FAILED", "try_locks(&a, &b)" );
		result = EXIT_FAILURE;
	}

	// Note, we are recursive and have just added 1 locks to a and b
	if ( 2 != a.lock_count() ) {
		log_error( nullptr, "%s resulted in 'a' having %d/2 locks", "try_locks(a, b)", a.lock_count() );
		result = EXIT_FAILURE;
	}
	if ( 2 != b.lock_count() ) {
		log_error( nullptr, "%s resulted in 'b' having %d/2 locks", "try_locks(a, b)", b.lock_count() );
		result = EXIT_FAILURE;
	}

	// Add another lock to each
	if ( !pwx::try_locks( a, b, c ) ) {
		log_error( nullptr, "%s FAILED", "try_locks(&a, &b, &c)" );
		result = EXIT_FAILURE;
	}

	// Note, we are recursive and have just added 1 locks to a, b and c
	if ( 3 != a.lock_count() ) {
		log_error( nullptr, "%s resulted in 'a' having %d/3 locks", "try_locks(a, b, c)", a.lock_count() );
		result = EXIT_FAILURE;
	}
	if ( 3 != b.lock_count() ) {
		log_error( nullptr, "%s resulted in 'b' having %d/3 locks", "try_locks(a, b, c)", b.lock_count() );
		result = EXIT_FAILURE;
	}
	if ( 2 != c.lock_count() ) {
		log_error( nullptr, "%s resulted in 'c' having %d/2 locks", "try_locks(a, b, c)", c.lock_count() );
		result = EXIT_FAILURE;
	}

	return result;
}


static int test_unlock_all( PLockable &a, PLockable &b, PLockable &c ) {
	int result = EXIT_SUCCESS;

	if ( !pwx::unlock_all( a, b ) ) {
		log_error( nullptr, "%s FAILED", "unlock_all(a, b)" );
		result = EXIT_FAILURE;
	}

	// Note, we are recursive and have just removed 1 lock from a and b
	if ( 2 != a.lock_count() ) {
		log_error( nullptr, "%s resulted in 'a' having %d/2 locks", "unlock_all(a, b)", a.lock_count() );
		result = EXIT_FAILURE;
	}
	if ( 2 != b.lock_count() ) {
		log_error( nullptr, "%s resulted in 'b' having %d/2 locks", "unlock_all(a, b)", b.lock_count() );
		result = EXIT_FAILURE;
	}

	// Reduce each by another one lock
	if ( !pwx::unlock_all( a, b, c ) ) {
		log_error( nullptr, "%s FAILED", "unlock_all(a, b, c)" );
		result = EXIT_FAILURE;
	}

	// Note, we are recursive and have just removed 1 lock from a, b and c
	if ( 1 != a.lock_count() ) {
		log_error( nullptr, "%s resulted in 'a' having %d/1 locks", "unlock_all(a, b, c)", a.lock_count() );
		result = EXIT_FAILURE;
	}
	if ( 1 != b.lock_count() ) {
		log_error( nullptr, "%s resulted in 'b' having %d/1 locks", "unlock_all(a, b, c)", b.lock_count() );
		result = EXIT_FAILURE;
	}
	if ( 1 != c.lock_count() ) {
		log_error( nullptr, "%s resulted in 'c' having %d/1 locks", "unlock_all(a, b, c)", c.lock_count() );
		result = EXIT_FAILURE;
	}

	return result;
}


static int test_clear_locks( PLockable &a, PLockable &b, PLockable &c ) {
	int result = EXIT_SUCCESS;

	if ( !a.clear_locks() ) {
		log_error(nullptr, "%s FAILED!", "a.clear_locks()");
		result = EXIT_FAILURE;
	}

	if (a.is_locked()) {
		log_error(nullptr, "'a' still locked after using %s!", "a.clear_locks()");
		result = EXIT_FAILURE;
	}

	PWX_UNLOCK_OBJ( &b );
	if (b.is_locked()) {
		log_error(nullptr, "'b' still locked after using %s!", "PWX_UNLOCK_OBJ( &b )");
		result = EXIT_FAILURE;
	}

	PWX_UNLOCK( &c );
	if (c.is_locked()) {
		log_error(nullptr, "'c' still locked after using %s!", "PWX_UNLOCK( &c )");
		result = EXIT_FAILURE;
	}

	if ( pwx::are_locked( &a, &b ) ) {
		log_error(nullptr, "%s returned true, must be false!", "pwx::are_locked( &a, &b )");
		result = EXIT_FAILURE;
	}

	if ( pwx::are_locked( b, c ) ) {
		log_error(nullptr, "%s returned true, must be false!", "pwx::are_locked( b, c )");
		result = EXIT_FAILURE;
	}

	if ( pwx::are_locked( &c, &a ) ) {
		log_error(nullptr, "%s returned true, must be false!", "pwx::are_locked( &c, &a )");
		result = EXIT_FAILURE;
	}

	// Note: The utility functions pwx::are_locked() do not use each other, so with these
	//       we have to test both the pointer and the reference variant.

	if ( pwx::are_locked( &a, &b, &c ) ) {
		log_error(nullptr, "%s returned true, must be false!", "pwx::are_locked( &a, &b, &c )");
		result = EXIT_FAILURE;
	}

	if ( pwx::are_locked( a, b, c ) ) {
		log_error(nullptr, "%s returned true, must be false!", "pwx::are_locked( a, b, c )");
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

	// Add 1 lock to a, b and c
	if ( EXIT_SUCCESS != test_lock( lock_a, lock_b, lock_c ) ) {
		result = EXIT_FAILURE;
	}

	// No change in lock count, still 1 for each
	if ( EXIT_SUCCESS != test_relock( lock_a, lock_b ) ) {
		result = EXIT_FAILURE;
	}

	// Two additional locks for a and b, one for c.
	if ( EXIT_SUCCESS != test_try_locks( lock_a, lock_b, lock_c ) ) {
		result = EXIT_FAILURE;
	}

	// Reduce the locks to 1 each
	if ( EXIT_SUCCESS != test_unlock_all( lock_a, lock_b, lock_c ) ) {
		result = EXIT_FAILURE;
	}

	// Fully unlock each
	if ( EXIT_SUCCESS != test_clear_locks( lock_a, lock_b, lock_c ) ) {
		result = EXIT_FAILURE;
	}


	pwx::finish();

	if ( EXIT_SUCCESS == result )
		log_info( nullptr, "%s", "Test successful" );
	else
		log_error( nullptr, "%s", "Test FAILED" );

	return result;
}
