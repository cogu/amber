/*****************************************************************************
* \file      Platform_Types.h
* \author    Conny Gustafsson
* \date      2017-10-20
* \brief     Platform Types for x86-based systems
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

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#define CPU_TYPE_8       8
#define CPU_TYPE_16      16
#define CPU_TYPE_32      32

#define MSB_FIRST        0
#define LSB_FIRST        1

#define HIGH_BYTE_FIRST  0
#define LOW_BYTE_FIRST   1

#define CPU_TYPE         CPU_TYPE_32
#define CPU_BIT_ORDER    LSB_FIRST
#define CPU_BYTE_ORDER   LOW_BYTE_FIRST

#ifndef TRUE
#define TRUE ((boolean)1)
#endif
#ifndef FALSE
#define FALSE ((boolean)0)
#endif
                      
typedef unsigned char         boolean;       /*        TRUE..FALSE          */
typedef signed char           sint8;         /*        -128..127            */
typedef unsigned char         uint8;         /*           0..255            */
typedef signed short          sint16;        /*      -32768..32767          */
typedef unsigned short        uint16;        /*           0..65535          */
typedef signed int           sint32;        /* -2147483648..2147483647     */
typedef unsigned int         uint32;        /*           0..4294967295     */
                                        
typedef signed char           sint8_least;
typedef unsigned char         uint8_least;
typedef signed short          sint16_least;
typedef unsigned short        uint16_least;
typedef signed int           sint32_least;
typedef unsigned int         uint32_least;
                                        
typedef float                 float32;
typedef double                float64;

#endif  /* PLATFORM_TYPES_H */
