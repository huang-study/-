#ifndef __SCREEN_H
#define __SCREEN_H
/*
- 这个是一个黑白点阵屏幕的图形库，使用数组中的每一个位代表屏幕中的像素点
- 帧缓存结构：VedioBuf[x],[y]
   - 屏幕中的y轴像素为连续排列的的位
	 - 位列序号为y轴坐标
需要实现将帧缓存刷新到屏幕的函数 RefreshScreen
*/
#include "stdint.h"

#define _SCREEN_PIXEL_X 128 // 屏幕水平像素数，移植需修改
#define _SCREEN_PIXEL_Y 64 // 屏幕垂直像素数，移植需修改

enum DisRever
{
	ScreenNoRever=0, // 不反显（显示为点亮，否则为不点亮）
	ScreenRever, // 反显
};

void RefreshScreen(void); // 需要自己实现
void ScreenFill(uint8_t fill);
void ScreenDrawPix(int32_t x, int32_t y, enum DisRever rever);
void ScreenDrawDot(int32_t x, int32_t y, uint32_t degree, enum DisRever rever);
void ScreenDrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t degree, enum DisRever rever);
void ScreenDrawCircule(int32_t x, int32_t y, int32_t r, uint32_t degree, enum DisRever rever);
void ScreenDrawSquare(int32_t x1, int32_t y1, int32_t x2, uint32_t y2, uint32_t fill, uint32_t degree, enum DisRever rever);
void ScreenDrawSquareLangth(int32_t x1, int32_t y1, int32_t length, uint32_t width, uint32_t fill, uint32_t degree, enum DisRever rever);

#endif /* __SCREEN_H */
