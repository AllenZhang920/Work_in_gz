#include "maintain_set.h"
#include "ui_maintain_set.h"

Maintain_Set::Maintain_Set(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Maintain_Set)
{

  ui->setupUi(this);
  ui->btn_test->setObjectName("styleButton");



  {
    ui->lineEdit->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->lineEdit_4->setVisible(false);

    //ui->widget_2_2_1->setVisible(false);
  }

  connect(ui->listWidget,SIGNAL(currentRowChanged(int)),ui->stackedWidget, SLOT(setCurrentIndex(int)));

  ui->listWidget->setCurrentRow(0);
  ui->listWidget->setSpacing(8);
  connect(ui->listWidget_2,SIGNAL(currentRowChanged(int)),ui->stackedWidget_2, SLOT(setCurrentIndex(int)));
  ui->listWidget_2->setCurrentRow(0);
  ui->listWidget_2->setSpacing(8);
  Battery_Group_No = 0;
  Batt_Number[0] = 0;
  Batt_Number[1] = 0;
  Batt_Number[2] = 0;
  Batt_Number[3] = 0;
  batteryflag = 0;
  IsComParameterChanged = false;

  addComBoxView();
  addCombox_Item();
  add_Group_Combox_item(Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable);
  Interface_Initialize();

  Maintain_Set_Parameter_Set_Init_1();
  Maintain_Set_Parameter_Set_Init_2();

  Adjustment_Dislpay_Data();//校准系数读取值，需不断刷新显示
  ChoosebatteryInterface(0);


}

Maintain_Set::~Maintain_Set()
{
  delete ui;
}
void Maintain_Set::MSdataload()
{
  Interface_Initialize();
  Maintain_Set_Parameter_Set_Init_1();
  Maintain_Set_Parameter_Set_Init_2();
  //Adjustment_Dislpay_Data();//校准系数读取值，需不断刷新显示
  //ChoosebatteryInterface(0);

}

void Maintain_Set::returnVal_show()
{
  ui->Variable_Address_1_Value->setText(QString("%1").arg(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Analog_Addr1_Value]));
  ui->Variable_Address_2_Value->setText(QString("%1").arg(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Analog_Addr2_Value]));
  ui->Variable_Address_3_Value->setText(QString("%1").arg(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Analog_Addr3_Value]));
  ui->Variable_Address_4_Value->setText(QString("%1").arg(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Analog_Addr4_Value]));

 // ui->Charge_Com_Coefficient->setText(QString("%1").arg(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Charge_Com_Coefficient_Value]));
  //ui->Temp_Com_Coefficient->setText(QString("%1").arg(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Temp_Com_Coefficient_Value]));

}

void Maintain_Set::Adjustment_Dislpay_Data()
{   if (ui->tabWidget->currentIndex()==0){
      UINT32 eps_run_state=0;
      DSP_State_Alarm_mutex.lockForRead();
      eps_run_state = Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status];//EPS运行状态01H：主电模式， 02H：应急模式，其他待机模式）
      DSP_State_Alarm_mutex.unlock();
      if((Shutdown_Flag==1)||(eps_run_state==0)){
          ui->Batt_Group_Number->setEnabled(true);
          ui->Batt_Capcity->setEnabled(true);
          ui->Single_Batt_Discharge_End_V->setEnabled(true);
          ui->Sys_Battery_Number->setEnabled(true);
          ui->Mains_Voltage_Top_limit->setEnabled(true);
          ui->Mains_Voltage_Low_limit->setEnabled(true);
          ui->Mains_Frequence_Top_limit->setEnabled(true);
          ui->Mains_Frequence_Low_limit->setEnabled(true);
          ui->comboBox_Dry_contact_6->setEnabled(true);
          ui->comboBox_Dry_contact_7->setEnabled(true);
          ui->comboBox_Dry_contact_8->setEnabled(true);
          ui->comboBox_Dry_contact_9->setEnabled(true);
          ui->comboBox_Dry_contact_11->setEnabled(true);
          ui->comboBox_Dry_contact_12->setEnabled(true);
          /*ui->comboBox_Dry_contact_13->setEnabled(true);
          ui->comboBox_Dry_contact_14->setEnabled(true);*///del zzc 721
          ui->line_frameCapacitySet->setEnabled(true);
          ui->combox_eps_modole->setEnabled(true);
          ui->ComboxFreqChangeEnabel->setEnabled(true);
          ui->ComboxFreqChooseEnabel->setEnabled(true);
          ui->combox_eps_switch_modole->setEnabled(true);
          //ui->combox_remote_control_1->setEnabled(true);
          ui->combox_remote_control_1->setEnabled(false);
          ui->combox_remote_control_2->setEnabled(true);
          ui->label_set->setText("");
          /*ui->widget_2_1->setEnabled(true);
          ui->widget_2_2->setEnabled(true);
          ui->widget_2_3->setEnabled(true);
          ui->widget_2_4->setEnabled(true);
          ui->widget_2_5->setEnabled(true);*/

        }else{
          ui->Batt_Group_Number->setEnabled(false);
          ui->Batt_Capcity->setEnabled(false);
          ui->Single_Batt_Discharge_End_V->setEnabled(false);
          ui->Sys_Battery_Number->setEnabled(false);
          ui->Mains_Voltage_Top_limit->setEnabled(false);
          ui->Mains_Voltage_Low_limit->setEnabled(false);
          ui->Mains_Frequence_Top_limit->setEnabled(false);
          ui->Mains_Frequence_Low_limit->setEnabled(false);
          ui->comboBox_Dry_contact_6->setEnabled(false);
          ui->comboBox_Dry_contact_7->setEnabled(false);
          ui->comboBox_Dry_contact_8->setEnabled(false);
          ui->comboBox_Dry_contact_9->setEnabled(false);
          ui->comboBox_Dry_contact_11->setEnabled(false);
          ui->comboBox_Dry_contact_12->setEnabled(false);
          /*ui->comboBox_Dry_contact_13->setEnabled(false);
          ui->comboBox_Dry_contact_14->setEnabled(false);*///del zzc 721
          ui->line_frameCapacitySet->setEnabled(false);
          ui->combox_remote_control_1->setEnabled(false);
          ui->combox_remote_control_2->setEnabled(false);
          ui->combox_eps_modole->setEnabled(false);
          ui->combox_eps_switch_modole->setEnabled(false);
          ui->ComboxFreqChangeEnabel->setEnabled(false);
          ui->ComboxFreqChooseEnabel->setEnabled(false);
          ui->label_set->setStyleSheet("color:rgb(255,0,0)");
          ui->label_set->setText(tr("部分参数需要在逆变器关闭的状态才能设置"));


        }

    }
    QString  qstr;
    qstr.setNum((float)(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_A_Phase_Voltage]\
                       +Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_B_Phase_Voltage]\
                       +Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Mains_C_Phase_Voltage])/30,'f',1);
    ui->Main_Vol_Calibration->setText(qstr + "V");   //主电电压

    qstr.setNum((float)(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_A_Phase_Voltage]\
                       +Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_B_Phase_Voltage]\
                       +Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_C_Phase_Voltage])/30,'f',1);
    ui->Emer_Vol_Calibration->setText(qstr + "V");   //应急电压

    qstr.setNum((float)(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_A_Phase_Current]\
                       +Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_B_Phase_Current]\
                       +Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_C_Phase_Current])/30,'f',1);
    ui->Emer_Curr_Calibration->setText(qstr + "A"); //应急电流

    qstr.setNum((float)Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Bus_Voltage]/10,'f',1);
    ui->Bus_Vol_Calibration_Factor_2->setText(qstr + "V");    //母线电压

    qstr.setNum((float)(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Voltage]\
                       +Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_B_Phase_Voltage]\
                       +Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_C_Phase_Voltage])/30,'f',1);
    ui->Oupt_Vol_Calibration->setText(qstr + "V");   //输出电压

    qstr.setNum((float)(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_A_Phase_Current]\
                       +Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_B_Phase_Current]\
                       +Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ouput_C_Phase_Current])/30,'f',1);
    ui->Oupt_Curr_Calibration->setText(qstr + "A");   //输出电流

    qstr.setNum((float)Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Charging_Voltage]/10,'f',1);
    ui->Batt_Vol_Calibration_Factor_2->setText(qstr + "V");   //电池电压

    qstr.setNum((float)Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Current]/10,'f',1);
    ui->Batt_Curr_Calibration_Factor_2->setText(qstr + "A");  //电池电流



    returnVal_show();

}




void Maintain_Set::Maintain_Set_Parameter_Set_Init_2()
{

//  QLabel *BomaAddr[8];
//  BomaAddr[0]=ui->boma;
//  BomaAddr[1]=ui->boma_2;
//  BomaAddr[2]=ui->boma_3;
//  BomaAddr[3]=ui->boma_4;
//  BomaAddr[4]=ui->boma_5;
//  BomaAddr[5]=ui->boma_6;
//  BomaAddr[6]=ui->boma_7;
//  BomaAddr[7]=ui->boma_8;

//  for(int i=0;i<6;i++)
//  {


//      printf("i is %d\n",i);
//      printf("Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable is %d\n",Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable);

//      if(i<Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable)
//        {
//          BomaAddr[i]->setVisible(true);
//        }
//      else
//        {
//          BomaAddr[i]->setVisible(false);

//        }
//  }





  QString  qstr;
  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_1);
  ui->Debug_Parameter_1->setText(qstr);
  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_2);
  ui->Debug_Parameter_2->setText(qstr);
  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_3);
  ui->Debug_Parameter_3->setText(qstr);
  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_4);
  ui->Debug_Parameter_4->setText(qstr);

  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_1);
  ui->Variable_Address_1->setText(qstr);
  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_2);
  ui->Variable_Address_2->setText(qstr);
  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_3);
  ui->Variable_Address_3->setText(qstr);
  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_4);
  ui->Variable_Address_4->setText(qstr);

  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Calibration_Coefficient);
  ui->Main_Vol_Calibration_Factor->setText(qstr);
//  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_B_Calibration_Coefficient);
// ui->Main_Vol_Calibration_Factor_B->setText(qstr);
//  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_C_Calibration_Coefficient);
//  ui->Main_Vol_Calibration_Factor_C->setText(qstr);

  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_Calibration_Coefficient);
  ui->Emer_Vol_Calibration_Factor->setText(qstr);

  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Emergency_Current_Calibration_Coefficient);
  ui->Emer_Curr_Calibration_Factor->setText(qstr);


  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Ouput_Voltage_Calibration_Coefficient);
  ui->Oupt_Vol_Calibration_Factor->setText(qstr);
  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_Calibration_Coefficient);
  ui->Oupt_Curr_Calibration_Factor->setText(qstr);


  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Battery_Voltage_Calibration_Coefficient);
  ui->Batt_Vol_Calibration_Factor->setText(qstr);

  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Battery_Current_Calibration_Coefficient);
  ui->Batt_Curr_Calibration_Factor->setText(qstr);
  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Bus_Voltage_Calibration_Coefficient);
  ui->Bus_Vol_Calibration_Factor->setText(qstr);
  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Equipment_Capacity);
  ui->line_frameCapacitySet->setText(qstr);

 // qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.SETDATA_InvDcVolt_Califactor,'f',0);
  //ui->Dc_Component_Calibration_Factor->setText(qstr);

  //add zzc 926
    qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0]);
    ui->lineEdit->setText(qstr);
    QString str1;
    QString strcode="";
    strcode.append(qstr);
    str1= QString("%1").arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1],4,10,QChar('0'));
    ui->lineEdit_2->setText(str1);
    strcode.append(str1);
    str1= QString("%1").arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2]*100+Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3]/100,6,10,QChar('0'));
    //qstr.setNum(((int)(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2]*100+Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3]/100)));
    ui->lineEdit_3->setText(str1);
    strcode.append(str1);
    str1= QString("%1").arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3]%100,2,10,QChar('0'));
    //qstr.setNum((int)(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3]%100));
    ui->lineEdit_4->setText(str1);
    strcode.append(str1);
    ui->Machine_Code->setText(strcode);
   // qstr.setNum()
    //add zzc 926 end


}


//*************************电池参数**保护参数***电池仪****************
void Maintain_Set::Maintain_Set_Parameter_Set_Init_1()
{
  QString  qstr;

  qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Battery_Discharge_Termination_V/10,'f',1);
  ui->Single_Batt_Discharge_End_V->setText(qstr);

  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Battery_Capacity);
  ui->Batt_Capcity->setText(qstr);


  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number);
  ui->Batt_Group_Number->setText(qstr);



  IsComParameterChanged = false;

  IsComParameterChanged = true;

  qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number);
  ui->Sys_Battery_Number->setText(qstr);



  //保护参数
  //qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.SETDATA_InvWorkTime);//6 应急剩余时间限值，范围【10-40】默认30min
  //ui->Batt_Alar_LeftTime_2->setText(qstr);  
  qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Top_limit/10,'f',1);//8 输出电压保护上限，范围【0-40】默认20V
  ui->Mains_Voltage_Top_limit->setText(qstr);
  qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Low_limit/10,'f',1);//8 输出电压保护下限，范围【0-40】默认20V
  ui->Mains_Voltage_Low_limit->setText(qstr);
  qstr.setNum((float)G_Sigle_Baterry_Voltage_Alarm.UnderVolAlarm/10,'f',1);
  ui->Single_Battery_Low_limit->setText(qstr);

  qstr.setNum((float)G_Sigle_Baterry_Voltage_Alarm.OverVolAlarm/10,'f',1);
  ui->Single_Battery_High_limit->setText(qstr);
  qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Top_limit/10,'f',1);//10 频率保护上限，范围【0~5.5Hz】，默认为1Hz
  ui->Mains_Frequence_Top_limit->setText(qstr);
  qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Low_limit/10,'f',1);//10 频率保护下限，范围【0~5.5Hz】，默认为1Hz
  ui->Mains_Frequence_Low_limit->setText(qstr);

  if(!Globa.Battery_GroupParam_Set.battery_detecting_disable){


      ui->Single_Battery_High_limit->setEnabled(true);
      ui->Single_Battery_Low_limit->setEnabled(true);
}else{

      ui->Single_Battery_High_limit->setEnabled(false);
      ui->Single_Battery_Low_limit->setEnabled(false);
    }

}





/*调试参数*/
void Maintain_Set::Maintain_Set_Parameter_Set_KeyValue()
{

  UINT32  Input_value = 0;

  UINT32  Input_value_2=0;

  UINT64   Input_value_3=0;

  QString  qstr;

  switch(Maintain_Set_Parameter_Set_No){
    case 1:{
      Input_value = ui->Batt_Group_Number->text().toInt(); //电池组数
      if((Input_value > BATTERYMAXGROUP)||(Input_value < 1)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入1-%1之间的数!").arg(BATTERYMAXGROUP));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
          if(Issuedequipment_parameters_06(DSPSTARTADDR, Battery_Group_Enum, Input_value, 1)){
         // if((Input_value<Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable)||(Input_value==0)){
               Globa.Eps_Param_Data.Current_Alarm_List_Battery_Refresh=1;
          //  }
        Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number = Input_value;
        if(Globa.Battery_GroupParam_Set.battery_detecting_disable==0){
          Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable=Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number;
          }else{
            Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable=0;
            Globa.Eps_Param_Data.Current_Alarm_List_Battery_Refresh=1;
          }
        System_Set_DSP_Parame_save();
        Insert_Operation_Work_Record(1, "修改电池组数");
        //battery_number(Globa.Battery_GroupParam_Set.Battery_GROUP);
        //Restar_SystemFlage = 1;
        if(Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable == 0){
          //ui->widget_2_2->setEnabled(false);

          ui->Single_Battery_High_limit->setEnabled(false);
          ui->Single_Battery_Low_limit->setEnabled(false);

          }else{
         //ui->widget_2_2->setEnabled(true);

           ui->Single_Battery_High_limit->setEnabled(true);
           ui->Single_Battery_Low_limit->setEnabled(true);

        }

         ChoosebatteryInterface(0);
        add_Group_Combox_item(Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable);
      }else{
              ZMessage msgBox;
              msgBox.setWindowTitle(tr("消息"));
              msgBox.setText(tr("参数下发失败!"));
              //msgBox.setIcon(QMessageBox::Information);
              //msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.exec();
            }


        }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number);
      ui->Batt_Group_Number->setText(qstr);
      break;
    }
    case 2:{ //电池容量
      Input_value = ui->Batt_Capcity->text().toInt();
      if((Input_value > 200)||(Input_value < 24)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入24~200之间的数据!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
        if(Issuedequipment_parameters_06(DSPSTARTADDR, Battery_Capacity_Enum, Input_value, 1)){
        //  QMessageBox msgBox;
        //  msgBox.setWindowTitle(tr("消息"));
        //  msgBox.setText(tr("参数下发成功"));
        //  msgBox.setIcon(QMessageBox::Information);
        //  msgBox.setStandardButtons(QMessageBox::Ok);
          Globa.Send_Dsp_Debug_Param_Set.Battery_Capacity = Input_value;
         // System_Battery_GroupParam_Set_save();
          System_Set_DSP_Parame_save();
        //  msgBox.exec();
          Insert_Operation_Work_Record(1, "修改电池容量");
        }else{
          ZMessage msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发失败!"));
          //msgBox.setIcon(QMessageBox::Information);
          //msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
      }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Battery_Capacity);
      ui->Batt_Capcity->setText(qstr);
      break;
    }
    case 3:{ //电池放电终止电压
      Input_value = (ui->Single_Batt_Discharge_End_V->text().toFloat()*10);
      if((Input_value > 109)||(Input_value < 102)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入10.2-10.9之间的数据!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
        if(Issuedequipment_parameters_06(DSPSTARTADDR, Battery_Discharge_Termination_V_Enum, Input_value, 1)){
        //  QMessageBox msgBox;
        //  msgBox.setWindowTitle(tr("消息"));
        //  msgBox.setText(tr("参数下发成功"));
        //  msgBox.setIcon(QMessageBox::Information);
        //  msgBox.setStandardButtons(QMessageBox::Ok);
          Globa.Send_Dsp_Debug_Param_Set.Battery_Discharge_Termination_V = Input_value;
          //System_Battery_GroupParam_Set_save();
         System_Set_DSP_Parame_save();
          // msgBox.exec();
          Insert_Operation_Work_Record(1, "修改单节电池放电终止电压");
        }else{
          ZMessage msgBox;
          //QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发失败!"));
          //msgBox.setIcon(QMessageBox::Information);
          //msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
      }
      qstr.setNum((float) Globa.Send_Dsp_Debug_Param_Set.Battery_Discharge_Termination_V/10,'f',1);
      ui->Single_Batt_Discharge_End_V->setText(qstr);
      break;
    }


    case 11:{
      Input_value = ui->Sys_Battery_Number->text().toUInt();
      if((Input_value > 41)||(Input_value < 28)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("提示"));
        msgBox.setText(tr("请输入28-41之间的数据!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
        if(Issuedequipment_parameters_06(DSPSTARTADDR, Battery_All_Section_Number_Enum, Input_value, 1)){

          Globa.Send_Dsp_Debug_Param_Set. Battery_All_Section_Number = Input_value;
          System_Set_DSP_Parame_save();
          for(int i=0;i<BATTERYMAXGROUP;i++)
          {
              Globa.Battery_Param_Set[i].Singlenumber[1]=Input_value-24;

          }
          Globa.Eps_Param_Data.Current_Alarm_List_Battery_Refresh=1;

          System_Battery_Param_Set_save();

          Insert_Operation_Work_Record(1, "修改电池节数");
        }else{
          ZMessage msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发失败!"));
          msgBox.exec();
        }
      }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number);
      ui->Sys_Battery_Number->setText(qstr);
      break;
    }


    case 6:{
      Input_value = (ui->Mains_Voltage_Top_limit->text().toFloat()*10); //主电电压范围
      if((Input_value >2750)|| (Input_value<2200)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入220-275之间的数!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
        if(Issuedequipment_parameters_06(DSPSTARTADDR, Mains_Voltage_Top_limit_Enum, Input_value, 1)){       
          Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Top_limit = Input_value;
          System_Set_DSP_Parame_save();
          Insert_Operation_Work_Record(1, "修改主电电压上限");
        }else{
          ZMessage msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发失败!"));
          msgBox.exec();
        }
      }
      qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Top_limit/10,'f',1);
      ui->Mains_Voltage_Top_limit->setText(qstr);
      break;
    }
  case 7:{
      Input_value = ui->Mains_Voltage_Low_limit->text().toFloat()*10; //主电电压范围
      if((Input_value >2200)|| (Input_value<1650)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入165-220之间的数!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
        if(Issuedequipment_parameters_06(DSPSTARTADDR, Mains_Voltage_Low_limit_Enum, Input_value, 1)){

          Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Low_limit = Input_value;
          System_Set_DSP_Parame_save();
          Insert_Operation_Work_Record(1, "修改主电电压下限");
        }else{
          ZMessage msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发失败!"));
          msgBox.exec();
        }
      }
      qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Low_limit/10,'f',1);
      ui->Mains_Voltage_Low_limit->setText(qstr);
      break;
    }
    case 8:{ //频率主电频率范围
      Input_value = ui->Mains_Frequence_Top_limit->text().toFloat()*10;
      if((Input_value > 550)||(Input_value<500)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入50-55之间的数据!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
        if(Issuedequipment_parameters_06(DSPSTARTADDR, Mains_Frequence_Top_limit_Enum, Input_value, 1)){

          Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Top_limit = Input_value;
          System_Set_DSP_Parame_save();
          Insert_Operation_Work_Record(1, "修改主电频率上限");
        }else{
            ZMessage msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("参数下发失败!"));
            msgBox.exec();
        }
      }
      qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Top_limit/10,'f',1);
      ui->Mains_Frequence_Top_limit->setText(qstr);
      break;
    }
    case 9:{ //频率主电频率范围
      Input_value = ui->Mains_Frequence_Low_limit->text().toFloat()*10;
      if((Input_value > 500)||(Input_value<450)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入45-50之间的数据!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
        if(Issuedequipment_parameters_06(DSPSTARTADDR, Mains_Frequence_Low_limit_Enum, Input_value, 1)){
          Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Low_limit = Input_value;
          System_Set_DSP_Parame_save();
          Insert_Operation_Work_Record(1, "修改主电频率下限");
        }else{
            ZMessage msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("参数下发失败!"));
            msgBox.exec();
        }
      }
      qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Low_limit/10,'f',1);
      ui->Mains_Frequence_Low_limit->setText(qstr);
      break;
    }
 /*   case 10:{ //应急剩余时间限值
      Input_value = ui->Batt_Alar_LeftTime_2->text().toInt();
      if((Input_value > 40)||(Input_value < 10)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入10-40之间的数据!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
        if(Issuedequipment_parameters_06(DSPSTARTADDR, 25, Input_value, 1)){
        //  QMessageBox msgBox;
       //   msgBox.setWindowTitle(tr("消息"));
      //    msgBox.setText(tr("参数下发成功"));
      //    msgBox.setIcon(QMessageBox::Information);
     //     msgBox.setStandardButtons(QMessageBox::Ok);
  //      Globa.Send_Dsp_Debug_Param_Set.SETDATA_InvWorkTime = Input_value;
          System_Set_DSP_Parame_save();
     //     msgBox.exec();
          Insert_Operation_Work_Record(1, "修改应急剩余时间限值");
        }else{
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发失败!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
      }
   //   qstr.setNum((int)Globa.Send_Dsp_Debug_Param_Set.SETDATA_InvWorkTime);
      ui->Batt_Alar_LeftTime_2->setText(qstr);
      break;
     }
*/
    case 25:{
      Input_value = ui->Variable_Address_1->text().toInt();
      if(Input_value > 65535){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入0-65535之间的数据!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }
      else
        {
          if(Issuedequipment_parameters_06(DSPSTARTADDR, Debug_Variable_Address_1_Enum, Input_value, 1))
            {
         //     QMessageBox msgBox;
          //    msgBox.setWindowTitle(tr("消息"));
         //     msgBox.setText(tr("参数下发成功!"));
         //     msgBox.setIcon(QMessageBox::Information);
          //    msgBox.setStandardButtons(QMessageBox::Ok);
              Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_1 = Input_value;
          //    msgBox.exec();
              Insert_Operation_Work_Record(1, "修改调试变量地址1");
            }
          else
            {
              ZMessage msgBox;
              //QMessageBox msgBox;
              msgBox.setWindowTitle(tr("消息"));
              msgBox.setText(tr("参数下发失败!"));
              //msgBox.setIcon(QMessageBox::Information);
              //msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.exec();

            }
        }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_1);
      ui->Variable_Address_1->setText(qstr);
      break;
    }
    case 26:{
      Input_value = ui->Variable_Address_2->text().toInt();
      if(Input_value > 65535)
        {
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("请输入0-65535之间的数据!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
      else
        {
          if(Issuedequipment_parameters_06(DSPSTARTADDR, Debug_Variable_Address_2_Enum, Input_value, 1))
            {
           //   QMessageBox msgBox;
            //  msgBox.setWindowTitle(tr("消息"));
           //   msgBox.setText(tr("参数下发成功!"));
            //  msgBox.setIcon(QMessageBox::Information);
            //  msgBox.setStandardButtons(QMessageBox::Ok);
              Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_2 = Input_value;
              //System_Set_DSP_Parame_save();
           //   msgBox.exec();
              Insert_Operation_Work_Record(1, "修改调试变量地址2");
            }
          else
            {
              ZMessage msgBox;
              //QMessageBox msgBox;
              msgBox.setWindowTitle(tr("消息"));
              msgBox.setText(tr("参数下发失败!"));
              //msgBox.setIcon(QMessageBox::Information);
              //msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.exec();

            }
        }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_2);
      ui->Variable_Address_2->setText(qstr);
      break;
    }
    case 27:{
      Input_value = ui->Variable_Address_3->text().toInt();
      if( Input_value > 65535 )
        {
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("请输入0-65535之间的数据!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
      else
        {
          if(Issuedequipment_parameters_06(DSPSTARTADDR, Debug_Variable_Address_3_Enum, Input_value, 1))
            {
           //   QMessageBox msgBox;
           //   msgBox.setWindowTitle(tr("消息"));
           //   msgBox.setText(tr("参数下发成功!"));
           //   msgBox.setIcon(QMessageBox::Information);
           //   msgBox.setStandardButtons(QMessageBox::Ok);
              Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_3 = Input_value;
              //System_Set_DSP_Parame_save();
          //    msgBox.exec();
             Insert_Operation_Work_Record(1, "修改调试变量地址3");
            }
          else
            {
              ZMessage msgBox;
              //QMessageBox msgBox;
              msgBox.setWindowTitle(tr("消息"));
              msgBox.setText(tr("参数下发失败!"));
              //msgBox.setIcon(QMessageBox::Information);
              //msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.exec();
            }
        }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_3);
      ui->Variable_Address_3->setText(qstr);
      break;
    }
    case 28:{
      Input_value = ui->Variable_Address_4->text().toInt();
      if( Input_value > 65535 )
        {
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("请输入0-65535之间的数据!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
      else
        {
          if(Issuedequipment_parameters_06(DSPSTARTADDR, Debug_Variable_Address_4_Enum, Input_value, 1))
            {
          //    QMessageBox msgBox;
          //    msgBox.setWindowTitle(tr("消息"));
         //     msgBox.setText(tr("参数下发成功!"));
         //     msgBox.setIcon(QMessageBox::Information);
          //    msgBox.setStandardButtons(QMessageBox::Ok);
              Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_4 = Input_value;
              //System_Set_DSP_Parame_save();
         //     msgBox.exec();
              Insert_Operation_Work_Record(1, "修改调试变量地址4");
            }
          else
            {
              ZMessage msgBox;
              //QMessageBox msgBox;
              msgBox.setWindowTitle(tr("消息"));
              msgBox.setText(tr("参数下发失败!"));
              //msgBox.setIcon(QMessageBox::Information);
              //msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.exec();
            }
        }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_4);
      ui->Variable_Address_4->setText(qstr);
      break;


    }
    case 21:{
      Input_value = ui->Debug_Parameter_1->text().toInt();
      if( Input_value > 65535 )
        {
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("请输入0-65535之间的数据!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
      else
        {
          if(Issuedequipment_parameters_06(DSPSTARTADDR, Debug_Parameter_Value_1_Enum, Input_value, 1))
            {
          //    QMessageBox msgBox;
          //    msgBox.setWindowTitle(tr("消息"));
          //    msgBox.setText(tr("参数下发成功!"));
         //     msgBox.setIcon(QMessageBox::Information);
          //    msgBox.setStandardButtons(QMessageBox::Ok);
              Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_1 = Input_value;
              //System_Set_DSP_Parame_save();
           //   msgBox.exec();
              Insert_Operation_Work_Record(1, "修改调试参数值1");
            }
          else
            {
              ZMessage msgBox;
              //QMessageBox msgBox;
              msgBox.setWindowTitle(tr("消息"));
              msgBox.setText(tr("参数下发失败!"));
              //msgBox.setIcon(QMessageBox::Information);
              //msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.exec();
            }
        }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_1);
      ui->Debug_Parameter_1->setText(qstr);
      break;
    }
    case 22:{
      Input_value = ui->Debug_Parameter_2->text().toInt();
      if( Input_value > 65535 )
        {
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("请输入0-65535之间的数据!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
      else
        {
          if(Issuedequipment_parameters_06(DSPSTARTADDR, Debug_Parameter_Value_2_Enum, Input_value, 1))
            {
         //     QMessageBox msgBox;
         //     msgBox.setWindowTitle(tr("消息"));
         //     msgBox.setText(tr("参数下发成功!"));
         //     msgBox.setIcon(QMessageBox::Information);
       //       msgBox.setStandardButtons(QMessageBox::Ok);
              Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_2 = Input_value;
              //System_Set_DSP_Parame_save();
          //    msgBox.exec();
              Insert_Operation_Work_Record(1, "修改调试参数值2");
            }
          else
            {
              ZMessage msgBox;
              //QMessageBox msgBox;
              msgBox.setWindowTitle(tr("消息"));
              msgBox.setText(tr("参数下发失败!"));
              //msgBox.setIcon(QMessageBox::Information);
              //msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.exec();
            }
        }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_2);
      ui->Debug_Parameter_2->setText(qstr);
      break;
    }
    case 23:{
      Input_value = ui->Debug_Parameter_3->text().toInt();
      if( Input_value > 65535 )
        {
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("请输入0-65535之间的数据!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
      else
        {
          if(Issuedequipment_parameters_06(DSPSTARTADDR, Debug_Parameter_Value_3_Enum, Input_value, 1))
            {
      //        QMessageBox msgBox;
       //       msgBox.setWindowTitle(tr("消息"));
      //        msgBox.setText(tr("参数下发成功!"));
     //         msgBox.setIcon(QMessageBox::Information);
     //         msgBox.setStandardButtons(QMessageBox::Ok);
              Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_3 = Input_value;
              //System_Set_DSP_Parame_save();
    //          msgBox.exec();
              Insert_Operation_Work_Record(1, "修改调试参数值3");
            }
          else
            {
              ZMessage msgBox;
              //QMessageBox msgBox;
              msgBox.setWindowTitle(tr("消息"));
              msgBox.setText(tr("参数下发失败!"));
              //msgBox.setIcon(QMessageBox::Information);
              //msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.exec();
            }
        }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_3);
      ui->Debug_Parameter_3->setText(qstr);
      break;
    }
    case 24:{
      Input_value = ui->Debug_Parameter_4->text().toInt();
      if( Input_value > 65535 )
        {
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("请输入0-65535之间的数据!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
      else
        {
          if(Issuedequipment_parameters_06(DSPSTARTADDR, Debug_Parameter_Value_4_Enum, Input_value, 1))
            {
         //     QMessageBox msgBox;
         //     msgBox.setWindowTitle(tr("消息"));
          //    msgBox.setText(tr("参数下发成功!"));
        //      msgBox.setIcon(QMessageBox::Information);
         //     msgBox.setStandardButtons(QMessageBox::Ok);
              Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_4 = Input_value;
              //System_Set_DSP_Parame_save();
         //     msgBox.exec();
              Insert_Operation_Work_Record(1, "修改调试参数值4");
            }
          else
            {
              ZMessage msgBox;
              //QMessageBox msgBox;
              msgBox.setWindowTitle(tr("消息"));
              msgBox.setText(tr("参数下发失败!"));
              //msgBox.setIcon(QMessageBox::Information);
              //msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.exec();
            }
        }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_4);
      ui->Debug_Parameter_4->setText(qstr);
      break;
    }


    case 29:{//主电电压校准系数
        Input_value = ui->Main_Vol_Calibration_Factor->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Mains_Voltage_Calibration_Coefficient_Enum, Input_value, 1)){
         //       QMessageBox msgBox;
        //        msgBox.setWindowTitle(tr("消息"));
       //         msgBox.setText(tr("参数下发成功!"));
        //        msgBox.setIcon(QMessageBox::Information);
        //        msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Calibration_Coefficient = Input_value;
        //        msgBox.exec();
                System_Set_DSP_Parame_save();
                Insert_Operation_Work_Record(1, "修改主电电压校准系数");
            }else{
                ZMessage msgBox;
                //QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                //msgBox.setIcon(QMessageBox::Information);
                //msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Calibration_Coefficient);
        ui->Main_Vol_Calibration_Factor->setText(qstr);
        break;
    }
/*    case 30:{
        Input_value = ui->Main_Vol_Calibration_Factor_B->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Mains_Voltage_B_Calibration_Coefficient_Enum, Input_value, 1)){
        //        QMessageBox msgBox;
      //          msgBox.setWindowTitle(tr("消息"));
      //          msgBox.setText(tr("参数下发成功!"));
      //          msgBox.setIcon(QMessageBox::Information);
     //           msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_B_Calibration_Coefficient = Input_value;

       //         msgBox.exec();
                Insert_Operation_Work_Record(1, "修改主电B电压校准系数");
            }else{
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_B_Calibration_Coefficient);
        ui->Main_Vol_Calibration_Factor_B->setText(qstr);
        break;
    }
    case 31:{
        Input_value = ui->Main_Vol_Calibration_Factor_C->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Mains_Voltage_C_Calibration_Coefficient_Enum, Input_value, 1)){
     //           QMessageBox msgBox;
     //           msgBox.setWindowTitle(tr("消息"));
     //           msgBox.setText(tr("参数下发成功!"));
      //          msgBox.setIcon(QMessageBox::Information);
     //           msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_C_Calibration_Coefficient = Input_value;
      //          msgBox.exec();
                Insert_Operation_Work_Record(1, "修改应急电流校准系数");
            }else{
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_C_Calibration_Coefficient);
        ui->Main_Vol_Calibration_Factor_C->setText(qstr);
        break;
    }*/
    case 32:{
        Input_value = ui->Emer_Vol_Calibration_Factor->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Emergency_Voltage_Calibration_Coefficient_Enum, Input_value, 1)){
       //         QMessageBox msgBox;
       //         msgBox.setWindowTitle(tr("消息"));
       //         msgBox.setText(tr("参数下发成功!"));
      //          msgBox.setIcon(QMessageBox::Information);
        //        msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_Calibration_Coefficient = Input_value;
                System_Set_DSP_Parame_save();
          //      msgBox.exec();
                Insert_Operation_Work_Record(1, "修改应急电压校准系数");
            }else{
                ZMessage msgBox;
                //QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                //msgBox.setIcon(QMessageBox::Information);
                //msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_Calibration_Coefficient);
        ui->Emer_Vol_Calibration_Factor->setText(qstr);
        break;
    }
/*   case 33:{
        Input_value = ui->Emer_Vol_Calibration_Factor_B->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Emergency_Voltage_B_Calibration_Coefficient_Enum, Input_value, 1)){
        //        QMessageBox msgBox;
      //          msgBox.setWindowTitle(tr("消息"));
      //          msgBox.setText(tr("参数下发成功!"));
      //          msgBox.setIcon(QMessageBox::Information);
     //           msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_B_Calibration_Coefficient = Input_value;
             //   System_Set_DSP_Parame_save();
      //          msgBox.exec();
                Insert_Operation_Work_Record(1, "修改应急B电压校准系数");

            }else{
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_B_Calibration_Coefficient);
        ui->Emer_Vol_Calibration_Factor_B->setText(qstr);
        break;
    }
    case 34:{
        Input_value = ui->Emer_Vol_Calibration_Factor_C->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Emergency_Voltage_C_Calibration_Coefficient_Enum, Input_value, 1)){
      //          QMessageBox msgBox;
     //           msgBox.setWindowTitle(tr("消息"));
      //          msgBox.setText(tr("参数下发成功!"));
      //          msgBox.setIcon(QMessageBox::Information);
      //          msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_C_Calibration_Coefficient = Input_value;
               // System_Set_DSP_Parame_save();
   //             msgBox.exec();
                Insert_Operation_Work_Record(1, "修改应急C电压校准系数");

            }else{
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_C_Calibration_Coefficient);
        ui->Emer_Vol_Calibration_Factor_C->setText(qstr);
        break;
    }
*/
    case 35:{
        Input_value = ui->Emer_Curr_Calibration_Factor->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Emergency_Current_Calibration_Coefficient_Enum, Input_value, 1)){
       //         QMessageBox msgBox;
     //           msgBox.setWindowTitle(tr("消息"));
    //            msgBox.setText(tr("参数下发成功!"));
   //             msgBox.setIcon(QMessageBox::Information);
   //             msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Emergency_Current_Calibration_Coefficient = Input_value;
                System_Set_DSP_Parame_save();
   //             msgBox.exec();
                Insert_Operation_Work_Record(1, "修改应急电流校准系数");
            }else{
                ZMessage msgBox;
                //QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                //msgBox.setIcon(QMessageBox::Information);
                //msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Emergency_Current_Calibration_Coefficient);
        ui->Emer_Curr_Calibration_Factor->setText(qstr);
        break;
    }
   /* case 36:{
        Input_value = ui->Emer_Curr_Calibration_Factor_B->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Emergency_Current_B_Calibration_Coefficient_Enum, Input_value, 1)){
   //             QMessageBox msgBox;
    //            msgBox.setWindowTitle(tr("消息"));
    //            msgBox.setText(tr("参数下发成功!"));
    //            msgBox.setIcon(QMessageBox::Information);
    //            msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Emergency_Current_B_Calibration_Coefficient = Input_value;
            //    System_Set_DSP_Parame_save();
    //            msgBox.exec();
                Insert_Operation_Work_Record(1, "修改应急B电流校准系数");

            }else{
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Emergency_Current_B_Calibration_Coefficient);
        ui->Emer_Curr_Calibration_Factor_B->setText(qstr);
        break;
    }
    case 37:{
        Input_value = ui->Emer_Curr_Calibration_Factor_C->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Emergency_Current_C_Calibration_Coefficient_Enum, Input_value, 1)){
      //          QMessageBox msgBox;
     //           msgBox.setWindowTitle(tr("消息"));
     //           msgBox.setText(tr("参数下发成功!"));
    //            msgBox.setIcon(QMessageBox::Information);
      //          msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Emergency_Current_C_Calibration_Coefficient = Input_value;
                System_Set_DSP_Parame_save();
    //            msgBox.exec();
                Insert_Operation_Work_Record(1, "修改应急C电流校准系数");

            }else{
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Emergency_Current_C_Calibration_Coefficient,'f',0);
        ui->Emer_Curr_Calibration_Factor_C->setText(qstr);
        break;
    }

*/
    case 38:{
        Input_value = ui->Oupt_Vol_Calibration_Factor->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Ouput_Voltage_Calibration_Coefficient_Enum, Input_value, 1)){
       //         QMessageBox msgBox;
       //         msgBox.setWindowTitle(tr("消息"));
       //         msgBox.setText(tr("参数下发成功!"));
       //         msgBox.setIcon(QMessageBox::Information);
        //        msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Ouput_Voltage_Calibration_Coefficient = Input_value;
                System_Set_DSP_Parame_save();
       //         msgBox.exec();
                Insert_Operation_Work_Record(1, "修改输出电压校准系数");

            }else{
                ZMessage msgBox;
                //QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                //msgBox.setIcon(QMessageBox::Information);
                //msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Ouput_Voltage_Calibration_Coefficient);
        ui->Oupt_Vol_Calibration_Factor->setText(qstr);
        break;
    }
    case 39:{
        Input_value = ui->Oupt_Curr_Calibration_Factor->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Ouput_Current_Calibration_Coefficient_Enum, Input_value, 1)){
     //           QMessageBox msgBox;
    //            msgBox.setWindowTitle(tr("消息"));
    //            msgBox.setText(tr("参数下发成功!"));
     //           msgBox.setIcon(QMessageBox::Information);
       //         msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_Calibration_Coefficient = Input_value;
                System_Set_DSP_Parame_save();
      //          msgBox.exec();
                Insert_Operation_Work_Record(1, "修改输出电流校准系数");

            }else{
                ZMessage msgBox;
                //QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                //msgBox.setIcon(QMessageBox::Information);
                //msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_Calibration_Coefficient);
        ui->Oupt_Curr_Calibration_Factor->setText(qstr);
        break;
    }
  /*  case 40:{
        Input_value = ui->Oupt_Vol_Calibration_Factor_C->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, 46, Input_value, 1)){
    //            QMessageBox msgBox;
    //            msgBox.setWindowTitle(tr("消息"));
   //             msgBox.setText(tr("参数下发成功!"));
   //             msgBox.setIcon(QMessageBox::Information);
    //            msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Ouput_Voltage_C_Calibration_Coefficient = Input_value;
          //      System_Set_DSP_Parame_save();
   //             msgBox.exec();
                Insert_Operation_Work_Record(1, "修改输出C电压校准系数");

            }else{
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Ouput_Voltage_C_Calibration_Coefficient);
        ui->Oupt_Vol_Calibration_Factor_C->setText(qstr);
        break;
     }

    case 41:{
        Input_value = ui->Oupt_Curr_Calibration_Factor_A->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, 47, Input_value, 1)){
                //QMessageBox msgBox;
                //msgBox.setWindowTitle(tr("消息"));
               // msgBox.setText(tr("参数下发成功!"));
               // msgBox.setIcon(QMessageBox::Information);
               // msgBox.setStandardButtons(QMessageBox::Ok);
               Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_A_Calibration_Coefficient = Input_value;
           //     System_Set_DSP_Parame_save();
               // msgBox.exec();
                Insert_Operation_Work_Record(1, "修改输出A电流校准系数");

            }else{
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_A_Calibration_Coefficient,'f',0);
        ui->Oupt_Curr_Calibration_Factor_A->setText(qstr);
        break;
     }
    case 42:{
        Input_value = ui->Oupt_Curr_Calibration_Factor_B->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, 48, Input_value, 1)){
     //           QMessageBox msgBox;
   //             msgBox.setWindowTitle(tr("消息"));
   //             msgBox.setText(tr("参数下发成功!"));
   //             msgBox.setIcon(QMessageBox::Information);
 //               msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_B_Calibration_Coefficient = Input_value;
             //   System_Set_DSP_Parame_save();
   //             msgBox.exec();
                Insert_Operation_Work_Record(1, "修改输出B电流校准系数");

            }else{
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_B_Calibration_Coefficient,'f',0);
        ui->Oupt_Curr_Calibration_Factor_B->setText(qstr);
        break;
     }
    case 43:{
        Input_value = ui->Oupt_Curr_Calibration_Factor_C->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, 49, Input_value, 1)){
     //           QMessageBox msgBox;
    //            msgBox.setWindowTitle(tr("消息"));
   //             msgBox.setText(tr("参数下发成功!"));
   //             msgBox.setIcon(QMessageBox::Information);
  //              msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_C_Calibration_Coefficient = Input_value;
            //    System_Set_DSP_Parame_save();
   //             msgBox.exec();
                Insert_Operation_Work_Record(1, "修改输出C电流校准系数");

            }else{
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_C_Calibration_Coefficient);
        ui->Oupt_Curr_Calibration_Factor_C->setText(qstr);
        break;
     }
*/
    case 44:{
        Input_value = ui->Batt_Vol_Calibration_Factor->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Battery_Voltage_Calibration_Coefficient_Enum, Input_value, 1)){
     //           QMessageBox msgBox;
     //           msgBox.setWindowTitle(tr("消息"));
     //         msgBox.setText(tr("参数下发成功!"));
    //         msgBox.setIcon(QMessageBox::Information);
    //            msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Battery_Voltage_Calibration_Coefficient = Input_value;
                System_Set_DSP_Parame_save();
     //           msgBox.exec();
                Insert_Operation_Work_Record(1, "修改电池电压校准系数");

            }else{
                ZMessage msgBox;
                //QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                //msgBox.setIcon(QMessageBox::Information);
                //msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Battery_Voltage_Calibration_Coefficient);
        ui->Batt_Vol_Calibration_Factor->setText(qstr);
        break;
     }
    case 45:{
        Input_value = ui->Batt_Curr_Calibration_Factor->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Battery_Current_Calibration_Coefficient_Enum, Input_value, 1)){
     //           QMessageBox msgBox;
     //          msgBox.setWindowTitle(tr("消息"));
    //            msgBox.setText(tr("参数下发成功!"));
     //           msgBox.setIcon(QMessageBox::Information);
    //            msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Battery_Current_Calibration_Coefficient = Input_value;
                System_Set_DSP_Parame_save();
    //            msgBox.exec();
                Insert_Operation_Work_Record(1, "修改电池电流校准系数");

            }else{
                ZMessage msgBox;
                //QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                //msgBox.setIcon(QMessageBox::Information);
                //msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Battery_Current_Calibration_Coefficient);
        ui->Batt_Curr_Calibration_Factor->setText(qstr);
        break;
     }
    case 46:{
        Input_value = ui->Bus_Vol_Calibration_Factor->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Bus_Voltage_Calibration_Coefficient_Enum, Input_value, 1)){
     //           QMessageBox msgBox;
     //           msgBox.setWindowTitle(tr("消息"));
    //            msgBox.setText(tr("参数下发成功!"));
     //           msgBox.setIcon(QMessageBox::Information);
      //          msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Bus_Voltage_Calibration_Coefficient = Input_value;
                System_Set_DSP_Parame_save();
    //            msgBox.exec();
                Insert_Operation_Work_Record(1, "修改母线电压校准系数");

            }else{
                ZMessage msgBox;
                //QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                //msgBox.setIcon(QMessageBox::Information);
                //msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Bus_Voltage_Calibration_Coefficient);
        ui->Bus_Vol_Calibration_Factor->setText(qstr);
        break;
     }
    /*case 47:{
        Input_value = ui->Dc_Component_Calibration_Factor->text().toInt();
        if((Input_value > 4506)||(Input_value < 3686)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 3686-4506 之间的数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, 48, Input_value, 1)){
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发成功!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.SETDATA_InvDcVolt_Califactor = Input_value;
                msgBox.exec();
                Insert_Operation_Work_Record(1, "修改直流分量校准系数");

            }else{
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        //qstr.setNum((float)Globa.Send_Dsp_Debug_Param_Set.SETDATA_InvDcVolt_Califactor,'f',0);
        ui->Dc_Component_Calibration_Factor->setText(qstr);
        break;
     }*/
    case 48:{
        Input_value = ui->line_frameCapacitySet->text().toInt();
        if((Input_value > 500)||(Input_value < 5)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("请输入 5-500 之间的整数据!"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }else{
            if(Issuedequipment_parameters_06(DSPSTARTADDR, Equipment_Capacity_Enum, Input_value, 1)){
     //           QMessageBox msgBox;
     //           msgBox.setWindowTitle(tr("消息"));
    //            msgBox.setText(tr("参数下发成功!"));
     //           msgBox.setIcon(QMessageBox::Information);
      //          msgBox.setStandardButtons(QMessageBox::Ok);
                Globa.Send_Dsp_Debug_Param_Set.Equipment_Capacity = Input_value;
                System_Set_DSP_Parame_save();
    //            msgBox.exec();
                Insert_Operation_Work_Record(1, "修改设备容量");

            }else{
                ZMessage msgBox;
                //QMessageBox msgBox;
                msgBox.setWindowTitle(tr("消息"));
                msgBox.setText(tr("参数下发失败!"));
                //msgBox.setIcon(QMessageBox::Information);
                //msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Equipment_Capacity);
        ui->line_frameCapacitySet->setText(qstr);
        break;
     }
  case 60:{
    Input_value = (ui->Single_Battery_Low_limit->text().toFloat()*10); //单体电池低压告警点
    if((Input_value >120)|| (Input_value<100)){
      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("请输入10-12之间的数!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }else{

        G_Sigle_Baterry_Voltage_Alarm.UnderVolAlarm = Input_value;
        System_Sigle_Baterry_Voltage_Alarm_Para_save();
        Insert_Operation_Work_Record(1, "修改单体电池低压告警点");

    }
    qstr.setNum((float)G_Sigle_Baterry_Voltage_Alarm.UnderVolAlarm/10,'f',1);
    ui->Single_Battery_Low_limit->setText(qstr);
    break;
  }
  case 61:{
    Input_value = (ui->Single_Battery_High_limit->text().toFloat()*10); //单体电池高压告警点
    if((Input_value >150)|| (Input_value<120)){
      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("请输入12-15之间的数!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }else{

        G_Sigle_Baterry_Voltage_Alarm.OverVolAlarm = Input_value;
        System_Sigle_Baterry_Voltage_Alarm_Para_save();
        Insert_Operation_Work_Record(1, "修改单体电池高压告警点");

    }
    qstr.setNum((float)G_Sigle_Baterry_Voltage_Alarm.OverVolAlarm/10,'f',1);
    ui->Single_Battery_High_limit->setText(qstr);
    break;
  }

  case 70:{ //
      Input_value = ui->lineEdit->text().toInt();
      if((Input_value > 9)||(Input_value < 1)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入1~9之间的数据!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
        if(Issuedequipment_parameters_06(DSPSTARTADDR, 44, Input_value, 1)){
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发成功"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0] = Input_value;
          SETDATA_Machine_Code[0]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0];
          System_Set_DSP_Parame_save();
          msgBox.exec();
          Insert_Operation_Work_Record(1, "修改机身编码1");
        }else{
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发失败!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
        }
      qstr.setNum(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0]);
      ui->lineEdit->setText(qstr);
      break;
      }
    case 71:{ //
      Input_value = ui->lineEdit_2->text().toInt();
      if((Input_value > 9999)||(Input_value < 1)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入1~9999之间的数据!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
        if(Issuedequipment_parameters_06(DSPSTARTADDR, 45, Input_value, 1)){
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发成功"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1] = Input_value;
          SETDATA_Machine_Code[1]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1];
          System_Set_DSP_Parame_save();
          msgBox.exec();
          Insert_Operation_Work_Record(1, "修改机身编码2");
        }else{
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发失败!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
        }
      QString str1;
      str1= QString("%1").arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1],4,10,QChar('0'));
      ui->lineEdit_2->setText(str1);
      break;
      }
    case 72:
  { //
    Input_value = ui->lineEdit_3->text().toInt();
    Input_value_2 = ui->lineEdit_4->text().toInt();
    if((Input_value > 999999)||(Input_value < 1)){
      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      if((Input_value > 999999)||(Input_value < 1))
      {
          msgBox.setText(tr("请输入1~999999之间的数据!"));
      }


      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }else{
      if((Issuedequipment_parameters_06(DSPSTARTADDR, 46, Input_value/100, 1))&&\
              (Issuedequipment_parameters_06(DSPSTARTADDR, 47, (Input_value%100)*100+Input_value_2, 1))){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("参数下发成功"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2] = Input_value/100;
        Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3] = (Input_value%100)*100+Input_value_2;
        SETDATA_Machine_Code[2]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2];
        SETDATA_Machine_Code[3]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3];
        System_Set_DSP_Parame_save();
        msgBox.exec();
        Insert_Operation_Work_Record(1, "修改机身编码3 4");
      }else{
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("参数下发失败!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }
      }
    QString str1;
    str1= QString("%1").arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2]*100+Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3]/100,6,10,QChar('0'));
    ui->lineEdit_3->setText(str1);
    str1= QString("%1").arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3]%100,2,10,QChar('0'));
    ui->lineEdit_4->setText(str1);


    break;
    }


    case 73:
    { //
      Input_value = ui->lineEdit_3->text().toInt();
      Input_value_2 = ui->lineEdit_4->text().toInt();
      if((Input_value_2 < 1)||(Input_value_2>99)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
         msgBox.setText(tr("请输入1~99之间的数据!"));


        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }else{
        if((Issuedequipment_parameters_06(DSPSTARTADDR, 46, Input_value/100, 1))&&\
                (Issuedequipment_parameters_06(DSPSTARTADDR, 47, (Input_value%100)*100+Input_value_2, 1))){
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发成功"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2] = Input_value/100;
          Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3] = (Input_value%100)*100+Input_value_2;
          SETDATA_Machine_Code[2]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2];
          SETDATA_Machine_Code[3]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3];
          System_Set_DSP_Parame_save();
          msgBox.exec();
          Insert_Operation_Work_Record(1, "修改机身编码3 4");
        }else{
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("参数下发失败!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
        }
      QString str1;
      str1= QString("%1").arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2]*100+Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3]/100,6,10,QChar('0'));
      ui->lineEdit_3->setText(str1);
      str1= QString("%1").arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3]%100,2,10,QChar('0'));
      ui->lineEdit_4->setText(str1);


      break;
      }
  case 80:{ //

    if(ui->Machine_Code->text().count()!=13)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("请输入13位的机身编码!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        break;
    }
    printf("ui->Machine_Code->text().  is %ld\n",ui->Machine_Code->text().toInt());
    Input_value = ui->Machine_Code->text().left(5).toInt();
    Input_value_2 = ui->Machine_Code->text().remove(0,5).toInt();


      if((Issuedequipment_parameters_06(DSPSTARTADDR, 44, Input_value/10000, 1))\
              &&(Issuedequipment_parameters_06(DSPSTARTADDR, 45, Input_value%10000, 1))\
              &&(Issuedequipment_parameters_06(DSPSTARTADDR, 46, Input_value_2/10000, 1))\
              &&(Issuedequipment_parameters_06(DSPSTARTADDR, 47, Input_value_2%10000, 1))){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("参数下发成功"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0] = Input_value/10000;
        Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1] = Input_value%10000;
        Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2] = Input_value_2/10000;
        Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3] = Input_value_2%10000;

        SETDATA_Machine_Code[0]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0];
        SETDATA_Machine_Code[1]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1];
        SETDATA_Machine_Code[2]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2];
        SETDATA_Machine_Code[3]=Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3];

        System_Set_DSP_Parame_save();
        msgBox.exec();
        Insert_Operation_Work_Record(1, "修改机身编码");
      }else{
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("消息"));
        msgBox.setText(tr("参数下发失败!"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
      }

      QString str1;
      str1= QString("%1%2%3%4").arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0]).arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1],4,10,QChar('0')).arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2],4,10,QChar('0')).arg(Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3],4,10,QChar('0'));
      ui->Machine_Code->setText(str1);

    break;
    }


    default:
        break;
    }
}



void Maintain_Set::Maintain_Set_Parameter_Set_GetKeyValue(int key)
{
    if(!this->isHidden()){
        if(key == 10){
            Maintain_Set_Decimal_Point_falg = false;
            Maintain_Set_Parameter_SetEdit->setText("");
        }else if(key == 11){

            Maintain_Set_Parameter_Set_Init_1();

            Maintain_Set_Parameter_Set_Init_2();

        }else if(key == 12){
            Maintain_Set_Parameter_Set_KeyValue();
        }else if(key < 10) {
            Maintain_Set_Parameter_SetEdit->setText(Maintain_Set_Parameter_SetEdit->text() + QString("%1").arg(key));
        }else if(key == 13){
            if(!Maintain_Set_Decimal_Point_falg){
                Maintain_Set_Decimal_Point_falg = true;
                Maintain_Set_Parameter_SetEdit->setText(Maintain_Set_Parameter_SetEdit->text() + QString("."));
            }
        }else if(key == 14){
            Maintain_Set_Parameter_SetEdit->setText(Maintain_Set_Parameter_SetEdit->text().mid(0,Maintain_Set_Parameter_SetEdit->text().count() - 1));
        }
    }
}



//**************************干接点设置***************************
void Maintain_Set::addComBoxView()
{

  ui->comboBox_Dry_contact_7->setView(new QListView());
  ui->comboBox_Dry_contact_6->setView(new QListView());
  ui->comboBox_Dry_contact_8->setView(new QListView());
  ui->comboBox_Dry_contact_9->setView(new QListView());
  ui->comboBox_Dry_contact_11->setView(new QListView());
  ui->comboBox_Dry_contact_12->setView(new QListView());

  ui->password_enabled_combox->setView(new QListView());
  ui->combox_Battery_detecting->setView(new QListView());
  ui->combox_eps_modole->setView(new QListView());

  ui->combox_remote_control_1->setView(new QListView());
  ui->combox_remote_control_2->setView(new QListView());
  ui->combox_eps_switch_modole->setView(new QListView());

  ui->ComboxFreqChangeEnabel->setView(new QListView());
  ui->Comboxbatteryenable->setView(new QListView());
  ui->ComboxFreqChooseEnabel->setView(new QListView());

}


void Maintain_Set::addCombox_Item()
{
  ui->comboBox_Dry_contact_6->clear();
  ui->comboBox_Dry_contact_7->clear();
  ui->comboBox_Dry_contact_8->clear();
  ui->comboBox_Dry_contact_9->clear();
  ui->comboBox_Dry_contact_11->clear();
  ui->comboBox_Dry_contact_12->clear();

  ui->password_enabled_combox->clear();

  ui->combox_Battery_detecting->clear();
  ui->combox_eps_modole->clear();
  ui->combox_remote_control_1->clear();
  ui->combox_remote_control_2->clear();
  ui->combox_eps_switch_modole->clear();
  ui->ComboxFreqChangeEnabel->clear();
  ui->Comboxbatteryenable->clear();
  ui->ComboxFreqChooseEnabel->clear();

  ui->comboBox_Dry_contact_6->addItem(tr("无"));
  ui->comboBox_Dry_contact_6->addItem(tr("主电异常"));
  ui->comboBox_Dry_contact_6->addItem(tr("输出过载"));
  ui->comboBox_Dry_contact_6->addItem(tr("应急故障"));
  ui->comboBox_Dry_contact_6->addItem(tr("电池低压"));
  ui->comboBox_Dry_contact_6->addItem(tr("风机故障"));
  ui->comboBox_Dry_contact_6->addItem(tr("过温故障"));
  ui->comboBox_Dry_contact_6->addItem(tr("系统故障"));
  ui->comboBox_Dry_contact_6->addItem(tr("消防联动"));

  ui->comboBox_Dry_contact_7->addItem(tr("无"));
  ui->comboBox_Dry_contact_7->addItem(tr("主电异常"));
  ui->comboBox_Dry_contact_7->addItem(tr("输出过载"));
  ui->comboBox_Dry_contact_7->addItem(tr("应急故障"));
  ui->comboBox_Dry_contact_7->addItem(tr("电池低压"));
  ui->comboBox_Dry_contact_7->addItem(tr("风机故障"));
  ui->comboBox_Dry_contact_7->addItem(tr("过温故障"));
  ui->comboBox_Dry_contact_7->addItem(tr("系统故障"));
  ui->comboBox_Dry_contact_7->addItem(tr("消防联动"));

  ui->comboBox_Dry_contact_8->addItem(tr("无"));
  ui->comboBox_Dry_contact_8->addItem(tr("主电异常"));
  ui->comboBox_Dry_contact_8->addItem(tr("输出过载"));
  ui->comboBox_Dry_contact_8->addItem(tr("应急故障"));
  ui->comboBox_Dry_contact_8->addItem(tr("电池低压"));
  ui->comboBox_Dry_contact_8->addItem(tr("风机故障"));
  ui->comboBox_Dry_contact_8->addItem(tr("过温故障"));
  ui->comboBox_Dry_contact_8->addItem(tr("系统故障"));
  ui->comboBox_Dry_contact_8->addItem(tr("消防联动"));

  ui->comboBox_Dry_contact_9->addItem(tr("无"));
  ui->comboBox_Dry_contact_9->addItem(tr("主电异常"));
  ui->comboBox_Dry_contact_9->addItem(tr("输出过载"));
  ui->comboBox_Dry_contact_9->addItem(tr("应急故障"));
  ui->comboBox_Dry_contact_9->addItem(tr("电池低压"));
  ui->comboBox_Dry_contact_9->addItem(tr("风机故障"));
  ui->comboBox_Dry_contact_9->addItem(tr("过温故障"));
  ui->comboBox_Dry_contact_9->addItem(tr("系统故障"));
  ui->comboBox_Dry_contact_9->addItem(tr("消防联动"));

  ui->comboBox_Dry_contact_11->addItem(tr("无"));
  ui->comboBox_Dry_contact_11->addItem(tr("主电异常"));
  ui->comboBox_Dry_contact_11->addItem(tr("输出过载"));
  ui->comboBox_Dry_contact_11->addItem(tr("应急故障"));
  ui->comboBox_Dry_contact_11->addItem(tr("电池低压"));
  ui->comboBox_Dry_contact_11->addItem(tr("风机故障"));
  ui->comboBox_Dry_contact_11->addItem(tr("过温故障"));
  ui->comboBox_Dry_contact_11->addItem(tr("系统故障"));
  ui->comboBox_Dry_contact_11->addItem(tr("消防联动"));

  ui->comboBox_Dry_contact_12->addItem(tr("无"));
  ui->comboBox_Dry_contact_12->addItem(tr("主电异常"));
  ui->comboBox_Dry_contact_12->addItem(tr("输出过载"));
  ui->comboBox_Dry_contact_12->addItem(tr("应急故障"));
  ui->comboBox_Dry_contact_12->addItem(tr("电池低压"));
  ui->comboBox_Dry_contact_12->addItem(tr("风机故障"));
  ui->comboBox_Dry_contact_12->addItem(tr("过温故障"));
  ui->comboBox_Dry_contact_12->addItem(tr("系统故障"));
  ui->comboBox_Dry_contact_12->addItem(tr("消防联动"));


  ui->password_enabled_combox->addItem(tr("使能"));
  ui->password_enabled_combox->addItem(tr("不使能"));

  ui->combox_Battery_detecting->addItem(tr("开启"));
  ui->combox_Battery_detecting->addItem(tr("关闭"));

  ui->combox_eps_modole->addItem(tr("EA-D类型"));
  ui->combox_eps_modole->addItem(tr("FEPS类型"));
  ui->combox_eps_switch_modole->addItem(tr("慢切模式"));
  ui->combox_eps_switch_modole->addItem(tr("快切模式"));

  ui->combox_remote_control_1->addItem(tr("远程开机"));
  //ui->combox_remote_control_1->addItem(tr("远程转应急"));
  ui->combox_remote_control_2->addItem(tr("远程转应急"));
  ui->combox_remote_control_2->addItem(tr("远程关机"));
  ui->combox_remote_control_2->addItem(tr("电池开路检测"));

  ui->ComboxFreqChangeEnabel->addItem(tr("不使能"));
  ui->ComboxFreqChangeEnabel->addItem(tr("使能"));
  ui->Comboxbatteryenable->addItem(tr("不使能"));
  ui->Comboxbatteryenable->addItem(tr("使能"));


  ui->ComboxFreqChooseEnabel->addItem(tr("不使能"));
  ui->ComboxFreqChooseEnabel->addItem(tr("使能"));


  /*ui->combox_frameCapacitySet->addItem(tr("10KVA"));
  ui->combox_frameCapacitySet->addItem(tr("20KVA"));
  ui->combox_frameCapacitySet->addItem(tr("30KVA"));
  ui->combox_frameCapacitySet->addItem(tr("40KVA"));
  ui->combox_frameCapacitySet->addItem(tr("50KVA"));
  ui->combox_frameCapacitySet->addItem(tr("60KVA"));
  ui->combox_frameCapacitySet->addItem(tr("70KVA"));
  ui->combox_frameCapacitySet->addItem(tr("80KVA"));




*/
}

void Maintain_Set::Interface_Initialize()
{
  IsComParameterChanged = false;
  battery_groupset_mutex.lockForRead();
  ui->comboBox_Dry_contact_6->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[0]);
  ui->comboBox_Dry_contact_7->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[1]);
  ui->comboBox_Dry_contact_8->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[2]);
  ui->comboBox_Dry_contact_9->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[3]);
  ui->comboBox_Dry_contact_11->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[4]);
  ui->comboBox_Dry_contact_12->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[5]);
  /*ui->comboBox_Dry_contact_13->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[6]);
  ui->comboBox_Dry_contact_14->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[7]);*///del zzc 721
  ui->password_enabled_combox->setCurrentIndex(Globa.Battery_GroupParam_Set.userpassword_enable);
  ui->combox_Battery_detecting->setCurrentIndex(Globa.Battery_GroupParam_Set.battery_detecting_disable);
  ui->combox_eps_modole->setCurrentIndex(Globa.Send_Dsp_Debug_Param_Set.EPS_Work_Mode_Selection);

  ui->ComboxFreqChooseEnabel->setCurrentIndex((Globa.Send_Dsp_Debug_Param_Set.EnableControl>>1)&1);
  ui->ComboxFreqChangeEnabel->setCurrentIndex((Globa.Send_Dsp_Debug_Param_Set.EnableControl>>3)&1);

  ui->Comboxbatteryenable->setCurrentIndex(Globa.Curstomer_Param_Set.battertyenable);


  if(Globa.Send_Dsp_Debug_Param_Set.Fast_Slow_Switching==0xFF){
      ui->combox_eps_switch_modole->setCurrentIndex(1);
  }else{
      ui->combox_eps_switch_modole->setCurrentIndex(0);
    }
  /*if(Globa.Send_Dsp_Debug_Param_Set.Input_Control_1==0){
      ui->combox_remote_control_1->setCurrentIndex(1);
    }else{
      ui->combox_remote_control_1->setCurrentIndex(0);
    }*/
  //ui->combox_remote_control_1->setCurrentIndex(Globa.Send_Dsp_Debug_Param_Set.Input_Control_1);
  ui->combox_remote_control_1->setCurrentIndex(0);
  /*if(Globa.Send_Dsp_Debug_Param_Set.Input_Control_2==0){
      ui->combox_remote_control_2->setCurrentIndex(0);
    }else{
      ui->combox_remote_control_2->setCurrentIndex(1);
    }*/
  ui->combox_remote_control_2->setCurrentIndex(Globa.Send_Dsp_Debug_Param_Set.Input_Control_2);
  battery_groupset_mutex.unlock();
 // ui->combox_frameCapacitySet->setCurrentIndex(Globa.Send_Dsp_Debug_Param_Set.Equipment_Capacity/10);

  IsComParameterChanged = true;

  if(!Globa.Battery_GroupParam_Set.battery_detecting_disable){
      Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable=Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number;
     // ui->Batt_Group_Number->setEnabled(true);

    }else if(Globa.Battery_GroupParam_Set.battery_detecting_disable==1){
      Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable=0;
      //Globa.Battery_GroupParam_Set.Battery_GROUP=0;   //因为本身对group=0的处理就已经实现了相当于关闭电池巡检功能
      Globa.Eps_Param_Data.Current_Alarm_List_Battery_Refresh=1;//删除可能已经存在的通信异常
     // ui->Batt_Group_Number->setEnabled(false);
    }
  //ChoosebatteryInterface(0);
  //add_Group_Combox_item(Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable);

}

//************************电池参数**************************************
void Maintain_Set::on_Batt_Group_Number_selectionChanged()//电池节数
{
  Maintain_Set_Parameter_SetEdit = ui->Batt_Group_Number;
  Maintain_Set_Parameter_Set_No = 1;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();

}

void Maintain_Set::on_Batt_Capcity_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Batt_Capcity;
  Maintain_Set_Parameter_Set_No = 2;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

void Maintain_Set::on_Single_Batt_Discharge_End_V_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Single_Batt_Discharge_End_V;
  Maintain_Set_Parameter_Set_No = 3;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

//电池组数选择
void Maintain_Set::on_Battery_Group_Combox_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
     Battery_Group_No = index;
     ChoosebatteryInterface(0);
  }
}





void Maintain_Set::ChoosebatteryInterface(UINT32 number)
{
  batteryflag = number;

   battery_number(Battery_Group_No);
}

//********************计算每组电池节数
void Maintain_Set::battery_number(UINT32 group)
{
    Batt_Number[group] = 0;
    for(UINT32 j=0;j<Globa.Battery_Param_Set[group].Battery_Number;j++){
      Batt_Number[group] = Batt_Number[group] + Globa.Battery_Param_Set[group].Singlenumber[j];
    }
  }

//********************计算每组电池节数
void Maintain_Set::add_Group_Combox_item(UINT32 group)
{


}
//*************************保护参数*******************************
/*void Maintain_Set::on_Mains_Voltage_Range_selectionChanged()
{

  Maintain_Set_Parameter_SetEdit = ui->Mains_Voltage_Range;
  Maintain_Set_Parameter_Set_No = 6;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}*/

void Maintain_Set::on_Mains_Voltage_Top_limit_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Mains_Voltage_Top_limit;
  Maintain_Set_Parameter_Set_No = 6;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

void Maintain_Set::on_Mains_Voltage_Low_limit_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Mains_Voltage_Low_limit;
  Maintain_Set_Parameter_Set_No = 7;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}



/*void Maintain_Set::on_Mains_Frequence_Range_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Mains_Frequence_Range;
  Maintain_Set_Parameter_Set_No = 8;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}*/
void Maintain_Set::on_Mains_Frequence_Top_limit_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Mains_Frequence_Top_limit;
  Maintain_Set_Parameter_Set_No = 8;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

void Maintain_Set::on_Mains_Frequence_Low_limit_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Mains_Frequence_Low_limit;
  Maintain_Set_Parameter_Set_No = 9;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}


/*void Maintain_Set::on_Batt_Alar_LeftTime_2_selectionChanged()//应急剩余时间限值
{
  Maintain_Set_Parameter_SetEdit = ui->Batt_Alar_LeftTime_2;
  Maintain_Set_Parameter_Set_No = 10;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}*/

void Maintain_Set::on_Sys_Battery_Number_selectionChanged()
{

  Maintain_Set_Parameter_SetEdit = ui->Sys_Battery_Number;
  Maintain_Set_Parameter_Set_No = 11;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

/************************干接点对应故障***************************************/

void Maintain_Set::on_comboBox_Dry_contact_6_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
    battery_groupset_mutex.lockForWrite();
    Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[0] = index;
    System_Battery_GroupParam_Set_save();
    battery_groupset_mutex.unlock();
    Insert_Operation_Work_Record(1, "修改干接点1对应的故障");
  }
}

void Maintain_Set::on_comboBox_Dry_contact_7_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
    battery_groupset_mutex.lockForWrite();
    Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[1] = index;
    System_Battery_GroupParam_Set_save();
    battery_groupset_mutex.unlock();
    Insert_Operation_Work_Record(1, "修改干接点2对应的故障");
  }
}

void Maintain_Set::on_comboBox_Dry_contact_8_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
    battery_groupset_mutex.lockForWrite();
    Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[2] = index;
    System_Battery_GroupParam_Set_save();
    battery_groupset_mutex.unlock();
    Insert_Operation_Work_Record(1, "修改干接点3对应的故障");
  }
}

void Maintain_Set::on_comboBox_Dry_contact_9_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
    battery_groupset_mutex.lockForWrite();
    Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[3] = index;
    System_Battery_GroupParam_Set_save();
    battery_groupset_mutex.unlock();
    Insert_Operation_Work_Record(1, "修改干接点4对应的故障");
  }
}
void Maintain_Set::on_comboBox_Dry_contact_11_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
    battery_groupset_mutex.lockForWrite();
    Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[4] = index;
    System_Battery_GroupParam_Set_save();
    battery_groupset_mutex.unlock();
    Insert_Operation_Work_Record(1, "修改干接点5对应的故障");
  }
}

void Maintain_Set::on_comboBox_Dry_contact_12_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
    battery_groupset_mutex.lockForWrite();
    Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[5] = index;
    System_Battery_GroupParam_Set_save();
    battery_groupset_mutex.unlock();
    Insert_Operation_Work_Record(1, "修改干接点6对应的故障");
  }
}

/*void Maintain_Set::on_comboBox_Dry_contact_13_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
    battery_groupset_mutex.lockForWrite();
    Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[6] = index;
    System_Battery_GroupParam_Set_save();
    battery_groupset_mutex.unlock();
    Insert_Operation_Work_Record(1, "修改干接点7对应的故障");
  }
}
*///del by zzc 721
/*
void Maintain_Set::on_comboBox_Dry_contact_14_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
    battery_groupset_mutex.lockForWrite();
    Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[7] = index;
    System_Battery_GroupParam_Set_save();
    battery_groupset_mutex.unlock();
    Insert_Operation_Work_Record(1, "修改干接点8对应的故障");
  }
}*///del by zzc 721
/*void Maintain_Set::on_comboBox_Dry_contact_6_currentIndexChanged(int index)
{

  if(IsComParameterChanged){
    if(Issuedequipment_parameters_06(DSPSTARTADDR, 48, index, 1)){
    //  QMessageBox msgBox;
     // msgBox.setWindowTitle(tr("消息"));
    //  msgBox.setText(tr("参数下发成功!"));
    //  msgBox.setIcon(QMessageBox::Information);
     // msgBox.setStandardButtons(QMessageBox::Ok);
      stem_node_mutex.lockForWrite();
      Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[0] = index;
      System_Battery_GroupParam_Set_save();
      stem_node_mutex.unlock();
     // msgBox.exec();
      Insert_Operation_Work_Record(1, "修改干接点1对应的故障");
    }else{
      IsComParameterChanged = false;
      ui->comboBox_Dry_contact_6->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[0]);
      IsComParameterChanged = true;

      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
  }
}

void Maintain_Set::on_comboBox_Dry_contact_7_currentIndexChanged(int index)
{

  if(IsComParameterChanged){
    if(Issuedequipment_parameters_06(DSPSTARTADDR, 49, index, 1)){
    //  QMessageBox msgBox;
     // msgBox.setWindowTitle(tr("消息"));
    //  msgBox.setText(tr("参数下发成功!"));
    //  msgBox.setIcon(QMessageBox::Information);
     // msgBox.setStandardButtons(QMessageBox::Ok);
         stem_node_mutex.lockForWrite();
      Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[1] = index;
      System_Battery_GroupParam_Set_save();
       stem_node_mutex.unlock();
     // msgBox.exec();
      Insert_Operation_Work_Record(1, "修改干接点2对应的故障");
    }else{
      IsComParameterChanged = false;
      ui->comboBox_Dry_contact_7->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[1]);
      IsComParameterChanged = true;

      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
  }
}
void Maintain_Set::on_comboBox_Dry_contact_8_currentIndexChanged(int index)
{

  if(IsComParameterChanged){
    if(Issuedequipment_parameters_06(DSPSTARTADDR, 50, index, 1)){
    //  QMessageBox msgBox;
     // msgBox.setWindowTitle(tr("消息"));
    //  msgBox.setText(tr("参数下发成功!"));
    //  msgBox.setIcon(QMessageBox::Information);
     // msgBox.setStandardButtons(QMessageBox::Ok);
         stem_node_mutex.lockForWrite();
      Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[2] = index;
      System_Battery_GroupParam_Set_save();
      stem_node_mutex.unlock();
     // msgBox.exec();
      Insert_Operation_Work_Record(1, "修改干接点3对应的故障");
    }else{
      IsComParameterChanged = false;
      ui->comboBox_Dry_contact_8->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[2]);
      IsComParameterChanged = true;

      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
  }
}

void Maintain_Set::on_comboBox_Dry_contact_9_currentIndexChanged(int index)
{

  if(IsComParameterChanged){
    if(Issuedequipment_parameters_06(DSPSTARTADDR, 51, index, 1)){
    //  QMessageBox msgBox;
     // msgBox.setWindowTitle(tr("消息"));
    //  msgBox.setText(tr("参数下发成功!"));
    //  msgBox.setIcon(QMessageBox::Information);
     // msgBox.setStandardButtons(QMessageBox::Ok);
         stem_node_mutex.lockForWrite();
      Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[3] = index;
      System_Battery_GroupParam_Set_save();
       stem_node_mutex.unlock();
     // msgBox.exec();
      Insert_Operation_Work_Record(1, "修改干接点4对应的故障");
    }else{
      IsComParameterChanged = false;
      ui->comboBox_Dry_contact_9->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[3]);
      IsComParameterChanged = true;

      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
  }
}

void Maintain_Set::on_comboBox_Dry_contact_11_currentIndexChanged(int index)
{

  if(IsComParameterChanged){
    if(Issuedequipment_parameters_06(DSPSTARTADDR, 52, index, 1)){
    //  QMessageBox msgBox;
     // msgBox.setWindowTitle(tr("消息"));
    //  msgBox.setText(tr("参数下发成功!"));
    //  msgBox.setIcon(QMessageBox::Information);
     // msgBox.setStandardButtons(QMessageBox::Ok);
         stem_node_mutex.lockForWrite();
      Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[4] = index;
      System_Battery_GroupParam_Set_save();
      stem_node_mutex.unlock();
     // msgBox.exec();
      Insert_Operation_Work_Record(1, "修改干接点5对应的故障");
    }else{
      IsComParameterChanged = false;
      ui->comboBox_Dry_contact_11->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[4]);
      IsComParameterChanged = true;

      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
  }
}

void Maintain_Set::on_comboBox_Dry_contact_12_currentIndexChanged(int index)
{

  if(IsComParameterChanged){
    if(Issuedequipment_parameters_06(DSPSTARTADDR, 53, index, 1)){
    //  QMessageBox msgBox;
     // msgBox.setWindowTitle(tr("消息"));
    //  msgBox.setText(tr("参数下发成功!"));
    //  msgBox.setIcon(QMessageBox::Information);
     // msgBox.setStandardButtons(QMessageBox::Ok);
         stem_node_mutex.lockForWrite();
      Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[5] = index;
      System_Battery_GroupParam_Set_save();
       stem_node_mutex.unlock();
     // msgBox.exec();
      Insert_Operation_Work_Record(1, "修改干接点6对应的故障");
    }else{
      IsComParameterChanged = false;
      ui->comboBox_Dry_contact_12->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[5]);
      IsComParameterChanged = true;

      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
  }
}

void Maintain_Set::on_comboBox_Dry_contact_13_currentIndexChanged(int index)
{

  if(IsComParameterChanged){
    if(Issuedequipment_parameters_06(DSPSTARTADDR, 54, index, 1)){
    //  QMessageBox msgBox;
     // msgBox.setWindowTitle(tr("消息"));
    //  msgBox.setText(tr("参数下发成功!"));
    //  msgBox.setIcon(QMessageBox::Information);
     // msgBox.setStandardButtons(QMessageBox::Ok);
         stem_node_mutex.lockForWrite();
      Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[6] = index;
      System_Battery_GroupParam_Set_save();
       stem_node_mutex.unlock();
     // msgBox.exec();
      Insert_Operation_Work_Record(1, "修改干接点7对应的故障");
    }else{
      IsComParameterChanged = false;
      ui->comboBox_Dry_contact_13->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[6]);
      IsComParameterChanged = true;

      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
  }
}

void Maintain_Set::on_comboBox_Dry_contact_14_currentIndexChanged(int index)
{

  if(IsComParameterChanged){
    if(Issuedequipment_parameters_06(DSPSTARTADDR, 55, index, 1)){
    //  QMessageBox msgBox;
     // msgBox.setWindowTitle(tr("消息"));
    //  msgBox.setText(tr("参数下发成功!"));
    //  msgBox.setIcon(QMessageBox::Information);
     // msgBox.setStandardButtons(QMessageBox::Ok);
         stem_node_mutex.lockForWrite();
      Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[7] = index;
      System_Battery_GroupParam_Set_save();
       stem_node_mutex.unlock();
     // msgBox.exec();
      Insert_Operation_Work_Record(1, "修改干接点8对应的故障");
    }else{
      IsComParameterChanged = false;
      ui->comboBox_Dry_contact_14->setCurrentIndex(Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[7]);
      IsComParameterChanged = true;

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

//机架容量设置
/*void Maintain_Set::on_combox_frameCapacitySet_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
    if(Issuedequipment_parameters_06(DSPSTARTADDR,0, 10*(index + 1), 1)){
     QMessageBox msgBox;
     msgBox.setWindowTitle(tr("消息"));
     msgBox.setText(tr("参数下发成功!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
    //  Globa.Send_Dsp_Debug_Param_Set.SETDATA_RackCapacity = index + 4;
      System_Set_DSP_Parame_save();
      msgBox.exec();
      Insert_Operation_Work_Record(1, "修改机架容量");
    }else{
      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      IsComParameterChanged = false;
      ui->combox_frameCapacitySet->setCurrentIndex(Globa.Send_Dsp_Debug_Param_Set.Equipment_Capacity/10 - 1);
      IsComParameterChanged = true;
      msgBox.exec();
    }
  }
}*/

void Maintain_Set::on_password_enabled_combox_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
    Globa.Battery_GroupParam_Set.userpassword_enable = index;
    Usr_Engir_Flag = (index==1)?0:1;
    System_Battery_GroupParam_Set_save();//系统参数设置（管理员）
    Insert_Operation_Work_Record(1, "修改密码启用功能");
  }
}

/*void Maintain_Set::on_btn_restore_2_clicked()
{

  QMessageBox msgBox;
  msgBox.setWindowTitle(tr("警告"));
  msgBox.setText(tr("是否恢复出厂默认设置?"));
  msgBox.setIcon(QMessageBox::Warning);
  msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
  if(msgBox.exec() == QMessageBox::Ok){
    QFile::remove(F_Battery_Param_Set);
    QFile::remove(F_BAK_Battery_Param_Set);
    QFile::remove(F_Battery_GroupParam_Set);
    QFile::remove(F_BAK_Battery_GroupParam_Set);
    QFile::remove(F_SET_DSP_PARAMETE);
    QFile::remove(F_BAK_SET_DSP_PARAMETE);//加上DSP参数

    msgBox.setText(tr("请重新启动!"));
    if(msgBox.exec() == QMessageBox::Ok){
      Insert_Operation_Work_Record(1, "恢复维护出厂默认设置");
      Reboot Reboot_View;
      Reboot_View.exec();
      //system("reboot");
    }
  }
}*/


//********************调试变量****************************
void Maintain_Set::on_Variable_Address_1_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Variable_Address_1;
  Maintain_Set_Parameter_Set_No = 25;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();

}

void Maintain_Set::on_Variable_Address_2_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Variable_Address_2;
  Maintain_Set_Parameter_Set_No = 26;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();

}

void Maintain_Set::on_Variable_Address_3_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Variable_Address_3;
  Maintain_Set_Parameter_Set_No = 27;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}


void Maintain_Set::on_Variable_Address_4_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Variable_Address_4;
  Maintain_Set_Parameter_Set_No = 28;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

//**********************调试参数*************************
void Maintain_Set::on_Debug_Parameter_1_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Debug_Parameter_1;
  Maintain_Set_Parameter_Set_No = 21;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

void Maintain_Set::on_Debug_Parameter_2_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Debug_Parameter_2;
  Maintain_Set_Parameter_Set_No = 22;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

void Maintain_Set::on_Debug_Parameter_3_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Debug_Parameter_3;
  Maintain_Set_Parameter_Set_No = 23;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

void Maintain_Set::on_Debug_Parameter_4_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Debug_Parameter_4;
  Maintain_Set_Parameter_Set_No = 24;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

void Maintain_Set::on_btn_test_clicked()//进入测试界面  test
{
   emit Changed_Current_View(TEST_DATA_DISPLAY, 0, 0, 0);
}

//*************校准参数********************************
void Maintain_Set::on_Main_Vol_Calibration_Factor_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Main_Vol_Calibration_Factor;
  Maintain_Set_Parameter_Set_No = 29;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}
/*
void Maintain_Set::on_Main_Vol_Calibration_Factor_B_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Main_Vol_Calibration_Factor_B;
  Maintain_Set_Parameter_Set_No = 30;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

void Maintain_Set::on_Main_Vol_Calibration_Factor_C_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Main_Vol_Calibration_Factor_C;
  Maintain_Set_Parameter_Set_No = 31;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}
*/
void Maintain_Set::on_Emer_Vol_Calibration_Factor_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Emer_Vol_Calibration_Factor;
  Maintain_Set_Parameter_Set_No = 32;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}
/*
void Maintain_Set::on_Emer_Vol_Calibration_Factor_B_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Emer_Vol_Calibration_Factor_B;
  Maintain_Set_Parameter_Set_No = 33;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

void Maintain_Set::on_Emer_Vol_Calibration_Factor_C_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Emer_Vol_Calibration_Factor_C;
  Maintain_Set_Parameter_Set_No = 34;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}
*/
void Maintain_Set::on_Emer_Curr_Calibration_Factor_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Emer_Curr_Calibration_Factor;
  Maintain_Set_Parameter_Set_No = 35;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}
/*
void Maintain_Set::on_Emer_Curr_Calibration_Factor_B_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Emer_Curr_Calibration_Factor_B;
  Maintain_Set_Parameter_Set_No = 36;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

void Maintain_Set::on_Emer_Curr_Calibration_Factor_C_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Emer_Curr_Calibration_Factor_C;
  Maintain_Set_Parameter_Set_No = 37;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}
*/
void Maintain_Set::on_Oupt_Vol_Calibration_Factor_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Oupt_Vol_Calibration_Factor;
  Maintain_Set_Parameter_Set_No = 38;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

void Maintain_Set::on_Oupt_Curr_Calibration_Factor_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Oupt_Curr_Calibration_Factor;
  Maintain_Set_Parameter_Set_No = 39;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

/*void Maintain_Set::on_Oupt_Vol_Calibration_Factor_C_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Oupt_Vol_Calibration_Factor_C;
  Maintain_Set_Parameter_Set_No = 40;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}*/

/*void Maintain_Set::on_Oupt_Curr_Calibration_Factor_A_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Oupt_Curr_Calibration_Factor_A;
  Maintain_Set_Parameter_Set_No = 41;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}
*/

/*void Maintain_Set::on_Oupt_Curr_Calibration_Factor_B_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Oupt_Curr_Calibration_Factor_B;
   Maintain_Set_Parameter_Set_No = 42;
   Maintain_Set_Parameter_SetEdit->setText("");
   Maintain_Set_Decimal_Point_falg = false;
   emit keyboard_show();

}*/
/*
void Maintain_Set::on_Oupt_Curr_Calibration_Factor_C_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->Oupt_Curr_Calibration_Factor_C;
  Maintain_Set_Parameter_Set_No = 43;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}

*/
//电池电压校准系数
void Maintain_Set::on_Batt_Vol_Calibration_Factor_selectionChanged()
{
    Maintain_Set_Parameter_SetEdit = ui->Batt_Vol_Calibration_Factor;
    Maintain_Set_Parameter_Set_No = 44;
    Maintain_Set_Parameter_SetEdit->setText("");
    Maintain_Set_Decimal_Point_falg = false;
    emit keyboard_show();
}
//电池电流校准系数
void Maintain_Set::on_Batt_Curr_Calibration_Factor_selectionChanged()
{
    Maintain_Set_Parameter_SetEdit = ui->Batt_Curr_Calibration_Factor;
    Maintain_Set_Parameter_Set_No = 45;
    Maintain_Set_Parameter_SetEdit->setText("");
    Maintain_Set_Decimal_Point_falg = false;
    emit keyboard_show();
}

void Maintain_Set::on_Bus_Vol_Calibration_Factor_selectionChanged()
{
    Maintain_Set_Parameter_SetEdit = ui->Bus_Vol_Calibration_Factor;
    Maintain_Set_Parameter_Set_No = 46;
    Maintain_Set_Parameter_SetEdit->setText("");
    Maintain_Set_Decimal_Point_falg = false;
    emit keyboard_show();
}
/*
void Maintain_Set::on_Dc_Component_Calibration_Factor_selectionChanged()
{
    Maintain_Set_Parameter_SetEdit = ui->Dc_Component_Calibration_Factor;
    Maintain_Set_Parameter_Set_No = 47;
    Maintain_Set_Parameter_SetEdit->setText("");
    Maintain_Set_Decimal_Point_falg = false;
    emit keyboard_show();
}
*/




void Maintain_Set::on_line_frameCapacitySet_selectionChanged()
{
  Maintain_Set_Parameter_SetEdit = ui->line_frameCapacitySet;
  Maintain_Set_Parameter_Set_No = 48;
  Maintain_Set_Parameter_SetEdit->setText("");
  Maintain_Set_Decimal_Point_falg = false;
  emit keyboard_show();
}



void Maintain_Set::on_combox_eps_modole_currentIndexChanged(int index)
{
  if(IsComParameterChanged){

  if(Issuedequipment_parameters_06(DSPSTARTADDR, EPS_Work_Mode_Selection_Enum, index, 1)){

     Globa.Send_Dsp_Debug_Param_Set.EPS_Work_Mode_Selection = index;
     System_Set_DSP_Parame_save();

     if(index==0)
     {
         Insert_Operation_Work_Record(1, "修改负载类型为EA-D型");
     }
     else if(index==1)
     {
         Insert_Operation_Work_Record(1, "修改负载类型为FEPS型");
     }

    }else{
      ZMessage msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.exec();
      IsComParameterChanged = false;
      ui->combox_eps_modole->setCurrentIndex(Globa.Send_Dsp_Debug_Param_Set.EPS_Work_Mode_Selection);
      IsComParameterChanged = true;
    }
  }
}
void Maintain_Set::on_combox_eps_switch_modole_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
  UINT32 fast_slow_mode=0;
  if(index==1)
    {
      fast_slow_mode=0xFF;
    }
  if(Issuedequipment_parameters_06(DSPSTARTADDR, Fast_Slow_Switch_Enum, fast_slow_mode, 1)){
  //    QMessageBox msgBox;
 //     msgBox.setWindowTitle(tr("消息"));
 //     msgBox.setText(tr("参数下发成功!"));
 //     msgBox.setIcon(QMessageBox::Information);
//      msgBox.setStandardButtons(QMessageBox::Ok);
   /*   if(Globa.Send_Dsp_Debug_Param_Set.EPS_Work_Mode_Selection!=index){
          Load_Stye_Change=index+1;
        }*/
     Globa.Send_Dsp_Debug_Param_Set.Fast_Slow_Switching = fast_slow_mode;
     System_Set_DSP_Parame_save();
//      msgBox.exec();
      //Insert_Operation_Work_Record(1, "修改EPS切换模式");
      if(fast_slow_mode==0)
        {
          Insert_Operation_Work_Record(1, "修改EPS切换模式为慢切模式");
        }
      else
        {
          Insert_Operation_Work_Record(1, "修改EPS切换模式为快切模式");
        }
    }else{
      ZMessage msgBox;
      //QMessageBox msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      //msgBox.setIcon(QMessageBox::Information);
      //msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
      IsComParameterChanged = false;
      ui->combox_eps_switch_modole->setCurrentIndex(index);
      IsComParameterChanged = true;
    }
  }
}

void Maintain_Set::on_combox_remote_control_1_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
  UINT32 remote_control=0;
  /*if(index==0)
    {
      remote_control=0xFF;
    }*/
  if(Issuedequipment_parameters_06(DSPSTARTADDR, Input_Control_1_Enum, remote_control, 1)){
     Globa.Send_Dsp_Debug_Param_Set.Input_Control_1  = remote_control;
     System_Set_DSP_Parame_save();
     Insert_Operation_Work_Record(1, "修改输入干接点1为远程开机");
     /*if(remote_control==0xFF){
         Insert_Operation_Work_Record(1, "修改输入干接点1为远程开机");
       }else{
         Insert_Operation_Work_Record(1, "修改输入干接点1为远程转应急");
       }
*/
    }else{
      ZMessage msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.exec();
      IsComParameterChanged = false;
      ui->combox_remote_control_1->setCurrentIndex(index);
      IsComParameterChanged = true;
    }
  }
}


void Maintain_Set::on_combox_remote_control_2_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
  UINT32 remote_control=0;
  /*if(index==1)
    {
      remote_control=0xFF;
    }*/
  remote_control=index;
  if(Issuedequipment_parameters_06(DSPSTARTADDR, Input_Control_2_Enum, remote_control, 1)){
     Globa.Send_Dsp_Debug_Param_Set.Input_Control_2  = remote_control;
     System_Set_DSP_Parame_save();
     if(remote_control==0){
         Insert_Operation_Work_Record(1, "修改输入干接点2为远程关机");
       }
     else if(remote_control==1){
         Insert_Operation_Work_Record(1, "修改输入干接点2为远程转应急");
       }else{
         Insert_Operation_Work_Record(1, "修改输入干接点2为电池开路检测");
       }

    }else{
      ZMessage msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.exec();
      IsComParameterChanged = false;
      ui->combox_remote_control_2->setCurrentIndex(index);
      IsComParameterChanged = true;
    }
  }
}



void Maintain_Set::on_listWidget_currentRowChanged(int currentRow)
{
  GPIO_SYS_BUZZ_ON;//干接点控制接口函数

  usleep(30000);
  GPIO_SYS_BUZZ_OFF;
}

void Maintain_Set::on_listWidget_2_currentRowChanged(int currentRow)
{
  GPIO_SYS_BUZZ_ON;//干接点控制接口函数

  usleep(30000);
  GPIO_SYS_BUZZ_OFF;
}

void Maintain_Set::on_combox_Battery_detecting_currentIndexChanged(int index)
{
  if(IsComParameterChanged){

    Globa.Battery_GroupParam_Set.battery_detecting_disable= index;

    if(!Globa.Battery_GroupParam_Set.battery_detecting_disable){
        Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable=Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number;

        Insert_Operation_Work_Record(1, "开启电池巡检功能");

        ui->Single_Battery_High_limit->setEnabled(true);
        ui->Single_Battery_Low_limit->setEnabled(true);
      }else if(Globa.Battery_GroupParam_Set.battery_detecting_disable==1){
        //Globa.Battery_GroupParam_Set.Battery_GROUP=0;   //因为本身对group=0的处理就已经实现了相当于关闭电池巡检功能
        //ui->Batt_Group_Number->setEnabled(false);
        Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable=0;
        Globa.Eps_Param_Data.Current_Alarm_List_Battery_Refresh=1;
        Insert_Operation_Work_Record(1, "关闭电池巡检功能");

        ui->Single_Battery_High_limit->setEnabled(false);
        ui->Single_Battery_Low_limit->setEnabled(false);
      }
    System_Battery_GroupParam_Set_save();//系统参数设置（管理员）
    ChoosebatteryInterface(0);
   add_Group_Combox_item(Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable);

  }
}









void Maintain_Set::on_Single_Battery_Low_limit_selectionChanged()
{
    Maintain_Set_Parameter_SetEdit = ui->Single_Battery_Low_limit;
    Maintain_Set_Parameter_Set_No = 60;
    Maintain_Set_Parameter_SetEdit->setText("");
    Maintain_Set_Decimal_Point_falg = false;
    emit keyboard_show();
}

void Maintain_Set::on_Single_Battery_High_limit_selectionChanged()
{
    Maintain_Set_Parameter_SetEdit = ui->Single_Battery_High_limit;
    Maintain_Set_Parameter_Set_No = 61;
    Maintain_Set_Parameter_SetEdit->setText("");
    Maintain_Set_Decimal_Point_falg = false;
    emit keyboard_show();
}
void Maintain_Set::on_lineEdit_selectionChanged()
{
    Maintain_Set_Parameter_SetEdit = ui->lineEdit;
    Maintain_Set_Parameter_Set_No = 70;
    Maintain_Set_Parameter_SetEdit->setText("");
    Maintain_Set_Decimal_Point_falg = false;
    emit keyboard_show();
}

void Maintain_Set::on_lineEdit_2_selectionChanged()
{
    Maintain_Set_Parameter_SetEdit = ui->lineEdit_2;
    Maintain_Set_Parameter_Set_No = 71;
    Maintain_Set_Parameter_SetEdit->setText("");
    Maintain_Set_Decimal_Point_falg = false;
    emit keyboard_show();
}

void Maintain_Set::on_lineEdit_3_selectionChanged()
{
    Maintain_Set_Parameter_SetEdit = ui->lineEdit_3;
    Maintain_Set_Parameter_Set_No = 72;
    Maintain_Set_Parameter_SetEdit->setText("");
    Maintain_Set_Decimal_Point_falg = false;
    emit keyboard_show();
}

void Maintain_Set::on_lineEdit_4_selectionChanged()
{
    Maintain_Set_Parameter_SetEdit = ui->lineEdit_4;
    Maintain_Set_Parameter_Set_No = 73;
    Maintain_Set_Parameter_SetEdit->setText("");
    Maintain_Set_Decimal_Point_falg = false;
    emit keyboard_show();
}

void Maintain_Set::on_Machine_Code_selectionChanged()
{
    Maintain_Set_Parameter_SetEdit = ui->Machine_Code;
    Maintain_Set_Parameter_Set_No = 80;
    Maintain_Set_Parameter_SetEdit->setText("");
    Maintain_Set_Decimal_Point_falg = false;
    emit keyboard_show();
}

void Maintain_Set::on_ComboxFreqChooseEnabel_currentIndexChanged(int index)
{
  if(IsComParameterChanged){

     UINT32 tmp=0;
     tmp=Globa.Send_Dsp_Debug_Param_Set.EnableControl&(0xFFFD)|(index<<1);


  if(Issuedequipment_parameters_06(DSPSTARTADDR, Enable_Control_Enum, tmp, 1)){

     Globa.Send_Dsp_Debug_Param_Set.EnableControl = tmp;
     System_Set_DSP_Parame_save();
      if(index==1)
      {
          Insert_Operation_Work_Record(1, "使能主电频率检测");
      }
      else if(index==0)
      {
          Insert_Operation_Work_Record(1, "关闭主电频率检测使能");
      }

    }else{
      ZMessage msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.exec();
      IsComParameterChanged = false;
      ui->ComboxFreqChooseEnabel->setCurrentIndex((Globa.Send_Dsp_Debug_Param_Set.EnableControl>>1)&1);
      IsComParameterChanged = true;
    }
  }
}

void Maintain_Set::on_ComboxFreqChangeEnabel_currentIndexChanged(int index)
{
  if(IsComParameterChanged){

     UINT32 tmp=0;
     tmp=Globa.Send_Dsp_Debug_Param_Set.EnableControl&(0xFFF7)|(index<<3);


  if(Issuedequipment_parameters_06(DSPSTARTADDR, Enable_Control_Enum, tmp, 1)){

     Globa.Send_Dsp_Debug_Param_Set.EnableControl = tmp;
     System_Set_DSP_Parame_save();
      if(index==1)
      {
          Insert_Operation_Work_Record(1, "使能变频启动");
      }
      else if(index==0)
      {
          Insert_Operation_Work_Record(1, "关闭变频启动使能");
      }

    }else{
      ZMessage msgBox;
      msgBox.setWindowTitle(tr("消息"));
      msgBox.setText(tr("参数下发失败!"));
      msgBox.exec();
      IsComParameterChanged = false;
      ui->ComboxFreqChangeEnabel->setCurrentIndex((Globa.Send_Dsp_Debug_Param_Set.EnableControl>>3)&1);
      IsComParameterChanged = true;
    }
  }
}

void Maintain_Set::on_Comboxbatteryenable_currentIndexChanged(int index)
{
  if(IsComParameterChanged){
    Globa.Curstomer_Param_Set.battertyenable= index;
    System_Curstomer_Param_Set_save();
    Insert_Operation_Work_Record(1, "修改电池剩余容量使能");
  }
}
