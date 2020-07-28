#include "screen.h"
#include "oled.h"

uint8_t VideoBuf[_SCREEN_PIXEL_X][_SCREEN_PIXEL_Y];

/***********功能描述：刷新显存到屏幕*****************/
void RefreshScreen(void)
{
	unsigned char x,y;
	
	for(y=0;y<8;y++)
	{
		OLED_Set_Pos(0,y);
    for(x=0;x<128;x++)
		{      
			OLED_WR_Byte(VideoBuf[x][y],OLED_DATA);	    	
		}
	}
} 
// 显存屏幕填充
void ScreenFill(uint8_t fill)
{
	uint32_t cnt, data_cnt;
	for(cnt = 0; cnt < _SCREEN_PIXEL_X; cnt++)
	{
		for(data_cnt = 0; data_cnt < (_SCREEN_PIXEL_Y/8 + (_SCREEN_PIXEL_Y%8==0?0:1)); data_cnt++)
		{
			VideoBuf[cnt][data_cnt] = fill==0?0:~(0x0);
		}
	}
}
// 显存画像素，所有绘画函数基于此函数操作
void ScreenDrawPix(int32_t x, int32_t y, enum DisReverse rever)
{
	if((x < 0) || (y < 0) || (x > (_SCREEN_PIXEL_X-1)) || (y > (_SCREEN_PIXEL_Y-1)))
		return;
	
	switch(rever)
	{
		case ScreenNoReverse:
			VideoBuf[x][y/8] |= 0x01<<(y%8);
			return;
		case ScreenReverse:
			VideoBuf[x][y/8] &= (~(0x01<<(y%8)));
			return;
		case ScreenAutoReverse:
			if((VideoBuf[x][y/8]&(0x01<<(y%8)))!=0)
			{
				ScreenDrawPix(x, y, ScreenReverse);
			}else
			{
				ScreenDrawPix(x, y, ScreenNoReverse);
			}
			return;
		default:
			return;
	}
}
// 显存画点
void ScreenDrawDot(int32_t x, int32_t y, uint32_t degree, enum DisReverse rever)
{
	int32_t xCnt, yCnt;
	
	if(0 == degree)
		return;
	
	if(x >= _SCREEN_PIXEL_X)
	{
		x = _SCREEN_PIXEL_X;
	}
	if(y >= _SCREEN_PIXEL_Y)
	{
		y = _SCREEN_PIXEL_Y-1;
	}
	
	for(xCnt = 0; xCnt < (_SCREEN_PIXEL_X>(x+degree)?degree:(_SCREEN_PIXEL_X-x)); xCnt++)
	{
		for(yCnt = 0; yCnt < (_SCREEN_PIXEL_Y>(y+degree)?degree:(_SCREEN_PIXEL_Y-y)); yCnt++)
		{
			ScreenDrawPix(x+xCnt, y+yCnt, rever);
		}
	}
}
// 画线，暂不能使用线粗，只能用单点，需要修改
void ScreenDrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t degree, enum DisReverse reverse)
{  // https://blog.csdn.net/yuanwuwei/article/details/70182793 中点画线
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
					ScreenDrawPix(x1,y1, reverse);
					if(e>0){y1+=1;e-=dx;}
					x1+=1;
					e+=dy;
				}
			}else        // 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					ScreenDrawPix(x1,y1, reverse);
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
					ScreenDrawPix(x1,y1, reverse);
					if(e>0){y1-=1;e-=dx;}   
					x1+=1;
					e+=dy;
				}
			}else        // 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					ScreenDrawPix(x1,y1, reverse);
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
				ScreenDrawPix(x1,y1, reverse);
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
				ScreenDrawPix(x1,y1, reverse);
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
					ScreenDrawPix(x1,y1, reverse);
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
					ScreenDrawPix(x1,y1, reverse);
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
// 画圆，暂不能使用线粗，只能用单点，需要修改
void _ScreenDrawCircule(int32_t x, int32_t y, int32_t r, uint32_t degree, enum DisReverse reverse)
{ // https://blog.csdn.net/p1126500468/article/details/50428613

	int a, b, num;
	a = 0;
	b = r;
	while(2 * b * b >= r * r)          // 1/8圆即可
	{
			ScreenDrawPix(x + a, y - b, reverse); // 0~1
			ScreenDrawPix(x - a, y - b, reverse); // 0~7
			ScreenDrawPix(x - a, y + b, reverse); // 4~5
			ScreenDrawPix(x + a, y + b, reverse); // 4~3
															
			ScreenDrawPix(x + b, y + a, reverse); // 2~3
			ScreenDrawPix(x + b, y - a, reverse); // 2~1
			ScreenDrawPix(x - b, y - a, reverse); // 6~7
			ScreenDrawPix(x - b, y + a, reverse); // 6~5
			
			a++;
			num = (a * a + b * b) - r*r;
			if(num > 0)
			{
					b--;
					a--;
			}
		}
}
// 画圆，暂不能使用线粗，只能用单点，需要修改
void ScreenDrawCircule(int32_t x1, int32_t y1, int32_t r, uint32_t degree, enum DisReverse reverse)
{ // https://www.cnblogs.com/iamfatotaku/p/12497354.html

	int d0, x = 0, y = r;//d0是判别式的值
//	_ScreenDrawCircule(x1, y1, r, degree, rever);
	d0 = 1.25 - r;   //判别式的初始值，1.25可以改为1
	while (x < y) {
		if (d0 >= 0) {
			d0 = d0 + 2 * (x - y) + 5;            //d0一定要先比x,y更新
			x += 1;                //因为d0表达式中的x,y是上一个点
			y -= 1;
			ScreenDrawPix(x + x1, y + y1, reverse);         //(x,y)
			ScreenDrawPix(-x + x1, y + y1, reverse);        //(-x,y)
			ScreenDrawPix(y + x1, x + y1, reverse);         //(y,x)
			ScreenDrawPix(-y + x1, x + y1, reverse);        //(-y,x)
			ScreenDrawPix(x + x1, -y + y1, reverse);        //(x,-y)
			ScreenDrawPix(-x + x1, -y + y1, reverse);       //(-x,-y)
			ScreenDrawPix(y + x1, -x + y1, reverse);        //(y,-y)
			ScreenDrawPix(-y + x1, -x + y1, reverse);       //(-y,-x)
		} else {
			d0 = d0 + 2 * x + 3;
			x += 1;
			y = y;
			ScreenDrawPix(x + x1, (y + y1), reverse);         //(x,y)
			ScreenDrawPix(-x + x1, (y + y1), reverse);        //(-x,y)
			ScreenDrawPix(y + x1, (x + y1), reverse);         //(y,x)
			ScreenDrawPix(-y + x1, (x + y1), reverse);        //(-y,x)
			ScreenDrawPix(x + x1, (-y + y1), reverse);        //(x,-y)
			ScreenDrawPix(-x + x1, (-y + y1), reverse);       //(-x,-y)
			ScreenDrawPix(y + x1, (-x + y1), reverse);        //(y,-y)
			ScreenDrawPix(-y + x1, (-x + y1), reverse);       //(-y,-x)
		}
	}
}
void ScreenDrawSquare(int32_t x1, int32_t y1, int32_t x2, int32_t y2, enum GraphFill fill, uint32_t degree, enum DisReverse reverse)
{
	uint32_t Cnt, xCnt;
	if(Fill == fill)
	{
		for(Cnt = 0; Cnt <= ((y2-y1)>=0?(y2-y1):(y1-y2)); Cnt++)
		{
			for(xCnt = 0; xCnt <= ((x2-x1)>=0?(x2-x1):(x1-x2)); xCnt++)
			{
				ScreenDrawPix(x1+xCnt, y1+Cnt, reverse);
			}
		}
	}else if(NoFill == fill)
	{
		for(Cnt = 0; Cnt <= ((y2-y1)>=0?(y2-y1):(y1-y2)); Cnt++)
		{
			ScreenDrawPix(x1, y1+Cnt, reverse);
			ScreenDrawPix(x2, y1+Cnt, reverse);
		}
		for(Cnt = 1; Cnt <= ((x2-x1)>=0?(x2-x1):(x1-x2))-1; Cnt++)
		{
			ScreenDrawPix(x1+Cnt, y1, reverse);
			ScreenDrawPix(x1+Cnt, y2, reverse);
		}
	}
}
void ScreenDrawSquareLangth(int32_t x1, int32_t y1, int32_t length, uint32_t width, enum GraphFill fill, uint32_t degree, enum DisReverse reverse)
{
	ScreenDrawSquare(x1, y1, x1+length-1, y1+width-1, fill, degree, reverse);
}
