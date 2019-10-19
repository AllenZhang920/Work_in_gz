#ifndef MAINTAIN_SET_H
#define MAINTAIN_SET_H

#include <QWidget>
#include "Share/param.h"
#include "Display_Data/reboot.h"

namespace Ui {
  class Maintain_Set;
}

class Maintain_Set : public QWidget
{
  Q_OBJECT
  
public:
  explicit Maintain_Set(QWidget *parent = 0);
  ~Maintain_Set();

  void returnVal_show();
  void Adjustment_Dislpay_Data();
  void MSdataload();
private slots:
  void Maintain_Set_Parameter_Set_GetKeyValue(int key);
  void on_Debug_Parameter_1_selectionChanged();
  void on_Debug_Parameter_2_selectionChanged();
  void on_Debug_Parameter_3_selectionChanged();
  void on_Debug_Parameter_4_selectionChanged();


  void on_Variable_Address_1_selectionChanged();
  void on_Variable_Address_2_selectionChanged();
  void on_Variable_Address_3_selectionChanged();
  void on_Variable_Address_4_selectionChanged();

  void on_Batt_Vol_Calibration_Factor_selectionChanged();
  void on_Batt_Curr_Calibration_Factor_selectionChanged();
  void on_Bus_Vol_Calibration_Factor_selectionChanged();


  void on_password_enabled_combox_currentIndexChanged(int index);

  void on_btn_test_clicked();



  void on_Single_Batt_Discharge_End_V_selectionChanged();

  void on_Batt_Group_Number_selectionChanged();
  void on_Batt_Capcity_selectionChanged();


  void on_comboBox_Dry_contact_6_currentIndexChanged(int index);

  void on_comboBox_Dry_contact_7_currentIndexChanged(int index);

  void on_comboBox_Dry_contact_8_currentIndexChanged(int index);

  void on_comboBox_Dry_contact_9_currentIndexChanged(int index);

  void on_Battery_Group_Combox_currentIndexChanged(int index);

 // void on_btn_restore_2_clicked();

  //void on_Oupt_Curr_Calibration_Factor_A_selectionChanged();

  void on_Main_Vol_Calibration_Factor_selectionChanged();

 // void on_Main_Vol_Calibration_Factor_B_selectionChanged();

 // void on_Main_Vol_Calibration_Factor_C_selectionChanged();

  void on_Emer_Vol_Calibration_Factor_selectionChanged();

  //void on_Emer_Vol_Calibration_Factor_B_selectionChanged();

  //void on_Emer_Vol_Calibration_Factor_C_selectionChanged();

  void on_Emer_Curr_Calibration_Factor_selectionChanged();

 // void on_Emer_Curr_Calibration_Factor_B_selectionChanged();

 // void on_Emer_Curr_Calibration_Factor_C_selectionChanged();

  void on_Oupt_Vol_Calibration_Factor_selectionChanged();

  void on_Oupt_Curr_Calibration_Factor_selectionChanged();

 // void on_Oupt_Vol_Calibration_Factor_C_selectionChanged();

 //void on_Oupt_Curr_Calibration_Factor_B_selectionChanged();

//  void on_Oupt_Curr_Calibration_Factor_C_selectionChanged();

  void on_Sys_Battery_Number_selectionChanged();



  void on_Mains_Voltage_Low_limit_selectionChanged();

  void on_Mains_Voltage_Top_limit_selectionChanged();

  void on_Mains_Frequence_Top_limit_selectionChanged();

  void on_Mains_Frequence_Low_limit_selectionChanged();

  void on_comboBox_Dry_contact_11_currentIndexChanged(int index);

  void on_comboBox_Dry_contact_12_currentIndexChanged(int index);

 /* void on_comboBox_Dry_contact_13_currentIndexChanged(int index);

  void on_comboBox_Dry_contact_14_currentIndexChanged(int index);

*/

  void on_line_frameCapacitySet_selectionChanged();

 // void on_btn_restore_4_clicked();

 // void on_password_enabled_combox_2_currentIndexChanged(int index);

  void on_combox_eps_modole_currentIndexChanged(int index);

  void on_listWidget_currentRowChanged(int currentRow);

  void on_listWidget_2_currentRowChanged(int currentRow);

  void on_combox_Battery_detecting_currentIndexChanged(int index);

  void on_combox_eps_switch_modole_currentIndexChanged(int index);



  void on_combox_remote_control_1_currentIndexChanged(int index);

  void on_combox_remote_control_2_currentIndexChanged(int index);

  //void on_pushButton_clicked();

  void on_Single_Battery_Low_limit_selectionChanged();

  void on_Single_Battery_High_limit_selectionChanged();

  void on_lineEdit_selectionChanged();

  void on_lineEdit_2_selectionChanged();

  void on_lineEdit_3_selectionChanged();

  void on_lineEdit_4_selectionChanged();

  void on_Machine_Code_selectionChanged();

  void on_ComboxFreqChooseEnabel_currentIndexChanged(int index);

  void on_ComboxFreqChangeEnabel_currentIndexChanged(int index);

  void on_Comboxbatteryenable_currentIndexChanged(int index);

signals:
  void keyboard_show();
  void Changed_Current_View(int index, int Param, int Param_1, int SeNumber);
private:
  Ui::Maintain_Set *ui;
  QLineEdit *Maintain_Set_Parameter_SetEdit;
  UINT32 Maintain_Set_Parameter_Set_No;
  UINT32 Battery_Group_No;//当前第i（0-3）个电池组
  UINT32 batteryflag;//当前第i（0-3）个电池组
  UINT32 Batt_Number[BATTERYMAXGROUP];//每组的电池总节数
  void Maintain_Set_Parameter_Set_Init_2();
  void Maintain_Set_Parameter_Set_Init_1();

  void Maintain_Set_Parameter_Set_KeyValue();
  void addCombox_Item();
  void addComBoxView();
  void Interface_Initialize();
  void ChoosebatteryInterface(UINT32 number);
  void battery_number(UINT32 group);
  void add_Group_Combox_item(UINT32 group);
  bool Maintain_Set_Decimal_Point_falg;
  bool IsComParameterChanged;

  UINT32 total_battery;



};

#endif // DEBUG_PARAMETER_SET_H
