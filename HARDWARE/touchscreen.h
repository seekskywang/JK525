#ifndef __TOUCH_SCREEN_H
#define	__TOUCH_SCREEN_H

#include "stm32f4xx.h"


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

extern u8 touchflag;
extern float XCOOR;
extern float YCOOR;
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

void Touch_GPIO_Config(void);
//void Touch_Init(void);
#endif /* __TOUCH_SCREEN_H */

