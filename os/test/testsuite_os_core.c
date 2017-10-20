//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "CuTest.h"
#include "os_core.h"
#include "systime.h"
#include "priority_queue.h"
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#endif


//////////////////////////////////////////////////////////////////////////////
// CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

#define EVENT_MASK_Test_Task1_TMT_5ms                 ((uint32_t) 0x00000001)
#define EVENT_MASK_Test_Task1_TMT_10ms                ((uint32_t) 0x00000002)
#define EVENT_MASK_Test_Task1_TMT_20ms                ((uint32_t) 0x00000004)
#define EVENT_MASK_Test_Task1_OnEntry_EcuM_Mode_RUN   ((uint32_t) 0x00000008)
#define EVENT_MASK_Test_Task1_OnExit_EcuM_Mode_RUN    ((uint32_t) 0x00000010)

#define OS_NUM_ALARMS_Test_Task1                      3
#define OS_NUM_EVENTS_Test_Task1                      5


#define EVENT_MASK_Test_Task2_TMT_20ms                ((uint32_t) 0x00000001)
#define EVENT_MASK_Test_Task2_TMT_50ms                ((uint32_t) 0x00000002)
#define EVENT_MASK_Test_Task2_TMT_100ms               ((uint32_t) 0x00000004)
#define EVENT_MASK_Test_Task2_OnEntry_EcuM_Mode_RUN   ((uint32_t) 0x00000008)
#define EVENT_MASK_Test_Task2_OnExit_EcuM_Mode_RUN    ((uint32_t) 0x00000010)

#define OS_NUM_ALARMS_Test_Task2                   3
#define OS_NUM_EVENTS_Test_Task2                   5

#define NUM_TEST_TASKS 2
//////////////////////////////////////////////////////////////////////////////
// LOCAL FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static void test_os_core(CuTest* tc);
static OS_TASK_HANDLER(Test_Task1, arg);
static OS_TASK_HANDLER(Test_Task2, arg);

//////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// LOCAL VARIABLES
//////////////////////////////////////////////////////////////////////////////
static os_task_t m_os_task_Test_Task1;
static os_task_t m_os_task_Test_Task2;

//////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////


CuSuite* testsuite_os_core(void)
{
	CuSuite* suite = CuSuiteNew();

   SUITE_ADD_TEST(suite, test_os_core);

	return suite;
}

//////////////////////////////////////////////////////////////////////////////
// LOCAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
static void test_os_core(CuTest* tc)
{

   //////////////////////////////////////////////////////////////////////////////
   // PUBLIC VARIABLES
   //////////////////////////////////////////////////////////////////////////////

   const os_alarm_cfg_t os_alarm_cfg_Test_Task1[OS_NUM_ALARMS_Test_Task1] = {
         //OS Task,              Event Mask,                     Init Delay (ms),  Period (ms)
         {&m_os_task_Test_Task1, EVENT_MASK_Test_Task1_TMT_5ms,   0u,               5u},
         {&m_os_task_Test_Task1, EVENT_MASK_Test_Task1_TMT_10ms,  0u,               10u},
         {&m_os_task_Test_Task1, EVENT_MASK_Test_Task1_TMT_20ms,  0u,               20u},
   };

   const os_alarm_cfg_t os_alarm_cfg_Test_Task2[OS_NUM_ALARMS_Test_Task2] = {
         //OS Task,       Event ID,                     Init Delay (ms),  Period (ms)
         {&m_os_task_Test_Task2, EVENT_MASK_Test_Task2_TMT_20ms,  0u,               20u},
         {&m_os_task_Test_Task2, EVENT_MASK_Test_Task2_TMT_50ms,  0u,               50u},
         {&m_os_task_Test_Task2, EVENT_MASK_Test_Task2_TMT_100ms, 0u,               100u},
   };

   const os_task_elem_t os_task_cfg[NUM_TEST_TASKS] = {
         //OS Task,              TaskHandlerPtr,   alarms,                       number of alarms
         {&m_os_task_Test_Task1, Test_Task1,       &os_alarm_cfg_Test_Task1[0], OS_NUM_ALARMS_Test_Task1},
         {&m_os_task_Test_Task2, Test_Task2,       &os_alarm_cfg_Test_Task2[0], OS_NUM_ALARMS_Test_Task2},
   };

   os_cfg_t os_cfg = {
         &os_task_cfg[0],
         NUM_TEST_TASKS,
         0,
         0
   };

   priority_queue_t *pq;
   os_alarm_cfg_t *alarm;
   adt_heap_elem_t *heap_elem;



   SysTime_initSimulated();
   CuAssertUIntEquals(tc, 0, SysTime_getTime());
   os_init(&os_cfg);
   pq = os_getPriorityQueue();
   CuAssertPtrNotNull(tc, pq);
   heap_elem = priority_queue_top(pq);
   alarm = (os_alarm_cfg_t*) heap_elem->pItem;
   CuAssertPtrNotNull(tc, alarm);
   CuAssertUIntEquals(tc, 5, heap_elem->u32Value);
   CuAssertPtrEquals(tc, &m_os_task_Test_Task1, alarm->taskPtr);
   CuAssertUIntEquals(tc, EVENT_MASK_Test_Task1_TMT_5ms, alarm->eventMask);
   SysTime_tick(5);
   os_scheduler_run();
   SysTime_tick(5);
   os_scheduler_run();
   SysTime_tick(5);
   os_scheduler_run();
   SysTime_tick(5);
   os_scheduler_run();

//   os_task_create(&m_os_Test_Task, Test_Task, 100);
//   os_tem_init(m_timerEventCfg, m_timerEventState, sizeof(m_timerEventState) / sizeof(m_timerEventState[0]));
//   os_tem_run();
//   os_tem_run();
//   os_tem_run();
}

OS_TASK_HANDLER(Test_Task1, arg)
{
   bool isRunning = true;
   os_task_t *self = (os_task_t*)arg;
   if (self == 0)
   {
      fprintf(stderr, "Error: Test_Task1 called with null argument\n");
      THREAD_RETURN(0);
   }

   while (isRunning == true)
   {

   }
   THREAD_RETURN(0);
}

OS_TASK_HANDLER(Test_Task2, arg)
{
   bool isRunning = true;
   os_task_t *self = (os_task_t*)arg;
   if (self == 0)
   {
      fprintf(stderr, "Error: Test_Task1 called with null argument\n");
      THREAD_RETURN(0);
   }

   printf("Test_Task2: entering main loop\n");
   while (isRunning == true)
   {
      uint32_t eventMask;
      int8_t result = os_task_waitEvent(self, &eventMask);
      if (result == 0)
      {
         if (eventMask & EVENT_MASK_Test_Task2_TMT_20ms)
         {
            printf("EVENT_MASK_Test_Task2_TMT_20ms");
         }
         if (eventMask & EVENT_MASK_Test_Task2_TMT_50ms)
         {
            printf("EVENT_MASK_Test_Task2_TMT_50ms");
         }
         if (eventMask & EVENT_MASK_Test_Task2_TMT_100ms)
         {
            printf("EVENT_MASK_Test_Task2_TMT_100ms");
         }
      }
      else if(result > 0)
      {
         isRunning = false;
      }
      else
      {
         fprintf(stderr, "os_task_waitEvent failed\n");
      }
   }
   THREAD_RETURN(0);
}
