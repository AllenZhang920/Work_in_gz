#ifndef CURRENT_ALARM_QUERY_H
#define CURRENT_ALARM_QUERY_H

#include <QWidget>
#include"Share/Globalvar.h"

#define MAXRAM_CAQ 9   //最大行数

namespace Ui {
  class Current_Alarm_Query;
}

class Current_Alarm_Query : public QWidget
{
  Q_OBJECT
  
public:
  explicit Current_Alarm_Query(QWidget *parent = 0);
  ~Current_Alarm_Query();
  void dislpay_alarm_data();

private slots:
    void on_pushButton_endpage_clicked();
    void on_pushButton_nextpage_clicked();
    void on_pushButton_uppage_clicked();
    void on_pushButton_homepage_clicked();

    void on_tabWidget_currentChanged(int index);

private:
  Ui::Current_Alarm_Query *ui;
  QList<CURRENT_ALARMFAULT_EVENT> current_alarm_count;
  QList<CURRENT_ALARMFAULT_EVENT> current_fault_count;
  void alarmquerytableview_int();
  INT32 current_article_number;
  INT32 ls_current_article_number;
  INT8 pageIndex;
};

#endif // CURRENT_ALARM_QUERY_H
