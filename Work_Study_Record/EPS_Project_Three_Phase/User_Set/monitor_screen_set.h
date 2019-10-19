#ifndef USER_SYSSET_INTERFACE_H
#define USER_SYSSET_INTERFACE_H

#include <QWidget>
#include "KeyBoard/popup_keboarddialog.h"
#include <QtGui>
#include "Share/param.h"
#include "Share/Common_Function.h"
#include "User_Set/calibration.h"
#include "Display_Data/reboot.h"

namespace Ui {
  class Monitor_Screen_Set;
}

class Monitor_Screen_Set : public QWidget
{
  Q_OBJECT
  
public:
  explicit Monitor_Screen_Set(QWidget *parent = 0);
  ~Monitor_Screen_Set();
  void  page2Initialization();
  void  page1Initialization();
signals:
 void Changed_Current_View(int index,int Param,int Param_1,int SeNumber);

private slots:
  void SysGetKeyValue(int key);
  void on_AlarmBox_currentIndexChanged(int index);
  void on_lineEdit_bright_selectionChanged();
  void on_ScrProEdit_selectionChanged();
  void on_NewCodeEdit_selectionChanged();
  void on_NewCode2Edit_selectionChanged();
  void on_backgroundlineEdit_selectionChanged();
  void on_backgroundcomboBox_currentIndexChanged(int index);
  void on_backgroundcomboBox_1_currentIndexChanged(int index);
  void on_comboBox_getip_currentIndexChanged(int index);
  void on_lineEdit_ipaddress_selectionChanged();
  void on_lineEdit_subnetmask_selectionChanged();
  void on_lineEdit_defaultgateway_selectionChanged();
  void on_lineEdit_year_selectionChanged();
  void on_lineEdit_month_selectionChanged();
  void on_lineEdit_date_selectionChanged();
  void on_lineEdit_hour_selectionChanged();
  void on_lineEdit_minute_selectionChanged();
  void on_lineEdit_second_selectionChanged();
  void on_system_language_currentIndexChanged(int index);
//  void on_restore_factory_set_clicked();


  void on_screen_calibaration_clicked();

  void on_listWidget_currentRowChanged(int currentRow);

private:
  Ui::Monitor_Screen_Set *ui;
  bool IsComParameterChanged;

  QLineEdit *SystemSetEdit;
  UINT32 anjianflag1;
  UINT32 anjianflag2;
  UINT32 SystemSetflag;
  UINT32 dianfalg;
  CURRENTTIMESET  setcurrent_time;  //设置当前时间
/********************** 网络设置参数**********************/
  void  page3Initialization();
  void  page4Initialization();
  void  System_Setting_int();
  void  System_Setting_Keyvalue();
  void   addItem_ComboBox();
  void calibration_screen();
//  bool removeDirWithContent(const QString &dirName);
signals:
// void CurrentViewChanged(int index);
 void keyboard_show();
};

#endif // USER_SYSSET_INTERFACE_H
