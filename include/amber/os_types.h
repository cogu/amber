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
#include <stdint.h>
#include <stddef.h>
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <pthread.h>
#endif
#include "osmacro.h"


//////////////////////////////////////////////////////////////////////////////
// CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

#if defined(UNIT_TEST) && !defined(SYSTIME_SIMULATED)
#define SYSTIME_SIMULATED
#endif

#define OS_TASK_HANDLER(x, arg) THREAD_PROTO(x, arg)
#define OS_TASK_HANDLER_PTR(x, arg) THREAD_PROTO_PTR(x, arg)

#define OS_QUIT_EVENT 0 //reserved for shutting down the worker thread
#define OS_USER_EVENT_ID     1
#define OS_INVALID_EVENT_ID 0xFFFF

//forward declarations
struct os_task_tag;

typedef uint32_t os_eventMask_t;

typedef struct os_alarmCfg_tag
{      
   os_eventMask_t event_mask;
   uint32_t init_delay_ms;
   uint32_t period_ms;
} os_alarmCfg_t;

typedef struct os_taskCfg_tag
{   
   OS_TASK_HANDLER_PTR(task_handler, arg);
   const os_alarmCfg_t *alarm_list;
   uint32_t num_alarms;
} os_taskCfg_t;


typedef void (os_alarmEventHookFuncType)(struct os_task_tag *task, os_eventMask_t event_mask);
typedef uint32_t (os_systemTimeFuncType)(void);

//////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////

#endif //OS_TYPES_H
