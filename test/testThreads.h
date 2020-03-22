#pragma once
#ifndef PWX_LIBPWX_TEST_TESTTHREADS_H_INCLUDED
#define PWX_LIBPWX_TEST_TESTTHREADS_H_INCLUDED

/** @file testThreads.h
  * (c) 2007 - 2020 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include "test_lib.h" // This is here for IDE Parsers to find the other stuff
#include <PLockable>
#include <PException>


using pwx::abool_t;


/// @brief base class for adding elements to a container
template<typename cont_t, typename key_t, typename value_t>
class thAdderBase {
public:
	typedef thAdderBase<cont_t, key_t, value_t> th_t;

	/* =================================
	 * === constructors / destructor ===
	 * =================================
	 */
	explicit thAdderBase( bool autostart ) : isRunning( autostart ) {}
	thAdderBase() PWX_DELETE; // derivates need to use the default ctor
	virtual ~thAdderBase() {}

	/* =================================
	 * === thread execution operator ===
	 * =================================
	 */
	virtual void operator()( cont_t* cont_, value_t* values, size_t start, key_t part ) {
		cont = cont_;
		while ( !this->isRunning.load( std::memory_order_acquire ) ) {
			std::this_thread::yield();
		}

		if ( cont ) {
			size_t   stop      = start + part;
			hrTime_t startTime = hrClock::now();
			for ( size_t idx = start; idx < stop; ++idx ) {
				PWX_TRY_PWX_FURTHER( this->privAddToCont( idx, &values[idx] ) )
			}
			this->timeMS = duration_cast<milliseconds>( hrClock::now() - startTime ).count();
		}

		uint32_t remaining = 0;
		if ( ( remaining = cont->lock_count() ) ) {
			cerr << "ERROR: " << remaining << " locks upon thAdderBase exit!" << endl;
			cont->clear_locks();
		}

		// Set thread to not running in a loop to be absolutely sure
		// this operator does not exit until isRunning is false.
		while ( this->isRunning.load( std::memory_order_acquire ) )
			this->isRunning.store( false, std::memory_order_release );
	}

	/* ======================
	 * === Public members ===
	 * ======================
	 */
	cont_t* cont      = nullptr;
	abool_t isRunning = ATOMIC_VAR_INIT( false );
	int64_t timeMS    = 0;

private:
	/* ==================================================================
	 * === private method to create and add elements to the container ===
	 * ==================================================================
	 */
	virtual void privAddToCont( key_t nr, value_t* value ) PWX_VIRTUAL_PURE;
};


///@brief simple class to add values to list based containers
/// IMPORTANT: Single threaded calls _MUST_ set autostart on creation !
template<typename cont_t, typename key_t, typename value_t>
class thAdderList : thAdderBase<cont_t, key_t, value_t> {
public:
	typedef thAdderBase<cont_t, key_t, value_t> base_t;

	/* =================================
	 * === constructors / destructor ===
	 * =================================
	 */
	explicit thAdderList( bool autostart ) : base_t( autostart ) {}
	thAdderList()               : base_t( false )     {}
	virtual ~thAdderList() {}

	/* =================================
	 * === thread execution operator ===
	 * =================================
	 */
	using base_t::operator();

	/* ======================
	 * === Public members ===
	 * ======================
	 */
	using base_t::cont;
	using base_t::isRunning;
	using base_t::timeMS;

private:
	/* ==================================================================
	 * === private method to create and add elements to the container ===
	 * ==================================================================
	 */
	virtual void privAddToCont( key_t, value_t* value ) {
		PWX_TRY_PWXSTD_FURTHER( cont->push( value ),
		                        "Element creation failed",
		                        "Creating and pushing an element to the list based container threw an exception" )
	}
};

///@brief simple class to sort values into list based containers
/// IMPORTANT: Single threaded calls _MUST_ set autostart on creation !
template<typename cont_t, typename key_t, typename value_t>
class thAdderSorted : thAdderBase<cont_t, key_t, value_t> {
public:
	typedef thAdderBase<cont_t, key_t, value_t> base_t;

	/* =================================
	 * === constructors / destructor ===
	 * =================================
	 */
	explicit thAdderSorted( bool autostart ) : base_t( autostart ) {}
	thAdderSorted()               : base_t( false )     {}
	virtual ~thAdderSorted() {}

	/* =================================
	 * === thread execution operator ===
	 * =================================
	 */
	using base_t::operator();

	/* ======================
	 * === Public members ===
	 * ======================
	 */
	using base_t::cont;
	using base_t::isRunning;
	using base_t::timeMS;

private:
	/* ==================================================================
	 * === private method to create and add elements to the container ===
	 * ==================================================================
	 */
	virtual void privAddToCont( key_t, value_t* value ) {
		PWX_TRY_PWXSTD_FURTHER( cont->insert_sorted( value ),
		                        "Element creation failed",
		                        "Creating and pushing an element to the list based container threw an exception" )
	}
};

///@brief simple class to add values to hash table containers
/// IMPORTANT: Single threaded calls _MUST_ set autostart on creation !
template<typename cont_t, typename key_t, typename value_t>
class thAdderHash : thAdderBase<cont_t, key_t, value_t> {
public:
	typedef thAdderBase<cont_t, key_t, value_t> base_t;

	/* =================================
	 * === constructors / destructor ===
	 * =================================
	 */
	explicit thAdderHash( bool autostart ) : base_t( autostart ) {}
	thAdderHash()               : base_t( false )     {}
	virtual ~thAdderHash() {}

	/* =================================
	 * === thread execution operator ===
	 * =================================
	 */
	using base_t::operator();

	/* ======================
	 * === Public members ===
	 * ======================
	 */
	using base_t::cont;
	using base_t::isRunning;
	using base_t::timeMS;

private:
	/* ==================================================================
	 * === private method to create and add elements to the container ===
	 * ==================================================================
	 */
	virtual void privAddToCont( key_t nr, value_t* value ) {
		PWX_TRY_PWXSTD_FURTHER( cont->push( *value, &nr ),
		                        "Element creation failed",
		                        "Creating and pushing an element to the hash table container threw an exception" )
	}
};


/// @brief simple class for searching values in a container
template<typename cont_t, typename key_t, typename value_t>
class thSearcherBase {
public:
	typedef thSearcherBase<cont_t, key_t, value_t> base_t;

	/* =================================
	 * === constructors / destructor ===
	 * =================================
	 */
	explicit thSearcherBase( bool autostart ) : isRunning( autostart ) {}
	thSearcherBase() PWX_DELETE;
	virtual ~thSearcherBase() {}

	/* =================================
	 * === thread execution operator ===
	 * =================================
	 */
	virtual void operator()( cont_t* cont_, value_t* retrieves, size_t start, key_t part ) {
		cont = cont_;
		while ( !this->isRunning.load( std::memory_order_acquire ) ) { }

		if ( cont ) {
			size_t   stop      = start + part;
			hrTime_t startTime = hrClock::now();
			key_t    found     = 0;
			for ( size_t idx = start; idx < stop; ++idx ) {
				if ( privFindInCont( &retrieves[idx] ) )
					++found;
			}
			this->timeMS = duration_cast<milliseconds>( hrClock::now() - startTime ).count();
			if ( found != part )
				cerr << "ERROR: " << ( part - found ) << " values not found in the container!" << endl;
		}

		uint32_t remaining = 0;
		if ( ( remaining = cont->lock_count() ) ) {
			cerr << "ERROR: " << remaining << " locks upon thSearcher exit!" << endl;
			cont->clear_locks();
		}

		// Set thread to not running in a loop to be absolutely sure
		// this operator does not exit until isRunning is false.
		while ( this->isRunning.load( std::memory_order_acquire ) )
			this->isRunning.store( false, std::memory_order_release );
	}

	/* ======================
	 * === Public members ===
	 * ======================
	 */
	cont_t* cont      = nullptr;
	abool_t isRunning = ATOMIC_VAR_INIT( false );
	int64_t timeMS    = 0;


private:
	/* =====================================================================
	 * === private method to search for an element or key in a container ===
	 * =====================================================================
	 */
	virtual bool privFindInCont( value_t* value ) PWX_VIRTUAL_PURE;
};


///@brief simple class to search values in list based containers
/// IMPORTANT: Single threaded calls _MUST_ set autostart on creation !
template<typename cont_t, typename key_t, typename value_t>
class thSearcherList : thSearcherBase<cont_t, key_t, value_t> {
public:
	typedef thSearcherBase<cont_t, key_t, value_t> base_t;

	/* =================================
	 * === constructors / destructor ===
	 * =================================
	 */
	explicit thSearcherList( bool autostart ) : base_t( autostart ) {}
	thSearcherList()               : base_t( false )     {}
	virtual ~thSearcherList() {}

	/* =================================
	 * === thread execution operator ===
	 * =================================
	 */
	using base_t::operator();

	/* ======================
	 * === Public members ===
	 * ======================
	 */
	using base_t::cont;
	using base_t::isRunning;
	using base_t::timeMS;

private:
	/* ========================================================
	 * === private method to find elements in the container ===
	 * ========================================================
	 */
	virtual bool privFindInCont( value_t* value ) {
		value_t* xVal = value;
		return cont->find( *xVal );
	}
};

///@brief simple class to find values in hash table containers
/// IMPORTANT: Single threaded calls _MUST_ set autostart on creation !
template<typename cont_t, typename key_t, typename value_t>
class thSearcherHash : thSearcherBase<cont_t, key_t, value_t> {
public:
	typedef thSearcherBase<cont_t, key_t, value_t> base_t;

	/* =================================
	 * === constructors / destructor ===
	 * =================================
	 */
	explicit thSearcherHash( bool autostart ) : base_t( autostart ) {}
	thSearcherHash()               : base_t( false )     {}
	virtual ~thSearcherHash() {}

	/* =================================
	 * === thread execution operator ===
	 * =================================
	 */
	using base_t::operator();

	/* ======================
	 * === Public members ===
	 * ======================
	 */
	using base_t::cont;
	using base_t::isRunning;
	using base_t::timeMS;

private:
	/* ========================================================
	 * === private method to find elements in the container ===
	 * ========================================================
	 */
	virtual bool privFindInCont( value_t* value ) {
		return cont->get( *value );
	}
};


/// @brief struct doing synchronized start/stop for clearing containers
/// IMPORTANT: Single threaded calls _MUST_ set autostart on creation !
template<typename cont_t>
struct thClearer {
public:
	typedef thClearer<cont_t> th_t;

	/* =================================
	 * === constructors / destructor ===
	 * =================================
	 */
	explicit thClearer( bool autostart ) : isRunning( autostart ) {}
	thClearer()               : th_t( false )          {}
	virtual ~thClearer() {}

	/* =================================
	 * === thread execution operator ===
	 * =================================
	 */
	virtual void operator()( cont_t* cont_ ) {
		cont = cont_;
		while ( !this->isRunning.load( std::memory_order_acquire ) ) { }

		if ( cont ) {
			hrTime_t startTime = hrClock::now();
			cont->clear();
			this->timeMS = duration_cast<milliseconds>( hrClock::now() - startTime ).count();
		}

		uint32_t remaining = 0;
		if ( ( remaining = cont->lock_count() ) ) {
			cerr << "ERROR: " << remaining << " locks upon thClearer exit!" << endl;
			cont->clear_locks();
		}

		// Set thread to not running in a loop to be absolutely sure
		// this operator does not exit until isRunning is false.
		while ( this->isRunning.load( std::memory_order_acquire ) )
			this->isRunning.store( false, std::memory_order_release );
	}

	/* ======================
	 * === Public members ===
	 * ======================
	 */
	std::atomic_bool isRunning = ATOMIC_VAR_INIT( false );
	cont_t*          cont      = nullptr;
	int64_t          timeMS    = 0;
};

#endif // PWX_LIBPWX_TEST_TESTTHREADS_H_INCLUDED

