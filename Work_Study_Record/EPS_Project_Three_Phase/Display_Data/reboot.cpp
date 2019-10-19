#include "reboot.h"
#include "ui_reboot.h"

Reboot::Reboot(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Reboot)
{
  ui->setupUi(this);
  this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
  system("reboot");
}

Reboot::~Reboot()
{
  delete ui;
}
