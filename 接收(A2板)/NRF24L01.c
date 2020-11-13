/*****************************************

NRF24L01��Ƶģ��C�ļ�(�ڵ���ղ���)

*****************************************/
#include"reg52.h"
#include"NRF24L01.h"
#include"Delay.h"

uchar bdata sta; //״̬��־
sbit RX_DR = sta^6;
sbit TX_DS = sta^5;
sbit MAX_RT = sta^4;

uchar code RX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x45,0x10,0x10,0x01};
uchar code RX_ADDRESS1[RX_ADR_WIDTH] = {0x32,0x45,0x10,0x10,0x01}; //���յ�ַ


uchar SPI_RW(uchar byte)//unsigned char NRFSPI(unsigned char date)
{
	uchar bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // ���8λ
	{
		if(byte & 0x80)
			MOSI = 1;
		else
			MOSI = 0;
		byte = (byte << 1); 	// 
		SCK = 1; 
		if(MISO)				// 
			byte |= 0x01; 			// ��ô�MISO��õ�һλ����
		SCK = 0; 				// 
	}
	return(byte);				//	��󷵻ش�MISO���������� 				
}

/*��д�Ĵ�������*/
uchar SPI_RW_Reg(uchar reg, uchar value)//unsigned char NRFReadReg(unsigned char RegAddr)
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
	SPI_RW(reg);   //д�Ĵ�����ַ      
	reg_val = SPI_RW(0); //д����Ĵ���ָ��
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


//void TxPacket(uchar * tx_buf)
//{
//	CE=0;			//StandBy Iģʽ	
//  SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	 //д�Ĵ���ָ��+���յ�ַʹ��ָ��+���յ�ַ+��ַ���	
//	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, RX_ADR_WIDTH); //Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
//	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);		  //д������
//	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);       // ʹ�ܽ���ͨ��0�Զ�Ӧ��
//	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);   // ʹ�ܽ���ͨ��0
//	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x0a);  // �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�50�� 
//	SPI_RW_Reg(WRITE_REG + RF_CH, 0x40);       // ѡ����Ƶͨ��0x40
//	//SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);  //���ý������ݳ��ȣ���������Ϊ4�ֽ�
//	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07); 			 // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������	
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		     // CRCʹ�ܣ�16λCRCУ�飬�ϵ� 
//	CE=1;		 //�ø�CE���������ݷ���
//	delay_us(10);
//}

void RX_Mode(void)
{
	CE=0;
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);  // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P1, RX_ADDRESS1, RX_ADR_WIDTH); // �����豸����ͨ��1ʹ�úͷ����豸��ͬ�ķ��͵�ַ 
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
	SPI_RW_Reg(WRITE_REG + RX_PW_P1, RX_PLOAD_WIDTH); // ����ͨ��1ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x03);     // ʹ�ܽ���ͨ��0,1�Զ�Ӧ��      
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x03); // ʹ�ܽ���ͨ��0,1     
	SPI_RW_Reg(WRITE_REG + RF_CH, 0x40);	 // ѡ����Ƶͨ��0x40
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);  // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������*/
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f); 	 // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	CE = 1; 
	DelayMs(100);
}


/*���ݶ�ȡ�����rx_buf���ջ�������*/


uchar RxPacket(uchar * rx_buf)
{
	uchar RevFlags = 5;  //����ͨ����
	sta = SPI_Read(READ_REG + STATUS);//�������ݺ��ȡ״̬�Ĵ���
	if(RX_DR)				// �ж��Ƿ���յ�����
	{
		RevFlags = (sta & 0x0e);	   //��ȡ������ɱ�־0x00/0x02
		CE = 0; 			//SPIʹ��
		SPI_Read_Buf(RD_RX_PLOAD, rx_buf, RX_PLOAD_WIDTH);//break;  // ��RXFIFO��ȡ����	
		CSN = 0;
		SPI_RW(FLUSH_RX);//�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס���� 
		CSN = 1;	  
		SPI_RW_Reg(WRITE_REG + STATUS, sta); //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�		
	}			
	SPI_RW_Reg(WRITE_REG + STATUS, sta); 	
	return(RevFlags);
}


uchar CheckACK()
{
	sta = SPI_Read(READ_REG + STATUS);
	if(TX_DS || MAX_RT)	  //��������ж�
	{
		SPI_RW_Reg(WRITE_REG + STATUS, 0xff);  // ���TX_DS��MAX_RT�жϱ�־
		CSN = 0;
		SPI_RW(FLUSH_TX); //�������FIFO 
		CSN = 1;
		return 0;
	}
	else
	return 1;
}
void init_NRF24L01(void)
{
	DelayMs(100);	
	CE = 0;	//оƬʹ��
	CSN = 1; // spiʧ�ܣ���ֹ24l01��spi���ܣ�
	SCK = 0; // Spiʱ���߳�ʼ��
	IRQ = 1;
	        

}

