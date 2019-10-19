/******************************************************
 *  初始化链表空间和文件的读写操作，初始化各个设备文件
 *
 *

 *
 **********************************************************/

#include"param.h"

#include"file.h"


INT32 ttyled_fd = -1;

INT32 Fd_Uart_Arm_Pc = -1;
INT32 Fd_Uart_Arm_M4 = -1;

GLOBAVAL Globa; //全局的数据结构体
Sigle_Baterry_Voltage_Alarm G_Sigle_Baterry_Voltage_Alarm;

int Usr_Engir_Flag;
int Super_Egir_Flag=0;
int Mon_CheckResultShow_Flag = 1;
int year_CheckResultShow_Flag = 1;
int Battery_Checker_Num_Change_Flag = 0;


extern QReadWriteLock  pp_mutex;
QReadWriteLock  pp_mutex;
void LOG(const char* ms, ... )
{
    pp_mutex.lockForWrite();
    char wzLog[512] = {0};
    char buffer[512] = {0};
    va_list args;
    va_start(args, ms);
    vsprintf( wzLog ,ms,args);
    va_end(args);

    time_t now;
    time(&now);
    struct tm *local;
    local = localtime(&now);
    printf("%04d-%02d-%02d %02d:%02d:%02d %s\n", local->tm_year+1900, local->tm_mon+1,
            local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec,
            wzLog);
    sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d %s\n", local->tm_year+1900, local->tm_mon+1,
                local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec,
                wzLog);
    FILE* file = fopen("/mnt/Nand1/system.log","a+");
    fwrite(buffer,1,strlen(buffer),file);
    fclose(file);
 pp_mutex.unlock();
//  syslog(LOG_INFO,wzLog);
    //return ;
}



unsigned int GetFileSize(char *filename)
{
unsigned int siz = 0;
FILE *fp = fopen(filename, "rb");
if (fp) {
fseek(fp, 0, SEEK_END);
siz = ftell(fp);
fclose(fp);
}
return siz;
}

void signal_SIGSEGV_deal(int sig)
{
LOG("OUCH11111111! - Ia got signal_SIGSEGV_deal %d\n", sig);
signal(SIGSEGV, SIG_IGN);
//system("reboot");//zzc428

//exit(0);// system("reboot");//zzc428


}
void signal_SIGBUS_deal(int sig)
{
LOG("OUCH22222222! - I got signal_SIGBUS_deal %d\n", sig);
signal(SIGBUS, SIG_IGN);
sleep(1);
exit(0);// system("reboot");
}
void signal_SIGPIPE_deal(int sig)
{
LOG("OUCH33333333! - I got signal_SIGPIPE_deal %d\n", sig);
signal(SIGPIPE, SIG_IGN);
sleep(1);
exit(0);// system("reboot");
}
/*************************************************************
               系统参数设置（用户）
**************************************************************/
extern STATUS System_UseSystem_save(void)
{
  UINT32 ret;
  UINT32 fileLen;

  fileLen = sizeof(Globa.User_Monitor_Screen_Set);

  ret = writeFile((UINT8 *)F_UseSystem,(UINT8 *)&(Globa.User_Monitor_Screen_Set), fileLen, 0, 0);
  if(ERROR16 == ret){
      perror("Save UseSystem: write File error\n");
      return ERROR16;
    }else{
      printf("Save UseSystem:write File success\n");
    }

  ret = copyFile((UINT8 *)F_UseSystem,(UINT8 *) F_BAK_UseSystem);
  if(ERROR16 == ret){
      printf("Save UseSystem:copy File error\n");
    }else{
      printf("Save UseSystem:copy file success\n");
    }
  return OK;
}


static void System_UseSystem_reset(void)  // 初始化内存
{
  Globa.User_Monitor_Screen_Set.System_language = 0;
  Globa.User_Monitor_Screen_Set.AlarmSound = 1;
  Globa.User_Monitor_Screen_Set.Screensavertime = 2;
  Globa.User_Monitor_Screen_Set.screen_brightness = 100;
  Globa.User_Monitor_Screen_Set.backgroundAddress = 1;
  Globa.User_Monitor_Screen_Set.Baudratechoice = 1;
  Globa.User_Monitor_Screen_Set.paritychoice = 2;
  Globa.User_Monitor_Screen_Set.dynamic_static_ipmethod = 1;
  Globa.User_Monitor_Screen_Set.keep[0] = 0;
  Globa.User_Monitor_Screen_Set.keep[1] = 0;
  Globa.User_Monitor_Screen_Set.keep[2] = 0;
  Globa.User_Monitor_Screen_Set.keep[3] = 0;
  Globa.User_Monitor_Screen_Set.keep[4] = 0;
  strcpy(Globa.User_Monitor_Screen_Set.userpassword, "123456");
  strcpy(Globa.User_Monitor_Screen_Set.engineerpassword, "888888");



}

static STATUS System_UseSystem_init(void)
{
  UINT32 ret, ret1;
  UINT32 fileLen;
  memset(&(Globa.User_Monitor_Screen_Set), 0, sizeof(Globa.User_Monitor_Screen_Set)); // 清零
  ret = readWholeFile((UINT8 *)F_UseSystem, (UINT8 *)&(Globa.User_Monitor_Screen_Set), &fileLen);
  if(OK == ret)
    {
      printf("read UseSystem File sucess\n");
      return OK;
    }

  printf("read UseSystem back File \n");
  ret = readWholeFile((UINT8 *)F_BAK_UseSystem, (UINT8 *)&(Globa.User_Monitor_Screen_Set), &fileLen);   // 若读取错误，读取备份文件
  if(OK == ret)
    {
      ret1 = copyFile((UINT8 *)F_BAK_UseSystem, (UINT8 *)F_UseSystem);
      if(ERROR16 == ret1)
        {
          printf("copy UseSystem back File error\n");
        }
      else
        {
          printf("copy UseSystem back File ok\n");
        }
      return OK;
    }
  else
    {
      System_UseSystem_reset();   //备份文件同样出错，按照默认初始化内存
      printf("read UseSystem back File error, auto reset\n");

      if(ERROR16 == System_UseSystem_save())
        {
          printf("ERROR save parameters!\n");
        }
      return OK;
    }
}


/*************************************************************
               下发给DSP参数设置
**************************************************************/
extern STATUS System_Set_DSP_Parame_save(void)
{
  UINT32 ret;
  UINT32 fileLen;

  fileLen = sizeof(Globa.Send_Dsp_Debug_Param_Set);

  ret = writeFile((UINT8 *)F_SET_DSP_PARAMETE,(UINT8 *)&(Globa.Send_Dsp_Debug_Param_Set), fileLen, 0, 0);
  if(ERROR16 == ret){
      perror("Save Set_DSP_Parame: write File error\n");
      return ERROR16;
    }else{
      printf("Save Set_DSP_Parame:write File success\n");
    }

  ret = copyFile((UINT8 *)F_SET_DSP_PARAMETE, (UINT8 *)F_BAK_SET_DSP_PARAMETE);
  if(ERROR16 == ret){
      printf("Save Set_DSP_Parame:copy File error\n");
    }else{
      printf("Save Set_DSP_Parame:copy file success\n");
    }
  return OK;
}

static void System_Set_DSP_Parame_reset(void)  // 初始化内存
{
 /* Globa.Send_Dsp_Debug_Param_Set.SETDATA_Stem_Node_Set[0] = 0;
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_Stem_Node_Set[1] = 0;
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_Stem_Node_Set[2] = 0;
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_Stem_Node_Set[3] = 0;
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_Stem_Node_Set[4] = 0;
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_Stem_Node_Set[5] = 0;
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_Stem_Node_Set[6] = 0;
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_Stem_Node_Set[7] = 0;

  Globa.Send_Dsp_Debug_Param_Set.SETDATA_ParallelNum = 8;   //电池节数, 范围【8~11】，默认10
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_InvWorkMode = 1;    //消防联动模式，1：应急，2：正常，默认应急
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_DisChaVolt = 105;   //电池放电终止电压，范围【10~11】*10 默认10.5V (下发105)
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_BatChaVolt = 142;   //单节电池均充电压，范围【12.2~16.2V】*10 默认14.2V
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_ChaVolt = 138;   //电池浮充电压，范围【11.8~15.8V】*10 默认13.8V
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_ChaCurr = 10;    //电池充电电流，范围【0.05~0.3C】*100 默认0.1C
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_InvWorkTime = 30;  //应急剩余时间限值，范围【10-40】默认30min
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_RackCapacity = 6;  //机架容量设置，范围【1-10K】默认4K 0-4 1-5 2-6
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_VolUpLimit = 20;   //输出电压保护上限，范围【0-40】默认20V
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_VolDownLimit = 20; //输出电压保护下限 ，范围【0-40】默认20V
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_FreqUpLimit = 10;  //频率保护上限，范围【0~4.5Hz】，默认为1Hz    x10
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_FreqDownLimit = 10;  //频率保护下限，范围【0~4.5Hz】，默认为1Hz  x10
  Globa.Send_Dsp_Debug_Param_Set.BattCapcity = 100;  //24~100 AH

  Globa.Send_Dsp_Debug_Param_Set.SETDATA_GridVolt_Califactor = 4096;  //主电电压校准系数，默认3686~4506V。
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_InvVolt_Califactor = 4096;  //应急电压校准系数，默认3686~4506
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_InvCurr_Califactor = 4096;  //应急电流校准系数，默认为3686~4506
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_BatVolt_Califactor = 4096;  //电池电压校准系数，默认为3686~4506
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_BatCurr_Califactor = 4096;  //电池电流校准系数，默认为3686~4506
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_BusVolt_Califactor = 4096;  //母线电压校准系数，默认为3686~4506
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_OutputVolt_Califactor = 4096;  //输出电压校准系数,默认为3686~4506
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_OutputCurr_Califactor = 4096;  //输出电流校准系数,默认为3686~4506
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_InvDcVolt_Califactor = 4096;  //直流分量校准系数，默认为3686~4506

  Globa.Send_Dsp_Debug_Param_Set.SETDATA_TestParamAddress_1 = 0;  //调试变量地址1，0x0000-0xffff,默认0
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_TestParamAddress_2 = 0;  //调试变量地址2，0x0000-0xffff,默认0
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_TestParamAddress_3 = 0;  //调试变量地址3，0x0000-0xffff,默认0
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_TestParam_1 = 0;  //调试参数值1，0~65535，默认0
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_TestParam_2 = 0;  //调试参数值2，0~65535，默认0
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_TestParam_3 = 0;  //调试参数值3，0~65535，默认0
  Globa.Send_Dsp_Debug_Param_Set.SETDATA_TestParam_4 = 0;  //调试参数值4，0~65535，默认0
  */
 Globa.Send_Dsp_Debug_Param_Set.Equipment_Capacity=30;  //0设备容量10~500
 Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Top_limit=2530;//01主电电压上限220-275
 Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Low_limit=1750;//02主电电压下限165-220
 Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Top_limit=525;//03主电频率上限50~55
 Globa.Send_Dsp_Debug_Param_Set.Mains_Frequence_Low_limit=475;//04主电频率上限45~50
 Globa.Send_Dsp_Debug_Param_Set.Battery_Capacity=100;      //05电池容量范围【24~200AH】，默认为100AH

  Globa.Send_Dsp_Debug_Param_Set.Battery_All_Section_Number=30; //06 电池节数, 范围【28~31】，默认30
  Globa.Send_Dsp_Debug_Param_Set.Battery_Discharge_Termination_V=105; //no useful      //07 电池放电终止电压，范围[10.2~10.9V】*10默认10.5V（下发105）
  Globa.Send_Dsp_Debug_Param_Set.EPS_Work_Mode_Selection=0;  //EPS工作模式选择 0--EA-D, 1--FEPS;
  Globa.Send_Dsp_Debug_Param_Set.Fast_Slow_Switching=0;

  Globa.Send_Dsp_Debug_Param_Set.EnableControl=0;
  Globa.Send_Dsp_Debug_Param_Set.EnableControl=Globa.Send_Dsp_Debug_Param_Set.EnableControl|(0<<0);
  Globa.Send_Dsp_Debug_Param_Set.EnableControl=Globa.Send_Dsp_Debug_Param_Set.EnableControl|(0<<1);
  Globa.Send_Dsp_Debug_Param_Set.EnableControl=Globa.Send_Dsp_Debug_Param_Set.EnableControl|(0<<2);
  Globa.Send_Dsp_Debug_Param_Set.EnableControl=Globa.Send_Dsp_Debug_Param_Set.EnableControl|(1<<3);

  printf(" Globa.Send_Dsp_Debug_Param_Set.EnableControl is %d\n", Globa.Send_Dsp_Debug_Param_Set.EnableControl);


  Globa.Send_Dsp_Debug_Param_Set.Input_Control_1=0;
  Globa.Send_Dsp_Debug_Param_Set.Input_Control_2=0;
  Globa.Send_Dsp_Debug_Param_Set.Monitoring_Software_version=0;     //09监控软件版本
  Globa.Send_Dsp_Debug_Param_Set.Manual_Open_Shut_Down=0;         //10应急手动开、关机
  Globa.Send_Dsp_Debug_Param_Set.Manual_Mode_Determine=0;         //11手动模式(监控确认)
  Globa.Send_Dsp_Debug_Param_Set.Month_Check_Value=0;              //12月检命令
  Globa.Send_Dsp_Debug_Param_Set.Year_Check_Value=0;               //13年检命令
  Globa.Send_Dsp_Debug_Param_Set.Clear_Fault=0;                   //14清除故障
  Globa.Send_Dsp_Debug_Param_Set.DSP_Restore_Factory_Set=0;      //15 DSP恢复出厂设置

  Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_1=0;    //16 调试变量地址1，0x0000-0xffff,默认0。
  Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_2=0;    //17 调试变量地址2，0x0000-0xffff,默认0
  Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_3=0;    //18 调试变量地址3，0x0000-0xffff,默认0
  Globa.Send_Dsp_Debug_Param_Set.Debug_Variable_Address_4=0;    //19 调试变量地址4，0x0000-0xffff,默认0
  Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_1=0;     //20 调试参数值1，0~65535，默认0
  Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_2=0;     //21 调试参数值2，0~65535，默认0
  Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_3=0;     //22 调试参数值3，0~65535，默认0
  Globa.Send_Dsp_Debug_Param_Set.Debug_Parameter_Value_4=0;     //23 调试参数值4，0~65535，默认0
  Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number = 1;      //电池组数,


  Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_Calibration_Coefficient=4096;//32 主电电压校准系数A，默认1.0,0.9~1.1放大1000倍存放。
 // Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_B_Calibration_Coefficient=4096;//33 主电电压校准系数A，默认1.0,0.9~1.1放大1000倍存放。
 // Globa.Send_Dsp_Debug_Param_Set.Mains_Voltage_C_Calibration_Coefficient=4096;//34主电电压校准系数A，默认1.0,0.9~1.1放大1000倍存放。

  Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_Calibration_Coefficient=4096;//35 应急A电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
 // Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_B_Calibration_Coefficient=4096;//36 应急B电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
//  Globa.Send_Dsp_Debug_Param_Set.Emergency_Voltage_C_Calibration_Coefficient=4096;//37 应急C电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
  Globa.Send_Dsp_Debug_Param_Set.Emergency_Current_Calibration_Coefficient=4096;//38 应急A电流校准系数.0,0.9~1.1放大1000倍存放。
 // Globa.Send_Dsp_Debug_Param_Set.Emergency_Current_B_Calibration_Coefficient=4096;//39 应急B电流校准系数.0,0.9~1.1放大1000倍存放。
//  Globa.Send_Dsp_Debug_Param_Set.Emergency_Current_C_Calibration_Coefficient=4096;//40 应急C电流校准系数.0,0.9~1.1放大1000倍存放。
  Globa.Send_Dsp_Debug_Param_Set.Battery_Voltage_Calibration_Coefficient=4096;   //41 电池电压校准系数，默认1.0,0.9~1.1放大1000倍存放。
  Globa.Send_Dsp_Debug_Param_Set.Battery_Current_Calibration_Coefficient=4096;   //42 电池电流校准系数，默认1.0,0.9~1.1放大1000倍存放。
  Globa.Send_Dsp_Debug_Param_Set.Bus_Voltage_Calibration_Coefficient=4096;     //43 母线电压校准系数.0,0.9~1.1放大1000倍存放。
  Globa.Send_Dsp_Debug_Param_Set.Ouput_Voltage_Calibration_Coefficient=4096;   //46输出C电压校准系数.0,0.9~1.1放大1000倍存放。
  Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_Calibration_Coefficient=4096;   //47 输出A电流校准系数.0,0.9~1.1放大1000倍存放。


  Globa.Send_Dsp_Debug_Param_Set.Machine_Code[0]=0;
  Globa.Send_Dsp_Debug_Param_Set.Machine_Code[1]=0;
  Globa.Send_Dsp_Debug_Param_Set.Machine_Code[2]=0;
  Globa.Send_Dsp_Debug_Param_Set.Machine_Code[3]=0;
  /*Globa.Send_Dsp_Debug_Param_Set.Ouput_Voltage_A_Calibration_Coefficient=4096;   //44输出A电压校准系数.0,0.9~1.1放大1000倍存放。
  Globa.Send_Dsp_Debug_Param_Set.Ouput_Voltage_B_Calibration_Coefficient=4096;   //45输出B电压校准系数.0,0.9~1.1放大1000倍存放。
  Globa.Send_Dsp_Debug_Param_Set.Ouput_Voltage_C_Calibration_Coefficient=4096;   //46输出C电压校准系数.0,0.9~1.1放大1000倍存放。
  Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_A_Calibration_Coefficient=4096;   //47 输出A电流校准系数.0,0.9~1.1放大1000倍存放。
  Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_B_Calibration_Coefficient=4096;   //48 输出B电流校准系数.0,0.9~1.1放大1000倍存放。
  Globa.Send_Dsp_Debug_Param_Set.Ouput_Current_C_Calibration_Coefficient=4096;   //49 输出C电流校准系数.0,0.9~1.1放大1000倍存放。
*/
}

static STATUS System_Set_DSP_Parame_init(void)
{
  UINT32 ret, ret1;
  UINT32 fileLen;
  memset(&(Globa.Send_Dsp_Debug_Param_Set), 0, sizeof(Globa.Send_Dsp_Debug_Param_Set)); // 清零
  ret = readWholeFile((UINT8 *)F_SET_DSP_PARAMETE, (UINT8 *)&(Globa.Send_Dsp_Debug_Param_Set), &fileLen);
  if(OK == ret){
      printf("read Set_DSP_Parame File sucess\n");
      return OK;
    }

  printf("read Set_DSP_Parame back File \n");
  ret = readWholeFile((UINT8 *)F_BAK_SET_DSP_PARAMETE, (UINT8 *)&(Globa.Send_Dsp_Debug_Param_Set), &fileLen);   // 若读取错误，读取备份文件
  if(OK == ret)
    {
      ret1 = copyFile((UINT8 *)F_BAK_SET_DSP_PARAMETE, (UINT8 *)F_SET_DSP_PARAMETE);
      if(ERROR16 == ret1)
        {
          printf("copy Set_DSP_Parame back File error\n");
        }
      else
        {
          printf("copy Set_DSP_Parame back File ok\n");
        }
      return OK;
    }
  else
    {
      System_Set_DSP_Parame_reset();   //备份文件同样出错，按照默认初始化内存
      printf("read Set_DSP_Parame back File error, auto reset\n");

      if(ERROR16 == System_Set_DSP_Parame_save())
        {
          printf("ERROR save parameters!\n");
        }
      return OK;
    }
}



/**************网络参数设置**************************/
int sConfigure_Net(NET_SETTING *net_set)
{
  INT8 tmp[100] = "";

#if (CORE_EDITION_MODEL == 0) //1808
  if(net_set->assign_method == 1){
      sprintf(tmp, "echo auto lo>/etc/network/interfaces");
      system(tmp);
      sprintf(tmp, "echo iface lo inet loopback>>/etc/network/interfaces");
      system(tmp);
      sprintf(tmp, "echo auto %s>>/etc/network/interfaces",MAC_INTERFACE);
      system(tmp);
      sprintf(tmp, "echo iface %s inet static>>/etc/network/interfaces",MAC_INTERFACE);
      system(tmp);
      sprintf(tmp, "echo address %s>>/etc/network/interfaces", net_set->IP_address);
      system(tmp);
      sprintf(tmp, "echo netmask %s>>/etc/network/interfaces", net_set->msk_address);
      system(tmp);
      sprintf(tmp, "echo gateway %s>>/etc/network/interfaces", net_set->gate_address);
      system(tmp);
    }else{
      sprintf(tmp, "echo auto %s>/etc/network/interfaces",MAC_INTERFACE);//
      system(tmp);
      sprintf(tmp, "echo iface %s inet dhcp>>/etc/network/interfaces",MAC_INTERFACE);
      system(tmp);
    }
  sprintf(tmp, "echo nameserver %s>/etc/resolv.conf", net_set->main_DNS_address);
  system(tmp);
  sprintf(tmp, "echo nameserver %s>>/etc/resolv.conf", net_set->minor_DNS_address);
  system(tmp);
#else
  if(net_set->assign_method == 1){
      sprintf(tmp, "echo export SYSCFG_IFACE0=y>/etc/rc.d/rs.conf");
      system(tmp);
      sprintf(tmp, "echo export INTERFACE0=\"eth0\">>/etc/rc.d/rs.conf");
      system(tmp);
      sprintf(tmp, "echo export IPADDR0=\"%s\">>/etc/rc.d/rs.conf", net_set->IP_address);
      system(tmp);
      sprintf(tmp, "echo export NETMASK0=\"%s\">>/etc/rc.d/rs.conf", net_set->msk_address);
      system(tmp);
      sprintf(tmp, "echo export GATEWAY0=\"%s\">>/etc/rc.d/rs.conf", net_set->gate_address);
      system(tmp);
      sprintf(tmp, "echo export NAMESERVER0=\"%s\">>/etc/rc.d/rs.conf", net_set->main_DNS_address);
      system(tmp);
      sprintf(tmp, "echo export NAMESERVER1=\"%s\">>/etc/rc.d/rs.conf", net_set->minor_DNS_address);
      system(tmp);
    }else{
      sprintf(tmp, "echo export SYSCFG_IFACE0=y>/etc/rc.d/rs.conf");
      system(tmp);
      sprintf(tmp, "echo export INTERFACE0=\"eth0\">>/etc/rc.d/rs.conf");
      system(tmp);
      sprintf(tmp, "echo export IPADDR0=\"%s\">>/etc/rc.d/rs.conf", "dhcp");
      system(tmp);
      sprintf(tmp, "echo export NAMESERVER0=\"%s\">>/etc/rc.d/rs.conf", net_set->main_DNS_address);
      system(tmp);
      sprintf(tmp, "echo export NAMESERVER1=\"%s\">>/etc/rc.d/rs.conf", net_set->minor_DNS_address);
      system(tmp);
    }
#endif
  system("/etc/init.d/networking restart");//重启网卡
  return OK;

}

extern STATUS System_Net_Set_Save(void)
{
  UINT16 ret = 0;
  UINT32 fileLen = 0;
  fileLen = sizeof(Globa.NET_SetParam);

  ret = writeFile((UINT8*)F_SETNET, (UINT8*)&(Globa.NET_SetParam),fileLen, 0, 0);

  if(ERROR16 == ret){
      perror("Save net setting: write File error\n");
      return ERROR16;
    }else{
      printf("Save net setting:write File success\n");
    }

  ret = copyFile((UINT8*)F_SETNET, (UINT8*)F_BAK_SETNET);
  if(ERROR16 == ret){
      printf("Save net setting:copy File error\n");
    }else{
      printf("Save net setting:copy file success\n");
    }
  sConfigure_Net(&Globa.NET_SetParam);
  return OK;
}

static void System_Net_Set_Reset(void)
{

  INT8 tmp[100] = "";

  memcpy(Globa.NET_SetParam.IP_address, "192.168.162.245", 15);
  memcpy(Globa.NET_SetParam.msk_address, "255.255.0.0", 15);
  memcpy(Globa.NET_SetParam.gate_address, "192.168.162.1", 15);
  //memcpy(Globa->NET_SetParam.mac_address,"xx:xx:xx:xx:xx:xx",17);

  memcpy(Globa.NET_SetParam.main_DNS_address, "101.226.4.6", 15);
  memcpy(Globa.NET_SetParam.minor_DNS_address, "114.114.114.114", 15);
  memcpy(Globa.NET_SetParam.cipher,"123456",6);	//
  Globa.NET_SetParam.assign_method = 1;//0 DHCP
#if(CORE_EDITION_MODEL == 0) // 1808
  sprintf(tmp, "echo auto %s>/etc/network/interfaces",MAC_INTERFACE);
  system(tmp);
  sprintf(tmp, "echo iface %s inet dhcp>>/etc/network/interfaces", MAC_INTERFACE);
  system(tmp);
  sprintf(tmp, "echo nameserver %s>/etc/resolv.conf", Globa.NET_SetParam.main_DNS_address);
  system(tmp);
  sprintf(tmp, "echo nameserver %s>>/etc/resolv.conf", Globa.NET_SetParam.minor_DNS_address);
  system(tmp);
#else
  sprintf(tmp, "echo export SYSCFG_IFACE0=y>/etc/rc.d/rs.conf");
  system(tmp);
  sprintf(tmp, "echo export INTERFACE0=\"eth0\">>/etc/rc.d/rs.conf");
  system(tmp);
  sprintf(tmp, "echo export IPADDR0=\"%s\">>/etc/rc.d/rs.conf", "dhcp");
  system(tmp);
  sprintf(tmp, "echo export NAMESERVER0=\"%s\">>/etc/rc.d/rs.conf", Globa.NET_SetParam.main_DNS_address);
  system(tmp);
  sprintf(tmp, "echo export NAMESERVER1=\"%s\">>/etc/rc.d/rs.conf", Globa.NET_SetParam.minor_DNS_address);
  system(tmp);
#endif
}

static void System_Net_Set_Judge()
{
  NET_SETTING   ls_NET_SetParam;
  if(Globa.NET_SetParam.assign_method == 0){
      sGet_Net_Mask(ls_NET_SetParam.msk_address);
      sGet_IP_Address(ls_NET_SetParam.IP_address);
      sGet_GateWay(ls_NET_SetParam.gate_address);
      if((strcmp(ls_NET_SetParam.msk_address, Globa.NET_SetParam.msk_address)!=0)||
         (strcmp(ls_NET_SetParam.IP_address, Globa.NET_SetParam.IP_address)!=0)||
         (strcmp(ls_NET_SetParam.gate_address, Globa.NET_SetParam.gate_address)!=0)){

          strcpy(Globa.NET_SetParam.IP_address, ls_NET_SetParam.IP_address);
          strcpy(Globa.NET_SetParam.gate_address, ls_NET_SetParam.gate_address);
          strcpy(Globa.NET_SetParam.msk_address, ls_NET_SetParam.msk_address);

          System_Net_Set_Save();
        }
    }
}

STATUS System_Net_Set_Init(void)
{
  UINT16 ret = 0, ret1 = 0;
  UINT32 fileLen = 0;
  memset(&(Globa.NET_SetParam), 0, sizeof(Globa.NET_SetParam));
  ret = readWholeFile((UINT8*)F_SETNET, (UINT8 *)&(Globa.NET_SetParam), &fileLen);
  if(OK == ret){
      printf("read net setting File sucess\n");
      System_Net_Set_Judge();
      sConfigure_Net(&Globa.NET_SetParam);
      system("/etc/init.d/networking restart");//重启网卡
      return OK;
    }

  printf("read net setting back File \n");
  ret = readWholeFile((UINT8*)F_BAK_SETNET, (UINT8 *)&(Globa.NET_SetParam), &fileLen);   /* 若读取错误，读取备份文件 */
  if(OK == ret){
      System_Net_Set_Judge();
      sConfigure_Net(&Globa.NET_SetParam);
      system("/etc/init.d/networking restart");//重启网卡
      ret1 = copyFile((UINT8*)F_BAK_SETNET, (UINT8*)F_SETNET);   /* 拷贝副本到正本 */
      if(ERROR16 == ret1){
          printf("copy net setting back File error\n");
        }else{
          printf("copy net setting back File ok\n");
        }
      return OK;
    }else{
      System_Net_Set_Reset();    /*备份文件同样出错，按照默认初始化内存 */
      printf("read net setting back File error, auto reset\n");
      if(ERROR16 == System_Net_Set_Save())
        printf("ERROR save net setting!\n");
      return OK;
    }
}


/*************************************************************
                     电池参数设置
**************************************************************/
extern STATUS System_Battery_GroupParam_Set_save(void)
{
  UINT32 ret;
  UINT32 fileLen;

  fileLen = sizeof(Globa.Battery_GroupParam_Set);

  ret = writeFile((UINT8 *)F_Battery_GroupParam_Set,(UINT8 *)&(Globa.Battery_GroupParam_Set), fileLen, 0, 0);
  if(ERROR16 == ret){
      perror("Save Battery_GroupParam_Set: write File error\n");
      return ERROR16;
    }else{
      printf("Save Battery_GroupParam_Set:write File success\n");
    }

  ret = copyFile((UINT8 *)F_Battery_GroupParam_Set, (UINT8 *)F_BAK_Battery_GroupParam_Set);
  if(ERROR16 == ret){
      printf("Save Battery_GroupParam_Set:copy File error\n");
    }else{
      printf("Save Battery_GroupParam_Set:copy file success\n");
    }
  return OK;
}

static void System_Battery_GroupParam_Set_reset(void)  // 初始化内存
{
  //Globa.Send_Dsp_Debug_Param_Set.Battery_Group_Number = 1;      //电池组数,
  Globa.Battery_GroupParam_Set.Battery_GROUP_Before_Disable=1;
  //Globa.Battery_GroupParam_Set.Battery_Capacity = 100;      //电池容量,
 // Globa.Send_Dsp_Debug_Param_Set.Battery_Discharge_Termination_V = 105;      //电池放电总止电压
  Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[0] = 1;
  Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[1] = 2;
  Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[2] = 3;
  Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[3] = 4;
  Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[4] = 5;
  Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[5] = 6;
  Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[6] = 7;
  Globa.Battery_GroupParam_Set.SETDATA_Stem_Node_Set[7] = 8;
  Globa.Battery_GroupParam_Set.userpassword_enable = 0;
  Globa.Battery_GroupParam_Set.battery_detecting_disable=1;//0开启

  Globa.Battery_GroupParam_Set.keep[0] = 0;
  Globa.Battery_GroupParam_Set.keep[1] = 0;
  Globa.Battery_GroupParam_Set.keep[2] = 0;
  Globa.Battery_GroupParam_Set.keep[3] = 0;
  Globa.Battery_GroupParam_Set.keep[4] = 0;
}

static STATUS System_Battery_GroupParam_Set_init(void)
{
  UINT32 ret, ret1;
  UINT32 fileLen;
  memset(&(Globa.Battery_GroupParam_Set), 0, sizeof(Globa.Battery_GroupParam_Set)); // 清零

  ret = readWholeFile((UINT8 *)F_Battery_GroupParam_Set, (UINT8 *)&(Globa.Battery_GroupParam_Set), &fileLen);


  if(OK == ret){
      printf("read Battery_GroupParam_Set File sucess\n");
      return OK;
    }

  printf("read Battery_GroupParam_Set back File \n");
  ret = readWholeFile((UINT8 *)F_BAK_Battery_GroupParam_Set, (UINT8 *)&(Globa.Battery_GroupParam_Set), &fileLen);   // 若读取错误，读取备份文件
  if(OK == ret){
      ret1 = copyFile((UINT8 *)F_BAK_Battery_GroupParam_Set, (UINT8 *)F_Battery_GroupParam_Set);
      if(ERROR16 == ret1){
          printf("copy Battery_GroupParam_Set back File error\n");
        }else{
          printf("copy Battery_GroupParam_Set back File ok\n");
        }
      return OK;
    }else{
      System_Battery_GroupParam_Set_reset();   //备份文件同样出错，按照默认初始化内存
      printf("read Battery_GroupParam_Set back File error, auto reset\n");

      if(ERROR16 == System_Battery_GroupParam_Set_save()){
          printf("ERROR save parameters!\n");
      }
      return OK;
    }

}


extern STATUS System_Sigle_Baterry_Voltage_Alarm_Para_save(void)
{
  UINT32 ret;
  UINT32 fileLen;

  fileLen = sizeof(G_Sigle_Baterry_Voltage_Alarm);

  ret = writeFile((UINT8 *)F_Sigle_Baterry_Voltage_Alarm_Para,(UINT8 *)&(G_Sigle_Baterry_Voltage_Alarm), fileLen, 0, 0);
  if(ERROR16 == ret){
      perror("Save G_Sigle_Baterry_Voltage_Alarm: write File error\n");
      return ERROR16;
    }else{
      printf("Save G_Sigle_Baterry_Voltage_Alarm:write File success\n");
    }

  ret = copyFile((UINT8 *)F_Sigle_Baterry_Voltage_Alarm_Para, (UINT8 *)F_BAK_Sigle_Baterry_Voltage_Alarm_Para);
  if(ERROR16 == ret){
      printf("Save G_Sigle_Baterry_Voltage_Alarm:copy File error\n");
    }else{
      printf("Save G_Sigle_Baterry_Voltage_Alarmt:copy file success\n");
    }
  return OK;
}

static void System_Sigle_Baterry_Voltage_Alarm_Para_reset(void)  // 初始化内存
{
  G_Sigle_Baterry_Voltage_Alarm.OverVolAlarm=145;
  G_Sigle_Baterry_Voltage_Alarm.UnderVolAlarm=Globa.Send_Dsp_Debug_Param_Set.Battery_Discharge_Termination_V+5;
}

static STATUS System_Sigle_Baterry_Voltage_Alarm_Para_init(void)
{
  UINT32 ret, ret1;
  UINT32 fileLen;
  memset(&(G_Sigle_Baterry_Voltage_Alarm), 0, sizeof(G_Sigle_Baterry_Voltage_Alarm)); // 清零
  ret = readWholeFile((UINT8 *)F_Sigle_Baterry_Voltage_Alarm_Para, (UINT8 *)&(G_Sigle_Baterry_Voltage_Alarm), &fileLen);
  if(OK == ret){
      printf("read G_Sigle_Baterry_Voltage_Alarm File sucess\n");
      return OK;
    }

  printf("read G_Sigle_Baterry_Voltage_Alarm back File \n");
  ret = readWholeFile((UINT8 *)F_BAK_Sigle_Baterry_Voltage_Alarm_Para, (UINT8 *)&(G_Sigle_Baterry_Voltage_Alarm), &fileLen);   // 若读取错误，读取备份文件
  if(OK == ret){
      ret1 = copyFile((UINT8 *)F_BAK_Sigle_Baterry_Voltage_Alarm_Para, (UINT8 *)F_Sigle_Baterry_Voltage_Alarm_Para);
      if(ERROR16 == ret1){
          printf("copy G_Sigle_Baterry_Voltage_Alarm back File error\n");
        }else{
          printf("copy G_Sigle_Baterry_Voltage_Alarm back File ok\n");
        }
      return OK;
    }else{
      System_Sigle_Baterry_Voltage_Alarm_Para_reset();   //备份文件同样出错，按照默认初始化内存
      printf("read G_Sigle_Baterry_Voltage_Alarm back File error, auto reset\n");

      if(ERROR16 == System_Sigle_Baterry_Voltage_Alarm_Para_save()){
          printf("ERROR save parameters!\n");
      }
      return OK;
    }
}



extern STATUS System_SysAppend_Set_save(void)
{
  UINT32 ret;
  UINT32 fileLen;

  fileLen = sizeof(SYSAPPENDPARAM);

  ret = writeFile((UINT8 *)F_SysAppend_Set,(UINT8 *)&(Globa.Sysappendparam), fileLen, 0, 0);
  if(ERROR16 == ret){
      perror("Save SysAppend_Sett: write File error\n");
      return ERROR16;
    }else{
      printf("Save SysAppend_Set:write File success\n");
    }

  ret = copyFile((UINT8 *)F_SysAppend_Set, (UINT8 *)F_BAK_SysAppend_Set);
  if(ERROR16 == ret){
      printf("Save SysAppend_Set:copy File error\n");
    }else{
      printf("Save SysAppend_Sett:copy file success\n");
    }
  return OK;
}

static void System_SysAppend_Set_reset(void)  // 初始化内存
{
  memset(&Globa.Sysappendparam.Switch_Alarm_Set[0],0,4*64);


}

static STATUS System_SysAppend_Set_init(void)
{
  UINT32 ret, ret1;
  UINT32 fileLen;
  memset(&(Globa.Sysappendparam), 0, sizeof(Globa.Sysappendparam)); // 清零
  ret = readWholeFile((UINT8 *)F_SysAppend_Set, (UINT8 *)&(Globa.Sysappendparam), &fileLen);
  if(OK == ret){
      printf("read SysAppend_Set File sucess\n");
      return OK;
    }

  printf("read BSysAppend_Set back File \n");
  ret = readWholeFile((UINT8 *)F_BAK_SysAppend_Set, (UINT8 *)&(Globa.Sysappendparam), &fileLen);   // 若读取错误，读取备份文件
  if(OK == ret){
      ret1 = copyFile((UINT8 *)F_BAK_SysAppend_Set, (UINT8 *)F_SysAppend_Set);
      if(ERROR16 == ret1){
          printf("copy SysAppend_Set back File error\n");
        }else{
          printf("copy SysAppend_Set back File ok\n");
        }
      return OK;
    }else{
      System_SysAppend_Set_reset();   //备份文件同样出错，按照默认初始化内存
      printf("read SysAppend_Set back File error, auto reset\n");

      if(ERROR16 == System_SysAppend_Set_save()){
          printf("ERROR save parameters!\n");
      }
      return OK;
    }
}





/*************************************************************
               电池参数设置
**************************************************************/
extern STATUS System_Battery_Param_Set_save(void)
{
  UINT32 ret;
  UINT32 fileLen;

  fileLen = BATTERYMAXGROUP*sizeof(BATTERY_PARAMER_SET);

  ret = writeFile((UINT8 *)F_Battery_Param2_Set,(UINT8 *)&(Globa.Battery_Param_Set[0]), fileLen, 0, 0);
  if(ERROR16 == ret){
      perror("Save Battery_Param_Set: write File error\n");
      return ERROR16;
    }else{
      printf("Save Battery_Param_Set:write File success\n");
    }

  ret = copyFile((UINT8 *)F_Battery_Param2_Set, (UINT8 *)F_BAK_Battery_Param2_Set);
  if(ERROR16 == ret){
      printf("Save Battery_Param_Set:copy File error\n");
    }else{
      printf("Save Battery_Param_Set:copy file success\n");
    }
  return OK;
}

static void System_Battery_Param_Set_reset(void)  // 初始化内存
{
 /*原代码 int i = 0;
  for(i= 0;i<BATTERYMAXGROUP;i++){
    Globa.Battery_Param_Set[i].Battery_Number = 2;      //电池节数,
    Globa.Battery_Param_Set[i].Singlenumber[0] = 24;
    Globa.Battery_Param_Set[i].Singlenumber[1] = 6;
    Globa.Battery_Param_Set[i].Singlenumber[2] = 24;
    Globa.Battery_Param_Set[i].Singlenumber[3] = 24;
    Globa.Battery_Param_Set[i].Singlenumber[4] = 24;
    Globa.Battery_Param_Set[i].keep[0] = 0;
    Globa.Battery_Param_Set[i].keep[1] = 0;
    Globa.Battery_Param_Set[i].keep[2] = 0;
    Globa.Battery_Param_Set[i].keep[3] = 0;
    Globa.Battery_Param_Set[i].keep[4] = 0;
  }
*/
  int i = 0;
    for(i= 0;i<BATTERYMAXGROUP;i++){
      Globa.Battery_Param_Set[i].Battery_Number = 2;      //电池节数,
      Globa.Battery_Param_Set[i].Singlenumber[0] = 24;
      Globa.Battery_Param_Set[i].Singlenumber[1] = 24;
     /* Globa.Battery_Param_Set[i].Singlenumber[2] = 24;
      Globa.Battery_Param_Set[i].Singlenumber[3] = 24;*///del by zzc 721
   //   Globa.Battery_Param_Set[i].Singlenumber[4] = 24;
      Globa.Battery_Param_Set[i].keep[0] = 0;
      Globa.Battery_Param_Set[i].keep[1] = 0;
      Globa.Battery_Param_Set[i].keep[2] = 0;
      Globa.Battery_Param_Set[i].keep[3] = 0;
      Globa.Battery_Param_Set[i].keep[4] = 0;
      }
}

static STATUS System_Battery_Param_Set_init(void)
{
  UINT32 ret, ret1;
  UINT32 fileLen;

  memset(&(Globa.Battery_Param_Set[0]), 0, BATTERYMAXGROUP*sizeof(BATTERY_PARAMER_SET)); // 清零
  ret = readWholeFile((UINT8 *)F_Battery_Param2_Set, (UINT8 *)&(Globa.Battery_Param_Set[0]), &fileLen);
  if(OK == ret){
      printf("read Battery_Param_Set File sucess\n");
      return OK;
    }

  printf("read Battery_Param_Set back File \n");
  ret = readWholeFile((UINT8 *)F_BAK_Battery_Param2_Set, (UINT8 *)&(Globa.Battery_Param_Set[0]), &fileLen);   // 若读取错误，读取备份文件
  if(OK == ret){
      ret1 = copyFile((UINT8 *)F_BAK_Battery_Param2_Set, (UINT8 *)F_Battery_Param2_Set);
      if(ERROR16 == ret1){
          printf("copy Battery_Param_Set back File error\n");
        }else{
          printf("copy Battery_Param_Set back File ok\n");
        }
      return OK;
    }else{
      System_Battery_Param_Set_reset();   //备份文件同样出错，按照默认初始化内存
      printf("read Battery_Param_Set back File error, auto reset\n");

      if(ERROR16 == System_Battery_Param_Set_save()){
          printf("ERROR save parameters!\n");
        }
      return OK;
    }
}


/*************************************************************
               客户设置参数
**************************************************************/
extern STATUS System_Curstomer_Param_Set_save(void)
{
  UINT32 ret;
  UINT32 fileLen;

  fileLen = sizeof(Globa.Curstomer_Param_Set);

  ret = writeFile((UINT8 *)F_SET_CUSTOMER_SETUP,(UINT8 *)&(Globa.Curstomer_Param_Set), fileLen, 0, 0);
  if(ERROR16 == ret){
      perror("Save SET_CUSTOMER_SETUP: write File error\n");
      return ERROR16;
    }else{
      printf("Save SET_CUSTOMER_SETUP:write File success\n");
    }

  ret = copyFile((UINT8 *)F_SET_CUSTOMER_SETUP, (UINT8 *)F_BAK_SET_CUSTOMER_SETUP);
  if(ERROR16 == ret){
      printf("Save SET_CUSTOMER_SETUP:copy File error\n");
    }else{
      printf("Save SET_CUSTOMER_SETUP:copy file success\n");
    }
  return OK;
}

static void System_Curstomer_Param_Set_reset(void)  // 初始化内存
{

  //Globa.Curstomer_Param_Set.EPS_Work_Mode_Selection = 2;  //EPS工作模式选择:持续性输出
  Globa.Curstomer_Param_Set.Distribution_Branches_Number = 32;  //配电支路数
  Globa.Curstomer_Param_Set.ATS_Configure_Flage =  0;  //ATS配置标志--0--无ATS 1--有ATS
  Globa.Curstomer_Param_Set.Distribution_Configure_Flage=0;//配电配置标志--0--无配电 1--有配电
  Globa.Curstomer_Param_Set.battertyenable= 1;
  Globa.Curstomer_Param_Set.Distribution_Branches_modelnum=1;
  Globa.Curstomer_Param_Set.keep[0] = 0; //预留;
  Globa.Curstomer_Param_Set.keep[1] = 0; //预留;
  Globa.Curstomer_Param_Set.keep[2] = 0; //预留;
  Globa.Curstomer_Param_Set.keep[3] = 0; //预留;


}

static STATUS System_Curstomer_Param_Set_init(void)
{
  UINT32 ret, ret1;
  UINT32 fileLen;
  memset(&(Globa.Curstomer_Param_Set), 0, sizeof(Globa.Curstomer_Param_Set)); // 清零
  ret = readWholeFile((UINT8 *)F_SET_CUSTOMER_SETUP, (UINT8 *)&(Globa.Curstomer_Param_Set), &fileLen);
  if(OK == ret){
      printf("read SET_CUSTOMER_SETUP File sucess\n");
      return OK;
    }

  printf("read SET_CUSTOMER_SETUP back File \n");
  ret = readWholeFile((UINT8 *)F_BAK_SET_CUSTOMER_SETUP, (UINT8 *)&(Globa.Curstomer_Param_Set), &fileLen);   // 若读取错误，读取备份文件
  if(OK == ret)
    {
      ret1 = copyFile((UINT8 *)F_BAK_SET_CUSTOMER_SETUP, (UINT8 *)F_SET_CUSTOMER_SETUP);
      if(ERROR16 == ret1)
        {
          printf("copy SET_CUSTOMER_SETUP back File error\n");
        }
      else
        {
          printf("copy SET_CUSTOMER_SETUP back File ok\n");
        }
      return OK;
    }
  else
    {
      System_Curstomer_Param_Set_reset();   //备份文件同样出错，按照默认初始化内存
      printf("read SET_CUSTOMER_SETUP back File error, auto reset\n");

      if(ERROR16 == System_Curstomer_Param_Set_save())
        {
          printf("ERROR SET_CUSTOMER_SETUP parameters!\n");
        }
      return OK;
    }
}


/******************创建目录 *******************/
static void Create_Directory()
{
  mkdir(F_PATH_Nand1, 0777);
  mkdir(F_PATH, 0777);
  mkdir(F_INIT, 0777);
  mkdir(F_UPDATE,0777);
  mkdir(F_PARAMPATH, 0777);
  mkdir(F_PARABAKPATH, 0777);
  mkdir(F_DATAPATH, 0777);
}

static void uart_m4_msg_init(void)
{
  if((Globa.uart_m4_msg_sent_id = msgget(IPC_PRIVATE, IPC_CREAT|0666))<0){
      printf("Create uart_m4_msg_sent_id error !\n\r" );
    }

  if((Globa.uart_m4_msg_recv_id = msgget(IPC_PRIVATE, IPC_CREAT|0666))<0){
      printf("Create uart_m4_msg_recv_id error !\n\r" );
    }

  if((Globa.uart_m4_set_msg_recv_id = msgget(IPC_PRIVATE, IPC_CREAT|0666))<0){
      printf("Create uart_arm_msg_recv_id error !\n\r" );
    }
}

static void backlight_control_msg_init(void)
{
  key_t con_sd_id;
  if((con_sd_id = ftok("/home/root/backlight", 'm')) < 0){
      printf("------------Create send_id error!--------------\n");
    }

  if((Globa.bk_send_msg_id = msgget(con_sd_id, IPC_CREAT|0660))<0){
      printf("Create bk_send_msg_id fail!\n");
    }else{
      printf("Create bk_send_msg_id=%d\n",Globa.bk_send_msg_id );
    }
}

/*******************干接点动作*****************************/
static int LedRelayControl_init()
{
  ttyled_fd=open("/dev/dm850_gpios",O_RDWR|O_SYNC);

  if(ttyled_fd==-1){
      perror("can't Open Led_Relay device\n");
      return -1;
    }else{
      printf("Open Led_Relay device  ok!\n");
      return(0);
    }
}
/*******************打开AMR--M4串口*****************/

INT32 UART_ttyS1_M4_init(void)
{
  Fd_Uart_Arm_M4 = open( UART_ARM_TO_M4, O_RDWR | O_NOCTTY|O_NDELAY );
  if(Fd_Uart_Arm_M4 != -1) {
      set_speed(Fd_Uart_Arm_M4, 115200);
      set_Parity(Fd_Uart_Arm_M4, 8, 1, 'N');
      return(0);
    }else{
      perror("open series fail!\n");
      return(-1);
    }

  //close(ttyS1_fd);该文件句柄不用关闭，系统运行时，串口1始终都是打开的;
}
/*******************打开AMR--M4串口*****************/

INT32 UART_ttyS2_PC_init(void)
{
  if(Fd_Uart_Arm_Pc != -1){
      close(Fd_Uart_Arm_Pc);
    }
  Fd_Uart_Arm_Pc = open( UART_ARM_TO_PC, O_RDWR | O_NOCTTY|O_NDELAY );

  if(Fd_Uart_Arm_Pc != -1) {
      if(Globa.User_Monitor_Screen_Set.Baudratechoice == 0){
          set_speed(Fd_Uart_Arm_Pc, 4800);
        }else if(Globa.User_Monitor_Screen_Set.Baudratechoice == 1){
          set_speed(Fd_Uart_Arm_Pc, 9600);
        }else
        set_speed(Fd_Uart_Arm_Pc, 19200);

      if(Globa.User_Monitor_Screen_Set.paritychoice == 0){
          set_Parity(Fd_Uart_Arm_Pc, 8, 1, 'O');
        }else if(Globa.User_Monitor_Screen_Set.paritychoice == 1){
          set_Parity(Fd_Uart_Arm_Pc, 8, 1, 'E');
        }else
        set_Parity(Fd_Uart_Arm_Pc, 8, 1, 'N');

      return(0);
    }else{
      perror("open series fail!\n");
      return(-1);
    }
  //close(ttyS1_fd);该文件句柄不用关闭，系统运行时，串口1始终都是打开的;
}


/****************系统配置文件初始化**************/
void  System_Config_File_Init()
{
  Create_Directory();//创建目录






  struct sigaction sact;
  sact.sa_handler = signal_SIGSEGV_deal;//my_SIGSEGV_deal;//SIG_IGN;my_SIGBUS_deal
  sact.sa_flags = 0;
  if (sigaction(SIGSEGV, &sact, NULL) == 0) {
       printf("SIGSEGV ignore\n\n\n");//DEBUG_FPUTS("my_SIGSEGV_deal init",fdtest);
  }
  sact.sa_handler = signal_SIGBUS_deal;//my_SIGBUS_deal;//SIG_IGN;
  sact.sa_flags = 0;
  if (sigaction(SIGBUS, &sact, NULL) == 0) {
       printf("SIGBUS ignore");//DEBUG_FPUTS("my_SIGBUS_deal init",fdtest);
  }



  memset(&Globa,0,sizeof(Globa));


  System_SysAppend_Set_init();

  System_UseSystem_init();

  System_Battery_GroupParam_Set_init();

  System_Set_DSP_Parame_init();

  //System_Battery_GroupParam_Set_init();
  System_Battery_Param_Set_init();

  System_Net_Set_Init();
  System_Curstomer_Param_Set_init();

  System_Sigle_Baterry_Voltage_Alarm_Para_init();
  LedRelayControl_init();//**初始化IO控制口--*
  backlight_control_msg_init();//背光灯消息队列初始建立
  uart_m4_msg_init();//与M4通信消息队列的建立

  UART_ttyS1_M4_init();//*打开AMR--M4串口*
  UART_ttyS2_PC_init();
 printf("---------System_init---------------\n\n");

}
