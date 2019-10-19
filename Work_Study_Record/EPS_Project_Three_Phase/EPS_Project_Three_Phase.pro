#-------------------------------------------------
#
# Project created by QtCreator 2015-08-31T08:20:22
#* Copyright(C)    2015-2015,      EAST Co.Ltd.
#* File name :     EPS系统
#* Author :        邓建辉            dengjh0226@163.com
#* Version :       1.0
#* Date :          2015.1.30
#* Last modified : 2014.1.30
#-------------------------------------------------


QT       += core gui\
            sql\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS +=$$PWD/lib/liboptkeypad.a \
    /usr/local/Qt-libiconv/lib/libiconv.a #libiconv库  为需要做转换的应用提供了一个iconv()的函数，以实现一个字符编码到另一个字符编码的转换

TARGET =../output/app/DcScreen_Power
MOC_DIR=../output/moc
RCC_DIR=../output/rcc
OBJECTS_DIR=../output/obj
UI_DIR=../output/ui

#TARGET =DcScreen_Power

TEMPLATE = app


SOURCES += main.cpp \
    interface_frame_work.cpp \
    application.cpp \
    Data_SR_Task/tcp_modbus_thread.cpp \
    Data_SR_Task/failure_event_alarm_process_thread.cpp \
    Data_SR_Task/battery_send_receive_thread.cpp \
    Data_SR_Task/arm_to_pc_send_receive_data_thread.cpp \
    Data_SR_Task/arm_to_m4_send_data_thread.cpp \
    Data_SR_Task/arm_to_m4_receive_data_thread.cpp \
    Data_SR_Task/arm_dsp_send_receive_data_thread.cpp \
    Display_Data/testdata_display.cpp \
    Display_Data/systeminfor.cpp \
    Display_Data/product_infor.cpp \
    Display_Data/mian_interface.cpp \
    Display_Data/eps_run_information.cpp \
    Display_Data/eps_main_menu.cpp \
    Display_Data/current_alarm_query.cpp \
    Display_Data/batteryform.cpp \
    Display_Data/alarmqueryform.cpp \
    Display_Data/aboutus.cpp \
    KeyBoard/popup_keboarddialog.cpp \
    KeyBoard/passwordkeyboarddialog.cpp \
    Maintain_Set/user_set.cpp \
    Maintain_Set/set_menu.cpp \
    Maintain_Set/maintain_set.cpp \
    Share/param.cpp \
    Share/file.cpp \
    Share/Common_Function.cpp \
    User_Set/monitor_screen_set.cpp \
    User_Set/calibration.cpp \
    USB/sUSB_Storage_Thread.cpp \
    USB/import_export_data.cpp \
    Display_Data/startup_shutdown.cpp \
    Data_SR_Task/ats_switch_send_receive_data.cpp \
    Display_Data/reboot.cpp \
    zmessage.cpp \
    monitor_thread_runnig_thread.cpp \
    updata_modbus_thread.cpp \
    branchalarmset.cpp


HEADERS  += interface_frame_work.h \
    application.h \
    Data_SR_Task/tcp_modbus_thread.h \
    Data_SR_Task/failure_event_alarm_process_thread.h \
    Data_SR_Task/battery_send_receive_thread.h \
    Data_SR_Task/arm_to_pc_send_receive_data_thread.h \
    Data_SR_Task/arm_to_m4_send_data_thread.h \
    Data_SR_Task/arm_to_m4_receive_data_thread.h \
    Data_SR_Task/arm_dsp_send_receive_data_thread.h \
    Display_Data/testdata_display.h \
    Display_Data/systeminfor.h \
    Display_Data/product_infor.h \
    Display_Data/mian_interface.h \
    Display_Data/eps_run_information.h \
    Display_Data/eps_main_menu.h \
    Display_Data/current_alarm_query.h \
    Display_Data/batteryform.h \
    Display_Data/alarmqueryform.h \
    Display_Data/aboutus.h \
    KeyBoard/popup_keboarddialog.h \
    KeyBoard/passwordkeyboarddialog.h \
    Maintain_Set/user_set.h \
    Maintain_Set/set_menu.h \
    Maintain_Set/maintain_set.h \
    Share/param.h \
    Share/Macro_Definition.h \
    Share/Globalvar.h \
    Share/file.h \
    Share/Device_Config.h \
    Share/Common_Function.h \
    User_Set/monitor_screen_set.h \
    User_Set/calibration.h \
    USB/sUSB_Storage_Thread.h \
    USB/import_export_data.h \
    Display_Data/startup_shutdown.h \
    Data_SR_Task/ats_switch_send_receive_data.h \
    Display_Data/reboot.h \
    zmessage.h \
    Share/backdata.h \
    monitor_thread_runnig_thread.h \
    updata_modbus_thread.h \
    branchalarmset.h

FORMS    += interface_frame_work.ui \
    Display_Data/testdata_display.ui \
    Display_Data/systeminfor.ui \
    Display_Data/product_infor.ui \
    Display_Data/mian_interface.ui \
    Display_Data/eps_main_menu.ui \
    Display_Data/current_alarm_query.ui \
    Display_Data/batteryform.ui \
    Display_Data/alarmqueryform.ui \
    Display_Data/aboutus.ui \
    KeyBoard/popup_keboarddialog.ui \
    KeyBoard/passwordkeyboarddialog.ui \
    Maintain_Set/user_set.ui \
    Maintain_Set/set_menu.ui \
    Maintain_Set/maintain_set.ui \
    User_Set/monitor_screen_set.ui \
    USB/import_export_data.ui \
    Display_Data/startup_shutdown.ui \
    Display_Data/reboot.ui \
    zmessage.ui \
    Display_Data/eps_run_information.ui \
    branchalarmset.ui


RESOURCES += \
    EPS_Project_Three_Phase.qrc
