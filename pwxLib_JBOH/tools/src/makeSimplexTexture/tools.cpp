#include "tools.h"
#include "icon.h"

int32_t initSFML(ENVIRONMENT * env)
{
  int32_t result = EXIT_SUCCESS;

  // Create Window:
  string title = "MakeSimplexTexture V";
  title += env->getVersion();
  title += " (c) PrydeWorX 2007-2012";
  PWX_TRY(env->screen = new sf::RenderWindow(sf::VideoMode(env->scrWidth, env->scrHeight), title))
  catch (std::bad_alloc &e)
    {
      result = EXIT_FAILURE;
      cerr << "Unable to init SFML RenderWindow: \"" << e.what() << "\"" << endl;
    }

  // Set Icon
  env->screen->SetIcon(mkst_icon.width, mkst_icon.height, mkst_icon.pixel_data);

  // Load font:
  if (EXIT_SUCCESS == result)
    {
      std::string fontpath = FONT_PATH;
      fontpath += FONT_SEP;
      fontpath += FONT_NAME;

      if (fontpath.size() > 2)
        {
          try
            {
              env->font = new sf::Font;
              if (env->font->LoadFromFile(std::string(fontpath), 12))
                env->fontSize = static_cast<float>(env->font->GetCharacterSize());
              else
                {
                  cerr << "Failed to load \"" << FONT_PATH << FONT_SEP << FONT_NAME << "\"\n";
                  if (env->font) delete env->font;
                }
            }
          catch (std::bad_alloc &e)
            {
              cerr << "Error creating font : " << e.what() << endl;
            }
        }
      else
        cerr << "No font specified!\n";

      if (!env->font)
        {
          cerr << "--> Using built in Arial font instead." << endl;
          env->font = new sf::Font(sf::Font::GetDefaultFont());
        }
    }

  // finally set the images to our screen width and height:
  result = (EXIT_SUCCESS == result) && env->image.Create(env->scrWidth, env->scrHeight) ? EXIT_SUCCESS : EXIT_FAILURE;
  result = (EXIT_SUCCESS == result) && env->bumpMap.Create(env->scrWidth, env->scrHeight) ? EXIT_SUCCESS : EXIT_FAILURE;

  return (result);
}


int32_t  parseHex(const char * src)
{
  int32_t result = 0;

  if (src)
    {
      int32_t pos    = ((src[0] == '0') && (src[1] == 'x')) ? 2 : 0;
      int32_t length = strlen(src) - pos;

      if (length > 8)
        length = 8; // because 8 * 4 bit shift = 32 bit

      while (length)
        {
          --length; // dec first, or everything will be shifted left 4 bit
          if ((src[pos] > 0x2f) && (src[pos] < 0x3a))
            result |= (src[pos] - 0x30) << (4 * length);  // Add number
          else if ((src[pos] > 0x40) && (src[pos] < 0x47))
            result |= (src[pos] - 0x37) << (4 * length);  // Add upper case letter
          else if ((src[pos] > 0x60) && (src[pos] < 0x67))
            result |= (src[pos] - 0x57) << (4 * length);  // Add lower case letter
          else
            length = 0; // break here!
          ++pos;
        }
    }

  return (result);
}
