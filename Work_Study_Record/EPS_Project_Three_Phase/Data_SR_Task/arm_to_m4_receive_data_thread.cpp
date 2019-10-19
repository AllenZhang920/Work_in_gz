#include "arm_to_m4_receive_data_thread.h"

Arm_To_M4_Receive_Data_Thread::Arm_To_M4_Receive_Data_Thread(QObject *parent) :
  QThread(parent)
{

}

void Arm_To_M4_Receive_Data_Thread::run()
{
  Arm_To_M4_Receive_Data();
  exec();
}

void Arm_To_M4_Receive_Data_Thread::Arm_To_M4_Receive_Data()
{
  INT32 ret = 0;
  UINT8	buffer[2*MAX_MODBUS_FRAMESIZE];
  UART_M4_MSG msg;
  UINT32 len = 0, frame_len = 0,frame_start = 0;
  UINT16 wCRCCheckValue=0;

  while(1){
    thread_m4_receive=1;
    len = read_datas_tty(Fd_Uart_Arm_M4, buffer, 2*MAX_MODBUS_FRAMESIZE, 0, 20000);//系统还有其他任务在执行，超时时间太短的话，收到的数据帧会被分片
    if(len == 0) continue;

    frame_start = 0;
    while(1){
     if(frame_start + 8 > len){//以最短帧（长度8个字节）算，判断是否超过接收到的总数据长度
/*
        printf("read_datas_tty erro not enough one frame ret = %d\n",len);
        printf("read_datas_tty erro data= ");
        for(; frame_start<len; frame_start++){
            printf("%02x ", buffer[frame_start]);
        }
        printf("\n");
*/
        break;
      }

      if(buffer[frame_start + 4] == 0x06 || buffer[frame_start + 4] == 0x10){//设置数据反馈信息
        frame_len = 8 + 3 ;//每帧数据长度为11字节
      }else if((buffer[frame_start + 4] == 0x84)||(buffer[frame_start + 4] == 0x83)||(buffer[frame_start + 4] == 0x82)||(buffer[frame_start + 4] == 0x86)||(buffer[frame_start + 4] == 0x90)){//没有排除错误码--错误码总共8+3个字节
        frame_len = 8  ;//每帧数据长度为11字节
      }else{
        frame_len = 8 + buffer[frame_start + 5]; //当前帧数据长度（3个前缀 + 1地址 + 1功能码 + 1长度 + N数据 +2校验）
      }


      if((frame_start + frame_len) <= len){//判断当前帧是否 超过 接收到的总数据长度
         wCRCCheckValue = ModbusCRC(&buffer[frame_start + 3], frame_len-5);//校验CRC
        if(((wCRCCheckValue >> 8) == buffer[frame_start + frame_len -2])&&((wCRCCheckValue&0x00ff) == buffer[frame_start + frame_len -1])){
          if(buffer[frame_start + 3] == DSPSTARTADDR&&buffer[frame_start]>>5==1){

            msg.type = 1;
          }else if((buffer[frame_start + 3] >= BATTERYSTARTADDR)&&(buffer[frame_start + 3] < 150)){
            msg.type = 2;
          }else if(buffer[frame_start + 3] == IOSTARTADDR){

            msg.type = 3;
          }else if(buffer[frame_start + 3] == IOSTARTADDR+1){

            msg.type = 4;
            }else if(buffer[frame_start + 3] == ATSSTARTADDR){

            msg.type = 5;
          }

          memcpy(msg.argv, &buffer[frame_start + 3], frame_len-3); // 接收到的数据位

          if((buffer[frame_start + 4] == 0x06)||(buffer[frame_start + 4] == 0x86)||(buffer[frame_start + 4] == 0x10)){
            ret= msgsnd(Globa.uart_m4_set_msg_recv_id, &msg, frame_len-3, IPC_NOWAIT);
          }else if((buffer[frame_start + 4] == 0x02)||(buffer[frame_start + 4] == 0x03)||(buffer[frame_start + 4] == 0x04)||(buffer[frame_start + 4] == 0x01)){
            ret = msgsnd(Globa.uart_m4_msg_recv_id, &msg, frame_len-3, IPC_NOWAIT);
          }

          if(ret <0){
            perror("UART_ttyS1_m4_receive msgsnd ERROR\n\n");
          }
        }else{
/*
          printf("标准MODBUS协议xxread_datas_tty crc erro ret = %d w=%02x, y=%02x\n",frame_len,wCRCCheckValue ,(buffer[frame_start + frame_len -2]<<8|buffer[frame_start + frame_len -1]));
          printf("标准MODBUS协议xxxread_datas_tty erro data= ");
          for(i=0; i<frame_len; i++){
              printf("%02x ", buffer[frame_start+i]);
            }
          printf("\n");
*/
        }

        frame_start = frame_start + frame_len;//下一数据帧的起始位置
        if(frame_start == len){//最后一帧刚好是总接收数据末尾
          break;
        }
      }else{//当前帧长度错误，导致剩余长度不够一帧
#if DEBUG_RS232_M4
        printf("当前帧  data erro ret = %d,%d,%d\n",frame_start,frame_len,len);
        printf("当前帧  erro data= ");
        for(; frame_start<len; frame_start++){
            printf("%02x ", buffer[frame_start]);
          }
        printf("\n");
#endif
        break;
      }
    }
  }
}
