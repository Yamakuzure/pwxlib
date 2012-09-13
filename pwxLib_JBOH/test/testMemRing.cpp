/** @file testMemRing.cpp
  *
  * @brief console test program for CMemRing.h
  *
  * (c) 2007-2012 Sven Eden, PrydeWorX
  * @author Sven Eden, PrydeWorX - Hamburg, Germany
  *         yamakuzure@users.sourceforge.net
  *         http://pwxlib.sourceforge.net
  *
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * This little program test the CMemRing in (hopefully) every aspect
  *
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.0.1     2010-09-27  sed, PrydeWorX  Initial release
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2.1   2011-04-29  sed, PrydeWorX  Added tests for delItem() methods.
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0.1   2011-08-07  sed, PrydeWorX  Added tests for the new 0.8.0.X functions
  * 0.8.0.2   2011-08-08  sed, PrydeWorX  Added tests for the new TMemRingInterface
  * 0.8.0.3   2011-09-01  sed, PrydeWorX  Changed the sorting of the non-MRF intCont to a sort_once loop for better viewing
  *                                       the work in progress and the result.
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5     2012-03-01  sed, PrydeWorX  Welcome to 2012. Version bump to new pwxLib release version
  * 0.8.5.1   2012-04-13  sed, PrydeWorX  Changed to new pwx_snprintf wrapper and silenced some annoying MSVC++10 warnings
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
  */

#include "pwxLib/internal/MSVCwarnPush.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include "pwxLib/internal/MSVCwarnPop.h"
//#include <pwxLib/CMemRingFactory.h>
#include <pwxLib/tools/MRInterface.h>

using namespace std;
using pwx::MRF;
using pwx::mrf::CMemRingFactory;
using pwx::mrf::TMemRing;
using pwx::mrf::MRInterface;
using pwx::mrf::TItem;
using pwx::StreamHelpers::adjLeft;
using pwx::StreamHelpers::adjRight;
using pwx::StreamHelpers::to_string;

/* we need a dummy class to test with ... */
class CDummy
{
public:
  CDummy(const char * txt): value(txt) { };
  string value;
};

/* ... that supports sorting ... */
bool operator>(CDummy &lhs, CDummy &rhs)
  {
    return (lhs.value > rhs.value);
  }

/* ... and has an operator<< upon ostream to work with save() methods: */
ostream& operator<<(ostream &os, const CDummy &dummy)
{
  os << dummy.value;
  return(os);
}

// Some types to make things easier:

typedef TMemRing<int32_t> intContType;
typedef TMemRing<float>   fltContType;
typedef TMemRing<CDummy>  dmyContType;

// print functions:
template <typename T>
void printContainer (const TMemRing<T> *cont);

int main()
{
  int32_t  result = EXIT_SUCCESS;
  int32_t  i      = 0; // A counter
  CDummy * xDummy = NULL;
  int32_t  errCnt = 0; // How many errors did we detect?
  string   errMsg;

  try
    {
      // Outer try - catch

      /** ========================================================================= **
       ** === Step one: make three containers of different types                === **
       ** ========================================================================= **/

      // One: Create a container for integers, and add a total of five members
      cout << "Step 01:\nUsing MRF.create(), add(), push(), unshift() and MRF.add()\n";
      cout << "to create an integer container and add five items:";
      intContType * intCont = MRF.create(1); // Use auto-id and auto-name
      intCont->add(2);
      intCont->push(3, 666, "The middle integer");
      intCont->unshift(4, 0, "Unsorted first");
      intCont = MRF.add(5, 0, "The MRF added");
      cout << "...done!" << endl;

      // Two: Create a container for floats, and add a total of five members
      cout << "\n  ----- ======================= -----\nStep 02:\nUsing MRF.add(), add(), push(), unshift() and MRF.add()\n";
      cout << "to create a float container and add five items:";
      fltContType * floatCont = MRF.add(1.1f, 0, "Root Floatee"); // Use auto-id and auto-name
      floatCont->add(2.2f);
      floatCont->push(3.3f, 666, "The middle float");
      floatCont->unshift(4.4f, 0, "Unsorted first");
      floatCont = MRF.add(5.5f, 0, "The MRF added");
      cout << "...done!" << endl;

      // Three: Create a container for CDummy instances, and add a total of five members
      cout << "\n  ----- ======================= -----\nStep 03:\nUsing MRF.get(), add(), push(), unshift() insert()\n";
      cout << "to create a CDummy container and add five items:";
      xDummy = new CDummy("Dummy One");
      dmyContType * dummyCont = MRF.get(xDummy); // Get has no item manipulation
      dummyCont->add(CDummy("Dummy Two"));
      dummyCont->push(CDummy("Dummy Three"), 666, "The middle Dummy");
      dummyCont->unshift(CDummy("Dummy Four"), 0, "Unsorted first");
      dummyCont->insert(CDummy("Dummy Five"), -1, 0, "Last by insert()");
      cout << "...done!" << endl;

      /** ========================================================================= **
       ** === Step two: we must have 3 containers and MRF.empty() must be false === **
       ** ========================================================================= **/

      i = MRF.size();
      cout << "\n  ----- ======================= -----\nStep 04:\nCurrent number of containers (should be 3) : " << i << endl;
      if (MRF.empty())
        {
          errMsg += to_string(++errCnt);
          errMsg += ".: We should have three containers, but MRF.empty() returns true!\n";
        }
      if (3 != i)
        {
          errMsg += to_string(++errCnt);
          errMsg += ".: The dispatcher is not working!\n";
        }
      // Check the numbering:
      if (0 != (i = MRF.getNr(intCont)))
        {
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: The integer container should have number 0, but has number %d!\n", ++errCnt, i);
          errMsg += msg;
        }
      if (1 != (i = MRF.getNr(floatCont)))
        {
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: The float container should have number 1, but has number %d!\n", ++errCnt, i);
          errMsg += msg;
        }
      if (2 != (i = MRF.getNr(dummyCont)))
        {
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: The CDummy container should have number 2, but has number %d!\n", ++errCnt, i);
          errMsg += msg;
        }


      /** ========================================================================= **
       ** === Step Three: Set container and item names                          === **
       ** ========================================================================= **/

      // One
      cout << "\n  ----- ======================= -----\nStep 05:\nSetting the name of the containers and root items to:" << endl;
      cout << " (The name of the first float should already be \"Root Floatee\")" << endl;
      cout << "\"<type> Container\" and \"Root <type> Item\":";
      intCont->setName("Integer Container");
      intCont->setItemName(0, "Root Integer Item");
      // Two
      floatCont->setName("Float Container");
      // (Item name already set!)
      // Three
      dummyCont->setName("CDummy Container");
      dummyCont->setItemName(0, "Root CDummy Item");
      cout << "...done!" << endl;

      /** ========================================================================= **
       ** === Step Four: push the containers upon cout                          === **
       ** ========================================================================= **/

      cout << "\n  ----- ======================= -----\nStep 06:\nPrint out all current containers:" << endl;
      printContainer(intCont);
      printContainer(floatCont);
      printContainer(dummyCont);

      /** ========================================================================= **
       ** === Step Five: test sorting methods and push the containers out again === **
       ** ========================================================================= **/

      // One
      bool isSorted = true;
      cout << "\n  ----- ======================= -----\nStep 07:\nSort integer Container ascending by content:";
      intCont->sort(true);
      printContainer(intCont);
      for (i = 0; isSorted && (i < (intCont->size() - 1)) ; ++i)
        {
          if ( *(intCont->getData(i)) > *(intCont->getData(i + 1)) )
            {
              isSorted = false;
              errMsg += to_string(++errCnt);
              errMsg += ".: Sorting the integer container ascending by content failed!\n";
            }
        }

      // Two
      isSorted = true;
      cout << "\n  ----- ======================= -----\nStep 08:\nSort float Container descending by id:";
      floatCont->sortById(false);
      printContainer(floatCont);
      for (i = 0; isSorted && (i < (floatCont->size() - 1)) ; ++i)
        {
          if ( floatCont->getItemId(i) < floatCont->getItemId(i + 1) )
            {
              isSorted = false;
              errMsg += to_string(++errCnt);
              errMsg += ".: Sorting the float container descending by item ids failed!\n";
            }
        }

      // Three
      isSorted = true;
      cout << "\n  ----- ======================= -----\nStep 09:\nSort CDummy Container ascending by name:";
      dummyCont->sortByName(true);
      printContainer(dummyCont);
      for (i = 0; isSorted && (i < (dummyCont->size() - 1)) ; ++i)
        {
          if ( STRCGT(dummyCont->getItemName(i), dummyCont->getItemName(i + 1)) )
            {
              isSorted = false;
              errMsg += to_string(++errCnt);
              errMsg += ".: Sorting the dummy container ascending by item names failed!\n";
            }
        }

      // Four
      isSorted = true;
      cout << "\n  ----- ======================= -----\nStep 10:\nAdd a new item sorted into the dummy container:";
      dummyCont->sort();
      dummyCont->add_sorted(CDummy("Dummy New"));
      printContainer(dummyCont);
      for (i = 0; isSorted && (i < (dummyCont->size() - 1)) ; ++i)
        {
          if ( dummyCont->getData(i)->value > dummyCont->getData(i + 1)->value )
            {
              isSorted = false;
              errMsg += to_string(++errCnt);
              errMsg += ".: Adding sorted failed,\n";
              errMsg += "   -> [";
              errMsg += dummyCont->getData(i)->value;
              errMsg += "] is smaller than [";
              errMsg += dummyCont->getData(i + 1)->value;
              errMsg += "]\n";
            }
        }

      /** ========================================================================= **
       ** === Step Six: Test SetId/Name of MRF on the containers                === **
       ** ========================================================================= **/

      cout << "\n  ----- ======================= -----\nStep 11:\nChange the ids and names of the containers via MRF" << endl;
      cout << "        methods setId() and setName() :" << endl;
      MRF.setId(0, 123u);
      MRF.setId(1, 231u);
      MRF.setId(2, 312u);
      MRF.setName(0, "Cont for Int");
      MRF.setName(1, "Cont for Float");
      MRF.setName(2, "Cont for CDummy");
      // cout the result:
      for (i = 0; i < 2; ++i)
        cout << "Container " << i << ": id " << MRF.getId(i) << " - name \"" << MRF.getName(i) << "\"" << endl;
      // Do the dummycont via MRF:
      cout << "Container " << MRF.getNr(dummyCont) << ": id " << MRF.getId(dummyCont) << " - name \"";
      cout << MRF.getName(dummyCont) << "\"" << endl;
      // Now test with the appropriate MRF methods, as we now know all three numbers/ids/names:
      if (MRF.getId(0) != MRF.getId("Cont for Int"))
        {
          errMsg += to_string(++errCnt);
          errMsg += ".: Setting id/name and/or getting the id via number/name failed:\n";
          errMsg += " --> Name of Container 0: ";
          errMsg += MRF.getName(0);
          errMsg += "\n --> ID of Container 0  : ";
          errMsg += MRF.getId(0);
          errMsg += "\n --> ID by Name returned: ";
          errMsg += MRF.getId("Cont for Int");
        }
      if (STRNE(MRF.getName(1), MRF.getName(231u)))
        {
          errMsg += to_string(++errCnt);
          errMsg += ".: Setting id/name and/or getting the name via number/id failed:\n";
          errMsg += " --> Name of Container 1: ";
          errMsg += MRF.getName(1);
          errMsg += "\n --> ID of Container 1  : ";
          errMsg += MRF.getId(1);
          errMsg += "\n --> Name by ID returned: ";
          errMsg += MRF.getName(231u);
        }
      if (MRF.getNr(312u) != MRF.getNr("Cont for CDummy"))
        {
          errMsg += to_string(++errCnt);
          errMsg += ".: Setting id/name and/or getting the nr via id/name failed:\n";
          errMsg += " --> Nr for Id 312              : ";
          errMsg += MRF.getNr(312u);
          errMsg += "\n --> Nr for \"Cont for CDummy\" : ";
          errMsg += MRF.getNr("Cont for CDummy");
        }


      /** ========================================================================= **
       ** === Step Seven: Test merging and the saving of item copies            === **
       ** ========================================================================= **/

      // One: Clone container
      cout << "\n  ----- ======================= -----\nStep 12: copy two integer items from intCont into a new container" << endl;
      cout << "        with a manually created pointer that is assigned to MRF as" << endl;
      cout << "        owner. The copying will be done with:" << endl;
      cout << "        secIntCont->add(intCont->getData(1), 1001, \"copy item 1\") and" << endl;
      cout << "        secIntCont->insert(intCont->getData(3), -1, 1003, \"copy item 2\")" << endl;
      intContType * secIntCont = new intContType(&MRF);
      secIntCont->add(intCont->getData(1), 1001, "copy item 1");
      secIntCont->insert(intCont->getData(3), -1, 1003, "copy item 2");
      secIntCont->setName("Second int cont");
      printContainer(secIntCont);

      // Two: Merge containers
      cout << "\n  ----- ======================= -----\nStep 13: Now merge the new container back into the integer container" << endl;
      cout << "        with Mrf.merge(intCont, secIntCont) :" << endl;
      MRF.merge(intCont, secIntCont);
      printContainer(intCont);

      /** ========================================================================= **
       ** === Step Eight: Test getData() and getItem()                          === **
       ** ========================================================================= **/
      cout << "\n  ----- ======================= -----\nStep 14: get the data of the two copied items by id and name:" << endl;
      // One:
      i = *(intCont->getData(1001u));
      cout << "Data of intCont item with the id 1001: " << i << endl;
      if (2 != i)
        {
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: copy item 1 should have data 2, but getData(id) %d!\n", ++errCnt, i);
          errMsg += msg;
        }
      // Two:
      i = *(intCont->getData("copy item 2"));
      cout << "Data of intCont item with the name \"copy item 2\": " << i << endl;
      if (4 != i)
        {
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: copy item 1 should have data 2, but getData(name) returns %d!\n", ++errCnt, i);
          errMsg += msg;
        }

      cout << "\n  ----- ======================= -----\nStep 15: get the TItem of the two copied items by id and name:" << endl;
      // Three:
      TItem<int32_t> * xItem = intCont->getItem("copy item 1");
      if (xItem)
        {
          cout << "Item " << xItem->getNr() << " (" << xItem->getId() << ") \"" << xItem->getName() << "\"" << endl;
          if (xItem->getNr() != 5)
            {
              char msg[128] = "";
              pwx_snprintf(msg, 127, "%d.: getData(name) returned item %d instead of copy item 1 (5)!\n", ++errCnt, xItem->getNr());
              errMsg += msg;
            }
        }
      else
        {
          errMsg += to_string(++errCnt);
          errMsg += ".: getItem(\"copy item 1\") did not return the corresponding TItem!\n";
        }

      // Four:
      xItem = intCont->getItem(1003u);
      if (xItem)
        {
          cout << "Item " << xItem->getNr() << " (" << xItem->getId() << ") \"" << xItem->getName() << "\"" << endl;
          if (xItem->getNr() != 6)
            {
              char msg[128] = "";
              pwx_snprintf(msg, 127, "%d.: getData(name) returned item %d instead of copy item 2 (6)!\n", ++errCnt, xItem->getNr());
              errMsg += msg;
            }
        }
      else
        {
          errMsg += to_string(++errCnt);
          errMsg += ".: getItem(1003) did not return the corresponding TItem!\n";
        }
      // Now put it away:
      xItem = NULL;

      /** ========================================================================= **
       ** === Step Nine: Test getData(), getItem() with wrong values            === **
       ** ========================================================================= **/
      cout << "\n  ----- ======================= -----\nStep 16: test getData() and getItem() with wrong values:" << endl;
      // One:
      try
        {
          cout << "getData() with illegal id: ";
          i = *(intCont->getData(12345u));
          cout << "did NOT throw!";
          errMsg += to_string(++errCnt);
          errMsg += ".: getData(illegal id) did NOT throw!)\n";
        }
      catch (pwx::mrf::idNotFound &e)
        {
          cout << "threw. (" << e.name() << ")" << endl;
          cout << " --> what  : [" << e.what() << "]" << endl;
          cout << " --> where : [" << e.where() << "]" << endl;
          cout << " --> desc  : [" << e.desc() << "]" << endl;
        }
      // two:
      try
        {
          cout << "getData() with illegal name: ";
          i = *(intCont->getData("Not a saved name"));
          cout << "did NOT throw!";
          errMsg += to_string(++errCnt);
          errMsg += ".: getData(illegal name) did NOT throw!)\n";
        }
      catch (pwx::mrf::nameNotFound &e)
        {
          cout << "threw. (" << e.name() << ")" << endl;
          cout << " --> what  : [" << e.what() << "]" << endl;
          cout << " --> where : [" << e.where() << "]" << endl;
          cout << " --> desc  : [" << e.desc() << "]" << endl;
        }

      /** ========================================================================= **
       ** === Step Ten: Move last to front, remove copies with shift() / pop()  === **
       ** ========================================================================= **/

      cout << "\n  ----- ======================= -----\nStep 17:\nmove last item to front with move(-1, 0):";
      intCont->move(-1, 0);
      if (intCont->getItemId(0) != 1003)
        {
          cout << " new root wrong, ";
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: move(-1, 0) failed: First item has id %d (should be 1003)\n", ++errCnt, intCont->getItemId(0));
          errMsg += msg;
        }
      else
        cout << " new root ok, ";
      if (intCont->getItemId(-1) != 1001)
        {
          cout << "new last wrong" << endl;
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: move(-1, 0) failed: Last item has id %d (should be 1001)\n", ++errCnt, intCont->getItemId(0));
          errMsg += msg;
        }
      else
        cout << "new last ok" << endl;
      printContainer(intCont);

      cout << "\n  ----- ======================= -----\nStep 18:\nremove first and last item with shift() and pop(): ";
      int32_t * intData = intCont->shift();
      cout << *intData;
      if (4 != *intData)
        {
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: first item should have data 2, but shift() returns %d!\n", ++errCnt, *intData);
          errMsg += msg;
        }
      i = MRF.getRefCount(intData);
      cout << " (refcount " << i << "), ";
      if (!i)
        delete(intData);
      intData = intCont->pop();
      cout << *intData;
      if (2 != *intData)
        {
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: last item should have data 2, but pop() returns %d!\n", ++errCnt, *intData);
          errMsg += msg;
        }
      i = MRF.getRefCount(intData);
      cout << " (refcount " << i << ")" << endl;
      if (!i)
        delete(intData);
      printContainer(intCont);

      /** ========================================================================= **
       ** === Step Eleven: Test operators of TMemRing<>                         === **
       ** ========================================================================= **/
      cout << "\n  ----- ======================= -----\nStep 19:\nCreate a container instance (int) and name it \"Instanced intCont : ";
      intContType newIntCont(NULL);
      newIntCont.setName("Instanced intCont");
      cout << "done" << endl;

      // One:
      cout << "Add value 10001 and new int(20002) with..." << endl;
      cout << " --> newIntCont += 10001 : ";
      newIntCont += 10001;
      cout << "done" << endl << " --> newIntCont += new int32_t(20002) : ";
      newIntCont += new int32_t(20002);
      cout << "done" << endl;
      newIntCont.setItemName(0, "Added by value");
      newIntCont.setItemName(1, "Added by pointer");
      cout << "done" << endl << "Show saved data via operator[] : " << endl;
      cout << "First  data: " << *(newIntCont[0]) << ", name \"" << newIntCont.getItemName(0) << "\"" << endl;
      cout << "Second data: " << *(newIntCont[1]) << ", name \"" << newIntCont.getItemName(1) << "\"" << endl;

      // Two:
      cout << "Add all items from intCont to the new instance via operater+= :";
      newIntCont += *intCont;
      cout << " done" << endl;
      printContainer(intCont);
      printContainer(&newIntCont);

      // Three:
      cout << "\n  ----- ======================= -----\nStep 20:\nAssign intCont to newIntCont via operator= :";
      newIntCont = *intCont;
      cout << " done" << endl;
      printContainer(intCont);
      printContainer(&newIntCont);

      // Four:
      cout << "\n  ----- ======================= -----\nStep 21:\nremove intCont's items 3 from newIntCont via operator -= :";
      newIntCont -= intCont->getData(3);
      cout << " done" << endl;
      printContainer(&newIntCont);

      // Five:
      cout << "\n  ----- ======================= -----\nStep 22:\ncall delItem(1), delItem(666u) and delItem(\"The MRF added\") :";
      newIntCont.delItem(1);
      newIntCont.delItem(666u);
      newIntCont.delItem("The MRF added");
      cout << " done" << endl;
      printContainer(&newIntCont);

      // Five:
      cout << "\n  ----- ======================= -----\nStep 23:\nremove all of intCont's items newIntCont via operator -= :";
      newIntCont -= *intCont;
      cout << " done" << endl;
      printContainer(intCont);
      printContainer(&newIntCont);

      /** ========================================================================= **
       ** === Step Twelve: Test erase(), eraseByData and destroy()              === **
       ** ========================================================================= **/

      cout << "\n  ----- ======================= -----\nStep 24:\nDelete the containers with MRF.erase(), MRF.eraseByData and destroy()" << endl;
      // One:
      cout << "integer container \"" << intCont->getName() << "\" - MRF.erase(intCont) : ";
      i = MRF.erase(intCont);
      if (1 != i)
        {
          cout << " FAIL (see error message below!)" << endl;
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: MRF.erase() on intCont did not return 1, but %d\n", ++errCnt, i);
          errMsg += msg;
        }
      else
        cout << " done." << endl;

      // Two:
      cout << "float container \"" << floatCont->getName() << "\" - MRF.erase(floatCont) : ";
      i = MRF.erase(floatCont);
      if (1 != i)
        {
          cout << " FAIL (see error message below!)" << endl;
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: MRF.erase() on floatCont did not return 1, but %d\n", ++errCnt, i);
          errMsg += msg;
        }
      else
        cout << " done." << endl;

      // Three:
      cout << "CDummy container \"" << dummyCont->getName() << "\" - MRF.eraseByData(xDummy) : ";
      i = MRF.eraseByData(xDummy);
      if (1 != i)
        {
          cout << " FAIL (see error message below!)" << endl;
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: MRF.eraseByData() on a CDummy Pointer did not return 1, but %d\n", ++errCnt, i);
          errMsg += msg;
        }
      else
        cout << " done." << endl;

      /** ========================================================================= **
       ** === Step Thirteen: Build an own factory to test saving and loading    === **
       ** ========================================================================= **/
      cout << "\n  ----- ======================= -----\nStep 25:\nCreate a new Instance of CMemRingFactory via new() operator :";
      CMemRingFactory * localMRF = new CMemRingFactory(false, false);
      cout << " done";
      cout << "\n  ----- ======================= -----\nStep 26:\nAdd a new integer container with 10 random items to localMRF," << endl;
      cout << "        all items get the same id and name, maps are not used :";
      intCont = new TMemRing<int32_t>(localMRF, false, false, "local int cont");
      srand(static_cast<unsigned int>(std::abs(time(NULL))));
      // add one nr on purpose:
      intCont->insert(55, -1, 4711, "noname");
      // now 4 being random
      for (i = 0; i < 4; ++i)
        intCont->insert(rand() / (RAND_MAX / 100), -1, 4711, "noname");
      // And again to have at least one doublet (to test sorting!)
      intCont->insert(55, -1, 4711, "noname");
      // now 4 being random
      for (i = 0; i < 4; ++i)
        intCont->insert(rand() / (RAND_MAX / 100), -1, 4711, "noname");
      cout << " done" << endl;
      printContainer(intCont);
      int32_t unsorted = 0;
      for (int32_t sr = 1 ; (unsorted = intCont->sort_once()) ; ++sr)
        {
          cout << "round " << sr << ", " << unsorted << " items sorted" << endl;
          printContainer(intCont);
        }


      /** ========================================================================= **
       ** === Step Fourteen: Enable mapping and let the maps be built           === **
       ** ========================================================================= **/
      cout << "\n  ----- ======================= -----\nStep 27:\nEnable the maps and have unique ids/names be generated :";
      intCont->useIdMap(true);
      intCont->useNameMap(true);
      cout << " done";
      printContainer(intCont);

      /** ========================================================================= **
       ** === Step Fifteen: Save localMRF in a textfile, clear it and load back === **
       ** ========================================================================= **/
      cout << "\n  ----- ======================= -----\nStep 28:\nSave the container into a temp file :";
      ofstream outFile;
      bool doContinue = false;
      const char * fileName = pwx::StreamHelpers::makeTemp("", "mrfTest_", "", outFile);

      if ( fileName && strlen(fileName))
        {
          // Test file:
          if (outFile.is_open() && outFile.good())
            {
              outFile << *intCont;
              outFile.close();
              cout << " done";
              doContinue = true;
            }
          else
            {
              cout << " (open) FAIL!";
              doContinue = false;
              char msg[128] = "";
              pwx_snprintf(msg, 127, "%d.: open on \"%s\" did not work.\n", ++errCnt, fileName);
              errMsg += msg;
            }
        }
      else
        {
          cout << " (makeTemp) FAIL!";
          doContinue = false;
          char msg[128] = "";
          pwx_snprintf(msg, 127, "%d.: makeTemp returned NULL.\n", ++errCnt);
          errMsg += msg;
        }

      // Two: clear
      if (doContinue)
        {
          cout << "\n  ----- ======================= -----\nStep 29:\nClear localMRF :";
          localMRF->clear();
          if (localMRF->size())
            {
              cout << " FAIL (see error message below!)" << endl;
              char msg[128] = "";
              pwx_snprintf(msg, 127, "%d.: Although localMRF->clear() was used, it reports a size of %d\n", ++errCnt, localMRF->size());
              errMsg += msg;
              doContinue = false;
            }
          else
            cout << " done" << endl;
        }
      // Three: load back
      ifstream inFile;
      if (doContinue)
        {
          cout << "\n  ----- ======================= -----\nStep 30:\nLoad back localMRF from " << fileName << " :";
          inFile.open(fileName);
          if (inFile.is_open() && inFile.good())
            {
              localMRF->load<int32_t>(inFile, false);
              cout << " done" << endl;
            }
          else
            {
              cout << " FAIL!";
              doContinue = false;
            }
          inFile.close();
        }

      intCont = localMRF->find<int32_t>(static_cast<int32_t *>(NULL));
      if (intCont)
        printContainer(intCont);
      else
        cout << "No container to display...";

      if (doContinue)
        {
          // If doContinue is true, we ought to actually have a file:
          cout << "\n  ----- ======================= -----\nStep 31:\nThis is the content of the temp file ";
          cout << fileName << " : " << endl;
          inFile.open(fileName);
          inFile.seekg(0);
          char line[1024] = "";
          while (!inFile.eof())
            {
              inFile.getline(line, 1023);
              cout << line << endl;
            }
          inFile.close();
        }
      // If we have a filename, get rid of the file:
      if (fileName)
        {
#if defined (_MSC_VER)
          _unlink(fileName);
#else
          unlink(fileName);
#endif // Microsoft wants an underscore, lalalalalaaaaa
          free (const_cast<char *>(fileName));
        }

      /** ========================================== **
       ** === Step Sixteen: Test the interface === **
       ** ========================================== **/
      cout << "\n  ----- ======================= -----\nStep 32:\nTest TRInterface with intcont :" << endl;
      if (doContinue)
        {
          MRInterface<int32_t> intInt(intCont);
          cout << "Receive data:\n-------------" << endl;
          cout << "1.: " << intInt.getItemId(1) << "; " << intInt.getItemName(1) << "; " << *(intInt.getData(1)) << endl;
          cout << "3.: " << intInt.getItemId(3) << "; " << intInt.getItemName(3) << "; " << *(intInt.getData(3)) << endl;
          cout << "6.: " << intInt.getItemId(6) << "; " << intInt.getItemName(6) << "; " << *(intInt.getData(6)) << endl;
          cout << "7.: " << intInt.getItemId(7) << "; " << intInt.getItemName(7) << "; " << *(intInt.getData(7)) << endl;
          cout << "9.: " << intInt.getItemId(9) << "; " << intInt.getItemName(9) << "; " << *(intInt.getData(9)) << endl;
          cout << "Change item data:\n-----------------" << endl;
          intInt.setItemId(1, 667);
          intInt.setItemName(2, "New Name");
          intInt.move(3, 5);
          intInt.add_sorted(5, 0, "add sorted");
          intInt.add(5, 0, "add");
          printContainer(intCont);
        }
      else
        cout << " ! Can not test, as loading/saving failed !" << endl;

      cout << "\n  ----- ======================= -----\nStep 33:\nDelete localMRF :";
      delete (localMRF);
      cout << " done";

      /** ============================================================================ **
       ** === Step Seventeen: Do speed tests with enabled/disabled maps and tracking === **
       ** ============================================================================ **/
      int32_t maxInt = 20000;
      cout << "\n  ----- ======================= -----\nStep 34:\nSpeed tests inserting " << maxInt << " integers :" << endl;
      clock_t startTime = 0;
      clock_t endTime   = 0;
      int32_t timeFull, timeNoMap, timeNoTrack, timeNone;

      // intCont is now bound to the already deleted localMRF, get a new one:
      MRF.clear();
      intCont = MRF.create(static_cast<int32_t *>(NULL));

      cout << "Maps enabled,  tracking enabled  :";
      intCont->clear();
      intCont->useIdMap(true);
      intCont->useNameMap(true);
      startTime = clock();
      try
        {
          for (i = 1; i <= maxInt; ++i)
            {
              int32_t * xInt = new int32_t(i);
              intCont->add(xInt);
            }
        }
      catch (::std::bad_alloc &e)
        {
          cout << "ERROR: " << e.what() << endl;
        }
      endTime = clock();
      timeFull = static_cast<int32_t>(
                    pwx_round(1000.0 * (
                                static_cast<double>(endTime - startTime)
                                / static_cast<double>(CLOCKS_PER_SEC))));
      cout << " " << intCont->size() << " done in " << timeFull << " ms" << endl;

      cout << "Maps disabled, tracking enabled  :";
      intCont->clear();
      intCont->useIdMap(false);
      intCont->useNameMap(false);
      startTime = clock();
      try
        {
          for (i = 1; i <= maxInt; ++i)
            {
              int32_t * xInt = new int32_t(i);
              intCont->add(xInt);
            }
        }
      catch (::std::bad_alloc &e)
        {
          cout << "ERROR: " << e.what() << endl;
        }
      endTime = clock();
      timeNoMap = static_cast<int32_t>(
                    pwx_round(1000.0 * (
                                static_cast<double>(endTime - startTime)
                                / static_cast<double>(CLOCKS_PER_SEC))));
      cout << " " << intCont->size() << " done in " << timeNoMap << " ms" << endl;

      cout << "Maps enabled,  tracking disabled :";
      intCont->clear();
      intCont->useIdMap(true);
      intCont->useNameMap(true);
      intCont->disableTracking();
      startTime = clock();
      try
        {
          for (i = 1; i <= maxInt; ++i)
            {
              int32_t * xInt = new int32_t(i);
              intCont->add(xInt);
            }
        }
      catch (::std::bad_alloc &e)
        {
          cout << "ERROR: " << e.what() << endl;
        }
      endTime = clock();
      timeNoTrack = static_cast<int32_t>(
                    pwx_round(1000.0 * (
                                static_cast<double>(endTime - startTime)
                                / static_cast<double>(CLOCKS_PER_SEC))));
      cout << " " << intCont->size() << " done in " << timeNoTrack << " ms" << endl;

      cout << "Maps disabled, tracking disabled :";
      intCont->clear();
      intCont->useIdMap(false);
      intCont->useNameMap(false);
      startTime = clock();
      try
        {
          for (i = 1; i <= maxInt; ++i)
            {
              int32_t * xInt = new int32_t(i);
              intCont->add(xInt);
            }
        }
      catch (::std::bad_alloc &e)
        {
          cout << "ERROR: " << e.what() << endl;
        }
      endTime = clock();
      timeNone = static_cast<int32_t>(
                    pwx_round(1000.0 * (
                                static_cast<double>(endTime - startTime)
                                / static_cast<double>(CLOCKS_PER_SEC))));
      cout << " " << intCont->size() << " done in " << timeNone << " ms" << endl;

    } // End of having right number of containers
  catch (pwx::Exception &e)
    {
      cerr << "\n =============================== " << endl;
      cerr << "Uncaught mrf exception \"" << e.name() << "\" from " << e.where() << endl;
      cerr << "Message    : " << e.what() << endl;
      cerr << "Description: " << e.desc() << endl;
      cerr << "Full Func  : " << e.pfunc() << endl;
      cerr << " ------------------------------- " << endl;
      cerr << "Trace:" << endl;
      cerr << e.trace() << endl;
      cerr << " =============================== " << endl;
      result = EXIT_FAILURE;
    }
  catch (exception &e)
    {
      // Oh no...
      cerr << "\n =============================== " << endl;
      cerr << "Uncaught std exception : \"" << e.what() << "\"" << endl;
      cerr << " =============================== " << endl;
      result = EXIT_FAILURE;
    }
  catch (...)
    {
      cerr << "\n =============================== " << endl;
      cerr << "PANIC! Unknown exception encountered!" << endl;
      cerr << " =============================== " << endl;
      result = EXIT_FAILURE;
    }

  cout << "\n  ----- Tests finished. Result: -----\n" << endl;

  // print out the error messages, if any;
  if (errCnt)
    {
      result = EXIT_FAILURE;
      cout << "\nThe following errors where detected:\n" << errMsg << endl;
    }

  // Get the final state out:
  if (EXIT_SUCCESS != result)
    cerr << "Test failed!\n" << endl;
  else
    cout << "Test succeeded!\n" << endl;
  return (result);
}

// --- implementation of the print function: ---
template<typename T>
void printContainer (const TMemRing<T> *cont)
{
  cout << endl << cont->getName();
  try
    {
      cout << " (Nr " << MRF.getNr(cont) << ", Id " << MRF.getId(cont) << ") :" << endl;
    }
  catch (pwx::mrf::dataNotFound&)
    {
      cout << " ( not managed by MRF )" << endl;
    }
  cout << adjRight(3) << "nr"    << " | ";
  cout << adjRight(4) << "id"    << " | ";
  cout << adjLeft(20) << "name"  << " | ";
  cout << adjLeft(11) << "data"  << " | ";
  cout << adjRight(8) << "refcount" << endl;
  cout << "----+------+----------------------+-------------+---------" << endl;

  for (int32_t i = 0; i < cont->size(); ++i)
    {
      cout << adjRight(3) << i                    << " | ";
      cout << adjRight(4) << cont->getItemId(i)   << " | ";
      cout << adjLeft(20) << cont->getItemName(i) << " | ";
      cout << adjLeft(11) << *(cont->getData(i))  << " | ";
      cout << adjRight(8) << cont->getItemRefCount(i) << endl;
    }
  if (cont->empty())
    cout << "  The container has no items!" << endl;
  cout << "----+------+----------------------+-------------+---------" << endl;
}
