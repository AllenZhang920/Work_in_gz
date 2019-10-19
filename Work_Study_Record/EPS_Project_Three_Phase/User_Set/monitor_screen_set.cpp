#include "monitor_screen_set.h"
#include "ui_monitor_screen_set.h"
#include <QtNetwork>
Monitor_Screen_Set::Monitor_Screen_Set(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Monitor_Screen_Set)
{
  ui->setupUi(this);
  this->setWindowFlags(Qt::FramelessWindowHint); //也是去掉标题栏的语
  this->setGeometry(0,0,800,480);

  dianfalg = 0;
  anjianflag1 = 0;
  anjianflag2 = 0;
  SystemSetflag = 0;
  IsComParameterChanged = false;

  ui->system_language->setView(new QListView());
  ui->AlarmBox->setView(new QListView());
  ui->backgroundcomboBox->setView(new QListView());
  ui->backgroundcomboBox_1->setView(new QListView());
  ui->comboBox_getip->setView(new QListView());

  ui->screen_calibaration->setObjectName("styleButton");
  //ui->restore_factory_set->setObjectName("styleButton");

  addItem_ComboBox();
  System_Setting_int();
  IsComParameterChanged =true;

  ui->background_port->setText(QString("COM7/COM8/LAN"));

  QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    //获取所有网络接口的列表
    foreach(QNetworkInterface interface,list)
    {
      if(interface.name().startsWith("eth0")){
        ui->lineEdit_mac->setText(QString(interface.hardwareAddress()));
        break;
      }
    }
   connect(ui->listWidget,SIGNAL(currentRowChanged(int)),ui->stackedWidget, SLOT(setCurrentIndex(int)));

   ui->listWidget->setSpacing(10);
   ui->listWidget->setCurrentRow(0);
}

Monitor_Screen_Set::~Monitor_Screen_Set()
{
  delete ui;
}



void Monitor_Screen_Set::addItem_ComboBox()
{
    ui->backgroundcomboBox->clear();
    ui->backgroundcomboBox_1->clear();
    ui->comboBox_getip->clear();
    ui->AlarmBox->clear();
    ui->system_language->clear();


    ui->system_language->addItem(tr("中文"));
    ui->system_language->addItem(tr("英文"));


    ui->AlarmBox->addItem(tr("静音"));
    ui->AlarmBox->addItem(tr("鸣叫"));
    ui->backgroundcomboBox->addItem("4800");
    ui->backgroundcomboBox->addItem("9600");
    ui->backgroundcomboBox->addItem("19200");

    ui->backgroundcomboBox_1->addItem(tr("奇校验"));
    ui->backgroundcomboBox_1->addItem(tr("偶校验"));
    ui->backgroundcomboBox_1->addItem(tr("无校验"));
    ui->comboBox_getip->addItem(tr("自动(DHCP)"));
    ui->comboBox_getip->addItem(tr("手动"));

    ui->system_language->setEnabled(false);

}

void Monitor_Screen_Set::page1Initialization()
{
    time_t systime=0;
    struct tm *ptm;
    systime= time(NULL);        //获得系统的当前时间
    ptm=localtime(&systime);

    setcurrent_time.year=ptm->tm_year +1900;
    setcurrent_time.month=ptm->tm_mon+1;
    setcurrent_time.date=ptm->tm_mday;
    setcurrent_time.hours=ptm->tm_hour;
    setcurrent_time.minutes=ptm->tm_min;
    setcurrent_time.seconds=ptm->tm_sec;

    ui->lineEdit_year->setText(QString::number(setcurrent_time.year));
    ui->lineEdit_month->setText(QString::number(setcurrent_time.month));
    ui->lineEdit_date->setText(QString::number(setcurrent_time.date));
    ui->lineEdit_hour->setText(QString::number(setcurrent_time.hours));
    ui->lineEdit_minute->setText(QString::number(setcurrent_time.minutes));
    ui->lineEdit_second->setText(QString::number(setcurrent_time.seconds));
}

void Monitor_Screen_Set::page2Initialization()
{
    ui->system_language->setCurrentIndex(Globa.User_Monitor_Screen_Set.System_language);
    ui->AlarmBox->setCurrentIndex(Globa.User_Monitor_Screen_Set.AlarmSound);
    ui->ScrProEdit->setText(QString("%1").arg(Globa.User_Monitor_Screen_Set.Screensavertime));
    ui->lineEdit_bright->setText(QString("%1").arg(Globa.User_Monitor_Screen_Set.screen_brightness));
    ui->NewCodeEdit->setText(QString(Globa.User_Monitor_Screen_Set.userpassword));
    ui->NewCode2Edit->setText(QString(Globa.User_Monitor_Screen_Set.userpassword));

}

void Monitor_Screen_Set::page3Initialization()
{
    ui->backgroundlineEdit->setText(QString("%1").arg(Globa.User_Monitor_Screen_Set.backgroundAddress));//后台地址
    ui->backgroundcomboBox->setCurrentIndex(Globa.User_Monitor_Screen_Set.Baudratechoice);
    ui->backgroundcomboBox_1->setCurrentIndex(Globa.User_Monitor_Screen_Set.paritychoice);
}

void Monitor_Screen_Set::page4Initialization()
{
  /*sGet_IP_Address(Globa.NET_SetParam.IP_address);
  sGet_Net_Mask(Globa.NET_SetParam.msk_address);
  sGet_GateWay(Globa.NET_SetParam.gate_address);*/
  ui->lineEdit_ipaddress->setText(QString("%1").arg(Globa.NET_SetParam.IP_address));
  ui->lineEdit_subnetmask->setText(QString("%1").arg(Globa.NET_SetParam.msk_address));
  ui->lineEdit_defaultgateway->setText(QString("%1").arg(Globa.NET_SetParam.gate_address));
  ui->comboBox_getip->setCurrentIndex(Globa.NET_SetParam.assign_method);
  if(Globa.NET_SetParam.assign_method == 0){
    ui->lineEdit_ipaddress->setEnabled(false);
    ui->lineEdit_subnetmask->setEnabled(false);
    ui->lineEdit_defaultgateway->setEnabled(false);
  }else{
    ui->lineEdit_ipaddress->setEnabled(true);
    ui->lineEdit_subnetmask->setEnabled(true);
    ui->lineEdit_defaultgateway->setEnabled(true);
  }

}

void Monitor_Screen_Set::System_Setting_int()
{
    page1Initialization();
    page2Initialization();
    page3Initialization();
    page4Initialization();
}

void Monitor_Screen_Set:: SysGetKeyValue(int key)
{
  if(!this->isHidden()){
    if(key == 10){
      SystemSetEdit->setText("");
     }else if(key == 11){
       System_Setting_int();
    }else if(key == 12){
      System_Setting_Keyvalue();
    }else if(key<10) {
      SystemSetEdit->setText(SystemSetEdit->text()+QString("%1").arg(key));
   }else if(key ==13){
     if(dianfalg == 1){
       SystemSetEdit->setText(SystemSetEdit->text()+QString("."));
     }
   }else if(key == 14){
     SystemSetEdit->setText(SystemSetEdit->text().mid(0,SystemSetEdit->text().count()-1));
   }
  }
}

void Monitor_Screen_Set::System_Setting_Keyvalue()
{
   char time[80];
   UINT32  integer_value = 0;

   switch(SystemSetflag){
     //时间设置
     case 1:
       integer_value = SystemSetEdit->text().toUInt();
       if((integer_value > 2034)||(integer_value < 2012)){
       // QMessageBox::information(NULL,tr("提示"),tr("请输入2012-2052之间的数据!"),QMessageBox::Ok);
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("消息"));
         msgBox.setText(tr("请输入2012-2034之间的数据!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
       }else{
         setcurrent_time.year = integer_value;
         sprintf(time, "date -s %02d%02d%02d%02d%04d.%d", setcurrent_time.month, setcurrent_time.date, setcurrent_time.hours, setcurrent_time.minutes, setcurrent_time.year, setcurrent_time.seconds);
         system(time);
         system("hwclock --systohc --utc");
         Insert_Operation_Work_Record(1, "修改时间-年");
         LOG("Modify system time--Year");
       }
       SystemSetEdit->setText(QString::number(setcurrent_time.year));
       break;
     case 2:
       integer_value = SystemSetEdit->text().toUInt();

       if((integer_value > 12)||(integer_value < 1)){
        // QMessageBox::information(NULL,tr("提示"),tr("请输入1-12之间的数据!"),QMessageBox::Ok);
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("消息"));
         msgBox.setText(tr("请输入1-12之间的数据!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
       }else{
         setcurrent_time.month = integer_value;
         sprintf(time, "date -s %02d%02d%02d%02d%04d.%d", setcurrent_time.month, setcurrent_time.date, setcurrent_time.hours, setcurrent_time.minutes, setcurrent_time.year, setcurrent_time.seconds);
         system(time);
         system("hwclock --systohc --utc");
         Insert_Operation_Work_Record(1, "修改时间-月");
         LOG("Modify system time--Month");
       }
       SystemSetEdit->setText(QString("%1").arg(setcurrent_time.month));
       break;
     case 3:
       integer_value = SystemSetEdit->text().toUInt();

       if((integer_value >31)||(integer_value < 1)){
        // QMessageBox::information(NULL,tr("提示"),tr("请输入1-31之间的数据!"),QMessageBox::Ok);
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("消息"));
         msgBox.setText(tr("请输入1-31之间的数据!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
       }else{
         setcurrent_time.date = integer_value;
         sprintf(time, "date -s %02d%02d%02d%02d%04d.%d", setcurrent_time.month, setcurrent_time.date, setcurrent_time.hours, setcurrent_time.minutes, setcurrent_time.year, setcurrent_time.seconds);
         system(time);
         system("hwclock --systohc --utc");
         Insert_Operation_Work_Record(1, "修改时间-日");
         LOG("Modify system time--Day");
       }
       SystemSetEdit->setText(QString("%1").arg(setcurrent_time.date));
       break;
     case 4:
       integer_value = SystemSetEdit->text().toUInt();

       if(integer_value > 23){
        // QMessageBox::information(NULL,tr("提示"),tr("请输入0-23之间的数据!"),QMessageBox::Ok);
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));
         msgBox.setText(tr("请输入0-23之间的数据!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
       }else{
         setcurrent_time.hours = integer_value;
         sprintf(time, "date -s %02d%02d%02d%02d%04d.%d", setcurrent_time.month, setcurrent_time.date, setcurrent_time.hours, setcurrent_time.minutes, setcurrent_time.year, setcurrent_time.seconds);
         system(time);
         system("hwclock --systohc --utc");
         Insert_Operation_Work_Record(1, "修改时间-时");
         LOG("Modify system time--Hour");
       }
       SystemSetEdit->setText(QString("%1").arg(setcurrent_time.hours));
       break;
     case 5:
       integer_value = SystemSetEdit->text().toUInt();

       if(integer_value > 59){
         //QMessageBox::information(NULL,tr("提示"),tr("请输入0-59之间的数据!"),QMessageBox::Ok);
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));
         msgBox.setText(tr("请输入0-59之间的数据!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
       }else{
         setcurrent_time.minutes = integer_value;
         sprintf(time, "date -s %02d%02d%02d%02d%04d.%d", setcurrent_time.month, setcurrent_time.date, setcurrent_time.hours, setcurrent_time.minutes, setcurrent_time.year, setcurrent_time.seconds);
         system(time);
         system("hwclock --systohc --utc");
         Insert_Operation_Work_Record(1, "修改时间-分");
         LOG("Modify system time--Minute");
       }
       SystemSetEdit->setText(QString("%1").arg(setcurrent_time.minutes));
       break;
     case 6:
       integer_value = SystemSetEdit->text().toUInt();

       if(integer_value > 59){
       //QMessageBox::information(NULL,tr("提示"),tr("请输入0-59之间的数据!"),QMessageBox::Ok);
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));
         msgBox.setText(tr("请输入0-59之间的数据!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
       }else{
         setcurrent_time.seconds = integer_value;
         sprintf(time, "date -s %02d%02d%02d%02d%04d.%d", setcurrent_time.month, setcurrent_time.date, setcurrent_time.hours, setcurrent_time.minutes, setcurrent_time.year, setcurrent_time.seconds);
         system(time);
         system("hwclock --systohc --utc");
         Insert_Operation_Work_Record(1, "修改时间-秒");
         LOG("Modify system time--Second");
       }
       SystemSetEdit->setText(QString("%1").arg(setcurrent_time.seconds));
       break;
     case 7://屏幕亮度
       integer_value = ui->lineEdit_bright->text().toUInt();

       if((integer_value > 100)||(integer_value < 10)){
         //QMessageBox::information(NULL,tr("提示"),tr("请输入10-100之间的数据!"),QMessageBox::Ok);
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));
         msgBox.setText(tr("请输入10-100之间的数据!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
       }else{
         Globa.User_Monitor_Screen_Set.screen_brightness = integer_value;
         BK_CON_STRUCT msg;
         msg.msg_type = 1;
         msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
         msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
         System_UseSystem_save();
         Insert_Operation_Work_Record(1, "修改监控屏屏幕亮度");
       }
       ui->lineEdit_bright->setText(QString("%1").arg(Globa.User_Monitor_Screen_Set.screen_brightness));
       break;
     case 8: // 屏保时间
       integer_value = SystemSetEdit->text().toUInt();

       if((integer_value > 30)||(integer_value < 1)){
       //  QMessageBox::information(NULL,tr("提示"),tr("请输入1-30之间的数据!"),QMessageBox::Ok);
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));
         msgBox.setText(tr("请输入1-30之间的数据!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
       }else{
         Globa.User_Monitor_Screen_Set.Screensavertime = integer_value;
         System_UseSystem_save();
         Insert_Operation_Work_Record(1, "修改监控屏屏保时间");
       }
       SystemSetEdit->setText(QString("%1").arg(Globa.User_Monitor_Screen_Set.Screensavertime));
       break;
     case 9:
       if(ui->NewCodeEdit->text().isEmpty()||ui->NewCodeEdit->text().count() != 6){
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));
         msgBox.setText(tr("请输入6位数据!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
         ui->NewCodeEdit->setText(QLatin1String(Globa.User_Monitor_Screen_Set.userpassword));
       }else{
         anjianflag1 = 1;
         if(anjianflag1 + anjianflag2 == 2){
           if(ui->NewCode2Edit->text() == ui->NewCodeEdit->text()){
             anjianflag1 = anjianflag2 = 0;
             QByteArray ba = ui->NewCodeEdit->text().toLatin1();
             strcpy(Globa.User_Monitor_Screen_Set.userpassword, ba.data());
             System_UseSystem_save();
             QMessageBox msgBox;
             msgBox.setWindowTitle(tr("提示"));
             msgBox.setText(tr("密码设置成功!"));
             msgBox.setIcon(QMessageBox::Information);
             msgBox.setStandardButtons(QMessageBox::Ok);
             msgBox.exec();
             Insert_Operation_Work_Record(1, "修改用户密码");
           }else{
             anjianflag1 = anjianflag2 = 0;
             QMessageBox msgBox;
             msgBox.setWindowTitle(tr("提示"));
             msgBox.setText(tr("两次输入密码不一致!"));
             msgBox.setIcon(QMessageBox::Information);
             msgBox.setStandardButtons(QMessageBox::Ok);
             msgBox.exec();
           }
           ui->NewCodeEdit->setText(QLatin1String(Globa.User_Monitor_Screen_Set.userpassword));
           ui->NewCode2Edit->setText(QLatin1String(Globa.User_Monitor_Screen_Set.userpassword));
         }
       }
       break;
     case 10:
       if(ui->NewCode2Edit->text().isEmpty()||ui->NewCode2Edit->text().count()!=6) {
        // QMessageBox::information(NULL,tr("提示"),tr("请输入6位数密码!"),QMessageBox::Ok);
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));

         msgBox.setText(tr("请输入6位数据!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
         ui->NewCode2Edit->setText(QLatin1String(Globa.User_Monitor_Screen_Set.userpassword));
       }else{
         anjianflag2 = 1;
         if(anjianflag1+ anjianflag2 == 2){
           if(ui->NewCode2Edit->text() == ui->NewCodeEdit->text()){
             anjianflag1 = anjianflag2 = 0;
             QByteArray ba = ui->NewCodeEdit->text().toLatin1();
             strcpy(Globa.User_Monitor_Screen_Set.userpassword, ba.data());
             System_UseSystem_save();
             QMessageBox msgBox;
             msgBox.setWindowTitle(tr("提示"));
             msgBox.setText(tr("密码设置成功!"));
             msgBox.setIcon(QMessageBox::Information);
             msgBox.setStandardButtons(QMessageBox::Ok);
             msgBox.exec();
             Insert_Operation_Work_Record(1, "修改用户密码");
           }else{
             anjianflag1 = anjianflag2 = 0;
             QMessageBox msgBox;
             msgBox.setWindowTitle(tr("提示"));
             msgBox.setText(tr("两次输入密码不一致!"));
             msgBox.setIcon(QMessageBox::Information);
             msgBox.setStandardButtons(QMessageBox::Ok);
             msgBox.exec();
           }
           ui->NewCodeEdit->setText(QLatin1String(Globa.User_Monitor_Screen_Set.userpassword));
           ui->NewCode2Edit->setText(QLatin1String(Globa.User_Monitor_Screen_Set.userpassword));
         }
       }
       break;
     case 11:
       if(ui->lineEdit_ipaddress->text().isEmpty()){
        // QMessageBox::warning(NULL,tr("警告"), tr("IP地址不能为空!"));
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));
         msgBox.setText(tr("IP地址不能为空!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
       }else{
         QRegExp rx("^((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$");
         if(!rx.exactMatch(ui->lineEdit_ipaddress->text())){
         //  QMessageBox::information(NULL,tr("提示"),tr("IP地址错误!"),QMessageBox::Ok);
           QMessageBox msgBox;
           msgBox.setWindowTitle(tr("提示"));
           msgBox.setText(tr("IP地址错误!"));
           msgBox.setIcon(QMessageBox::Information);
           msgBox.setStandardButtons(QMessageBox::Ok);
           msgBox.exec();
         }else{
           QByteArray ba = ui->lineEdit_ipaddress->text().toLatin1();
           strcpy(Globa.NET_SetParam.IP_address, ba.data());
           System_Net_Set_Save();
           Insert_Operation_Work_Record(1, "修改本机IP地址");
         }
       }
       ui->lineEdit_ipaddress->setText(QString("%1").arg(Globa.NET_SetParam.IP_address));
       break;
     case 12:
       if(ui->lineEdit_subnetmask->text().isEmpty()){
        // QMessageBox::warning(NULL,tr("警告"), tr("子网掩码不能为空!"));
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));
         msgBox.setText(tr("子网掩码不能为空!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
       }else{
         QRegExp rx("^((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$");
         if(!rx.exactMatch(ui->lineEdit_subnetmask->text())){
        //    QMessageBox::information (NULL, tr("提示"),tr("子网掩码错误!"),QMessageBox::Ok);
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("提示"));
            msgBox.setText(tr("子网掩码错误!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
         }else{
           QByteArray ba = ui->lineEdit_subnetmask->text().toLatin1();
           strcpy(Globa.NET_SetParam.msk_address, ba.data());
           System_Net_Set_Save();
           Insert_Operation_Work_Record(1, "修改本机子网掩码");
         }
       }
       ui->lineEdit_subnetmask->setText(QString("%1").arg(Globa.NET_SetParam.msk_address));
       break;
     case 13:
       if(ui->lineEdit_defaultgateway->text().isEmpty()){
      //   QMessageBox::warning(NULL,tr("警告"), tr("默认网关不能为空!"));
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));
         msgBox.setText(tr("默认网关不能为空!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
       }else{
         QRegExp rx("^((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$");
         if(!rx.exactMatch(ui->lineEdit_defaultgateway->text())){
          // QMessageBox::information (NULL, tr("提示"),tr("默认网关错误!"),QMessageBox::Ok);
           QMessageBox msgBox;
           msgBox.setWindowTitle(tr("提示"));
           msgBox.setText(tr("默认网关错误!"));
           msgBox.setIcon(QMessageBox::Information);
           msgBox.setStandardButtons(QMessageBox::Ok);
           msgBox.exec();
         }else{
           QByteArray ba = ui->lineEdit_defaultgateway->text().toLatin1();
           strcpy(Globa.NET_SetParam.gate_address, ba.data());
           System_Net_Set_Save();
           Insert_Operation_Work_Record(1, "修改本机默认网关");
         }
       }
       ui->lineEdit_defaultgateway->setText(QString("%1").arg(Globa.NET_SetParam.gate_address));
      break;
   case 14:
     if((SystemSetEdit->text().toInt()>255)||(SystemSetEdit->text().toInt()<1)){
       QMessageBox msgBox;
       msgBox.setWindowTitle(tr("提示"));
       msgBox.setText(tr("请输入1-255之间的数据!"));
       msgBox.setIcon(QMessageBox::Information);
       msgBox.setStandardButtons(QMessageBox::Ok);
       msgBox.exec();
     }else{
       Globa.User_Monitor_Screen_Set.backgroundAddress = SystemSetEdit->text().toInt();
       System_UseSystem_save();
       Insert_Operation_Work_Record(1, "修改本机地址");
     }
     SystemSetEdit->setText(QString("%1").arg(Globa.User_Monitor_Screen_Set.backgroundAddress));
    break;
   default:
     break;
   }
}




/*********************系统时间设置****************************/
void Monitor_Screen_Set::on_lineEdit_year_selectionChanged()
{
   SystemSetEdit=ui->lineEdit_year;
   SystemSetflag=1;
   SystemSetEdit->setText("");
   dianfalg=0;
   //keyboardsyparameter.exec();
   emit keyboard_show();

}

void Monitor_Screen_Set::on_lineEdit_month_selectionChanged()
{
    SystemSetEdit=ui->lineEdit_month;
    SystemSetflag=2;
    SystemSetEdit->setText("");
    dianfalg=0;
   // keyboardsyparameter.exec();
    emit keyboard_show();


}

void Monitor_Screen_Set::on_lineEdit_date_selectionChanged()
{
    SystemSetEdit=ui->lineEdit_date;
    SystemSetflag=3;
    SystemSetEdit->setText("");
    dianfalg=0;
  //  keyboardsyparameter.exec();
    emit keyboard_show();


}

void Monitor_Screen_Set::on_lineEdit_hour_selectionChanged()
{
    SystemSetEdit=ui->lineEdit_hour;
    SystemSetflag=4;
    SystemSetEdit->setText("");
    dianfalg=0;
  //  keyboardsyparameter.exec();
    emit keyboard_show();


}

void Monitor_Screen_Set::on_lineEdit_minute_selectionChanged()
{
    SystemSetEdit=ui->lineEdit_minute;
    SystemSetflag=5;
    SystemSetEdit->setText("");
    dianfalg=0;
   // keyboardsyparameter.exec();
    emit keyboard_show();


}

void Monitor_Screen_Set::on_lineEdit_second_selectionChanged()
{
    SystemSetEdit=ui->lineEdit_second;
    SystemSetflag=6;
    SystemSetEdit->setText("");
    dianfalg=0;
 //   keyboardsyparameter.exec();
    emit keyboard_show();


}

/*********************告警消息，屏保时间亮度****************************/
void Monitor_Screen_Set::on_AlarmBox_currentIndexChanged(int index)
{
    if(IsComParameterChanged){
       Globa.User_Monitor_Screen_Set.AlarmSound = index;
       System_UseSystem_save();
       Insert_Operation_Work_Record(1, "修改屏幕告警是否鸣叫");
    }
}

void Monitor_Screen_Set::on_lineEdit_bright_selectionChanged()
{
    SystemSetEdit=ui->lineEdit_bright;
    SystemSetflag=7;
    SystemSetEdit->setText("");
    dianfalg=0;
 //   keyboardsyparameter.exec();
    emit keyboard_show();


}

void Monitor_Screen_Set::on_ScrProEdit_selectionChanged()
{
    SystemSetEdit=ui->ScrProEdit;
    SystemSetflag=8;
    SystemSetEdit->setText("");
    dianfalg=0;
    emit keyboard_show();


}

/*********************重新设置密码***************************/

void Monitor_Screen_Set::on_NewCodeEdit_selectionChanged()
{
    SystemSetEdit=ui->NewCodeEdit;
    SystemSetflag=9;
    SystemSetEdit->setText("");
    dianfalg=0;
    emit keyboard_show();


}

void Monitor_Screen_Set::on_NewCode2Edit_selectionChanged()
{
    SystemSetEdit=ui->NewCode2Edit;
    SystemSetflag=10;
    SystemSetEdit->setText("");
    dianfalg=0;
    emit keyboard_show();

}

/*********************后台通讯设置***************************/

void Monitor_Screen_Set::on_backgroundlineEdit_selectionChanged()
{
    SystemSetEdit=ui->backgroundlineEdit;
    SystemSetflag=14;
    SystemSetEdit->setText("");
    dianfalg=0;
    emit keyboard_show();
}

void Monitor_Screen_Set::on_backgroundcomboBox_currentIndexChanged(int index)
{
    if(IsComParameterChanged){
       Globa.User_Monitor_Screen_Set.Baudratechoice=index;  //波特率选择
       UART_ttyS2_PC_init();
       System_UseSystem_save();
       Insert_Operation_Work_Record(1, "修改后台通讯波特率");
    }
}

void Monitor_Screen_Set::on_backgroundcomboBox_1_currentIndexChanged(int index)
{
    if(IsComParameterChanged){
       Globa.User_Monitor_Screen_Set.paritychoice=index; //奇偶校验
       UART_ttyS2_PC_init();
       System_UseSystem_save();
       Insert_Operation_Work_Record(1, "修改后台通讯奇偶校验");

    }
}

/******************** *后台网络设置***************************/

void Monitor_Screen_Set::on_comboBox_getip_currentIndexChanged(int index)
{
    if(IsComParameterChanged){
       Globa.NET_SetParam.assign_method = index;
       if(Globa.NET_SetParam.assign_method  == 0){
        ui->lineEdit_ipaddress->setEnabled(false);
        ui->lineEdit_subnetmask->setEnabled(false);
        ui->lineEdit_defaultgateway->setEnabled(false);
      }else{
        ui->lineEdit_ipaddress->setEnabled(true);
        ui->lineEdit_subnetmask->setEnabled(true);
        ui->lineEdit_defaultgateway->setEnabled(true);
      }
       System_Net_Set_Save();
       Insert_Operation_Work_Record(1, "修改获取IP地址方式");

    }
}

void Monitor_Screen_Set::on_lineEdit_ipaddress_selectionChanged()
{
    SystemSetEdit=ui->lineEdit_ipaddress;
    SystemSetflag=11;
    SystemSetEdit->setText("");

    dianfalg=1;
  //  keyboardsyparameter.exec();
    emit keyboard_show();

}

void Monitor_Screen_Set::on_lineEdit_subnetmask_selectionChanged()
{
    SystemSetEdit=ui->lineEdit_subnetmask;
    SystemSetEdit->setText("");
    SystemSetflag=12;

    dianfalg=1;
    emit keyboard_show();

}

void Monitor_Screen_Set::on_lineEdit_defaultgateway_selectionChanged()
{
    SystemSetEdit=ui->lineEdit_defaultgateway;
    SystemSetEdit->setText("");
    SystemSetflag=13;
    dianfalg=1;
    emit keyboard_show();

}




void Monitor_Screen_Set::on_system_language_currentIndexChanged(int index)
{

  if(IsComParameterChanged){
    Globa.User_Monitor_Screen_Set.System_language = index;
    System_UseSystem_save();//系统参数设置（管理员）
    Restar_SystemFlage = 1;
    Insert_Operation_Work_Record(1, "修改系统语言");
  }
}

void Monitor_Screen_Set::on_screen_calibaration_clicked()
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(tr("消息"));
  msgBox.setText(tr("是否校准屏幕?"));
  msgBox.setIcon(QMessageBox::Information);
  msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
  if(msgBox.exec() == QMessageBox::Ok){
     Calibration  ls_clibration;
     ls_clibration.exec();
     Insert_Operation_Work_Record(1, "屏幕校准");
  }
}

/*void Monitor_Screen_Set::on_restore_factory_set_clicked()
{

  QMessageBox msgBox;
  msgBox.setWindowTitle(tr("警告"));
  msgBox.setText(tr("是否恢复出厂默认设置?"));
  msgBox.setIcon(QMessageBox::Warning);
  msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
  if(msgBox.exec() == QMessageBox::Ok){
    if(1 == true){
     QFile::remove(F_UseSystem);
     QFile::remove(F_BAK_UseSystem);
     QFile::remove(F_SETNET);

     QFile::remove(F_BAK_SETNET);//加上net参数
     msgBox.setText(tr("请重新启动!"));
     if(msgBox.exec() == QMessageBox::Ok){
       Insert_Operation_Work_Record(1, "恢复监控出厂默认设置");
       Reboot Reboot_View;
       Reboot_View.exec();
       //system("reboot");
      }
    }else{
      msgBox.setText(tr("恢复失败!"));
      msgBox.exec();
    }
  }
}
*/

void Monitor_Screen_Set::on_listWidget_currentRowChanged(int currentRow)
{
  GPIO_SYS_BUZZ_ON;//干接点控制接口函数

  usleep(30000);
  GPIO_SYS_BUZZ_OFF;
}
