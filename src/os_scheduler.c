/*****************************************************************************
* \file      os_scheduler.c
* \author    Conny Gustafsson
* \date      2021-03-16
* \brief     Implements timer-triggered alarms and events
*
* Copyright (c) 2017 Conny Gustafsson
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
******************************************************************************/
//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <Windows.h>
#include <Process.h>
#else
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#endif
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "osmacro.h" //msocket header
#include "osutil.h"  //msocket header
#include "amber/os_scheduler.h"
#include "amber/os_systime.h"
#include "adt_ary.h"
#include <malloc.h>
#ifdef MEM_LEAK_CHECK
# include "CMemLeak.h"
#endif


//////////////////////////////////////////////////////////////////////////////
// CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
#define ONE_MILLISECOND 1

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
#if (!defined(SYSTIME_SIMULATED) && !defined(UNIT_TEST))
DYN_STATIC THREAD_PROTO(scheduler_worker,arg);
DYN_STATIC void os_scheduler_run(void);
#endif

//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////
static priority_queue_t m_pq;
static bool m_is_worker_thread_valid;
THREAD_T m_worker_thread;
SPINLOCK_T m_spin;
#ifdef _WIN32
unsigned int m_thread_id;
#endif
bool m_run_worker = false;
static os_alarmEventHookFuncType *m_alarm_event_hook = NULL;

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
void os_scheduler_init(void)
{
   priority_queue_create(&m_pq);
   m_run_worker = false;   
   m_is_worker_thread_valid = false;
   m_alarm_event_hook = NULL;
   SPINLOCK_INIT(m_spin);
}

void os_scheduler_start(void)
{
#if (!defined(_WIN32)) && (!defined(SYSTIME_SIMULATED))
   pthread_attr_t attr;
#endif

#if (!defined(SYSTIME_SIMULATED) && !defined(UNIT_TEST))
   m_is_worker_thread_valid = true;
   m_run_worker = true;
# ifdef _WIN32
   THREAD_CREATE(m_worker_thread, scheduler_worker, 0, m_thread_id);
   if (m_worker_thread == INVALID_HANDLE_VALUE)
   {
      m_is_worker_thread_valid = false;
      m_run_worker = false;
      return;
   }
# else
   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, PROG_MAX_STACK_SIZE);
   THREAD_CREATE_ATTR(m_worker_thread,attr, scheduler_worker,0);
   //TODO: Check for errors
# endif
#endif
}

os_error_t os_scheduler_insert(os_alarm_t *alarm)
{
   if (alarm != NULL) 
   {
      const uint32_t init_delay_ms = alarm->init_delay_ms;
      const uint32_t period_ms = alarm->period_ms;
      adt_error_t result = priority_queue_push(&m_pq, (void*)alarm, init_delay_ms + period_ms);
      return convert_from_adt_to_os_error(result);
   }
   return OS_INVALID_ARGUMENT_ERROR;
}

void os_scheduler_stop(void)
{
   if (m_run_worker)
   {
      assert(m_is_worker_thread_valid);
      SPINLOCK_ENTER(m_spin);
      m_run_worker = false;
      SPINLOCK_LEAVE(m_spin);
#ifndef SYSTIME_SIMULATED
      THREAD_JOIN(m_worker_thread);
      THREAD_DESTROY(m_worker_thread);
#endif      
      m_is_worker_thread_valid = false;
   }
}

void os_scheduler_shutdown(void)
{   
   priority_queue_destroy(&m_pq);   
}


//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
#if (!defined(SYSTIME_SIMULATED) && !defined(UNIT_TEST))
THREAD_PROTO(scheduler_worker,arg)
{
   (void)arg;
   for(;;)
   {
      bool running;
      SPINLOCK_ENTER(m_spin);
      running = m_run_worker;
      SPINLOCK_LEAVE(m_spin);
      if(!running)
      {
         break;
      }
      os_systime_wait(ONE_MILLISECOND);
      os_scheduler_run();
   }
   THREAD_RETURN(0);
}
#endif

#ifdef UNIT_TEST
DYN_STATIC priority_queue_t *os_time_getPriorityQueue(void)
{
   return &m_pq;
}
#endif

DYN_STATIC void os_scheduler_run(void)
{   
   uint32_t current_time_ms = os_systime_get_time();
   while(1)
   {
      adt_heap_elem_t* elem = priority_queue_top(&m_pq);
      if (current_time_ms >= elem->u32Value)
      {
         os_alarm_t *alarm = (os_alarm_t*) elem->pItem;
         //printf("{%u, %08X},\n", current_time_ms, (int) alarm->event_mask);

         //call hook if set
         if (m_alarm_event_hook != NULL)
         {
            m_alarm_event_hook(alarm->task, alarm->event_mask);
         }
         //call task handler if task is set
         priority_queue_incrementTopPriority(&m_pq, alarm->period_ms);
         if (alarm->task != NULL)
         {
            os_task_set_event(alarm->task, alarm->event_mask);
         }
      }
      else
      {
         break;
      }
   }
}