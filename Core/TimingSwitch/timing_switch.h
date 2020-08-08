#ifndef __TIMING_SWITCH_H
#define __TIMING_SWITCH_H

#include "stm32f1xx_hal.h"

#ifndef TRUE
#define TRUE 0
#endif /* #ifndef TRUE */

#ifndef FALSE
#define FALSE 1
#endif /* #ifndef TRUE */

#define MAX_SWITCH      (32) // ���ʱ�����������޸Ĵ�ֵ���Ӷ�ʱ����

enum SwitchStatus
{
	SwitchON=1,
	SwitchOFF=2,
	SwitchERR=3
};

// ������

// ��ȡ����״̬
enum SwitchStatus SwitchSetEn(uint16_t SwitchNum, enum SwitchStatus En); // ���ö�ʱ����ʹ��,����״̬
enum SwitchStatus SwitchGetEn(uint16_t SwitchNum);
// ���ÿ���״̬
enum SwitchStatus SwitchSetOut(uint16_t SwitchNum, enum SwitchStatus Stat); // ���ö�ʱ�������
enum SwitchStatus SwitchGetOut(uint16_t SwitchNum);
// ģ���ʼ��
void TimingSwitchInit(uint32_t TimingCyc); // ��ʼ����ʱ����,���붨ʱ������(us)
// ������ʱ��
uint8_t SwitchTimingStart(uint16_t TimingSwitchNum, unsigned long TimerLen); // �򿪶�ʱ��
uint8_t SwitchTimingStop(uint16_t uint16_tTimerNum); // ֹͣ��ʱ��

// ����ʱ���жϵ���
extern void SwitchTiming_cb(void);

void SwitchTiming_cb(void);

#endif /* __TIMING_SWITCH_H */
