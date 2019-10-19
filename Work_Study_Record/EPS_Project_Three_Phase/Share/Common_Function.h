#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include "Macro_Definition.h"
#include <QWidget>

extern UINT16 ModbusCRC(UINT8 *puchMsg, UINT8 usDataLen);

extern INT32 Led_Relay_Control(INT32 pin,INT32 value);
extern UINT32 Issuedequipment_parameters_06(UINT8 adress ,UINT16 Register , UINT16 pdata,UINT8 id);
extern UINT32 Issuedequipment_parameters_10(UINT8 adress ,UINT16 Register , UINT16 pdata,UINT8 id);
extern INT32 set_Parity(INT32 fd,INT32 databits,INT32 stopbits,INT32 parity);
extern void set_speed(INT32 fd, UINT32 speed);
extern INT32 read_datas_tty(INT32 fd, UINT8 *rcv_buf, INT32 Len, UINT32 sec, UINT32 usec);
extern INT32 write_datas_tty(INT32 fd, UINT8 *send_buf, INT32 Len);
extern INT32 sGet_Net_Mask( char *netMask);

extern INT32 sGet_IP_Address( char *IPAddress);

extern INT32 sGet_GateWay(char *gateway);
extern UINT32 Issuede_Remote_CTL(UINT16 Register, UINT16 pdata);
extern void Insert_Operation_Work_Record(UINT32 Type, QString Record_Name);
#endif // COMMON_FUNCTION_H
