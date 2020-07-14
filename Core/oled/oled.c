//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : Evk123
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : 0.69��OLED �ӿ���ʾ����(STM32F103ZEϵ��IIC)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��            
//              ----------------------------------------------------------------
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  
#include "bit_domain.h"
//#include "delay.h"
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   


union BitDomain_64 VideoMem[128];

/**********************************************
//IIC Start
**********************************************/
void IIC_Start(void)
{

	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{
OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	
}

void IIC_Wait_Ack(void)
{

	//GPIOB->CRH &= 0XFFF0FFFF;	//����PB12Ϊ��������ģʽ
	//GPIOB->CRH |= 0x00080000;
//	OLED_SDA = 1;
//	delay_us(1);
	//OLED_SCL = 1;
	//delay_us(50000);
/*	while(1)
	{
		if(!OLED_SDA)				//�ж��Ƿ���յ�OLED Ӧ���ź�
		{
			//GPIOB->CRH &= 0XFFF0FFFF;	//����PB12Ϊͨ���������ģʽ
			//GPIOB->CRH |= 0x00030000;
			return;
		}
	}
*/
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}


}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   Write_IIC_Data(dat);
   
		}
	else {
   Write_IIC_Command(dat);
		
	}


}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x02,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}

//��������

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte((((x+2)&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte(((x+2)&0x0f),OLED_CMD); 
}     	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}
void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n����
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
	HAL_Delay(100);
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  

/***********����������ˢ���Դ浽��Ļ*****************/
void RefreshVideoMem(void)
{ 	
	unsigned char x,y;
	
	for(y=0;y<8;y++)
	{
		OLED_Set_Pos((0-2),y);
    for(x=0;x<128;x++)
	    {      
	    	OLED_WR_Byte(VideoMem[x].word8[y],OLED_DATA);	    	
	    }
	}
} 
// �Դ���Ļ���
void OLEDFill(uint8_t fill)
{
	uint32_t cnt;
	for(cnt = 0; cnt < 128; cnt++)
	{
		VideoMem[cnt].word64 = fill==0?0:~(0x0llu);
	}
}
// �Դ滭��
void OLEDDrawDot(uint32_t x, uint32_t y, uint32_t degree, enum DisRever rever)
{
	uint8_t xCnt;
	
	if(0 == degree)
		return;
	
	if(x >= 128)
	{
		x = 127;
	}
	if(y >= 64)
	{
		y = 63;
	}
	
	for(xCnt = 0; xCnt <= (128>(x+degree)?degree-1:(128-x-1)); xCnt++)
	{
		if(OLEDNORever == rever)
		{ // ������
			VideoMem[x+xCnt].word64 |= (~((~0x00llu)<<(64>(y+degree)?degree:(64-y)))) << y;
		}else if(OLEDRever == rever)
		{ // ����
			VideoMem[x+xCnt].word64 &= ~((~((~0x00llu)<<(64>(y+degree)?degree:(64-y)))) << y);
		}else if(OLEDAUTORever == rever)
		{ // �Զ����ԣ������Ѿ���ʾ��ͼ�������෴�Ĳ���
			
		}else // ��������
			break;
	}
}
void OLEDDrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t degree, enum DisRever rever)
{  // https://blog.csdn.net/yuanwuwei/article/details/70182793 �е㻭��
	int dx,dy,e;
	dx=x2-x1; 
	dy=y2-y1;

	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					OLEDDrawDot(x1,y1,degree, rever);
					if(e>0){y1+=1;e-=dx;}
					x1+=1;
					e+=dy;
				}
			}else        // 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					OLEDDrawDot(x1,y1,degree, rever);
					if(e>0){x1+=1;e-=dy;}   
					y1+=1;
					e+=dx;
				}
			}
		}else
		{  // dy<0
			dy=-dy;   // dy=abs(dy)
			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					OLEDDrawDot(x1,y1,degree, rever);
					if(e>0){y1-=1;e-=dx;}   
					x1+=1;
					e+=dy;
				}
			}else        // 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					OLEDDrawDot(x1,y1,degree, rever);
					if(e>0){x1+=1;e-=dy;}   
					y1-=1;
					e+=dx;
				}
			}
		}   
	}else //dx<0
	{
		dx=-dx;     //dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
		if(dx>=dy) // 4/8 octant
		{
			e=dy-dx/2;
			while(x1>=x2)
			{
				OLEDDrawDot(x1,y1,degree, rever);
				if(e>0)
				{
					y1+=1;
					e-=dx;
				}   
				x1-=1;
				e+=dy;
			}
		}else        // 3/8 octant
		{
			e=dx-dy/2;
			while(y1<=y2)
			{
				OLEDDrawDot(x1,y1,degree, rever);
				if(e>0)
				{
					x1-=1;
					e-=dy;
				}   
				y1+=1;
				e+=dx;
			}
		}
		}else           // dy<0
		{
			dy=-dy;   // dy=abs(dy)
			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					OLEDDrawDot(x1,y1,degree, rever);
					if(e>0)
					{
						y1-=1;
						e-=dx;
					}   
					x1-=1;
					e+=dy;
				}
			}else        // 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					OLEDDrawDot(x1,y1,degree, rever);
					if(e>0)
					{
						x1-=1;
						e-=dy;
					}   
					y1-=1;
					e+=dx;
				}
			}
		}   
	}
}
void _OLEDDrawCircule(int32_t x, int32_t y, int32_t r, uint32_t degree, enum DisRever rever)
{ // https://blog.csdn.net/p1126500468/article/details/50428613

	int a, b, num;
	a = 0;
	b = r;
	while(2 * b * b >= r * r)          // 1/8Բ����
	{
			OLEDDrawDot(x + a, y - b, degree, rever); // 0~1
			OLEDDrawDot(x - a, y - b, degree, rever); // 0~7
			OLEDDrawDot(x - a, y + b, degree, rever); // 4~5
			OLEDDrawDot(x + a, y + b, degree, rever); // 4~3
															
			OLEDDrawDot(x + b, y + a, degree, rever); // 2~3
			OLEDDrawDot(x + b, y - a, degree, rever); // 2~1
			OLEDDrawDot(x - b, y - a, degree, rever); // 6~7
			OLEDDrawDot(x - b, y + a, degree, rever); // 6~5
			
			a++;
			num = (a * a + b * b) - r*r;
			if(num > 0)
			{
					b--;
					a--;
			}
		}
}
void OLEDDrawCircule(int32_t x1, int32_t y1, int32_t r, uint32_t degree, enum DisRever rever)
{ // https://www.cnblogs.com/iamfatotaku/p/12497354.html

	int d0, x = 0, y = r;//d0���б�ʽ��ֵ
//	_OLEDDrawCircule(x1, y1, r, degree, rever);
	d0 = 1.25 - r;   //�б�ʽ�ĳ�ʼֵ��1.25���Ը�Ϊ1
	while (x < y) {
		if (d0 >= 0) {
			d0 = d0 + 2 * (x - y) + 5;            //d0һ��Ҫ�ȱ�x,y����
			x += 1;                //��Ϊd0���ʽ�е�x,y����һ����
			y -= 1;
			OLEDDrawDot(x + x1, y + y1, degree, rever);         //(x,y)
			OLEDDrawDot(-x + x1, y + y1, degree, rever);        //(-x,y)
			OLEDDrawDot(y + x1, x + y1, degree, rever);         //(y,x)
			OLEDDrawDot(-y + x1, x + y1, degree, rever);        //(-y,x)
			OLEDDrawDot(x + x1, -y + y1, degree, rever);        //(x,-y)
			OLEDDrawDot(-x + x1, -y + y1, degree, rever);       //(-x,-y)
			OLEDDrawDot(y + x1, -x + y1, degree, rever);        //(y,-y)
			OLEDDrawDot(-y + x1, -x + y1, degree, rever);       //(-y,-x)
		} else {
			d0 = d0 + 2 * x + 3;
			x += 1;
			y = y;
			OLEDDrawDot(x + x1, (y + y1), degree, rever);         //(x,y)
			OLEDDrawDot(-x + x1, (y + y1), degree, rever);        //(-x,y)
			OLEDDrawDot(y + x1, (x + y1), degree, rever);         //(y,x)
			OLEDDrawDot(-y + x1, (x + y1), degree, rever);        //(-y,x)
			OLEDDrawDot(x + x1, (-y + y1), degree, rever);        //(x,-y)
			OLEDDrawDot(-x + x1, (-y + y1), degree, rever);       //(-x,-y)
			OLEDDrawDot(y + x1, (-x + y1), degree, rever);        //(y,-y)
			OLEDDrawDot(-y + x1, (-x + y1), degree, rever);       //(-y,-x)
		}
	}
}
void OLEDDrawSquare(int32_t x1, int32_t y1, int32_t x2, uint32_t y2, uint32_t fill, uint32_t degree, enum DisRever rever)
{
	uint32_t fillcnt;
	if(1 == fill)
	{
		OLEDDrawLine(x1, y1, x1, y2, degree, rever);
		OLEDDrawLine(x1, y1, x2, y1, degree, rever);
		OLEDDrawLine(x1, y2, x2, y2, degree, rever);
		OLEDDrawLine(x2, y1, x2, y2, degree, rever);
		for(fillcnt = 0; fillcnt < y2-y1; fillcnt++)
		{
			OLEDDrawLine(x1, y1+fillcnt, x2, y1+fillcnt, degree, rever);
		}
	}else if(0 == fill)
	{
		OLEDDrawLine(x1, y1, x1, y2, degree, rever);
		OLEDDrawLine(x1, y1, x2, y1, degree, rever);
		OLEDDrawLine(x1, y2, x2, y2, degree, rever);
		OLEDDrawLine(x2, y1, x2, y2, degree, rever);
	}
}
void OLEDDrawSquareLangth(int32_t x1, int32_t y1, int32_t length, uint32_t width, uint32_t fill, uint32_t degree, enum DisRever rever)
{
	OLEDDrawSquare(x1, y1, x1+length, y1+width, fill, degree, rever);
}
