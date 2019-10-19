#include "monitor_thread_runnig_thread.h"
UINT32 thread_dsp=1;
UINT32 thread_m4_send=1;
UINT32 thread_m4_receive=1;
UINT32 thread_pc=1;
UINT32 thread_switch=1;
UINT32 thread_battery=1;
UINT32 thread_failure=1;
UINT32 thread_tcp=1;
UINT32 thread_usb=1;
UINT32 thread_interface_frame=1;

Monitor_Thread_Runnig_Thread::Monitor_Thread_Runnig_Thread(QObject *parent) :
  QThread(parent)
{
  loop = 0;
}
void Monitor_Thread_Runnig_Thread::run()
{ bool watchdogenable=1;
  sleep(20);
  //system("rm /mnt/Nand1/Dcscreen.log");
  printf("Start Watchdog\n");
  while(watchdogenable){
   // if(loop == 0){
        //printf("monitor\n");

      //GPIO_SYS_RUN_MONITOR;

      //LOG("watchdog time");
      //printf("thread_interface_frame is %d\n",thread_interface_frame);
      //printf("thread_dsp_frame is %d\n",thread_dsp);

    //}

    if(loop >= 30){//不能太小，测试5s时会死掉，测试10s十次未重启，设置20s留有足够的裕值
      loop = 0;
      if((thread_dsp==1)&&(thread_m4_send==1)&&(thread_m4_receive==1)&&(thread_pc==1)&&(thread_interface_frame==1)\
         &&(thread_switch==1)&&(thread_battery==1)&&(thread_failure==1)&&(thread_tcp==1)&&(thread_usb==1)){
          //printf("thread ok\n");
          thread_dsp=0;
          thread_m4_send=0;
          thread_m4_receive=0;
          thread_pc=0;
          thread_switch=0;
          thread_battery=0;
          thread_failure=0;
          thread_tcp=0;
          thread_usb=0;
          thread_interface_frame=0;
          //LOG("safe");
          //system("date >>/mnt/Nand1/DCscreen.log;echo 'thread safe'>>/mnt/Nand1/DCscreen.log");
        }else{
          //printf("some thread die\n");
          if((thread_dsp==0))
          {
              LOG("thread_dsp die,   reboot");
          }
          if((thread_m4_send==0))
          {
              LOG("thread_m4_send die,   reboot");
          }
          if((thread_m4_receive==0))
          {
              LOG("thread_m4_receive die,   reboot");
          }
          if((thread_pc==0))
          {
              LOG("thread_pc die,   reboot");
          }
          if((thread_switch==0))
          {
              LOG("thread_switch die,   reboot");
          }
          if((thread_battery==0))
          {
              LOG("thread_battery die,   reboot");
          }
          if((thread_failure==0))
          {
              LOG("thread_failure die,   reboot");
          }
          if((thread_tcp==0))
          {
              LOG("thread_tcp die,   reboot");
          }
          if((thread_usb==0))
          {
              LOG("thread_usb die,   reboot");
          }
          if((thread_interface_frame==0))
          {
              LOG("thread_interface_frame die,   reboot");
          }
          //system("date >>/mnt/Nand1/DCscreen.log;echo 'thread die'>>/mnt/Nand1/DCscreen.log");
          system("reboot");



        }
    }
    loop ++;
    usleep(800000);
   }
  while(!watchdogenable){
      sleep(5);
    }


}

