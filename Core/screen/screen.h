#ifndef __SCREEN_H
#define __SCREEN_H
/*
- �����һ���ڰ׵�����Ļ��ͼ�ο⣬ʹ�������е�ÿһ��λ������Ļ�е����ص�
- ֡����ṹ��VedioBuf[x],[y]
   - ��Ļ�е�y������Ϊ�������еĵ�λ
	 - λ�����Ϊy������
��Ҫʵ�ֽ�֡����ˢ�µ���Ļ�ĺ��� RefreshScreen
*/
#include "stdint.h"

#define _SCREEN_PIXEL_X 128 // ��Ļˮƽ����������ֲ���޸�
#define _SCREEN_PIXEL_Y 64 // ��Ļ��ֱ����������ֲ���޸�

enum DisRever
{
	ScreenNoRever=0, // �����ԣ���ʾΪ����������Ϊ��������
	ScreenRever, // ����
};

void RefreshScreen(void); // ��Ҫ�Լ�ʵ��
void ScreenFill(uint8_t fill);
void ScreenDrawPix(int32_t x, int32_t y, enum DisRever rever);
void ScreenDrawDot(int32_t x, int32_t y, uint32_t degree, enum DisRever rever);
void ScreenDrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t degree, enum DisRever rever);
void ScreenDrawCircule(int32_t x, int32_t y, int32_t r, uint32_t degree, enum DisRever rever);
void ScreenDrawSquare(int32_t x1, int32_t y1, int32_t x2, uint32_t y2, uint32_t fill, uint32_t degree, enum DisRever rever);
void ScreenDrawSquareLangth(int32_t x1, int32_t y1, int32_t length, uint32_t width, uint32_t fill, uint32_t degree, enum DisRever rever);

#endif /* __SCREEN_H */
