#ifndef EPS_RUN_INFORMATION_H
#define EPS_RUN_INFORMATION_H

#include <QWidget>
#include"Share/Globalvar.h"
#include"Share/param.h"
#define  MAXRAM_ERI 10

namespace Ui {
  class Eps_Run_Information;
}

class Eps_Run_Information : public QWidget
{
  Q_OBJECT
  
public:
  explicit Eps_Run_Information(QWidget *parent = 0);
  ~Eps_Run_Information();
  void Eps__Run_Refresh_Data();
  void tableview_init();

private slots:
  void on_upButton_clicked();

  void on_downButton_clicked();

private:
  Ui::Eps_Run_Information *ui;
  void Eps_Run_Tableview_1();
  void Eps_Run_Tableview_2();
  void Eps_Run_Tableview_3();
  void Eps_Run_Tableview_4();
  void Eps_Run_Tableview_5();
  void Eps_Run_Information_init();
  UINT32 pagenum;

};

#endif // EPS_RUN_INFORMATION_H
