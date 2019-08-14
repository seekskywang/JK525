#ifndef _KEYBOARD_H
#define _KEYBOARD_H
//==========================================================
//��������
#define _HW_CONFIG_FALSE	(0)
#define _HW_CONFIG_TRUE		(1)

#define TICKS_PER_SEC 100	//ϵͳʱ��ÿ���������100x10mS(T0)=1S
#define TICKS_PER_SEC_SOFTTIMER 100	//��ʱ��ʱ��ÿ���������100x10mS=1S
//Lcd�Ƿ�֧�ֳ�������
#define HW_KEYBOARD_CONTINUE_SUPPORT	(_HW_CONFIG_TRUE)

//==========================================================
//������������

#define 	Key_F1  	21
#define		Key_F2  	11
#define 	Key_F3  	16
#define 	Key_F4  	20
#define		Key_F5 		13
#define		Key_Disp 	 22//9ESC
#define		Key_SETUP	 17//ENT
#define		Key_FAST	18
#define		Key_LEFT	23//
#define		Key_RIGHT  	12//
#define		Key_UP		15
#define		Key_DOWN	25
#define		Key_NUM7	'7'
#define		Key_NUM8	'8'
#define		Key_NUM9	'9'
#define		Key_NUM4	'4'
#define		Key_NUM5	'5'
#define		Key_NUM6	'6'
#define		Key_NUM1	'1'
#define		Key_NUM2	'2'
#define		Key_NUM3	'3'
#define		Key_NUM0	'0'
#define		Key_DOT		14
#define		Key_BACK	10
#define		Key_LOCK	(60)//
#define		Key_BIAS	(61)
#define		Key_REST	(62)
#define		Key_TRIG	(63)
#define		Key_POWER	(64)//



#define _Hc164CpL GPIO_ResetBits(GPIOC,GPIO_Pin_8)//GPIO_ClearValue(1, (1<<1))
#define _Hc164CpH GPIO_SetBits(GPIOC,GPIO_Pin_8)//GPIO_SetValue(1, (1<<1))
#define _Hc164DL GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define _Hc164DH GPIO_SetBits(GPIOC,GPIO_Pin_12)


#define LONGKEYCOUNT 	(TICKS_PER_SEC)	//������������(1s)
//#define SHORTKEYCOUNT 	(TICKS_PER_SEC/20)	//�̰���������(50ms)
#define SHORTKEYCOUNT 	(TICKS_PER_SEC/30)	//�̰���������(33ms)
#define CONTINUEKEYCOUNT (TICKS_PER_SEC/7)	//��������������(100ms)
//��ֵ����
#define HW_KEY_SET			((u16)(ClrD5))
#define HW_KEY_UP			((u16)(ClrD3))
#define HW_KEY_DOWN			((u16)(ClrD4))
#define HW_KEY_LEFT			((u16)(ClrD7))
#define HW_KEY_RIGHT		((u16)(ClrD8))
#define HW_KEY_ENTER		((u16)(ClrD6))
#define HW_KEY_RESET		((u16)(ClrD2))//pc4
#define HW_KEY_START		((u16)(ClrD1))
#define HW_KEY_RESET1		((u16)(ClrD0))//pc4
#define HW_KEY_START1		((u16)(ClrD15))

#define P1_PIN                  GPIO_Pin_9                 
#define P1_GPIO_PORT            GPIOH                      
#define P1_GPIO_CLK             RCC_AHB1Periph_GPIOH

#define P2_PIN                  GPIO_Pin_10                 
#define P2_GPIO_PORT            GPIOH                      
#define P2_GPIO_CLK             RCC_AHB1Periph_GPIOH

#define P3_PIN                  GPIO_Pin_5                
#define P3_GPIO_PORT            GPIOC                      
#define P3_GPIO_CLK             RCC_AHB1Periph_GPIOH

#define P4_PIN                  GPIO_Pin_11                 
#define P4_GPIO_PORT            GPIOH                      
#define P4_GPIO_CLK             RCC_AHB1Periph_GPIOH

#define P5_PIN                  GPIO_Pin_1                 
#define P5_GPIO_PORT            GPIOI                      
#define P5_GPIO_CLK             RCC_AHB1Periph_GPIOI

#define P6_PIN                  GPIO_Pin_3                 
#define P6_GPIO_PORT            GPIOI                      
#define P6_GPIO_CLK             RCC_AHB1Periph_GPIOI
//#define P6_EXTI_PORTSOURCE      EXTI_PortSourceGPIOI
//#define P6_EXTI_PINSOURCE       EXTI_PinSource3
//#define P6_EXTI_LINE            EXTI_Line3
//#define P6_EXTI_IRQ             EXTI0_IRQn

//#define P6_IRQHandler           EXTI0_IRQHandler

/*******************************************************/

 /** �������±��ú�
	* ��������Ϊ�ߵ�ƽ������ KEY_ON=1�� KEY_OFF=0
	* ����������Ϊ�͵�ƽ���Ѻ����ó�KEY_ON=0 ��KEY_OFF=1 ����
	*/
#define KEY_ON	1
#define KEY_OFF	0
/*******************************************************/

 /** ��ֵ
	* 
	*/



//��������
#define LONG_PRESS		((u8)0x80)
#define KEY_NONE		((u8)0xff)
#define KEY_SET			((u8)('a'))
//#define KEY_UP			((u8)('b'))
//#define KEY_DOWN		((u8)('c'))
//#define KEY_LEFT		((u8)('d'))
//#define KEY_RIGHT		((u8)('e'))
//#define KEY_ENTER		((u8)('f'))
#define KEY_RESET		((u8)('g'))
#define KEY_START		((u8)('h'))

#define L_KEY_SET		(LONG_PRESS | KEY_SET)		//����
#define L_KEY_UP		(LONG_PRESS | KEY_UP)		//��
#define L_KEY_DOWN		(LONG_PRESS | KEY_DOWN)		//��
#define L_KEY_LEFT		(LONG_PRESS | KEY_LEFT)		//��
#define L_KEY_RIGHT		(LONG_PRESS | KEY_RIGHT)	//��
#define L_KEY_ENTER		(LONG_PRESS | KEY_ENTER)	//ȷ��
#define L_KEY_RESET		(LONG_PRESS | KEY_RESET)	//��λ
#define L_KEY_START		(LONG_PRESS | KEY_START)	//����
//==========================================================
//�����ṹ������
typedef struct 
{
	u8 value;	//��ֵ
	u8 state;	//���а���=TRUE
	u8 continuance;//����������Ч=TRUE
}Key_TypeDef;

extern  uint32_t CurrentTextColor   ;
extern  uint32_t CurrentBackColor   ;
//==========================================================
//ȫ�ֱ���
extern Key_TypeDef Keyboard;//����ṹ��

//==========================================================
//��������
extern u8 Key_Identiry(void);//����ʶ�𣬷��ؼ�ֵ
extern void Keyboard_Init(void);//�����ṹ���ʼ��
extern void Key_Scan(void);//����ɨ��
extern u8 Key_Read(void);//��ȡ���������ؼ�ֵ
extern u8 Key_Read_WithTimeOut(u8 timeticks);//����ʽ��ȡ���������ؼ�ֵ


#endif 
