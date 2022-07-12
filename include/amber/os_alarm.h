/*****************************************************************************
* \file      os_alarm.h
* \author    Conny Gustafsson
* \date      2021-03-16
* \brief     OS alarm class
*
* Copyright (c) 2021 Conny Gustafsson
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
#ifndef OS_ALARM_H
#define OS_ALARM_H

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "amber/os_types.h"
#include "amber/os_task.h"

//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
typedef struct os_alarm_tag
{
   os_task_t* task; //weak reference
   os_eventMask_t event_mask;
   uint32_t init_delay_ms;
   uint32_t period_ms;
} os_alarm_t;

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
void os_alarm_create(os_alarm_t* self, os_task_t* task, os_eventMask_t event_mask, uint32_t init_delay_ms, uint32_t period_ms);
os_alarm_t* os_alarm_new(os_task_t* task, os_eventMask_t event_mask, uint32_t init_delay_ms, uint32_t period_ms);
void os_alarm_delete(os_alarm_t* self);

#endif //OS_ALARM_H
