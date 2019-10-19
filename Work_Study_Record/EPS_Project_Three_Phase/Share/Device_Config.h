#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include "Macro_Definition.h"
#include"Share/backdata.h"

typedef struct{
  UINT16  year;
  UINT8   month;
  UINT8   date;
  UINT8   hours;
  UINT8   minutes;
  UINT8   seconds;
}CURRENTTIMESET;

typedef struct _UART_M4_MSG{
  long  type;
  UINT8 argv[MAX_MODBUS_FRAMESIZE];
}UART_M4_MSG;

/********************qt 屏保设置*************************/
typedef struct _BK_CON_STRUCT{
  long   msg_type;
  UINT32 msg_text[1];
}BK_CON_STRUCT;

typedef struct _CURRENT_ALARMFAULT_EVENT{
  QString   name;
  QString   time;
}CURRENT_ALARMFAULT_EVENT ; // 存放当前告警信息记录

typedef struct _HISTORY_ALARMFAULT_EVENT{
  QString    name;
  QString   start_time;
  QString   end_time;
}HISTORY_ALARMFAULT_EVENT ; // 存放历史告警信息记录


typedef struct _OPERATION_WORK_RECORD{
  QString   name;
  QString   time;
}OPERATION_WORK_RECORD; //操作记录以及工作记录

typedef struct _NET_SETTING{
  INT8 IP_address[16];
  INT8 msk_address[16];
  INT8 gate_address[16];
  INT8 main_DNS_address[16];
  INT8 minor_DNS_address[16];
  INT8 assign_method;
  INT8 cipher[16];
}NET_SETTING;


/*------------------调试参数设置（超级用户电池模块的)-----------------------*/
typedef struct{
  UINT32   Batterycurrentgaincalculation;  //电池电流计算增益因子
  UINT32   BatteryCapacitorchargingtime;   //电池电容充电时间
  UINT32   BatteryVoltageadjustmentfactor1; //电池电压计算增益调整因子1-13
  UINT32   BatteryVoltageadjustmentfactor2; //电池电压计算增益调整因子14-24
  UINT32   Batteryhuoerbianbi;              //霍尔电流传感器参数
}Debugparameter_batterySet;

/*---------------------------- 电池巡检仪  -------------------------------*/
typedef enum _BATT_MOD_FUN04_ENUM{
  BATT_MOD_CHANEL_1_V,    //= 0 第一路电池电压
  BATT_MOD_CHANEL_2_V,
  BATT_MOD_CHANEL_3_V,
  BATT_MOD_CHANEL_4_V,
  BATT_MOD_CHANEL_5_V,
  BATT_MOD_CHANEL_6_V,     //=5
  BATT_MOD_CHANEL_7_V,
  BATT_MOD_CHANEL_8_V,
  BATT_MOD_CHANEL_9_V,
  BATT_MOD_CHANEL_10_V,
  BATT_MOD_CHANEL_11_V,     //=10
  BATT_MOD_CHANEL_12_V,
  BATT_MOD_CHANEL_13_V,
  BATT_MOD_CHANEL_14_V,
  BATT_MOD_CHANEL_15_V,
  BATT_MOD_CHANEL_16_V,     //=15
  BATT_MOD_CHANEL_17_V,
  BATT_MOD_CHANEL_18_V,
  BATT_MOD_CHANEL_19_V,
  BATT_MOD_CHANEL_20_V,
  BATT_MOD_CHANEL_21_V,     //=20
  BATT_MOD_CHANEL_22_V,
  BATT_MOD_CHANEL_23_V,
  BATT_MOD_CHANEL_24_V,
  BATT_MOD_BAT_H_A,
  BATT_MOD_BAT_L_A,         //=25
  BATT_MOD_TEMPRATURE_1,
  BATT_MOD_TEMPRATURE_2,
  BATT_MOD_TEMPRATURE_MCU,
  BATT_MOD_TEMPRATURE_AD,
  BATT_MOD_RESERVER_1,      //=30
  BATT_MOD_RESERVER_2       //=31
}BATT_MOD_FUN04_ENUM;

/*---------------------电池仪参数（管理员） ------------------------*/
typedef struct{
  UINT32   Battery_Number;                       //电池巡检仪个数
  UINT32   Singlenumber[BATTERYMAXNUMEBR];       //检测单体数 //  x1
  UINT32   keep[5]; //霍尔电流传感器参数
}BATTERY_PARAMER_SET;

/*---------------------电池仪参数（管理员） ------------------------*/
typedef struct{

  UINT32   Battery_GROUP;       //电池组数
  UINT32   Battery_Capacity;   //电池总容量
  UINT32   Battery_Discharge_End_V;   //电池放电总止电压
  UINT32   SETDATA_Stem_Node_Set[8];  //干结点设置
  UINT32   userpassword_enable;    //密码使能
  UINT32   battery_detecting_disable;    //开/关电池巡检功能0:开启,  1:关闭
  UINT32   Battery_GROUP_Before_Disable;//关闭电池巡检功能前电池组数
  UINT32   keep[5];
}BATTERY_GROUPPARAMER_SET;


/*-------------------------系统参数设置（用户） ------------------------*/
typedef struct{
  UINT32    System_language;
  UINT32    AlarmSound;                //告警消音
  UINT32    Screensavertime;           //屏保时间 x10
  UINT32    screen_brightness;        //屏幕亮度
  UINT32    backgroundAddress;      //后台地址
  UINT32    Baudratechoice;         //波特率选择             x1
  UINT32    paritychoice;          //奇偶校验   x1
  UINT32    dynamic_static_ipmethod;//是否动态获取ip地址***0-动态**** 1-静态******/
  UINT32    keep[5]; //预留
  INT8      userpassword[7];                     //用户密码
  INT8      engineerpassword[7];//工程师密码
}User_System;


/*----------- 获取状态量和告警信息-1字节------功能码0x02-----------------*/
typedef enum _DSP_STATE_ALARM_ENUM{
  Mains_Input_Phase_Fault, //主电输入相序接反
  Mains_Abnormal,          //主电异常（干结点）
  Mains_Voltage_Abnormal,  //主电电压异常（高低压）
  Mains_A_Phase_Voltage_Abnormal,  //主电A相电压异常
  Mains_B_Phase_Voltage_Abnormal,  //主电B相电压异常
  Mains_C_Phase_Voltage_Abnormal,  //主电C相电压异常
  Mains_Frequency_Abnormal,  //主电频率异常
  Mains_Open,              //主电开路
  Mains_SCR_Fault,        //主电SCR故障
  Mains_Switch_Frequent,
  Mains_A_Phase_Overload,  //主电A相电流过载
  Mains_B_Phase_Overload,  //主电b相电流过载
  Mains_C_Phase_Overload,  //主电c相电流过载
  Ouput_Open,                 //输出开路
  Ouput_Over_Load_Shutdown,   //输出过载
  Ouput_Short_Circuit,        //输出短路
  Emergency_Fault,           //应急故障
  Emergency_Soft_Start_Fails,//应急软启失败
  Emergency_Over_Voltage,     //应急过压
  Emergency_Under_Voltage,     //应急欠压
  Emergency_SCR_Fault,        //应急SCR故障
  Emergency_Over_Courent,     //应急过流
  Emergency_Igbt_Over_Current,       //VCE
  Emergency_Over_Temp,        //应急过温
  EMERGENCY_UNBALANCE,          //应急电压不平衡
  Battery_Fuse_Fault,       //熔断器故障（电池）
  Battery_Contacts_Fault,   //电池接触器异常
  DC_Soft_Start_Fails,      //直流软启失败
  Battery_Connection_Abnormal,// 电池连接异常
  Battery_Over_Voltage,     //电池过压
  Battery_Under_Voltage,     //电池di压
  Battery_Over_Temp,        //电池过温
  Battery_OVER_CURR,    //电池过流
  Battery_UNDER_VOLTAGE_SHUTDOWN,    //电池di压关机
  Bus_Over_Voltage,     //母线过压
  Bus_Short_Circuit, //母线短路
  Charge_Open_Short_Circuit, //充电开短路
  Fan_Fault,         // 风机故障（所有风扇）
  Over_Temp_Fault,    //过温故障（干结点）
  DSP_System_Fault_falge, //DSP系统故障（干结点）
  Fire_Linkagel, //消防联动（干结点）
  DCDC_Fault,  //DCDC故障
  DCDC_Alarm, //DCDC告警
  Sampling_board_Error, //采样板错误
  Monitor_version_Error,      //监控版本错误
  Keep_1,                 //保留1
  Keep_2,                 //保留2
  User_Error,             //用户操作错误（监控）
  Strong_Start_Mode,      //强启模式
  Manual_Mode,            //手动模式
  Automatic_Mode,         //自动模式
  Simulation_Mains_Failure_Test,//模拟主电故障测试（实验按钮）
  Reset,              //
  Mains_SCR_Status,      //主电SCR状态
  Battery_Contactor_Status,//电池接触器状态
  Maintenance_Bypass_Status,
  Remote_Start,//远程开机
  Remote_Shutdown,//远程关机
  Inverter_Shutdown,
  Check_Battery_Number,// 59检查电池节数（监控确认）
  Keep_3 = 63,// 保留3
  End_0x02
}DSP_STATE_ALARM_ENUM;

/*----------- 获取模拟量数据2字节-----------04-------------*/
typedef enum _DSP_ANALOG_DATA_ENUM{
  Mains_A_Phase_Voltage,       //主电电压（*10）  2-------------------0
  Mains_B_Phase_Voltage,      //主电电压（*10）  2-------------------1
  Mains_C_Phase_Voltage,       //主电电压（*10）	 2
  Mains_A_Phase_Frequency,     //主电频率（*10）  2-------------------0
//  Mains_B_Phase_Frequency,      //主电频率（*10）  2-------------------1
//  Mains_C_Phase_Frequency,       //主电频率（*10）	 2
  Keep_4,
  Keep_5,
  Keep_6,
  Ouput_A_Phase_Voltage,       //输出电压（*10）  2-------------------0
  Ouput_B_Phase_Voltage,       //输出电压（*10）  2-------------------1
  Ouput_C_Phase_Voltage,       //输出电压（*10）	 2

  Ouput_A_Phase_Current,       //输出电流ph_a*10）  2-------------------0
  Ouput_B_Phase_Current,       //输出电流ph_B*10）  2-------------------1
  Ouput_C_Phase_Current,       //输出电流ph_c*10）	 2

  Ouput_A_Phase_Load_Per,       //A相输出负载百分比*10）  2-------------------0
  Ouput_B_Phase_Load_Per,       //b相输出负载百分比*10）  2-------------------1
  Ouput_C_Phase_Load_Per,       //b相输出负载百分比*10）	 2

  Ouput_A_Phase_Frequency,     //输出频率*10）  2-------------------0
  //Ouput_B_Phase_Frequency,     //输出频率*10）  2-------------------1
  //Ouput_C_Phase_Frequency,     //输出频率*10）
  Keep_17,
  Keep_18,
  Inv_Frequency,                   //应急频率

  Inv_A_Phase_Voltage,       //应急电压（*10）  2-------------------0
  Inv_B_Phase_Voltage,       //应急电压（*10）  2-------------------1
  Inv_C_Phase_Voltage,       //应急电压（*10）	 2

  Inv_A_Phase_Current,       //应急电流ph_a*10）  2-------------------0
  Inv_B_Phase_Current,       //应急电流ph_B*10）  2-------------------1
  Inv_C_Phase_Current,       //应急电流ph_c*10）	 2
  Bus_Voltage,                 //母线电压
  Battery_Charging_Voltage, //电池电压（充电电压）
  Battery_Current,          //电池电流
  Battery_Temp,             //电池温度
  Battery_Temp_Compen_Coefficient,//电池温度补偿系数
  Battery_Remaining_Time,      //电池剩余后备时间
  Battery_Remaining_Capacity,  //电池剩余容量%
  Keep_33,
  Ambient_Temperature,        //环境温度
  DSP_Software_Version,      //DSP软件版本
  Keep_36,
  Battery_Start,                //电池状态
  Battery_Contactor_Signal_status,// 电池接触器驱动信号状态

  EPS_Run_Status,                  //EPS运行状态 00H:待机 01H：主电模式， 03H：应急模式）
  Month_Check_Status,              //月检
  Year_Check_Status,               //年检
  Analog_Addr1_Value,           //地址1内容（*1）
  Analog_Addr2_Value,           //地址2内容（*1）
  Analog_Addr3_Value,           //地址3内容（*1）
  Analog_Addr4_Value,
  Charge_Com_Coefficient_Value,
  Temp_Com_Coefficient_Value,
  End_0x04
}DSP_ANALOG_DATA_ENUM;

/*----------- 调试反馈回来到值----------04-------------*/
typedef enum _DSP_DEBUG_PARAMETER_ENUM{
  Debug_DspVersion,	        //DSP模块版本号	2
  Debug_GridVolt_Califactor,	//主电电压系数，默认为1.0。放大1000倍存放。（900~1100）	2
  Debug_InvVolt_Califactor,	//应急电压系数，默认为1.0。放大1000倍存放。（900~1100）	2
  Debug_InvCurr_Califactor,	//应急电流系数，默认为1.0。放大1000倍存放。（900~1100）	2
  Debug_BatVolt_Califactor,	//电池电压系数，默认为1.0。放大1000倍存放。（900~1100）	2
  Debug_BatCurr_Califactor,	//电池电流系数，默认为1.0。放大1000倍存放。（900~1100）	2
  Debug_BusVolt_Califactor,	//母线电压系数，默认为1.0。放大1000倍存放。（900~1100）	2
  Debug_OutputVolt_Califactor,	//输出电压系数，默认为1.0。放大1000倍存放。（900~1100）	2
  Debug_OutputCurr_Califactor,	//输出电流系数，默认为1.0。放大1000倍存放。（900~1100）	2
  Debug_InvDcVolt_Califactor,   //直流分量系数，默认为1.0。放大1000倍存放。（900~1100）	2
  Debug_Parameter_keep = 15
}DSP_DEBUG_PARAMETER_ENUM;
typedef enum _SET_DSP_PARAMETE_ENUM{
  Equipment_Capacity_Enum, //0设备容量10~500
  Mains_Voltage_Top_limit_Enum,//01主电电压上限380-475
  Mains_Voltage_Low_limit_Enum,//02主电电压下限285-380
  Mains_Frequence_Top_limit_Enum,//03主电频率上限50~55
  Mains_Frequence_Low_limit_Enum,//04主电频率上限45~50
  Battery_Group_Enum,//05电池组数
  Battery_Capacity_Enum,      //06电池容量范围【24~200AH】，默认为100AH

  Battery_All_Section_Number_Enum, //07 电池节数, 范围【28~31】，默认28
  Battery_Discharge_Termination_V_Enum,       //08 电池放电终止电压，范围【8.5~12.5V】*10默认10.5V（下发105）
 // UINT32 Fire_Linkage_Mode;           //08消防联动模式1:应急输出，2:持续性输出，3非持续性输出 4 受控输出 默认持续性输出
  EPS_Work_Mode_Selection_Enum,  //09EPS工作模式选择
  Fast_Slow_Switch_Enum,//10快慢切
  Input_Control_1_Enum,//11
  Input_Control_2_Enum,//12
  Enable_Control_Enum,
  Keep14,
  Keep15,
  Monitoring_Software_version_Enum,     //16监控软件版本
  Manual_Open_Shut_Down_Enum,        //17应急手动开、关机
  Manual_Mode_Determine_Enum,         //18手动模式(监控确认)
  Month_Check_Value_Enum,              //19月检命令
  Year_Check_Value_Enum,               //20年检命令
  Clear_Fault_Enum,                   //21清除故障
  DSP_Restore_Factory_Set_Enum,      //22 DSP恢复出厂设置
  Check_Battery_Number_Right_Enum,//电池节数正常（监控确认）
  Debug_Variable_Address_1_Enum,    //24 调试变量地址1，0x0000-0xffff,默认0。
  Debug_Variable_Address_2_Enum,    //25 调试变量地址2，0x0000-0xffff,默认0
  Debug_Variable_Address_3_Enum,    //26 调试变量地址3，0x0000-0xffff,默认0
  Debug_Variable_Address_4_Enum,   //27 调试变量地址4，0x0000-0xffff,默认0
  Debug_Parameter_Value_1_Enum,     //28 调试参数值1，0~65535，默认0
  Debug_Parameter_Value_2_Enum,     //29 调试参数值2，0~65535，默认0
  Debug_Parameter_Value_3_Enum,     //30 调试参数值3，0~65535，默认0
  Debug_Parameter_Value_4_Enum,     //31 调试参数值4，0~65535，默认0
  Mains_Voltage_Calibration_Coefficient_Enum,//32 主电电压校准系数A，默认1.0,0.9~1.1放大1000倍存放。
  //Keep33,//33 主电电压校准系数A，默认1.0,0.9~1.1放大1000倍存放。
  //Keep34,//34主电电压校准系数A，默认1.0,0.9~1.1放大1000倍存放。

  Emergency_Voltage_Calibration_Coefficient_Enum,//33 应急A电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
  //Keep36,//36 应急B电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
  //Keep37,//37 应急C电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
  Emergency_Current_Calibration_Coefficient_Enum,//34 应急A电流校准系数.0,0.9~1.1放大1000倍存放。
  //Keep39,//39 应急B电流校准系数.0,0.9~1.1放大1000倍存放。
  //Keep40,//40 应急C电流校准系数.0,0.9~1.1放大1000倍存放。
  Battery_Voltage_Calibration_Coefficient_Enum,   //35 电池电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
  Battery_Current_Calibration_Coefficient_Enum,   //36 电池电流校准系数，默认1.0,0.9~1.1放大1000倍存放。
  Bus_Voltage_Calibration_Coefficient_Enum,     //37 母线电压校准系数.0,0.9~1.1放大1000倍存放。
  Ouput_Voltage_Calibration_Coefficient_Enum,   //38输出A电压校准系数.0,0.9~1.1放大1000倍存放。
 // UINT32 Ouput_Voltage_B_Calibration_Coefficient;   //45输出B电压校准系数.0,0.9~1.1放大1000倍存放。
 // UINT32 Ouput_Voltage_C_Calibration_Coefficient;   //46输出C电压校准系数.0,0.9~1.1放大1000倍存放。
  Ouput_Current_Calibration_Coefficient_Enum,   //39输出A电流校准系数.0,0.9~1.1放大1000倍存放。
 // UINT32 Ouput_Current_B_Calibration_Coefficient;   //48 输出B电流校准系数.0,0.9~1.1放大1000倍存放。
 // UINT32 Ouput_Current_C_Calibration_Coefficient;   //49 输出C电流校准系数.0,0.9~1.1放大1000倍存放。
  //Keep46,
  //Keep47,
  CODE_0_Enum=44,
  CODE_1_Enum=45,
  CODE_2_Enum=46,
  CODE_3_Enum=47,

  End_0x03=48       //预留，0~65535，默认0
}SET_DSP_PARAMETE_ENUM;

typedef enum _BACK_DATA02_ENUM{
  Bsystem_fault=0,//系统总故障
  Bdsp_commu_fault=1,//DSP通信异常
  Bio_commu_fault=2,//开关量通信异常
  Bats_commu_fault=3,//ATS通信异常
  Bbattery_commu_fault=4,//电池巡检仪通信异常开始
  Bdsp_fault=24,//DSP上传故障
  Bio_state=88,////开关量状态
  Bbattery_fault=128//DSP上传故障
}BACK_DATA02_ENUM;

typedef enum _BACK_DATA04_ENUM{
  Bdsp_data=0,
  Bemergency_data=19,
  Bbattery_group=48,
  Bbattery_total_num=49,
  Bbattery_voltage=56
}BACK_DATA04_ENUM;

/*----------- 下发参数给DSP配置参数定义--数据2字节-------03------------------*/
typedef struct _SET_DSP_PARAMETE{
 /* UINT32 Equipment_Capacity;  //0设备容量10~500
  UINT32 Mains_Voltage_Range; //01主电电压范围285~475 (380±25%)
  UINT32 Mains_Frequence_Range; //02主电频率范围~55(50±10%)
  UINT32 Battery_Capacity;      //03电池容量范围【24~200AH】，默认为100AH

  UINT32 Battery_All_Section_Number; //04 电池节数, 范围【28~31】，默认28
  UINT32 Battery_Discharge_Termination_V;       //05 电池放电终止电压，范围【8.5~12.5V】*10默认10.5V（下发105）
  UINT32 Fire_Linkage_Mode;                    //06 消防联动模式  1:应急输出，2:持续性输出，3非持续性输出 4 受控输出 默认持续性输出
  UINT32 Keep1;                                //07
  UINT32 Monitoring_Software_version;     //08监控软件版本
  UINT32 Manual_Open_Shut_Down;         //09应急手动开、关机
  UINT32 Manual_Mode_Determine;         //10手动模式(监控确认)
  UINT32 Month_Check_Value;              //11月检命令
  UINT32 Year_Check_Value;               //12年检命令
  UINT32 Clear_Fault;                   //13清除故障
  UINT32 DSP_Restore_Factory_Set;      // 14DSP恢复出厂设置
  UINT32 Keep2;                  //15
*/


  UINT32 Equipment_Capacity;  //0设备容量10~500
  UINT32 Mains_Voltage_Top_limit;//01主电电压上限380-475
  UINT32 Mains_Voltage_Low_limit;//02主电电压下限285-380
  UINT32 Mains_Frequence_Top_limit;//03主电频率上限50~55
  UINT32 Mains_Frequence_Low_limit;//04主电频率上限45~50
  UINT32 Battery_Group_Number;//05电池组数
  UINT32 Battery_Capacity;      //06电池容量范围【24~200AH】，默认为100AH

  UINT32 Battery_All_Section_Number; //07 电池节数, 范围【28~41】，默认28
  UINT32 Battery_Discharge_Termination_V;       //08 电池放电终止电压，范围【8.5~12.5V】*10默认10.5V（下发105）
 // UINT32 Fire_Linkage_Mode;           //0消防联动模式1:应急输出，2:持续性输出，3非持续性输出 4 受控输出 默认持续性输出
  UINT32 EPS_Work_Mode_Selection;  //09EPS工作模式选择
  UINT32 Fast_Slow_Switching;//10快慢切
  UINT32 Input_Control_1;//输入干接点1
  UINT32 Input_Control_2;//输入干接点2
  UINT32 EnableControl;//
  UINT32 Keep14[2];
  UINT32 Monitoring_Software_version;     //16监控软件版本
  UINT32 Manual_Open_Shut_Down;         //17应急手动开、关机
  UINT32 Manual_Mode_Determine;         //18手动模式(监控确认)
  UINT32 Month_Check_Value;              //19月检命令
  UINT32 Year_Check_Value;               //20年检命令
  UINT32 Clear_Fault;                   //21清除故障
  UINT32 DSP_Restore_Factory_Set;      //22 DSP恢复出厂设置
  UINT32 Keep23;
  UINT32 Debug_Variable_Address_1;    //24 调试变量地址1，0x0000-0xffff,默认0。
  UINT32 Debug_Variable_Address_2;    //25 调试变量地址2，0x0000-0xffff,默认0
  UINT32 Debug_Variable_Address_3;    //26 调试变量地址3，0x0000-0xffff,默认0
  UINT32 Debug_Variable_Address_4;    //27 调试变量地址4，0x0000-0xffff,默认0
  UINT32 Debug_Parameter_Value_1;     //28 调试参数值1，0~65535，默认0
  UINT32 Debug_Parameter_Value_2;     //29 调试参数值2，0~65535，默认0
  UINT32 Debug_Parameter_Value_3;     //30 调试参数值3，0~65535，默认0
  UINT32 Debug_Parameter_Value_4;     //31 调试参数值4，0~65535，默认0

  //UINT32 keep3[7];

  UINT32 Mains_Voltage_Calibration_Coefficient;//32 主电电压校准系数A，默认1.0,0.9~1.1放大1000倍存放。
  //UINT32 Mains_Voltage_B_Calibration_Coefficient;//33 主电电压校准系数A，默认1.0,0.9~1.1放大1000倍存放。
  //UINT32 Mains_Voltage_C_Calibration_Coefficient;//34主电电压校准系数A，默认1.0,0.9~1.1放大1000倍存放。
  //UINT32 keep33[2];
  UINT32 Emergency_Voltage_Calibration_Coefficient;//33 应急A电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
  //UINT32 Emergency_Voltage_B_Calibration_Coefficient;//36 应急B电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
  //UINT32 Emergency_Voltage_C_Calibration_Coefficient;//37 应急C电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
  //UINT32 keep36[2];
  UINT32 Emergency_Current_Calibration_Coefficient;//34 应急A电流校准系数.0,0.9~1.1放大1000倍存放。
  //UINT32 Emergency_Current_B_Calibration_Coefficient;//39 应急B电流校准系数.0,0.9~1.1放大1000倍存放。
  //UINT32 Emergency_Current_C_Calibration_Coefficient;//40 应急C电流校准系数.0,0.9~1.1放大1000倍存放。
  //UINT32 keep35[2];
  UINT32 Battery_Voltage_Calibration_Coefficient;   //35 电池电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
  UINT32 Battery_Current_Calibration_Coefficient;   //36 电池电流校准系数，默认1.0,0.9~1.1放大1000倍存放。
  UINT32 Bus_Voltage_Calibration_Coefficient;     // 37母线电压校准系数.0,0.9~1.1放大1000倍存放。
  UINT32 Ouput_Voltage_Calibration_Coefficient;   //38输出A电压校准系数.0,0.9~1.1放大1000倍存放。
 // UINT32 Ouput_Voltage_B_Calibration_Coefficient;   //45输出B电压校准系数.0,0.9~1.1放大1000倍存放。
 // UINT32 Ouput_Voltage_C_Calibration_Coefficient;   //46输出C电压校准系数.0,0.9~1.1放大1000倍存放。
  UINT32 Ouput_Current_Calibration_Coefficient;   //39 输出A电流校准系数.0,0.9~1.1放大1000倍存放。
 // UINT32 Ouput_Current_B_Calibration_Coefficient;   //48 输出B电流校准系数.0,0.9~1.1放大1000倍存放。
 // UINT32 Ouput_Current_C_Calibration_Coefficient;   //49 输出C电流校准系数.0,0.9~1.1放大1000倍存放。  
  UINT32 keep40[4];
  UINT32 Machine_Code[4];
  //UINT32 keep47;
  UINT32 end_0x03;       //预留，0~65535，默认0
}SET_DSP_PARAMETE;

typedef struct _DSP_DATA{
  UINT8  DSP_STATE_ALARM[End_0x02];           //获取状态量和告警信息----1字节
  UINT16 DSP_ANALOG_DATA[End_0x04];          //获取模拟量数据2字节
  UINT32 DSP_DEBUG_PARAMETER[Debug_Parameter_keep + 1]; //获取软件版本和校准系数
}DSP_DATA;

typedef struct _BATTERY_DATA{
  UINT32 Battery_Monomer_V[BATTERYMAXNUMEBR][BATT_MOD_CHANEL_24_V + 1]; // 电池单体电压
  UINT32 Battery_Monomer_V_Fault[BATTERYMAXNUMEBR][BATT_MOD_CHANEL_24_V + 1]; // 电池单体电压故障
}BATTERY_DATA;


typedef struct _EPS_PARAM{
  UINT32       Dsp_Communication_Fault;  //DSP通讯故障标志，0---正常 1---故障
  UINT32       Dsp_Data_Refresh[3];     //各个数据刷新标志
  UINT32       Dsp_Debug_Parame_flags;  //DSP获取版本信息，0---正常 1---故障
  UINT32       Battery_Data_Refresh[BATTERYMAXGROUP][BATTERYMAXNUMEBR];         //DSP各个数据刷新标志
  UINT32       Battery_Mod_Issued_flags[BATTERYMAXGROUP][BATTERYMAXNUMEBR]; //电池读取下面设备标志位
  UINT32       Battery_Communication_Fault[BATTERYMAXGROUP][BATTERYMAXNUMEBR];//电池通讯故障标志，0---正常 1---故障
  UINT32       Current_Alarm_List_Battery_Refresh;//当前电池告警界面刷新标志（清除当前电池警告链表）
  UINT32       Current_Alarm_List_Switch_Refresh;//当前Switch告警界面刷新标志（清除当前Switch警告链表）
  DSP_DATA     Dsp_Data_Param;
  BATTERY_DATA Battery_Data_Param[BATTERYMAXGROUP];
  UINT32       PC_Communication_Fault_1;
  UINT32       PC_Communication_Fault_2;
  UINT16       Battery_Tempature[BATTERYMAXGROUP][BATTERYMAXNUMEBR];//t1
}EPS_PARAM;

/***************ATS信息量*****************/
typedef struct _ATS_RUNDATA{
  UINT32       ATS_Communication_Fault;  //ATS通讯故障标志，0---正常 1---故障
  UINT32       ATS_Data_Refresh[3];     //各个数据刷新标志
  UINT32       ATS_Debug_Parame_flags;  //获取版本信息，0---正常 1---故障
}ATS_RUNDATA;

/***************客户设置参数*****************/
typedef struct _CUSTOMER_SETUP{
 // UINT32       EPS_Work_Mode_Selection;  //EPS工作模式选择
  UINT32       Distribution_Branches_Number;  //配电支路数
  UINT32       ATS_Configure_Flage;     //ATS配置标志--0--无ATS 1--有ATS
  UINT32       Distribution_Configure_Flage;//配电配置标志--0--无配电 1--有配电
  UINT32       battertyenable;
  UINT32       Distribution_Branches_modelnum;
  UINT32       keep[3]; //预留;
}CUSTOMER_SETUP;


typedef struct _SYSAPPENDPARAM{
  UINT32   Switch_Alarm_Set[64];//
  UINT32 keep[128];
}SYSAPPENDPARAM;

typedef struct _GLOBAVAL{
  EPS_PARAM                  Eps_Param_Data;
  NET_SETTING                NET_SetParam;
  User_System                User_Monitor_Screen_Set;
  Debugparameter_batterySet  Debug_Param_BattSet[BATTERYMAXGROUP][BATTERYMAXNUMEBR];
  BATTERY_PARAMER_SET        Battery_Param_Set[BATTERYMAXGROUP]; //
  BATTERY_GROUPPARAMER_SET   Battery_GroupParam_Set;
  SET_DSP_PARAMETE           Send_Dsp_Debug_Param_Set;          //下发参数给DSP配置参数定义--数据2字节--
  ATS_RUNDATA                Ats_Run_Data;
  UINT32                     Switch_Value[8];// 开关模块的值，因为只需要1个模块，32路。
  UINT32                     ATS_Value[8];// ATS模块的值

  SYSAPPENDPARAM  Sysappendparam;


  //
  UINT32                     Switch_Module_Fault[2];
  UINT32                     ATS_Module_Fault;
  CUSTOMER_SETUP             Curstomer_Param_Set;

  key_t                      uart_m4_msg_sent_id;
  key_t                      uart_m4_msg_recv_id;
  key_t                      uart_m4_set_msg_recv_id;
  key_t                      bk_send_msg_id;
  UINT32                     modbus_485_PC04[600];
  UINT32                     modbus_485_PC02[600];
  UINT32                     modbus_485_PC03[reg_03max_num];
  UINT32                     Switch_Set_Flag;
}GLOBAVAL;

extern GLOBAVAL	Globa;//声明外部全局变量



#endif // DEVICE_CONFIG_H
