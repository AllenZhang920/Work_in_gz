#ifndef PARAM_H
#define PARAM_H

#include"Globalvar.h"

extern void  System_Config_File_Init();
extern STATUS System_UseSystem_save();
extern STATUS System_Set_DSP_Parame_save(void);
extern STATUS System_Battery_Param_Set_save(void);
extern STATUS System_Battery_GroupParam_Set_save(void);
extern STATUS System_Curstomer_Param_Set_save(void);
extern STATUS System_Net_Set_Save(void);
extern STATUS System_Sigle_Baterry_Voltage_Alarm_Para_save(void);
extern STATUS System_SysAppend_Set_save(void);
extern INT32 UART_ttyS2_PC_init(void);
extern void LOG(const char* ms, ... );
extern unsigned int GetFileSize(char *filename);

#endif // PARAM_H
