/*****************************************

  NRF24L01射频模块头文件

*****************************************/
#ifndef _NRF24L01_H_
#define _NRF24L01_H_
/*******************************************************/
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

#define READ_REG       0x00 		// 读寄存器指令
#define WRITE_REG      0x20 		// 写寄存器指令
#define RD_RX_PLOAD    0x61 	    // 读取接收数据指令
#define WR_TX_PLOAD    0xA0 	    // 写待发数据指令
#define FLUSH_TX       0xE1 	 	// 冲洗发送FIFO指令
#define FLUSH_RX       0xE2 		// 冲洗接收 FIFO指令
#define REUSE_TX_PL    0xE3 	    // 定义重复装载数据指令
#define NOP            0xFF 		// 保留
/******************寄存器地址****************************/
#define CONFIG         0x00 		// 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA          0x01 		// 自动应答功能设置
#define EN_RXADDR      0x02 		// 可用信道设置
#define SETUP_AW       0x03 		// 收发地址宽度设置
#define SETUP_RETR     0x04 	    // 自动重发功能设置
#define RF_CH          0x05 		// 工作频率设置
#define RF_SETUP       0x06 		// 发射速率、功耗功能设置
#define STATUS 		   0x07 		// 状态寄存器
#define OBSERVE_TX 	   0x08 	    // 发送监测功能
#define CD 			   0x09 		// 地址检测
#define RX_ADDR_P0 	   0x0A 	    // 频道0接收数据地址
#define RX_ADDR_P1 	   0x0B      	// 频道1接收数据地址
#define RX_ADDR_P2     0x0C     	// 频道2接收数据地址
#define RX_ADDR_P3     0x0D     	// 频道3接收数据地址
#define RX_ADDR_P4     0x0E 	    // 频道4接收数据地址
#define RX_ADDR_P5     0x0F 	    // 频道5接收数据地址
#define TX_ADDR        0x10 		// 发送地址寄存器
#define RX_PW_P0       0x11 		// 接收频道0接收数据长度
#define RX_PW_P1       0x12 		// 接收频道0接收数据长度
#define RX_PW_P2       0x13 		// 接收频道0接收数据长度
#define RX_PW_P3       0x14 		// 接收频道0接收数据长度
#define RX_PW_P4       0x15 		// 接收频道0接收数据长度
#define RX_PW_P5       0x16 	    // 接收频道0接收数据长度
#define FIFO_STATUS    0x17 	    // FIFO栈入栈出状态寄存器设置

#define TX_ADR_WIDTH    5 		
#define RX_ADR_WIDTH    5 		
#define TX_PLOAD_WIDTH  8   
#define RX_PLOAD_WIDTH  8  

sbit MISO = P1^6;
sbit MOSI = P1^1;
sbit SCK = P1^7;
sbit CE = P1^2;
sbit CSN = P1^3;
sbit IRQ = P1^4;

//void delay_us(uint i);   //误差 0us
uchar SPI_RW(uchar byte);
uchar SPI_RW_Reg(uchar reg, uchar value);
uchar SPI_Read(uchar reg);
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bytes);
uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar bytes);
void TxPacket(uchar * tx_buf);
void RX_Mode();

uchar RxPacket(uchar * rx_buf);

uchar CheckACK();
void init_NRF24L01(void);

#endif