#ifndef __SPI_H
#define __SPI_H
//#include "sys.h"
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

 
 				  	    													  
void SPI3_Init(void);			 //��ʼ��SPI��
void SPI3_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI3_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
		 
#endif

