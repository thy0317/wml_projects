#include "stm32f10x.h"
#include "sys_time.h"
#include "led.h"
#include "key.h"
#include "ws2812.h"
#include "uart.h"
#include "led_visual.h"

int main(void)
{
    static unsigned int state = 0;
    static uint32_t index1 = 0, index2 = 0, sent_P1 = 0;
    static uint32_t time_out = 6000; /* 若要段2流水至圈2 圈2刚好开始流水 需调整此时间 单位ms */

    set_breath_value();
    SysTickInit(1000); /* 1ms */
    WS2812_DOInit();
    Uart1Init();
    OutInit();
    LedInit();
    KeyInit();

    CtrlOff();
    
    DelayMs(50);
    for (int i = 0; i < LED_ALL; i++) { /* 亮白灯 */
        color_buf[i] = 0xffffff;
        color_cricle2[i % CRICLE2_IN] = 0xffa0ff;
        color_cricle3[i % CRICLE3_IN] = 0xffa0ff;
    }
    led_visual();

    while (1) {
        #if 1 /* TODO：提前开关窗 */
        if (state == 1 || state == 2 || state == 3) {
            if (KeyTask() == kPressed) { /* 提前关窗 */
                state = 4;
                for (int i = 0; i < LED_ALL; i++) { /* 亮白灯 */
                    color_buf[i] = 0xffffff;
                    color_cricle2[i % CRICLE2_IN] = 0xffa0ff;
                    color_cricle3[i % CRICLE3_IN] = 0xffa0ff;
                }
            }
        }
        if (state == 5 || state == 6) {
            if (KeyTask() == kNone) { /* 提前开窗 */
                state = 0;
                for (int i = 0; i < LED_ALL; i++) { /* 亮白灯 */
                    color_buf[i] = 0xffffff;
                    color_cricle2[i % CRICLE2_IN] = 0xffa0ff;
                    color_cricle3[i % CRICLE3_IN] = 0xffa0ff;
                }
            }
        }
        #endif
        switch (state) {
            case 0: {
                if (KeyTask() == kNone) {
                    CtrlOn(); /* 闭合继电器 */
                    DelayMs(1000); /* 等待1s */
                    /* 下一阶段状态 */
                    state = 1;
                    for (int i = 0; i < LINE1 + CRICLE1; i++) {
                        color_buf[i] = 0x00;
                    }
                    led_visual();
                    index1 = 0;
                    sent_P1 = 0;
                    time_out = 1000; /* 先流水1s */
                    TimeOutRestart();
                }
            } break;
            case 1: {
                led_value(&index1, START, LINE1 + CRICLE1);
                if (TimeOut(time_out) == 1) {
                    if (sent_P1 == 0) {
                        sent_P1 = 1;
                        Uart1Sends("P1", 2);
                        time_out = 8000; /* 段1圈1流水1+8s */
                    }
                    else { /* 时间到 进入下一阶段 */
                        for (int i = 0; i < (LINE1 + CRICLE1); i++) { /* 恢复亮白色 */
                            color_buf[i] = 0xffffff;
                        }
                        for (int i = 0; i < CRICLE3_IN; i++) {
                            color_cricle3[i] = 0xffa0ff;
                        }
                        /* 下一阶段状态 */
                        state = 2;
                        for (int i = 0; i < (LINE2 + CRICLE2); i++) { /* 恢复亮白色 */
                            color_buf[i + LINE2_START] = 0x00;
                        }
                        for (int i = 0; i < CRICLE2_IN; i++) {
                            color_cricle2[i] = 0x00;
                        }
                        index1 = 0;
                        index2 = 0;
                        time_out = 6000;
                    }
                    TimeOutRestart(); /* 重新计时 */
                }

                if (TimePass(5000) == 1) { /* 经过1+5s */
                    CtrlOff(); /* 打开继电器 */
                }

                led_visual();
                DelayMs(SPEED);
            } break;
            case 2: {
                led_value(&index1, LINE2_START, LINE2 + CRICLE2); /* 段2流水 */
                led_cricle2_in(index2++);

                if (TimeOut(time_out) == 1) {
                    for (int i = 0; i < (LINE2 + CRICLE2); i++) { /* 恢复亮白色 */
                        color_buf[i + LINE2_START] = 0xffffff;
                    }
                    for (int i = 0; i < CRICLE2_IN; i++) {
                        color_cricle2[i] = 0xffa0ff;
                    }
                    /* 下一阶段状态 */
                    for (int i = 0; i < (LINE3 + CRICLE3); i++) { /* 恢复亮白色 */
                        color_buf[i + LINE3_START] = 0;
                    }
                    for (int i = 0; i < CRICLE3_IN; i++) {
                        color_cricle3[i] = 0;
                    }
                    state = 3;
                    index1 = 0;
                    index2 = 0;
                    time_out = 6000;
                    TimeOutRestart();
                }

                led_visual();
                DelayMs(SPEED);
            } break;
            case 3: {
                led_value(&index1, LINE3_START, LINE3 + CRICLE3); /* 段3流水 */
                led_cricle3_in((index2++) / 3);
                if (TimeOut(time_out) == 1) {
                    /* 下一阶段状态 */
                    state = 4;
                    for (int i = 0; i < LED_ALL; i++) { /* 亮白灯 */
                        color_buf[i] = 0xffffff;
                        color_cricle2[i % CRICLE2_IN] = 0xffa0ff;
                        color_cricle3[i % CRICLE3_IN] = 0xffa0ff;
                    }
                }
                led_visual();
                DelayMs(SPEED);
            } break;
            case 4: { /* 等待按钮 */
                if (KeyTask() == kPressed) {
                    CtrlOff(); /* 继电器断开 */
                    Uart1Sends("P2", 2); /* 串口发送P2 */
                    /* 下一阶段状态 */
                    state = 5;
                    index1 = 0;
                    sent_P1 = 0;
                    time_out = 5000;
                    TimeOutRestart();
                }
            } break;
            case 5: {
                led_value(&index1, START, LINE1 + CRICLE1); /* 段1流水 */
                
                if (TimeOut(time_out) == 1) { /* 时间到 进入下一阶段 */
                    for (int i = 0; i < (LINE1 + CRICLE1); i++) { /* 恢复亮白色 */
                        color_buf[i] = 0xffffff;
                    }
                    for (int i = 0; i < CRICLE3_IN; i++) {
                        color_cricle3[i] = 0xffa0ff;
                    }
                    /* 下一阶段状态 */
                    state = 6;
                    for (int i = 0; i < (LINE2 + CRICLE2); i++) { /* 恢复亮白色 */
                        color_buf[i + LINE2_START] = 0x00;
                    }
                    for (int i = 0; i < CRICLE2_IN; i++) {
                        color_cricle2[i] = 0x00;
                    }
                    index1 = 0;
                    index2 = 0;
                    time_out = 6000;
                    TimeOutRestart();
                }
                led_visual();
                DelayMs(SPEED);
            } break;
            case 6: {
                led_value(&index1, LINE2_START, LINE2 + CRICLE2); /* 段2流水 */
                led_cricle2_in(index2++);

                if (TimeOut(time_out) == 1) {
                    for (int i = 0; i < (LINE2 + CRICLE2); i++) { /* 恢复亮白色 */
                        color_buf[i + LINE2_START] = 0xffffff;
                    }
                    for (int i = 0; i < CRICLE2_IN; i++) {
                        color_cricle2[i] = 0xffa0ff;
                    }
                    /* 下一阶段状态 */
                    state = 7;
                }

                led_visual();
                DelayMs(SPEED);
            } break;
            case 7: {
                for (int i = 0; i < LED_ALL; i++) { /* 亮白灯 */
                    color_buf[i] = 0xffffff;
                    color_cricle2[i % CRICLE2_IN] = 0xffa0ff;
                    color_cricle3[i % CRICLE3_IN] = 0xffa0ff;
                }
                led_visual();
                Uart1Sends("P3", 2); /* 发送P3 */
                state = 0; /* 回到初始阶段 */
            } break;
            default:
                break;
        }
    }
}
