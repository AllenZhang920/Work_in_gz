#include "arm_dsp_send_receive_data_thread.h"
#include "Share/param.h"
UINT16 SETDATA_Machine_Code[4]= {0};
ARM_DSP_Send_Receive_Data_Thread::ARM_DSP_Send_Receive_Data_Thread(QWidget *parent) :
  QThread(parent)
{
  //pSend_Dsp_Debug_Param_Set = &Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number;
}

void ARM_DSP_Send_Receive_Data_Thread::run()
{
  ARM_DSP_Send_Receive_Data();
  exec();
}


/************************************************************************
****  DSP储存数据
************************************************************************/
void ARM_DSP_Send_Receive_Data_Thread::DSP_DATA_Stone(UINT8 addr, UINT8 function, UINT8 *pdata)
{
  UINT8 i = 0;
  UINT32 emer_state=0;
  if(addr == DSPSTARTADDR){
    if(function == 0x02){//获取状态量和告警
       DSP_State_Alarm_mutex.lockForWrite();
      //if(Reset_time==0){
      for(i = 0; i < End_0x02; i++ ){
        Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[i] = GETBIT(*(pdata + i/8), i%8+1);//1个字节数

      }

      Globa.Eps_Param_Data.Dsp_Data_Refresh[0] = 1;//获取状态量和告警数据刷新标志
        //}
      DSP_State_Alarm_mutex.unlock();
    /*pc21  modbus_pc02_mutex.lockForWrite();
     for(i =0; i < End_0x02; i++ ){

         Globa.modbus_485_PC02[i+Bdsp_fault] = GETBIT(*(pdata + i/8), i%8+1);//1个字节数
       }
      modbus_pc02_mutex.unlock();
*/
   }else if(function == 0x04){//获取模拟量数据
      DSP_Analog_Data_mutex.lockForWrite();
     for(i=0; i<End_0x04; i++){
       Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[i] = (*(pdata + 2*i))<<8|(*(pdata + 2*i+1));
     }
      Globa.Eps_Param_Data.Dsp_Data_Refresh[1] = 1;//获取模拟量 数据刷新标志
      emer_state=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status];
      if(emer_state!=2){
          memset(&Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_Frequency],0,2*7);
        }
      DSP_Analog_Data_mutex.unlock();


   }else if(function == 0x03){//获取软件版本和校准系数
     pSend_Dsp_Debug_Param_Set = &Globa.Send_Dsp_Debug_Param_Set.Equipment_Capacity;
     for(i=0; i<End_0x03; i++){
       *pSend_Dsp_Debug_Param_Set++ = (*(pdata + 2*i))<<8|(*(pdata + 2*i+1));
     }
     SETDATA_Machine_Code[0] = Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0];
     SETDATA_Machine_Code[1] = Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1];
     SETDATA_Machine_Code[2] = Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2];
     SETDATA_Machine_Code[3] = Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3];
     Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable=Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number;
     System_Set_DSP_Parame_save();
     System_Battery_GroupParam_Set_save();

     for(int i=0;i<BATTERYMAXGROUP;i++)
     {

         if((INT32)(Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number-24)>=0)
           {

             Globa.Battery_Param_Set[i].Singlenumber[1]=Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number-24;

           }
         else
           {

             Globa.Battery_Param_Set[i].Singlenumber[1]=0;

           }


     }
     Globa.Eps_Param_Data.Current_Alarm_List_Battery_Refresh=1;

     System_Battery_Param_Set_save();

      for(int t=0;t<20;t++){
         test_data[t]=*(pdata+20+t);
      }
      Globa.Eps_Param_Data.Dsp_Debug_Parame_flags = 1;
      Globa.Eps_Param_Data.Dsp_Data_Refresh[2] = 1;//软件版本和校准系数 数据刷新标志
     }
   }
}
void ARM_DSP_Send_Receive_Data_Thread::DSP_ANALOG_STATUS_DATA()
{
  modbus_pc04_mutex.lockForWrite();


  DSP_Analog_Data_mutex.lockForRead();
  Globa.modbus_485_PC04[MAINS_INPUT_NUM_LINES]=3;
  Globa.modbus_485_PC04[OUTPUT_NUM_LINES]=3;

  Globa.modbus_485_PC04[MAINS_AC_VOLT_PHASE_A]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_A_Phase_Voltage];
  Globa.modbus_485_PC04[MAINS_AC_VOLT_PHASE_B]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_B_Phase_Voltage];
  Globa.modbus_485_PC04[MAINS_AC_VOLT_PHASE_C]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_C_Phase_Voltage];
  Globa.modbus_485_PC04[MAINS_FREQUENCY]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_A_Phase_Frequency];

  Globa.modbus_485_PC04[INVERTER_AC_VOLT_PHASE_A]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_A_Phase_Voltage];
  Globa.modbus_485_PC04[INVERTER_AC_VOLT_PHASE_B]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_B_Phase_Voltage];
  Globa.modbus_485_PC04[INVERTER_AC_VOLT_PHASE_C]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_C_Phase_Voltage];
  Globa.modbus_485_PC04[INVERTER_FREQUENCY]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_Frequency];


  Globa.modbus_485_PC04[OUTPUT_AC_VOLT_PHASE_A]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Voltage];
  Globa.modbus_485_PC04[OUTPUT_AC_VOLT_PHASE_B]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_B_Phase_Voltage];
  Globa.modbus_485_PC04[OUTPUT_AC_VOLT_PHASE_C]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_C_Phase_Voltage];
  Globa.modbus_485_PC04[OUTPUT_AC_CURRENT_PHASE_A]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Current];
  Globa.modbus_485_PC04[OUTPUT_AC_CURRENT_PHASE_B]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_B_Phase_Current];
  Globa.modbus_485_PC04[OUTPUT_AC_CURRENT_PHASE_C]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_C_Phase_Current];
  Globa.modbus_485_PC04[OUTPUT_FREQUENCY]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Frequency];

  Globa.modbus_485_PC04[BUS_DC_VOLT_TOTAL]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Bus_Voltage];

  Globa.modbus_485_PC04[LOAD_LEVEL_PHASE_A]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Load_Per];
  Globa.modbus_485_PC04[LOAD_LEVEL_PHASE_B]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_B_Phase_Load_Per];
  Globa.modbus_485_PC04[LOAD_LEVEL_PHASE_C]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_C_Phase_Load_Per];

  Globa.modbus_485_PC04[AMBIENT_TEMPERATURE]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ambient_Temperature];
  Globa.modbus_485_PC04[BATTERY_DC_VOLTAGE_POSITIVE]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Charging_Voltage];
  Globa.modbus_485_PC04[BATTERY_CURRENT]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Current];
  Globa.modbus_485_PC04[BATTERY_REMAIN_CAPACITY]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Remaining_Capacity];
  Globa.modbus_485_PC04[BATTERY_REMAIN_TIME]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Remaining_Time];

  /*if(!Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status]){
      Globa.modbus_485_PC04[SYSTEM_WORK_MODE]=2;
    }else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status]==1){
      Globa.modbus_485_PC04[SYSTEM_WORK_MODE]=4;
    }else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status]==2){
      Globa.modbus_485_PC04[SYSTEM_WORK_MODE]=5;
    }*/
  switch (Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status]) {
    case 1:
      Globa.modbus_485_PC04[SYSTEM_WORK_MODE]=4;
      break;
    case 2:
      Globa.modbus_485_PC04[SYSTEM_WORK_MODE]=5;
      break;
    case 0:
      Globa.modbus_485_PC04[SYSTEM_WORK_MODE]=2;
      break;
    default:
      break;
    }

  if((Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Start]==1)||(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Start]==2)){
      Globa.modbus_485_PC04[BATTERY_DISCHARGE_STATUS]=2;
    }else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Start]==3){
      Globa.modbus_485_PC04[BATTERY_DISCHARGE_STATUS]=3;
    }else{
      Globa.modbus_485_PC04[BATTERY_DISCHARGE_STATUS]=1;
    }
    DSP_Analog_Data_mutex.unlock();


    DSP_State_Alarm_mutex.lockForRead();
    if((Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Manual_Mode]==1)&&(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Automatic_Mode]==0)){
        Globa.modbus_485_PC04[SYSTEM_MODE_SELECT]=2;
      }else{
        Globa.modbus_485_PC04[SYSTEM_MODE_SELECT]=1;
      }
    if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Battery_Connection_Abnormal]==0){
        Globa.modbus_485_PC04[BATTERY_CONNECTED_STATUS]=2;
      }else{
        Globa.modbus_485_PC04[BATTERY_CONNECTED_STATUS]=1;
      }
    if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Inverter_Shutdown]==1)
    {
        Globa.modbus_485_PC04[INVERTER_WORK_STATUS]=1;
    }else{
        Globa.modbus_485_PC04[INVERTER_WORK_STATUS]=3;
      }
    DSP_State_Alarm_mutex.unlock();
  modbus_pc04_mutex.unlock();
}
void ARM_DSP_Send_Receive_Data_Thread::DSP_NOTICE_ALARM_DATA()
{
  modbus_pc02_mutex.lockForWrite();
  DSP_State_Alarm_mutex.lockForRead();



  memset(&Globa.modbus_485_PC02[SUMMARY_NOTICE],0,sizeof(Globa.modbus_485_PC02));


  if(Globa.Eps_Param_Data.Dsp_Communication_Fault||Globa.ATS_Module_Fault||Globa.Switch_Module_Fault[0]||Globa.Switch_Module_Fault[1])
  {
      Globa.modbus_485_PC02[COMMUNICATION_FAILED]=1;//   2	通信异常
  }
  else if(!Globa.Eps_Param_Data.Dsp_Communication_Fault)
  {
      Globa.modbus_485_PC02[COMMUNICATION_FAILED]=0;//   2	通信异常

  }
  Globa.modbus_485_PC02[MAINS_AC_ABNORMAL]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_Abnormal];//	3	市电异常
  Globa.modbus_485_PC02[MAINS_AC_VOLTAGE_ABNORMAL]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_Voltage_Abnormal];
  Globa.modbus_485_PC02[MAINS_FREQUENCY_ABNORMAL]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_Frequency_Abnormal];
  Globa.modbus_485_PC02[MAINS_AC_PHASE_ERROR]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_Input_Phase_Fault];
  Globa.modbus_485_PC02[BATTERY_LOW_VOLTAGE]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Battery_Under_Voltage];
  Globa.modbus_485_PC02[BATTERY_END_OF_DISCHARGE]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Battery_UNDER_VOLTAGE_SHUTDOWN];//?
  Globa.modbus_485_PC02[BATTERY_VOLTAGE_ABNORMAL]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Battery_Over_Voltage];
  Globa.modbus_485_PC02[OUTPUT_ABNORMAL_VOLTAGE]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Ouput_Open];//	17	输出异常
  Globa.modbus_485_PC02[SYSTEM_FAN_LOCK]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Fan_Fault];	//	19	系统风扇告警
  Globa.modbus_485_PC02[SYSTEM_FORCE_START]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Strong_Start_Mode];//23  系统强制启动


  Globa.modbus_485_PC02[MAINS_SCR_FAULT]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_SCR_Fault];//市电SCR故障
  Globa.modbus_485_PC02[BUS_DC_SOFT_START_OVERTIME]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[DC_Soft_Start_Fails];//	35	BUS软启动超时
  Globa.modbus_485_PC02[ BUS_DC_SHORT]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Bus_Short_Circuit];//	36	母线短路


  Globa.modbus_485_PC02[BUS_DC_HIGH_VOLTAGE]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Bus_Over_Voltage];//	    ,     37//母线过压
  Globa.modbus_485_PC02[INVERTER_FAULT]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_Fault]|Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_Over_Courent];//	39	逆变器故障
  Globa.modbus_485_PC02[INVERTER_SOFT_START_OVERTIME]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_Soft_Start_Fails];//	40	逆变器软启动超时
  Globa.modbus_485_PC02[INVERTER_IGBT_OVER_CURRENT]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_Igbt_Over_Current];	//	41	逆变器IGBT过流
  Globa.modbus_485_PC02[INVERTER_SCR_FAULT]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_SCR_Fault];//	42	逆变SCR故障
    //printf("igbt%d\n",Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_Igbt_Over_Current]);
    //printf("escr%d\n",Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_SCR_Fault]);
    Globa.modbus_485_PC02[INVERTER_OVER_TEMPERATURE]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_Over_Temp];//	43	逆变器过温
    Globa.modbus_485_PC02[INVERTER_HIGH_VOLTAGE]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_Over_Voltage];	//	44	逆变高压
    Globa.modbus_485_PC02[INVERTER_LOW_VOLTAGE]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_Under_Voltage];	//	45	逆变低压
    //Globa.modbus_485_PC02[BATTERY_TEMPERATURE_ABNORMAL]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[0];
    //Globa.modbus_485_PC02[BATTERY_TEMPERATURE_ABNORMAL]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[0];


    //Globa.modbus_485_PC02[OUTOPUT_FAULT]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_Over_Courent];//46输出故障
    Globa.modbus_485_PC02[OUTPUT_SHORT]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Ouput_Short_Circuit];//47输出短路
    Globa.modbus_485_PC02[OVERLOAD_FAILED]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_A_Phase_Overload]\
                                          |Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_B_Phase_Overload]\
                                          |Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_C_Phase_Overload];//过载故障
    Globa.modbus_485_PC02[OVER_LOAD_OVER_TIME]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Ouput_Over_Load_Shutdown]|Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Battery_Fuse_Fault];//49	过载超时
    Globa.modbus_485_PC02[SYSTEM_OVER_TEMPERATURE_FAULT]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Over_Temp_Fault];//52	系统过温故障
    Globa.modbus_485_PC02[BATTERY_ABNORMAL]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Battery_OVER_CURR];//55	电池过流

    Globa.modbus_485_PC02[CHARGER_FAULT]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Charge_Open_Short_Circuit];//	56	充电器故障?
    Globa.modbus_485_PC02[USER_OPERATE_ERROR]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[User_Error];//	57	用户操作错误
    Globa.modbus_485_PC02[MAINS_AC_LOSS]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_Open];//65市电丢失
    //Globa.modbus_485_PC02[BATTERY_CONTACTOR_ABNORMAL]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Battery_Contacts_Fault];//97电池接触器异常
    Globa.modbus_485_PC02[ALARM_EMERGENCY_UNBALANCE]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[EMERGENCY_UNBALANCE];//124应急电压不平衡
    Globa.modbus_485_PC02[ALARM_Battery_Connection_Abnormal]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Battery_Connection_Abnormal];//125 电池连接异常
    //Globa.modbus_485_PC02[BATTERY_VOLTAGE_FAULT]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Battery_Over_Voltage];//177电池电压故障
    if(Globa.Curstomer_Param_Set.Distribution_Configure_Flage==1)
      {
        for(int j=0;j<64;j++)
        {
            Globa.modbus_485_PC02[ALARM_SWITCH+j]=(Globa.Switch_Value[j/8]>>(j%8))&0x01;
        }
    }


    if(Globa.Curstomer_Param_Set.ATS_Configure_Flage==1)
    {
        if(Globa.ATS_Value[0]==0)
        {
            Globa.modbus_485_PC02[MAIN_STATUS]=1;
            Globa.modbus_485_PC02[STANDBY_STATUS]=0;

         }
        else
        {
            Globa.modbus_485_PC02[MAIN_STATUS]=0;
            Globa.modbus_485_PC02[STANDBY_STATUS]=1;
        }


    }


    DSP_State_Alarm_mutex.unlock();

    DSP_Analog_Data_mutex.lockForRead();
    if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status] == 2\
       ||Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status]==2){
        Globa.modbus_485_PC02[BATTERY_TEST_ABNORMAL]=1;	//	21	电池自检失败
     }else{
        Globa.modbus_485_PC02[BATTERY_TEST_ABNORMAL]=0;
      }

    DSP_Analog_Data_mutex.unlock();
  modbus_pc02_mutex.unlock();
}
void ARM_DSP_Send_Receive_Data_Thread::DSP_DATA_Clean()
{

  DSP_State_Alarm_mutex.lockForWrite();
  memset(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM, 0, sizeof(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM));
  DSP_State_Alarm_mutex.unlock();

  /*pc22modbus_pc02_mutex.lockForWrite();
  memset(&Globa.modbus_485_PC02[Bdsp_fault],0,End_0x02*4);
  modbus_pc02_mutex.unlock();
*/
  DSP_Analog_Data_mutex.lockForWrite();
  memset(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA, 0, sizeof(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA));
  DSP_Analog_Data_mutex.unlock();

  modbus_pc04_mutex.lockForWrite();
  memset(&Globa.modbus_485_PC04[MAINS_INPUT_NUM_LINES],0,STATUS_BASE_TYPE*4);
  modbus_pc04_mutex.unlock();

  modbus_pc02_mutex.lockForWrite();
  //memset(&Globa.modbus_485_PC02[SUMMARY_NOTICE],0,sizeof(Globa.modbus_485_PC02));
  Globa.modbus_485_PC02[COMMUNICATION_FAILED]=1;//
  modbus_pc02_mutex.unlock();
}

void ARM_DSP_Send_Receive_Data_Thread::ARM_DSP_Send_Receive_Data()
{
  UINT32 loop = 0;
  INT32  ret = 0;

  UART_M4_MSG msg;
  UINT16 wCRCCheckValue = 0;  
  UINT32 DSP_Comm_Fault = 0;
  bool Issuedequipment_Flag=false;//下发监控版本标志

  UINT32 DSP_Fault_Count=0;
  UINT32 DSP_Detection_Mod_sent_flag=0;


  while(1){
      thread_dsp=1;
      DSP_ANALOG_STATUS_DATA();
      DSP_NOTICE_ALARM_DATA();
    if(!Issuedequipment_Flag){
        if(Issuedequipment_parameters_06(DSPSTARTADDR, Monitoring_Software_version_Enum, Globa.Send_Dsp_Debug_Param_Set.Monitoring_Software_version, 1)){
        Issuedequipment_Flag=true;//下发监控版本
      }
    }


    if(DSP_Detection_Mod_sent_flag == 1){//
      if(DSP_Fault_Count>= MAX_LOST_COMMUNIT_NUM){//没有响应
        if(DSP_Comm_Fault == 1){
          DSP_Comm_Fault = 0;
        }
      }else{
        DSP_Fault_Count++;
      }
    }



    if(Globa.Eps_Param_Data.Dsp_Debug_Parame_flags == 0){/********************获取软件版本和校准系数********************/
      msg.argv[fun_addr] = DSPSTARTADDR;   //addr  这个地址要加偏移量
      msg.argv[fun_fun] = 0x03;  //功能码
      msg.argv[fun_regh] = 0x00;  //起始地址高字节
      msg.argv[fun_regl] = 0x00;  //起始地址低字节，起始地址----获取故障信息
      msg.argv[fun_lenh] = 0x00;  //总共位数的数据个数高位
      msg.argv[fun_lenl] = 48; //总共位数的数据个数低位---48

      wCRCCheckValue = ModbusCRC(&msg.argv[fun_addr], 6);  //Modbus 协议crc校验
      msg.argv[fun_crch] = wCRCCheckValue >> 8;
      msg.argv[fun_crcl] = wCRCCheckValue & 0x00ff;

    }else{//********************DSP状态告警查询(0x02)********************
      msg.argv[fun_addr] = DSPSTARTADDR;   //addr  这个地址要加偏移量
      msg.argv[fun_fun] = 0x02;     //功能码
      msg.argv[fun_regh] = 0x00;    //起始地址高字节
      msg.argv[fun_regl] = 0x00;    //起始地址低字节，起始地址----获取状态量和告警信息
      msg.argv[fun_lenh] = 0x00;    //总共位数的数据个数高位
      msg.argv[fun_lenl] = End_0x02;//总共位数的数据个数低位

      wCRCCheckValue = ModbusCRC(&msg.argv[fun_addr], 6);  //Modbus 协议crc校验
      msg.argv[fun_crch] = wCRCCheckValue >> 8;
      msg.argv[fun_crcl] = wCRCCheckValue & 0x00ff;

    }

     //发送消息给 串口5
     msg.type = 1; //m4的串口通道号
     msgsnd(Globa.uart_m4_msg_sent_id, &msg, 8, IPC_NOWAIT);
     DSP_Detection_Mod_sent_flag=1;
     usleep(30000);

     if(DSP_Detection_Mod_sent_flag == 1){//有发送命令，等待响应
     for(loop=0;loop<50;loop++){
       ret = msgrcv(Globa.uart_m4_msg_recv_id, &msg, sizeof(UART_M4_MSG)-sizeof(long), 1, IPC_NOWAIT);//非阻塞等待
       if(ret>0){
         DSP_DATA_Stone(msg.argv[0], msg.argv[1],&msg.argv[3]); //  数据存放在链表中
         DSP_Comm_Fault = 1;
         DSP_Fault_Count=0;
         DSP_Detection_Mod_sent_flag=0;
         break;
       }
       usleep(20000);
    }
}
    /********************获取模拟量********************/
    msg.argv[fun_addr] = DSPSTARTADDR;   //addr  这个地址要加偏移量
    msg.argv[fun_fun] = 0x04;       //功能码
    msg.argv[fun_regh] = 0x00;      //起始地址高字节
    msg.argv[fun_regl] = 0x00;      //起始地址低字节，起始地址----获取模拟量
    msg.argv[fun_lenh] = 0x00;      //总共位数的数据个数高位
    msg.argv[fun_lenl] = End_0x04;  //总共位数的数据个数低位

    wCRCCheckValue = ModbusCRC(&msg.argv[fun_addr], 6);  //Modbus 协议crc校验
    msg.argv[fun_crch] = wCRCCheckValue >> 8;
    msg.argv[fun_crcl] = wCRCCheckValue & 0x00ff;
    msg.type = 1; //m4的串口通道号

    msgsnd(Globa.uart_m4_msg_sent_id, &msg, 8, IPC_NOWAIT);
    DSP_Detection_Mod_sent_flag=1;
    usleep(20000);
    if(DSP_Detection_Mod_sent_flag == 1){//有发送命令，等待响应
    for(loop=0;loop<50;loop++){
      ret = msgrcv(Globa.uart_m4_msg_recv_id, &msg, sizeof(UART_M4_MSG)-sizeof(long), 1, IPC_NOWAIT);//非阻塞等待
      if(ret>0){
        DSP_DATA_Stone(msg.argv[0], msg.argv[1], &msg.argv[3]); //  数据存放在链表中
        DSP_Comm_Fault = 1;
        DSP_Fault_Count=0;
        DSP_Detection_Mod_sent_flag=0;

        break;
      }
      usleep(20000);
    }
   }



    if(DSP_Comm_Fault){
       Globa.Eps_Param_Data.Dsp_Communication_Fault = 0;


    }else{
      Globa.Eps_Param_Data.Dsp_Communication_Fault = 1;
      Globa.Eps_Param_Data.Dsp_Debug_Parame_flags =0;
      DSP_DATA_Clean();
    }
    /*pc23
    modbus_pc02_mutex.lockForWrite();
    Globa.modbus_485_PC02[Bdsp_commu_fault]=Globa.Eps_Param_Data.Dsp_Communication_Fault;
    modbus_pc02_mutex.unlock();*/
    usleep(200000);
  }
}
