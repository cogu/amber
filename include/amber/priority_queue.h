/*****************************************************************************
* \file      priority_queue.h
* \author    Conny Gustafsson
* \date      2017-10-12
* \brief     Custom priority queue implementation that can be used as scheduler
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
#ifndef OS_PQ_H
#define OS_PQ_H

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "adt_heap.h"
#include "adt_error.h"


//////////////////////////////////////////////////////////////////////////////
// CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
typedef struct priority_queue_tag {
   adt_ary_t elements; //strong references to adt_heap_elem_t
   int32_t startIndex;
}priority_queue_t;

//////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
//constructor/destructor
priority_queue_t* priority_queue_new(void);
void priority_queue_delete(priority_queue_t *self);
void priority_queue_vdelete(void *arg);
void priority_queue_create(priority_queue_t *self);
void priority_queue_destroy(priority_queue_t *self);

//accessors
adt_error_t priority_queue_push(priority_queue_t *self, void *pItem, uint32_t u32Priority);
adt_heap_elem_t* priority_queue_pop(priority_queue_t *self);
adt_heap_elem_t* priority_queue_top(priority_queue_t *self);
uint32_t priority_queue_topPriority(priority_queue_t *self);
void *priority_queue_topItem(priority_queue_t *self);

//special functions
void priority_queue_incrementTopPriority(priority_queue_t *self, uint32_t value);


#endif //OS_PQ_H
