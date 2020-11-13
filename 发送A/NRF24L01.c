#include "NRF24L01.h"
#include "delay.h"

uchar bdata sta; //状态标志
sbit RX_DR = sta^6;
sbit TX_DS = sta^5;
sbit MAX_RT = sta^4;

uchar code TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x45,0x10,0x10,0x01}; //本地地址

uchar SPI_RW(uchar byte)
{
	uchar bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // 输出8位
	{
		MOSI = (byte & 0x80); 	//   获得byte的最高位数据
		byte = (byte << 1); 	// 
		SCK = 1; 				// 
		byte |= MISO; 			// 获得从MISO获得的一位数据
		SCK = 0; 				// 
	}
	return(byte);				//	最后返回从MISO读出的数据 				
}

/*读写寄存器函数*/
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;
	CSN = 0;              // CSN拉低，启动SPI通信
	status = SPI_RW(reg); //返回从MISO读出的数据,status应为上次向该寄存器内写的value
	SPI_RW(value);        // 向寄存器中写入数据
	CSN = 1; 			  // CSN拉高，终止SPI通信
	return(status);       // 返回2401 reg寄存器的状态
}
/*************************读出从机相关寄存器的数据*******************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
	CSN = 0; 		     
	SPI_RW(reg);         
	reg_val = SPI_RW(0); 
	CSN = 1;             
	return(reg_val);     
}

/*用于写数据，reg为寄存器地址 pBuf为待写入数据地址 bytes为写入数据的个数*/
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bytes)
{
	uchar status,byte_ctr;
	CSN = 0; // SPI使能
	status = SPI_RW(reg);
	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)
	{
		SPI_RW(*pBuf++);//指向下一个数据
	} 
	CSN = 1; 
	return(status); // 返回24l01的状态
}
/*用于读数据，reg为寄存器地址，pBuf为待读出数据地址，bytes为读出数据的个数*/
uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar bytes)
{
	uchar status, byte_ctr;
	CSN = 0;
	status = SPI_RW(reg);
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
		pBuf[byte_ctr] = SPI_RW(0);
	CSN = 1;
	return(status);

}
/*发送tx_buf中的数据*/
void TxPacket(uchar * tx_buf)
{
	CE=0;
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);			//StandBy I模式	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  频道0自动	ACK应答允许	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21
	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x0a);  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0x40);        //   设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07); 			 // 装载数据	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
	CE=1;		 //置高CE，激发数据发送
	delay_us(10);
}
/*设定24L01为接收方式*/
void RX_Mode(void)
{
	CE=0;
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  频道0自动	ACK应答允许	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0x40);        //   设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为4字节
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f); // Set PWR_UP bit, enable CRC(2 bytes)
	
	CE = 1; 
}

/*数据读取后放如rx_buf接收缓冲区中*/
uchar RxPacket(uchar * rx_buf)
{
	unsigned char RevFlags=0;
	sta=SPI_Read(READ_REG+STATUS);//发送数据后读取状态寄存器
	if(RX_DR)				// 判断是否接收到数据
	{
		//RevFlags=(sta&0x0e);	   //读取数据完成标志0x00/0x02
		CE=0; 			//SPI使能
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);//break;  // 从RXFIFO读取数据
		RevFlags=1;	   //读取数据完成标志
		
		//改为下面注释语句会造成时而接收时而不能且只能接收一次
		CSN=0;
		SPI_RW(FLUSH_RX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！ 
		CSN=1;
		SPI_RW_Reg(WRITE_REG +STATUS,0xff); //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标
		//
	}
	//SPI_RW_Reg(WRITE_REG+STATUS,0xff); //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标
	return(RevFlags);
}

uchar CheckACK()
{
	sta = SPI_Read(READ_REG + STATUS);
	if(TX_DS || MAX_RT)
	{
		SPI_RW_Reg(WRITE_REG + STATUS, 0xff);
		CSN = 0;
		SPI_RW(FLUSH_TX);
		CSN = 1;
		return 0;
	}
	else
	return 1;
}

void init_NRF24L01(void)
{
	delay_us(100);	
	CE=0;	//芯片使能
	CSN=1; // spi失能（禁止24l01的spi功能）
	SCK=0; // Spi时钟线初始化
	IRQ = 1;
	
}	
