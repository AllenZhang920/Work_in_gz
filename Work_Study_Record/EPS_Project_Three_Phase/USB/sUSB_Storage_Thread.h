#ifndef SUSB_STORAGE_THREAD_H
#define SUSB_STORAGE_THREAD_H
#include<QThread>
#include <QWidget>
#include"Share/Globalvar.h"

class sUSB_Storage_Thread_thread:public QThread
{
    Q_OBJECT
public:
    explicit sUSB_Storage_Thread_thread(QWidget *parent = 0);
    void run();
private:
   int init_hotplug_sock(void);
   int getDevPath(const char *buf,char *path,int size);
   void sUSB_Storage_Task(void);

};

#endif // SUSB_STORAGE_THREAD_H
