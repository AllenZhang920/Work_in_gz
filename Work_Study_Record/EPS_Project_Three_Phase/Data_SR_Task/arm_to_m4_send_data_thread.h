#ifndef ARM_TO_M4_SEND_DATA_THREAD_H
#define ARM_TO_M4_SEND_DATA_THREAD_H

#include <QThread>
#include"Share/Globalvar.h"

class Arm_To_M4_Send_Data_Thread : public QThread
{
  Q_OBJECT
public:
  explicit Arm_To_M4_Send_Data_Thread(QWidget *parent = 0);
  void run();
  void Arm_To_M4_Send_Data();
  
};

#endif // ARM_TO_M4_SEND_DATA_THREAD_H
