/** @file main.cpp
  *
  * @brief main file for makeSimplexTexture
  *
  * This is the main file for makeSimplexTexture, a small utility to create
  * tiff textures of Simplex Noise structures utilizing pwxLib RNG, using
  * SFML (for preview) and ImageMagick++ (for rendering).
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
  * 0.6.0     2010-05-28  sed, PrydeWorX  initial public release
  * 0.6.1     2010-06-07  sed, PrydeWorX  added z-/w-sequences
  * 0.6.2     2010-07-12  sed, PrydeWorX  added batch mode, image type
  *                                       selection and color border control.
  * 0.7.0     2010-11-09  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.0.1   2011-03-04  sed, PrydeWorX  Changed from manual parsing to the usage of pwxArgs
  * 0.7.0.2   2011-03-10  sed, PrydeWorX  Patched for compatibility with Visual C++ 2008
  * 0.7.0.3   2011-03-22  sed, PrydeWorX  Changed from SDL+SDL_ttf+ImageMagick to SFML
  * 0.7.1     2011-03-26  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.7.2     2011-04-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0     2011-07-14  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.0.1   2011-07-21  sed, PrydeWorX  Fixed some typos
  * 0.8.0.2   2011-08-08  sed, PrydeWorX  Fixed a bug that caused all offsets to be written into the X-Offset
  * 0.8.0.3   2011-09-20  sed, PrydeWorX  Eventually made the rendering multi threaded, greatly reducing size and complexity
  *                                       of renderer.cpp.
  * 0.8.1     2011-09-30  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.2     2011-10-07  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3     2011-10-16  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.3.1   2011-10-17  sed, PrydeWorX  Corrected the help text, the default file extension is png now, not tif
  * 0.8.3.2   2012-01-11  sed, PrydeWorX  Eventually I found the time to add an icon...
  * 0.8.5     2012-03-01  sed, PrydeWorX  Version bump to new pwxLib release version
  * 0.8.5.1   2012-03-23  sed, PrydeWorX  Fixed various Effective C++ issues
  * 0.8.5.2   2012-04-23  sed, PrydeWorX  Fixed various VC++10 issues
  * 0.8.6     2012-04-??  sed, PrydeWorX  Version bump to new pwxLib release version
  @endverbatim
**/

#include "main.h"
#include "workloop.h"  // singled out, has only relevance to main.cpp!
#include "consoleui.h" // only main has to put messages onto the console with these functions

int main ( int argc, char** argv )
{
  int32_t result = EXIT_SUCCESS;
  ENVIRONMENT * env = NULL;

  try
    {
      srand(static_cast<uint32_t>(time(NULL)));
      env = new ENVIRONMENT(rand() % 25000 + 25000);
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
      // A : Initialize Display:
      if (env->withGUI)
        result = initSFML(env);

      // B : Render and load first Image:
      if (EXIT_SUCCESS == result)
        result = loadNewImage(env);
      else
        cerr << "initSFML failed!" << endl;

      // C : Enter WorkLoop:
      if (env->withGUI && (EXIT_SUCCESS == result))
        result = workLoop(env);
    } // End of there is work to be done

  // Clean out environment
  if (env) delete (env);
  env = NULL;

  return (result);
}
