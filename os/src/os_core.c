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
#include "osmacro.h"
#include "osutil.h"
#include "os_core.h"
#include "systime.h"
#include "adt_ary.h"
#include <malloc.h>
#ifdef MEM_LEAK_CHECK
# include "CMemLeak.h"
#endif


//////////////////////////////////////////////////////////////////////////////
// CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
#if (!defined(SYSTIME_SIMULATED) && !defined(UNIT_TEST))
static THREAD_PROTO(TimerEventWorker,arg);
#endif
//////////////////////////////////////////////////////////////////////////////
// LOCAL FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static void initOsTasks(void);
static void shutdownOsTasks(void);
static void startOsTasks(void);
static void stopOsTasks(void);
static void initScheduler(void);
DYN_STATIC void os_scheduler_run(void);





//////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// LOCAL VARIABLES
//////////////////////////////////////////////////////////////////////////////
static priority_queue_t m_pq;
static bool m_workerThreadValid;
static os_cfg_t *m_cfg = 0;
THREAD_T m_thread_worker;
SPINLOCK_T m_spin;
#ifdef _MSC_VER
DWORD m_threadId;
#endif
uint8_t m_isRunning = 0;
uint32_t (*m_getTimeFn)(void) = 0;
void (*m_alarmEventHookFunc)(const os_alarm_cfg_t *alarm);


//////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

/**
 * \param cfg array of event object.
 * \param u32CfgLen number of elements in configuration array
 * \timerfunc pointer to function that returns current system time (default: SysTime_getTime)
 */
void os_init(os_cfg_t *cfg)
{   
   m_cfg = cfg;
   priority_queue_create(&m_pq);
   m_isRunning = 0;
   
   m_workerThreadValid = false;
   if (m_cfg->timerFunc != 0)
   {
      m_getTimeFn = m_cfg->timerFunc;
   }
   else
   {
      m_getTimeFn = SysTime_getTime;
   }
   m_alarmEventHookFunc = cfg->alarmEventHookFunc;
   
   initOsTasks();
   initScheduler();
}

void os_shutdown(void)
{
   os_stop();
   priority_queue_destroy(&m_pq);
   shutdownOsTasks();
}

void os_start(void)
{
#if (!defined(_MSC_VER)) && (!defined(SYSTIME_SIMULATED))
   pthread_attr_t attr;
#endif

   SPINLOCK_INIT(m_spin);   
   m_workerThreadValid = false;

   startOsTasks();
   m_isRunning = 1;
#if (!defined(SYSTIME_SIMULATED) && !defined(UNIT_TEST))
# ifdef _MSC_VER
   THREAD_CREATE(m_thread_worker, TimerEventWorker, 0, m_threadId);
   if (m_thread_worker == INVALID_HANDLE_VALUE)
   {
      m_workerThreadValid = false;
      return;
   }
# else
   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, PROG_MAX_STACK_SIZE);
   THREAD_CREATE_ATTR(m_thread_worker,attr, TimerEventWorker,0);
# endif
   m_workerThreadValid = true;
#endif

}

void os_stop(void)
{
   if (m_isRunning != 0)
   {
      SPINLOCK_ENTER(m_spin);
      m_isRunning = 0;
      SPINLOCK_LEAVE(m_spin);
#ifndef SYSTIME_SIMULATED
      THREAD_JOIN(m_thread_worker);
      THREAD_DESTROY(m_thread_worker);
#endif
      SPINLOCK_DESTROY(m_spin);
      m_workerThreadValid = false;
      stopOsTasks();
   }
}

#ifdef UNIT_TEST
priority_queue_t *os_getPriorityQueue(void)
{
   return &m_pq;
}
#endif


//////////////////////////////////////////////////////////////////////////////
// LOCAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
DYN_STATIC void os_scheduler_run(void)
{
   uint32_t currentTimeMs = m_getTimeFn();
   while(1)
   {
      adt_heap_elem_t* elem = priority_queue_top(&m_pq);
      if (currentTimeMs >= elem->u32Value)
      {
         const os_alarm_cfg_t *alarm = (const os_alarm_cfg_t*) elem->pItem;
         //printf("{%u, %08X},\n", currentTimeMs, (int) alarm->eventMask);

         //call hook if set
         if (m_alarmEventHookFunc != 0)
         {
            m_alarmEventHookFunc(alarm);
         }
         //call task handler if task is set
         priority_queue_incrementTopPriority(&m_pq, alarm->u32PeriodMs);
         if (alarm->taskPtr >= 0)
         {
            os_task_setEvent(alarm->taskPtr, alarm->eventMask);
         }
      }
      else
      {
         break;
      }
   }
}

#if (!defined(SYSTIME_SIMULATED) && !defined(UNIT_TEST))
THREAD_PROTO(TimerEventWorker,arg){
   SysTime_reset();

   os_scheduler_run();

   for(;;)
   {
      uint8_t running;
      SPINLOCK_ENTER(m_spin);
      running = m_isRunning;
      SPINLOCK_LEAVE(m_spin);
      if(running == 0){
         break;
      }
      SysTime_wait(1);
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

static void initOsTasks(void)
{
   uint32_t i;
   for (i = 0; i<m_cfg->numTasks; i++)
   {
      os_task_create(m_cfg->taskList[i].taskPtr, m_cfg->taskList[i].taskHandlerPtr);
   }
}

static void shutdownOsTasks(void)
{
   uint32_t i;
   for (i = 0; i<m_cfg->numTasks; i++)
   {
      os_task_destroy(m_cfg->taskList[i].taskPtr);
   }
}

static void startOsTasks(void)
{
   uint32_t i;
   for (i = 0; i<m_cfg->numTasks; i++)
   {    
      os_task_start(m_cfg->taskList[i].taskPtr);
   }
}

static void stopOsTasks(void)
{
   uint32_t i;
   for (i = 0; i<m_cfg->numTasks; i++)
   {
      os_task_stop(m_cfg->taskList[i].taskPtr);
   }
}

static void initScheduler(void)
{
   uint32_t i;
   for (i = 0; i<m_cfg->numTasks; i++)
   {
      uint32_t j;
      for (j = 0; j<m_cfg->taskList[i].numAlarms; j++)
      {
         uint32_t u32InitDelayMs = m_cfg->taskList[i].alarms[j].u32InitDelayMs;
         uint32_t u32PeriodMs = m_cfg->taskList[i].alarms[j].u32PeriodMs;
         if (u32InitDelayMs > 0)
         {
            priority_queue_push(&m_pq, (void*)&m_cfg->taskList[i].alarms[j], u32InitDelayMs);
         }
         else
         {
            priority_queue_push(&m_pq, (void*)&m_cfg->taskList[i].alarms[j], u32PeriodMs);
         }
      }
   }
}
