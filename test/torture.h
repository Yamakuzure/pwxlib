#pragma once
#ifndef PWX_LIBPWX_TEST_TORTURE_H_INCLUDED
#define PWX_LIBPWX_TEST_TORTURE_H_INCLUDED 1

/** @file torture.h
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include <chrono>
#include <iostream>
#include <limits>
#include <thread>

#include <PContainers>
#include <RNG>

using std::cout;
using std::cerr;
using std::endl;

typedef int32_t data_t; // Type to use for the container to torture

const size_t maxIterations = 100; //!< This number of iterations are done by the threads that iterate
const data_t maxValue = std::numeric_limits<data_t>::max();
const data_t minValue = std::numeric_limits<data_t>::lowest();

// --- output lock helper ---
pwx::CLockable outLock; //!< Just lock before c[out|err]ing anything and unlock afterwards.


// Centralize the thread identification output
#define THREAD_IDENT(channel, thrdName) \
	channel << "Thread 0x"; \
	std::hex(channel); \
	channel << thId; \
	std::dec(channel); \
	channel << " \"" << thrdName << "\""

// The threads all share the same header with the default
// methods ctor, dtor, copy ctor and operator=. This little
// macro saves typing and centralizes things.
#define THRD_HEADER(thrdName) \
	private: \
	list_t* cont = nullptr; \
	public: \
	explicit thrd##thrdName() : thrdBase() { } \
	thrd##thrdName(const thrd##thrdName&)				PWX_DELETE; \
	virtual ~thrd##thrdName() noexcept { /* nothing to be done here! */ } \
	thrd##thrdName &operator=(const thrd##thrdName&)	PWX_DELETE


// All threads should print out that they are created, what
// they are, wait for the start and then that they are started.
#define THRD_STARTER(thrdName) \
	size_t thId = CURRENT_THREAD_ID; \
	cont = static_cast<list_t*>(cont_); \
	outLock.lock(); \
	cout << "[Create] "; \
	THREAD_IDENT(cout, thrdName); \
	cout << endl; \
	outLock.unlock(); \
	waitForStart(); \
	std::chrono::milliseconds threadStartWaitTime( 1 ); \
	std::this_thread::sleep_for(threadStartWaitTime); \
	outLock.lock(); \
	cout << "[Start] "; \
	THREAD_IDENT(cout, thrdName); \
	cout << endl; \
	outLock.unlock()


// This little macro is only meant to make the catch blocks in each
// of the thread implementations better readable. No great magic, though...
#define THRD_CATCHER(thrdName) \
	catch (pwx::CException &e) { \
		outLock.lock(); \
		cerr << "[Catch] == "; \
		THREAD_IDENT(cerr, thrdName); \
		cerr << " ==\n"; \
		cerr << "pwx exception \"" << e.name() << "\" caught!" << endl; \
		cerr << "What : \"" << e.what() << "\"" << endl; \
		cerr << "Desc : \"" << e.desc() << "\"" << endl; \
		cerr << "Where: \"" << e.where() << "\"" << endl; \
		cerr << "pFunc: \"" << e.pfunc() << "\"" << endl; \
		cerr << "\nTrace:\n" << e.trace() << "\n-----" << endl; \
		fprintf(stderr, "Caught at %s:%d %s\n", basename(__FILE__), __LINE__, PWX_FUNC); \
		cerr.flush(); \
		outLock.unlock(); \
		isKilled = true; \
	} catch (std::exception &e) { \
		outLock.lock(); \
		cerr << "[Catch] == "; \
		THREAD_IDENT(cerr, thrdName); \
		cerr << " ==\n"; \
		cerr << "std exception caught!" << endl; \
		cerr << "What : \"" << e.what() << "\"" << endl; \
		outLock.unlock(); \
		isKilled = true; \
	} catch (...) { \
		outLock.lock(); \
		cerr << "[Catch] == "; \
		THREAD_IDENT(cerr, thrdName); \
		cerr << " ==\n"; \
		cerr << "Something completely unknown was caught!" << endl; \
		outLock.unlock(); \
		isKilled = true; \
	}

// This macro sets isRunning to false and checks that no locks have been
// left on the container. If remaining locks are found, a message is
// issued and the locks cleared.
#define THREAD_END(thrdName) \
	cout << "[Stop ] "; \
	THREAD_IDENT(cout, thrdName); \
	cout << endl; \
	if (cont->try_lock()) { \
		/* This means we can lock it. Because we *have* a lock still? */ \
		uint32_t lC = cont->lock_count(); \
		if (lC > 1) { \
			outLock.lock(); \
			cerr << "== "; \
			THREAD_IDENT(cerr, thrdName); \
			cerr << " ==\n"; \
			cerr << (lC - 1) << " locks still held on the container!" << endl; \
			outLock.unlock(); \
		} \
		cont->clear_locks(); \
	} \
	isRunning = false

/// @brief enum to determine what is to be tested
enum eTestType {
	E_TEST_ALL    =  1,
	E_TEST_LIST   =  2,
	E_TEST_LIST_D =  3,
	E_TEST_LIST_S =  4,
	E_TEST_QUEUE  =  5,
	E_TEST_RING   =  6,
	E_TEST_RING_D =  7,
	E_TEST_RING_S =  8,
	E_TEST_SET    =  9,
	E_TEST_STACK  = 10,
};


/// @brief enum to determine the thread type to start
enum eThreadType {
	E_THRD_PUSH       =  1, //!< Thread uses push()
	E_THRD_PUSH_FRONT =  2, //!< Thread uses push_front()
	E_THRD_PUSH_BACK  =  3, //!< Thread uses push_back()
	E_THRD_POP        =  4, //!< Thread uses pop()
	E_THRD_POP_FRONT  =  5, //!< Thread uses pop_front()
	E_THRD_POP_BACK   =  6, //!< Thread uses pop_back()
	E_THRD_INSERT     =  7, //!< Thread uses insNextElem() on random indexes
	E_THRD_REMOVE     =  8, //!< Thread uses remNextElem() on random indexes
	E_THRD_DELETE     =  9, //!< Thread uses delNextElem() on random indexes
	E_THRD_GET        = 10, //!< Thread uses get() on random indexes
	E_THRD_GETDATA    = 11, //!< Thread uses getData() on random indexes
	E_THRD_CLEAR      = 12, //!< (*) Special thread, started exactly once to finish with an empty container
	E_THRD_OPADD      = 13, //!< (*) Thread creates a new container and uses operator+=() with the given container
	E_THRD_OPSUB      = 14  //!< (*) Thread clones the container, waits a bit and substracts the container with operator-=()
	                    // (*): These are started exactly once as the last threads
};
int32_t maxThreadType = 11; //!< Largest number from eThreadType to create randomly


// --- Thread class hierarchy ---

/// @brief base class for all threads
struct thrdBase {
	std::atomic_bool isRunning; //!< initialized with false, true while the thread works
	std::atomic_bool isKilled;  //!< if a thread is deleted while it is running, the dtor sets this to true.

	explicit thrdBase() : isRunning( false ), isKilled( false ) { }
	thrdBase( const thrdBase& )				PWX_DELETE;
	virtual ~thrdBase() noexcept;
	thrdBase& operator=( const thrdBase& )	PWX_DELETE;
	void waitForStart() noexcept;

	virtual void operator()( pwx::VContainer* )    PWX_VIRTUAL_PURE;
};

/// @brief thread using clear()
template<typename list_t>
struct thrdClear : public thrdBase {
	THRD_HEADER( Clear );

	/// @brief main execution method
	virtual void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "Clear" );

		// Give other threads a head start of 5 ms
		std::chrono::milliseconds waitTime( 5 );
		std::this_thread::sleep_for( waitTime );

		// Now fire away if this thread hasn't been killed and is still allowed to work:
		while ( cont && isRunning && !isKilled && cont->size() ) {
			PWX_TRY( cont->clear() )
			THRD_CATCHER( "Clear" );
		}

		THREAD_END( "Clear" );
	}
};


/// @brief thread using push()
template<typename list_t>
struct thrdPush : public thrdBase {
	THRD_HEADER( Push );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "Push" );

		if ( cont ) {
			uint32_t valCount = 0;
			for ( size_t i = 0; isRunning && !isKilled && ( i < maxIterations ); ++i ) {
				PWX_TRY( cont->push( new data_t( pwx::RNG.random( minValue, maxValue ) ) ) )
				THRD_CATCHER( "Push" );
				++valCount;
			}

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "Push" );
			cout << " did push() " << valCount << " times.\n";
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		}

		THREAD_END( "Push" );
	}
};


/// @brief thread using push_front()
template<typename list_t>
struct thrdPushFront : public thrdBase {
	THRD_HEADER( PushFront );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "PushFront" );

		if ( cont ) {
			uint32_t valCount = 0;
			for ( size_t i = 0; isRunning && !isKilled && ( i < maxIterations ); ++i ) {
				PWX_TRY( cont->push_front( new data_t( pwx::RNG.random( minValue, maxValue ) ) ) )
				THRD_CATCHER( "PushFront" );
				++valCount;
			}

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "PushFront" );
			cout << " did push_front() " << valCount << " times.\n";
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		}

		THREAD_END( "PushFront" );
	}
};


/// @brief thread using push_back()
template<typename list_t>
struct thrdPushBack : public thrdBase {
	THRD_HEADER( PushBack );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "PushBack" );

		if ( cont ) {
			uint32_t valCount = 0;
			for ( size_t i = 0; isRunning && !isKilled && ( i < maxIterations ); ++i ) {
				PWX_TRY( cont->push_back( new data_t( pwx::RNG.random( minValue, maxValue ) ) ) )
				THRD_CATCHER( "PushBack" );
				++valCount;
			}

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "PushBack" );
			cout << " did push_back() " << valCount << " times.\n";
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		}

		THREAD_END( "PushBack" );
	}
};


/// @brief thread using pop()
template<typename list_t>
struct thrdPop : public thrdBase {
	THRD_HEADER( Pop );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "Pop" );

		// Removing threads start with a yield:
		std::this_thread::yield();

		if ( cont ) {
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for ( size_t i = 0
			                 ; isRunning && !isKilled && !cont->empty() && ( i < maxIterations )
			                ; ++i ) {
				try {
					auto* elem = cont->pop();
					if ( elem ) {
						curVal = **elem;
						if ( curVal > maxValFound ) maxValFound = curVal;
						if ( curVal < minValFound ) minValFound = curVal;
						delete elem;
						++valCount;
					}
				}
				THRD_CATCHER( "Pop" );
			}

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "Pop" );
			cout << " did pop() " << valCount << " times.\n";
			if ( valCount ) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		} // End of having a container to use

		THREAD_END( "Pop" );
	}
};


/// @brief thread using pop_front()
template<typename list_t>
struct thrdPopFront : public thrdBase {
	THRD_HEADER( PopFront );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "PopFront" );

		// Removing threads start with a yield:
		std::this_thread::yield();

		if ( cont ) {
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for ( size_t i = 0
			                 ; isRunning && !isKilled && !cont->empty() && ( i < maxIterations )
			                ; ++i ) {
				try {
					auto* elem = cont->pop_front();
					if ( elem ) {
						curVal = **elem;
						if ( curVal > maxValFound ) maxValFound = curVal;
						if ( curVal < minValFound ) minValFound = curVal;
						delete elem;
						++valCount;
					}
				}
				THRD_CATCHER( "PopFront" );
			}

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "PopFront" );
			cout << " did pop_front() " << valCount << " times.\n";
			if ( valCount ) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		} // End of having a container to use

		THREAD_END( "PopFront" );
	}
};


/// @brief thread using pop_back()
template<typename list_t>
struct thrdPopBack : public thrdBase {
	THRD_HEADER( PopBack );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "PopBack" );

		// Removing threads start with a yield:
		std::this_thread::yield();

		if ( cont ) {
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for ( size_t i = 0
			                 ; isRunning && !isKilled && !cont->empty() && ( i < maxIterations )
			                ; ++i ) {
				try {
					auto* elem = cont->pop_back();
					if ( elem ) {
						curVal = **elem;
						if ( curVal > maxValFound ) maxValFound = curVal;
						if ( curVal < minValFound ) minValFound = curVal;
						delete elem;
						++valCount;
					}
				}
				THRD_CATCHER( "PopBack" );
			}

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "PopBack" );
			cout << " did pop_back() " << valCount << " times.\n";
			if ( valCount ) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		} // End of having a container to use

		THREAD_END( "PopBack" );
	}
};


/// @brief thread using insert()
template<typename list_t>
struct thrdInsert : public thrdBase {
	THRD_HEADER( Insert );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "Insert" );

		if ( cont ) {
			uint32_t idx      = 0;
			uint32_t valCount = 0;
			for ( size_t i = 0; isRunning && !isKilled && ( i < maxIterations ); ++i ) {
				idx = pwx::RNG.random( ( uint32_t )0, cont->size() );
				PWX_TRY( cont->insNextElem( cont->get( idx ), new data_t( pwx::RNG.random( minValue, maxValue ) ) ) )
				THRD_CATCHER( "Insert" );
				++valCount;
			}

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "Insert" );
			cout << " did Insert() " << valCount << " times.\n";
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		}

		THREAD_END( "Insert" );
	}
};


/// @brief thread using remove()
template<typename list_t>
struct thrdRemove : public thrdBase {
	THRD_HEADER( Remove );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "Remove" );

		// Removing threads start with a yield:
		std::this_thread::yield();

		if ( cont ) {
			uint32_t idx       = 0;
			uint32_t cnt       = 0;
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for ( size_t i = 0; isRunning && !isKilled && ( i < maxIterations ); ++i ) {
				cnt = cont->size();
				if ( cnt ) {
					idx = pwx::RNG.random( ( uint32_t )0, cnt - 1 );
					try {
						auto* curElem = cont->remNextElem( cont->get( idx ) );
						if ( curElem ) {
							curVal = **curElem;
							if ( curVal > maxValFound ) maxValFound = curVal;
							if ( curVal < minValFound ) minValFound = curVal;
							++valCount;
							delete curElem;
						}
					}
					THRD_CATCHER( "Remove" );
				} else
					std::this_thread::yield();
			} // End of iteration loop

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "Remove" );
			cout << " did remNextElem() " << valCount << " times.\n";
			if ( valCount ) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();

		} // End of having a container

		THREAD_END( "Remove" );
	}
};


/// @brief thread using delete()
template<typename list_t>
struct thrdDelete : public thrdBase {
	THRD_HEADER( Delete );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "Delete" );

		// Removing threads start with a yield:
		std::this_thread::yield();

		if ( cont ) {
			uint32_t idx      = 0;
			uint32_t cnt      = 0;
			uint32_t valCount = 0;
			for ( size_t i = 0; isRunning && !isKilled && ( i < maxIterations ); ++i ) {
				cnt = cont->size();
				if ( cnt ) {
					idx = pwx::RNG.random( ( uint32_t )0, cnt - 1 );
					PWX_TRY( cont->delNextElem( cont->get( idx ) ) )
					THRD_CATCHER( "Delete" );
					++valCount;
				} else
					std::this_thread::yield();
			}

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "Delete" );
			cout << " did delete() " << valCount << " times.\n";
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		}

		THREAD_END( "Delete" );
	}
};


/// @brief thread using get()
template<typename list_t>
struct thrdGet : public thrdBase {
	THRD_HEADER( Get );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "Get" );

		if ( cont ) {
			uint32_t idx       = 0;
			uint32_t cnt       = 0;
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for ( size_t i = 0; isRunning && !isKilled && ( i < maxIterations ); ++i ) {
				cnt = cont->size();
				if ( cnt ) {
					idx = pwx::RNG.random( ( uint32_t )0, cnt );
					try {
						auto* curElem = cont->get( idx );
						if ( curElem ) {
							curVal = **curElem;
							if ( curVal > maxValFound ) maxValFound = curVal;
							if ( curVal < minValFound ) minValFound = curVal;
							++valCount;
						}
					}
					THRD_CATCHER( "Get" );
				} else
					std::this_thread::yield();
			} // End of iteration loop

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "Get" );
			cout << " did get() " << valCount << " times.\n";
			if ( valCount ) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		} // End of having a container

		THREAD_END( "Get" );
	}
};


/// @brief thread using getData()
template<typename list_t>
struct thrdGetData : public thrdBase {
	THRD_HEADER( GetData );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "GetData" );

		if ( cont ) {
			uint32_t idx       = 0;
			uint32_t cnt       = 0;
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for ( size_t i = 0; isRunning && !isKilled && ( i < maxIterations ); ++i ) {
				cnt = cont->size();
				if ( cnt ) {
					idx = pwx::RNG.random( ( uint32_t )0, cnt );
					try {
						curVal = cont->getData( idx );
						if ( curVal > maxValFound ) maxValFound = curVal;
						if ( curVal < minValFound ) minValFound = curVal;
						++valCount;
					}
					THRD_CATCHER( "GetData" );
				} else
					std::this_thread::yield();
			} // End of iteration loop

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "GetData" );
			cout << " did getData() " << valCount << " times.\n";
			if ( valCount ) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		} // End of having a container


		THREAD_END( "GetData" );
	}
};


/// @brief thread using operator+=()
template<typename list_t>
struct thrdOpAdd : public thrdBase {
	THRD_HEADER( OpAdd );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "OpAdd" );

		// Give other threads a head start of 2 ms
		std::chrono::milliseconds waitTime( 2 );
		std::this_thread::sleep_for( waitTime );

		// Now fire away if this thread hasn't been killed and is still allowed to work:
		if ( cont && isRunning && !isKilled ) {
			list_t newCont;
			PWX_TRY( newCont += *cont )
			THRD_CATCHER( "OpAdd" );

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "OpAdd" );
			cout << " used operator+=  and copied ";
			cout << newCont.size() << " elements." << endl;
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		}

		THREAD_END( "OpAdd" );
	}
};


/// @brief thread using operator-=()
template<typename list_t>
struct thrdOpSub : public thrdBase {
	THRD_HEADER( OpSub );

	/// @brief main execution method
	void operator()( pwx::VContainer* cont_ ) {
		THRD_STARTER( "OpSub" );

		// Give other threads a head start of 1 ms before cloning
		std::chrono::milliseconds waitTime( 2 );
		std::this_thread::sleep_for( waitTime );

		// Now fire away if this thread hasn't been killed and is still allowed to work:
		if ( cont && isRunning && !isKilled ) {
			list_t newCont( *cont );
			uint32_t oldSize = newCont.size();

			// Wait another 2 ms
			std::this_thread::sleep_for( waitTime );

			// Now substract the other
			PWX_TRY( newCont -= *cont )
			THRD_CATCHER( "OpSub" );

			// Get some stats out:
			outLock.lock();
			THREAD_IDENT( cout, "OpSub" );
			cout << " cloned " << oldSize << " elements.\n";
			cout << " -> used operator-= to delete " << ( oldSize - newCont.size() ) << " elements, ";
			cout << " now holding " << newCont.size() << " elements." << endl;
			if ( isKilled )
				cout << "=== The thread has been killed! ===" << endl;
			outLock.unlock();
		}

		THREAD_END( "OpSub" );
	}
};


// --- function templates ---

/// @brief the template that produces the test results
template<typename list_t>
int32_t do_test( uint32_t numThreads ) {
	int32_t       result  = EXIT_SUCCESS;
	thrdBase**    worker  = nullptr;
	std::thread** threads = nullptr;
	list_t        cont;

	// Create the thread pointer array:
	PWX_TRY_STD_FURTHER( worker = new thrdBase*[numThreads], "new_failed", "Unable to create thread pointer array" );

	// Create the random worker but three, as "Clear", "OpAdd" and "OpSub" need to be added once
	for ( size_t i = 0; i < ( numThreads - 3 ); ++i ) {
		eThreadType tType = ( eThreadType )pwx::RNG.random( 1, maxThreadType );
		if      ( E_THRD_PUSH       == tType ) {
			PWX_TRY_STD_FURTHER( worker[i] = new thrdPush<list_t>,      "new_failed", "Couldn't create thrdPush" );
		} else if ( E_THRD_PUSH_FRONT == tType ) {
			PWX_TRY_STD_FURTHER( worker[i] = new thrdPushFront<list_t>, "new_failed", "Couldn't create thrdPushFront" );
		} else if ( E_THRD_PUSH_BACK  == tType ) {
			PWX_TRY_STD_FURTHER( worker[i] = new thrdPushBack<list_t>,  "new_failed", "Couldn't create thrdPushBack" );
		} else if ( E_THRD_POP        == tType ) {
			PWX_TRY_STD_FURTHER( worker[i] = new thrdPop<list_t>,       "new_failed", "Couldn't create thrdPop" );
		} else if ( E_THRD_POP_FRONT  == tType ) {
			PWX_TRY_STD_FURTHER( worker[i] = new thrdPopFront<list_t>,  "new_failed", "Couldn't create thrdPopFront" );
		} else if ( E_THRD_POP_BACK   == tType ) {
			PWX_TRY_STD_FURTHER( worker[i] = new thrdPopBack<list_t>,   "new_failed", "Couldn't create thrdPopBack" );
		} else if ( E_THRD_INSERT     == tType ) {
			PWX_TRY_STD_FURTHER( worker[i] = new thrdInsert<list_t>,    "new_failed", "Couldn't create thrdInsert" );
		} else if ( E_THRD_REMOVE     == tType ) {
			PWX_TRY_STD_FURTHER( worker[i] = new thrdRemove<list_t>,    "new_failed", "Couldn't create thrdRemove" );
		} else if ( E_THRD_DELETE     == tType ) {
			PWX_TRY_STD_FURTHER( worker[i] = new thrdDelete<list_t>,    "new_failed", "Couldn't create thrdDelete" );
		} else if ( E_THRD_GET        == tType ) {
			PWX_TRY_STD_FURTHER( worker[i] = new thrdGet<list_t>,       "new_failed", "Couldn't create thrdGet" );
		} else if ( E_THRD_GETDATA    == tType ) {
			PWX_TRY_STD_FURTHER( worker[i] = new thrdGetData<list_t>,   "new_failed", "Couldn't create thrdGetData" );
		} else {
			cerr << "What the hell?" << endl;
			cerr << "pwx::RNG.random(1, " << maxThreadType << ") just returned " << tType << "!" << endl;
			PWX_THROW( "VroomBang", "RNG.random did something completely wrong.", "Go home, RNG! You are drunk!" );
		}
	} // End of creating random worker

	// The three special worker are still to be created:
	PWX_TRY_STD_FURTHER( worker[numThreads - 3] = new thrdClear<list_t>,   "new_failed", "Couldn't create thrdClear" );
	PWX_TRY_STD_FURTHER( worker[numThreads - 2] = new thrdOpAdd<list_t>,   "new_failed", "Couldn't create thrdOpAdd" );
	PWX_TRY_STD_FURTHER( worker[numThreads - 1] = new thrdOpSub<list_t>,   "new_failed", "Couldn't create thrdOpSub" );

	// Before we can fire away, the container needs to be filled with the
	// first x random values, with x equalling ten times the maximum iterations.
	// Otherwise we run into problems if more threads pull something out than
	// putting in.
	for ( size_t i = 0; i < ( 10 * maxIterations ); ++i ) {
		PWX_TRY_PWXSTD_FURTHER( cont.push( new data_t( pwx::RNG.random( minValue, maxValue ) ) ),
		                        "init_failed",
		                        "Failed to add a random data_t to the container" );
	}

	// Now the threads can be created:
	PWX_TRY_STD_FURTHER( threads = new std::thread*[numThreads], "new_failed", "Couldn't create threads array" );
	for ( size_t i = 0; i < numThreads; ++i ) {
		outLock.lock();
		cout << "Creating thread number " << ( i + 1 ) << endl;
		outLock.unlock();
		PWX_TRY_STD_FURTHER( threads[i] = new std::thread( std::ref( *( worker[i] ) ), &cont ),
		                     "new_failed", "do_test could not call new operator on std::thread" );
	}

	// And GO!
	outLock.lock();
	cout << "Starting threads" << endl;
	outLock.unlock();

	for ( size_t i = 0; i < numThreads; ++i )
		worker[i]->isRunning = true;

	// Now just wait for the threads to finish and be done with it:
	bool isFinished = false;
	while ( !isFinished ) {
		isFinished = true;
		for ( size_t i = 0; isFinished && ( i < numThreads ); ++i ) {
			if ( worker[i]->isRunning )
				isFinished = false;
		}
		if ( isFinished ) {
			for ( size_t i = 0; i < numThreads; ++i ) {
				worker[i]->isKilled = true;
				threads[i]->join();
				delete threads[i];
				threads[i] = nullptr;
				delete worker[i];
			}
		} else
			std::this_thread::yield();
	} // End of joining loop

	// Remove the worker and thread arrays:
	delete [] worker;
	delete [] threads;

	cout << "All threads finished" << endl;

	// Now check the container size, it must not be larger than zero:
	uint32_t contSize = cont.size();
	if ( contSize ) {
		cout.flush();
		cerr << "Something went wrong:\n";
		cerr << " The test container still has " << contSize << " elements!" << endl;
		result = EXIT_FAILURE;
	}

	return result;
}

#endif // PWX_LIBPWX_TEST_TORTURE_H_INCLUDED
