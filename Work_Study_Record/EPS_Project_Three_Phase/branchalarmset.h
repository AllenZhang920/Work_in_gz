#ifndef BRANCHALARMSET_H
#define BRANCHALARMSET_H

#include <QWidget>

namespace Ui {
  class BranchAlarmSet;
}

class BranchAlarmSet : public QWidget
{
  Q_OBJECT
  
public:
  explicit BranchAlarmSet(QWidget *parent = 0);
  ~BranchAlarmSet();
  void tableinit();
  void tablefresh();

private  slots:
  void changestatus(int row,int col);



  void on_upButton_clicked();

  void on_downButton_clicked();

private:
  Ui::BranchAlarmSet *ui;
  int pagenum;
};

#endif // BRANCHALARMSET_H
