#ifndef IMPORT_EXPORT_DATA_H
#define IMPORT_EXPORT_DATA_H

#include <QWidget>
#include"Share/Globalvar.h"
#include "unistd.h"
#include "Display_Data/reboot.h"
#include"Share/param.h"
#include <iconv.h>

namespace Ui {
  class Import_Export_Data;
}

class Import_Export_Data : public QWidget
{
  Q_OBJECT
  
public:
  explicit Import_Export_Data(QWidget *parent = 0);
  ~Import_Export_Data();
  void IEdataloadUI();
  
private slots:
  void on_btn_updatasoft_clicked();
  void on_btn_dataexport_clicked();
  void on_btn_sysfileinput_clicked();
  void on_btn_sysfileoutput_clicked();



private:
  Ui::Import_Export_Data *ui;


  FILE *fp;
  FILE *fp1;
  FILE *fp2;
  FILE *fp4;
  char file_name[250];

  void Read_HistoryAlarm_to_file(QString datafilename);
  bool Export_HistoryAlarm_data(const QString &dirName);

  void Read_HistoryFault_to_file(QString datafilename);
  bool Export_HistoryFault_data(const QString &dirName);



  void Read_WorkState_to_file(QString datafilename);
  bool Export_WorkStation_data(const QString &dirName);

  void Read_ManualRecord_to_file(QString datafilename);
  bool Export_ManualRecord_data(const QString &dirName);

  QString Utf8_To_GB(QString strText);
  int Gb2312ToUtf8(char *sOut, int iMaxOutLen, const char *sIn, int iInLen);
  int Utf8ToGb2312(char *sOut, int iMaxOutLen, const char *sIn, int iInLen);

  bool Copy_Directory_Files(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
  bool Copy_File(char *oldfilePath, char *newfilePath);
};

#endif // IMPORT_EXPORT_DATA_H
