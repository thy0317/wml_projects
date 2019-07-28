#ifndef __KEY_H
#define __KEY_H

#define KEY_CLK         RCC_APB2Periph_GPIOB
#define KEY_PORT        GPIOB
#define KEY_PIN         GPIO_Pin_1

#define IS_HOLD_KEY     1

enum KeyValue {
    kNone = 0,
    kPressed,
    kKeyValueMax
};

void KeyInit(void);
enum KeyValue KeyTask(void);

#endif /* __KEY_H */
