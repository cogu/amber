/*****************************************************************************
* \file      adt_ringbuf_cfg.h
* \author    Conny Gustafsson
* \date      2013-12-19
* \brief     Ringbuffer data structure
*
* Copyright (c) 2013-2017 Conny Gustafsson
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

/***
* This is a template file, copy and rename this file to adt_ringbuf_cfg.h.
* Users should copy this file to the file name adt_ringbuf_cfg.h and place it amongst local project includes.
*/

#ifndef ADT_RINGBUF_CFG_H__
#define ADT_RINGBUF_CFG_H__

#define RBFS_ENABLE 1    //ringbuffer static (fixed sized blocks)
#define RBFD_ENABLE 0    //ringbuffer dynamic (dynamically sized blocks)
#define RBFU16_ENABLE 1  //special ringbuffer for uint16 values

#endif //RINGBUF_CFG_H__
