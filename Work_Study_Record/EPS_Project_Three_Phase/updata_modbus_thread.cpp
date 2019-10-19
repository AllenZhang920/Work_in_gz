#include "updata_modbus_thread.h"


Updata_Modbus_Thread::Updata_Modbus_Thread(QObject *parent) :
  QThread(parent)
{
}
void Updata_Modbus_Thread :: run(){
  Updata_Modbus_Data();
  exit();
}
void Updata_Modbus_Thread ::Updata_Modbus_Data(){
  while(1){
      sleep(1);
      Updata_ModbusFun03_Config();

    }
}

void Updata_Modbus_Thread ::Updata_ModbusFun03_Config(){
  modbus_pc03_mutex.lockForWrite();
  Globa.modbus_485_PC03[CONFIG_INPUT_VOLTAGE_HIGH_LIMIT]=Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Top_limit;//主电电压上限
  Globa.modbus_485_PC03[CONFIG_INPUT_VOLTAGE_LOW_LIMIT]=Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Low_limit;
  Globa.modbus_485_PC03[CONFIG_INPUT_FREQUENCY_HIGH_LIMIT]=Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Top_limit;
  Globa.modbus_485_PC03[CONFIG_INPUT_FREQUENCY_LOW_LIMIT]=Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Low_limit;

  Globa.modbus_485_PC03[CONFIG_BATTERY_AH]=Globa.Send_Dsp_Debug_Param_Set.Battery_Capacity;

  Globa.modbus_485_PC03[CONFIG_BATTERY_BLOCKS]=Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number;//
  Globa.modbus_485_PC03[CONFIG_BATTERY_STRINGS]=Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number;//
  Globa.modbus_485_PC03[CONFIG_BATTERY_EOD_VOLT]=Globa.Send_Dsp_Debug_Param_Set.Battery_Discharge_Termination_V;//
  Globa.modbus_485_PC03[CONFIG_INPUT_VOLTAGE_AD_PHASE_A]=Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Calibration_Coefficient;//
  Globa.modbus_485_PC03[CONFIG_INVERTER_VOLTAGE_AD_PHASE_A]=Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_Calibration_Coefficient;//
  Globa.modbus_485_PC03[CONFIG_OUTPUT_VOLTAGE_AD_PHASE_A]=Globa.Send_Dsp_Debug_Param_Set.Ouput_Voltage_Calibration_Coefficient;//
  Globa.modbus_485_PC03[CONFIG_BATTERY_VOLTAGE_AD]=Globa.Send_Dsp_Debug_Param_Set.Battery_Voltage_Calibration_Coefficient;//
  Globa.modbus_485_PC03[CONFIG_POSITIVE_BUS_VOLTAGE_AD]=Globa.Send_Dsp_Debug_Param_Set.Bus_Voltage_Calibration_Coefficient;//
  Globa.modbus_485_PC03[CONFIG_CODE0]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0];//
  Globa.modbus_485_PC03[CONFIG_CODE1]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1];//
  Globa.modbus_485_PC03[CONFIG_CODE2]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2];//
  Globa.modbus_485_PC03[CONFIG_CODE3]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3];//






  /*if(Globa.Send_Dsp_Debug_Param_Set.Manual_Open_Shut_Down==0x0F)
    {
      Globa.modbus_485_PC03[INVERTER_ON_COMMAND]=0;
      Globa.modbus_485_PC03[INVERTER_OFF_COMMAND]=1;
    }else{
      Globa.modbus_485_PC03[INVERTER_ON_COMMAND]=1;
      Globa.modbus_485_PC03[INVERTER_OFF_COMMAND]=0;
    }
    */
  modbus_pc03_mutex.unlock();



}
