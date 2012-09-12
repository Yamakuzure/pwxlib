#include "environment.h"

/** @brief return version as c-string
  *
  * @todo: document this function
  */
const char * ENVIRONMENT::getVersion() const
{
  return(version.c_str());
}

/** @brief Default destructor **/
ENVIRONMENT::~ENVIRONMENT()
  {
    // Be dead safe about our threads _first_!
    if (thread)
      {
        clearThreads();
        for (int32_t tNum = 0; tNum < numThreads; ++tNum)
          {
            if (thread[tNum])
              {
                delete thread[tNum];
                thread[tNum] = NULL;
              }
          }
        delete [] thread;
      }
    if (threadPrg)  delete [] threadPrg;
    if (threadRun)  delete [] threadRun;
    if (helpText)   delete helpText;
    if (statsText)  delete statsText;
    if (screen)     delete screen;
    if (font)       delete font;
  }

/// @brief simple function that finishes and clears all threads
void ENVIRONMENT::clearThreads()
  {
    for (int32_t tNum = 0; tNum < numThreads; ++tNum)
      {
        if (threadRun[tNum] && thread[tNum])
          thread[tNum]->Wait();

        threadRun[tNum] = false;

        if (thread[tNum])
          {
            delete thread[tNum];
            thread[tNum] = NULL;
          }
      }
  }
