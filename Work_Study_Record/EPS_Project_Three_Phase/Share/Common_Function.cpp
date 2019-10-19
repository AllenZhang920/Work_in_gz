#include"Common_Function.h"

#include"Globalvar.h"
#include <QList>


/************library function API********************************/

/* 速度 */
static const UINT32 speed_arr[] = {B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B921600};
/* 串口速度 */
static const UINT32 name_arr[] = {300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};


const UINT8 auchCRCHi[] = {
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
  0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
  0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
  0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
  0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
  0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
  0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
  0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
  0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
  0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
  0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

const UINT8 auchCRCLo[] = {
  0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
  0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
  0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
  0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
  0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
  0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
  0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
  0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
  0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
  0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
  0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
  0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
  0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
  0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
  0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
  0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
  0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
  0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
  0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
  0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
  0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
  0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
  0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
  0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
  0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
  0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ;



UINT16 ModbusCRC(UINT8 *puchMsg, UINT8 usDataLen)
{
  UINT16 uchCRCHi = 0xff;
  UINT16 uchCRCLo = 0xff;
  UINT16 uIndex ; // CRC循环中的索引

  while(usDataLen--){
      uIndex = uchCRCHi ^ *puchMsg++; // 计算CRC
      uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
      uchCRCLo = auchCRCLo[uIndex] ;
    }
  return ((uchCRCHi << 8 )|uchCRCLo);
}


INT32 Led_Relay_Control(INT32 pin, INT32 value)
{
  INT32 ret;
  if(((pin >= 0) && (pin < 23))&&((value == 0)||(value == 1))){
      ret = ioctl(ttyled_fd,value,pin);
      if(ret){
          printf("ioctl DRIVER_GPIO_IOCTL");
          return(-1);
        }
    }else{
      printf("error LED gpio %d value %d\n",pin,value);
      return -1;
    }
  return(0);
}

/***************set series API***************************
**description：设置串口通信速率
***parameter  :fd     类型 int  打开串口的文件句柄
**************speed  类型 int  串口速度
***return		  :none
**********************************************************/
void set_speed(INT32 fd, UINT32 speed)
{
  UINT32  i = 0;
  INT32   status = 0;
  struct termios   Opt;

  tcgetattr(fd, &Opt);
  for(i= 0; i < sizeof(speed_arr)/sizeof(int); i++){
      if(speed == name_arr[i]){
          tcflush(fd, TCIOFLUSH);
          cfsetispeed(&Opt, speed_arr[i]);
          cfsetospeed(&Opt, speed_arr[i]);
          status = tcsetattr(fd, TCSANOW, &Opt);
          if(status != 0){
              perror("tcsetattr fd");
              return;
            }
          tcflush(fd,TCIOFLUSH);
        }
    }
}



/*************************************************************
**description：设置串口数据位，停止位和效验位
***parameter :fd     类型  int  打开的串口文件句柄
databits 类型  int 数据位   取值 为 7 或者8
stopbits 类型  int 停止位   取值为 1 或者2
parity  类型  int  效验类型 取值为N,E,O,S,M
**return		：none
**ICANON 启用标准模式 (canonical mode)。允许使用特殊字符
 EOF, EOL, EOL2, ERASE, KILL, LNEXT, REPRINT, STATUS, 和 WERASE，
 以及按行的缓冲。ECHO 回显输入字符。ECHOE 如果同时设置了 ICANON，字符 ERASE 擦除前一个输入字符，WERASE 擦除前一个词。
 ISIG 当接受到字符 INTR, QUIT, SUSP, 或 DSUSP 时，产生相应的信号
 OPOST 启用具体实现自行定义的输出处理。其余 c_oflag 标志常量定义在 POSIX 1003.1-2001 中，除非另外说明。
 CLOCAL 忽略 modem 控制线。CREAD 表示使能数据接收器。IGNBRK忽略输入中的 BREAK 状态。
 BRKINT如果设置了 IGNBRK，将忽略 BREAK。如果没有设置，但是设置了 BRKINT，
 那么 BREAK 将使得输入和输出队列被刷新，如果终端是一个前台进程组的控制终端，
 这个进程组中所有进程将收到 SIGINT 信号。如果既未设置 IGNBRK 也未设置BRKINT，
 BREAK 将视为与 NUL 字符同义，除非设置了 PARMRK，这种情况下它被视为序列 \377 \0 \0。
 IGNPAR忽略桢错误和奇偶校验错。
 PARMRK如果没有设置 IGNPAR，在有奇偶校验错或桢错误的字符前插入 \377 \0。
 如果既没有设置 IGNPAR 也没有设置PARMRK，将有奇偶校验错或桢错误的字符视为 \0。
 INPCK启用输入奇偶检测。
 ISTRIP去掉第八位。
 IGNCR忽略输入中的回车。
 ICRNL将输入中的回车翻译为新行 (除非设置了 IGNCR)。
 IXON启用输出的 XON/XOFF 流控制。
**************************************************************/

INT32 set_Parity(INT32 fd, INT32 databits, INT32 stopbits, INT32 parity)
{
  struct termios options;

  if( tcgetattr( fd,&options)  !=  0) {
      perror("SetupSerial 1");
      return(FALSE);
    }
  options.c_cflag &= ~CSIZE; //字符长度掩码。取值为 CS5, CS6, CS7, 或 CS8。

  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input屏蔽操作*/
  options.c_oflag &= ~OPOST;   /*Output*/
  options.c_cflag |= CLOCAL | CREAD;
  options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  switch (databits){  /*设置数据位数*/
    case 5:
      options.c_cflag |= CS5;
      break;

    case 6:
      options.c_cflag |= CS6;
      break;

    case 7:
      options.c_cflag |= CS7;
      break;

    case 8:
      options.c_cflag |= CS8;
      break;

    default:
      printf("####Unsupported data size error####\n\n");
      return (FALSE);
    }
  switch (parity){
    case 'N':
      options.c_cflag &= ~PARENB;   /* Clear parity enable */
      options.c_iflag &= ~INPCK;     /* Enable parity checking */
      break;

    case 'O':
      options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
      options.c_iflag |= INPCK;
      break;

    case 'E':
      options.c_cflag |= PARENB;     /* Enable parity */
      options.c_cflag &= ~PARODD;   /* 转换为偶效验*/
      // options.c_iflag |= (INPCK | ISTRIP);       /* Disnable parity checking */
      options.c_iflag |= INPCK ;       /* Disnable parity checking */
      break;

    case 'S':
      options.c_cflag |= PARENB | CS8 | CMSPAR;
      break;

    case 'M':
    case 4://MARK校验
      options.c_cflag |= PARENB|CS8|CMSPAR|PARODD;
      break;

    default:
      printf("####Unsupported parity error####\n\n");
      return (FALSE);
    }
  /* 设置停止位*/
  switch (stopbits){
    case 1:
      options.c_cflag &= ~CSTOPB;
      break;

    case 2:
      options.c_cflag |= CSTOPB;
      break;

    default:
      printf("####Unsupported stop bits error####\n\n");
      return (FALSE);
    }

  tcflush(fd,TCIFLUSH);
  options.c_cc[VTIME] = 0; /* Issuedequipment_parameters设置超时15 seconds*/
  options.c_cc[VMIN] = 0; /* define the minimum bytes data to be readed*/

  if(tcsetattr(fd,TCSANOW,&options) != 0){
      perror("SetupSerial 3 error");
      return (FALSE);
    }

  return (TRUE);
}

/*************************************************************************
**description：*select读主要实现的功能是，在一定时间内不停地
*看串口有没有数据，有数据则进行读，当时间过
*去后还没有数据，则返回超时错误，Linux下直接
*用read读串口可能会造成堵塞，或数据读出错误。
*然而用select先查询com口，再用read去读就可以避免，
*并且当com口延时时，程序可以退出，这样就不至
*于由于com口堵塞，程序就死了
***parameter :fd     类型  int  打开的串口文件句柄
rcv_buf : 指向接收数据的指针地址
Len:要返回的数据长度
sec :设置等待时间秒数
usec  :设置等待时间微秒
**return		：正常返回实际接收的数据长度，其余返回FALSE
**************************************************************************/
INT32 read_datas_tty(INT32 fd, UINT8 *rcv_buf, INT32 Len, UINT32 sec, UINT32 usec)
{
  INT32 retval;
  UINT8 *pBuf;
  fd_set rfds;    // 文件的描述
  struct timeval tv;  //指定时间变量 在time.h（struct timeval 为类型 ）
  INT32 ret;

  INT32 pos = Len;

  tv.tv_sec = sec;//set the rcv wait time
  tv.tv_usec = usec;//100000us = 0.1s
  pBuf = rcv_buf;

  while(1){
      FD_ZERO(&rfds);  //清零
      FD_SET(fd, &rfds);  //将需要测试的fd加入

      tv.tv_sec = sec;//当收到数据后超时时间重新复位计时
      tv.tv_usec = usec;

      retval = select(fd+1, &rfds, NULL, NULL, &tv);  //测试
      if(retval == -1){
          printf("xxxxxxxxxxxxxxxxxxxxxxxselect()\n");
          break;
        }else if(retval){
          if(FD_ISSET(fd, &rfds)){ //先判断一下这外被监视的句柄是否真的变成可读的了
              ret= read(fd, pBuf, pos);   //ret= read(fd, pBuf, 1);
              if(ret > 0){//if(ret == 1){
                  pBuf = pBuf + ret;
                  pos  = pos - ret;
                  if(pos == 0){
                      break;
                    }
                }else{
                  break;
                }
            }else{
              break;
            }
        }else{
          break;
        }
    }

  //#if DEBUG_RS232
  //   printf("Count=%d, Len = %d, pBuf= ",(Len - pos), Len);
  //   for(i=0; i<pos; i++ )printf(" %0x",rcv_buf[i]);
  //   printf("\n");
  //#endif

  return (Len - pos);
}


/******************read and write series***********************/
/*******************write数据，如果一次就发送成功则返回原函数执行
 ***********如果连续执行最大次数都失败则返回错误************************/
INT32 write_datas_tty(INT32 fd, UINT8 *send_buf, INT32 Len)
{
  INT32 count = -1;
  INT32 i = 0;

  for(i = 0; i < 3; i++){
      count = write(fd, send_buf, Len);
      if(count == Len){
          return count;
        }
      usleep(1000);
    }
  return -1;
}

/**********read IP broadcoast netmask gateway API*********************/
INT32 sGet_Net_Mask( char *netMask)
{
  int sock;
  struct sockaddr_in sin;
  struct ifreq ifr;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock == -1){
      perror("socket");
      return -1;
    }
  strncpy(ifr.ifr_name, MAC_INTERFACE, IFNAMSIZ);/*这个是网卡的标识符*/
  ifr.ifr_name[IFNAMSIZ - 1] = 0;
  if(ioctl(sock, SIOCGIFNETMASK, &ifr)< 0){
      perror("ioctl");
      close(sock);
      return -1;
    }
  memcpy(&sin, &ifr.ifr_addr, sizeof(sin));

  sprintf(netMask, "%.15s",inet_ntoa(sin.sin_addr));
  //printf("ZWYM = %s\n",netMask);
  close(sock);
  return 0;
}

INT32 sGet_IP_Address(char *IPAddress)
{
  int sock;
  struct sockaddr_in sin;
  struct ifreq ifr;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock == -1){
      perror("socket");
      return -1;
    }

  strncpy(ifr.ifr_name, MAC_INTERFACE, IFNAMSIZ);/*这个是网卡的标识符*/

  ifr.ifr_name[IFNAMSIZ - 1] = 0;

  if (ioctl(sock, SIOCGIFADDR, &ifr) < 0){
      perror("ioctl");
      close(sock); //cwenfu 20120410
      return -1;
    }
  memcpy(&sin, &ifr.ifr_addr, sizeof(sin));

  sprintf(IPAddress, "%.15s",inet_ntoa(sin.sin_addr));
  // printf("IP Address = %s\n",IPAddress);
  close(sock);
  return 0;
}

INT32 sGet_GateWay( char *gateway)
{
  FILE *fp;
  char buf[512];
  char iface[16];
  unsigned long dest_addr, gate_addr;

  fp = fopen("/proc/net/route", "r");
  if(fp == NULL)  return -1;

  /* Skip title line */
  fgets(buf, sizeof(buf), fp);
  while(fgets(buf, sizeof(buf), fp)) {
      if(sscanf(buf, "%s\t%lX\t%lX", iface, &dest_addr, &gate_addr) != 3 || dest_addr != 0){
          continue;
        }

      sprintf(gateway, "%d.%d.%d.%d",(INT32)(gate_addr >> 0)&0x000000ff, (INT32)(gate_addr >> 8)&0x000000ff, (INT32)(gate_addr >> 16)&0x000000ff,(INT32)(gate_addr >> 24 )&0x000000ff);
      //printf("sxxxxxxxxxxxxxxxxxxxxxxxxxxGet_GateWay = %s\n",gateway);
      break;
    }

  fclose(fp);
  return 0;

}

/*************************************通用协议发送修改数据********************************************/
UINT32  Issuedequipment_parameters_06(UINT8 adress ,UINT16 Register , UINT16 pdata,UINT8 id)
{
  UINT16 wCRCCheckValue;
  INT32 j=0,ret=0;
  UART_M4_MSG msg;

  /****************功能码0x06设备参数设置数据******************************/
  msg.argv[fun_addr] = adress;     //addr  这个地址要加偏移量
  msg.argv[fun_fun] = 0x06;       //功能码
  msg.argv[fun_regh] = Register / 256;  //起始地址高字节
  msg.argv[fun_regl] = Register % 256;  //起始地址低字节，起始地址0
  msg.argv[fun_lenh] = pdata / 256;     //寄存器值高位
  msg.argv[fun_lenl] = pdata % 256;     //寄存器值低位

  wCRCCheckValue = ModbusCRC(&msg.argv[fun_addr], 6);  //Modbus 协议crc校验
  msg.argv[fun_crch] = wCRCCheckValue >> 8;
  msg.argv[fun_crcl] = wCRCCheckValue & 0x00ff;

  //发送消息给 串口id
  msg.type = id; //m4的串口通道号
  msgsnd(Globa.uart_m4_msg_sent_id, &msg, fun_crcl+1, IPC_NOWAIT); //（消息队列发送消息时候）包括了通道号，数据
  //收取串口 id 的该通道消息
  for(j = 0; j < 10; j++){//最多等待 1s
      ret = msgrcv(Globa.uart_m4_set_msg_recv_id, &msg, sizeof(UART_M4_MSG)-sizeof(long), id, IPC_NOWAIT);
      if(ret > 5){
          if((msg.argv[fun_addr] == adress)&&(msg.argv[fun_fun] == 0x06))
            {
              return 1;
            }
        }
      usleep(100*1000); //100ms
    }

  return 0;
}


/*************************************通用协议发送修改数据********************************************/
UINT32 Issuedequipment_parameters_10(UINT8 adress ,UINT16 Register , UINT16 pdata,UINT8 id)
{
  UINT16 wCRCCheckValue;
  INT32 j=0,ret=0;
  UART_M4_MSG msg;

  /****************功能码0x06设备参数设置数据******************************/
  msg.argv[fun_addr] = adress;     //addr  这个地址要加偏移量
  msg.argv[fun_fun] = 0x10;       //功能码
  msg.argv[fun_regh] = Register / 256;  //起始地址高字节
  msg.argv[fun_regl] = Register % 256;  //起始地址低字节，起始地址0
  msg.argv[fun_lenh] = 0;     //寄存器值高位
  msg.argv[fun_lenl] = 1;     //寄存器值低位
  msg.argv[fun_lenl+1] = 0x02;
  msg.argv[fun_lenl+2] = pdata / 256;
  msg.argv[fun_lenl+3] = pdata % 256;

  wCRCCheckValue = ModbusCRC(&msg.argv[fun_addr], 9);  //Modbus 协议crc校验
  msg.argv[fun_lenl+4] = wCRCCheckValue >> 8;
  msg.argv[fun_lenl+5] = wCRCCheckValue & 0x00ff;

  //发送消息给 串口id
  msg.type = id; //m4的串口通道号
  msgsnd(Globa.uart_m4_msg_sent_id, &msg, fun_lenl+6, IPC_NOWAIT); //（消息队列发送消息时候）包括了通道号，数据
  //收取串口 id 的该通道消息
  for(j = 0; j < 10; j++){//最多等待 1s
      ret = msgrcv(Globa.uart_m4_set_msg_recv_id, &msg, sizeof(UART_M4_MSG)-sizeof(long), id, IPC_NOWAIT);
      if(ret > 5){

          if((msg.argv[fun_addr] == adress)&&(msg.argv[fun_fun] == 0x10))
            {
              for (int ii = 0; ii < fun_lenl+6; ii++)
                {
                  printf("msg.argv[%d] = %d\n", ii, msg.argv[ii]);
                }
            }

          if((msg.argv[fun_addr] == adress)&&(msg.argv[fun_fun] == 0x10))
            {
              return 1;
            }
        }
      usleep(100*1000); //100ms
    }

  return 0;
}



/*************************************插入操作记录以及工作记录********************************************/
void Insert_Operation_Work_Record(UINT32 Type, QString Record_Name)
{
  bool exis_flag = false;
  QDateTime time = QDateTime::currentDateTime();
  QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
  OPERATION_WORK_RECORD ls_history_Record;
  ls_history_Record.name = Record_Name;
  ls_history_Record.time = time_str;

  if(Type == 1)
    {//插入操作记录
      QFileInfo info(HISTORY_OPERATION_RECORD_DB);
      exis_flag =info.exists();
      {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Insert_Read_history_operation");
        db.setDatabaseName(HISTORY_OPERATION_RECORD_DB);
        if(db.open()){
            QSqlQuery  query(db);
            if(!exis_flag)
              {
                query.exec("create table history_Operation_Record(name TEXT, time TEXT)");
              }

            if(MAX_LIST_COUNT<=history_Operation_Record.count()){
              query.exec(QString("delete from history_Operation_Record where rowid in (select rowid from history_Operation_Record limit %1)").arg(history_Operation_Record.count()- MAX_LIST_COUNT + 1));
            }
            query.exec(QString("insert into history_Operation_Record values('%1','%2')").arg(Record_Name).arg(time_str));
            query.clear();
            db.close();
          }
      }
      QSqlDatabase::removeDatabase("Insert_Read_history_operation");

      history_Operation_Record_mutex.lockForWrite();

      while(MAX_LIST_COUNT<=history_Operation_Record.count()){
        history_Operation_Record.removeLast();
      }
      history_Operation_Record.insert(0,ls_history_Record);
      history_Operation_Record_mutex.unlock();
    }
  else if(Type == 0)
    {//插入EPS工作状态
      QFileInfo info(HISTORY_EPSWORK_RECORD_DB);
      exis_flag =info.exists();
      {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","EpsWork_Read_history_EpsWork");
        db.setDatabaseName(HISTORY_EPSWORK_RECORD_DB);
        if(db.open())
          {
            QSqlQuery  query(db);
            if(!exis_flag)
              {
                query.exec("create table history_EpsWork_Record(name TEXT, time TEXT)");
              }

            if(MAX_LIST_COUNT<=history_EpsWork_Record.count()){

              query.exec(QString("delete from history_EpsWork_Record where rowid in (select rowid from history_EpsWork_Record limit %1)").arg(history_EpsWork_Record.count()- MAX_LIST_COUNT + 1));
            }

            query.exec(QString("insert into history_EpsWork_Record values('%1','%2')").arg(Record_Name).arg(time_str));
            query.clear();
            db.close();
          }
      }
      QSqlDatabase::removeDatabase("EpsWork_Read_history_EpsWork");

      history_EpsWork_Record_mutex.lockForWrite();

      while(MAX_LIST_COUNT<=history_EpsWork_Record.count()){
        history_EpsWork_Record.removeLast();
      }
      history_EpsWork_Record.insert(0,ls_history_Record);
      history_EpsWork_Record_mutex.unlock();
    }
}
