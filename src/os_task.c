//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <assert.h>
#include "amber/os_types.h"
#include "amber/os_task.h"
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
#define THREAD_STACK_SIZE 262144 //256KB

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

os_error_t os_task_create(os_task_t *self, THREAD_PROTO_PTR(handler, arg))
{
   if (self != NULL)
   {
      self->worker_func = handler;
      self->worker_thread_valid = false;
      self->quit_event = false;      
      self->event_mask = 0u;      
      MUTEX_INIT(self->mutex);
#ifdef _WIN32
      self->event_handle = CreateEvent(NULL, false, false, NULL);
#else      
      pthread_cond_init(&self->cond_var, NULL);       
#endif
      return OS_NO_ERROR;
   }
   return OS_INVALID_ARGUMENT_ERROR;
}

void os_task_destroy(os_task_t *self)
{
   if (self != NULL)
   {
#ifdef _WIN32
      MUTEX_DESTROY(self->mutex);
      CloseHandle(self->event_handle);
#else
       pthread_cond_destroy(&self->cond_var);       
#endif
   }
}

void os_task_start(os_task_t *self)
{
   if ( (self != NULL) && (self->worker_thread_valid == false) )
   {
#ifndef _WIN32
      pthread_attr_t attr;
#endif

#ifdef _WIN32
      self->worker_thread_valid = true;
      THREAD_CREATE(self->worker_thread, self->worker_func, self, self->thread_id);
      if (self->worker_thread == INVALID_HANDLE_VALUE)
      {
         self->worker_thread_valid = false;
         return;
      }
#else
      pthread_attr_init(&attr);
      pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE);
      THREAD_CREATE_ATTR(self->worker_thread, attr, self->worker_func, self);
      //TODO: Check for errors
#endif
   }   
}

void os_task_stop(os_task_t *self)
{
   if ((self != NULL) && (self->worker_thread_valid))
   {
#ifdef _WIN32
      MUTEX_LOCK(self->mutex);
      self->quit_event = true;
      MUTEX_UNLOCK(self->mutex);
      SetEvent(self->event_handle);
#else
      MUTEX_LOCK(self->mutex);
      self->quit_event = true;
      pthread_cond_signal(&self->cond_var);
      MUTEX_UNLOCK(self->mutex);
#endif
      THREAD_JOIN(self->worker_thread);
      THREAD_DESTROY(self->worker_thread);
      self->worker_thread_valid = false;
   }
}

void os_task_set_event(os_task_t* self, uint32_t event_mask)
{
   if ( (self != NULL) && (event_mask != 0u) ) 
   {      
#ifdef _WIN32
      MUTEX_LOCK(self->mutex);
      self->event_mask |= event_mask;
      MUTEX_UNLOCK(self->mutex);
      SetEvent(self->event_handle);
#else
      MUTEX_LOCK(self->mutex);
      self->event_mask |= event_mask;
      pthread_cond_signal(&self->cond_var);
      MUTEX_UNLOCK(self->mutex);
#endif
   }
   else
   {
      if (self == NULL)
      {
         fprintf(stderr, "os_task_set_event() called with NULL pointer. Event-Mask: %08X\n", event_mask);
      }
      if (event_mask == 0u)
      {
         fprintf(stderr, "os_task_set_event() called with empty event mask\n");
      }
   }   
}

os_error_t os_task_wait_event(os_task_t* self, uint32_t* event_mask)
{
   os_error_t retval = OS_NO_ERROR;
   if ((self != NULL) && (event_mask != NULL))
   {
      uint32_t tmp_mask = 0u;
      while (tmp_mask == 0u)
      {
#ifdef _WIN32         
         DWORD result = WaitForSingleObject(self->event_handle, INFINITE);
         if (result == WAIT_OBJECT_0)
         {
            MUTEX_LOCK(self->mutex);
            tmp_mask = self->event_mask;
            self->event_mask = 0u;
            if (self->quit_event)
            {
               retval = -1;
            }
            MUTEX_UNLOCK(self->mutex);
         }
         else
         {
            //Check for error in return from WaitForSingleObject
            return OS_WAIT_ERROR;
         }
#else
         MUTEX_LOCK(self->mutex);
         if ((self->quit_event == false) && (self->event_mask == 0u))
         {
            int result;
            result = pthread_cond_wait(&self->cond_var, &self->mutex);
            if (result == 0)
            {
               tmp_mask = self->event_mask;
               self->event_mask = 0u;
               if (self->quit_event)
               {
                  retval = -1;
               }
            }
         }
         else
         {
            tmp_mask = self->event_mask;
            self->event_mask = 0u;
            if (self->quit_event)
            {
               retval = -1;
            }
         }
         MUTEX_UNLOCK(self->mutex);
#endif
      }
      *event_mask = tmp_mask;
   }
   return retval;
}

void os_task_clear_event(os_task_t *self)
{
   if (self != NULL)
   {
#ifdef _WIN32
      MUTEX_LOCK(self->mutex);
      self->event_mask = 0u;
      MUTEX_UNLOCK(self->mutex);
#else
      MUTEX_LOCK(self->mutex);
      self->event_mask = 0u;
      MUTEX_UNLOCK(self->mutex);
#endif
   }
}


//////////////////////////////////////////////////////////////////////////////
// LOCAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
