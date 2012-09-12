#ifndef PWX_MAKESIMPLEXTEXTURE_ENVIRONMENT_H
#define PWX_MAKESIMPLEXTEXTURE_ENVIRONMENT_H


#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <cmath>
#include <string>
using std::string;
#include <cstdio> // for sprintf
#if defined (_MSC_VER)
#  include <io.h>   // for _access (used by pwx_file_exists())
#endif

class CHelpText;
class CStatsText;

#include "textbox.h" // Adds On Screen help and stats
#include "wzseq.h"

/** @struct ENVIRONMENT
  * @brief struct to keep general values together that are used in the programs functions
**/
struct ENVIRONMENT: public pwx::Lockable, private pwx::Uncopyable
{
  double            borderHi;    //!< Upper border, defaults to the simplex maximum of  1.0
  double            borderLo;    //!< Lower border, defaults to the simplex minimum of -1.0
  double            borderMid;   //!< Middle border, set to the median of the lower and upper border
  sf::Image         bumpMap;     //!< Optional bumpmapping for the generated texture
  sf::Color         colHi;       //!< Must have color, if alone uses the full range, otherwise the positive Simplex value
  sf::Color         colMid;      //!< optional middle color, takes over the zero value
  sf::Color         colLow;      //!< optional low color, takes over negative colors
  bool              colLowEmpty; //!< set to false if the user sets a color via command line
  bool              colMidEmpty; //!< set to false if the user sets a color via command line
  int32_t           dimensions;  //!< can be 2, 3 or 4
  bool              doWork;      //!< is set to false if no work is to be done
  sf::Font         *font;        //!< The font to be used for the display
  float             fontSize;    //!< Base size of the font, used to determine the text box sizes
  bool              helpBoxShown;//!< set to true if the help box is shown
  CHelpText        *helpText;    //!< Help text for OS display
  sf::Image         image;       //!< The image to be rendered
  int32_t           imageNum;    //!< used to count the available images, to not overwrite one
  char              imgMsg[256]; //!< used to display the stats of the current image
  bool              mainShown;   //!< True if the texture, false if the bumpmap is shown on screen
  double            modZ;        //!< modification value for offZ, used for z/Z key and iterations (default 1.0)
  double            modW;        //!< modification value for offW, used for w/W key and iterations (default 1.0)
  char              msg[256];    //!< message to display in the window title
  int32_t           msgShown;    //!< showMsg sets to -1, workLoop then triggers timer and sets to 1, showState sets to 0
  int32_t           numThreads;  //!< Number of threads to spawn for the render calculations. Default is 8
  double            offX;        //!< x-offset
  double            offY;        //!< y-offset
  double            offZ;        //!< z-offset enabled by dimension >= 3
  double            offW;        //!< w-offset enabled by dimension == 4
  bool              removeBox;   //!< Set to true if a txtBox is shown than shall be removed
  sf::RenderWindow *screen;      //!< the screen to be created
  int32_t           scrHeight;   //!< height of the screen
  int32_t           scrSize;     //!< simply a shortcut to height * width
  int32_t           scrWidth;    //!< screen width
  eSequenceW        seqW;        //!< determines the sequence of the w coordinate
  eSequenceZ        seqZ;        //!< determines the sequence of the z coordinate
  double            spxRedu;     //!< Simplex Reduction Value, defaults to 1.0
  double            spxSmoo;     //!< Simplex Smooth Value, defaults to 1.0
  int32_t           spxWave;     //!< Simplex Waves Value, defaults to 1
  double            spxZoom;     //!< Simplex Zoom, defaults to 4.0
  bool              statBoxShown;//!< set to true if the stats box is shown
  CStatsText       *statsText;   //!< Text to display by showOSStat()
  int32_t           seed;        //!< If set by command line argument, sets a new seed for RNG
  string            targetExt;   //!< target extension, determines image type
  sf::Thread      **thread;      //!< The threads themselves.
  int32_t          *threadPrg;   //!< Threads write their progress in this
  bool             *threadRun;   //!< Threads set it to true when they start and to false when they end
  bool              verbose;     //!< if set to false (-q/--quiet) when batch mode is enabled, nothing is printed out
  bool              withBump;    //!< Save a textureXXXX.bump.tif alongside the original texture. Defaults to true!
  bool              withGUI;     //!< When set to false by -B/--batch option, no SDL is used

  /* --- non-struct methods --- */
  void         clearThreads();
  const char * getVersion() const;

  /** @brief setCoordsByX
    *
    * Set dX to dW according to sequences concerning x
    *
    * The relevant sequences are:
    *  eiZincX,    //!< Z = modZ + (X * modZ)
    *  eiZincW,    //!< Z = modZ + (W * modZ)
    *  eiZisX,     //!< Z equals X
    *  eiZisW,     //!< Z equals W
    *  eiZisXY,    //!< Z equals X * Y
    *  eiZisXYdW,  //!< Z equals (X * Y) / W
    *  eiZisXYmW,  //!< Z equals (X * Y) % W
    *  eiZisXdY,   //!< Z equals X / Y
    *  eiZisYdX,   //!< Z equals Y / X
    *  eiZisXaY,   //!< Z equals X + Y
    *  eiZisXsY,   //!< Z equals X - Y
    *  eiZisYsX,   //!< Z equals Y - X
    *  eiZisXmY,   //!< Z equals X % Y
    *  eiZisYmX,   //!< Z equals Y % Y
    * and the same with seqW
  **/
  void setCoordsByX(int32_t x, int32_t y, double &dX, double &dY, double &dZ, double &dW)
    {
      dX = offX + static_cast<double>(x);

      if ((seqZ != eiZNone) || (seqW != eiWNone))
        {
          bool zIsChanged = false;
          bool wIsChanged = false;
          int32_t w, xy, z;

          // 1) Process seqZ
          if (seqZ != eiZNone)
            {
              zIsChanged = true;
              switch (seqZ)
                {
                  case eiZincX:
                    dZ = modZ + (modZ * dX);
                    break;
                  case eiZisX:
                    dZ = dX;
                    break;
                  case eiZisXY:
                    dZ = offZ + (dX * dY);
                    break;
                  case eiZisXYdW:
                    dZ = offZ + (dW ? (dX * dY) / dW : 0);
                    break;
                  case eiZisXYmW:
                    w  = static_cast<int32_t>(pwx_round(dW));
                    xy = static_cast<int32_t>(pwx_round(dX * dY));
                    dZ = offZ + (w ? static_cast<double>(xy % w) : 0);
                    break;
                  case eiZisXdY:
                    dZ = offZ + (y ? static_cast<double>(x) / static_cast<double>(y) : 0);
                    break;
                  case eiZisYdX:
                    dZ = offZ + (x ? static_cast<double>(y) / static_cast<double>(x) : 0);
                    break;
                  case eiZisXaY:
                    dZ = offZ + dX + dY;
                    break;
                  case eiZisXsY:
                    dZ = offZ + (dX - dY);
                    break;
                  case eiZisYsX:
                    dZ = offZ + (dY - dX);
                    break;
                  case eiZisXmY:
                    dZ = offZ + (y > 0 ? static_cast<double>(x % y) : 0);
                    break;
                  case eiZisYmX:
                    dZ = offZ + (x > 0 ? static_cast<double>(y % x) : 0);
                    break;
                  // The next two are processed later if, and only if, W is modified
                  case eiZincW:
                  case eiZisW:
                  // the next two are already processed by setCoordsByY()
                  case eiZincY:
                  case eiZisY:
                  default:
                    zIsChanged = false;
                    break;
                } // end switch (seqZ)
            } // End of seqZ processing

          // 2) Process seqW
          if (seqW != eiWNone)
            {
              wIsChanged = true;
              switch (seqW)
                {
                  case eiWincX:
                    dW = modW + (dX * modW);
                    break;
                  case eiWisX:
                    dW = dX;
                    break;
                  case eiWisXY:
                    dW = offW + (dX * dY);
                    break;
                  case eiWisXYdZ:
                    dW = offW + (dZ ? (dX * dY) / dZ : 0);
                    break;
                  case eiWisXYmZ:
                    z  = static_cast<int32_t>(pwx_round(dZ));
                    xy = static_cast<int32_t>(pwx_round(dX * dY));
                    dW = offW + (z ? static_cast<double>(xy % z) : 0);
                    break;
                  case eiWisXdY:
                    dW = offW + (y ? static_cast<double>(x) / static_cast<double>(y) : 0);
                    break;
                  case eiWisYdX:
                    dW = offW + (x ? static_cast<double>(y) / static_cast<double>(x) : 0);
                    break;
                  case eiWisXaY:
                    dW = offW + dX + dY;
                    break;
                  case eiWisXsY:
                    dW = offW + (dX - dY);
                    break;
                  case eiWisYsX:
                    dW = offW + (dY - dX);
                    break;
                  case eiWisXmY:
                    dW = offW + (y > 0 ? static_cast<double>(x % y) : 0);
                    break;
                  case eiWisYmX:
                    dW = offW + (x > 0 ? static_cast<double>(y % x) : 0);
                    break;
                  case eiWincZ:
                    if (zIsChanged)
                      dW = modW + (dZ * modW);
                    else
                      wIsChanged = false;
                    break;
                  case eiWisZ:
                    if (zIsChanged)
                      dW = dZ;
                    else
                      wIsChanged = false;
                    break;
                  // the next two are already processed by setCoordsByY()
                  case eiWincY:
                  case eiWisY:
                  default:
                    wIsChanged = false;
                    break;
                } // end switch (seqW)
            } // End of seqW processing

          // 3) Mod Z if W is changed
          if (wIsChanged && (seqZ == eiZincW))
            dZ = modZ + (dW * modZ);
          else if (wIsChanged && (seqZ == eiZisW))
            dZ = dW;

          // Processing of W if Z changed is already done in 2)

        } // End processing either seqZ, seqW or both
    }

  /** @brief setCoordsByY
    *
    * Set dY, dZ, dW according to sequences concerning y
    * This is the very limited version for the outer loop, excluding x
    *
    * The relevant sequences are:
    *  eiZincY,    //!< Z = modZ + (Y * modZ)
    *  eiZincW,    //!< Z = modZ + (W * modZ)
    *  eiZisY,     //!< Z equals Y
    *  eiZisW,     //!< Z equals W
    * and the same with seqW
  **/
  void setCoordsByY(int32_t y, double &dY, double &dZ, double &dW)
    {
      dY = offY + static_cast<double>(y);
      if ((seqZ != eiZNone) || (seqW != eiWNone))
        {
          bool zIsChanged = false;
          bool wIsChanged = false;

          // 1) Use seqZ
          if (seqZ == eiZincY)
            {
              dZ = modZ + (dY * modZ);
              zIsChanged = true;
            }
          else if (seqZ == eiZisY)
            {
              dZ = dY;
              zIsChanged = true;
            }

          // 2) Use seqW
          if (seqW == eiWincY)
            {
              dW = modW + (dY * modW);
              wIsChanged = true;
            }
          else if (seqW == eiWisY)
            {
              dW = dY;
              wIsChanged = true;
            }

          // 3) Mod Z if W is changed
          if (wIsChanged && (seqZ == eiZincW))
            dZ = modZ * (dW * modZ);
          else if (wIsChanged && (seqZ == eiZisW))
            dZ = dW;

          // 4) Mod W if Z is changed
          if (zIsChanged && (seqW == eiWincZ))
            dW = modW + (dZ * modW);
          else if (zIsChanged && (seqW == eiWisZ))
            dW = dZ;
        }
      // That's it.
    }

  /** @brief setImageNum
    *
    * Just a little method that sets imageNum to the lowest possible value with
    * the current targetExt
  **/
  void setImageNum()
    {
      char fname[32] = "";
      imageNum = 0;
      do
        pwx_snprintf(fname, 31, "texture%08d.%s", ++imageNum, targetExt.c_str());
      while (pwx_file_exists(fname));
    }

private:
  string version;

public:
  /** @brief Default constructor **/
  explicit ENVIRONMENT(int32_t aSeed = 0):
      borderHi(1.0),borderLo(-1.0),borderMid(0.0),
#if defined(PWX_HAS_CXX11_INIT)
      bumpMap({}),
#endif
      colHi(255, 255, 0),colMid(0, 0, 0),colLow(0, 0, 0),
      colLowEmpty(true),colMidEmpty(true),
      dimensions(2),doWork(true),font(NULL),fontSize(12.),helpBoxShown(false),helpText(NULL),
#if defined(PWX_HAS_CXX11_INIT)
      image({}),
#endif
      imageNum(0),mainShown(true),modZ(1.0),modW(1.0),
      msgShown(0),numThreads(8), offX(0.0),offY(0.0),offZ(0.0),offW(0.0),removeBox(false),
      screen(NULL),scrHeight(600),scrSize(480000),scrWidth(800),
      seqW(eiWNone),seqZ(eiZNone),spxRedu(1.0),spxSmoo(1.0),spxWave(1),spxZoom(9.0),
      statBoxShown(false),statsText(NULL),
      seed(aSeed),targetExt("png"),thread(NULL), threadPrg(NULL), threadRun(NULL),
      verbose(true),withBump(true),withGUI(true),
      version("0.8.6")
    {
      memset(msg, 0, 256);
      memset(imgMsg, 0, 256);

      // Now find first available imagenum:
      setImageNum();
    }

  // The dtor has to be outline, or we can't get rid of the forward declared text boxes.
  ~ENVIRONMENT();

private:
  /* --- absolutely no copying allowed --- */
  ENVIRONMENT(ENVIRONMENT&);
  ENVIRONMENT &operator=(ENVIRONMENT&);
};

/// @brief tiny little helpers to let threads know their number
struct threadEnv : private pwx::Uncopyable
{
  ENVIRONMENT *env;
  int32_t      threadNum;   //!< This is used to tell threads which number they are for displaying the progress

  explicit threadEnv(ENVIRONMENT *aEnv, int32_t aNum):env(aEnv),threadNum(aNum) {}
  ~threadEnv() { env = NULL; }

private:
  /* --- do not copy --- */
  threadEnv(threadEnv&);
  threadEnv &operator=(threadEnv&);
};

#endif // PWX_MAKESIMPLEXTEXTURE_ENVIRONMENT_H
