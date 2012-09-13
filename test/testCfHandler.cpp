#include <iostream>
#include <pwxLib/CCnfFileHandler.h>

using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using pwx::CFH;
using namespace pwx::cfh::cfConstants;
using namespace pwx::cfh::cfFlags;

int main()
  {
    int result = EXIT_SUCCESS;

    cout << "Testing Config File Handler\n===================\n" << endl;

    // We need three temporary files first:
    ofstream outCfg;
    ofstream outIni;
    ofstream outRc;
    const char *confCfg = pwx::StreamHelpers::makeTemp("", "test", "cfg", outCfg);
    const char *confIni = pwx::StreamHelpers::makeTemp("", "test", "ini", outIni);
    const char *confRc  = pwx::StreamHelpers::makeTemp("", "test", "rc", outRc);

    try
      {
        if ((NULL == confCfg) || (NULL == confIni) || (NULL == confRc))
          {
            cerr << "FATAL: makeTemp FAILED!" << endl;
            if (confCfg) free (const_cast<char *>(confCfg));
            if (confIni) free (const_cast<char *>(confIni));
            if (confRc ) free (const_cast<char *>(confRc ));
            return EXIT_FAILURE;
          }
        // We do not need autosave here
        CFH.setAutoSave(false);

        /** *********************************************************************
          * ** Test 1: Create an empty ini file and write some test keys to it **
          ******************************************************************** **/
        cout << "Test 1: Create an empty ini file for some test keys\n" << endl;
        CFH.create("TestConfig_01", confIni, cfINI, 0, true);

        cout << "       - Add one key + data + comment using addData and addComment" << endl;
        CFH.addData("talk", "hello,world");
        CFH.addComment("talk", "I am a comment");

        cout << "       - Add a group \"general\"" << endl;
        CFH.setGroup("general");

        cout << "       - Add a key with overlength data with setData(),\n";
        cout << "         and comment with setComment()" << endl;
        CFH.setData("talk", "Hello World is all that is generally said by any program written first in all those fancy programmers handbooks.");
        CFH.setComment("talk", "This is a comment, too.");

        cout << "       - Add a group \"other\"" << endl;
        CFH.setGroup("other");

        cout << "       - Add a key, data, comment with setKey" << endl;
        CFH.setKey("talk", "Hello World", "This is just another comment.");

        cout << "       - Print out the content\n------------------------" << endl;
        CFH.save("TestConfig_01", cout); // To cout
        CFH.save("TestConfig_01"); // to confIni_c_str()
        cout << "------------------------\n" << endl;

        /** *********************************************************************
          * ** Test 2: Add the overlength key/comment to rc and cfg            **
          ******************************************************************** **/
        cout << "Test 2: Create an empty rc and an empty cfg file\n         The Config file does not sort data." << endl;
        CFH.create("TestConfig_02", confCfg, cfConfig, 80, true);
        CFH.create("TestConfig_03", confRc, cfRc, 80, true);

        cout << "       - Add the above overlength key+data+comment to the config\n";
        cout << "         file using the full addData() and addComment() functions.\n";
        cout << "         This time a very lengthy comment is added." << endl;
        CFH.addData("TestConfig_02", CFC_NoGroupName, "talk",
                    "Hello,World,is,all,that,is,generally,said,by,any,program,written,first,in,all,those,fancy,programmers,handbooks.");
        CFH.addComment("TestConfig_02", CFC_NoGroupName,
                       "talk", "This is just a comment to test the line wrapping feature that should work for comments as well.");

        cout << "       - Add the above overlength key+data+comment to the rc file\n";
        cout << "         using the full setData() and setComment() functions, too" << endl;
        CFH.setData("TestConfig_03", CFC_NoGroupName, "talk",
                    "Hello World is all that is generally said by any program written first in all those fancy programmers handbooks.");
        CFH.setComment("TestConfig_03", CFC_NoGroupName,
                       "talk", "This is just a comment to test the line wrapping feature that should work for comments as well.");

        cout << "       - Now make the data in the rc file unique\n";
        CFH.unique("talk");

        cout << "       - Print out the contents\n------------------------\n === Config File === \n" << endl;
        CFH.save("TestConfig_02", cout); // To cout
        CFH.save("TestConfig_02"); // to confIni_c_str()
        cout << "\n === Rc File === \n" << endl;
        CFH.save("TestConfig_03", cout); // To cout
        CFH.save("TestConfig_03"); // to confIni_c_str()
        cout << "------------------------\n" << endl;

        /** *********************************************************************
          * ** Test 3: Test sort() and getData()                               **
          ******************************************************************** **/
        cout << "Test 3: Test flag change and sorting of data in the ini file\n" << endl;
        cout << "       - File is marked as being changed (false): " << (CFH.isChanged("TestConfig_01") ? "true" : "false") << endl;
        cout << "       - Change data separator from comma to space: ";
        CFH.addFlags("TestConfig_01", pwx::cfh::cfDSpace);
        cout << "done" << endl;
        cout << "       - File is marked as being changed (true) : " << (CFH.isChanged("TestConfig_01") ? "true" : "false") << endl;
        cout << "       - Add an unsorted text to [toSort] -> \"text\": ";
        CFH.setKey("toSort", "text", "This is a space separated text to be sorted", NULL);
        cout << "\"" << CFH.getData("text") << "\"" << endl;
        cout << "       - now sort the text : " << endl;
        CFH.sort("text");
        cout << "       - print [toSort] -> \"text\", now sorted:" << endl;
        cout << " \"" << CFH.getData("text") << "\"" << endl;
        cout << "       - Change data separator from space back to comma: ";
        CFH.addFlags("TestConfig_01", pwx::cfh::cfDComma);
        cout << "done" << endl;
        cout << "       - print [toSort] -> \"text\", again     :" << endl;
        cout << " \"" << CFH.getData("text") << "\"" << endl;
        cout << "------------------------\n" << endl;

        /** ************************************************************************
          * ** Test 4: Test Data retrieval with all three sets getData/Count/Item **
          *********************************************************************** **/
        cout << "Test 4: Test the three data retrieval function chains" << endl;
        cout << "       - Length of \"talk\" key data in config file: ";
        cout << CFH.getDataCount("TestConfig_02", CFC_NoGroupName, "talk") << endl;
        cout << "       - Length of \"talk\" key data in rc file    : ";
        cout << CFH.getDataCount("TestConfig_03", CFC_NoGroupName, "talk") << endl;
        cout << "       (The rc should have less after they where made unique!)" << endl;
        cout << "       - get Item 1, 3 and 7 of both files:" << endl;
        cout << "       Config: ";
        cout << CFH.getDataItem("TestConfig_02", CFC_NoGroupName, "talk", 1) << ",";
        cout << CFH.getDataItem("talk", 3) << "," << CFH.getDataItem("talk", 7) << endl;
        cout << "       RC    : ";
        cout << CFH.getDataItem("TestConfig_03", CFC_NoGroupName, "talk", 1) << ",";
        cout << CFH.getDataItem("talk", 3) << "," << CFH.getDataItem("talk", 7) << endl;
        cout << "       - get the full data string from Ini -> [other] -> \"talk\":\n";
        cout << "         \"" << CFH.getData("TestConfig_01", "other", "talk") << "\"" << endl;
        cout << "------------------------\n" << endl;

        /** ************************************************************************
          * ** Test 5: Test Comment change and retrieval with set/getComment()    **
          *********************************************************************** **/
        cout << "Test 5: Test the getComment() function" << endl;
        cout << "        The groupless key is taken for this in all three files." << endl;
        cout << "       - set the comment to something more descriptive first." << endl;
        CFH.setComment("TestConfig_01", CFC_NoGroupName, "talk", "This is IniFile -> GroupLess -> talk");
        CFH.setComment("TestConfig_02", CFC_NoGroupName, "talk", "This is ConfigFile -> GroupLess -> talk");
        CFH.setComment("TestConfig_03", CFC_NoGroupName, "talk", "This is RcFile -> GroupLess -> talk");
        cout << "       - Length of \"talk\" key comment in ini file   : ";
        cout << CFH.getCommentSize("TestConfig_01", CFC_NoGroupName, "talk") << endl;
        cout << "       - Length of \"talk\" key comment in config file: ";
        cout << CFH.getCommentSize("TestConfig_02", CFC_NoGroupName, "talk") << endl;
        cout << "       - Length of \"talk\" key comment in rc file    : ";
        cout << CFH.getCommentSize("TestConfig_03", CFC_NoGroupName, "talk") << endl;
        cout << "       - ini comment   : \"";
        cout << CFH.getComment("TestConfig_01", CFC_NoGroupName, "talk") << "\"" <<endl;
        cout << "       - config comment: \"";
        cout << CFH.getComment("TestConfig_02", CFC_NoGroupName, "talk") << "\"" <<endl;
        cout << "       - rc comment    : \"";
        cout << CFH.getComment("TestConfig_03", CFC_NoGroupName, "talk") << "\"" <<endl;

        cout << "------------------------\n" << endl;

        /** *********************
          * ** Clean up ...    **
          *********************** **/
        cout << "Clean up behind ourselves..." << endl;
        cout << " - close ini file and delete temp file:" << endl;
        cout << "   -> " << CFH.close("TestConfig_01", false) << " open files left" << endl;
        if (pwx_file_exists(confIni))
          pwx_unlink(confIni);
        cout << " - close config file and delete temp file:" << endl;
        cout << "   -> " << CFH.close("TestConfig_02", false) << " open files left" << endl;
        if (pwx_file_exists(confCfg))
          pwx_unlink(confCfg);
        cout << " - close rc file and delete temp file:" << endl;
        cout << "   -> " << CFH.close("TestConfig_03", false) << " open files left" << endl;
        if (pwx_file_exists(confRc))
          pwx_unlink(confRc);
      }
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

    if (confCfg) free (const_cast<char *>(confCfg));
    if (confIni) free (const_cast<char *>(confIni));
    if (confRc ) free (const_cast<char *>(confRc ));

    return result;
  }
