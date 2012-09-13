#include <iostream>
#include <pwxLib/tools/Args.h>
#include <pwxLib/tools/StreamHelpers.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using pwx::StreamHelpers::adjRight;
using namespace pwx::args::constants;


void callback(const char *arg, void *user_data);
void printErrors();

int main ()
{
  char *argv[13] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                     NULL, NULL, NULL, NULL, NULL, NULL
                   }; // Pool for forged arguments
  bool showHelp  = false;
  int  iInc      = 0;
  int  iDec      = 0;
  int  iAssign   = 0;
  float fInc     = 0.0;
  float fDec     = 0.0;
  float fAssign  = 0.0;

  for (int i = 0; i <13; ++i)
    {
      argv[i] = static_cast<char *>(calloc(33, sizeof(char)));
      if (!argv[i])
        {
          cerr << "Failed to allocate " << (33 * sizeof(char)) << "bytes";
          cerr << " for forged command line argument number" << i << endl;
          for (int j = i - 1; j >= 0; --j)
            free (argv[j]);
          return 1;
        }
    }
  pwx_strncpy(argv[0], "progname", 32);


  cout << " ============================================================\n";
  cout << " == Test one: Set arguments to use their targets directly ===\n";
  cout << " ============================================================\n";
  cout << "\nAdd args for bool, integer inc, dec and assign and\n";
  cout << "floating point inc, dec and assign.\n" << endl;

  pwx::args::addArgBool ("h", "help", 2, "Show this help text", &showHelp, ETT_TRUE);
  pwx::args::addArgInt32("a", "incInt", -2, "Raise iInc by one", 0, NULL, &iInc, ETT_INC, -10, 10);
  pwx::args::addArgInt32("b", "decInt", -2, "Lower iInc by one", 0, NULL, &iDec, ETT_DEC, -10, 10);
  pwx::args::addArgInt32("c", "assInt", -2, "Assign value to iAssign", 1, "number", &iAssign, ETT_INT, 0, 20);
  pwx::args::addArgFloat("d", "incFloat", 2, "Raise fInc by one", 0, NULL, &fInc, ETT_INC, -10, 10);
  pwx::args::addArgFloat("e", "decFloat", 2, "Lower fInc by one", 0, NULL, &fDec, ETT_DEC, -10, 10);
  pwx::args::addArgFloat("f", "assFloat", 2, "Assign value to fAssign", 1, "float", &fAssign, ETT_FLOAT, -10, 30);

  cout << "All Help texts:" << endl;
  pwx::args::printArgHelp(cout, "a", 8, 20, 20);
  pwx::args::printArgHelp(cout, "b", 8, 20, 20);
  pwx::args::printArgHelp(cout, "c", 8, 20, 20);
  pwx::args::printArgHelp(cout, "d", 8, 20, 20);
  pwx::args::printArgHelp(cout, "e", 8, 20, 20);
  pwx::args::printArgHelp(cout, "f", 8, 20, 20);
  pwx::args::printArgHelp(cout, "h", 8, 20, 20);
  cout << endl;

  cout << "Current values:\nshowHelp : " << (showHelp ? "true" : "false") << "\n";
  cout << "iInc   : " << iInc << "\niDec   : " << iDec << "\niAssign: " << iAssign << endl;
  cout << "fInc   : " << fInc << "\nfDec   : " << fDec << "\nfAssign: " << fAssign << endl;

  cout << "\nForging command line \"--help abc 10 -fed 20.0\"" << endl;
  pwx_strncpy(argv[1], "--help", 32);
  pwx_strncpy(argv[2], "abc", 32);
  pwx_strncpy(argv[3], "10", 32);
  pwx_strncpy(argv[4], "-fed", 32);
  pwx_strncpy(argv[5], "20.0", 32);

  cout << "Loading args... (should be 7) : " << pwx::args::loadArgs(6, argv) << endl;
  printErrors();
  pwx::args::clearErrors();
  cout << "\nProcessing args..." << endl;
  pwx::args::procArgs();

  cout << "Current values:\nshowHelp : " << (showHelp ? "true" : "false") << "\n";
  cout << "iInc   : " << iInc << "\niDec   : " << iDec << "\niAssign: " << iAssign << endl;
  cout << "fInc   : " << fInc << "\nfDec   : " << fDec << "\nfAssign: " << fAssign << endl;

  cout << "\ncleaning...";
  pwx::args::clear();
  cout << " done.\n" << endl;

  cout << " ===========================================================================\n";
  cout << " == Test two: Use a cb on a multi-component arg, interrupt by direct mod ===\n";
  cout << " ===========================================================================\n";
  cout << "\nUse a callback function for up to three filenames, interrupted\n";
  cout << "by a simple int increase with max value 2.\n" << endl;
  cout << "Additionally the illegal combo-string -abc tests the error reporting.\n" << endl;
  iInc = 0;

  // here we use the short aliases
  pwx::args::addArg("i", "increase", 2, "Increase iInc by one", 0, "", &iInc, ETT_INC, 0, 2);
  pwx::args::addArg("f", "file", 2, "Add file names for processing", -3, "file", &callback, NULL);

  cout << "All Help texts:" << endl;
  pwx::args::printArgHelp(cout, "file", 8, 20, 20);
  pwx::args::printArgHelp(cout, "increase", 8, 20, 20);

  cout << "\nCurrent value of iInc (0 ?): " << iInc << endl;

  cout << "\nForging command line \"-i --file fileA --increase fileB -abc -f -i fileC fileD\"" << endl;
  pwx_strncpy(argv[ 1], "-i", 32);
  pwx_strncpy(argv[ 2], "--file", 32);
  pwx_strncpy(argv[ 3], "fileA", 32);
  pwx_strncpy(argv[ 4], "--increase", 32);
  pwx_strncpy(argv[ 5], "fileB", 32);
  pwx_strncpy(argv[ 6], "-abc", 32);
  pwx_strncpy(argv[ 7], "-f", 32);
  pwx_strncpy(argv[ 8], "-i", 32);
  pwx_strncpy(argv[ 9], "fileC", 32);
  pwx_strncpy(argv[10], "fileD", 32);

  cout << "Loading args... (should be -3) : " << pwx::args::loadArgs(11, argv) << endl;
  cout << "\nPrinting errors, should be three unknown options and one superfluous component:\n";
  printErrors();
  pwx::args::clearErrors();
  cout << "\nProcessing args..." << endl;
  pwx::args::procArgs();

  cout << "\nCurrent value of iInc (2 ?): " << iInc << endl;

  cout << "\ncleaning...";
  pwx::args::clear();
  cout << " done.\n" << endl;

  cout << " ============================================================================\n";
  cout << " == Test three: Use a cb on a multi-component arg that needs 3 and gets 2 ===\n";
  cout << " ============================================================================\n";
  cout << "\nUse a callback function for an option that needs three names, but\n";
  cout << "only gets two to check if it is correctly reported as an error\n" << endl;

  pwx::args::addArg("n", "name", -2, "Set the three names", 3, "nameA nameB nameC", &callback, NULL);

  cout << "All Help texts:" << endl;
  pwx::args::printArgHelp(cout, "name", 8, 20, 20);

  cout << "\nForging command line \"-n nameA name nameB\"" << endl;
  pwx_strncpy(argv[1], "-n", 32);
  pwx_strncpy(argv[2], "nameA", 32);
  pwx_strncpy(argv[3], "name", 32);
  pwx_strncpy(argv[4], "nameB", 32);

  cout << "Loading args... (should be 2) : " << pwx::args::loadArgs(5, argv) << endl;
  cout << "\nPrinting errors, should be a missing third component:\n";
  printErrors();
  pwx::args::clearErrors();
  cout << "\nProcessing args..." << endl;
  pwx::args::procArgs();

  cout << "\ncleaning...";
  pwx::args::clear();
  cout << " done.\n" << endl;


  cout << " ============================================================================\n";
  cout << " == Test four: Test the difference between ETT_CHAR and ETT_STRING        ===\n";
  cout << " ============================================================================\n";

  char   chShort[2] = { 0x0, 0x0 };
  char   chLong[5]  = { 0x0, 0x0, 0x0, 0x0, 0x0 };
  string stShort    = "";
  string stLong     = "";

  pwx::args::addArgChar  ("a", "", 2, "One char on char",   1, "string", chShort,  1, ETT_CHAR);
  pwx::args::addArgChar  ("b", "", 2, "String on char",     1, "string", chLong,   4, ETT_STRING);
  pwx::args::addArgString("c", "", 2, "One char on string", 1, "string", &stShort, ETT_CHAR);
  pwx::args::addArgString("d", "", 2, "String on string",   1, "string", &stLong,  ETT_STRING);

  cout << "All Help texts:" << endl;
  pwx::args::printArgHelp(cout, "a", 4, 0, 20);
  pwx::args::printArgHelp(cout, "b", 4, 0, 20);
  pwx::args::printArgHelp(cout, "c", 4, 0, 20);
  pwx::args::printArgHelp(cout, "d", 4, 0, 20);

  cout << "\nForging command line \"-a Hij -b ello -c WoW -d orld\"" << endl;
  pwx_strncpy(argv[1], "-a", 32);
  pwx_strncpy(argv[2], "Hij", 32);
  pwx_strncpy(argv[3], "-b", 32);
  pwx_strncpy(argv[4], "ello", 32);
  pwx_strncpy(argv[5], "-c", 32);
  pwx_strncpy(argv[6], "WoW", 32);
  pwx_strncpy(argv[7], "-d", 32);
  pwx_strncpy(argv[8], "orld", 32);

  cout << "Loading args... (should be 4) : " << pwx::args::loadArgs(9, argv) << endl;
  cout << "\nPrinting errors, should have no errors:\n";
  printErrors();
  pwx::args::clearErrors();
  cout << "\nProcessing args..." << endl;
  pwx::args::procArgs();
  cout << "\nPrint out the targets, should read \"Hello World\"" << endl;
  cout << chShort << chLong << " " << stShort << stLong << endl;

  cout << "\ncleaning...";
  pwx::args::clear();
  cout << " done.\n" << endl;


  cout << " ============================================================================\n";
  cout << " == Test five: add/sub two values on int/float with ETT_ADD and ETT_SUB   ===\n";
  cout << " ============================================================================\n";
  cout << "\nUse ETT_ADD and ETT_SUB on two integer and two floating point values with\n";
  cout << "two components each to see if the additions work correctly.\n" << endl;

  pwx::args::addArgInt32("", "addInt",   2, "add up integers",    -10, "int",   &iInc, ETT_ADD,   0, 10);
  pwx::args::addArgInt32("", "subInt",   2, "substract integers", -10, "int",   &iDec, ETT_SUB, -10,  0);
  pwx::args::addArgFloat("", "addFloat", 2, "add up floats",      -10, "float", &fInc, ETT_ADD,   0, 10);
  pwx::args::addArgFloat("", "subFloat", 2, "substract floats",   -10, "float", &fDec, ETT_SUB, -10,  0);

  iDec = 0;
  iInc = 0;
  fDec = 0;
  fInc = 0;

  cout << "All Help texts:" << endl;
  pwx::args::printArgHelp(cout, "addInt",   0, 25, 20);
  pwx::args::printArgHelp(cout, "subInt",   0, 25, 20);
  pwx::args::printArgHelp(cout, "addFloat", 0, 25, 20);
  pwx::args::printArgHelp(cout, "subFloat", 0, 25, 20);

  cout << "\nForging command line \"--addInt 1 6 --subInt 2 5 --addFloat 3.5 4.5 --subFloat 5.5 2.5\"" << endl;
  pwx_strncpy(argv[1], "--addInt", 32);
  pwx_strncpy(argv[2], "1", 32);
  pwx_strncpy(argv[3], "6", 32);
  pwx_strncpy(argv[4], "--subInt", 32);
  pwx_strncpy(argv[5], "2", 32);
  pwx_strncpy(argv[6], "5", 32);
  pwx_strncpy(argv[7], "--addFloat", 32);
  pwx_strncpy(argv[8], "3.5", 32);
  pwx_strncpy(argv[9], "4.5", 32);
  pwx_strncpy(argv[10], "--subFloat", 32);
  pwx_strncpy(argv[11], "5.5", 32);
  pwx_strncpy(argv[12], "2.5", 32);

  cout << "Loading args... (should be 4) : " << pwx::args::loadArgs(13, argv) << endl;
  cout << "\nPrinting errors, should none:\n";
  printErrors();
  pwx::args::clearErrors();

  cout << "\nValues before processing:\n";
  cout << "Int A   (0)   : " << adjRight(2) << iInc << endl;
  cout << "Int B   (0)   : " << adjRight(2) << iDec << endl;
  cout << "Float A (0.0) : " << adjRight(2,1) << fInc << endl;
  cout << "Float B (0.0) : " << adjRight(2,1) << fDec << endl;
  cout << "\nProcessing args..." << endl;
  pwx::args::procArgs();
  cout << "\nValues after processing:\n";
  cout << "Int A   ( 7)   : " << adjRight(2) << iInc << endl;
  cout << "Int B   (-7)   : " << adjRight(2) << iDec << endl;
  cout << "Float A ( 8.0) : " << adjRight(2,1) << fInc << endl;
  cout << "Float B (-8.0) : " << adjRight(2,1) << fDec << endl;

  cout << "\ncleaning...";
  pwx::args::clear();
  cout << " done.\n" << endl;


  // Clean our arg pool up
  for (int i = 0; i <13; ++i)
    {
      if (argv[i])
        free (argv[i]);
    }

  cout << "  -------------------------\n --- All tests finished! ---\n  -------------------------" << endl;
  return 0;
}

// This callback function simply prints out what it gets
void callback(const char * arg, void *user_data)
{
  cout << "Callback: \"" << (arg ? arg : "") << "\" [User Data: " << user_data << "]" << endl;
}

// Simple function to print out every errors that got reported
void printErrors()
{
  size_t errC = pwx::args::getErrorCount();

  cout << "Reported errors : " << errC << endl;
  if (errC)
    {
      for (size_t i = 0 ; i < errC ; ++i)
        cout << adjRight(2) << i + 1 << ": " << pwx::args::getError(i) << endl;
    }
}
