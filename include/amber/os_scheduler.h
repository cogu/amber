/*****************************************************************************
* \file      os_scheduler.h
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
#ifndef OS_SCHEDULER_H
#define OS_SCHEDULER_H

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "amber/os_types.h"
#include "amber/os_error.h"
#include "amber/os_alarm.h"
#include "amber/priority_queue.h"
//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
void os_scheduler_init(void);
void os_scheduler_start(void);
os_error_t os_scheduler_insert(os_alarm_t *alarm);
void os_scheduler_stop(void);
void os_scheduler_shutdown(void);

#if defined(UNIT_TEST) || defined(SYSTIME_SIMULATED)
#define DYN_STATIC
DYN_STATIC void os_scheduler_run(void);
DYN_STATIC priority_queue_t *os_scheduler_get_priority_queue(void);
#else
#define DYN_STATIC static
#endif


#endif //OS_SCHEDULER_H
