#ifndef ALARMQUERYFORM_H
#define ALARMQUERYFORM_H

#include <QWidget>
#include"Share/Globalvar.h"
#define MAXRAM 9    //最大行数
namespace Ui {
class AlarmQueryForm;
}

class AlarmQueryForm : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmQueryForm(QWidget *parent = 0);
    ~AlarmQueryForm();
    INT32 current_article_number;
    INT32 ls_current_article_number;

    void dislpay_alarm_data();
    void AlarmOverloadUI();
 //   void alarm_delete();
    QList<HISTORY_ALARMFAULT_EVENT> history_alarm_count;
    QList<HISTORY_ALARMFAULT_EVENT> history_fault_count;
    QList<OPERATION_WORK_RECORD>history_Operation_Record_count; // 存放当前历史信息记录
    QList<OPERATION_WORK_RECORD>history_EpsWork_Record_count; // 操作记录以及工作记录
//signals:
 //   void keyboard_show();

private slots:
    void on_pushButton_endpage_clicked();
    void on_pushButton_nextpage_clicked();
    void on_pushButton_uppage_clicked();
    void on_pushButton_homepage_clicked();

    void on_historypushButton_clicked();
    void on_tabWidget_currentChanged(int index);
 //   void AlarmDelete(QString pwd);
//    void Delete_history_data(QString password);
    //void on_dateEdit_dateChanged(const QDate &date);
private:
    Ui::AlarmQueryForm *ui;
    void alarmquerytableview_int();//告警界面图形初始化
   // UINT32 tableviewflag ;   //当前显示为什么页面

};

#endif // ALARMQUERYFORM_H
