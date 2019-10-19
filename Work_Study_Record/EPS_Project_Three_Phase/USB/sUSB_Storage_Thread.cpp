#include"sUSB_Storage_Thread.h"
#include <QTimer>
#include<QDebug>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>


UINT32 USB_selection=0;

char devPath[30]={0};

sUSB_Storage_Thread_thread::sUSB_Storage_Thread_thread(QWidget *parent) :
  QThread(parent)
{

  this->setStackSize(1024*300);

}
int sUSB_Storage_Thread_thread::init_hotplug_sock(void)
{
  struct sockaddr_nl snl;
  const int buffersize = 16  * 1024;
  int retval;
  memset(&snl, 0x00, sizeof(struct sockaddr_nl));
  snl.nl_family = AF_NETLINK;
  snl.nl_pid = getpid();
  snl.nl_groups = 1;
  int hotplug_sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
  if (hotplug_sock == -1)
    {
      printf("error getting socket: %s\n", strerror(errno));
      return -1;
    }
  /* set receive buffersize */
  setsockopt(hotplug_sock, SOL_SOCKET, SO_RCVBUFFORCE, &buffersize, sizeof(buffersize));
  retval = bind(hotplug_sock, (struct sockaddr *) &snl, sizeof(struct sockaddr_nl));
  if (retval < 0) {
      printf("bind failed: %s\n", strerror(errno));
      close(hotplug_sock);
      hotplug_sock = -1;
      return -1;
    }
  return hotplug_sock;
}

int sUSB_Storage_Thread_thread::getDevPath(const char *buf,char *path,int size)
{
  int i,j;
  char tmp[size];
  for(i = 'a';i <= 'z';i++)
    {
      for(j = 1;j <= 8;j++)
        {
          sprintf(tmp,"/sd%c%d",i,j);
          if(strstr(buf,tmp))
            {
              strcpy(path,"/media");
              strcat(path,tmp);
              printf("getDevPath:%s\n",path);
              return 0;
            }
        }
    }
  path[0]=0;
  return -1;
}

void  sUSB_Storage_Thread_thread::sUSB_Storage_Task(void)
{
  char buf[1024]={0};
  fd_set fdsr; //文件描述符集的定义
  int hotplug_sock=0;
  int sum,size;
  char *str;
  struct timeval tv;
  int usb = 0;

  hotplug_sock = init_hotplug_sock();
  while(1){
      thread_usb=1;
      FD_ZERO(&fdsr); //每次进入循环都重建描述符集
      FD_SET(hotplug_sock, &fdsr);
      tv.tv_sec = 1;
      tv.tv_usec = 0;
      int ret = select(hotplug_sock + 1, &fdsr, NULL, NULL, &tv);
      if(ret > 0)
        {//=0表示超时，下一轮循环
          sum = 0;
          size=recv(hotplug_sock,buf,sizeof(buf),0);
            printf("the hotplugbuf is %s\n",buf);
          while(sum<size)
            {
              str=buf+sum;
              sum+=strlen(str);
              buf[sum]='\n';
            }
          buf[sum]='\0';
          if(strstr(buf,"ACTION=add"))
            {
              if(getDevPath(buf,devPath,30)==0)
                {
                  usb = 1 ;
                  USB_selection = 1;

                }
            }
          if(strstr(buf,"ACTION=remove"))
            {
              if(getDevPath(buf,devPath,30)==0)
                {
                  usb = 0 ;
                  USB_selection = 0;
                }
            }
        }
      else
        { //<0表示探测失败或者=0无数据
          if(usb == 0)
            {
              USB_selection = 0;
            }
          else
            {
              USB_selection = 1;
            }
          sleep(3);
        }
    }

}//while


void sUSB_Storage_Thread_thread::run()
{

  sUSB_Storage_Task();
  exec();
}


