/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   RTC����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./RTC/bsp_rtc.h"
#include "./usart/bsp_debug_usart.h"
#include "./lcd/bsp_lcd.h"
#include "pbdata.h"

 
/**
  * @brief  ����ʱ�������
  * @param  ��
  * @retval ��
  */
void RTC_TimeAndDate_Set(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	// ��ʼ��ʱ��
	RTC_TimeStructure.RTC_H12 = RTC_H12_AMorPM;
	RTC_TimeStructure.RTC_Hours = HOURS;        
	RTC_TimeStructure.RTC_Minutes = MINUTES;      
	RTC_TimeStructure.RTC_Seconds = SECONDS;      
	RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	
  // ��ʼ������	
	RTC_DateStructure.RTC_WeekDay = WEEKDAY;       
	RTC_DateStructure.RTC_Date = DATE;         
	RTC_DateStructure.RTC_Month = MONTH;         
	RTC_DateStructure.RTC_Year = YEAR;        
	RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
}

/**
  * @brief  ��ʾʱ�������
  * @param  ��
  * @retval ��
  */
void RTC_TimeAndDate_Show(void)
{
	uint8_t Rtctmp=0;
	char LCDTemp[100];
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	
	while(1)
	{
		// ��ȡ����
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
		
		// ÿ���ӡһ��
  	if(Rtctmp != RTC_TimeStructure.RTC_Seconds)
    {
						
			// ��ӡ����
      printf("The Date :  Y:20%0.2d - M:%0.2d - D:%0.2d - W:%0.2d\r\n", 
			RTC_DateStructure.RTC_Year,
			RTC_DateStructure.RTC_Month, 
			RTC_DateStructure.RTC_Date,
			RTC_DateStructure.RTC_WeekDay);
		
			//Һ����ʾ����
			//�Ȱ�Ҫ��ʾ��������sprintf����ת��Ϊ�ַ�����Ȼ�������Һ����ʾ������ʾ
			sprintf(LCDTemp,"The Date :  Y:20%0.2d - M:%0.2d - D:%0.2d - W:%0.2d", 
			RTC_DateStructure.RTC_Year,
			RTC_DateStructure.RTC_Month, 
			RTC_DateStructure.RTC_Date,
			RTC_DateStructure.RTC_WeekDay);

			LCD_DisplayStringLineEx(10,50,48,48,(uint8_t *)LCDTemp,0); 
			
			// ��ӡʱ��
      printf("The Time :  %0.2d:%0.2d:%0.2d \r\n\r\n", 
			RTC_TimeStructure.RTC_Hours, 
			RTC_TimeStructure.RTC_Minutes, 
			RTC_TimeStructure.RTC_Seconds);
			
			//Һ����ʾʱ��
			sprintf(LCDTemp,"The Time :  %0.2d:%0.2d:%0.2d", 
			RTC_TimeStructure.RTC_Hours, 
			RTC_TimeStructure.RTC_Minutes, 
			RTC_TimeStructure.RTC_Seconds);

			LCD_DisplayStringLineEx(10,100,48,48,(uint8_t *)LCDTemp,0);
			
      (void)RTC->DR;
    }
    Rtctmp = RTC_TimeStructure.RTC_Seconds;
	}	
}

/**
  * @brief  RTC���ã�ѡ��RTCʱ��Դ������RTC_CLK�ķ�Ƶϵ��
  * @param  ��
  * @retval ��
  */
void RTC_CLK_Config(void)
{  
	RTC_InitTypeDef RTC_InitStructure;
	
	/*ʹ�� PWR ʱ��*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
  PWR_BackupAccessCmd(ENABLE);

#if defined (RTC_CLOCK_SOURCE_LSI) 
  /* ʹ��LSI��ΪRTCʱ��Դ������� 
	 * Ĭ��ѡ��LSE��ΪRTC��ʱ��Դ
	 */
  /* ʹ��LSI */ 
  RCC_LSICmd(ENABLE);
  /* �ȴ�LSI�ȶ� */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }
  /* ѡ��LSI��ΪRTC��ʱ��Դ */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

#elif defined (RTC_CLOCK_SOURCE_LSE)

  /* ʹ��LSE */ 
  RCC_LSEConfig(RCC_LSE_ON);
   /* �ȴ�LSE�ȶ� */   
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }
  /* ѡ��LSE��ΪRTC��ʱ��Դ */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);    

#endif /* RTC_CLOCK_SOURCE_LSI */

  /* ʹ��RTCʱ�� */
  RCC_RTCCLKCmd(ENABLE);

  /* �ȴ� RTC APB �Ĵ���ͬ�� */
  RTC_WaitForSynchro();
   
/*=====================��ʼ��ͬ��/�첽Ԥ��Ƶ����ֵ======================*/
	/* ����������ʱ��ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */
	
	/* �����첽Ԥ��Ƶ����ֵ */
	RTC_InitStructure.RTC_AsynchPrediv = ASYNCHPREDIV;
	/* ����ͬ��Ԥ��Ƶ����ֵ */
	RTC_InitStructure.RTC_SynchPrediv = SYNCHPREDIV;	
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	/* ��RTC_InitStructure�����ݳ�ʼ��RTC�Ĵ��� */
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		printf("\n\r RTC ʱ�ӳ�ʼ��ʧ�� \r\n");
	}	
}

/**
  * @brief  RTC���ã�ѡ��RTCʱ��Դ������RTC_CLK�ķ�Ƶϵ��
  * @param  ��
  * @retval ��
  */
#define LSE_STARTUP_TIMEOUT     ((uint16_t)0x05000)
void RTC_CLK_Config_Backup(void)
{  
  __IO uint16_t StartUpCounter = 0;
	FlagStatus LSEStatus = RESET;	
	RTC_InitTypeDef RTC_InitStructure;
	
	/* ʹ�� PWR ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
  PWR_BackupAccessCmd(ENABLE);
	
/*=========================ѡ��RTCʱ��Դ==============================*/
/* Ĭ��ʹ��LSE�����LSE��������ʹ��LSI */
  /* ʹ��LSE */
  RCC_LSEConfig(RCC_LSE_ON);	
	
	/* �ȴ�LSE�����ȶ��������ʱ���˳� */
  do
  {
    LSEStatus = RCC_GetFlagStatus(RCC_FLAG_LSERDY);
    StartUpCounter++;
  }while((LSEStatus == RESET) && (StartUpCounter != LSE_STARTUP_TIMEOUT));
	
	
	if(LSEStatus == SET )
  {
		printf("\n\r LSE �����ɹ� \r\n");
		/* ѡ��LSE��ΪRTC��ʱ��Դ */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  }
	else
	{
		printf("\n\r LSE ���ϣ�תΪʹ��LSI \r\n");
		
		/* ʹ��LSI */	
		RCC_LSICmd(ENABLE);
		/* �ȴ�LSI�ȶ� */ 
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{			
		}
		
		printf("\n\r LSI �����ɹ� \r\n");
		/* ѡ��LSI��ΪRTC��ʱ��Դ */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	}
	
  /* ʹ�� RTC ʱ�� */
  RCC_RTCCLKCmd(ENABLE);
  /* �ȴ� RTC APB �Ĵ���ͬ�� */
  RTC_WaitForSynchro();

/*=====================��ʼ��ͬ��/�첽Ԥ��Ƶ����ֵ======================*/
	/* ����������ʱ��ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */
	
	/* �����첽Ԥ��Ƶ����ֵΪ127 */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	/* ����ͬ��Ԥ��Ƶ����ֵΪ255 */
	RTC_InitStructure.RTC_SynchPrediv = 0xFF;	
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	/* ��RTC_InitStructure�����ݳ�ʼ��RTC�Ĵ��� */
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		printf("\n\r RTC ʱ�ӳ�ʼ��ʧ�� \r\n");
	}	
}

//cnt:�Զ���װ��ֵ.����0,�����ж�.
void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
	
	RTC_WakeUpCmd(DISABLE);//�ر�WAKE UP
	
	RTC_WakeUpClockConfig(wksel);//����ʱ��ѡ��
	
	RTC_SetWakeUpCounter(cnt);//����WAKE UP�Զ���װ�ؼĴ���
	
	
	RTC_ClearITPendingBit(RTC_IT_WUT); //���RTC WAKE UP�ı�־
  EXTI_ClearITPendingBit(EXTI_Line22);//���LINE22�ϵ��жϱ�־λ 
	 
	RTC_ITConfig(RTC_IT_WUT,ENABLE);//����WAKE UP ��ʱ���ж�
	RTC_WakeUpCmd( ENABLE);//����WAKE UP ��ʱ����
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE22
  EXTI_Init(&EXTI_InitStructure);//����
 
 
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
}

//RTC WAKE UP�жϷ�����
void RTC_WKUP_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)//WK_UP�ж�?
	{ 
		RTC_ClearFlag(RTC_FLAG_WUTF);	//����жϱ�־
		//LED1=!LED1;
        Disp_RTCflag=1;
	}   
	EXTI_ClearITPendingBit(EXTI_Line22);//����ж���22���жϱ�־ 								
}
/**********************************END OF FILE*************************************/
