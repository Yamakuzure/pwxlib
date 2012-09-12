#include "workloop.h"
#if defined(_MSC_VER)
#  include <stdlib.h>
#else
#  include <unistd.h>
#endif // geting sleep()

int32_t workLoop(ENVIRONMENT * env)
{
  int32_t   result  = EXIT_SUCCESS;
  bool      done    = false;
  bool      render  = false;
  double    elapsed = 0.0;
  sf::Event event;
  sf::Clock clock;
  sf::Event::KeyEvent key;

  while (!done && (EXIT_SUCCESS == result) && env->screen->IsOpened())
    {
      /* ======================== *
       * === A) Proces events === *
       * ======================== */
      // message processing loop
      while (env->screen->GetEvent(event))
        {
          /// 1.: Check whether to quit:
          if (sf::Event::Closed == event.Type)
            {
              env->screen->Close();
              done = true;
            }

          /// 2.: Check Key presses:
          else if (sf::Event::KeyPressed == event.Type)
            {
              if (env->helpBoxShown)
                {
                  // This only switches back to the image:
                  env->helpBoxShown = false;
                  env->removeBox    = true;
                }
              else if (env->statBoxShown)
                {
                  // This only switches back to the image:
                  env->statBoxShown = false;
                  env->removeBox    = true;
                }
              else
                {
                  // Otherwise process key presses:
                  key = event.Key;
                  switch (key.Code)
                    {
                      case sf::Key::Left:
                        env->offX -= key.Control ? 10.0 : 1.0;
                        showMsg(env, "New Offset X: %g", env->offX);
                        break;
                      case sf::Key::Right:
                        env->offX += key.Control ? 10.0 : 1.0;
                        showMsg(env, "New Offset X: %g", env->offX);
                        break;
                      case sf::Key::Down:
                        env->offY -= key.Control ? 10.0 : 1.0;
                        showMsg(env, "New Offset Y: %g", env->offY);
                        break;
                      case sf::Key::Up:
                        env->offY += key.Control ? 10.0 : 1.0;
                        showMsg(env, "New Offset Y: %g", env->offY);
                        break;
                      case sf::Key::Escape:
                        env->screen->Close();
                        done = true;
                        break;
                      case sf::Key::H:
                        result = showOSHelp(env);
                        break;
                      case sf::Key::S:
                        result = saveTexture(env);
                        break;
                      case sf::Key::Space:
                        result = swapImage(env);
                        break;
                      case sf::Key::Tab:
                        result = showOSStat(env);
                        break;
                      case sf::Key::R:
                        if (key.Shift)
                          render = true;
                        break;
                      case sf::Key::W:
                        if (key.Alt)
                          {
                            // Change sequence pattern
                            if (key.Shift)
                              env->seqW--;
                            else
                              env->seqW++;
                            showSeqPat(env);
                          } // end of changing sequence
                        else
                          {
                            // change offset
                            if (key.Shift)
                              env->offW -= key.Control ? 10.0 * env->modW : env->modW;
                            else
                              env->offW += key.Control ? 10.0 * env->modW : env->modW;
                            showMsg(env, "New Offset W: %g", env->offW);
                          } // end of changing offset
                        break;
                      case sf::Key::Z:
                        if (key.Alt)
                          {
                            // Change sequence pattern
                            if (key.Shift)
                              {
                                env->seqZ--;
                                while ( (env->dimensions < 4)
                                      &&( (env->seqZ == eiZincW)
                                        ||(env->seqZ == eiZisW)
                                        ||(env->seqZ == eiZisXYdW)
                                        ||(env->seqZ == eiZisXYmW)
                                      ) )
                                  env->seqZ--; // Skip sequencers that need a dimension not set
                              }
                            else
                              {
                                env->seqZ++;
                                while ( (env->dimensions < 4)
                                      &&( (env->seqZ == eiZincW)
                                        ||(env->seqZ == eiZisW)
                                        ||(env->seqZ == eiZisXYdW)
                                        ||(env->seqZ == eiZisXYmW)
                                      ) )
                                  env->seqZ++; // Skip sequencers that need a dimension not set
                              }
                            showSeqPat(env);
                          } // End of changing sequence
                        else
                          {
                            // change offset
                            if (key.Shift)
                              env->offZ -= key.Control ? 10.0 * env->modZ : env->modZ;
                            else
                              env->offZ += key.Control ? 10.0 * env->modZ : env->modZ;
                            showMsg(env, "New Offset Z: %g", env->offZ);
                          } // End of changing offset
                        break;
                      case sf::Key::D:
                        if (key.Shift)
                          env->dimensions -= env->dimensions > 2 ? 1 : 0;
                        else
                          env->dimensions += env->dimensions < 4 ? 1 : 0;
                        showMsg(env, "New Number of dimensions: %d", env->dimensions);
                        break;
                      default:
                        // nothing....
                        break;
                    } // end of switching the key code
                } // end of nothing shown
            }
        } // end of message processing
      // Have we set a message?
      if (env->msgShown < 0)
        {
          // Yes...
          env->msgShown = 1;
          clock.Reset();
          elapsed = 0.0;
        }
      // We have to reset the elapsed time if we show the help or stat box,
      // those should not disappear automatically:
      else if ( (elapsed > 5.0)
             && (env->helpBoxShown || env->statBoxShown) )
        {
          elapsed = 0.0;
          clock.Reset();
        }
      // Otherwise pick up the time:
      else
        elapsed = clock.GetElapsedTime();

      /* ======================== *
       * === B) draw screen   === *
       * ======================== */
      if (EXIT_SUCCESS == result && !done
        && ((elapsed > 5.0 && env->msgShown) || env->helpBoxShown || env->statBoxShown || env->removeBox) )
        {
          env->screen->Clear();
          // The textboxes are drawn over the image
          env->screen->Draw(sf::Sprite(env->image));
          if (env->helpBoxShown)
            {
              sf::Sprite box(*(env->helpText->txtBox), sf::Vector2f(5.0, 5.0));
              env->screen->Draw(box);
              for (int i = 0; i < env->helpText->txtSize; ++i)
                env->screen->Draw(env->helpText->txtContent[i]);
            }
          else if (env->statBoxShown)
            {
              sf::Sprite box(*(env->statsText->txtBox), sf::Vector2f(5.0, 5.0));
              env->screen->Draw(box);
              for (int i = 0; i < env->statsText->txtSize; ++i)
                env->screen->Draw(env->statsText->txtContent[i]);
            }
          else
            {
              // we have a reset.
              env->msgShown  = 0;
              env->removeBox = false;
            }
          // finally, update the screen
          env->screen->Display();
        }

      /* ======================== *
       * === B) render image  === *
       * ======================== */
      if (render && !done)
        result = loadNewImage(env);

      /* ======================== *
       * === C) waste time    === *
       * ======================== */
      if (!done)
        {
          if (render)
            render = false;
          else
            pwx_sleep(100); // I daresay ten polls per second should be enough.
        }
    } // end main loop

  return (result);
}
