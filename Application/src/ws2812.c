#include "stm32f10x_rcc.h"
#include "ws2812.h"

/**
 * @brief   do pin init and operations
 * @{
 */
/* DO pin operation */
#define DO_HIGH(port, pin)       (port->BSRR = pin)
#define DO_LOW(port, pin)        (port->BRR = pin)

void WS2812_DOInit(void)
{
    GPIO_InitTypeDef do_init;
    RCC_APB2PeriphClockCmd(DO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(CRICLE2_DO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(CRICLE3_DO_CLK, ENABLE);

    do_init.GPIO_Pin = DO_PIN;
    do_init.GPIO_Mode = GPIO_Mode_Out_PP;
    do_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DO_PORT, &do_init);
    DO_PORT->BRR = DO_PIN;

    do_init.GPIO_Pin = CRICLE2_DO_PIN;
    GPIO_Init(CRICLE2_DO_PORT, &do_init);
    CRICLE2_DO_PORT->BRR = DO_PIN;

    do_init.GPIO_Pin = CRICLE3_DO_PIN;
    GPIO_Init(CRICLE3_DO_PORT, &do_init);
    CRICLE3_DO_PORT->BRR = DO_PIN;
}

void WS_delay_us(__IO u32 nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
} 

void WS_delay2us()
{
	unsigned char i;
	for(i=0; i<11; i++);
}

void WS_delay05us()
{
//	unsigned char i;
//	for(i=0; i<1; i++);
}

/**
 * T0H  220ns ~ 380ns
 * T0L  750ns ~ 1us
 * T1H  750ns ~ 1us
 * T1L  220ns ~ 380ns
 * RST  >80us
 */
static void WS2812_T0(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    DO_HIGH(GPIOx, GPIO_Pin);
    /* 6*nop ~  */
//    WS_delay05us();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();//__NOP();
    DO_LOW(GPIOx, GPIO_Pin);
    /* 30*nop ~ */
//    WS_delay2us();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();//__NOP();__NOP();
    __NOP();__NOP();//__NOP();//__NOP();__NOP();//__NOP();
//    __NOP();__NOP();__NOP();__NOP();__NOP();//__NOP();
}

static void WS2812_T1(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    DO_HIGH(GPIOx, GPIO_Pin);
    /* 30*nop ~ */
//    WS_delay2us();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();//__NOP();__NOP();__NOP();
//        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    DO_LOW(GPIOx, GPIO_Pin);
    /* 6*nop ~ */
//    WS_delay05us();
}

void WS2812_RST(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    DO_LOW(GPIOx, GPIO_Pin);
//	WS_delay_us(50); 
        for (int i = 0; i < 410; i++) {
            __NOP();
        }
    DO_HIGH(GPIOx, GPIO_Pin);
    DO_LOW(GPIOx, GPIO_Pin);
}

void WS2812_Write(uint32_t grb888, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    for (int i = 0; i < 24; i++) {
        if (0x800000 == (grb888 & 0x800000)) {
            WS2812_T1(GPIOx, GPIO_Pin);
        }
        else {
            WS2812_T0(GPIOx, GPIO_Pin);
        }

        grb888 <<= 1;
    }
}

void WS2812_WriteRGB(uint8_t green, uint8_t red, uint8_t blue, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint32_t rgb888 = green << 16 | red << 8 | blue;
    WS2812_Write(rgb888, GPIOx, GPIO_Pin);
}

/**
 * @}
 */

/**
 * @brief   some typical colour
 * @{
 */
void WS2812_Red(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    WS2812_WriteRGB(0xff, 0, 0, GPIOx, GPIO_Pin);
}

void WS2812_Green(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    WS2812_WriteRGB(0, 0xff, 0, GPIOx, GPIO_Pin);
}

void WS2812_Blue(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    WS2812_WriteRGB(0, 0, 0xff, GPIOx, GPIO_Pin);
}

void WS2812_Black(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    WS2812_WriteRGB(0, 0, 0x00, GPIOx, GPIO_Pin);
}

const uint32_t color[136] = {
    0xFFB6C1, 0xFFC0CB, 0xDC143C, 0xFFF0F5, 0xDB7093, 0xFF69B4, 0xFF1493, 0xC71585, 
    0xDA70D6, 0xD8BFD8, 0xDDA0DD, 0xEE82EE, 0xFF00FF, 0xFF00FF, 0x8B008B, 0x800080, 
    0xBA55D3, 0x9400D3, 0x9932CC, 0x4B0082, 0x8A2BE2, 0x9370DB, 0x7B68EE, 0x6A5ACD, 
    0x483D8B, 0xE6E6FA, 0xF8F8FF, 0x0000FF, 0x0000CD, 0x191970, 0x00008B, 0x000080, 
    0x4169E1, 0x6495ED, 0xB0C4DE, 0x778899, 0x708090, 0x1E90FF, 0xF0F8FF, 0x4682B4, 
    0x87CEFA, 0x87CEEB, 0x00BFFF, 0xADD8E6, 0xB0E0E6, 0x5F9EA0, 0xF0FFFF, 0xE1FFFF, 
    0xAFEEEE, 0x00FFFF, 0x00FFFF, 0x00CED1, 0x2F4F4F, 0x008B8B, 0x008080, 0x48D1CC, 
    0x20B2AA, 0x40E0D0, 0x7FFFAA, 0x00FA9A, 0x00FF7F, 0xF5FFFA, 0x3CB371, 0x2E8B57, 
    0xF0FFF0, 0x90EE90, 0x98FB98, 0x8FBC8F, 0x32CD32, 0x00FF00, 0x228B22, 0x008000, 
    0x006400, 0x7FFF00, 0x7CFC00, 0xADFF2F, 0x556B2F, 0xF5F5DC, 0xFAFAD2, 0xFFFFF0, 
    0xFFFFE0, 0xFFFF00, 0x808000, 0xBDB76B, 0xFFFACD, 0xEEE8AA, 0xF0E68C, 0xFFD700, 
    0xFFF8DC, 0xDAA520, 0xFFFAF0, 0xFDF5E6, 0xF5DEB3, 0xFFE4B5, 0xFFA500, 0xFFEFD5, 
    0xFFEBCD, 0xFFDEAD, 0xFAEBD7, 0xD2B48C, 0xDEB887, 0xFFE4C4, 0xFF8C00, 0xFAF0E6, 
    0xCD853F, 0xFFDAB9, 0xF4A460, 0xD2691E, 0x8B4513, 0xFFF5EE, 0xA0522D, 0xFFA07A, 
    0xFF7F50, 0xFF4500, 0xE9967A, 0xFF6347, 0xFFE4E1, 0xFA8072, 0xFFFAFA, 0xF08080, 
    0xBC8F8F, 0xCD5C5C, 0xFF0000, 0xA52A2A, 0xB22222, 0x8B0000, 0x800000, 0xFFFFFF, 
    0xF5F5F5, 0xDCDCDC, 0xD3D3D3, 0xC0C0C0, 0xA9A9A9, 0x808080, 0x696969, 0x000000, 
};

/**
 * @}
 */

