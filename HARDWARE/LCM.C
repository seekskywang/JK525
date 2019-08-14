#include "pbdata.h"
#include "hzk.h"
//==========================================================
//ȫ�ֱ���
LcdAddr_TypeDef LcdAddr;//LCD����ṹ��
//д�Ĵ�������
//regval:�Ĵ���ֵ

//==========================================================
//�������ƣ�Lcd_ReadBF
//�������ܣ���ȡҺ����״̬λֱ��BF��־��Ч
//��ڲ�������
//���ڲ�������
//�������ڣ�2014.12.16
//�޸����ڣ�2014.12.16 11:18
//��ע˵��������200�κ��Զ��˳�
//			D1:��д����λ��Ϊ1�ſɲ���
//			D0:BFλ��Ϊ1�ſɲ���
//==========================================================
void Lcd_ReadBF(void)
{
	register u16 i;
	register u8 dat;

	i=2000;
	while(i>0)
	{
		dat=LCD_RAM;
		if((dat&((u8)(SetD1|SetD0)))==(u8)(SetD1|SetD0))	//D1,D0=1,Lcd Ready
			break;
		i--;
	}
}
//==========================================================
//�������ƣ�Lcd_WriteData
//�������ܣ�дһ���ֽڵ����ݵ�LCD
//��ڲ�����u8 ��д����
//���ڲ�������
//�������ڣ�2014.12.16
//�޸����ڣ�2014.12.16 11:18
//��ע˵������
//==========================================================
void Lcd_WriteData(u8 dat)
{
	Lcd_ReadBF();	//�ȴ�Lcd׼����
	LCD_REG=dat;	//д����
}
//==========================================================
//�������ƣ�Lcd_WriteCmd
//�������ܣ�дһ���ֽ�����
//��ڲ�����u8 ��д����
//���ڲ�������
//�������ڣ�2014.12.16
//�޸����ڣ�2014.12.16 11:18
//��ע˵������
//==========================================================
void Lcd_WriteCmd(u8 dat)
{
	Lcd_ReadBF();	//�ȴ�Lcd׼����
	LCD_RAM=dat;	//д����
}
//==========================================================
//�������ƣ�Lcd_AutoWriteData
//�������ܣ��Զ�дģʽ��дһ���ֽڵ����ݵ�LCD
//��ڲ�����u8 ��д����
//���ڲ�������
//�������ڣ�2014.12.16
//�޸����ڣ�2014.12.16 11:18
//��ע˵����D3:�Զ�д����λ��Ϊ1�ſɲ���
//			D2:�Զ�������λ��Ϊ1�ſɲ���
//==========================================================
void Lcd_AutoWriteData(u8 dat)
{
	register u8 i;
	register u8 t;

	i=200;
	while(i>0)
	{
		t=LCD_RAM;
		if((t&((u8)SetD3))==(u8)(SetD3))	//D3=1,Lcd Auto Ready
			break;
		i--;
	}
	LCD_REG=dat;	//д����
}

//==========================================================
//�������ƣ�Lcd_Set_Grh_Addr
//�������ܣ��趨ͼ������
//��ڲ�������
//���ڲ�������
//�������ڣ�2014.12.17
//�޸����ڣ�2014.12.17 09:22
//��ע˵����ͼ������LcdAddr
//==========================================================
void Lcd_Set_Grh_Addr( void )
{
	register u16 addr;

	if(LcdAddr.x>=LCD_ADDR_X_MAX)//X��ַ�б�
	{
		LcdAddr.x=0;//��һ��
		LcdAddr.y++;
	}
	if(LcdAddr.y>=LCD_ADDR_Y_MAX)//Y��ַ�б�
	{
		LcdAddr.y=0;//��һ��
	}

//	addr=LCD_GRH_ADDR_START;
//	addr+=(LCD_GRH_ADDR_WIDTH*LcdAddr.y+LcdAddr.x);
	addr=LCD_GRH_ADDR_WIDTH*LcdAddr.y+LcdAddr.x;
	Lcd_WriteData(addr&0xFF);//��ַL
	Lcd_WriteData((addr>>8)&0xFF);//��ַH
	Lcd_WriteCmd(LCD_CMD_ADDR);//��д��ַ
}
//==========================================================
//�������ƣ�Lcd_Set_Txt_Addr
//�������ܣ��趨�ı�����
//��ڲ�������
//���ڲ�������
//�������ڣ�2014.12.26
//�޸����ڣ�2014.12.26 14:04
//��ע˵����ͼ������LcdAddr
//==========================================================
void Lcd_Set_Txt_Addr( void )
{
	register u16 addr;

	if(LcdAddr.x>=LCD_ADDR_X_MAX)//X��ַ�б�
	{
		LcdAddr.x=0;//��һ��
		LcdAddr.y++;
	}
	if(LcdAddr.y>=LCD_ADDR_Y_MAX)//Y��ַ�б�
	{
		LcdAddr.y=0;//��һ��
	}

	addr=LCD_TXT_ADDR_START;
	addr+=(LcdAddr.x+(LcdAddr.y/8)*LCD_TXT_ADDR_WIDTH);
	Lcd_WriteData(addr&0xFF);//��ַL
	Lcd_WriteData((addr>>8)&0xFF);//��ַH
	Lcd_WriteCmd(LCD_CMD_ADDR);//��д��ַ
}
//==========================================================
//�������ƣ�Lcd_Clr
//�������ܣ�Һ��������
//��ڲ�������
//���ڲ�������
//�������ڣ�2014.12.16
//�޸����ڣ�2014.12.16 15:33
//��ע˵������
//==========================================================
void Lcd_Clr( void )
{
	u16 i;

	LcdAddr.x=0;LcdAddr.y=0;//�׵�ַ
	Lcd_Set_Grh_Addr();//���µ�ǰ��д��ַ
	Lcd_WriteCmd(LCD_CMD_AUTO_WRITE);//�Զ�д����
//	for(i=0;i<((LCD_ADDR_X_MAX)*(LCD_ADDR_Y_MAX)+(LCD_ADDR_X_MAX)*(LCD_ADDR_Y_MAX/8));i++)//ͼ��+�ı�
//	{
//		Lcd_AutoWriteData(0x00);//���			
//	}
	for(i=0;i<(LCD_ADDR_X_MAX)*(LCD_ADDR_Y_MAX);i++)//ͼ��
	{
		Lcd_AutoWriteData(0x00);//���			
	}
	Lcd_WriteCmd(LCD_CMD_AUTO_RESET);//�Զ�д���ݸ�λ

	Lcd_Set_Txt_Addr();//���µ�ǰ��д��ַ
	Lcd_WriteCmd(LCD_CMD_AUTO_WRITE);//�Զ�д����
	for(i=0;i<(LCD_ADDR_X_MAX)*(LCD_ADDR_Y_MAX/8);i++)//�ı�
	{
		Lcd_AutoWriteData(0x00);//���			
	}
	Lcd_WriteCmd(LCD_CMD_AUTO_RESET);//�Զ�д���ݸ�λ
}
//==========================================================
//�������ƣ�Lcd_WR_CGRAM
//�������ܣ�д���ݵ��ⲿCGRAM
//��ڲ�������
//���ڲ�������
//�������ڣ�2014.12.17
//�޸����ڣ�2014.12.17 13:03
//��ע˵������ʾ�ַ�80H-FFH�Զ������ⲿCGRAM��CGRAM��ʼ��ַ�ο�ƫ��������
//ע�����T6963C�ڲ�CGROMֻ֧��8*8�ĵ�128���ַ����ұ����ASCII����Ӧ
//			�ⲿCGRAMͨ��д��16*8��16*16���ֿ�������ݣ��ӿ���ʾ�ٶ�
//Ӧ��˵�����ⲿCGRAMд��16*8������(0-9)��������
//			��ʾ�׸�16*8�ַ�ʱֻ���ڵ�ǰ��д��ַд��80H����һ��д��81H����
//==========================================================
void Lcd_WR_CGRAM(void)
{
	u8 i;
	u8 j;

	Lcd_WriteData(LCD_CGRAM_ADDR_START&0xFF);//��ַL����ƫ��������ֵ��Ӧ
	Lcd_WriteData((LCD_CGRAM_ADDR_START>>8)&0xFF);//��ַH
	Lcd_WriteCmd(LCD_CMD_ADDR);//��д��ַ

	Lcd_WriteCmd(LCD_CMD_AUTO_WRITE);//�Զ�д����
	for(j=0;j<sizeof(TxtArray)/sizeof(Txt1608_TypeDef);j++)//д���ֿ�����
	{
		for(i=0;i<16;i++)
		{
			Lcd_AutoWriteData(AsciiArray[TxtArray[j].Ch[0]-LCD_1608_CODE_START].Dot[i]);//д����
		}
	}
	Lcd_WriteCmd(LCD_CMD_AUTO_RESET);//�Զ�д���ݸ�λ
}

//==========================================================
//�������ƣ�Lcd_Init
//�������ܣ�Һ������ʼ��
//��ڲ�������
//���ڲ�������
//�������ڣ�2014.12.16
//�޸����ڣ�2014.12.16 11:39
//��ע˵�������������Ӧ�ȷ����ݺ�����
//==========================================================
void Lcd_Init(void)
{
	Lcd_WriteCmd(LCD_CMD_DISP_OFF);//��ʾ��

	Lcd_WriteData(LCD_GRH_ADDR_START&0xFF);//ͼ���׵�ַL
	Lcd_WriteData((LCD_GRH_ADDR_START>>8)&0xFF);//ͼ���׵�ַH
	Lcd_WriteCmd(LCD_CMD_GRH_ADDR);//ͼ���׵�ַ
	
	Lcd_WriteData(LCD_GRH_ADDR_WIDTH);//ͼ�ο���L
	Lcd_WriteData(0);//ͼ�ο���H
	Lcd_WriteCmd(LCD_CMD_GRH_WIDTH);//ͼ�ο���

	Lcd_WriteData(LCD_TXT_ADDR_START&0xFF);//�ı��׵�ַL
	Lcd_WriteData((LCD_TXT_ADDR_START>>8)&0xFF);//�ı��׵�ַH
	Lcd_WriteCmd(LCD_CMD_TXT_ADDR);//�ı��׵�ַ
	
	Lcd_WriteData(LCD_TXT_ADDR_WIDTH);//�ı�����L
	Lcd_WriteData(0);//�ı�����H
	Lcd_WriteCmd(LCD_CMD_TXT_WIDTH);//�ı�����
	
	Lcd_WriteCmd(LCD_CMD_MODE_INTERNAL_CGROM|LCD_CMD_MODE_OR);//�ڲ�CGROM����ģʽ

//	Lcd_WriteCmd(LCD_CMD_DISP_OFF|LCD_CMD_DISP_TXT);//�ı���ʾ����ͼ�κ͹���
//	Lcd_WriteCmd(LCD_CMD_DISP_OFF|LCD_CMD_DISP_GRH);//ͼ����ʾ�����ı��͹���
	Lcd_WriteCmd(LCD_CMD_DISP_OFF|LCD_CMD_DISP_TXT|LCD_CMD_DISP_GRH);//�ı���ͼ����ʾ��������

	Lcd_WriteData(LCD_CGRAM_ADDR_OFFSET);//ƫ��������5λ��ַ
	Lcd_WriteData(0);
	Lcd_WriteCmd(LCD_CMD_OFFSET);//External CGRAMƫ�ƼĴ���(D4-D0)����Ӧ��5λ��ַ
	Lcd_WR_CGRAM();//д���ַ�0-9��16*8�����

//	Lcd_WriteData(0);//����ַL
//	Lcd_WriteData(0);//����ַH
//	Lcd_WriteCmd(LCD_CMD_CURSOR_POS);//����ַ
//	Lcd_WriteCmd(LCD_CMD_CURSOR_1LINE);//�����״1��
	
	Lcd_Clr();//����
}

//==========================================================
//�������ƣ�Lcd_Wr_Txt1608
//�������ܣ���ʾ8*16���ı��ַ�
//��ڲ�����u8 ch �ַ�
//���ڲ�������
//�������ڣ�2014.12.26
//�޸����ڣ�2014.12.26 14:56
//��ע˵������ʾ��ַ���ı���ַ����
//==========================================================
void Lcd_Wr_Txt1608(u8 ch)
{
	register u8 i;
	register u8 t;
	
	t=LcdAddr.y;//����
	for(i=0;i<sizeof(TxtArray)/sizeof(TxtArray[0]);i++)
	{
		if(ch==TxtArray[i].Ch[0])//�����ַ�
			break;
	}
	if(i<sizeof(TxtArray)/sizeof(TxtArray[0]))
	{
		Lcd_Set_Txt_Addr();//д��ַ
		Lcd_WriteData(TxtArray[i].Index[0]);//д����
		Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//д����,��ַ����
		LcdAddr.y+=8;//��һ��
		Lcd_Set_Txt_Addr();//д��ַ
		Lcd_WriteData(TxtArray[i].Index[1]);//д����
		Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//д����,��ַ����
	}
	LcdAddr.y=t;//�ָ�
	LcdAddr.x++;//����1���ֽ�
}

//==========================================================
//�������ƣ�Lcd_Wr_1608
//�������ܣ���ʾ8*16���ַ�
//��ڲ�����u8 ch �ַ�, u8 highlight !=0����
//���ڲ�������
//�������ڣ�2014.12.18
//�޸����ڣ�2014.12.18 15:31
//��ע˵������
//==========================================================
void Lcd_Wr_1608(u8 ch, u8 highlight)
{
	register u8 i;
	register u8 t;
	
	t=LcdAddr.y;//����
	for(i=0;i<16;i++)
	{
		Lcd_Set_Grh_Addr();//д��ַ
		if(highlight==0)
			Lcd_WriteData(AsciiArray[ch-' '].Dot[i]);//д����
		else
			Lcd_WriteData(~AsciiArray[ch-' '].Dot[i]);//д����
//		Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//д����,��ַ����
		Lcd_WriteCmd(LCD_CMD_WR_INC_ADDR);//д����,��ַ����
		LcdAddr.y++;//��һ��
	}
	LcdAddr.y=t;//�ָ�
	LcdAddr.x++;//����1���ֽ�
}

//==========================================================
//�������ƣ�Lcd_Wr_1616
//�������ܣ���ʾ16*16���ַ�
//��ڲ�����u8 ch �ַ�, u8 highlight !=0����
//���ڲ�������
//�������ڣ�2014.12.18
//�޸����ڣ�2014.12.18 15:02
//��ע˵������
//==========================================================
void Lcd_Wr_1616(u8 *pt, u8 highlight)
{
	register u8 i;
	register u8 index;
	register u8 t;
	
	t=LcdAddr.y;//����

	for(i=0;i<sizeof(Hanzi16Array)/sizeof(Hanzi16Array[0]);i++)
	{
		if((*pt==Hanzi16Array[i].Str[0])&&(*(pt+1)==Hanzi16Array[i].Str[1]))//���Һ���
			break;
	}
	if(i<sizeof(Hanzi16Array)/sizeof(Hanzi16Array[0]))
	{
		index=i;//�ַ�����
		for(i=0;i<32;)
		{
			Lcd_Set_Grh_Addr();//д��ַ
			if(highlight==0)
				Lcd_WriteData(Hanzi16Array[index].Dot[i++]);//д����
			else
				Lcd_WriteData(~Hanzi16Array[index].Dot[i++]);//д����
			Lcd_WriteCmd(LCD_CMD_WR_INC_ADDR);//д����,��ַ��һ
			if(highlight==0)
				Lcd_WriteData(Hanzi16Array[index].Dot[i++]);//д����
			else
				Lcd_WriteData(~Hanzi16Array[index].Dot[i++]);//д����
			Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//д����,��ַ����
			LcdAddr.y++;//��һ��
		}
	}
	LcdAddr.y=t;//�ָ�
	LcdAddr.x+=2;//����2���ֽ�
}

//==========================================================
//�������ƣ�Lcd_Draw_Line
//�������ܣ���ʾֱ��
//��ڲ�����x,y:��ʼ�㣬len:���Ȼ�߶�
//���ڲ�������
//�������ڣ�2014.12.26
//�޸����ڣ�2014.12.26 15:42
//��ע˵����X=0:�����ߣ�X!=0:������
//==========================================================
void Lcd_Draw_Line(u8 x, u8 y, u8 len)
{
	register u8 i;
	u8 addrx,addry;

	addrx=LcdAddr.x;//����
	addry=LcdAddr.y;
	LcdAddr.x=x;LcdAddr.y=y;

	if(x==0)//������
	{
		Lcd_Set_Grh_Addr();//д��ַ
		Lcd_WriteCmd(LCD_CMD_AUTO_WRITE);//�Զ�д����
		if(len>LCD_ADDR_X_MAX)//��󳤶�
			len=LCD_ADDR_X_MAX;
		for(i=0;i<len;i++)
		{
			Lcd_AutoWriteData(0xFF);
		}
		Lcd_WriteCmd(LCD_CMD_AUTO_RESET);//�Զ�д���ݸ�λ
	}
	else//������
	{
		if(y>LCD_ADDR_Y_MAX)//���߶�
			y=LCD_ADDR_Y_MAX;
		if((len+y)>LCD_ADDR_Y_MAX)//���߶�
			len=LCD_ADDR_Y_MAX-y;//���߶�;
		for(i=addry;i<len;i++)
		{
			Lcd_Set_Grh_Addr();//д��ַ
			Lcd_AutoWriteData(0x80);//D7-D0���������λ
			Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//д����,��ַ����
			LcdAddr.y++;//��һ��
		}
	}
	LcdAddr.x=addrx;//�ָ�
	LcdAddr.y=addry;//�ָ�
}

//==========================================================
//�������ƣ�Lcd_Wr_1212
//�������ܣ���ʾ12*12���ַ�
//��ڲ�����u8 ch �ַ�, u8 highlight !=0����
//���ڲ�������
//�������ڣ�2014.12.18
//�޸����ڣ�2014.12.18 15:02
//��ע˵������������x��ַ���ֽ�(8λ)Ѱַ����Ҫ��������
//==========================================================
//void Lcd_Wr_1212A(u8 *pt, u8 highlight)
//{
//	register u8 i;
//	register u8 index;
//	register u8 t;
//	register u8 dat;
//
//	t=LcdAddr.y;//����
//
//	for(i=0;i<sizeof(Hanzi12Array)/sizeof(Hanzi12Array[0]);i++)
//	{
//		if((*pt==Hanzi12Array[i].Str[0])&&(*(pt+1)==Hanzi12Array[i].Str[1]))//���Һ���
//			break;
//	}
//	if(i<sizeof(Hanzi12Array)/sizeof(Hanzi12Array[0]))//�ַ���Ч
//	{
//		index=i;//�ַ�����
//		for(i=0;i<24;)//д����
//		{
//			Lcd_Set_Grh_Addr();//д��ַ
//			if(highlight==0)
//				Lcd_WriteData(Hanzi12Array[index].Dot[i++]);//д����
//			else
//				Lcd_WriteData(~Hanzi12Array[index].Dot[i++]);//д����
//			Lcd_WriteCmd(LCD_CMD_WR_INC_ADDR);//д����,��ַ��һ
//			//�����ָ��£�8λ��ֻ���¸�4λ����4λ����
//			if(highlight==0)
//				dat=(Hanzi12Array[index].Dot[i++]);
//			else
//				dat=(~Hanzi12Array[index].Dot[i++]);
//			dat&=0xF0;//���¸�4λ
//			Lcd_WriteCmd(LCD_CMD_RD_NOC_ADDR);//������,��ַ����
//			dat|=(LCD_DAT_RD_BUS&0x0F);//��ȡ��ǰ��ַ��������,������4λ
//			Lcd_WriteData(dat);//д����
//			Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//д����,��ַ����
//			LcdAddr.y++;//��һ��
//		}
//	}
//	LcdAddr.y=t;//�ָ�
//	LcdAddr.x+=2;//����2���ֽ�
//}
////==========================================================
//void Lcd_Wr_1212B(u8 *pt, u8 highlight)
//{
//	register u8 i;
//	register u8 index;
//	register u8 t;
//	register u8 dat,dat2;
//
//	t=LcdAddr.y;//����
//
//	for(i=0;i<sizeof(Hanzi12Array)/sizeof(Hanzi12Array[0]);i++)
//	{
//		if((*pt==Hanzi12Array[i].Str[0])&&(*(pt+1)==Hanzi12Array[i].Str[1]))//���Һ���
//			break;
//	}
//	if(i<sizeof(Hanzi12Array)/sizeof(Hanzi12Array[0]))//�ַ���Ч
//	{
//		index=i;//�ַ�����
//		for(i=0;i<24;)//д����
//		{
//			Lcd_Set_Grh_Addr();//д��ַ
//			//ǰ����ָ��£�8λ��ֻ���µ�4λ����4λ����
//			if(highlight==0)
//				dat=(Hanzi12Array[index].Dot[i++]);
//			else
//				dat=(~Hanzi12Array[index].Dot[i++]);
//			dat2=dat;//����
//			dat&=0xF0;//���µ�4λ
//			dat>>=4;
//			Lcd_WriteCmd(LCD_CMD_RD_NOC_ADDR);//������,��ַ����
//			dat|=(LCD_DAT_RD_BUS&0xF0);//��ȡ��ǰ��ַ��������,������4λ
//			Lcd_WriteData(dat);//д����
//			Lcd_WriteCmd(LCD_CMD_WR_INC_ADDR);//д����,��ַ��һ
//			if(highlight==0)
//				dat=(Hanzi12Array[index].Dot[i++]);
//			else
//				dat=(~Hanzi12Array[index].Dot[i++]);
//			dat&=0xF0;//������4λ
//			dat>>=4;
//			dat2&=0x0F;//���µ�4λ
//			dat2<<=4;
//			dat|=dat2;
//			Lcd_WriteData(dat);//д����
//			Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//д����,��ַ����
//			LcdAddr.y++;//��һ��
//		}
//	}
//	LcdAddr.y=t;//�ָ�
//	LcdAddr.x+=2;//����2���ֽ�
//}

//void LCD_WR_REG(u8 regval)
//{ 
//	LCD_REG=regval;//д��Ҫд�ļĴ������	 
//}
////дLCD����
////data:Ҫд���ֵ
//void LCD_WR_DATA(u8 data)
//{										    	   
//	LCD_RAM=data;		 
//}
//��LCD����
//����ֵ:������ֵ
//u16 LCD_RD_DATA(void)
//{										    	   
//	return LCD_RAM;		 
//}					   
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
//void LCD_WriteReg(u8 LCD_Reg, u8 LCD_RegValue)
//{	
//	LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������	 
//	LCD_RAM = LCD_RegValue;//д������	    		 
//}

//���Ĵ���
//LCD_Reg:�Ĵ�����ַ
//����ֵ:����������
//u16 LCD_ReadReg(u8 LCD_Reg)
//{										   
//	LCD_WR_REG(LCD_Reg);		//д��Ҫ���ļĴ������
//	delay_us(5);		  
//	return LCD_RD_DATA();		//���ض�����ֵ
//}  