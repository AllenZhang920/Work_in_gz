#ifndef ARM_TO_PC_SEND_RECEIVE_DATA_THREAD_H
#define ARM_TO_PC_SEND_RECEIVE_DATA_THREAD_H

#include <QThread>
#include"Share/param.h"

class Arm_To_Pc_Send_Receive_Data_Thread : public QThread
{
  Q_OBJECT
public:
  explicit Arm_To_Pc_Send_Receive_Data_Thread(QWidget *parent = 0);
  void  run();
  void  Arm_To_Pc_Send_Receive_Data();
  void  modbus_func04_ack( UINT16 start_reg_addr, UINT16 reg_num);
  void  modbus_func02_ack( UINT16 start_reg_addr, UINT16 reg_num);
  void  modbus_fault_ack(UINT8 err_function_code ,UINT8 err_Correct_code);
  void  modbus_fun06_ack(UINT16 start_reg_addr ,UINT16 Input_value);
  void  modbus_func03_ack( UINT16 start_reg_addr, UINT16 reg_num);
  void Modbus_Fault_Response(INT32 UART_COM, UINT8 *buffer, UINT8 err_num);

  UINT32 SendSetup(UINT8 adress ,UINT16 Register , UINT16 pdata,UINT8 id);
  INT32 ModbusSetup06(UINT16 regaddress, UINT8 *msg, UINT32 length);

};

#endif // ARM_TO_PC_SEND_RECEIVE_DATA_THREAD_H
