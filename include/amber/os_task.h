/*****************************************************************************
* \file      os_task.h
* \author    Conny Gustafsson
* \date      2017-10-12
* \brief     Amber OS task
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
#ifndef OS_TASK_H
#define OS_TASK_H

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#include <semaphore.h>
#endif
#include <stdbool.h>
#include "osmacro.h"
#include "amber/os_types.h"
#include "amber/os_error.h"



//////////////////////////////////////////////////////////////////////////////
// CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
typedef struct os_task_tag
{
   OS_TASK_HANDLER_PTR(worker_func, arg);
   THREAD_T worker_thread;
   MUTEX_T mutex;
#ifdef _WIN32   
   unsigned int thread_id;
   HANDLE event_handle; //Windows event is used to signal when event mask has been modified
#else	
	pthread_cond_t cond_var; //used to signal when event mask has been modified
#endif
	uint32_t event_mask; //each bit represents 1 event flag.
	bool worker_thread_valid;
	bool quit_event;
} os_task_t;


//////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
os_error_t os_task_create(os_task_t *self, OS_TASK_HANDLER_PTR(handler, arg));
void os_task_destroy(os_task_t *self);
void os_task_start(os_task_t *self);
void os_task_stop(os_task_t *self);
void os_task_set_event(os_task_t *self, uint32_t event_mask);
os_error_t os_task_wait_event(os_task_t *self, uint32_t *event_mask);
void os_task_clear_event(os_task_t *self);

#endif //OS_TASK_H
