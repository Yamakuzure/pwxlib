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
  ** E) Clear all resources and add args for everything                  **
  ** F) Print Help text using automatic text generation.                 **
  ** G) Parse fake argc/argv with valid values and print result.         **
  ** H) Test combinated arguments with shifted parameters                **
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
	cout << "\n--- help text begin (40, 2, '|', ' ', ':', true, true) ---" << endl;
	cout << PAH.getHelpStr("add", 40, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("-h", 40, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("i", 40, 2, '|', ' ', ':', true, true) << endl;
	cout << "--- help text end ---\n" << endl;
	++env.testSuccess;

	/************************************************************************
	** C) Parse fake argc/argv with invalid values and print errors.       **
	************************************************************************/
	cout << adjRight (4, 0) << ++env.testCount << " Parse invalid argv : " << endl;
	setFakeArg(4);
	addFakeArg(1, "--hwlp");    // typo
	addFakeArg(2, "--add");     // wrong, must be without --
	addFakeArg(3, "42");        // Should have been added.
	int errCount = PAH.parseArgs(xArgc, xArgv);
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
	cout << "\n" << adjRight (4, 0) << ++env.testCount << " Parse valid argv : " << endl;
	addFakeArg(1, "i");    // increase tgt_inc by 1
	addFakeArg(2, "add");  // add next value to tgt_add
	addFakeArg(3, "2");    // So the result is 1 and 2
	errCount = PAH.parseArgs(xArgc, xArgv);
	cout << "  argv: " << xArgv[1] << ", " << xArgv[2] << ", " << xArgv[3] << endl;
	cout << "  -> Errors (must be 0) : " << errCount;
	if (0 == errCount) {
		cout << " => SUCCESS" << endl;

		// tgt_inc must be 1 now
		cout << "  -> tgt_inc (must be 1) : " << tgt_inc;
		if (1 << tgt_inc) cout << " => SUCCESS" << endl;
		else              cout << " => FAILURE" << endl;

		// tgt_add must be 2 now
		cout << "  -> tgt_add (must be 2) : " << tgt_add;
		if (2 << tgt_inc) cout << " => SUCCESS" << endl;
		else              cout << " => FAILURE" << endl;

		if ((1 == tgt_inc) && (2 == tgt_add))
			++env.testSuccess;
		else
			++env.testFail;
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
	** E) Clear all resources and add args for everything                  **
	************************************************************************/
	cout << "\n" << adjRight (4, 0) << ++env.testCount << " Create new argument set : ";
	PAH.clearArgs();

	// reset old targets and add new ones:
	tgt_bool = true; // Will be used to test ATT_FALSE now.
	tgt_inc  = 0;
	tgt_add  = 0;
	float       tgt_dec     = 0.f;
	double      tgt_sub     = 0.0;
	uint32_t    tgt_set_err = 0; // set type STT_ERROR
	long double tgt_set_ign = 0.0; // set type STT_IGNORE
	int64_t     tgt_set_ovw = 0; // set type STT_OVERWRITE

	// Pass through targets:
	// const char* init_arg, int32_t* pass_argc, char*** pass_argv
	int32_t pass_argc = 0;
	char**  pass_argv = nullptr;

	try {
		PAH.addArg("-a", "--add", pwx::eArgTargetType::ATT_ADD, &tgt_add,
					"Add num to tgt_add", "num");
		PAH.addArg("-d", "--dec", pwx::eArgTargetType::ATT_DEC, &tgt_dec,
					"Decrease tgt_dec by 1.0", nullptr);
		PAH.addArg("-f", "--false", pwx::eArgTargetType::ATT_FALSE, &tgt_bool,
					"Set tgt_bool to false", nullptr);
		PAH.addArg("-i", "--inc", pwx::eArgTargetType::ATT_INC, &tgt_inc,
					"Increase tgt_inc by 1", nullptr);
		PAH.addArg("-E", "--set_err", pwx::eArgTargetType::ATT_SET, &tgt_set_err,
					"Set tgt_set_err to num", "num", STT_ERROR);
		PAH.addArg("-I", "--set_ign", pwx::eArgTargetType::ATT_SET, &tgt_set_ign,
					"Set tgt_set_ign to num", "num", STT_IGNORE);
		PAH.addArg("-O", "--set_ovw", pwx::eArgTargetType::ATT_SET, &tgt_set_ovw,
					"Set tgt_set_ovw to num", nullptr); // STT_OVERWRITE is default!
		PAH.addArg("-s", "--sub", pwx::eArgTargetType::ATT_SUB, &tgt_sub,
					"Substract num from tgt_sub", nullptr);
		// add callbacks
		PAH.addArg(nullptr, "push",    cb_addstr, "Add word to end of cbtarget", "word");
		PAH.addArg(nullptr, "unshift", cb_addstr, "Add word to start of cbtarget", "word");

		// add passthrough system:
		PAH.addPassthrough("--", &pass_argc, &pass_argv);

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
	** F) Print Help text using automatic text generation.                 **
	************************************************************************/
	cout << adjRight (4, 0) << ++env.testCount << " Print auto help text : " << endl;
	cout << "\n--- help text begin (50, 2, '|', ' ', ':', true, true) ---" << endl;
	cout << PAH.getHelpStr("-a", 50, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("-d", 50, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("-f", 50, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("-i", 50, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("-E", 50, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("-I", 50, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("-O", 50, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("-s", 50, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("push", 50, 2, '|', ' ', ':', true, true) << endl;
	cout << PAH.getHelpStr("unshift", 50, 2, '|', ' ', ':', true, true) << endl;

	cout << "--- help text end ---\n" << endl;
	++env.testSuccess;

	/************************************************************************
	** G) Parse fake argc/argv with valid values and print result.         **
	************************************************************************/
	/************************************************************************
	** H) Test combinated arguments with shifted parameters                **
	************************************************************************/



	// clean up before returning
	clrFakeArg();
	cb_clrstr();

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
		if (xArgv[nr])
			delete xArgv[nr];
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
