#include "interface_frame_work.h"
#include <QApplication>
#include"application.h"

#include"Share/param.h"

#ifdef _ARM_
Q_IMPORT_PLUGIN(optkeypad)  //导出静态插件库
#endif


static int iscalibrate(void)
{
  int ret=0;//
  if(access("/etc/pointercal",F_OK)){//文件是否存在 ，存在返回0，不存在返回-1
    ret=-1;
  }else{
    ret=7;
    char pcalbuf[100]d;
    char *tokptr;
    FILE* pcal_fp = fopen("/etc/pointercal","r");
    if(pcal_fp==NULL){
      remove("/etc/pointercal");
      return -1;
    }
    memset(pcalbuf,0,100);
    fgets(pcalbuf,100,pcal_fp);
    fclose(pcal_fp);
    tokptr = strtok(pcalbuf," ");
    while(tokptr!=NULL){
      tokptr = strtok(NULL," ");
      ret--;
      if(!ret)break;
    }

    if(ret){
      remove("/etc/pointercal");
      ret=-1;
    }
  }
  return ret;
}

int main(int argc, char *argv[])
{
  Application a(argc, argv);

  //添加字符编码模式
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

  a.setWindowIcon(QIcon(":/images/settingnormal.png"));

  //系统初始化
  System_Config_File_Init();

  //加载翻译文件
  if(!Globa.User_Monitor_Screen_Set.System_language)
    {
      QTranslator *qtGloble = new QTranslator();
      qtGloble->load(":/Qm/qt_zh_CN.qm");
      QCoreApplication::installTranslator(qtGloble);

      QTranslator * trans= new QTranslator();
      trans->load(":/Qm/EPS_Project_zh.qm");
      QCoreApplication::installTranslator(trans);
    }

  //加载样式表文件
  QFile qss(":/Qss/style_cn.qss");
  qss.open(QFile::ReadOnly);
  a.setStyleSheet(qss.readAll());
  qss.close();

  Interface_Frame_Work w;
  w.show();

  if(iscalibrate()!=0){
    w.open_cal();
  }
  return a.exec();
}
