#include "NRF24L01.h"
#include "delay.h"

uchar bdata sta; //״̬��־
sbit RX_DR = sta^6;
sbit TX_DS = sta^5;
sbit MAX_RT = sta^4;

uchar code TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x45,0x10,0x10,0x01}; //���ص�ַ

uchar SPI_RW(uchar byte)
{
	uchar bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // ���8λ
	{
		MOSI = (byte & 0x80); 	//   ���byte�����λ����
		byte = (byte << 1); 	// 
		SCK = 1; 				// 
		byte |= MISO; 			// ��ô�MISO��õ�һλ����
		SCK = 0; 				// 
	}
	return(byte);				//	��󷵻ش�MISO���������� 				
}

/*��д�Ĵ�������*/
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;
	CSN = 0;              // CSN���ͣ�����SPIͨ��
	status = SPI_RW(reg); //���ش�MISO����������,statusӦΪ�ϴ���üĴ�����д��value
	SPI_RW(value);        // ��Ĵ�����д������
	CSN = 1; 			  // CSN���ߣ���ֹSPIͨ��
	return(status);       // ����2401 reg�Ĵ�����״̬
}
/*************************�����ӻ���ؼĴ���������*******************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
	CSN = 0; 		     
	SPI_RW(reg);         
	reg_val = SPI_RW(0); 
	CSN = 1;             
	return(reg_val);     
}

/*����д���ݣ�regΪ�Ĵ�����ַ pBufΪ��д�����ݵ�ַ bytesΪд�����ݵĸ���*/
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bytes)
{
	uchar status,byte_ctr;
	CSN = 0; // SPIʹ��
	status = SPI_RW(reg);
	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)
	{
		SPI_RW(*pBuf++);//ָ����һ������
	} 
	CSN = 1; 
	return(status); // ����24l01��״̬
}
/*���ڶ����ݣ�regΪ�Ĵ�����ַ��pBufΪ���������ݵ�ַ��bytesΪ�������ݵĸ���*/
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
/*����tx_buf�е�����*/
void TxPacket(uchar * tx_buf)
{
	CE=0;
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);			//StandBy Iģʽ	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21
	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x0a);  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0x40);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07); 			 // װ������	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	CE=1;		 //�ø�CE���������ݷ���
	delay_us(10);
}
/*�趨24L01Ϊ���շ�ʽ*/
void RX_Mode(void)
{
	CE=0;
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0x40);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ4�ֽ�
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f); // Set PWR_UP bit, enable CRC(2 bytes)
	
	CE = 1; 
}

/*���ݶ�ȡ�����rx_buf���ջ�������*/
uchar RxPacket(uchar * rx_buf)
{
	unsigned char RevFlags=0;
	sta=SPI_Read(READ_REG+STATUS);//�������ݺ��ȡ״̬�Ĵ���
	if(RX_DR)				// �ж��Ƿ���յ�����
	{
		//RevFlags=(sta&0x0e);	   //��ȡ������ɱ�־0x00/0x02
		CE=0; 			//SPIʹ��
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);//break;  // ��RXFIFO��ȡ����
		RevFlags=1;	   //��ȡ������ɱ�־
		
		//��Ϊ����ע���������ʱ������ʱ��������ֻ�ܽ���һ��
		CSN=0;
		SPI_RW(FLUSH_RX);//�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס���� 
		CSN=1;
		SPI_RW_Reg(WRITE_REG +STATUS,0xff); //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�
		//
	}
	//SPI_RW_Reg(WRITE_REG+STATUS,0xff); //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�
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
	CE=0;	//оƬʹ��
	CSN=1; // spiʧ�ܣ���ֹ24l01��spi���ܣ�
	SCK=0; // Spiʱ���߳�ʼ��
	IRQ = 1;
	
}	
