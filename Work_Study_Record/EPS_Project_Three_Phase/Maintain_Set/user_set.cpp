#include "user_set.h"
#include "ui_user_set.h"

User_Set::User_Set(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::User_Set)
{
  ui->setupUi(this);
  this->setGeometry(0,0,800,480);
  checkButton=0;
  ui->Month_Detection->setObjectName("styleButton");
  ui->Year_Detection->setObjectName("styleButton");
  ui->btn_restore->setObjectName("styleButton");
  ui->btn_reset->setObjectName("styleButton");
  //ui->btn_clear_fault->setObjectName("styleButton");


  ui->setAlarm->setObjectName("mButton");

  connect(ui->listWidget,SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
  ui->listWidget->setSpacing(10);



  ui->comboBox_ATS->setView(new QListView());
  ui->comboBox_ATS->clear();
  ui->comboBox_ATS->addItem(tr("无"));
  ui->comboBox_ATS->addItem(tr("有"));


  ui->comboBox_Distribution->setView(new QListView);
  ui->comboBox_Distribution->clear();
  ui->comboBox_Distribution->addItem(tr("无"));
  ui->comboBox_Distribution->addItem(tr("有"));

  ui->comboBox_protectenable->setView(new QListView());
  ui->comboBox_protectenable->clear();
  ui->comboBox_protectenable->addItem(tr("不使能"));
  ui->comboBox_protectenable->addItem(tr("使能"));

  ui->comboBox_outputalarmenable->setView(new QListView());
  ui->comboBox_outputalarmenable->clear();
  ui->comboBox_outputalarmenable->addItem(tr("不使能"));
  ui->comboBox_outputalarmenable->addItem(tr("使能"));


  IsComParameterChanged = false;
  ui->comboBox_ATS->setCurrentIndex(Globa.Curstomer_Param_Set.ATS_Configure_Flage);
  ui->comboBox_Distribution->setCurrentIndex(Globa.Curstomer_Param_Set.Distribution_Configure_Flage);
  ui->comboBox_protectenable->setCurrentIndex((Globa.Send_Dsp_Debug_Param_Set.EnableControl>>0)&1);
  ui->comboBox_outputalarmenable->setCurrentIndex((Globa.Send_Dsp_Debug_Param_Set.EnableControl>>2)&1);

  IsComParameterChanged =true;
  ui->Distribution_branches_number->setText(QString("%1").arg(Globa.Curstomer_Param_Set.Distribution_Branches_Number));
  ui->label_SWITCH->setText(tr("COM3,模块%1个，起始地址64").arg(Globa.Curstomer_Param_Set.Distribution_Branches_modelnum));
  if(Globa.Curstomer_Param_Set.Distribution_Configure_Flage==0){
      ui->Distribution_branches_number->setEnabled(false);
      ui->label_SWITCH->setVisible(false);
      ui->setAlarm->setVisible(false);
    }



}

User_Set::~User_Set()
{
  delete ui;
}

void User_Set::fresh()
{
  IsComParameterChanged = false;
  ui->comboBox_ATS->setCurrentIndex(Globa.Curstomer_Param_Set.ATS_Configure_Flage);
  ui->comboBox_Distribution->setCurrentIndex(Globa.Curstomer_Param_Set.Distribution_Configure_Flage);
  ui->comboBox_protectenable->setCurrentIndex((Globa.Send_Dsp_Debug_Param_Set.EnableControl>>0)&1);
  ui->comboBox_outputalarmenable->setCurrentIndex((Globa.Send_Dsp_Debug_Param_Set.EnableControl>>2)&1);

  IsComParameterChanged =true;
  ui->Distribution_branches_number->setText(QString("%1").arg(Globa.Curstomer_Param_Set.Distribution_Branches_Number));
  ui->label_SWITCH->setText(tr("COM3,模块%1个，起始地址64").arg(Globa.Curstomer_Param_Set.Distribution_Branches_modelnum));

  if(Globa.Curstomer_Param_Set.Distribution_Configure_Flage==0){
      ui->Distribution_branches_number->setEnabled(false);
      ui->label_SWITCH->setVisible(false);
      ui->setAlarm->setVisible(false);
    }


    if((Globa.Send_Dsp_Debug_Param_Set.EPS_Work_Mode_Selection==1)&&(ui->listWidget->count()==3)){
      ui->listWidget->takeItem(0);
      ui->stackedWidget->removeWidget(ui->page_1);
      ui->listWidget->setCurrentRow(0);
    }else if((Globa.Send_Dsp_Debug_Param_Set.EPS_Work_Mode_Selection==0)&&(ui->listWidget->count()==2)){
        ui->listWidget->insertItem(0,tr("自检"));
        ui->stackedWidget->insertWidget(0,ui->page_1);
   }
}
void User_Set::User_Set_Parame_Set_KeyValue()
{

  if((ui->Distribution_branches_number->text().toInt() > 64)){
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("消息"));
    msgBox.setText(tr("请输入0-64之间的数!"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
  }else{
    Globa.Curstomer_Param_Set.Distribution_Branches_Number = ui->Distribution_branches_number->text().toInt();
    if( Globa.Curstomer_Param_Set.Distribution_Branches_Number>32)
      Globa.Curstomer_Param_Set.Distribution_Branches_modelnum=2;
    else
    {
        Globa.Curstomer_Param_Set.Distribution_Branches_modelnum=1;
    }
    System_Curstomer_Param_Set_save();
     Globa.Eps_Param_Data.Current_Alarm_List_Switch_Refresh=1;
    Insert_Operation_Work_Record(1, "修改配电支路数");
  }
  ui->Distribution_branches_number->setText(QString("%1").arg(Globa.Curstomer_Param_Set.Distribution_Branches_Number));
  ui->label_SWITCH->setText(tr("COM3,模块%1个，起始地址64").arg(Globa.Curstomer_Param_Set.Distribution_Branches_modelnum));

}


void User_Set::User_Set_Parame_Set_GetKeyValue(int key)
{
  if(!this->isHidden()){
    if(key == 10){
      ui->Distribution_branches_number->setText("");
    }else if(key == 11){
      ui->Distribution_branches_number->setText(QString("%1").arg(Globa.Curstomer_Param_Set.Distribution_Branches_Number));
    }else if(key == 12){
      User_Set_Parame_Set_KeyValue();
    }else if(key < 10) {
      ui->Distribution_branches_number->setText(ui->Distribution_branches_number->text() + QString("%1").arg(key));
    }else if(key == 14){
      ui->Distribution_branches_number->setText(ui->Distribution_branches_number->text().mid(0,ui->Distribution_branches_number->text().count() - 1));
    }
  }
}

//*************************自检*************************************
void User_Set::on_Month_Detection_clicked()
{checkButton=0;

  emit passwordboard_show();



}

void User_Set::on_Year_Detection_clicked()
{ checkButton=1;

  emit passwordboard_show();

}

void User_Set::CheckMenu(QString pwd)//静音
{
  if(!this->isHidden()){
      QDateTime time = QDateTime::currentDateTime();
      QString time_password = time.toString("yyMMdd");//设置系统

      switch(checkButton){
        //设置选项
        case 0:
          //用户密码显示界面
         if((pwd== QLatin1String(Globa.User_Monitor_Screen_Set.userpassword))||(pwd ==time_password)){
             //QMessageBox msgBox;
              // msgBox.setWindowTitle(tr("警告"));
              // msgBox.setText(tr("是否启动月检?"));
              // msgBox.setIcon(QMessageBox::Warning);
              // msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
              // if(msgBox.exec() == QMessageBox::Ok){
               if((Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status]%3 != 1)&&(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status]%3 != 1)){
                 if(Issuedequipment_parameters_06(DSPSTARTADDR, Month_Check_Value_Enum, 0xFF, 1)){
                   DSP_Analog_Data_mutex.lockForWrite();
                   Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status] = 4;
                   DSP_Analog_Data_mutex.unlock();
                //   QMessageBox msgBox;
               //    msgBox.setWindowTitle(tr("消息"));
              //     msgBox.setText(tr("参数下发成功!"));
              //     msgBox.setIcon(QMessageBox::Information);
              //     msgBox.setStandardButtons(QMessageBox::Ok);
              //     msgBox.exec();
                   Insert_Operation_Work_Record(1, "手动启动月检");

                  // sleep(10);
                   Mon_CheckResultShow_Flag = 1;
                 }else{
                     ZMessage msgBox;
                     //QMessageBox msgBox;
                     msgBox.setWindowTitle(tr("消息"));
                     msgBox.setText(tr("参数下发失败!"));
                     //msgBox.setIcon(QMessageBox::Information);
                     //msgBox.setStandardButtons(QMessageBox::Ok);
                     msgBox.exec();
                 }
               }else {
                   ZMessage msgBox;
                   msgBox.setWindowTitle(tr("警告"));
                   msgBox.setText(tr("正在进行月检或年检"));
                   //msgBox.setIcon(QMessageBox::Information);
                   //msgBox.setStandardButtons(QMessageBox::Ok);
                   msgBox.exec();
                 }
             //}
            return;
          }
          break;

        case 1:
          //用户密码显示界面
         if((pwd== QLatin1String(Globa.User_Monitor_Screen_Set.userpassword))||(pwd == time_password)){
             // QMessageBox msgBox;
               // msgBox.setWindowTitle(tr("警告"));
                //msgBox.setText(tr("是否启动年检?"));
                //msgBox.setIcon(QMessageBox::Warning);
                //msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
               // if(msgBox.exec() == QMessageBox::Ok){
                if((Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status]%3 != 1)&&(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status]%3 != 1)){
                  if(Issuedequipment_parameters_06(DSPSTARTADDR, Year_Check_Value_Enum, 0xFF, 1)){
                    DSP_Analog_Data_mutex.lockForWrite();
                    Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status] = 4;
                    DSP_Analog_Data_mutex.unlock();
               //     QMessageBox msgBox;
               //     msgBox.setWindowTitle(tr("消息"));
               //     msgBox.setText(tr("参数下发成功!"));
               //     msgBox.setIcon(QMessageBox::Information);
                //    msgBox.setStandardButtons(QMessageBox::Ok);
               //     msgBox.exec();
                    Insert_Operation_Work_Record(1, "手动启动年检");
                    year_CheckResultShow_Flag = 1;
                  }else{
                      ZMessage msgBox;
                      //QMessageBox msgBox;
                      msgBox.setWindowTitle(tr("消息"));
                      msgBox.setText(tr("参数下发失败!"));
                      //msgBox.setIcon(QMessageBox::Information);
                      //msgBox.setStandardButtons(QMessageBox::Ok);
                      msgBox.exec();
                  }
                }else {
                    ZMessage msgBox;
                    msgBox.setWindowTitle(tr("警告"));
                    msgBox.setText(tr("正在进行月检或年检"));
                    //msgBox.setIcon(QMessageBox::Information);
                    //msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.exec();
                  }
              //    }
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

//***********************消防联动模式**********************************
/*void User_Set::on_comboBox_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
  if(Issuedequipment_parameters_06(DSPSTARTADDR, 8, index+1, 1)){
  //    QMessageBox msgBox;
 //     msgBox.setWindowTitle(tr("消息"));
 //     msgBox.setText(tr("参数下发成功!"));
 //     msgBox.setIcon(QMessageBox::Information);
//      msgBox.setStandardButtons(QMessageBox::Ok);
      Globa.Curstomer_Param_Set.EPS_Work_Mode_Selection = index+1;;
      System_Curstomer_Param_Set_save();
//      msgBox.exec();
      Insert_Operation_Work_Record(1, "修改消防联动时EPS状态");
    }else{
      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
      IsComParameterChanged = false;
      ui->comboBox->setCurrentIndex(Globa.Curstomer_Param_Set.EPS_Work_Mode_Selection - 1);
      IsComParameterChanged = true;
    }
  }
}
*/

bool User_Set::removeDirWithContent(const QString &dirName)
{
  static QVector<QString> dirNames;
  QDir dir;

  QFileInfoList filst;
  QFileInfoList::iterator curFi;
  //初始化
  dirNames.clear();
  if(dir.exists()){
    dirNames<<dirName;
  }else{
    return false;
  }
  //遍历各级文件夹，并将这些文件夹中的文件删除
  for(int i=0;i<dirNames.size();++i){
    dir.setPath(dirNames[i]);
    filst=dir.entryInfoList(QDir::Dirs|QDir::Files
                           |QDir::Readable|QDir::Writable
                           |QDir::Hidden|QDir::NoDotAndDotDot
                           ,QDir::Name);
    if(filst.size()>0){
      curFi=filst.begin();
      while(curFi!=filst.end()){
        //遇到文件夹,则添加至文件夹列表dirs尾部
        if(curFi->isDir()){
          dirNames.push_back(curFi->filePath());
        }else if(curFi->isFile()){
          //遇到文件,则删除之
          if(!dir.remove(curFi->fileName())){
            return true;
          }
        }
         curFi++;
      }//end of while
    }
  }
  //删除文件夹
  for(int i=dirNames.size()-1;i>=0;--i){
    dir.setPath(dirNames[i]);
    if(!dir.rmdir(".")){
       return true;
    }
  }
  return false;
}
//***************恢复出厂设置********************
void User_Set::on_btn_restore_clicked()
{

  QMessageBox msgBox;
  msgBox.setWindowTitle(tr("警告"));
  msgBox.setText(tr("是否恢复出厂默认设置?"));
  msgBox.setIcon(QMessageBox::Warning);
  msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
  if(msgBox.exec() == QMessageBox::Ok){
    if(1 == true){
     QFile::remove(F_SET_CUSTOMER_SETUP);
     QFile::remove(F_BAK_SET_CUSTOMER_SETUP);
     QFile::remove(F_UseSystem);
     QFile::remove(F_BAK_UseSystem);
     LOG("Factory reset");
     msgBox.setText(tr("请重新启动!"));
     if(msgBox.exec() == QMessageBox::Ok){
       Insert_Operation_Work_Record(1, "恢复客户设置出厂默认设置");
       LOG("Factory reset,  reboot system");
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



void User_Set::on_comboBox_ATS_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
  //  QMessageBox msgBox;
  //  msgBox.setWindowTitle(tr("消息"));
  //  msgBox.setText(tr("参数下发成功!"));
  //  msgBox.setIcon(QMessageBox::Information);
  //  msgBox.setStandardButtons(QMessageBox::Ok);
    Globa.Curstomer_Param_Set.ATS_Configure_Flage = index;
    System_Curstomer_Param_Set_save();
  //  msgBox.exec();
    Insert_Operation_Work_Record(1, "修改ATS有无状态");
  }
}

void User_Set::on_comboBox_Distribution_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
  //  QMessageBox msgBox;
  //  msgBox.setWindowTitle(tr("消息"));
  //  msgBox.setText(tr("参数下发成功!"));
  //  msgBox.setIcon(QMessageBox::Information);
  //  msgBox.setStandardButtons(QMessageBox::Ok);
    Globa.Curstomer_Param_Set.Distribution_Configure_Flage = index;
    System_Curstomer_Param_Set_save();
  //  msgBox.exec();
    if(index==0){
      ui->Distribution_branches_number->setEnabled(false);
      ui->label_SWITCH->setVisible(false);
      ui->setAlarm->setVisible(false);
    }else{
      ui->Distribution_branches_number->setEnabled(true);
      ui->label_SWITCH->setVisible(true);
      ui->setAlarm->setVisible(true);
      }
    Insert_Operation_Work_Record(1, "修改配电信息有无状态");
  }
}

void User_Set::on_Distribution_branches_number_selectionChanged()
{
  ui->Distribution_branches_number->setText("");
  emit keyboard_show();
}
/*
void User_Set::on_btn_clear_fault_clicked()
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(tr("警告"));
  msgBox.setText(tr("清除故障"));
  msgBox.setIcon(QMessageBox::Warning);
  msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
  if(msgBox.exec() == QMessageBox::Ok){
    if(Issuedequipment_parameters_06(DSPSTARTADDR, Clear_Fault_Enum, 0xFF, 1)){
        Insert_Operation_Work_Record(1, "清除故障");
    }else{
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("参数下发失败!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }
  }
}
*/


void User_Set::on_listWidget_currentRowChanged(int currentRow)
{
  GPIO_SYS_BUZZ_ON;//干接点控制接口函数

  usleep(30000);
  GPIO_SYS_BUZZ_OFF;
}

void User_Set::on_comboBox_protectenable_currentIndexChanged(int index)
{
  if(IsComParameterChanged){

     UINT32 tmp=0;
     tmp=Globa.Send_Dsp_Debug_Param_Set.EnableControl&(0xFFFE)|(index<<0);


  if(Issuedequipment_parameters_06(DSPSTARTADDR, Enable_Control_Enum, tmp, 1)){

     Globa.Send_Dsp_Debug_Param_Set.EnableControl = tmp;
     System_Set_DSP_Parame_save();
      if(index==1)
      {
          Insert_Operation_Work_Record(1, "使能主电过载保护");
      }
      else if(index==0)
      {
          Insert_Operation_Work_Record(1, "关闭主电过载保护使能");
      }

    }else{
      ZMessage msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.exec();
      IsComParameterChanged = false;
      ui->comboBox_protectenable->setCurrentIndex((Globa.Send_Dsp_Debug_Param_Set.EnableControl>>0)&1);
      IsComParameterChanged = true;
    }
  }
}

void User_Set::on_comboBox_outputalarmenable_currentIndexChanged(int index)
{
  if(IsComParameterChanged){

     UINT32 tmp=0;
     tmp=Globa.Send_Dsp_Debug_Param_Set.EnableControl&(0xFFFB)|(index<<2);


  if(Issuedequipment_parameters_06(DSPSTARTADDR, Enable_Control_Enum, tmp, 1)){

     Globa.Send_Dsp_Debug_Param_Set.EnableControl = tmp;
     System_Set_DSP_Parame_save();
      if(index==1)
      {
          Insert_Operation_Work_Record(1, "使能输出开路告警");
      }
      else if(index==0)
      {
          Insert_Operation_Work_Record(1, "关闭输出开路告警使能");
      }

    }else{
      ZMessage msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.exec();
      IsComParameterChanged = false;
      ui->comboBox_outputalarmenable->setCurrentIndex((Globa.Send_Dsp_Debug_Param_Set.EnableControl>>2)&1);
      IsComParameterChanged = true;
    }
  }
}


void User_Set::on_btn_reset_clicked()
{
  if(Issuedequipment_parameters_06(DSPSTARTADDR, Clear_Fault_Enum, 0xff, 1))
    {


    }
  else
    {
      ZMessage msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.exec();
    }
}



void User_Set::on_setAlarm_clicked()
{
     emit Changed_Current_View(BRANCHALARMSET, 0, 0, 0);
}
