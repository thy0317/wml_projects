#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "led.h"

/**
 * @brief   led pin init and operations
 * @{
 */
void LedInit(void)
{
    GPIO_InitTypeDef led_init;
    RCC_APB2PeriphClockCmd(LED_CLK, ENABLE);

    led_init.GPIO_Pin = LED_PIN;
    led_init.GPIO_Mode = GPIO_Mode_Out_PP;
    led_init.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(LED_PORT, &led_init);
}

void LedOn(void)
{
    GPIO_ResetBits(LED_PORT, LED_PIN);
}

void LedOff(void)
{
    GPIO_SetBits(LED_PORT, LED_PIN);
}

void LedToggle(void)
{
    static int state = 0;

    if (state) {
        LedOff();
        state = 1;
    }
    else {
        LedOn();
        state = 0;
    }
}
/**
 * @}
 */

/**
 * @brief   Out pin init and operation
 * @{
 */
void OutInit(void)
{
    GPIO_InitTypeDef out_init;
    RCC_APB2PeriphClockCmd(OUT_CLK, ENABLE);

    out_init.GPIO_Pin = OUT_PIN;
    out_init.GPIO_Mode = GPIO_Mode_Out_PP;
    out_init.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(OUT_PORT, &out_init);
}

void OutOn(void)
{
    GPIO_SetBits(OUT_PORT, OUT_PIN);
}

void OutOff(void)
{
    GPIO_ResetBits(OUT_PORT, OUT_PIN);
}

void OutToggle(void)
{
    static int state = 0;

    if (state) {
        OutOff();
        state = 1;
    }
    else {
        OutOn();
        state = 0;
    }
}
/**
 * @}
 */

void CtrlOn(void)
{
    LedOn();
    OutOn();
}

void CtrlOff(void)
{
    LedOff();
    OutOff();
}
