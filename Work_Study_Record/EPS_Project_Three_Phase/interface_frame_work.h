#ifndef INTERFACE_FRAME_WORK_H
#define INTERFACE_FRAME_WORK_H

#include <QWidget>
#include "Display_Data/reboot.h"

#include "Data_SR_Task/arm_to_m4_receive_data_thread.h"
#include "Data_SR_Task/arm_to_m4_send_data_thread.h"
#include "Data_SR_Task/arm_to_pc_send_receive_data_thread.h"
#include "Data_SR_Task/failure_event_alarm_process_thread.h"
#include "Data_SR_Task/tcp_modbus_thread.h"
#include "Data_SR_Task/battery_send_receive_thread.h"
#include "Data_SR_Task/arm_dsp_send_receive_data_thread.h"
#include "Data_SR_Task/ats_switch_send_receive_data.h"
#include "updata_modbus_thread.h"
#include"monitor_thread_runnig_thread.h"

#include "USB/import_export_data.h"
#include "USB/sUSB_Storage_Thread.h"

#include "KeyBoard/popup_keboarddialog.h"
#include "KeyBoard/passwordkeyboarddialog.h"

#include "User_Set/monitor_screen_set.h"
#include "Maintain_Set/user_set.h"
#include "Maintain_Set/set_menu.h"
#include "Maintain_Set/maintain_set.h"

#include "Display_Data/alarmqueryform.h"
#include "Display_Data/eps_main_menu.h"
#include "Display_Data/current_alarm_query.h"
#include "Display_Data/eps_run_information.h"
#include "Display_Data/product_infor.h"
#include "Display_Data/batteryform.h"
#include "Display_Data/mian_interface.h"
#include "Display_Data/systeminfor.h"
#include "Display_Data/aboutus.h"

#include "Display_Data/testdata_display.h"
#include "User_Set/calibration.h"
#include "branchalarmset.h"
namespace Ui {
  class Interface_Frame_Work;
}

class Interface_Frame_Work : public QWidget
{
  Q_OBJECT
  
public:
  explicit Interface_Frame_Work(QWidget *parent = 0);
  ~Interface_Frame_Work();
  void open_cal();


private:
  Ui::Interface_Frame_Work *ui;
  QStackedWidget *stackedWidget;
  QPushButton *GobackButton;
  QPushButton *MainButton;
  QPushButton  *EPS_INFOR;
  QPushButton  *pushButton_Battery;
  QLabel    *label_time;
  QLabel *lb_status;//通信状态
  QLabel *lb_off_battery;
  QLabel *lb_mode;//工作模式
  QLabel *lb_mode_value;//工作模式
  QLabel *lb_inverter_value;//逆变器状态
  UINT32  oldstate;
  UINT32  battery_off_flag;

  QString oldEPSStatus;

  bool key_effective[2];
  QTimer *main_timer;
  QTimer *display_data_timer;
  QTimer *reset_timer;
  QTime timer_up;

  Mian_Interface   *mian_interface_View;
  Eps_Main_Menu *Eps_Main_Menu_View;
  Current_Alarm_Query *Current_Alarm_Query_View;
  Setmenu             *setmenu;
  Monitor_Screen_Set *Monitor_Screen_Set_View;
  AlarmQueryForm             *AlarmQuery_View;
  User_Set              *User_Set_View;
  Import_Export_Data         *Import_Export_Data_View;
  Eps_Run_Information        *Eps_Run_Information_View;
  Product_Infor      *Product_Infor_View;
  BatteryForm                *BatteryForm_View;
  Maintain_Set        *Maintain_Set_View;
  SystemInfor        *SystemInfor_View;
  AboutUs            *AboutUs_View;

  TestData_Display *TestData_Display_View;

  BranchAlarmSet *Branchalarmset_View;

  POPUP_KEBOARDDialog     popup_keyboard_view;
  PasswordkeyboardDialog  password_keyboard;

  Arm_To_M4_Send_Data_Thread           Arm_To_M4_Send_Data_Task;
  Arm_To_M4_Receive_Data_Thread        Arm_To_M4_Receive_Data_Task;
  sUSB_Storage_Thread_thread           Usb_Read_Path_Task;
  Tcp_Modbus_Thread                    Tcp_Modbus_Rece_Send_Data_task;
  Arm_To_Pc_Send_Receive_Data_Thread   Arm_To_Pc_Send_Rece_Data_Task;
  Failure_Event_Alarm_Process_Thread   Failure_Event_Alarm_Process_Task;
  Updata_Modbus_Thread                 Updata_Modbus_Thread_Task;
  Battery_Send_Receive_Thread          Battery_Send_Receive_Data_Task;
  ARM_DSP_Send_Receive_Data_Thread     ARM_DSP_Send_Receive_Data_Task;
  Ats_Switch_Send_Receive_data         Ats_Switch_Send_Receive_data_Task;
  Monitor_Thread_Runnig_Thread        Monitor_Thread_Runnig_Thread_Task;
  void Create_Mian_Button();
  void Start_Task_Function();
  void Load_All_View_Initial();

  void time_display();
  void Read_history_alarm_Database_to_List();
  void Read_history_operationwork_records();
  void Read_history_fault_Database_to_List();

  void Set_Object_Icon(UINT32 state);
  void Alarm_Information_Process();  //弹出报警框的事件处理函数

  ZMessage msgBox_StrongStartingMode;
  QMessageBox msgBox_ManualMode;
  ZMessage msgBox_AutomaticMode;
  ZMessage msgBox_BatteryTestMode;
  ZMessage msgBox_monthcheck;
  ZMessage msgBox_yearcheck;
  QMessageBox msgBox_ShutDown;
  QMessageBox msgBox_Check_Battery_Number;
  QMessageBox msgBox_Notice_Battery_Off;
//  void Work_Mode_record();

public slots:
  void GobackClicked();//返回上一级界面按钮槽函数
  void MainClicked();//主界面按钮槽函数
  void Display_keyboard_function();
  void Display_keyboard();
  void Display_Current_View(int index,int Param,int Param_1,int SeNumber);
  void Display_Data_Function();
  void main_timer_function();
  void reset_timer_function();
  void reset_timer_start();
  void pushButton_Battery_clicked();
  void pushButton_EPS_INFOR_clicked();
signals:
 void Changed_Current_View(int index,int Param,int Param_1,int SeNumber);

protected:
 void keyPressEvent(QKeyEvent *e);
 void  keyReleaseEvent(QKeyEvent *e);

};

#endif // INTERFACE_FRAME_WORK_H
