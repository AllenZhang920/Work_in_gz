#ifndef MONITORI_SYSTEM_INFOR_H
#define MONITORI_SYSTEM_INFOR_H

#include <QWidget>
#include"Share/Macro_Definition.h"
#include "Share/Globalvar.h"

namespace Ui {
  class Product_Infor;
}

class Product_Infor : public QWidget
{
  Q_OBJECT
  
public:
  explicit Product_Infor(QWidget *parent = 0);
  ~Product_Infor();
  void data_fresh();
  
private:
  Ui::Product_Infor *ui;

};

#endif // MONITORI_SYSTEM_INFOR_H
