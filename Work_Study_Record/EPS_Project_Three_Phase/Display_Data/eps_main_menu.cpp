#include "eps_main_menu.h"

#include "ui_eps_main_menu.h"

Eps_Main_Menu::Eps_Main_Menu(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Eps_Main_Menu)
{
  ui->setupUi(this);

  //样式表
  ui->Sys_Run_Information->setObjectName("styleButton");
  ui->Sys_Information->setObjectName("styleButton");
  ui->Sys_Battery_Data->setObjectName("styleButton");

  ui->Sys_Parame_Set->setObjectName("styleButton");
  ui->Sys_history_Data->setObjectName("styleButton");
  ui->Sys_Update_Data->setObjectName("styleButton");
  inputView = 0;
}

Eps_Main_Menu::~Eps_Main_Menu()
{
  delete ui;
}

void Eps_Main_Menu::on_Sys_Parame_Set_clicked() //设置选项
{
  inputView = 1;
  if(!Globa.Battery_GroupParam_Set.userpassword_enable){
      emit keyboard_show();
    }else{
      emit Changed_Current_View(SETMENU_VIEW, 0, 0, 0);
    }

}

void Eps_Main_Menu::on_Sys_history_Data_clicked()//历史查询
{
  inputView = 2;
  if(!Globa.Battery_GroupParam_Set.userpassword_enable){
      emit keyboard_show();
    }else{
      emit Changed_Current_View(ALARM_QUERY_VIEW, 0, 0, 0);
    }

}

void Eps_Main_Menu::on_Sys_Update_Data_clicked()//数据下载
{
  inputView = 3;
  if(!Globa.Battery_GroupParam_Set.userpassword_enable){
      emit keyboard_show();
    }else{
      emit Changed_Current_View(DATAE_EPORT_IMPORT_VIEW, 0, 0, 0);
    }
}

void Eps_Main_Menu::on_Sys_Run_Information_clicked() //EPS运行信息
{
  emit Changed_Current_View(EPS_RUN_INFORMATION_VIEW, 0, 0, 0);

}

void Eps_Main_Menu::on_Sys_Battery_Data_clicked()//电池信息
{
  emit Changed_Current_View(BATTERYFORM_VIEW, 0, 0, 0);

}

void Eps_Main_Menu::on_Sys_Information_clicked()//产品信息
{
  emit Changed_Current_View(SYSTEMINFOR_VIEW, 0, 0, 0);
}

void Eps_Main_Menu::eps_meanu_fresh()
{Super_Egir_Flag=0;
  if(Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable == 0){
    ui->Sys_Battery_Data->setVisible(false);
    ui->Sys_Update_Data->setGeometry(160,310,200,70);
  }else{
    ui->Sys_Battery_Data->setVisible(true);
    ui->Sys_Update_Data->setGeometry(450,310,200,70);
  }
}




void Eps_Main_Menu::inputmenu(QString pwd)
{


  if(!this->isHidden())
    {
      QDateTime time = QDateTime::currentDateTime();
      QString time_password = time.toString("yyMMdd");//设置系统
      switch(inputView){
        //设置选项
        case 1:
          //用户密码显示界面
         if(pwd== QLatin1String(Globa.User_Monitor_Screen_Set.userpassword)){
            Usr_Engir_Flag = 1;
            emit Changed_Current_View(SETMENU_VIEW, 0, 0, 0);
            return;
          }else if(pwd == time_password){//工程师密码显示界面
            Usr_Engir_Flag = 0;
            emit Changed_Current_View(SETMENU_VIEW, 0, 0, 0);
            return;
           }
          break;
        case 2: //历史查询
          //用户密码显示界面
          if(pwd== QLatin1String(Globa.User_Monitor_Screen_Set.userpassword)){
            Usr_Engir_Flag = 1;
            emit Changed_Current_View(ALARM_QUERY_VIEW, 0, 0, 0);
            return;
          }else if(pwd == time_password){//工程师密码显示界面
            Usr_Engir_Flag = 0;
            emit Changed_Current_View(ALARM_QUERY_VIEW, 0, 0, 0);
            return;
          }
          break;
          //数据下载
        case 3:
          //用户密码显示界面
          if(pwd== QLatin1String(Globa.User_Monitor_Screen_Set.userpassword)){
              Usr_Engir_Flag = 1;
              Super_Egir_Flag=0;
              emit Changed_Current_View(DATAE_EPORT_IMPORT_VIEW, 0, 0, 0);
              return;
            }else if(pwd == time_password){//工程师密码显示界面
          //工程师密码显示界面
          //if(pwd== QLatin1String("234567")){
              Usr_Engir_Flag = 0;
              Super_Egir_Flag=0;
              emit Changed_Current_View(DATAE_EPORT_IMPORT_VIEW, 0, 0, 0);
              return;
            }
          else if(pwd == ("000000")){//super密码显示界面
                     Usr_Engir_Flag = 0;
                     Super_Egir_Flag=1;
                     emit Changed_Current_View(DATAE_EPORT_IMPORT_VIEW, 0, 0, 0);
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
