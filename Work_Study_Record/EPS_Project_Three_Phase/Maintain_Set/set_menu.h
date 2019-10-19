#ifndef SETMENU_H
#define SETMENU_H

#include <QWidget>
#include"Share/Globalvar.h"

namespace Ui {
  class Setmenu;
}

class Setmenu : public QWidget
{
  Q_OBJECT

public:
  explicit Setmenu(QWidget *parent = 0);
  ~Setmenu();

  void OverloadUI();

private slots:
  void on_but_user_clicked();
  void on_but_defend_clicked();
  void on_but_debug_clicked();

//  void inputmenu(QString pwd);
signals:
//  void keyboard_show();
  void Changed_Current_View(int index,int Param,int Param_1,int SeNumber);
private:
  Ui::Setmenu *ui;
  int inputview;
};

#endif // SETMENU_H
