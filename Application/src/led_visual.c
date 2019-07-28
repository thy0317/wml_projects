#include "stm32f10x.h"
#include "ws2812.h"
#include "led_visual.h"

uint32_t color_buf[LED_ALL] = {0};
uint32_t flow_color = 8;

uint32_t color_cricle2[CRICLE2_IN] = {0};
uint32_t color_cricle3[CRICLE3_IN] = {0};


#define BREATH_COUNT    32
uint32_t breath_value[BREATH_COUNT] = {0};

void set_breath_value(void)
{
    uint32_t temp = 512 / BREATH_COUNT;
    
    for (int i = 0; i < (BREATH_COUNT / 2); i++) {
        breath_value[i] = 16 + temp * i - 1;
    }
    for (int i = (BREATH_COUNT / 2);  i < BREATH_COUNT; i++) {
        breath_value[i] = breath_value[BREATH_COUNT - i - 1];
    }
}

// void led_visual1(int count, int offset)
// {
//     uint8_t red = 0;
//     for (int i = 0; i < count; i++) {
//         WS2812_WriteRGB(0x02, 0, 0);
//     }
//     for (int i = 0; i < 6; i++) {
//         red |= 0x03;
//         WS2812_WriteRGB(red, 0, 0);
//         red <<= 1;
//     }
//     WS2812_RST();
// }

void set_flow_color(flow_color_t color)
{
    flow_color = 8 * color;
}

void led_value(uint32_t *count, uint32_t offset, uint32_t total)
{
    uint32_t red = 0;
    int index = 0;
    
    for (; index < *count; index++) {
        color_buf[index + offset] = (uint32_t)0x00;
    }
    for (int i = 0; i < 8; i++, index++) {
        color_buf[(index % total) + offset] = (uint32_t)red;
        red <<= 1;
        red |= 0x010101;
    }
    *count += 1;
    *count %= total;
}

void led_cricle2_in(uint32_t count)
{
    uint32_t temp = breath_value[count % BREATH_COUNT];
    uint32_t value = (temp << 16) | ((temp * 0xa0 / 0xff) << 8) | temp;
    for (int i = 0; i < CRICLE2_IN; i++) {
        color_cricle2[i] = value;
    }
}

void led_cricle3_in(uint32_t count)
{
    static uint32_t value[6] = {0, 14, 42, 84, 140, 255};
    uint32_t temp = 0x03;
    for (int i = 0; i < 6; i++) {
        temp = value[i];
        for (int j = 0; j < CRICLE3_IN_PER; j++) {
            color_cricle3[((count + i) % 6) * CRICLE3_IN_PER + j] = (temp << 16) | ((temp * 0xa0 / 0xff) << 8) | temp;
        }
    }
}

void led_cricle3_in_old(uint32_t count)
{
    uint32_t value = (uint32_t)(0xff) << flow_color;
    if ((count % 2)) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < CRICLE3_IN_PER; j++) {
                color_cricle3[2 * i * CRICLE3_IN_PER + j] = value;
                color_cricle3[2 * i * CRICLE3_IN_PER + CRICLE3_IN_PER + j] = 0;
            }
        }
    }
    else {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < CRICLE3_IN_PER; j++) {
                color_cricle3[2 * i * CRICLE3_IN_PER + j] = 0;
                color_cricle3[2 * i * CRICLE3_IN_PER + CRICLE3_IN_PER + j] = value;
            }
        }
    }
}

void led_visual(void)
{
    SysTick->CTRL &= ~ (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    for (int i = 0; i < LED_ALL; i++) {
        WS2812_Write(color_buf[i], DO_PORT, DO_PIN);
    }
    WS2812_RST(DO_PORT, DO_PIN);

    for (int i = 0; i < CRICLE2_IN; i++) {
        WS2812_Write(color_cricle2[i], CRICLE2_DO_PORT, CRICLE2_DO_PIN);
    }
    WS2812_RST(CRICLE2_DO_PORT, CRICLE2_DO_PIN);

    for (int i = 0; i < CRICLE3_IN; i++) {
        WS2812_Write(color_cricle3[i], CRICLE3_DO_PORT, CRICLE3_DO_PIN);
    }
    WS2812_RST(CRICLE3_DO_PORT, CRICLE3_DO_PIN);

    SysTick->CTRL = (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

