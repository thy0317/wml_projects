#include "uart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

/**
 * @brief   usart1 used for rs232
 * @{
 */
void Uart1Init(void)
{
    GPIO_InitTypeDef uart1_pin_init;
    USART_InitTypeDef uart1_init;
    NVIC_InitTypeDef nvic_init;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    uart1_init.USART_BaudRate = 9600;
    uart1_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uart1_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    uart1_init.USART_Parity = USART_Parity_No;
    uart1_init.USART_StopBits = USART_StopBits_1;
    uart1_init.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &uart1_init);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    /* PA9 - tx  PA10 - rx */
    uart1_pin_init.GPIO_Pin = GPIO_Pin_10;
    uart1_pin_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    uart1_pin_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &uart1_pin_init);

    uart1_pin_init.GPIO_Pin = GPIO_Pin_9;
    uart1_pin_init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &uart1_pin_init);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    
    nvic_init.NVIC_IRQChannel = USART1_IRQn;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&nvic_init);
    USART_Cmd(USART1, ENABLE);
}

void Uart1Send(uint8_t data)
{
    while (!(USART1->SR & USART_FLAG_TC)) {
    }
    USART1->SR &= ~USART_FLAG_TC;
    USART1->DR = data;
}

void Uart1Sends(char *data, uint32_t length)
{
    while (length) {
        Uart1Send(*data);
        ++data;
        --length;
    }
}

volatile uint8_t rx[10] = {0}, rx_index = 0;
volatile uint8_t light = 0;
void USART1_IRQHandler(void)
{
    if (USART1->SR & USART_FLAG_RXNE) {
        rx[rx_index++] = USART1->DR;
        if (rx_index > 9) {
            rx_index = 0;
        }
        // light = USART1->DR;
    }
    
    if (USART1->SR & USART_FLAG_IDLE) {
        USART1->SR &= ~ USART_FLAG_IDLE;
        light = 1;
    }
    
}
