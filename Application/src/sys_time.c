// #include "misc.h"
#include "stm32f10x.h"
#include "sys_time.h"

volatile uint32_t systick_count = 0;
uint32_t previous = 0;

void SysTickInit(uint32_t ticks)
{
    SysTick_Config(SystemCoreClock / ticks);
}

uint32_t GetSysTick(void)
{
    return systick_count;
}

void SysTick_Handler(void)
{
    ++systick_count;
}

void DelayMs(uint32_t ms)
{
    uint32_t current = systick_count;
    while ((systick_count - current) < GetMillisecond(ms)) {
    }
}

static uint32_t restart = 1;
uint32_t TimeOut(uint32_t ms)
{
    uint32_t ret_code = 0;

    if (restart == 1) {
        restart = 0;
        previous = systick_count;
    }

    if ((systick_count - previous) > GetMillisecond(ms)) {
        ret_code = 1;
    }

    return ret_code;
}

uint32_t TimePass(uint32_t ms)
{
    return ((systick_count - previous) > GetMillisecond(ms));
}

void TimeOutRestart(void)
{
    restart = 1;
}
