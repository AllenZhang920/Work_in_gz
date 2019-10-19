#include "alarmqueryform.h"
#include "ui_alarmqueryform.h"
#include"Share/param.h"
#define   ALARMQUERYPRINT  0


AlarmQueryForm::AlarmQueryForm(QWidget *parent) :
 QWidget(parent),
 ui(new Ui::AlarmQueryForm)
{


 ui->setupUi(this);

 ui->pushButton_homepage->setObjectName("styleButton");
 ui->pushButton_uppage->setObjectName("styleButton");
 ui->pushButton_nextpage->setObjectName("styleButton");
 ui->pushButton_endpage->setObjectName("styleButton");

 this->setGeometry(0,0,800,480);
 //tableviewflag = 0;

 alarmquerytableview_int();

 current_article_number = 0;
 ls_current_article_number = 0;

 dislpay_alarm_data();


}

AlarmQueryForm::~AlarmQueryForm()
{
 delete ui;
}

void AlarmQueryForm::alarmquerytableview_int()
{


 ui->tableWidget_1->setColumnCount(4); //设置列 历史告警
 ui->tableWidget_1->setColumnWidth(0,60);
 ui->tableWidget_1->setColumnWidth(1,251);
 ui->tableWidget_1->setColumnWidth(2,191);
 ui->tableWidget_1->setColumnWidth(3,191);
 ui->tableWidget_1->setRowCount(MAXRAM);//设置行
 ui->tableWidget_1->setEditTriggers(QAbstractItemView::NoEditTriggers); // 使其内部框只显示而不能输入
 ui->tableWidget_1->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行
 ui->tableWidget_1->setSelectionBehavior(QAbstractItemView::SelectRows); //单击选择一行
 ui->tableWidget_1->setAlternatingRowColors(true); //设置隔一行变颜色
 ui->tableWidget_1->horizontalHeader()->setClickable(false);
 ui->tableWidget_1->verticalHeader()->setHidden(true);
 ui->tableWidget_1->horizontalHeader()->setResizeMode(QHeaderView::Fixed);//列表不能移动

 ui->tableWidget_2->setColumnCount(3); //设置列 工作状态
 ui->tableWidget_2->setColumnWidth(0,55);
 ui->tableWidget_2->setColumnWidth(1,340);
 ui->tableWidget_2->setColumnWidth(2,300);
 ui->tableWidget_2->setRowCount(MAXRAM);//设置行
 ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers); // 使其内部框只显示而不能输入
 ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行
 ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows); //单击选择一行
 ui->tableWidget_2->setAlternatingRowColors(true); //设置隔一行变颜色
 ui->tableWidget_2->horizontalHeader()->setClickable(false);
 ui->tableWidget_2->verticalHeader()->setHidden(true);
 ui->tableWidget_2->horizontalHeader()->setResizeMode(QHeaderView::Fixed);//列表不能移动

 ui->tableWidget_3->setColumnCount(3); //设置列 操作记录
 ui->tableWidget_3->setColumnWidth(0,55);
 ui->tableWidget_3->setColumnWidth(1,340);
 ui->tableWidget_3->setColumnWidth(2,300);
 ui->tableWidget_3->setRowCount(MAXRAM);//设置行
 ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers); // 使其内部框只显示而不能输入
 ui->tableWidget_3->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行
 ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows); //单击选择一行
 ui->tableWidget_3->setAlternatingRowColors(true); //设置隔一行变颜色
 ui->tableWidget_3->horizontalHeader()->setClickable(false);
 ui->tableWidget_3->verticalHeader()->setHidden(true);
 ui->tableWidget_3->horizontalHeader()->setResizeMode(QHeaderView::Fixed);//列表不能移动

 ui->tableWidget_4->setColumnCount(4); //设置列 历史故障
 ui->tableWidget_4->setColumnWidth(0,60);
 ui->tableWidget_4->setColumnWidth(1,250);
 ui->tableWidget_4->setColumnWidth(2,190);
 ui->tableWidget_4->setColumnWidth(3,190);
 ui->tableWidget_4->setRowCount(MAXRAM);//设置行
 ui->tableWidget_4->setEditTriggers(QAbstractItemView::NoEditTriggers); // 使其内部框只显示而不能输入
 ui->tableWidget_4->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行
 ui->tableWidget_4->setSelectionBehavior(QAbstractItemView::SelectRows); //单击选择一行
 ui->tableWidget_4->setAlternatingRowColors(true); //设置隔一行变颜色
 ui->tableWidget_4->horizontalHeader()->setClickable(false);
 ui->tableWidget_4->verticalHeader()->setHidden(true);
 ui->tableWidget_4->horizontalHeader()->setResizeMode(QHeaderView::Fixed);//列表不能移动

}

/********************* 选择哪个界面***************/
void AlarmQueryForm::on_tabWidget_currentChanged(int index)
{
 if(index == 0){
     ui->historypushButton->setEnabled(true);
   }else if(index == 1){
     ui->historypushButton->setEnabled(true);
   }else if(index == 2){
     ui->historypushButton->setEnabled(true);
   }else if(index == 3){
     ui->historypushButton->setEnabled(true);
   }
 current_article_number = 0;
 ls_current_article_number = 0;
 //tableviewflag = index;
}

/********************* 显示当前界面数据***************/
void AlarmQueryForm::dislpay_alarm_data()
{
 INT32 current_allNumber = 0,i= 0;

 if(ui->tabWidget->currentIndex() == 0){     //历史故障界面
   history_fault_tablelocks_mutex.lockForWrite();
   history_fault_count.clear();
   history_fault_count = history_fault_eventlog;
/*   if(current_fault_count.count()==0)
     {printf("current_fault_count is 0\n");}*/
   history_fault_tablelocks_mutex.unlock();

   if((history_fault_count.count() > 0)&&(history_fault_count.count() % MAXRAM == 0)){
     current_allNumber = history_fault_count.count()/MAXRAM-1;
   }else{
     current_allNumber = history_fault_count.count()/MAXRAM;
   }

   if(ls_current_article_number != current_article_number){
     current_article_number = ls_current_article_number;
   }

   if((current_article_number < current_allNumber)){ //即显示一页数据，MAXRAM项
     if(current_article_number == 0){
       ui->pushButton_homepage->setEnabled(false);
       ui->pushButton_uppage->setEnabled(false);
       ui->pushButton_nextpage->setEnabled(true);
       ui->pushButton_endpage->setEnabled(true);
     }else{
       ui->pushButton_homepage->setEnabled(true);
       ui->pushButton_nextpage->setEnabled(true);
       ui->pushButton_uppage->setEnabled(true);
       ui->pushButton_endpage->setEnabled(true);
     }
     ui->tableWidget_4->clearContents();

     for(i= 0; i< MAXRAM; i++){
     //  qDebug()<<"dislpay_alarm_data--nextpag8999 001"<<i<<current_article_number<<current_allNumber<<history_fault_count.count()<<i + MAXRAM*current_article_number;
       ui->tableWidget_4->setItem(i,0,new QTableWidgetItem (QString("%1").arg(MAXRAM*current_article_number +i +1)));
      // qDebug()<<"dislpay_alarm_data--nextpag8999 0035"<<i<<current_article_number<<current_allNumber<<history_fault_count.count()<<i + MAXRAM*current_article_number;
     //  qDebug()<<"dislpay_alarm_data--nextpag8999 0038888"<<i<<current_article_number<<current_allNumber<<history_fault_count.count()<<i + MAXRAM*current_article_number;
     //  qDebug()<<"dislpay_alarm_data--nextpag8999 002"<<i<<current_article_number<<current_allNumber<<history_fault_count.count()<<i + MAXRAM*current_article_number<<history_fault_count.at(i + MAXRAM*current_article_number).name;
       ui->tableWidget_4->setItem(i,1,new QTableWidgetItem (QString("%1").arg(history_fault_count.at(i + MAXRAM*current_article_number).name)));
     //  qDebug()<<"dislpay_alarm_data--nextpag8999 003"<<i<<current_article_number<<current_allNumber<<history_fault_count.count()<<i + MAXRAM*current_article_number;

       ui->tableWidget_4->setItem(i,2,new QTableWidgetItem (QString("%1").arg(history_fault_count.at(i + MAXRAM*current_article_number).start_time)));
       ui->tableWidget_4->setItem(i,3,new QTableWidgetItem (QString("%1").arg(history_fault_count.at(i + MAXRAM*current_article_number).end_time)));
     }
   }else if(current_article_number == current_allNumber){
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
      ui->tableWidget_4->clearContents();
     for(i=MAXRAM*(current_article_number);i< history_fault_count.count();i++){
     //  qDebug()<<"dislpay_alarm_data--nextpag7777--0 "<<i-MAXRAM*(current_article_number)<<current_article_number<<current_allNumber<<i<<history_fault_count.count();
       ui->tableWidget_4->setItem(i-MAXRAM*(current_article_number),0,new QTableWidgetItem (QString("%1").arg(i +1)));
     //  qDebug()<<"dislpay_alarm_data--nextpag7777 --17899"<<i-MAXRAM*(current_article_number)<<current_article_number<<current_allNumber<<i<<history_fault_count.count();
     //  qDebug()<<"dislpay_alarm_data--nextpag7777 --1999999"<<i-MAXRAM*(current_article_number)<<current_article_number<<current_allNumber<<i<<history_fault_count.count();

     //   qDebug()<<"dislpay_alarm_data--nextpag7777 --1"<<i-MAXRAM*(current_article_number)<<current_article_number<<current_allNumber<<i<<history_fault_count.count()<<history_fault_count.at(i).name;
        ui->tableWidget_4->setItem(i-MAXRAM*(current_article_number),1,new QTableWidgetItem (QString("%1").arg(history_fault_count.at(i).name)));
      //  qDebug()<<"dislpay_alarm_data--nextpag7777 --2"<<i-MAXRAM*(current_article_number)<<current_article_number<<current_allNumber<<i<<history_fault_count.count();

        ui->tableWidget_4->setItem(i-MAXRAM*(current_article_number),2,new QTableWidgetItem (QString("%1").arg(history_fault_count.at(i).start_time)));
         ui->tableWidget_4->setItem(i-MAXRAM*(current_article_number),3,new QTableWidgetItem (QString("%1").arg(history_fault_count.at(i).end_time)));
       }
    //  qDebug()<<"dislpay_alarm_data--nextpag7777 0000 "<<current_article_number<<current_allNumber<<i<<history_fault_count.count();
    }else if(current_article_number > current_allNumber){
      current_article_number = current_allNumber ;
      ls_current_article_number = current_allNumber;
    }
    ui->tableWidget_4->setRowCount(MAXRAM);//设置行
  }
 else if(ui->tabWidget->currentIndex() == 1){     //历史告警界面
   history_alarm_tablelocks_mutex.lockForWrite();
   history_alarm_count.clear();
   history_alarm_count = history_alarm_eventlog;
   history_alarm_tablelocks_mutex.unlock();

   if((history_alarm_count.count() > 0)&&(history_alarm_count.count() % MAXRAM == 0)){
     current_allNumber = history_alarm_count.count()/MAXRAM-1;
   }else{
     current_allNumber = history_alarm_count.count()/MAXRAM;
   }

   if(ls_current_article_number != current_article_number){
     current_article_number = ls_current_article_number;
   }
   if((current_article_number < current_allNumber)){ //即显示一页数据，MAXRAM项
     if(current_article_number == 0){
       ui->pushButton_homepage->setEnabled(false);
       ui->pushButton_uppage->setEnabled(false);
       ui->pushButton_nextpage->setEnabled(true);
       ui->pushButton_endpage->setEnabled(true);
     }else{
       ui->pushButton_homepage->setEnabled(true);
       ui->pushButton_nextpage->setEnabled(true);
       ui->pushButton_uppage->setEnabled(true);
       ui->pushButton_endpage->setEnabled(true);
     }
     ui->tableWidget_1->clearContents();
     for(i= 0; i< MAXRAM; i++){
       ui->tableWidget_1->setItem(i,0,new QTableWidgetItem (QString("%1").arg(MAXRAM*current_article_number +i +1)));
       ui->tableWidget_1->setItem(i,1,new QTableWidgetItem (QString("%1").arg(history_alarm_count.at(i + MAXRAM*current_article_number).name)));
       ui->tableWidget_1->setItem(i,2,new QTableWidgetItem (QString("%1").arg(history_alarm_count.at(i + MAXRAM*current_article_number).start_time)));
       ui->tableWidget_1->setItem(i,3,new QTableWidgetItem (QString("%1").arg(history_alarm_count.at(i + MAXRAM*current_article_number).end_time)));
       }
   }else if(current_article_number == current_allNumber){
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
     for(i=MAXRAM*(current_article_number);i< history_alarm_count.count();i++){
       ui->tableWidget_1->setItem(i-MAXRAM*(current_article_number),0,new QTableWidgetItem (QString("%1").arg(i +1)));
       ui->tableWidget_1->setItem(i-MAXRAM*(current_article_number),1,new QTableWidgetItem (QString("%1").arg(history_alarm_count.at(i).name)));
       ui->tableWidget_1->setItem(i-MAXRAM*(current_article_number),2,new QTableWidgetItem (QString("%1").arg(history_alarm_count.at(i).start_time)));
       ui->tableWidget_1->setItem(i-MAXRAM*(current_article_number),3,new QTableWidgetItem (QString("%1").arg(history_alarm_count.at(i).end_time)));
     }
   }else if(current_article_number > current_allNumber){
     current_article_number = current_allNumber ;
     ls_current_article_number = current_allNumber;
   }
   ui->tableWidget_1->setRowCount(MAXRAM);//设置行
 }
 else if(ui->tabWidget->currentIndex() == 2){//工作状态
   history_EpsWork_Record_mutex.lockForRead();
   history_EpsWork_Record_count = history_EpsWork_Record; // 操作记录以及工作记录
   history_EpsWork_Record_mutex.unlock();

   if((history_EpsWork_Record_count.count() > 0)&&(history_EpsWork_Record_count.count() % MAXRAM == 0)){
     current_allNumber = history_EpsWork_Record_count.count()/MAXRAM-1;
   }else{
     current_allNumber = history_EpsWork_Record_count.count()/MAXRAM;
   }

   if(ls_current_article_number != current_article_number){
     current_article_number = ls_current_article_number;
   }

   if((current_article_number < current_allNumber)){ //即显示一页数据，MAXRAM项
     ui->tableWidget_2->clearContents();
     for(i= 0; i< MAXRAM; i++){
       ui->tableWidget_2->setItem(i,0,new QTableWidgetItem (QString("%1").arg(MAXRAM*current_article_number +i +1)));
       ui->tableWidget_2->setItem(i,1,new QTableWidgetItem (QString("%1").arg(history_EpsWork_Record_count.at(i + MAXRAM*current_article_number).name)));
       ui->tableWidget_2->setItem(i,2,new QTableWidgetItem (QString("%1").arg(history_EpsWork_Record_count.at(i + MAXRAM*current_article_number).time)));
     }
     if(current_article_number == 0){
       ui->pushButton_homepage->setEnabled(false);
       ui->pushButton_uppage->setEnabled(false);
       ui->pushButton_nextpage->setEnabled(true);
       ui->pushButton_endpage->setEnabled(true);
     }else{
       ui->pushButton_homepage->setEnabled(true);
       ui->pushButton_nextpage->setEnabled(true);
       ui->pushButton_uppage->setEnabled(true);
       ui->pushButton_endpage->setEnabled(true);
     }
   }else if(current_article_number == current_allNumber){
     ui->tableWidget_2->clearContents();
     for(i=MAXRAM*(current_article_number);i< history_EpsWork_Record_count.count();i++){
         ui->tableWidget_2->setItem(i-MAXRAM*(current_article_number),0,new QTableWidgetItem (QString("%1").arg(i +1)));
         ui->tableWidget_2->setItem(i-MAXRAM*(current_article_number),1,new QTableWidgetItem (QString("%1").arg(history_EpsWork_Record_count.at(i).name)));
         ui->tableWidget_2->setItem(i-MAXRAM*(current_article_number),2,new QTableWidgetItem (QString("%1").arg(history_EpsWork_Record_count.at(i).time)));
     }
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
   }else if(current_article_number > current_allNumber){
     current_article_number = current_allNumber ;
     ls_current_article_number = current_allNumber;
   }
   ui->tableWidget_2->setRowCount(MAXRAM);//设置行
 }

 else if(ui->tabWidget->currentIndex() == 3){//操作记录以及工作记录
   history_Operation_Record_mutex.lockForRead();
   history_Operation_Record_count = history_Operation_Record; // 操作记录以及工作记录
   history_Operation_Record_mutex.unlock();
   if((history_Operation_Record_count.count() > 0)&&(history_Operation_Record_count.count() % MAXRAM == 0)){
     current_allNumber = history_Operation_Record_count.count()/MAXRAM-1;
   }else{
     current_allNumber = history_Operation_Record_count.count()/MAXRAM;
   }

   if(ls_current_article_number != current_article_number){
     current_article_number = ls_current_article_number;
   }

   if((current_article_number < current_allNumber)){ //即显示一页数据，MAXRAM项
     ui->tableWidget_3->clearContents();
     for(i= 0; i< MAXRAM; i++){
       ui->tableWidget_3->setItem(i,0,new QTableWidgetItem (QString("%1").arg(MAXRAM*current_article_number +i +1)));
       ui->tableWidget_3->setItem(i,1,new QTableWidgetItem (QString("%1").arg(history_Operation_Record_count.at(i + MAXRAM*current_article_number).name)));
       ui->tableWidget_3->setItem(i,2,new QTableWidgetItem (QString("%1").arg(history_Operation_Record_count.at(i + MAXRAM*current_article_number).time)));
     }
     if(current_article_number == 0){
         ui->pushButton_homepage->setEnabled(false);
         ui->pushButton_uppage->setEnabled(false);
         ui->pushButton_nextpage->setEnabled(true);
         ui->pushButton_endpage->setEnabled(true);
       }else{
         ui->pushButton_homepage->setEnabled(true);
         ui->pushButton_nextpage->setEnabled(true);
         ui->pushButton_uppage->setEnabled(true);
         ui->pushButton_endpage->setEnabled(true);
       }
     }else if(current_article_number == current_allNumber){
       ui->tableWidget_3->clearContents();
       for(i=MAXRAM*(current_article_number);i< history_Operation_Record_count.count();i++){
         ui->tableWidget_3->setItem(i-MAXRAM*(current_article_number),0,new QTableWidgetItem (QString("%1").arg(i +1)));
         ui->tableWidget_3->setItem(i-MAXRAM*(current_article_number),1,new QTableWidgetItem (QString("%1").arg(history_Operation_Record_count.at(i).name)));
         ui->tableWidget_3->setItem(i-MAXRAM*(current_article_number),2,new QTableWidgetItem (QString("%1").arg(history_Operation_Record_count.at(i).time)));
       }

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
     }else if(current_article_number > current_allNumber){
       current_article_number = current_allNumber;
       ls_current_article_number = current_allNumber;
     }
     ui->tableWidget_3->setRowCount(MAXRAM);//设置行
   }
}
/*
void AlarmQueryForm::Delete_history_data(QString password)
{
 if(!this->isHidden()){
     if(password == QLatin1String(Globa.User_Monitor_Screen_Set.userpassword)){
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));
         msgBox.setText(tr("确定删除历史记录?"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
         if(msgBox.exec()  == QMessageBox::Ok)
           {
             if(tableviewflag == 0)
               {
                 {
                   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","timer_int");
                   db.setDatabaseName(HISTORY_FAULT_DB);     // 连接历史数据库
                   if(db.isOpen()){
                       db.close();
                     }
                 }
                 QSqlDatabase::removeDatabase("timer_int");
                 QFile::remove(HISTORY_FAULT_DB);

                 ui->tableWidget_4->clearContents();

                 history_fault_tablelocks_mutex.lockForWrite();
                 history_fault_eventlog.clear();
                 history_fault_count.clear();
                 history_fault_tablelocks_mutex.unlock();
                 current_article_number = 0;
               }

             if(tableviewflag == 1)
               {
                 {
                   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","timer_int");
                   db.setDatabaseName(HISTORY_ALARM_DB);     // 连接历史数据库
                   if(db.isOpen()){
                       db.close();
                     }
                 }
                 QSqlDatabase::removeDatabase("timer_int");
                 QFile::remove(HISTORY_ALARM_DB);

                 ui->tableWidget_1->clearContents();

                 history_alarm_tablelocks_mutex.lockForWrite();
                 history_alarm_eventlog.clear();
                 history_alarm_count.clear();
                 history_alarm_tablelocks_mutex.unlock();
                 current_article_number = 0;
               }
             if(tableviewflag == 2)
               {
                 {
                   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","timer_int");
                   db.setDatabaseName(HISTORY_EPSWORK_RECORD_DB);     // 连接历史数据库
                   if(db.isOpen()){
                       db.close();
                     }
                 }
                 QSqlDatabase::removeDatabase("timer_int");
                 QFile::remove(HISTORY_EPSWORK_RECORD_DB);

                 ui->tableWidget_2->clearContents();

                 history_EpsWork_Record_mutex.lockForWrite();
                 history_EpsWork_Record.clear();
                 history_EpsWork_Record_count.clear();
                 history_EpsWork_Record_mutex.unlock();
                 current_article_number = 0;
               }
             if(tableviewflag == 3)
               {
                 {
                   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","timer_int");
                   db.setDatabaseName(HISTORY_OPERATION_RECORD_DB);     // 连接历史数据库
                   if(db.isOpen()){
                       db.close();
                     }
                 }
                 QSqlDatabase::removeDatabase("timer_int");
                 QFile::remove(HISTORY_OPERATION_RECORD_DB);

                 ui->tableWidget_3->clearContents();

                 history_Operation_Record_mutex.lockForWrite();
                 history_Operation_Record.clear();
                 history_Operation_Record_count.clear();
                 history_Operation_Record_mutex.unlock();
                 current_article_number = 0;
               }
           }
       }
     else
       {
         QMessageBox msgBox;
         msgBox.setWindowTitle(tr("提示"));
         msgBox.setText(tr("密码错误!"));
         msgBox.setIcon(QMessageBox::Information);
         msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
         msgBox.exec();
       }
   }

}
*/
void AlarmQueryForm::on_historypushButton_clicked()
{
 if(!this->isHidden()){

   QMessageBox msgBox;
   msgBox.setWindowTitle(tr("提示"));
   msgBox.setText(tr("确定删除历史记录?"));
   msgBox.setIcon(QMessageBox::Information);
   msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
   if(msgBox.exec()  == QMessageBox::Ok){
       if(ui->tabWidget->currentIndex() == 0)
         {

           {
             QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","timer_int112");
             db.setDatabaseName(HISTORY_FAULT_DB);     // 连接历史数据库
             if(db.isOpen()){
                 db.close();
               }
           }
           QSqlDatabase::removeDatabase("timer_int112");
           QFile::remove(HISTORY_FAULT_DB);

           ui->tableWidget_4->clearContents();

           history_fault_tablelocks_mutex.lockForWrite();
           history_fault_eventlog.clear();
           history_fault_count.clear();
           history_fault_tablelocks_mutex.unlock();
           current_article_number = 0;
           ls_current_article_number = 0;

         }

       if(ui->tabWidget->currentIndex() == 1)
         {

           {
             QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","timer_int");
             db.setDatabaseName(HISTORY_ALARM_DB);     // 连接历史数据库
             if(db.isOpen()){
                 db.close();
               }
           }
           QSqlDatabase::removeDatabase("timer_int");
           QFile::remove(HISTORY_ALARM_DB);

           ui->tableWidget_1->clearContents();

           history_alarm_tablelocks_mutex.lockForWrite();
           history_alarm_eventlog.clear();
           history_alarm_count.clear();
           history_alarm_tablelocks_mutex.unlock();
           current_article_number = 0;
           ls_current_article_number = 0;

         }
       if(ui->tabWidget->currentIndex() == 2)
         {
           {
             QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","timer_int");
             db.setDatabaseName(HISTORY_EPSWORK_RECORD_DB);     // 连接历史数据库
             if(db.isOpen()){
                 db.close();
               }
           }
           QSqlDatabase::removeDatabase("timer_int");
           QFile::remove(HISTORY_EPSWORK_RECORD_DB);

           ui->tableWidget_2->clearContents();

           history_EpsWork_Record_mutex.lockForWrite();
           history_EpsWork_Record.clear();
           history_EpsWork_Record_count.clear();
           history_EpsWork_Record_mutex.unlock();
           current_article_number = 0;
           ls_current_article_number = 0;

         }
       if(ui->tabWidget->currentIndex() == 3)
         {
           {
             QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","timer_int");
             db.setDatabaseName(HISTORY_OPERATION_RECORD_DB);     // 连接历史数据库
             if(db.isOpen()){
                 db.close();
               }
           }
           QSqlDatabase::removeDatabase("timer_int");
           QFile::remove(HISTORY_OPERATION_RECORD_DB);

           ui->tableWidget_3->clearContents();

           history_Operation_Record_mutex.lockForWrite();
           history_Operation_Record.clear();
           history_Operation_Record_count.clear();
           history_Operation_Record_mutex.unlock();
           current_article_number = 0;
           ls_current_article_number = 0;

         }

     }
 }
}


/*******************方向按键*************************/
void AlarmQueryForm::on_pushButton_homepage_clicked()
{
 ls_current_article_number = 0;
// current_article_number =0;
}

void AlarmQueryForm::on_pushButton_uppage_clicked()
{

 if(ls_current_article_number > 0){
   ls_current_article_number --;
 }

// if(current_article_number > 0){
//   current_article_number --;
// }
}

void AlarmQueryForm::on_pushButton_nextpage_clicked()
{
   if(ui->tabWidget->currentIndex() == 0){
   //  qDebug()<<"AlarmQueryForm--nextpage01 "<<history_fault_count.count()<<current_article_number<<MAXRAM;

     if(history_fault_count.count()%MAXRAM != 0){
       if(ls_current_article_number < history_fault_count.count()/MAXRAM){
         ls_current_article_number ++;
       }
     }else{
       if(ls_current_article_number < history_fault_count.count()/MAXRAM -1){
         ls_current_article_number ++;
       }
     }
  //   qDebug()<<"AlarmQueryForm--nextpage02 "<<history_fault_count.count()<<current_article_number<<MAXRAM;

   }else if(ui->tabWidget->currentIndex() == 1){
     if(history_alarm_count.count()%MAXRAM != 0){
       if(ls_current_article_number < history_alarm_count.count()/MAXRAM){
         ls_current_article_number ++;
       }
     }else{
       if(ls_current_article_number < history_alarm_count.count()/MAXRAM -1){
         ls_current_article_number ++;
       }
     }
   }else if(ui->tabWidget->currentIndex() == 2){
     if(history_EpsWork_Record_count.count()%MAXRAM != 0){
       if(ls_current_article_number < history_EpsWork_Record_count.count()/MAXRAM){
         ls_current_article_number ++;
       }
     }else{
       if(ls_current_article_number < history_EpsWork_Record_count.count()/MAXRAM -1){
         ls_current_article_number ++;
       }
     }
   }else if(ui->tabWidget->currentIndex() == 3){
     if(history_Operation_Record_count.count()%MAXRAM != 0){
       if(ls_current_article_number < history_Operation_Record_count.count()/MAXRAM){
         ls_current_article_number ++;
       }
     }else{
       if(ls_current_article_number < history_Operation_Record_count.count()/MAXRAM -1){
         ls_current_article_number ++;
       }
     }
   }
}

void AlarmQueryForm::on_pushButton_endpage_clicked()
{
 if(ui->tabWidget->currentIndex() == 0){
   //qDebug()<<"AlarmQueryForm--endpage01 "<<history_fault_count.count()<<current_article_number<<MAXRAM;
   if(history_fault_count.count()%MAXRAM != 0){
       ls_current_article_number = history_fault_count.count()/MAXRAM;
   }else if(history_fault_count.count() > 0)
     ls_current_article_number = history_fault_count.count()/MAXRAM-1;

  // qDebug()<<"AlarmQueryForm --endpage02"<<history_fault_count.count()<<current_article_number<<MAXRAM;

 }else if(ui->tabWidget->currentIndex() == 1){
   if(history_alarm_count.count()%MAXRAM != 0){
     ls_current_article_number = history_alarm_count.count()/MAXRAM;
   }else if(history_alarm_count.count() > 0)
     ls_current_article_number = history_alarm_count.count()/MAXRAM-1;
 }else if(ui->tabWidget->currentIndex() == 2){
   if(history_EpsWork_Record_count.count()%MAXRAM != 0){
     ls_current_article_number = history_EpsWork_Record_count.count()/MAXRAM;
   }else if(history_EpsWork_Record_count.count() > 0)
     ls_current_article_number = history_EpsWork_Record_count.count()/MAXRAM-1;
 }else if(ui->tabWidget->currentIndex() == 3){
   if(history_Operation_Record_count.count()%MAXRAM != 0){
     ls_current_article_number = history_Operation_Record_count.count()/MAXRAM;
   }else if(history_Operation_Record_count.count() > 0)
    ls_current_article_number = history_Operation_Record_count.count()/MAXRAM-1;
 }
}



void AlarmQueryForm::AlarmOverloadUI()
{
 if(Usr_Engir_Flag == 1)
   ui->historypushButton->setVisible(false);
 else if(Usr_Engir_Flag == 0)
   ui->historypushButton->setVisible(true);
}
