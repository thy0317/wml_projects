#ifndef __UART_H
#define __UART_H

#include <stdint.h>

void Uart1Init(void);
void Uart1Sends(char *data, uint32_t length);


#endif /* __UART_H */
