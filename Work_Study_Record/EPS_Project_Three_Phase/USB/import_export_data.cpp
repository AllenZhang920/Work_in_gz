#include "import_export_data.h"
#include "ui_import_export_data.h"

Import_Export_Data::Import_Export_Data(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Import_Export_Data)
{
  ui->setupUi(this);
  this->setGeometry(0,0,800,480);

  ui->progressBar->setRange(0,99);
  ui->progressBar->setValue(0);


  ui->label->setStyleSheet("color:rgb(255,0,0);");

  //样式表
  ui->btn_updatasoft->setObjectName("styleButton");
  ui->btn_sysfileinput->setObjectName("styleButton");

  ui->btn_dataexport->setObjectName("styleButton");
  ui->btn_sysfileoutput->setObjectName("styleButton");

}

Import_Export_Data::~Import_Export_Data()
{
  delete ui;
}



//**********************导出历史告警*********************************
void Import_Export_Data::Read_HistoryAlarm_to_file(QString datafilename)
{
  char buff[250];
  char new_buff[250];

  //QByteArray ba_datafilename = datafilename.toAscii();
  //sprintf(buff, "%s\n",ba_datafilename.data());
  //printf("Read_HistoryAlarm_to_file = %s",buff);


  {
    QSqlDatabase db =QSqlDatabase::addDatabase("QSQLITE","Read_HistoryAlarm_to_file");
    db.setDatabaseName(datafilename);     // 连接当前告警数据库
    if(db.open()){
        QSqlQuery query(db);

        query.exec("select name, start_time,end_time from history_Alarm_Eventlog");

        while(query.next())
          {
            QByteArray ba_name = query.value(0).toString().toLocal8Bit();
            QByteArray ba_start_time = query.value(1).toString().toLatin1();
            QByteArray ba_end_time = query.value(2).toString().toLatin1();
            sprintf(buff, "%s, %s, %s\n",ba_name.data(),ba_start_time.data(),ba_end_time.data());
            Utf8ToGb2312(new_buff, sizeof(buff), buff, sizeof(buff));
            //            printf("\n\n %s \n", new_buff);
            fputs(new_buff, fp);
          }
        query.clear();
        db.close();
      }
  }
  QSqlDatabase::removeDatabase("Read_HistoryAlarm_to_file");

}
void Import_Export_Data::Read_HistoryFault_to_file(QString datafilename)
{
  char buff[250];
  char new_buff[250];

  //QByteArray ba_datafilename = datafilename.toAscii();
  //sprintf(buff, "%s\n",ba_datafilename.data());
  //printf("Read_HistoryAlarm_to_file = %s",buff);


  {
    QSqlDatabase db =QSqlDatabase::addDatabase("QSQLITE","Read_HistoryFault_to_file");
    db.setDatabaseName(datafilename);     // 连接当前告警数据库
    if(db.open()){
        QSqlQuery query(db);

        query.exec("select name, start_time,end_time from history_Fault_Eventlog");

        while(query.next())
          {
            QByteArray ba_name = query.value(0).toString().toLocal8Bit();
            QByteArray ba_start_time = query.value(1).toString().toLatin1();
            QByteArray ba_end_time = query.value(2).toString().toLatin1();
            sprintf(buff, "%s, %s, %s\n",ba_name.data(),ba_start_time.data(),ba_end_time.data());
            Utf8ToGb2312(new_buff, sizeof(buff), buff, sizeof(buff));
            //            printf("\n\n %s \n", new_buff);
            fputs(new_buff, fp4);
          }
        query.clear();
        db.close();
      }
  }
  QSqlDatabase::removeDatabase("Read_HistoryFault_to_file");

}
bool Import_Export_Data::Export_HistoryAlarm_data(const QString &dirName)
{ printf("Export_HistoryAlarm_data\n");//add by zzc 625
  static QVector<QString> dirNames;
  QDir dir;
  char buff[250]={0};
  char new_buff[250]={0};
  QString data_file_name;
  int fd;

  fp = fopen(file_name, "wb+");
  fd = fileno(fp);
  if(NULL == fp)
    {
      perror("open HistoryAlarm error \n");
      return false;
    }
  else
    {
      printf("open HistoryAlarm ok \n");
      sprintf(buff, "故障名称, 开始时间, 结束时间\n");
      Utf8ToGb2312(new_buff, sizeof(buff), buff, sizeof(buff));
      fputs(new_buff, fp);
    }

  //初始化
  dirNames.clear();
  if(dir.exists()){
      dirNames<<dirName;
    }else{
      return false;
    }
  /*
  //遍历各级文件夹，并将这些文件夹中的文件删除
  QFileInfoList filst;
  QFileInfoList::iterator curFi;
  for(int i=0;i<dirNames.size();++i)
    {
      dir.setPath(dirNames[i]);
      filst=dir.entryInfoList(QDir::Dirs|QDir::Files
                              |QDir::Readable|QDir::Writable
                              |QDir::Hidden|QDir::NoDotAndDotDot
                              ,QDir::Name);
      if(filst.size()>0){
          curFi=filst.begin();
          //  qDebug()<<"_____0010001_____-------"<<dir.filePath(curFi->fileName());
          while(curFi!=filst.end()){
              //遇到文件夹,则添加至文件夹列表dirs尾部
              data_file_name = dir.filePath(curFi->fileName());
              if(curFi->isDir()){
                  dirNames.push_back(curFi->filePath());
                }else if(curFi->isFile()){
                  Read_HistoryAlarm_to_file(data_file_name, Section);

                }
              curFi++;
            }
          fputs("[End]\n", fp);
          fflush(fp);
          fsync(fd);
          sleep(1);
          fclose(fp);
          sleep(1);
          fp = fopen(file_name, "r");
          if(NULL == fp){
              perror("open battery_datatable error \n");
              fclose(fp);
            }else{
              printf("open battery_datatable----------- ok \n");
              int ret = fseek(fp,-6,SEEK_END);
              fgets(buff, 250, fp);
              printf("----------------------==%d==%s\n",ret,buff);
              if(strcmp(buff, "[End]\n") == 0){
                  printf("battery_datatable----文件完整--- ok \n");
                  fclose(fp);
                  return true;
                }
            }
        }
    }
  */
  data_file_name = dirName;
  Read_HistoryAlarm_to_file(data_file_name);
  fputs("[End]\n", fp);
  fflush(fp);
  fsync(fd);
  sleep(1);
  fclose(fp);
  sleep(1);
  fp = fopen(file_name, "r");
  if(NULL == fp)
    {
      perror("open HistoryAlarm error \n");
      fclose(fp);
    }
  else
    {
      printf("open HistoryAlarm----------- ok \n");
      int ret = fseek(fp,-6,SEEK_END);
      fgets(buff, 250, fp);
      printf("----------------------==%d==%s\n",ret,buff);
      if( strcmp(buff, "[End]\n") == 0 )
        {
          printf("HistoryAlarm----文件完整--- ok \n");
          fclose(fp);
          return true;
        }
    }
  return false;
}
bool Import_Export_Data::Export_HistoryFault_data(const QString &dirName)
{
  static QVector<QString> dirNames;
  QDir dir;
  char buff[250]={0};
  char new_buff[250]={0};
  QString data_file_name;
  int fd;

  fp4 = fopen(file_name, "wb+");
  fd = fileno(fp4);
  if(NULL == fp4)
    {
      perror("open HistoryFault error \n");
      return false;
    }
  else
    {
      printf("open HistoryFault ok \n");
      sprintf(buff, "故障名称, 开始时间, 结束时间\n");
      Utf8ToGb2312(new_buff, sizeof(buff), buff, sizeof(buff));
      fputs(new_buff, fp4);
    }

  //初始化
  dirNames.clear();
  if(dir.exists()){
      dirNames<<dirName;
    }else{
      return false;
    }
  data_file_name = dirName;
  Read_HistoryFault_to_file(data_file_name);
  fputs("[End]\n", fp4);
  fflush(fp4);
  fsync(fd);
  sleep(1);
  fclose(fp4);
  sleep(1);
  fp4 = fopen(file_name, "r");
  if(NULL == fp4)
    {
      perror("open HistoryFault error \n");
      fclose(fp4);
    }
  else
    {
      printf("open HistoryFault----------- ok \n");
      int ret = fseek(fp4,-6,SEEK_END);
      fgets(buff, 250, fp4);
      printf("----------------------==%d==%s\n",ret,buff);
      if( strcmp(buff, "[End]\n") == 0 )
        {
          printf("HistoryFault----文件完整--- ok \n");
          fclose(fp4);
          return true;
        }
    }
  return false;
}
//*********************导出EPS工作状态*******************
void Import_Export_Data::Read_WorkState_to_file(QString datafilename)
{
  char buff[250];
  char new_buff[250];
  {
    QSqlDatabase db =QSqlDatabase::addDatabase("QSQLITE","Read_WorkState_to_file");
    db.setDatabaseName(datafilename);     // 连接当前告警数据库
    if(db.open()){
        QSqlQuery query(db);
        query.exec("select * from history_EpsWork_Record");
        while(query.next())
          {
            QByteArray ba_name = query.value(0).toString().toLocal8Bit();//toLocal8Bit();
            QByteArray ba_time = query.value(1).toString().toLatin1();
            sprintf(buff, "%s, %s\n",ba_name.data(),ba_time.data());
            Utf8ToGb2312(new_buff, sizeof(buff), buff, sizeof(buff));
            fputs(new_buff, fp1);
          }
        query.clear();
        db.close();
      }
  }
  QSqlDatabase::removeDatabase("Read_WorkState_to_file");

}

bool Import_Export_Data::Export_WorkStation_data(const QString &dirName)
{printf("Export_WorkStation_data\n");//add by zzc 625
  static QVector<QString> dirNames;
  QDir dir;
  QString data_file_name;
  char buff[250]={0};
  char new_buff[250]={0};
  int fd;

  fp1 = fopen(file_name, "wb+");
  fd = fileno(fp1);
  if(NULL == fp1)
    {
      perror("open Export_WorkStation error\n");
      return false;
    }
  else
    {
      printf("open Export_WorkStation ok \n");
      sprintf(buff, "状态信息, 时间\n");
      Utf8ToGb2312(new_buff, sizeof(buff), buff, sizeof(buff));
      fputs(new_buff, fp1);
    }

  //初始化
  dirNames.clear();
  if(dir.exists()){
      dirNames<<dirName;
    }else{
      return false;
    }
  printf("open Export_WorkStation 111ok \n");

  /*
    //遍历各级文件夹，并将这些文件夹中的文件删除
    QFileInfoList filst;
    QFileInfoList::iterator curFi;
    for(int i=0;i<dirNames.size();++i){
      dir.setPath(dirNames[i]);
      filst=dir.entryInfoList(QDir::Dirs|QDir::Files
                             |QDir::Readable|QDir::Writable
                             |QDir::Hidden|QDir::NoDotAndDotDot
                             ,QDir::Name);
      if(filst.size()>0){
        curFi=filst.begin();
        while(curFi!=filst.end()){
          //遇到文件夹,则添加至文件夹列表dirs尾部
          data_file_name=dir.filePath(curFi->fileName());
          if(curFi->isDir()){
            dirNames.push_back(curFi->filePath());
          }else if(curFi->isFile()){
            Read_WorkState_to_file(data_file_name);

          }
           curFi++;
        }

        fputs("[End]\n", fp1);
        fflush(fp1);
        fsync(fd);
        sleep(1);
        fclose(fp1);
        sleep(1);
        fp1 = fopen(file_name, "r");
        if(NULL == fp1){
          perror("open Insulation_data error \n");
          fclose(fp1);
        }else{
           printf("open Insulation_data----------- ok \n");
           int ret = fseek(fp1,-6,SEEK_END);
           fgets(buff, 250, fp1);
           printf("----------------------==%d==%s\n",ret,buff);
           if(strcmp(buff, "[End]\n") == 0){
             printf("Insulation_data----文件完整--- ok \n");
             fclose(fp1);
             return true;
          }
        }
      }
    }
    */
  data_file_name = dirName;
  Read_WorkState_to_file(data_file_name);
  fputs("[End]\n", fp1);
  fflush(fp1);
  fsync(fd);
  sleep(1);
  fclose(fp1);
  sleep(1);
  fp1 = fopen(file_name, "r");
  if(NULL == fp1)
    {
      perror("open Export_WorkStation error \n");
      fclose(fp1);
    }
  else
    {
      printf("open Export_WorkStation----------- ok \n");
      int ret = fseek(fp1,-6,SEEK_END);
      fgets(buff, 250, fp1);
      printf("----------------------==%d==%s\n",ret,buff);
      if( strcmp(buff, "[End]\n") == 0 )
        {
          printf("Export_WorkStation----文件完整--- ok \n");
          fclose(fp1);
          return true;
        }
    }
  return false;
}

//********************导出操作记录*******************
void Import_Export_Data::Read_ManualRecord_to_file(QString datafilename)
{printf("Read_ManualRecord_to_file\n");//add by 625
  char buff[250];
  char new_buff[250];
  {
    QSqlDatabase db =QSqlDatabase::addDatabase("QSQLITE","Read_ManualRecord_to_file");
    db.setDatabaseName(datafilename);     // 连接当前告警数据库
    if(db.open()){
        QSqlQuery query(db);
        query.exec("select * from history_Operation_Record");
        while(query.next())
          {
            QByteArray ba_name = query.value(0).toString().toLocal8Bit();
            QByteArray ba_time = query.value(1).toString().toLatin1();
            sprintf(buff, "%s, %s\n",ba_name.data(),ba_time.data());

            Utf8ToGb2312(new_buff, sizeof(buff), buff, sizeof(buff));            
            fputs(new_buff, fp2);
          }
        query.clear();
        db.close();
      }
  }
  QSqlDatabase::removeDatabase("Read_ManualRecord_to_file");

}

bool Import_Export_Data::Export_ManualRecord_data(const QString &dirName)
{printf("Export_ManualRecord_data\n");//add by zzc 625
  static QVector<QString> dirNames;
  QDir dir;
  QString data_file_name;
  char buff[250]={0};
  char new_buff[250]={0};
  int fd;

  fp2 = fopen(file_name, "wb+");
  fd = fileno(fp2);
  if(NULL == fp2)
    {
      perror("open ManualRecord error \n");
      return false;
    }
  else
    {
      printf("open ManualRecord ok \n");
      sprintf(buff, "操作信息, 时间\n");
      Utf8ToGb2312(new_buff, sizeof(buff), buff, sizeof(buff));
      fputs(new_buff, fp2);
    }

  //初始化
  dirNames.clear();
  if(dir.exists()){
      dirNames<<dirName;
    }else{
      return false;
    }
  /*
    //遍历各级文件夹，并将这些文件夹中的文件删除
    QFileInfoList filst;
    QFileInfoList::iterator curFi;
    for(int i=0;i<dirNames.size();++i){
      dir.setPath(dirNames[i]);
      filst=dir.entryInfoList(QDir::Dirs|QDir::Files
                             |QDir::Readable|QDir::Writable
                             |QDir::Hidden|QDir::NoDotAndDotDot
                             ,QDir::Name);
      if(filst.size()>0){
        curFi=filst.begin();
        while(curFi!=filst.end()){
          //遇到文件夹,则添加至文件夹列表dirs尾部
          data_file_name=dir.filePath(curFi->fileName());
          if(curFi->isDir()){
            dirNames.push_back(curFi->filePath());
          }else if(curFi->isFile()){
            Read_WorkState_to_file(data_file_name);

          }
           curFi++;
        }

        fputs("[End]\n", fp1);
        fflush(fp1);
        fsync(fd);
        sleep(1);
        fclose(fp1);
        sleep(1);
        fp1 = fopen(file_name, "r");
        if(NULL == fp1){
          perror("open Insulation_data error \n");
          fclose(fp1);
        }else{
           printf("open Insulation_data----------- ok \n");
           int ret = fseek(fp1,-6,SEEK_END);
           fgets(buff, 250, fp1);
           printf("----------------------==%d==%s\n",ret,buff);
           if(strcmp(buff, "[End]\n") == 0){
             printf("Insulation_data----文件完整--- ok \n");
             fclose(fp1);
             return true;
          }
        }
      }
    }
    */
  data_file_name = dirName;
  printf("open ManualRecord ok01112222 \n");

  Read_ManualRecord_to_file(data_file_name);
  fputs("[End]\n", fp2);
  fflush(fp2);
  fsync(fd);
  sleep(1);
  fclose(fp2);
  sleep(1);
  fp2 = fopen(file_name, "r");
  if(NULL == fp2)
    {
      perror("open ManualRecord error \n");
      fclose(fp2);
    }
  else
    {
      printf("open ManualRecord----------- ok \n");
      printf("open ManualRecord afert1----------- ok \n");//add by 625

      int ret = fseek(fp2,-6,SEEK_END);
      printf("open ManualRecord afert2----------- ok \n");//add by 625

      fgets(buff, 250, fp2);
      printf("open ManualRecord afert3----------- ok \n");//add by 625

      printf("----------------------==%d==%s\n",ret,buff);
      printf("open ManualRecord afert4----------- ok \n");//add by 625

      if( strcmp(buff, "[End]\n") == 0 )
        {
          printf("ManualRecord----文件完整--- ok \n");
          fclose(fp2);
          return true;
        }
      printf("open ManualRecord afert6----------- ok \n");//add by 625

    }
  printf("open ManualRecord afert7----------- ok \n");//add by 625

  return false;
}

//utf8转成Gb2312
int Import_Export_Data::Utf8ToGb2312(char *sOut, int iMaxOutLen, const char *sIn, int iInLen)
{

  printf("utf gb2312\n");//add by 625
  char *pIn = (char *)sIn;
  char *pOut = sOut;
  size_t ret;
  size_t iLeftLen=iMaxOutLen;
  iconv_t cd;

#ifdef _ARM_
  cd = iconv_open("gb2312", "utf-8");
  if (cd == (iconv_t) - 1)
    {
      return -1;
    }
  size_t iSrcLen=iInLen;
  ret = iconv(cd, &pIn,&iSrcLen, &pOut,&iLeftLen);
  if(ret == (size_t) - 1)
    {
      iconv_close(cd);
      return -1;
    }

  iconv_close(cd);
#endif
  return (iMaxOutLen - iLeftLen);
}

//Gb2312转成utf8
int Import_Export_Data::Gb2312ToUtf8(char *sOut, int iMaxOutLen, const char *sIn, int iInLen)
{
  char *pIn = (char *)sIn;
  char *pOut = sOut;
  size_t ret;
  size_t iLeftLen=iMaxOutLen;
  iconv_t cd;
#ifdef _ARM_
  cd = iconv_open("utf-8", "gb2312");
  if (cd == (iconv_t) - 1)
    {
      return -1;
    }
  size_t iSrcLen=iInLen;
  ret = iconv(cd, &pIn,&iSrcLen, &pOut,&iLeftLen);
  if (ret == (size_t) - 1)
    {
      iconv_close(cd);
      return -1;
    }

  iconv_close(cd);
#endif
  return (iMaxOutLen - iLeftLen);
}
//****************************拷贝文件函数夹*******************************
//拷贝文件夹：

bool Import_Export_Data::Copy_File(char *oldfilePath, char *newfilePath)
{
  INT32 ret = 0,ret1 = 0;
  char read_buf[1024] = "";
  FILE *fp_old;
  FILE *fp_new;
  INT32 fd_new;
  INT32 size1;
  INT32 size2;
  fp_old = fopen(oldfilePath, "rb");
  if(fp_old == NULL)
    {
      return 0;
    }

  fp_new = fopen(newfilePath, "wb+");
  if(fp_new == NULL)
    {
      fclose(fp_old);  //关闭文件指针，释放内存
      return 0;
    }
  else
    {
      fd_new = fileno(fp_new);
      ret = fread(&read_buf, sizeof(UINT8), 1024, fp_old);
      if(ret > 0)
        {
          ret1 = fwrite(&read_buf, sizeof(UINT8), ret, fp_new);
          if(ret1 != ret)
            {
              fclose(fp_old);  //关闭文件指针，释放内存
              fclose(fp_new);
              return 0;
            }
        }
      while(ret == 1024)
        {
          ret = fread(&read_buf, sizeof(UINT8), 1024, fp_old);
          if(ret > 0)
            {
              ret1 = fwrite(&read_buf, sizeof(UINT8), ret, fp_new);
              if(ret1 != ret)
                {
                  fclose(fp_old);
                  fclose(fp_new);
                  return 0;
                }
            }
        }
    }


  //fclose(fp_old);  //关闭文件指针，释放内存
  fflush(fp_new); 
  usleep(10*1000);
  fsync(fd_new);//new add
  fseek(fp_old, 0, SEEK_END);   ///将文件指针移动文件结尾
  size1=ftell(fp_old); ///求出当前文件指针距离文件开始的字节数
  printf("the source file size is %d\n",size1);
  fseek(fp_new, 0, SEEK_END);   ///将文件指针移动文件结尾
  size2=ftell(fp_new); ///求出当前文件指针距离文件开始的字节数
  printf("the new file size is %d\n",size2);

  fclose(fp_old);
  fclose(fp_new);
  if(size1==size2){
      return 1;
    }else{
      QFile::remove(newfilePath);
      printf("Copy File failed\n");
      return 0;
    }
}

bool Import_Export_Data::Copy_Directory_Files(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
  QDir sourceDir(fromDir);
  QDir targetDir(toDir);
  char file_old_name[250];
  char file_new_name[250];

  if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
      if(!targetDir.mkdir(targetDir.absolutePath()))
        return true;
    }
  if(!sourceDir.exists()){    /**< 如果源目录标目录不存在则返回 */
      return false;
    }
  QFileInfoList fileInfoList = sourceDir.entryInfoList();
  foreach(QFileInfo fileInfo, fileInfoList){
      if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        continue;

      if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
          if(!Copy_Directory_Files(fileInfo.filePath(),targetDir.filePath(fileInfo.fileName()),coverFileIfExist))
            return true;
        }else{     /**< 当允许覆盖操作时，将旧文件进行删除操作 */
          if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
              targetDir.remove(fileInfo.fileName());
            }
          /// 进行文件copy
          QByteArray old_name = fileInfo.filePath().toLatin1();
          QByteArray new_name = targetDir.filePath(fileInfo.fileName()).toLatin1();

          ::sprintf(file_old_name, "%s", old_name.data());
          ::sprintf(file_new_name, "%s", new_name.data());
          // if(!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()))){
          if(!Copy_File(file_old_name, file_new_name)){
              return true;
            }
        }
    }
  return true;
}




//******************************升级程序*******************************
void Import_Export_Data::on_btn_updatasoft_clicked()
{ bool cofile = false;
  ui->progressBar->setValue(0);
  if(USB_selection==0)
    {
      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("提示"));
      msgBox.setText(tr("升级失败，请检查USB!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
  else
    {
      ui->progressBar->setValue(10);
      sprintf(file_name,"%s/%s",devPath,APPLIACTION_USB_UP_PATH);//APPLIACTION_USB_UP_PATH = "DcScreen_Power.tar.gz"
      if(access(file_name,F_OK) != -1){
        if(access(APPLIACTION_UP_PATH,F_OK) != -1){//APPLIACTION_UP_PATH  =="/mnt/Nand1/update/DcScreen_Power.tar.gz.tmp"
          QFile::remove(APPLIACTION_UP_PATH);
          ui->progressBar->setValue(30);
        }
        ui->progressBar->setValue(40);

        cofile = Copy_File(file_name, (char *)APPLIACTION_UP_PATH);
        ui->progressBar->setValue(70);
         sleep(2);
         ui->progressBar->setValue(90);
        }


      if(cofile)
        {
          ui->progressBar->setValue(99);
          if(Super_Egir_Flag==1){
              system("rm -r /mnt/Nand1/ea_init/*");
              system("rm -r /mnt/Nand1/ea_app/*");
            }

        /*  if(access(F_Battery_Param_Set,F_OK) != -1)//add by zzc 721 delete old battery_set
          {
            QFile::remove(F_Battery_Param_Set);
          }
          if(access(F_BAK_Battery_Param_Set,F_OK) != -1)//add by zzc 721 delete old battery_set
         {
            QFile::remove(F_BAK_Battery_Param_Set);
          }*/
          /*QFile::remove(F_Battery_Param_Set);
          QFile::remove(F_BAK_Battery_Param_Set);
          QFile::remove(F_Battery_GroupParam_Set);
          QFile::remove(F_BAK_Battery_GroupParam_Set);
          QFile::remove(F_SET_DSP_PARAMETE);
          QFile::remove(F_BAK_SET_DSP_PARAMETE);//加上DSP参数*/
          Insert_Operation_Work_Record(1, "升级程序");
          LOG("Copy upgrade program successfully");
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("提示"));
          msgBox.setText(tr("升级成功，是否立即重启？"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
          if(msgBox.exec() == QMessageBox::Ok)
            { LOG("Copy upgrade program successfully,reboot system");
              Reboot Reboot_View;
              Reboot_View.exec();
             // system("reboot");
            }
          Restar_SystemFlage = 1;
        }
      else
        {
          ui->progressBar->setValue(0);
          LOG("Copy upgrade program failed");
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("提示"));
          msgBox.setText(tr("升级失败，文件传输故障"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
    }
}

//*****************************导出数据********************************
void Import_Export_Data::on_btn_dataexport_clicked()
{
  bool ret1=false,ret=false,ret2=false;
  ui->progressBar->setValue(0);
  if(USB_selection == 0)
    {
      QMessageBox::information(this,tr("提示"),tr("导出历史数据失败,请确认是否插上USB!"),QMessageBox::Ok);
    }
  else
    {
      sprintf(file_name,"%s/%s",devPath,"Export_Data");
      ::mkdir(file_name ,0777);
      ui->progressBar->setValue(5);
      sprintf(file_name,"%s/%s/%s",devPath,"Export_Data","history_alarm_file.csv");
      ret = Export_HistoryAlarm_data(HISTORY_ALARM_DB);
      ui->progressBar->setValue(20);
      sleep(1);


      sprintf(file_name,"%s/%s/%s",devPath,"Export_Data","history_fault_file.csv");
      ret = Export_HistoryFault_data(HISTORY_FAULT_DB);
      ui->progressBar->setValue(25);
      sleep(1);

      ui->progressBar->setValue(35);
      sprintf(file_name,"%s/%s/%s",devPath,"Export_Data","eps_work_state_file.csv");
      ret1 = Export_WorkStation_data(HISTORY_EPSWORK_RECORD_DB);
      ui->progressBar->setValue(60);
      sleep(1);

      ui->progressBar->setValue(65);
      sprintf(file_name,"%s/%s/%s",devPath,"Export_Data","manual_record_file.csv");
      ret2 = Export_ManualRecord_data(HISTORY_OPERATION_RECORD_DB);
      ui->progressBar->setValue(90);
      sleep(1);

      if(ret&&ret1&&ret2){
          sleep(1);
          ui->progressBar->setValue(99);
          QMessageBox::information(this,tr("提示"),tr("请拔出U盘!"),QMessageBox::Ok);
        }else{
          QMessageBox::information(this,tr("提示"),tr("导出历史数据库失败!"),QMessageBox::Ok);
        }
    }
}


//***************************系统配置文件导入*****************************
void Import_Export_Data::on_btn_sysfileinput_clicked()
{
  bool cofile1=false;
  ui->progressBar->setValue(0);
  if(USB_selection == 0){
      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("提示"));
      msgBox.setText(tr("导入失败，请确认是否插上USB!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }else{
      sprintf(file_name,"%s/%s",devPath,SYS_CONFIGURATIONFILE_USB_UP_PATH);
      ui->progressBar->setValue(20);
      if(access(file_name,F_OK) != -1){
          cofile1 = Copy_Directory_Files(file_name, SYS_CONFIGURATIONFILE_UP_PATH,true); //可以覆盖
          ui->progressBar->setValue(70);
          sleep(3);
          ui->progressBar->setValue(80);
          sleep(2);
          ui->progressBar->setValue(90);
        }
      if(cofile1){
          ui->progressBar->setValue(99);
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("提示"));
          msgBox.setText(tr("配置导入成功，请重新启动系统!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
          if(msgBox.exec() == QMessageBox::Ok){
              Reboot Reboot_View;
              Reboot_View.exec();
              //system("reboot");
            }
          Restar_SystemFlage = 1;
        }else{
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("提示"));
          msgBox.setText(tr("配置导入失败，请重新导入!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
    }
}

//***************************系统配置文件导出*****************************
void Import_Export_Data::on_btn_sysfileoutput_clicked()
{
  bool cofile1=false;
  ui->progressBar->setValue(0);
  if(USB_selection == 0)
    {
      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("提示"));
      msgBox.setText(tr("导出失败，请确认是否插上USB！"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
  else
    {
      sprintf(file_name,"%s/%s",devPath,SYS_CONFIGURATIONFILE_USB_UP_PATH);
      ui->progressBar->setValue(30);
      cofile1 = Copy_Directory_Files(SYS_CONFIGURATIONFILE_UP_PATH, file_name,true); //可以覆盖
      ui->progressBar->setValue(60);
      sleep(2);
      ui->progressBar->setValue(70);
      sleep(1);
      ui->progressBar->setValue(85);
      sleep(2);
      if(cofile1){
          ui->progressBar->setValue(99);
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("提示"));
          msgBox.setText(tr("导出成功!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }else{
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("提示"));
          msgBox.setText(tr("导出失败请重新导出!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
    }
}



void Import_Export_Data::IEdataloadUI()
{
  if(Super_Egir_Flag==1)
    {
      ui->label->setVisible(true);
    }else{
      ui->label->setVisible(false);
    }

  if(Usr_Engir_Flag == 1){
    ui->btn_updatasoft->setVisible(false);
    ui->btn_sysfileinput->setVisible(false);
    ui->btn_sysfileoutput->setVisible(false);
    ui->btn_dataexport->setGeometry(310,150,200,70);
  }else if(Usr_Engir_Flag == 0){
    ui->btn_updatasoft->setVisible(true);
    ui->btn_sysfileinput->setVisible(true);
    ui->btn_sysfileoutput->setVisible(true);
    ui->btn_dataexport->setGeometry(150,110,200,70);
  }
}

