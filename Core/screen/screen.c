/*
 * @Description: 通过现存实现屏幕显示（二色屏—），优点，操作简单，通过减少通信次数提高刷新率，很容易实现任意点显示
 *               移植只需要实现 RefreshScreen 即可
 *               每次操作现存后需要调用 RefreshScreen 将现存刷新到屏幕显示
 * @Author: Huang
 * @Email: huangmailbox@126.com
 * @Version: 
 * @Date: 2020-07-31 23:07:11
 * @LastEditors: Huang
 * @LastEditTime: 2020-08-08 08:28:46
 */
#include "screen.h"
#include "oled.h"

uint8_t VideoBuf[_SCREEN_PIXEL_X][_SCREEN_PIXEL_Y];

/***********功能描述：刷新显存到屏幕*****************/
void RefreshScreen(void)
{
	unsigned char x, y;

	for (y = 0; y < 8; y++)
	{
		OLED_Set_Pos(0, y);
		for (x = 0; x < 128; x++)
		{
			OLED_WR_Byte(VideoBuf[x][y], OLED_DATA);
		}
	}
}
// 显存屏幕填充
void ScreenFill(uint8_t fill)
{
	uint32_t cnt, data_cnt;
	for (cnt = 0; cnt < _SCREEN_PIXEL_X; cnt++)
	{
		for (data_cnt = 0; data_cnt < (_SCREEN_PIXEL_Y / 8 + (_SCREEN_PIXEL_Y % 8 == 0 ? 0 : 1)); data_cnt++)
		{
			VideoBuf[cnt][data_cnt] = fill == 0 ? 0 : ~(0x0);
		}
	}
}
// 设置像素显示值
void ScreenSetPix(int32_t x, int32_t y, enum ScreenPixStatus status)
{
	if (PixERR == status)
		return;
	if ((x < 0) || (y < 0) || (x > (_SCREEN_PIXEL_X - 1)) || (y > (_SCREEN_PIXEL_Y - 1)))
		return;

	switch (status)
	{
	case PixON:
		VideoBuf[x][y / 8] |= 0x01 << (y % 8);
		return;
	case PixOFF:
		VideoBuf[x][y / 8] &= (~(0x01 << (y % 8)));
		return;
	case PixERR:
		return;
	}
}
// 获取像素状态
enum ScreenPixStatus ScreenGetPix(int32_t x, int32_t y)
{
	if ((x < 0) || (y < 0) || (x > (_SCREEN_PIXEL_X - 1)) || (y > (_SCREEN_PIXEL_Y - 1)))
		return PixERR;

	return (VideoBuf[x][y / 8] & (~(0x01 << (y % 8)))) == 0 ? PixOFF : PixON;
}
// 绘制像素点，不反显，反显，自动反显三种操作方式
void ScreenDrawPix(int32_t x, int32_t y, enum DisReverse reverse)
{
	switch ((int)reverse)
	{
	case ScreenNoReverse:
		ScreenSetPix(x, y, PixON);
		return;
	case ScreenReverse:
		ScreenSetPix(x, y, PixOFF);
		return;
	case ScreenAutoReverse:
		switch ((int)ScreenGetPix(x, y))
		{
		case PixON:
			ScreenSetPix(x, y, PixOFF);
			return;
		case PixOFF:
			ScreenSetPix(x, y, PixON);
			return;
		case PixERR:
			return;
		}
		return;
	}
}
// 画图部分
// 显存画点
void ScreenDrawDot(int32_t x, int32_t y, uint32_t degree, enum DisReverse reverse)
{
	int32_t xCnt, yCnt;

	if (0 == degree)
		return;

	if (x >= _SCREEN_PIXEL_X)
	{
		x = _SCREEN_PIXEL_X;
	}
	if (y >= _SCREEN_PIXEL_Y)
	{
		y = _SCREEN_PIXEL_Y - 1;
	}

	for (xCnt = 0; xCnt < (_SCREEN_PIXEL_X > (x + degree) ? degree : (_SCREEN_PIXEL_X - x)); xCnt++)
	{
		for (yCnt = 0; yCnt < (_SCREEN_PIXEL_Y > (y + degree) ? degree : (_SCREEN_PIXEL_Y - y)); yCnt++)
		{
			ScreenDrawPix(x + xCnt, y + yCnt, reverse);
		}
	}
}
// 画线，暂不能使用线粗，只能用单点，需要修改
void ScreenDrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t degree, enum DisReverse reverse)
{ // https://blog.csdn.net/yuanwuwei/article/details/70182793 中点画线
	int dx, dy, e;
	dx = x2 - x1;
	dy = y2 - y1;

	if (dx >= 0)
	{
		if (dy >= 0) // dy>=0
		{
			if (dx >= dy) // 1/8 octant
			{
				e = dy - dx / 2;
				while (x1 <= x2)
				{
					ScreenDrawPix(x1, y1, reverse);
					if (e > 0)
					{
						y1 += 1;
						e -= dx;
					}
					x1 += 1;
					e += dy;
				}
			}
			else // 2/8 octant
			{
				e = dx - dy / 2;
				while (y1 <= y2)
				{
					ScreenDrawPix(x1, y1, reverse);
					if (e > 0)
					{
						x1 += 1;
						e -= dy;
					}
					y1 += 1;
					e += dx;
				}
			}
		}
		else
		{				  // dy<0
			dy = -dy;	  // dy=abs(dy)
			if (dx >= dy) // 8/8 octant
			{
				e = dy - dx / 2;
				while (x1 <= x2)
				{
					ScreenDrawPix(x1, y1, reverse);
					if (e > 0)
					{
						y1 -= 1;
						e -= dx;
					}
					x1 += 1;
					e += dy;
				}
			}
			else // 7/8 octant
			{
				e = dx - dy / 2;
				while (y1 >= y2)
				{
					ScreenDrawPix(x1, y1, reverse);
					if (e > 0)
					{
						x1 += 1;
						e -= dy;
					}
					y1 -= 1;
					e += dx;
				}
			}
		}
	}
	else //dx<0
	{
		dx = -dx;	 //dx=abs(dx)
		if (dy >= 0) // dy>=0
		{
			if (dx >= dy) // 4/8 octant
			{
				e = dy - dx / 2;
				while (x1 >= x2)
				{
					ScreenDrawPix(x1, y1, reverse);
					if (e > 0)
					{
						y1 += 1;
						e -= dx;
					}
					x1 -= 1;
					e += dy;
				}
			}
			else // 3/8 octant
			{
				e = dx - dy / 2;
				while (y1 <= y2)
				{
					ScreenDrawPix(x1, y1, reverse);
					if (e > 0)
					{
						x1 -= 1;
						e -= dy;
					}
					y1 += 1;
					e += dx;
				}
			}
		}
		else // dy<0
		{
			dy = -dy;	  // dy=abs(dy)
			if (dx >= dy) // 5/8 octant
			{
				e = dy - dx / 2;
				while (x1 >= x2)
				{
					ScreenDrawPix(x1, y1, reverse);
					if (e > 0)
					{
						y1 -= 1;
						e -= dx;
					}
					x1 -= 1;
					e += dy;
				}
			}
			else // 6/8 octant
			{
				e = dx - dy / 2;
				while (y1 >= y2)
				{
					ScreenDrawPix(x1, y1, reverse);
					if (e > 0)
					{
						x1 -= 1;
						e -= dy;
					}
					y1 -= 1;
					e += dx;
				}
			}
		}
	}
}
// 画圆，暂不能使用线粗，只能用单点，需要修改
void ScreenDrawCircule(int32_t x1, int32_t y1, int32_t r, uint32_t degree, enum DisReverse reverse)
{ // https://www.cnblogs.com/iamfatotaku/p/12497354.html

	int d0, x = 0, y = r; //d0是判别式的值
						  //	_ScreenDrawCircule(x1, y1, r, degree, rever);
	d0 = 1.25 - r;		  //判别式的初始值，1.25可以改为1
		ScreenDrawPix(x1, y1 + r, reverse);
		ScreenDrawPix(x1, y1 - r, reverse);
		ScreenDrawPix(x1 + r, y1, reverse);
		ScreenDrawPix(x1 - r, y1, reverse);
	while (x < y)
	{
		if (d0 >= 0)
		{
			d0 = d0 + 2 * (x - y) + 5; //d0一定要先比x,y更新
			x += 1;					   //因为d0表达式中的x,y是上一个点
			y -= 1;
			ScreenDrawPix(x + x1, y + y1, reverse);	  //(x,y)
			ScreenDrawPix(-x + x1, y + y1, reverse);  //(-x,y)
			ScreenDrawPix(y + x1, x + y1, reverse);	  //(y,x)
			ScreenDrawPix(-y + x1, x + y1, reverse);  //(-y,x)
			ScreenDrawPix(x + x1, -y + y1, reverse);  //(x,-y)
			ScreenDrawPix(-x + x1, -y + y1, reverse); //(-x,-y)
			ScreenDrawPix(y + x1, -x + y1, reverse);  //(y,-y)
			ScreenDrawPix(-y + x1, -x + y1, reverse); //(-y,-x)
		}
		else
		{
			d0 = d0 + 2 * x + 3;
			x += 1;
			y = y;
			ScreenDrawPix(x + x1, (y + y1), reverse);	//(x,y)
			ScreenDrawPix(-x + x1, (y + y1), reverse);	//(-x,y)
			ScreenDrawPix(y + x1, (x + y1), reverse);	//(y,x)
			ScreenDrawPix(-y + x1, (x + y1), reverse);	//(-y,x)
			ScreenDrawPix(x + x1, (-y + y1), reverse);	//(x,-y)
			ScreenDrawPix(-x + x1, (-y + y1), reverse); //(-x,-y)
			ScreenDrawPix(y + x1, (-x + y1), reverse);	//(y,-y)
			ScreenDrawPix(-y + x1, (-x + y1), reverse); //(-y,-x)
		}
	}
}
void ScreenDrawSquare(int32_t x1, int32_t y1, int32_t x2, int32_t y2, enum GraphFill fill, uint32_t degree, enum DisReverse reverse)
{
	uint32_t Cnt, xCnt;
	if (Fill == fill)
	{
		for (Cnt = 0; Cnt <= ((y2 - y1) >= 0 ? (y2 - y1) : (y1 - y2)); Cnt++)
		{
			for (xCnt = 0; xCnt <= ((x2 - x1) >= 0 ? (x2 - x1) : (x1 - x2)); xCnt++)
			{
				ScreenDrawPix(x1 + xCnt, y1 + Cnt, reverse);
			}
		}
	}
	else if (NoFill == fill)
	{
		for (Cnt = 0; Cnt <= ((y2 - y1) >= 0 ? (y2 - y1) : (y1 - y2)); Cnt++)
		{
			ScreenDrawPix(x1, y1 + Cnt, reverse);
			ScreenDrawPix(x2, y1 + Cnt, reverse);
		}
		for (Cnt = 1; Cnt <= ((x2 - x1) >= 0 ? (x2 - x1) : (x1 - x2)) - 1; Cnt++)
		{
			ScreenDrawPix(x1 + Cnt, y1, reverse);
			ScreenDrawPix(x1 + Cnt, y2, reverse);
		}
	}
}
void ScreenDrawSquareLangth(int32_t x1, int32_t y1, int32_t length, uint32_t width, enum GraphFill fill, uint32_t degree, enum DisReverse reverse)
{
	ScreenDrawSquare(x1, y1, x1 + length - 1, y1 + width - 1, fill, degree, reverse);
}
/********************************************
// fill_Picture
********************************************/
void ScreenDrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, struct BMPPicture *BMP, enum DisReverse reverse)
{
	int temp = 0;
	int disL, disH;
	if (BMP == NULL)
		return;

	if (x1 < x0)
	{
		temp = x1;
		x1 = x0;
		x0 = temp;
	}
	if (y1 < y0)
	{
		temp = y1;
		y1 = y0;
		y0 = temp;
	}
	temp = 0;
	disH = ((y1 - y0) > BMP->hight ? (y1 - y0) : BMP->hight);
	disL = ((y1 - y0) > BMP->length ? (y1 - y0) : BMP->length);
	for (int yCnt = 0; yCnt < disH / 8; yCnt++)
	{
		for (int xCnt = 0; xCnt < disL; xCnt++, temp++)
		{
			for (int lineCnt = 0; lineCnt < 8; lineCnt++)
			{
				ScreenDrawPix(xCnt, yCnt + lineCnt,
							  (BMP->BMPData[yCnt * disL + xCnt] & (0x01 << lineCnt)) == (reverse == ScreenReverse ? 1 : 0) ? ScreenReverse : ScreenNoReverse);
			}
		}
	}
}
//// 写字
//void ScreenShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
//{
//	unsigned char c=0,i=0;
//		c=chr-' ';//得到偏移后的值
//		if(x>Max_Column-1){x=0;y=y+2;}
//		if(Char_Size ==16)
//			{
//			OLED_Set_Pos(x,y);
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
//			OLED_Set_Pos(x,y+1);
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
//			}
//			else {
//				OLED_Set_Pos(x,y);
//				for(i=0;i<6;i++)
//				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
//
//			}
//}
//void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
//{
//	uint8_t t,temp;
//	uint8_t enshow=0;
//	for(t=0;t<len;t++)
//	{
//		temp=(num/oled_pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
//				continue;
//			}else enshow=1;
//
//		}
//	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
//	}
//}
//void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
//{
//	unsigned char j=0;
//	while (chr[j]!='\0')
//	{		OLED_ShowChar(x,y,chr[j],Char_Size);
//			x+=8;
//		if(x>120){x=0;y+=2;}
//			j++;
//	}
//}
//void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
//{
//	uint8_t t,adder=0;
//	OLED_Set_Pos(x,y);
//    for(t=0;t<16;t++)
//		{
//				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
//				adder+=1;
//     }
//		OLED_Set_Pos(x,y+1);
//    for(t=0;t<16;t++)
//			{
//				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
//				adder+=1;
//      }
//}