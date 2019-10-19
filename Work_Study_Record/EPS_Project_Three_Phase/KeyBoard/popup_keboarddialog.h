#ifndef POPUP_KEBOARDDIALOG_H
#define POPUP_KEBOARDDIALOG_H

#include <QDialog>

namespace Ui {
class POPUP_KEBOARDDialog;
}

class POPUP_KEBOARDDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit POPUP_KEBOARDDialog(QWidget *parent = 0);

    ~POPUP_KEBOARDDialog();

private slots:
    void on_clearButton_clicked();

    void on_zeroButton_clicked();

    void on_oneButton_clicked();

    void on_threeButton_clicked();

    void on_twoButton_clicked();

    void on_fourButton_clicked();

    void on_fiveButton_clicked();

    void on_sixButton_clicked();

    void on_sevenButton_clicked();

    void on_nightButton_clicked();

    void on_nineButton_clicked();

    void on_escButton_clicked();

    void on_enterButton_clicked();

    void on_pointButton_clicked();

    void on_pushButton_back_clicked();

signals:
    void PressKey(int key);

private:
    Ui::POPUP_KEBOARDDialog *ui;

};

#endif // POPUP_KEBOARDDIALOG_H
