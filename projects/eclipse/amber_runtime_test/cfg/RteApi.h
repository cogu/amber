/*****************************************************************************
* \file      RteApi.h
* \author    Conny Gustafsson
* \date      2017-10-14
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
#ifndef RTE_API_H
#define RTE_API_H

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "Platform_Types.h"


//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
typedef uint8 Rte_ModeType_EcuM_Mode;

# define RTE_MODE_EcuM_Mode_POST_RUN ((Rte_ModeType_EcuM_Mode)0)
# define RTE_MODE_EcuM_Mode_RUN ((Rte_ModeType_EcuM_Mode)1)
# define RTE_MODE_EcuM_Mode_SHUTDOWN ((Rte_ModeType_EcuM_Mode)2)
# define RTE_MODE_EcuM_Mode_SLEEP ((Rte_ModeType_EcuM_Mode)3)
# define RTE_MODE_EcuM_Mode_STARTUP ((Rte_ModeType_EcuM_Mode)4)
# define RTE_MODE_EcuM_Mode_WAKE_SLEEP ((Rte_ModeType_EcuM_Mode)5)

//////////////////////////////////////////////////////////////////////////////
// PUBLIC VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
void Rte_Start(void);
void Rte_SetMode_EcuM_Mode(Rte_ModeType_EcuM_Mode newMode);

#endif //RTE_API_H
