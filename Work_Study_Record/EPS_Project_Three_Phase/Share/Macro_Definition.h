#ifndef MACRO_DEFINITION_H
#define MACRO_DEFINITION_H

#include <QThread>
#include <QTimer>
#include <QDateTime>
#include <QtGui>
#include <QtGlobal>
#include <QDebug>
#include <QList>
#include <QDate>
#include <sys/shm.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/ethernet.h>
#include <signal.h>
#include <netinet/ip.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys/types.h>  /*数据类型，比如一些XXX_t的那种*/
#include <sys/stat.h>   /*定义了一些返回值的结构，没看明白*/
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/
#include <sys/time.h>
#include <math.h>
#include <QDebug>
#include <QtSql>

#define CORE_EDITION_MODEL  0  //核心板的型号 0- TI 1808  FRS1286

#define _ARM_
#define V001A    //重庆隧道


#define UART_ARM_TO_PC  "/dev/ttyS2"
#define UART_ARM_TO_M4  "/dev/ttyS1"

#define BATTERYMAXNUMEBR   2
#define BATTERYMAXGROUP    8 //最大组数

#define BATTERYSTARTADDR  130  //电池模块模块起始地址
#define DSPSTARTADDR      1 //DSP通讯地址
#define ATSSTARTADDR      1 //ATS起始地址
#define IOSTARTADDR      64 //开关起始地址


#define reg_04input_nregs 600
#define reg_02input_nregs 600
#define reg_03max_num 300
#define reg_06_03_start_reg 61440
#define reg_06_03_end_reg 61465

#define  MAX_LOST_COMMUNIT_NUM  5         //最大失去响应的次数（超过次数报通信中断故障）

#define  SECTION_NUMBER    2     //馈出支路总共段数

#define  DEBUG_RS232 0
#define  DEBUG_RS232_M4 1
#define  DEBUG_RS232_PC 0
#define  MAX_MODBUS_FRAMESIZE 255
#define  M4_UART_NUM 8

#define  UART_M4_BUFF_NUM 16
#define  battery_temp_display -1000//不显示电池温度参考值
#define   ADMIN_MAX_USERS     		2
#define   ADMIN_NAME_SIZE     		32
#define   ADMIN_PASSWORD_SIZE 		32
#define BACK_ALMOST    0.1//回差点即告警消失时的值*该值为回差 eg:>10A报警，则消失为<10-10*该值
#define MAX_LIST_COUNT  1000  //最大存储1000条历史记录，多余删除最开始的


/*下表为GPIO对应表，要控制哪个PIN仅需要调用函数ioctl(ttyled_fd,value,pin);  其中PIN 为下面对应的0---21, value为0，1。
GPIO驱动控制：DA850_GPIO_RELAY1对应参数（0，0）---（pin,0|1)依此类推。

DA850_GPIO_RELAY1         GPIO_TO_PIN(5, 11)
DA850_GPIO_RELAY2         GPIO_TO_PIN(5, 12)
DA850_GPIO_RELAY3         GPIO_TO_PIN(5, 13)
DA850_GPIO_RELAY4         GPIO_TO_PIN(5, 14)
DA850_GPIO_RELAY5         GPIO_TO_PIN(5, 15)
DA850_GPIO_RELAY6         GPIO_TO_PIN(4, 0)
DA850_GPIO_RELAY7         GPIO_TO_PIN(4, 1)
DA850_GPIO_RELAY8         GPIO_TO_PIN(5, 7)
DA850_GPIO_RELAY9         GPIO_TO_PIN(5, 8)
DA850_GPIO_RELAY10        GPIO_TO_PIN(5, 9)

DA850_GPIO_RST_M4         GPIO_TO_PIN(3, 2)
DA850_GPIO_BUS_FAULT      GPIO_TO_PIN(3, 0)
DA850_GPIO_SYS_FAULT      GPIO_TO_PIN(3, 1)
DA850_GPIO_RD_CTL1        GPIO_TO_PIN(3, 3)
DA850_GPIO_RD_CTL2        GPIO_TO_PIN(3, 4)
DA850_GPIO_RD_CTL3        GPIO_TO_PIN(3, 5)
DA850_GPIO_RD_CTL4        GPIO_TO_PIN(3, 6)
DA850_GPIO_BUZZ           GPIO_TO_PIN(3, 7)

DA850_GPIO_BAT_FUSE_FAULT GPIO_TO_PIN(6, 0)
DA850_GPIO_CHARGER_FAULT        GPIO_TO_PIN(2, 1)
DA850_GPIO_RUN_LED              GPIO_TO_PIN(5, 10)
DA850_GPIO_RD_PC                GPIO_TO_PIN(2, 8)*/
/*Section: Macro Definitions----------------------------------------------------*/
/**********************故障灯亮灭函数*******************************************/
#define MAIN_ELECTRIC_INDICATOR_ON      Led_Relay_Control(20, 1)  //主电指示
#define MAIN_ELECTRIC_INDICATOR_OFF     Led_Relay_Control(20, 0)

#define CHARGE_INDICATOR_ON             Led_Relay_Control(18, 1)  //充电指示
#define CHARGE_INDICATOR_OFF            Led_Relay_Control(18, 0)

#define EMERGENCY_INDICATOR_ON          Led_Relay_Control(19, 1)  //应急指示
#define EMERGENCY_INDICATOR_OFF         Led_Relay_Control(19, 0)

#define STRONG_INSTRUCTION_ON           Led_Relay_Control(11, 1)  //强启指示
#define STRONG_INSTRUCTION_OFF          Led_Relay_Control(11, 0)

#define FAULT_INDICATOR_ON              Led_Relay_Control(12, 1)  //故障指示
#define FAULT_INDICATOR_OFF             Led_Relay_Control(12, 0)


#define GPIO_RST_M4_ON                  Led_Relay_Control(10, 1)
#define GPIO_RST_M4_OFF                 Led_Relay_Control(10, 0)




#define LED_MEET_EMERGENCIES_POWER_ON      Led_Relay_Control(13, 1)   //LED_MEET_EMERGENCIES_POWER_ON
#define LED_MEET_EMERGENCIES_POWER_OFF     Led_Relay_Control(13, 0)   //LED_MEET_EMERGENCIES_POWER_OFF


#define GPIO_RELAY1_ON    Led_Relay_Control(0,1)
#define GPIO_RELAY1_OFF    Led_Relay_Control(0,0)


#define GPIO_SYS_BUZZ_ON      Led_Relay_Control(17, 0)
#define GPIO_SYS_BUZZ_OFF     Led_Relay_Control(17, 1)  //蜂鸣器鸣叫

#define GPIO_RD_PC485_RECEIVE     Led_Relay_Control(21, 1)
#define GPIO_RD_PC485_SEND        Led_Relay_Control(21, 0)

#define GPIO_SYS_RUN_MONITOR     Led_Relay_Control(22, 1)  //


#define MAX(x, y)   (((x) < (y)) ? (y) : (x))

#define MIN(x, y)   (((x) < (y)) ? (x) : (y))

#define BCD2HEX(x) (((x) >> 4) * 10 + ((x) & 0x0F))         /*20H -> 20*/
#define HEX2BCD(x) (((x) % 10) + ((((x) / 10) % 10) << 4))  /*20 -> 20H*/

#define BYTESWAP(x) ((MSB(x) | (LSB(x) << 8)))
#define BITS(x,y) (((x)>>(y))&0x00000001)   /* 判断某位是否为1 */
#define SETBITS(x,y,n) (x) = (n) ? ((x)|(1 << (y))) : ((x) &(~(1 << (y))));
#define SETBITSTO0(x,y) (x) = ((x) &(~(1 << (y))));
#define SETBITSTO1(x,y) (x) = ((x)|(1 << (y)));
#define INVERSE(x,y)    (x) = ((x)^(1<<y));  /* 给某位置反 */

#define GETBIT(x,y) (((x)>>((y)-1))&0x01)

//Big-Endian
#define BigtoLittle16(A) ((((UINT16)(A) & 0xff00) >> 8)|(((UINT16)(A) & 0x00ff) << 8))
#define BigtoLittle32(A) ((((UINT32)(A) & 0xff000000) >> 24)|(((UINT32)(A) & 0x00ff0000) >> 8)|(((UINT32)(A) & 0x0000ff00) << 8)|(((UINT32)(A) & 0x000000ff) << 24))

#define OK 0
#define ERROR8 0xFF
#define ERROR16 0xFFFF
#define ERROR32 0xFFFFFFFF

#define MAC_INTERFACE  "eth0"
#define VOLTAGE_THRESHOLD   500 //电压门限值小于一定值时为断开，
#define YEAR_SELF_CHECK_TIME   365*24*60*60  //年检总时间
#define MONTH_SELF_CHECK_TIME  30*24*60*60  //月检总时间

/*****************参数文件路径*************************************/
#define F_PATH_Nand1        "/mnt/Nand1/"
#define F_PATH              "/mnt/Nand1/ea_app/"
#define F_INIT              "/mnt/Nand1/ea_init/"
#define F_UPDATE            "/mnt/Nand1/update/"
//出厂mac、序列号等不可删除文件夹
/* 数据文件 */
#define F_SYSTEMLOGPATH     "/mnt/Nand1/system.log"
#define F_DATAPATH          "/mnt/Nand1/ea_app/data/"
#define F_PARAMPATH         "/mnt/Nand1/ea_app/param/"              //系统参数文件路径
#define F_PARABAKPATH       "/mnt/Nand1/ea_app/backup/"             //系统参数备份文件路径

#define F_UseSystem            "/mnt/Nand1/ea_app/param/UseSystem"              //系统参数设置*/
#define F_BAK_UseSystem        "/mnt/Nand1/ea_app/backup/UseSystem.bak"         //系统参数设置*/

#define F_SET_DSP_PARAMETE   "/mnt/Nand1/ea_app/param/Set_DSP_Parame"           //下发给DSP参数设置
#define F_BAK_SET_DSP_PARAMETE  "/mnt/Nand1/ea_app/backup/Set_DSP_Parame.bak"   //下发给DSP参数设置份文件

#define F_SETNET             "/mnt/Nand1/ea_app/param/Net_Set"                 //网络设置
#define F_BAK_SETNET         "/mnt/Nand1/ea_app/backup/Net_Set.bak"             //网络设置

#define F_Sigle_Baterry_Voltage_Alarm_Para             "/mnt/Nand1/ea_app/param/Sigle_Baterry_Voltage_Alarm_Para "                 //网络设置
#define F_BAK_Sigle_Baterry_Voltage_Alarm_Para         "/mnt/Nand1/ea_app/backup/Sigle_Baterry_Voltage_Alarm_Para.bak"             //网络设置




#define F_Battery_Param_Set      "/mnt/Nand1/ea_app/param/Battery_Param_Set"   //电池参数设置
#define F_BAK_Battery_Param_Set      "/mnt/Nand1/ea_app/backup/Battery_Param_Set.bak" //电池参数设置

#define F_Battery_Param2_Set      "/mnt/Nand1/ea_app/param/Battery_Param2_Set"   //电池参数设置//add by zzc 721  delete old battery set
#define F_BAK_Battery_Param2_Set      "/mnt/Nand1/ea_app/backup/Battery_Param2_Set.bak" //电池参数设置


#define F_Battery_GroupParam_Set      "/mnt/Nand1/ea_app/param/Battery_GroupParam_Set"   //电池参数设置
#define F_BAK_Battery_GroupParam_Set      "/mnt/Nand1/ea_app/backup/Battery_GroupParam_Set.bak" //电池参数设置


#define F_SET_CUSTOMER_SETUP   "/mnt/Nand1/ea_app/param/Curstomer_Param_Set"           //下发给DSP参数设置
#define F_BAK_SET_CUSTOMER_SETUP  "/mnt/Nand1/ea_app/backup/Curstomer_Param_Set.bak"   //下发给DSP参数设置份文件

#define F_SysAppend_Set      "/mnt/Nand1/ea_app/param/SysAppend_Set"   //
#define F_BAK_SysAppend_Set  "/mnt/Nand1/ea_app/backup/SysAppend_Set.bak" //


//#define F_Stem_Node_Set          "/mnt/Nand1/ea_app/param/Stem_Node_Set"    //干接点选择
//#define F_BAK_Stem_Node_Set      "/mnt/Nand1/ea_app/backup/Stem_Node_Set"    //干接点选择

#define HISTORY_ALARM_DB              "/mnt/Nand1/ea_app/data/history_alarm.db"   /*事件历史告警日志数据库*/
#define HISTORY_FAULT_DB              "/mnt/Nand1/ea_app/data/history_fault.db"   /*事件历史告警日志数据库*/
#define HISTORY_EPSWORK_RECORD_DB     "/mnt/Nand1/ea_app/data/Eps_Work_Record.db"  /*工作记录*/
#define HISTORY_OPERATION_RECORD_DB   "/mnt/Nand1/ea_app/data/Operation_Record.db" /*操作记录*/


//文件相关目录定义
#define NAMENUM 100              //文件名大小
//***********************升级软件文件以及拷贝出来的相关文件*****************************

//#define  APPLIACTION_UP_PATH   "/home/root/DcScreen_Power.tmp"  //---程序升级 替换原来的位置的老程序
//#define  APPLIACTION   "DcScreen_Power"                         //-------程序升级 新程序的位置
#define  SCREENCALIBRATION       "/etc/pointercal"
#define  RESTORE_FACTORY_SETTING "/mnt/Nand1"
#define  SYS_CONFIGURATIONFILE_USB_UP_PATH  "ea_app"            //------配置点表文件
#define  SYS_CONFIGURATIONFILE_UP_PATH     "/mnt/Nand1/ea_app"  //------配置点表文件原来的位置
#define  APPLIACTION_UP_PATH   "/mnt/Nand1/update/DcScreen_Power.tar.gz" //---程序升级 替换原来的位置的老程序

#define  APPLIACTION_USB_UP_PATH   "DcScreen_Power.tar.gz"

typedef char                INT8;
typedef short int           INT16;
typedef int                 INT32;
typedef long long           INT64;

typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned int        UINT32;
typedef unsigned long long  UINT64;

typedef unsigned char       byte;
typedef unsigned char       bits;

typedef unsigned char       uchar;

typedef unsigned short      STATUS;


typedef enum _MODBUS_FRAME_ENUM{
  fun_addr=0,
  fun_fun,
  fun_regh,
  fun_regl,
  fun_lenh,
  fun_lenl,
  fun_crch,
  fun_crcl
}MODBUS_FRAME_ENUM;


static const char maintimetable[2][7][20]={
  {"星期一", "星期二", "星期三", "星期四", "星期五", "星期六","星期日"},
  {" Monday", "Tuesday", "Wednesday", "Thursday"," Friday ", "Saturday", "Sunday"}

};

static const char Statusbranchumberable[2][2][10]={

  {"断开","闭合"},
  {"Off", "Closed"}

};

static const char Statusbranchalarmable[2][2][10]={

  {"告警","不告警"},
  {"Alarm", "Ignore"}

};

static const char Alarmbranchumberable[3][10]={
  "正常","跳闸"," "
};
static const char System_state[2][2][20]={
  {"系统正常","系统故障"},
  {"System normal","System failure"}
};


static const char EPSWorkTable[2][3][20]={
  {"待机","主电","应急"},
  {"standby mode","main electric ","Emergency mode"}
}; //交流状态

#endif // MACRO_DEFINITION_H
