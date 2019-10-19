                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  #include "systeminfor.h"
#include "ui_systeminfor.h"

SystemInfor::SystemInfor(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SystemInfor)
{
  ui->setupUi(this);

  //样式表
  ui->btn_productinfor->setObjectName("styleButton");
  ui->btn_aboutus->setObjectName("styleButton");
}

SystemInfor::~SystemInfor()
{
  delete ui;
}

void SystemInfor::on_btn_productinfor_clicked()
{
    emit Changed_Current_View(MONITOR_SYSTEM_INFOR_VIEW, 0, 0, 0);
}

void SystemInfor::on_btn_aboutus_clicked()
{
    emit Changed_Current_View(ABOUTUS_VIEW, 0, 0, 0);
}
