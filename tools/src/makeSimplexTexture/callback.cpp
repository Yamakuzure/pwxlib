#include "callback.h"
#include "environment.h"
#include "consoleui.h"

void cbDimension(const char *arg, void *env)
  {
    if (arg && strlen(arg) && env)
      {
        if (STREQ("3D", arg))
          static_cast<ENVIRONMENT *>(env)->dimensions = 3;
        else if (STREQ("4D", arg))
          static_cast<ENVIRONMENT *>(env)->dimensions = 4;
      }
  }

void cbColorHi(const char *arg, void *env)
  {
    if (arg && strlen(arg) && env)
      {
        int32_t value = parseHex(arg);
        ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
        xEnv->colHi.r = static_cast<uint8_t>((value & 0xff0000) >> 16);
        xEnv->colHi.g = static_cast<uint8_t>((value & 0x00ff00) >>  8);
        xEnv->colHi.b = static_cast<uint8_t>( value & 0x0000ff);
      }
  }

void cbColorMid(const char *arg, void *env)
  {
    if (arg && strlen(arg) && env)
      {
        int32_t value = parseHex(arg);
        ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
        xEnv->colMid.r = static_cast<uint8_t>((value & 0xff0000) >> 16);
        xEnv->colMid.g = static_cast<uint8_t>((value & 0x00ff00) >>  8);
        xEnv->colMid.b = static_cast<uint8_t>( value & 0x0000ff);
        xEnv->colMidEmpty = false;
      }
  }

void cbColorLow(const char *arg, void *env)
  {
    if (arg && strlen(arg) && env)
      {
        int32_t value = parseHex(arg);
        ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
        xEnv->colLow.r = static_cast<uint8_t>((value & 0xff0000) >> 16);
        xEnv->colLow.g = static_cast<uint8_t>((value & 0x00ff00) >>  8);
        xEnv->colLow.b = static_cast<uint8_t>( value & 0x0000ff);
        xEnv->colLowEmpty = true;
      }
  }

void cbHelpVersion(const char *arg, void *env)
  {
    if (arg && strlen(arg) && env)
      {
        ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
        if (STREQ(arg, "help"))
          showHelp(xEnv);
        else if (STREQ(arg, "version"))
          showVersion(xEnv);
        xEnv->doWork = false;
      }
  }

void cbSetSequence(const char *arg, void *env)
  {
    if (arg && strlen(arg) && env)
      {
        ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
        bool       need4D = false;
        if (STREQ(arg, "zIncModX"))
          xEnv->seqZ = eiZincX;
        else if (STREQ(arg, "zIncModY"))
          xEnv->seqZ = eiZincY;
        else if (STREQ(arg, "zIncModW"))
          {
            need4D     = true;
            xEnv->seqZ = eiZincW;
          }
        else if (STREQ(arg, "zIsX"))
          xEnv->seqZ = eiZisX;
        else if (STREQ(arg, "zIsY"))
          xEnv->seqZ = eiZisY;
        else if (STREQ(arg, "zIsW"))
          {
            need4D     = true;
            xEnv->seqZ = eiZisW;
          }
        else if (STREQ(arg, "zIsXY"))
          xEnv->seqZ = eiZisXY;
        else if (STREQ(arg, "zIsXYdW"))
          {
            need4D     = true;
            xEnv->seqZ = eiZisXYdW;
          }
        else if (STREQ(arg, "zIsXYmW"))
          {
            need4D     = true;
            xEnv->seqZ = eiZisXYmW;
          }
        else if (STREQ(arg, "zIsXdY"))
          xEnv->seqZ = eiZisXdY;
        else if (STREQ(arg, "zIsYdX"))
          xEnv->seqZ = eiZisYdX;
        else if (STREQ(arg, "zIsXaY"))
          xEnv->seqZ = eiZisXaY;
        else if (STREQ(arg, "zIsXsY"))
          xEnv->seqZ = eiZisXsY;
        else if (STREQ(arg, "zIsYsX"))
          xEnv->seqZ = eiZisYsX;
        else if (STREQ(arg, "zIsXmY"))
          xEnv->seqZ = eiZisXmY;
        else if (STREQ(arg, "zIsYmX"))
          xEnv->seqZ = eiZisYmX;
        // Sequence modifiers for Coordinate W
        else if (STREQ(arg, "wIncModX"))
          {
            need4D     = true;
            xEnv->seqW = eiWincX;
          }
        else if (STREQ(arg, "wIncModY"))
          {
            need4D     = true;
            xEnv->seqW = eiWincY;
          }
        else if (STREQ(arg, "wIncModZ"))
          {
            need4D     = true;
            xEnv->seqW = eiWincZ;
          }
        else if (STREQ(arg, "wIsX"))
          {
            need4D     = true;
            xEnv->seqW = eiWisX;
          }
        else if (STREQ(arg, "wIsY"))
          {
            need4D     = true;
            xEnv->seqW = eiWisY;
          }
        else if (STREQ(arg, "wIsZ"))
          {
            need4D     = true;
            xEnv->seqW = eiWisZ;
          }
        else if (STREQ(arg, "wIsXY"))
          {
            need4D     = true;
            xEnv->seqW = eiWisXY;
          }
        else if (STREQ(arg, "wIsXYdZ"))
          {
            need4D     = true;
            xEnv->seqW = eiWisXYdZ;
          }
        else if (STREQ(arg, "wIsXYmZ"))
          {
            need4D     = true;
            xEnv->seqW = eiWisXYmZ;
          }
        else if (STREQ(arg, "wIsXdY"))
          {
            need4D     = true;
            xEnv->seqW = eiWisXdY;
          }
        else if (STREQ(arg, "wIsYdX"))
          {
            need4D     = true;
            xEnv->seqW = eiWisYdX;
          }
        else if (STREQ(arg, "wIsXaY"))
          {
            need4D     = true;
            xEnv->seqW = eiWisXaY;
          }
        else if (STREQ(arg, "wIsXsY"))
          {
            need4D     = true;
            xEnv->seqW = eiWisXsY;
          }
        else if (STREQ(arg, "wIsYsX"))
          {
            need4D     = true;
            xEnv->seqW = eiWisYsX;
          }
        else if (STREQ(arg, "wIsXmY"))
          {
            need4D     = true;
            xEnv->seqW = eiWisXmY;
          }
        else if (STREQ(arg, "wIsYmX"))
          {
            need4D     = true;
            xEnv->seqW = eiWisYmX;
          }

        // Now the dimensions need to be set to the required minimum:
        if ((xEnv->dimensions < 3) && !need4D)
          xEnv->dimensions = 3;
        else if ((xEnv->dimensions < 4) && need4D)
          xEnv->dimensions = 4;
      }
  }
