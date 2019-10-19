#ifndef SYSTEMINFOR_H
#define SYSTEMINFOR_H

#include <QWidget>
#include "Share/Globalvar.h"

namespace Ui {
  class SystemInfor;
}

class SystemInfor : public QWidget
{
  Q_OBJECT
  
public:
  explicit SystemInfor(QWidget *parent = 0);
  ~SystemInfor();

signals:
    void Changed_Current_View(int index, int Param, int Param_1, int SeNumber);
  
private slots:
  void on_btn_productinfor_clicked();
  void on_btn_aboutus_clicked();

private:
  Ui::SystemInfor *ui;
};

#endif // SYSTEMINFOR_H
