/*****************************************************************************
* \file      os_types.h
* \author    Conny Gustafsson
* \date      2017-10-12
* \brief     Amber OS type definitions
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
#ifndef OS_TYPES_H
#define OS_TYPES_H

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <pthread.h>
#endif
#include "osmacro.h"

//////////////////////////////////////////////////////////////////////////////
// CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
#define OS_TASK_HANDLER(x, arg) THREAD_PROTO(x, arg)
#define OS_TASK_HANDLER_PTR(x, arg) THREAD_PROTO_PTR(x, arg)

#define OS_QUIT_EVENT 0 //reserved for shutting down the worker thread
#define OS_USER_EVENT_ID     1
#define OS_INVALID_EVENT_ID 0xFFFF

//forward declarations
struct os_task_tag;

typedef struct os_alarm_cfg_tag
{
   struct os_task_tag *taskPtr;
   uint32_t eventMask;
   uint32_t u32InitDelayMs;
   uint32_t u32PeriodMs;
} os_alarm_cfg_t;

typedef struct os_task_elem_tag
{
   struct os_task_tag *taskPtr;
   OS_TASK_HANDLER_PTR(taskHandlerPtr, arg);
   const os_alarm_cfg_t *alarms;
   uint32_t numAlarms;
} os_task_elem_t;

typedef struct os_cfg_tag
{
   const os_task_elem_t *taskList;
   uint32_t numTasks;
   uint32_t(*timerFunc)(void);
   void(*alarmEventHookFunc)(const os_alarm_cfg_t *alarm);
} os_cfg_t;


//////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////

#endif //OS_TYPES_H
