/*****************************************************************************
* \file      RteTask.c
* \author    Conny Gustafsson
* \date      2017-10-13
* \brief     Description
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
#include <stdio.h>
#include "os.h"
#include "os_event_cfg.h"
#include "os_task_cfg.h"
#include "systime.h"

//////////////////////////////////////////////////////////////////////////////
// PRIVATE CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
OS_TASK_HANDLER(Test_Task1, arg)
{
   bool isRunning = true;
   os_task_t *self = (os_task_t*)arg;
   if (self == 0)
   {
      fprintf(stderr, "Error: Test_Task1 called with null argument\n");
      THREAD_RETURN(0);
   }

   printf("Test_Task1: entering main loop\n");
   while (isRunning == true)
   {
      uint32_t eventMask;
      int8_t result = os_task_waitEvent(self, &eventMask);
      if (result == 0)
      {
         if (eventMask & EVENT_MASK_Test_Task1_TMT_5ms)
         {
            //printf("%u: EVENT_MASK_Test_Task1_TMT_5ms\n", (unsigned int) SysTime_getTime());
         }
         if (eventMask & EVENT_MASK_Test_Task1_TMT_10ms)
         {
            //printf("%u: EVENT_MASK_Test_Task1_TMT_10ms\n", (unsigned int) SysTime_getTime());
         }
         if (eventMask & EVENT_MASK_Test_Task1_TMT_20ms)
         {
            //printf("%u: EVENT_MASK_Test_Task1_TMT_20ms\n", (unsigned int) SysTime_getTime());
         }
      }
      else if(result > 0)
      {
         printf("Test_Task1_QuitEvent\n");
         isRunning = false;
      }
      else
      {
         fprintf(stderr, "os_task_waitEvent failed\n");
      }
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
            //printf("%u: EVENT_MASK_Test_Task2_TMT_20ms\n", (unsigned int) SysTime_getTime());
         }
         if (eventMask & EVENT_MASK_Test_Task2_TMT_50ms)
         {
            //printf("%u: EVENT_MASK_Test_Task2_TMT_50ms\n", (unsigned int) SysTime_getTime());
         }
         if (eventMask & EVENT_MASK_Test_Task2_TMT_100ms)
         {
            //printf("%u: EVENT_MASK_Test_Task2_TMT_100ms\n", (unsigned int) SysTime_getTime());
         }
      }
      else if(result > 0)
      {
         printf("Test_Task2_QuitEvent\n");
         isRunning = false;
      }
      else
      {
         fprintf(stderr, "os_task_waitEvent failed\n");
      }
   }
   THREAD_RETURN(0);
}
//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////


