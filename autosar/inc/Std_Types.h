/*****************************************************************************
* \file      Std_Types.h
* \author    Conny Gustafsson
* \date      2017-10-20
* \brief     Standard type definitions
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

#ifndef STD_TYPES_H
#define STD_TYPES_H

#include "Platform_Types.h"

#define STD_HIGH     1U /* Voltage level 5V or 3.3V */
#define STD_LOW      0U /* Voltage level 0V */

#define STD_ACTIVE   1U /* Logical level active */
#define STD_IDLE     0U /* Logical level idle */

#define STD_ON       1U
#define STD_OFF      0U

#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
#define E_OK 0U
#endif
#define E_NOT_OK 1u

typedef uint8 Std_ReturnType;

#endif  /* STD_TYPES_H */
