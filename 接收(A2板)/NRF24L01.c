/*****************************************

NRF24L01射频模块C文件(节点接收部分)

*****************************************/
#include"reg52.h"
#include"NRF24L01.h"
#include"Delay.h"

uchar bdata sta; //状态标志
sbit RX_DR = sta^6;
sbit TX_DS = sta^5;
sbit MAX_RT = sta^4;

uchar code RX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x45,0x10,0x10,0x01};
uchar code RX_ADDRESS1[RX_ADR_WIDTH] = {0x32,0x45,0x10,0x10,0x01}; //接收地址


uchar SPI_RW(uchar byte)//unsigned char NRFSPI(unsigned char date)
{
	uchar bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // 输出8位
	{
		if(byte & 0x80)
			MOSI = 1;
		else
			MOSI = 0;
		byte = (byte << 1); 	// 
		SCK = 1; 
		if(MISO)				// 
			byte |= 0x01; 			// 获得从MISO获得的一位数据
		SCK = 0; 				// 
	}
	return(byte);				//	最后返回从MISO读出的数据 				
}

/*读写寄存器函数*/
uchar SPI_RW_Reg(uchar reg, uchar value)//unsigned char NRFReadReg(unsigned char RegAddr)
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
	SPI_RW(reg);   //写寄存器地址      
	reg_val = SPI_RW(0); //写入读寄存器指令
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


//void TxPacket(uchar * tx_buf)
//{
//	CE=0;			//StandBy I模式	
//  SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	 //写寄存器指令+接收地址使能指令+接收地址+地址宽度	
//	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, RX_ADR_WIDTH); //为了应答接收设备，接收通道0地址和发送地址相同
//	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);		  //写入数据
//	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);       // 使能接收通道0自动应答
//	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);   // 使能接收通道0
//	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x0a);  // 自动重发延时等待250us+86us，自动重发50次 
//	SPI_RW_Reg(WRITE_REG + RF_CH, 0x40);       // 选择射频通道0x40
//	//SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);  //设置接收数据长度，本次设置为4字节
//	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07); 			 // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益	
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		     // CRC使能，16位CRC校验，上电 
//	CE=1;		 //置高CE，激发数据发送
//	delay_us(10);
//}

void RX_Mode(void)
{
	CE=0;
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);  // 接收设备接收通道0使用和发送设备相同的发送地址
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P1, RX_ADDRESS1, RX_ADR_WIDTH); // 接收设备接收通道1使用和发送设备相同的发送地址 
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); // 接收通道0选择和发送通道相同有效数据宽度
	SPI_RW_Reg(WRITE_REG + RX_PW_P1, RX_PLOAD_WIDTH); // 接收通道1选择和发送通道相同有效数据宽度
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x03);     // 使能接收通道0,1自动应答      
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x03); // 使能接收通道0,1     
	SPI_RW_Reg(WRITE_REG + RF_CH, 0x40);	 // 选择射频通道0x40
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);  // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益*/
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f); 	 // CRC使能，16位CRC校验，上电，接收模式
	CE = 1; 
	DelayMs(100);
}


/*数据读取后放如rx_buf接收缓冲区中*/


uchar RxPacket(uchar * rx_buf)
{
	uchar RevFlags = 5;  //接收通道号
	sta = SPI_Read(READ_REG + STATUS);//发送数据后读取状态寄存器
	if(RX_DR)				// 判断是否接收到数据
	{
		RevFlags = (sta & 0x0e);	   //读取数据完成标志0x00/0x02
		CE = 0; 			//SPI使能
		SPI_Read_Buf(RD_RX_PLOAD, rx_buf, RX_PLOAD_WIDTH);//break;  // 从RXFIFO读取数据	
		CSN = 0;
		SPI_RW(FLUSH_RX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！ 
		CSN = 1;	  
		SPI_RW_Reg(WRITE_REG + STATUS, sta); //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标		
	}			
	SPI_RW_Reg(WRITE_REG + STATUS, sta); 	
	return(RevFlags);
}


uchar CheckACK()
{
	sta = SPI_Read(READ_REG + STATUS);
	if(TX_DS || MAX_RT)	  //发送完毕中断
	{
		SPI_RW_Reg(WRITE_REG + STATUS, 0xff);  // 清除TX_DS或MAX_RT中断标志
		CSN = 0;
		SPI_RW(FLUSH_TX); //用于清空FIFO 
		CSN = 1;
		return 0;
	}
	else
	return 1;
}
void init_NRF24L01(void)
{
	DelayMs(100);	
	CE = 0;	//芯片使能
	CSN = 1; // spi失能（禁止24l01的spi功能）
	SCK = 0; // Spi时钟线初始化
	IRQ = 1;
	        

}

