#ifndef USER_SET_H
#define USER_SET_H

#include <QWidget>
#include "Share/param.h"
#include "Display_Data/reboot.h"

namespace Ui {
class User_Set;
}

class User_Set : public QWidget
{
  Q_OBJECT
  
public:
  explicit User_Set(QWidget *parent = 0);
  ~User_Set();
  void fresh();
signals:
  void keyboard_show();
  void passwordboard_show();
  
private slots:
  void on_Month_Detection_clicked();
  void on_Year_Detection_clicked();

  void User_Set_Parame_Set_GetKeyValue(int key);

//void on_comboBox_currentIndexChanged(int index);

  void on_btn_restore_clicked();

  void on_comboBox_ATS_currentIndexChanged(int index);

  void on_Distribution_branches_number_selectionChanged();

//  void on_btn_clear_fault_clicked();

  void on_comboBox_Distribution_currentIndexChanged(int index);

  void on_listWidget_currentRowChanged(int currentRow);
  void CheckMenu(QString pwd);
  void on_comboBox_protectenable_currentIndexChanged(int index);

  void on_comboBox_outputalarmenable_currentIndexChanged(int index);



  void on_btn_reset_clicked();



  void on_setAlarm_clicked();

signals:
  void Changed_Current_View(int index, int Param, int Param_1, int SeNumber);

private:
  Ui::User_Set *ui;
  QLineEdit *pUser_Set_LineEdit;
  bool IsComParameterChanged;
  bool User_Set_Parame_Decimal_Point_falg;

  void User_Set_Parame_Set_KeyValue();
  bool removeDirWithContent(const QString &dirName);
  void calibration_screen();
  UINT32 User_Set_Parame_Set_No;
  UINT32  checkButton;
};

#endif // USER_SET_H
