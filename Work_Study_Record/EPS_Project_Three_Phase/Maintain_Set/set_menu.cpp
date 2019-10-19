#include "set_menu.h"
#include "ui_set_menu.h"
#include <QDebug>
Setmenu::Setmenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setmenu)
{
    this->setObjectName("widget");
    ui->setupUi(this);
    this->setGeometry(0,0,800,480);
    this->setProperty("object",true);
    inputview=0;

    //样式表
    ui->but_user->setObjectName("styleButton");
    ui->but_defend->setObjectName("styleButton");
    ui->but_debug->setObjectName("styleButton");
}

Setmenu::~Setmenu()
{
    delete ui;
}

void Setmenu::OverloadUI()
{
  if(Usr_Engir_Flag == 1){
    ui->but_debug->setVisible(false);
    ui->but_user->setGeometry(300,120,200,70);
    ui->but_defend->setGeometry(300,260,200,70);
  }else if(Usr_Engir_Flag == 0){
    ui->but_debug->setVisible(true);
    ui->but_user->setGeometry(300,90,200,70);
    ui->but_defend->setGeometry(300,190,200,70);
  }
}

void Setmenu::on_but_user_clicked()//监控设置
{
  emit Changed_Current_View(USER_SYS_SET_VIEW, 0, 0, 0);
}

void Setmenu::on_but_defend_clicked()//客户设置
{
  emit Changed_Current_View(USER_SET_VIEW, 0, 0, 0);
}

void Setmenu::on_but_debug_clicked()//维护设置
{
  emit Changed_Current_View(MAINTAIN_SET, 0, 0, 0);

}

