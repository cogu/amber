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
#include "amber/os_alarm.h" 
#include "amber/os_core.h" 
#include "amber/os_systime.h"
#include "amber/os_scheduler.h"
#include "os_task_cfg.h"
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
// LOCAL FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static os_error_t create_tasks(const os_taskCfg_t* task_cfg_list, uint32_t num_tasks);
static os_error_t create_alarms(const os_taskCfg_t* task_cfg_list, uint32_t num_tasks);
static void destroy_tasks(void);
static void destroy_alarms(void);
static void start_tasks(void);
static void stop_tasks(void);
static os_error_t init_scheduler(void);

//////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// LOCAL VARIABLES
//////////////////////////////////////////////////////////////////////////////
static os_task_t* m_task_list = NULL;
static os_alarm_t* m_alarm_list = NULL;
static uint32_t m_num_tasks = 0u;
static uint32_t m_num_alarms = 0u;
//////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

os_error_t os_init(const os_taskCfg_t* task_cfg_list, uint32_t num_tasks)
{         
   int rc;
   os_error_t retval = OS_NO_ERROR;
#if !defined(UNIT_TEST) && !defined(SYSTIME_SIMULATED)
   rc = os_systime_init(ONE_MILLISECOND);
   if (rc != 0)
   {
      retval = OS_SYSTIME_ERROR;
   }
#endif
   if (retval == OS_NO_ERROR)
   {
      retval = create_tasks(task_cfg_list, num_tasks);
   }
   if (retval == OS_NO_ERROR)
   {
      retval = create_alarms(task_cfg_list, num_tasks);
   }  
   if (retval == OS_NO_ERROR)
   {
      retval = init_scheduler();
   }   
   return retval;
}

void os_shutdown(void)
{
   os_scheduler_shutdown();
   os_systime_destroy();
   destroy_alarms();
   destroy_tasks();   
}

void os_start(void)
{
   start_tasks();
   os_scheduler_start();
}

void os_stop(void)
{
   os_scheduler_stop();
   stop_tasks();
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

static os_error_t create_tasks(const os_taskCfg_t* task_cfg_list, uint32_t num_tasks)
{
   uint32_t i;
   m_num_tasks = num_tasks;
   m_num_alarms = 0u;
   m_task_list = (os_task_t*) malloc(sizeof(os_task_t) * num_tasks);
   if (m_task_list == NULL)
   {
      return OS_MEM_ERROR;
   }  
   for (i = 0u; i < num_tasks; i++)
   {      
      os_error_t result = os_task_create(&m_task_list[i], task_cfg_list[i].task_handler);
      if (result != OS_NO_ERROR)
      {
         return result;
      }
      os_task_instance[i] = &m_task_list[i];
      m_num_alarms += task_cfg_list[i].num_alarms;
   }
   return OS_NO_ERROR;
}

static os_error_t create_alarms(const os_taskCfg_t* task_cfg_list, uint32_t num_tasks)
{
   uint32_t i;
   uint32_t alarm_index = 0u;
   if (m_num_alarms > 0u)
   {
      m_alarm_list = (os_alarm_t*)malloc(sizeof(os_alarm_t) * m_num_alarms);
      if (m_alarm_list == NULL)
      {
         return OS_MEM_ERROR;
      }
      for (i = 0u; i < num_tasks; i++)
      {
         uint32_t j;
         for (j = 0; j < task_cfg_list[i].num_alarms; j++)
         {
            const os_alarmCfg_t* alarm_cfg = NULL;
            if (alarm_index >= m_num_alarms)
            {
               return OS_INDEX_ERROR;
            }
            alarm_cfg = &task_cfg_list[i].alarm_list[j];
            os_alarm_create(&m_alarm_list[alarm_index],
               &m_task_list[i],
               alarm_cfg->event_mask,
               alarm_cfg->init_delay_ms,
               alarm_cfg->period_ms);
            alarm_index++;
         }         
      }
   }
   return OS_NO_ERROR;
}

static void destroy_tasks(void)
{
   if (m_task_list != NULL)
   {
      uint32_t i;
      for (i = 0; i < m_num_tasks; i++)
      {
         os_task_destroy(&m_task_list[i]);
      }
      free(m_task_list);
   }
}

static void destroy_alarms(void)
{   
   if (m_alarm_list != NULL)
   {
      free(m_alarm_list);
   }
}

static void start_tasks(void)
{
   uint32_t i;
   for (i = 0; i<m_num_tasks; i++)
   {    
      os_task_start(&m_task_list[i]);
   }
}

static void stop_tasks(void)
{
   uint32_t i;
   for (i = 0; i < m_num_tasks; i++)
   {
      os_task_stop(&m_task_list[i]);
   }
}

static os_error_t init_scheduler(void)
{   
   uint32_t i;
   os_scheduler_init();
   for (i = 0; i< m_num_alarms; i++)
   {
      os_error_t result = os_scheduler_insert(&m_alarm_list[i]);
      if (result != OS_NO_ERROR)
      {
         return result;
      }
   }
   return OS_NO_ERROR;
}

