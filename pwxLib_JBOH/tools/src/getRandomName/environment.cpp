#include "environment.h"

/** @brief default ctor
**/
ENVIRONMENT::ENVIRONMENT(int32_t aSeed):count(1),doWork(true),filename(""),
  offsetX(0.0),offsetY(0.0),offsetZ(0.0),offsetW(0.0),
  maxChars(15),maxParts(3),maxSyllables(5),
  modX(0.0),modY(0.0),modZ(0.0),modW(0.0),
  useModVals(false),useRandom(true),useSeedOff(true),seed(aSeed),
  version("0.8.6")
{ }

/** @brief return version as c-string
  *
  * @todo: document this function
**/
const char * ENVIRONMENT::getVersion() const
{
  return(version.c_str());
}

/** @brief setOffsets
  *
  * simply sets offsetX/Y/Z/W according to mod*, useModVals and useRandom
**/
void ENVIRONMENT::setOffsets(double dX, double dY, double dZ, double dW, bool useArg)
{
  if (useArg)
    {
      offsetX = dX;
      offsetY = dY;
      offsetZ = dZ;
      offsetW = dW;
    }
  else if (useModVals)
    {
      offsetX += modX;
      offsetY += modY;
      offsetZ += modZ;
      offsetW += modW;
    }
  else
    {
      offsetX++;
      offsetY++;
      offsetZ++;
      offsetW++;
    }
}

/** these are callback functions for pwxArgs **/
void cbSetX    (const char *value, void *env)
  {
    ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
    xEnv->offsetX     = atof(value);
    xEnv->useSeedOff  = false;
  }
void cbSetY    (const char *value, void *env)
  {
    ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
    xEnv->offsetY     = atof(value);
    xEnv->useSeedOff  = false;
  }
void cbSetZ    (const char *value, void *env)
  {
    ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
    xEnv->offsetZ     = atof(value);
    xEnv->useSeedOff  = false;
  }
void cbSetW    (const char *value, void *env)
  {
    ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
    xEnv->offsetW     = atof(value);
    xEnv->useSeedOff  = false;
  }
void cbSetModX (const char *value, void *env)
  {
    ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
    xEnv->modX        = atof(value);
    xEnv->useModVals  = true;
    xEnv->useRandom   = false;
  }
void cbSetModY (const char *value, void *env)
  {
    ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
    xEnv->modX        = atof(value);
    xEnv->useModVals  = true;
    xEnv->useRandom   = false;
  }
void cbSetModZ (const char *value, void *env)
  {
    ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
    xEnv->modX        = atof(value);
    xEnv->useModVals  = true;
    xEnv->useRandom   = false;
  }
void cbSetModW (const char *value, void *env)
  {
    ENVIRONMENT *xEnv = static_cast<ENVIRONMENT *>(env);
    xEnv->modX        = atof(value);
    xEnv->useModVals  = true;
    xEnv->useRandom   = false;
  }
