#include "screen.h"
#include "oled.h"

uint8_t VideoBuf[_SCREEN_PIXEL_X][_SCREEN_PIXEL_Y];

/***********功能描述：刷新显存到屏幕*****************/
void RefreshScreen(void)
{ //.完善刷新函数，测试显示（走点）

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
// 显存画像素
void ScreenDrawPix(int32_t x, int32_t y, enum DisRever rever)
{
	if((x < 0) || (y < 0) || (x > (_SCREEN_PIXEL_X-1)) || (y > (_SCREEN_PIXEL_Y-1)))
		return;
	
	switch(rever)
	{
		case ScreenNoRever:
			VideoBuf[x][y/8] |= 0x01<<(y%8);
			return;
		case ScreenRever:
			VideoBuf[x][y/8] &= (~(0x01<<(y%8)));
			return;
		default:
			return;
	}
}
// 显存画点
void ScreenDrawDot(int32_t x, int32_t y, uint32_t degree, enum DisRever rever)
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
	
	for(xCnt = 0; xCnt <= (_SCREEN_PIXEL_X>(x+degree)?degree:(_SCREEN_PIXEL_X-x)); xCnt++)
	{
		for(yCnt = 0; yCnt <= (_SCREEN_PIXEL_Y>(y+degree)?degree:(_SCREEN_PIXEL_Y-y)); yCnt++)
		{
			ScreenDrawPix(x+xCnt, y+yCnt, rever);
		}
	}
}
void ScreenDrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t degree, enum DisRever rever)
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
					ScreenDrawDot(x1,y1,degree, rever);
					if(e>0){y1+=1;e-=dx;}
					x1+=1;
					e+=dy;
				}
			}else        // 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					ScreenDrawDot(x1,y1,degree, rever);
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
					ScreenDrawDot(x1,y1,degree, rever);
					if(e>0){y1-=1;e-=dx;}   
					x1+=1;
					e+=dy;
				}
			}else        // 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					ScreenDrawDot(x1,y1,degree, rever);
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
				ScreenDrawDot(x1,y1,degree, rever);
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
				ScreenDrawDot(x1,y1,degree, rever);
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
					ScreenDrawDot(x1,y1,degree, rever);
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
					ScreenDrawDot(x1,y1,degree, rever);
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
void _ScreenDrawCircule(int32_t x, int32_t y, int32_t r, uint32_t degree, enum DisRever rever)
{ // https://blog.csdn.net/p1126500468/article/details/50428613

	int a, b, num;
	a = 0;
	b = r;
	while(2 * b * b >= r * r)          // 1/8圆即可
	{
			ScreenDrawDot(x + a, y - b, degree, rever); // 0~1
			ScreenDrawDot(x - a, y - b, degree, rever); // 0~7
			ScreenDrawDot(x - a, y + b, degree, rever); // 4~5
			ScreenDrawDot(x + a, y + b, degree, rever); // 4~3
															
			ScreenDrawDot(x + b, y + a, degree, rever); // 2~3
			ScreenDrawDot(x + b, y - a, degree, rever); // 2~1
			ScreenDrawDot(x - b, y - a, degree, rever); // 6~7
			ScreenDrawDot(x - b, y + a, degree, rever); // 6~5
			
			a++;
			num = (a * a + b * b) - r*r;
			if(num > 0)
			{
					b--;
					a--;
			}
		}
}
void ScreenDrawCircule(int32_t x1, int32_t y1, int32_t r, uint32_t degree, enum DisRever rever)
{ // https://www.cnblogs.com/iamfatotaku/p/12497354.html

	int d0, x = 0, y = r;//d0是判别式的值
//	_ScreenDrawCircule(x1, y1, r, degree, rever);
	d0 = 1.25 - r;   //判别式的初始值，1.25可以改为1
	while (x < y) {
		if (d0 >= 0) {
			d0 = d0 + 2 * (x - y) + 5;            //d0一定要先比x,y更新
			x += 1;                //因为d0表达式中的x,y是上一个点
			y -= 1;
			ScreenDrawDot(x + x1, y + y1, degree, rever);         //(x,y)
			ScreenDrawDot(-x + x1, y + y1, degree, rever);        //(-x,y)
			ScreenDrawDot(y + x1, x + y1, degree, rever);         //(y,x)
			ScreenDrawDot(-y + x1, x + y1, degree, rever);        //(-y,x)
			ScreenDrawDot(x + x1, -y + y1, degree, rever);        //(x,-y)
			ScreenDrawDot(-x + x1, -y + y1, degree, rever);       //(-x,-y)
			ScreenDrawDot(y + x1, -x + y1, degree, rever);        //(y,-y)
			ScreenDrawDot(-y + x1, -x + y1, degree, rever);       //(-y,-x)
		} else {
			d0 = d0 + 2 * x + 3;
			x += 1;
			y = y;
			ScreenDrawDot(x + x1, (y + y1), degree, rever);         //(x,y)
			ScreenDrawDot(-x + x1, (y + y1), degree, rever);        //(-x,y)
			ScreenDrawDot(y + x1, (x + y1), degree, rever);         //(y,x)
			ScreenDrawDot(-y + x1, (x + y1), degree, rever);        //(-y,x)
			ScreenDrawDot(x + x1, (-y + y1), degree, rever);        //(x,-y)
			ScreenDrawDot(-x + x1, (-y + y1), degree, rever);       //(-x,-y)
			ScreenDrawDot(y + x1, (-x + y1), degree, rever);        //(y,-y)
			ScreenDrawDot(-y + x1, (-x + y1), degree, rever);       //(-y,-x)
		}
	}
}
void ScreenDrawSquare(int32_t x1, int32_t y1, int32_t x2, uint32_t y2, uint32_t fill, uint32_t degree, enum DisRever rever)
{
	uint32_t fillcnt;
	if(1 == fill)
	{
		ScreenDrawLine(x1, y1, x1, y2, degree, rever);
		ScreenDrawLine(x1, y1, x2, y1, degree, rever);
		ScreenDrawLine(x1, y2, x2, y2, degree, rever);
		ScreenDrawLine(x2, y1, x2, y2, degree, rever);
		for(fillcnt = 0; fillcnt < y2-y1; fillcnt++)
		{
			ScreenDrawLine(x1, y1+fillcnt, x2, y1+fillcnt, degree, rever);
		}
	}else if(0 == fill)
	{
		ScreenDrawLine(x1, y1, x1, y2, degree, rever);
		ScreenDrawLine(x1, y1, x2, y1, degree, rever);
		ScreenDrawLine(x1, y2, x2, y2, degree, rever);
		ScreenDrawLine(x2, y1, x2, y2, degree, rever);
	}
}
void ScreenDrawSquareLangth(int32_t x1, int32_t y1, int32_t length, uint32_t width, uint32_t fill, uint32_t degree, enum DisRever rever)
{
	ScreenDrawSquare(x1, y1, x1+length, y1+width, fill, degree, rever);
}
