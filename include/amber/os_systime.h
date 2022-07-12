/*****************************************************************************
* \file      os_time.h
* \author    Conny Gustafsson
* \date      2013-10-01
* \brief     System time simulation layer for Linux and Windows
*
* Copyright (c) 2013-2021 Conny Gustafsson
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
#ifndef OS_SYS_TIME_H
#define OS_SYS_TIME_H


/********************************* Includes **********************************/
#ifdef USE_PLATFORM_TYPES
#include "Platform_Types.h"
#define _UINT8 uint8
#define _UINT32 uint32
#else
#include <stdint.h>
#define _UINT8 uint8_t
#define _UINT32 uint32_t
#endif


/**************************** Constants and Types ****************************/

/********************************* Functions *********************************/
#if defined(UNIT_TEST) || defined(SYSTIME_SIMULATED)
void os_systime_init_simulated(void);
void os_systime_tick(_UINT32 tick_ms);
#endif
int os_systime_init(_UINT32 tick_ms);
void os_systime_destroy(void);
_UINT8 os_systime_wait(int is_blocking);
_UINT32 os_systime_get_time(void);
void os_systime_reset(void);

#undef _UINT8
#undef _UINT32

#endif //OS_SYS_TIME_H

