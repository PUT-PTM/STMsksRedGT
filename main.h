#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"
#include <stdio.h>

#define BT_BAUD 28800
#define MAX_STRLEN 20


void Delay(__IO uint32_t nTime);
void init_USART1(uint32_t baudrate);
void initPA15();
void setPA15On();
void togglePA15();

#endif /* __MAIN_H */
