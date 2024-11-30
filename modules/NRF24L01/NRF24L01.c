#include "NRF24L01.h"
#include "main.h"
#include "spi.h"
#include "stdio.h"
/********    从机发送     *******/
//NRF24L01 驱动函数

unsigned char idel_mode_flag = 0;
unsigned char mode_time_counter = 0;

const unsigned char INIT_ADDR0[5]= {0x02,0x3A,0xB1,0xB1,0x01};
const unsigned char INIT_ADDR1[5]= {0x02,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR2[5]= {0x03,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR3[5]= {0x04,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR4[5]= {0x05,0x3A,0x01,0x01,0x01};
const unsigned char INIT_ADDR5[5]= {0x06,0x3A,0x01,0x01,0x01};


//通过SPI写寄存器
static uint8_t NRF24L01_Write_Reg(nrf24l01_t* nrf,uint8_t regaddr,uint8_t  data);

//读取SPI寄存器值 ，regaddr:要读的寄存器
static uint8_t NRF24L01_Read_Reg(nrf24l01_t* nrf,uint8_t regaddr);


//在指定位置读出指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
static uint8_t NRF24L01_Read_Buf(nrf24l01_t* nrf,uint8_t regaddr,uint8_t* pBuf,uint8_t datalen);

//在指定位置写指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
static uint8_t NRF24L01_Write_Buf(nrf24l01_t* nrf,uint8_t regaddr, uint8_t *pBuf,uint8_t datalen);



//封装spi读写函数
uint8_t nRF24_SPI_Send_Byte(nrf24l01_t* nrf,uint8_t txData)
{
	uint8_t rxdata;
  rxdata=nrf->spi_t->Transrecv(nrf->spi_t->spiinstance,&txData,SPI_BLACK_MODE);
	return(rxdata);							// return read unsigned char
}


//通过SPI写寄存器
uint8_t NRF24L01_Write_Reg(nrf24l01_t* nrf,uint8_t regaddr,uint8_t  data)
{
	uint8_t status;
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_LOW);//使能SPI传输                  
	status =nrf->nrf24_spi_send_byte (nrf,regaddr);   //发送寄存器号
	nrf->nrf24_spi_send_byte (nrf,data);           //写入寄存器的值                 
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_HIGH);  //禁止SPI传输
	return(status);       		         //返回状态值
}


//读取SPI寄存器值 ，regaddr:要读的寄存器
uint8_t NRF24L01_Read_Reg(nrf24l01_t* nrf,uint8_t regaddr)
{
	uint8_t reg_val;
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_LOW);                //使能SPI传输
	nrf->nrf24_spi_send_byte (nrf,regaddr);      //发送寄存器号
	reg_val=nrf->nrf24_spi_send_byte (nrf,0XFF);//读取寄存器内容
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_HIGH);               //禁止SPI传输
	return(reg_val);                 //返回状态值
}


//在指定位置读出指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
uint8_t NRF24L01_Read_Buf(nrf24l01_t* nrf,uint8_t regaddr,uint8_t* pBuf,uint8_t datalen)
{
  uint8_t status;
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_LOW); //使能SPI传输
	status=nrf->nrf24_spi_send_byte (nrf,regaddr);   //发送寄存器值(位置),并读取状态值
	for(int i=0;i<=datalen;i++)
	{
  	pBuf[i]=nrf->nrf24_spi_send_byte(nrf,0XFF);//读取寄存器内容
	}
//	nrf->spi_t->spireceive(nrf->spi_t->spiinstance ,pBuf,datalen,SPI_BLACK_MODE);                 
	NRF_CSN_Enable( nrf,NRF24L01_CSN_HIGH); //关闭SPI传输
	return status;                        //返回读到的状态值
}


//在指定位置写指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
uint8_t NRF24L01_Write_Buf(nrf24l01_t* nrf,uint8_t regaddr, uint8_t *pBuf,uint8_t datalen)
{
	uint8_t status;
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_LOW); //使能SPI传输
	status = nrf->nrf24_spi_send_byte (nrf,regaddr);                //发送寄存器值(位置),并读取状态值
	for(int i=0;i<=datalen;i++)
	{
  	nrf->nrf24_spi_send_byte (nrf,pBuf[i]);//读取寄存器内容
	}
	//nrf->spi_t->spitransmit(nrf->spi_t->spiinstance ,pBuf,datalen,SPI_BLACK_MODE);   
	nrf->nrf_csn_enable(nrf,NRF24L01_CSN_HIGH);   //关闭SPI传输
	return status;                                       //返回读到的状态值
}


//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
uint8_t NRF24L01_TxPacket(nrf24l01_t* nrf,uint8_t *txbuf)
{
	uint8_t state;
	Nrf24l01_Set_Mode(nrf,NRF24L01_CSN_TX);
	
	nrf->nrf_ce_enable(nrf,NRF24L01_CE_DIS);
	NRF24L01_Write_Buf(nrf,WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
	nrf->nrf_ce_enable(nrf,NRF24L01_CE_EN);  
	
	while(nrf->nrf24l01getiqrf(nrf)!=0)  printf("2\n\r");                         //等待发送完成
	state=NRF24L01_Read_Reg(nrf,STATUS);                     //读取状态寄存器的值
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+STATUS,state);      //清除TX_DS或MAX_RT中断标志
	if(state&MAX_TX)                                     //达到最大重发次数
	{
		NRF24L01_Write_Reg(nrf,FLUSH_TX,0xff);               //清除TX FIFO寄存器
		printf("TX MAX_TX error!\r\n");
		return MAX_TX;
	}
	if(state&TX_OK)                                      //发送完成
	{
		printf("TX OK!\r\n");
		return TX_OK;
	}
	printf("TX other error!\r\n");
	return 0xff;                                         //其他原因发送失败
}


//启动NRF24L01接收一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
uint8_t NRF24L01_RxPacket(nrf24l01_t* nrf,uint8_t *rxbuf)
{
	uint8_t state;
	Nrf24l01_Set_Mode( nrf, NRF24L01_CSN_RX );
	
	state=NRF24L01_Read_Reg(nrf,STATUS);                //读取状态寄存器的值
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+STATUS,state); //清除TX_DS或MAX_RT中断标志
	if(state&TX_OK)
	{
		printf("RX send ack!\r\n"); //成功发送ACK
	}
	if(state&RX_OK)                                 //接收到数据
	{
		NRF24L01_Read_Buf(nrf,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(nrf,FLUSH_RX,0xff);          //清除RX FIFO寄存器
		printf("RX read data!\r\n");
		return 0;
	}
	return 1;                                      //没收到任何数据
}


/***************************************/
//该函数初始化NRF24L01到RX模式时
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了
/***************************************/
//该函数初始化NRF24L01到TX模式时
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,
//选择RF频道,波特率和LNA HCURR PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了
//CE为高大于10us,则启动发送.
void Nrf24l01_Set_Mode(nrf24l01_t* nrf,Nrf24l01_MODE mode)
{
	switch(mode)
{
		case NRF24L01_CSN_RX:
	NRF_CE_Enable(nrf,NRF24L01_CE_EN);	
	NRF24L01_Write_Buf(nrf,SPI_WRITE_REG+RX_ADDR_P0,(uint8_t*)INIT_ADDR0,RX_ADR_WIDTH);//写RX节点地址	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+EN_AA,0x01);//使能通道0的自动应答	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+RF_CH,CH_Num);//设置RF通信频率	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX接收模式
	NRF_CE_Enable(nrf,NRF24L01_CE_DIS);//CE为高,进入接收模式
		break;
		case NRF24L01_CSN_TX:
	NRF_CE_Enable(nrf,NRF24L01_CE_EN);	
	NRF24L01_Write_Buf(nrf,SPI_WRITE_REG+TX_ADDR,(uint8_t*)INIT_ADDR0,TX_ADR_WIDTH);//写TX节点地址	
	NRF24L01_Write_Buf(nrf,SPI_WRITE_REG+RX_ADDR_P0,(uint8_t*)INIT_ADDR0,RX_ADR_WIDTH);//设置TX节点地址,主要为了使能ACK	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+EN_AA,0x01);//使能通道0的自动应答
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+EN_RXADDR,0x01);	//使能通道0的接收地址	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+RF_CH,CH_Num);	//设置RF通道为40	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启	
	NRF24L01_Write_Reg(nrf,SPI_WRITE_REG+CONFIG,0x0e);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX发送模式,开启所有中断	
	NRF_CE_Enable(nrf,NRF24L01_CE_DIS);// CE为高,10us后启动发送
			break;
		default:			
			while(1)
				;
			
  }
}



//上电检测NRF24L01是否在位
//写5个数据然后再读回来进行比较，
//相同时返回值:0，表示在位;否则返回1，表示不在位
/** 未搭建接收端检查会失败 **/
uint8_t NRF24L01_Check(nrf24l01_t* nrf)
{
	uint8_t buf[5]= {0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t buf1[5];
	uint8_t i;
	NRF24L01_Write_Buf(nrf,SPI_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.
	NRF24L01_Read_Buf(nrf,TX_ADDR,buf1,5);              //读出写入的地址
	for(i=0; i<5; i++)if(buf1[i]!=0XA5)break;
	if(i!=5)
	{
		printf(("nRF24L01 TEST FAIL,检查硬件连接\r\n"));
		return 1;                               //NRF24L01不在位
	}
	  printf(("nRF24L01 TEST OK\r\n"));
	  return 0;		                           //NRF24L01在位
}



/* 使能spi信号 使能后才能正常通信  */
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


/*  拉高拉低片选信号线实现对指定外设读取  */
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


/*实现Ready状态监视*/
uint8_t NRF24L01GetIRQf(nrf24l01_t* nrf)
{
	return HAL_GPIO_ReadPin(nrf->spi_t->spiinstance->IQR_24L01_GPIO_Port,nrf->spi_t->spiinstance->IQR_24L01_Pin );
}


/* 初始哈*/
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
