/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "pbdata.h"
#include "usb_bsp.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"
#include    "bsp_exti.h"
//#include "touchscreen.h"
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;
 
/* Private function prototypes -----------------------------------------------*/
extern void USB_OTG_BSP_TimerIRQ (void);
//extern void Read__Convert_read(void);
/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
    
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    TimingDelay_Decrement();


}
void USART3_IRQHandler(void)
{
	u8 dat;

	if(( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET))//��������ж�
	{
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
        USART_ClearFlag(USART3, USART_FLAG_RXNE);
//		RI=0;//����ձ�־
	}
    USART_ClearFlag(USART3, USART_FLAG_RXNE);//
    USART_ClearFlag(USART3, USART_FLAG_TXE);
    

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

void  TIM2_IRQHandler (void)//U�����˶�ʱ��2
{
    USB_OTG_BSP_TimerIRQ();
	 	
}

void  BASIC_TIM_IRQHandler (void)
{
    static u8 num=0;
    u8 i;
	
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	
//		LED1_TOGGLE;
		TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update);  		
//        Key_Scan();
        //????
//        if(SystemStatus==SYS_STATUS_CLEAR)
//        {
//            num++;
//            if(num>9)//10mS??
//            {
//                num=0;
//                F_100ms=TRUE;//100ms????
//                //Test_value.Clear_Time++;
//            }
//        
//        }else
        if(SystemStatus==SYS_STATUS_TEST)
        {
            num++;
            if(num>9)//10mS??
            {
                num=0;
                F_100ms=TRUE;//100ms????
                Test_value.Test_Time++;
                //Voltage++;
            }
        }
        else
        {
            if(num!=0)
                num=0;
        }
//		Touch_Scan();
        Key_Scan();	  
				
        for (i=0;i<MAXTSOFTTIMER;i++)
        {
            if (SoftTimer[i])
                SoftTimer[i]--;
            
        }	
        
	
	}		 	
}
void KEY1_IRQHandler(void)//���ػ�
{
  //ȷ���Ƿ������EXTI Line�ж�
 //   u32 i;
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		delay_ms(1000);
       
        if(GPIO_ReadInputDataBit( GPIOE,  GPIO_Pin_3)==0)
        {
            Int_Pe3flag=0;
            if(softswitch)
            {
                softswitch=0;
                SetSystemStatus(SYS_STATUS_TOOL);
            
            }else
            {
                softswitch=1;
                
                SetSystemStatus(SYS_STATUS_POWER);
            
            }
            NVIC_DisableIRQ(EXTI3_IRQn);
        }
		//GPIO_ClearInt(0, 1<<19);
		
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
}

//void EXTI15_10_IRQHandler(void)
//{
//  //ȷ���Ƿ������EXTI Line�ж�
//	if(EXTI_GetITStatus(EXTI_Line13) != RESET) 
//	{
//		// LED2 ȡ��		
//		//LED2_TOGGLE;
//    //����жϱ�־λ
//        //Read__Convert_read();
//		EXTI_ClearITPendingBit(EXTI_Line13);     
//	}  
//}
#ifdef USE_USB_OTG_FS  
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
    
#endif
{
  USBH_OTG_ISR_Handler(&USB_OTG_Core);
}

/**	������⣬Ӳ����֧��
  * @brief  EXTI1_IRQHandler
  *         This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
#if 0
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
      USB_Host.usr_cb->OverCurrentDetected();
      EXTI_ClearITPendingBit(EXTI_Line1);
  }
}


#endif
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
