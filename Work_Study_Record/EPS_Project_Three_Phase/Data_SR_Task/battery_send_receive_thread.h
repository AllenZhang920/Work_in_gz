#ifndef BATTERY_SEND_RECEIVE_THREAD_H
#define BATTERY_SEND_RECEIVE_THREAD_H

#include <QThread>
#include"Share/Globalvar.h"

class Battery_Send_Receive_Thread : public QThread
{
  Q_OBJECT
public:
  explicit Battery_Send_Receive_Thread(QWidget *parent = 0);
  void run();
  void Battery_Send_Receive_Data();
  void Battery_Stone(UINT8 adress, UINT8 function, UINT8 *pdata);
  void battery_list_clean(UINT32 adress);
};

#endif // BATTERY_SEND_RECEIVE_THREAD_H
