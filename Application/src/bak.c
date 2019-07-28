
int main_ok(void)
{
    static unsigned int state = 0, pressed;

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
        color_buf[i] = 0xffffff; // 0xffffff
        color_cricle2[i % CRICLE2_IN] = 0xffa0ff; // 0xffa0ff
        color_cricle3[i % CRICLE3_IN] = 0xffa0ff;
    }
    led_visual();

    while (1) {
        switch (state) {
            case 0: {
                if (KeyTask() == kPressed) {
                    CtrlOn(); /* 闭合继电器 */
                    DelayMs(1000); /* 等待1s */
                    for (int i = 0; i < LINE1 + CRICLE1; i++) {
                        color_buf[i] = 0x00;
                    }
                    led_visual();
                    state = 1;
                    TimeOutRestart();
                }
            } break;
            case 1: {
                static uint32_t index1 = 0;
                static uint32_t sent_P1 = 0, time_out = 1000; /* 段1圈1流水1s */
                led_value(&index1, START, LINE1 + CRICLE1);
                if (TimeOut(time_out) == 1) {
                    if (sent_P1 == 0) {
                        sent_P1 = 1;
                        Uart1Sends("P1", 2);
                        time_out = 3000; /* 段1圈1流水9s */
                    }
                    else { /* 时间到 进入下一阶段 */
                        for (int i = 0; i < (LINE1 + CRICLE1); i++) { /* 恢复暗红色 */
                            color_buf[i] = 0xffffff;
                        }
                        for (int i = 0; i < CRICLE2_IN; i++) {
                            color_cricle2[i] = 0xffa0ff;
                        }
                        for (int i = 0; i < CRICLE3_IN; i++) {
                            color_cricle3[i] = 0xffa0ff;
                        }
                        state = 2;
                        index1 = 0;
                        sent_P1 = 0;
                        time_out = 1000;
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
                static uint32_t index1 = 0, index2;
                static uint32_t time_out = 6000; /* 若要段2流水至圈2 圈2刚好开始流水 需调整此时间 单位ms */
                led_value(&index1, LINE2_START, LINE2 + CRICLE2); /* 段2流水 */
                led_cricle2_in(index2++);

                if (TimeOut(time_out) == 1) {
                    for (int i = 0; i < (LINE2 + CRICLE2); i++) { /* 恢复暗红色 */
                        color_buf[i + LINE2_START] = 0xffffff;
                    }
                    for (int i = 0; i < CRICLE2_IN; i++) {
                        color_cricle2[i] = 0xffa0ff;
                    }
                    for (int i = 0; i < CRICLE3_IN; i++) {
                        color_cricle3[i] = 0xffa0ff;
                    }
                    state = 3;
                    index1 = 0;
                    time_out = 500; /* 若要段2流水至圈2 圈2刚好开始流水 需调整此时间 单位ms */
                    TimeOutRestart();
                }

                led_visual();
                DelayMs(SPEED);
            } break;
            case 3: {
                static uint32_t index1 = 0, index2 = 0;
                static uint32_t time_out = 6000;
                led_value(&index1, LINE3_START, LINE3 + CRICLE3); /* 段3流水 */
                led_cricle3_in((index2++) / 3);
                if (TimeOut(time_out) == 1) {
                    for (int i = 0; i < (LINE3 + CRICLE3); i++) { /* 恢复暗红色 */
                        color_buf[i + LINE3_START] = 0xffffff;
                    }
                    for (int i = 0; i < CRICLE2_IN; i++) {
                        color_cricle2[i] = 0xffa0ff;
                    }
                    for (int i = 0; i < CRICLE3_IN; i++) {
                        color_cricle3[i] = 0xffa0ff;
                    }
                    state = 4;
                    index1 = 0;
                    index2 = 0;
                    time_out = 500;
                    TimeOutRestart();
                }
                led_visual();
                DelayMs(SPEED);
            } break;
            case 4: { /* 等待按钮 */
                if (KeyTask() == kPressed) {
                    pressed = 0;
                    Uart1Sends("P2", 2); /* 串口发送P2 */
                    state = 5;
                }
            } break;
            case 5: {
                static uint32_t index1 = 0, index2 = 0;
                static uint32_t sent_P1 = 0, time_out = 5000;
                led_value(&index1, START, LINE1 + CRICLE1); /* 段1流水 */
                
                if (TimeOut(time_out) == 1) { /* 时间到 进入下一阶段 */
                    for (int i = 0; i < (LINE1 + CRICLE1); i++) { /* 恢复淡蓝色 */
                        color_buf[i] = 0xffffff;
                    }
                    for (int i = 0; i < CRICLE2_IN; i++) {
                        color_cricle2[i] = 0xffa0ff;
                    }
                    for (int i = 0; i < CRICLE3_IN; i++) {
                        color_cricle3[i] = 0xffa0ff;
                    }
                    state = 6;
                    index1 = 0;
                    index2 = 0;
                    sent_P1 = 0;
                    time_out = 1000;
                    TimeOutRestart();
                }
                led_visual();
                DelayMs(SPEED);
            } break;
            case 6: {
                static uint32_t index1 = 0, index2;
                static uint32_t time_out = 6000; /* 若要段2流水至圈2 圈2刚好开始流水 需调整此时间 单位ms */
                led_value(&index1, LINE2_START, LINE2 + CRICLE2); /* 段2流水 */
                led_cricle2_in(index2++);

                if (TimeOut(time_out) == 1) {
                    for (int i = 0; i < (LINE2 + CRICLE2); i++) { /* 恢复暗红色 */
                        color_buf[i + LINE2_START] = 0xffffff;
                    }
                    for (int i = 0; i < CRICLE2_IN; i++) {
                        color_cricle2[i] = 0xffa0ff;
                    }
                    for (int i = 0; i < CRICLE3_IN; i++) {
                        color_cricle3[i] = 0xffa0ff;
                    }
                    state = 7;
                    index1 = 0;
                    time_out = 500; /* 若要段2流水至圈2 圈2刚好开始流水 需调整此时间 单位ms */
                    TimeOutRestart();
                }

                led_visual();
                DelayMs(SPEED);
            } break;
            case 7: {
                for (int i = 0; i < LED_ALL; i++) { /* 亮白灯 */
                    color_buf[i] = 0xffffff; // 0xffffff
                    color_cricle2[i % CRICLE2_IN] = 0xffa0ff; // 0xffa0ff
                    color_cricle3[i % CRICLE3_IN] = 0xffa0ff;
                }
                led_visual();
                Uart1Sends("P3", 2); /* 发送P3 */
                set_flow_color(kRed);
                state = 0; /* 回到初始阶段 */
            } break;
            default:
                break;
        }
    }
}


int main1(void)
{
    static unsigned int state = 0, pressed;

    SysTickInit(1000); /* 1ms */
    WS2812_DOInit();
    Uart1Init();
    OutInit();
    LedInit();
    KeyInit();

    CtrlOff();
    
    DelayMs(500);
    for (int i = 0; i < LED_ALL; i++) { /* 亮白灯 */
        color_buf[i] = 0xffffff; // 0x0f0c0f
    }
    led_visual();

    // while (1);
    while (1) {
        #if 0 /* TODO：提前按关窗 */
        if (state != 0) {
            if (KeyTask() == kPressed) {
                state = 4;
                pressed = 1;
                TimeOutRestart();
            }
        }
        #endif
        switch (state)
        {
            case 0: { /* 空闲态 等待按钮 */
                if (KeyTask() == kPressed) {
                    for (int i = 0; i < LED_ALL; i++) { /* 亮红灯 */
                        color_buf[i] = 0x000300;
                        color_cricle2[i % CRICLE2_IN] = 0x000300;
                        color_cricle3[i % CRICLE3_IN] = 0x000300;
                    }
                    led_visual();
                    CtrlOn(); /* 闭合继电器 */
                    DelayMs(1000); /* 等待1s */
                    state = 1;
                    TimeOutRestart();
                }
            } break;
            case 1: { /* 段1 圈1 串口P1 */
                static uint32_t index1 = 0, index2 = 0;
                static uint32_t sent_P1 = 0, time_out = 1000; /* 段1先流水1s */
                led_value(&index1, START, LINE1); /* 段1流水 */
                
                if (TimeOut(time_out) == 1) {
                    if (sent_P1 == 0) {
                        sent_P1 = 1;
                        Uart1Sends("P1", 2);
                        time_out = 9000; /* 圈1流水9s */
                    }
                    else { /* 时间到 进入下一阶段 */
                        for (int i = 0; i < (LINE1 + CRICLE1); i++) { /* 恢复暗红色 */
                            color_buf[i] = 0x000300;
                            color_cricle2[i % CRICLE2_IN] = 0x000300;
                            color_cricle3[i % CRICLE3_IN] = 0x000300;
                        }
                        state = 2;
                        index1 = 0;
                        index2 = 0;
                        sent_P1 = 0;
                        time_out = 1000;
                    }
                    TimeOutRestart(); /* 重新计时 */
                }
                else {
                    if (sent_P1 == 1) {
                        led_value(&index2, CRICLE1_START, CRICLE1); /* 圈1流水 */
                    }
                }

                if (TimePass(5000) == 1) { /* 经过1+5s */
                    CtrlOff(); /* 打开继电器 */
                }

                led_visual();
                DelayMs(SPEED);
            } break;
            case 2: { /* 段2 圈2 流水 */
                static uint32_t index1 = 0, index2 = 0;
                static uint32_t cricle = 0, time_out = 500; /* 若要段2流水至圈2 圈2刚好开始流水 需调整此时间 单位ms */
                led_value(&index1, LINE2_START, LINE2 + CRICLE2); /* 段2流水 */
                
                if (TimeOut(time_out) == 1) {
                    if (cricle == 0) {
                        cricle = 1;
                        time_out = 6000; /* 圈2流水6s */
                    }
                    else { /* 时间到 进入下一阶段 */
                        for (int i = 0; i < (LINE2 + CRICLE2); i++) { /* 恢复暗红色 */
                            color_buf[i + LINE2_START] = 0x000300;
                            color_cricle2[i % CRICLE2_IN] = 0x000300;
                            color_cricle3[i % CRICLE3_IN] = 0x000300;
                        }
                        state = 3;
                        index1 = 0;
                        index2 = 0;
                        cricle = 0;
                        time_out = 500; /* 若要段2流水至圈2 圈2刚好开始流水 需调整此时间 单位ms */
                    }
                    TimeOutRestart();
                }
                else {
                    if (cricle == 1) {
                        led_value(&index2, CRICLE2_START, CRICLE2); /* 圈2流水 */
                        led_cricle2_in(index2);
                    }
                }
                led_visual();
                DelayMs(SPEED);
            } break;
            case 3: { /* 段3 圈3 流水 */
                static uint32_t index1 = 0, index2 = 0;
                static uint32_t cricle = 0, time_out = 500;
                led_value(&index1, LINE3_START, LINE3); /* 段3流水 */
                
                if (TimeOut(time_out) == 1) {
                    if (cricle == 0) {
                        cricle = 1;
                        time_out = 6000;
                    }
                    else { /* 时间到 进入下一阶段 */
                        for (int i = 0; i < (LINE3 + CRICLE3); i++) { /* 恢复暗红色 */
                            color_buf[i + LINE3_START] = 0x000300;
                            color_cricle2[i % CRICLE2_IN] = 0x000300;
                            color_cricle3[i % CRICLE3_IN] = 0x000300;
                        }
                        state = 4;
                        index1 = 0;
                        index2 = 0;
                        cricle = 0;
                        time_out = 500;
                    }
                    TimeOutRestart();
                }
                else {
                    if (cricle == 1) {
                        led_value(&index2, CRICLE3_START, CRICLE3); /* 圈3流水 */
                        led_cricle3_in(index2);
                    }
                }
                led_visual();
                DelayMs(SPEED);
            } break;
            case 4: { /* 等待按钮 */
                if (KeyTask() == kPressed || pressed) {
                    pressed = 0;
                    for (int i = 0; i < LED_ALL; i++) { /* 全部淡蓝色 */
                        color_buf[i] = 0x000003;
                        color_cricle2[i % CRICLE2_IN] = 0x000003;
                        color_cricle3[i % CRICLE3_IN] = 0x000003;
                    }
                    set_flow_color(kBlue); /* 设置流水颜色为蓝色 */
                    CtrlOff(); /* 继电器断开 */
                    Uart1Sends("P2", 2); /* 串口发送P2 */
                    state = 5; /* 进入下一阶段 */
                }
            } break;
            case 5: {
                static uint32_t index1 = 0, index2 = 0;
                static uint32_t sent_P1 = 0, time_out = 500;
                led_value(&index1, START, LINE1); /* 段1流水 */
                
                if (TimeOut(time_out) == 1) {
                    if (sent_P1 == 0) {
                        sent_P1 = 1;
                        // Uart1Sends("P1", 2);
                        time_out = 5000;
                    }
                    else { /* 时间到 进入下一阶段 */
                        for (int i = 0; i < (LINE1 + CRICLE1); i++) { /* 恢复淡蓝色 */
                            color_buf[i] = 0x000003;
                            color_cricle2[i % CRICLE2_IN] = 0x000003;
                            color_cricle3[i % CRICLE3_IN] = 0x000003;
                        }
                        state = 6;
                        index1 = 0;
                        index2 = 0;
                        sent_P1 = 0;
                        time_out = 1000;
                    }
                    TimeOutRestart();
                }
                else {
                    if (sent_P1 == 1) {
                        led_value(&index2, CRICLE1_START, CRICLE1); /* 圈1流水 */
                    }
                        
                }
                led_visual();
                DelayMs(SPEED);
            } break;
            case 6: { /* 段2 圈2 流水 */
                static uint32_t index1 = 0, index2 = 0;
                static uint32_t cricle = 0, time_out = 500;
                led_value(&index1, LINE2_START, LINE2); /* 段2流水 */
                
                if (TimeOut(time_out) == 1) {
                    if (cricle == 0) {
                        cricle = 1;
                        time_out = 6000;
                    }
                    else { /* 时间到 进入下一阶段 */
                        for (int i = 0; i < (LINE2 + CRICLE2); i++) { /* 恢复淡蓝色 */
                            color_buf[i + LINE2_START] = 0x000003;
                            color_cricle2[i % CRICLE2_IN] = 0x000003;
                            color_cricle3[i % CRICLE3_IN] = 0x000003;
                        }
                        state = 7;
                        index1 = 0;
                        index2 = 0;
                        cricle = 0;
                        time_out = 1000;
                    }
                    TimeOutRestart();
                }
                else {
                    if (cricle == 1) {
                        led_value(&index2, CRICLE2_START, CRICLE2); /* 圈2流水 */
                        led_cricle2_in(index2);
                    }
                        
                }
                led_visual();
                DelayMs(SPEED);
            } break;
            case 7: {
                for (int i = 0; i < LED_ALL; i++) { /* 亮白灯 */
                    color_buf[i] = 0x0f0c0f;
                }
                led_visual();
                Uart1Sends("P3", 2); /* 发送P3 */
                set_flow_color(kRed);
                state = 0; /* 回到初始阶段 */
            } break;
            default:
                break;
        }
    }
}
