/*****************************************************************************
* \file      systime.h
* \author    Conny Gustafsson
* \date      2013-10-01
* \brief     System Time API
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
#ifndef SYS_TIME_H
#define SYS_TIME_H


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
void SysTime_initSimulated(void);
void SysTime_tick(_UINT32 tickMs);
#endif
int SysTime_init(_UINT32 tickMs);

void SysTime_destroy(void);
_UINT8 SysTime_wait(int isBlocking);
_UINT32 SysTime_getTime(void);
void SysTime_reset(void);

#undef _UINT8
#undef _UINT32

#endif //SYS_TIME_H

