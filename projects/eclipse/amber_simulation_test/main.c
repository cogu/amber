#include <stdio.h>
#include <unistd.h>
#include "systime.h"
#include "os.h"
#include "osmacro.h"

int main(int argc, char **argv)
{
   SysTime_initSimulated();
   os_init(&g_os_cfg);
   os_start();
   SLEEP(10);
   int i;
   for(i=0;i<200;i++)
   {
      SysTime_tick(1);
//      SLEEP(10);
      os_scheduler_run();
      SLEEP(1);
   }
   //SLEEP(10);
   os_stop();
   //SLEEP(10);
   os_shutdown();
   return 0;
}
