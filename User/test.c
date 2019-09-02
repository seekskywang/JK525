#include "pbdata.h"
#include <math.h>
#include "ff.h"
#include "cpld.h"
#include "./APP/usbh_bsp.h"
#include "./RTC/bsp_rtc.h"
#include "bsp_exti.h"
//#include "touchscreen.h"
#include "./touch/touch.h"
#include "./ch376/ch376.h"
#include "FILESYS.h"



//反馈切换
// 输入0 1 2 3  U16_4094
// 0  1000V
//1   100V
//2 10V 
//==========================================================
#define POWERON_DISP_TIME (20)	//开机显示界面延时20*100mS=2s
u8 U15_4094,U16_4094;

FRESULT result;
FATFS fs;
FIL file;
//const u8 RANGE_UNIT[11]=
//{
//	4,
//	3,
//	2,
//	1,
//	3,
//	2,
//	1,
//	3,
//	2,
//	1,
//	3
//	


//};
//==========================================================
//函数名称：Power_Process
//函数功能：上电处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.26 08:53
//备注说明：开机长按SET进入校准调试模式
//==========================================================
//void DIS_Line(void)
//{
//  uint16_t i,j;
//  
//  uint32_t *p = (uint32_t *)(LCD_LAYER2_START_ADDR );
//  
//  for(j=0;j<480;j++)
//  for(i=0;i<272;i++)
//  {
//    //ARGB8888 
//    *p = 0x7Fff00;
//    p++;
//    }
//  
//  
//}  
void Power_Process(void)
{
	u16 i;
    u8 j;
    Disp_Coordinates_Typedef Debug_Cood;
   
	u8 key;
    Int_Pe3flag=0;
	
//    Turnon_Led();
//    Debug_USART_Config();//串口1
    RTC_CLK_Config();
    RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//配置WAKE UP中断,1秒钟中断一次
    GPIO_Configuration();//端口初始化
	Init_CH376();
    Ads1251_Int();
     if (RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
      {
        /* 设置时间和日期 */
            RTC_TimeAndDate_Set();
      }
      else
      {
        /* 检查是否电源复位 */   
        /* 使能 PWR 时钟 */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
            /* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
            PWR_BackupAccessCmd(ENABLE);
            /* 等待 RTC APB 寄存器同步 */
            RTC_WaitForSynchro();   
      } 
    SysTick_Init();
	  set_mid(); 
    delay_ms(10);
    
//    Keyboard_Init();//按键初始化
    TIM6_Configuration();

    //////////////////////////////////////////液晶初始化
    LCD_Init();
    LCD_LayerInit();
    LTDC_Cmd(ENABLE);
	/*把背景层刷黑色*/
    LCD_SetLayer(LCD_BACKGROUND_LAYER);  
    LCD_Clear(LCD_COLOR_RED);
	
  /*初始化后默认使用前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
//	/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
    LCD_SetTransparency(0xff);
	LCD_Clear(LCD_COLOR_TEST_BACK);
      TIM_PWMOUTPUT_Config(50);
    SPI_FLASH_Init();
	InitGlobalValue();//初始化全局变量
	Read_set_flash();
//    lcd_image((uint8_t *)gImage_open);
	Parameter_valuecomp();//比较读出的数据

    TIM6_Configuration();//定时器6定时10ms
	
	MenuIndex=0;//待机菜单项
	i=0;//显示延时
    open_flag=1;
//    Range=3;
    Range_Control(Range);
    RangeChange_Flag=1;
	Touch_GPIO_Config();
	//CH376DirCreate( wang.txt );
//	Touch_Init();
//     USBH_Init(&USB_OTG_Core,
//			USB_OTG_HS_CORE_ID,
//            &USB_Host,
//            &USBH_MSC_cb,
//            &USR_cb);
//     USBH_Process(&USB_OTG_Core, &USB_Host);
	tp_dev.init();				//触摸屏初始化
	while(GetSystemStatus()==SYS_STATUS_POWER)
	{
		i++;
        j++;
       
		if(i>POWERON_DISP_TIME)//延时20*100mS=2s后自动退出
            SetSystemStatus(SYS_STATUS_TEST);//待测状态

        delay_ms(100);
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);//等待按键(100*10ms/10=100ms)
        if(key!=KEY_NONE)
        {
            switch(key)
            {
                case	    Key_F1:
                    Trip_On();
                    break;
                case		Key_F2:
                    Trip_Off();
                    break;
                case 	    Key_F3:
                    Lock_On();
                    break;
                case 	    Key_F4:
                    Lock_Off();
                    break;
                case		Key_F5:
                    break;
                case		Key_Disp:
                    Turnon_Led();
                    break;
                case		Key_SETUP:
                    Turnoff_Led();
                    break;
                case		Key_FAST:
                    LCD_Clear(LCD_COLOR_TEST_BACK);
                    Debug_Cood.xpos=180;
                    Debug_Cood.ypos =120;
                    Debug_Cood.lenth=120;
                    input_password(&Debug_Cood);
                //SetSystemStatus(SYS_STATUS_DEBUG);
                    break;
                case		Key_LEFT:
                    break;
                case		Key_RIGHT:
                    break;
                case		Key_UP:
                    Led_Pass_On();
                    break;
                case		Key_DOWN:
                    Led_Fail_On();
                    break;
                case		Key_NUM7:
                    break;
                case		Key_NUM8:
                    break;
                case		Key_NUM9:
                    break;
                case		Key_NUM4:
                    break;
                case		Key_NUM5:
                    break;
                case		Key_NUM6:
                    break;
                case		Key_NUM1:
                    break;
                case		Key_NUM2:
                    break;
                case		Key_NUM3:
                    break;
                case		Key_NUM0:
                    break;
                case		Key_DOT:
                    break;
                case		Key_BACK:
                    break;
                case		Key_LOCK:
                    break;
                case		Key_BIAS:
                    break;
                case		Key_REST:
                    break;
                case		Key_TRIG:
                    break;
                case		Key_POWER:
                    break;
            }
        }
	}
}

//==========================================================
//函数名称：Idle_Process
//函数功能：待测主程序
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.26 08:59
//备注说明：无
//==========================================================



//==========================================================
//函数名称：Setup_Process
//函数功能：设置程序
//入口参数：无
//出口参数：无
//创建日期：2015.10.28
//修改日期：2015.10.28 10:45
//备注说明：无
//==========================================================
void Setup_Process(void)
{

	u8 keynum=0,i;
	
	Disp_Coordinates_Typedef  Coordinates;
//	Send_Ord_Typedef Uart;
	
	vu8 key=0;
	vu8 Disp_Flag=1;
	keynum=0;
    LCD_Clear(LCD_COLOR_TEST_BACK);
    Disp_Test_Set_Item();
	 
 	while(GetSystemStatus()==SYS_STATUS_SETUP)
	{
	    
		if(Disp_Flag==1)
		{
			
			DispSet_value(keynum);
           
			Disp_Flag=0;
		
		}
		//Touch_Scan();
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{
			if(tp_dev.x[0]<640&&tp_dev.y[0]<480)
			{	

				for(i=0;i<5;i++)
				{
					if((tp_dev.x[0]>(24+BUTTON_W*i))&&(tp_dev.x[0]<(24+BUTTON_W*(i+1)))&&(tp_dev.y[0]>BUTTON_1))
					{
						touch_value=i;
						Disp_Flag=1;
						break;
						
					}
					else
						touch_value=0xff;
				}
				if(touch_value==0xff)//没有检测到触摸屏 接着往下检测
				{
					for(i=0;i<16;i++)
					{
						if(i<16/2)
						{
							if(((tp_dev.x[0]>DISPX1)&&(tp_dev.x[0]<DISPX1+RECT))&&(tp_dev.y[0]<(FIRSTLINE+HIGH1*(i+1)))&&(tp_dev.y[0]>(FIRSTLINE+HIGH1*i)))
							{	
								touch_value=i+5;
								Disp_Flag=1;
								break;
							}
							
						}
						else
						{
							if(((tp_dev.x[0]>DISPX2)&&(tp_dev.x[0]<DISPX2+RECT))&&(tp_dev.y[0]<(FIRSTLINE+HIGH1*(i-8+1)))&&(tp_dev.y[0]>(FIRSTLINE+HIGH1*(i-8))))
							{	
								touch_value=i+5;
								Disp_Flag=1;
								break;
							}
						
						}
					
					
					}
				
				
				
				}
				
					
				
				switch(touch_value)
					{
						case 0:
							switch(keynum)
							{
								case 0:
									SetSystemStatus(SYS_STATUS_TEST);
								break;
								case 1:
									Jk516save .Set_Data.trip=0;
									break;
								case 2:
									Jk516save .Set_Data.speed=0;
									break;
								case 3:
									Jk516save .Set_Data.Res_comp=0;
									break;
								case 4:
									break;
								case 5:
									Jk516save .Set_Data.V_comp=0;
									break;
								case 6:
									break;
								case 7:
									Jk516save.Set_Data.Range_Set=0;
									Jk516save.Set_Data.Range=0;//自动
									break;
								case 8:
									Jk516save .Set_Data.beep=0;
									break;
								case 9:
									break;
								case 10:
									break;
								case 11:
									break;
								case 12:
									break;
								
								default :
									break;
								
							}
							break;
						case 1:
							switch(keynum)
							{
								case 0:
									//SetSystemStatus(SYS_STATUS_TEST);
								break;
								case 1:
									Jk516save .Set_Data.trip=1;
									break;
								case 2:
									//Jk516save .Set_Data.speed=1;
									break;
								case 3:
									Jk516save .Set_Data.Res_comp=1;
									break;
								case 4:
									break;
								case 5:
									Jk516save .Set_Data.V_comp=1;
									break;
								case 6:
									break;
								case 7:
									Jk516save.Set_Data.Range_Set=1;
									break;
								case 8:
									Jk516save .Set_Data.beep=1;
									break;
								case 9:
									break;
								case 10:
									break;
								case 11:
									break;
								case 12:
									break;
								
								default :
									break;
								
							}
							break;
							
						case 2:
							switch(keynum)
							{
								case 0:
									SetSystemStatus(SYS_STATUS_SYSSET);
								break;
								case 1:
									//Jk516save .Set_Data.trip=0;
									break;
								case 2:
									//Jk516save .Set_Data.trip=0;
									break;
								case 3:
									break;
								case 4:
									break;
								case 5:
									break;
								case 6:
									break;
								case 7:
									Jk516save.Set_Data.Range_Set=2;//增加档位选择的计算
									Range=Jisuan_Range(Jk516save.Set_Data.Nominal_Res);
									Jk516save.Set_Data.Range=Range;
									Range_Control(Range);
									break;
								case 8:
									Jk516save .Set_Data.beep=2;
									break;
								case 9:
									break;
								case 10:
									break;
								case 11:
									break;
								case 12:
									break;
								
								default :
									break;
								
							}
							break;
							//SetSystemStatus();
							
						case 3:
							switch(keynum)
							{
								case 0:
									SetSystemStatus(SYS_STATUS_SYS);
								break;
								case 1:
									//Jk516save .Set_Data.trip=0;
									break;
								case 2:
									//Jk516save .Set_Data.trip=0;
									break;
								case 3:
									break;
								case 4:
									break;
								case 5:
									break;
								case 6:
									break;
								case 7:
									Jk516save.Set_Data.Range_Set=1;
									if(Jk516save.Set_Data.Range<RANGE_MAX)
										Jk516save.Set_Data.Range++;
									else
										Jk516save.Set_Data.Range=0;
									Range=Jk516save.Set_Data.Range;
									break;
								case 8:
									break;
								case 9:
									break;
								case 10:
									break;
								case 11:
									break;
								case 12:
									break;
								
								default :
									break;
								
							}
							break;
							//SetSystemStatus();
							
						case 4:
							switch(keynum)
							{
								case 0:
									//SetSystemStatus(SYS_STATUS_TEST);
								break;
								case 1:
									//Jk516save .Set_Data.trip=0;
									break;
								case 2:
									//Jk516save .Set_Data.trip=0;
									break;
								case 3:
									break;
								case 4:
									break;
								case 5:
									break;
								case 6:
									break;
								case 7:
									Jk516save.Set_Data.Range_Set=1;
									if(Jk516save.Set_Data.Range<RANGE_MAX)
										Jk516save.Set_Data.Range++;
									else
										Jk516save.Set_Data.Range=0;
									Range=Jk516save.Set_Data.Range;
									break;
								case 8:
									break;
								case 9:
									break;
								case 10:
									break;
								case 11:
									break;
								case 12:
									break;
								
								default :
									break;
								
							}
							break;
						case 5:
							keynum=1;
							break;
						case 6:
							keynum=2;
							break;
						case 7:
							//keynum=3;
							break;
						case 8:
							keynum=3;
							break;
						case 9://弹出数字框
							keynum=4;
							break;
						case 10:
							//keynum=5;
							break;
						case 11:
							keynum=5;
							break;
						case 12:
							keynum=6;
							break;
						case 13:
							keynum=7;
							break;
						
						case 14:
							keynum=8;
							break;
						case 15:
							break;
						case 16:
							keynum=9;
							break;
						case 17:
							keynum=10;
							break;
						case 18:
							break;
						case 19:
							keynum=11;
							break;
						case 20:
							keynum=12;
							break;
						default:
							break;
					
					
					}
				
			}
		
		}
        key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
        if(key!=KEY_NONE)
		{	Disp_Flag=1;
			switch(key)
			{
				case Key_F1:

					switch(keynum)
					{
						case 0:
							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_TEST);//
//							else
//								SetSystemStatus(SYS_STATUS_FILE);
								
							break;
						case 1:
							Jk516save.Set_Data.trip=0;
							
							break;
						case 2:
							Jk516save.Set_Data.speed=0;//慢速
							
								
							
							break;
						case 3:
							Jk516save.Set_Data.Res_comp=0;
							//SaveData.Main_Func.Level=0;
							break;
						case 4:
                            Coordinates.xpos=DISPX1;
                            Coordinates.ypos=FIRSTLINE+HIGH1*4;
                            Coordinates.lenth=RECT;
                            Jk516save.Set_Data.Res_low=Disp_Set_Num(&Coordinates);
                            
                    
							
							break;
						case 5:
							Jk516save.Set_Data.V_comp=0;
							break;
						case 6:
                            Coordinates.xpos=DISPX1;
                            Coordinates.ypos=FIRSTLINE+HIGH1*7;
                            Coordinates.lenth=RECT;
                            Jk516save.Set_Data.V_low=Disp_Set_CompNum(&Coordinates);
							
							
							break;
						case 7:
                            Jk516save.Set_Data.Range_Set=0;
							Jk516save.Set_Data.Range=0;//自动
							break;
						case 8:
							Jk516save.Set_Data.beep=0;
							break;
						case 9:
                            Coordinates.xpos=DISPX2;
                            Coordinates.ypos=FIRSTLINE+HIGH1*3;
                            Coordinates.lenth=RECT;
                            Jk516save.Set_Data.Nominal_Res=Disp_Set_Num(&Coordinates);

							break;
						case 10:
                            Coordinates.xpos=DISPX2;
                            Coordinates.ypos=FIRSTLINE+HIGH1*4;
                            Coordinates.lenth=RECT;
                            Jk516save.Set_Data.High_Res=Disp_Set_Num(&Coordinates);

							break;
						case 11:
                            Coordinates.xpos=DISPX2;
                            Coordinates.ypos=FIRSTLINE+HIGH1*6;
                            Coordinates.lenth=RECT;
                            Jk516save.Set_Data.Nominal_V=Disp_Set_CompNum(&Coordinates);

							break;
						case 12:
                            Coordinates.xpos=DISPX2;
                            Coordinates.ypos=FIRSTLINE+HIGH1*7;
                            Coordinates.lenth=RECT;
                            Jk516save.Set_Data.V_high=Disp_Set_CompNum(&Coordinates);

							break;
						
							
						default:
							break;
					
					
					}

				break;
				case Key_F2:
					

					switch(keynum)
					{
						case 0:
							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_SETUP);
//							else
//								SetSystemStatus(SYS_STATUS_SYSSET);
								
							break;
						case 1:
							
							Jk516save.Set_Data.trip=1;//中速
							
							
							break;
						case 2:
							Jk516save.Set_Data.speed=1;
							
							
							
							break;
						case 3:
							Jk516save.Set_Data.Res_comp=1;
							break;
						case 4:
//							if(Save_Res.Set_Data.Res_low.Num%1000<900)
//								Save_Res.Set_Data.Res_low.Num+=100;
//							else
//							{
//								//Save_Res.Set_Data.Res_low.Num%=100;
//								Save_Res.Set_Data.Res_low.Num-=900;
//								
//							}
							break;
						case 5:
							Jk516save.Set_Data.V_comp=1;
							break;
						case 6:
//							if(Save_Res.Set_Data.V_low.Num%1000<900)
//								Save_Res.Set_Data.V_low.Num+=100;
//							else
//								Save_Res.Set_Data.V_low.Num-=900;
//							Uart_Send_Flag=2;
							break;
						case 7:
                            Jk516save.Set_Data.Range_Set=1;
							//Jk516save.Set_Data.Range=10;//手动
							break;
						case 8:
							Jk516save.Set_Data.beep=1;
							break;

						default:
							break;
					
					
					}				
				

				break;
				case Key_F3:
					switch(keynum)
					{
						case 0:
								SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						case 1:
                           // Jk516save.Set_Data.trip=2;//快速
							break;
						case 2:
                            Jk516save.Set_Data.speed=2;
							break;
						case 3:
							break;
						case 4:

							break;
						case 5:
							break;
						case 6:

							break;
						case 7:
//                            Jk516save.Set_Data.Range_Set=0;
//                            Range=Jisuan_Range(Jk516save.Set_Data.Nominal_Res);
                            
                            Jk516save.Set_Data.Range_Set=2;//增加档位选择的计算
                            Range=Jisuan_Range(Jk516save.Set_Data.Nominal_Res);
                            Jk516save.Set_Data.Range=Range;
                            Range_Control(Range);
							//Jk516save.Set_Data.Range=11;//当前量程计算量程位置
							break;
						case 8:
							Jk516save.Set_Data.beep=2;
							break;

						
						default:
							break;
					
					
					}	
					
				break;
				case Key_F4:
					switch(keynum)
					{
						case 0:
								SetSystemStatus(SYS_STATUS_SYS);
							break;
						case 1:
                            //Jk516save.Set_Data.trip=3;//极速
							
									
							break;
						case 2:
                            Jk516save.Set_Data.speed=3;
							
							break;
						case 3:
							break;
						case 4:

							break;
						case 5:
							break;
						case 6:
							if(Jk516save.Set_Data.V_low.Num%10<9)
								Jk516save.Set_Data.V_low.Num+=1;
							else
								Jk516save.Set_Data.V_low.Num-=9;
							break;
						case 7:
                            Jk516save.Set_Data.Range_Set=1;
                            if(Jk516save.Set_Data.Range<RANGE_MAX)
                                Jk516save.Set_Data.Range++;
                            else
                                Jk516save.Set_Data.Range=0;
                            Range=Jk516save.Set_Data.Range;
							break;

						
						default:
							break;					
					}	
				
				break;
				case Key_F5:
					switch(keynum)
					{
						case 4:
						case 9:
							break;
						case 7:	
							Jk516save.Set_Data.Range_Set=1;
                            if(Jk516save.Set_Data.Range)
							Jk516save.Set_Data.Range--;
                            else
                                Jk516save.Set_Data.Range=RANGE_MAX;
                                
                            Range=Jk516save.Set_Data.Range;
						break;
						
						default:
							break;
					
					
					}
                    
					
				break;
				case Key_Disp:
                        SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                        //SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				
				case Key_LEFT:
					if(keynum==0)
						keynum=12;
					else
					if(keynum>6)
						keynum-=6;
					else
						keynum+=5;
					
						
				break;
				case Key_RIGHT:
					if(keynum==0)
						keynum=1;
					else
					if(keynum<=6)
						keynum+=6;
					else
						keynum-=5;
					
						
				break;
				case Key_DOWN:
					if(keynum>11)
						keynum=0;
					else
						keynum++;
					
					
				break;
				case Key_UP:
					if(keynum<1)
						keynum=11;
					else
						keynum--;
					
				break;
				case Key_DOT:

					break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				break;
                case Key_FAST:
				
				
																							
					break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	 	
	
	
	
	}
	Store_set_flash();
}

//==========================================================
//函数名称：Test_Process
//函数功能：测试主程序
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.29 14:18
//备注说明：x.xx ms(12MHz/12T)
//==========================================================
void Test_Process(void)
{
    
	vu8 key;
    vu16 USB_Count=0;
    UINT fnum;
    vu8 test_Vsorting,test_Rsorting;
    u32 color;
    u8 keynum=0;
    u8 test_over=0;
    u8 OpenRangflag=0;
    vu8 Trip_Over_usb=0;
	vu8 i;
    u8 ry_flag=0;
    Send_To_UTypedef pt;
    Disp_Coordinates_Typedef  Coordinates;
	u8 Disp_Flag=1;//显示标志
    vu8 range_flag=0;
	ffit_data1=0;
    Ad_over=0;
    Set_flag=0;
    test_start=0;
    V_Range=0;
	F_100ms=FALSE;//100ms定时	
    debug_flag=0;//	blinkT=0;//闪烁时间计时
    LCD_Clear(LCD_COLOR_TEST_BACK);
    pt=Send_To_U;
//    if(Jk516save.Set_Data.Range_Set==1)//固定量程
//    {
//        Range=Jk516save.Set_Data.Range;
//    
//    }
//    else if(Jk516save.Set_Data.Range_Set==0)//自动
//    {
//        Range=6;
//    }
//    else if(Jk516save.Set_Data.Range_Set==2)//计算在哪个档
//    {
//        Range=Jisuan_Range(Jk516save.Set_Data.Nominal_Res);
//    }
//    else
//    {
//        Range=Jk516save.Set_Data.Range;
//    
//    }
    Disp_Test_Item();  
    open_flag=0;    
    V_Range=Jisuan_V_Range(Jk516save.Set_Data.Nominal_V);
    Range_Control(Range);	
    open_flag=1; 
    Select_V_I(1);
    delay_us(120);
    i=0;
    range_over=0;
//    Send_ComBuff.send_head=0xaa;
//    Send_ComBuff.sendend=0xbf;
//    Send_ComBuff.over=0;
    //LCD_DispString_EN_CH( 10,300, "关量显示 OK" );
//	Range_Control(2,0);
//	while(open_flag);
//		Send_To_Cpld(cpld_buff,2);
//V_Range_Control(0);
V_Range=1;
	while(GetSystemStatus()==SYS_STATUS_TEST)
	{
      // USB_Count++;
		if(GPIO_ReadInputDataBit( GPIOE,  GPIO_Pin_3)==0)
		{
			delay_ms(100);
			if(GPIO_ReadInputDataBit( GPIOE,  GPIO_Pin_3)==0)
			{
			GPIO_ResetBits(GPIOH,GPIO_Pin_4);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
			}
			
		}
			
//        Touch_Scan();
	
        Colour.Fword=White;
        if(Disp_RTCflag)
        {
            Disp_RTCflag=0;
            Disp_dateandtime();//1秒更新一次
            Int_Pe3flag=0;
            
        }
         if(Disp_Flag==1 )//显示设置的值
		{
			Disp_Test_value(keynum);
			Colour.Fword=White;
            Colour.black=LCD_COLOR_TEST_BACK;
            Disp_R_V();
		
		}  
         if(Disp_usbflag||Disp_Flag)
        {
			//Colour.black=LCD_COLOR_TEST_BACK;
            Disp_Usbflag(USB_Openflag);
            Disp_Flag=0;
            
        }
        OpenRangflag=read_cpld();//判断是否开路
        if(OpenRangflag)
        {
            ry_flag++;
            if(ry_flag>3)
                ry_flag=3;
            OpenRangflag=0;
//            if(Jk516save.Set_Data.Range_Set!=1)
//            if(ry_flag==1)
//            {
//                Range_Control(Range);
//                
//            }
        
        
        }else
        ry_flag=0;
            
        if(Jk516save.Set_Data.trip==0)
            test_start=1;
        if(open_flag==0)//不开路
        {
            if(test_start==1)
            {
                range_flag=1;
				Jk516save.Set_Data.speed=3;
                switch(Jk516save.Set_Data.speed)
                {
                    case 0:

                    
                    case 1:

                    
                    case 2:

                    
                    case 3:
                        Select_V_I(1);
                        if(Range==0)
							V_I_amp(1);
						else
							V_I_amp(0);
                        //read_adI_one();//
                        delay_ms(15);
						//read_adI_one();//
                        read_adI_one();//
                        //read_adI_4();
						if(I_ad>0x7fffff)
								I_ad=0;
						I_ad/=720;
                        Range_value=I_ad;
						
                        Range_Changecomp();	//换挡 比较
                        while(range_over)
                        {
							read_adI_one();
							if(I_ad>0x7fffff)
								I_ad=0;
							
							I_ad/=720;
							Range_value=I_ad;
							
							//Range_value/=1000;
                            Range_Changecomp();	//换挡 比较
							
                            

                        
                        }
                           read_adI_4();//
                           
						
                        Select_V_I(0);
                        if(V_Range==0)
                            V_I_amp(1);
						else
							V_I_amp(0);
                        //read_adV_one();
                        delay_ms(15);;
                        // read_adV_one();
                        read_adV_one();
						if(V_ad>0x7fffff)
							V_ad=0xffffff-V_ad;
						V_ad/=80;
                        Range_Value_V=V_ad;
                        VRange_Changecomp();	//换挡 比较
                        while(range_over)
                        {
                            //VRange_Changecomp();	//换挡 比较
                            
                            //read_adV_4();//
                           //Range_Value_V=V_ad/100;
							
							read_adV_one();
							if(V_ad>0x7fffff)
							V_ad=0xffffff-V_ad;
							V_ad/=80;
							Range_Value_V=V_ad;
							VRange_Changecomp();	//换挡 比较
                        
                        
                        }
						read_adV_4();
						//Select_V_I(1);
                        break;
                    default:

                        break;
                
                
                
                }
                
                test_over=1;
            
               
            }
           
                      
        }
        else
        {
            V_Range=1;
            VRange_Changecomp();
            
           if(range_flag) //开路的时候的档位控制
           {
               if(Jk516save.Set_Data.Range_Set==0)
               {
                   Range=3;
                    
               }
               else
               {
               
               }
               if(Jk516save.Set_Data.Range_Set!=1)
               Range_Control(Range);
                   i=0;
                   range_flag=0;
                   RangeChange_Flag=1;
               
               
               
           }
            Disp_Open();
        }
            
       
	 	if(test_start&&test_over==1)//显示测试值
		{   

            test_over=0;
            if(Jk516save.Set_Data.trip)
            Trip_Over_usb=1;
            
            Ad_over=0; 

              disp_I=disp_I-Jk516save.Clear[Range];//再判断极性
            
               if(disp_I<0)
               {
                   disp_I=-disp_I;
                   polarity_r=0;
               }
               else
               {
                polarity_r=1;
               
               }
                
               if(disp_I>3800)
               {
                   i=0;
                   open_flag=1;
           
                    Disp_Open();
         
                   
               }
                
                disp_V=disp_V-Jk516save.Clear_V[V_Range];
                if(disp_V<0)
                {
                    polarity_v=0;
                    disp_V=-disp_V;
                
                }else
                {
                    polarity_v=1;
                
                }

                Res_count.r=I_ad;
                Test_Debug();//校正
                I_ad=Res_count.r;
                if(open_flag==0)
                {
                    
                    if(RangeChange_Flag)
                    {
                        Disp_Range(Jk516save.Set_Data.Range_Set,Range);
                        Send_ComBuff.Range=Range;
                        RangeChange_Flag=0;
                        i=0;
                    }

                    if(range_over==0)
                    {
                        test_start=0;
                        Test_Value_V=V_Datacov(disp_V ,V_Range);//把数据的小数点和单位 和极性都加上
                        Test_Value=Datacov(disp_I,Range);
                        
                        Disp_Testvalue(Test_Value,Test_Value_V,0);//显示电阻和电压
                        if(Jk516save.Set_Data.V_comp)
                        {
                            test_Vsorting= V_Comp();
                            if(polarity_v==0)
                                test_Vsorting=2;
                            
                        }
                        if(Jk516save.Set_Data.Res_comp)
                            test_Rsorting=R_Comp();
                        color=Colour.black;
                        //下面是分选
                       
                        if(Jk516save.Set_Data.Res_comp==1&&Jk516save.Set_Data.V_comp==1)
                        {
                             
                            Colour.black=LCD_COLOR_RED;
                            if(test_Vsorting==0&&test_Rsorting==0)
                            {
                                Colour.black=LCD_COLOR_GREEN;
                                memcpy(DispBuf,"RV GD",5);  
                                
                                Send_ComBuff.comp=0;
                                Beep_Out(0);
                                Led_Pass_On();
                                
                            
                            }
                            else 
                            {
                                Beep_Out(1);
                                Led_Fail_On();
                                if(test_Vsorting==0)
                                {
                                    memcpy(DispBuf,"R  FL",5);
                                    Send_ComBuff.comp=1;
                                }
                                else if(test_Rsorting==0)
                                {
                                    memcpy(DispBuf,"V  FL",5);
                                    Send_ComBuff.comp=2;
                                    
                                }
                                else
                                {
                                    memcpy(DispBuf,"RV FL",5);
                                    Send_ComBuff.comp=3;

                                }                                    
                            }
                            memcpy((void *)Send_To_U.comp,DispBuf,5);
                            DispBuf[5]=0;
//                            LCD_DrawFullRect(SORTING_XDISP, SORTING_Y_DISP, 60, 22);
//                            WriteString_16(SORTING_XDISP, SORTING_Y_DISP, DispBuf,  0);
                         Colour.black=color;
                        }
                        else
                            if(Jk516save.Set_Data.Res_comp==1)
                            {
                                if(test_Rsorting)//不合格
                                {
                                    Colour.black=LCD_COLOR_RED;
                                    memcpy(DispBuf,"R  FL",5);
                                    Send_ComBuff.comp=1;                                    
                                    Beep_Out(1);
                                    Led_Fail_On();
                                }
                                else//合格
                                {
                                    Colour.black=LCD_COLOR_GREEN;
                                    memcpy(DispBuf,"R  GD",5);
                                    Send_ComBuff.comp=0;                                    
                                    Beep_Out(0);
                                    Led_Pass_On();
                                
                                }
                            
                             memcpy((void *)Send_To_U.comp,DispBuf,5);
                             DispBuf[5]=0;

                            
                            }else if(Jk516save.Set_Data.V_comp==1)
                            {
                                if(test_Vsorting)//不合格
                                {
                                    Colour.black=LCD_COLOR_RED;
                                    memcpy(DispBuf,"V  FL",5);
                                    Send_ComBuff.comp=3;
                                    Beep_Out(1);
                                    Led_Fail_On();
                                }
                                else//合格
                                {
                                    Beep_Out(0);
                                    Led_Pass_On();
                                    Colour.black=LCD_COLOR_GREEN;
                                    memcpy(DispBuf,"V  GD",5);
                                    Send_ComBuff.comp=0;
                                
                                }
                             memcpy((void *)Send_To_U.comp,DispBuf,5);
                             DispBuf[5]=0; 

                            }
                            else
                            {
//                                Close_Compled();
//                                Colour.black=LCD_COLOR_TEST_MID;
//                                LCD_DrawFullRect(SORTING_XDISP, SORTING_Y_DISP, 60, 22);
                            
                            }
                      
                        
                        
                    }
                    
                }
            
		}
        if(Jk516save.Sys_Setvalue.u_flag)
        {
            
            USBH_Process(&USB_OTG_Core, &USB_Host);
            if(Jk516save.Set_Data.trip)
                USB_Count=0;
                
            if(Trip_Over_usb)
            {
                Trip_Over_usb=0;
                USB_Count=200;
            
            }
            if(USB_Count>=200)
            {

                Trip_Over_usb=0;
                if(Disp_usbflag)
                {
                    
                    Disp_usbflag=0;
                    
                }
                USB_Count=0;
                                
                result = f_mount(&fs,"0:",1);
                if(result == FR_OK)
                {
                    vu8 copybuff[100];
                    memset((void *)copybuff,0,100);
                    
                    memcpy ((void *)copybuff,"0:/JK/",6);
                    strcat((char *)copybuff,(char *)Jk516save.Sys_Setvalue.textname);
                    strcat((char *)copybuff,(char *)".xls");
                    result=f_mkdir("0://JK");
                    result = f_open(&file, (char *)copybuff,FA_CREATE_NEW | FA_WRITE );
                         if ( result == FR_OK ) 
                         {
                              f_lseek (&file,file.fsize);
                             result=f_write(&file,"序号\t电阻\t电压\t分选\r\n",sizeof("序号\t电阻\t电压\t分选\r\n"),&fnum);
                             f_close(&file);
                         
                         
                         }else
                         if(result==FR_EXIST)
                         {
                             result = f_open(&file, (char *)copybuff, FA_WRITE );
                             f_lseek (&file,file.fsize);
                             pt=Send_To_U;
                            result=f_write(&file,(u8 *)&pt,sizeof(Send_To_U),&fnum);
                             f_close(&file);
                         
                         }
                    }
            }
        }
             
		Uart_Process();//串口处理
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{
			if(tp_dev.x[0]<640&&tp_dev.y[0]<480)
			{	

				for(i=0;i<5;i++)
				{
					if((tp_dev.x[0]>(24+BUTTON_W*i))&&(tp_dev.x[0]<(24+BUTTON_W*(i+1)))&&(tp_dev.y[0]>BUTTON_1))
						touch_value=i;
					else
						touch_value=0xff;
				
					switch(touch_value)
					{
						case 0:
							break;
						case 1:
							SetSystemStatus(SYS_STATUS_SETUP);
							break;
						case 2:
							SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						case 3:
							SetSystemStatus(SYS_STATUS_SYS);
							break;
						case 4:
                            SetSystemStatus(SYS_STATUS_CLEAR);
							break;
						default:
							break;
					
					
					}
				}
				
			}
		
		}
        if(Keyboard.state==TRUE)
        {
            Disp_Flag=1;
			
			
            key=Key_Read();
			
            if(key!=KEY_NONE)
            switch(key)
            {
                    case Key_F1:
                        
                        switch(keynum)
                        {
                            case 0:
                                keynum=0;
                                   
                                break;
                            case 1:
                                Jk516save.Set_Data.trip=0;
                            
                            
    
                                
                                break;
                            case 2:
                                
                                Coordinates.xpos=DISPX1;
                                Coordinates.ypos=FIRSTLINE+HIGH1;
                                Coordinates.lenth=RECT;
                                Jk516save.Set_Data.High_Res=Disp_Set_Num(&Coordinates);
                            break;
                            case 3:
                                Coordinates.xpos=DISPX1;
                                Coordinates.ypos=FIRSTLINE+HIGH1*2;
                                Coordinates.lenth=RECT;
                                Jk516save.Set_Data.Res_low=Disp_Set_Num(&Coordinates);
                                
                            break;
    						case 4:
                                Jk516save.Set_Data.Range=RANGE_MAX;//0  自动  10   最佳量程
                                Jk516save.Set_Data.Range_Set=0;
   
                                break;
                            case 5:
                                Coordinates.xpos=DISPX2;
                                Coordinates.ypos=FIRSTLINE+HIGH1*1;
                                Coordinates.lenth=RECT;
                                Jk516save.Set_Data.V_high=Disp_Set_CompNum(&Coordinates);
                                
                            break;
                            
                            
                            case 6:
                                Coordinates.xpos=DISPX2;
                                Coordinates.ypos=FIRSTLINE+HIGH1*2;
                                Coordinates.lenth=RECT;
                                Jk516save.Set_Data.V_low=Disp_Set_CompNum(&Coordinates);
                                
                            break;
						
                            default:
                                break;
                        
                        
                        }

                    break;
                    case Key_F2:
                        switch(keynum)
                        {
                            case 0:
                                
    								SetSystemStatus(SYS_STATUS_SETUP);
                                break;
                            case 1:
    							Jk516save.Set_Data.trip=1;

                                
                                break;
  					
    						case 4:
    							
                                Jk516save.Set_Data.Range_Set=1;
                                Jk516save.Set_Data.Range=Range;
                          

    							break;
 
                            
                            default:
                                break;
                        
                        
                        }
 
                    break;
                    case Key_F3:
                        switch(keynum)
                        {
                            case 0:
    								SetSystemStatus(SYS_STATUS_SYSSET);
                                break;
					
    						case 4://MAX_R_RANGE
                                Jk516save.Set_Data.Range_Set=2;//增加档位选择的计算
                                Range=Jisuan_Range(Jk516save.Set_Data.Nominal_Res);
                                Jk516save.Set_Data.Range=Range;
                                Range_Control(Range);
                                

    							break;

                            
                            default:
                                break;
                        
                        
                        }
                    break;
                    case Key_F4:
                        switch(keynum)//
                        {
                           case 0:
    								SetSystemStatus(SYS_STATUS_SYS);
                                break;
                            case 4:
                                if(Range>=RANGE_MAX)
                                    Range=0;
                                else
                                    Range++;
                                Jk516save.Set_Data.Range_Set=1;
                                Jk516save.Set_Data.Range=Range;
                                Range_Control(Range);
                                
                            break;
  
                            default:
                                break;
                        
                        
                        }	
                    break;
                    case Key_F5:
                        switch(keynum)
                        {
                            case 0:
                                 SetSystemStatus(SYS_STATUS_CLEAR);
                                break;
							case 4:
                                if(Range<=0)
                                    Range=RANGE_MAX;
                                else
                                    Range--;
                                Jk516save.Set_Data.Range_Set=1;
                                Jk516save.Set_Data.Range=Range;
                                Range_Control(Range);
                                
                            break;
                            default:
                                break;
                        }
  
                    break;
                    case Key_Disp:
                    break;
                    case Key_SETUP://ENTER
                        keynum=0;
                        //SetSystemStatus(SYS_STATUS_SETUP);
                    break;
                    case Key_FAST:
						TP_Adjust();
                    break;
                    case Key_LEFT:
                        if(keynum<1)
                            keynum=6;
                        else
                            keynum--;
                      
                    break;
                    case Key_RIGHT:
                        if(keynum>5)
                            keynum=0;
                        else
                            keynum++;
                       
                 
                            
                    break;
                    case  Key_UP:
                        if(keynum<1)
                            keynum=6;
                        else
                            keynum--;
                        
                    break;
                    case Key_DOWN:
                        if(keynum>5)
                            keynum=0;
                        else
                            keynum++;
                    break;
                    
                    case Key_NUM1:
                    //break;
                    case Key_NUM2:
                    //break;
                    case Key_NUM3:
                    //break;
                    case Key_NUM4:
                    //break;
                    case Key_NUM5:
                    //break;
                    case Key_NUM6:
                    //break;
                    case Key_NUM7:
                    //break;
                    case Key_NUM8:
                    //break;
                    case Key_NUM9:
                    //break;
                    case Key_NUM0:
                    break;
                    case Key_DOT:

                    break;
                    case Key_BACK:
                        
                    break;
                    case Key_LOCK:


                    break;
                    case Key_BIAS:
                        
                    break;
                    case Key_REST:
                    break;
                    case Key_TRIG:
                        test_start=1;
                        
                    break;
                    default:
                        SetSystemStatus(SYS_STATUS_TEST);
                    break;
                        
                }
         }
     }
    Store_set_flash();
   // f_mount(NULL, "0:", 0);
}
//==========================================================
//函数名称：TestAbort_Process
//函数功能：测试中止处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26 
//修改日期：2015.10.29 13:35
//备注说明：无
//==========================================================
//void TestAbort_Process(void)
//{
//	u8 key;

//	//输出关闭
////	Plc_Start_Off();//关远控启动

// 	//讯响控制
////	if(SaveData.Parameter.Beep)
////		Beep_One();//响一声

//	//显示信息
////	LcdAddr.x=MSG_ADDR_X; LcdAddr.y=0;
////	Lcd_SetAddr();//设置坐标
////	Disp_SysMessage();//显示信息
//	
//	//DAC_SetChannel1Data(DAC_Align_12b_R,0);
////	Led_HV_Off();//关测试灯
////	delay_ms(30);
////	Electro_Test(0);
////	delay_ms(30);
////	
////	Electro_discharge(0);
////	delay_ms(30);
////	BeepOff();
////	
//	while(GetSystemStatus()==SYS_STATUS_TEST_ABORT)
//	{
////		Uart_Process();//串口处理
//		
//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/20);//等待按键(100*10ms/20=50ms)
//		switch(key)
//		{
//			case KEY_SET:	//设置键
//			case L_KEY_SET:	//长按设置键
//				break;
//	
//			case KEY_UP:	//上键
//			case L_KEY_UP:	//长按上键
//				break;
//	
//			case KEY_DOWN:		//下键
//			case L_KEY_DOWN:	//长按下键
//				break;

//			case KEY_START:		//启动键
//			case L_KEY_START:	//长按启动键
//				SetSystemStatus(SYS_STATUS_TEST);//测试开始
//				break;
//	
//			case KEY_ENTER:		//确认键
//			case L_KEY_ENTER:	//长按确认键
//				break;
//	
////			case KEY_RESET:		//复位键
////			case L_KEY_RESET:	//长按复位键
////				SetSystemStatus(SYS_STATUS_IDLE);//系统状态-待机
////				break;
//			
//			default:
//				break;
//		}
//	}
//	Test_value.Test_Time=0;
//}
//==========================================================
//函数名称：TestFinish_Process
//函数功能：测试结束处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.29 13:38
//备注说明：无
//注意事项：无
//==========================================================
//void TestFinish_Process(void)
//{
//	u8 key;

//	//输出关闭
////	Led_HV_Off();//关闭高压灯
////	Plc_Start_Off();//关远控启动
//	//显示信息
//	//SetSystemMessage(MSG_PASS);
//	
////	LcdAddr.x=MSG_ADDR_X; LcdAddr.y=0;
////	Lcd_SetAddr();//设置坐标
////	Disp_SysMessage();//显示信息
//	//分选讯响控制

//	
//	Electro_discharge(0);
//	while(GetSystemStatus()==SYS_STATUS_TEST_FINISH)
//	{
////		Uart_Process();//串口处理
//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/50);//等待按键(100*10ms/50=20ms)
//		switch(key)
//		{
//			case KEY_SET:	//设置键
//			case L_KEY_SET:	//长按设置键
//				SetSystemStatus(SYS_STATUS_SETUP);//设置状态
//				break;
//	
//			case KEY_UP:	//上键
//			case L_KEY_UP:	//长按上键
//				break;
//	
//			case KEY_DOWN:		//下键
//			case L_KEY_DOWN:	//长按下键
//				break;
//	
//			case KEY_START:		//启动键
//			case L_KEY_START:	//长按启动键
//				SetSystemStatus(SYS_STATUS_TEST);//测试状态
//				break;
//	
//			case KEY_ENTER:		//确认键
////			case L_KEY_ENTER:	//长按确认键
////				SetSystemStatus(SYS_STATUS_IDLE);//待测状态
////				break;
////	
////			case KEY_RESET:		//复位键
////			case L_KEY_RESET:	//长按复位键
////				SetSystemStatus(SYS_STATUS_IDLE);//待机状态
////				break;
//			
//			default:
//				break;
//		}
//	}
//}	
void Electro_discharge(u8 data)//data=1  测试  0  放电
{
	u8 value;
	value=U16_4094;
	if(!data)
		value|=HW_GYBFD_ON;
	else
		value&=HW_GYBFD_OFF;
	U16_4094=value;
	_4094_data( );
	Out_4094(_4094_databuff);
	delay_ms(30);

}
void Electro_Test(u8 data)
{
	u8 value;
	value=U15_4094;
	if(data)
		value|=HW_FD_TEST;
	else
		value&=HW_FD_NOTEST;
	U15_4094=value;
	_4094_data( );
	Out_4094(_4094_databuff);
	delay_ms(30);

}

void Feedback_RY(u8 dat)//反馈电阻
{
	
	//U16_4094
	u8 value;
	value=U15_4094;
	//value&=HW_GYB_1000VFEEDMASK;
	switch(dat)
	{
		case 0:
			value|=HW_GYB_1000VFEED;
			break;
		case 1:
			value&=HW_GYB_100VFEED;
			break;
		
		default:
			//value&=HW_GYB_1000VFEED;
			break;
	}
	U15_4094=value;
	_4094_data( );
	Out_4094(_4094_databuff);

}
//电压采样电阻切换
//输入 ： 0 1 2 3
// 0  1000V  1  100V  2  10V
//
//
//
void V_Samping(u8 dat)
{
	vu8 value;
	value=U16_4094;
	switch(dat)
	{
		case 0:
			value&=HW_GYB_100V;
			break;
		case 1:
			value|=HW_GYB_1000V;
			break;
		
			
		default:
			//value&=HW_GYB_1000V;
			break;
	}
	U16_4094=value;
	_4094_data( );
	Out_4094(_4094_databuff);
	delay_ms(20);

}

//用户校正
void Use_DebugProcess(void)
{
	
	vu32 keynum=0;
    float a,b;
	vu8 key;
//    u8 Disp_Testflag=0;
    vu8 range_flag=0;
//    float i_mid;
//	float ddd,eee;
	Disp_Coordinates_Typedef Coordinates;
    Disp_Coordinates_Typedef Debug_Cood;
    vu8 i;
	vu8 Disp_flag=1;
    vu8 list=0;
 //   uint32_t  scan_I[200],scan_V[200];
    test_start=0;
    LCD_Clear(LCD_COLOR_TEST_BACK);
	Disp_UserCheck_Item();
    Debug_stanedcomp();//校正值比较
    EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE); 
    NVIC_EnableIRQ(EXTI3_IRQn);
    Range_Control(0);
 	while(GetSystemStatus()==SYS_STATUS_DEBUG)
	{

         
//        read_adI_1();//
//        Range_value=I_ad;

//        Select_V_I(0);
//        
//        read_adV_1();
//        Range_Value_V=V_ad;
        
        Select_V_I(1);
        delay_ms(15);
        read_adI_4();//
        Range_value=disp_I;
        Select_V_I(0);
         delay_ms(15);;
		read_adV_4();
        Range_Value_V=disp_V;


		if(test_start)
		{
            {
               // test_start=0;
                Ad_over=0; 
                //先判断极性
                //根据极性 来确定换挡值
                //再计算电阻
                
                if(disp_I<0)
                {
                    polarity_r=0;//﹣
                    disp_I=-disp_I;
                }
                else
                {
                    polarity_r=1;
                    
                                        
                }
                
                disp_I=disp_I-Jk516save.Clear[list-1];//再判断极性
                //I_ad=(I_ad)/169;

                
                if(disp_V<0)
                {
                    disp_V=-disp_V;
                    
                }
                if(list>5)
                disp_V=V_ad-Jk516save.Clear_V[list-6];
                //V_ad=V_ad/45;
                {
                    Test_Value_V=V_Datacov(disp_V ,V_Range);//把数据的小数点和单位 和极性都加上
                    Test_Value=Datacov(disp_I,list-1);
                    Disp_Debug_Reference(list,Test_Value_V,Test_Value);
                    
                }

               
            }

            
		}else
		if(Disp_flag==1)//不启动的时候
		{
			Disp_Debug_value(list);
			Disp_flag=0;	
		}
    
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
		
		if(key!=KEY_NONE)
		{
			Disp_flag=1;
			//Key_Beep();
			switch(key)
			{
				case Key_F1:
                    test_start=1;

				break;
				case Key_F2:

				break;
				case Key_F3:

				break;
				case Key_F4:
                    if(list==0)
                    {
                        Store_set_flash();
                        SetSystemStatus(SYS_STATUS_TEST);
                    }

				break;
				case Key_F5:

				break;
				case Key_Disp:
                    if(list)
                        test_start=1;
                    
					//Savetoeeprom();
                    //SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                        test_start=0;
                    if(list==0)
                    {
                        LCD_Clear(LCD_COLOR_TEST_BACK);
                        
                        Debug_Cood.xpos=70;
                        Debug_Cood.ypos =272-70;
                        Debug_Cood.lenth=120;
                        input_num(&Debug_Cood);
                         LCD_Clear(LCD_COLOR_TEST_BACK);
                        Disp_UserCheck_Item();
                    
                    }

				break;
				
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
                    if(test_start==0)
                        {
                    open_flag=0;
                    
                    if(test_start==0)
                    {
                        if(list<1)
                            list=DEBUG_RANGE;
                        else
                            list--;
                        if(list)
                        {
                            if(list<5)
                            {
                                Range_Control(list-1);
                                Range=list-1;
                                
                            }
                            else
                            {
                                V_Range=DEBUG_RANGE-list-1;
                                Range_Control(RANGE_MAX);
                                
                            }
                        }
                    
                    }
                }

							
				break;
				case Key_DOWN:
                    if(test_start==0)
                    {
                        open_flag=0;
                        if(list<DEBUG_RANGE)
                            list++;
                        else
                            list=0;
                        if(list)
                        {
                            if(list<5)
                            {
                                Range_Control(list-1);
                                Range=list-1;
                                
                            }
                            else
                                
                                {
                                    V_Range=DEBUG_RANGE-list-1;
                                    Range_Control(RANGE_MAX);
                                }
                        }
                    }
					
				break;
					case Key_DOT:
						break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				break;
				case Key_FAST:
                    
                
                                
                                
                                //Jk516save.Set_Data.High_Res=Disp_Set_Num(&Coordinates);
                
				Coordinates.xpos=LIST1+160;
				Coordinates.ypos=FIRSTLINE+(SPACE1-2)*(list);
				Coordinates.lenth=70;
                if(test_start)
                {
                    if(list<8)
                    {
                        Jk516save.Debug_Value[list-1].standard=Debug_Set_Res(&Coordinates);//电阻
                        Jk516save.Debug_Value[list-1].ad_value=(float)Test_Value.res/Jk516save.Debug_Value[list-1].standard;
                    }
                    else
                    {
                      Jk516save.Debug_Value[list-1].standard=Debug_Set_Num(&Coordinates);//电压
                        Jk516save.Debug_Value[list-1].ad_value=(float)Test_Value_V.res/Jk516save.Debug_Value[list-1].standard;
                        
                    }
                    
                }
                test_start=0;

				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
                    
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}
void Clear_Process(void)
{
//    u32 i=0;
    u8 range_v,range;;
    u8 list=0;
    u8 Clear_num=0;
   // float v_ad,i_ad;
    //u32 scan_I[50],scan_V[50];
    range=Jk516save.Set_Data.Range;
    Range_Control(0);
    Disp_Range(0,list);
    while(GetSystemStatus()==SYS_STATUS_CLEAR)
    {
         Select_V_I(1);
        delay_ms(30);
         read_adI_4();//
                           
						
         Select_V_I(0);
                        
        if(list==0)
            V_Range_Control(0);
        else
            V_Range_Control(1);
        delay_ms(30);
        read_adV_4();

            Jk516save.Clear[list]=disp_I;
            if(list<2)
            {
                Jk516save.Clear_V[list]=disp_V;
                range_v=list;
                
            }
            Range_Control(list);
            Disp_Range(0,list);
        
            if(list>=RANGE_MAX)
            {
                SetSystemStatus(SYS_STATUS_TEST);
                //SaveData.Parameter.Range=range;
                Jk516save.Set_Data.Range=range;
                Store_set_flash();
                
            }
            list++;
        
    
    }

}
//系统设置
void Use_SysSetProcess(void)
{	
	//vu32 keynum=0;
	vu8 key,i;
//    uint8_t Disp_buff[12];
    char key_count=0;
	vu8 Disp_flag=1;
//    u8 keynum1=0;
    u8 keynum=0;
    RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
    LCD_Clear(LCD_COLOR_TEST_BACK);
	//Button_Page_Typedef Button_Page;
	//keynum=0;
	//Button_Page.page=0;
    //lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_Sys_Item();
	//Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_SYSSET)
	{
	  	
		if(Disp_flag==1)
		{
           
            
            Jk516save.Sys_Setvalue.textname[7]=0;
			Disp_Sys_value(keynum);
			Disp_flag=0;
            
		}
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{
			if(tp_dev.x[0]<640&&tp_dev.y[0]<480)
			{	

				for(i=0;i<5;i++)
				{
					if((tp_dev.x[0]>(24+BUTTON_W*i))&&(tp_dev.x[0]<(24+BUTTON_W*(i+1)))&&(tp_dev.y[0]>BUTTON_1))
						touch_value=i;
					else
						touch_value=0xff;
				
					switch(touch_value)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_TEST);
							break;
						case 1:
							SetSystemStatus(SYS_STATUS_SETUP);
							break;
						case 2:
							//SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						case 3:
							SetSystemStatus(SYS_STATUS_SYS);
							break;
						case 4:
							break;
						default:
							break;
					
					
					}
				}
				
			}
		
		}
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
        RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
		if(key!=KEY_NONE)
		{
			Disp_flag=1;
           //Key_Beep();
			switch(key)
			{
				case Key_F1:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_TEST);
							break;
						case 1:
							Jk516save.Sys_Setvalue.uart=0;
							break;
						case 2:
							Jk516save.Sys_Setvalue.buard=0;
							break;
						case 3:
                            Jk516save.Sys_Setvalue.u_flag=0;
							
							break;
						case 4:
							Jk516save.Sys_Setvalue.plc=0;
							break;
						case 5:
							Jk516save.Sys_Setvalue.lanage=0;
                            LCD_Clear(LCD_COLOR_TEST_BACK);
                            Disp_Sys_Item();
							break;
						case 6:
							if(RTC_DateStructure.RTC_Year<1)
								RTC_DateStructure.RTC_Year=RTC_YEAR_MAX;
							else
                                if(RTC_DateStructure.RTC_Year>=RTC_YEAR_MAX)
                                    RTC_DateStructure.RTC_Year=0;
                                else
								RTC_DateStructure.RTC_Year--;
                           // RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, RTC_TIME_DISP.YEAR);
                            
                            //RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            //RTC_CalibCounterCmd(LPC_RTC, ENABLE);
                                RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                                RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 7:
							if(RTC_DateStructure.RTC_Month<=1)
								RTC_DateStructure.RTC_Month=RTC_MONTH_MAX;
							else
								RTC_DateStructure.RTC_Month--;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 8:
							if(RTC_DateStructure.RTC_Date<=1)
								RTC_DateStructure.RTC_Date=RTC_DAYOFMONTH_MAX;
							else
								RTC_DateStructure.RTC_Date--;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
                           
							break;
						case 9:
							if(RTC_TimeStructure.RTC_Hours<1)
								RTC_TimeStructure.RTC_Hours=RTC_HOUR_MAX;
							else
								RTC_TimeStructure.RTC_Hours--;
//                            
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 10:
							if(RTC_TimeStructure.RTC_Minutes<1)
								RTC_TimeStructure.RTC_Minutes=RTC_MINUTE_MAX;
							else
								RTC_TimeStructure.RTC_Minutes--;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 11:
							if(RTC_TimeStructure.RTC_Seconds<1)
								RTC_TimeStructure.RTC_Seconds=RTC_SECOND_MAX;
							else
								RTC_TimeStructure.RTC_Seconds--;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 12://
							//SaveData.Sys_Setup.Bias=0;
							break;
						case 18:
							//SaveData.Sys_Setup.Bus_Mode=0;
							break;
						case 13:
//							if(SaveData.Sys_Setup.GP_Addr<1)
//								SaveData.Sys_Setup.GP_Addr=99;
//							else
//								SaveData.Sys_Setup.GP_Addr--;
							break;
//						case 14:
//							SaveData.Sys_Setup.Talk_Only=1;
//							break;
//						case 15:
//							if(SaveData.Sys_Setup.Timer_Value.Hour<1)
//								SaveData.Sys_Setup.Timer_Value.Hour=12;
//							else
//								SaveData.Sys_Setup.Timer_Value.Hour--;
//							break;
//						case 16:
//							if(SaveData.Sys_Setup.Timer_Value.Min<1)
//								SaveData.Sys_Setup.Timer_Value.Min=59;
//							else
//								SaveData.Sys_Setup.Timer_Value.Min--;
//							break;
//						case 17:
//							if(SaveData.Sys_Setup.Timer_Value.Sec<1)
//								SaveData.Sys_Setup.Timer_Value.Sec=59;
//							else
//								SaveData.Sys_Setup.Timer_Value.Sec--;
//							break;
						default:
							break;
					
					}
                    
				break;
				case Key_F2:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SETUP);
							break;
						case 1:
							Jk516save.Sys_Setvalue.uart=1;
							break;
						case 2:
							Jk516save.Sys_Setvalue.buard=1;
							break;
						case 3:
							Jk516save.Sys_Setvalue.u_flag=1;
							break;
						case 4:
							Jk516save.Sys_Setvalue.plc=1;
							break;
						case 5:
							Jk516save.Sys_Setvalue.lanage=1;
                            LCD_Clear(LCD_COLOR_TEST_BACK);
                            Disp_Sys_Item();
							break;
						case 6:
							if(RTC_DateStructure.RTC_Year>=RTC_YEAR_MAX)
								RTC_DateStructure.RTC_Year=0;
							else
								RTC_DateStructure.RTC_Year++;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 7:
							if(RTC_DateStructure.RTC_Month>=RTC_MONTH_MAX)
								RTC_DateStructure.RTC_Month=1;
							else
								RTC_DateStructure.RTC_Month++;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 8:
							if(RTC_DateStructure.RTC_Date>=RTC_DAYOFMONTH_MAX)
								RTC_DateStructure.RTC_Date=1;
							else
								RTC_DateStructure.RTC_Date++;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 9:
							if(RTC_TimeStructure.RTC_Hours>=RTC_HOUR_MAX)
								RTC_TimeStructure.RTC_Hours=0;
							else
								RTC_TimeStructure.RTC_Hours++;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 10:
							if(RTC_TimeStructure.RTC_Minutes>=RTC_MINUTE_MAX)
								RTC_TimeStructure.RTC_Minutes=0;
							else
								RTC_TimeStructure.RTC_Minutes++;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 11:
							if(RTC_TimeStructure.RTC_Seconds>=RTC_SECOND_MAX)
								RTC_TimeStructure.RTC_Seconds=0;
							else
								RTC_TimeStructure.RTC_Seconds++;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;

						default:
							break;
					
					}
                    
				break;
				case Key_F3:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						case 2:
							Jk516save.Sys_Setvalue.buard=2;
							
						break;
						default:
						break;
					
					}
						
				break;
				case Key_F4:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYS);
							break;
						case 2:
							Jk516save.Sys_Setvalue.buard=3;
							
						break;
						default:
						break;
					
					}
				break;
				case Key_F5:
					switch(keynum)
					{
						case 0:
							break;//恢复系统复位
						case 2:
							Jk516save.Sys_Setvalue.buard=4;
							
						break;
						default:
						break;
					
					}
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUP);
				break;
				case Key_FAST:
                    if(keynum==12)
                    {
                        //dispflag=0;
//                        for(i=0;i<8;i++)
//                        {
//                            Save_Res.Sys_Setvalue.textname[i]=Disp_buff[i];
//                        
//                        }
                        key_count=0;
                        keynum=0;
                        //Savetoeeprom();
                    }
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_DOWN:
					if(keynum>SYS_MAX-1)
						keynum=0;
					else
						keynum++;
						
				break;
				case Key_UP:
					if(keynum<1)
						keynum=SYS_MAX;
					else
						keynum--;
				break;
				case Key_NUM1:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='1';
                            key_count++;
                                
                        }
                    
                    }
                        //Save_Res.Sys_Setvalue
				break;
				case Key_NUM2:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='2';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM3:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='3';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM4:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='4';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM5:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='5';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM6:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='6';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM7:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='7';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM8:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='8';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM9:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='9';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM0:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='0';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
    Store_set_flash();
}

void Sys_Process(void)
{
	vu32 keynum=0;
	vu8 key,i;
	vu8 Disp_flag=1;
    LCD_Clear(LCD_COLOR_TEST_BACK);

	while(GetSystemStatus()==SYS_STATUS_SYS)
	{
		if(Disp_flag==1)
		{
			Disp_Sys();
			Disp_flag=0;	
		}
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{
			if(tp_dev.x[0]<640&&tp_dev.y[0]<480)
			{	

				for(i=0;i<5;i++)
				{
					if((tp_dev.x[0]>(24+BUTTON_W*i))&&(tp_dev.x[0]<(24+BUTTON_W*(i+1)))&&(tp_dev.y[0]>BUTTON_1))
						touch_value=i;
					else
						touch_value=0xff;
				
					switch(touch_value)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_TEST);
							break;
						case 1:
							SetSystemStatus(SYS_STATUS_SETUP);
							break;
						case 2:
							SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						case 3:
							
							break;
						case 4:
							break;
						default:
							break;
					
					
					}
				}
				
			}
		
		}
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
		if(key!=KEY_NONE)
		{
			Disp_flag=1;
//            Key_Beep();
			switch(key)
			{
				case Key_F1:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_TEST);
							break;
						
						default:
							break;
					
					}
                    
				break;
				case Key_F2:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SETUP);
							break;
						
					
					}
                    
				break;
				case Key_F3:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						
						default:
						break;
					
					}
						
				break;
				case Key_F4:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYS);
							break;
						
							
						
						default:
						break;
					
					}
				break;
				
				
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUP);
				break;
				
				
				default:
				break;
					
			}
		
		
		}
	
	
	
	
	}

}
//输出电阻切换
//输入 ： 0 1 2 3
// 0  0R  1  10K  2  100K
//
//
//
//void Internal_ResChange(u8 dat)
//{
//	u8 value;
//	value=U16_4094;
//	value&=HW_GYB_10KR;
//	switch(dat)
//	{
//		case 0:
//			value|=HW_GYB_0R;
//			break;
//		case 1:
//			
//			break;
//		case 2:
//			value|=HW_GYB_100KR;
//			break;
//			
//		default:
//			value&=HW_GYB_10KR;
//			break;
//	}
//	U16_4094=value;
//	_4094_data( );
//	Out_4094(_4094_databuff);
//	delay_ms(20);;

//}
//void Disp_Overtest(void)
//{


//}

