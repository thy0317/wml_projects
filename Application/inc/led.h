#ifndef __LED_H
#define __LED_H

#define LED_CLK     RCC_APB2Periph_GPIOB
#define LED_PORT    GPIOB
#define LED_PIN     GPIO_Pin_13

#define OUT_CLK     RCC_APB2Periph_GPIOC
#define OUT_PORT    GPIOC
#define OUT_PIN     GPIO_Pin_9

/* led */
void LedInit(void);
void LedOn(void);
void LedOff(void);
void LedToggle(void);

/* out put */
void OutInit(void);
void OutOn(void);
void OutOff(void);
void OutToggle(void);

/* simple ctrl */
void CtrlOn(void);
void CtrlOff(void);

#endif /* __LED_H */
