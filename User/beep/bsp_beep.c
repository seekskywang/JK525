/**
  ******************************************************************************
  * @file    bsp_beep.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ط�����Ӧ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

#include "./beep/bsp_beep.h" 
#include "./tim/bsp_basic_tim.h"
#include "./systick/bsp_SysTick.h"

void Beep_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

//	/*����BEEP��ص�GPIO����ʱ��*/
//	RCC_AHB1PeriphClockCmd(BEEP_GPIO_CLK, ENABLE ); 

//	/*ѡ��Ҫ���Ƶ�GPIOG����*/															   
//	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;	

//	/*��������ģʽΪͨ���������*/
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   

//	/*�������ŵ��������Ϊ�������*/
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    
//	/*��������Ϊ����ģʽ*/
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

//	/*������������Ϊ2MHz */   
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

//	/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
//	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
	
	/*ѡ��Ҫ���Ƶ�GPIOG����*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	

	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   

	/*�������ŵ��������Ϊ�������*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
	/*��������Ϊ����ģʽ*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	/*������������Ϊ2MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

	/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_ResetBits(BEEP_PORT,BEEP_PIN);
	
}

//��������
void ButtonSound(void)
{
	BEEP_ON;
	delay_1ms(5);
	BEEP_OFF;
}

/*********************************************END OF FILE**********************/
