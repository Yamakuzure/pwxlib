#include "testPAH.h"
using pwx::PAH;

#include <string>

// array to test callback function arguments
static std::string cbtarget[5] = {"", "", "", "", ""};

// And the callback function
static void cb_addstr(const char* arg, const char* param);
// Helper to clear the strings
static void cb_clrstr();

// array for fake argv
static int32_t xArgc = 0;
static char**  xArgv = nullptr;

// Helper to add/set/free the fake argc/argv
// index 0 is always "./testlib" !
static void addFakeArg(int32_t nr, const char* arg); // Set arg number nr to arg
static void clrFakeArg();                            // free xArgv
static void setFakeArg(int32_t aSize);                // (re)alloc to size aSize

/** @brief central function to test pwx::PAH
  *
  *************************************************************************
  ** Add some arguments and test them with fake argc/argv.               **
  ** A) Add three arguments, one short, one long, one short and long.    **
  ** B) Print Help text using automatic text generation.                 **
  ** C) Parse fake argc/argv with invalid values and print errors.       **
  ** D) Parse fake argc/argv with valid values and print result.         **
  ** E) Clear all resources.                                             **
  ** F) Add new arguments for all target types and a passthrough         **
  ** G) Parse fake argc/argv with valid values and print result.         **
  *************************************************************************
**/
int32_t testPAH (sEnv &env)
{
	int32_t result = EXIT_SUCCESS;

	// --- Targets for the argument parsing ---
	bool tgt_bool = false;
	int  tgt_inc  = 0;
	int  tgt_add  = 0;

	cout << "\nTest CArgHandler instance pwx::PAH\n-----------------------------------" << endl;

	/************************************************************************
	** A) Add three arguments, one short, one long, one short and long.    **
	************************************************************************/
	cout << adjRight (4, 0) << ++env.testCount << " Add three arguments : ";
	try {
		PAH.addArg("-h", "--help", pwx::eArgTargetType::ATT_TRUE, &tgt_bool,
					"Print this help text", nullptr);
		PAH.addArg("i", nullptr, pwx::eArgTargetType::ATT_INC, &tgt_inc,
					"Increase value tgt_inc by one on each occurence", nullptr);
		PAH.addArg(nullptr, "add", pwx::eArgTargetType::ATT_ADD, &tgt_add,
					"Add <num> to tgt_add on each occurence", "num");

		// As none has thrown, this is a success.
		++env.testSuccess;
		cout << "SUCCESS" << endl;
	} catch (pwx::CException &e) {
		cout << "FAILURE" << endl;
		cout << "   exception name()/what(): \"" <<e.name();
		cout << "\" / \"" << e.what() << "\"" << endl;
		++env.testFail;
		return EXIT_FAILURE;
	}


	/************************************************************************
	** B) Print Help text using automatic text generation.                 **
	************************************************************************/
	cout << adjRight (4, 0) << ++env.testCount << " Print auto help text : " << endl;
	cout << "--- help text begin (40, 2, '|', ' ', ':', true, true) ---" << endl;
	cout << PAH.getHelpStr("add", 40, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("-h", 40, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("i", 40, 2, '|', ' ', ':', true, true) << endl;
	cout << "--- help text end ---" << endl;
	++env.testSuccess;

	/************************************************************************
	** C) Parse fake argc/argv with invalid values and print errors.       **
	************************************************************************/
	cout << adjRight (4, 0) << ++env.testCount << " Parse invalid argv : " << endl;
	setFakeArg(4);
	addFakeArg(1, "--hwlp");    // typo
	addFakeArg(2, "--add");     // wrong, must be without --
	addFakeArg(3, "42");        // Should have been added.
	int errCount = PAH.parseArgs(xArgc, const_cast<const char**>(xArgv));
	cout << "  argv: " << xArgv[1] << ", " << xArgv[2] << ", " << xArgv[3] << endl;
	cout << "  -> Errors (must be 3) : " << errCount;
	if (3 == errCount) {
		cout << " => SUCCESS" << endl;
		++env.testSuccess;
	} else {
		cout << " => FAILURE" << endl;
		++env.testFail;
	}
	if (errCount) {
		cout << "  -> Errors found: " << endl;
		for (int i = 1; i <= errCount; ++i) {
			cout << adjRight(7, 0) << i << ": ";
			cout << PAH.getErrorStr(i) << " [";
			cout << PAH.getError(i) << "]" << endl;
		}
	}



	/************************************************************************
	** D) Parse fake argc/argv with valid values and print result.         **
	************************************************************************/
	/************************************************************************
	** E) Clear all resources.                                             **
	************************************************************************/
	/************************************************************************
	** F) Add new arguments for all target types and a passthrough         **
	************************************************************************/
	/************************************************************************
	** G) Parse fake argc/argv with valid values and print result.         **
	************************************************************************/

	// clean up before returning
	clrFakeArg();

	return result;
}


// --- callback test function ---
// simply add the argument to the string array (push = back, unshift = front)
static void cb_addstr(const char* arg, const char* param)
{
	if (arg && param) {
		if (STREQ(arg, "push")) {
			for (int i = 4; i >= 0; --i) {
				if (!cbtarget[i].size()) {
					cbtarget[i].assign(param);
					i = -1;
				}
			}
		} else if (STREQ(arg, "unshift")) {
			for (int i = 0; i < 5; ++i) {
				if (!cbtarget[i].size()) {
					cbtarget[i].assign(param);
					i = 5;
				}
			}
		}
	}
}



// --- internal helpers ---

/// @internal clear the string array
static void cb_clrstr()
{
	for (int i = 0; i < 5; ++i)
		cbtarget[i].clear();
}


/// @internal Set arg number nr to arg
static void addFakeArg(int32_t nr, const char* arg)
{
	if (arg && strlen(arg)) {
		if (nr >= xArgc)
			setFakeArg(nr + 1);
		xArgv[nr] = new char[strlen(arg) + 1];
		strncpy(xArgv[nr], arg, strlen(arg));
	}
}


/// @internal free xArgv
static void clrFakeArg()
{
	if (xArgv) {
		for (int32_t i = 0; i < xArgc; ++i) {
			if (xArgv[i]) {
				delete xArgv[i];
				xArgv[i] = nullptr;
			}
		}
		delete [] xArgv;
		xArgv = nullptr;
	}
	xArgc = 0;
}


/// @internal (re)alloc to size aSize
static void setFakeArg(int32_t aSize)
{
	if (aSize > xArgc) {
		int32_t oldArgc = xArgc;
		char**  oldArgv = xArgv;
		xArgc = aSize;
		xArgv = new char*[xArgc];
		for (int32_t i = 0; i < xArgc; ++i) {
			xArgv[i] = nullptr;
			if (i < oldArgc) {
				xArgv[i]   = oldArgv[i];
				oldArgv[i] = nullptr;
			} else if (0 == i)
				addFakeArg(0, "./testLib"); // this must be there!
		}
		if (oldArgc)
			delete [] oldArgv;
	}
}
