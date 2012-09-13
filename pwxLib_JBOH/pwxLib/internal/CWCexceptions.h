#pragma once
#ifndef PWX_CWCEXCEPTIONS_H_INCLUDED
#define PWX_CWCEXCEPTIONS_H_INCLUDED 1

/** @internal
  * @file pwxMemRingExceptions
  *
  * @brief Declarations and definitions of exceptions used by pwxMemRing
  *
  * @see CMemRingFactory.h for documentation and history
  *
**/

#ifndef PWX_CWAVECOLOR_H_INCLUDED
# error "Do not include CWCexceptions.h, include CWaveColor.h!!"
#endif // Check for main file

#include "pwxLib/tools/Exception.h"

namespace pwx {
namespace CWC {
// note: The namespaces are documented in tools/CWaveColor.h!

/** @brief std::bad_alloc caught after sWave new operator execution
**/
class cantCreateWaveEntry : public ::pwx::Exception
{
public:
  explicit cantCreateWaveEntry (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                                const char *const aDesc): Exception ("cantCreateWaveEntry", aWhat, aWhere, prettyfunc, aDesc) { }
  cantCreateWaveEntry (const cantCreateWaveEntry &e):Exception(e) { }
  PWX_INLINE virtual ~cantCreateWaveEntry() throw();
};

// --destructors heaven ---
cantCreateWaveEntry::~cantCreateWaveEntry() throw() { }

} // End of namespace CWC
} // End of namespace pwx


#endif // PWX_CWCEXCEPTIONS_H_INCLUDED

