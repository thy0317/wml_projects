#ifndef __WS2812_H
#define __WS2812_H

// #include "stm32f10x.h"
#include <stdint.h>

#include "stm32f10x_gpio.h"

/* Data Output pin define */
/* 流水灯引脚 */
#define DO_CLK                  RCC_APB2Periph_GPIOD
#define DO_PORT                 GPIOD
#define DO_PIN                  GPIO_Pin_8
/* 圈2内部灯引脚 */
#define CRICLE2_DO_CLK          RCC_APB2Periph_GPIOD
#define CRICLE2_DO_PORT         GPIOD
#define CRICLE2_DO_PIN          GPIO_Pin_13
/* 圈3内部灯引脚 */
#define CRICLE3_DO_CLK          RCC_APB2Periph_GPIOD
#define CRICLE3_DO_PORT         GPIOD
#define CRICLE3_DO_PIN          GPIO_Pin_12

void WS2812_DOInit(void);
void WS2812_RST(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void WS2812_Write(uint32_t rgb888, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void WS2812_WriteRGB(uint8_t red, uint8_t green, uint8_t blue, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void WS2812_Red(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void WS2812_Green(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void WS2812_Blue(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void WS2812_Black(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif /* __WS2812_h */
