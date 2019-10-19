#ifndef BACKDATA_H
#define BACKDATA_H
typedef enum _ANALOG_BASE_TYPE_NUM{
  MAINS_INPUT_NUM_LINES	,//	0	输入相数
  MAINS_AC_VOLT_PHASE_A	,//	1	"市电A相电压/市电电压"
  MAINS_AC_VOLT_PHASE_B	,//	2	市电B相电压
  MAINS_AC_VOLT_PHASE_C	,//	3	市电C相电压
  MAINS_AC_CURRENT_PHASE_A	,//	4	"市电A相电流/市电电流"
  MAINS_AC_CURRENT_PHASE_B	,//	5	市电B相电流
  MAINS_AC_CURRENT_PHASE_C	,//	6	市电C相电流
  MAINS_FREQUENCY	,//	7	市电频率
  MAINS_ACTIVE_POWER_PHASE_A	,//	8	"A相输入有功功率/输入有功功率"
  MAINS_ACTIVE_POWER_PHASE_B	,//	9	B相输入有功功率
  MAINS_ACTIVE_POWER_PHASE_C	,//	10	C相输入有功功率
  BYPASS_AC_VOLT_PHASE_A	,//	11	"旁路A相电压/旁路电压"
  BYPASS_AC_VOLT_PHASE_B	,//	12	旁路B相电压
  BYPASS_AC_VOLT_PHASE_C	,//	13	旁路C相电压
  BYPASS_AC_CURRENT_PHASE_A	,//	14	"旁路A相电流/旁路电流"
  BYPASS_AC_CURRENT_PHASE_B	,//	15	旁路B相电流
  BYPASS_AC_CURRENT_PHASE_C	,//	16	旁路C相电流
  BYPASS_AC_POWER_PHASE_A	,//	17	"旁路A相功率/旁路功率"
  BYPASS_AC_POWER_PHASE_B	,//	18	旁路B相功率
  BYPASS_AC_POWER_PHASE_C	,//	19	旁路C相功率
  BYPASS_FREQUENCY	,//	20	旁路频率
  OUTPUT_NUM_LINES	,//	21	输出相数
  INVERTER_AC_VOLT_PHASE_A	,//	22	"逆变A相电压/逆变电压"
  INVERTER_AC_VOLT_PHASE_B	,//	23	逆变B相电压
  INVERTER_AC_VOLT_PHASE_C	,//	24	逆变C相电压
  INVERTER_FREQUENCY	,//	25	逆变频率
  OUTPUT_AC_VOLT_PHASE_A	,//	26	"输出A相电压/输出电压"
  OUTPUT_AC_VOLT_PHASE_B	,//	27	输出B相电压
  OUTPUT_AC_VOLT_PHASE_C	,//	28	输出C相电压
  OUTPUT_AC_CURRENT_PHASE_A	,//	29	"输出A相电流/输出电流"
  OUTPUT_AC_CURRENT_PHASE_B	,//	30	输出B相电流
  OUTPUT_AC_CURRENT_PHASE_C	,//	31	输出C相电流
  OUTPUT_FREQUENCY	,//	32	输出频率
  BUS_DC_VOLT_TOTAL    ,//	33	"正母线电压/母线电压"
  BUS_DC_VOLT_NEGATIVE	,//	34	负母线电压
  ACTIVE_POWER_PHASE_A	,//	35	"A相输出有功功率/输出有功功率"
  ACTIVE_POWER_PHASE_B	,//	36	B相输出有功功率
  ACTIVE_POWER_PHASE_C	,//	37	C相输出有功功率
  APPARENT_POWER_PHASE_A	,//	38	"A相输出视在功率/输出视在功率"
  APPARENT_POWER_PHASE_B	,//	39	B相输出视在功率
  APPARENT_POWER_PHASE_C	,//	40	C相输出视在功率
  LOAD_LEVEL_PHASE_A	,//	41	"A相输出负载百分比/输出负载百分比"
  LOAD_LEVEL_PHASE_B	,//	42	B相输出负载百分比
  LOAD_LEVEL_PHASE_C	,//	43	C相输出负载百分比
  SYSTEM_TEMPERATURE	,//	44	机内温度1
  AMBIENT_TEMPERATURE	,//	45	环境温度
  BATTERY_TEMPERATURE	,//	46	电池温度
  BATTERY_DC_VOLTAGE_POSITIVE	,//	47	"电池正边电压/电池电压"
  BATTERY_DC_VOLTAGE_NEGATIVE	,//	48	电池负边电压
  BATTERY_CURRENT	,//	49	电池电流
  BATTERY_REMAIN_CAPACITY	,//	50	电池剩余容量
  BATTERY_DISCHARGE_TIME_SECONDS	,//	51	电池已放电时间
  BATTERY_REMAIN_TIME	,//	52	电池剩余时间

  CHARGE_DC_VOLTAGE_POSITIVE	,//	53	"正边充电电压/充电电压"
  CHARGE_DC_VOLTAGE_NEGATIVE	,//	54	负边充电电压
  CHARGE_CURRENT_POSITIVE	,//	55	"正边充电电流/充电电流"
  CHARGE_CURRENT_NAGETIVE	,//	56	负边充电电流
  SYSTEM_TIME	,//	57	系统时间
  INPUT_LINE_BADS	,//	58	输入故障次数

  ANALOG_BASE_TYPE=59
}ANALOG_BASE_TYPE_NUM;

typedef enum _STATUS_BASE_TYPE_NUM{
  SYSTEM_WORK_MODE=65	,//	65	系统工作模式
  SYSTEM_SUPPLY_MODE	,//	66	系统供电模式
  RECTIFIER_WORK_STATUS	,//	67	整流器工作状态
  BYPASS_SUPPLY_STATUS	,//	68	旁路供电状态
  INVERTER_WORK_STATUS	,//	69	逆变器工作状态
  INVERTER_SUPPLY_STATUS	,//	70	逆变器供电状态
  CHARGER_WORK_MODE	,//	71	充电器工作模式
  CHARGER_WORK_STATUS	,//	72	充电器工作状态
  BATTERY_CONNECTED_STATUS	,//	73	电池连接状态
  BATTERY_DISCHARGE_STATUS	,//	74	电池充放电状态
  INPUT_SWITCH_STATUS	,//	75	输入空开状态
  BYPASS_SWITCH_STATUS	,//	76	旁路空开状态
  MANUAL_BYPASS_SWITCH_STATUS	,//	77	手动旁路空开状态
  SERVICE_BYPASS_SWITCH_STATUS	,//	78	维修旁路空开状态
  OUTPUT_SWITCH_STATUS	,//	79	输出空开状态
  BATTERY_SWITCH_STATUS	,//	80	电池空开状态
  GENERATOR_CONNECT_STATUS	,//	81	发电机接入状态
  SERVICE_MODE	,//	82	系统维修状态
  BYPASS_STATIC_SWITCH_STATUS	,//	83	旁路静态开关状态
  INVERTER_STATIC_SWITCH_STATUS	,//	84	逆变静态开关状态
  SYSTEM_STARTUP_ENABLE_STATUS	,//	85	开机允许状态
  SYSTEM_MODE_SELECT,//86  系统模式
  STATUS_BASE_TYPE=87
}STATUS_BASE_TYPE_NUM;

typedef enum _NOTICE_BASE_TYPE_NUM{
  SUMMARY_NOTICE	,//	0	系统告警
  EPO_ACTIVE	,//	1	紧急关机告警
  COMMUNICATION_FAILED	,//	2	通信异常
  MAINS_AC_ABNORMAL	,//	3	市电异常
  MAINS_AC_VOLTAGE_ABNORMAL	,//	4	市电电压异常
  MAINS_FREQUENCY_ABNORMAL	,//	5	市电频率异常
  MAINS_AC_PHASE_ERROR	,//	6	"市电相序错误/市电L-N线反接(单相)"
  BYPASS_AC_ABNORMAL	,//	7	旁路异常
  BYPASS_AC_VOLTAGE_ABNORMAL	,//	8	旁路电压异常
  BYPASS_FREQUENCY_ABNORMAL	,//	9	旁路频率异常
  BYPASS_AC_PHASE_ERROR	,//	10	"旁路相序错误/旁路L-N线反接(单相)"
  BATTERY_BAD_NOTICE	,//	11	电池更换提示
  BATTERY_LOW_VOLTAGE	,//	12	电池低压
  BATTERY_END_OF_DISCHARGE	,//	13	电池放电终止
  BATTERY_VOLTAGE_ABNORMAL	,//	14	电池电压异常
  BATTERY_TEMPERATURE_ABNORMAL	,//	15	电池温度异常
  OVERLOAD_FAILED	,//	16	过载异常
  OUTPUT_ABNORMAL_VOLTAGE	,//	17	输出异常
  EEPROM_FAULT	,//	18	EEPROM读写错误
  SYSTEM_FAN_LOCK	,//	19	系统风扇告警
  N_LINE_LOSS	,//	20	N线丢失
  BATTERY_TEST_ABNORMAL	,//	21	电池自检失败
  SYSTEM_TEST_ABNORMAL	,//	22	系统测试失败
  SYSTEM_FORCE_START,//23
  NOTICE_BASE_TYPE=24
}NOTICE_BASE_TYPE_NUM;

typedef enum _ALARM_BASE_TYPE_NUM{
  SUMMARY_ALARM=28	,//	28	系统故障
  AUXILIARY_POWER_FAULT	,//	29	辅助电源故障
  MAINS_SCR_FAULT	,//	30	市电SCR故障
  BYPASS_FAULT	,//	31	旁路故障
  BYPASS_SCR_FAULT	,//	32	旁路SCR故障
  RECTIFIER_FAULT	,//	33	整流器故障
  RECTIFIER_IGBT_OVER_CURRENT	,//	34	整流器IGBT过流
  BUS_DC_SOFT_START_OVERTIME	,//	35	BUS软启动超时
  BUS_DC_SHORT	,//	36	BUS短路
  BUS_DC_HIGH_VOLTAGE	,//	37	BUS高压
  BUS_DC_LOW_VOLTAGE	,//	38	BUS低压
  INVERTER_FAULT	,//	39	逆变器故障
  INVERTER_SOFT_START_OVERTIME	,//	40	逆变器软启动超时
  INVERTER_IGBT_OVER_CURRENT	,//	41	逆变器IGBT过流
  INVERTER_SCR_FAULT	,//	42	逆变SCR故障
  INVERTER_OVER_TEMPERATURE	,//	43	逆变器过温
  INVERTER_HIGH_VOLTAGE	,//	44	逆变高压
  INVERTER_LOW_VOLTAGE	,//	45	逆变低压
  OUTOPUT_FAULT	,//	46	输出故障
  OUTPUT_SHORT	,//	47	输出短路
  OVER_LOAD_FAULT	,//	48	过载故障
  OVER_LOAD_OVER_TIME	,//	49	过载超时
  BYPASS_OVER_LOAD	,//	50	旁路过载
  BYPASS_OVER_LOAD_FAULT	,//	51	旁路过载延时到
  SYSTEM_OVER_TEMPERATURE_FAULT	,//	52	系统过温故障
  IGBT_SHORT_FAULT	,//	53	IGBT短路
  RADIATOR_OVER_TEMPERATURE	,//	54	散热器过温
  BATTERY_ABNORMAL	,//	55	电池异常
  CHARGER_FAULT	,//	56	充电器故障
  USER_OPERATE_ERROR	,//	57	用户操作错误
  SHUTDOWN_FAULT	,//	58	关机故障
  ALARM_BASE_TYPE=59
}ALARM_BASE_TYPE_NUM;
typedef enum _NOTICE_EXTEND_TYPE_NUM{
  MAINS_AC_LOSS=65,//市电丢失
  BATTERY_CONTACTOR_ABNORMAL=97,//电池接触器异常
  NOTICE_EXTEND_TYPE=98
}NOTICE_EXTEND_TYPE_NUM;

typedef enum _ALARM_EXTEND_TYPE_NUM{
  ALARM_EMERGENCY_UNBALANCE=124,//应急电压不平衡
   ALARM_Battery_Connection_Abnormal=125,// 电池连接异常
  //BATTERY_VOLTAGE_FAULT=177,//电池电压故障
  MAIN_STATUS=150,
  STANDBY_STATUS=151,
  ALARM_SWITCH=160,
  ALARM_EXTEND_TYPE=300
}ALARM_EXTEND_TYPE_NUM;

typedef enum _COMMAND_BASE_TYPE_NUM{

  CONFIG_SYSTEM_WORK_MODE=71,//	工作模式设定
  CONFIG_SYSTEM_LONG_TYPE,//	机型设置
  CONFIG_SYSTEM_PF,//	输出功率因数设置
  keepcommand1,//（预留）
  keepcommand2,//       （预留）
  keepcommand3,//        （预留）
  keepcommand4,//        （预留）
  keepcommand5,//        （预留）
  CONFIG_RATE_KW,//	额定功率
  CONFIG_RATE_KVA,//	额定容量
  CONFIG_RATE_LICENSED_KVA,//	授权系统容量
  CONFIG_RATE_INPUT_VOLT,//	额定输入电压
  CONFIG_RATE_INPUT_FREQUENCY,//	额定输入频率
  CONFIG_RATE_OUTPUT_VOLT,//	额定输出电压
  CONFIG_RATE_OUTPUT_FREQUENCY,//	额定输出频率
  CONFIG_RATE_BYPASS_VOLT,//	额定旁路电压
  CONFIG_RATE_DCBUS_VOLT,//	额定母线电压
  keepcommand11,//（预留）
  keepcommand12,//       （预留）
  keepcommand13,//        （预留）
  keepcommand14,//        （预留）
  keepcommand15,//        （预留）
  CONFIG_INPUT_VOLTAGE_HIGH_LIMIT,//	输入电压上限	√
  CONFIG_INPUT_VOLTAGE_LOW_LIMIT,//	输入电压下限	√
  CONFIG_INPUT_FREQUENCY_HIGH_LIMIT,//	输入频率上限	√
  CONFIG_INPUT_FREQUENCY_LOW_LIMIT,//	输入频率下限	√
  CONFIG_BYPASS_VOLTAGE_HIGH_LIMIT,//	旁路电压上限
  CONFIG_BYPASS_VOLTAGE_LOW_LIMIT,//	旁路电压下限
  CONFIG_BYPASS_FREQUENCY_HIGH_LIMIT,//	旁路频率上限
  CONFIG_BYPASS_FREQUENCY_LOW_LIMIT,//	旁路频率下限
  CONFIG_ECO_INPUT_VOLTAGE_HIGH_LIMIT,//	ECO电压上限
  CONFIG_ECO_INPUT_VOLTAGE_LOW_LIMIT,//	ECO电压下限
  CONFIG_INPUT_CURRENT_LIMIT,//	输入电流限流值
  CONFIG_SLEW_RATE,//	同步速率
  keepcommand21,//（预留）
  keepcommand22,//       （预留）
  keepcommand23,//        （预留）
  keepcommand24,//        （预留）
  keepcommand25,//        （预留）
  CONFIG_UNITED_SYSTEM,//	单机/并机
  CONFIG_UNITED_NUMBER,//	并机台数
  CONFIG_UNITED_ID,//	并机序号
  CONFIG_RUN_MODE_TYPE,//	机柜地址
  CONFIG_REDUNT_NUM,//	冗余模组数
  keepcommand31,//（预留）
  keepcommand32,//       （预留）
  keepcommand33,//        （预留）
  keepcommand34,//        （预留）
  keepcommand35,//        （预留）
  CONFIG_BATTERY_TYPE,//	电池类型
  CONFIG_BATTERY_AH,//	电池容量	√
  CONFIG_BATTERY_CELL_VOLT,//	电池单体电压
  CONFIG_BATTERY_CELLS,//	电池单体数
  CONFIG_BATTERY_BLOCKS,//	电池节数	√
  CONFIG_BATTERY_STRINGS,//	电池组数 √
  CONFIG_BATTERY_TOTAL_VOLT,//	电池额定电压
  CONFIG_BATTERY_EQUALIZE_VOLT,//	电池均充电压
  CONFIG_BATTERY_FLOAT_VOLT,//	电池浮充电压
  CONFIG_CHARGE_START_ENABLE,//	充电器使能
  CONFIG_STEP1_CHARGE_TIME,//	阶段一充电时间
  CONFIG_STEP2_CHARGE_TIME,//	阶段二充电时间
  CONFIG_STEP3_CHARGE_TIME,//	阶段三充电时间
  CONFIG_TEMPERATURE_COMPENSATE_COEFFICIENT,//	温度补偿系数
  CONFIG_BATTERY_DOD_VOLT,//	电池DOD电压
  CONFIG_BATTERY_EOD_VOLT,//	电池EOD电压	√
  CONFIG_CHARGE_SPEED,//	充电速率
  CONFIG_RATE_CHARGE_CURRENT,//	充电电流
  CONFIG_BATTERY_CHARGE_CURRENT_LIMIT,//	充电限电流
  CONFIG_BATTERY_CHARGE_VOLTAGE_LIMIT,//	充电过压值
  CONFIG_BATTERY_WATT_TIME_TABLE,//	电池数据表
  CONFIG_BATTERY_SOH_THRESHOLD,//	电池损耗阀值
  CONFIG_BATTERY_WARRANTY_TIME,//	电池过保周期
  CONFIG_SELFTEST_ENABLE,//	自检功能使能
  CONFIG_SELFTEST_CYCLE,//	自检周期
  CONFIG_SELFTEST_TIME,//	自检时间
  CONFIG_SELFTEST_DOD,//	自检电压点
  CONFIG_SELFTEST_PERIOD,//	自检时间段
  CONFIG_NEXT_TEST_DATE,//	下次自检时间
  keepcommand41,//（预留）
  keepcommand42,//       （预留）
  keepcommand43,//        （预留）
  keepcommand44,//        （预留）
  keepcommand45,//        （预留）
  CONFIG_SYSTEM_LANGUAGE,// 	系统语言
  CONFIG_COMMUNICATION_PROTOCOL,// 	通信协议
  CONFIG_COMMUNICATION_ADDRESS,// 	通信地址
  CONFIG_COMMUNICATION_BAUDRATE,// 	通信波特率
  CONFIG_COMMUNICATION_PARITY,// 	通信校验
  CONFIG_COMMUNICATION_STOP_BIT	,// 通信停止位
  keepcommand51,//（预留）
  keepcommand52,//       （预留）
  keepcommand53,//        （预留）
  keepcommand54,//        （预留）
  keepcommand55,//        （预留）
  CONFIG_RECITIFIER_START_DELAY_TIME,//	整流启动延时时间
  keepcommand61,//       发电机输入功率配比
  keepcommand62,//        并网功能选择
  keepcommand63,//        逆变器并网输出功率
  keepcommand64,//        逆变器节能模式
  keepcommand65,//        逆变器休眠模式
  keepcommand66,//        休眠模式负载点
  keepcommand67,//        休眠模式延时关机时间
  keepcommand68,//        DC自动开机功能
  keepcommand69,//        DC放电恢复开机等待时间设置
  CONFIG_EPO_ACTIVE_OUTPUT_STATE,// 	EPO动作模式
  CONFIG_AUTOSTARTUP_ENABLE,// 	AC自启动使能
  CONFIG_AUTORESTART_ENABLE,// 	掉电重启使能
  CONFIG_TOBYP_ENABLE,// 	开旁路使能
  CONFIG_MANUAL_BYP_ENABLE,// 	手动旁路使能
  CONFIG_REMOTE_ENABLE,// 	远程操作使能
  CONFIG_FORCE_START_ENABLE,// 	强制启动使能
  CONFIG_DISPLAY_POWER,// 	功率显示使能
  CONFIG_LCD_TYPE,// 	显示屏选择
  keepcommand71 ,//      输入电压显示设置
  keepcommand72 ,//      输出电压显示设置
  CONFIG_BUZZER_ENABLE,//	报警音使能
  keepcommand81,//（预留）
  keepcommand82,//       （预留）
  keepcommand83,//        （预留）
  keepcommand84,//        （预留）
  keepcommand85,//        （预留）
  CONFIG_EXTEND_TYPE_NUM,//	设置量扩展类数量
  CONFIG_INPUT_VOLTAGE_AD_PHASE_A,//	"A相输入电压采样校正/输入电压采样校正"	√
  CONFIG_INPUT_VOLTAGE_AD_PHASE_B,//	B相输入电压采样校正
  CONFIG_INPUT_VOLTAGE_AD_PHASE_C,//	C相输入电压采样校正
  CONFIG_BYPASS_VOLTAGE_AD_PHASE_A,//	"A相旁路电压采样校正/旁路电压采样校正"
  CONFIG_BYPASS_VOLTAGE_AD_PHASE_B,//	B相旁路电压采样校正
  CONFIG_BYPASS_VOLTAGE_AD_PHASE_C,//	C相旁路电压采样校正
  CONFIG_INVERTER_VOLTAGE_AD_PHASE_A,//	"A相逆变电压采样校正/逆变电压采样校正"	√
  CONFIG_INVERTER_VOLTAGE_AD_PHASE_B,//	B相逆变电压采样校正
  CONFIG_INVERTER_VOLTAGE_AD_PHASE_C,//	C相逆变电压采样校正
  CONFIG_OUTPUT_VOLTAGE_AD_PHASE_A,//	"A相输出电压采样校正/输出电压采样校正"	√
  CONFIG_OUTPUT_VOLTAGE_AD_PHASE_B,//	B相输出电压采样校正
  CONFIG_OUTPUT_VOLTAGE_AD_PHASE_C,//	C相输出电压采样校正
  CONFIG_BATTERY_VOLTAGE_AD,//	电池电压采样校正	√
  CONFIG_POSITIVE_BUS_VOLTAGE_AD,//	正母线电压采样校正	√
  CONFIG_NEGTIVE_BUS_VOLTAGE_AD,//	负母线电压采样校正
  CONFIG_INVERTER_VOLTAGE_COEF_PHASE_A,//	"A相逆变电压微调/逆变电压微调"
  CONFIG_INVERTER_VOLTAGE_COEF_PHASE_B,//	B相逆变电压微调
  CONFIG_INVERTER_VOLTAGE_COEF_PHASE_C,//	C相逆变电压微调
  SETTING_BATTUSED_MODE,//	电池共用方式设置
  CONFIG_GENERATOR_CONNECT_DISABLE_CHARGE,//	发电机接入关闭充电器使能
  CONFIG_BATTERY_TEMPERATURE_HIGH_LIMIT,//	高温告警
  CONFIG_TEMPER_SENSOR_NUM,//	温度传感器数量
  CONFIG_DRY_CONTAC,//T	干接点功能
  CONFIG_TOUCH_COEF,//	屏幕校准参数
  CONFIG_CHARGER_NUM_LIMIT,//	在线充电器数量
  CONFIG_INPUT_WIRE_TYPE,//	输入类型
  CONFIG_CODE0=187  ,
  CONFIG_CODE1=188,
  CONFIG_CODE2=189,
  CONFIG_CODE3=190,


  INVERTER_ON_COMMAND=61440,//逆变开机/开机
  INVERTER_OFF_COMMAND=61441,// 逆变关机/关机
  SETTING_CLEAR_FAULT=61453,//清故障命令

  COMMAND_BASE_TYPE=61454
}COMMAND_BASE_TYPE_NUM;

#endif // BACKDATA_H
