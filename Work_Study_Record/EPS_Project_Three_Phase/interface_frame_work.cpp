#include "interface_frame_work.h"
#include "ui_interface_frame_work.h"


UINT32 screen_brightness_count = 0;//屏保时间计数
UINT32 messagebox_disappear_count=0;//消息对话框出现时间计数
UINT32 Silent_Mode_flag = 0;
UINT32 Reset_flag=0;
UINT32 resetMonitor=0;
UINT32 Reset_count=0;
UINT32 Reset_time=0;
UINT32 Battery_Charge_Flag=0;
UINT32 Shutdown_Flag=0;
UINT32 Shutdown_Flag_flag=0;
UINT32 Load_Stye_Change=0;//负载模式改变，不改变为0，改变为EA_D：1，改变为FEPS：2
bool screensaverflag = true;
UINT32 Restar_SystemFlage = 0 ;//重新启动系统标志
UINT32 messageFlag[10] = {0};
UINT32 Check_Battery_Number_finish=0;
//bool Issuedequipment_Flag=false;//下发监控版本标志



QList<CURRENT_ALARMFAULT_EVENT>current_alarm_eventlog ; // 存放当前告警信息记录
QList<HISTORY_ALARMFAULT_EVENT>history_alarm_eventlog ; // 存放当前历史信息记录
QList<CURRENT_ALARMFAULT_EVENT>current_fault_eventlog ; // 当前故障链表
QList<HISTORY_ALARMFAULT_EVENT>history_fault_eventlog ; // 历史故障链表
QList<OPERATION_WORK_RECORD>history_Operation_Record; //操作记录
QList<OPERATION_WORK_RECORD>history_EpsWork_Record; //工作记录

static int PreIndex = 255;
int preCurIndex = 255;
static int PreParam = 255; //相同界面中的具体参数显示界面
static int PreParam_1 = 255; //相同界面中的具体参数显示界面
static int Section_Number = 255;//相同界面中的显示那个段数据：0-第一段/1-第二段

static int PreArray[20] ={MIAN_INTERFACE_VIEW,MIAN_INTERFACE_VIEW,MIAN_INTERFACE_VIEW,
                          EPS_MAIN_MENU_VIEW,EPS_MAIN_MENU_VIEW,EPS_MAIN_MENU_VIEW,
                          EPS_MAIN_MENU_VIEW,EPS_MAIN_MENU_VIEW,EPS_MAIN_MENU_VIEW,
                          SETMENU_VIEW,SETMENU_VIEW,SETMENU_VIEW,SYSTEMINFOR_VIEW,
                          SYSTEMINFOR_VIEW,MAINTAIN_SET
                         };
QReadWriteLock  DSP_State_Alarm_mutex;      //全局互斥锁---DSP状态量和告警信息
QReadWriteLock  DSP_Analog_Data_mutex;      //全局互斥锁PC --获取模拟量数据
QReadWriteLock  DSP_Fault_Data_mutex;      //全局互斥锁PC ---获取故障信息--逆变器发生故障前后的一些数据
QReadWriteLock  DSP_Debug_Parameter_mutex; //全局互斥锁PC --获取软件版本和校准系数
QReadWriteLock  Battery_mutex; //全局互斥锁PC --获取软件版本和校准系数
QReadWriteLock  Battery_Charge_Flag_mutex;

QReadWriteLock  modbus_pc02_mutex;    //全局互斥锁PC
QReadWriteLock  modbus_pc04_mutex;    //全局互斥锁PC
QReadWriteLock  modbus_pc03_mutex;
QReadWriteLock  current_alarm_tablelocks_mutex;    //全局互斥锁
QReadWriteLock  history_alarm_tablelocks_mutex;    //全局互斥锁
QReadWriteLock  current_fault_tablelocks_mutex;    //全局互斥锁
QReadWriteLock  history_fault_tablelocks_mutex;    //全局互斥锁
QReadWriteLock  history_Operation_Record_mutex;    //全局互斥锁
QReadWriteLock  history_EpsWork_Record_mutex;      //全局互斥锁

QReadWriteLock  history_alarm_date_mutex;    //全局互斥锁
QReadWriteLock  battery_groupset_mutex;

Interface_Frame_Work::Interface_Frame_Work(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Interface_Frame_Work)
{
  ui->setupUi(this);
  this->setGeometry(0,0,800,480);
  this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏 -desktop must shield
  //  setAutoFillBackground(true);
  //  QPalette palette;
  //  palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/Pagingbackground.png")));
  //  this->setPalette(palette);
  oldstate = 255;
  //battery_off_flag=1;

  Read_history_alarm_Database_to_List();
  Read_history_operationwork_records();
  Read_history_fault_Database_to_List();

  Start_Task_Function();//创建主任务
  Load_All_View_Initial();
  Create_Mian_Button();

  key_effective[0] = false;
  key_effective[1] = false;
  connect(this,SIGNAL(Changed_Current_View(int,int,int,int)),this,SLOT(Display_Current_View(int,int,int,int)));
  Display_Current_View(0,0,0,0);

  main_timer = new QTimer(this);
  connect(main_timer,SIGNAL(timeout()),this,SLOT(main_timer_function()));
  main_timer->start(1000);

  display_data_timer = new QTimer(this);
  connect(display_data_timer,SIGNAL(timeout()),this,SLOT(Display_Data_Function()));
  display_data_timer->start(1500);

  reset_timer=new QTimer(this);
  connect(reset_timer,SIGNAL(timeout()),this,SLOT(reset_timer_function()));

  connect(&Failure_Event_Alarm_Process_Task,SIGNAL(Reset_Timer_Flag(int)),this,SLOT(reset_timer_start()));

  BK_CON_STRUCT  msg;
  msg.msg_type = 1;
  msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;

  msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
  printf("filesize is %d\n",GetFileSize(F_SYSTEMLOGPATH));
  if(GetFileSize(F_SYSTEMLOGPATH)>1024000)//1000KB
    QFile::remove(F_SYSTEMLOGPATH);
  LOG("System start");
}

Interface_Frame_Work::~Interface_Frame_Work()
{
  delete ui;
}
void Interface_Frame_Work::open_cal()
{
  Calibration  ls_clibration;
  ls_clibration.exec();
}

void Interface_Frame_Work::keyPressEvent(QKeyEvent *e)
{
  //  qDebug()<<"keyPressEvent界面号： "<<PreIndex<<"keyPressEvent界面中的内容："<<PreParam<<"keyPressEvent界面中的内容所以那一段"<<Section_Number;
  if(e->key() == Qt::Key_Escape)
    {
      key_effective[1] = true;
    }
  else if(e->key() == Qt::Key_Home)
    {
      key_effective[0] = true;
    }

  if((key_effective[0] == true)&&(key_effective[1] == true))
    {
      QMessageBox msgBox;
      msgBox.setWindowTitle(tr("提示"));
      msgBox.setText(tr("是否重启系统!"));
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
      if(msgBox.exec()  == QMessageBox::Ok)
        { LOG("Keypress reboot");
          Reboot Reboot_View;
          Reboot_View.exec();
          //system("reboot");
        }
      else
        {
          key_effective[0]=key_effective[1]=false ;
        }
    }
}

void Interface_Frame_Work::keyReleaseEvent(QKeyEvent *e)
{
  if(e->key() == Qt::Key_Escape)
    {
      emit Changed_Current_View(PreArray[PreIndex],PreParam,PreParam_1,Section_Number);
      key_effective[1] = false;
    }
  else if(e->key() == Qt::Key_Home)
    {
      emit Changed_Current_View(MIAN_INTERFACE_VIEW,0,0,0);
      key_effective[0] = false;
    }
}

/*****************创建主页按钮返回按钮键************************/
void Interface_Frame_Work::Create_Mian_Button()
{
  GobackButton = new QPushButton(this);
  GobackButton->setObjectName("btn_05");
  GobackButton->setGeometry(720,410,60,60);
  connect(GobackButton, SIGNAL(clicked()), this, SLOT(GobackClicked()));

  MainButton = new QPushButton(this);
  MainButton->setObjectName("btn_03");
  MainButton->setGeometry(20,410,60,60);
  connect(MainButton, SIGNAL(clicked()), this, SLOT(MainClicked()));

  label_time = new QLabel(this);
  label_time->setObjectName("btn_03");
  label_time->setStyleSheet("color:rgb(0, 0, 0)");//黑色
  label_time->setGeometry(530,0,260,30);

  //lb_status=new QLabel(this);
 // lb_status->setGeometry(0,0,100,30);
  //Set_Object_Icon(0);

  lb_mode=new QLabel(this);
 // lb_mode->setGeometry(130,0,100,30);
  lb_mode->setGeometry(20,0,100,30);
  lb_mode->setText("工作模式:");

  lb_mode_value = new QLabel(this);
 // lb_mode_value->setGeometry(210,0,120,30);
  lb_mode_value->setGeometry(110,0,180,30);
  //lb_mode_value->setText("工作模式");

  lb_status=new QLabel(this);
  lb_status->setGeometry(230,0,280,30);
  lb_status->setText("");
  lb_status->setStyleSheet("color:rgb(255,0,0);");

  lb_off_battery=new QLabel(this);
  lb_off_battery->setGeometry(200,480,280,30);
  lb_off_battery->setText("");
  lb_off_battery->setStyleSheet("color:rgb(255,0,0);");



 // lb_inverter_value=new QLabel(this);
 // lb_inverter_value->setGeometry(230,0,100,30);
 // lb_inverter_value->setText("");
  //lb_inverter_value->setStyleSheet("color:rgb(255,0,0);");

  EPS_INFOR = new QPushButton(this) ;
  EPS_INFOR->setGeometry(350,60,120,150);
  EPS_INFOR->setStyleSheet("background:transparent;border:0px;");

  pushButton_Battery = new QPushButton(this);
  pushButton_Battery->setGeometry(350,280,110,110);
  pushButton_Battery->setStyleSheet("background:transparent;border:0px;");

  connect(pushButton_Battery,SIGNAL(clicked()),this,SLOT(pushButton_Battery_clicked()));
  connect(EPS_INFOR,SIGNAL(clicked()),this,SLOT(pushButton_EPS_INFOR_clicked()));


}
void Interface_Frame_Work::pushButton_Battery_clicked()
{

  emit Changed_Current_View(BATTERYFORM_VIEW, 0, 0, 0);

}
void Interface_Frame_Work::pushButton_EPS_INFOR_clicked()
{
  emit Changed_Current_View(EPS_MAIN_MENU_VIEW, 0, 0, 0);


}
/********************加载各种图片并显示*****************/
void Interface_Frame_Work::Set_Object_Icon(UINT32 state)
{
  if(oldstate == state) return;

  oldstate = state;
  QPixmap pixmap(":/images/"+QString::number(state)+".png");
  INT32 nWidth = pixmap.width();
  INT32 nHeight = pixmap.height();
  lb_status->resize(QSize(nWidth, nHeight));
  lb_status->setPixmap(pixmap);
}

//登记每次模式切换时的模式
//void Interface_Frame_Work::Work_Mode_record()
//{
//  if(oldEPSStatus == EPSWorkTable[Globa.User_Monitor_Screen_Set.System_language][Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[EPSStatus]]) return;
//  else
//    {
//      Insert_Operation_Work_Record(0, EPSWorkTable[Globa.User_Monitor_Screen_Set.System_language][Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[EPSStatus]]);
//      oldEPSStatus = EPSWorkTable[Globa.User_Monitor_Screen_Set.System_language][Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[EPSStatus]];
//      return;
//    }
//}


/*****************启动线程任务************************/
void Interface_Frame_Work::Start_Task_Function()
{

  Arm_To_M4_Send_Data_Task.setStackSize(128000);
  Arm_To_M4_Receive_Data_Task.setStackSize(128000);

  Usb_Read_Path_Task.setStackSize(128000);
  Tcp_Modbus_Rece_Send_Data_task.setStackSize(128000);
  Arm_To_Pc_Send_Rece_Data_Task.setStackSize(128000);
  Failure_Event_Alarm_Process_Task.setStackSize(228000);
  Battery_Send_Receive_Data_Task.setStackSize(228000);
  ARM_DSP_Send_Receive_Data_Task.setStackSize(228000);
  Ats_Switch_Send_Receive_data_Task.setStackSize(228000);
  Monitor_Thread_Runnig_Thread_Task.setStackSize(228000);
  Updata_Modbus_Thread_Task.setStackSize(228000);

  Arm_To_M4_Send_Data_Task.start();

  Arm_To_M4_Receive_Data_Task.start();

  ARM_DSP_Send_Receive_Data_Task.start();

  Battery_Send_Receive_Data_Task.start();

  Ats_Switch_Send_Receive_data_Task.start();


  Usb_Read_Path_Task.start();

  Arm_To_Pc_Send_Rece_Data_Task.start();

  Tcp_Modbus_Rece_Send_Data_task.start();

  Failure_Event_Alarm_Process_Task.start();

  Monitor_Thread_Runnig_Thread_Task.start();

  Updata_Modbus_Thread_Task.start();

  //qDebug()<<"创建各个界面";

}

//******************主定时器*屏幕保护即回到主窗口界面*************
void Interface_Frame_Work::main_timer_function()
{ thread_interface_frame=1;
  static UINT32 timecount=0;
  timecount++;
  time_display();
  screen_brightness_count ++ ;
  UINT32 messagebox_flag=0;
      foreach(QWidget *widget, QApplication::topLevelWidgets()){
        //  if(!(widget->isHidden())&&((widget->metaObject()->className() == QMessageBox::staticMetaObject.className()||(widget->metaObject()->className() == ZMessage::staticMetaObject.className())/*||(widget->objectName()=="Startup_Shutdown")*/))&&(widget!=&msgBox_ManualMode)&&(widget!=&msgBox_ShutDown)){
            if(!(widget->isHidden())&&(widget->metaObject()->className() == ZMessage::staticMetaObject.className())){
              messagebox_flag=1;
              messagebox_disappear_count++;
              if(messagebox_disappear_count>4){
                   widget->close();
                   messagebox_disappear_count=0;
              }
          }

      }
      if((!messagebox_flag)&&(messagebox_disappear_count!=0)){
          messagebox_disappear_count=0;
        }


 /*if(screen_brightness_count %5 == 0){

     foreach(QWidget *widget, QApplication::topLevelWidgets())
     {
       if(!(widget->isHidden())&&(widget->metaObject()->className() == QMessageBox::staticMetaObject.className()))
         {
           widget->close();
         }
     }
   }*/

  if(screen_brightness_count > Globa.User_Monitor_Screen_Set.Screensavertime*60){
    if(screensaverflag == true){
      BK_CON_STRUCT  msg;
      msg.msg_type = 1;
      msg.msg_text[0] = 0;
      msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
      screensaverflag = false;
      emit Changed_Current_View(MIAN_INTERFACE_VIEW,0,0,0);

      /*5.13
       *foreach(QWidget *widget, QApplication::topLevelWidgets()){
        if(!(widget->isHidden())&&(widget != this)){
          widget->close();
        }
      }*/
    }
  }
  //工作模式的显示
  DSP_State_Alarm_mutex.lockForRead();
  if(Globa.Send_Dsp_Debug_Param_Set.Manual_Open_Shut_Down==0x0F)
  {
      Shutdown_Flag=1;
  } else
  {

      Shutdown_Flag=0;
  }
  //Shutdown_Flag=Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Inverter_Shutdown];
  DSP_State_Alarm_mutex.unlock();

  DSP_Analog_Data_mutex.lockForRead();
  lb_mode_value->setText(tr("%1").arg(EPSWorkTable[Globa.User_Monitor_Screen_Set.System_language][Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status]]));
  if(Shutdown_Flag==1){

          lb_status->setText(tr("逆变器关闭"));


    }else if((Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status] != 1)&&(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status] != 1)){
      lb_status->setText("");
    }
  DSP_Analog_Data_mutex.unlock();
  /*if(battery_off_flag==0){
      if(timecount%2!=1)
      {
          lb_status->setText(tr("逆变器已经关闭，请断开电池空开"));
      }else{
          lb_status->setText(tr(""));
        }
    }else{
      lb_status->setText(tr(""));
    }*/
}

//*************************定时器
void Interface_Frame_Work::reset_timer_function(){
  reset_timer->stop();
  printf("timeout for silence\n");
  Reset_flag=0;
  //Silent_Mode_flag=0;
}

//**************************定时器start
void Interface_Frame_Work::reset_timer_start(){
  reset_timer->stop();
  printf("start silent timer\n");
  reset_timer->start(60000);//60 seconds
}

//*************************主页时间***************************
void Interface_Frame_Work::time_display()
{
  QDateTime current_date_time = QDateTime::currentDateTime();
  QString current_date = current_date_time.toString(" yyyy-MM-dd hh:mm:ss ");
  label_time->setText(current_date + QString("   %1").arg(maintimetable[Globa.User_Monitor_Screen_Set.System_language][current_date_time.date().dayOfWeek()-1]));
}

//*************************返回上一个界面**********************
void Interface_Frame_Work::GobackClicked()
{
  emit Changed_Current_View(PreArray[PreIndex],PreParam,PreParam_1,Section_Number);
}

//**************************返回主页**************************
void Interface_Frame_Work::MainClicked()
{
  emit Changed_Current_View(MIAN_INTERFACE_VIEW,0,0,0);
}

//*************************显示键盘功能***********************
void Interface_Frame_Work::Display_keyboard_function()
{
  popup_keyboard_view.exec();
}

//*************************显示键盘功能***********************
void Interface_Frame_Work::Display_keyboard()
{
  password_keyboard.exec();
}


//*********创建界面并把其全部加载到窗体中，创立各个界面之间的信号与槽关系。******
void Interface_Frame_Work::Load_All_View_Initial()
{
  stackedWidget = new QStackedWidget(this);
  stackedWidget->setGeometry(0,0,800,480);
  mian_interface_View = new Mian_Interface(this);  //0
  Eps_Main_Menu_View = new Eps_Main_Menu(this);//1
  setmenu = new Setmenu(this);//2
  Monitor_Screen_Set_View = new Monitor_Screen_Set(this);//4
  User_Set_View = new User_Set(this);//10
  AlarmQuery_View = new AlarmQueryForm(this);//11
  Import_Export_Data_View = new Import_Export_Data(this);//12
  Current_Alarm_Query_View = new Current_Alarm_Query(this);//13
  Eps_Run_Information_View = new Eps_Run_Information(this);//15

  Product_Infor_View = new Product_Infor(this);//16

  BatteryForm_View = new BatteryForm(this);//17


  Maintain_Set_View = new Maintain_Set(this);                  //维护设置



  SystemInfor_View = new SystemInfor(this);                    //系统信息

  AboutUs_View = new AboutUs(this);                            //关于我们

  TestData_Display_View = new TestData_Display(this);//13

  Branchalarmset_View=new BranchAlarmSet(this);

  stackedWidget->addWidget(mian_interface_View);           //0    -- 0  主页
  stackedWidget->addWidget(Eps_Main_Menu_View);      //1    -- 0  EPS主菜单

  stackedWidget->addWidget(Current_Alarm_Query_View);      //2   --  0  当前告警

  stackedWidget->addWidget(Eps_Run_Information_View);      //3   --  1  EPS运行信息
  stackedWidget->addWidget(BatteryForm_View);              //4   --  1  电池信息
  stackedWidget->addWidget(SystemInfor_View);              //5   --  1  系统信息
  stackedWidget->addWidget(setmenu);                       //6   --  1  设置选项
  stackedWidget->addWidget(AlarmQuery_View);               //7   --  1  告警信息
  stackedWidget->addWidget(Import_Export_Data_View);       //8   --  1  U盘数据导入导出

  stackedWidget->addWidget(Monitor_Screen_Set_View);       //9   --  6  监控屏设置
  stackedWidget->addWidget(User_Set_View);                 //10  --  6  客户设置
  stackedWidget->addWidget(Maintain_Set_View);             //11  --  6  维护设置

  stackedWidget->addWidget(Product_Infor_View);            //12   -- 5  产品信息
  stackedWidget->addWidget(AboutUs_View);                  //13   -- 5  关于我们


  stackedWidget->addWidget(TestData_Display_View);         //14------0  调试界面

  stackedWidget->addWidget(Branchalarmset_View);

  connect(mian_interface_View,SIGNAL(Changed_Current_View(int,int,int,int)),this,SLOT(Display_Current_View(int,int,int,int)));
  connect(mian_interface_View,SIGNAL(keyboard_show()),this,SLOT(Display_keyboard()));
  connect(&password_keyboard,SIGNAL(PressKey(QString)), mian_interface_View,SLOT(InputMenu(QString)));

  //EPS主菜单,选择显示各个界面
  connect(Eps_Main_Menu_View,SIGNAL(Changed_Current_View(int,int,int,int)),this,SLOT(Display_Current_View(int,int,int,int)));
  connect(Eps_Main_Menu_View,SIGNAL(keyboard_show()),this,SLOT(Display_keyboard()));//显示密码键盘
  connect(&password_keyboard,SIGNAL(PressKey(QString)),Eps_Main_Menu_View ,SLOT(inputmenu(QString)));//验证密码

//  connect(AlarmQuery_View,SIGNAL(keyboard_show()),this,SLOT(Display_keyboard()));//显示密码键盘
 // connect(&password_keyboard,SIGNAL(PressKey(QString)),AlarmQuery_View ,SLOT(AlarmDelete(QString)));//验证密码

  //系统信息
  connect(SystemInfor_View,SIGNAL(Changed_Current_View(int,int,int,int)),this,SLOT(Display_Current_View(int,int,int,int)));

  //历史查询
  //  connect(AlarmQuery_View,SIGNAL(keyboard_show()),this,SLOT(Display_keyboard()));
  //  connect(&password_keyboard,SIGNAL(PressKey(QString)),AlarmQuery_View ,SLOT(Delete_history_data(QString)));

  //设置菜单界面，密码使能会提示输入密码才能进入
  connect(setmenu,SIGNAL(Changed_Current_View(int,int,int,int)),this,SLOT(Display_Current_View(int,int,int,int)));
  //  connect(setmenu,SIGNAL(keyboard_show()),this,SLOT(Display_keyboard()));
  //  connect(&password_keyboard,SIGNAL(PressKey(QString)),setmenu ,SLOT(inputmenu(QString)));

  //监控设置，设置参数，并将参数下发给DSP
  connect(Monitor_Screen_Set_View,SIGNAL(keyboard_show()),this,SLOT(Display_keyboard_function()));
  connect(&popup_keyboard_view,SIGNAL(PressKey(int)),Monitor_Screen_Set_View,SLOT(SysGetKeyValue(int)));

  //客户设置，设置参数，并将参数下发给DSP
  connect(User_Set_View,SIGNAL(keyboard_show()),this,SLOT(Display_keyboard_function()));
  connect( &popup_keyboard_view,SIGNAL(PressKey(int)),User_Set_View,SLOT(User_Set_Parame_Set_GetKeyValue(int)) );
  connect(User_Set_View,SIGNAL(passwordboard_show()),this,SLOT(Display_keyboard()));
  connect(&password_keyboard,SIGNAL(PressKey(QString)), User_Set_View,SLOT(CheckMenu(QString)));

  //维护设置界面，输入参数，并将参数下发给DSP
  connect(Maintain_Set_View,SIGNAL(keyboard_show()),this,SLOT(Display_keyboard_function()));
  connect(&popup_keyboard_view,SIGNAL(PressKey(int)),Maintain_Set_View,SLOT(Maintain_Set_Parameter_Set_GetKeyValue(int)));

  connect(Maintain_Set_View,SIGNAL(Changed_Current_View(int,int,int,int)),this,SLOT(Display_Current_View(int,int,int,int)));

  connect(User_Set_View,SIGNAL(Changed_Current_View(int,int,int,int)),this,SLOT(Display_Current_View(int,int,int,int)));


  qDebug()<<"创建各个界面---   sEND\n\n";

}

/*******************通过信号与槽切换显示界面********************/
void Interface_Frame_Work::Display_Current_View(int index,int Param,int Param_1,int SeNumber)
{

#ifdef __QDEBUG
  qDebug()<<"index:"<<index<<"Param:"<<Param<<"Param_1"<<Param_1<<"SeNumber:"<<SeNumber;
  qDebug()<<"PreIndex:"<<PreIndex<<"PreParam:"<<PreParam<<"Section_Number SeNumber"<<Section_Number;
#endif

  if((PreIndex == index)&&(PreParam == Param)&&(Section_Number == SeNumber))
    {
      return;
    }
  PreIndex = index;
  PreParam = Param;
  PreParam_1 = Param_1;
  Section_Number = SeNumber;

  preCurIndex = Interface_Frame_Work::stackedWidget->currentIndex();
  EPS_INFOR->setVisible(false);
  pushButton_Battery->setVisible(false);
  switch(index){
    case MIAN_INTERFACE_VIEW:            //主界面
      GobackButton->setVisible(false);
      MainButton->setVisible(false);
      EPS_INFOR->setVisible(true);
      //pushButton_Battery->setVisible(true);
      break;
    case EPS_MAIN_MENU_VIEW:      //EPS主菜单
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      Eps_Main_Menu_View->eps_meanu_fresh();
      break;
    case CURRENT_ALARM_QUERY_VIEW:     //当前告警
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      break;


    case EPS_RUN_INFORMATION_VIEW:    //EPS运行信息
      Eps_Run_Information_View->tableview_init();
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      break;
    case BATTERYFORM_VIEW:           //电池信息
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      BatteryForm_View->Battery_choice_int();
      BatteryForm_View->battery_int();//每次进入电池信息界面初始化一次

      break;
    case SYSTEMINFOR_VIEW:           //产品信息
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      break;
    case SETMENU_VIEW:               //设置选项
      //每次进入界面时，根据Usr_Eng_Flag的标志来决定显示的内容
      setmenu->OverloadUI();
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      break;
    case ALARM_QUERY_VIEW:           //告警信息
      //每次进入界面时，根据Usr_Eng_Flag的标志来决定显示的内容
      AlarmQuery_View->AlarmOverloadUI();
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      break;
    case DATAE_EPORT_IMPORT_VIEW:   //U盘数据导入导出
      //每次进入界面时，根据Usr_Eng_Flag的标志来决定显示的内容
      Import_Export_Data_View->IEdataloadUI();
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      break;


    case USER_SYS_SET_VIEW:         //监控屏设置
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      Monitor_Screen_Set_View->page1Initialization();
      Monitor_Screen_Set_View->page2Initialization();
      break;
    case USER_SET_VIEW:             //客户设置
      User_Set_View->fresh();
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      break;
    case MAINTAIN_SET:              //维护设置
      Maintain_Set_View->MSdataload();
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      break;


    case MONITOR_SYSTEM_INFOR_VIEW: //产品信息
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      Product_Infor_View->data_fresh();
      break;
    case ABOUTUS_VIEW:              //关于我们
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      break;


    case TEST_DATA_DISPLAY:        //调试界面
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      break;

    case BRANCHALARMSET:
      GobackButton->setVisible(true);
      MainButton->setVisible(true);
      Branchalarmset_View->tablefresh();
      break;

    default:
      GobackButton->setVisible(false);
      MainButton->setVisible(false);
      break;
    }
  popup_keyboard_view.hide();
  password_keyboard.hide();

  stackedWidget->setCurrentIndex(index);
}

/***********************刷新函数各个界面需要刷新的函数************************/
void Interface_Frame_Work::Display_Data_Function()
{

  timer_up.restart();

 /* if(!Issuedequipment_Flag){
      printf("inin\n");

         if(Issuedequipment_parameters_06(DSPSTARTADDR, Monitoring_Software_version_Enum, Globa.Send_Dsp_Debug_Param_Set.Monitoring_Software_version, 1)){
         Issuedequipment_Flag=true;//下发监控版本
       }
     }
*/
  //  int ret1 = timer_up.elapsed();
  //thread_interface_frame=1;
  Alarm_Information_Process();            //弹出报警框的事件处理函数

  if(!mian_interface_View->isHidden())    //主界面
    { if(Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable == 0){
          pushButton_Battery->setVisible(false);
        }else if(Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable!=0){
          pushButton_Battery->setVisible(true);
        }
      display_data_timer->stop();
      if(Restar_SystemFlage == 1)
        {
          Restar_SystemFlage  = 0;
          QMessageBox msgBox;
          msgBox.setWindowTitle(tr("提示"));
          msgBox.setText(tr("参数被修改，需要重启系统生效!"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
          if(msgBox.exec()  == QMessageBox::Ok)
            { LOG("Parameter changed, reboot");
              Reboot Reboot_View;
              Reboot_View.exec();
              //system("reboot");
            }
        }
      mian_interface_View->Screen_Refresh_Data();
      //      int ret2 = timer_up.elapsed();
      display_data_timer->start(1500);
      //      qDebug()<<"------- mian_interface_View"<<ret2 - ret1;
    }
  else if(!Eps_Run_Information_View->isHidden())  //EPS运行信息
    {
      display_data_timer->stop();
      Eps_Run_Information_View->Eps__Run_Refresh_Data();
      display_data_timer->start(1500);
      qDebug()<<"------- mian_interface_View";
    }
  else if(!Current_Alarm_Query_View->isHidden()) //当前告警
    {
      display_data_timer->stop();
      Current_Alarm_Query_View->dislpay_alarm_data();
      display_data_timer->start(1500);
    }
  else if(!AlarmQuery_View->isHidden())       //告警信息
    {
      display_data_timer->stop();
      AlarmQuery_View->dislpay_alarm_data();
      display_data_timer->start(1500);
    }
  else if(!BatteryForm_View->isHidden())      //电池信息
    {
      display_data_timer->stop();
      BatteryForm_View->timerUpdata();
      display_data_timer->start(1500);
    }

  else if(!TestData_Display_View->isHidden())  //调试界面
    {
      display_data_timer->stop();
      TestData_Display_View->data_fresh();
      display_data_timer->start(1500);

    }
  else if(!Maintain_Set_View->isHidden())      //维护设置
    {
      display_data_timer->stop();
      Maintain_Set_View->Adjustment_Dislpay_Data();
      display_data_timer->start(1500);
    }

  else if(!User_Set_View->isHidden())      //用户设置
    {
      display_data_timer->stop();
    //  if(Load_Stye_Change!=0){
       //   User_Set_View->fresh();
       // }
    //  User_Set_View->mon_year_lable_fresh();
      display_data_timer->start(1500);
    }
  else if(!Branchalarmset_View->isHidden())      //用户设置
    {
      display_data_timer->stop();
      display_data_timer->start(1500);
    }


  //
  else
    {
      return;
    }
}



//弹出报警框的事件处理函数
void Interface_Frame_Work::Alarm_Information_Process()
{

  /*for(int t=0;t<current_alarm_eventlog.count();t++){
      if(current_alarm_eventlog.at(t).name == QString(tr("电池开路"))){
          if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status] != 1){
                battery_off_flag=1;
                break;
            }

    }
    battery_off_flag=0;
  }*/
  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Strong_Start_Mode] == 1){
    if(messageFlag[0] == 1){
      screen_brightness_count = 0;
      BK_CON_STRUCT  msg;
      msg.msg_type = 1;
      msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
      msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
      screensaverflag = true;
      msgBox_StrongStartingMode.setWindowTitle(tr("警告"));
      msgBox_StrongStartingMode.setText(tr("强启模式!"));
      //msgBox_StrongStartingMode.setIcon(QMessageBox::Information);
      //msgBox_StrongStartingMode.setStandardButtons(QMessageBox::Ok);
      msgBox_StrongStartingMode.show();
      messageFlag[0] = 0;
      Insert_Operation_Work_Record(1, "强启模式开启");
    }
  }else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Strong_Start_Mode] == 0){
    messageFlag[0] = 1;
  }


  if( Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Manual_Mode] == 1 ){
    if(messageFlag[1] == 1){
      screen_brightness_count = 0;
      BK_CON_STRUCT  msg;
      msg.msg_type = 1;
      msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
      msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
      screensaverflag = true;
      msgBox_ManualMode.setWindowTitle(tr("警告"));
      msgBox_ManualMode.setText(tr("    手动模式开启!       \n\n"));
      msgBox_ManualMode.setIcon(QMessageBox::Information);
      msgBox_ManualMode.setStandardButtons(QMessageBox::Yes|QMessageBox::No);

      if(msgBox_ManualMode.exec() == QMessageBox::Yes){
        if(Issuedequipment_parameters_06(DSPSTARTADDR, Manual_Mode_Determine_Enum, 0xFF, 1)){
          ZMessage msgBox;
          //QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("切换至手动模式!"));

          //msgBox.setIcon(QMessageBox::Information);
          //msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
          Insert_Operation_Work_Record(1, "切换至手动模式");
        }else{
          ZMessage msgBox;
          //QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("设置失败!"));

          //msgBox.setIcon(QMessageBox::Information);
          //msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
        }
      }
      messageFlag[1] = 0;
    }
  }else{
    messageFlag[1] = 1;
  }

  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Automatic_Mode] == 1){
    if(messageFlag[2] == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;
        msgBox_AutomaticMode.setWindowTitle(tr("警告"));
        msgBox_AutomaticMode.setText(tr("自动模式开启!"));

        //msgBox_AutomaticMode.setIcon(QMessageBox::Information);
        //msgBox_AutomaticMode.setStandardButtons(QMessageBox::Ok);
        msgBox_AutomaticMode.exec();
       messageFlag[2] = 0;
        Insert_Operation_Work_Record(1, "自动模式开启");
    }
  }else{
    messageFlag[2] = 1;
  }


  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Simulation_Mains_Failure_Test] == 1 ){
    if(messageFlag[3] == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;
        msgBox_BatteryTestMode.setWindowTitle(tr("警告"));
        msgBox_BatteryTestMode.setText(tr("模拟主电故障测试!"));

        //msgBox_BatteryTestMode.setIcon(QMessageBox::Information);
        //msgBox_BatteryTestMode.setStandardButtons(QMessageBox::Ok);
        msgBox_BatteryTestMode.exec();
        messageFlag[3] = 0;
        Insert_Operation_Work_Record(1, "模拟主电故障测试");
    }
  }else if( Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Simulation_Mains_Failure_Test] == 0 ){
    messageFlag[3] = 1;
  }

  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Reset] == 1 ){
    if(messageFlag[4] == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;
         msgBox_BatteryTestMode.setWindowTitle(tr("警告"));
         msgBox_BatteryTestMode.setText(tr("复位"));

        //msgBox_BatteryTestMode.setIcon(QMessageBox::Information);
        //msgBox_BatteryTestMode.setStandardButtons(QMessageBox::Ok);
        msgBox_BatteryTestMode.exec();
       messageFlag[4] = 0;
       Insert_Operation_Work_Record(1, "复位");
    }
  }else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Reset]== 0 ){
    messageFlag[4] = 1;
  }

  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Remote_Start] == 1 ){
    if(messageFlag[5] == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;
       msgBox_BatteryTestMode.setWindowTitle(tr("警告"));
       if(Globa.Send_Dsp_Debug_Param_Set.Input_Control_1==0){
           msgBox_BatteryTestMode.setText(tr("远程开机"));
           Insert_Operation_Work_Record(1, "远程开机");
           msgBox_BatteryTestMode.exec();
         }
       else{
          /* msgBox_BatteryTestMode.setText(tr("远程转应急"));
           Insert_Operation_Work_Record(1, "远程转应急");*/
         }
       // msgBox_BatteryTestMode.setIcon(QMessageBox::Information);
       //msgBox_BatteryTestMode.setStandardButtons(QMessageBox::Ok);

        messageFlag[5] = 0;

    }
  }else if( Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Remote_Start] == 0 ){
    messageFlag[5] = 1;
  }

  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Remote_Shutdown] == 1 ){
    if(messageFlag[6] == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;

      if(Globa.Send_Dsp_Debug_Param_Set.Input_Control_2==0){
          msgBox_BatteryTestMode.setWindowTitle(tr("警告"));
          msgBox_BatteryTestMode.setText(tr("远程转应急"));
          Insert_Operation_Work_Record(1, "远程转应急");
          msgBox_BatteryTestMode.exec();
        }else if(Globa.Send_Dsp_Debug_Param_Set.Input_Control_2==1){
          msgBox_BatteryTestMode.setWindowTitle(tr("警告"));
          msgBox_BatteryTestMode.setText(tr("远程关机"));
          Insert_Operation_Work_Record(1, "远程关机");
          msgBox_BatteryTestMode.exec();
        }


      //msgBox_BatteryTestMode.setIcon(QMessageBox::Information);
      //msgBox_BatteryTestMode.setStandardButtons(QMessageBox::Ok);

      messageFlag[6] = 0;

    }
  }else if( Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Remote_Shutdown] == 0 ){
    messageFlag[6] = 1;
  }

  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[User_Error] == 1 ){
    if(messageFlag[7] == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;
      msgBox_BatteryTestMode.setWindowTitle(tr("警告"));
      msgBox_BatteryTestMode.setText(tr("用户操作错误"));

      //msgBox_BatteryTestMode.setIcon(QMessageBox::Information);
      //msgBox_BatteryTestMode.setStandardButtons(QMessageBox::Ok);
      msgBox_BatteryTestMode.exec();
      messageFlag[7] = 0;
      Insert_Operation_Work_Record(1, "用户操作错误");
    }
  }else if( Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[User_Error] == 0 ){
    messageFlag[7] = 1;
  }
  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Fire_Linkagel] == 1 ){
    if(messageFlag[8] == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;
        msgBox_BatteryTestMode.setWindowTitle(tr("警告"));
        msgBox_BatteryTestMode.setText(tr("启动消防联动"));

        //msgBox_BatteryTestMode.setIcon(QMessageBox::Information);
        //msgBox_BatteryTestMode.setStandardButtons(QMessageBox::Ok);
        msgBox_BatteryTestMode.exec();
        messageFlag[8] = 0;
        Insert_Operation_Work_Record(1, "启动消防联动");
    }
  }else if( Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Fire_Linkagel] == 0 ){
    messageFlag[8] = 1;
  }
  if((Shutdown_Flag==1)&&(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status]!=1)){
    if(messageFlag[9] == 1){
      screen_brightness_count = 0;
      BK_CON_STRUCT  msg;
      msg.msg_type = 1;
      msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
      msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
      screensaverflag = true;
      //if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status] != 1){


     // if((Shutdown_Flag==1)&&(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[EPS_Run_Status]!=1)){
          msgBox_Notice_Battery_Off.setWindowTitle(tr("警告"));
          msgBox_Notice_Battery_Off.setText(tr("\n若不再使用机器，请断开电池空开 \n"));
          msgBox_Notice_Battery_Off.setIcon(QMessageBox::Warning);
          msgBox_Notice_Battery_Off.setWindowFlags(Qt::FramelessWindowHint);
          msgBox_Notice_Battery_Off.setStandardButtons(QMessageBox::Yes);
          msgBox_Notice_Battery_Off.setButtonText (QMessageBox::Yes,QString("确定"));

          messageFlag[9] = 0;

              if(msgBox_Notice_Battery_Off.exec() == QMessageBox::Yes){
                  Insert_Operation_Work_Record(1, "关闭逆变器，提示用户断开电池空开");

               }

        //}

      messageFlag[9] = 0;
    }
  }else{
    messageFlag[9] = 1;
  }
  //printf("qian\n");
  //printf("Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Check_Battery_Number] is %d\n",Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Check_Battery_Number]);
/*  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Check_Battery_Number] == 1 ){
    if(messageFlag[9] == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;

       // printf("in\n");
        msgBox_Check_Battery_Number.setWindowTitle(tr("警告"));
        msgBox_Check_Battery_Number.setText(tr("请确认连接的电池节数为%1节?").arg(Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number));
        msgBox_Check_Battery_Number.setIcon(QMessageBox::Warning);
        msgBox_Check_Battery_Number.setWindowFlags(Qt::FramelessWindowHint);
        //msgBox_ShutDown.setBackgroundRole();
        msgBox_Check_Battery_Number.setStandardButtons(QMessageBox::Yes);
        msgBox_Check_Battery_Number.setButtonText (QMessageBox::Yes,QString("确定"));


        if(msgBox_Check_Battery_Number.exec() == QMessageBox::Yes){


          if(Issuedequipment_parameters_06(DSPSTARTADDR,Check_Battery_Number_Right_Enum, 0xFF, 1)){

            //Shutdown_Flag=1;
            Insert_Operation_Work_Record(1, tr("确认电池节数为%1节").arg(Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number));
            ZMessage msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("确认电池节数为%1节!").arg(Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number));

            //msgBox.setIcon(QMessageBox::Information);
            //msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
          }
        }


      messageFlag[9] = 0;
   //   Insert_Operation_Work_Record(1, "用户操作错误");
    }
  }else if( Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Check_Battery_Number] == 0 ){
    messageFlag[9] = 1;
  }
*/
  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Check_Battery_Number] == 1 ){
      if(Check_Battery_Number_finish == 0){
          screen_brightness_count = 0;
          BK_CON_STRUCT  msg;
          msg.msg_type = 1;
          msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
          msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
          screensaverflag = true;

         // printf("in\n");
          msgBox_Check_Battery_Number.setWindowTitle(tr("警告"));
          msgBox_Check_Battery_Number.setText(tr("\n请确认连接的电池节数为%1节?\n").arg(Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number));
          msgBox_Check_Battery_Number.setIcon(QMessageBox::Warning);
          msgBox_Check_Battery_Number.setWindowFlags(Qt::FramelessWindowHint);
          //msgBox_ShutDown.setBackgroundRole();
          //msgBox_Check_Battery_Number.setStyleSheet("background-color:rgb(180,0,200)");
          msgBox_Check_Battery_Number.setStandardButtons(QMessageBox::Yes);
          //msgBox_Check_Battery_Number.setFixedSize(300,300);
          //msgBox_Check_Battery_Number.
          //msgBox_Check_Battery_Number.setPalette(QPalette(QPixmap(":/images/infor.png")));
          //msgBox_Check_Battery_Number.setIconPixmap(QPixmap(":/images/infor.png"));
          //msgBox_Check_Battery_Number.setBackgroundRole(QPalette);
          //msgBox_Check_Battery_Number.setStyleSheet("border-image: url(:/images/infor.png");
          //msgBox_Check_Battery_Number.setMinimumSize(200,200);
          //msgBox_Check_Battery_Number.setGeometry(250,120,300,300);
          msgBox_Check_Battery_Number.setButtonText (QMessageBox::Yes,QString("确定"));


          if(msgBox_Check_Battery_Number.exec() == QMessageBox::Yes){


            if(Issuedequipment_parameters_06(DSPSTARTADDR,Check_Battery_Number_Right_Enum, 0xFF, 1)){

              //Shutdown_Flag=1;
              Insert_Operation_Work_Record(1, tr("确认电池节数为%1节").arg(Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number));
              ZMessage msgBox;
              msgBox.setWindowTitle(tr("消息"));
              msgBox.setText(tr("确认电池节数为%1节!").arg(Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number));


              msgBox.exec();
            }
          }


        Check_Battery_Number_finish =1;

      }
    }

  if( Shutdown_Flag_flag == 1 ){
    if(1){

      msgBox_ShutDown.setWindowTitle(tr("警告"));
      msgBox_ShutDown.setText(tr("若确认关机，当市电异常时，有断电风险!"));
      msgBox_ShutDown.setIcon(QMessageBox::Warning);
      msgBox_ShutDown.setStandardButtons(QMessageBox::Yes|QMessageBox::No);


      if(msgBox_ShutDown.exec() == QMessageBox::Yes){


        if(Issuedequipment_parameters_06(DSPSTARTADDR,Manual_Open_Shut_Down_Enum, 0x0F, 1)){

          //Shutdown_Flag=1;
          Globa.Send_Dsp_Debug_Param_Set.Manual_Open_Shut_Down=0x0F;
          System_Set_DSP_Parame_save();
          Insert_Operation_Work_Record(1, "手动关机");
          /*modbus_pc03_mutex.lockForWrite();
          Globa.modbus_485_PC03[INVERTER_OFF_COMMAND]=1;
          Globa.modbus_485_PC03[INVERTER_ON_COMMAND]=0;
          modbus_pc03_mutex.unlock();*/
          /*ZMessage msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("设置成功!"));
          msgBox.exec();*/
        }else{
          /*QMessageBox msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("       设置失败!           \n\n"));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();*/
            ZMessage msgBox;
            msgBox.setWindowTitle(tr("消息"));
            msgBox.setText(tr("设置失败!"));

            msgBox.exec();
        }
      }
     // messageFlag[8] = 0;
      Shutdown_Flag_flag=0;


    }
  }else{
   // messageFlag[8] = 1;
  }
 /* if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_STATE_ALARM[Check_Battery_Number] == 1 ){
    if(1){

      msgBox_ShutDown.setWindowTitle(tr("警告"));
      msgBox_ShutDown.setText(tr("请确认电池节数是否正确?"));
      msgBox_ShutDown.setIcon(QMessageBox::Warning);
      msgBox_ShutDown.setStandardButtons(QMessageBox::Yes);


      if(msgBox_ShutDown.exec() == QMessageBox::Yes){


        if(Issuedequipment_parameters_06(DSPSTARTADDR,Check_Battery_Number_Right_Enum, 0xFF, 1)){

          //Shutdown_Flag=1;
          Insert_Operation_Work_Record(1, "确认电池节数正确");
          ZMessage msgBox;
          msgBox.setWindowTitle(tr("消息"));
          msgBox.setText(tr("确认电池节数正确!"));

          //msgBox.setIcon(QMessageBox::Information);
          //msgBox.setStandardButtons(QMessageBox::Ok);

          msgBox.exec();
        }
      }
     // messageFlag[8] = 0;
      Shutdown_Flag_flag=0;


    }
  }else{
   // messageFlag[8] = 1;
  }
*/


  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status] == 2){
    if(Mon_CheckResultShow_Flag == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;
      msgBox_monthcheck.setWindowTitle(tr("警告"));
      msgBox_monthcheck.setText(tr("月检不通过!"));
      //msgBox_monthcheck.setIcon(QMessageBox::Information);
      //msgBox_monthcheck.setStandardButtons(QMessageBox::Ok);
      msgBox_monthcheck.exec();
      Mon_CheckResultShow_Flag = 0;
      Insert_Operation_Work_Record(1, "月检不通过");
      lb_status->setText(tr(""));
    }
  }
  else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status] == 3){
    if(Mon_CheckResultShow_Flag == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;
      msgBox_monthcheck.setWindowTitle(tr("警告"));
      msgBox_monthcheck.setText(tr("月检通过!"));
      //msgBox_monthcheck.setIcon(QMessageBox::Information);
      //msgBox_monthcheck.setStandardButtons(QMessageBox::Ok);
      msgBox_monthcheck.exec();
      Mon_CheckResultShow_Flag = 0;
      Insert_Operation_Work_Record(1, "月检通过");
      lb_status->setText(tr(""));
    }
  }else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status] == 1){
    lb_status->setText(tr("月检进行中"));
  }

  if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status] == 2){
    if(year_CheckResultShow_Flag == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;
       msgBox_yearcheck.setWindowTitle(tr("警告"));
       msgBox_yearcheck.setText(tr("年检不通过!"));
       //msgBox_yearcheck.setIcon(QMessageBox::Information);
       //msgBox_yearcheck.setStandardButtons(QMessageBox::Ok);
       msgBox_yearcheck.exec();
       year_CheckResultShow_Flag = 0;
       Insert_Operation_Work_Record(1, "年检不通过");
       lb_status->setText(tr(""));
    }
  }else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status] == 3){
    if(year_CheckResultShow_Flag == 1){
        screen_brightness_count = 0;
        BK_CON_STRUCT  msg;
        msg.msg_type = 1;
        msg.msg_text[0] = Globa.User_Monitor_Screen_Set.screen_brightness;
        msgsnd(Globa.bk_send_msg_id, &msg, 1, IPC_NOWAIT); // 减少校验码 （消息队列发送消息时候）包括了通道号，数据
        screensaverflag = true;
       msgBox_yearcheck.setWindowTitle(tr("警告"));
       msgBox_yearcheck.setText(tr("年检通过!"));
       //msgBox_yearcheck.setIcon(QMessageBox::Information);
       //msgBox_yearcheck.setStandardButtons(QMessageBox::Ok);
       msgBox_yearcheck.exec();
       year_CheckResultShow_Flag = 0;
       Insert_Operation_Work_Record(1, "年检通过");
       lb_status->setText(tr(""));
    }
  }else if(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status] == 1){
    lb_status->setText(tr("年检进行中"));
  }

 /* if((Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Month_Check_Status] == 0)&&(Globa.Eps_Param_Data.Dsp_Data_Param.DSP_ANALOG_DATA[Year_Check_Status] == 0)){
    lb_status->setText(tr(""));
  }*/
}

/***********************读取历史告警数据库到历史链表中*********************/
void Interface_Frame_Work::Read_history_alarm_Database_to_List()
{
  HISTORY_ALARMFAULT_EVENT ls_history_alarm;
  QFileInfo info(HISTORY_ALARM_DB);
  bool exis_flag =info.exists();
  {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Read_history");
    db.setDatabaseName(HISTORY_ALARM_DB);
    if(db.open()){
        QSqlQuery  query(db);
        if(!exis_flag){
            query.exec("create table history_Alarm_Eventlog(name TEXT, start_time TEXT, end_time TEXT)");
            query.clear();
            db.close();
            return ;
          }
        query.exec("select * from history_Alarm_Eventlog");
        while(query.next()){
          ls_history_alarm.name = query.value(0).toString();
          ls_history_alarm.start_time = query.value(1).toString();
          ls_history_alarm.end_time = query.value(2).toString();

          history_alarm_tablelocks_mutex.lockForWrite();
          history_alarm_eventlog.insert(0,ls_history_alarm);
          history_alarm_tablelocks_mutex.unlock();
        }
        query.clear();
        db.close();
      }
  }
  QSqlDatabase::removeDatabase("Read_history");
}


/***********************读取历史故障数据库到历史链表中*********************/
void Interface_Frame_Work::Read_history_fault_Database_to_List()
{
  HISTORY_ALARMFAULT_EVENT ls_history_fault;
  QFileInfo info(HISTORY_FAULT_DB);
  bool exis_flag =info.exists();
  {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Read_history_fault");
    db.setDatabaseName(HISTORY_FAULT_DB);
    if(db.open()){
        QSqlQuery  query(db);
        if(!exis_flag){
            query.exec("create table history_Fault_Eventlog(name TEXT, start_time TEXT, end_time TEXT)");
            query.clear();
            db.close();
            return ;
          }
        query.exec("select * from history_Fault_Eventlog");
        while(query.next()){
            ls_history_fault.name = query.value(0).toString();
            ls_history_fault.start_time = query.value(1).toString();
            ls_history_fault.end_time = query.value(2).toString();

            history_fault_tablelocks_mutex.lockForWrite();
            history_fault_eventlog.insert(0,ls_history_fault);
            history_fault_tablelocks_mutex.unlock();
          }
        query.clear();
        db.close();
      }
  }
  QSqlDatabase::removeDatabase("Read_history_fault");
}


/***********************读取历史插入操作记录以及工作记录到历史链表中*********************/
void Interface_Frame_Work::Read_history_operationwork_records()
{
  OPERATION_WORK_RECORD ls_history_Record;
  bool exis_flag = false;
  QFileInfo info(HISTORY_OPERATION_RECORD_DB);
  exis_flag =info.exists();
  {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Read_history_operation");
    db.setDatabaseName(HISTORY_OPERATION_RECORD_DB);
    if(db.open()){
      QSqlQuery  query(db);
      if(!exis_flag){
        query.exec("create table history_Operation_Record(name TEXT, time TEXT)");
        query.clear();
        db.close();

        return ;
      }
      query.exec("select * from history_Operation_Record");
      while(query.next()){
        ls_history_Record.name = query.value(0).toString();
        ls_history_Record.time = query.value(1).toString();
        history_Operation_Record.insert(0,ls_history_Record);
      }
      query.clear();
      db.close();
    }
  }
  QSqlDatabase::removeDatabase("Read_history_operation");


  QFileInfo info1(HISTORY_EPSWORK_RECORD_DB);
  exis_flag =info1.exists();
  {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Read_history_EpsWork");
    db.setDatabaseName(HISTORY_EPSWORK_RECORD_DB);
    if(db.open())
      {
        QSqlQuery  query(db);
        if(!exis_flag)
          {
            query.exec("create table history_EpsWork_Record(name TEXT, time TEXT)");
            query.clear();
            db.close();

            return ;
          }
        query.exec("select * from history_EpsWork_Record");
        while(query.next())
          {
            ls_history_Record.name = query.value(0).toString();
            ls_history_Record.time = query.value(1).toString();
            history_EpsWork_Record.insert(0,ls_history_Record);
          }
        query.clear();
        db.close();
      }
  }
  QSqlDatabase::removeDatabase("Read_history_EpsWork");

}

