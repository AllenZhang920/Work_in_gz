#include "mian_interface.h"
#include "ui_mian_interface.h"

UINT32 System_Fault = 0;

Mian_Interface::Mian_Interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mian_Interface)
{
    ui->setupUi(this);
    this->setGeometry(0,0,800,480);
    Olde_NO = 0;
    inputButton=0;
    Flow_Graph();

    //样式表
    ui->but_menu->setObjectName("styleButton");
    ui->but_log->setObjectName("styleButton");
    ui->Startup_Shutdown->setObjectName("styleButton");
    ui->Silent_Button->setObjectName("styleButton");

}

Mian_Interface::~Mian_Interface()
{
    delete ui;
}

void Mian_Interface::Flow_Graph()
{

   QPixmap pix;
   pix.load(":/images/2_03.png");
   ui->lb_line->setPixmap(pix);
}

/*
void Mian_Interface::Flow_Graph_show(UINT32 NO)
{
  if(Olde_NO == NO) return;
   movieline->stop();
   if(NO == 1){ //主电
     movieline->setFileName("://images/3_03.png");
   }else if(NO == 2){//应急
     movieline->setFileName("://images/4_03.png");
   }else if(NO == 3){//充电
     movieline->setFileName("://images/1_03.png");
   }else{
     movieline->setFileName("://images/2_03.png");
   }
   movieline->start();
   Olde_NO = NO;
}*/
void Mian_Interface::Flow_Graph_show(UINT32 NO)
{
   if(Olde_NO == NO) return;
   QPixmap pix;
   if(NO == 1){ //主电
     pix.load(":/images/3_03.png");
   }else if(NO == 2){//应急
     pix.load(":/images/4_03.png");
   }else if(NO == 3){//充电
     pix.load(":/images/1_03.png");
   }else{
     pix.load(":/images/2_03.png");
   }
  ui->lb_line->setPixmap(pix);
   Olde_NO = NO;
}
//*******************系统设置界面*********************
void Mian_Interface::Screen_Refresh_Data()
{
    QString qstr;
    static UINT32 timenumer=0;

    UINT32 ls_Value[6][3] = {{0}};

    INT16  ambient_temp_value=0;



    timenumer ++ ;
    if(System_Fault == 1){
        ui->label->setStyleSheet("color: rgb(255, 0, 0);");
    }else{
        ui->label->setStyleSheet("color: rgb(255, 255, 255);");
    }
    ui->label->setText(QString("%1").arg(System_state[Globa.User_Monitor_Screen_Set.System_language][System_Fault]));


    DSP_Analog_Data_mutex.lockForRead();
    ls_Value[0][0] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_A_Phase_Voltage];
    ls_Value[0][1] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_B_Phase_Voltage];
    ls_Value[0][2] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_C_Phase_Voltage];

    ls_Value[1][0] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_A_Phase_Frequency];
    ls_Value[1][1] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_A_Phase_Frequency];
    ls_Value[1][2] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_A_Phase_Frequency];

    ls_Value[2][0] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Voltage];
    ls_Value[2][1] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_B_Phase_Voltage];
    ls_Value[2][2] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_C_Phase_Voltage];

    ls_Value[3][0] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Frequency];
    ls_Value[3][1] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Frequency];
    ls_Value[3][2] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Frequency];

    ls_Value[4][0] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Current];
    ls_Value[4][1] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_B_Phase_Current];
    ls_Value[4][2] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_C_Phase_Current];

    ls_Value[5][0] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Load_Per];
    ls_Value[5][1] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_B_Phase_Load_Per];
    ls_Value[5][2] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_C_Phase_Load_Per];

    ambient_temp_value=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ambient_Temperature];
    DSP_Analog_Data_mutex.unlock();

    qstr.setNum((float) ls_Value[0][0]/10,'f',1);//主电(市电)电压
    ui->Main_Electric_Voltage_A->setText(qstr);
    qstr.setNum((float) ls_Value[0][1]/10,'f',1);//主电(市电)电压
    ui->Main_Electric_Voltage_B->setText(qstr);

    qstr.setNum((float) ls_Value[0][2]/10,'f',1);//主电(市电)电压
    ui->Main_Electric_Voltage_C->setText(qstr);

    qstr.setNum((float) ls_Value[1][0]/10,'f',1);//主电频率
    ui->Main_Electric_Frequency_A->setText(qstr);

    qstr.setNum((float) ls_Value[1][1]/10,'f',1);//主电频率
    ui->Main_Electric_Frequency_B->setText(qstr);

    qstr.setNum((float) ls_Value[1][2]/10,'f',1);//主电频率
    ui->Main_Electric_Frequency_C->setText(qstr);

    qstr.setNum((float) ls_Value[2][0]/10,'f',1);//输出电压
    ui->Output_Voltage_A->setText(qstr);

    qstr.setNum((float) ls_Value[2][1]/10,'f',1);//输出电压
    ui->Output_Voltage_B->setText(qstr);

    qstr.setNum((float) ls_Value[2][2]/10,'f',1);//输出电压
    ui->Output_Voltage_C->setText(qstr);

    qstr.setNum((float) ls_Value[3][0]/10,'f',1);//输出频率
    ui->Output_Frecquency_A->setText(qstr);

    qstr.setNum((float) ls_Value[3][1]/10,'f',1);//输出频率
    ui->Output_Frecquency_B->setText(qstr);

    qstr.setNum((float) ls_Value[3][2]/10,'f',1);//输出频率
    ui->Output_Frecquency_C->setText(qstr);

    qstr.setNum((float) ls_Value[4][0]/10,'f',1);//输出电流：
    ui->Output_Current_A->setText(qstr);

    qstr.setNum((float) ls_Value[4][1]/10,'f',1);//输出电流：
    ui->Output_Current_B->setText(qstr);

    qstr.setNum((float) ls_Value[4][2]/10,'f',1);//输出电流：
    ui->Output_Current_C->setText(qstr);

    qstr.setNum(ls_Value[5][0]);//输出百分比
    ui->Output_Load_Pre_A->setText(qstr);

    qstr.setNum(ls_Value[5][1]);//输出百分比
    ui->Output_Load_Pre_B->setText(qstr);

    qstr.setNum(ls_Value[5][2]);//输出百分比
    ui->Output_Load_Pre_C->setText(qstr);

    qstr.setNum((float) ambient_temp_value/10,'f',1);//EPS环境温度
    ui->temp->setText(qstr);



    current_alarm_tablelocks_mutex.lockForWrite();
    current_fault_tablelocks_mutex.lockForWrite();

    if(Reset_flag==0){
      if(current_fault_eventlog.count() > 0){
        if(timenumer%2 != 0){
          ui->label_falult->setText(tr("%1").arg(current_fault_eventlog.at(0).name));
        }else{
          ui->label_falult->setText(tr(""));
        }
     }else if(current_alarm_eventlog.count() > 0){
          if(timenumer%2 != 0){
            ui->label_falult->setText(tr("%1").arg(current_alarm_eventlog.at(0).name));
          }else{
            ui->label_falult->setText(tr(""));
          }
        }else{
        ui->label_falult->setText(tr(""));
       }
    }else {
        ui->label_falult->setText(tr(""));
      }
    current_fault_tablelocks_mutex.unlock();
    current_alarm_tablelocks_mutex.unlock();
/*    if(current_fault_eventlog.count() > 0){
      if(timenumer%2 != 0){
        ui->label->setText(tr("%1").arg(current_fault_eventlog.at(0).name));
      }else{
        ui->label->setText(tr(""));
      }
    }else{
      ui->label->setText(tr(""));
    }
*/

    if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status] == 1){//00:待机;01H：主电，02H:应急
          if(Battery_Charge_Flag == 1){
            Flow_Graph_show(3);
          }else{
            Flow_Graph_show(1);
          }
        }else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status] == 2){
          Flow_Graph_show(2);
        }else{
          Flow_Graph_show(0);
        }

   /* if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status] == 3){//03H：主电，04H:应急
      if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Keep_2] == 1){
        Flow_Graph_show(3);
      }else{
        Flow_Graph_show(1);
      }
    }else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status] == 4){
      Flow_Graph_show(2);
    }else{
      Flow_Graph_show(0);
    }*/
}

void Mian_Interface::on_but_menu_clicked()//主菜单
{
    emit Changed_Current_View(EPS_MAIN_MENU_VIEW, 0, 0, 0);
}

void Mian_Interface::on_but_log_clicked()//日志记录
{
    emit Changed_Current_View(CURRENT_ALARM_QUERY_VIEW, 0, 0, 0);
}

void Mian_Interface::on_Startup_Shutdown_clicked()
{ inputButton=0;
  if(!Globa.Battery_GroupParam_Set.userpassword_enable){
    emit keyboard_show();
  }else{
    //emit Changed_Current_View(BATTERYFORM_VIEW, 0, 0, 0);
    Startup_Shutdown  Startup_Shutdown_View;
    /*
    if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Inverter_Shutdown]==1)
    {
        Startup_Shutdown_View.apper_open();
    }else
    {
        Startup_Shutdown_View.apper_close();
    }*/
    Startup_Shutdown_View.exec();
  }

}

void Mian_Interface::on_Silent_Button_clicked()
{/* inputButton=1;
  if(!Globa.Battery_GroupParam_Set.userpassword_enable){
    emit keyboard_show();
  }else{*/
    //emit Changed_Current_View(BATTERYFORM_VIEW, 0, 0, 0);
    Silent_Mode_flag=1;
 /* }*/

}

void Mian_Interface::InputMenu(QString pwd)//静音
{
  if(!this->isHidden()){
/*    if(pwd == Globa.User_Monitor_Screen_Set.userpassword){
      Startup_Shutdown  Startup_Shutdown_View;
      Startup_Shutdown_View.exec();
    }else{
      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("提示"));
      msgBox.setText(tr("密码错误，请重新输入!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }

*/
      QDateTime time = QDateTime::currentDateTime();
      QString time_password = time.toString("yyMMdd");//设置系统
      switch(inputButton){
        //设置选项
        case 0:
          //用户密码显示界面
         if(pwd== QLatin1String(Globa.User_Monitor_Screen_Set.userpassword)){
            Usr_Engir_Flag = 1;
            Startup_Shutdown  Startup_Shutdown_View;
            /*
            if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Inverter_Shutdown]==1)
            {
                Startup_Shutdown_View.apper_open();
            }else
            {
                Startup_Shutdown_View.apper_close();
            }*/
            Startup_Shutdown_View.exec();
            return;
          }else if(pwd == time_password){//工程师密码显示界面
            Usr_Engir_Flag = 0;
            Startup_Shutdown  Startup_Shutdown_View;
            /*
            if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Inverter_Shutdown]==1)
            {
                Startup_Shutdown_View.apper_open();
            }else
            {
                Startup_Shutdown_View.apper_close();
            }*/
            Startup_Shutdown_View.exec();
            return;
          }
          break;

        case 1:
          //用户密码显示界面
         if(pwd== QLatin1String(Globa.User_Monitor_Screen_Set.userpassword)){
            Usr_Engir_Flag = 1;
            Silent_Mode_flag=1;
            return;
          }else if(pwd == time_password){//工程师密码显示界面
            Usr_Engir_Flag = 0;
            Silent_Mode_flag=1;
            return;
          }
          break;
          }
          {
           QMessageBox msgBox;
           msgBox.setWindowTitle(tr("提示"));
           msgBox.setText(tr("密码错误，请重新输入!"));
           msgBox.setIcon(QMessageBox::Information);
           msgBox.setStandardButtons(QMessageBox::Ok);
           msgBox.exec();
          }

      }
}

