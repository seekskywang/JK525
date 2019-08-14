#include "stm32f4xx.h"
#include "./ch376/ch376.h"
#include "./systick/bsp_SysTick.h"
#include "pbdata.h"
//#include "jk508.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);



void CH376_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(CH376_INT_CLK|CH376_SCK_CLK|CH376_SCS_CLK|CH376_MOSI_CLK|CH376_MISO_CLK,ENABLE);	/*��������GPIO�ڵ�ʱ��*/
	CH376_SPI_CLK_INIT(CH376_SPI_CLK,ENABLE);

	
	 //�������Ÿ���
	GPIO_PinAFConfig(CH376_SCK_PORT,CH376_SPI_SCK_PINSOURCE,CH376_SPI_SCK_AF); 
	GPIO_PinAFConfig(CH376_MISO_PORT,CH376_SPI_MISO_PINSOURCE,CH376_SPI_MISO_AF); 
	GPIO_PinAFConfig(CH376_MOSI_PORT,CH376_SPI_MOSI_PINSOURCE,CH376_SPI_MOSI_AF); 
	
	/*!< ���� CH376 ����: SCK */
	GPIO_InitStructure.GPIO_Pin = CH376_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	  
	GPIO_Init(CH376_SCK_PORT, &GPIO_InitStructure);
	  
    /*!< ���� CH376 ����: MISO */
	GPIO_InitStructure.GPIO_Pin = CH376_MISO_PIN;
	GPIO_Init(CH376_MISO_PORT, &GPIO_InitStructure);
	
	/*!< ���� CH376 ����: MOSI */
	GPIO_InitStructure.GPIO_Pin = CH376_MOSI_PIN;
	GPIO_Init(CH376_MOSI_PORT, &GPIO_InitStructure);  	
	
	/*!< ���� CH376 ����: CS */
    GPIO_InitStructure.GPIO_Pin = CH376_SCS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(CH376_SCS_PORT, &GPIO_InitStructure);
	
	/*!< ���� CH376 ����: INT */
	GPIO_InitStructure.GPIO_Pin = CH376_INT_PIN;//CH376 INT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   //����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CH376_INT_PORT, &GPIO_InitStructure);
	
	
	
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
}
/*******************************************************************************
* ��      ��      : ��ʱָ��΢��ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ.
*******************************************************************************/
void mDelayuS( u8 us )
{
	while ( us -- );
}

/*******************************************************************************
* ��      ��      : ��ʱָ������ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
*******************************************************************************/
void mDelaymS( u8 ms )
{
	while ( ms -- ) 
	{
		mDelayuS( 250 );
		mDelayuS( 250 );
		mDelayuS( 250 );
		mDelayuS( 250 );
	}
}

/*******************************************************************************
* ��      ��      : ��CH376д  ����.
* ��      ��      : UINT8 mCmd:Ҫ���͵�����.
*******************************************************************************/
void xWriteCH376Cmd(u8 mCmd)
{
//	SPI_CH376_SCS_HIGH();    /* ��ֹ֮ǰδͨ��xEndCH376Cmd��ֹSPIƬѡ */
	GPIO_SetBits(CH376_SCS_PORT,CH376_SCS_PIN);
	//delay_1us(1);
	delay(20);
/* ����˫��I/O����ģ��SPI�ӿ�,��ô����ȷ���Ѿ�����SPI_SCS,SPI_SCK,SPI_SDIΪ���
*  ����,SPI_SDOΪ���뷽�� */
	GPIO_ResetBits(CH376_SCS_PORT,CH376_SCS_PIN);/* SPIƬѡ��Ч */
//	SPI_CH376_SCS_LOW();      /* SPIƬѡ��Ч */
	SPIx_ReadWriteByte(mCmd);  /* ���������� */
//	Delay(10); 
//	Delay(10); 
//	Delay(10);   /* ��ʱ1.5uSȷ����д���ڴ���1.5uS,����������һ�е�״̬��ѯ���� */
//	Delay(10); 
	delay(50);
}


/*******************************************************************************
* ��      ��      : ��CH376д   ����.
* ��      ��      : UINT8 mData:
*                   Ҫ���͵�����.
*******************************************************************************/
void xWriteCH376Data(u8 mData)
{
	SPIx_ReadWriteByte(mData);
	delay(10);  /* ȷ����д���ڴ���0.6uS */
}

/*******************************************************************************
* ��  ��  ��      : xReadCH376Data
* ��      ��      : ��CH376������.
*******************************************************************************/
u8 xReadCH376Data(void)
{
	u8 i;
	delay(10);
	i = SPIx_ReadWriteByte( 0xFF );
	return( i );
}

/*******************************************************************************
* ��  ��  ��      : xEndCH376Cmd   ��������.
*******************************************************************************/

void xEndCH376Cmd(void)
{
	SPI_CH376_SCS_HIGH();/* ��ֹSPIƬѡ */
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPIx_ReadWriteByte(u8 TxData)
{		
		SPITimeout = SPIT_FLAG_TIMEOUT;

	  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
	  while (SPI_I2S_GetFlagStatus(CH376_SPI, SPI_I2S_FLAG_TXE) == RESET)
	   {
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	   }

	  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	  SPI_I2S_SendData(CH376_SPI,TxData);

	  SPITimeout = SPIT_FLAG_TIMEOUT;

	  /* �ȴ����ջ������ǿգ�RXNE�¼� */
	  while (SPI_I2S_GetFlagStatus(CH376_SPI, SPI_I2S_FLAG_RXNE) == RESET)
	   {
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
	   }

	  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	  return SPI_I2S_ReceiveData(CH376_SPI);			    
}

/*******************************************************************************
* ��      ��      : ��ѯCH376�ж�(INT#�͵�ƽ).
* ��      ��      : FALSE:���ж�.       TRUE:���ж�.
*******************************************************************************/
u8 Query376Interrupt(void)
{
	u8 i;
	i = (GPIO_ReadInputDataBit(CH376_INT_PORT,CH376_INT_PIN) ? FALSE : TRUE ); 	/* ���������CH376���ж�������ֱ�Ӳ�ѯ�ж����� */
	return( i ); 
}


/*******************************************************************************
* ��      ��      : ��ʼ��CH376.
* ��      ��      : FALSE:���ж�.  TRUE:���ж�.
*******************************************************************************/
u8 Init_CH376(void)
{
	u8 res;
	delay_ms(1);
	
//	SPI_CH376_SCS_HIGH();/* ��ֹSPIƬѡ */
	GPIO_SetBits(CH376_SCS_PORT,CH376_SCS_PIN);/* ��ֹSPIƬѡ */
	GPIO_SetBits(CH376_INT_PORT,CH376_INT_PIN);
	
	xWriteCH376Cmd( CMD11_CHECK_EXIST );
	xWriteCH376Data(0x55);
	res = xReadCH376Data();
	xEndCH376Cmd();
	if (res != 0xAA) 		
	{
		return(ERR_USB_UNKNOWN);  /* ͨѶ�ӿڲ�����,����ԭ����:�ӿ������쳣,�����豸Ӱ��(Ƭѡ��Ψһ),���ڲ�����,һֱ�ڸ�λ,���񲻹��� */
	}
//	else{
//		return(CH376OK);
//	}
	xWriteCH376Cmd( CMD11_SET_USB_MODE ); /* �豸USB����ģʽ */
	xWriteCH376Data( 0x06 );
	delay(20);
	res = xReadCH376Data();
	xEndCH376Cmd();
	
	if ( res == CMD_RET_SUCCESS )  //RES=51  ��������ɹ�
	{
	    return( USB_INT_SUCCESS ); //USB������ߴ�������ɹ� 
	}
	else 
	{
	    return( ERR_USB_UNKNOWN );/* ����ģʽ���� */
	}
}

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
	//FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}

