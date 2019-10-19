#include "current_alarm_query.h"
#include "ui_current_alarm_query.h"

Current_Alarm_Query::Current_Alarm_Query(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Current_Alarm_Query)
{
  ui->setupUi(this);
  ui->pushButton_homepage->setObjectName("styleButton");
  ui->pushButton_uppage->setObjectName("styleButton");
  ui->pushButton_nextpage->setObjectName("styleButton");
  ui->pushButton_endpage->setObjectName("styleButton");
  current_article_number = 0;
  ls_current_article_number=0;
  pageIndex = 0;
  alarmquerytableview_int();

}

Current_Alarm_Query::~Current_Alarm_Query()
{
  delete ui;
}

void Current_Alarm_Query::alarmquerytableview_int()
{
  ui->tableWidget->setColumnCount(3); // 设置列（当前告警）
  ui->tableWidget->setColumnWidth(0,50);
  ui->tableWidget->setColumnWidth(1,345);
  ui->tableWidget->setColumnWidth(2,300);
  ui->tableWidget->setRowCount(MAXRAM_CAQ);//设置行
  ui->tableWidget->verticalHeader()->setHidden(true);
  ui->tableWidget->horizontalHeader()->setClickable(false);
  ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);//列表不能移动
  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 使其内部框只显示而不能输入
  ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行
  ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //单击选择一行
  ui->tableWidget->setAlternatingRowColors(true);//设置隔一行变颜色

  ui->tableWidget_1->setColumnCount(3); // 设置列（当前告警）
  ui->tableWidget_1->setColumnWidth(0,50);
  ui->tableWidget_1->setColumnWidth(1,345);
  ui->tableWidget_1->setColumnWidth(2,300);
  ui->tableWidget_1->setRowCount(MAXRAM_CAQ);//设置行
  ui->tableWidget_1->verticalHeader()->setHidden(true);
  ui->tableWidget_1->horizontalHeader()->setClickable(false);
  ui->tableWidget_1->horizontalHeader()->setResizeMode(QHeaderView::Fixed);//列表不能移动
  ui->tableWidget_1->setEditTriggers(QAbstractItemView::NoEditTriggers); // 使其内部框只显示而不能输入
  ui->tableWidget_1->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行
  ui->tableWidget_1->setSelectionBehavior(QAbstractItemView::SelectRows); //单击选择一行
  ui->tableWidget_1->setAlternatingRowColors(true); //设置隔一行变颜色
}

void Current_Alarm_Query::on_tabWidget_currentChanged(int index)
{
  current_article_number = 0;
  ls_current_article_number=0;
  pageIndex = index;//未使用

  qDebug()<<pageIndex;
}

/********************* 显示当前界面数据***************/
void Current_Alarm_Query::dislpay_alarm_data()
{

  INT32 current_allNumber = 0,i= 0;

  if (ui->tabWidget->currentIndex()==0)
    { if((Reset_flag==1)||(Reset_time==1)){
        current_fault_count.clear();
        }else{
      current_fault_tablelocks_mutex.lockForWrite();
      current_fault_count.clear();
      current_fault_count = current_fault_eventlog;
      printf("current_fault_eventlog is %d\n",current_fault_eventlog.count());
      if(current_fault_count.count()==0)
        {printf("current_fault_count is 0\n");}
      current_fault_tablelocks_mutex.unlock();
        }
      if((current_fault_count.count() > 0)&&(current_fault_count.count()%MAXRAM_CAQ == 0)){
          current_allNumber = current_fault_count.count()/MAXRAM_CAQ-1;
        }else{
          current_allNumber = current_fault_count.count()/MAXRAM_CAQ;
        }
     /* if((current_fault_count.count() > 0)&&(current_fault_count.count()%MAXRAM_CAQ == 0)){
          current_allNumber = current_fault_count.count()/MAXRAM_CAQ-1;
        }else{
          current_allNumber = current_fault_count.count()/MAXRAM_CAQ;
        }
      */
      if(ls_current_article_number != current_article_number){
          current_article_number = ls_current_article_number;
        }
      if((current_article_number < current_allNumber))
        {
          if(current_article_number == 0){//即显示一页数据，MAXRAM_CAQ项
              ui->pushButton_homepage->setEnabled(false);
              ui->pushButton_nextpage->setEnabled(true);
              ui->pushButton_uppage->setEnabled(false);
              ui->pushButton_endpage->setEnabled(true);
            }else{
              ui->pushButton_homepage->setEnabled(true);
              ui->pushButton_nextpage->setEnabled(true);
              ui->pushButton_uppage->setEnabled(true);
              ui->pushButton_endpage->setEnabled(true);
            }
          ui->tableWidget->clearContents();

          for(i= 0; i< MAXRAM_CAQ; i++)
            {
              ui->tableWidget->setItem(i , 0,new QTableWidgetItem (QString("%1").arg(MAXRAM_CAQ*current_article_number+i+1)));
              ui->tableWidget->setItem(i , 1,new QTableWidgetItem (QString("%1").arg(current_fault_count.at(i + MAXRAM_CAQ*current_article_number).name)));
              ui->tableWidget->setItem(i , 2,new QTableWidgetItem (QString("%1").arg(current_fault_count.at(i + MAXRAM_CAQ*current_article_number).time)));
            }
        }

      else if(current_article_number == current_allNumber)
        {//最后一页
          if(current_article_number == 0){
              ui->pushButton_homepage->setEnabled(false);
              ui->pushButton_nextpage->setEnabled(false);
              ui->pushButton_uppage->setEnabled(false);
              ui->pushButton_endpage->setEnabled(false);
            }else{
              ui->pushButton_homepage->setEnabled(true);
              ui->pushButton_uppage->setEnabled(true);
              ui->pushButton_nextpage->setEnabled(false);
              ui->pushButton_endpage->setEnabled(false);
            }
          ui->tableWidget->clearContents();

          for(i=MAXRAM_CAQ*(current_article_number);i< current_fault_count.count();i++)
            {
              ui->tableWidget->setItem(i-MAXRAM_CAQ*(current_article_number),0,new QTableWidgetItem (QString("%1").arg(i+1)));
              ui->tableWidget->setItem(i-MAXRAM_CAQ*(current_article_number),1,new QTableWidgetItem (QString("%1").arg(current_fault_count.at(i).name)));
              ui->tableWidget->setItem(i-MAXRAM_CAQ*(current_article_number),2,new QTableWidgetItem (QString("%1").arg(current_fault_count.at(i).time)));
            }

        }

      else if(current_article_number > current_allNumber)
        {
          current_article_number = current_allNumber ;
          ls_current_article_number = current_allNumber;
        }

      ui->tableWidget->setRowCount(MAXRAM_CAQ);//设置行


    }


  else if(ui->tabWidget->currentIndex()==1)
    { if((Reset_flag==1)||(Reset_time==1)){
        current_alarm_count.clear();
        }else{
      current_alarm_tablelocks_mutex.lockForWrite();
      current_alarm_count.clear();
      current_alarm_count = current_alarm_eventlog;
  /*    printf("current_alarm_eventlog is %d\n",current_alarm_eventlog.count());
      if(current_alarm_count.count()==0)
        {printf("current_alarm_count is 0\n");}*/
      current_alarm_tablelocks_mutex.unlock();
        }
      if((current_alarm_count.count() > 0)&&(current_alarm_count.count()%MAXRAM_CAQ == 0)){
          current_allNumber = current_alarm_count.count()/MAXRAM_CAQ-1;
        }else{
          current_allNumber = current_alarm_count.count()/MAXRAM_CAQ;
        }
      if(ls_current_article_number != current_article_number){
        current_article_number = ls_current_article_number;
      }
      if((current_article_number < current_allNumber))
        { //即显示一页数据，MAXRAM_CAQ项
          if(current_article_number == 0){
              ui->pushButton_homepage->setEnabled(false);
              ui->pushButton_nextpage->setEnabled(true);
              ui->pushButton_uppage->setEnabled(false);
              ui->pushButton_endpage->setEnabled(true);
            }else{
              ui->pushButton_homepage->setEnabled(true);
              ui->pushButton_nextpage->setEnabled(true);
              ui->pushButton_uppage->setEnabled(true);
              ui->pushButton_endpage->setEnabled(true);
            }
          ui->tableWidget_1->clearContents();

          for(i= 0; i< MAXRAM_CAQ; i++)
            {
              ui->tableWidget_1->setItem(i , 0,new QTableWidgetItem (QString("%1").arg(MAXRAM_CAQ*current_article_number+i+1)));
              ui->tableWidget_1->setItem(i , 1,new QTableWidgetItem (QString("%1").arg(current_alarm_count.at(i + MAXRAM_CAQ*current_article_number).name)));
              ui->tableWidget_1->setItem(i , 2,new QTableWidgetItem (QString("%1").arg(current_alarm_count.at(i + MAXRAM_CAQ*current_article_number).time)));
            }
        }

      else if(current_article_number == current_allNumber)
        {//最后一页
          if(current_article_number == 0){
              ui->pushButton_homepage->setEnabled(false);
              ui->pushButton_nextpage->setEnabled(false);
              ui->pushButton_uppage->setEnabled(false);
              ui->pushButton_endpage->setEnabled(false);
            }else{
              ui->pushButton_homepage->setEnabled(true);
              ui->pushButton_uppage->setEnabled(true);
              ui->pushButton_nextpage->setEnabled(false);
              ui->pushButton_endpage->setEnabled(false);
            }
          ui->tableWidget_1->clearContents();

          for(i=MAXRAM_CAQ*(current_article_number);i< current_alarm_count.count();i++)
            {
              ui->tableWidget_1->setItem(i-MAXRAM_CAQ*(current_article_number),0,new QTableWidgetItem (QString("%1").arg(i+1)));
              ui->tableWidget_1->setItem(i-MAXRAM_CAQ*(current_article_number),1,new QTableWidgetItem (QString("%1").arg(current_alarm_count.at(i).name)));
              ui->tableWidget_1->setItem(i-MAXRAM_CAQ*(current_article_number),2,new QTableWidgetItem (QString("%1").arg(current_alarm_count.at(i).time)));
            }
        }
      else if(current_article_number > current_allNumber)
        {
          current_article_number = current_allNumber ;
          ls_current_article_number = current_allNumber;

        }

      ui->tableWidget->setRowCount(MAXRAM_CAQ);//设置行

    }


}

/*******************方向按键*************************/
void Current_Alarm_Query::on_pushButton_homepage_clicked()
{
  ls_current_article_number = 0;
}

void Current_Alarm_Query::on_pushButton_uppage_clicked()
{

  if(ls_current_article_number > 0){
      ls_current_article_number --;
    }

}

void Current_Alarm_Query::on_pushButton_nextpage_clicked()
{
  if(ui->tabWidget->currentIndex() == 0)
    {
      current_fault_tablelocks_mutex.lockForWrite();
      if(current_fault_count.count()%MAXRAM_CAQ != 0){
          if(ls_current_article_number < current_fault_count.count()/MAXRAM_CAQ){
              ls_current_article_number ++;
            }
        }else{
          if(ls_current_article_number < current_fault_count.count()/MAXRAM_CAQ -1){
              ls_current_article_number ++;
            }
        }
      current_fault_tablelocks_mutex.unlock();
    }
  else if (ui->tabWidget->currentIndex() == 1)
    {
      current_alarm_tablelocks_mutex.lockForWrite();
      if(current_alarm_count.count()%MAXRAM_CAQ != 0){
          if(ls_current_article_number < current_alarm_count.count()/MAXRAM_CAQ){
              ls_current_article_number ++;
            }
        }else{
          if(ls_current_article_number < current_alarm_count.count()/MAXRAM_CAQ -1){
              ls_current_article_number ++;
            }
        }
      current_alarm_tablelocks_mutex.unlock();
    }

}

void Current_Alarm_Query::on_pushButton_endpage_clicked()
{
  if(ui->tabWidget->currentIndex() == 0)
    {
      current_fault_tablelocks_mutex.lockForWrite();

      if(current_fault_count.count()%MAXRAM_CAQ != 0){
          ls_current_article_number = current_fault_count.count()/MAXRAM_CAQ ;
        }else if(current_fault_count.count() > 0){
          ls_current_article_number = current_fault_count.count()/MAXRAM_CAQ -1;
        }
      current_fault_tablelocks_mutex.unlock();
    }
  else if (ui->tabWidget->currentIndex() == 1)
    {
      current_alarm_tablelocks_mutex.lockForWrite();

      if(current_alarm_count.count()%MAXRAM_CAQ != 0){
          ls_current_article_number = current_alarm_count.count()/MAXRAM_CAQ ;
        }else if(current_alarm_count.count() > 0){
          ls_current_article_number = current_alarm_count.count()/MAXRAM_CAQ -1;
        }
      current_alarm_tablelocks_mutex.unlock();
    }


}
