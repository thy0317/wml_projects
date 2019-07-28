#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "key.h"
#include "sys_time.h"

/**
 * @brief   key pin init and operations
 * @{
 */
void KeyInit(void)
{
    GPIO_InitTypeDef key_init;
    RCC_APB2PeriphClockCmd(KEY_CLK, ENABLE);

    key_init.GPIO_Pin = KEY_PIN;
    key_init.GPIO_Mode = GPIO_Mode_IPU;
    key_init.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(KEY_PORT, &key_init);
}

uint8_t ReadKey(void)
{
    return GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN);
}

enum KeyValue KeyTask(void)
{
    enum KeyValue ret_value = kNone;
    #if IS_HOLD_KEY /* 自锁开关 */
    if (ReadKey() == Bit_RESET) { /* 关窗 */
        ret_value = kPressed;
    }
    else { /* 开窗 */
        ret_value = kNone;
    }
    #else
    static unsigned int previous_tick = 0, scan_state = 0;
    unsigned int current_tick = GetSysTick();

    if ((current_tick - previous_tick) > GetMillisecond(50)) { /* 50ms */
        previous_tick = current_tick;
        
        switch (scan_state)
        {
            case 0: {
                if (ReadKey() == Bit_RESET) { /* press */
                    scan_state = 1;
                }
            } break;
            case 1: {
                if (ReadKey() == Bit_RESET) { /* pressing */
                    scan_state = 2;
                }
                else {
                    scan_state = 0;
                }
            } break;
            case 2: {
                if (ReadKey() == Bit_SET) { /* pressed */
                    scan_state = 0;
                    ret_value = kPressed;
                }
            } break;
            default: {
                scan_state = 0;
                ret_value = kNone;
            } break;
        }
    }

    return ret_value;
    #endif
}

/**
 * @}
 */
