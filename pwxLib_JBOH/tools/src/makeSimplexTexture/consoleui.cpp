#include "consoleui.h"
#include "callback.h"
#include <pwxLib/tools/Args.h>
using namespace pwx::args::constants;
using pwx::args::addArgCb;
using pwx::args::addArgBool;
using pwx::args::addArgDouble;
using pwx::args::addArgInt32;
using pwx::args::addArgString;

int32_t processArguments(ENVIRONMENT * env, int argc, char * argv[])
{
  int32_t result = EXIT_SUCCESS;
  assert(env);

  // -- normal arguments ---
  addArgInt32 ("",  "height", -2, "Set window height (minimum 480)", 1, "height", &env->scrHeight, ETT_INT, 480, maxInt32Limit);
  addArgCb    ("",  "help", -2, "Show this help and exit", 0, NULL, cbHelpVersion, env);
  addArgCb    ("",  "version", -2, "Show the programs version and exit", 0, NULL, cbHelpVersion, env);
  addArgInt32 ("",  "width", -2, "Set window width (minimum 640)", 1, "width", &env->scrWidth, ETT_INT, 640, maxInt32Limit);
  addArgString("e", "ext", -2, "Set the file extension to the desired image format. The default is \"png\".", 1, "ext", &env->targetExt, ETT_STRING);
  addArgDouble("l", "bordlo",  -2, "Set low border (-1.0 to 1.0)", 1, "value", &env->borderLo, ETT_FLOAT, -1.0, 1.0);
  addArgDouble("h", "bordhi",  -2, "Set high border (-1.0 to 1.0)", 1, "value", &env->borderHi, ETT_FLOAT, -1.0, 1.0);
  addArgBool  ("n", "no-bumpmap", -2, "Omit the calculation of a bumpmap", &env->withBump, ETT_FALSE);
  addArgBool  ("q", "quiet", -2, "No output to the console", &env->verbose, ETT_FALSE);
  addArgInt32 ("s", "seed", -2, "Set seed", 1, "value", &env->seed, ETT_INT, 0, maxInt32Limit);
  addArgInt32 ("t", "threads", -2, "Set number of threads (minimum 4, default 8)", 1, "num", &env->numThreads, ETT_INT, 4, maxInt32Limit);
  addArgBool  ("B", "batch", -2, "Enable batch mode (no GUI)", &env->withGUI, ETT_FALSE);
  addArgCb    ("H", "colhi", -2, "Set the high color", 1, "value", cbColorHi, env);
  addArgCb    ("L", "collow", -2, "Set the low color", 1, "value", cbColorLow, env);
  addArgCb    ("M", "colmid", -2, "Set the middle color", 1, "value", cbColorMid, env);
  addArgDouble("R", "reduct", -2, "Set the reduction on each wave (minimum 1.0)", 1, "value", &env->spxRedu, ETT_FLOAT, 1.0, 1000.0);
  addArgDouble("S", "smooth", -2, "Set the smoothing on each wave (minimum 1.0)", 1, "value", &env->spxSmoo, ETT_FLOAT, 1.0, 1000.0);
  addArgInt32 ("W", "waves", -2, "Set number of waves (minimum 1)", 1, "value", &env->spxWave, ETT_INT, 1, maxInt32Limit);
  addArgDouble("Z", "zoom", -2, "Set the zoom factor (minimum 0.001)", 1, "value", &env->spxZoom, ETT_FLOAT, 0.001, 1000.0);

  // --- normal options with special passages in the help text ---
  addArgCb     ("",  "3D", 0, "", 0, NULL, cbDimension, env);
  addArgCb     ("",  "4D", 0, "", 0, NULL, cbDimension, env);
  addArgDouble ("",  "modz", -2, "", 1, NULL, &env->modZ, ETT_FLOAT, -1000000.0, 1000000.0);
  addArgDouble ("",  "modw", -2, "", 1, NULL, &env->modW, ETT_FLOAT, -1000000.0, 1000000.0);
  addArgDouble ("w", "",   0, "", 1, NULL, &env->offW, ETT_FLOAT, -1000000.0, 1000000.0);
  addArgDouble ("x", "",   0, "", 1, NULL, &env->offX, ETT_FLOAT, -1000000.0, 1000000.0);
  addArgDouble ("y", "",   0, "", 1, NULL, &env->offY, ETT_FLOAT, -1000000.0, 1000000.0);
  addArgDouble ("z", "",   0, "", 1, NULL, &env->offZ, ETT_FLOAT, -1000000.0, 1000000.0);

  // --- sequential arguments ---
  // These have a special passage in the help text, so they do not need descriptions
  addArgCb("", "zIncModX", -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIncModY", -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIncModW", -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsX",     -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsY",     -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsW",     -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsXY",    -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsXYdW",  -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsXYmW",  -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsXdY",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsYdX",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsXaY",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsXsY",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsYsX",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsXmY",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "zIsYmX",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIncModX", -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIncModY", -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIncModZ", -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsX",     -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsY",     -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsZ",     -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsXY",    -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsXYdZ",  -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsXYmZ",  -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsXdY",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsYdX",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsXaY",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsXsY",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsYsX",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsXmY",   -2, "", 0, NULL, cbSetSequence, env);
  addArgCb("", "wIsYmX",   -2, "", 0, NULL, cbSetSequence, env);

  if (pwx::args::loadArgs(argc, argv) >= 0)
    pwx::args::procArgs();

  if (0 != pwx::args::getErrorCount())
    {
      result = EXIT_FAILURE;
      cout << "The following errors occured:" << endl;
      for (size_t i = 0; i < pwx::args::getErrorCount(); ++i)
        cout << pwx::StreamHelpers::adjRight(2,0) << (i + 1) << ".: " << pwx::args::getError(i) << endl;
      cout << endl;
      showHelp(env);
    }
  else
    {
      // Low and high border need to be sanitized, and the medium shortcut added:
      if (env->borderLo > env->borderHi)
        {
          double dTmp = env->borderHi;
          env->borderHi = env->borderLo;
          env->borderLo = dTmp;
        }
      env->borderMid = 0.5 * (env->borderHi + env->borderLo);

      // If we do not have medium _and_ low color, high color needs to expand to the middle:
      if (env->colMidEmpty && env->colLowEmpty)
        {
          env->colMid.r = static_cast<uint8_t>(pwx_round(0.5 * env->colHi.r));
          env->colMid.g = static_cast<uint8_t>(pwx_round(0.5 * env->colHi.g));
          env->colMid.b = static_cast<uint8_t>(pwx_round(0.5 * env->colHi.b));
          env->colMidEmpty = false;
        }

      // Set scrSize
      env->scrSize = env->scrWidth * env->scrHeight;

      // Apply numThreads for the threads and threadPrg numbers:
      try
        {
          env->thread    = new sf::Thread*[env->numThreads];
          env->threadPrg = new int32_t[env->numThreads];
          env->threadRun = new bool[env->numThreads];
          for (int32_t i = 0; i < env->numThreads; ++i)
            {
              env->thread[i]    = NULL;
              env->threadPrg[i] = 0;
              env->threadRun[i] = false;
            }
        }
      catch (std::bad_alloc &e)
        {
          result = EXIT_FAILURE;
          cout << "ERROR: unable to allocate " << env->numThreads << " integers for thread progress!";
          cout << " [" << e.what() << "]" << endl;
        }
    } // End of parsing was successful

  return (result);
}

void showHelp(ENVIRONMENT * env)
{
  int32_t spw = 6;  // Short option print width
  int32_t lpw = 20; // Long option print width
  int32_t dpw = 48; // Description print width

  cout << "getMakeSimplexTexture - ";
  showVersion(env);
  cout << "------------------------";
  showVerDash(env);
  cout << endl;
  cout << "  Usage:" << endl;
  cout << "mkst [options]" << endl << endl;
  cout << "The default behavior, when no options are given, is to open" << endl;
  cout << "a 800x600 window, generate one texture, and wait for a" << endl;
  cout << "keypress." << endl;
  cout << endl << "  Options:" << endl;
  cout << "x/y/z/w <value>           Set offset of the specified dimension." << endl;
  cout << "        All four dimensions default to 0.0 and use a stepping of one" << endl;
  cout << "        per pixel, modified by zoom which defaults to 9.0. (This" << endl;
  cout << "        means every pixel raises x and/or y by 1.0 / 9.0)" << endl;
  cout << "        To use z, you have to set 3D or 4D, to use w 4D is needed." << endl;
  cout << "        See \"sequence patterns\" below about how to control z and w." << endl;
  cout << "3D / 4D                   Enable third/fourth dimension" << endl;
  pwx::args::printArgHelp(cout, "B", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "h", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "l", spw, lpw, dpw);
  cout << "        The borders are the limit under/over which value the low and" << endl;
  cout << "        high color are set. The middle color is always set in the" << endl;
  cout << "        middle between those borders. The default sequence for" << endl;
  cout << "        low->mid->high is -1.0->0.0->+1.0" << endl;
  pwx::args::printArgHelp(cout, "H", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "M", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "L", spw, lpw, dpw);
  cout << "        Colors are needed as 0xRRGGBB and default to " << endl;
  cout << "        0xffff00 for colHi, 0x7f7f00 for colMid and" << endl;
  cout << "        0x000000 for colLow" << endl;
  pwx::args::printArgHelp(cout, "e", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "height", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "help", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "n", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "q", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "R", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "s", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "S", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "t", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "version", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "width", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "W", spw, lpw, dpw);
  pwx::args::printArgHelp(cout, "Z", spw, lpw, dpw);
  cout << endl << "The next set of arguments can be used to change the modifiers for" << endl;
  cout << "the coordinates." << endl;
  cout << "Changing the modifier for w and/or z may lead to very strange results, because" << endl;
  cout << "they are used with the *IncMod* sequence patterns, too." << endl;
  cout << "      [--]mod<zw>   <value> change the modification value for z or w" << endl;
  cout << endl << "  Sequence Patterns:" << endl;
  cout << "The following options may be added to command line options or set" << endl;
  cout << "via alt-z or alt-w at runtime. w and z are static values by default." << endl;
  cout << " (to keep the output short, <xy> means either x or y.)" << endl;
  cout << " <wz>IncMod<XYZW> w/z is increased whenever x/y/z/w is modified" << endl;
  cout << " <wz>Is<XYZW>     Set w/z to be equal to x/y/z/w" << endl;
  cout << " <wz>IsXY         Set w/z to be equal to x * y" << endl;
  cout << " <wz>IsXYd<ZW>    Set w/z to be equal to (x * y) / <ZW>" << endl;
  cout << " <wz>IsXYm<ZW>    Set w/z to be equal to (x * y) % <ZW>" << endl;
  cout << " <wz>Is<XY>d<YX>  Set w/z to be either x / y or y / x" << endl;
  cout << " <wz>IsXaY        Set w/z to be equal to x + y" << endl;
  cout << " <wz>Is<XY>s<YX>  Set w/z to be either x - y or y - x" << endl;
  cout << " <wz>Is<XY>m<YX>  Set w/z to be either x % y or y % x" << endl;
  cout << endl << "  GUI key mapping:" << endl;
  cout << "CURSOR: modify x with left/right, y with up/down cursor keys" << endl;
  cout << "d/D   : increase/decrease dimensions (2-4)" << endl;
  cout << "ESC   : quit program" << endl;
  cout << "h     : show on-screen help" << endl;
  cout << "R     : Render image with current settings" << endl;
  cout << "s     : save current texture (and bumpmap unless -n is specified)" << endl;
  cout << "SPACE : switch between texture and bumpmap. This does only work" << endl;
  cout << "        if the -n/no-bumpmap isn't used." << endl;
  cout << "TAB   : Show stats of the current image" << endl;
  cout << "w/W   : increase/decrease offset w by mod w. (defaults to 1.0)" << endl;
  cout << "        press ctrl to increase/decrease with 10 times mod w." << endl;
  cout << "        press alt to toggle w coordinate sequence setting." << endl;
  cout << "z/Z   : increase/decrease offset z by mod z. (defaults to 1.0)" << endl;
  cout << "        press alt to toggle z coordinate sequence setting." << endl;
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
