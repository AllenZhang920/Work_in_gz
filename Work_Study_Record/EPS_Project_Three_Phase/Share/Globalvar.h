#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include"Device_Config.h"
#include"Common_Function.h"
#include "zmessage.h"
#include <QFileInfo>

extern UINT16 SETDATA_Machine_Code[4];

typedef struct{
  UINT32   OverVolAlarm;
  UINT32   UnderVolAlarm;

}Sigle_Baterry_Voltage_Alarm;
extern    Sigle_Baterry_Voltage_Alarm G_Sigle_Baterry_Voltage_Alarm;

extern UINT32 thread_dsp;
extern UINT32 thread_m4_send;
extern UINT32 thread_m4_receive;
extern UINT32 thread_pc;
extern UINT32 thread_switch;
extern UINT32 thread_battery;
extern UINT32 thread_failure;
extern UINT32 thread_tcp;
extern UINT32 thread_usb;
extern UINT32 thread_interface_frame;

extern UINT8 test_data[20];

extern INT32 Fd_Uart_Arm_M4;
extern INT32 Fd_Uart_Arm_Pc;
extern INT32 ttyled_fd;

extern UINT32 Restar_SystemFlage  ;//重新启动系统标志
extern UINT32 ac_module_issued_flags ; // 读取下面设备标志位
extern UINT32 dc_module_issued_flags ; // 读取下面设备标志位

extern QList<CURRENT_ALARMFAULT_EVENT>current_alarm_eventlog ; // 当前告警链表
extern QList<HISTORY_ALARMFAULT_EVENT>history_alarm_eventlog ; // 历史告警链表

extern QList<CURRENT_ALARMFAULT_EVENT>current_fault_eventlog ; // 当前故障链表
extern QList<HISTORY_ALARMFAULT_EVENT>history_fault_eventlog ; // 历史故障链表

extern QList<OPERATION_WORK_RECORD>history_Operation_Record; // 存放当前历史信息记录
extern QList<OPERATION_WORK_RECORD>history_EpsWork_Record; // 操作记录以及工作记录


extern QReadWriteLock  DSP_State_Alarm_mutex;      //全局互斥锁---DSP状态量和告警信息
extern QReadWriteLock  DSP_Analog_Data_mutex;      //全局互斥锁PC --获取模拟量数据
extern QReadWriteLock  DSP_Fault_Data_mutex;      //全局互斥锁PC ---获取故障信息--逆变器发生故障前后的一些数据
extern QReadWriteLock  DSP_Debug_Parameter_mutex; //全局互斥锁PC --获取软件版本和校准系数
extern QReadWriteLock  Battery_mutex;
extern QReadWriteLock  Battery_Charge_Flag_mutex;

extern QReadWriteLock  modbus_pc04_mutex;    //全局互斥锁PC
extern QReadWriteLock  modbus_pc02_mutex;
extern QReadWriteLock  modbus_pc03_mutex;
extern QReadWriteLock  current_alarm_tablelocks_mutex;    //全局互斥锁
extern QReadWriteLock  history_alarm_tablelocks_mutex;    //全局互斥锁
extern QReadWriteLock  current_fault_tablelocks_mutex;    //全局互斥锁
extern QReadWriteLock  history_fault_tablelocks_mutex;    //全局互斥锁

extern QReadWriteLock  history_Operation_Record_mutex;    //全局互斥锁
extern QReadWriteLock  history_EpsWork_Record_mutex;    //全局互斥锁

extern QReadWriteLock  history_alarm_date_mutex;    //全局互斥锁
extern QReadWriteLock  battery_groupset_mutex;//


extern UINT32 System_Fault;//系统总故障
extern bool  screensaverflag;//下发屏保标志

extern UINT32 screen_brightness_count;//屏保时间计数
extern UINT32 messagebox_disappear_count;//消息对话框出现时间计数

extern UINT32  System_all_led;
extern UINT32  page_selectflag;
extern UINT32 USB_selection;
extern char devPath[30];
extern UINT32  resetMonitor;//
extern UINT32  Silent_Mode_flag;
extern UINT32  Reset_flag;
extern UINT32  Reset_count;//s计数
extern UINT32  Reset_time;//s延时
extern UINT32  Battery_Charge_Flag;
extern UINT32  Shutdown_Flag;
extern UINT32  Shutdown_Flag_flag;
extern UINT32  Load_Stye_Change;//负载模式改变，不改变为0，改变为EA_D：1，改变为FEPS：2

extern UINT32 messageFlag[10];
extern UINT32 Check_Battery_Number_finish;

extern int preCurIndex;
extern int Usr_Engir_Flag;
extern int Super_Egir_Flag;
extern int Mon_CheckResultShow_Flag;
extern int year_CheckResultShow_Flag;
extern int Battery_Checker_Num_Change_Flag;

/*------------------------------- 选择界面 -------------------------------*/
typedef enum _Select_Interface{
  MIAN_INTERFACE_VIEW = 0 ,     //0  main_interface 主界面
  EPS_MAIN_MENU_VIEW,     //1  eps_interface_infor EPS主菜单

  CURRENT_ALARM_QUERY_VIEW,     //2  current_alarm_query  当前告警


  EPS_RUN_INFORMATION_VIEW,     //3  eps_run_information  EPS运行信息
  BATTERYFORM_VIEW,             //4  batteryform  电池信息
  SYSTEMINFOR_VIEW,             //5  SystemInfor_View  系统信息
  SETMENU_VIEW,                 //6  setmenu 设置选项
  ALARM_QUERY_VIEW,             //7  alarmqueryform  历史查询
  DATAE_EPORT_IMPORT_VIEW,      //8  import_export_data  U盘数据导入导出

  USER_SYS_SET_VIEW,            //9  user_sysset_interface 监控屏设置
  USER_SET_VIEW,                //10  stem_node_set  客户设置
  MAINTAIN_SET,                 //11  maintain_set  维护设置

  MONITOR_SYSTEM_INFOR_VIEW,    //12  monitori_system_infor  产品信息
  ABOUTUS_VIEW,                 //13  AboutUs 关于我们

  TEST_DATA_DISPLAY,             //14  调试界面
  BRANCHALARMSET
}Select_Interface;


#endif // GLOBALVAR_H
