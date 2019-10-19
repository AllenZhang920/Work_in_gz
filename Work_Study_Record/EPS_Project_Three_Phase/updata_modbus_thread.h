#ifndef UPDATA_MODBUS_THREAD_H
#define UPDATA_MODBUS_THREAD_H

#include <QThread>
#include"Share/param.h"
class Updata_Modbus_Thread : public QThread
{
  Q_OBJECT
public:
  explicit Updata_Modbus_Thread(QObject *parent = 0);
  void run();
  void Updata_Modbus_Data();
  void Updata_ModbusFun03_Config();
signals:
  
public slots:
  
};

#endif // UPDATA_MODBUS_THREAD_H
