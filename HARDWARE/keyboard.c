#include "pbdata.h"
//#include "touchscreen.h"

//==========================================================
//ȫ�ֱ���
Key_TypeDef Keyboard;//����ṹ��

static u8 LastKeyVal;//�ϴμ�ֵ
static u8 LastKeyTicks;//��������
static u8 ContinueKeyTicks;//������������
const u8 Scan_Value[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};

void Key_Delay(u32 num)
{
    while(num)
    {
        num--;
    }
    

}
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(P1_GPIO_CLK|P2_GPIO_CLK|P3_GPIO_CLK|P4_GPIO_CLK|P5_GPIO_CLK|P6_GPIO_CLK,ENABLE);	/*��������GPIO�ڵ�ʱ��*/
	
//	 /* ���� NVIC */
//	NVIC_Configuration();
	  
	GPIO_InitStructure.GPIO_Pin = P1_PIN;/*ѡ�񰴼�������*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ���ģʽ*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*�������ŵ��������Ϊ�������*/    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;/*������������Ϊ2MHz */    
	GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/


  
	GPIO_InitStructure.GPIO_Pin = P2_PIN;/*ѡ�񰴼�������*/       
	GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/

	
	GPIO_InitStructure.GPIO_Pin = P3_PIN;/*ѡ�񰴼�������*/       
	GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/

	

	GPIO_InitStructure.GPIO_Pin = P4_PIN;/*ѡ�񰴼�������*/       
	GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/

	
	GPIO_InitStructure.GPIO_Pin = P5_PIN;/*ѡ�񰴼�������*/       
	GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/

	
	GPIO_InitStructure.GPIO_Pin = P6_PIN;/*ѡ�񰴼�������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*��������Ϊ����ģʽ*/   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*������������*/
	GPIO_Init(P6_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
}

void Keyboard_Init(void)
{
	
    
	Key_GPIO_Config();	
	Keyboard.value=KEY_NONE;//��ֵΪ��
	Keyboard.state=FALSE;	//������Ч
	Keyboard.continuance=FALSE;//����������Ч
	
	LastKeyVal=KEY_NONE;//�ϴμ�ֵ
	LastKeyTicks=0;//��������
	ContinueKeyTicks=0;//������������
	Key_Up_flag=0;
}

void _164Delaay(void)
{
	u16 i;
	for(i=0;i<10;i--)
	;

};
void HW_Sendvalueto164(u8 value)
{
	uint8_t i;
	_Hc164CpL;
	for(i=0;i<8;i++)
	{
		if((value&0x80)==0x80)
			_Hc164DH;
		else
			_Hc164DL;
		_Hc164CpH;
		_164Delaay();
		_Hc164CpL;
		value<<=1;
        //_164Delaay();
	}

}



//==========================================================
//�������ƣ�Key_Read
//�������ܣ���ȡ����
//��ڲ�������
//���ڲ�������
//�������ڣ�2014.09.29
//�޸����ڣ�2014.09.29 10:50
//��ע˵������
//==========================================================
u8 Key_Read(void)
{
	if (Keyboard.state)	//�м�����
	{
		Keyboard.state=FALSE;
		//Beep_One();	//��������һ��
        Key_beep();
        
		return (Keyboard.value);
	}
	//��ֱ�Ӵ����Դ��ͨ�ð���
	return (KEY_NONE);	
}

//==========================================================
//�������ƣ�Key_Read_WithTimeOut
//�������ܣ�����ʽ��ȡ����
//��ڲ�����ticks:�ȴ�������(50ms����)
//���ڲ���������ֵ
//�������ڣ�2014.09.29
//�޸����ڣ�2014.09.29 10:56
//��ע˵����ֱ��������Ӧ����ʱ��
//==========================================================
u8 Key_Read_WithTimeOut(u8 ticks)
{
   // u8 i=1;
//	if (ticks)//��ʱ�ȴ�����
//		{
			SetSoftTimer(KEYBOARD_SOFTTIMER,ticks);//���ð�����ʱ����(����ʱ������,50ms����)
				
//			while ((!GetSoftTimerOut(KEYBOARD_SOFTTIMER))&&(!Keyboard.state)) //��ʱδ�������ް�������
//				{
////					RunOnKeyFree();//�ڰ����ȴ������д�������ҵ��
//                    //i=GetSoftTimerOut(KEYBOARD_SOFTTIMER);
//				}
//		}
//	else//���޵ȴ�����
//		{
//			while (!Keyboard.state) 
//				{
//					RunOnKeyFree();//�ڰ����ȴ������д�������ҵ��
//				}			
//		}
	return Key_Read();//��������
}

//��������ֵ
u8 Touch_Identiry(u8 page,u8 item)//page��������  item�Ƕ�������
{
//	u8 i;
	u8 touch_key=KEY_NONE;
//	if(page==SYS_STATUS_TEST)//���Խ���FIRSTLINE+i*HIGH1
//	{
//		for(i=0;i<6;i++)
//		{
//			if(i<3)
//			{
//				if((XCOOR>DISPX1&&XCOOR<LIST2)&&((YCOOR>(FIRSTLINE+i*HIGH1))&&(YCOOR<(FIRSTLINE+(i+1)*HIGH1))))
//					return 0x70+i;
//			}
//			else
//			{
//				if((XCOOR>DISPX1&&XCOOR<LIST2)&&((YCOOR>(FIRSTLINE+(i-3)*HIGH1))&&(YCOOR<(FIRSTLINE+(i-3+1)*HIGH1))))
//					return 0x70+i;
//			
//			
//			}
//		
//		}
//	
//	
//	}else if(page==SYS_STATUS_SYSSET)//ϵͳ����
//	{
//	
//	}else if(page==SYS_STATUS_SYS)//ϵͳ��Ϣ
//	{
//	
//	}else if(page==SYS_STATUS_SETUP)//���ý���
//	{
//	
//	
//	}
	return touch_key;


}

//ɨ���ֵ
u8  Key_Identiry(void)
{
	u8 key_value;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = P1_PIN;/*ѡ�񰴼�������*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ���ģʽ*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*�������ŵ��������Ϊ�������*/    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;/*������������Ϊ2MHz */    
	GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/


  
	GPIO_InitStructure.GPIO_Pin = P2_PIN;/*ѡ�񰴼�������*/       
	GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/

	
	GPIO_InitStructure.GPIO_Pin = P3_PIN;/*ѡ�񰴼�������*/
	GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/

	

	GPIO_InitStructure.GPIO_Pin = P4_PIN;/*ѡ�񰴼�������*/       
	GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/

	
	GPIO_InitStructure.GPIO_Pin = P5_PIN;/*ѡ�񰴼�������*/       
	GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/

	
	GPIO_InitStructure.GPIO_Pin = P6_PIN;/*ѡ�񰴼�������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*��������Ϊ����ģʽ*/   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*������������*/
	GPIO_Init(P6_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ��          ����*/
	
	GPIO_SetBits(P1_GPIO_PORT,P1_PIN);/*P1���ø�*/
	GPIO_SetBits(P2_GPIO_PORT,P2_PIN);/*P2���ø�*/
	GPIO_SetBits(P3_GPIO_PORT,P3_PIN);/*P3���ø�*/
	GPIO_SetBits(P4_GPIO_PORT,P4_PIN);/*P4���ø�*/
	GPIO_SetBits(P5_GPIO_PORT,P5_PIN);/*P5���ø�*/
//	Key_Delay(0XFFFF);
	if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
	{
		GPIO_InitStructure.GPIO_Pin = P2_PIN;/*ѡ�񰴼�������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*��������Ϊ����ģʽ*/   
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*������������*/
		GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P3_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P4_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P5_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		Key_Delay(0XFF);
		if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
		{
			
//			Key_Delay(0x9FFFF);
			if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
				{
					key_value = 22;//ESC
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
				{
					key_value = 12;//right////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
				{
					key_value = 17;//enter///////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
				{
					key_value = 23;//left////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
				{
					key_value = 13;//FUNC_5
					return key_value;
				}
			}
		}
		GPIO_InitStructure.GPIO_Pin = P2_PIN;/*ѡ�񰴼�������*/     
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ���ģʽ*/   
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*�������ŵ��������Ϊ�������*/    
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;/*������������Ϊ2MHz */    
		GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_SetBits(P2_GPIO_PORT,P2_PIN);/*P2���ø�*/
		GPIO_InitStructure.GPIO_Pin = P1_PIN;/*ѡ�񰴼�������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*��������Ϊ����ģʽ*/   
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*������������*/
		GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P3_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P4_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P5_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
				
		Key_Delay(0xFF);
		if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
		{
			if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
				{
					key_value = 20;//f4//////////
					return key_value;
				}
			}			
			else if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
				{
					key_value = 25;//down
					return key_value;
				}
				}
			else if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
				{
					key_value = '0';//0/////////////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
				{
					key_value = 14;//dot
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
				{
					key_value = 10;//BACKSPACE
					return key_value;
				}
			}
		}
				
			
			
			
			
		GPIO_InitStructure.GPIO_Pin = P3_PIN;/*ѡ�񰴼�������*/     
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ���ģʽ*/   
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*�������ŵ��������Ϊ�������*/    
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;/*������������Ϊ2MHz */    
		GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_SetBits(P3_GPIO_PORT,P3_PIN);/*P2���ø�*/
		GPIO_InitStructure.GPIO_Pin = P1_PIN;/*ѡ�񰴼�������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*��������Ϊ����ģʽ*/   
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*������������*/
		GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P2_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P4_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P5_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		Key_Delay(0XFF);
		if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
		{
			if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
				{
					key_value = 11;//FUNC_3
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
				{
					key_value = '2';//2///////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
				{
					key_value = '5';//5///////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
				{
					key_value ='8';//8///////////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
				{
					key_value = 15;//up
					return key_value;
				}
			}
		}
			
		GPIO_InitStructure.GPIO_Pin = P4_PIN;/*ѡ�񰴼�������*/     
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ���ģʽ*/   
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*�������ŵ��������Ϊ�������*/    
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;/*������������Ϊ2MHz */    
		GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_SetBits(P4_GPIO_PORT,P4_PIN);/*P2���ø�*/
		GPIO_InitStructure.GPIO_Pin = P1_PIN;/*ѡ�񰴼�������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*��������Ϊ����ģʽ*/   
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*������������*/
		GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P2_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P3_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P5_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		Key_Delay(0XFF);
			
		if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
		{
			if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
				{
					key_value = 16;//FUNC_2
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
				{
					key_value = '3';//3/////////////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
				{
					key_value = 18;//ok////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
				{
					key_value = '9';//9///////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
				{
					key_value = '6';//6
					return key_value;
				}
			}
		}
			
		GPIO_InitStructure.GPIO_Pin = P5_PIN;/*ѡ�񰴼�������*/     
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ���ģʽ*/   
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*�������ŵ��������Ϊ�������*/    
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;/*������������Ϊ2MHz */    
		GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_SetBits(P5_GPIO_PORT,P5_PIN);/*P2���ø�*/
		GPIO_InitStructure.GPIO_Pin = P1_PIN;/*ѡ�񰴼�������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*��������Ϊ����ģʽ*/   
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*������������*/
		GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P2_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P3_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		GPIO_InitStructure.GPIO_Pin = P4_PIN;/*ѡ�񰴼�������*/
		GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*ʹ������Ľṹ���ʼ������*/
		Key_Delay(0XFF);
		if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
		{
			if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
				{
					key_value = 21;//FUNC_1
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
				{
					key_value = '1';//1//////////////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
				{
					key_value = 23;//left
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
				{
					key_value = '4';//////////////////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
				{
					key_value = '7';//7////////
					return key_value;
				}
			}
		}

	}
	key_value=Touch_Identiry(GetSystemStatus(),Touch_Item);//��������ɨ��
	if(key_value!=KEY_NONE)
		return key_value;
	/// ��һ����ֵת��
	return KEY_NONE;
	
}
//==========================================================
//�������ƣ�Key_Scan
//�������ܣ�����ɨ��
//��ڲ�������
//���ڲ�������
//�������ڣ�2014.09.28 
//�޸����ڣ�2014.09.28 11:01
//��ע˵������10mS�жϵ���
//==========================================================
// u8 Key_Identiry(void)
//{
////	u8 key;
////    key=Key_Scan();
//    return  Key_Scan()
//    
//	
//    
//}
void Key_Scan(void)
{
	u8 keyval;
	
	keyval = Key_Identiry();//����ʶ�𣬷��ؼ�ֵKey_Up_flag,Touch_Up_flag
	//Touch_Scan();
	
	//��������ʶ��
	if (keyval==KEY_NONE)//�޼�
	{
		if (LastKeyVal!=KEY_NONE)	//�ϴ��м����̰����б�
		{
			if ( (LastKeyTicks>SHORTKEYCOUNT) && (LastKeyTicks<LONGKEYCOUNT) )
			{
				Keyboard.value=LastKeyVal;	//��ֵ
				Keyboard.state=TRUE;		//�а���
			}
		}
		LastKeyVal=KEY_NONE;
		LastKeyTicks=0;
		Keyboard.continuance=FALSE; //��������	
	}
	else	//�м�
	{
		if (LastKeyVal==keyval)//�ϴΰ����ͱ��ΰ�����ͬ
		{
			if (LastKeyTicks<LONGKEYCOUNT+1)//������������100x10mS
				LastKeyTicks++;
			
			if (LastKeyTicks==LONGKEYCOUNT)//���ڳ�����������
			{
				ContinueKeyTicks=0;//������������
				//keyval|=LONG_PRESS;	//������
				Keyboard.value=keyval;	//��ֵ
				Keyboard.state=TRUE;	//�а���
//				Keyboard.continuance=FALSE; //��������
				Keyboard.continuance=TRUE; //��������
			}
			else if (LastKeyTicks>LONGKEYCOUNT)//���ڳ�����������
			{
				if(HW_KEYBOARD_CONTINUE_SUPPORT)//����������Ч�б�
				{
					//keyval|=LONG_PRESS;	//������
					Keyboard.value=keyval;//��ֵ
//					Keyboard.state=TRUE;//�а���
					Keyboard.continuance=TRUE; //��������
					ContinueKeyTicks++;
					if(ContinueKeyTicks>CONTINUEKEYCOUNT)//��������������
					{
						ContinueKeyTicks=0;//������������
						if(Keyboard.state==FALSE)//�����Ѷ�ȡ
							Keyboard.state=TRUE;//�а���
					}
				}
			}					
		}
		else//�ϴΰ����ͱ��ΰ�����ͬ
		{
			LastKeyVal=keyval;
			LastKeyTicks=0;
		}
	}
}

