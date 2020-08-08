#include "timing_switch.h"
#include "tim.h"

// 通过越界来实现计数，无需进行极限判断，需保证判断的值和计数值类型一致

struct
{
	uint16_t TimingSwitchCounter;
	uint16_t TimingCounterList[MAX_SWITCH];
	union// 保存定时器的值或指针，
	{
		void *pTim;
		uint32_t TimNum;
	}Tim;
	uint16_t TimingCyc;
	uint8_t SwitchEn[(MAX_SWITCH/8)+(((MAX_SWITCH&0x07)==0?0:1))]; // 每一个位代表每一个定时开关的使能
	uint8_t SwitchOut[(MAX_SWITCH/8)+(((MAX_SWITCH&0x07)==0?0:1))]; // 每一个位代表每一个定时开关的触发
}SwitchInfo;

// 移植需要修改，并需要将 SwitchTiming_cb 放到定时器的更新中断处理函数中调用
void TimingStart_cb(uint16_t TimingCyc)
{ // 定时器开启函
	HAL_TIM_Base_Start_IT(&htim4);
}
void TimingStop_cb(void)
{ // 定时器关闭函数
	HAL_TIM_Base_Stop_IT(&htim4);
}
// 修改定时器编号的初始化值
void TimingSwitchInit(uint32_t TimingCyc) // 初始化定时开关,传入定时器周期(us)
{
	uint16_t uint8_tCount = 0;
	SwitchInfo.TimingSwitchCounter = 0;
	SwitchInfo.TimingCyc = TimingCyc;
	
	SwitchInfo.Tim.TimNum = 0;
	
	for (uint8_tCount = 0; uint8_tCount < MAX_SWITCH; uint8_tCount++)
	{
		SwitchInfo.TimingCounterList[uint8_tCount] = 0;
		SwitchSetEn(uint8_tCount + 1, SwitchOFF);
		SwitchSetOut(uint8_tCount + 1, SwitchOFF);
	}
	return;
}
enum SwitchStatus SwitchSetEn(uint16_t SwitchNum, enum SwitchStatus En) // 设置定时开关使能,返回状态
{
	if((MAX_SWITCH < SwitchNum) || (En == SwitchERR) || (0 == SwitchNum))
		return SwitchERR;
	
	SwitchNum--;
	
	switch((int)En)
	{
		case SwitchON:
			SwitchInfo.SwitchEn[SwitchNum/8]|=(0x01<<(SwitchNum&0x7));
			return SwitchON;
		case SwitchOFF:
			SwitchInfo.SwitchEn[SwitchNum/8]&=(~(0x01<<(SwitchNum&0x7)));
			return SwitchOFF;
		default:
			return SwitchERR;
	}
}
enum SwitchStatus SwitchGetEn(uint16_t SwitchNum)
{
	if((MAX_SWITCH < SwitchNum) || (SwitchNum == 0))
		return SwitchERR;
	
	SwitchNum--;
	
	return (SwitchInfo.SwitchEn[SwitchNum/8]&(0x01<<(SwitchNum&0x7)))==0?SwitchOFF:SwitchON;
}

enum SwitchStatus SwitchSetOut(uint16_t SwitchNum, enum SwitchStatus Stat) // 设置定时开关使能,返回状态
{
	if((MAX_SWITCH < SwitchNum) || (Stat == SwitchERR) || (SwitchNum == 0))
		return SwitchERR;
	
	SwitchNum--;
	
	switch((int)Stat)
	{
		case SwitchON:
			SwitchInfo.SwitchOut[SwitchNum/8]|=(0x01<<(SwitchNum&0x7));
			return SwitchON;
		case SwitchOFF:
			SwitchInfo.SwitchOut[SwitchNum/8]&=(~(0x01<<(SwitchNum&0x7)));
			return SwitchOFF;
		default:
			return SwitchOFF;
	}
}
enum SwitchStatus SwitchGetOut(uint16_t SwitchNum)
{
	if((MAX_SWITCH < SwitchNum) || (SwitchNum == 0))
		return SwitchERR;
	
	SwitchNum--;
	
	return (SwitchInfo.SwitchOut[SwitchNum/8]&(0x01<<(SwitchNum&0x7)))==0?SwitchOFF:SwitchON;
}


uint8_t SwitchTimingStart(uint16_t TimingSwitchNum, unsigned long TimerLen) // 打开定时器
{
    if ((TimingSwitchNum > MAX_SWITCH) || (TimingSwitchNum == 0) || (TimerLen == 0))
        return FALSE;
    if(SwitchGetEn(TimingSwitchNum) == SwitchOFF)
    {
			SwitchSetEn(TimingSwitchNum, SwitchON);
			
			TimingStart_cb(SwitchInfo.TimingCyc);
			
			SwitchInfo.TimingCounterList[TimingSwitchNum - 1] = TimerLen/SwitchInfo.TimingCyc + SwitchInfo.TimingSwitchCounter; // 保存当前计数值和延时时间之和，当计数值到此时会触发
    }
    return TRUE;
}

uint8_t SwitchTimingStop(uint16_t uint16_tTimerNum) // 停止定时器
{
    if ((uint16_tTimerNum > MAX_SWITCH) || (uint16_tTimerNum == 0))
        return FALSE;
    SwitchSetEn(uint16_tTimerNum, SwitchOFF);
    SwitchSetOut(uint16_tTimerNum, SwitchOFF);
		
		TimingStop_cb();
		
    return TRUE;
}

// 定时器更新时间回调函数
void SwitchTiming_cb(void)
{
	uint8_t uint8_tCount = 0;
	SwitchInfo.TimingSwitchCounter++;
	do
	{
		if (SwitchGetEn(uint8_tCount + 1) == SwitchON)
		{
			if (SwitchGetOut(uint8_tCount + 1) == SwitchOFF)
			{
				if (SwitchInfo.TimingSwitchCounter == SwitchInfo.TimingCounterList[uint8_tCount])
				{
						SwitchSetOut(uint8_tCount + 1, SwitchON);
				}
			}
		}
	} while (++uint8_tCount < MAX_SWITCH);
}
