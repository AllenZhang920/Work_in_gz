#ifndef ZMESSAGE_H
#define ZMESSAGE_H

#include <QDialog>

namespace Ui {
  class ZMessage;
}

class ZMessage : public QDialog
{
  Q_OBJECT
  
public:
  explicit ZMessage(QWidget *parent = 0);
  ~ZMessage();
  void setText(QString str);

private:
  Ui::ZMessage *ui;
};

#endif // ZMESSAGE_H
