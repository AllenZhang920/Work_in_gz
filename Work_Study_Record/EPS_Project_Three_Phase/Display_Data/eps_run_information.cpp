#include "eps_run_information.h"
#include "ui_eps_run_information.h"

Eps_Run_Information::Eps_Run_Information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Eps_Run_Information)
{
    ui->setupUi(this);
    pagenum=0;
    ui->upButton->setObjectName("mButton");
    ui->downButton->setObjectName("mButton");

    ui->label_main->setVisible(false);
    ui->label_standby->setVisible(false);
    tableview_init();


}

Eps_Run_Information::~Eps_Run_Information()
{
    delete ui;
}
void Eps_Run_Information::tableview_init()
{
  pagenum=0;
  ui->upButton->setEnabled(false);
  if(Globa.Curstomer_Param_Set.Distribution_Branches_Number<=32)
  {
      ui->downButton->setVisible(false);
      ui->upButton->setVisible(false);
  }
  else
  {
      ui->downButton->setVisible(true);
      ui->upButton->setVisible(true);
      ui->downButton->setEnabled(true);
  }

  if(Globa.Curstomer_Param_Set.battertyenable==0)
  {
      ui->label_42->setVisible(false);
      ui->Residual_Capacity->setVisible(false);
      ui->label_43->setVisible(false);
      ui->Residual_Time->setVisible(false);
  }
  else
  {
      ui->label_42->setVisible(true);
      ui->Residual_Capacity->setVisible(true);
      ui->label_43->setVisible(true);
      ui->Residual_Time->setVisible(true);
  }

  ui->tabWidget_1->setStyleSheet("QTabBar::tab{width:100}");
  ui->tableWidget->clearContents();

  QHeaderView *headerView = ui->tableWidget->verticalHeader();
  headerView->setHidden(true); // 消除序号
  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 使其内部框只显示而不能输入
  ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行
  ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //单击选择一行
  ui->tableWidget->setAlternatingRowColors(true); //设置隔一行变颜色

  ui->tableWidget->setColumnWidth(0,85);
  ui->tableWidget->setColumnWidth(1,80);
  ui->tableWidget->setColumnWidth(2,85);
  ui->tableWidget->setColumnWidth(3,80);
  ui->tableWidget->setColumnWidth(4,85);
  ui->tableWidget->setColumnWidth(5,80);
  ui->tableWidget->setColumnWidth(6,85);
  ui->tableWidget->setColumnWidth(7,80);
  ui->tableWidget->horizontalHeader()->setClickable(false);
  ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);//列表不能移动
 //ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

  ui->tableWidget->setRowCount(8);//设置行
  for(int i = 0;i<8;i++){
    ui->tableWidget->setItem(i, 0, new QTableWidgetItem());
    ui->tableWidget->setItem(i, 1, new QTableWidgetItem());
    ui->tableWidget->setItem(i, 2, new QTableWidgetItem());
    ui->tableWidget->setItem(i, 3, new QTableWidgetItem());
    ui->tableWidget->setItem(i, 4, new QTableWidgetItem());
    ui->tableWidget->setItem(i, 5, new QTableWidgetItem());
    ui->tableWidget->setItem(i, 6, new QTableWidgetItem());
    ui->tableWidget->setItem(i, 7, new QTableWidgetItem());
 //ui->tableWidget->item(i,0)->setTextAlignment(Qt::AlignCenter);
   // ui->tableWidget->item(i,1)->setTextAlignment(Qt::AlignCenter);
   // ui->tableWidget->item(i,2)->setTextAlignment(Qt::AlignCenter);
   // ui->tableWidget->item(i,3)->setTextAlignment(Qt::AlignCenter);
   // ui->tableWidget->item(i,4)->setTextAlignment(Qt::AlignCenter);
  //  ui->tableWidget->item(i,5)->setTextAlignment(Qt::AlignCenter);
  }

  Eps_Run_Tableview_1();
  Eps_Run_Tableview_2();

  Eps_Run_Tableview_3();

  Eps_Run_Tableview_4();

  Eps_Run_Tableview_5();

  Eps_Run_Information_init();

}
/*void Eps_Run_Information::Eps_Run_Information_init()
{if(ui->tabWidget_1->indexOf(ui->tab_3)==-1)
    {printf("dd\n");}
  if(Globa.Curstomer_Param_Set.ATS_Configure_Flage == 0){
    //ui->tabWidget_1->setTabEnabled(4,false);
    ui->tabWidget_1->removeTab(4);
     // ui->tabWidget_1->removeTab(ui->tab_4);

  }else{
   // ui->tabWidget_1->setTabEnabled(4,true);
    ui->tabWidget_1->addTab(ui->tab_4,QString("ATS信息"));
  }

}*/

void Eps_Run_Information::Eps_Run_Information_init()
{
 if(Globa.Curstomer_Param_Set.Distribution_Configure_Flage == 0){
     if(ui->tabWidget_1->indexOf(ui->tab_3)!=-1){
         ui->tabWidget_1->removeTab(ui->tabWidget_1->indexOf(ui->tab_3));
       }
 }else{
     if(ui->tabWidget_1->indexOf(ui->tab_3)==-1){
         ui->tabWidget_1->addTab(ui->tab_3,QString("配电信息"));
       }
   }

 if(Globa.Curstomer_Param_Set.ATS_Configure_Flage == 0){
     if(ui->tabWidget_1->indexOf(ui->tab_4)!=-1){
         ui->tabWidget_1->removeTab(ui->tabWidget_1->indexOf(ui->tab_4));
       }
 }else{
     if(ui->tabWidget_1->indexOf(ui->tab_4)==-1){
         ui->tabWidget_1->addTab(ui->tab_4,QString("ATS信息"));
       }
   }
}

/*****主电与输出信息**************/
void Eps_Run_Information::Eps_Run_Tableview_1()
{
  QString qstr;
  UINT32 ls_Value[6][3] = {{0}};
//主电信息和输出信息合在一起做，显示分开
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
}



void Eps_Run_Information::Eps_Run_Tableview_2()
{ UINT32 eps_run_state=0;
  QString qstr;
  UINT32 ls_Value[3][3] = {{0}};
  DSP_State_Alarm_mutex.lockForRead();
  eps_run_state = Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status];//EPS运行状态01H：主电模式， 02H：应急模式，其他待机模式）
  DSP_State_Alarm_mutex.unlock();
  if(eps_run_state==1){
     //value=00000
    }else{
  DSP_Analog_Data_mutex.lockForRead();
  ls_Value[0][0] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_A_Phase_Voltage];
  ls_Value[0][1] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_B_Phase_Voltage];
  ls_Value[0][2] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_C_Phase_Voltage];

  ls_Value[1][0] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_A_Phase_Current];
  ls_Value[1][1] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_B_Phase_Current];
  ls_Value[1][2] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_C_Phase_Current];

  ls_Value[2][0] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_Frequency];
  ls_Value[2][1] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_Frequency];
  ls_Value[2][2] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Inv_Frequency];

  DSP_Analog_Data_mutex.unlock();
}
  qstr.setNum((float) ls_Value[0][0]/10,'f',1);//应急A电压
  ui->Emer_Vol__A->setText(qstr);
  qstr.setNum((float) ls_Value[0][1]/10,'f',1);//应急B电压
  ui->Emer_Vol__B->setText(qstr);
  qstr.setNum((float) ls_Value[0][2]/10,'f',1);//应急C电压
  ui->Emer_Vol__C->setText(qstr);

 // qstr.setNum((float) ls_Value[1][0]/10,'f',1);//应急A电流
 // ui->Emer_Curr_A->setText(qstr);
 // qstr.setNum((float) ls_Value[1][1]/10,'f',1);//应急B电流
 // ui->Emer_Curr_B->setText(qstr);
 // qstr.setNum((float) ls_Value[1][2]/10,'f',1);//应急C电流
 // ui->Emer_Curr_C->setText(qstr);

  qstr.setNum((float) ls_Value[2][0]/10,'f',1);//应急A频率
  ui->Inv_Frequency_A->setText(qstr);
  qstr.setNum((float) ls_Value[2][1]/10,'f',1);//应急B频率
  ui->Inv_Frequency_B->setText(qstr);
  qstr.setNum((float) ls_Value[2][2]/10,'f',1);//应急C频率
  ui->Inv_Frequency_C->setText(qstr);


}

/*****充电信息**************/
void Eps_Run_Information::Eps_Run_Tableview_3()
{

  QString qstr;
  UINT32 ls_Value[9] = {0};
  INT16 battery_temp_value=0;
  DSP_Analog_Data_mutex.lockForRead();
  ls_Value[0] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Charging_Voltage];
  ls_Value[1] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Current];
  ls_Value[2] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Temp_Compen_Coefficient];
  ls_Value[3] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Start];
  ls_Value[4] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Remaining_Capacity];
  ls_Value[5] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Remaining_Time];

  //t3 ls_Value[6] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Temp];
  ls_Value[7] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Bus_Voltage];
  ls_Value[8] =  Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Ambient_Temperature];

  DSP_Analog_Data_mutex.unlock();
  //t4//
  Battery_mutex.lockForWrite();
  battery_temp_value=battery_temp_display;
  for(int i = 0;(i<Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable)&&(i<BATTERYMAXGROUP);i++){
     for(int j=0;j<Globa.Battery_Param_Set[i].Battery_Number;j++){
      if(Globa.Eps_Param_Data.Battery_Communication_Fault[i][j]==0){
      //printf("epsqioan%d\n",Globa.Eps_Param_Data.Battery_Tempature[i][j]);
      //printf("btemqian%d\n",battery_temp_value);
         battery_temp_value=MAX((INT16)(Globa.Eps_Param_Data.Battery_Tempature[i][j]),battery_temp_value);
       // }
         //printf("epshou%d\n",Globa.Eps_Param_Data.Battery_Tempature[i][j]);
         //printf("btemphou%d\n",battery_temp_value);
        }
    }
    }
  //battery_temp_value=Globa.Eps_Param_Data.Battery_Tempature;
  //Globa.Eps_Param_Data.Battery_Tempature=0;
  Battery_mutex.unlock();

  qstr.setNum((float) ls_Value[1]/10,'f',1);//充电电流
  ui->Charging_Current->setText(qstr);
  qstr.setNum((float) ls_Value[0]/10,'f',1);//电池电压：
  ui->Battery_Voltage->setText(qstr);
  qstr.setNum(ls_Value[2]);//电池温度补偿系数
  ui->Battery_Temp_Compen_Coefficient->setText(qstr);
 /* qstr.setNum((float) ls_Value[3]/10,'f',1);//电池状态
  ui->Charging_State->setText(qstr);*/
  qstr.setNum(ls_Value[4]);//剩余容量：
  ui->Residual_Capacity->setText(qstr);
  qstr.setNum((float) ls_Value[5],'f',1);//剩余时间
  ui->Residual_Time->setText(qstr); 
  qstr.setNum((float) battery_temp_value/10,'f',1);//电池温度
  ui->Battery_Temp->setText(qstr);
  if(battery_temp_value==battery_temp_display)
    {
      ui->Battery_Temp->setText("----");
    }
  qstr.setNum((float) ls_Value[7]/10,'f',1);//母线电压
  ui->Bus_Voltage_Bus->setText(qstr);
  //qstr.setNum((float) ls_Value[8]/10,'f',1);//EPS环境温度
  //ui->EPS_Temp->setText(qstr);
  switch(ls_Value[3]){
    case 1:  {
         ui->Charging_State->setText(QString("均充"));
         break;
      }
    case 2:  {
         ui->Charging_State->setText(QString("浮充"));
         break;
      }
    case 3:  {
         ui->Charging_State->setText(QString("放电"));
         break;
      }
    default:{
        ui->Charging_State->setText("----");
        break;
      }
  }


}

/*****馈电信息**************/
void Eps_Run_Information::Eps_Run_Tableview_4()
{
  UINT32 i = 0;
  UINT32 value[64] = {0};
  for(i = 0;i<64;i++)
  {
    value[i] = (Globa.Switch_Value[i/8]>>(i%8))&0x01;
  }
  int startloop=pagenum>0?32:0;
  int endloop=pagenum>0?Globa.Curstomer_Param_Set.Distribution_Branches_Number:MIN(Globa.Curstomer_Param_Set.Distribution_Branches_Number,32);
  ui->tableWidget->clearContents();

  for(i = startloop;i<endloop;i++)
  {
    int namerow=(i-pagenum*32)/4;
    int namecol=2*(i%4);
    ui->tableWidget->setItem(namerow,namecol,new QTableWidgetItem (QString("支路%1").arg(i+1)));
    ui->tableWidget->setItem(namerow,namecol+1,new QTableWidgetItem (QString("%1").arg(Statusbranchumberable[0][value[i]])));
  }

//  if(Globa.Curstomer_Param_Set.Distribution_Branches_Number%4 == 0){
//    for(i = 0;i<Globa.Curstomer_Param_Set.Distribution_Branches_Number/4;i++){
//      ui->tableWidget->item(i,0)->setText(QString("支路%1").arg(i*4+1));
//      ui->tableWidget->item(i,1)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4]]));
//      ui->tableWidget->item(i,2)->setText(QString("支路%1").arg(i*4+2));
//      ui->tableWidget->item(i,3)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 1]]));
//      ui->tableWidget->item(i,4)->setText(QString("支路%1").arg(i*4+3));
//      ui->tableWidget->item(i,5)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 2]]));
//      ui->tableWidget->item(i,6)->setText(QString("支路%1").arg(i*4+4));
//      ui->tableWidget->item(i,7)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 3]]));
//    }
//  }else if(Globa.Curstomer_Param_Set.Distribution_Branches_Number%4 == 1){
//    for(i = 0;i<Globa.Curstomer_Param_Set.Distribution_Branches_Number/4;i++){
//      ui->tableWidget->item(i,0)->setText(QString("支路%1").arg(i*4+1));
//      ui->tableWidget->item(i,1)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4]]));
//      ui->tableWidget->item(i,2)->setText(QString("支路%1").arg(i*4+2));
//      ui->tableWidget->item(i,3)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 1]]));
//      ui->tableWidget->item(i,4)->setText(QString("支路%1").arg(i*4+3));
//      ui->tableWidget->item(i,5)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 2]]));
//      ui->tableWidget->item(i,6)->setText(QString("支路%1").arg(i*4+4));
//      ui->tableWidget->item(i,7)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 3]]));

//    }
//    ui->tableWidget->item(i,0)->setText(QString("支路%1").arg(i*4+1));
//    ui->tableWidget->item(i,1)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4]]));
//  }else if(Globa.Curstomer_Param_Set.Distribution_Branches_Number%4 == 2){
//    for(i = 0;i<Globa.Curstomer_Param_Set.Distribution_Branches_Number/4;i++){
//      ui->tableWidget->item(i,0)->setText(QString("支路%1").arg(i*4+1));
//      ui->tableWidget->item(i,1)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4]]));
//      ui->tableWidget->item(i,2)->setText(QString("支路%1").arg(i*4+2));
//      ui->tableWidget->item(i,3)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 1]]));
//      ui->tableWidget->item(i,4)->setText(QString("支路%1").arg(i*4+3));
//      ui->tableWidget->item(i,5)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 2]]));
//      ui->tableWidget->item(i,6)->setText(QString("支路%1").arg(i*4+4));
//      ui->tableWidget->item(i,7)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 3]]));

//    }
//    ui->tableWidget->item(i,0)->setText(QString("支路%1").arg(i*4+1));
//    ui->tableWidget->item(i,1)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4]]));
//    ui->tableWidget->item(i,2)->setText(QString("支路%1").arg(i*4+2));
//    ui->tableWidget->item(i,3)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 1]]));

//  }else if(Globa.Curstomer_Param_Set.Distribution_Branches_Number%4 == 3){
//    for(i = 0;i<Globa.Curstomer_Param_Set.Distribution_Branches_Number/4;i++){
//      ui->tableWidget->item(i,0)->setText(QString("支路%1").arg(i*4+1));
//      ui->tableWidget->item(i,1)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4]]));
//      ui->tableWidget->item(i,2)->setText(QString("支路%1").arg(i*4+2));
//      ui->tableWidget->item(i,3)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 1]]));
//      ui->tableWidget->item(i,4)->setText(QString("支路%1").arg(i*4+3));
//      ui->tableWidget->item(i,5)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 2]]));
//      ui->tableWidget->item(i,6)->setText(QString("支路%1").arg(i*4+4));
//      ui->tableWidget->item(i,7)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 3]]));

//    }
//    ui->tableWidget->item(i,0)->setText(QString("支路%1").arg(i*4+1));
//    ui->tableWidget->item(i,1)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4]]));
//    ui->tableWidget->item(i,2)->setText(QString("支路%1").arg(i*4+2));
//    ui->tableWidget->item(i,3)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 1]]));
//    ui->tableWidget->item(i,4)->setText(QString("支路%1").arg(i*4+3));
//    ui->tableWidget->item(i,5)->setText(QString("%1").arg(Statusbranchumberable[0][value[i*4 + 2]]));
//  }

}
/*****ATS信息**************/
void Eps_Run_Information::Eps_Run_Tableview_5()
{


  ui->label_input->setText("--");
  ui->label_model->setText("--");
  ui->label_main->setText("--");
  ui->label_standby->setText("--");

  if(Globa.ATS_Module_Fault==0)
  {
      if(Globa.ATS_Value[0]==0)
      {
          ui->label_input->setText("市电");
      }
      else
      {
          ui->label_input->setText("备电");
      }

      if(Globa.ATS_Value[3]==1)
      {
          ui->label_model->setText("自投自复");
      }
      else if(Globa.ATS_Value[3]==2)
      {
          ui->label_model->setText("互为备用");
      }
      else if(Globa.ATS_Value[3]==3)
      {
          ui->label_model->setText("自投不自复");
      }
      else if(Globa.ATS_Value[3]==4)
      {
          ui->label_model->setText("手动");
      }

      if(Globa.ATS_Value[1]==0)
      {
          ui->label_main->setText("故障");
      }
      else
      {
          ui->label_main->setText("正常");
      }
      if(Globa.ATS_Value[2]==0)
      {
          ui->label_standby->setText("故障");
      }
      else
      {
          ui->label_standby->setText("正常");
      }
  }



}

void Eps_Run_Information::Eps__Run_Refresh_Data()
{


  if(Globa.Curstomer_Param_Set.battertyenable==0)
  {
      ui->label_42->setVisible(false);
      ui->Residual_Capacity->setVisible(false);
      ui->label_43->setVisible(false);
      ui->Residual_Time->setVisible(false);
  }
  else
  {
      ui->label_42->setVisible(true);
      ui->Residual_Capacity->setVisible(true);
      ui->label_43->setVisible(true);
      ui->Residual_Time->setVisible(true);
  }


 // Eps_Run_Information_init();

  if(ui->tabWidget_1->currentIndex() == 0||ui->tabWidget_1->currentIndex()==2){//主电信息和输出信息合在一起做，显示分开
    Eps_Run_Tableview_1();
  }else if(ui->tabWidget_1->currentIndex() == 1){
    Eps_Run_Tableview_2();
  }else if(ui->tabWidget_1->currentIndex() == 3){
    Eps_Run_Tableview_3();
  }else if(ui->tabWidget_1->currentIndex()==4&&(ui->tabWidget_1->indexOf(ui->tab_3)==4)){
    Eps_Run_Tableview_4();
  }else if(ui->tabWidget_1->indexOf(ui->tab_4)!=-1){
    Eps_Run_Tableview_5();
  }//else if(ui->tabWidget_1->currentIndex() == 3){
    //Eps_Run_Tableview_4();
  //}else if(ui->tabWidget_1->currentIndex() == 4){
   // Eps_Run_Tableview_5();
//}
}

void Eps_Run_Information::on_upButton_clicked()
{
    pagenum=0;
    ui->downButton->setEnabled(true);
    ui->upButton->setEnabled(false);
}

void Eps_Run_Information::on_downButton_clicked()
{
  pagenum=1;
  ui->upButton->setEnabled(true);
  ui->downButton->setEnabled(false);
}
