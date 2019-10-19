#include "product_infor.h"
#include "ui_product_infor.h"
#include <QDebug>




char *letter[]={"","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};


Product_Infor::Product_Infor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Product_Infor)
{
    ui->setupUi(this);
    ui->product_model->setText(QString("%1KVA").arg(Globa.Send_Dsp_Debug_Param_Set.Equipment_Capacity));
    ui->mainSoftVersion->setText(QString("EPS_S_Control_SW V%1").arg(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[DSP_Software_Version],2,10,QChar('0')));
    ui->monitorSoftVersion->setText(QString("EPS_S_Monitor_SW V02.1"));
    //ui->factorydate->setText(QString("2017-4-28"));
}

Product_Infor::~Product_Infor()
{
    delete ui;
}

void Product_Infor::data_fresh()
{
  ui->product_model->setText(QString("%1KVA").arg(Globa.Send_Dsp_Debug_Param_Set.Equipment_Capacity));

  ui->mainSoftVersion->setText(QString("EPS_S_Control_SW V%1").arg(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[DSP_Software_Version],2,10,QChar('0')));

  QString str1=QString("%1").arg(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[DSP_Software_Version]%256,2,10,QChar('0'));
  QString str2="";
  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[DSP_Software_Version]/256<=26)
  {
      str2=QString("%1").arg(letter[Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[DSP_Software_Version]/256]);

  }
  ui->mainSoftVersion->setText(str1+str2);


  str1 = QString("%1").arg(SETDATA_Machine_Code[3],4,10,QChar('0'));
  str2 = QString("%1").arg(SETDATA_Machine_Code[2],4,10,QChar('0'));
  QString str3 = QString("%1").arg(SETDATA_Machine_Code[1],4,10,QChar('0'));
  QString str4 = QString("%1").arg(SETDATA_Machine_Code[0],1,10,QChar('0'));
  ui->Machine_Code->setText(str4 +str3+str2+str1);
}

