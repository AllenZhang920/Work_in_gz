#ifndef REBOOT_H
#define REBOOT_H

#include <QDialog>

namespace Ui {
  class Reboot;
}

class Reboot : public QDialog
{
  Q_OBJECT
  
public:
  explicit Reboot(QWidget *parent = 0);
  ~Reboot();
  
private:
  Ui::Reboot *ui;
};

#endif // REBOOT_H
