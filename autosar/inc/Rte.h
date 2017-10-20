/*****************************************************************************
* \file      Rte.h
* \author    Conny Gustafsson
* \date      2017-10-20
* \brief     RTE Type definitions
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

#ifndef _RTE_H
#define _RTE_H
#include "Std_Types.h"

/* common errors */
#define RTE_E_OK               ((Std_ReturnType)  0)
#define RTE_E_INVALID          ((Std_ReturnType)  1)

/* overlayed errors */
#define RTE_E_LOST_DATA        ((Std_ReturnType) 64)
#define RTE_E_MAX_AGE_EXCEEDED ((Std_ReturnType) 64)

/* infrastructure errors */
#define RTE_E_TIMEOUT          ((Std_ReturnType)129)
#define RTE_E_LIMIT            ((Std_ReturnType)130)
#define RTE_E_NO_DATA          ((Std_ReturnType)131)

#endif /* _RTE_H */
