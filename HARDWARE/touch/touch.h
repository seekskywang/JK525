#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "stm32f4xx.h"
//#include "ott2001a.h"	    
//#include "gt9147.h"	    
//#include "ft5206.h"	    
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


#define TOUCH_XPLUS_GPIO_PORT     GPIOB
#define TOUCH_XPLUS_GPIO_CLK      RCC_AHB1Periph_GPIOB
#define TOUCH_XPLUS_GPIO_PIN      GPIO_Pin_9
#define TOUCH_XPLUS_PINSOURCE     GPIO_PinSource9


#define TOUCH_XMINUS_GPIO_PORT    GPIOG
#define TOUCH_XMINUS_GPIO_CLK     RCC_AHB1Periph_GPIOG
#define TOUCH_XMINUS_GPIO_PIN     GPIO_Pin_13
#define TOUCH_XMINUS_PINSOURCE    GPIO_PinSource13

#define TOUCH_YPLUS_GPIO_PORT     GPIOI
#define TOUCH_YPLUS_GPIO_CLK      RCC_AHB1Periph_GPIOI
#define TOUCH_YPLUS_GPIO_PIN      GPIO_Pin_7


#define TOUCH_YMINUS_GPIO_PORT     GPIOI
#define TOUCH_YMINUS_GPIO_CLK      RCC_AHB1Periph_GPIOI
#define TOUCH_YMINUS_GPIO_PIN      GPIO_Pin_6
#define TOUCH_YMINUS_PINSOURCE     GPIO_PinSource6


#define X_P0 		GPIO_ResetBits(TOUCH_XPLUS_GPIO_PORT,TOUCH_XPLUS_GPIO_PIN)
#define X_P1 		GPIO_SetBits(TOUCH_XPLUS_GPIO_PORT,TOUCH_XPLUS_GPIO_PIN)

#define X_N0 		GPIO_ResetBits(TOUCH_XMINUS_GPIO_PORT,TOUCH_XMINUS_GPIO_PIN)
#define X_N1 		GPIO_SetBits(TOUCH_XMINUS_GPIO_PORT,TOUCH_XMINUS_GPIO_PIN)

#define Y_P0 		GPIO_ResetBits(TOUCH_YMINUS_GPIO_PORT,TOUCH_YMINUS_GPIO_PIN)
#define Y_P1 		GPIO_SetBits(TOUCH_YMINUS_GPIO_PORT,TOUCH_YMINUS_GPIO_PIN)

#define Y_N0 		GPIO_ResetBits(TOUCH_YPLUS_GPIO_PORT,TOUCH_YPLUS_GPIO_PIN)
#define Y_N1 		GPIO_SetBits(TOUCH_YPLUS_GPIO_PORT,TOUCH_YPLUS_GPIO_PIN)

///////////////////////////////////////////////////////

#define ADC_GPIO_PORT0    GPIOA
#define ADC_GPIO_PIN0     GPIO_Pin_0
#define ADC_GPIO_CLK0     RCC_AHB1Periph_GPIOA
#define ADC_CHANNEL0      ADC_Channel_0

#define ADC_GPIO_PORT1    GPIOA
#define ADC_GPIO_PIN1     GPIO_Pin_1
#define ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOA
#define ADC_CHANNEL1      ADC_Channel_1

#define RHEOSTAT_NOFCHANEL1      2


//void Touch_Scan(void);
// ADC ��ź궨��
#define RHEOSTAT_ADC              ADC1
#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC1
// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define RHEOSTAT_ADC_DR_ADDR    ((u32)ADC1+0x4c)


// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0

#define TP_PRES_DOWN 0x80  //����������	  
#define TP_CATH_PRES 0x40  //�а��������� 
#define CT_MAX_TOUCH  5    //������֧�ֵĵ���,�̶�Ϊ5��

//������������
typedef struct
{
	u8 (*init)(void);			//��ʼ��������������
	u8 (*scan)(u8);				//ɨ�败����.0,��Ļɨ��;1,��������;	 
	void (*adjust)(void);		//������У׼ 
	u16 x[CT_MAX_TOUCH]; 		//��ǰ����
	u16 y[CT_MAX_TOUCH];		//�����������5������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
								//x[4],y[4]�洢��һ�ΰ���ʱ������. 
	u8  sta;					//�ʵ�״̬ 
								//b7:����1/�ɿ�0; 
	                            //b6:0,û�а�������;1,�а�������. 
								//b5:����
								//b4~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
/////////////////////������У׼����(����������ҪУ׼)//////////////////////								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
//b0:0,����(�ʺ�����ΪX����,����ΪY�����TP)
//   1,����(�ʺ�����ΪY����,����ΪX�����TP) 
//b1~6:����.
//b7:0,������
//   1,������ 
	u8 touchtype;
}_m_tp_dev;
 static void set_yz(void){	X_P0;	X_N0;	Y_P1;	Y_N1;}
static void set_yy(void){	X_P0;	X_N1;	Y_P1;	Y_N0;}
static void set_xx(void){	X_P1;	X_N0;	Y_P0;	Y_N1;}
static void set_mid(void){	X_P0;	X_N1;	Y_P1;	Y_N0;}
extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��
extern u8 touch_flag;

//������оƬ��������	   
#define PEN  		PBin(1)  	//T_PEN
#define DOUT 		PBin(2)   	//T_MISO
#define TDIN 		PFout(11)  	//T_MOSI
#define TCLK 		PBout(0)  	//T_SCK
#define TCS  		PCout(13)  	//T_CS  
   
//����������
//void TP_Write_Byte(u8 num);						//�����оƬд��һ������
u16 TP_Read_AD(u8 CMD);							//��ȡADת��ֵ
u16 TP_Read_XOY(u8 xy);							//���˲��������ȡ(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);					//˫�����ȡ(X+Y)
u8 TP_Read_XY2(u16 *x,u16 *y);					//����ǿ�˲���˫���������ȡ
void TP_Drow_Touch_Point(u16 x,u16 y,u32 color);//��һ������У׼��
void TP_Draw_Big_Point(u16 x,u16 y,u32 color);	//��һ�����
void TP_Save_Adjdata(void);						//����У׼����
u8 TP_Get_Adjdata(void);						//��ȡУ׼����
void TP_Adjust(void);							//������У׼
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac);//��ʾУ׼��Ϣ
//������/������ ���ú���
u8 TP_Scan(u8 tp);								//ɨ��
u8 TP_Init(void);								//��ʼ��
 void Touch_GPIO_Config(void) ;
#endif

















