//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "os_types.h"
#include "os_task.h"
#ifdef _MSC_VER
#include <process.h>
#endif
#include "osmacro.h"
#include <malloc.h>
#ifdef MEM_LEAK_CHECK
# include "CMemLeak.h"
#endif


//////////////////////////////////////////////////////////////////////////////
// CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
#define THREAD_STACK_SIZE 65536

//////////////////////////////////////////////////////////////////////////////
// LOCAL FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// LOCAL VARIABLES
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

/**
 * return 0 on success, non-zero on error
 */
int8_t os_task_create(os_task_t *self, THREAD_PROTO_PTR(handler, arg))
{
   if (self != 0)
   {
      self->workerThreadValid = false;
      self->quitEvent = false;
      self->workerFunc = handler;
      self->eventMask1 = 0u;
#ifdef _WIN32
       SPINLOCK_INIT(self->lock);
#else
       pthread_cond_init(&self->cond_var, NULL);
       MUTEX_INIT(self->mutex);
#endif
         return 0;
   }
   return -1;
}

void os_task_destroy(os_task_t *self)
{
   if (self != 0)
   {
#ifdef _WIN32
      SPINLOCK_DESTROY(self->lock);
#else
       pthread_cond_destroy(&self->cond_var);
       MUTEX_DESTROY(self->mutex);
#endif
   }
}

void os_task_start(os_task_t *self)
{
   if ( (self != 0) && (self->workerThreadValid == false) )
   {
#ifndef _MSC_VER
      pthread_attr_t attr;
#endif

#ifdef _WIN32
      THREAD_CREATE(self->workerThread, self->thread_func, self, self->threadId);
      if (self->workerThread == INVALID_HANDLE_VALUE) 
      {
         self->workerThreadValid = false;
         return;
      }
#else
      pthread_attr_init(&attr);
      pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE);
      THREAD_CREATE_ATTR(self->workerThread, attr, self->workerFunc, self);
#endif
      self->workerThreadValid = true;
   }   
}

void os_task_stop(os_task_t *self)
{
   if (self != 0)
   {
#ifdef _WIN32
      SPINLOCK_ENTER(self->lock);
      self->workerThreadRunning=false;
      SPINLOCK_LEAVE(self->lock);
      SetEvent(self->hEvent);
#else
      MUTEX_LOCK(self->mutex);
      self->quitEvent = true;
      pthread_cond_signal(&self->cond_var);
      MUTEX_UNLOCK(self->mutex);
#endif
      THREAD_JOIN(self->workerThread);
      THREAD_DESTROY(self->workerThread);
      self->workerThreadValid = false;
   }
}

void os_task_setEvent(os_task_t *self, uint32_t eventMask)
{
   if (self != 0)
   {
#ifdef _WIN32
      SPINLOCK_ENTER(self->lock);
      self->eventMask1|=eventMask;
      SPINLOCK_LEAVE(self->lock);
      SetEvent(self->hEvent);
#else
      MUTEX_LOCK(self->mutex);
      self->eventMask1|=eventMask;
      pthread_cond_signal(&self->cond_var);
      MUTEX_UNLOCK(self->mutex);
#endif
   }
}

/**
 * returns negative (-1) on error, 0 on success, 1 when quit event has been activated
 */
int8_t os_task_waitEvent(os_task_t *self, uint32_t *eventMask)
{
   int8_t retval = -1;
   if ( (self != 0 ) && (eventMask != 0) )
   {
   #ifdef _WIN32
      DWORD result = WaitForSingleObject(self->hEvent, INFINITE);
      if (result == WAIT_OBJECT_0)
      {
         SPINLOCK_ENTER(self->lock);
         *eventMask = self->eventMask1;
         retval = self->quitEvent? 1 : 0;
         SPINLOCK_LEAVE(self->lock);
      }
   #else

      MUTEX_LOCK(self->mutex);
      if ((self->quitEvent == false) && (self->eventMask1 == 0))
      {
         int result;
         result = pthread_cond_wait(&self->cond_var, &self->mutex);
         if (result == 0)
         {
            *eventMask = self->eventMask1;
            self->eventMask1=0u;
            retval = self->quitEvent? 1 : 0;
         }
      }
      else
      {
         *eventMask = self->eventMask1;
         self->eventMask1=0u;
         retval = self->quitEvent? 1 : 0;
      }
      MUTEX_UNLOCK(self->mutex);
   #endif
   }
   return retval;
}

void os_task_clearEvent(os_task_t *self)
{
   if (self != 0)
   {
#ifdef _WIN32
      SPINLOCK_ENTER(self->lock);
      self->eventMask1 = 0u;
      SPINLOCK_LEAVE(self->lock);
#else
      MUTEX_LOCK(self->mutex);
      self->eventMask1 = 0u;
      MUTEX_UNLOCK(self->mutex);
#endif
   }
}


//////////////////////////////////////////////////////////////////////////////
// LOCAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
