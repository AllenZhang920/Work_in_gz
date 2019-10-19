#include "arm_to_m4_send_data_thread.h"
#include"Share/param.h"
/****************************************************************************
**description：给串口M4发送数据线程
数据帧格式：|1|2|3|(数据开始_1)|...|数据结束_N|
说明：
ARM9请求帧格式：{
      1：第1字节的比特位代表的意义如下：
        高位 | bit7 bit6 bit5 | bit4 bit3 bit2 | bit1 bit0 |  低位
              --------------- | -------------- | --------- |
               串口标示           波特率          数据位
               0: com1;           0: 1200;        0: 8bit
               1: com2;           1: 2400;        1: 7bit
               2: com3;           2: 4800;        2: 6bit
               3: com4;           3: 9600;        3: 5bit
               4: com5;           4: 19200;
               5: com6;           5: 38400;
               6: com7;           6: 57600;
               7: NO;             7: 115200;

       2：第2字节的比特位代表的意义如下：
        高位 | bit7 bit6  | bit5 bit4 | bit3 bit2 | bit1 bit0 | 低位
             | ---------- | --------- | --------- | ---------
               校验位      停止位      流控          保留
               0: none     0: 1bit     0: none
               1: even     1: 1.5bit   1: Xon/Xoff
               2: odd      2: 2bit     2: 硬件
               3: NO       3: NO       3: NO


       3：第3字节保留：（具体使用含义有arm端决定，M4不改变该字节的值，原值返回）
}

M4响应帧帧格式：{
                1：前面3字节数据原值返回，后面数据为设备响应数据帧：
}

**********************************************************/
Arm_To_M4_Send_Data_Thread::Arm_To_M4_Send_Data_Thread(QWidget *parent) :
  QThread(parent)
{
}


void Arm_To_M4_Send_Data_Thread::run()
{
   Arm_To_M4_Send_Data();
   exec();

}

void Arm_To_M4_Send_Data_Thread::Arm_To_M4_Send_Data()
{
  INT32 Count=0;
  INT32 ret;

  UART_M4_MSG  msg;
  UINT8  buffer[MAX_MODBUS_FRAMESIZE + 2] = {0};

  while(1){
    thread_m4_send=1;
    usleep(5000);
    ret = msgrcv(Globa.uart_m4_msg_sent_id, &msg,sizeof(UART_M4_MSG)-sizeof(long), 0, 0);//消息为空时阻塞等待
    if(ret < 0){
      continue;
    }
    if((msg.type > 0)&&(msg.type <= 5)&&(ret <= MAX_MODBUS_FRAMESIZE)){
      switch(msg.type){
        case 1:
          buffer[2] = 1; // DSP模块

         // buffer[0] = ((buffer[2] - 1) << 5) | (4 << 2); //填写串口号和波特率标示
          buffer[0] = ((buffer[2] - 1) << 5) | (3 << 2); //填写串口号和波特率标示
          buffer[1] = 0;
          break;
        case 2:
          buffer[2] = 2; // batterymo模块
          buffer[0] = ((buffer[2] - 1) << 5) | (3 << 2); //填写串口号和波特率标示
          buffer[1] = 0;
          break;
        case 3:
          buffer[2] = 7; //switch
          buffer[0] = ((buffer[2] - 1) << 5) | (3 << 2); //填写串口号和波特率标示
          buffer[1] = 0;
          break;
        case 4:
          buffer[2] = 7; //switch
          buffer[0] = ((buffer[2] - 1) << 5) | (3 << 2); //填写串口号和波特率标示
          buffer[1] = 0;
          break;
        case 5:
          buffer[2] = 4; //ATS
          buffer[0] = ((buffer[2] - 1) << 5) | (3 << 2); //填写串口号和波特率标示
          buffer[1] = 2;
          break;
        default:
          break;
     }

      memcpy(&buffer[3], msg.argv, ret);
      Count = write_datas_tty(Fd_Uart_Arm_M4, buffer, ret+3); // 发送最大次数
    }
  }
}
