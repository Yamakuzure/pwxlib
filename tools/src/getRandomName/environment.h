#ifndef PWX_GETRANDOMNAME_ENVIRONMENT_H
#define PWX_GETRANDOMNAME_ENVIRONMENT_H

#include <cmath>
#include <cstdlib>
#include <pwxLib/tools/DefaultDefines.h>
using std::string;

/** @brief ENVIRONMENT
  *
  * This is not a real class, but a "classed" pImpl struct.
  * It is used to circumvent the neccessity to add too many arguments to
  * global functions.
  *
  * The reason to not using a struct is to be able to inherit from pwx::Uncopyable.
  * ...another reason is that in C++, "struct" is a synonym for "class" anyway...
**/

class ENVIRONMENT: private pwx::Uncopyable
{
public:
  /** Default constructor */
  explicit ENVIRONMENT(int32_t aSeed = 0);

  /** struct-simulating public members **/
  int32_t    count;        //!< the number of names to generate
  bool   doWork;       //!< is set to false if no work is to be done
  string filename;     //!< the name of the file to write the names to
  double offsetX;      //!< Can be set by command line argument, defaults to -seed and +seed
  double offsetY;      //!< Can be set by command line argument, defaults to -seed and +seed
  double offsetZ;      //!< Can be set by command line argument, defaults to -seed and +seed
  double offsetW;      //!< Can be set by command line argument, defaults to -seed and +seed
  int32_t    maxChars;     //!< Can be set by command line argument, defaults to 15
  int32_t    maxParts;     //!< Can be set by command line argument, defaults to 3
  int32_t    maxSyllables; //!< Can be set by command line argument, defaults to 5
  double modX;         //!< X-Coordinate modifier, can be set by modx argument, random -5 to +5 otherwise
  double modY;         //!< Y-Coordinate modifier, can be set by modx argument, random -5 to +5 otherwise
  double modZ;         //!< Z-Coordinate modifier, can be set by modx argument, random -5 to +5 otherwise
  double modW;         //!< W-Coordinate modifier, can be set by modx argument, random -5 to +5 otherwise
  bool   useModVals;   //!< If one mod* argument is used, this becomes true. Negated by useRandom false.
  bool   useRandom;    //!< If set to false by -n/--norandom, all four mods are used as +1.0
  bool   useSeedOff;   //!< Is set to false by any of the x/y/z/w options
  int32_t    seed;         //!< If set by command line argument, sets a new seed for RNG

  /** non struct methods **/
  const char * getVersion() const;
  void         setOffsets(double dX = 0.0, double dY = 0.0, double dZ = 0.0, double dW = 0.0, bool useArg = false);

protected:
private:
  string version;
};

/** these are callback functions for pwxArgs **/
void cbSetX    (const char *value, void *env);
void cbSetY    (const char *value, void *env);
void cbSetZ    (const char *value, void *env);
void cbSetW    (const char *value, void *env);
void cbSetModX (const char *value, void *env);
void cbSetModY (const char *value, void *env);
void cbSetModZ (const char *value, void *env);
void cbSetModW (const char *value, void *env);

#endif // PWX_GETRANDOMNAME_ENVIRONMENT_H


