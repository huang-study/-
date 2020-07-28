#include "screen.h"
#include "oled.h"

uint8_t VideoBuf[_SCREEN_PIXEL_X][_SCREEN_PIXEL_Y];

/***********����������ˢ���Դ浽��Ļ*****************/
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
// �Դ���Ļ���
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
// �Դ滭���أ����л滭�������ڴ˺�������
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
// �Դ滭��
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
// ���ߣ��ݲ���ʹ���ߴ֣�ֻ���õ��㣬��Ҫ�޸�
void ScreenDrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t degree, enum DisReverse reverse)
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
// ��Բ���ݲ���ʹ���ߴ֣�ֻ���õ��㣬��Ҫ�޸�
void _ScreenDrawCircule(int32_t x, int32_t y, int32_t r, uint32_t degree, enum DisReverse reverse)
{ // https://blog.csdn.net/p1126500468/article/details/50428613

	int a, b, num;
	a = 0;
	b = r;
	while(2 * b * b >= r * r)          // 1/8Բ����
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
// ��Բ���ݲ���ʹ���ߴ֣�ֻ���õ��㣬��Ҫ�޸�
void ScreenDrawCircule(int32_t x1, int32_t y1, int32_t r, uint32_t degree, enum DisReverse reverse)
{ // https://www.cnblogs.com/iamfatotaku/p/12497354.html

	int d0, x = 0, y = r;//d0���б�ʽ��ֵ
//	_ScreenDrawCircule(x1, y1, r, degree, rever);
	d0 = 1.25 - r;   //�б�ʽ�ĳ�ʼֵ��1.25���Ը�Ϊ1
	while (x < y) {
		if (d0 >= 0) {
			d0 = d0 + 2 * (x - y) + 5;            //d0һ��Ҫ�ȱ�x,y����
			x += 1;                //��Ϊd0���ʽ�е�x,y����һ����
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
