#ifndef ATS_SWITCH_SEND_RECEIVE_DATA_H
#define ATS_SWITCH_SEND_RECEIVE_DATA_H

#include <QThread>
#include "Share/Globalvar.h"

class Ats_Switch_Send_Receive_data : public QThread
{
  Q_OBJECT
public:
  explicit Ats_Switch_Send_Receive_data(QWidget *parent = 0);
  void run();
  void Ats_Switch_Send_Receive();

};

#endif // ATS_SWITCH_SEND_RECEIVE_DATA_H
