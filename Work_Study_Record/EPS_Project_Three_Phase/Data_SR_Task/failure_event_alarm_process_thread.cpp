#include "failure_event_alarm_process_thread.h"
/***********************************************
 *history_Alarm_Eventlog-----历史数据库中数据表的名字
 **********************************************/
#include"Share/param.h"


Failure_Event_Alarm_Process_Thread::Failure_Event_Alarm_Process_Thread(QWidget *parent) :
  QThread(parent)
{
  alarm_failure = 0;
  EPS_Run_State = 0;
  ls_EPS_Run_State = 255;
  Last_DSP_Reset=0;
  check_48h_flag=0;
  memset(Dsp_Failure_happenflag, 0, sizeof(Dsp_Failure_happenflag));
  memset(Battery_Failure_happenflag, 0, sizeof(Battery_Failure_happenflag));
  memset(Swith_Off_happenflag,0,sizeof(Swith_Off_happenflag));
  IO_Failure_happenflag[0]=0;
  IO_Failure_happenflag[1]=0;
  ATS_Failure_happenflag=0;
   System_Total_fault = 0;
   System_Total_Alarm = 0;
  Sys_Self_Check_Parame.Month_Self_Check_Start_Time = Read_systime();  //月检起始时间 (获取当前系统运行时间)
  Sys_Self_Check_Parame.Month_Self_Check_Time = Read_systime();        //月检当前运行时间 (获取当前系统运行时间)
  Sys_Self_Check_Parame.Month_Self_Check_Falg = 0;                     //月检标志
  Sys_Self_Check_Parame.Month_Self_first_Check_Falg = 0;

  Sys_Self_Check_Parame.Year_Self_Check_Start_Time= Read_systime();    //年检起始时间  (获取当前系统运行时间)
  Sys_Self_Check_Parame.Year_Self_Check_Time= Read_systime();          //年检当前运行时间 (获取当前系统运行时间)
  Sys_Self_Check_Parame.Year_Self_Check_Falg = 0;                      //年检标志
  Sys_Self_Check_Parame.Year_Self_first_Check_Falg = 0;
 // check_48h_timer = new QTimer(this);
 // connect(check_48h_timer,SIGNAL(timeout()),this,SLOT(Check_48h_Function()));
 // check_48h_timer->start(checktime);
}
void Failure_Event_Alarm_Process_Thread::Check_48h_Function()
{

 /* check_48h_timer->stop();
  check_48h_flag=1;
  Sys_Self_Check_Parame.Month_Self_Check_Start_Time=Read_systime();
  Sys_Self_Check_Parame.Year_Self_Check_Start_Time= Read_systime();*/
}
void Failure_Event_Alarm_Process_Thread::run()
{
  Failure_Event_Alarm_Process();
  exec();
}

void Failure_Event_Alarm_Process_Thread::Failure_Event_Alarm_Process()
{ //UINT32 Battery_Under_Voltage_Flag;
  UINT32 i;
  thread_failure=1;
  sleep(4);
  thread_failure=1;
  sleep(4);
  thread_failure=1;
  sleep(2);
  while(1){


    thread_failure=1;
    if(GetFileSize(F_SYSTEMLOGPATH)>1024000)//1000KB
      QFile::remove(F_SYSTEMLOGPATH);
    Battery_Under_Voltage_Flag=Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE];


    Event_Alarm_Process_function();

    if((Battery_Under_Voltage_Flag==1)&&(System_Total_fault==0)&&(Globa.User_Monitor_Screen_Set.AlarmSound == 1)&&(Silent_Mode_flag == 0)&&(Reset_flag == 0)){
            for(i=0;i<8;i++){
                if(i%2==0){
                    GPIO_SYS_BUZZ_ON;

                  }else{
                    GPIO_SYS_BUZZ_OFF;

                  }
                usleep(125000);
              }
          }
        else{
            usleep(1000000);
          }


  }
}

//**************把当前告警事件插入到当前告警事件链表,历史告警事件链表和历史告警事件数据库中****************
void Failure_Event_Alarm_Process_Thread::Save_Current_alarm_event(QString failure_event_name)
{
  CURRENT_ALARMFAULT_EVENT ls_alarm_eventlog;
  HISTORY_ALARMFAULT_EVENT ls_history_alarm;

  QDateTime time = QDateTime::currentDateTime();
  QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
  QString time_end;
  ls_alarm_eventlog.name =  failure_event_name;
  ls_alarm_eventlog.time = time_str;

  //把当前事件插入到当前告警事件链表中
  current_alarm_tablelocks_mutex.lockForWrite();
  current_alarm_eventlog.insert(0,ls_alarm_eventlog);
  //printf("save to current\n");
  current_alarm_tablelocks_mutex.unlock();

  Silent_Mode_flag = 0;//有新的告警信息插入则报警
  if(Reset_time==0){
      //Silent_Mode_flag = 0;//有新的告警信息插入则报警
      Reset_flag=0;

    }

  //把当前事件插入历史告警事件数据库中
  QFileInfo info(HISTORY_ALARM_DB);
  bool exist_flag = info.exists();
  { history_alarm_tablelocks_mutex.lockForWrite();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","insert_Currenthistory_Alarm_Eventlog");
    db.setDatabaseName(HISTORY_ALARM_DB);
    if(db.open()){
        QSqlQuery  query(db);
        if(!exist_flag)
          {
            query.exec("create table history_Alarm_Eventlog(name TEXT, start_time TEXT, end_time TEXT)");
          }

        if(MAX_LIST_COUNT<=history_alarm_eventlog.count()){
          query.exec(tr("delete from history_Alarm_Eventlog where rowid in (select rowid from history_Alarm_Eventlog limit %1)").arg(history_alarm_eventlog.count()- MAX_LIST_COUNT + 1));
        }

        query.exec(QString("insert into history_Alarm_Eventlog values('%1','%2','%3')").arg(failure_event_name).arg(ls_alarm_eventlog.time).arg(time_end));
        query.clear();
        db.close();
      }
      history_alarm_tablelocks_mutex.unlock();
  }
  QSqlDatabase::removeDatabase("insert_Currenthistory_Alarm_Eventlog");

  ls_history_alarm.name = failure_event_name;
  ls_history_alarm.start_time = time_str;
  ls_history_alarm.end_time =  time_end ;

  //把当前事件插入到历史告警事件链表中

  history_alarm_tablelocks_mutex.lockForWrite();
  while(MAX_LIST_COUNT<=history_alarm_eventlog.count()){
    history_alarm_eventlog.removeLast();
  }
  history_alarm_eventlog.insert(0,ls_history_alarm);
  //printf("save to history\n");
  history_alarm_tablelocks_mutex.unlock();
}

//***************删除当前告警链表中的选定事件************************
void Failure_Event_Alarm_Process_Thread::Delete_Current_alarm_event(QString failure_event_name)
{
  INT32 i=0;

  current_alarm_tablelocks_mutex.lockForWrite();

  for(i=0;i<current_alarm_eventlog.count();i++){
    if(current_alarm_eventlog.at(i).name == failure_event_name){
      current_alarm_eventlog.removeAt(i);
      break;
    }
  }
  current_alarm_tablelocks_mutex.unlock();

}

//**************插入告警事件到历史数据库以及历史链表中,并删除该项在当前链表中的记录****************
void Failure_Event_Alarm_Process_Thread::Insert_History_alarm_eventlog(QString failure_event_name)
{
  INT32 i=0;
  CURRENT_ALARMFAULT_EVENT ls_current_alarm;
  HISTORY_ALARMFAULT_EVENT ls_history_alarm;
  bool Alarm_insert_falg =false;

  current_alarm_tablelocks_mutex.lockForWrite();

  for(i=0;i<current_alarm_eventlog.count();i++)
    {
      if(current_alarm_eventlog.at(i).name == failure_event_name)
        {
          ls_current_alarm.name = failure_event_name;
          ls_current_alarm.time = current_alarm_eventlog.at(i).time;
          current_alarm_eventlog.removeAt(i);
          Alarm_insert_falg = true;
          break;
        }
    }
  current_alarm_tablelocks_mutex.unlock();


  if(Alarm_insert_falg){
      QDateTime time = QDateTime::currentDateTime();
      QString time_str =time.toString("yyyy-MM-dd hh:mm:ss");//设置系统

      QFileInfo info(HISTORY_ALARM_DB);
      bool exist_flag = info.exists();
      {   history_alarm_tablelocks_mutex.lockForWrite();

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Insert_History_alarm_eventlog");
        db.setDatabaseName(HISTORY_ALARM_DB);
        if(db.open()){
            QSqlQuery  query(db);
            if(!exist_flag){
                query.exec("create table history_Alarm_Eventlog(name TEXT, start_time TEXT, end_time TEXT)");
              }


            if(MAX_LIST_COUNT<history_alarm_eventlog.count()){
              query.exec(tr("delete from history_Alarm_Eventlog where rowid in (select rowid from history_Alarm_Eventlog limit %1)").arg(history_alarm_eventlog.count()- MAX_LIST_COUNT + 1));
            }

            if(query.exec("select * from history_Alarm_Eventlog")){
              //sprintf(temp,"update history_Alarm_Eventlog set end_time = '%1' WHERE ID = '%1' AND start_time = '%1'",time_str,failure_event_name,start_time);
              query.exec(QString("update history_Alarm_Eventlog set end_time = '%1' WHERE name = '%2' and start_time = '%3'").arg(time_str).arg(failure_event_name).arg(ls_current_alarm.time));

            }
            query.clear();
            db.close();
          }

        history_alarm_tablelocks_mutex.unlock();
      }
      QSqlDatabase::removeDatabase("Insert_History_alarm_eventlog");


      history_alarm_tablelocks_mutex.lockForWrite();
      while(MAX_LIST_COUNT<history_alarm_eventlog.count()){
        history_alarm_eventlog.removeLast();
      }

      for(i = 0;i<history_alarm_eventlog.count();i++){
        if((history_alarm_eventlog.at(i).name == ls_current_alarm.name)&&(history_alarm_eventlog.at(i).start_time == ls_current_alarm.time)){
           ls_history_alarm.name = ls_current_alarm.name;
           ls_history_alarm.start_time = ls_current_alarm.time;
           ls_history_alarm.end_time =  time_str ;
           history_alarm_eventlog.replace(i ,ls_history_alarm);
         }
      }
      history_alarm_tablelocks_mutex.unlock();
    }
}


//历史故障处理

//**************把当前故障事件插入到当前告警事件链表,历史故障事件链表和历史告警事件数据库中****************
void Failure_Event_Alarm_Process_Thread::Save_Current_fault_event(QString failure_event_name)
{
  CURRENT_ALARMFAULT_EVENT ls_fault_eventlog;
  HISTORY_ALARMFAULT_EVENT ls_history_fault;

  QDateTime time = QDateTime::currentDateTime();
  QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
  QString time_end;
  ls_fault_eventlog.name =  failure_event_name;
  ls_fault_eventlog.time = time_str;

  //把当前事件插入到当前告警事件链表中
  current_fault_tablelocks_mutex.lockForWrite();
  current_fault_eventlog.insert(0,ls_fault_eventlog);
  current_fault_tablelocks_mutex.unlock();
  Silent_Mode_flag = 0;//有新的告警信息插入则报警
  if(Reset_time==0){
      //Silent_Mode_flag = 0;//有新的告警信息插入则报警
      Reset_flag=0;
    }

//  qDebug()<<"Save_Current_fault_event--nextpage01 "<<current_fault_eventlog.count();
  //把当前事件插入历史告警事件数据库中
  QFileInfo info(HISTORY_FAULT_DB);
  bool exist_flag = info.exists();
  {history_fault_tablelocks_mutex.lockForWrite();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","insert_Currenthistory_Fault_Eventlog");
    db.setDatabaseName(HISTORY_FAULT_DB);
    if(db.open()){
      QSqlQuery  query(db);
      if(!exist_flag)
        {
          query.exec("create table history_Fault_Eventlog(name TEXT, start_time TEXT, end_time TEXT)");
        }

        if(MAX_LIST_COUNT<=history_fault_eventlog.count()){
          query.exec(tr("delete from history_Fault_Eventlog where rowid in (select rowid from history_Fault_Eventlog limit %1)").arg(history_fault_eventlog.count()- MAX_LIST_COUNT + 1));
        }

        query.exec(QString("insert into history_Fault_Eventlog values('%1','%2','%3')").arg(failure_event_name).arg(ls_fault_eventlog.time).arg(time_end));
        query.clear();
        db.close();
      }
    history_fault_tablelocks_mutex.unlock();
  }
  QSqlDatabase::removeDatabase("insert_Currenthistory_Fault_Eventlog");

  ls_history_fault.name = failure_event_name;
  ls_history_fault.start_time = time_str;
  ls_history_fault.end_time =  time_end ;

  //把当前事件插入到历史告警事件链表中
  history_fault_tablelocks_mutex.lockForWrite();

  while(MAX_LIST_COUNT<=history_fault_eventlog.count()){
    history_fault_eventlog.removeLast();
  }
  history_fault_eventlog.insert(0,ls_history_fault);
  history_fault_tablelocks_mutex.unlock();
}

//***************删除当前故障链表中的选定事件************************
void Failure_Event_Alarm_Process_Thread::Delete_Current_fault_event(QString failure_event_name)
{
  INT32 i=0;
  current_fault_tablelocks_mutex.lockForWrite();
  for(i=0;i<current_fault_eventlog.count();i++)
    {
      if(current_fault_eventlog.at(i).name == failure_event_name)
        {
          current_fault_eventlog.removeAt(i);
          break;
        }
    }
  current_fault_tablelocks_mutex.unlock();
}

//**************插入故障事件到历史数据库以及历史链表中,并删除该项在当前链表中的记录****************
void Failure_Event_Alarm_Process_Thread::Insert_History_fault_eventlog(QString failure_event_name)
{
  INT32 i=0;
  CURRENT_ALARMFAULT_EVENT ls_current_fault;
  HISTORY_ALARMFAULT_EVENT ls_history_fault;
  bool Fault_insert_falg =false;

  current_fault_tablelocks_mutex.lockForWrite();

  for(i=0;i<current_fault_eventlog.count();i++)
    {
      if(current_fault_eventlog.at(i).name == failure_event_name)
        {
          ls_current_fault.name = failure_event_name;
          ls_current_fault.time = current_fault_eventlog.at(i).time;
          current_fault_eventlog.removeAt(i);
          Fault_insert_falg = true;
          break;
        }
    }
  current_fault_tablelocks_mutex.unlock();

  if(Fault_insert_falg){
      QDateTime time = QDateTime::currentDateTime();
      QString time_str =time.toString("yyyy-MM-dd hh:mm:ss");//设置系统

      QFileInfo info(HISTORY_FAULT_DB);
      bool exist_flag = info.exists();
      { history_fault_tablelocks_mutex.lockForWrite();
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Insert_History_fault_eventlog");
        db.setDatabaseName(HISTORY_FAULT_DB);
        if(db.open()){
            QSqlQuery  query(db);
            if(!exist_flag){
                query.exec("create table history_Fault_Eventlog(name TEXT, start_time TEXT, end_time TEXT)");
              }

            if(MAX_LIST_COUNT<history_fault_eventlog.count()){
              query.exec(tr("delete from history_Fault_Eventlog where rowid in (select rowid from history_Fault_Eventlog limit %1)").arg(history_fault_eventlog.count()- MAX_LIST_COUNT + 1));
            }

            if(query.exec("select * from history_Fault_Eventlog")){
                // sprintf(temp,"update history_Fault_Eventlog set end_time = '%1' WHERE ID = '%1' AND start_time = '%1'",time_str,failure_event_name,start_time);
                query.exec(QString("update history_Fault_Eventlog set end_time = '%1' WHERE name = '%2' and start_time = '%3'").arg(time_str).arg(failure_event_name).arg(ls_current_fault.time));
              }
            query.clear();
            db.close();
          }
      history_fault_tablelocks_mutex.unlock();
      }
      QSqlDatabase::removeDatabase("Insert_History_fault_eventlog");


      history_fault_tablelocks_mutex.lockForWrite();

      while(MAX_LIST_COUNT<history_fault_eventlog.count()){
        history_fault_eventlog.removeLast();
      }

      for(i = 0;i<history_fault_eventlog.count();i++){
        if((history_fault_eventlog.at(i).name == ls_current_fault.name)&&(history_fault_eventlog.at(i).start_time == ls_current_fault.time)){
          ls_history_fault.name = ls_current_fault.name;
          ls_history_fault.start_time = ls_current_fault.time;
          ls_history_fault.end_time =  time_str ;
          history_fault_eventlog.replace(i ,ls_history_fault);
        }
      }
      history_fault_tablelocks_mutex.unlock();
    }
}


void Failure_Event_Alarm_Process_Thread::Battery_Failure_Event_Alarm_Function(UINT32 number)
{
  UINT32   Battery_Data[24*BATTERYMAXNUMEBR] = {0};
  UINT32 i = 0,j=0,z=0, Battery_Data_Refresh_Falg[BATTERYMAXNUMEBR] = {0}, Battery_Communication_Falg[BATTERYMAXNUMEBR] = {0};
  UINT32  Battery_No = 0, batteryNumber_no = 0;
  Battery_mutex.lockForRead();



  for(i=0;i<Globa.Battery_Param_Set[number].Battery_Number;i++){
    Battery_Communication_Falg[i] = Globa.Eps_Param_Data.Battery_Communication_Fault[number][i];
    Battery_Data_Refresh_Falg[i] = Globa.Eps_Param_Data.Battery_Data_Refresh[number][i];
    Globa.Eps_Param_Data.Battery_Data_Refresh[number][i] = 0;
  }



  for(z=0,i=0;i<Globa.Battery_Param_Set[number].Battery_Number;i++){

    for(j= 0;j<Globa.Battery_Param_Set[number].Singlenumber[i];j++){

      Battery_Data[z] =  Globa.Eps_Param_Data.Battery_Data_Param[number].Battery_Monomer_V[i][j];
      z++;
    }
  }
  Battery_mutex.unlock();


  for(Battery_No =0;Battery_No<Globa.Battery_Param_Set[number].Battery_Number;Battery_No++){
      if(Battery_No != 0){
              batteryNumber_no = batteryNumber_no + Globa.Battery_Param_Set[number].Singlenumber[Battery_No - 1];
            }else{
              batteryNumber_no = 0;
            }
      if(!Battery_Communication_Falg[Battery_No]){
      if(Battery_Failure_happenflag[number][Battery_No][0] == 1){
        failure_name = tr("%1#电池组%2#电池仪通讯中断").arg(number + 1).arg(Battery_No + 1);
        Insert_History_alarm_eventlog(failure_name);

        Battery_Failure_happenflag[number][Battery_No][0] = 0;
      }

      if(Battery_Data_Refresh_Falg[Battery_No] == 1){
        Battery_Data_Refresh_Falg[Battery_No] = 0;
        for(i = 0; i< Globa.Battery_Param_Set[number].Singlenumber[Battery_No]; i++){
          if((Battery_Data[i + batteryNumber_no] >= G_Sigle_Baterry_Voltage_Alarm.OverVolAlarm*100)||(Battery_Data[i + batteryNumber_no] <= G_Sigle_Baterry_Voltage_Alarm.UnderVolAlarm*100)){//单体电压异常 过欠压

              if(Battery_Failure_happenflag[number][Battery_No][i + 1] == 0){
              failure_name = tr("%1#蓄电池组第%2节电池电压异常").arg(number + 1).arg(batteryNumber_no + i + 1);
              Save_Current_alarm_event(failure_name);

              Battery_Failure_happenflag[number][Battery_No][i + 1] = 1;

            }
          }else if((Battery_Data[i + batteryNumber_no] < G_Sigle_Baterry_Voltage_Alarm.OverVolAlarm*100)&&(Battery_Data[i + batteryNumber_no] > G_Sigle_Baterry_Voltage_Alarm.UnderVolAlarm*100)){

              if(Battery_Failure_happenflag[number][Battery_No][i + 1] == 1){
              failure_name = tr("%1#蓄电池组第%2节电池电压异常").arg(number + 1).arg(batteryNumber_no + i + 1);
              Insert_History_alarm_eventlog(failure_name);

              Battery_Failure_happenflag[number][Battery_No][i + 1] = 0;
            }
          }

        }


      }
    }else{
      if(Battery_Failure_happenflag[number][Battery_No][0] == 0){
        failure_name = tr("%1#电池组%2#电池仪通讯中断").arg(number + 1).arg(Battery_No + 1);
        Save_Current_alarm_event(failure_name);
        Battery_Failure_happenflag[number][Battery_No][0] = 1;
      }

     for(i = 0; i< Globa.Battery_Param_Set[number].Singlenumber[Battery_No]; i++){
       if(Battery_Failure_happenflag[number][Battery_No][i + 1] == 1){
          failure_name = tr("%1#蓄电池组第%2节电池电压异常").arg(number + 1).arg(batteryNumber_no + i+1);
          Delete_Current_alarm_event(failure_name);
          Battery_Failure_happenflag[number][Battery_No][i + 1] = 0;
        }

      }
    }

  }



}

void Failure_Event_Alarm_Process_Thread:: Switch_Failure_Event_Alarm_Function()
{ INT32 t, i;
  UINT32 IO_Fault_Data[2]={0},ATS_Fault_Data=0;
  UINT32 io_num=0;
  UINT32 IO_Configure_Falg,ATS_Configure_Falg;




  IO_Configure_Falg=Globa.Curstomer_Param_Set.Distribution_Configure_Flage;
  ATS_Configure_Falg=Globa.Curstomer_Param_Set.ATS_Configure_Flage;

  if(IO_Configure_Falg==1)
  {
      if(Globa.Switch_Set_Flag==1)
      {
          current_alarm_tablelocks_mutex.lockForWrite();

          for( t=0;t<current_alarm_eventlog.count();t++)
          {
              if(current_alarm_eventlog.at(t).name.indexOf(QString("支路")) !=-1)
              {
                  current_alarm_eventlog.removeAt(t);
                  t--;
              }
          }
          Globa.Switch_Set_Flag=0;
          memset(Swith_Off_happenflag,0,sizeof(Swith_Off_happenflag));

          current_alarm_tablelocks_mutex.unlock();
      }


  }

  IO_Fault_Data[0]=Globa.Switch_Module_Fault[0];
  IO_Fault_Data[1]=Globa.Switch_Module_Fault[1];

  ATS_Fault_Data=Globa.ATS_Module_Fault;
  UINT16 Switch_Status[64];

  for(int j=0;j<Globa.Curstomer_Param_Set.Distribution_Branches_Number;j++)
  {
      Switch_Status[j]=(Globa.Switch_Value[j/8]>>(j%8))&0x01;
  }
  if(IO_Configure_Falg==1)
  {
    IsSwitch_Branchnumber_Change_funtion();
    for(io_num=0;io_num<Globa.Curstomer_Param_Set.Distribution_Branches_modelnum;io_num++)
    {


        if(!IO_Fault_Data[io_num])
        {//IO communication normal
          if(IO_Failure_happenflag[io_num] == 1)
          {
                failure_name = tr("%1#开关量模块通信异常").arg(io_num+1);
                Insert_History_alarm_eventlog(failure_name);
                IO_Failure_happenflag[io_num]= 0;
          }

          int startloop=io_num>0?32:0;
          int endloop=io_num>0?Globa.Curstomer_Param_Set.Distribution_Branches_Number:MIN(Globa.Curstomer_Param_Set.Distribution_Branches_Number,32);



          for(int j = startloop; j< endloop; j++)
          {
            if(Switch_Status[j]==0)
            {//断开

                if(Globa.Sysappendparam.Switch_Alarm_Set[j]==0&&Swith_Off_happenflag[j] == 0)
                {


                        failure_name = tr("支路%1断开").arg(j+1);
                        Save_Current_alarm_event(failure_name);
                        Swith_Off_happenflag[j] = 1;
                }
            }
            else if(Switch_Status[j]==1)
            {

                if(Swith_Off_happenflag[j] == 1)
                {
                    failure_name = tr("支路%1断开").arg(j+1);

                    Insert_History_alarm_eventlog(failure_name);
                    Swith_Off_happenflag[j] = 0;
                }
            }

          }

       }
      else if(IO_Fault_Data[io_num])
       {
           if(IO_Failure_happenflag[io_num]== 0)
           {
                 failure_name = tr("%1#开关量模块通信异常").arg(io_num+1);

                 Save_Current_alarm_event(failure_name);
                 IO_Failure_happenflag[io_num]= 1;
           }
           int startloop=io_num>0?32:0;
           int endloop=io_num>0?Globa.Curstomer_Param_Set.Distribution_Branches_Number:MIN(Globa.Curstomer_Param_Set.Distribution_Branches_Number,32);


           for(int j=startloop;j< endloop; j++)
           {
              if(Swith_Off_happenflag[j] == 1)
              {
                  failure_name = tr("支路%1断开").arg(j+1);

                  Delete_Current_alarm_event(failure_name);
                  Swith_Off_happenflag[j] = 0;
              }
           }


        }
    }

  }
  else if(IO_Configure_Falg==0)
  {
          IO_Failure_happenflag[0]=0;IO_Failure_happenflag[1]=0;
          memset(Swith_Off_happenflag,0,sizeof(Swith_Off_happenflag));
          for(i = 0;i<8;i++)
          {
            Globa.Switch_Value[i] = 0;
          }
          current_alarm_tablelocks_mutex.lockForWrite();
          for(t=0;t<current_alarm_eventlog.count();t++)
          {
              if(current_alarm_eventlog.at(t).name.indexOf(QString("开关量模块通信异常")) !=-1)
              {
                  current_alarm_eventlog.removeAt(t);
                  break;
              }
           }
          for(t=0;t<current_alarm_eventlog.count();t++)
          {
              if(current_alarm_eventlog.at(t).name.indexOf(QString("支路")) !=-1)
              {
                  current_alarm_eventlog.removeAt(t);
                  t--;
              }
          }
          current_alarm_tablelocks_mutex.unlock();
 }

  if(ATS_Configure_Falg==1)
  {
    if(!ATS_Fault_Data)
    {//ATS通信正常
        if(ATS_Failure_happenflag == 1)
          {
            failure_name = tr("ATS模块通信异常");
            Insert_History_alarm_eventlog(failure_name);
            ATS_Failure_happenflag= 0;
          }
    }
    else if(ATS_Fault_Data)
    {
         if(ATS_Failure_happenflag== 0)
         {
             failure_name = tr("ATS模块通信异常");
             Save_Current_alarm_event(failure_name);
             ATS_Failure_happenflag= 1;
         }
    }
  }
  else if(ATS_Configure_Falg==0)
  {
      ATS_Failure_happenflag=0;
      current_alarm_tablelocks_mutex.lockForWrite();
      for(t=0;t<current_alarm_eventlog.count();t++)
      {
        if(current_alarm_eventlog.at(t).name.indexOf(QString("ATS模块通信异常")) !=-1)
        {
           current_alarm_eventlog.removeAt(t);
           break;
        }
      }
      current_alarm_tablelocks_mutex.unlock();
  }

}

void Failure_Event_Alarm_Process_Thread::Dry_Contact_Function()
{
  UINT32 i=0;
  UINT32 Dry_Contact_Falg[9] = {0};
  UINT32 Dry_Contact_Alarm[8]={0};
  Dry_Contact_Falg[0]=0;
  DSP_State_Alarm_mutex.lockForRead();
  Dry_Contact_Falg[1]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_Abnormal];
  Dry_Contact_Falg[2]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_A_Phase_Overload]\
                      |Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_B_Phase_Overload]\
                      |Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Mains_C_Phase_Overload];
  Dry_Contact_Falg[3]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Emergency_Fault];
  Dry_Contact_Falg[4]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Battery_Under_Voltage];
  Dry_Contact_Falg[5]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Fan_Fault];
  Dry_Contact_Falg[6]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Over_Temp_Fault];
  Dry_Contact_Falg[7]=System_Fault;
  Dry_Contact_Falg[8]=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Fire_Linkagel];
  DSP_State_Alarm_mutex.unlock();

  battery_groupset_mutex.lockForRead();
  for(i=0;i<6;i++)
  {
      Dry_Contact_Alarm[i]=Dry_Contact_Falg[Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[i]];
  }//change zzc 721 "i<8"-->"i<6"
  battery_groupset_mutex.unlock();

  Led_Relay_Control(0,Dry_Contact_Alarm[0]);
  Led_Relay_Control(1,Dry_Contact_Alarm[1]);
  Led_Relay_Control(2,Dry_Contact_Alarm[2]);
  Led_Relay_Control(6,Dry_Contact_Alarm[3]);
  Led_Relay_Control(5,Dry_Contact_Alarm[4]);
  Led_Relay_Control(4,Dry_Contact_Alarm[5]);

}

void Failure_Event_Alarm_Process_Thread::IsBattery_Group_Change_funtion()
{
  UINT32 Current_Alarm_List_Battery_Refresh_Falg=0;
  int t;
  Current_Alarm_List_Battery_Refresh_Falg=Globa.Eps_Param_Data.Current_Alarm_List_Battery_Refresh;
  Globa.Eps_Param_Data.Current_Alarm_List_Battery_Refresh=0;

  if(Current_Alarm_List_Battery_Refresh_Falg==1){
      memset(Battery_Failure_happenflag, 0, sizeof(Battery_Failure_happenflag));
      current_alarm_tablelocks_mutex.lockForWrite();
      for(t=0;t<current_alarm_eventlog.count();t++){
        if(current_alarm_eventlog.at(t).name.indexOf(QString("电池仪通讯中断")) !=-1){
           current_alarm_eventlog.removeAt(t);
           t--;
        }
      }

      for(t=0;t<current_alarm_eventlog.count();t++)
        {
          if(current_alarm_eventlog.at(t).name.indexOf(QString("电池电压异常")) !=-1)
            {
             current_alarm_eventlog.removeAt(t);
             t--;

            }
        }
  //   current_fault_tablelocks_mutex.unlock();
      current_alarm_tablelocks_mutex.unlock();
     Current_Alarm_List_Battery_Refresh_Falg=0;
     /*pc28modbus_pc02_mutex.lockForWrite();
     memset(&Globa.modbus_485_PC02[Bbattery_commu_fault],0,4*16);
     memset(&Globa.modbus_485_PC02[Bbattery_fault],0,4*400);
     modbus_pc02_mutex.unlock();*/
    /*pc43 modbus_pc04_mutex.lockForWrite();
     memset(&Globa.modbus_485_PC04[Bbattery_total_num],0,4*407);
     modbus_pc04_mutex.unlock();*/

  }
}

void Failure_Event_Alarm_Process_Thread::IsSwitch_Branchnumber_Change_funtion()
{
  UINT32 Current_Alarm_List_Switch_Refresh_Falg=0;
  int t;
  Current_Alarm_List_Switch_Refresh_Falg=Globa.Eps_Param_Data.Current_Alarm_List_Switch_Refresh;
  Globa.Eps_Param_Data.Current_Alarm_List_Switch_Refresh=0;

  if(Current_Alarm_List_Switch_Refresh_Falg==1){
      //memset(Battery_Failure_happenflag, 0, sizeof(Battery_Failure_happenflag));
      IO_Failure_happenflag[0]=0;
      IO_Failure_happenflag[1]=0;
      memset(Swith_Off_happenflag,0,sizeof(Swith_Off_happenflag));
      current_alarm_tablelocks_mutex.lockForWrite();
      for(t=0;t<current_alarm_eventlog.count();t++){
        if(current_alarm_eventlog.at(t).name.indexOf(QString("开关量模块通信异常")) !=-1){
           current_alarm_eventlog.removeAt(t);
           t--;
        }
      }

      for(t=0;t<current_alarm_eventlog.count();t++)
        {
          if(current_alarm_eventlog.at(t).name.indexOf(QString("支路")) !=-1)
            {
             current_alarm_eventlog.removeAt(t);
             t--;
      //   Fault_insert_falg = true;
      //    break;
            }
        }
  //   current_fault_tablelocks_mutex.unlock();
      current_alarm_tablelocks_mutex.unlock();
     Current_Alarm_List_Switch_Refresh_Falg=0;
     /*pc28modbus_pc02_mutex.lockForWrite();
     memset(&Globa.modbus_485_PC02[Bbattery_commu_fault],0,4*16);
     memset(&Globa.modbus_485_PC02[Bbattery_fault],0,4*400);
     modbus_pc02_mutex.unlock();*/
    /*pc43 modbus_pc04_mutex.lockForWrite();
     memset(&Globa.modbus_485_PC04[Bbattery_total_num],0,4*407);
     modbus_pc04_mutex.unlock();*/

  }
}

void Failure_Event_Alarm_Process_Thread::DSP_Failure_Event_Alarm_Function()
{
  UINT32 DSP_Fault_Data[DSP_FAULT_YearCheckOk + 1] = {0};
  UINT32 i = 0, Dsp_Data_Refresh_Falg = 0;

  DSP_State_Alarm_mutex.lockForRead();

  DSP_Fault_Data[DSP_FAULT_COMMUNICATION_FAULT] = Globa.Eps_Param_Data.Dsp_Communication_Fault;
  if(!DSP_Fault_Data[DSP_FAULT_COMMUNICATION_FAULT]){
    if(EPS_Run_State != Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status]){
       EPS_Run_State = Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status];//EPS运行状态01H：主电模式， 02H：应急模式，其他待机模式）
    }
  }

  //将全局变量中的值复制到局部变量
  if(!DSP_Fault_Data[DSP_FAULT_COMMUNICATION_FAULT]){
    for(i = DSP_FAULT_MAINS_INPUT_PHASE_REVER;i< Keep_2 + 1; i++){
      DSP_Fault_Data[i] = Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[i - 1];
    }

  //复制年检月检状态信息
    DSP_Fault_Data[DSP_FAULT_MonthCheckOk] = Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status];
    DSP_Fault_Data[DSP_FAULT_YearCheckOk] = Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status];

    Dsp_Data_Refresh_Falg = Globa.Eps_Param_Data.Dsp_Data_Refresh[0];
    Globa.Eps_Param_Data.Dsp_Data_Refresh[0] = 0;
  }
  DSP_State_Alarm_mutex.unlock();

  if(!DSP_Fault_Data[DSP_FAULT_COMMUNICATION_FAULT]){//通信正常
    if(Dsp_Failure_happenflag[DSP_FAULT_COMMUNICATION_FAULT] == 1){
      failure_name = tr("DSP通信异常");
      Insert_History_alarm_eventlog(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_COMMUNICATION_FAULT] = 0;
    }

    if(Dsp_Data_Refresh_Falg == 1){
      Dsp_Data_Refresh_Falg = 0;
      if(DSP_Fault_Data[DSP_FAULT_MAINS_INPUT_PHASE_REVER] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_INPUT_PHASE_REVER] == 0){
          failure_name = tr("主电输入相序接反");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_INPUT_PHASE_REVER] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_INPUT_PHASE_REVER] == 1){
          failure_name = tr("主电输入相序接反");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_INPUT_PHASE_REVER] = 0;
        }
      }

 /*     if(DSP_Fault_Data[DSP_FAULT_MAINS_ABNORMAL] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_ABNORMAL] == 0){
          failure_name = tr("主电异常");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_ABNORMAL] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_ABNORMAL] == 1){
          failure_name = tr("主电异常");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_ABNORMAL] = 0;
        }
      }
*/
     if(DSP_Fault_Data[DSP_FAULT_MAINS_V_ABNORMAL] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL] == 0){
          failure_name = tr("主电电压异常");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL] == 1){
          failure_name = tr("主电电压异常");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_MAINS_V_ABNORMAL_A] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_A] == 0){
          failure_name = tr("主电A相电压异常");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_A] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_A] == 1){
          failure_name = tr("主电A相电压异常");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_A] = 0;
        }
      }


      if(DSP_Fault_Data[DSP_FAULT_MAINS_V_ABNORMAL_B] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_B] == 0){
          failure_name = tr("主电B相电压异常");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_B] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_B] == 1){
          failure_name = tr("主电B相电压异常");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_B] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_MAINS_V_ABNORMAL_C] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_C] == 0){
          failure_name = tr("主电C相电压异常");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_C] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_C] == 1){
          failure_name = tr("主电C相电压异常");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_C] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_MAINS_Freq_Fault] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_Freq_Fault] == 0){
          failure_name = tr("主电频率异常");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_Freq_Fault] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_Freq_Fault] == 1){
          failure_name = tr("主电频率异常");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_Freq_Fault] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_MAINS_OPEN_Fault] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_OPEN_Fault] == 0){
          failure_name = tr("主电开路");
          //printf("Mains_open_save\n");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_OPEN_Fault] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_OPEN_Fault] == 1){
          failure_name = tr("主电开路");
          Insert_History_alarm_eventlog(failure_name);
          //printf("Mains_open_insert\n");

          Dsp_Failure_happenflag[DSP_FAULT_MAINS_OPEN_Fault] = 0;
        }
      }



      if(DSP_Fault_Data[DSP_FAULT_MAINS_SCR_Fault] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_SCR_Fault] == 0){
          failure_name = tr("主电SCR故障");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_SCR_Fault] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_SCR_Fault] == 1){
          failure_name = tr("主电SCR故障");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_SCR_Fault] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_MAINS_Switch_Frequent] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_Switch_Frequent] == 0){
          failure_name = tr("切换次数超限");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_Switch_Frequent] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_Switch_Frequent] == 1){
          failure_name = tr("切换次数超限");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MAINS_Switch_Frequent] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_OUTPUT_A_OVERLOAD] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_A_OVERLOAD] == 0){
          failure_name = tr("输出A相过载");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_A_OVERLOAD] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_A_OVERLOAD] == 1){
          failure_name = tr("输出A相过载");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_A_OVERLOAD] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_OUTPUT_B_OVERLOAD] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_B_OVERLOAD] == 0){
          failure_name = tr("输出B相过载");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_B_OVERLOAD] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_B_OVERLOAD] == 1){
          failure_name = tr("输出B相过载");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_B_OVERLOAD] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_OUTPUT_C_OVERLOAD] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_C_OVERLOAD] == 0){
          failure_name = tr("输出C相过载");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_C_OVERLOAD] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_C_OVERLOAD] == 1){
          failure_name = tr("输出C相过载");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_C_OVERLOAD] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_OUTPUT_OPEN] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OPEN] == 0){
          failure_name = tr("输出开路");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OPEN] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OPEN] == 1){
          failure_name = tr("输出开路");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OPEN] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_OUTPUT_OVERLOAD_SHUTDOWN] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OVERLOAD_SHUTDOWN] == 0){
          failure_name = tr("主电过载保护");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OVERLOAD_SHUTDOWN] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OVERLOAD_SHUTDOWN] == 1){
          failure_name = tr("主电过载保护");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OVERLOAD_SHUTDOWN] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_OUTPUTt_SHORT_CIRCUIT] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUTt_SHORT_CIRCUIT] == 0){
          failure_name = tr("输出短路");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUTt_SHORT_CIRCUIT] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUTt_SHORT_CIRCUIT] == 1){
          failure_name = tr("输出短路");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OUTPUTt_SHORT_CIRCUIT] = 0;
        }
      }

 /*     if(DSP_Fault_Data[DSP_FAULT_EMERGENCY_Fault] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_Fault] == 0){
          failure_name = tr("应急故障");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_Fault] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_Fault] == 1){
          failure_name = tr("应急故障");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_Fault] = 0;
        }
      }
*/
      if(DSP_Fault_Data[DSP_FAULT_EMERGENCY_SOFT_START_FAILS] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SOFT_START_FAILS] == 0){
          failure_name = tr("应急软启失败");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SOFT_START_FAILS] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SOFT_START_FAILS] == 1){
          failure_name = tr("应急软启失败");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SOFT_START_FAILS] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_EMERGENCY_OVER_VOLTAGE] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_VOLTAGE] == 0){
          failure_name = tr("应急过压");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_VOLTAGE] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_VOLTAGE] == 1){
          failure_name = tr("应急过压");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_VOLTAGE] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_EMERGENCY_UNDER_VOLTAGE] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNDER_VOLTAGE] == 0){
          failure_name = tr("应急欠压");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNDER_VOLTAGE] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNDER_VOLTAGE] == 1){
          failure_name = tr("应急欠压");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNDER_VOLTAGE] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_EMERGENCY_SCR_Fault] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SCR_Fault] == 0){
          failure_name = tr("应急SCR故障");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SCR_Fault] = 1;
        }
      }else{
       if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SCR_Fault] == 1){
          failure_name = tr("应急SCR故障");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SCR_Fault] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_EMERGENCY_OVER_CURRENT] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_CURRENT] == 0){
          failure_name = tr("应急过流");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_CURRENT] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_CURRENT] == 1){
          failure_name = tr("应急过流");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_CURRENT] = 0;
        }
      }


      if(DSP_Fault_Data[DSP_FAULT_EMERGENCY_VCE] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_VCE] == 0){
          failure_name = tr("硬件过流");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_VCE] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_VCE] == 1){
          failure_name = tr("硬件过流");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_VCE] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_EMERGENCY_OVER_TEMP] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_TEMP] == 0){
          failure_name = tr("应急过温");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_TEMP] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_TEMP] == 1){
          failure_name = tr("应急过温");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_TEMP] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_EMERGENCY_UNBALANCE] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNBALANCE] == 0){
          failure_name = tr("应急电压不平衡");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNBALANCE] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNBALANCE] == 1){
          failure_name = tr("应急电压不平衡");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNBALANCE] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_FUSE_FAILURE] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_FUSE_FAILURE] == 0){
          failure_name = tr("应急过载保护");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_FUSE_FAILURE] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_FUSE_FAILURE] == 1){
          failure_name = tr("应急过载保护");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_FUSE_FAILURE] = 0;
        }
      }
/*
      if(DSP_Fault_Data[DSP_FAULT_BATTERY_CONTACTS_ABNORMAL] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONTACTS_ABNORMAL] == 0){
          failure_name = tr("电池接触器异常");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONTACTS_ABNORMAL] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONTACTS_ABNORMAL] == 1){
          failure_name = tr("电池接触器异常");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONTACTS_ABNORMAL] = 0;
        }
      }
*/
      if(DSP_Fault_Data[DSP_FAULT_DC_SOFT_START_FAILS] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_DC_SOFT_START_FAILS] == 0){
          failure_name = tr("直流软启失败");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_DC_SOFT_START_FAILS] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_DC_SOFT_START_FAILS] == 1){
          failure_name = tr("直流软启失败");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_DC_SOFT_START_FAILS] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_BATTERY_CONNECTION_EXCEPTION] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONNECTION_EXCEPTION] == 0){
          failure_name = tr("电池开路");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONNECTION_EXCEPTION] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONNECTION_EXCEPTION] == 1){
          failure_name = tr("电池开路");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONNECTION_EXCEPTION] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_BATTERY_OVER_VOLTAGE] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_VOLTAGE] == 0){
          failure_name = tr("电池过压");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_VOLTAGE] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_VOLTAGE] == 1){
          failure_name = tr("电池过压");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_VOLTAGE] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_BATTERY_UNDER_VOLTAGE] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE] == 0){
          failure_name = tr("电池低压");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE] == 1){
          failure_name = tr("电池低压");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE] = 0;
        }
      }

     /* if(DSP_Fault_Data[DSP_FAULT_BATTERY_OVER_TEMP] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_TEMP] == 0){
          failure_name = tr("电池过温");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_TEMP] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_TEMP] == 1){
          failure_name = tr("电池过温");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_TEMP] = 0;
        }
      }
*/
      if(DSP_Fault_Data[DSP_FAULT_BATTERY_OVER_CURR] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_CURR] == 0){
          failure_name = tr("电池过流");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_CURR] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_CURR] == 1){
          failure_name = tr("电池过流");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_CURR] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_BATTERY_UNDER_VOLTAGE_SHUTDOWN] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE_SHUTDOWN] == 0){
          failure_name = tr("电池低压关机");
          Save_Current_fault_event(failure_name);
          Insert_Operation_Work_Record(1, "电池低压关机");
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE_SHUTDOWN] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE_SHUTDOWN] == 1){
          failure_name = tr("电池低压关机");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE_SHUTDOWN] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_BUS_OVERVOLTAGE] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_BUS_OVERVOLTAGE] == 0){
          failure_name = tr("母线过压");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BUS_OVERVOLTAGE] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_BUS_OVERVOLTAGE] == 1){
          failure_name = tr("母线过压");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BUS_OVERVOLTAGE] = 0;
        }
      }

     /* if(DSP_Fault_Data[DSP_FAULT_BUS_SHORT_CIRCUIT] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_BUS_SHORT_CIRCUIT] == 0){
          failure_name = tr("母线短路");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BUS_SHORT_CIRCUIT] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_BUS_SHORT_CIRCUIT] == 1){
          failure_name = tr("母线短路");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_BUS_SHORT_CIRCUIT] = 0;
        }
      }
*/
      if(DSP_Fault_Data[DSP_FAULT_CHARGE_OPEN] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_CHARGE_OPEN] == 0){
          failure_name = tr("充电开路");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_CHARGE_OPEN] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_CHARGE_OPEN] == 1){
          failure_name = tr("充电开路");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_CHARGE_OPEN] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_FAN_FAULT] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_FAN_FAULT] == 0){
          failure_name = tr("风机故障");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_FAN_FAULT] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_FAN_FAULT] == 1){
          failure_name = tr("风机故障");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_FAN_FAULT] = 0;
        }
      }

 /*     if(DSP_Fault_Data[DSP_FAULT_OVER_TEMP] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_OVER_TEMP] == 0){
          failure_name = tr("过温故障");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OVER_TEMP] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_OVER_TEMP] == 1){
          failure_name = tr("过温故障");
          Insert_History_fault_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_OVER_TEMP] = 0;
        }
      }
      */
/*
      if(DSP_Fault_Data[DSP_FAULT_SYSTEM_FAUL] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_SYSTEM_FAUL] == 0){
          failure_name = tr("DSP系统故障");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_SYSTEM_FAUL] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_SYSTEM_FAUL] == 1){
            failure_name = tr("DSP系统故障");
            Insert_History_fault_eventlog(failure_name);
            Dsp_Failure_happenflag[DSP_FAULT_SYSTEM_FAUL] = 0;
        }
      }
*/
 /*     if(DSP_Fault_Data[DSP_FAULT_FIRE_LINKAGE] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_FIRE_LINKAGE] == 0){
          failure_name = tr("消防联动");
          Save_Current_fault_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_FIRE_LINKAGE] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_FIRE_LINKAGE] == 1){
            failure_name = tr("消防联动");
            Insert_History_fault_eventlog(failure_name);
            Dsp_Failure_happenflag[DSP_FAULT_FIRE_LINKAGE] = 0;
        }
      }
*/
      if(DSP_Fault_Data[DSP_FAULT_NO_FUCHONG] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_NO_FUCHONG] == 0){
          failure_name = tr("主电电压过低");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_NO_FUCHONG] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_NO_FUCHONG] == 1){
          failure_name = tr("主电电压过低");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_NO_FUCHONG] = 0;
        }
      }
/*
      if(DSP_Fault_Data[DSP_FAULT_DCDC_ALARM] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_DCDC_ALARM] == 0){
          failure_name = tr("DCDC告警");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_DCDC_ALARM] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_DCDC_ALARM] == 1){
          failure_name = tr("DCDC告警");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_DCDC_ALARM] = 0;
        }
      }
*/
      if(DSP_Fault_Data[DSP_FAULT_SAMPLING_BOARD_ERROR] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_SAMPLING_BOARD_ERROR] == 0){
          failure_name = tr("采样板错误");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_SAMPLING_BOARD_ERROR] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_SAMPLING_BOARD_ERROR] == 1){
          failure_name = tr("采样板错误");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_SAMPLING_BOARD_ERROR] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_MONITOR_VERSION_ERROR] == 1){
        if(Dsp_Failure_happenflag[DSP_FAULT_MONITOR_VERSION_ERROR] == 0){
          failure_name = tr("监控版本错误");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MONITOR_VERSION_ERROR] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MONITOR_VERSION_ERROR] == 1){
          failure_name = tr("监控版本错误");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MONITOR_VERSION_ERROR] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_MonthCheckOk] == 2){
        if(Dsp_Failure_happenflag[DSP_FAULT_MonthCheckOk] == 0){
           failure_name = tr("月检不通过");
           Save_Current_alarm_event(failure_name);
           Dsp_Failure_happenflag[DSP_FAULT_MonthCheckOk] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_MonthCheckOk] == 1){
          failure_name = tr("月检不通过");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_MonthCheckOk] = 0;
        }
      }

      if(DSP_Fault_Data[DSP_FAULT_YearCheckOk] == 2){
        if(Dsp_Failure_happenflag[DSP_FAULT_YearCheckOk] == 0){
          failure_name = tr("年检不通过");
          Save_Current_alarm_event(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_YearCheckOk] = 1;
        }
      }else{
        if(Dsp_Failure_happenflag[DSP_FAULT_YearCheckOk] == 1){
          failure_name = tr("年检不通过");
          Insert_History_alarm_eventlog(failure_name);
          Dsp_Failure_happenflag[DSP_FAULT_YearCheckOk] = 0;
        }
      }
    }
  }else{//通信异常
    if(Dsp_Failure_happenflag[DSP_FAULT_COMMUNICATION_FAULT] == 0){
      failure_name = tr("DSP通信异常");
      Save_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_COMMUNICATION_FAULT] = 1;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_INPUT_PHASE_REVER] == 1){
      failure_name = tr("主电输入相序接反");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_MAINS_INPUT_PHASE_REVER] = 0;
    }
/*
    if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_ABNORMAL] == 1){
      failure_name = tr("主电异常");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_MAINS_ABNORMAL] = 0;
    }
*/
    if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL] == 1){
      failure_name = tr("主电电压异常");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_A] == 1){
      failure_name = tr("主电A相电压异常");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_A] = 0;
    }
    if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_B] == 1){
      failure_name = tr("主电B相电压异常");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_B] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_C] == 1){
      failure_name = tr("主电C相电压异常");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_MAINS_V_ABNORMAL_C] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_Freq_Fault] == 1){
      failure_name = tr("主电频率异常");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_MAINS_Freq_Fault] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_OPEN_Fault] == 1){
      failure_name = tr("主电开路");
      Delete_Current_alarm_event(failure_name);
      //printf("Mains_open_delete\n");

      Dsp_Failure_happenflag[DSP_FAULT_MAINS_OPEN_Fault] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_SCR_Fault] == 1){
      failure_name = tr("主电SCR故障");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_MAINS_SCR_Fault] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_MAINS_Switch_Frequent] == 1){
      failure_name = tr("切换次数超限");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_MAINS_Switch_Frequent] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_A_OVERLOAD] == 1){
      failure_name = tr("输出A相过载");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_A_OVERLOAD] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_B_OVERLOAD] == 1){
      failure_name = tr("输出B相过载");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_B_OVERLOAD] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_C_OVERLOAD] == 1){
      failure_name = tr("输出C相过载");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_C_OVERLOAD] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OPEN] == 1){
      failure_name = tr("输出开路");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OPEN] = 0;
    }
    if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OVERLOAD_SHUTDOWN] == 1){
      failure_name = tr("主电过载保护");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_OUTPUT_OVERLOAD_SHUTDOWN] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_OUTPUTt_SHORT_CIRCUIT] == 1){
      failure_name = tr("输出短路");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_OUTPUTt_SHORT_CIRCUIT] = 0;
    }
/*    if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_Fault] == 1){
      failure_name = tr("应急故障");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_Fault] = 0;
    }
*/
    if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SOFT_START_FAILS] == 1){
      failure_name = tr("应急软启失败");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SOFT_START_FAILS] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_VOLTAGE] == 1){
      failure_name = tr("应急过压");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_VOLTAGE] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNDER_VOLTAGE] == 1){
      failure_name = tr("应急欠压");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNDER_VOLTAGE] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SCR_Fault] == 1){
       failure_name = tr("应急SCR故障");
       Delete_Current_fault_event(failure_name);
       Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_SCR_Fault] = 0;
     }

    if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_CURRENT] == 1){
      failure_name = tr("应急过流");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_CURRENT] = 0;
    }
    if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_VCE] == 1){
      failure_name = tr("硬件过流");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_VCE] = 0;
    }
    if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_TEMP] == 1){
      failure_name = tr("应急过温");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_OVER_TEMP] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNBALANCE] == 1){
      failure_name = tr("应急电压不平衡");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_EMERGENCY_UNBALANCE] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_FUSE_FAILURE] == 1){
      failure_name = tr("应急过载保护");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_FUSE_FAILURE] = 0;
    }
    /*if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONTACTS_ABNORMAL] == 1){
      failure_name = tr("电池接触器异常");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONTACTS_ABNORMAL] = 0;
    }*/
    if(Dsp_Failure_happenflag[DSP_FAULT_DC_SOFT_START_FAILS] == 1){
      failure_name = tr("直流软启失败");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_DC_SOFT_START_FAILS] = 0;
    }
    if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONNECTION_EXCEPTION] == 1){
      failure_name = tr("电池开路");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_BATTERY_CONNECTION_EXCEPTION] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_VOLTAGE] == 1){
      failure_name = tr("电池过压");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_VOLTAGE] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE] == 1){
      failure_name = tr("电池低压");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE] = 0;
    }
 /*   if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_TEMP] == 1){
      failure_name = tr("电池过温");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_TEMP] = 0;
    }
*/
    if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_CURR] == 1){
      failure_name = tr("电池过流");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_BATTERY_OVER_CURR] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE_SHUTDOWN] == 1){
      failure_name = tr("电池低压关机");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE_SHUTDOWN] = 0;
    }
    if(Dsp_Failure_happenflag[DSP_FAULT_BUS_OVERVOLTAGE] == 1){
      failure_name = tr("母线过压");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_BUS_OVERVOLTAGE] = 0;
    }

   /* if(Dsp_Failure_happenflag[DSP_FAULT_BUS_SHORT_CIRCUIT] == 1){
      failure_name = tr("母线短路");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_BUS_SHORT_CIRCUIT] = 0;
    }
*/
    if(Dsp_Failure_happenflag[DSP_FAULT_CHARGE_OPEN] == 1){
      failure_name = tr("充电开路");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_CHARGE_OPEN] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_FAN_FAULT] == 1){
      failure_name = tr("风机故障");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_FAN_FAULT] = 0;
    }

 /*   if(Dsp_Failure_happenflag[DSP_FAULT_OVER_TEMP] == 1){
      failure_name = tr("过温故障");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_OVER_TEMP] = 0;
    }
    */
/*
    if(Dsp_Failure_happenflag[DSP_FAULT_SYSTEM_FAUL] == 1){
      failure_name = tr("DSP系统故障");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_SYSTEM_FAUL] = 0;
    }*/
 /*   if(Dsp_Failure_happenflag[DSP_FAULT_FIRE_LINKAGE] == 1){
      failure_name = tr("消防联动");
      Delete_Current_fault_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_FIRE_LINKAGE] = 0;
      }
*/
    if(Dsp_Failure_happenflag[DSP_FAULT_NO_FUCHONG] == 1){
      failure_name = tr("主电电压过低");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_NO_FUCHONG] = 0;
    }
  /*
    if(Dsp_Failure_happenflag[DSP_FAULT_DCDC_ALARM] == 1){
      failure_name = tr("DCDC告警");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_DCDC_ALARM] = 0;
    }
*/
    if(Dsp_Failure_happenflag[DSP_FAULT_SAMPLING_BOARD_ERROR] == 1){
      failure_name = tr("采样板错误");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_SAMPLING_BOARD_ERROR] = 0;
    }
    if(Dsp_Failure_happenflag[DSP_FAULT_MONITOR_VERSION_ERROR] == 1){
      failure_name = tr("监控版本错误");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_MONITOR_VERSION_ERROR] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_MonthCheckOk] == 1){
      failure_name = tr("月检不通过");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_MonthCheckOk] = 0;
    }

    if(Dsp_Failure_happenflag[DSP_FAULT_YearCheckOk] == 1){
      failure_name = tr("年检不通过");
      Delete_Current_alarm_event(failure_name);
      Dsp_Failure_happenflag[DSP_FAULT_YearCheckOk] = 0;
    }
  }
}

//*********************读取系统所有时间，uptime -为系统运行时间************
time_t Failure_Event_Alarm_Process_Thread::Read_systime()
{
  /*struct sysinfo sys_time;
  sysinfo(&sys_time); //读取系统所有时间，uptime -为系统运行时间
  return sys_time.uptime;*/
  time_t sys_time;
  sys_time=time(NULL);
  return sys_time;
}

/*******************************************************************
*自动月/年检测需要以通讯方式告知DSP，由DSP完成检测，并上报状态；
*机器持续主电工作48小时后每隔30±2天，机器自动由主电输出状态转为应急输出状态，并持续工作60秒后，
*自动恢复到主电工作状态（在自动模式下这个检测是否正常，怎么知道）
*机器持续主电工作一年后，机器自动由主电输出状态转为应急输出状态，并持续至放电终止，
*然后自动恢复到主电工作状态。注：持续应急放电时间不应小于30分钟。
*年月检功能应设置手动实验功能，手动自检不应影响自动自检计时，
*如系统断电且应急工作至放电终止后，应在接通电源后重新开始计时。
*******************************************************************/
void Failure_Event_Alarm_Process_Thread::Self_Checking_Function()
{

  Sys_Self_Check_Parame.Month_Self_Check_Time = Read_systime(); //月检当前运行时间
  Sys_Self_Check_Parame.Year_Self_Check_Time = Read_systime();  //年检当前运行时间

  //月检
  //printf("in check\n");
  //printf("self%d\n",Sys_Self_Check_Parame.Month_Self_Check_Time);
  //printf("star%d\n",Sys_Self_Check_Parame.Month_Self_Check_Start_Time);
  /*if(Sys_Self_Check_Parame.Month_Self_Check_Time<Sys_Self_Check_Parame.Month_Self_Check_Start_Time){
    Sys_Self_Check_Parame.Month_Self_Check_Start_Time=Sys_Self_Check_Parame.Month_Self_Check_Time;
    Sys_Self_Check_Parame.Year_Self_Check_Start_Time=Sys_Self_Check_Parame.Year_Self_Check_Time;
    }*/  //deleted by zzc 629
 // printf("self%d\n",Sys_Self_Check_Parame.Month_Self_Check_Time);
  //printf("star%d\n",Sys_Self_Check_Parame.Month_Self_Check_Start_Time);

  if(((Sys_Self_Check_Parame.Year_Self_Check_Time - Sys_Self_Check_Parame.Year_Self_Check_Start_Time) >=YEAR_SELF_CHECK_TIME)\
     &&((Sys_Self_Check_Parame.Month_Self_Check_Time - Sys_Self_Check_Parame.Month_Self_Check_Start_Time) >=MONTH_SELF_CHECK_TIME)){
      Sys_Self_Check_Parame.Month_Self_Check_Start_Time=Sys_Self_Check_Parame.Month_Self_Check_Time;
    }//为了测试年检时不产生月检

  if((Sys_Self_Check_Parame.Month_Self_Check_Time - Sys_Self_Check_Parame.Month_Self_Check_Start_Time) >= MONTH_SELF_CHECK_TIME){//机器持续主电工作48小时后每隔30±2天//30*24*60*60
      //printf("fs\n");
      if(EPS_Run_State == 1){//01H：主电模式
      if((Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status]%3 != 1 )&&(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status]%3 != 1)){
        if(Issuedequipment_parameters_06(DSPSTARTADDR, Month_Check_Value_Enum, 0xFF, 1)){//; //月检命令（0xff 有效，默认0）启动月检命令
          Sys_Self_Check_Parame.Month_Self_Check_Falg = 1;//月检标志
          Sys_Self_Check_Parame.Month_Self_Check_Start_Time = Read_systime();
          Sys_Self_Check_Parame.Month_Self_first_Check_Falg  = 0;
          Insert_Operation_Work_Record(1, "自动启动月检");
          Mon_CheckResultShow_Flag = 1;
          DSP_Analog_Data_mutex.lockForWrite();
          Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status] = 4;
          DSP_Analog_Data_mutex.unlock();
        }
        else{
              //QMessageBox msgBox;
              ZMessage msgBox;
              msgBox.setWindowTitle(tr("消息"));
              msgBox.setText(tr("参数下发失败!"));
              //msgBox.setIcon(QMessageBox::Information);
              //msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.exec();
            }
      }else {
          ZMessage msgBox;
          //QMessageBox msgBox;
          msgBox.setWindowTitle(tr("警告"));
          msgBox.setText(tr("正在进行月检或年检"));
          //msgBox.setIcon(QMessageBox::Information);
          //msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
    }
  }

  //年检
   else if(((Sys_Self_Check_Parame.Year_Self_Check_Time - Sys_Self_Check_Parame.Year_Self_Check_Start_Time) >= YEAR_SELF_CHECK_TIME)){//365*24*60*60机器持续主电工作一年后，机器自动由主电输出状态转为应急输出状态。
     if(EPS_Run_State == 1){//主电模式
       if((Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status]%3 != 1 )&&(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status]%3 != 1)){
         if(Issuedequipment_parameters_06(DSPSTARTADDR, Year_Check_Value_Enum, 0xFF, 1)){
           Sys_Self_Check_Parame.Year_Self_Check_Falg = 1;//年检标志
           Sys_Self_Check_Parame.Year_Self_Check_Start_Time = Read_systime();
           Insert_Operation_Work_Record(1, "自动启动年检");
           year_CheckResultShow_Flag = 1;
           DSP_Analog_Data_mutex.lockForWrite();
           Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status] = 4;
           DSP_Analog_Data_mutex.unlock();
         }
         else{
               ZMessage msgBox;
               //QMessageBox msgBox;
               msgBox.setWindowTitle(tr("消息"));
               msgBox.setText(tr("参数下发失败!"));
               //msgBox.setIcon(QMessageBox::Information);
               //msgBox.setStandardButtons(QMessageBox::Ok);
               msgBox.exec();
             }
       }
       else {
                 ZMessage msgBox;
                 //QMessageBox msgBox;
                 msgBox.setWindowTitle(tr("警告"));
                 msgBox.setText(tr("正在进行月检或年检"));
                 //msgBox.setIcon(QMessageBox::Information);
                 //msgBox.setStandardButtons(QMessageBox::Ok);
                 msgBox.exec();
               }
     }
}
}




//*************************系统事件处理*******************************
void Failure_Event_Alarm_Process_Thread::Event_Alarm_Process_function()
{

   UINT32 i = 0;
  UINT32 DSP_Reset=0;
  UINT32 EPS_State_Emer=0;
  System_Total_fault = 0;
  System_Total_Alarm = 0;

  DSP_Failure_Event_Alarm_Function();

  Switch_Failure_Event_Alarm_Function();

  Dry_Contact_Function();
  System_Fault=0;
  if(EPS_Run_State == 1){//0-待机，1-主电，2-应急
    EMERGENCY_INDICATOR_OFF;
    MAIN_ELECTRIC_INDICATOR_ON;
  }else if(EPS_Run_State == 2){
    EMERGENCY_INDICATOR_ON;
    MAIN_ELECTRIC_INDICATOR_OFF;
  }else{
    EMERGENCY_INDICATOR_OFF;
    MAIN_ELECTRIC_INDICATOR_OFF;
  }

  if(ls_EPS_Run_State != EPS_Run_State){
    ls_EPS_Run_State = EPS_Run_State;

    if(ls_EPS_Run_State == 0){//0-待机，1-主电，2-应急
      Insert_Operation_Work_Record(0, "待机模式");
    }else if(ls_EPS_Run_State == 1){
      Insert_Operation_Work_Record(0, "主电模式");
    }else if(ls_EPS_Run_State == 2){
      Silent_Mode_flag = 0;
      Insert_Operation_Work_Record(0, "应急模式");
    }
}

  if(EPS_Run_State==2){
      EPS_State_Emer=1;
 //     Silent_Mode_flag = 0;
    }else{
      EPS_State_Emer=0;
    }

  Battery_Charge_Flag_mutex.lockForWrite();
  if((Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Start]==1)||(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Battery_Start]==2)){
      Battery_Charge_Flag=1;
  }else {
      Battery_Charge_Flag=0;
  }

  Battery_Charge_Flag_mutex.unlock();
  if(Battery_Charge_Flag == 1){//充电
    CHARGE_INDICATOR_ON;
  }else{
    CHARGE_INDICATOR_OFF;
  }

DSP_State_Alarm_mutex.lockForRead();
  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Strong_Start_Mode] == 1){//强启模式
    STRONG_INSTRUCTION_ON;
  }else{
    STRONG_INSTRUCTION_OFF;
  }
DSP_State_Alarm_mutex.unlock();


IsBattery_Group_Change_funtion();

 for(i = 0; ((i<Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable)&&(i<BATTERYMAXGROUP)); i++){
        Battery_Failure_Event_Alarm_Function(i);
  }


 for(i = 0; i<(DSP_FAULT_YearCheckOk + 1); i++){
     if((i == DSP_FAULT_MAINS_SCR_Fault)||(i == DSP_FAULT_MAINS_Switch_Frequent)||(i == DSP_FAULT_OUTPUT_A_OVERLOAD)||(i == DSP_FAULT_OUTPUT_B_OVERLOAD)||
       (i == DSP_FAULT_OUTPUT_C_OVERLOAD)|| (i == DSP_FAULT_OUTPUT_OVERLOAD_SHUTDOWN)||(i == DSP_FAULT_OUTPUTt_SHORT_CIRCUIT)||(i == DSP_FAULT_EMERGENCY_SCR_Fault)||
       (i == DSP_FAULT_EMERGENCY_OVER_CURRENT)||(i == DSP_FAULT_EMERGENCY_OVER_TEMP)||(i == DSP_FAULT_FUSE_FAILURE)||(i == DSP_FAULT_EMERGENCY_UNBALANCE)||(i == DSP_FAULT_FAN_FAULT)){
       System_Total_fault |= Dsp_Failure_happenflag[i];
       }else if(i!=DSP_FAULT_BATTERY_UNDER_VOLTAGE){
         System_Total_Alarm|=Dsp_Failure_happenflag[i];
       }
}

 current_alarm_tablelocks_mutex.lockForWrite();
 if(current_alarm_eventlog.count()!=0)

  {
     System_Total_Alarm=1;

 }
 current_alarm_tablelocks_mutex.unlock();

  if((Reset_flag==0)&&(Reset_time==0)){
  System_Fault = System_Total_fault|System_Total_Alarm/*|Dsp_Failure_happenflag[DSP_FAULT_BATTERY_UNDER_VOLTAGE]*/;
    }else{
      System_Fault=0;
    }
  if((/*System_Total_fault*/System_Fault == 1)){
    FAULT_INDICATOR_ON;
  }else{
    FAULT_INDICATOR_OFF;
  }

  DSP_State_Alarm_mutex.lockForRead();
  DSP_Reset=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Reset];
  DSP_State_Alarm_mutex.unlock();




   if(DSP_Reset==1){
        if(DSP_Reset!=Last_DSP_Reset){
           Reset_count=0;
           Reset_time=1;
           //Silent_Mode_flag=1;
           Reset_flag=1;
           emit Reset_Timer_Flag(0);

          }
     }
    Last_DSP_Reset=DSP_Reset;

    if(Reset_time==1){
     Reset_count++;
     if(Reset_count>=14)
       {Reset_time=0;
        Reset_count=0;
       }
   }



  if((Globa.User_Monitor_Screen_Set.AlarmSound == 1)&&(Silent_Mode_flag == 0)&&(Reset_flag == 0)){
    alarm_failure ++;
    //printf("the fengmiqi alarm\n");
    if(System_Total_fault == 1){
           GPIO_SYS_BUZZ_ON; // 声光报警--声报警

       }else{
         if((System_Total_Alarm == 1)&&(Battery_Under_Voltage_Flag==0)){
           if(alarm_failure %2 == 0){
             GPIO_SYS_BUZZ_ON; // 声光报警--声报警

           }else{
             GPIO_SYS_BUZZ_OFF; // 声光报警--声报警
           }
         }else if((EPS_State_Emer == 1)&&(Battery_Under_Voltage_Flag==0)){
           if(alarm_failure %4 == 0){
             GPIO_SYS_BUZZ_ON; // 声光报警--声报警

           }else{
             GPIO_SYS_BUZZ_OFF; // 声光报警--声报警
           }
         }else{
           GPIO_SYS_BUZZ_OFF; // 声光报警--声报警
         }
       }
   /*0.5s
     if(System_Total_fault == 1){
            GPIO_SYS_BUZZ_ON; // 声光报警--声报警
        }else{
          if((System_Total_Alarm == 1)&&(Battery_Under_Voltage_Flag==0)){
            if((alarm_failure %4 == 0)||(alarm_failure %4 == 1)){
              GPIO_SYS_BUZZ_ON; // 声光报警--声报警
            }else{
              GPIO_SYS_BUZZ_OFF; // 声光报警--声报警
            }
          }else if((EPS_State_Emer == 1)&&(Battery_Under_Voltage_Flag==0)){
            if(alarm_failure %5 == 0){
              GPIO_SYS_BUZZ_ON; // 声光报警--声报警
            }else{
              GPIO_SYS_BUZZ_OFF; // 声光报警--声报警
            }
          }else{
            GPIO_SYS_BUZZ_OFF; // 声光报警--声报警
          }
        }*/
  /*  if(System_Total_fault == 1){
      if(alarm_failure %4 == 0){
        GPIO_SYS_BUZZ_ON; // 声光报警--声报警
      }else{
        GPIO_SYS_BUZZ_OFF; // 声光报警--声报警
      }
    }else{
      if(System_Total_Alarm == 1){
        if(alarm_failure %5 == 0){
          GPIO_SYS_BUZZ_ON; // 声光报警--声报警
        }else{
          GPIO_SYS_BUZZ_OFF; // 声光报警--声报警
        }
      }else{
        GPIO_SYS_BUZZ_OFF; // 声光报警--声报警
      }
    }*/
  }else{
      GPIO_SYS_BUZZ_OFF; // 声光报警--声报警
    }
 // printf("di\n");
  if(Globa.Send_Dsp_Debug_Param_Set.EPS_Work_Mode_Selection==0){
  if((Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status]%3 != 1 )\
     &&(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status]%3 != 1)\
     &&(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status]%3!=1)){

         //check_48h_timer->stop();

         //check_48h_timer->start(checktime);
 if(!check_48h_flag){
     Sys_Self_Check_Parame.Month_Self_Check_Start_Time=Read_systime();
     Sys_Self_Check_Parame.Year_Self_Check_Start_Time= Read_systime();

   }
    }else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status]%3==1){
      if(!check_48h_flag){
      if(Read_systime()-Sys_Self_Check_Parame.Month_Self_Check_Start_Time>=checktime){
        check_48h_flag=1;
        Sys_Self_Check_Parame.Month_Self_Check_Start_Time+=checktime;

        //Sys_Self_Check_Parame.Year_Self_Check_Start_Time= Read_systime();
        }
        }
    }


    Self_Checking_Function();//自动月/年检测需要以通讯方式告知DSP，由DSP完成检测，并上报状态；

    }else if(Globa.Send_Dsp_Debug_Param_Set.EPS_Work_Mode_Selection==1){
      Sys_Self_Check_Parame.Year_Self_Check_Start_Time= Read_systime();
      Sys_Self_Check_Parame.Month_Self_Check_Start_Time=Read_systime();
    }
}
