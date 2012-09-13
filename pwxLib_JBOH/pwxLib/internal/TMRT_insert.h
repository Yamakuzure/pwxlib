#pragma once
#ifndef PWX_MRF_TMRT_INSERT_H_INCLUDED
#define PWX_MRF_TMRT_INSERT_H_INCLUDED 1

/** @internal
  * @file TMRT_insert.h
  *
  * @brief insertion methods for TMemRing<T> which depend on CMemRingFactory.
  *
  * This file simply breaks the circular dependency between TMemRing and CMemRingFactory
  * added by using MRF as an Owner in CMemRing.
  *
  * @see CMemRingFactory.h for documentation and history
**/

#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
# error "Do not include TMRT_insert.h, include CMemRingFactory.h!!"
#endif // Check for main file


namespace pwx {
namespace mrf {
// Note: The namespaces are documented in CMemRingFactory.h


/** @brief internal method to insert and optionally register items
  *
  * If something is wrong with the the placement of an item, a pwx::mrf::strayItem exception
  * is thrown. This indicates that an operation is done in a multi threaded environment where two
  * threads move an item without proper locking. Please make sure to use MRInterface<T> with
  * multi threaded operations and define PWX_THREADS before including the first pwxLib header.
**/
template<typename T>
int32_t TMemRing<T>::insert (T *data, int32_t aNr, uint32_t aId, const char *aName, bool doReg)
{
  assert(data); // runtime check to not call with NULL data

  TItem<T> * oldCurr  = current;
  TItem<T> * newItem  = NULL;
  int32_t    xNr      = (aNr < -1) || (aNr >= dataCount) ? normalizeNumber(aNr) : aNr;
  uint32_t   xId      = idMapUsed   ? getUniqueId(aId)     : aId;
  char     * xName    = nameMapUsed ? getUniqueName(aName) : NULL; // This might result in a malloc'd string
  ::std::string sName = nameMapUsed ? xName : aName ? aName : "Item";

  // Free xName if it is set
  if (xName)
    free (xName);

  // Now we need a new TItem:
  try
    {
      PWX_TRY(newItem = new TItem<T>(data, xNr, xId, sName.c_str(), this))
      catch (::std::bad_alloc &e)
        {
          char txt[128]       = "";
          pwx_snprintf(txt, 127, "insert() tried to create a TItem with id %u, name \"%s%s\"",
                  aId, sName.substr(0,29).c_str(), sName.size() > 29 ? "..." : "");
          PWX_THROW(cantCreateItem, e.what(), txt)
        }

      // register the new TItem:
      if (owner && doReg && trackRef)
        owner->registItem(newItem);

      // This'll take care of the numbering and correct position, even if current is NULL
      if (!dataCount || (!setCurrToNr(-1 == aNr ? -1 : xNr)))
        // Note: -1 is especially handled by setCurrToNr
        current = root;

      if (aNr < 0)
        {
          // We have to insert after if the number is negative
          PWX_TRY(xNr = movInsAfter(newItem, current, oldCurr))
          PWX_THROW_FURTHER
        }
      else
        {
          // We have to insert before if the number is positive
          PWX_TRY(xNr = movInsBefore(newItem, current, oldCurr))
          PWX_THROW_FURTHER
        }
    }
  catch (::std::bad_alloc &e)
    {
      char msg[91] = "";
      pwx_snprintf(msg, 90, "The data \"%s%s\" can not be inserted into \"%s%s\" !",
                     sName.substr(0,21).c_str(), sName.size() > 21 ? "..." : "",
                     name.substr(0,22).c_str(),  name.size() > 22 ? "..." : "");
      // Move current back:
      current = oldCurr;
      // Now get it out:
      PWX_THROW(cantCreateItem, e.what(), msg)
    } // We don't catch noId(Name)MapAvailable here, the caller must handle them!

  // give its number back:
  return (xNr);
}

/** @brief internal method to register a new container with it's owner
  *
  * This can throw cantCreateItem, noIdMapAvailable and noNameMapAvailable.
**/
template<typename T>
void TMemRing<T>::registerWithOwner ()
{
  PWX_TRY(owner->registCont(this, false))
  PWX_THROW_FURTHER
}

} // End of namespace mrf

} // End of namespace pwx

#endif // PWX_MRF_TMRT_INSERT_H_INCLUDED
