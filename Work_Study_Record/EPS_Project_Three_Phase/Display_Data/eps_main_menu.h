#ifndef EPS_MAIN_MENU_H
#define EPS_MAIN_MENU_H

#include <QWidget>
#include"Share/Globalvar.h"
#define BATVOLCNT 12    //电池电压每页显示个数 **必需为偶数**
namespace Ui {
  class Eps_Main_Menu;
}

class Eps_Main_Menu : public QWidget
{
  Q_OBJECT
  
public:
  explicit Eps_Main_Menu(QWidget *parent = 0);
  ~Eps_Main_Menu();
  void eps_meanu_fresh();
signals:
 void Changed_Current_View(int index,int Param,int Param_1,int SeNumber);
    void keyboard_show();

private slots:
  void on_Sys_Parame_Set_clicked();

  void on_Sys_history_Data_clicked();

  void on_Sys_Update_Data_clicked();

  void on_Sys_Run_Information_clicked();

  void on_Sys_Battery_Data_clicked();

  void on_Sys_Information_clicked();

  void inputmenu(QString pwd);

private:
  Ui::Eps_Main_Menu *ui;
  int inputView;
};

#endif // EPS_MAIN_MENU_H
