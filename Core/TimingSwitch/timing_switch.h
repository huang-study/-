#ifndef __TIMING_SWITCH_H
#define __TIMING_SWITCH_H

#include "stm32f1xx_hal.h"

#ifndef TRUE
#define TRUE 0
#endif /* #ifndef TRUE */

#ifndef FALSE
#define FALSE 1
#endif /* #ifndef TRUE */

#define MAX_SWITCH      (32) // 最大定时开关数量，修改此值增加定时开关

enum SwitchStatus
{
	SwitchON=1,
	SwitchOFF=2,
	SwitchERR=3
};

// 供调用

// 获取开关状态
enum SwitchStatus SwitchSetEn(uint16_t SwitchNum, enum SwitchStatus En); // 设置定时开关使能,返回状态
enum SwitchStatus SwitchGetEn(uint16_t SwitchNum);
// 设置开关状态
enum SwitchStatus SwitchSetOut(uint16_t SwitchNum, enum SwitchStatus Stat); // 设置定时开关输出
enum SwitchStatus SwitchGetOut(uint16_t SwitchNum);
// 模块初始化
void TimingSwitchInit(uint32_t TimingCyc); // 初始化定时开关,传入定时器周期(us)
// 开启定时器
uint8_t SwitchTimingStart(uint16_t TimingSwitchNum, unsigned long TimerLen); // 打开定时器
uint8_t SwitchTimingStop(uint16_t uint16_tTimerNum); // 停止定时器

// 供定时器中断调用
extern void SwitchTiming_cb(void);

void SwitchTiming_cb(void);

#endif /* __TIMING_SWITCH_H */
