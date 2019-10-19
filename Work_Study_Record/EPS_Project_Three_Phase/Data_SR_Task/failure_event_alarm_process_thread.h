#ifndef FAILURE_EVENT_ALARM_PROCESS_THREAD_H
#define FAILURE_EVENT_ALARM_PROCESS_THREAD_H

#include <QThread>
#include"Share/Globalvar.h"
#define checktime  48*3600
class Failure_Event_Alarm_Process_Thread: public QThread
{
  Q_OBJECT
public:
  explicit Failure_Event_Alarm_Process_Thread(QWidget *parent = 0);
  void run();

private:
  typedef enum _DSP_MOD_FAULT_ENUM{
      DSP_FAULT_COMMUNICATION_FAULT,          //0,DSP通讯中断
      DSP_FAULT_MAINS_INPUT_PHASE_REVER,     //1主电输入相序接反------0
      DSP_FAULT_MAINS_ABNORMAL,            //2主电异常（干结点）
      DSP_FAULT_MAINS_V_ABNORMAL,          //3主电电压异常（高低压）
      DSP_FAULT_MAINS_V_ABNORMAL_A,         //4主电A相电压异常------3

      DSP_FAULT_MAINS_V_ABNORMAL_B,         //5主电B相电压异常
      DSP_FAULT_MAINS_V_ABNORMAL_C,         //6主电C相电压异常
      DSP_FAULT_MAINS_Freq_Fault,          //7主电频率异常
      DSP_FAULT_MAINS_OPEN_Fault,         //8主电开路-----7

      DSP_FAULT_MAINS_SCR_Fault,        //9主电SCR故障
      DSP_FAULT_MAINS_Switch_Frequent,     //10切换频繁
      DSP_FAULT_OUTPUT_A_OVERLOAD, //11输出A相过流
      DSP_FAULT_OUTPUT_B_OVERLOAD, //12输出B相过流
      DSP_FAULT_OUTPUT_C_OVERLOAD,//13输出C相过流
      DSP_FAULT_OUTPUT_OPEN,    //14输出开路
      DSP_FAULT_OUTPUT_OVERLOAD_SHUTDOWN,       //15输出过载
      DSP_FAULT_OUTPUTt_SHORT_CIRCUIT,      //16输出短路
      DSP_FAULT_EMERGENCY_Fault,          //17应急故障（干结点）
      DSP_FAULT_EMERGENCY_SOFT_START_FAILS,//18应急软启失败
      DSP_FAULT_EMERGENCY_OVER_VOLTAGE,   //19应急过压
      DSP_FAULT_EMERGENCY_UNDER_VOLTAGE,   //20应急欠压
      DSP_FAULT_EMERGENCY_SCR_Fault,      //21应急SCR故障
      DSP_FAULT_EMERGENCY_OVER_CURRENT,  //22应急过流
      DSP_FAULT_EMERGENCY_VCE, //23应急通讯故障
      DSP_FAULT_EMERGENCY_OVER_TEMP,     //24应急过温-
      DSP_FAULT_EMERGENCY_UNBALANCE, //25应急电压不平衡
      DSP_FAULT_FUSE_FAILURE, //26熔断器故障（电池）
      DSP_FAULT_BATTERY_CONTACTS_ABNORMAL, //27电池接触器异常
      DSP_FAULT_DC_SOFT_START_FAILS, //28直流软启失败
      DSP_FAULT_BATTERY_CONNECTION_EXCEPTION, //29电池连接异常
      DSP_FAULT_BATTERY_OVER_VOLTAGE,      //30电池过压
      DSP_FAULT_BATTERY_UNDER_VOLTAGE,      //31电池低压
      DSP_FAULT_BATTERY_OVER_TEMP,           //电池过温
      DSP_FAULT_BATTERY_OVER_CURR,    //电池过流
      DSP_FAULT_BATTERY_UNDER_VOLTAGE_SHUTDOWN,//电池容量测试Battery capacity test
      DSP_FAULT_BUS_OVERVOLTAGE,//母线过压
      DSP_FAULT_BUS_SHORT_CIRCUIT,//母线短路
      DSP_FAULT_CHARGE_OPEN,//充电故障(开路、短路)
      DSP_FAULT_FAN_FAULT,     //风机故障（所有风扇）
      DSP_FAULT_OVER_TEMP,     // 过温故障（干结点）
      DSP_FAULT_SYSTEM_FAUL,//DSP系统故障（干结点）
      DSP_FAULT_FIRE_LINKAGE,//消防联动故障（干结点）
      DSP_FAULT_NO_FUCHONG,//
      DSP_FAULT_DCDC_ALARM,//DCDC告警
      DSP_FAULT_SAMPLING_BOARD_ERROR,//采样板错误
      DSP_FAULT_MONITOR_VERSION_ERROR, //监控版本错误
      DSP_FAULT_MonthCheckOk,      //月检不通过1
      DSP_FAULT_YearCheckOk        //年检不通过1
   }DSP_MOD_FAULT_ENUM;

typedef struct{
    time_t    Month_Self_Check_Start_Time;  //月检起始时间
    time_t    Month_Self_Check_Time ;       //月检当前运行时间
    time_t    Year_Self_Check_Start_Time;   //月检起始时间
    time_t    Year_Self_Check_Time ;        //月检当前运行时间
    UINT32    Month_Self_first_Check_Falg;  //年检标志
    UINT32    Month_Self_Check_Falg;        //年检标志
    UINT32    Year_Self_first_Check_Falg;   //年检标志
    UINT32    Year_Self_Check_Falg;         //年检标志
}SYS_SELF_CHECK;
  SYS_SELF_CHECK  Sys_Self_Check_Parame;

  UINT32  alarm_failure;
  QString failure_name;
  UINT32  EPS_Run_State;  //EPS运行状态0-待机，1-主电，2-应急
  UINT32  ls_EPS_Run_State;  //EPS运行状态0-待机，1-主电，2-应急
  UINT32  Last_DSP_Reset;//上一次的消音状态，0：不消音；1：消音
  UINT32  Dsp_Failure_happenflag[DSP_FAULT_YearCheckOk + 1];
  UINT32  Battery_Failure_happenflag[BATTERYMAXGROUP][BATTERYMAXNUMEBR][BATT_MOD_CHANEL_24_V + 2];
  UINT32  IO_Failure_happenflag[2];
  UINT32  Swith_Off_happenflag[64];
  UINT32  ATS_Failure_happenflag;
  UINT32 System_Total_fault, System_Total_Alarm;
  UINT32 Battery_Under_Voltage_Flag;
 // QTimer *check_48h_timer;
  bool check_48h_flag;
  void Save_Current_alarm_event(QString failure_event_name);
  void Delete_Current_alarm_event(QString failure_event_name);
  void Insert_History_alarm_eventlog(QString failure_event_name);

  void Save_Current_fault_event(QString failure_event_name);
  void Delete_Current_fault_event(QString failure_event_name);
  void Insert_History_fault_eventlog(QString failure_event_name);

  void Event_Alarm_Process_function();
  void Failure_Event_Alarm_Process();
  void IsBattery_Group_Change_funtion();
  void IsSwitch_Branchnumber_Change_funtion();
  void DSP_Failure_Event_Alarm_Function();
  void Switch_Failure_Event_Alarm_Function();
  void Dry_Contact_Function();
  void Self_Checking_Function();
  void Battery_Failure_Event_Alarm_Function(UINT32 number);
  time_t Read_systime();
  void Delete_history_databas();

signals:
  void Changed_Current_View(int index,int Param,int Param_1,int SeNumber);
  void Reset_Timer_Flag(int index);
public slots:
  void Check_48h_Function();
};

#endif // FAILURE_EVENT_ALARM_PROCESS_THREAD_H
