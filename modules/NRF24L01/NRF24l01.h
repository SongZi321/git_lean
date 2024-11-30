
#ifndef __NRF24L01_H
#define __NRF24L01_H
#include "bsp_spi.h"
#include "gpio.h"
//NRF24L01 ��������
/****************************************************************************************************/
//NRF24L01�Ĵ�����������
#define SPI_READ_REG    0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define SPI_WRITE_REG   0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���	 
//SPI(NRF24L01)�Ĵ�����ַ
#define CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
//bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
//bit5:���ݷ�������ж�;bit6:���������ж�;
#define MAX_TX  	    0x10  //�ﵽ����ʹ����ж�
#define TX_OK       	0x20  //TX��������ж�
#define RX_OK   	    0x40  //���յ������ж�

#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define FIFO_STATUS     0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
//bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;
/**********************************************************************************************************/
//NRF24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5                               //5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5                               //5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32                              //32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32                              //32�ֽڵ��û����ݿ��
#define MAX_TIME_INTO_IDEL		10						//����IDELģʽ�����ʱ�䣬��λ��S
#define CH_Num	40
#define debug_out(fmt,args...)  printf(fmt,##args)
/**********************************************************************************************************/
/*  spi����ʹ��  */
typedef struct NRF24l01_t nrf24l01_t; 
typedef enum
{
	NRF24L01_CE_EN=0,   //ʹ�ܿ���ͨ��
  NRF24L01_CE_DIS   //ʧ�ܲ���ͨ��
	
}Nrf24l01_CE;

/*  Ƭѡ���ͺ�̧��  */
typedef enum
{
	NRF24L01_CSN_LOW=0,    //Ƭѡ�޺����ͣ�����ͨ��
	NRF24L01_CSN_HIGH      //Ƭѡ�ź����ߣ��Ͼ�ͨ��
	
}Nrf24l01_CSN;

/* NRF24L01���շ���ģʽ  */
typedef enum
{ 
	
	NRF24L01_CSN_TX=0,	//����ģʽ
	NRF24L01_CSN_RX,    //����ģʽ
	NRF24L01_CSN_RXTX   //���ͽ���ģʽ
	
}Nrf24l01_MODE;




/* �˽ṹ������װ�õĽṹ��ͺ���ָ�� */
 struct NRF24l01_t{
	SPI_t* spi_t;
  uint8_t (*nrf24_spi_send_byte)(nrf24l01_t* nrf,uint8_t txData);/*����HAL���SPI��д�ֽں���*/
  uint8_t (*nrf24l01_check)(nrf24l01_t* nrf);                     /*�ϵ���NRF24L01�Ƿ���λ*/
	uint8_t (*nrf24l01getiqrf)(nrf24l01_t* nrf);                   /*ʵ��Ready״̬����*/
  void    (*nrf_ce_enable)(nrf24l01_t* nrf, Nrf24l01_CE ce);     /* ʹ��spi�ź� ʹ�ܺ��������ͨ��  */
  void    (*nrf_csn_enable)(nrf24l01_t* nrf,Nrf24l01_CSN csn);   /*  ��������Ƭѡ�ź���ʵ�ֶ�ָ�������ȡ  */
	
};

typedef uint8_t(*Nrf24l01getiqrf)(nrf24l01_t* nrf);                   /*ʵ��Ready״̬����*/
typedef uint8_t (*Nrf24_spi_send_byte)(nrf24l01_t* nrf,uint8_t txData);/*����HAL���SPI��д�ֽں���*/
typedef uint8_t (*Nrf24l01_check)(nrf24l01_t* nrf);                    /*�ϵ���NRF24L01�Ƿ���λ*/
typedef void    (*Nrf_ce_enable)(nrf24l01_t* nrf, Nrf24l01_CE ce);     /* ʹ��spi�ź� ʹ�ܺ��������ͨ��  */
typedef void    (*Nrf_csn_enable)(nrf24l01_t* nrf,Nrf24l01_CSN csn);   /*  ��������Ƭѡ�ź���ʵ�ֶ�ָ�������ȡ  */


//��װspi��д����
uint8_t nRF24_SPI_Send_Byte(nrf24l01_t* nrf,uint8_t txData);

/** δ����ն˼���ʧ�� **/
uint8_t NRF24L01_Check(nrf24l01_t* nrf);

/* ʹ��spi�ź� ʹ�ܺ��������ͨ��  */
void NRF_CE_Enable(nrf24l01_t* nrf, Nrf24l01_CE ce);

/*  ��������Ƭѡ�ź���ʵ�ֶ�ָ�������ȡ  */
void NRF_CSN_Enable(nrf24l01_t* nrf,Nrf24l01_CSN csn);

/*ʵ��Ready״̬����*/
uint8_t NRF24L01GetIRQf(nrf24l01_t* nrf);


//�ú�����ʼ��NRF24L01��RXģʽ
//�ú�����ʼ��NRF24L01��TXģʽ
void Nrf24l01_Set_Mode(nrf24l01_t* nrf,Nrf24l01_MODE mode);

/* ����һ���������� */
uint8_t NRF24L01_TxPacket(nrf24l01_t* nrf,uint8_t *txbuf);   

/* ����һ���������� */
uint8_t NRF24L01_RxPacket(nrf24l01_t* nrf,uint8_t *rxbuf);                                  




/**   ��ʼ������     **/
void Nrf24l01_init(nrf24l01_t*          nrf,
	                 SPI_t*               spi,
	                 Nrf24l01getiqrf      iqr,
                   Nrf24_spi_send_byte  send,
                   Nrf24l01_check       chick,
                   Nrf_ce_enable        ce,
                   Nrf_csn_enable       cs);





#endif



