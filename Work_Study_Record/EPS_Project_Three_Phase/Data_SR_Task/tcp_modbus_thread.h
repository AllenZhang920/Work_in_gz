#ifndef TCP_MODBUS_THREAD_H
#define TCP_MODBUS_THREAD_H

#include <QThread>
#include"Share/param.h"

#define MODBUS_TCP_PORT 502      // the port users will be connecting to
#define BACKLOG 20              // how many pending connections queue will hold
#define MAXCLIENT 32              // allow connections number
#define MAX_IDLECONNCTIME 5      // how many idle time connections will be closed,minite
#define DEBUG_MODBUS_TCP  0
#define BUF_SIZE 255
#define FUN_02_REGEST_MAXNUMBER 40
#define FUN_04_REGEST_MAXNUMBER 100
#define FUN_10_REGEST_MAXNUMBER 20
#define COMMS_FAILURE 0
#define ILLEGAL_FUNCTION 1
#define ILLEGAL_DATA_ADDRESS 2
#define ILLEGAL_DATA_VALUE 3
#define SLAVE_DEVICE_FAILURE 4
#define SEVERISBUSY 0x06

class Tcp_Modbus_Thread : public QThread
{
public:
  Tcp_Modbus_Thread();
  void run();
  INT32 TCP_Modbus_Server();
  void Modbus_Fault_Response(UINT32 fd, UINT8 *msg, UINT8 err_num);

  INT32 Battery_Tcp_modbus_Deal(UINT32 fd, UINT8 *msg, UINT32 length);
  //void modbus_fun06_ack(UINT16 start_reg_addr ,UINT16 Input_value);
  INT32 ModbusSetup06(UINT16 regaddress, UINT8 *msg, UINT32 length);
  UINT32 SendSetup(UINT8 adress ,UINT16 Register , UINT16 pdata,UINT8 id);
  void  modbus_func03_ack( UINT16 start_reg_addr, UINT16 reg_num);
 //----modbus报文头格式------------
typedef enum modbus_mbap{
    MBAP_flag,                //事务处理标识符
    MBAP_proto=2,             //协议标识符
    MBAP_length=4,            //长度
    MBAP_unit=6,              //单元标识符
    MBAP_fun                 //功能码
}MODBUS_MBAP;

//----modbus 异常响应格式------------
typedef enum modbus_exmb{
    EXMB_fun=7,                //异常功能码
    EXMB_state                //异常码
}MODBUS_EXMB;

//----modbus 02 读离散量输入格式------------
typedef enum modbus_cmb02{
    CMB02_fun=7,               //功能码
    CMB02_addrH,               //起始地址 高字节
    CMB02_addrL,               //起始地址 低字节
    CMB02_regnumH,             //寄存器个数  高字节
    CMB02_regnumL             //寄存器个数  低字节
}MODBUS_CMB02;


//----modbus 02 响应格式------------
typedef enum modbus_rmb02{
    RMB02_fun=7,                 //功能码
    RMB02_regnum,                //数据字节数
    RMB02_regdata               //寄存器数据 = n
}MODBUS_RMB02;


//----modbus 04 读输入寄存器 格式------------
typedef enum modbus_cmb04{
    CMB04_fun=7,               //功能码
    CMB04_addrH,               //起始地址 高字节
    CMB04_addrL,               //起始地址 低字节
    CMB04_regnumH,             //寄存器个数  高字节
    CMB04_regnumL            //寄存器个数  低字节
}MODBUS_CMB04;
//----modbus 04 响应格式------------
typedef enum modbus_rmb04{
    RMB04_fun=7,                  //功能码
    RMB04_regnum,                 //数据字节数
    RMB04_regdata                //寄存器数据 = 寄存器个数*2
}MODBUS_RMB04;

//----modbus 06 读输入寄存器 格式------------
typedef enum modbus_cmb06{
    CMB06_fun=7,               //功能码
    CMB06_addrH,               //起始地址 高字节
    CMB06_addrL,               //起始地址 低字节
    CMB06_regdataH,             //寄存器值
    CMB06_regdataL             //寄存器值
}MODBUS_CMB06;
//----modbus 06 响应格式------------
typedef enum modbus_rmb06{
    RMB06_fun=7,                  //功能码
    RMB06_addrH,               //起始地址 高字节
    RMB06_addrL,               //起始地址 低字节
    RMB06_regdataH,             //寄存器值
    RMB06_regdataL            //寄存器值
}MODBUS_RMB06;


//----modbus 10 读输入寄存器 格式------------
typedef enum modbus_cmb10{
    CMB10_fun=7,               //功能码
    CMB10_addrH,               //起始地址 高字节
    CMB10_addrL,               //起始地址 低字节
    CMB10_regnumH,             //寄存器个数  高字节
    CMB10_regnumL,             //寄存器个数  低字节
    CMB10_datanum,             //数据字节数 = 寄存器个数*2
    CMB10_regdata            //寄存器数据 = 寄存器个数*2
}MODBUS_CMB10;

//----modbus 10 响应格式------------
typedef enum modbus_rmb10{
    RMB10_fun=7,                  //功能码
    RMB10_addrH,               //起始地址 高字节
    RMB10_addrL,               //起始地址 低字节
    RMB10_regnumH,             //寄存器个数  高字节
    RMB10_regnumL             //寄存器个数  低字节
}MODBUS_RMB10;

};

#endif // TCP_MODBUS_THREAD_H
