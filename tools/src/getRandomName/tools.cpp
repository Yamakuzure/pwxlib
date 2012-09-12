#include "tools.h"

#include <pwxLib/tools/Args.h>
using namespace pwx::args;
using pwx::StreamHelpers::adjRight;
using pwx::StreamHelpers::to_string;

int32_t processArguments(ENVIRONMENT * env, int argc, char * argv[])
{
  int32_t result     = EXIT_SUCCESS;
  bool    doShowHelp = false;
  bool    doShowVers = false;
  assert(env);

  // The arguments for the coordinates and the miodificatorshave a combined help text
  // Therefore they neither have a description, nor a component name.
  addArgCb("x", "",    -2, NULL, 1, NULL, cbSetX, env);
  addArgCb("y", "",    -2, NULL, 1, NULL, cbSetY, env);
  addArgCb("z", "",    -2, NULL, 1, NULL, cbSetZ, env);
  addArgCb("w", "",    -2, NULL, 1, NULL, cbSetW, env);
  addArgCb("", "modx", -2, NULL, 1, NULL, cbSetModX, env);
  addArgCb("", "mody", -2, NULL, 1, NULL, cbSetModY, env);
  addArgCb("", "modz", -2, NULL, 1, NULL, cbSetModZ, env);
  addArgCb("", "modw", -2, NULL, 1, NULL, cbSetModW, env);

  // The rest of the arguments are set up normally
  addArg("c",  "count",    -2, "Generate <count> names", 1, "count",                            &env->count,
         ETT_INT, 0, maxInt32Limit);
  addArg(NULL, "chars",    -2, "Set maximum characters to generate.", 1, "value",               &env->maxChars,
         ETT_INT, 0, maxInt32Limit);
  addArg("h",  "help",     -2, "Show this help and exit",                                       &doShowHelp,
         ETT_TRUE);
  addArg("n",  "norandom", -2, "Increase coordinates by 1 instead of random modifiers. (*)", &env->useRandom,
         ETT_FALSE);
  addArg("o",  "out",      -2, "Write generated name(s) into <file>.", 1, "filename",           &env->filename,
         ETT_STRING);
  addArg(NULL, "parts",    -2, "Set maximum parts to generate.",       1, "value",              &env->maxParts,
         ETT_INT, 0, maxInt32Limit);
  addArg("s",  "seed",     -2, "Set the seed to <value>.",             1, "value",              &env->seed,
         ETT_INT, minInt32Limit, maxInt32Limit);
  addArg(NULL, "sylls",    -2, "Set maximum syllables to generate.",   1, "value",              &env->maxSyllables,
         ETT_INT, 0, maxInt32Limit);
  addArg("v",  "version",  -2, "Show program version and exit",                                 &doShowVers,
         ETT_TRUE);

  // Load arguments and check for errors:
  loadArgs(argc, argv);

  size_t errC = getErrorCount();
  if (errC)
    {
      int32_t errSize = to_string(errC).size();
      doShowHelp = true;
      result     = EXIT_FAILURE;

      cerr << errC << " errors occurred:" << endl;
      for (size_t i = 0 ; i < errC ; ++i)
        cerr << adjRight(errSize) << i + 1 << ": " << getError(i) << endl;
    }
  else
    // No errors, than we can process the arguments:
    procArgs();

  // Opt out via help or version display?
  if (doShowHelp)
    {
      showHelp(env);
      env->doWork = false;
    }
  else if (doShowVers)
    {
      showVersion(env);
      env->doWork = false;
    }

  // clean up arguments:
  pwx::args::clear();

  return (result);
}

void showHelp(ENVIRONMENT * env)
{
  cout << "getRandomName - ";
  showVersion(env);
  cout << "----------------";
  showVerDash(env);
  cout << endl << endl;
  cout << "  Usage:" << endl;
  cout << "getrn [options]" << endl << endl;
  cout << "The default behavior, when no options are given, is to write" << endl;
  cout << "one random names with 1-3 parts, 2-5 syllables and  6-15" << endl;
  cout << "characters to stdout." << endl << endl;
  cout << "  Options:" << endl;
  cout << "[-]x,y,z,w <value>          set value for the x,y,z or w coordinate. If" << endl;
  cout << "                            no offset is set, the program uses random ones" << endl;
  cout << getArgHelp("c", 6, 22, 48) << endl;
  cout << getArgHelp("chars", 6, 22, 48) << endl;
  cout << getArgHelp("h", 6, 22, 48) << endl;
  cout << "      [--]mod<xyzw> <value> set modifier for x, y, z or w coordinate" << endl;
  cout << "                            instead of using random ones (*)" << endl;
  cout << getArgHelp("n", 6, 22, 48) << endl;
  cout << getArgHelp("o", 6, 22, 48) << endl;
  cout << getArgHelp("parts", 6, 22, 48) << endl;
  cout << getArgHelp("sylls", 6, 22, 48) << endl;
  cout << getArgHelp("s", 6, 22, 48) << endl;
  cout << getArgHelp("v", 6, 22, 48) << endl;
  cout << endl << "(*) : Note on mod* and norandom:" << endl;
  cout << "      If you set at least one modifier, no random values will be used," << endl;
  cout << "      but the value(s) you set, and 0.0 for all you omit. The norandom" << endl;
  cout << "      option is only taken into account if you do not set any modifier" << endl;
  cout << "      and sets all four to 1.0." << endl;
}

void showVersion(ENVIRONMENT * env)
{
  cout << "Version " << env->getVersion() << endl;
}

void showVerDash(ENVIRONMENT * env)
{
  int32_t length = 8 + strlen(env->getVersion());
  for (int32_t i = 0; i < length; ++i)
    cout << "-";
}
