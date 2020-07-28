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

enum DisReverse
{
	ScreenNoReverse=0, // 不反显（显示为点亮，否则为不点亮）
	ScreenReverse, // 反显
	ScreenAutoReverse // 自动反显，只能在基于drow pix的函数，不能越级使用，否则会与图像自身反显
};
enum GraphFill
{
	Fill,
	NoFill
};

void RefreshScreen(void); // 需要自己实现
void ScreenFill(uint8_t fill);
void ScreenDrawPix(int32_t x, int32_t y, enum DisReverse rever);
void ScreenDrawDot(int32_t x, int32_t y, uint32_t degree, enum DisReverse rever);
void ScreenDrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t degree, enum DisReverse rever);
void ScreenDrawCircule(int32_t x, int32_t y, int32_t r, uint32_t degree, enum DisReverse rever);
void ScreenDrawSquare(int32_t x1, int32_t y1, int32_t x2, int32_t y2, enum GraphFill fill, uint32_t degree, enum DisReverse rever);
void ScreenDrawSquareLangth(int32_t x1, int32_t y1, int32_t length, uint32_t width, enum GraphFill fill, uint32_t degree, enum DisReverse rever);

#endif /* __SCREEN_H */
