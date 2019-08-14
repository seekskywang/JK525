/**
  ******************************************************************************
  * @file GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and 
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"	 
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "pbdata.h"


void NMI_Handler(void)
{
}

void USART1_IRQHandler(void)
{
   if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
   {
		USART_SendData(USART1,USART_ReceiveData(USART1));
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
   }
}


/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
}

void TIM3_IRQHandler(void)
{
	static u8 num=0;
	u8 i;
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	//if(disp_time<99999)
	disp_time++;
	Test_Time++;
	
//	else
//		disp_time=0;
	Key_Scan();
	//ʱ������
	if(SystemStatus==SYS_STATUS_TEST)
	{
		num++;
		if(num>9)//10mSʱ��
		{
			num=0;
			F_100ms=TRUE;//100ms��ʱ��־
			Test_value.Test_Time++;
			//Voltage++;
		}
	}
	else
	{
		if(num!=0)
			num=0;
	}	
	for (i=0;i<MAXTSOFTTIMER;i++)
	{
		if (SoftTimer[i])
			SoftTimer[i]--;
		
	}	
	

}
/****************************************************************************
* ��    �ƣ�void EXTI9_5_IRQHandler(void)
* ��    �ܣ�EXTI9-5�жϴ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void EXTI9_5_IRQHandler(void)
{
}

/****************************************************************************
* ��    �ƣ�void EXTI1_IRQHandler(void)
* ��    �ܣ�EXTI2�жϴ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void EXTI1_IRQHandler(void)
{
    
}

/****************************************************************************
* ��    �ƣ�void EXTI2_IRQHandler(void)
* ��    �ܣ�EXTI2�жϴ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void EXTI2_IRQHandler(void)
{
    
}

/****************************************************************************
* ��    �ƣ�void EXTI3_IRQHandler(void)
* ��    �ܣ�EXTI3�жϴ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void EXTI3_IRQHandler(void)
{
   
}

void USART3_IRQHandler(void)
{
	u8 dat;
    if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)  
   {  
       USART_ReceiveData(USART3);  
     USART_ClearFlag(USART3, USART_FLAG_PE);  
   }  
      
   if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  
   {  
       USART_ReceiveData(USART3);  
     USART_ClearFlag(USART3, USART_FLAG_ORE);  
   }  
      
    if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)  
   {  
       USART_ReceiveData(USART3);  
      USART_ClearFlag(USART3, USART_FLAG_FE);  
   }
	if(( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET))//��������ж�
	{
        USART_ClearFlag(USART3, USART_FLAG_RXNE);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		if (!ComBuf.rec.end)//����û����
		{
			SetRecTimeOut(REC_TIME_OUT);//���ý��ճ�ʱ����
			dat=USART_ReceiveData(USART3);
			if (dat==(u8)(UART_REC_BEGIN))//֡ͷ
			{
				if(ComBuf.rec.ptr!=0) //���ֽ�
				{
					ComBuf.rec.ptr=0;//���½��� 
				}
				else
				{
					ComBuf.rec.buf[ComBuf.rec.ptr++]=dat;
				}
			}
			else if (dat==(u8)(UART_REC_END))//֡β
			{
				ComBuf.rec.buf[ComBuf.rec.ptr++]=dat;
				ComBuf.rec.end=TRUE;//���ս���
				ComBuf.rec.len=ComBuf.rec.ptr;//��������ݳ���
				ComBuf.rec.ptr=0;//ָ���������¿�ʼ�µ�һ֡����
				ComBuf.rec.TimeOut=0;//���ճ�ʱ����
			}
			else
			{
				if (ComBuf.rec.ptr>=REC_LEN_MAX)//������֡����
				{
					ComBuf.rec.ptr=0;//���½���
				}
				else
				{
					ComBuf.rec.buf[ComBuf.rec.ptr++]=dat;
				}
			}
		}	
        
	}
    
    //USART_ClearFlag(USART3, USART_FLAG_RXNE);//
    USART_ClearFlag(USART3, USART_FLAG_TXE);
    dat=USART_GetFlagStatus(USART3, USART_FLAG_TXE);
    

//	if(( USART_GetFlagStatus(USART3, USART_FLAG_TXE) != RESET))//��������ж�
//	{
//		if (ComBuf.send.begin)//���Ϳ�ʼ
//		{
//			if (ComBuf.send.ptr<ComBuf.send.len)//û���������
//			{
////				SBUF=ComBuf.send.buf[ComBuf.send.ptr++];
//                 USART_SendData(USART3, (uint8_t) ComBuf.send.buf[ComBuf.send.ptr++]); /*����һ���ַ�����*/ 

//                  /* Loop until the end of transmission */
//                  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)/*�ȴ��������*/
//                  {
//                  
//                  }
//			}
//			else
//			{
//				ComBuf.send.begin=FALSE;//���ͽ���
//				ComBuf.send.ptr=0;//ָ������
//			}
//		}
//USART_ClearFlag(USART3, USART_FLAG_TXE);//		TI=0;//�巢�ͱ�־
//	}	
}
