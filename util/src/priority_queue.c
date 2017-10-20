/*****************************************************************************
* \file      priority_queue.c
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
//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <malloc.h>
#include "priority_queue.h"

//////////////////////////////////////////////////////////////////////////////
// CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// LOCAL FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// LOCAL VARIABLES
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
priority_queue_t* priority_queue_new(void)
{
   priority_queue_t *self = (priority_queue_t*) malloc(sizeof(priority_queue_t));
   if (self != (priority_queue_t*) 0)
   {
      priority_queue_create(self);
   }
   return self;
}

void priority_queue_delete(priority_queue_t *self)
{
   if (self != 0)
   {
      priority_queue_destroy(self);
      free(self);
   }
}

void priority_queue_vdelete(void *arg)
{
   priority_queue_delete((priority_queue_t*) arg);
}
void priority_queue_create(priority_queue_t *self)
{
   if (self != 0)
   {
      adt_ary_create(&self->elements, adt_heap_elem_vdelete);
      self->startIndex = 0;
   }
}

void priority_queue_destroy(priority_queue_t *self)
{
   if (self != 0)
   {
      adt_ary_destroy(&self->elements);
   }
}

//accessors
void priority_queue_push(priority_queue_t *self, void *pItem, uint32_t u32Priority)
{
   if (self != 0)
   {
      adt_heap_elem_t *pElem = adt_heap_elem_new(pItem, u32Priority);
      if (pElem != 0)
      {
         adt_ary_push(&self->elements, pElem);
         adt_heap_sortUp(&self->elements, adt_ary_length(&self->elements)-1, ADT_MIN_HEAP);
      }
   }
}

adt_heap_elem_t* priority_queue_pop(priority_queue_t *self)
{
   if (self != 0)
   {
      void **ppElem;
      ppElem = adt_ary_shift(&self->elements);
      if (ppElem != 0)
      {
         return (adt_heap_elem_t*) *ppElem;
      }
   }
   return (adt_heap_elem_t*) 0;
}

adt_heap_elem_t* priority_queue_top(priority_queue_t *self)
{
   if (self != 0)
   {
      void **ppElem;
      ppElem = adt_ary_get(&self->elements, 0);
      if (ppElem != 0)
      {
         return (adt_heap_elem_t*) *ppElem;
      }
   }
   return (adt_heap_elem_t*) 0;
}

uint32_t priority_queue_topPriority(priority_queue_t *self)
{
   if (self != 0)
   {
      void **ppElem;
      ppElem = adt_ary_get(&self->elements, 0);
      if (ppElem != 0)
      {
         adt_heap_elem_t *pElem = (adt_heap_elem_t*) ppElem;
         return pElem->u32Value;
      }
   }
   return 0;
}

void *priority_queue_topItem(priority_queue_t *self)
{
   if (self != 0)
   {
      void **ppElem;
      ppElem = adt_ary_get(&self->elements, 0);
      if (ppElem != 0)
      {
         adt_heap_elem_t *pElem = (adt_heap_elem_t*) ppElem;
         return pElem->pItem;
      }
   }
   return (void*) 0;
}


//special function
///TODO: this function needs to support overflow situation, right now it doesn't
void priority_queue_incrementTopPriority(priority_queue_t *self, uint32_t value)
{
   if (self != 0)
   {
      adt_heap_elem_t *pElem = priority_queue_top(self);
      if (pElem != 0)
      {
         pElem->u32Value += value; ///FIXME: handle overflow situation
         adt_heap_sortDown(&self->elements, 0, ADT_MIN_HEAP);
      }
   }
}



//////////////////////////////////////////////////////////////////////////////
// LOCAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////


