#include "battery_send_receive_thread.h"
#include"Share/param.h"
Battery_Send_Receive_Thread::Battery_Send_Receive_Thread(QWidget *parent) :
  QThread(parent)
{


}

void Battery_Send_Receive_Thread::run()
{

  Battery_Send_Receive_Data();
  exec();

}


void Battery_Send_Receive_Thread::Battery_Stone(UINT8 adress, UINT8 function, UINT8 *pdata)
{
  UINT32 i = 0;
  UINT32 NO = (adress - BATTERYSTARTADDR)>=0? (adress - BATTERYSTARTADDR):0;
  if(function == 0x04){
    Battery_mutex.lockForWrite();
    for(i = 0; i< BATT_MOD_CHANEL_24_V + 1; i++){
      Globa.Eps_Param_Data.Battery_Data_Param[NO/BATTERYMAXNUMEBR].Battery_Monomer_V[NO%BATTERYMAXNUMEBR][i] = (*(pdata+2*i))<<8|(*(pdata+2*i+1)); // 一个电池巡检仪电池单体电压//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"
      //qDebug()<<"___-batt界面初始化111111"<<i<<NO/4<<NO<<Globa.Eps_Param_Data.Battery_Data_Param[NO/4].Battery_Monomer_V[NO%4][i];
    }
    Globa.Eps_Param_Data.Battery_Data_Refresh[NO/BATTERYMAXNUMEBR][NO%BATTERYMAXNUMEBR] = 1;//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"
   /* if(((32767>Globa.Eps_Param_Data.Battery_Tempature)&&(32767<(*(pdata+2*26))<<8|(*(pdata+2*26+1))))\
        ||((32767<Globa.Eps_Param_Data.Battery_Tempature)&&(32767>(*(pdata+2*26))<<8|(*(pdata+2*26+1)))))
      {
        Globa.Eps_Param_Data.Battery_Tempature=MIN(Globa.Eps_Param_Data.Battery_Tempature,(*(pdata+2*26))<<8|(*(pdata+2*26+1)));//t2
        printf("min\n");
      }else{
        Globa.Eps_Param_Data.Battery_Tempature=MAX(Globa.Eps_Param_Data.Battery_Tempature,(*(pdata+2*26))<<8|(*(pdata+2*26+1)));//t2
        printf("max\n");
      }*/
    Globa.Eps_Param_Data.Battery_Tempature[NO/BATTERYMAXNUMEBR][NO%BATTERYMAXNUMEBR]=(*(pdata+2*26))<<8|(*(pdata+2*26+1));//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"

    Battery_mutex.unlock();
    modbus_pc04_mutex.lockForWrite();
    for(i = 0; i< BATT_MOD_CHANEL_24_V + 1; i++){
      //Globa.Eps_Param_Data.Battery_Data_Param[NO/4].Battery_Monomer_V[NO%4][i] = (*(pdata+2*i))<<8|(*(pdata+2*i+1)); // 一个电池巡检仪电池单体电压
       // Globa.modbus_485_PC04[56+NO*24+i] = (*(pdata+2*i))<<8|(*(pdata+2*i+1)); // 一个电池巡检仪电池单体电压
    }
   modbus_pc04_mutex.unlock();
  }else if(function == 0x03){
    Globa.Debug_Param_BattSet[NO/BATTERYMAXNUMEBR][NO%BATTERYMAXNUMEBR].Batteryhuoerbianbi = (*(pdata + 6)) << 8 | (*(pdata + 7));              //霍尔电流传感器参数//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"
    Globa.Debug_Param_BattSet[NO/BATTERYMAXNUMEBR][NO%BATTERYMAXNUMEBR].Batterycurrentgaincalculation = (*(pdata + 8)) << 8 | (*(pdata + 9));//电池电流计算增益因子
    Globa.Debug_Param_BattSet[NO/BATTERYMAXNUMEBR][NO%BATTERYMAXNUMEBR].BatteryCapacitorchargingtime = (*(pdata + 16)) << 8 | (*(pdata + 17));   //电池电容充电时间
    Globa.Debug_Param_BattSet[NO/BATTERYMAXNUMEBR][NO%BATTERYMAXNUMEBR].BatteryVoltageadjustmentfactor1 = (*(pdata + 18)) << 8 | (*(pdata + 19)); //电池电压计算增益调整因子1-13
    Globa.Debug_Param_BattSet[NO/BATTERYMAXNUMEBR][NO%BATTERYMAXNUMEBR].BatteryVoltageadjustmentfactor2 = (*(pdata + 20)) << 8 | (*(pdata + 21)); //电池电压计算增益调整因子14-24//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"

    Globa.Eps_Param_Data.Battery_Mod_Issued_flags[NO/BATTERYMAXNUMEBR][NO%BATTERYMAXNUMEBR] = 1;//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"
  }
}


void Battery_Send_Receive_Thread::battery_list_clean(UINT32 adress)
{
  /*UINT32 i;
  if((adress >= 130)&&(adress <= 134)){//电池地址--即代表第一个电池模块
     Battery_mutex.lockForWrite();
    for(i = 0; i< BATT_MOD_CHANEL_24_V + 1; i++){
      Globa.Eps_Param_Data.Battery_Data_Param[0].Battery_Monomer_V[adress - BATTERYSTARTADDR ][i] = 0; // 电池单体电压
    }
    Globa.Eps_Param_Data.Battery_Data_Refresh[0][adress - BATTERYSTARTADDR] = 0;
    Battery_mutex.unlock();
  }*/

  UINT32 i;
  UINT32 NO = (adress - BATTERYSTARTADDR)>=0? (adress - BATTERYSTARTADDR):0;
  if((adress >= 130)&&(adress <= 145)){//电池地址--即代表第一个电池模块
     Battery_mutex.lockForWrite();
    for(i = 0; i< BATT_MOD_CHANEL_24_V + 1; i++){
      Globa.Eps_Param_Data.Battery_Data_Param[NO/BATTERYMAXNUMEBR].Battery_Monomer_V[NO%BATTERYMAXNUMEBR][i] = 0; // 电池单体电压//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"
    }
    Globa.Eps_Param_Data.Battery_Tempature[NO/BATTERYMAXNUMEBR][NO%BATTERYMAXNUMEBR]=battery_temp_display;
    Globa.Eps_Param_Data.Battery_Data_Refresh[NO/BATTERYMAXNUMEBR][NO%BATTERYMAXNUMEBR] = 0;//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"
    Battery_mutex.unlock();
  }
}

/************************************************************************
****  电池仪模块数据线程
************************************************************************/
void Battery_Send_Receive_Thread::Battery_Send_Receive_Data()
{
  UINT32 Batt_comm_fault_num[BATTERYMAXGROUP][BATTERYMAXNUMEBR] ={{0}};
  UINT32 Battery_number = 0,Group_No = 0;
  UINT32 loop = 0;
  INT32  ret = 0;

  UART_M4_MSG msg;
  UINT16 wCRCCheckValue = 0;

  while(1){
    thread_battery=1;

    for(Group_No = 0;(Group_No<Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable)&&(Group_No<BATTERYMAXGROUP);Group_No++){
      thread_battery=1;

        Battery_number = 0;
       for(Battery_number = 0;Battery_number<Globa.Battery_Param_Set[Group_No].Battery_Number;Battery_number++){
         if(Globa.Eps_Param_Data.Battery_Mod_Issued_flags[Group_No][Battery_number] == 0){
           msg.argv[fun_addr] = Battery_number + BATTERYSTARTADDR + BATTERYMAXNUMEBR*Group_No;   //addr  这个地址要加偏移量//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"
           msg.argv[fun_fun] = 0x03;  //功能码
           msg.argv[fun_regh] = 0x00;  //起始地址高字节
           msg.argv[fun_regl] = 0x00;  //起始地址低字节，起始地址0
           msg.argv[fun_lenh] = 0x00;  //总共位数的数据个数高位
           msg.argv[fun_lenl] = 12; //总共位数的数据个数低位

           wCRCCheckValue = ModbusCRC(&msg.argv[fun_addr], 6);  //Modbus 协议crc校验
           msg.argv[fun_crch] = wCRCCheckValue >> 8;
           msg.argv[fun_crcl] = wCRCCheckValue & 0x00ff;
           //发送消息给 串口5
           msg.type = 2; //m4的串口通道号
           msgsnd(Globa.uart_m4_msg_sent_id, &msg, 8, IPC_NOWAIT);
         }else{
           msg.argv[fun_addr] = Battery_number + BATTERYSTARTADDR + BATTERYMAXNUMEBR*Group_No;   //addr  这个地址要加偏移量//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"
           msg.argv[fun_fun] = 0x04;  //功能码
           msg.argv[fun_regh] = 0x00;  //起始地址高字节
           msg.argv[fun_regl] = 0x00;  //起始地址低字节，起始地址0
           msg.argv[fun_lenh] = 0x00;  //总共位数的数据个数高位
           msg.argv[fun_lenl] = BATT_MOD_RESERVER_2; //总共位数的数据个数低位

           wCRCCheckValue = ModbusCRC(&msg.argv[fun_addr], 6);  //Modbus 协议crc校验
           msg.argv[fun_crch] = wCRCCheckValue >> 8;
           msg.argv[fun_crcl] = wCRCCheckValue & 0x00ff;
           //发送消息给 串口5
           msg.type = 2; //m4的串口通道号
           msgsnd(Globa.uart_m4_msg_sent_id, &msg, 8, IPC_NOWAIT);
         }

        usleep(20000);
        for(loop = 0; loop < 50; loop++){
          ret = msgrcv(Globa.uart_m4_msg_recv_id, &msg, sizeof(UART_M4_MSG)-sizeof(long), 2, IPC_NOWAIT);//非阻塞等待
          if(ret>0){
            if((Battery_number + BATTERYSTARTADDR + BATTERYMAXNUMEBR*Group_No) == msg.argv[0]){//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"
               Battery_Stone(msg.argv[0], msg.argv[1], &msg.argv[3]); //  数据存放在链表中
               Globa.Eps_Param_Data.Battery_Communication_Fault[Group_No][Battery_number] = 0;
               Batt_comm_fault_num[Group_No][Battery_number] = 0;
               break;
            }
          }
          usleep(20000);
        }

        if(loop >= 50){
          if(Batt_comm_fault_num[Group_No][Battery_number] >= MAX_LOST_COMMUNIT_NUM){//5次没有响应 =MAX_LOST_COMMUNIT_NUM
            if(Globa.Eps_Param_Data.Battery_Communication_Fault[Group_No][Battery_number] == 0){
              battery_list_clean(Battery_number + BATTERYSTARTADDR + BATTERYMAXNUMEBR*Group_No); //  清除数据链表中的数据//change by zzc 721 "4"-->"BATTERYMAXNUMEBR"
              Globa.Eps_Param_Data.Battery_Communication_Fault[Group_No][Battery_number] = 1;
            }
          }else{
            Batt_comm_fault_num[Group_No][Battery_number]++;
          }
        }
      }
      usleep(300*1000);// usleep(100000);---原来
    }
    if(Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable==0){
        usleep(300000);
        Battery_mutex.lockForWrite();
        for(int i=0;i<BATTERYMAXGROUP;i++){
            for(int j=0;j<BATTERYMAXNUMEBR;j++){
                Globa.Eps_Param_Data.Battery_Tempature[i][j]=battery_temp_display;
              }
           }
        Battery_mutex.unlock();
      }
  }
}



