/*****************************************************************************
* \file      systime_wl.c
* \author    Conny Gustafsson
* \date      2013-10-01
* \brief     System Time simulation for Windows and Linux
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

/********************************* Includes **********************************/
#include <stdio.h>
#if defined(_WIN32) || defined(__CYGWIN__)
#include <Windows.h>
#else
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#endif
#include "systime.h"
#ifdef USE_PLATFORM_TYPES
#define _UINT8 uint8
#define _UINT32 uint32
#else
#define _UINT8 uint8_t
#define _UINT32 uint32_t
#endif


/**************************** Constants and Types ****************************/
#if defined(_WIN32) || defined(__CYGWIN__)
#define SKIP_ITERATIONS  100
#define MAX_ITERATIONS 1000000
#define TIMER_RESOLUTION_1MS 10000U //1ms
#define TIMER_RESOLUTION_05MS 5000U //0,5ms
#define TIMER_TIMEOUT_MS 10U

typedef LONG(CALLBACK* NTSETTIMERRESOLUTION)
(
   IN ULONG DesiredTime,
   IN BOOLEAN SetResolution,
   OUT PULONG ActualTime
);

typedef LONG(CALLBACK* NTQUERYTIMERRESOLUTION)
(
   OUT PULONG MaximumTime,
   OUT PULONG MinimumTime,
   OUT PULONG CurrentTime
);
#else
static struct timespec m_sleepTime = { 0, 1000000*10};
static struct timespec m_beginTime;
static struct timespec m_nextTime;
#endif
/********************************* Variables *********************************/
#if defined(_WIN32) || defined(__CYGWIN__)
NTSETTIMERRESOLUTION NtSetTimerResolution;
NTQUERYTIMERRESOLUTION NtQueryTimerResolution;
LARGE_INTEGER m_freq;
static HANDLE m_hTimer = INVALID_HANDLE_VALUE;
LARGE_INTEGER m_ref;
#endif
static _UINT32 m_u32SystimeTick;

/************************* Local Function Prototypes *************************/
#if !defined(_WIN32) && !defined(__CYGWIN__)
static struct timespec timespec_add(struct  timespec  time1,struct  timespec  time2);
static struct timespec timespec_subtract(struct  timespec  time1,struct  timespec  time2);
#endif
/***************************** Exported Functions ****************************/
#if defined(UNIT_TEST) || defined(SYSTIME_SIMULATED)
void SysTime_initSimulated(void)
{
	m_isSimulated = 1u;
   SysTime_reset();
}

void SysTime_tick(_UINT32 tickMs)
{
   m_u32SystimeTick += tickMs;
}
#endif

int SysTime_init(_UINT32 tickMs)
{
#if defined(_WIN32) || defined(__CYGWIN__)
   //Windows
   ULONG min, max, cur;
   HMODULE hNtDll;
   LARGE_INTEGER dt;
   BOOL b;
   int i, n = 0;
   QueryPerformanceFrequency(&m_freq);
   QueryPerformanceCounter(&m_ref);
   hNtDll = LoadLibrary("NtDll.dll");
   if (!hNtDll)
   {
      return -2;
   }
   
   NtQueryTimerResolution = (NTQUERYTIMERRESOLUTION)GetProcAddress(hNtDll, "NtQueryTimerResolution");
   NtSetTimerResolution = (NTSETTIMERRESOLUTION)GetProcAddress(hNtDll, "NtSetTimerResolution");
   
   NtQueryTimerResolution(&max, &min, &cur);
   NtSetTimerResolution(TIMER_RESOLUTION_05MS, TRUE, &cur);
#ifdef SYSTIME_STAT
   printf("[systime] TimerResolution: %lu %lu %lu\n", (unsigned long)max, (unsigned long)min, (unsigned long)cur);
#endif
   FreeLibrary(hNtDll);
   
   
   dt.QuadPart = TIMER_RESOLUTION_1MS * tickMs;
   m_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
   if ((m_hTimer == INVALID_HANDLE_VALUE) || (m_hTimer == 0))
   {
      printf("CreateWaitableTimer failed\n");
      return -1;
   }
   b = SetWaitableTimer(m_hTimer, &dt, tickMs, NULL, NULL, FALSE);
   if (!b)
   {
      printf("SetWaitableTimer failed\n");
      return -1;
   }
   
   //run timer for some time to get rid of initial jitter
   for (i = 0; i < MAX_ITERATIONS; ++i)
   {
      if (WaitForSingleObject(m_hTimer, 0) == WAIT_OBJECT_0)
      {
         n++;
         if (n >= SKIP_ITERATIONS)
         {
            break;
         }
      }
   }
   SysTime_reset();
   return 0;
#else
	//Linux
   if( (tickMs>0) && (tickMs<=1000) ){
      m_sleepTime.tv_sec = 0;
      m_sleepTime.tv_nsec = tickMs*1000000;
      SysTime_reset();
      return 0;
   }
   return -1;
#endif
}

void SysTime_destroy(void)
{	
#if defined(_WIN32) || defined(__CYGWIN__)
	if (m_hTimer != INVALID_HANDLE_VALUE )
   {
      CloseHandle(m_hTimer);
   }
#endif   
}

_UINT32 SysTime_getTime(void){
   return m_u32SystimeTick;
}


#if (!defined(UNIT_TEST)) && (!defined(SYSTIME_SIMULATED))
_UINT8 SysTime_wait(int isBlocking)
{
#if defined(_WIN32) || defined(__CYGWIN__)
   if ( m_hTimer != INVALID_HANDLE_VALUE )
   {
      DWORD timeout = isBlocking ? INFINITE : 0;
      if (WaitForSingleObject(m_hTimer, timeout) == WAIT_OBJECT_0)
      {
         m_u32SystimeTick++;
         return 1;
      }
   }
   return 0;
#else
   if( (m_isSimulated == 0) && (isBlocking != 0))
   {
      clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,&m_nextTime, NULL);
      m_nextTime=timespec_add(m_nextTime,m_sleepTime);
   }
   else
   {
      return 0; //nonblocking is not supported in Linux version
   }
   g_u32SystimeTick++;
   return 1;
#endif   
}

#endif

void SysTime_reset(void)
{
#if (!defined(UNIT_TEST)) && (!defined(SYSTIME_SIMULATED))

# if defined(_WIN32) || defined(__CYGWIN__)
   SysTime_wait(1);
   QueryPerformanceCounter(&m_ref);
# else
   clock_gettime(CLOCK_MONOTONIC, &m_beginTime);
   m_nextTime = timespec_add(m_beginTime,m_sleepTime);
# endif

#endif
   m_u32SystimeTick = 0;
}


/****************************** Local Functions ******************************/
#if !defined(_WIN32) && !defined(__CYGWIN__)
struct  timespec  timespec_add (struct  timespec  time1,struct  timespec  time2){
    struct  timespec  result ;
    result.tv_sec = time1.tv_sec + time2.tv_sec ;
    result.tv_nsec = time1.tv_nsec + time2.tv_nsec ;
    if (result.tv_nsec >= 1000000000L) {     /* Carry? */
        result.tv_sec++ ;  result.tv_nsec = result.tv_nsec - 1000000000L ;
    }
    return (result) ;
}

struct  timespec  timespec_subtract (struct  timespec  time1,struct  timespec  time2){
    struct  timespec  result ;
    /* Subtract the second time from the first. */
    if ((time1.tv_sec < time2.tv_sec) ||
        ((time1.tv_sec == time2.tv_sec) &&
         (time1.tv_nsec <= time2.tv_nsec))) {      /* TIME1 <= TIME2? */
        result.tv_sec = result.tv_nsec = 0 ;
    } else {                  /* TIME1 > TIME2 */
        result.tv_sec = time1.tv_sec - time2.tv_sec ;
        if (time1.tv_nsec < time2.tv_nsec) {
            result.tv_nsec = time1.tv_nsec + 1000000000L - time2.tv_nsec ;
            result.tv_sec-- ;          /* Borrow a second. */
        } else {
            result.tv_nsec = time1.tv_nsec - time2.tv_nsec ;
        }
    }

    return (result) ;

}
#endif
