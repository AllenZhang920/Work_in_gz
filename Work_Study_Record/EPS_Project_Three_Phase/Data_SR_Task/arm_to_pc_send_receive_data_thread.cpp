#include "arm_to_pc_send_receive_data_thread.h"
extern UINT16 ParameterRange[700];
Arm_To_Pc_Send_Receive_Data_Thread::Arm_To_Pc_Send_Receive_Data_Thread(QWidget *parent) :
  QThread(parent)
{

}

void Arm_To_Pc_Send_Receive_Data_Thread::run()
{

  Arm_To_Pc_Send_Receive_Data();

}


/******************************************************************************/
/*             收到上位机发送的modbus协议命令 之后反馈出去的数据                     */
/*             响应 04 功能码数据                                                 */
/******************************************************************************/
void Arm_To_Pc_Send_Receive_Data_Thread::modbus_func04_ack( UINT16 start_reg_addr, UINT16 reg_num)
{
   UINT32 i,j,z;
   UINT16 cur_reg_addr;
   UINT16 wCRCCheckValue;
   UINT8  send_buf[MAX_MODBUS_FRAMESIZE];   //响应上位机的MODBUS数据发送缓冲区

   UINT32 index = 0;

   send_buf[index++] = Globa.User_Monitor_Screen_Set.backgroundAddress;//dev_cfg.dev_para.uart1_addr;//本机MODBUS地址
   send_buf[index++] = 0x04;//功能码
   //返回的字节数
   i = reg_num * 2;                  //1个地址有2个字节数据,必须<=254,也即最大1次访问127个寄存器地址
   send_buf[index++] = i ;    //字节数
   //填充实际数据

/*  填充后台可能用到的形式
   UINT32 Pc_Analog[400]={0};
   DSP_Analog_Data_mutex.lockForRead();
   for(z=0; z<End_0x04; z++){
      Pc_Analog[z]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[z];

     }
   DSP_Analog_Data_mutex.unlock();

   modbus_pc_mutex.lockForWrite();
   memset(Globa.modbus_485_PC04,0,sizeof(sizeof(Globa.modbus_485_PC04)));
   for(z=0; z<End_0x04; z++){

      Globa.modbus_485_PC04[z]= Pc_Analog[z];
   }
   modbus_pc_mutex.unlock();
*/


   modbus_pc04_mutex.lockForRead();

   for(j=0; j < reg_num;j++){
     cur_reg_addr = start_reg_addr + j;

     send_buf[index++] = Globa.modbus_485_PC04[cur_reg_addr] >> 8;    //先传高字节,注意数组不要越界
     send_buf[index++] = Globa.modbus_485_PC04[cur_reg_addr];         //再传低字节
   }

   modbus_pc04_mutex.unlock();

   //计算CRC
    wCRCCheckValue = ModbusCRC(send_buf, i+3);  //Modbus 协议crc校验
    send_buf[index++] = wCRCCheckValue >> 8;  //crc low byte
    send_buf[index++] = wCRCCheckValue & 0x00ff;//crc high byte
    write_datas_tty(Fd_Uart_Arm_Pc, send_buf, index);
}

/*******************************************************************/
//            收到上位机发送的modbus协议命令 之后反馈出去的数据
//            响应2功能码数据
/********************************************************************/
void Arm_To_Pc_Send_Receive_Data_Thread::modbus_func02_ack( UINT16 start_reg_addr, UINT16 reg_num)
{
  UINT32   bytes_cnt;
  UINT32   j,i;
  UINT32   index = 0;
  UINT32   registers_length=0;

  UINT16 wCRCCheckValue;
  UINT8  send_buf[MAX_MODBUS_FRAMESIZE];   //响应上位机的MODBUS数据发送缓冲区


  memset(send_buf, 0x00, sizeof(send_buf));
  send_buf[index++] = Globa.User_Monitor_Screen_Set.backgroundAddress;//dev_cfg.dev_para.uart1_addr;//本机MODBUS地址
  send_buf[index++] = 0x02;//func code

  if(reg_num <= 8){//离散量数不超过1个字节的8个bit,1个字节即可表示
    bytes_cnt = 1;//1个字节
  }else{
    bytes_cnt = (reg_num%8 == 0) ? (reg_num >> 3): ((reg_num >> 3) + 1);
  }

  send_buf[index++] = bytes_cnt;//返回离散量状态所需的字节数,未使用的高bit为0--
  modbus_pc02_mutex.lockForRead();

  for(j=0; j < bytes_cnt; j++){
    send_buf[index] = 0 ;
    for(i=0; i < 8; i++){
      registers_length ++;
      send_buf[index] |= ((Globa.modbus_485_PC02[start_reg_addr + 8*j+i] & 0x01)<< i);
      if(registers_length == reg_num){
        break;
      }
    }
    index++;
  }
  modbus_pc02_mutex.unlock();
  //计算CRC
  wCRCCheckValue = ModbusCRC(send_buf, 3+bytes_cnt);  //Modbus 协议crc校验
  send_buf[index++] = wCRCCheckValue >> 8; //crc low byte
  send_buf[index++] = wCRCCheckValue & 0x00ff;//crc high byte
  write_datas_tty(Fd_Uart_Arm_Pc, send_buf, index);
}

/********************** 接收到异常的MODBUS指令时向上位机报告异常 ********************/
void Arm_To_Pc_Send_Receive_Data_Thread::modbus_fault_ack(UINT8 err_function_code ,UINT8 err_Correct_code)
{
  UINT16  wCRCCheckValue;
  UINT8  send_buf[MAX_MODBUS_FRAMESIZE];   //响应上位机的MODBUS数据发送缓冲区

  send_buf[0] = Globa.User_Monitor_Screen_Set.backgroundAddress;
  send_buf[1] = err_function_code;
  send_buf[2] = err_Correct_code;  //异常代码 02 非法数据值

  wCRCCheckValue = ModbusCRC(send_buf, 3);  //Modbus 协议crc校验

  send_buf[3] = wCRCCheckValue >> 8;//crc low byte
  send_buf[4] = wCRCCheckValue & 0x00ff ;//crc high byte
  write_datas_tty(Fd_Uart_Arm_Pc, send_buf,5);

}
/****************************************************************/

UINT32 Arm_To_Pc_Send_Receive_Data_Thread::SendSetup(UINT8 adress ,UINT16 Register , UINT16 pdata,UINT8 id)
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


INT32 Arm_To_Pc_Send_Receive_Data_Thread::ModbusSetup06(UINT16 regaddress, UINT8 *msg, UINT32 length)
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
                                 System_Set_DSP_Parame_save();
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
                                   Globa.Send_Dsp_Debug_Param_Set.Manual_Open_Shut_Down=0x0F;
                                   System_Set_DSP_Parame_save();
                                   Insert_Operation_Work_Record(1, "远程手动关机");

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

/*void Arm_To_Pc_Send_Receive_Data_Thread::modbus_fun06_ack(UINT16 start_reg_addr ,UINT16 Input_value)
{
 // UINT32 i = 0;
  //i = i;
 // Input_value = Input_value;

  //printf("in pc \n");
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
/******************************************************************************/
/*             收到上位机发送的modbus协议命令 之后反馈出去的数据                     */
/*             响应 03 功能码数据                                                 */
/******************************************************************************/
void Arm_To_Pc_Send_Receive_Data_Thread::modbus_func03_ack( UINT16 start_reg_addr, UINT16 reg_num)
{
    /*start_reg_addr = start_reg_addr;
    reg_num = reg_num;
*/
  UINT32 i,j,z;
  UINT16 cur_reg_addr;
  UINT16 wCRCCheckValue;
  UINT8  send_buf[MAX_MODBUS_FRAMESIZE];   //响应上位机的MODBUS数据发送缓冲区

  UINT32 index = 0;

  send_buf[index++] = Globa.User_Monitor_Screen_Set.backgroundAddress;//dev_cfg.dev_para.uart1_addr;//本机MODBUS地址
  send_buf[index++] = 0x03;//功能码
  //返回的字节数
  i = reg_num * 2;                  //1个地址有2个字节数据,必须<=254,也即最大1次访问127个寄存器地址
  send_buf[index++] = i ;    //字节数

  //send_buf[index++] = Globa.modbus_485_PC03[cur_reg_addr] >> 8;    //先传高字节,注意数组不要越界
 // send_buf[index++] = Globa.modbus_485_PC03[cur_reg_addr];         //再传低字节



if((start_reg_addr <= CONFIG_CODE3)&&(start_reg_addr >= 0)){
      for(j=0; j < reg_num;j++){
         cur_reg_addr = start_reg_addr + j;
         send_buf[index++] = Globa.modbus_485_PC03[cur_reg_addr] >> 8;    //先传高字节,注意数组不要越界
         send_buf[index++] = Globa.modbus_485_PC03[cur_reg_addr];         //再传低字节
       }
    }



  //计算CRC
   wCRCCheckValue = ModbusCRC(send_buf, i+3);  //Modbus 协议crc校验
   send_buf[index++] = wCRCCheckValue >> 8;  //crc low byte
   send_buf[index++] = wCRCCheckValue & 0x00ff;//crc high byte
   write_datas_tty(Fd_Uart_Arm_Pc, send_buf, index);

}

void Arm_To_Pc_Send_Receive_Data_Thread::Modbus_Fault_Response(INT32 UART_COM, UINT8 *buffer, UINT8 err_num)
{
  /*msg[EXMB_fun] = msg[EXMB_fun] + 0x80;
  msg[EXMB_state] = err_num;
  msg[MBAP_length + 1] = 0x03;
  send(fd, msg, msg[MBAP_length + 1] + MBAP_unit, 0);*/
  UINT16  wCRCCheckValue;
  buffer[1]=buffer[1]+0x80;
  buffer[2]=err_num;
  wCRCCheckValue = ModbusCRC(buffer, 3);  //Modbus 协议crc校验
  buffer[3] = wCRCCheckValue >> 8;//crc low byte
  buffer[4] = wCRCCheckValue & 0x00ff ;//crc high byte
  write_datas_tty(UART_COM, buffer,5);

}

void Arm_To_Pc_Send_Receive_Data_Thread::Arm_To_Pc_Send_Receive_Data()
{
  int ret;

  UINT8	buffer[MAX_MODBUS_FRAMESIZE];
  UINT16   start_reg = 0, reg_num = 0, reg_value = 0;
  UINT16 wCRCCheckValue = 0;
  UINT32  time_number = 0;
  GPIO_RD_PC485_RECEIVE;
  usleep(2000);
  while(1){
    thread_pc=1;
    GPIO_RD_PC485_RECEIVE;
    usleep(2000);
    if(time_number == 0){
      Globa.Eps_Param_Data.PC_Communication_Fault_1 = 0;
    }else if(time_number >= 500){
      Globa.Eps_Param_Data.PC_Communication_Fault_1 = 1;
      time_number = 1;
    }
    time_number ++ ;
  //  printf("____ UART_ttyS2 receive from pc  %d  =%d %d\n ",time_number, Globa.Eps_Param_Data.PC_Communication_Fault_1,Globa.Eps_Param_Data.PC_Communication_Fault_2);

    ret= read_datas_tty(Fd_Uart_Arm_Pc, buffer, MAX_MODBUS_FRAMESIZE, 0, 20000);//系统还有其他任务在执行，超时时间太短的话，收到的数据帧会被分片

#if DEBUG_RS232_PC
    if(ret>0){
      printf("____ UART_ttyS2 receive from pc =%d\n ", ret);
      for(int i=0; i<ret; i++){
        printf("%02x ", buffer[i]);
      }
       printf("\n");
    }
#endif
    if(ret >5){
      time_number = 0;
      if(buffer[0] ==  Globa.User_Monitor_Screen_Set.backgroundAddress){
        wCRCCheckValue = ModbusCRC(&buffer[0], ret-2);//校验CRC

        if(((wCRCCheckValue >> 8) == buffer[ret-2])&&((wCRCCheckValue & 0x00ff) == buffer[ret-1])){
          GPIO_RD_PC485_SEND;
          usleep(2000);
          if(buffer[1] == 0x04){
            start_reg = (buffer[2] << 8) + buffer[3];
            reg_num = (buffer[4]<<8) + buffer[5];
            if((start_reg > reg_04input_nregs ) || ((start_reg + reg_num) > reg_04input_nregs + 1)){  //寄存器访问越界
              if(start_reg > reg_04input_nregs){
                modbus_fault_ack(0x84, 0x02);    //报告异常代码_无效地址
              }else{
                modbus_fault_ack(0x84,0x03);//报告异常代码无效数据
              }
              if(Globa.User_Monitor_Screen_Set.Baudratechoice ==0){
                usleep((2*5+10)*2000);
              }else if(Globa.User_Monitor_Screen_Set.Baudratechoice == 1){
                usleep((2*5+10)*1000);
              }else
                usleep((2*5+10)*500);
//                Led_Relay_Control(21, 1);
            }else{//正常响应
              modbus_func04_ack(start_reg, reg_num);//起始地址实际有效值均小于1个字节，寄存器个数也小于1个字节
              if(Globa.User_Monitor_Screen_Set.Baudratechoice == 0){
                usleep((2*reg_num + 20)*2000);
              }else if(Globa.User_Monitor_Screen_Set.Baudratechoice == 1){
                usleep((2*reg_num + 20)*1000);
              }else
                usleep((2*reg_num + 20)*500);
            //  Led_Relay_Control(21, 1);
            }
          }else if(buffer[1] == 0x02){
            start_reg = (buffer[2] << 8) + buffer[3];
            reg_num = (buffer[4]<<8) + buffer[5];//
            if((start_reg > reg_02input_nregs ) || ((start_reg + reg_num) > reg_02input_nregs + 1)){  //寄存器访问越界
              if(start_reg > reg_02input_nregs){
                modbus_fault_ack(0x82, 0x02);    //报告异常代码_无效地址
              }else{
                modbus_fault_ack(0x82,0x03);//报告异常代码无效数据
              }

              if(Globa.User_Monitor_Screen_Set.Baudratechoice == 0){
                usleep((2*5 + 10)*2000);
              }else if(Globa.User_Monitor_Screen_Set.Baudratechoice == 1){
                usleep((2*5 + 10)*1000);
              }else
                 usleep((2*5 + 10)*500);
           //   Led_Relay_Control(21, 1);
            }else{//正常响应
              modbus_func02_ack(start_reg, reg_num);//起始地址实际有效值均小于1个字节，寄存器个数也小于1个字节
              if(Globa.User_Monitor_Screen_Set.Baudratechoice == 0){
                usleep((reg_num/4 + 20)*2000);
              }else if(Globa.User_Monitor_Screen_Set.Baudratechoice == 1){
                usleep((reg_num/4 + 20)*1000);
              }else
                usleep((reg_num/4 + 20)*500);
          //    Led_Relay_Control(21, 1);
            }
          }else if(buffer[1] == 0x03){
            start_reg = (buffer[2] << 8) + buffer[3];
            reg_num = (buffer[4]<<8) + buffer[5];//
            if((start_reg > reg_03max_num ) ||(start_reg <0 )|| ((start_reg + reg_num) > reg_03max_num)){  //寄存器访问越界
              if((start_reg > reg_03max_num ) ||(start_reg <0 )){
                modbus_fault_ack(0x83, 0x02);    //报告异常代码_无效地址
              }else{
                modbus_fault_ack(0x83,0x03);//报告异常代码无效数据
              }
              if(Globa.User_Monitor_Screen_Set.Baudratechoice ==0){
                usleep((2*5+10)*2000);
              }else if(Globa.User_Monitor_Screen_Set.Baudratechoice == 1){
                usleep((2*5+10)*1000);
              }else
                usleep((2*5+10)*500);
           //   Led_Relay_Control(21, 1);
            }else{//正常响应
              modbus_func03_ack(start_reg, reg_num);//起始地址实际有效值均小于1个字节，寄存器个数也小于1个字节
              if(Globa.User_Monitor_Screen_Set.Baudratechoice == 0){
                usleep((2*reg_num + 20)*2000);
              }else if(Globa.User_Monitor_Screen_Set.Baudratechoice == 1){
                usleep((2*reg_num + 20)*1000);
              }else
                usleep((2*reg_num + 20)*500);
           //   Led_Relay_Control(21, 1);
            }
          }else if(buffer[1] == 0x06){ //对时功能 0x10
            start_reg = (buffer[2] << 8) + buffer[3];
            reg_value = (buffer[4]<<8) + buffer[5];//寄存器设置值
            if((!((start_reg <= reg_06_03_end_reg)&&(start_reg >= reg_06_03_start_reg)))&&(!((start_reg>=0)&&(start_reg<=CONFIG_CODE3)))){//寄存器访问越界

            //if((start_reg >reg_06_03_end_reg)||(start_reg <reg_06_03_start_reg)){  //寄存器访问越界
              modbus_fault_ack(0x86, 0x02);    //报告异常代码_无效地址
              if(Globa.User_Monitor_Screen_Set.Baudratechoice == 0){
                usleep((2*5 + 10)*2000);
              }else if(Globa.User_Monitor_Screen_Set.Baudratechoice == 1){
                usleep((2*5 + 10)*1000);
              }else
                 usleep((2*5 + 10)*500);
           //   Led_Relay_Control(21, 1);
            }else{//正常响应
               /*modbus_fun06_ack(start_reg, reg_value);//起始地址实际有效值均小于1个字节，寄存器个数也小于1个字节
               write_datas_tty(Fd_Uart_Arm_Pc, buffer,8);*/
                UINT8 back_num=0;
                back_num=ModbusSetup06(start_reg,&buffer[4],2);
                if(back_num==0x02)
                {
                    Modbus_Fault_Response(Fd_Uart_Arm_Pc, &buffer[0], 2);
                }
                if(back_num==0x03)
                {
                  Modbus_Fault_Response(Fd_Uart_Arm_Pc, &buffer[0], 3);
                }
                else if(back_num==0x06)
                {//服务器繁忙
                  Modbus_Fault_Response(Fd_Uart_Arm_Pc, &buffer[0], 6);
                }
                else
                {//设置完成,正常回复,将收到的消息再发回去
                  //send(fd, msg, msg[MBAP_length+1] + MBAP_unit, 0);//发送数据
                  write_datas_tty(Fd_Uart_Arm_Pc, buffer,8);
                  }


             if(Globa.User_Monitor_Screen_Set.Baudratechoice == 0){
                usleep((reg_num/4 + 20)*2000);
              }else if(Globa.User_Monitor_Screen_Set.Baudratechoice == 1){
                usleep((reg_num/4 + 20)*1000);
              }else
                usleep((reg_num/4 + 20)*500);
          //    Led_Relay_Control(21, 1);
            }
          }else if(buffer[1] == 0x10){//06
            if(((buffer[2]<<8|buffer[3]) ==61458)&&((buffer[4]<<8|buffer[5])== 6)){ // 6个寄存器，3为起始寄存器地址
              CURRENTTIMESET  SetcurrentTime;  //设置当前时间
              char time[80];
              SetcurrentTime.year =  buffer[7]<<8|buffer[8];
              SetcurrentTime.month =  buffer[9]<<8|buffer[10];
              SetcurrentTime.date =  buffer[11]<<8|buffer[12];
              SetcurrentTime.hours =  buffer[13]<<8|buffer[14];
              SetcurrentTime.minutes =  buffer[15]<<8|buffer[16];
              SetcurrentTime.seconds =  buffer[17]<<8|buffer[18];
              if((SetcurrentTime.year>=2012)&&(SetcurrentTime.month>0)&&(SetcurrentTime.month<=12)&&(SetcurrentTime.date>0)&&(SetcurrentTime.date<=31)&&(SetcurrentTime.hours<24)&&(SetcurrentTime.minutes<60)&&(SetcurrentTime.seconds<60)){
                sprintf(time, "date -s %02d%02d%02d%02d%04d.%d", SetcurrentTime.month, SetcurrentTime.date, SetcurrentTime.hours, SetcurrentTime.minutes, SetcurrentTime.year, SetcurrentTime.seconds);
                system(time);
                system("hwclock --systohc --utc");
                printf("clock\nbuffer:");
                for(int i=0;i<ret;i++)
                  {
                    printf("%x ",buffer[i]);
                  }
                printf("\n");



                int wCRCCheckValue = ModbusCRC(buffer, 6);  //Modbus 协议crc校验

                buffer[6] = wCRCCheckValue >> 8;//crc low byte
                buffer[7] = wCRCCheckValue & 0x00ff ;//crc high byte


                write_datas_tty(Fd_Uart_Arm_Pc, buffer,8);
                if(Globa.User_Monitor_Screen_Set.Baudratechoice == 0){
                   usleep((2 + 20)*2000);
                 }else if(Globa.User_Monitor_Screen_Set.Baudratechoice == 1){
                   usleep((2 + 20)*1000);
                 }else
                   usleep((2 + 20)*500);
              }else{
                modbus_fault_ack(0x90, 0x03);//报告异常代码无效数据
              }
            }else{
              if((buffer[2]<<8|buffer[3]) != 3){
                modbus_fault_ack(0x90, 0x02);//报告异常代码无效数据
              }else{
                modbus_fault_ack(0x90, 0x03);//报告异常代码无效数据
              }
            }
          }else{
            modbus_fault_ack((0x80 + buffer[1]), 0x01);//报告异常代码无效数据
            if(Globa.User_Monitor_Screen_Set.Baudratechoice == 0){
              usleep(20*2000);
            }else if(Globa.User_Monitor_Screen_Set.Baudratechoice == 1){
              usleep(20*1000);
            }else
              usleep(20*500);
       //     Led_Relay_Control(21, 1);
          }
        }
      }
    }
  }
}
