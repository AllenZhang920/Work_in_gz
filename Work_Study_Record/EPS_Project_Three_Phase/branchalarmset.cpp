#include "branchalarmset.h"
#include "ui_branchalarmset.h"
#include "Share/Globalvar.h"
#include "Share/param.h"

BranchAlarmSet::BranchAlarmSet(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::BranchAlarmSet)
{
  ui->setupUi(this);
  this->setObjectName("widget");
  ui->upButton->setObjectName("mButton");
  ui->downButton->setObjectName("mButton");
  pagenum=0;
  ui->upButton->setEnabled(false);
  tableinit();
  tablefresh();

}

BranchAlarmSet::~BranchAlarmSet()
{
  delete ui;
}


void BranchAlarmSet::tableinit()
{
  ui->tableWidget->clearContents();

  QHeaderView *headerView = ui->tableWidget->verticalHeader();
  headerView->setHidden(true); // 消除序号
  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 使其内部框只显示而不能输入
//  ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行
//  ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //单击选择一行
  //ui->tableWidget->setAlternatingRowColors(true); //设置隔一行变颜色

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

  }
   connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(changestatus(int, int)));
}

void BranchAlarmSet::tablefresh()
{



  if(Globa.Curstomer_Param_Set.Distribution_Branches_Number<=32)
  {
      ui->downButton->setVisible(false);
      ui->upButton->setVisible(false);
  }
  else
  {
      ui->downButton->setVisible(true);
      ui->upButton->setVisible(true);

  }

  UINT32 i = 0;
  UINT32 value[64] = {0};
  for(i = 0;i<64;i++)
  {
      value[i] = Globa.Sysappendparam.Switch_Alarm_Set[i];
  }
  int startloop=pagenum>0?32:0;
  int endloop=pagenum>0?Globa.Curstomer_Param_Set.Distribution_Branches_Number:MIN(Globa.Curstomer_Param_Set.Distribution_Branches_Number,32);
  ui->tableWidget->clearContents();

  for(i = startloop;i<endloop;i++)
  {


    int namerow=(i-pagenum*32)/4;
    int namecol=2*(i%4);
    ui->tableWidget->setItem(namerow,namecol,new QTableWidgetItem (QString("支路%1").arg(i+1)));
    ui->tableWidget->setItem(namerow,namecol+1,new QTableWidgetItem (QString("%1").arg(Statusbranchalarmable[0][value[i]])));

    // ui->tableWidget->setItem(namerow,namecol,new QTableWidgetItem (QString("支路1")));

  }
}


void BranchAlarmSet::changestatus(int row, int col)
{


  if(pagenum>1||row>7||col>7||((col+1)%2)||(32*pagenum+(8*row+col)/2+1)>Globa.Curstomer_Param_Set.Distribution_Branches_Number)
      return;
    if(Globa.Sysappendparam.Switch_Alarm_Set[32*pagenum+4*row+col/2]==1)
    {
      Globa.Sysappendparam.Switch_Alarm_Set[32*pagenum+4*row+col/2]=0;

    }
    else
    {
       Globa.Sysappendparam.Switch_Alarm_Set[32*pagenum+4*row+col/2]=1;
       current_alarm_tablelocks_mutex.lockForWrite();
       Globa.Switch_Set_Flag=1;//将屏蔽的告警删除标记
       current_alarm_tablelocks_mutex.unlock();



    }
    System_SysAppend_Set_save();
    ui->tableWidget->setItem(row,col,new QTableWidgetItem (QString("%1").arg(Statusbranchalarmable[0][Globa.Sysappendparam.Switch_Alarm_Set[32*pagenum+4*row+col/2]])));

}

void BranchAlarmSet::on_upButton_clicked()
{
  pagenum=0;
  ui->downButton->setEnabled(true);
  ui->upButton->setEnabled(false);
  tablefresh();
}

void BranchAlarmSet::on_downButton_clicked()
{
  pagenum=1;
  ui->upButton->setEnabled(true);
  ui->downButton->setEnabled(false);
  tablefresh();
}
