#include "zmessage.h"
#include "ui_zmessage.h"

ZMessage::ZMessage(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ZMessage)
{
  ui->setupUi(this);
  ui->label->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
}

void ZMessage::setText(QString str)
{
  ui->label->setText(str);
//  ui->label->adjustSize();
 // ui->label->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
//ui->label->adjustSize();
  //ui->label->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
}
ZMessage::~ZMessage()
{
  delete ui;
}
