#include "batteryform.h"
#include "ui_batteryform.h"

#include <pthread.h>

#include <QtCore>

#include<QDebug>


BatteryForm::BatteryForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatteryForm)
{
    ui->setupUi(this);
    ui->pushButton_homepage->setObjectName("styleButton");
    ui->pushButton_uppage->setObjectName("styleButton");
    ui->pushButton_nextpage->setObjectName("styleButton");
    ui->pushButton_endpage->setObjectName("styleButton");
   // qDebug()<<"___-batt界面初始化";
    this->setGeometry(0,0,800,480);
    ui->betterypack_choice->setView(new QListView());

    QHeaderView *headerView = ui->tableWidget->verticalHeader();
    headerView->setHidden(true); // 消除序号
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 使其内部框只显示而不能输入
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //单击选择一行
    ui->tableWidget->setAlternatingRowColors(true); //设置隔一行变颜色

    ui->tableWidget->setColumnWidth(0,119);
    ui->tableWidget->setColumnWidth(1,121);
    ui->tableWidget->setColumnWidth(2,119);
    ui->tableWidget->setColumnWidth(3,121);
    ui->tableWidget->setColumnWidth(4,119);
    ui->tableWidget->setColumnWidth(5,121);
    ui->tableWidget->horizontalHeader()->setClickable(false);
    //ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);//列表不能移动
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setRowCount(8);//设置行

    Battery_choice_int();

    CurrentNumberpages = 0;//当前页数
    packnumber = 0;

/*    minVoltage = 0;
    maxVoltage = 0;*/
    ui->tableWidget->setColumnCount(6); // 设置列（这里是固定的）
    ui->betterypack_choice->setCurrentIndex(0);
    //qDebug()<<"___-batt界面初始化end";

    battery_int();
   // qDebug()<<"___-batt界面初始化111111";

}

BatteryForm::~BatteryForm()
{
    delete ui;
}


void BatteryForm::battery_int()
{
   UINT32 i=0,j=0;
   for(i=0;i<BATTERYMAXGROUP;i++){
     TotalNumberpages[i] = 0 ;//总页数
     batteryNumber[i] = 0;
   }
   for(j=0;(j<Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable)&&(j<BATTERYMAXGROUP);j++){//计算第j组的电池总个数
    for(i=0; i<Globa.Battery_Param_Set[j].Battery_Number; i++){
      batteryNumber[j] = batteryNumber[j] + Globa.Battery_Param_Set[j].Singlenumber[i];
    }
    if(batteryNumber[j] != 0){
       if(batteryNumber[j] % 24==0){
         TotalNumberpages[j] = batteryNumber[j] / 24-1 ;//计算第j组的总页数
       }else{
         TotalNumberpages[j] = batteryNumber[j] / 24 ;//总页数
       }
     }else{
       TotalNumberpages[j] = 0;
       ui->pushButton_homepage->setEnabled(false);
       ui->pushButton_nextpage->setEnabled(false);
       ui->pushButton_uppage->setEnabled(false);
       ui->pushButton_endpage->setEnabled(false);
     }
  }
}


void BatteryForm::Battery_Fullpage_function()
{
   UINT32 row = 0;
   for(row=0; row<8; row++){
     ui->tableWidget->setItem(row, 0, new QTableWidgetItem(tr("第%1节").arg(3*row + 24*CurrentNumberpages + 1)));
     ui->tableWidget->setItem(row, 1, new QTableWidgetItem (QString("%1.%2%3%4").arg(MonomerVoltage[3*row + 24*CurrentNumberpages]/1000).arg(MonomerVoltage[3*row + 24*CurrentNumberpages]%1000/100).arg(MonomerVoltage[3*row + 24*CurrentNumberpages]%1000%100/10).arg(MonomerVoltage[3*row + 24*CurrentNumberpages]%1000%100%10)));

    /* if(MonomerVoltage[3*row + 24*CurrentNumberpages] == maxVoltage ){
       ui->tableWidget->item(row,1)->setBackgroundColor(Qt::blue);
     }else if(MonomerVoltage[3*row + 24*CurrentNumberpages] == minVoltage ){
       ui->tableWidget->item(row,1)->setBackgroundColor(Qt::green);
     }*/

     ui->tableWidget->setItem(row, 2, new QTableWidgetItem(tr("第%1节").arg(3*row + 24*CurrentNumberpages + 2)));
     ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString("%1.%2%3%4").arg(MonomerVoltage[3*row + 24*CurrentNumberpages+1]/1000).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +1]%1000/100).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +1]%1000%100/10).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +1]%1000%100%10)));
 /* if(MonomerVoltage[3*row + 24*CurrentNumberpages +1] == maxVoltage ){
       ui->tableWidget->item(row,3)->setBackgroundColor(Qt::blue);
     }else if(MonomerVoltage[3*row + 24*CurrentNumberpages +1] == minVoltage ){
       ui->tableWidget->item(row,3)->setBackgroundColor(Qt::green);
     }*/
     ui->tableWidget->setItem(row, 4, new QTableWidgetItem(tr("第%1节").arg(3*row + 24*CurrentNumberpages + 3)));
     ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString("%1.%2%3%4").arg(MonomerVoltage[3*row + 24*CurrentNumberpages+2]/1000).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +2]%1000/100).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +2]%1000%100/10).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +2]%1000%100%10)));
/*   if(MonomerVoltage[3*row + 24*CurrentNumberpages +2] == maxVoltage ){
       ui->tableWidget->item(row,5)->setBackgroundColor(Qt::blue);
     }else if(MonomerVoltage[3*row + 24*CurrentNumberpages +2] == minVoltage ){
       ui->tableWidget->item(row,5)->setBackgroundColor(Qt::green);
     }*/
     ui->tableWidget->item(row,0)->setTextAlignment(Qt::AlignCenter);
     ui->tableWidget->item(row,1)->setTextAlignment(Qt::AlignCenter);
     ui->tableWidget->item(row,2)->setTextAlignment(Qt::AlignCenter);
     ui->tableWidget->item(row,3)->setTextAlignment(Qt::AlignCenter);
     ui->tableWidget->item(row,4)->setTextAlignment(Qt::AlignCenter);
     ui->tableWidget->item(row,5)->setTextAlignment(Qt::AlignCenter);
   }

}
void BatteryForm::Battery_NotFullpage_function()
{
  UINT32 row = 0;

  for(row=0; row<(batteryNumber[packnumber] - 24*CurrentNumberpages)/3; row++){//batteryNumber-24*CurrentNumberpages相减时本身结果就少加了1，所以就不用减一了
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(tr("第%1节").arg(3*row + 24*CurrentNumberpages + 1)));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem (QString("%1.%2%3%4").arg(MonomerVoltage[3*row + 24*CurrentNumberpages]/1000).arg(MonomerVoltage[3*row+24*CurrentNumberpages]%1000/100).arg(MonomerVoltage[3*row + 24*CurrentNumberpages]%1000%100/10).arg(MonomerVoltage[3*row+ 24*CurrentNumberpages]%1000%100%10)));
 /*   if(MonomerVoltage[3*row + 24*CurrentNumberpages] == maxVoltage ){
      ui->tableWidget->item(row,1)->setBackgroundColor(Qt::blue);
    }else if(MonomerVoltage[3*row + 24*CurrentNumberpages] == minVoltage ){
      ui->tableWidget->item(row,1)->setBackgroundColor(Qt::green);
    }*/
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(tr("第%1节").arg(3*row + 24*CurrentNumberpages+2)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString("%1.%2%3%4").arg(MonomerVoltage[3*row + 24*CurrentNumberpages +1]/1000).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +1]%1000/100).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +1]%1000%100/10).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +1]%1000%100%10)));
 /*   if(MonomerVoltage[3*row + 24*CurrentNumberpages +1] == maxVoltage ){
      ui->tableWidget->item(row,3)->setBackgroundColor(Qt::blue);
    }else if(MonomerVoltage[3*row + 24*CurrentNumberpages +1] == minVoltage ){
      ui->tableWidget->item(row,3)->setBackgroundColor(Qt::green);
    }*/
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(tr("第%1节").arg(3*row + 24*CurrentNumberpages+3)));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem (QString("%1.%2%3%4").arg(MonomerVoltage[3*row+24*CurrentNumberpages +2]/1000).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +2]%1000/100).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +2]%1000%100/10).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +2]%1000%100%10)));
/*  if(MonomerVoltage[3*row + 24*CurrentNumberpages +2] == maxVoltage ){
      ui->tableWidget->item(row,5)->setBackgroundColor(Qt::blue);
    }else if(MonomerVoltage[3*row + 24*CurrentNumberpages +2] == minVoltage ){
      ui->tableWidget->item(row,5)->setBackgroundColor(Qt::green);
    }*/

    ui->tableWidget->item(row,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(row,1)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(row,2)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(row,3)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(row,4)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(row,5)->setTextAlignment(Qt::AlignCenter);

  }
  if(batteryNumber[packnumber] % 3  >0){
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(tr("第%1节").arg(3*row + 24*CurrentNumberpages+1)));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem (QString("%1.%2%3%4").arg(MonomerVoltage[3*row + 24*CurrentNumberpages]/1000).arg(MonomerVoltage[3*row + 24*CurrentNumberpages]%1000/100).arg(MonomerVoltage[3*row + 24*CurrentNumberpages]%1000%100/10).arg(MonomerVoltage[3*row + 24*CurrentNumberpages]%1000%100%10)));
   /* if(MonomerVoltage[3*row + 24*CurrentNumberpages] == maxVoltage ){
      ui->tableWidget->item(row,1)->setBackgroundColor(Qt::blue);
    }else if(MonomerVoltage[3*row + 24*CurrentNumberpages] == minVoltage ){
      ui->tableWidget->item(row,1)->setBackgroundColor(Qt::green);
    }*/
    ui->tableWidget->item(row,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(row,1)->setTextAlignment(Qt::AlignCenter);

    if(batteryNumber[packnumber] % 3 == 2){
      ui->tableWidget->setItem(row, 2, new QTableWidgetItem(tr("第%1节").arg(3*row+ 24*CurrentNumberpages + 2)));
      ui->tableWidget->setItem(row, 3, new QTableWidgetItem (QString("%1.%2%3%4").arg(MonomerVoltage[3*row + 24*CurrentNumberpages +1]/1000).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +1]%1000/100).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +1]%1000%100/10).arg(MonomerVoltage[3*row + 24*CurrentNumberpages +1]%1000%100%10)));
  /*if(MonomerVoltage[3*row + 24*CurrentNumberpages +1] == maxVoltage ){
        ui->tableWidget->item(row,3)->setBackgroundColor(Qt::blue);
      }else if(MonomerVoltage[3*row + 24*CurrentNumberpages +1] == minVoltage ){
        ui->tableWidget->item(row,3)->setBackgroundColor(Qt::green);
     }*/
      ui->tableWidget->item(row,2)->setTextAlignment(Qt::AlignCenter);
      ui->tableWidget->item(row,3)->setTextAlignment(Qt::AlignCenter);

    }
  }
  //}
}


void BatteryForm::timerUpdata()
{
  UINT32 i = 0,j = 0,z=0;


  memset(&MonomerVoltage, 0x00, sizeof(MonomerVoltage));

  for(z=0,i=0;i<Globa.Battery_Param_Set[packnumber].Battery_Number;i++){
    for(j= 0;j<Globa.Battery_Param_Set[packnumber].Singlenumber[i];j++){//把第packnumber组模块所有电池的值付给MonomerVoltage
      MonomerVoltage[z++] =  Globa.Eps_Param_Data.Battery_Data_Param[packnumber].Battery_Monomer_V[i][j];

    }
  }
  minVoltage = MonomerVoltage[0];
  maxVoltage = MonomerVoltage[0];
  for(i=0;i<batteryNumber[packnumber];i++){
    maxVoltage = MAX(maxVoltage,MonomerVoltage[i]);
    minVoltage = MIN(MonomerVoltage[i],minVoltage);
  }

  ui->lineEdit_H_Voltager->setText(QString("%1.%2%3%4 V").arg(maxVoltage/1000).arg(maxVoltage%1000/100).arg(maxVoltage%1000%100/10).arg(maxVoltage%1000%100%10));
  ui->lineEdit_L_Voltager->setText(QString("%1.%2%3%4 V").arg(minVoltage/1000).arg(minVoltage%1000/100).arg(minVoltage%1000%100/10).arg(minVoltage%1000%100%10));

 // ui->lineEdit_HL_Voltager->setText(QString("%1.%2%3%4 V").arg((maxVoltage- minVoltage)/1000).arg((maxVoltage- minVoltage)%1000/100).arg((maxVoltage- minVoltage)%1000%100/10).arg((maxVoltage- minVoltage)%1000%100%10));

  if(Globa.Battery_Param_Set[packnumber].Battery_Number !=0 ){
    if(CurrentNumberpages < TotalNumberpages[packnumber]){ ////即显示一页数据
       ui->tableWidget->clearContents();
       ui->tableWidget->setRowCount(8);//设置行
       if(CurrentNumberpages == 0)
         {
         ui->pushButton_homepage->setEnabled(false);
         ui->pushButton_uppage->setEnabled(false);
         ui->pushButton_nextpage->setEnabled(true);
         ui->pushButton_endpage->setEnabled(true);
       }else{
         ui->pushButton_homepage->setEnabled(true);
         ui->pushButton_uppage->setEnabled(true);
         ui->pushButton_nextpage->setEnabled(true);
         ui->pushButton_endpage->setEnabled(true);
       }
       Battery_Fullpage_function();
    }else if(CurrentNumberpages == TotalNumberpages[packnumber]){
      ui->tableWidget->clearContents();
      ui->tableWidget->setRowCount(8);//设置行
      if(CurrentNumberpages == 0){
        ui->pushButton_homepage->setEnabled(false);
        ui->pushButton_uppage->setEnabled(false);
        ui->pushButton_nextpage->setEnabled(false);
        ui->pushButton_endpage->setEnabled(false);
      }else{
        ui->pushButton_homepage->setEnabled(true);
        ui->pushButton_uppage->setEnabled(true);
        ui->pushButton_nextpage->setEnabled(false);
        ui->pushButton_endpage->setEnabled(false);
      }
      Battery_NotFullpage_function();
    }
  }else{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(8);//设置行
  }

}
/****************页面翻转键*************************/
void BatteryForm::on_pushButton_homepage_clicked()
{
   CurrentNumberpages = 0;
}

void BatteryForm::on_pushButton_uppage_clicked()
{
  if(CurrentNumberpages > 0)
     CurrentNumberpages--;
}

void BatteryForm::on_pushButton_nextpage_clicked()
{
  if(CurrentNumberpages < TotalNumberpages[packnumber])
    CurrentNumberpages++;
}

void BatteryForm::on_pushButton_endpage_clicked()
{
    CurrentNumberpages = TotalNumberpages[packnumber];
}

void BatteryForm::Battery_choice_int()
{
    ui->betterypack_choice->clear();
    for(UINT32 i=0;(i<Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable)&&(i<BATTERYMAXGROUP);i++){
      ui->betterypack_choice->insertItem(i,QString(" %1#蓄电池组").arg(i+1));
    }
    ui->betterypack_choice->setCurrentIndex(0);
}

void BatteryForm::on_betterypack_choice_currentIndexChanged(int index)
{
    packnumber = index;
    ui->groupBox->setTitle(tr("%1#蓄电池组数据").arg(packnumber + 1));
    CurrentNumberpages = 0;
}


