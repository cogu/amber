#include <stdio.h>
#include <unistd.h>
#include "systime.h"
#include "os.h"
#include "osmacro.h"
#include "RteApi.h"

int main(int argc, char **argv)
{
   int i;
   SysTime_init(1);
   os_init(&g_os_cfg);
   os_start();
   SLEEP(100);
   Rte_SetMode_EcuM_Mode(RTE_MODE_EcuM_Mode_RUN);
   for(i=0;i<5;i++)
   {
      SLEEP(1000);
   }
   Rte_SetMode_EcuM_Mode(RTE_MODE_EcuM_Mode_SHUTDOWN);
   SLEEP(100);
   printf("shutting down\n");
   os_stop();
   os_shutdown();
   return 0;
}
