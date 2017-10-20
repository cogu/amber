/*****************************************************************************
* \file      os_events_cfg.h
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
#ifndef OS_EVENTS_CFG_H
#define OS_EVENTS_CFG_H

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "Platform_Types.h"

//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
#define EVENT_MASK_Test_Task1_TMT_5ms                 ((uint32) 0x00000001)
#define EVENT_MASK_Test_Task1_TMT_10ms                ((uint32) 0x00000002)
#define EVENT_MASK_Test_Task1_TMT_20ms                ((uint32) 0x00000004)
#define EVENT_MASK_Test_Task1_OnEntry_EcuM_Mode_RUN   ((uint32) 0x00000008)
#define EVENT_MASK_Test_Task1_OnExit_EcuM_Mode_RUN    ((uint32) 0x00000010)

#define OS_NUM_ALARMS_Test_Task1                      3
#define OS_NUM_EVENTS_Test_Task1                      5


#define EVENT_MASK_Test_Task2_TMT_20ms                ((uint32) 0x00000001)
#define EVENT_MASK_Test_Task2_TMT_50ms                ((uint32) 0x00000002)
#define EVENT_MASK_Test_Task2_TMT_100ms               ((uint32) 0x00000004)
#define EVENT_MASK_Test_Task2_OnEntry_EcuM_Mode_RUN   ((uint32) 0x00000008)
#define EVENT_MASK_Test_Task2_OnExit_EcuM_Mode_RUN    ((uint32) 0x00000010)

#define OS_NUM_ALARMS_Test_Task2                   3
#define OS_NUM_EVENTS_Test_Task2                   5


//////////////////////////////////////////////////////////////////////////////
// PUBLIC VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////


#endif //OS_EVENTS_CFG_H
