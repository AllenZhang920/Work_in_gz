#ifndef TESTDATA_DISPLAY_H
#define TESTDATA_DISPLAY_H

#include <QWidget>
#include "Share/Device_Config.h"



namespace Ui {
  class TestData_Display;
}

class TestData_Display : public QWidget
{
  Q_OBJECT
  
public:
  explicit TestData_Display(QWidget *parent = 0);
  ~TestData_Display();

  void data_fresh();
  
private:
  Ui::TestData_Display *ui;
};

#endif // TESTDATA_DISPLAY_H
