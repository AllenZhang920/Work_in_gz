#include "passwordkeyboarddialog.h"
#include "ui_passwordkeyboarddialog.h"

PasswordkeyboardDialog::PasswordkeyboardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordkeyboardDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
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
    ui->escButton->setObjectName("calcc");
    ui->pushButton_back->setObjectName("calcdel");
    ui->clearButton->setObjectName("calcdel");
    ui->enterButton->setObjectName("calcok");

}
PasswordkeyboardDialog::~PasswordkeyboardDialog()
{
    delete ui;
}

void PasswordkeyboardDialog::on_oneButton_clicked()
{
    ui->lineEdit_password->setText(ui->lineEdit_password->text()+QString("%1").arg(1));
}

void PasswordkeyboardDialog::on_twoButton_clicked()
{
    ui->lineEdit_password->setText(ui->lineEdit_password->text()+QString("%1").arg(2));
}

void PasswordkeyboardDialog::on_threeButton_clicked()
{
    ui->lineEdit_password->setText(ui->lineEdit_password->text()+QString("%1").arg(3));
}

void PasswordkeyboardDialog::on_fourButton_clicked()
{
    ui->lineEdit_password->setText(ui->lineEdit_password->text()+QString("%1").arg(4));
}

void PasswordkeyboardDialog::on_fiveButton_clicked()
{
    ui->lineEdit_password->setText(ui->lineEdit_password->text()+QString("%1").arg(5));
}

void PasswordkeyboardDialog::on_sixButton_clicked()
{
    ui->lineEdit_password->setText(ui->lineEdit_password->text()+QString("%1").arg(6));
}

void PasswordkeyboardDialog::on_sevenButton_clicked()
{
    ui->lineEdit_password->setText(ui->lineEdit_password->text()+QString("%1").arg(7));
}

void PasswordkeyboardDialog::on_nightButton_clicked()
{
    ui->lineEdit_password->setText(ui->lineEdit_password->text()+QString("%1").arg(8));
}

void PasswordkeyboardDialog::on_nineButton_clicked()
{
    ui->lineEdit_password->setText(ui->lineEdit_password->text()+QString("%1").arg(9));
}

void PasswordkeyboardDialog::on_zeroButton_clicked()
{
    ui->lineEdit_password->setText(ui->lineEdit_password->text()+QString("%1").arg(0));
}

void PasswordkeyboardDialog::on_clearButton_clicked()
{
    ui->lineEdit_password->setText("");
}

void PasswordkeyboardDialog::on_pushButton_back_clicked()
{
    ui->lineEdit_password->setText(ui->lineEdit_password->text().mid(0,ui->lineEdit_password->text().count()-1));

}

void PasswordkeyboardDialog::on_escButton_clicked()
{
    this->close();
    ui->lineEdit_password->setText("");

}

void PasswordkeyboardDialog::on_enterButton_clicked()
{
    emit PressKey(ui->lineEdit_password->text());
    // ui->lineEdit_password->setText("");
    this->close();
    ui->lineEdit_password->setText("");


}


