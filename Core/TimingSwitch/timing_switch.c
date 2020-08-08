#include "timing_switch.h"
#include "tim.h"

// ͨ��Խ����ʵ�ּ�����������м����жϣ��豣֤�жϵ�ֵ�ͼ���ֵ����һ��

struct
{
	uint16_t TimingSwitchCounter;
	uint16_t TimingCounterList[MAX_SWITCH];
	union// ���涨ʱ����ֵ��ָ�룬
	{
		void *pTim;
		uint32_t TimNum;
	}Tim;
	uint16_t TimingCyc;
	uint8_t SwitchEn[(MAX_SWITCH/8)+(((MAX_SWITCH&0x07)==0?0:1))]; // ÿһ��λ����ÿһ����ʱ���ص�ʹ��
	uint8_t SwitchOut[(MAX_SWITCH/8)+(((MAX_SWITCH&0x07)==0?0:1))]; // ÿһ��λ����ÿһ����ʱ���صĴ���
}SwitchInfo;

// ��ֲ��Ҫ�޸ģ�����Ҫ�� SwitchTiming_cb �ŵ���ʱ���ĸ����жϴ������е���
void TimingStart_cb(uint16_t TimingCyc)
{ // ��ʱ��������
	HAL_TIM_Base_Start_IT(&htim4);
}
void TimingStop_cb(void)
{ // ��ʱ���رպ���
	HAL_TIM_Base_Stop_IT(&htim4);
}
// �޸Ķ�ʱ����ŵĳ�ʼ��ֵ
void TimingSwitchInit(uint32_t TimingCyc) // ��ʼ����ʱ����,���붨ʱ������(us)
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
enum SwitchStatus SwitchSetEn(uint16_t SwitchNum, enum SwitchStatus En) // ���ö�ʱ����ʹ��,����״̬
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

enum SwitchStatus SwitchSetOut(uint16_t SwitchNum, enum SwitchStatus Stat) // ���ö�ʱ����ʹ��,����״̬
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


uint8_t SwitchTimingStart(uint16_t TimingSwitchNum, unsigned long TimerLen) // �򿪶�ʱ��
{
    if ((TimingSwitchNum > MAX_SWITCH) || (TimingSwitchNum == 0) || (TimerLen == 0))
        return FALSE;
    if(SwitchGetEn(TimingSwitchNum) == SwitchOFF)
    {
			SwitchSetEn(TimingSwitchNum, SwitchON);
			
			TimingStart_cb(SwitchInfo.TimingCyc);
			
			SwitchInfo.TimingCounterList[TimingSwitchNum - 1] = TimerLen/SwitchInfo.TimingCyc + SwitchInfo.TimingSwitchCounter; // ���浱ǰ����ֵ����ʱʱ��֮�ͣ�������ֵ����ʱ�ᴥ��
    }
    return TRUE;
}

uint8_t SwitchTimingStop(uint16_t uint16_tTimerNum) // ֹͣ��ʱ��
{
    if ((uint16_tTimerNum > MAX_SWITCH) || (uint16_tTimerNum == 0))
        return FALSE;
    SwitchSetEn(uint16_tTimerNum, SwitchOFF);
    SwitchSetOut(uint16_tTimerNum, SwitchOFF);
		
		TimingStop_cb();
		
    return TRUE;
}

// ��ʱ������ʱ��ص�����
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
