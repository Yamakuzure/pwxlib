#pragma once
#ifndef PWX_PWXMEMRINGEXCEPTIONS_H_INCLUDED
#define PWX_PWXMEMRINGEXCEPTIONS_H_INCLUDED 1

/** @internal
  * @file pwxMemRingExceptions
  *
  * @brief Declarations and definitions of exceptions used by pwxMemRing
  *
  * @see CMemRingFactory.h for documentation and history
  *
**/

#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
# error "Do not include pwxMemRingExceptions.h, include CMemRingFactory.h!!"
#endif // Check for main file

#include "pwxLib/tools/Exception.h"

namespace pwx {
namespace mrf {

/** @brief std::bad_alloc caught after TMemRing new operator execution
**/
class cantCreateContainer : public ::pwx::Exception
{
public:
  explicit cantCreateContainer (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                                const char *const aDesc): Exception ("cantCreateContainer", aWhat, aWhere, prettyfunc, aDesc) { }
  cantCreateContainer (const cantCreateContainer &e):Exception(e) { }
  PWX_INLINE virtual ~cantCreateContainer() throw();
};


/** @brief std::bad_alloc caught after TItem new operator execution
**/
class cantCreateItem : public ::pwx::Exception
{
public:
  explicit cantCreateItem (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                           const char *const aDesc): Exception ("cantCreateItem", aWhat, aWhere, prettyfunc, aDesc) { }
  cantCreateItem (const cantCreateItem &e):Exception(e) { }
  PWX_INLINE virtual ~cantCreateItem() throw();
};


/** @brief no Id Map Available exception
**/
class noIdMapAvailable : public ::pwx::Exception
{
public:
  explicit noIdMapAvailable (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                             const char *const aDesc): Exception ("noIdMapAvailable", aWhat, aWhere, prettyfunc, aDesc) { }
  noIdMapAvailable (const noIdMapAvailable &e):Exception(e) {  }
  PWX_INLINE virtual ~noIdMapAvailable() throw();
};


/** @brief no Name Map Available exception
**/
class noNameMapAvailable : public ::pwx::Exception
{
public:
  explicit noNameMapAvailable (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                               const char *const aDesc): Exception ("noNameMapAvailable", aWhat, aWhere, prettyfunc, aDesc) { }
  noNameMapAvailable (const noNameMapAvailable &e):Exception(e) { }
  PWX_INLINE virtual ~noNameMapAvailable() throw();
};


/** @brief the given id can not be found in the selected container
**/
class idNotFound : public ::pwx::Exception
{
public:
  explicit idNotFound (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("idNotFound", aWhat, aWhere, prettyfunc, aDesc) { }
  idNotFound (const idNotFound &e):Exception(e) { }
  PWX_INLINE virtual ~idNotFound() throw();
};


/** @brief the given data can not be found in the selected container
**/
class dataNotFound : public ::pwx::Exception
{
public:
  explicit dataNotFound (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                         const char *const aDesc): Exception ("dataNotFound", aWhat, aWhere, prettyfunc, aDesc) { }
  dataNotFound (const dataNotFound &e):Exception(e) { }
  PWX_INLINE virtual ~dataNotFound() throw();
};


/** @brief the given name can not be found in the selected container
**/
class nameNotFound : public ::pwx::Exception
{
public:
  explicit nameNotFound (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                         const char *const aDesc): Exception ("nameNotFound", aWhat, aWhere, prettyfunc, aDesc) { }
  nameNotFound (const nameNotFound &e):Exception(e) { }
  PWX_INLINE virtual ~nameNotFound() throw();
};


/** @brief the given nr is out of range in the selected container
**/
class nrOutOfRange : public ::pwx::Exception
{
public:
  explicit nrOutOfRange (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                         const char *const aDesc): Exception ("nrOutOfRange", aWhat, aWhere, prettyfunc, aDesc) { }
  nrOutOfRange (const nrOutOfRange &e):Exception(e) { }
  PWX_INLINE virtual ~nrOutOfRange() throw();
};


/** @brief allocation failed with (c|m|re)alloc
**/
class allocFailed : public ::pwx::Exception
{
public:
  explicit allocFailed (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                        const char *const aDesc): Exception ("allocFailed", aWhat, aWhere, prettyfunc, aDesc) { }
  allocFailed (const allocFailed &e):Exception(e) { }
  PWX_INLINE virtual ~allocFailed() throw();
};


/** @brief allocation failed with (c|m|re)alloc
**/
class strayItem : public ::pwx::Exception
{
public:
  explicit strayItem (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                        const char *const aDesc): Exception ("strayItem", aWhat, aWhere, prettyfunc, aDesc) { }
  strayItem (const strayItem &e):Exception(e) { }
  PWX_INLINE virtual ~strayItem() throw();
};


/** @brief loading a container or item from an input stream failed
**/
class loadFailed : public ::pwx::Exception
{
public:
  explicit loadFailed (const char *const aWhat, const char *const aWhere, const char *const prettyfunc,
                       const char *const aDesc): Exception ("loadFailed", aWhat, aWhere, prettyfunc, aDesc) { }
  loadFailed (const loadFailed &e):Exception(e) { }
  PWX_INLINE virtual ~loadFailed() throw();
};

// --destructors heaven ---
loadFailed::~loadFailed()                   throw() { }
allocFailed::~allocFailed()                 throw() { }
strayItem::~strayItem()                     throw() { }
nrOutOfRange::~nrOutOfRange()               throw() { }
nameNotFound::~nameNotFound()               throw() { }
dataNotFound::~dataNotFound()               throw() { }
idNotFound::~idNotFound()                   throw() { }
noNameMapAvailable::~noNameMapAvailable()   throw() { }
noIdMapAvailable::~noIdMapAvailable()       throw() { }
cantCreateItem::~cantCreateItem()           throw() { }
cantCreateContainer::~cantCreateContainer() throw() { }


} // End namespace mrf

} // End of pwx namespace

#endif // PWX_PWXMEMRINGEXCEPTIONS_H_INCLUDED
