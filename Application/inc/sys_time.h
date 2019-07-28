#ifndef __SYS_TIME_H
#define __SYS_TIME_H

#include "stdint.h"

#define TICKS_PER_SECOND    100

#define GetSecond(a)        (a * TICKS_PER_SECOND)
#define GetMillisecond(a)   (a * TICKS_PER_SECOND / 1000)

void SysTickInit(uint32_t ticks);
uint32_t GetSysTick(void);
void DelayMs(uint32_t ms);
uint32_t TimeOut(uint32_t ms);
void TimeOutRestart(void);
uint32_t TimePass(uint32_t ms);

#endif /* __SYS_TIME_H */
