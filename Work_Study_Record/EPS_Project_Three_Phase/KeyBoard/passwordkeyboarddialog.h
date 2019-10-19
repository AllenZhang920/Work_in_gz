#ifndef PASSWORDKEYBOARDDIALOG_H
#define PASSWORDKEYBOARDDIALOG_H

#include <QDialog>

namespace Ui {
class PasswordkeyboardDialog;
}

class PasswordkeyboardDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PasswordkeyboardDialog(QWidget *parent = 0);

    ~PasswordkeyboardDialog();

signals:
    void PressKey(QString key);
private slots:
    void on_oneButton_clicked();

    void on_twoButton_clicked();

    void on_threeButton_clicked();

    void on_fourButton_clicked();

    void on_fiveButton_clicked();

    void on_sixButton_clicked();

    void on_sevenButton_clicked();

    void on_nightButton_clicked();

    void on_nineButton_clicked();

    void on_zeroButton_clicked();

    void on_clearButton_clicked();

    void on_escButton_clicked();

    void on_enterButton_clicked();

    void on_pushButton_back_clicked();

private:
    Ui::PasswordkeyboardDialog *ui;
};

#endif // PASSWORDKEYBOARDDIALOG_H
