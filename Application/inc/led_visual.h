#ifndef __LED_VISUAL_H
#define __LED_VISUAL_H

#include <stdint.h>

/* 灯带共300个灯珠 以下定义每段灯珠数 */
#define START           (0)
#define LINE1           (8)
#define CRICLE1         (27)
#define LINE2           (4)
#define CRICLE2         (27)
#define LINE3           (5)
#define CRICLE3         (27)

#define CRICLE2_IN      (95) /* 圈2内部灯珠数 */
#define CRICLE3_IN_PER  (9)
#define CRICLE3_IN      (6 * CRICLE3_IN_PER) /* 圈3内部灯珠数 */

/* 流水速度 ms */
#define SPEED           (70)


#define CRICLE1_START   (LINE1)
#define LINE2_START     (CRICLE1_START + CRICLE1)
#define CRICLE2_START   (LINE2_START + LINE2)
#define LINE3_START     (CRICLE2_START + CRICLE2)
#define CRICLE3_START   (LINE3_START + LINE3)
#define LED_ALL         (START + LINE1 + CRICLE1 + LINE2 + CRICLE2 + LINE3 + CRICLE3)
//#define LED_ALL         (9)

typedef enum {
    kBlue = 0,
    kRed
} flow_color_t;

extern uint32_t color_buf[LED_ALL];
extern uint32_t color_cricle2[CRICLE2_IN];
extern uint32_t color_cricle3[CRICLE3_IN];

void set_breath_value(void);
void set_flow_color(flow_color_t color);
void led_value(uint32_t *count, uint32_t offset, uint32_t total);
void led_cricle2_in(uint32_t count);
void led_cricle3_in(uint32_t count);
void led_visual(void);



#endif /* __LED_VISUAL_H */
