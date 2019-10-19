#ifndef STARTUP_SHUTDOWN_H
#define STARTUP_SHUTDOWN_H

#include <QDialog>
#include"Share/Globalvar.h"
#include"Share/param.h"

namespace Ui {
  class Startup_Shutdown;
}

class Startup_Shutdown : public QDialog
{
  Q_OBJECT
  
public:
  explicit Startup_Shutdown(QWidget *parent = 0);
  ~Startup_Shutdown();
    void apper_open();
    void apper_close();
private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();


private:
  Ui::Startup_Shutdown *ui;

};

#endif // STARTUP_SHUTDOWN_H
