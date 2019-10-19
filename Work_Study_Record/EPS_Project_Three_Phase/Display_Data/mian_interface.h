#ifndef MIAN_INTERFACE_H
#define MIAN_INTERFACE_H

#include <QWidget>
#include <QMovie>
#include"Share/Globalvar.h"
#include "Display_Data/startup_shutdown.h"

namespace Ui {

class Mian_Interface;

}

class Mian_Interface : public QWidget
{
    Q_OBJECT
    
public:
    explicit Mian_Interface(QWidget *parent = 0);
    ~Mian_Interface();
    void Screen_Refresh_Data();
signals:
    void Changed_Current_View(int index, int Param, int Param_1, int SeNumber);
    void keyboard_show();

private slots:
    void on_but_menu_clicked();

    void on_but_log_clicked();

    void InputMenu(QString pwd);

    void on_Startup_Shutdown_clicked();
    void on_Silent_Button_clicked();

private:
    Ui::Mian_Interface *ui;
    QMovie* movieline;
    void    Flow_Graph_show(UINT32 NO);
    void    Flow_Graph();
    UINT32  Olde_NO;
    UINT32  inputButton;

};

#endif // MIAN_INTERFACE_H
