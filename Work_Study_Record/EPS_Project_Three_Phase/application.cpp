#include "application.h"
#include <fcntl.h>
#include <stdio.h>
#include  <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <QtSql>
#include <QTimer>
#include <math.h>
#include <signal.h>
#include <execinfo.h>
#include <QtGui>

Application::Application(int &argc, char **argv):
  QApplication(argc,argv,Application::GuiServer)
{

#ifdef __QDEBUG
  qDebug()<<"__________---------_-------";
#endif

}


bool Application::notify(QObject *obj, QEvent  *e)
{
  if(e->type() == QEvent::MouseButtonPress)
    {
      screen_brightness_count = 0;
     // Silent_Mode_flag = 1;
      if(screensaverflag == false)
        {
          BK_CON_STRUCT  msg;
          msg.msg_type = 1;
          msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
          msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
          screensaverflag = true;
        }
      GPIO_SYS_BUZZ_OFF;
       if((obj->metaObject()->className() == QLineEdit::staticMetaObject.className())\
          ||(obj->metaObject()->className() == QPushButton::staticMetaObject.className())\
          ||(obj->metaObject()->className() == QTabBar::staticMetaObject.className())){
       GPIO_SYS_BUZZ_ON;//干接点控制接口函数

       usleep(30000);
       GPIO_SYS_BUZZ_OFF;

     }
    }
  else if(e->type() == QEvent::KeyPress)
    {
      screen_brightness_count = 0;
    //  Silent_Mode_flag = 1;
      if(screensaverflag == false)
        {
          BK_CON_STRUCT  msg;
          msg.msg_type = 1;
          msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
          msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
          screensaverflag = true;
        }

    }



  return QApplication::notify(obj,e);

}

