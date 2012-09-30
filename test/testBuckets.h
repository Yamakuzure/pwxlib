#pragma once
#ifndef PWX_LIBPWX_TEST_TESTBUCKETS_H_INCLUDED
#define PWX_LIBPWX_TEST_TESTBUCKETS_H_INCLUDED 1

#include "main.h" // This is here for IDE Parsers to find the other stuff

/// Typedefs for the specific buckets:
typedef pwx::TStack<data_t> stack_t;
typedef pwx::TQueue<data_t> queue_t;
typedef pwx::TSet<data_t>   set_t;

/** @brief test a specific bucket type template
  *
  * (Buckets are simple containers using a list as a member, like stacks, queues and sets.)
  *
  ****************************************************************************
  ** === Generic template function to test bucket type containers ===       **
  ** A) Create a container and add 5 integers                               **
  ** B) Pop three integers, must be 1, 2, 3 (Queue, Set) or 5, 4, 3 (Stack) **
  ** C) Shift two integers, must be 5, 4 (Queue, Set) or 1, 2 (Stack)       **
  ** D) Speed test inserting, checking and removing MAXINTCOUNT integers    **
  ****************************************************************************
**/
template<typename list_t>
int32_t testBucket(int32_t& testCount, int32_t& testSuccess, int32_t& testFail)
{
  int32_t result = EXIT_SUCCESS;

  cout << "Test ";
  if (isSameType(list_t, stack_t))
    cout << "stacks" << endl;
  else if (isSameType(list_t, queue_t))
    cout << "queues" << endl;
  else if (isSameType(list_t, set_t))
    cout << "sets" << endl;
  else
    {
      cout << " : nothing - the type is unknown!" << endl;
      return EXIT_FAILURE;
    }
  cout << "----------------------------" << endl;

  list_t intCont(do_not_destroy); // The list

  /** A) Create a container and add 5 integers                               **/
  cout << adjRight(4,0) << ++testCount << " A) Add five integers : ";

  int32_t numbers[5] = { 1, 2, 3, 4, 5 }; // The integers to store

  // Add the first via unshift (doesn't matter with the order)
  uint32_t intCount = intCont.unshift(&numbers[0]);

  // Add the remaining using push
  for (int i = 1; i < 5; ++i)
    intCount = intCont.push(&numbers[i]);

  cout << intCount << " elements added = ";
  if (5 != intCount)
    {
      cout << "FAIL" << endl;
      ++testFail;
      result = EXIT_FAILURE;
    }
  else
    {
      cout << "Success" << endl;
      ++testSuccess;
    }

  /** B) Pop three integers, must be 1, 2, 3 (Queue, Set) or 5, 4, 3 (Stack) **/
  if (EXIT_SUCCESS == result)
    {
      cout << adjRight(4,0) << ++testCount << " B) Pop three times: ";
      auto elemA = intCont.pop();
      auto elemB = intCont.pop();
      auto elemC = intCont.pop();
      cout << **elemA << ", " << **elemB << ", " << **elemC << " - ";
      if ( (isSameType(list_t, stack_t)
            && (5 == **elemA) && (4 == **elemB) && (3 == **elemC) )
        || ( (1 == **elemA) && (2 == **elemB) && (3 == **elemC) ) )
        {
          cout << "Success" << endl;
          ++testSuccess;
        }
      else
        {
          cout << "FAIL" << endl;
          ++testFail;
          result = EXIT_FAILURE;
        }
      delete elemA;
      delete elemB;
      delete elemC;
    } // end of sub test B

  /** B2) Try whether another "4" can be added **/
  if (isSameType(list_t, set_t))
    {
      cout << adjRight(4,0) << ++testCount << " B2) unshift(&4) : ";
      intCont.unshift(&numbers[3]);
      cout << "new size " << intCont.size();
      if (2 == intCont.size())
        {
          cout << " - Success" << endl;
          ++testSuccess;
        }
      else
        {
          cout << " - FAIL" << endl;
          ++testFail;
          result = EXIT_FAILURE;
        }
    }

  /** C) Shift two integers, must be 5, 4 (Queue, Set) or 1, 2 (Stack)       **/
  if (EXIT_SUCCESS == result)
    {
      cout << adjRight(4,0) << ++testCount << " C) Shift two times: ";
      auto elemA = intCont.shift();
      auto elemB = intCont.shift();
      cout << **elemA << ", " << **elemB << " - ";
      if ( (isSameType(list_t, stack_t)
            && (1 == **elemA) && (2 == **elemB) )
        || ( (5 == **elemA) && (4 == **elemB) ) )
        {
          cout << "Success" << endl;
          ++testSuccess;
        }
      else
        {
          cout << "FAIL" << endl;
          ++testFail;
          result = EXIT_FAILURE;
        }
      delete elemA;
      delete elemB;
    } // end of sub test B

/// @todo : Put into unified speed test once RNG is available for random access
//  /** D) Speed test inserting, checking and removing MAXINTCOUNT integers **/
//  if (EXIT_SUCCESS == result)
//    {
//      clock_t startTime = 0;
//      clock_t endTime   = 0;
//
//      cout << adjRight(4,0) << ++testCount << " D) Speed test (" << MAXINTCOUNT << " elements) : "; cout.flush();
//
//      // for this test we need a container that does delete data:
//      list_t spdCont; // The list without the do-not-destroy-destroy()-method
//
//      cout << "push"; cout.flush();
//
//      startTime = clock();
//
//      for (int32_t i = 1; i < (MAXINTCOUNT + 1); ++i)
//        {
//          // Note: As main() does a catch-all, no try{} needed here.
//          int32_t* newInt = new int32_t(i);
//          spdCont.push(newInt);
//        }
//
//      if ((EXIT_SUCCESS == result) && (MAXINTCOUNT != static_cast<int32_t>(spdCont.size())))
//        {
//          cout << " FAILED: spdCont has " << spdCont.size() << " elements" << endl;
//          result = EXIT_FAILURE;
//        }
//
//      if (EXIT_SUCCESS == result)
//        {
//          cout << ", pop"; cout.flush();
//
//          for (int32_t i = 1; i < (MAXINTCOUNT + 1); ++i)
//            {
//              auto elem = spdCont.pop();
//              if ( (isSameType(list_t, stack_t) && ((MAXINTCOUNT + 1 - i) != **elem) )
//                || (i != **elem) )
//                {
//                  cout << "FAIL" << endl;
//                  i = MAXINTCOUNT + 1;
//                  result = EXIT_FAILURE;
//                }
//            }
//        }
//
//      if (EXIT_SUCCESS == result)
//        {
//          if (spdCont.size())
//            {
//              cout << " FAILED: " << spdCont.size() << " items remain" << endl;
//              result = EXIT_FAILURE;
//            }
//          else
//            {
//              endTime = clock();
//              double msNeeded = 1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC));
//              cout << adjRight(5,0) << msNeeded << " ms - Success" << endl;
//            }
//        }
//
//      if (EXIT_SUCCESS == result)
//        ++testSuccess;
//      else
//        ++testFail;
//
//    } // End of sub test E
  cout << endl;

  return result;
}


#endif // PWX_LIBPWX_TEST_TESTBUCKETS_H_INCLUDED

