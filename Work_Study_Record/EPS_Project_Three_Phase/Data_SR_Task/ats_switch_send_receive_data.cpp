#include "ats_switch_send_receive_data.h"

Ats_Switch_Send_Receive_data::Ats_Switch_Send_Receive_data(QWidget *parent) :
  QThread(parent)
{


}
void Ats_Switch_Send_Receive_data::run()
{

  Ats_Switch_Send_Receive();
  exec();

}

void Ats_Switch_Send_Receive_data::Ats_Switch_Send_Receive()
{
  UINT32 loop = 0;

  UINT32 IO_comm_fault_num[2] = {0}; // IO通讯故障 1---故障
  UINT32 IO_sent_flag[2] = {0};
  int ionum=0;
  UINT32 Ats_comm_fault_num = 0; // ATS通讯故障 1---故障
  UINT32 Ats_sent_flag = 0;
  int ret,i=0;
  UART_M4_MSG msg;
  UINT16 wCRCCheckValue;
  /****************      << IO 模块 >>  功能码0x03数据收发区   **********************/
  while(1){
    thread_switch=1;
    if(Globa.Curstomer_Param_Set.ATS_Configure_Flage==1){
      if(loop % 20 == 0){//每个模块之间间隔 1S再次发送查询, 最快可调整为1S
        if(Ats_sent_flag == 1){//前一个模块序号 为 charge_number 的没有响应
          if(Ats_comm_fault_num >= MAX_LOST_COMMUNIT_NUM){//5次没有响应
              if(Globa.ATS_Module_Fault  == 0){
                  for(i = 0;i<8;i++)
                   {

                      Globa.ATS_Value[i] = 0;
                    }
                Globa.ATS_Module_Fault = 1;
              }
          }else{
            Ats_comm_fault_num++;
          }
        }

        msg.argv[0] = ATSSTARTADDR; //IO_number + SWMODSTARTADDR;   //addr
        msg.argv[1]  = 0x03;  //功能码
        msg.argv[2] = 0x00;  //起始地址高字节
        msg.argv[3] = 0x00;  //起始地址低字节，起始地址0
        msg.argv[4] = 0x00;  //数据个数高字节
        msg.argv[5] = 8; //数据个数低字节

        wCRCCheckValue = ModbusCRC(&msg.argv[0], 6);  //Modbus 协议crc校验

        msg.argv[6] = wCRCCheckValue >> 8;
        msg.argv[7] = wCRCCheckValue & 0x00ff;

        msg.type = 5; //m4的串口通道号
        msgsnd(Globa.uart_m4_msg_sent_id, &msg, 8, IPC_NOWAIT); //（消息队列发送消息时候）包括了通道号，数据
        usleep(20000);
        Ats_sent_flag = 1;
      }

      if(Ats_sent_flag == 1){//有发送命令，等待响应
        ret = msgrcv(Globa.uart_m4_msg_recv_id, &msg, sizeof(UART_M4_MSG)-sizeof(long), 5, IPC_NOWAIT);//非阻塞等待
        if(ret>0){
          if(ATSSTARTADDR== msg.argv[0]){
            for(i = 0;i<8;i++)
             {
                Globa.ATS_Value[i] = msg.argv[3 + 2*i]<<8|msg.argv[3 + 2*i+1];
              }
            Ats_comm_fault_num = 0;
            Ats_sent_flag = 0;
            Globa.ATS_Module_Fault=0;
          }
        }
      }
    }
 if(Globa.Curstomer_Param_Set.Distribution_Configure_Flage==1){
    /****************      << IO 模块 >>  功能码0x03数据收发区   **********************/
    if(loop % 20 == 0)
    {
      //每个模块之间间隔 1S再次发送查询, 最快可调整为1S
     for(ionum=0;ionum<Globa.Curstomer_Param_Set.Distribution_Branches_modelnum;ionum++)
     {   thread_switch=1;
         if(IO_sent_flag[ionum] == 1){//前一个模块序号 为 charge_number 的没有响应
             if(IO_comm_fault_num[ionum] >= MAX_LOST_COMMUNIT_NUM){//5次没有响应
                 if(Globa.Switch_Module_Fault[ionum]  == 0){
                for(i = 4*ionum;i<4*ionum+4;i++){
                  Globa.Switch_Value[i] = 0;
                }
                Globa.Switch_Module_Fault[ionum] = 1;
              }
            }else{
              IO_comm_fault_num[ionum]++;
            }
          }

          msg.argv[0] = IOSTARTADDR+ionum; //IO_number + SWMODSTARTADDR;   //addr
          msg.argv[1]  = 0x02;  //功能码
          msg.argv[2] = 0x00;  //起始地址高字节
          msg.argv[3] = 0x00;  //起始地址低字节，起始地址0
          msg.argv[4] = 0x00;  //数据个数高字节
          msg.argv[5] = 32; //数据个数低字节

          wCRCCheckValue = ModbusCRC(&msg.argv[0], 6);  //Modbus 协议crc校验

          msg.argv[6] = wCRCCheckValue >> 8;
          msg.argv[7] = wCRCCheckValue & 0x00ff;

          msg.type = 3+ionum; //m4的串口通道号
          msgsnd(Globa.uart_m4_msg_sent_id, &msg, 8, IPC_NOWAIT); //（消息队列发送消息时候）包括了通道号，数据
          usleep(20000);
          IO_sent_flag[ionum] = 1;
     }

  }
    for(ionum=0;ionum<Globa.Curstomer_Param_Set.Distribution_Branches_modelnum;ionum++)
    {
        if(IO_sent_flag[ionum] == 1)
        {//有发送命令，等待响应


          ret = msgrcv(Globa.uart_m4_msg_recv_id, &msg, sizeof(UART_M4_MSG)-sizeof(long), 3+ionum, IPC_NOWAIT);//非阻塞等待
          if(ret>0){


            if(IOSTARTADDR+ionum == msg.argv[0]){
              for(i = 0;i<4;i++){
                Globa.Switch_Value[i+4*ionum] = msg.argv[3 + i];
              }
              Globa.Switch_Module_Fault[ionum] = 0;
              IO_comm_fault_num[ionum] = 0;
              IO_sent_flag[ionum] = 0;
            }
          }
        }

    }



}

    loop++;//循环计数值， 作为数据抄收和处理的时间基准
    if(loop >20){//计数到5S后重新计数
      loop = 1;
    }
    usleep(50000); //50ms 循环一次，次为时间基准
 /*   modbus_pc02_mutex.lockForWrite();
    Globa.modbus_485_PC02[106]=Globa.Switch_Modlue_Comfalge;
    modbus_pc02_mutex.unlock();*/
  }
}
