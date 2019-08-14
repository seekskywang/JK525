/**
  ******************************************************************************
  * @file    bsp_general_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ͨ�ö�ʱ��PWM�������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./tim_pwm/bsp_general_tim.h"

/**
  * @brief  ͨ�ö�ʱ�� TIMx,x[1,8]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
	
 uint32_t testCNT;	
//static void TIM2_NVIC_Configuration(void)
//{
//    NVIC_InitTypeDef NVIC_InitStructure; 
//    // �����ж���Ϊ0
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
//		// �����ж���Դ
//    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 	
//		// ������ռ���ȼ�
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
//	  // ���������ȼ�
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;	
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
//static void TIM2_Mode_Config(u32 TIM_Period,u32 TIM_Prescaler)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	// ����TIMx_CLK,x[6,7] 
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

//  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
//  //����ʱ����0������4999����Ϊ5000�Σ�Ϊһ����ʱ����
//  TIM_TimeBaseStructure.TIM_Period = TIM_Period-1;     //100  
//	
//	// �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=90MHz 
//	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
//  TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler-1;	//9000
//  // ����ʱ�ӷ�Ƶ
//  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//  // ������ʽ
//  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//	
//	// ��ʼ����ʱ��TIMx, x[1,8]
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//	
//	
//	// �����ʱ�������жϱ�־λ
//	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//	
//	// ������ʱ�������ж�
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
//	
//	// ʹ�ܶ�ʱ��
//	TIM_Cmd(TIM2, ENABLE);	
//}

/**
  * @brief  ��ʼ���߼����ƶ�ʱ����ʱ��10ms����һ���ж�
  * @param  ��
  * @retval ��
  */
//void TIM2_Configuration(u32 Period,u32 Prescaler)
//{
//	TIM2_NVIC_Configuration();	
//  
//  TIM2_Mode_Config(Period,Prescaler);
//}

/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIM3_GPIO_Config(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE); 
  /* ��ʱ��ͨ�����Ÿ��� */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); 
  
	/* ��ʱ��ͨ���������� */															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* PA�����ٶ�ʱ��2PWM���   ����ٲ�ѯ��ʱ��3��PWM���ͨ��
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM3_PWMOUTPUT_Config(u32 TIM_Period,u32 TIM_Prescaler )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// ����TIMx_CLK,x[2,3,4,5,12,13,14] 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������8999����Ϊ9000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Period = TIM_Period-1;       
	
	// �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=90MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler-1;	
  // ����ʱ�ӷ�Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// ��ʼ����ʱ��TIMx, x[2,3,4,5,12,13,14] 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/*PWMģʽ����*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 3000-1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  
	/*ʹ��ͨ��1����*/
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(TIM3, ENABLE);	
}

/**
  * @brief  ��ʼ���߼����ƶ�ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIM3_Configuration(u32 TIM_Period,u32 TIM_Prescaler )
{
	TIM3_GPIO_Config();
  
  TIM3_PWMOUTPUT_Config(TIM_Period,TIM_Prescaler);
}


/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIM14_GPIO_Config(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (GENERAL_OCPWM_GPIO_CLK, ENABLE); 
  /* ��ʱ��ͨ�����Ÿ��� */
	GPIO_PinAFConfig(GENERAL_OCPWM_GPIO_PORT,GENERAL_OCPWM_PINSOURCE,GENERAL_OCPWM_AF); 
  
	/* ��ʱ��ͨ���������� */															   
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GENERAL_OCPWM_GPIO_PORT, &GPIO_InitStructure);
}


/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_PWMOUTPUT_Config(u32 cycle,u32 pre)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// ����TIMx_CLK,x[2,3,4,5,12,13,14] 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); 

  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������8999����Ϊ9000�Σ�Ϊһ����ʱ����  100K��0��01ms 200Hz�� 5ms
    
  TIM_TimeBaseStructure.TIM_Period = cycle-1;  //     
	
	// �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=90MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = 900-1;	
  // ����ʱ�ӷ�Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// ��ʼ����ʱ��TIMx, x[2,3,4,5,12,13,14] 
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	
	/*PWMģʽ����*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = pre-1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC1Init(TIM14, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  
	/*ʹ��ͨ��1����*/
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(TIM14, ENABLE);	
}

/**
  * @brief  ��ʼ���߼����ƶ�ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIM14_Configuration(u32 cycle,u32 pre)
{
	TIM14_GPIO_Config();
    
  
  TIM_PWMOUTPUT_Config( cycle, pre);
}
void TIM6_pwm(u32 arr,u32 psc)
{
GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);

	GPIO_PinAFConfig(GPIOH, GPIO_PinSource12,GPIO_Mode_AF);
//	GPIO_PinAFConfig(GPIOF,GPIO_PinSource4,GPIO_AF_TIM6);
	//GPIO_InitStructure
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOH,&GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;//div
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=arr;//reload
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);//init tim6
	
	//pwm mode
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OC1Init(TIM6,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM6,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM6,ENABLE);
	TIM_Cmd(TIM6,ENABLE);

}

/*********************************************END OF FILE**********************/
