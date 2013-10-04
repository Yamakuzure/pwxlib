#include "testPAH.h"
using pwx::PAH;

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

	// ---


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
	cout << PAH.getHelpStr("add", 38, '|', '=', ':') << endl;
	cout << PAH.getHelpStr("-h", 38, '|', '=', ':') << endl;
	cout << PAH.getHelpStr("i", 38, '|', '=', ':') << endl;
	++env.testSuccess;

	/************************************************************************
	** C) Parse fake argc/argv with invalid values and print errors.       **
	************************************************************************/
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

	return result;
}

