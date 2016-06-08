#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "tm_stm32f4_delay.h"


#include "bluetooth.h"
#include "czujnik.h"
#include "silniki.h"


int main(void) {
	SystemInit();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	TM_DELAY_Init(); // funkcje opozniajace

	inicjalizacja();
	czujnik_init();
	silniki_init();
	while (1) {
		ustawSilnik();
	}
}

