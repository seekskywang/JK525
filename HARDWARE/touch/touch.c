#include "touch.h" 
//#include "lcd.h"
//#include "bsp_lcd.h"
#include "stdlib.h"
#include "math.h"
#include "pbdata.h"
#include "stm32f4xx.h"
//#include "stm32f4xx_adc.h"
//#include "24cxx.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//������������֧��ADS7843/7846/UH7843/7846/XPT2046/TSC2046/OTT2001A�ȣ� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.2
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									   
//********************************************************************************
//�޸�˵��
//V1.1 20140721
//����MDK��-O2�Ż�ʱ,�����������޷���ȡ��bug.��TP_Write_Byte�������һ����ʱ,�������.
//V1.2 20141130 
//���ݴ���������FT5206��֧��
//////////////////////////////////////////////////////////////////////////////////
u8 touch_flag;
__IO uint16_t ADC_Converted[RHEOSTAT_NOFCHANEL1]={0};
_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	TP_Adjust,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};					
//Ĭ��Ϊtouchtype=0������.
u8 CMD_RDX=0;
u8 CMD_RDY=1;

		 
void Touch_GPIO_Config(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*������ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (TOUCH_XPLUS_GPIO_CLK|TOUCH_XMINUS_GPIO_CLK|TOUCH_YPLUS_GPIO_CLK|
	TOUCH_YMINUS_GPIO_CLK,ENABLE); 

	GPIO_InitStructure.GPIO_Pin = TOUCH_YPLUS_GPIO_PIN|TOUCH_YMINUS_GPIO_PIN;/*ѡ������*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ����ģʽ*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;/*������������Ϊ2MHz */    
	GPIO_Init(TOUCH_YPLUS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TOUCH_XMINUS_GPIO_PIN;/*ѡ������*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ���ģʽ*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*�������ŵ��������Ϊ�������*/    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;/*������������Ϊ2MHz */    
	GPIO_Init(TOUCH_XMINUS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TOUCH_XPLUS_GPIO_PIN;/*ѡ������*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*��������Ϊ���ģʽ*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*�������ŵ��������Ϊ�������*/    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*��������Ϊ����ģʽ*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;/*������������Ϊ2MHz */    
	GPIO_Init(TOUCH_XPLUS_GPIO_PORT, &GPIO_InitStructure);
	
//	Touch_Up_flag=0;
//	GPIO_ResetBits(TOUCH_XMINUS_GPIO_PORT,TOUCH_XMINUS_GPIO_PIN);
}
static void AD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/*=====================ͨ��1======================*/	
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(ADC_GPIO_CLK0,ENABLE);		
	// ���� IO
    GPIO_InitStructure.GPIO_Pin = ADC_GPIO_PIN0|ADC_GPIO_PIN1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    //������������	
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(ADC_GPIO_PORT0, &GPIO_InitStructure);

}
static void Rheostat_ADC_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
  // ------------------DMA Init �ṹ����� ��ʼ��--------------------------
  // ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
  // ����DMAʱ��
  RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_DMA_CLK, ENABLE); 
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;	
  // �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_Converted;  
  // ���ݴ��䷽��Ϊ���赽�洢��	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// ��������СΪ��ָһ�δ����������
	DMA_InitStructure.DMA_BufferSize = RHEOSTAT_NOFCHANEL1;	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // �洢����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  // // �������ݴ�СΪ���֣��������ֽ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO ��С��FIFOģʽ��ֹʱ�������������	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// ѡ�� DMA ͨ����ͨ������������
  DMA_InitStructure.DMA_Channel = RHEOSTAT_ADC_DMA_CHANNEL; 
  //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	DMA_Init(RHEOSTAT_ADC_DMA_STREAM, &DMA_InitStructure);
	// ʹ��DMA��
  DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);
	
	// ����ADCʱ��
	RCC_APB2PeriphClockCmd(RHEOSTAT_ADC_CLK , ENABLE);
  // -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	// ����ADCģʽ
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // ʱ��Ϊfpclk x��Ƶ	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  // ��ֹDMAֱ�ӷ���ģʽ	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // ����ʱ����	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
  // ADC �ֱ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // ɨ��ģʽ����ͨ���ɼ���Ҫ	
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  // ����ת��	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  //��ֹ�ⲿ���ش���
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //ʹ������������ⲿ�����������ã�ע�͵�����
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //�����Ҷ���	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //ת��ͨ�� 1��
  ADC_InitStructure.ADC_NbrOfConversion = RHEOSTAT_NOFCHANEL1;                                    
  ADC_Init(RHEOSTAT_ADC, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // ���� ADC ͨ��ת��˳��Ͳ���ʱ������
  ADC_RegularChannelConfig(RHEOSTAT_ADC, ADC_CHANNEL0, 1, 
	                         ADC_SampleTime_144Cycles);
  ADC_RegularChannelConfig(RHEOSTAT_ADC, ADC_CHANNEL1, 2, 
	                         ADC_SampleTime_144Cycles); 

  // ʹ��DMA���� after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(RHEOSTAT_ADC, ENABLE);
  // ʹ��ADC DMA
  ADC_DMACmd(RHEOSTAT_ADC, ENABLE);
	
	// ʹ��ADC
  ADC_Cmd(RHEOSTAT_ADC, ENABLE);  
  //��ʼadcת�����������
  ADC_SoftwareStartConv(RHEOSTAT_ADC);
}
u16 TP_Read_AD(u8 CMD)	  
{ 	 

	u16 Num=0; 
	if(CMD)
	{
		
		set_yy();
		delay_1us(40);
		Num = ADC_Converted[1];
		

	}else{
		
		set_xx();
		delay_1us(40);
		Num = ADC_Converted[0];
		
	
	}
	return(Num);   
}
//��ȡһ������ֵ(x����y)
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
//xy:ָ�CMD_RDX/CMD_RDY��
//����ֵ:����������
#define READ_TIMES 3 	//��ȡ����
#define LOST_VAL 1	  	//����ֵ
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
		buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//��ȡx,y����
//��Сֵ��������100.
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;	
	//set_mid();	
	xtemp=TP_Read_XOY(CMD_RDX);
	//set_mid();
	ytemp=TP_Read_XOY(CMD_RDY);	 
	//set_mid();	
	if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}
//����2�ζ�ȡ������IC,�������ε�ƫ��ܳ���
//ERR_RANGE,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
#define ERR_RANGE 50 //��Χ 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
		
        return 1;
    }else return 0;	  
}  
//////////////////////////////////////////////////////////////////////////////////		  
//��LCD�����йصĺ���  
//��һ��������
//����У׼�õ�
//x,y:����
//color:��ɫ
void TP_Drow_Touch_Point(u16 x,u16 y,u32 color)
{
	POINT_COLOR=color;
	LCD_DrawLine(x+12,y,25,0);//����
	LCD_DrawLine(x,y+13,25,1);//����
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x-2,y+1);
	LCD_DrawPoint(x,y-1);
	LCD_DrawPoint(x-2,y-1);
	LCD_Draw_Circle(x,y,6);//������Ȧ
}	  
//��һ�����(2*2�ĵ�)		   
//x,y:����
//color:��ɫ
void TP_Draw_Big_Point(u16 x,u16 y,u32 color)
{	    
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);//���ĵ� 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}						  
//////////////////////////////////////////////////////////////////////////////////		  
//��������ɨ��
//tp:0,��Ļ����;1,��������(У׼�����ⳡ����)
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
u8 TP_Scan(u8 tp)
{		
	u16 value;
	set_yz(); 
    delay_us(20);
value=ADC_Converted[0];	
	//if(PEN==0)//�а�������
	if(value>100)
	touch_flag=1;
	if(touch_flag)//
	{
		touch_flag=0;
		if(tp)TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]);//��ȡ��������
		else if(TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]))//��ȡ��Ļ����
		{
	 		tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;//�����ת��Ϊ��Ļ����
			tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)//֮ǰû�б�����
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//��������  
			tp_dev.x[4]=tp_dev.x[0];//��¼��һ�ΰ���ʱ������
			tp_dev.y[4]=480-tp_dev.y[0];  	   			 
		}			   
	}
	else
	{
		if(tp_dev.sta&TP_PRES_DOWN)//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);//��ǰ����ɿ�	
		}else//֮ǰ��û�б�����
		{
			tp_dev.x[4]=0;
			tp_dev.y[4]=0;
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;//���ص�ǰ�Ĵ���״̬
}	  
//////////////////////////////////////////////////////////////////////////	 
//������EEPROM����ĵ�ַ�����ַ,ռ��13���ֽ�(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
//����У׼����										    
void TP_Save_Adjdata(void)
{
	s32 temp;			 
	//����У�����!		   							  
	Jk516save.Touch_save.x_pos=tp_dev.xfac*100000000;//����xУ������      
      
	Jk516save.Touch_save.y_pos=tp_dev.yfac*100000000;//����yУ������    
    
	//����xƫ����
	Jk516save.Touch_save.xoff=tp_dev.xoff;
    		    
	//����yƫ����
	Jk516save.Touch_save.yoff=tp_dev.yoff;	
	//���津������
	Jk516save.Touch_save.save_flag=0x0a;
	Store_set_flash();
}
//�õ�������EEPROM�����У׼ֵ
//����ֵ��1���ɹ���ȡ����
//        0����ȡʧ�ܣ�Ҫ����У׼
u8 TP_Get_Adjdata(void)
{					  
	s32 tempfac;
	tempfac=Jk516save.Touch_save.save_flag;//��ȡ�����,���Ƿ�У׼���� 		 
	if(tempfac==0X0A)//�������Ѿ�У׼����			   
	{    												 
		tempfac=Jk516save.Touch_save.x_pos;		   
		tp_dev.xfac=(float)tempfac/100000000;//�õ�xУ׼����
		tempfac=Jk516save.Touch_save.y_pos;			          
		tp_dev.yfac=(float)tempfac/100000000;//�õ�yУ׼����
	    //�õ�xƫ����
		tp_dev.xoff=Jk516save.Touch_save.xoff;			   	  
 	    //�õ�yƫ����
		tp_dev.yoff=Jk516save.Touch_save.yoff;				 	  
 		//tp_dev.touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);//��ȡ�������ͱ��
//		if(tp_dev.touchtype)//X,Y��������Ļ�෴
//		{
//			CMD_RDX=0X90;
//			CMD_RDY=0XD0;	 
//		}else				   //X,Y��������Ļ��ͬ
//		{
//			CMD_RDX=0XD0;
//			CMD_RDY=0X90;	 
//		}		 
		return 1;	 
	}
	return 0;
}	 
//��ʾ�ַ���
u8* const TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";
 					  
//��ʾУ׼���(��������)
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
{	  
//	POINT_COLOR=RED;
//	LCD_ShowString(40,160,lcddev.width,lcddev.height,16,"x1:");
// 	LCD_ShowString(40+80,160,lcddev.width,lcddev.height,16,"y1:");
// 	LCD_ShowString(40,180,lcddev.width,lcddev.height,16,"x2:");
// 	LCD_ShowString(40+80,180,lcddev.width,lcddev.height,16,"y2:");
//	LCD_ShowString(40,200,lcddev.width,lcddev.height,16,"x3:");
// 	LCD_ShowString(40+80,200,lcddev.width,lcddev.height,16,"y3:");
//	LCD_ShowString(40,220,lcddev.width,lcddev.height,16,"x4:");
// 	LCD_ShowString(40+80,220,lcddev.width,lcddev.height,16,"y4:");  
// 	LCD_ShowString(40,240,lcddev.width,lcddev.height,16,"fac is:");     
//	LCD_ShowNum(40+24,160,x0,4,16);		//��ʾ��ֵ
//	LCD_ShowNum(40+24+80,160,y0,4,16);	//��ʾ��ֵ
//	LCD_ShowNum(40+24,180,x1,4,16);		//��ʾ��ֵ
//	LCD_ShowNum(40+24+80,180,y1,4,16);	//��ʾ��ֵ
//	LCD_ShowNum(40+24,200,x2,4,16);		//��ʾ��ֵ
//	LCD_ShowNum(40+24+80,200,y2,4,16);	//��ʾ��ֵ
//	LCD_ShowNum(40+24,220,x3,4,16);		//��ʾ��ֵ
//	LCD_ShowNum(40+24+80,220,y3,4,16);	//��ʾ��ֵ
// 	LCD_ShowNum(40+56,240,fac,3,16); 	//��ʾ��ֵ,����ֵ������95~105��Χ֮��.

}
		 
//������У׼����
//�õ��ĸ�У׼����
void TP_Adjust(void)
{								 
	u16 pos_temp[4][2];//���껺��ֵ
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	double fac; 	
	u16 outtime=0;
 	cnt=0;				
//	POINT_COLOR=BLUE;
//	BACK_COLOR =WHITE;
	LCD_Clear(LCD_COLOR_WHITE);//����   
//	POINT_COLOR=RED;//��ɫ 
//	LCD_Clear(WHITE);//���� 	   
//	POINT_COLOR=BLACK;
//	LCD_ShowString(40,40,160,100,16,(u8*)TP_REMIND_MSG_TBL);//��ʾ��ʾ��Ϣ
	TP_Drow_Touch_Point(20,20,LCD_COLOR_RED);//����1 
	tp_dev.sta=0;//���������ź� 
	tp_dev.xfac=0;//xfac��������Ƿ�У׼��,����У׼֮ǰ�������!�������	 
	while(1)//�������10����û�а���,���Զ��˳�
	{
		tp_dev.scan(1);//ɨ����������
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//����������һ��(��ʱ�����ɿ���.)
		{	
			outtime=0;		
			tp_dev.sta&=~(1<<6);//��ǰ����Ѿ����������.
						   			   
			pos_temp[cnt][0]=tp_dev.x[0];
			pos_temp[cnt][1]=tp_dev.y[0];
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					TP_Drow_Touch_Point(20,20,LCD_COLOR_WHITE );	 			//�����1 
					TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,20,LCD_COLOR_RED);	//����2
					break;
				case 2:
 					TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,20,LCD_COLOR_WHITE);	//�����2
					TP_Drow_Touch_Point(20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_RED);	//����3
					break;
				case 3:
 					TP_Drow_Touch_Point(20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_WHITE);			//�����3
 					TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_RED);	//����4
					break;
				case 4:	 //ȫ���ĸ����Ѿ��õ�
	    		    //�Ա����
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�õ�1,2�ľ���
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�õ�3,4�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)//���ϸ�
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_WHITE);	//�����4
   	 					TP_Drow_Touch_Point(20,20,LCD_COLOR_RED);								//����1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����   
 						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�õ�1,3�ľ���
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�õ�2,4�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//���ϸ�
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,LCD_PIXEL_HEIGHT,LCD_COLOR_WHITE);	//�����4
   	 					TP_Drow_Touch_Point(20,20,LCD_COLOR_RED);								//����1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����   
						continue;
					}//��ȷ��
								   
					//�Խ������
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�õ�1,4�ľ���
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�õ�2,3�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//���ϸ�
					{
						cnt=0;

						TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_WHITE);	//�����4
   	 					TP_Drow_Touch_Point(20,20,LCD_COLOR_RED);	
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����   
						continue;
					}//��ȷ��
					//������
					tp_dev.xfac=(float)(LCD_PIXEL_WIDTH-40)/(pos_temp[1][0]-pos_temp[0][0]);//�õ�xfac		 
					tp_dev.xoff=(LCD_PIXEL_WIDTH-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//�õ�xoff
						  
					tp_dev.yfac=(float)(LCD_PIXEL_HEIGHT-40)/(pos_temp[2][1]-pos_temp[0][1]);//�õ�yfac
					tp_dev.yoff=(LCD_PIXEL_HEIGHT-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//�õ�yoff  
					if(fabs(tp_dev.xfac)>2||fabs(tp_dev.yfac)>2)//������Ԥ����෴��.
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_WHITE);	//�����4
   	 					TP_Drow_Touch_Point(20,20,LCD_COLOR_RED);								//����1
						//LCD_ShowString(40,26,lcddev.width,lcddev.height,16,"TP Need readjust!");
//						tp_dev.touchtype=!tp_dev.touchtype;//�޸Ĵ�������.
//						if(tp_dev.touchtype)//X,Y��������Ļ�෴
//						{
//							CMD_RDX=0;
//							CMD_RDY=1;	 
//						}else				   //X,Y��������Ļ��ͬ
//						{
//							CMD_RDX=1;
//							CMD_RDY=0;	 
//						}			    
						continue;
					}		
//					POINT_COLOR=BLUE;
//					LCD_Clear(WHITE);//����
//					LCD_ShowString(35,110,lcddev.width,lcddev.height,16,"Touch Screen Adjust OK!");//У�����
//					delay_ms(1000);
					TP_Save_Adjdata();  
// 					LCD_Clear(WHITE);//����   
					return;//У�����				 
			}
		}
		delay_ms(10);
		outtime++;
		if(outtime>1000)
		{
			TP_Get_Adjdata();
			break;
	 	} 
 	}
}	
//��������ʼ��  		    
//����ֵ:0,û�н���У׼
//       1,���й�У׼
u8 TP_Init(void)
{
//  GPIO_InitTypeDef  GPIO_InitStructure;	
	AD_GPIO_Config();
	Rheostat_ADC_Mode_Config();
	

		
   
		TP_Read_XY(&tp_dev.x[0],&tp_dev.y[0]);//��һ�ζ�ȡ��ʼ��	 

		if(TP_Get_Adjdata())return 0;//�Ѿ�У׼
		else			   //δУ׼?
		{ 										    //
//			LCD_Clear(WHITE);//����
			TP_Adjust();  	//��ĻУ׼ 
			TP_Save_Adjdata();	 
		}			
//		TP_Get_Adjdata();	
//	}
	return 1; 									 
}
