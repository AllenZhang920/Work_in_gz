#include "popup_keboarddialog.h"
#include "ui_popup_keboarddialog.h"

#include <QDebug>

POPUP_KEBOARDDialog::POPUP_KEBOARDDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::POPUP_KEBOARDDialog)
{
   ui->setupUi(this);

   this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    this->setGeometry(145,336,515,128);

    this->hide();
    ui->oneButton->setObjectName("calc");
    ui->twoButton->setObjectName("calc");
    ui->threeButton->setObjectName("calc");
    ui->fourButton->setObjectName("calc");
    ui->fiveButton->setObjectName("calc");
    ui->sixButton->setObjectName("calc");
    ui->sevenButton->setObjectName("calc");
    ui->nightButton->setObjectName("calc");
    ui->nineButton->setObjectName("calc");
    ui->zeroButton->setObjectName("calc");
    ui->pointButton->setObjectName("calc");
    ui->escButton->setObjectName("calcc");
    ui->pushButton_back->setObjectName("calcdel");
    ui->clearButton->setObjectName("calcdel");
    ui->enterButton->setObjectName("calcok");
}


POPUP_KEBOARDDialog::~POPUP_KEBOARDDialog()
{
    delete ui;

}


void POPUP_KEBOARDDialog::on_clearButton_clicked()
{

    emit PressKey(10);
}

void POPUP_KEBOARDDialog::on_zeroButton_clicked()
{
    emit PressKey(0);
}

void POPUP_KEBOARDDialog::on_oneButton_clicked()
{

    emit PressKey(1);

}

void POPUP_KEBOARDDialog::on_threeButton_clicked()
{
    emit PressKey(3);
}

void POPUP_KEBOARDDialog::on_twoButton_clicked()
{
    emit PressKey(2);
}

void POPUP_KEBOARDDialog::on_fourButton_clicked()
{
    emit PressKey(4);
}

void POPUP_KEBOARDDialog::on_fiveButton_clicked()
{
    emit PressKey(5);
}

void POPUP_KEBOARDDialog::on_sixButton_clicked()
{
    emit PressKey(6);
}

void POPUP_KEBOARDDialog::on_sevenButton_clicked()
{
    emit PressKey(7);
}

void POPUP_KEBOARDDialog::on_nightButton_clicked()
{
    emit PressKey(8);
}

void POPUP_KEBOARDDialog::on_nineButton_clicked()
{
    emit PressKey(9);
}

void POPUP_KEBOARDDialog::on_escButton_clicked()
{
    emit PressKey(11);
    this->hide();
}

void POPUP_KEBOARDDialog::on_enterButton_clicked()
{
    emit PressKey(12);
    this->hide();
}

void POPUP_KEBOARDDialog::on_pointButton_clicked()
{
  emit PressKey(13);
}

void POPUP_KEBOARDDialog::on_pushButton_back_clicked()
{
    emit PressKey(14);

}
