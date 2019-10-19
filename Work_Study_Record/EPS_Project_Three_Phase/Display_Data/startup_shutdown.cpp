#include "startup_shutdown.h"
#include "ui_startup_shutdown.h"

Startup_Shutdown::Startup_Shutdown(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Startup_Shutdown)
{
  ui->setupUi(this);
 // this->setGeometry(145,336,515,128);
  this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
  ui->pushButton->setObjectName("styleButton");
  ui->pushButton_2->setObjectName("styleButton");
  ui->pushButton_3->setObjectName("styleButton");


}

Startup_Shutdown::~Startup_Shutdown()
{
  delete ui;
}

void Startup_Shutdown::on_pushButton_clicked()
{
  if(Issuedequipment_parameters_06(DSPSTARTADDR, Manual_Open_Shut_Down_Enum, 0xFF, 1)){
    //QMessageBox msgBox;
    //msgBox.setWindowTitle(tr("消息"));
    //msgBox.setText(tr("参数下发成功"));
    //msgBox.setIcon(QMessageBox::Information);
    //msgBox.setStandardButtons(QMessageBox::Ok);
   // Globa.Battery_GroupParam_Set.Battery_Capacity = Input_value;
    //System_Battery_GroupParam_Set_save();
   // msgBox.exec();
    //Shutdown_Flag=0;
    Shutdown_Flag_flag=0;
    Globa.Send_Dsp_Debug_Param_Set.Manual_Open_Shut_Down=0xFF;
    System_Set_DSP_Parame_save();
    Insert_Operation_Work_Record(1, "手动开机");
    /*modbus_pc03_mutex.lockForWrite();
    Globa.modbus_485_PC03[INVERTER_ON_COMMAND]=1;
    Globa.modbus_485_PC03[INVERTER_OFF_COMMAND]=0;
    modbus_pc03_mutex.unlock();*/
  }else{
      ZMessage msgBox;
      //QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      //msgBox.setIcon(QMessageBox::Information);
      //msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
  }
  reject();

}
void Startup_Shutdown::apper_open()
{
    //ui->pushButton->setVisible(true);
    //ui->pushButton_2->setVisible(false);
    //ui->pushButton_3->setVisible(false);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(true);
}

void Startup_Shutdown::apper_close()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
}
void Startup_Shutdown::on_pushButton_2_clicked()
{
    Shutdown_Flag_flag=1;
    reject();

}

void Startup_Shutdown::on_pushButton_3_clicked()
{
  reject();
}
