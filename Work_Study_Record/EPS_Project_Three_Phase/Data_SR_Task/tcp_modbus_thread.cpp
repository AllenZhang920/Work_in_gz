#include "tcp_modbus_thread.h"

Tcp_Modbus_Thread::Tcp_Modbus_Thread()
{


}
UINT16 ParameterRange[700]={0};


void Tcp_Modbus_Thread::run()
{
   TCP_Modbus_Server();
   exit();
}
/******************modbus 异常响应格式****************************************************/
void Tcp_Modbus_Thread::Modbus_Fault_Response(UINT32 fd, UINT8 *msg, UINT8 err_num)
{
  msg[EXMB_fun] = msg[EXMB_fun] + 0x80;
  msg[EXMB_state] = err_num;
  msg[MBAP_length + 1] = 0x03;
  send(fd, msg, msg[MBAP_length + 1] + MBAP_unit, 0);
}

UINT32 Tcp_Modbus_Thread::SendSetup(UINT8 adress ,UINT16 Register , UINT16 pdata,UINT8 id)
{
unsigned char Counter=0;
while(Counter<3)
{
  Counter++;
  if(1==Issuedequipment_parameters_06(adress ,Register ,pdata,id))
    {
    return 1;
  }
  else
  {
    usleep(20000);
  }
}

return 0;
}
INT32 Tcp_Modbus_Thread::ModbusSetup06(UINT16 regaddress, UINT8 *msg, UINT32 length)
{
 // UINT16 RealParameter=0;
   UINT16 Input_value=0;
   UINT16 reg_range_add=0;//对应输入范围序号
   Input_value=(msg[0]<<8)|(msg[1]);
   if(((regaddress <= reg_06_03_end_reg)&&(regaddress >= reg_06_03_start_reg))||((regaddress>=0)&&(regaddress<=CONFIG_CODE3)))
   {
     //确定开始地址所对应的参数范围

       if((regaddress != INVERTER_ON_COMMAND)&&(regaddress != INVERTER_OFF_COMMAND)&&(regaddress != CONFIG_INPUT_VOLTAGE_HIGH_LIMIT)\
          &&(regaddress != CONFIG_INPUT_VOLTAGE_LOW_LIMIT)&&(regaddress != CONFIG_INPUT_FREQUENCY_HIGH_LIMIT)&&(regaddress != CONFIG_INPUT_FREQUENCY_LOW_LIMIT)\
          &&(regaddress != CONFIG_BATTERY_AH)&&(regaddress != CONFIG_BATTERY_BLOCKS)&&(regaddress != CONFIG_BATTERY_EOD_VOLT)\
          &&(regaddress != CONFIG_BATTERY_STRINGS)&&(regaddress != CONFIG_INPUT_VOLTAGE_AD_PHASE_A)&&(regaddress != CONFIG_INVERTER_VOLTAGE_AD_PHASE_A)\
          &&(regaddress != CONFIG_OUTPUT_VOLTAGE_AD_PHASE_A)&&(regaddress != CONFIG_BATTERY_VOLTAGE_AD)&&(regaddress != CONFIG_POSITIVE_BUS_VOLTAGE_AD)\
          &&(regaddress != CONFIG_CODE0)&&(regaddress != CONFIG_CODE1)&&(regaddress != CONFIG_CODE2)&&(regaddress != CONFIG_CODE3)&&(regaddress != SETTING_CLEAR_FAULT))

       {

           return 2;

       }


       if((regaddress <= reg_06_03_end_reg)&&(regaddress >= reg_06_03_start_reg))
         {
           reg_range_add=regaddress-61440+300;
         }else{
           reg_range_add=regaddress;
         }

      if((Input_value<(ParameterRange[(reg_range_add)*2]))||
          (Input_value>(ParameterRange[(reg_range_add)*2+1])))
        {
           return 0x03;   //设置的参数范围超出了
       }
      //RealParameter=Globa.modbus_485_PC03[regaddress];
      // if(Input_value!=RealParameter){
       switch(regaddress){
         case CONFIG_INPUT_VOLTAGE_HIGH_LIMIT:{

                             if(SendSetup(DSPSTARTADDR, Mains_Voltage_Top_limit_Enum, Input_value, 1)){

                                Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Top_limit=Input_value;
                                Insert_Operation_Work_Record(1, "修改主电电压上限");
                               }else{
                                return 0x06;
                                }

                         break;
                        }

         case CONFIG_INPUT_VOLTAGE_LOW_LIMIT:{

                             if(SendSetup(DSPSTARTADDR, Mains_Voltage_Low_limit_Enum, Input_value, 1)){

                                Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Low_limit=Input_value;
                                System_Set_DSP_Parame_save();
                                Insert_Operation_Work_Record(1, "修改主电电压下限");
                               }else{
                                return 0x06;
                                }

                         break;
                        }


         case CONFIG_INPUT_FREQUENCY_HIGH_LIMIT:{

                             if(SendSetup(DSPSTARTADDR, Mains_Frequence_Top_limit_Enum, Input_value, 1)){
                                 Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Top_limit = Input_value;
                                 System_Set_DSP_Parame_save();
                                 Insert_Operation_Work_Record(1, "修改主电频率上限");
                               }else{
                                return 0x06;
                                }

                         break;
                        }

         case CONFIG_INPUT_FREQUENCY_LOW_LIMIT:{

                             if(SendSetup(DSPSTARTADDR, Mains_Frequence_Low_limit_Enum, Input_value, 1)){
                                 Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Low_limit = Input_value;
                                 System_Set_DSP_Parame_save();
                               //  msgBox.exec();
                                 Insert_Operation_Work_Record(1, "修改主电频率下限");
                               }else{
                                return 0x06;
                                }

                         break;
                        }
         case CONFIG_BATTERY_AH:{

                             if(SendSetup(DSPSTARTADDR, Battery_Capacity_Enum, Input_value, 1)){
                                 Globa.Send_Dsp_Debug_Param_Set.Battery_Capacity = Input_value;// System_Battery_GroupParam_Set_save();
                                 System_Set_DSP_Parame_save();
                                 Insert_Operation_Work_Record(1, "修改电池容量");
                               }else{
                                return 0x06;
                                }

                         break;
                        }
         case CONFIG_BATTERY_BLOCKS:{

                             if(SendSetup(DSPSTARTADDR, Battery_All_Section_Number_Enum, Input_value, 1)){
                                 Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number = Input_value;
                                 System_Set_DSP_Parame_save();
                                 Insert_Operation_Work_Record(1, "修改电池节数");
                               }else{
                                return 0x06;
                                }

                         break;
                        }
         case CONFIG_BATTERY_STRINGS:{

                             if(SendSetup(DSPSTARTADDR, Battery_Group_Enum, Input_value, 1)){
                                 Globa.Eps_Param_Data.Current_Alarm_List_Battery_Refresh=1;
                                 Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number = Input_value;
                                 if(Globa.Battery_GroupParam_Set.battery_detecting_disable==0){
                                   Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable=Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number;
                                   }else{
                                     Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable=0;
                                     Globa.Eps_Param_Data.Current_Alarm_List_Battery_Refresh=1;
                                   }
                                 System_Set_DSP_Parame_save();
                                 Insert_Operation_Work_Record(1, "远程修改电池组数");
                               }else{
                                return 0x06;
                                }

                         break;
                        }


         case CONFIG_BATTERY_EOD_VOLT:{

                             if(SendSetup(DSPSTARTADDR, Battery_Discharge_Termination_V_Enum, Input_value, 1)){
                                 Globa.Send_Dsp_Debug_Param_Set.Battery_Discharge_Termination_V = Input_value;

                                System_Set_DSP_Parame_save();

                                 Insert_Operation_Work_Record(1, "修改单节电池放电终止电压");
                               }else{
                                return 0x06;
                                }

                         break;
                        }

         case CONFIG_INPUT_VOLTAGE_AD_PHASE_A:{//主电电压校准系数

                             if(SendSetup(DSPSTARTADDR, Mains_Voltage_Calibration_Coefficient_Enum, Input_value, 1)){
                                 Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Calibration_Coefficient = Input_value;
                                 System_Set_DSP_Parame_save();
                                 Insert_Operation_Work_Record(1, "修改主电电压校准系数");
                               }else{
                                return 0x06;
                                }

                         break;
                        }

         case CONFIG_INVERTER_VOLTAGE_AD_PHASE_A:{//应急电压校准系数

                        if(SendSetup(DSPSTARTADDR, Emergency_Voltage_Calibration_Coefficient_Enum, Input_value, 1)){
                                 Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_Calibration_Coefficient = Input_value;
                                 System_Set_DSP_Parame_save();
                                 Insert_Operation_Work_Record(1, "修改应急电压校准系数");
                        }else{
                              return 0x06;
                         }

                         break;
                      }
         case CONFIG_OUTPUT_VOLTAGE_AD_PHASE_A:{//输出电压校准系数

                        if(SendSetup(DSPSTARTADDR, Ouput_Voltage_Calibration_Coefficient_Enum, Input_value, 1)){
                            Globa.Send_Dsp_Debug_Param_Set.Ouput_Voltage_Calibration_Coefficient = Input_value;
                            System_Set_DSP_Parame_save();
                            Insert_Operation_Work_Record(1, "修改输出电压校准系数");
                        }else{
                              return 0x06;
                         }

                         break;
                      }
         case CONFIG_BATTERY_VOLTAGE_AD:{//电池电压校准系数

                        if(SendSetup(DSPSTARTADDR, Battery_Voltage_Calibration_Coefficient_Enum, Input_value, 1)){
                            Globa.Send_Dsp_Debug_Param_Set.Battery_Voltage_Calibration_Coefficient = Input_value;
                            System_Set_DSP_Parame_save();
                            Insert_Operation_Work_Record(1, "修改电池电压校准系数");
                        }else{
                              return 0x06;
                         }

                         break;
                      }
         case CONFIG_POSITIVE_BUS_VOLTAGE_AD:{//母线电压校准系数

                        if(SendSetup(DSPSTARTADDR, Bus_Voltage_Calibration_Coefficient_Enum, Input_value, 1)){
                            Globa.Send_Dsp_Debug_Param_Set.Bus_Voltage_Calibration_Coefficient = Input_value;
                            System_Set_DSP_Parame_save();
                            Insert_Operation_Work_Record(1, "修改母线电压校准系数");
                        }else{
                              return 0x06;
                         }

                         break;
                      }

       case CONFIG_CODE0:{

                           if(SendSetup(DSPSTARTADDR, CODE_0_Enum, Input_value, 1)){
                               Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0]= Input_value;
                               SETDATA_Machine_Code[0]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0];
                               System_Set_DSP_Parame_save();
                               Insert_Operation_Work_Record(1, "远程设置机身编码1");
                             }else{
                              return 0x06;
                              }

                       break;
                      }
       case CONFIG_CODE1:{

                           if(SendSetup(DSPSTARTADDR, CODE_1_Enum, Input_value, 1)){
                               Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1]= Input_value;
                               SETDATA_Machine_Code[1]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1];
                               System_Set_DSP_Parame_save();
                               Insert_Operation_Work_Record(1, "远程设置机身编码2");
                             }else{
                              return 0x06;
                              }

                       break;
                      }

       case CONFIG_CODE2:{

                           if(SendSetup(DSPSTARTADDR, CODE_2_Enum, Input_value, 1)){
                               Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2]= Input_value;
                               SETDATA_Machine_Code[2]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2];
                               System_Set_DSP_Parame_save();
                               Insert_Operation_Work_Record(1, "远程设置机身编码3");
                             }else{
                              return 0x06;
                              }

                       break;
                      }

       case CONFIG_CODE3:{

                           if(SendSetup(DSPSTARTADDR, CODE_3_Enum, Input_value, 1)){
                               Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3]= Input_value;
                               SETDATA_Machine_Code[3]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3];
                               System_Set_DSP_Parame_save();
                               Insert_Operation_Work_Record(1, "远程设置机身编码4");
                             }else{
                              return 0x06;
                              }

                       break;
                      }





          /* if((regaddress != 61440)&&(regaddress != 61441)&&(regaddress != CONFIG_INPUT_VOLTAGE_HIGH_LIMIT)\
              &&(regaddress != CONFIG_INPUT_VOLTAGE_LOW_LIMIT)&&(regaddress != CONFIG_INPUT_FREQUENCY_HIGH_LIMIT)&&(regaddress != CONFIG_INPUT_FREQUENCY_LOW_LIMIT)\
              &&(regaddress != CONFIG_BATTERY_AH)&&(regaddress != CONFIG_BATTERY_BLOCKS)&&(regaddress != CONFIG_BATTERY_EOD_VOLT)\
              )

           */

                case INVERTER_ON_COMMAND:{
                                //modbus_pc03_mutex.lockForWrite();
                                if(Input_value==1){
                                    if(SendSetup(DSPSTARTADDR, Manual_Open_Shut_Down_Enum, 0xFF, 1)){
                                       //Shutdown_Flag_flag=0;
                                       /*modbus_pc03_mutex.lockForWrite();
                                       Globa.modbus_485_PC03[INVERTER_ON_COMMAND]=1;
                                       Globa.modbus_485_PC03[INVERTER_OFF_COMMAND]=0;
                                       modbus_pc03_mutex.unlock();*/
                                       Globa.Send_Dsp_Debug_Param_Set.Manual_Open_Shut_Down=0xFF;
                                       System_Set_DSP_Parame_save();
                                       Insert_Operation_Work_Record(1, "远程手动开机");

                                      }else{
                                       Insert_Operation_Work_Record(1, "远程手动开机失败");
                                       return 0x06;
                                       }
                                }else{

                                    /*modbus_pc03_mutex.lockForWrite();
                                    Globa.modbus_485_PC03[INVERTER_ON_COMMAND]=0;
                                    modbus_pc03_mutex.unlock();*/
                                       return 0;
                                  }
                        //printf("in 61440\n \n");
                                //modbus_pc03_mutex.unlock();
                                break;
                            }
                case INVERTER_OFF_COMMAND:{

                               if(Input_value==1){
                                  if(SendSetup(DSPSTARTADDR,Manual_Open_Shut_Down_Enum, 0x0F, 1)){
                                  /*modbus_pc03_mutex.lockForWrite();
                                  Globa.modbus_485_PC03[INVERTER_OFF_COMMAND]=1;
                                  Globa.modbus_485_PC03[INVERTER_ON_COMMAND]=0;
                                  modbus_pc03_mutex.unlock();*/
                                  Globa.Send_Dsp_Debug_Param_Set.Manual_Open_Shut_Down=0x0F;
                                  System_Set_DSP_Parame_save();
                                  Insert_Operation_Work_Record(1, "远程手动关机");

                             //printf("in 61441\n \n");
                                }else
                               {
                                 Insert_Operation_Work_Record(1, "远程手动关机失败");
                                 return 0x06;
                               }
                              }else{
                                /*modbus_pc03_mutex.lockForWrite();
                                Globa.modbus_485_PC03[INVERTER_OFF_COMMAND]=0;
                                modbus_pc03_mutex.unlock();*/
                                return 0;
                                 }

                             break;
                        }
                  case SETTING_CLEAR_FAULT:
                   {

                        if(Input_value==1){
                           if(SendSetup(DSPSTARTADDR, Clear_Fault_Enum, 0xff, 1))
                             {

                                Insert_Operation_Work_Record(1, "远程复位");

                              }else{
                              Insert_Operation_Work_Record(1, "远程复位失败");
                              return 0x06;
                              }
                          }else{


                              return 0;
                         }

                      break;
                   }


           }
     //}
    //System_ManualControl_save();
    return 1;

 }
 else
{

      return 02;
 }
  return 1;
}

/*void Tcp_Modbus_Thread::modbus_fun06_ack(UINT16 start_reg_addr ,UINT16 Input_value)
{
  //UINT32 i = 0; i = i;
  //Input_value = Input_value;
 // switch(start_reg_addr){
  //}

    switch(start_reg_addr){
      case INVERTER_ON_COMMAND:{
                    modbus_pc03_mutex.lockForWrite();
                    if(Input_value==1){
                        if(Issuedequipment_parameters_06(DSPSTARTADDR, Manual_Open_Shut_Down_Enum, 0xFF, 1)){
                           Shutdown_Flag_flag=0;
                           Globa.modbus_485_PC03[INVERTER_ON_COMMAND-reg_06_03_start_reg]=1;
                           Globa.modbus_485_PC03[INVERTER_OFF_COMMAND-reg_06_03_start_reg]=0;
                           Insert_Operation_Work_Record(1, "远程手动开机");

                          }else{
                           Insert_Operation_Work_Record(1, "远程手动开机失败");

                           }
                    }else{
                         Globa.modbus_485_PC03[INVERTER_ON_COMMAND-reg_06_03_start_reg]=0;

                      }
            //printf("in 61440\n \n");
                    modbus_pc03_mutex.unlock();
                    break;
                }
      case INVERTER_OFF_COMMAND:{
                   modbus_pc03_mutex.lockForWrite();
                   if(Input_value==1){
                      if(Issuedequipment_parameters_06(DSPSTARTADDR,Manual_Open_Shut_Down_Enum, 0x0F, 1)){
                      Globa.modbus_485_PC03[INVERTER_OFF_COMMAND-reg_06_03_start_reg]=1;
                      Globa.modbus_485_PC03[INVERTER_ON_COMMAND-reg_06_03_start_reg]=0;
                      Insert_Operation_Work_Record(1, "远程手动关机");

                 //printf("in 61441\n \n");
                    }else
                   {
                     Insert_Operation_Work_Record(1, "远程手动关机失败");

                   }
                  }else{
                    Globa.modbus_485_PC03[INVERTER_OFF_COMMAND-reg_06_03_start_reg]=0;

                     }
                   modbus_pc03_mutex.unlock();
                 break;
            }
  }

}
*/

INT32 Tcp_Modbus_Thread::Battery_Tcp_modbus_Deal(UINT32 fd, UINT8 *msg, UINT32 length)
{
  UINT32 i,j;
  UINT16 start_reg,reg_num;
  UINT16 cur_reg_addr;
  UINT32 index = 0,bytes_cnt=0;
  UINT32 registers_length = 0;
  UINT16 Buf[MAX_MODBUS_FRAMESIZE] = {0};   //响应上位机的MODBUS数据发送缓冲区


  if((length < MBAP_fun)||(length >= BUF_SIZE)){//length <7 or >BUF_SIZE
    Modbus_Fault_Response(fd, msg, COMMS_FAILURE);
    return(-1);
  }else if(!((msg[MBAP_proto] == 0)&&(msg[MBAP_proto + 1] ==0))){//协议标识符不正确
    Modbus_Fault_Response(fd, msg, COMMS_FAILURE);
    return(-1);
  }else if(!((msg[MBAP_unit] >= 1)&&(msg[MBAP_unit] < 255))){//地址无效
    Modbus_Fault_Response(fd, msg, COMMS_FAILURE);
    return (-1);
  }else if(!((msg[MBAP_length] == 0)&&(msg[MBAP_length+1] >= 1)&& \
          (msg[MBAP_length+1] <=(INT32)(BUF_SIZE-MBAP_unit))&&(msg[MBAP_length+1] + MBAP_unit == (INT32)length))){//长度不正确
    Modbus_Fault_Response(fd, msg, COMMS_FAILURE);
    return (-1);
  }

  //*******符号条件的数据**********
  if(msg[MBAP_unit + 1] == 0x02){
    if(length == CMB02_regnumL + 1){//帧长度正确
      start_reg = (msg[CMB02_addrH]<<8) + msg[CMB02_addrL];
      reg_num = (msg[CMB02_regnumH]<<8) + msg[CMB02_regnumL];
      if(start_reg <= reg_02input_nregs - 1){//地址正确
        if(reg_num <= (reg_02input_nregs - start_reg)){//读取的数据范围正确)
          msg[RMB02_regnum] = (msg[CMB02_regnumL] + 7)/8;//寄存器个数
          msg[MBAP_length+1] =  RMB02_regdata + msg[RMB02_regnum] - MBAP_unit; //长度

          if(reg_num <= 8){//离散量数不超过1个字节的8个bit,1个字节即可表示
            bytes_cnt = 1;//1个字节
          }else{
            bytes_cnt = (reg_num%8 == 0) ? (reg_num >> 3): ((reg_num >> 3) + 1);
          }

          modbus_pc02_mutex.lockForRead();
          index = RMB02_regdata ;
          for(j=0; j < bytes_cnt; j++){
            msg[index] = 0 ;
            for(i=0; i < 8; i++){
              registers_length ++;
              msg[index] |= ((Globa.modbus_485_PC02[start_reg + 8*j+i] & 0x01)<< i);
              if(registers_length == reg_num){
                break;
              }
            }
            index++;
          }
          modbus_pc02_mutex.unlock();
#if DEBUG_MODBUS_TCP
          printf("sent ength = %d data =", msg[MBAP_length+1] + MBAP_unit);
          for(j=0; j<msg[MBAP_length+1] + MBAP_unit; j++)printf("%02x ",msg[j]);
          printf("\n");
#endif
          send(fd, msg, msg[MBAP_length+1] + MBAP_unit, 0);//发送数据
          return (0);
        }else{//数据异常响应
          Modbus_Fault_Response(fd, msg, ILLEGAL_DATA_VALUE);
        }
      }
    }else{//长度异常响应
      Modbus_Fault_Response(fd, msg, COMMS_FAILURE);
    }

  }else if(msg[MBAP_unit + 1] == 0x04){
    if(length == CMB04_regnumL + 1){ //帧长度正确
      start_reg = (msg[CMB04_addrH]<<8) + msg[CMB04_addrL];
      reg_num = (msg[CMB04_regnumH]<<8) + msg[CMB04_regnumL];
      if(start_reg <= (reg_04input_nregs - 1)){//地址正确
        if(reg_num <= (reg_04input_nregs - start_reg)){//读取的数据范围正确
          msg[RMB04_regnum] =  msg[CMB04_regnumL]*2;   //寄存器个数
          msg[MBAP_length+1] =  RMB04_regdata + msg[RMB04_regnum] - MBAP_unit;  //长度

          modbus_pc04_mutex.lockForRead();
          index = RMB04_regdata;
          for(j=0; j < reg_num; j++){
            cur_reg_addr = start_reg + j;

            msg[index++] = Globa.modbus_485_PC04[cur_reg_addr] >> 8;    //先传高字节,注意数组不要越界
            msg[index++] = Globa.modbus_485_PC04[cur_reg_addr];         //再传低字节
          }
          modbus_pc04_mutex.unlock();

#if DEBUG_MODBUS_TCP
            printf("sent ength = %d data =", msg[MBAP_length+1] + MBAP_unit);
            for(j=0; j<msg[MBAP_length+1] + MBAP_unit; j++)printf("%02x ",msg[j]);
              printf("\n");
#endif
          send(fd, msg, msg[MBAP_length+1] + MBAP_unit, 0);//发送数据
          return (0);
        }else{
          Modbus_Fault_Response(fd, msg, ILLEGAL_DATA_VALUE);
        }
      }
    }else{
      Modbus_Fault_Response(fd, msg, COMMS_FAILURE);
    }
  }else if(msg[MBAP_unit+1] == 0x10){ //对时功能 0x10
    if(((msg[CMB10_addrH]<<8 | msg[CMB10_addrL]) == 61458)&&((msg[CMB10_regnumH]<<8|msg[CMB10_regnumL])== 6)){ // 6个寄存器，1为起始寄存器地址
      CURRENTTIMESET  SetcurrentTime;  //设置当前时间
      char time[80];
      SetcurrentTime.year =  msg[CMB10_regdata]<<8|msg[CMB10_regdata+ 1];
      SetcurrentTime.month = msg[CMB10_regdata+ 2]<<8|msg[CMB10_regdata+ 3];
      SetcurrentTime.date =  msg[CMB10_regdata+ 4]<<8|msg[CMB10_regdata+ 5];
      SetcurrentTime.hours = msg[CMB10_regdata+ 6]<<8|msg[CMB10_regdata+ 7];
      SetcurrentTime.minutes = msg[CMB10_regdata+ 8]<<8|msg[CMB10_regdata+ 9];
      SetcurrentTime.seconds = msg[CMB10_regdata+ 10]<<8|msg[CMB10_regdata+ 11];
      if((SetcurrentTime.year>=2012)&&(SetcurrentTime.month>0)&&(SetcurrentTime.month<=12)&&(SetcurrentTime.date>0)&&(SetcurrentTime.date<=31)&&(SetcurrentTime.hours<24)&&(SetcurrentTime.minutes<60)&&(SetcurrentTime.seconds<60)){
        sprintf(time, "date -s %02d%02d%02d%02d%04d.%d", SetcurrentTime.month, SetcurrentTime.date, SetcurrentTime.hours, SetcurrentTime.minutes, SetcurrentTime.year, SetcurrentTime.seconds);
        system(time);
        system("hwclock --systohc --utc");
        usleep(10000);
        send(fd, msg, msg[MBAP_length+1] + MBAP_unit, 0);//发送数据
      }
    }
  }else if(msg[MBAP_unit+1] == 0x06){ //对时功能 0x10
   /* start_reg = (msg[CMB04_addrH]<<8) + msg[CMB04_addrL];
    reg_num = (msg[CMB04_regnumH]<<8) + msg[CMB04_regnumL];
    modbus_fun06_ack(start_reg,reg_num);
    send(fd, msg, msg[MBAP_length+1] + MBAP_unit, 0);//发送数据*/


        if(length == 12)
        {//单个设置,长度12合法
          start_reg = (msg[CMB06_addrH]<<8) + msg[CMB06_addrL];
          if(((start_reg <= reg_06_03_end_reg)&&(start_reg >= reg_06_03_start_reg))||((start_reg>=0)&&(start_reg<=CONFIG_CODE3)))
          {//开始地址范围正确
            reg_num=ModbusSetup06(start_reg, &msg[CMB06_regdataH], 2);
            if(reg_num==0x02)
            {
              Modbus_Fault_Response(fd, msg, ILLEGAL_DATA_ADDRESS);
            }
            if(reg_num==0x03)
            {
              Modbus_Fault_Response(fd, msg, ILLEGAL_DATA_VALUE);
            }
            else if(reg_num==0x06)
            {//服务器繁忙
              Modbus_Fault_Response(fd, msg, SEVERISBUSY);
            }
            else
            {//设置完成,正常回复,将收到的消息再发回去
              send(fd, msg, msg[MBAP_length+1] + MBAP_unit, 0);//发送数据
            }
          }
          else
          {//收到的寄存器数据不正确
             Modbus_Fault_Response(fd, msg, ILLEGAL_DATA_ADDRESS);
          }
        }
        else
        {//长度异常响应
          Modbus_Fault_Response(fd, msg, COMMS_FAILURE);
        }



  }else if(msg[MBAP_unit + 1] == 0x03){
    if(length == CMB04_regnumL + 1){ //帧长度正确
      start_reg = (msg[CMB04_addrH]<<8) + msg[CMB04_addrL];
      reg_num = (msg[CMB04_regnumH]<<8) + msg[CMB04_regnumL];
      if((start_reg <= reg_03max_num)&&(start_reg >= 0)){
          if(reg_num <= (reg_03max_num+1 - start_reg)){//读取的数据范围正确
            msg[RMB04_regnum] =  msg[CMB04_regnumL]*2;   //寄存器个数
            msg[MBAP_length+1] =  RMB04_regdata + msg[RMB04_regnum] - MBAP_unit;  //长度

         /*  // modbus_pc_mutex.lockForRead();
            index = RMB04_regdata;

            for(j=0; j < reg_num; j++){
              cur_reg_addr = start_reg + j;
              msg[index++] = Buf[cur_reg_addr] >> 8;    //先传高字节,注意数组不要越界
              msg[index++] = Buf[cur_reg_addr] & 0x00ff;         //再传低字节
            }
           // modbus_pc_mutex.unlock();*/
            modbus_pc03_mutex.lockForRead();
            index = RMB04_regdata;
            for(j=0; j < reg_num; j++){
              cur_reg_addr = start_reg + j;

              msg[index++] = Globa.modbus_485_PC03[cur_reg_addr] >> 8;    //先传高字节,注意数组不要越界
              msg[index++] = Globa.modbus_485_PC03[cur_reg_addr];         //再传低字节
            }
            modbus_pc03_mutex.unlock();

    #if DEBUG_MODBUS_TCP
              printf("sent ength = %d data =", msg[MBAP_length+1] + MBAP_unit);
              for(j=0; j<msg[MBAP_length+1] + MBAP_unit; j++)printf("%02x ",msg[j]);
                printf("\n");
    #endif
            send(fd, msg, msg[MBAP_length+1] + MBAP_unit, 0);//发送数据
            return (0);
          }else{
            Modbus_Fault_Response(fd, msg, ILLEGAL_DATA_VALUE);
          }
        }
    }else{
      Modbus_Fault_Response(fd, msg, COMMS_FAILURE);
    }
  }else{                             //功能码 非法功能码
    Modbus_Fault_Response(fd, msg, ILLEGAL_FUNCTION);
  }

  return (-1);
}


INT32 Tcp_Modbus_Thread::TCP_Modbus_Server()
{  ParameterRange[2*CONFIG_INPUT_VOLTAGE_HIGH_LIMIT]=2200;
   ParameterRange[2*CONFIG_INPUT_VOLTAGE_HIGH_LIMIT+1]=2750;
   ParameterRange[2*CONFIG_INPUT_VOLTAGE_LOW_LIMIT]=1650;
   ParameterRange[2*CONFIG_INPUT_VOLTAGE_LOW_LIMIT+1]=2200;
   ParameterRange[2*CONFIG_INPUT_FREQUENCY_HIGH_LIMIT]=500;
   ParameterRange[2*CONFIG_INPUT_FREQUENCY_HIGH_LIMIT+1]=550;
   ParameterRange[2*CONFIG_INPUT_FREQUENCY_LOW_LIMIT]=450;
   ParameterRange[2*CONFIG_INPUT_FREQUENCY_LOW_LIMIT+1]=500;
   ParameterRange[2*CONFIG_BATTERY_AH]=24;
   ParameterRange[2*CONFIG_BATTERY_AH+1]=200;
   ParameterRange[2*CONFIG_BATTERY_BLOCKS]=28;
   ParameterRange[2*CONFIG_BATTERY_BLOCKS+1]=41;
   ParameterRange[2*CONFIG_BATTERY_STRINGS]=0;
   ParameterRange[2*CONFIG_BATTERY_STRINGS+1]=BATTERYMAXGROUP;
   ParameterRange[2*CONFIG_BATTERY_EOD_VOLT]=102;
   ParameterRange[2*CONFIG_BATTERY_EOD_VOLT+1]=109;

   ParameterRange[2*CONFIG_INPUT_VOLTAGE_AD_PHASE_A]=3686;
   ParameterRange[2*CONFIG_INPUT_VOLTAGE_AD_PHASE_A+1]=4506;
   ParameterRange[2*CONFIG_INVERTER_VOLTAGE_AD_PHASE_A]=3686;
   ParameterRange[2*CONFIG_INVERTER_VOLTAGE_AD_PHASE_A+1]=4506;
   ParameterRange[2*CONFIG_OUTPUT_VOLTAGE_AD_PHASE_A]=3686;
   ParameterRange[2*CONFIG_OUTPUT_VOLTAGE_AD_PHASE_A+1]=4506;
   ParameterRange[2*CONFIG_BATTERY_VOLTAGE_AD]=3686;
   ParameterRange[2*CONFIG_BATTERY_VOLTAGE_AD+1]=4506;
   ParameterRange[2*CONFIG_POSITIVE_BUS_VOLTAGE_AD]=3686;
   ParameterRange[2*CONFIG_POSITIVE_BUS_VOLTAGE_AD+1]=4506;

   ParameterRange[2*CONFIG_CODE0]=1;
   ParameterRange[2*CONFIG_CODE0+1]=9;
   ParameterRange[2*CONFIG_CODE1]=1;
   ParameterRange[2*CONFIG_CODE1+1]=9999;
   ParameterRange[2*CONFIG_CODE2]=1;
   ParameterRange[2*CONFIG_CODE2+1]=9999;
   ParameterRange[2*CONFIG_CODE3]=1;
   ParameterRange[2*CONFIG_CODE3+1]=9999;


   ParameterRange[2*(INVERTER_ON_COMMAND-61440+300)]=0;
   ParameterRange[2*(INVERTER_ON_COMMAND-61440+300)+1]=1;
   ParameterRange[2*(INVERTER_OFF_COMMAND-61440+300)]=0;
   ParameterRange[2*(INVERTER_OFF_COMMAND-61440+300)+1]=1;


   ParameterRange[2*(SETTING_CLEAR_FAULT-61440+300)]=0;
   ParameterRange[2*(SETTING_CLEAR_FAULT-61440+300)+1]=1;
  UINT32 i =0;
  //INT32 k= 0,j=0;
  UINT8 msg[BUF_SIZE];

  INT32 Listen_socket = 0,ret,bind_on;
  struct sockaddr_in local_addr;
  struct sockaddr_in client_addr;

  fd_set fdsr;//文件描述符集的定义
  socklen_t addr_size;
  addr_size = sizeof(struct sockaddr_in);

  UINT32 fd[MAXCLIENT] ={0};
  UINT32 con_time[MAXCLIENT] = {0};
  INT32 new_fd;
  struct timeval tv;
  time_t systime;
  struct tm *tm_t;
  INT32 lastminute = 0, flag_minutechange = 0, maxsock = Listen_socket;

   //建立socket套接字
  if((Listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
     perror("TCP_MODBUS_THREAD failed create socket");
     return(-1);
  }
  maxsock =  Listen_socket;
  //bind API 函数将允许地址的立即重用
  bind_on = 1;
  ret = setsockopt( Listen_socket, SOL_SOCKET, SO_REUSEADDR, &bind_on, sizeof(bind_on));

  struct timeval tv1;
  tv1.tv_sec = 5;
  tv1.tv_usec = 0;
  //设置发送时限
  setsockopt(Listen_socket, SOL_SOCKET, SO_SNDTIMEO, &tv1, sizeof(tv1));
  //设置接收时限
  setsockopt(Listen_socket, SOL_SOCKET, SO_RCVTIMEO, &tv1, sizeof(tv1));

  //设置本机服务类型
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(MODBUS_TCP_PORT);
  local_addr.sin_addr.s_addr = INADDR_ANY;

  //绑定本机IP和端口号
  if(bind(Listen_socket, (struct sockaddr*)&local_addr, sizeof(struct sockaddr)) == -1){
    perror("TCP_MODBUS_THREAD failed bind");
    return(-1);
  }

  //监听客户端连接
   if(listen(Listen_socket, BACKLOG) == -1){
     perror("TCP_MODBUS_THREAD failed listen");
     return(-1);
   }

   while(1){
     thread_tcp=1;
     flag_minutechange = 0;
     systime = time(NULL);        //获得系统的当前时间
     tm_t = localtime(&systime);		//结构指针指向当前时间
     if(tm_t->tm_min != lastminute){//每次循环开始都读取系统时间，与上次分钟数比较，为以下超时判断提供依据
       lastminute = tm_t->tm_min;
       flag_minutechange = 1;
     }

     FD_ZERO(&fdsr); //每次进入循环都重建描述符集
     FD_SET(Listen_socket, &fdsr);//用来设置描述词组fdsrt中相关Listen_socket的位
     for(i = 0; i < MAXCLIENT; i++){//将存在的套接字加入描述符集
       if(fd[i] != 0){
         FD_SET(fd[i], &fdsr);
         if(flag_minutechange == 1){
           con_time[i]--;
           if(con_time[i] <= 0){
             close(fd[i]);
             FD_CLR(fd[i], &fdsr); //用来清除描述词组fdsr中相关fd 的位
             //qDebug()<<"__________fd[i]___________;"<<fd[i]<<con_time[i];
             fd[i] = 0;
           }
         }
       }
     }

     tv.tv_sec = 10;
     tv.tv_usec = 0;

     ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv); //阻塞tv机构当中的一断时间
     if(ret < 0){//<0表示探测失败
       Globa.Eps_Param_Data.PC_Communication_Fault_2 = 1;
       perror("failed select\n");
       break;//退出，看门狗起作用，等待重启
     }else if(ret == 0){//=0表示超时，下一轮循环
       Globa.Eps_Param_Data.PC_Communication_Fault_2 = 1;
       continue;
     }
    //如果select发现有异常，循环判断各活跃连接是否有数据到来
     for(i = 0; i < MAXCLIENT; i++){
       if(FD_ISSET(fd[i], &fdsr)){     //检查与fdsr联系的描述符fd是否可以读写，返回非零表示可以读写
         ret = recv(fd[i], msg, BUF_SIZE, 0);
         if(ret <= 0){// recv<=0,表明客户端关闭连接，服务器也关闭相应连接，并把连接套接子从文件描述符集中清除
#if DEBUG_MODBUS_TCP
           printf("client[%d] close\n", i);
#endif
           close(fd[i]);
           FD_CLR(fd[i], &fdsr);
           fd[i] = 0;
           Globa.Eps_Param_Data.PC_Communication_Fault_2 = 1;
         }else{//否则表明客户端有数据发送过来，作相应接受处理
           con_time[i] = MAX_IDLECONNCTIME; //重新写入fd[i]的超时数，再此之后如果MAX_IDLECONNCTIME分钟内此连接无反应，服务器会关闭该连接

#if DEBUG_MODBUS_TCP
           printf("recv %d length = %d msg =",++k, ret);
           for(j=0; j<ret; j++)printf("%02x ",msg[j]);
             printf("\n");
#endif
             Globa.Eps_Param_Data.PC_Communication_Fault_2 = 0;
             Battery_Tcp_modbus_Deal(fd[i], msg, ret);
         }
       }
     }

       // 以下说明异常有来自客户端的连接请求
     if(FD_ISSET(Listen_socket, &fdsr)){ //判断其描述符是否可读，有数据来了表示可读
       new_fd = accept(Listen_socket, (struct sockaddr *)&client_addr, &addr_size);
       if(new_fd <= 0){
         perror("failed accept");
         continue;
       }

       //判断活跃连接数时候是否小于最大连接数，如果是，添加新连接到文件描述符集中
       for(i = 0; i < MAXCLIENT; i++){
         if(fd[i] == 0){
           fd[i] = new_fd;
           con_time[i] = MAX_IDLECONNCTIME; //每次新建立连接，设置该连接的超时数，如果此连接此后MAX_IDLECONNCTIME分钟内无反应，关闭该连接
#if DEBUG_MODBUS_TCP
           printf("new connection client[%d] %s:%d\n", i, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
#endif
           if(new_fd > maxsock){
             maxsock = new_fd;
           }

           break;
         }
       }

       if(i >= MAXCLIENT){

#if DEBUG_MODBUS_TCP
         printf("MAXCLIENT arrive, exit\n");
         send(new_fd, "over MAXCLIENT\n", 25, 0);
#endif
         close(new_fd);
       }
     }

   }
   return(-1);
}
