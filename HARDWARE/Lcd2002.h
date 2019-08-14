//==========================================================
//�ļ����ƣ�Lcd2002.h
//����������ͨ��2002�ַ�Һ������ͷ�ļ�
//�ļ��汾��Ver1.0
//�������ڣ�2014.09.28
//�޸����ڣ�2014.09.28 16:38
//�ļ����ߣ��ƺ���
//�ļ���ע����
//ע�������
//==========================================================
#ifndef __Lcd2002_h__
#define __Lcd2002_h__

//==========================================================
//LCD2002��ַ����
////9820A
////#define LCD_BUS P0 //���ݿڽ�P0��
////sbit	LCD_DI = P2^7;//Һ�������������
////sbit	LCD_RW = P2^6;//Һ����д����
////sbit	LCD_EN = P2^5;//Һ��EN����(�����أ�1��Ч)
//#define LCD_CMD_WR_BUS  XBYTE[0x3FFF]
//#define LCD_CMD_RD_BUS  XBYTE[0x7FFF]
//#define LCD_DAT_WR_BUS  XBYTE[0xBFFF]
//#define LCD_DAT_RD_BUS  XBYTE[0xFFFF]

//9850
//#define LCD_CMD_WR_BUS  XBYTE[0xC0FF]
//#define LCD_CMD_RD_BUS  XBYTE[0xC8FF]
//#define LCD_DAT_WR_BUS  XBYTE[0xE000]
//#define LCD_DAT_RD_BUS  XBYTE[0xE8FF]

//1602Һ��ָ��/����ѡ������
#define	LCD_RS_Set()	GPIOC->BSRR=GPIO_Pin_10//GPIO_SetBits(GPIOC, GPIO_Pin_10)
#define	LCD_RS_Clr()	GPIOC->BRR=GPIO_Pin_10

//1602Һ����д����
#define	LCD_RW_Set()	GPIOC->BSRR=GPIO_Pin_11//GPIOC->ODR |=GPIO_Pin_11
#define	LCD_RW_Clr()	GPIOC->BRR=GPIO_Pin_11

//1602Һ��ʹ������
#define	LCD_EN_Set()	GPIOC->BSRR=GPIO_Pin_12
#define	LCD_EN_Clr()	GPIOC->BRR=GPIO_Pin_12

//1602Һ�����ݶ˿�	PD0~7
#define	DATAOUT(x)	GPIOD->ODR=x;//GPIO_Write(GPIOD, x)

#define Bank1_LCD_C        ((u32)0x6C000000)
#define Bank1_LCD_D        ((u32)0x6C000002)

#define LCD_RAM           *(__IO uint8_t *)(Bank1_LCD_D)
#define LCD_REG           *(__IO uint8_t *)(Bank1_LCD_C)

//==========================================================
//LCD��������
#define LCD_CMD_XSFW	(u8)(0x30)	//��ʾ��λ����
#define LCD_CMD_DYHDZ	(u8)(0x80)	//��һ����ʾ��ַ
#define LCD_CMD_DEHDZ	(u8)(0xC0)	//�ڶ�����ʾ��ַ
#define LCD_CMD_DDDZ	(u8)(0x80)	//DDRAM��ַ����(D7=1):[D6-D0Ϊ��ַ,��7λ��ַд��AC��,���Ĳ������Դ�]
#define LCD_CMD_GGDZ	(u8)(0x40)	//GGRAM��ַ����(D6=1):[D5-D0Ϊ��ַ,�û��Զ����ַ����ַ���ַ]
#define LCD_CMD_GZFS	(u8)(0x38)	//������ʽ����(D5=1):[DL=D4=1:8λ���߿��(0Ϊ4λ);N=D3=2:������ʾ(0Ϊһ��);F=D2=0:5X7����(0Ϊ5X11)]
#define LCD_CMD_HMGD	(u8)(0x18)	//�������������(D4=1):[S/C=D3=1:�������(0Ϊ������);R/L=D2=1:�ҹ���(0�����)]
#define LCD_CMD_XSZT	(u8)(0x0C)	//��ʾ״̬����(D3=1):[D=D2=1:��ʾ��(0Ϊ��);C=D1=0:��겻��ʾ(1��ʾ),B=D0=0:��˸�ر�(1����)]
#define LCD_CMD_SRFS	(u8)(0x06)	//���뷽ʽ����(D2=1):[I/D=D1=1:AC���ݵ�ַָ���Զ���1(0Ϊ����),S=D0=1:���������/����ƶ�(0Ϊ��ֹ)]
#define LCD_CMD_DZQL	(u8)(0x02)	//���ݵ�ַָ����������(D1=1)
#define LCD_CMD_XSQP	(u8)(0x01)	//��ʾ��������(D0=1)

//==========================================================
//��������
#define LCDADDR_X_LIMIT ((u8)(20-1))//�е�ַ��ֵ(20��)
#define LCDADDR_Y_LIMIT ((u8)(2-1))//�е�ַ��ֵ(2��)
#define LCDADDR_MAX 	((u8)(20*2))//���һ����ʾ���ַ�����(40��)

//LCD����ṹ�嶨��
typedef struct 
{
	u8 x;	//������
	u8 y;	//������
}LcdAddr_TypeDef;

//==========================================================
//ȫ�ֱ���
extern LcdAddr_TypeDef LcdAddr;//LCD��ַ�ṹ��

//==========================================================
//�����б�
//u8 Lcd_ReadStatus(void);//��LCD״̬��
void Lcd_ReadBF(void);//��ȡҺ����״̬λֱ��BF��־��Ч(��200�κ��Զ��˳�)
void Lcd_WriteCmd(u8);//д����
void Lcd_WriteData(u8);//д����
//u8 Lcd_ReadData(void);//����ǰ��ַ������
void Lcd_SetAddr(void);//LCD����LcdAdr����
// void Lcd_WriteDataAt(u8 dat);//��ַLcdAdrд����
//u8 Lcd_ReadDataAt(void);//��ַLcdAdr������
void Lcd_Clr(void);//ȫ������
void Lcd_Init(void);//Һ����ʼ��
void Lcd_AddrUpdate(void);//LCD����LcdAdr����


#endif
