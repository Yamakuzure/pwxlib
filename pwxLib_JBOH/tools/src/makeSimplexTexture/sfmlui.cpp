#include "sfmlui.h"
#include <cstdarg>


int32_t saveTexture(ENVIRONMENT * env)
{
  int32_t result    = EXIT_SUCCESS;
  char    fname[32] = "";

  pwx_snprintf(fname, 31, "texture%08d.%s", env->imageNum, env->targetExt.c_str());
  showMsg(env, "saving texture %d ...", env->imageNum);

  env->image.SaveToFile(std::string(fname));
  if (env->withBump)
    {
      pwx_snprintf(fname, 31, "texture%08d.bump.%s", env->imageNum, env->targetExt.c_str());
      env->bumpMap.SaveToFile(std::string(fname));
    }

  showMsg(env, "texture %d saved!", env->imageNum++);
  if (!env->withGUI)
    cout << endl;

  return (result);
}


void showMsg(ENVIRONMENT * env, const char * fmt, ...)
{
  if (!env->withGUI && env->verbose)
    {
      memset(env->msg, '\b', strlen(env->msg));
      cout << env->msg;
    }

  va_list vl;
  va_start(vl, fmt);
  pwx_vsnprintf(env->msg, 255, fmt, vl);
  va_end(vl);

  if (env->withGUI)
    {
      env->screen->Clear();
      env->screen->Draw(sf::Sprite(env->image));

      // Set the text first, we need its size
      sf::String sMsg(env->msg, *(env->font), env->fontSize);
      sMsg.SetColor(sf::Color(::std::min(env->colHi.r, ::std::min(env->colMid.r, env->colLow.r)),
                              ::std::min(env->colHi.g, ::std::min(env->colMid.g, env->colLow.g)),
                              ::std::min(env->colHi.b, ::std::min(env->colMid.b, env->colLow.b)) ));
      sf::FloatRect txtRect(sMsg.GetRect());

      // Draw the text background box:
      uint32_t bWidth = static_cast<uint32_t>(pwx_round(txtRect.GetWidth() + 4));
      uint32_t bHeight= static_cast<uint32_t>(pwx_round(txtRect.GetHeight() + 4));
      sf::Vector2f boxPos(static_cast<float>(env->scrWidth  / 2 - bWidth / 2 - 2),
                          static_cast<float>(env->scrHeight - 2 - bHeight));
      sf::Vector2f txtPos(static_cast<float>(env->scrWidth  / 2 - bWidth / 2    ),
                          static_cast<float>(env->scrHeight     - bHeight));
      sf::Image  boxBg(bWidth, bHeight, sf::Color(::std::max(env->colHi.r, ::std::max(env->colMid.r, env->colLow.r)),
                                                  ::std::max(env->colHi.g, ::std::max(env->colMid.g, env->colLow.g)),
                                                  ::std::max(env->colHi.b, ::std::max(env->colMid.b, env->colLow.b)) ));
      sf::Sprite box(boxBg, boxPos);
      env->screen->Draw(box);

      // Set the text position:
      sMsg.SetPosition(txtPos);

      // Draw the text and display it:
      env->screen->Draw(sMsg);
      env->screen->Display();

      env->msgShown = -1;
    }
  else if (env->verbose)
    {
      cout << env->msg;
      cout.flush();
    }
}


int32_t showOSHelp(ENVIRONMENT * env)
{
  int32_t result = EXIT_SUCCESS;

  // We create the text surface on demand and keep it then
  if (!env->helpText)
    {
      try
        {
          // Create the helpText:
          env->helpText = new CHelpText();
          result = env->helpText->initialize(env);
        }
      catch (std::bad_alloc &e)
        {
          cerr << "Error creating help text : \"" << e.what() << "\"" << endl;
          result = EXIT_FAILURE;
        }
    }

  // We have a help text?
  if ((EXIT_SUCCESS == result) && env->helpText)
    {
      // We have it, tell environment to display it:
      env->helpText->generate(env);
      env->helpBoxShown = true;
    }

  return (result);
}


int32_t showOSStat(ENVIRONMENT * env)
{
  int32_t result = EXIT_SUCCESS;

  // We create the text surface on demand and keep it then
  if (!env->statsText)
    {
      try
        {
          // Create the statsText:
          env->statsText = new CStatsText();
          result = env->statsText->initialize(env);
        }
      catch (std::bad_alloc &e)
        {
          cerr << "Error creating stats text : \"" << e.what() << "\"" << endl;
          result = EXIT_FAILURE;
        }
    }

  // We have the On Screen Display?
  if ((EXIT_SUCCESS == result) && env->statsText)
    {
      env->statsText->generate(env);
      // We have it, tell environment to display it:
      env->statBoxShown = true;
    }

  return (result);
}


void showSeqPat(ENVIRONMENT * env, char * seqZtgt, char * seqWtgt)
{
  char chMsgZ[33] = "";
  char chMsgW[33] = "";

  if (env->dimensions >= 3)
    {
      // create string for Sequence Z
      switch (env->seqZ)
        {
          case eiZNone:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"static %g\"", static_cast<int32_t>(env->seqZ), env->offZ);
            break;
          case eiZincX:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"inc %g by X\"", static_cast<int32_t>(env->seqZ), env->modZ);
            break;
          case eiZincY:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"inc %g by Y\"", static_cast<int32_t>(env->seqZ), env->modZ);
            break;
          case eiZincW:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"inc %g by W\"", static_cast<int32_t>(env->seqZ), env->modZ);
            break;
          case eiZisX:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals X\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisY:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals Y\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisXY:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals X * Y\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisXYdW:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals (X * Y) / W\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisXYmW:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals (X * Y) %% W\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisXdY:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals X / Y\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisYdX:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals Y / X\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisXaY:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals X + Y\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisXsY:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals X - Y\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisYsX:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals Y - X\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisXmY:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals X %% Y\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisYmX:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals Y %% X\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZisW:
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"equals W\"", static_cast<int32_t>(env->seqZ));
            break;
          case eiZlast:
          default:
            cerr << "ERROR: seqZ reached unusable helper state!" << endl;
            env->seqZ = eiZNone;
            pwx_snprintf(chMsgZ, 32, "Seq Z: %d \"static %g\"", static_cast<int32_t>(env->seqZ), env->offZ);
        } // End of Sequence Z string creation
    } // End check for third dimension

  if (env->dimensions >= 4)
    {
      // create string for Sequence W
      switch (env->seqW)
        {
          case eiWNone:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"static %g\"", static_cast<int32_t>(env->seqW), env->offW);
            break;
          case eiWincX:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"inc %g by X\"", static_cast<int32_t>(env->seqW), env->modW);
            break;
          case eiWincY:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"inc %g by Y\"", static_cast<int32_t>(env->seqW), env->modW);
            break;
          case eiWincZ:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"inc %g by Z\"", static_cast<int32_t>(env->seqW), env->modW);
            break;
          case eiWisX:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals X\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisY:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals Y\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisXY:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals X * Y\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisXYdZ:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals (X * Y) / Z\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisXYmZ:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals (X * Y) %% Z\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisXdY:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals X / Y\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisYdX:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals Y / X\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisXaY:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals X + Y\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisXsY:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals X - Y\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisYsX:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals Y - X\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisXmY:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals X %% Y\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisYmX:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals Y %% X\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWisZ:
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"equals Z\"", static_cast<int32_t>(env->seqW));
            break;
          case eiWlast:
          default:
            cerr << "ERROR: seqW reached unusable helper state!" << endl;
            env->seqW = eiWNone;
            pwx_snprintf(chMsgW, 32, "Seq W: %d \"static %g\"", static_cast<int32_t>(env->seqW), env->offW);
        } // End of Sequence W string creation

    } // end check for fourth dimension

  // Now work with the result(s):
  if (seqWtgt && seqZtgt)
    {
      if (env->dimensions >= 3)
        pwx_snprintf(seqZtgt, 255, "  -> %s", chMsgZ);
      if (env->dimensions >= 4)
        pwx_snprintf(seqWtgt, 255, "  -> %s", chMsgW);
    }
  else if (env->dimensions >= 4)
    // Show both, we are 4D
    showMsg(env, "%s - %s", chMsgZ, chMsgW);
  else
    // Show Z only, we are 3D
    showMsg(env, "%s", chMsgZ);
}

void showState(ENVIRONMENT * env)
{
  pwx_snprintf(env->imgMsg, 255, "Texture %08d : %dx%d - Seed %d, Zoom %g, Smooth %g",
               env->imageNum, env->scrWidth, env->scrHeight, env->seed, env->spxZoom, env->spxSmoo);
  if (env->spxWave > 1)
    {
      pwx_snprintf(env->msg, 255, "(%d waves - reduct %g)", env->spxWave, env->spxRedu);
      showMsg(env, "%s %s", env->imgMsg, env->msg);
    }
  else
    showMsg(env, "%s", env->imgMsg);
  env->msgShown = -1;
}
