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

enum ScreenPixStatus
{
	PixERR=0,
	PixON,
	PixOFF
};

enum DisReverse
{
	ScreenNoReverse=0, // �����ԣ���ʾΪ����������Ϊ��������
	ScreenReverse, // ����
	ScreenAutoReverse // �Զ����ԣ�ֻ���ڻ���drow pix�ĺ���������Խ��ʹ�ã��������ͼ��������
};
enum GraphFill
{
	Fill,
	NoFill
};

struct BMPPicture
{ // λͼɨ��˳��Ϊ����ʽɨ�裬���Ͻ�Ϊ��һ�е�һ�еĵ�0������, �����ֽڶ���
	int length;
	int hight;
	unsigned char *BMPData;
};

void RefreshScreen(void); // ��Ҫ�Լ�ʵ��

void ScreenFill(uint8_t fill);

void ScreenSetPix(int32_t x, int32_t y, enum ScreenPixStatus status);
enum ScreenPixStatus ScreenGetPix(int32_t x, int32_t y);
void ScreenDrawPix(int32_t x, int32_t y, enum DisReverse reverse);

void ScreenDrawDot(int32_t x, int32_t y, uint32_t degree, enum DisReverse rever);
void ScreenDrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t degree, enum DisReverse rever);
void ScreenDrawCircule(int32_t x, int32_t y, int32_t r, uint32_t degree, enum DisReverse rever);
void ScreenDrawSquare(int32_t x1, int32_t y1, int32_t x2, int32_t y2, enum GraphFill fill, uint32_t degree, enum DisReverse rever);
void ScreenDrawSquareLangth(int32_t x1, int32_t y1, int32_t length, uint32_t width, enum GraphFill fill, uint32_t degree, enum DisReverse rever);

#endif /* __SCREEN_H */
