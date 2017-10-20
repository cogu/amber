/*****************************************************************************
* \file      os_task_cfg.c
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
//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "os_event_cfg.h"
#include "os_task_cfg.h"

//////////////////////////////////////////////////////////////////////////////
// PRIVATE CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////
static os_task_t m_os_task_Test_Task1;
static os_task_t m_os_task_Test_Task2;


//////////////////////////////////////////////////////////////////////////////
// PUBLIC VARIABLES
//////////////////////////////////////////////////////////////////////////////
const os_alarm_cfg_t os_alarm_cfg_Test_Task1[OS_NUM_ALARMS_Test_Task1] = {
      //OS Task,              Event Mask,                     Init Delay (ms),  Period (ms)
      {&m_os_task_Test_Task1, EVENT_MASK_Test_Task1_TMT_5ms,   0u,               5u},
      {&m_os_task_Test_Task1, EVENT_MASK_Test_Task1_TMT_10ms,  0u,               10u},
      {&m_os_task_Test_Task1, EVENT_MASK_Test_Task1_TMT_20ms,  0u,               20u},
};

const os_alarm_cfg_t os_alarm_cfg_Test_Task2[OS_NUM_ALARMS_Test_Task2] = {
      //OS Task,       Event ID,                     Init Delay (ms),  Period (ms)
      {&m_os_task_Test_Task2, EVENT_MASK_Test_Task2_TMT_20ms,  0u,               20u},
      {&m_os_task_Test_Task2, EVENT_MASK_Test_Task2_TMT_50ms,  0u,               50u},
      {&m_os_task_Test_Task2, EVENT_MASK_Test_Task2_TMT_100ms, 0u,               100u},
};

const os_task_elem_t os_task_cfg[OS_NUM_TASKS] = {
      //OS Task,              TaskHandlerPtr,   alarms,                       number of alarms
      {&m_os_task_Test_Task1, Test_Task1,       &os_alarm_cfg_Test_Task1[0], OS_NUM_ALARMS_Test_Task1},
      {&m_os_task_Test_Task2, Test_Task2,       &os_alarm_cfg_Test_Task2[0], OS_NUM_ALARMS_Test_Task2},
};

os_cfg_t g_os_cfg = {
      &os_task_cfg[0],
      OS_NUM_TASKS,
      0,
      0
};

//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
void os_task_onExit_EcuM_Mode_RUN(void)
{
   os_task_setEvent(&m_os_task_Test_Task1, EVENT_MASK_Test_Task1_OnExit_EcuM_Mode_RUN);
   os_task_setEvent(&m_os_task_Test_Task2, EVENT_MASK_Test_Task2_OnExit_EcuM_Mode_RUN);
}

void os_task_onEntry_EcuM_Mode_RUN(void)
{
   os_task_setEvent(&m_os_task_Test_Task1, EVENT_MASK_Test_Task1_OnEntry_EcuM_Mode_RUN);
   os_task_setEvent(&m_os_task_Test_Task2, EVENT_MASK_Test_Task2_OnEntry_EcuM_Mode_RUN);
}

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////


