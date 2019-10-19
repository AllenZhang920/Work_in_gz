#ifndef ARM_DSP_SEND_RECEIVE_DATA_THREAD_H
#define ARM_DSP_SEND_RECEIVE_DATA_THREAD_H

#include <QThread>
#include "Share/Globalvar.h"

class ARM_DSP_Send_Receive_Data_Thread : public QThread
{
  Q_OBJECT
public:
  explicit ARM_DSP_Send_Receive_Data_Thread(QWidget *parent = 0);
  void run();
  void ARM_DSP_Send_Receive_Data();
  void DSP_DATA_Clean();
  void DSP_ANALOG_STATUS_DATA();
  void DSP_NOTICE_ALARM_DATA();
  void DSP_DATA_Stone(UINT8 addr, UINT8 function, UINT8 *pdata);
private:
  UINT32 *pSend_Dsp_Debug_Param_Set;
  //Globa.Send_Dsp_Debug_Param_Set *pSend_Dsp_Debug_Param_Set;

};

#endif // ARM_DSP_SEND_RECEIVE_DATA_THREAD_H
