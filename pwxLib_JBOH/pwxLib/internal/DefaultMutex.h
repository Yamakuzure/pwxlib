#pragma once
#ifndef PWX_DEFAULTMUTEX_H_INCLUDED
#define PWX_DEFAULTMUTEX_H_INCLUDED 1

#ifndef PWX_DEFINES_H_INCLUDED
# error "Do not include DefaultTempTools.h directly, include pwxDefaultDefines.h instead!"
#endif // Check for default defines

/** @internal
  * @file DefaultMutex.h
  *
  * @brief location of helping templates and classes for multi threading.
  *
  * Define the classes Lock and Lockable. Both do nothing unless PWX_THREADS is defined.
  *
  * @see pwxDefaultDefines.h for history
  *
**/

// These are needed for the MutEx functionality:
#include "pwxLib/internal/MSVCwarnPush.h"
#if defined(PWX_THREADS)
#  if defined(_WIN32)
#    include <windows.h>
#  else
#    include <pthread.h>
#  endif
#endif
#include "pwxLib/internal/MSVCwarnPop.h"

/** @namespace pwx
  *
  * @brief Namespace for everything PrydeWorX related
  *
  * All PrydeWorX projects reside in this namespace and it's sub-namespaces.
**/
namespace pwx
{

/** @class Lockable
  * @brief Add locking mechanics to a class
  *
  * When inheriting public from this class the child class can
  * be locked using lock() or trylock() and unlocked by using unlock() for
  * parallel access.
**/
class Lockable
{
#if defined(PWX_THREADS)
#  if defined(_WIN32)
     CRITICAL_SECTION locked;
#  else
     pthread_mutex_t locked;
#  endif
#endif
public:
  /// @brief default constructor
#if defined(PWX_THREADS)
  explicit Lockable()
#  if defined(PWX_HAS_CXX11_INIT)
    : locked({})
#  endif
    {
#  if defined(_WIN32)
      InitializeCriticalSection(&locked);
#  else
      pthread_mutex_init(&locked, NULL);
#  endif
#else
  explicit Lockable()
    {
#endif
    }

  /// @brief default destructor
  virtual ~Lockable()
    {
#if defined(PWX_THREADS)
#  if defined(_WIN32)
      DeleteCriticalSection(&locked);
#  else
      pthread_mutex_destroy(&locked);
#  endif
#endif
    }

  /// @brief copy constructor, initializes individual mutex
#if defined(PWX_THREADS)
  Lockable(const Lockable&)
#  if defined(PWX_HAS_CXX11_INIT)
    : locked({})
#  endif
    {
      // Although the descendant is copied, it needs its own mutex:
#  if defined(_WIN32)
      InitializeCriticalSection(&locked);
#  else
      pthread_mutex_init(&locked, NULL);
#  endif
#else
  Lockable(const Lockable&)
    {
#endif
    }

  /* --- locking methods --- */

  /// @brief return true if this lockable is currently locked
  bool isLocked()
    {
#if defined(PWX_THREADS)
#  if defined(_WIN32)
      if (TryEnterCriticalSection(&locked))
        {
          LeaveCriticalSection(&locked);
          return false;
        }
      else
        return true;
#  else
      if (pthread_mutex_trylock(&locked))
        return true;
      else
        {
          pthread_mutex_unlock(&locked);
          return false;
        }
#  endif
#else
      return false;
#endif
    }

  /// @brief return true if this lockable can be locked or is already locked by the calling thread
  bool tryLock()
    {
#if defined(PWX_THREADS)
#  if defined(_WIN32)
      if (TryEnterCriticalSection(&locked))
        return true;
      else
        return false;
#  else
      if (pthread_mutex_trylock(&locked))
        return false;
      else
        return true;
#  endif
#else
      return true;
#endif
    }

  /** @brief lock this object
    *
    * This method will try to lock until it is possible.
    * Until your thread calls unlock(), all further calls
    * to this method will not return until the lock is removed.
  **/
  void lock()
    {
#if defined(PWX_THREADS)
#  if defined(_WIN32)
      EnterCriticalSection(&locked);
#  else
      pthread_mutex_lock(&locked);
#  endif
#endif
    }

  /// @brief unlock this object
  void unlock()
    {
#if defined(PWX_THREADS)
#  if defined(_WIN32)
      LeaveCriticalSection(&locked);
#  else
      pthread_mutex_unlock(&locked);
#  endif
#endif
    }

private:
  /// @brief assignment operator is strictly forbidden
  Lockable &operator=(const Lockable&);

};


/** @class Lock
  * @brief Use RAII to lock/unlock a lockable object
  *
  * This class can be used to lock an object which is derived
  * from Lockable on instantiation.
  * The object is unlocked on destruction of this object and
  * is therefore exception safe locked.
**/
class Lock : private Uncopyable
{
  Lockable *lockedObject; //!< The object to be locked

  /* --- empty ctor, copy ctor and assignment operator must not be used --- */
  Lock();
  Lock(Lock&);
  Lock &operator=(Lock&);
public:
  /** @brief default ctor, locks @a lockable and then returns
    *
    * If you do not provide @a lockable, this whole object does nothing
    *
    * @param[in] lockable pointer to an object that is derived from pwx::Lockable
  **/
  explicit Lock(::pwx::Lockable *lockable):lockedObject(lockable)
    {
      assert (lockedObject && "ERROR: pwx::Lock ctor needs a lockable object to work with!");
      if (lockedObject)
        lockedObject->lock();
    }

  /** @brief default dtor, unlocks the locked object **/
  ~Lock()
    {
      if (lockedObject)
        lockedObject->unlock();
    }
};

} // End of namespace pwx

#endif // PWX_DEFAULTMUTEX_H_INCLUDED

