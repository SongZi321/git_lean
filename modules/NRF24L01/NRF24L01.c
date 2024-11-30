#include "NRF24L01.h"
#include "main.h"
#include "spi.h"
#include "stdio.h"
/********    �ӻ�����     *******/
//NRF24L01 ��������

unsigned char idel_mode_flag = 0;
unsigned char mode_time_counter = 0;

const unsigned char INIT_ADDR0[5]= {0x02,0x3A,0xB1,0xB1,0x01};
const unsigned char INIT_ADDR1[5]= {0x02,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR2[5]= {0x03,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR3[5]= {0x04,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR4[5]= {0x05,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR5[5]= {0x06,0x3A,0x01,0x01,0x01};


//ͨ��SPIд�Ĵ���
static uint8_t NRF24L01_Write_Reg(nrf24l01_t* nrf,uint8_t regaddr,uint8_t  data);

//��ȡSPI�Ĵ���ֵ ��regaddr:Ҫ���ļĴ���
static uint8_t NRF24L01_Read_Reg(nrf24l01_t* nrf,uint8_t regaddr);


//��ָ��λ�ö���ָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
static uint8_t NRF24L01_Read_Buf(nrf24l01_t* nrf,uint8_t regaddr,uint8_t* pBuf,uint8_t datalen);

//��ָ��λ��дָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
static uint8_t NRF24L01_Write_Buf(nrf24l01_t* nrf,uint8_t regaddr, uint8_t *pBuf,uint8_t datalen);



//��װspi��д����
uint8_t nRF24_SPI_Send_Byte(nrf24l01_t* nrf,uint8_t txData)
{
	uint8_t rxdata;
  rxdata=nrf->spi_t->Transrecv(nrf->spi_t->spiinstance,&txData,SPI_BLACK_MODE);
	return(rxdata);							// return read unsigned char
}


//ͨ��SPIд�Ĵ���
uint8_t NRF24L01_Write_Reg(nrf24l01_t* nrf,uint8_t regaddr,uint8_t  data)
{
	uint8_t status;
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_LOW);//ʹ��SPI����                  
	status =nrf->nrf24_spi_send_byte (nrf,regaddr);   //���ͼĴ�����
	nrf->nrf24_spi_send_byte (nrf,data);           //д��Ĵ�����ֵ                 
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_HIGH);  //��ֹSPI����
	return(status);       		         //����״ֵ̬
}


//��ȡSPI�Ĵ���ֵ ��regaddr:Ҫ���ļĴ���
uint8_t NRF24L01_Read_Reg(nrf24l01_t* nrf,uint8_t regaddr)
{
	uint8_t reg_val;
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_LOW);                //ʹ��SPI����
	nrf->nrf24_spi_send_byte (nrf,regaddr);      //���ͼĴ�����
	reg_val=nrf->nrf24_spi_send_byte (nrf,0XFF);//��ȡ�Ĵ�������
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_HIGH);               //��ֹSPI����
	return(reg_val);                 //����״ֵ̬
}


//��ָ��λ�ö���ָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
uint8_t NRF24L01_Read_Buf(nrf24l01_t* nrf,uint8_t regaddr,uint8_t* pBuf,uint8_t datalen)
{
  uint8_t status;
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_LOW); //ʹ��SPI����
	status=nrf->nrf24_spi_send_byte (nrf,regaddr);   //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for(int i=0;i<=datalen;i++)
	{
  	pBuf[i]=nrf->nrf24_spi_send_byte(nrf,0XFF);//��ȡ�Ĵ�������
	}
//	nrf->spi_t->spireceive(nrf->spi_t->spiinstance ,pBuf,datalen,SPI_BLACK_MODE);                 
	NRF_CSN_Enable( nrf,NRF24L01_CSN_HIGH); //�ر�SPI����
	return status;                        //���ض�����״ֵ̬
}


//��ָ��λ��дָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
uint8_t NRF24L01_Write_Buf(nrf24l01_t* nrf,uint8_t regaddr, uint8_t *pBuf,uint8_t datalen)
{
	uint8_t status;
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_LOW); //ʹ��SPI����
	status = nrf->nrf24_spi_send_byte (nrf,regaddr);                //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for(int i=0;i<=datalen;i++)
	{
  	nrf->nrf24_spi_send_byte (nrf,pBuf[i]);//��ȡ�Ĵ�������
	}
	//nrf->spi_t->spitransmit(nrf->spi_t->spiinstance ,pBuf,datalen,SPI_BLACK_MODE);   
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_HIGH);   //�ر�SPI����
	return status;                                       //���ض�����״ֵ̬
}


//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
uint8_t NRF24L01_TxPacket(nrf24l01_t* nrf,uint8_t *txbuf)
{
	uint8_t state;
	Nrf24l01_Set_Mode(nrf,NRF24L01_CSN_TX);
	
	nrf->nrf_ce_enable(nrf,NRF24L01_CE_DIS);
	NRF24L01_Write_Buf(nrf,WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
	nrf->nrf_ce_enable(nrf,NRF24L01_CE_EN);  
	
	while(nrf->nrf24l01getiqrf(nrf)!=0)  printf("2\n\r");                         //�ȴ��������
	state=NRF24L01_Read_Reg(nrf,STATUS);                     //��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+STATUS,state);      //���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_TX)                                     //�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(nrf,FLUSH_TX,0xff);               //���TX FIFO�Ĵ���
		printf("TX MAX_TX error!\r\n");
		return MAX_TX;
	}
	if(state&TX_OK)                                      //�������
	{
		printf("TX OK!\r\n");
		return TX_OK;
	}
	printf("TX other error!\r\n");
	return 0xff;                                         //����ԭ����ʧ��
}


//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
uint8_t NRF24L01_RxPacket(nrf24l01_t* nrf,uint8_t *rxbuf)
{
	uint8_t state;
	Nrf24l01_Set_Mode( nrf, NRF24L01_CSN_RX );
	
	state=NRF24L01_Read_Reg(nrf,STATUS);                //��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+STATUS,state); //���TX_DS��MAX_RT�жϱ�־
	if(state&TX_OK)
	{
		printf("RX send ack!\r\n"); //�ɹ�����ACK
	}
	if(state&RX_OK)                                 //���յ�����
	{
		NRF24L01_Read_Buf(nrf,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(nrf,FLUSH_RX,0xff);          //���RX FIFO�Ĵ���
		printf("RX read data!\r\n");
		return 0;
	}
	return 1;                                      //û�յ��κ�����
}


/***************************************/
//�ú�����ʼ��NRF24L01��RXģʽʱ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������
/***************************************/
//�ú�����ʼ��NRF24L01��TXģʽʱ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,
//ѡ��RFƵ��,�����ʺ�LNA HCURR PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������
//CEΪ�ߴ���10us,����������.
void Nrf24l01_Set_Mode(nrf24l01_t* nrf,Nrf24l01_MODE mode)
{
	switch(mode)
{
		case NRF24L01_CSN_RX:
	NRF_CE_Enable(nrf,NRF24L01_CE_EN);	
	NRF24L01_Write_Buf(nrf,SPI_WRITE_REG+RX_ADDR_P0,(uint8_t*)INIT_ADDR0,RX_ADR_WIDTH);//дRX�ڵ��ַ	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+EN_AA,0x01);//ʹ��ͨ��0���Զ�Ӧ��	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+RF_CH,CH_Num);//����RFͨ��Ƶ��	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX����ģʽ
	NRF_CE_Enable(nrf,NRF24L01_CE_DIS);//CEΪ��,�������ģʽ
		break;
		case NRF24L01_CSN_TX:
	NRF_CE_Enable(nrf,NRF24L01_CE_EN);	
	NRF24L01_Write_Buf(nrf,SPI_WRITE_REG+TX_ADDR,(uint8_t*)INIT_ADDR0,TX_ADR_WIDTH);//дTX�ڵ��ַ	
	NRF24L01_Write_Buf(nrf,SPI_WRITE_REG+RX_ADDR_P0,(uint8_t*)INIT_ADDR0,RX_ADR_WIDTH);//����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+EN_AA,0x01);//ʹ��ͨ��0���Զ�Ӧ��
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+EN_RXADDR,0x01);	//ʹ��ͨ��0�Ľ��յ�ַ	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+RF_CH,CH_Num);	//����RFͨ��Ϊ40	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+CONFIG,0x0e);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX����ģʽ,���������ж�	
	NRF_CE_Enable(nrf,NRF24L01_CE_DIS);// CEΪ��,10us����������
			break;
		default:			
			while(1)
				;
			
  }
}



//�ϵ���NRF24L01�Ƿ���λ
//д5������Ȼ���ٶ��������бȽϣ�
//��ͬʱ����ֵ:0����ʾ��λ;���򷵻�1����ʾ����λ
/** δ����ն˼���ʧ�� **/
uint8_t NRF24L01_Check(nrf24l01_t* nrf)
{
	uint8_t buf[5]= {0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t buf1[5];
	uint8_t i;
	NRF24L01_Write_Buf(nrf,SPI_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.
	NRF24L01_Read_Buf(nrf,TX_ADDR,buf1,5);              //����д��ĵ�ַ
	for(i=0; i<5; i++)if(buf1[i]!=0XA5)break;
	if(i!=5)
	{
		printf(("nRF24L01 TEST FAIL,���Ӳ������\r\n"));
		return 1;                               //NRF24L01����λ
	}
	  printf(("nRF24L01 TEST OK\r\n"));
	  return 0;		                           //NRF24L01��λ
}



/* ʹ��spi�ź� ʹ�ܺ��������ͨ��  */
void NRF_CE_Enable(nrf24l01_t* nrf, Nrf24l01_CE ce)
{
	if(ce == NRF24L01_CE_EN)
	{
		HAL_GPIO_WritePin(nrf->spi_t->spiinstance->CE_24L01_GPIO_Port , nrf->spi_t->spiinstance->CE_24L01_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(nrf->spi_t->spiinstance->CE_24L01_GPIO_Port , nrf->spi_t->spiinstance->CE_24L01_Pin, GPIO_PIN_RESET);
	}
}


/*  ��������Ƭѡ�ź���ʵ�ֶ�ָ�������ȡ  */
void NRF_CSN_Enable(nrf24l01_t* nrf,Nrf24l01_CSN csn)
{
	if(csn == NRF24L01_CSN_HIGH)
	{
     HAL_GPIO_WritePin(nrf->spi_t->spiinstance->CSN_24L01_GPIO_Port , nrf->spi_t->spiinstance->CSN_24L01_Pin, GPIO_PIN_SET);
	}
	else
	{
    HAL_GPIO_WritePin(nrf->spi_t->spiinstance->CSN_24L01_GPIO_Port , nrf->spi_t->spiinstance->CSN_24L01_Pin, GPIO_PIN_RESET);	
	}
}


/*ʵ��Ready״̬����*/
uint8_t NRF24L01GetIRQf(nrf24l01_t* nrf)
{
	return HAL_GPIO_ReadPin(nrf->spi_t->spiinstance->IQR_24L01_GPIO_Port,nrf->spi_t->spiinstance->IQR_24L01_Pin );
}


/* ��ʼ��*/
void Nrf24l01_init(nrf24l01_t*          nrf,
	                 SPI_t*               spi,
	                 Nrf24l01getiqrf      iqr,
                   Nrf24_spi_send_byte  send,
                   Nrf24l01_check       chick,
                   Nrf_ce_enable        ce,
                   Nrf_csn_enable       cs)
{
	if( spi==NULL | iqr==NULL | send==NULL | chick==NULL | ce==NULL |cs==NULL )
	{
	return;
	}
	  nrf->spi_t = spi; 
	
  	nrf->nrf24l01getiqrf = NRF24L01GetIRQf;
  	nrf->nrf24_spi_send_byte = nRF24_SPI_Send_Byte;
    nrf->nrf24l01_check = NRF24L01_Check;
	  nrf->nrf_ce_enable = NRF_CE_Enable;
	  nrf->nrf_csn_enable = NRF_CSN_Enable;
	
	  nrf->nrf_ce_enable(nrf,NRF24L01_CE_DIS);
	  nrf->nrf_csn_enable(nrf,NRF24L01_CSN_HIGH);
}
