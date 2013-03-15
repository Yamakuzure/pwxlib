#pragma once
#ifndef PWX_LIBPWX_TEST_TORTURE_H_INCLUDED
#define PWX_LIBPWX_TEST_TORTURE_H_INCLUDED 1

#ifndef PWX_THREADS
#  error "This program can not be compiled with PWX_THREADS unset!"
#endif

#include <pwx.h>
#include <iostream>
#include <chrono>
#include <limits>

using std::cout;
using std::cerr;
using std::endl;

typedef int32_t data_t; // Type to use for the container to torture

const size_t maxIterations = 100; //!< This number of iterations are done by the threads that iterate
const data_t maxValue = std::numeric_limits<data_t>::max();
const data_t minValue = std::numeric_limits<data_t>::lowest();

// --- output lock helper ---
pwx::CLockable* outLock; //!< Just lock before c[out|err]ing anything and unlock afterwards.


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
	thrd##thrdName &operator=(const thrd##thrdName&)	PWX_DELETE;


// All threads should print out that they are created, what
// they are, wait for the start and then that they are started.
#define THRD_STARTER(thrdName) \
	auto thId = std::this_thread::get_id(); \
	cont = static_cast<list_t*>(cont_); \
	PWX_LOCK_NOEXCEPT(outLock) \
	cout << "Thread " << thId << " \"" << thrdName << "\" created" << endl; \
	PWX_UNLOCK_NOEXCEPT(outLock) \
	waitForStart(); \
	std::chrono::milliseconds threadStartWaitTime( 1 ); \
	std::this_thread::sleep_for(threadStartWaitTime); \
	PWX_LOCK_NOEXCEPT(outLock) \
	cout << "Thread " << thId << " \"" << thrdName << "\" started" << endl; \
	PWX_UNLOCK_NOEXCEPT(outLock)


// This little macro is only meant to make the catch blocks in each
// of the thread implementations better readable. No great magic, though...
#define THRD_CATCHER(thrdName) \
catch (pwx::CException &e) { \
	PWX_LOCK_NOEXCEPT(outLock) \
	cerr << "== Thread " << thId << " \"" << thrdName << "\" ==\n"; \
	cerr << "pwx exception \"" << e.name() << "\" caught!" << endl; \
	cerr << "What : \"" << e.what() << "\"" << endl; \
	cerr << "What : \"" << e.desc() << "\"" << endl; \
	cerr << "Where: \"" << e.where() << "\"" << endl; \
	cerr << "pFunc: \"" << e.pfunc() << "\"" << endl; \
	cerr << "\nTrace:\n" << e.trace() << "\n-----" << endl; \
	PWX_UNLOCK_NOEXCEPT (outLock) \
	isKilled = true; \
} catch (std::exception &e) { \
	PWX_LOCK_NOEXCEPT(outLock) \
	cerr << "== Thread " << thId << " \"" << thrdName << "\" ==\n"; \
	cerr << "std exception caught!" << endl; \
	cerr << "What : \"" << e.what() << "\"" << endl; \
	PWX_UNLOCK_NOEXCEPT(outLock) \
	isKilled = true; \
} catch (...) { \
	PWX_LOCK_NOEXCEPT(outLock) \
	cerr << "== Thread " << thId << " \"" << thrdName << "\" ==\n"; \
	cerr << "Something completely unknown was caught!" << endl; \
	PWX_UNLOCK_NOEXCEPT(outLock) \
	isKilled = true; \
}


/// @brief enum to determine what is to be tested
enum eTestType
{
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
enum eThreadType
{
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


// --- Prototypes, the implementations are in torture.cpp below main() ---
int32_t setNumThreads(const char* chNum, uint32_t &numThreads);
int32_t setTestType(const char* chType, eTestType &testType);
int32_t startTest(uint32_t numThreads, eTestType testType);

// --- Thread class hierarchy ---

/// @brief base class for all threads
struct thrdBase
{
	std::atomic_bool isRunning; //!< initialized with false, true while the thread works
	std::atomic_bool isKilled;  //!< if a thread is deleted while it is running, the dtor sets this to true.

	explicit thrdBase() : isRunning(false), isKilled(false) { }
	thrdBase(const thrdBase&)				PWX_DELETE;
	virtual ~thrdBase() noexcept;
	thrdBase &operator=(const thrdBase&)	PWX_DELETE;
	void waitForStart() noexcept;

	virtual void operator()(pwx::VContainer*)    PWX_VIRTUAL_PURE;
};

/// @brief thread using clear()
template<typename list_t>
struct thrdClear : public thrdBase
{
	THRD_HEADER(Clear)

	/// @brief main execution method
	virtual void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("Clear")

		// Give other threads a head start of 5 ms
		std::chrono::milliseconds waitTime( 5 );
		std::this_thread::sleep_for(waitTime);

		// Now fire away if this thread hasn't been killed and is still allowed to work:
		if (cont && isRunning && !isKilled) {
			PWX_TRY(cont->clear())
			THRD_CATCHER("Clear")
		}

		// Done!
		isRunning = false;
	}
};


/// @brief thread using push()
template<typename list_t>
struct thrdPush : public thrdBase
{
	THRD_HEADER(Push)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("Push")

		if (cont) {
			uint32_t valCount = 0;
			for (size_t i = 0; isRunning && !isKilled && (i < maxIterations); ++i) {
				PWX_TRY(cont->push(new data_t(pwx::RNG.random(minValue, maxValue))))
				THRD_CATCHER("Push")
				++valCount;
			}

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " did push() " << valCount << " times.\n";
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		}

		// Done!
		isRunning = false;
	}
};


/// @brief thread using push_front()
template<typename list_t>
struct thrdPushFront : public thrdBase
{
	THRD_HEADER(PushFront)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("PushFront")

		if (cont) {
			uint32_t valCount = 0;
			for (size_t i = 0; isRunning && !isKilled && (i < maxIterations); ++i) {
				PWX_TRY(cont->push_front(new data_t(pwx::RNG.random(minValue, maxValue))))
				THRD_CATCHER("PushFront")
				++valCount;
			}

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " did push_front() " << valCount << " times.\n";
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		}

		// Done!
		isRunning = false;
	}
};


/// @brief thread using push_back()
template<typename list_t>
struct thrdPushBack : public thrdBase
{
	THRD_HEADER(PushBack)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("PushBack")

		if (cont) {
			uint32_t valCount = 0;
			for (size_t i = 0; isRunning && !isKilled && (i < maxIterations); ++i) {
				PWX_TRY(cont->push_back(new data_t(pwx::RNG.random(minValue, maxValue))))
				THRD_CATCHER("PushBack")
				++valCount;
			}

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " did push_back() " << valCount << " times.\n";
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		}

		// Done!
		isRunning = false;
	}
};


/// @brief thread using pop()
template<typename list_t>
struct thrdPop : public thrdBase
{
	THRD_HEADER(Pop)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("Pop")

		// Removing threads start with a yield:
		std::this_thread::yield();

		if (cont) {
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for (size_t i = 0
				; isRunning && !isKilled && !cont->empty() && (i < maxIterations)
				; ++i) {
				try {
					auto* elem = cont->pop();
					if (elem) {
						curVal = **elem;
						if (curVal > maxValFound) maxValFound = curVal;
						if (curVal < minValFound) minValFound = curVal;
						delete elem;
						++valCount;
					}
				}
				THRD_CATCHER("Pop")
			}

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " did pop() " << valCount << " times.\n";
			if (valCount) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		} // End of having a container to use

		// Done!
		isRunning = false;
	}
};


/// @brief thread using pop_front()
template<typename list_t>
struct thrdPopFront : public thrdBase
{
	THRD_HEADER(PopFront)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("PopFront")

		// Removing threads start with a yield:
		std::this_thread::yield();

		if (cont) {
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for (size_t i = 0
				; isRunning && !isKilled && !cont->empty() && (i < maxIterations)
				; ++i) {
				try {
					auto* elem = cont->pop_front();
					if (elem) {
						curVal = **elem;
						if (curVal > maxValFound) maxValFound = curVal;
						if (curVal < minValFound) minValFound = curVal;
						delete elem;
						++valCount;
					}
				}
				THRD_CATCHER("PopFront")
			}

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " did pop_front() " << valCount << " times.\n";
			if (valCount) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		} // End of having a container to use

		// Done!
		isRunning = false;
	}
};


/// @brief thread using pop_back()
template<typename list_t>
struct thrdPopBack : public thrdBase
{
	THRD_HEADER(PopBack)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("PopBack")

		// Removing threads start with a yield:
		std::this_thread::yield();

		if (cont) {
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for (size_t i = 0
				; isRunning && !isKilled && !cont->empty() && (i < maxIterations)
				; ++i) {
				try {
					auto* elem = cont->pop_back();
					if (elem) {
						curVal = **elem;
						if (curVal > maxValFound) maxValFound = curVal;
						if (curVal < minValFound) minValFound = curVal;
						delete elem;
						++valCount;
					}
				}
				THRD_CATCHER("PopBack")
			}

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " did pop_back() " << valCount << " times.\n";
			if (valCount) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		} // End of having a container to use

		// Done!
		isRunning = false;
	}
};


/// @brief thread using insert()
template<typename list_t>
struct thrdInsert : public thrdBase
{
	THRD_HEADER(Insert)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("Insert")

		if (cont) {
			uint32_t idx      = 0;
			uint32_t valCount = 0;
			for (size_t i = 0; isRunning && !isKilled && (i < maxIterations); ++i) {
				idx = pwx::RNG.random((uint32_t)0, cont->size());
				PWX_TRY(cont->insNextElem(cont->get(idx), new data_t(pwx::RNG.random(minValue, maxValue))))
				THRD_CATCHER("Insert")
				++valCount;
			}

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " did Insert() " << valCount << " times.\n";
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		}

		// Done!
		isRunning = false;
	}
};


/// @brief thread using remove()
template<typename list_t>
struct thrdRemove : public thrdBase
{
	THRD_HEADER(Remove)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("Remove")

		// Removing threads start with a yield:
		std::this_thread::yield();

		if (cont) {
			uint32_t idx       = 0;
			uint32_t cnt       = 0;
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for (size_t i = 0; isRunning && !isKilled && (i < maxIterations); ++i) {
				cnt = cont->size();
				if (cnt) {
					idx = pwx::RNG.random((uint32_t)0, cnt - 1);
					try {
						auto* curElem = cont->remNextElem(cont->get(idx));
						if (curElem) {
							curVal = **curElem;
							if (curVal > maxValFound) maxValFound = curVal;
							if (curVal < minValFound) minValFound = curVal;
							++valCount;
							delete curElem;
						}
					}
					THRD_CATCHER("Remove")
				} else
					std::this_thread::yield();
			} // End of iteration loop

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " did remNextElem() " << valCount << " times.\n";
			if (valCount) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)

		} // End of having a container

		// Done!
		isRunning = false;
	}
};


/// @brief thread using delete()
template<typename list_t>
struct thrdDelete : public thrdBase
{
	THRD_HEADER(Delete)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("Delete")

		// Removing threads start with a yield:
		std::this_thread::yield();

		if (cont) {
			uint32_t idx      = 0;
			uint32_t cnt      = 0;
			uint32_t valCount = 0;
			for (size_t i = 0; isRunning && !isKilled && (i < maxIterations); ++i) {
				cnt = cont->size();
				if (cnt) {
					idx = pwx::RNG.random((uint32_t)0, cnt - 1);
					PWX_TRY(cont->delNextElem(cont->get(idx)))
					THRD_CATCHER("Delete")
					++valCount;
				} else
					std::this_thread::yield();
			}

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " did delete() " << valCount << " times.\n";
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		}

		// Done!
		isRunning = false;
	}
};


/// @brief thread using get()
template<typename list_t>
struct thrdGet : public thrdBase
{
	THRD_HEADER(Get)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("Get")

		if (cont) {
			uint32_t idx       = 0;
			uint32_t cnt       = 0;
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for (size_t i = 0; isRunning && !isKilled && (i < maxIterations); ++i) {
				cnt = cont->size();
				if (cnt) {
					idx = pwx::RNG.random((uint32_t)0, cnt);
					try {
						auto* curElem = cont->get(idx);
						if (curElem) {
							curVal = **curElem;
							if (curVal > maxValFound) maxValFound = curVal;
							if (curVal < minValFound) minValFound = curVal;
							++valCount;
						}
					}
					THRD_CATCHER("Get")
				} else
					std::this_thread::yield();
			} // End of iteration loop

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " did get() " << valCount << " times.\n";
			if (valCount) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		} // End of having a container

		// Done!
		isRunning = false;
	}
};


/// @brief thread using getData()
template<typename list_t>
struct thrdGetData : public thrdBase
{
	THRD_HEADER(GetData)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("GetData")

		if (cont) {
			uint32_t idx       = 0;
			uint32_t cnt       = 0;
			data_t minValFound = maxValue;
			data_t maxValFound = minValue;
			size_t valCount    = 0;
			data_t curVal      = 0;
			for (size_t i = 0; isRunning && !isKilled && (i < maxIterations); ++i) {
				cnt = cont->size();
				if (cnt) {
					idx = pwx::RNG.random((uint32_t)0, cnt);
					try {
						curVal = cont->getData(idx);
						if (curVal > maxValFound) maxValFound = curVal;
						if (curVal < minValFound) minValFound = curVal;
						++valCount;
					}
					THRD_CATCHER("GetData")
				} else
					std::this_thread::yield();
			} // End of iteration loop

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " did getData() " << valCount << " times.\n";
			if (valCount) {
				cout << " -> minimum value found: " << minValue;
				cout << "\n -> maximum value found: " << maxValue << endl;
			}
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		} // End of having a container


		// Done!
		isRunning = false;
	}
};


/// @brief thread using operator+=()
template<typename list_t>
struct thrdOpAdd : public thrdBase
{
	THRD_HEADER(OpAdd)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("OpAdd")

		// Give other threads a head start of 2 ms
		std::chrono::milliseconds waitTime( 2 );
		std::this_thread::sleep_for(waitTime);

		// Now fire away if this thread hasn't been killed and is still allowed to work:
		if (cont && isRunning && !isKilled) {
			list_t newCont;
			PWX_TRY(newCont += *cont)
			THRD_CATCHER("OpAdd")

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " used operator+=  and copied ";
			cout << newCont.size() << " elements." << endl;
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		}

		// Done!
		isRunning = false;
	}
};


/// @brief thread using operator-=()
template<typename list_t>
struct thrdOpSub : public thrdBase
{
	THRD_HEADER(OpSub)

	/// @brief main execution method
	void operator()(pwx::VContainer* cont_)
	{
		THRD_STARTER("OpSub")

		// Give other threads a head start of 1 ms before cloning
		std::chrono::milliseconds waitTime( 2 );
		std::this_thread::sleep_for(waitTime);

		// Now fire away if this thread hasn't been killed and is still allowed to work:
		if (cont && isRunning && !isKilled) {
			list_t newCont(*cont);
			uint32_t oldSize = newCont.size();

			// Wait another 2 ms
			std::this_thread::sleep_for(waitTime);

			// Now substract the other
			PWX_TRY(newCont -= *cont)
			THRD_CATCHER("OpSub")

			// Get some stats out:
			PWX_LOCK_NOEXCEPT(outLock)
			cout << "Thread " << thId << " cloned " << oldSize << "elements.\n";
			cout << " -> used operator-= to delete " << (oldSize - newCont.size()) << " elements, ";
			cout << " now holding " << newCont.size() << " elements." << endl;
			if (isKilled)
				cout << "=== The thread has been killed! ===" << endl;
			PWX_UNLOCK_NOEXCEPT(outLock)
		}

		// Done!
		isRunning = false;
	}
};


// --- function templates ---

/// @brief the template that produces the test results
template<typename list_t>
int32_t do_test(uint32_t numThreads)
{
	int32_t       result  = EXIT_SUCCESS;
	thrdBase**    worker  = nullptr;
	std::thread** threads = nullptr;
	list_t        cont;

	// Create the thread pointer array:
	PWX_TRY_STD_FURTHER(worker = new thrdBase*[numThreads], "new_failed", "Unable to create thread pointer array")

	// Create the random worker but three, as "Clear", "OpAdd" and "OpSub" need to be added once
	for (size_t i = 0; i < (numThreads - 3); ++i) {
		eThreadType tType = (eThreadType)pwx::RNG.random(1, maxThreadType);
		if      (E_THRD_PUSH       == tType)
			PWX_TRY_STD_FURTHER(worker[i] = new thrdPush<list_t>,      "new_failed", "Couldn't create thrdPush")
		else if (E_THRD_PUSH_FRONT == tType)
			PWX_TRY_STD_FURTHER(worker[i] = new thrdPushFront<list_t>, "new_failed", "Couldn't create thrdPushFront")
		else if (E_THRD_PUSH_BACK  == tType)
			PWX_TRY_STD_FURTHER(worker[i] = new thrdPushBack<list_t>,  "new_failed", "Couldn't create thrdPushBack")
		else if (E_THRD_POP        == tType)
			PWX_TRY_STD_FURTHER(worker[i] = new thrdPop<list_t>,       "new_failed", "Couldn't create thrdPop")
		else if (E_THRD_POP_FRONT  == tType)
			PWX_TRY_STD_FURTHER(worker[i] = new thrdPopFront<list_t>,  "new_failed", "Couldn't create thrdPopFront")
		else if (E_THRD_POP_BACK   == tType)
			PWX_TRY_STD_FURTHER(worker[i] = new thrdPopBack<list_t>,   "new_failed", "Couldn't create thrdPopBack")
		else if (E_THRD_INSERT     == tType)
			PWX_TRY_STD_FURTHER(worker[i] = new thrdInsert<list_t>,    "new_failed", "Couldn't create thrdInsert")
		else if (E_THRD_REMOVE     == tType)
			PWX_TRY_STD_FURTHER(worker[i] = new thrdRemove<list_t>,    "new_failed", "Couldn't create thrdRemove")
		else if (E_THRD_DELETE     == tType)
			PWX_TRY_STD_FURTHER(worker[i] = new thrdDelete<list_t>,    "new_failed", "Couldn't create thrdDelete")
		else if (E_THRD_GET        == tType)
			PWX_TRY_STD_FURTHER(worker[i] = new thrdGet<list_t>,       "new_failed", "Couldn't create thrdGet")
		else if (E_THRD_GETDATA    == tType)
			PWX_TRY_STD_FURTHER(worker[i] = new thrdGetData<list_t>,   "new_failed", "Couldn't create thrdGetData")
		else {
			cerr << "What the hell?" << endl;
			cerr << "pwx::RNG.random(1, " << maxThreadType << ") just returned " << tType << "!" << endl;
			PWX_THROW("VroomBang", "RNG.random did something completely wrong.", "Go home, RNG! You are drunk!")
		}
	} // End of creating random worker

	// The three special worker are still to be created:
	PWX_TRY_STD_FURTHER(worker[numThreads - 3] = new thrdClear<list_t>,   "new_failed", "Couldn't create thrdClear")
	PWX_TRY_STD_FURTHER(worker[numThreads - 2] = new thrdOpAdd<list_t>,   "new_failed", "Couldn't create thrdOpAdd")
	PWX_TRY_STD_FURTHER(worker[numThreads - 1] = new thrdOpSub<list_t>,   "new_failed", "Couldn't create thrdOpSub")

	// Before we can fire away, the container needs to be filled with the
	// first x random values, with x equaling ten times the maximum iterations.
	// Otherwise we run into problems if more threads pull something out than
	// putting in.
	for (size_t i = 0; i < (10 * maxIterations); ++i) {
		PWX_TRY_PWXSTD_FURTHER(cont.push(new data_t(pwx::RNG.random(minValue, maxValue))),
								"init_failed",
								"Failed to add a random data_t to the container")
	}

	// Now the threads can be created:
    PWX_TRY_STD_FURTHER(threads = new std::thread*[numThreads], "new_failed", "Couldn't create threads array")
	for (size_t i = 0; i < numThreads; ++i) {
		PWX_LOCK_NOEXCEPT(outLock)
		cout << "Creating thread number " << (i + 1) << endl;
		PWX_UNLOCK_NOEXCEPT(outLock)
		PWX_TRY_STD_FURTHER(threads[i] = new std::thread(std::ref(*(worker[i])), &cont),
							"new_failed", "do_test could not call new operator on std::thread")
	}

	// And GO!
	PWX_LOCK_NOEXCEPT(outLock)
	cout << "Starting threads" << endl;
	PWX_UNLOCK_NOEXCEPT(outLock)

	for (size_t i = 0; i < numThreads; ++i)
		worker[i]->isRunning = true;

	// Now just wait for the threads to finish and be done with it:
    bool isFinished = false;
    while (!isFinished) {
		isFinished = true;
		for (size_t i = 0; isFinished && (i < numThreads); ++i) {
			if (worker[i]->isRunning)
				isFinished = false;
		}
		if (isFinished) {
			for (size_t i = 0; i < numThreads; ++i) {
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
	if (contSize) {
		cout.flush();
		cerr << "Something went wrong:\n";
		cerr << " The test container still has " << contSize << " elements!" << endl;
		result = EXIT_FAILURE;
	}

	return result;
}

#endif // PWX_LIBPWX_TEST_TORTURE_H_INCLUDED