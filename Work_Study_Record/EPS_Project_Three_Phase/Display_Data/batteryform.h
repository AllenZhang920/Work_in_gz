#ifndef BATTERYFORM_H
#define BATTERYFORM_H

#include <QWidget>

#include <QtGui>

#include"Share/Globalvar.h"

namespace Ui {

 class BatteryForm;

}

class BatteryForm : public QDialog
{
    Q_OBJECT
    
public:
    explicit BatteryForm(QWidget *parent = 0);
    ~BatteryForm();
     void timerUpdata();
     void Choosewhich_interface(int index);
     void battery_int();
     void Battery_choice_int();

private slots:
    void on_betterypack_choice_currentIndexChanged(int index);

    void on_pushButton_endpage_clicked();

    void on_pushButton_nextpage_clicked();

    void on_pushButton_uppage_clicked();

    void on_pushButton_homepage_clicked();

private:
    Ui::BatteryForm *ui;

    UINT32 batteryNumber[BATTERYMAXGROUP];  //各组模块的各个电池

    UINT32 TotalNumberpages[BATTERYMAXGROUP];//各组模块的总页数
    UINT32 minVoltage ;
    UINT32 maxVoltage ;
    UINT32 CurrentNumberpages;//当前页数
    UINT32 packnumber;
    void Battery_NotFullpage_function();

    void Battery_Fullpage_function();

    UINT32  MonomerVoltage[24*5]; //电池 16*24
};

#endif // BATTERYFORM_H
