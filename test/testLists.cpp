#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <pwx.h>
using pwx::adjRight;

#include "destroy.h"
#include "testLists.h"

/// List/Ring element count for the speed tests
const uint32_t MAXINTCOUNT = 2000000;

/** @brief testSingleList
  *
  *************************************************************************
  ** === Test 1 ===                                                      **
  ** A) Create a Singly linked list and add 5 integers                   **
  ** B) Print out elements 1, 3 and 5 which should equal their numbers   **
  ** C) Remove elements 1, 3 and 5, resulting size should be 2           **
  ** D) Print out all remaining elements, which should be 2 and 4        **
  ** E) Speed test inserting, checking and removing MAXINTCOUNT integers **
  *************************************************************************
**/
int testSingleList(int& testCount, int& testSuccess, int& testFail)
{
  int     result    = EXIT_SUCCESS;

  cout << "Test 1 : Singly linked lists" << endl;
  cout << "----------------------------" << endl;

  pwx::TSingleList<int32_t> intList(do_not_destroy); // The list

  /** A) Create a Singly linked list and add 5 integers                 **/
  cout << " A) Add five integers : ";
  ++testCount;

  int32_t numbers[5] = { 1, 2, 3, 4, 5 }; // The integers to store
  // Add the first using ins_next:
  intList.insNext(nullptr, &numbers[0]);
  // Add the next three using ins_next_elem
  for (int i = 1; i < 4; ++i)
    intList.insNextElem(intList[-1], &numbers[i]);
  // Add the last after tail
  uint32_t intCount = intList.insNext(&numbers[3], &numbers[4]);

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

  /** B) Print out elements 1, 3 and 5 which should equal their numbers **/
  if (EXIT_SUCCESS == result)
    {
      cout << " B) Print out element 1, 3, 5: ";
      ++testCount;
      int32_t intOne   = *intList[0]->data;
      int32_t intThree = *(*intList[2]).data;
      int32_t intFive  = **intList[4];
      cout << intOne << ", " << intThree << ", " << intFive << " = ";
      if ((1 != intOne) || (3 != intThree) || (5 != intFive))
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
    } // end of sub test B

  /** C) Remove elements 1, 3 and 5, resulting size should be 2         **/
  if (EXIT_SUCCESS == result)
    {
      cout << " C) Remove elements 1, 3 and 5: ";
      ++testCount;
      intList.delNext(nullptr); // remove 1, which is head
      intList.delNextElem(intList.find(&numbers[1])); // remove 3 via pointer search for "2"
      intList.delNext(&numbers[3]); // remove 5 via direct pointer to "4"
      intCount = intList.size();
      if (2 != intCount)
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
    } // end of sub test C

  /** D) Print out all remaining elements, which should be 2 and 4       **/
  if (EXIT_SUCCESS == result)
    {
      cout << " D) Remaining elements: ";
      ++testCount;
      int32_t intOne  = **intList[0];
      int32_t intTwo  = **intList[1];
      cout << intOne << ", " << intTwo << " = ";
      if ((2 != intOne) || (4 != intTwo))
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
    } // End of sub test D

  /** E) Speed test inserting, checking and removing MAXINTCOUNT integers **/
  if (EXIT_SUCCESS == result)
    {
      clock_t startTime = 0;
      clock_t endTime   = 0;

      ++testCount;
      cout << " E) Speed test (" << MAXINTCOUNT << " elements) : "; cout.flush();

      // for this test we need a container that does delete data:
      pwx::TSingleList<uint32_t> uintList; // The list without the do-not-destroy-destroy()-method

      cout << "ins"; cout.flush();

      startTime = clock();

      for (uint32_t i = 1; i < (MAXINTCOUNT + 1); ++i)
        {
          // Note: As main() does a catch-all, no try{} needed here.
          uint32_t* newInt = new uint32_t(i);
          uintList.insNextElem(uintList[-1], newInt);
        }

      cout << ", chk"; cout.flush();

      for (uint32_t i = 1; i < (MAXINTCOUNT + 1); ++i)
        {
          if (**uintList[i-1] != i)
            {
              cout << " FAILED: " << **uintList[i] << " != " << i << endl;
              result = EXIT_FAILURE;
              i = MAXINTCOUNT + 1;
            }
        }

      if ((EXIT_SUCCESS == result) && (MAXINTCOUNT != uintList.size()))
        {
          cout << " FAILED: uintList has " << uintList.size() << " elements" << endl;
          result = EXIT_FAILURE;
        }

      if (EXIT_SUCCESS == result)
        {
          cout << ", clr"; cout.flush();
          uintList.clear();
          if (uintList.size())
            {
              cout << " FAILED: " << uintList.size() << " items remain" << endl;
              result = EXIT_FAILURE;
            }
          else
            {
              endTime = clock();
              double msNeeded = 1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC));
              cout << adjRight(5,0) << msNeeded << " ms - Success" << endl;
            }
        }

      if (EXIT_SUCCESS == result)
        ++testSuccess;
      else
        ++testFail;

      cout << endl;
    } // End of sub test E

  return result;
}

/** @brief testDoubleList
  *
  *************************************************************************
  ** === Test 2 ===                                                      **
  ** A) Create a Doubly linked list and add 5 integers                   **
  ** B) Print out elements 1, 3 and 5 which should equal their numbers   **
  ** C) Remove elements 1, 3 and 5, resulting size should be 2           **
  ** D) Print out all remaining elements, which should be 2 and 4        **
  ** E) Speed test inserting, checking and removing MAXINTCOUNT integers **
  *************************************************************************
  */
int testDoubleList(int& testCount, int& testSuccess, int& testFail)
{
  int result = EXIT_SUCCESS;

  cout << "Test 2 : Doubly linked lists" << endl;
  cout << "----------------------------" << endl;

  pwx::TDoubleList<int32_t> intList(do_not_destroy); // The list

  /** A) Create a Singly linked list and add 5 integers                 **/
  cout << " A) Add five integers : ";
  ++testCount;

  int32_t numbers[5] = { 1, 2, 3, 4, 5 }; // The integers to store
  // Add the first using ins_next:
  intList.insNext(nullptr, &numbers[0]);
  // Add the next three using ins_next_elem
  for (int i = 1; i < 4; ++i)
    intList.insNextElem(intList[-1], &numbers[i]);
  // Add the last after tail
  uint32_t intCount = intList.insNext(&numbers[3], &numbers[4]);

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

  /** B) Print out elements 1, 3 and 5 which should equal their numbers **/
  if (EXIT_SUCCESS == result)
    {
      cout << " B) Print out element 1, 3, 5: ";
      ++testCount;
      int32_t intOne   = *intList[0]->data;
      int32_t intThree = *(*intList[2]).data;
      int32_t intFive  = **intList[4];
      cout << intOne << ", " << intThree << ", " << intFive << " = ";
      if ((1 != intOne) || (3 != intThree) || (5 != intFive))
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
    } // end of sub test B

  /** C) Remove elements 1, 3 and 5, resulting size should be 2         **/
  if (EXIT_SUCCESS == result)
    {
      cout << " C) Remove elements 1, 3 and 5: ";
      ++testCount;
      intList.delNext(nullptr); // remove 1, which is head
      intList.delNextElem(intList.find(&numbers[1])); // remove 3 via pointer search for "2"
      intList.delPrev(nullptr); // remove 5, which is tail
      intCount = intList.size();
      if (2 != intCount)
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
    } // end of sub test C

  /** D) Print out all remaining elements, which should be 2 and 4       **/
  if (EXIT_SUCCESS == result)
    {
      cout << " D) Remaining elements: ";
      ++testCount;
      int32_t intOne  = **intList[0];
      int32_t intTwo  = **intList[1];
      cout << intOne << ", " << intTwo << " = ";
      if ((2 != intOne) || (4 != intTwo))
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
    } // End of sub test D

  /** E) Speed test inserting, checking and removing MAXINTCOUNT integers **/
  if (EXIT_SUCCESS == result)
    {
      clock_t startTime = 0;
      clock_t endTime   = 0;

      ++testCount;
      cout << " E) Speed test (" << MAXINTCOUNT << " elements) : "; cout.flush();

      // for this test we need a container that does delete data:
      pwx::TDoubleList<uint32_t> uintList; // The list without the do-not-destroy-destroy()-method

      cout << "ins"; cout.flush();

      startTime = clock();

      for (uint32_t i = 1; i < (MAXINTCOUNT + 1); ++i)
        {
          uint32_t* newInt = new uint32_t(i);
          uintList.insNextElem(uintList[-1], newInt);
        }

      cout << ", chk"; cout.flush();

      for (uint32_t i = 1; i < (MAXINTCOUNT + 1); ++i)
        {
          if (**uintList[i-1] != i)
            {
              cout << " FAILED: " << **uintList[i] << " != " << i << endl;
              result = EXIT_FAILURE;
              i = MAXINTCOUNT + 1;
            }
        }

      if ((EXIT_SUCCESS == result) && (MAXINTCOUNT != uintList.size()))
        {
          cout << " FAILED: uintList has " << uintList.size() << " elements" << endl;
          result = EXIT_FAILURE;
        }

      if (EXIT_SUCCESS == result)
        {
          cout << ", clr"; cout.flush();
          uintList.clear();
          if (uintList.size())
            {
              cout << " FAILED: " << uintList.size() << " items remain" << endl;
              result = EXIT_FAILURE;
            }
          else
            {
              endTime = clock();
              double msNeeded = 1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC));
              cout << adjRight(5,0) << msNeeded << " ms - Success" << endl;
            }
        }

      if (EXIT_SUCCESS == result)
        ++testSuccess;
      else
        ++testFail;

      cout << endl;
    } // End of sub test E


  return result;
}

/** @brief testStack
  *
  * Only a speed test is done, the stack is based on the single list
  * which is already tested now.
  */
int testStack(int& testCount, int& testSuccess, int& testFail)
{
  int result = EXIT_SUCCESS;

  cout << "Test 3 : Stacks" << endl;
  cout << "---------------" << endl;

  clock_t startTime = 0;
  clock_t endTime   = 0;

  ++testCount;
  cout << "\n Only a speed test is done, as the stack is based\n";
  cout << " on the singly linked list which is already tested.\n\n";
  cout << "Speed test (" << MAXINTCOUNT << " elements) : "; cout.flush();

  pwx::TStack<uint32_t> uintStack;

  cout << "ins"; cout.flush();

  startTime = clock();

  for (uint32_t i = 1; i < (MAXINTCOUNT + 1); ++i)
    {
      uint32_t* newInt = new uint32_t(i);
      uintStack.push(newInt);
    }

  cout << ", chk"; cout.flush();

  // Stacks are FiLo, so the order is reversed.
  for (uint32_t i = MAXINTCOUNT; i > 0; --i)
    {
      auto xUint = uintStack.pop();
      if (**xUint != i)
        {
          cout << " FAILED: " << **xUint << " != " << i << endl;
          result = EXIT_FAILURE;
          i = MAXINTCOUNT + 1;
        }
      delete xUint;
    }

  if (EXIT_SUCCESS == result)
    {
      if (uintStack.size())
        {
          cout << " FAILED: " << uintStack.size() << " items remain" << endl;
          result = EXIT_FAILURE;
        }
      else
        {
          endTime = clock();
          double msNeeded = 1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC));
          cout << adjRight(5,0) << msNeeded << " ms - Success" << endl;
        }
    }

  if (EXIT_SUCCESS == result)
    ++testSuccess;
  else
    ++testFail;

  cout << endl;

  return result;
}

/** @brief testQueue
  *
  * Only a speed test is done, the queue is based on the double list
  * which is already tested now.
  */
int testQueue(int& testCount, int& testSuccess, int& testFail)
{
  int result = EXIT_SUCCESS;

  cout << "Test 4 : Queues" << endl;
  cout << "---------------" << endl;

  clock_t startTime = 0;
  clock_t endTime   = 0;

  ++testCount;
  cout << "\n Only a speed test is done, as the queue is based\n";
  cout << " on the doubly linked list which is already tested.\n\n";
  cout << "Speed test (" << MAXINTCOUNT << " elements) : "; cout.flush();

  pwx::TQueue<uint32_t> uintQueue;

  cout << "ins"; cout.flush();

  startTime = clock();

  for (uint32_t i = 1; i < (MAXINTCOUNT + 1); ++i)
    {
      uint32_t* newInt = new uint32_t(i);
      uintQueue.push(newInt);
    }

  cout << ", chk"; cout.flush();

  for (uint32_t i = 1; i < (MAXINTCOUNT + 1); ++i)
    {
      auto xUint = uintQueue.pop();
      if (**xUint != i)
        {
          cout << " FAILED: " << **xUint << " != " << i << endl;
          result = EXIT_FAILURE;
          i = MAXINTCOUNT + 1;
        }
      delete xUint;
    }

  if (EXIT_SUCCESS == result)
    {
      if (uintQueue.size())
        {
          cout << " FAILED: " << uintQueue.size() << " items remain" << endl;
          result = EXIT_FAILURE;
        }
      else
        {
          endTime = clock();
          double msNeeded = 1000.0 * (static_cast<double>(endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC));
          cout << adjRight(5,0) << msNeeded << " ms - Success" << endl;
        }
    }

  if (EXIT_SUCCESS == result)
    ++testSuccess;
  else
    ++testFail;

  cout << endl;

  return result;

  return result;
}
