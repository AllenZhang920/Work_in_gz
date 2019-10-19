#ifndef MONITOR_THREAD_RUNNIG_THREAD_H
#define MONITOR_THREAD_RUNNIG_THREAD_H

#include <QThread>
#include"Share/Globalvar.h"
#include"Share/param.h"
class Monitor_Thread_Runnig_Thread : public QThread
{
  Q_OBJECT
public:
  explicit Monitor_Thread_Runnig_Thread(QObject *parent = 0);
  void run();
  UINT32 loop;
signals:
  
public slots:
  
};

#endif // MONITOR_THREAD_RUNNIG_THREAD_H
