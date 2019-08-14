/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ����������
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
#include "touchscreen.h"
#include "./systick/bsp_SysTick.h"
//#include "bsp_adc.h"

u16 Xconvert;
u16 Yconvert;
u32 corpara[20];
float XCOOR;
float YCOOR;
u8 touchflag;
u8 press;

//void Touch_GPIO_Config(void) 
//{
//	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	/*������ص�GPIO����ʱ��*/
//	RCC_AHB1PeriphClockCmd (TOUCH_XPLUS_GPIO_CLK|TOUCH_XMINUS_GPIO_CLK|TOUCH_YPLUS_GPIO_CLK|
//	TOUCH_YMINUS_GPIO_CLK,ENABLE); 

//	GPIO_InitStructure.GPIO_Pin = TOUCH_YPLUS_GPIO_PIN|TOUCH_YMINUS_GPIO_PIN;/*ѡ������*/     
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ����ģʽ*/   
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;/*������������Ϊ2MHz */    
//	GPIO_Init(TOUCH_YPLUS_GPIO_PORT, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = TOUCH_XMINUS_GPIO_PIN;/*ѡ������*/     
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ���ģʽ*/   
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*�������ŵ��������Ϊ�������*/    
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;/*������������Ϊ2MHz */    
//	GPIO_Init(TOUCH_XMINUS_GPIO_PORT, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = TOUCH_XPLUS_GPIO_PIN;/*ѡ������*/     
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ���ģʽ*/   
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*�������ŵ��������Ϊ�������*/    
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;/*������������Ϊ2MHz */    
//	GPIO_Init(TOUCH_XPLUS_GPIO_PORT, &GPIO_InitStructure);
//	
//	Touch_Up_flag=0;
////	GPIO_ResetBits(TOUCH_XMINUS_GPIO_PORT,TOUCH_XMINUS_GPIO_PIN);
//}

//static void AD_GPIO_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;	
//	/*=====================ͨ��1======================*/	
//	// ʹ�� GPIO ʱ��
//	RCC_AHB1PeriphClockCmd(ADC_GPIO_CLK0,ENABLE);		
//	// ���� IO
//    GPIO_InitStructure.GPIO_Pin = ADC_GPIO_PIN0|ADC_GPIO_PIN1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//    //������������	
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//	GPIO_Init(ADC_GPIO_PORT0, &GPIO_InitStructure);

//}

//static void Rheostat_ADC_Mode_Config(void)
//{
//	DMA_InitTypeDef DMA_InitStructure;
//	ADC_InitTypeDef ADC_InitStructure;
//    ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	
//  // ------------------DMA Init �ṹ����� ��ʼ��--------------------------
//  // ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
//  // ����DMAʱ��
//  RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_DMA_CLK, ENABLE); 
//	// �����ַΪ��ADC ���ݼĴ�����ַ
//	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;	
//  // �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���	
//	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_Converted;  
//  // ���ݴ��䷽��Ϊ���赽�洢��	
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
//	// ��������СΪ��ָһ�δ����������
//	DMA_InitStructure.DMA_BufferSize = RHEOSTAT_NOFCHANEL1;	
//	// ����Ĵ���ֻ��һ������ַ���õ���
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  // �洢����ַ�̶�
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
//  // // �������ݴ�СΪ���֣��������ֽ� 
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
//  //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
//	// ѭ������ģʽ
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//  // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
//  // FIFO ��С��FIFOģʽ��ֹʱ�������������	
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
//	// ѡ�� DMA ͨ����ͨ������������
//  DMA_InitStructure.DMA_Channel = RHEOSTAT_ADC_DMA_CHANNEL; 
//  //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
//	DMA_Init(RHEOSTAT_ADC_DMA_STREAM, &DMA_InitStructure);
//	// ʹ��DMA��
//  DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);
//	
//	// ����ADCʱ��
//	RCC_APB2PeriphClockCmd(RHEOSTAT_ADC_CLK , ENABLE);
//  // -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
//	// ����ADCģʽ
//  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
//  // ʱ��Ϊfpclk x��Ƶ	
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
//  // ��ֹDMAֱ�ӷ���ģʽ	
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
//  // ����ʱ����	
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  
//  ADC_CommonInit(&ADC_CommonInitStructure);
//	
//  // -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
//  // ADC �ֱ���
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//  // ɨ��ģʽ����ͨ���ɼ���Ҫ	
//  ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
//  // ����ת��	
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
//  //��ֹ�ⲿ���ش���
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
//  //ʹ������������ⲿ�����������ã�ע�͵�����
//  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
//  //�����Ҷ���	
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//  //ת��ͨ�� 1��
//  ADC_InitStructure.ADC_NbrOfConversion = RHEOSTAT_NOFCHANEL1;                                    
//  ADC_Init(RHEOSTAT_ADC, &ADC_InitStructure);
//  //---------------------------------------------------------------------------
//	
//  // ���� ADC ͨ��ת��˳��Ͳ���ʱ������
//  ADC_RegularChannelConfig(RHEOSTAT_ADC, ADC_CHANNEL0, 1, 
//	                         ADC_SampleTime_144Cycles);
//  ADC_RegularChannelConfig(RHEOSTAT_ADC, ADC_CHANNEL1, 2, 
//	                         ADC_SampleTime_144Cycles); 

//  // ʹ��DMA���� after last transfer (Single-ADC mode)
//  ADC_DMARequestAfterLastTransferCmd(RHEOSTAT_ADC, ENABLE);
//  // ʹ��ADC DMA
//  ADC_DMACmd(RHEOSTAT_ADC, ENABLE);
//	
//	// ʹ��ADC
//  ADC_Cmd(RHEOSTAT_ADC, ENABLE);  
//  //��ʼadcת�����������
//  ADC_SoftwareStartConv(RHEOSTAT_ADC);
//}

//void Touch_Init(void)
//{
//	AD_GPIO_Config();
//	Rheostat_ADC_Mode_Config();
//}

//static void set_yz(void){	X_P1;	X_N1;	Y_P0;	Y_N0;}
//static void set_yy(void){	X_P0;	X_N1;	Y_P1;	Y_N0;}
//static void set_xx(void){	X_P1;	X_N0;	Y_P0;	Y_N1;}
//static void set_mid(void){	X_P0;	X_N1;	Y_P1;	Y_N0;}

void delay_touch(void)
{
	u16 j;
	j=50000;
	while(j--);

}
//void Touch_Scan(void)
//{
//	static u8 i;
////	static u16 ADX[5];
////	static u16 ADY[5];
//	static u8 touchdelay;
////	u32 var32;

//	if(i%4 < 2)
//	{
//		
//		delay_touch();
//		Yconvert = ADC_Converted[1]/50;
//		set_xx();

//	}else{
//		
//		delay_touch();
//		Xconvert = ADC_Converted[0]/50;
//		set_yy();
//	
//	}
//	i++;
//	
//	
//	
//	//if(touchflag == 1)
//	{
//		//if(touchdelay > 5)
//		{
//			if(Xconvert <= 5)
//			{
//				XCOOR = 0;
//			}else{
//				XCOOR = (float)(8.49f*(float)Xconvert-15.48f);
//			}
//			
//			if(Yconvert <= 3)
//			{
//				YCOOR = 0;
//			}else{
//				YCOOR = 480 - (int)(6.38f*(float)Yconvert-9.16f);
//			}
//		}
////		else{
////			touchdelay++;
////		}
//	}
//	
//	
//	if(Xconvert <= 5 || Yconvert <= 3)
//	{
//		press = 0;
//		touchflag = 0;
//		touchdelay = 0;
//		XCOOR = 0;
//		YCOOR = 0;
//	}else{
//		touchflag = 1;
//	}
//	
//}

