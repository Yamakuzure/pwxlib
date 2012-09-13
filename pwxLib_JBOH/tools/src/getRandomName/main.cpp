/** @file main.cpp
  *
  * @brief main file for getRandomName
  *
  * This is the main file for getRandomName, a small tool to create random names
  * utilizing RNG of the pwxLib.
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
  @verbatim
  * History and Changelog:
  * ----------------------
  * Version   Date        Maintainer      Change(s)
  * 0.6.0     2010-05-19  sed, PrydeWorX  Initial public release
  * 0.6.1     2010-07-08  sed, PrydeWorX  added options to use own modifiers for the coordinates
  * 0.6.2     2010-07-09  sed, PrydeWorX  added norandom option and made random starting offsets used only when
  *                                       no manual offsets are set
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.0.1   2011-03-02  sed, PrydeWorX  changed from manual parsing to the usage of pwxArgs
  * 0.7.0.2   2011-03-06  sed, PrydeWorX  Patches for compatibility with Visual C++ 2008
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5     2012-03-01  sed, PrydeWorX  Welcome to 2012. Version bump to new pwxLib release version
  * 0.8.5.1   2012-03-20  sed, PrydeWorX  Fixed a memory leak introduced with version 0.7.0.2
  * 0.8.5.2   2012-03-23  sed, PrydeWorX  Fixed an Effective C++ issue
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

#include <fstream>
#include <pwxLib/CRandom.h>
using pwx::RNG;

#include "main.h"

int main(int argc, char * argv[])
{
  int32_t result = EXIT_SUCCESS;
  ENVIRONMENT * env = NULL;

  // Set a random seed first:
  RNG.setSeed(RNG.random(10000, 25000));

  try
    {
      env = new ENVIRONMENT(RNG.getSeed());
      result = processArguments(env, argc, argv);
    }
  catch (std::bad_alloc &e)
    {
      cerr << "ERROR : Unable to create environment!" << endl;
      cerr << "REASON: \"" << e.what() << "\"" << endl;
      env = NULL;
      result = EXIT_FAILURE;
    }

  if ((EXIT_SUCCESS == result) && env && env->doWork)
    {
      RNG.setSeed(env->seed);
      if (env->useSeedOff)
        {
          double maxOffset = env->seed ? static_cast<double>(env->seed) : 1000.0;
          double minOffset = env->seed ? static_cast<double>(env->seed) * -1.0 : -1000.0;
          env->setOffsets(RNG.random(minOffset, maxOffset),
                          RNG.random(minOffset, maxOffset),
                          RNG.random(minOffset, maxOffset),
                          RNG.random(minOffset, maxOffset), true);
        }

      if (env->filename.size() > 0)
        {
          std::ofstream ofs(env->filename.c_str());

          if (ofs.is_open())
            {
              char disp[32];
              memset(disp, 0, 32);
              for (int32_t i = 0; i < env->count; ++i)
                {
                  ofs << RNG.rndName(env->offsetX, env->offsetY, env->offsetZ, env->offsetW,
                                     env->maxChars, env->maxSyllables, env->maxParts) << endl;

                  if (env->useRandom && !env->useModVals)
                    env->setOffsets(RNG.random(-5.0L, 5.0L),
                                    RNG.random(-5.0L, 5.0L),
                                    RNG.random(-5.0L, 5.0L),
                                    RNG.random(-5.0L, 5.0L), true);
                  else
                    env->setOffsets();

                  memset(disp, '\b', strlen(disp));
                  cout << disp;
                  pwx_snprintf(disp, 31, "%d", i + 1);
                  cout << disp;
                  cout.flush();
                }
              ofs.close();
              cout << endl;
            }
          else
            cerr << "ERROR: Can't write into " << env->filename << "!" << endl;
        }
      else
        {
          for (int32_t i = 0; i < env->count; ++i)
            {
              char* xName = RNG.rndName(env->offsetX, env->offsetY, env->offsetZ, env->offsetW,
                                        env->maxChars, env->maxSyllables, env->maxParts);
              if (xName)
                {
                  cout << xName << endl;
                  free (xName);
                }
              if (env->useRandom && !env->useModVals)
                env->setOffsets(RNG.random(-5.0L, 5.0L),
                                RNG.random(-5.0L, 5.0L),
                                RNG.random(-5.0L, 5.0L),
                                RNG.random(-5.0L, 5.0L), true);
              else
                env->setOffsets();
            }
        }

    }

  if (env)
    delete env;

  return (result);
}
