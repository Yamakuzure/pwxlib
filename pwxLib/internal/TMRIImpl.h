#pragma once
#ifndef PWX_TMRIIMPL_H_INCLUDED
#define PWX_TMRIIMPL_H_INCLUDED 1

/** @internal
  * @file TMRIImpl.h
  *
  * @brief Implementations for TItem
  *
  * @see CMemRingFactory.h for documentation and history
  *
**/

#ifndef PWX_CMEMRINGFACTORY_H_INCLUDED
# error "Do not include TMRIImpl.h, include CMemRingFactory.h!!"
#endif // Check for main file

namespace pwx {
namespace mrf {
// Note: The namespaces are documented in CMemRingFactory.h

// --- public methods ---

/** @brief add an item to the list of copies
  *
  * This is a method that is called by destination items to
  * tell this item that they have been made a copy from this one.
**/
template <typename T>
void TItem<T>::addToCopies (TItem<T> *item, bool cascade)
{
  assert(item); // Will check for NULL

  if  ( item && (item != this)
     && ( copies.empty()
       || (0 == count_if(copies.begin(), copies.end(), bind1st(::std::equal_to<TItem<T> *>(), item))) ) )
    {
      copies.push_back(item);
      item->addToCopies(this, false);

      if (cascade)
        {
          // Now we need to tell everybody in copies, that there is a new copy:

          tIter iter = copies.begin();
          while ((iter != copies.end()) && *iter)
            {
              (*iter)->addToCopies(item, false);
              ++iter;
            }
        } // End of need cascadation
    } // End of addition is legal and needed
}


/** @brief add an item to the list of copies
  *
  * convenience wrapper to work with references
**/
template <typename T>
void TItem<T>::addToCopies (TItem<T> &item, bool cascade)
{
  addToCopies(&item, cascade);
}


/** @brief remove an item from the list of copies
  *
  * This method is called by destroy() to inform copies that
  * they'll go away.
**/
template <typename T>
void TItem<T>::delFromCopies (TItem<T> *item)
{
  assert(item); // Will check for NULL at run-time
  if (item)
    copies.remove(item);
}


/** @brief destroy this item
  *
  * This method tries to destroy this item after telling all copies
  * that it is going away. If there are no copies, the held data is
  * destroyed.
  *
  * <I>Note</I>: Do <I>never</I> set @a withData to false unless you a) have a pointer to the
  * held data so you can delete it yourself, or b) are sure that there are copies
  * elsewhere this item might not be aware of.
  *
  * <B>Important</B>: Please do not destroy items with multiple threads. As the item does
  * not "come back" into the ring structure, any thread that is currently searching and reaches
  * this item before it goes away, it will most probably produce a crash.
  *
  * @param[in] withData delete the held data if true (default) when there are no copies
  * @return The current root item of the ring or NULL if root has not been touched
**/
template <typename T>
TItem<T> * TItem<T>::destroy (bool withData)
{
  lock();
  TItem<T> * result = detach(true); // This will take care of the rings consistency

  // Tell all copies that we are going bye bye and delete data if possible
  if (copies.size())
    clearCopies();
  else if (withData && data)
    {
      if (isArrayType(T))
        delete [] (data);
      else
        delete (data);
    }

  // And off we go:
  delete(this);

  return result && !result->nr && (result != this) ? result : NULL;
}


/** @brief return a const reference to the held data
**/
template <typename T>
const T& TItem<T>::getData () const
{
  return (*data);
}


/** @brief return a read/write reference to the held data
**/
template <typename T>
T& TItem<T>::getData ()
{
  return (*data);
}


/** @brief return a pointer to the held data
**/
template <typename T>
T* TItem<T>::getDataPointer ()
{
  return (data);
}


/** @brief return the current next item of this one
**/
template <typename T>
TItem<T> * TItem<T>::getNext () const
{
  return (next);
}


/** @brief return the current previous item of this one
 */
template <typename T>
TItem<T> * TItem<T>::getPrev () const
{
  return (prev);
}


/** @brief Returns the current reference count of the item, aka number of copies known.
**/
template <typename T>
int32_t TItem<T>::getRefCount () const
{
  return (copies.size() + 1); // Plus one, because we are referencing it, too!
}


/** @brief Convenience method for items to shift them around.
  *
  * The item will be detached, and then inserted after @a Prev,
  * before @a aNext, or between both if both are given and
  * neighbors.
  * If both are given but they are _not_ neighbors, the item
  * will be placed after @a aPrev, @a aNext is ignored then.
  * If no new neighbors are given, the item is simply detached.
  *
  * <B>Important</B>: Because this method will change the structure of the container it is
  * held in, it locks itself before taking itself out of the container. Do <I>never</I> call
  * this method after locking this item or you produce a dead lock. <I>Second</I> do not
  * call this method with both @a aPrev and @a aNext set to NULL in a multi threaded
  * environment or searching threads might crash.
  *
  * If something is wrong with the item placement, a pwx::mrf::strayItem exception is thrown.
  * This indicates that an operation is done in a multi threaded environment where two threads move
  * an item without proper locking. If you are sure to have defined PWX_THREADS <I>and</I> to
  * use MRInterface<T> in all your threads, then file a bug report, please!
  *
  * @param[in] aPrev the new prev item this one will be moved after
  * @param[in] aNext the new next item this one will move aside
  * @param[out] newNum The new number of the item after the move
  * @return Pointer to the root item if found or NULL if root has not been touched
**/
template <typename T>
TItem<T> * TItem<T>::move (TItem<T> *aPrev, TItem<T> *aNext, int32_t *newNum)
{
  TItem<T> *result = NULL;

  /* There are the following cases:
   * aPrev is given and does not equal prev or aPrev is not
   * given but aNext and aNext does not equal next.
   * -> This is a normal move, but there are the following
   *    special cases which are handled first:
   * 1.: aPrev is given and equals next
   *     -> swap this and next
   * 2.: aNext is given (aPrev is NULL) and equals prev
   *     -> swap this and prev
   * 3.: aPrev equals prev and this is root
   *     -> root shall become the new last item
   * 4.: aPrev is NULL, aNext is given, equals next and is root
   *     -> The last item (this) shall become the new root.
   * 5.: Neither aPrev nor aNext is given
   *     -> A normal detach
  */
  if (aPrev && (aPrev == next))
    {
      // Special Case 1: swap this and next
      TItem<T> *sRoot = this->swap(next);
      if (sRoot && !sRoot->nr)
        result = sRoot;
      assert (prev && "ERROR: Item->Move special case 1 nullified prev!");
      assert (next && "ERROR: Item->Move special case 1 nullified next!");
    }
  else if (!aPrev && aNext && (aNext == prev))
    {
      // Special Case 2: swap this and prev
      TItem<T> *sRoot = this->swap(prev);
      if (sRoot && !sRoot->nr)
        result = sRoot;
      assert (prev && "ERROR: Item->Move special case 2 nullified prev!");
      assert (next && "ERROR: Item->Move special case 2 nullified next!");
    }
  else if (aPrev && (aPrev == prev) && (0 == nr))
    {
      // Special Case 3: root shall become the last item:
      nr = prev->nr + 1;
      TItem<T> *sRoot = next->setNr(0);

      // Now, do we really *know* the new root?
      assert(sRoot && !sRoot->nr && (sRoot == next) && "\nERROR: root 2 last FAILED!\n");
      if (sRoot && (0 == sRoot->nr))
        // Note to self: This must be superfluous, this simply can't fail, can it?
        result = sRoot;
      assert (prev && "ERROR: Item->Move special case 3 nullified prev!");
      assert (next && "ERROR: Item->Move special case 3 nullified next!");
    }
  else if (!aPrev && aNext && (aNext == next) && (0 == next->nr))
    {
      // Special Case 4: Last item should become root:
      nr = 0;
      TItem<T> *sRoot = next ? next->setNr(1) : this;
      assert(sRoot && !sRoot->nr && (sRoot == this) && "\nERROR: last 2 root FAILED!\n");
      if (sRoot && (0 == sRoot->nr))
        // Note to self: This must be superfluous, too, just like above, right?
        result = sRoot;
      assert (prev && "ERROR: Item->Move special case 4 nullified prev!");
      assert (next && "ERROR: Item->Move special case 4 nullified next!");
    }
  else if (!aPrev && !aNext)
    {
      // Special Case 5: Normal detach
      ::pwx::Lock selfLock(this);
      TItem<T> *dRoot = detach(true); // this must be true as a normal detach might occur
      if (!dRoot && (nr < 1))
        // This means this is the first item, and therefore it must be root
        dRoot = this;
      nr = 0;
      if (dRoot && (0 == dRoot->nr))
        result = dRoot;
      assert (!prev && "ERROR: Item->Move special case 5 created an illegal prev!");
      assert (!next && "ERROR: Item->Move special case 5 created an illegal next!");
    }
  else if ( (aPrev && (aPrev != prev))
         ||(!aPrev && aNext && (aNext != next)) )
    {
      TMemRing<T> *newOwner = aPrev ? aPrev->owner : aNext ? aNext->owner : NULL;

      // The old next and prev items is needed to be known
      TItem<T> *oldNext = next;
      TItem<T> *oldPrev = prev;
      int32_t   oldNr   = nr;

      // Determine whether detach/insert have to renumber:
      bool autoRenumber = oldNext && (owner == newOwner) ? false : true;

      // We only renumber here if the owner changes. A NULL oldNext has no consequences
      ::pwx::Lock selfLock(this);
      TItem<T> *dRoot = detach(autoRenumber);
      if (aPrev)
        {
          // This item is to become the next neighbor to aPrev:
          prev = aPrev;
          nr   = prev->nr + 1;
        }
      else
        {
          // This item is to become the prev neighbor to aNext
          next = aNext;
          nr   = next->nr; // This is to shift next aside.
        }

      // Only renumber upon insert if there is no old Next or the owner changes
      TItem<T> *iRoot = NULL;
      PWX_TRY(iRoot = insert(autoRenumber))
      PWX_THROW_FURTHER

      // If the owner did not change and we had a next item, it is to be renumbered now
      TItem<T> *rRoot = NULL;
      if (!autoRenumber)
        {
          // Unless this item becomes the new root, the old next is used as a starting point
          if (nr)
            {
              // if the move stays left of the old number, the old prev is the base for the renumbering
              if (nr < oldNr)
                rRoot = oldPrev->setNr(oldNr);
              else
                rRoot = oldNext->setNr(oldNr);
            }
          else
            rRoot = setNr(0);
        }

      // Now set result if one of the determined roots are there and valid
      if (rRoot && (0 == rRoot->nr))
        result = rRoot;
      else if (iRoot && (0 == iRoot->nr))
        result = iRoot;
      else if (dRoot && (0 == dRoot->nr))
        result = dRoot;

      // Finally if the owner changes, we need to know:
      owner = newOwner;
      assert (prev && "ERROR: Item->Move normal move nullified prev!");
      assert (next && "ERROR: Item->Move normal move nullified next!");
    }
  // no else, because it simply means that aPrev is given and equals prev
  assert (((prev && next) || (!prev && !next)) && "ERROR: Some unknown dark force nullified prev or next!");


  if (newNum)
    *newNum = nr;

  assert ((!result || !result->nr) && "\nERROR: TItem<T>::move tries to report wrong new root!\n");

  return result && !result->nr ? result : NULL;
}


/** @brief save the item front data in an ostream
  *
  * although the save() method of an item can be used through a
  * container, the motivation is to allow to use this method
  * from inside an operator<<() on a stream, which saves
  * a friend declaration on the said operator.
  *
  * Important: The stored data must have an operator<<(stream, data)
  *            to be used with!
**/
template <typename T>
void TItem<T>::save (::std::ostream &os) const
{
  if (os.good())
    {
      // regenerating the copies list is a task for the loading
      // of an item.
      CItem::save(os);
      if (data)
        {
          os << base::ID_TITEM_START << ";";
          os << *data << ::std::endl; // We do not know what data is, so we endline here
        }
    }
}


/** @brief Sets a new number and cascades the numbering
  * @return Pointer to the root item if found or NULL if root has not been touched
**/
template <typename T>
TItem<T> * TItem<T>::setNr (int32_t aNr)
{
  TItem<T> * result = NULL;

  // Note: We do not check for a number change, because this method should
  // be able to check the next and previous item even if this item already
  // has the target number.
  if (next && prev)
    {
      // Three cases are there to take care of:
      // Case 1: This is to become the new root
      // Case 2: The previous item has a different nr than aNr-1
      // Case 3: The next item has a different nr than aNr+1
      // Case 4: This item becomes the only item and therefore root
      int32_t xNr  = aNr;
      tItem  *item = NULL;
      nr           = xNr;

      if (0 == xNr)
        {
          // Case one, this item becomes the new root
          item   = next;
          result = this;
          // Now cascade until the renumbering is complete
          while (item && (item != this))
            {
              item->nr = ++xNr;
              item = item->next;
            }
        } // End of new root cascade
      else
        {
          // If aNr is something else than 0, check the neighborhood
          item = prev;
          // Case 2: Cascade down until (the new) root is found or the numbering is correct
          while (item && xNr && (--xNr != item->nr))
            {
              item->nr = xNr;
              if (xNr)
                item = item->prev;
              else
                result = item;
            }
          item = next;
          xNr  = nr;
          // Case 3: Cascade up until root is found or the numbering is correct
          while (item && item->nr && (++xNr != item->nr))
            {
              item->nr = xNr;
              item = item->next;
              if (0 == item->nr)
                result = item;
            }
        } // End of normal check
    } // End of having neighbors
  else
    {
      // Case 4: We are just us:
      nr = 0;
      result = this; // If this is the only item, it is root
    }

  assert((!result || !result->nr) && "\nERROR: setNr() tried to report a wrong new root!\n");

  return result && !result->nr ? result : NULL;
}


/** @brief swap two items
  *
  * This method lets this item swap position with @a other.
  * If @a other is NULL, this item is simply taken out of
  * the ring.
  *
  * <B>Important</B>: Please do not swap items with multiple threads without setting @a other.
  * As the item then does not "come back" into the ring structure, any thread that is currently
  * searching and reaches this item before it goes away, it will most probably produce a crash.
  *
  * @param[in]  other pointer to the item to swap with
  * @return pointer to the root item if found or NULL if root has not been touched
**/
template <typename T>
TItem<T> * TItem<T>::swap(TItem<T> *other)
{
  TItem<T> * result = NULL;

  if (other && (other != this))
    {
      /* Note: We do not use two calls to move(), because that would
       * cause a wrong renumbering with the first call, when only one
       * item is moved.*/

      // Save current values
      int32_t   thNr    = nr;
      TItem<T> *thPrev  = prev;
      TItem<T> *thNext  = next;
      int32_t   otNr    = other->nr;
      TItem<T> *otPrev  = other->prev;
      TItem<T> *otNext  = other->next;

      // Case 1: other is our current next:
      if (next == other)
        {
          prev = other;
          next = otNext;
          other->prev = thPrev;
          other->next = this;
          // Notify neighbors:
          thPrev->next = other;
          otNext->prev = this;
        }
      // Case 2: other is our current prev:
      else if (prev == other)
        {
          prev = otPrev;
          next = other;
          other->prev = this;
          other->next = thNext;
          // Notify neighbors:
          otPrev->next = this;
          thNext->prev = other;
        }
      // Case 3: other and this aren't connected
      else
        {
          prev = otPrev;
          next = otNext;
          other->prev = thPrev;
          other->next = thNext;
          // Notify neighbors:
          thPrev->next = other;
          thNext->prev = other;
          otPrev->next = this;
          otNext->prev = this;
        }

      // Now swap number:
      other->nr = thNr;
      this->nr  = otNr;

      // Do we have a new root?
      if (!otNr)
        result = this;
      else if (!thNr)
        result = other;

      // Finally swap owners if needed:
      if (other->owner != owner)
        {
          TMemRing<T> *otOwner = other->owner;
          other->owner = owner;
          owner = otOwner;
        }
    }
  else if (!other)
    {
      // Before taking this item out of the ring it has to be locked.
      ::pwx::Lock selfLock(this);
      result = detach(true);
      nr = 0;
    }

  assert((!result || !result->nr) && "\nERROR: TItem<T>::swap() tried to report a wrong new root!\n");

  return result && !result->nr ? result : NULL;
}


/** @brief Assignement operator, the source is saved in copies.
  *
  * The operator tells all copies that this item is going away.
  * If there are no copies, then the held data is deleted.
**/
template <typename T>
TItem<T>& TItem<T>::operator=(TItem<T> &source)
{
  if (*source != this)
    {
      if (copies.size())
        // clear copies, because we are going to drop off our data
        clearCopies();
      else if (data)
        {
          // No copies? Then data is going bye bye...
          if (isArrayType(T))
            delete [] (data);
          else
            delete (data);
        }

      // Now copy source into this:
      CItem::operator=(source); // Copy core data
      data = source.getDataPointer();

      // make sure everybody knows we are a copy:
      source.addToCopies(this, true);
    }

  return (*this);
}


// --- private methods ---

/** @brief small method to clear copies
  *
  * This method clears this item from the list of known copies.
  * But: The data is not touched, the caller of this method has
  * to decide whether to delete the data or not!
**/
template <typename T>
void TItem<T>::clearCopies()
{
  if (!copies.empty())
    {
      // Our copies need to know what we are up to
      copies.unique(); // clean up first
      tIter iter = copies.begin();
      while (iter != copies.end() && *iter)
        {
          (*iter)->delFromCopies(this);
          ++iter;
        }
      copies.clear();
    }
}


/** @brief small method to take this item out of the ring
  * @return Pointer to the root item if found or NULL if root has not been touched
  *
  * <B>Important:</B> Do never set @a doRenumbering to false unless the very same item
  * is inserted into the same ring after detachment, like in a move operation.
  *
  * @param[in] doRenumbering true: renumber the memory ring after the detaching
**/
template <typename T>
TItem<T> * TItem<T>::detach(bool doRenumbering)
{
  assert ((next && prev) || (!next && !prev)); // either both or none
  TItem<T> *result = NULL;

  if (next && prev)
    {
      if (next != prev)
        {
          next->prev = prev;
          prev->next = next;
          // We need to let the ring renumber itself:
          if (doRenumbering && (next->nr > 0))
            // We were not the last item
            result = next->setNr(nr);
        }
      else
        {
          // If they are equal, this is a two-items-ring
          next->prev = NULL;
          prev->next = NULL;
          if (doRenumbering)
            result = prev->setNr(0);
        }

      // Now complete the detachment:
      next = NULL;
      prev = NULL;
      nr   = 0;
    }
  // If this was the only item, nothing has to be done.

  assert((!result || !result->nr) && "\nERROR: detach() tried to report a wrong new root!\n");

  return result && !result->nr ? result : NULL;
}

/** @brief small method to insert this item before prev or after next, whatever is set
  *
  * <B>Important:</B> Do never set @a doRenumbering to false unless the very same item
  * is inserted into the same ring after detachment, like in a move operation.
  *
  * If something is wrong, like prev or next are set to this item, a pwx::mrf::strayItem
  * exception is thrown.
  *
  * @param[in] doRenumbering true: renumber the memory ring after the detaching
  * @return Pointer to the root item if found or NULL if no root item has been touched.
**/
template <typename T>
TItem<T> * TItem<T>::insert(bool doRenumbering)
{
  TItem<T> *result = NULL;

  assert ( ( (prev && (prev != this)) || (next && (next != this)) || (!prev && !next))
        && "ERROR: TItem::insert called with invalid prev/next values!");

  if (prev)
    {
      if (prev != this)
        {
          // First set a valid next
          if (prev->next && (prev->next != this))
            next = prev->next;
          else
            // Was single item, now is two-items-ring
            next = prev;
          // Now announce our presence
          prev->next = this;
          next->prev = this;

          // And go for renumbering:
          if (doRenumbering)
            result = setNr(prev->nr + 1);
        } // End of all in order
      else
        {
          // Bad mana, stray item detected!
          char msg[103] = "";
          pwx_snprintf(msg, 102, "Item nr %d, id %d, \"%s\" has prev set to this on insert!", nr, id, name.substr(0,32).c_str());
          PWX_THROW(strayItem, msg, "prev == this on insert")
        }
    } // end of having prev
  else if (next)
    {
      if (next != this)
        {
          // First set a valid prev
          if (next->prev && (next->prev != this))
            prev = next->prev;
          else
            // Was single item, now is two-items-ring
            prev = next;
          // Now announce our presence
          prev->next = this;
          next->prev = this;

          // And go for renumbering:
          if (doRenumbering)
            result = setNr(next->nr);
        } // End of all in order
      else
        {
          // Bad mana, stray item detected!
          char msg[103] = "";
          pwx_snprintf(msg, 102, "Item nr %d, id %d, \"%s\" has next set to this on insert!", nr, id, name.substr(0,32).c_str());
          PWX_THROW(strayItem, msg, "next == this on insert")
        }
    } // end of having next
  else
    // what's that?
    nr = 0;

  assert((!result || !result->nr) && "\nERROR: TItem<T>::insert() tried to report a wrong new root!\n");

  return result && !result->nr ? result : NULL;
}


// --- stream operators ---
template<typename T>
::std::ostream& operator<<(::std::ostream &os, TItem<T> &item)
{
  item.save(os);
  return(os);
}

} // End of namespace mrf

} // End of namespace pwx

#endif // PWX_TMRIIMPL_H_INCLUDED
